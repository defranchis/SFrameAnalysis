
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TDirectoryFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH1.h"
#include "TLine.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <iomanip>

int main ( int argc, char * argv[] ){

    gROOT->ForceStyle(); 

    gStyle->SetOptStat(0000);
    gStyle->SetLegendBorderSize(0); 
    gStyle->SetOptTitle(0);

    using namespace std;

    TString name = (TString) argv[1];

    ifstream inList ("utility/ttList.txt",ios::in);
    TString pathToFiles = "~/HH/Framework/Analysis/SFrameAnalysis/newRootFiles/";
    vector<TString> sampleList;

    pathToFiles += name;

    vector<TString> listOfDirs;
    vector<TString> listOfHists;

    TFile * outFile = new TFile ("rootFiles/"+name+".root","recreate");

    while (!inList.eof())
    {
        TString sampleName;
        inList>>sampleName;
        if (!inList.eof()) sampleList.push_back(sampleName);
    }

    cout<<endl;



        

    //protected scope
    {
        unsigned int iSample = 0;
        TString sampleName = sampleList.at(iSample);
        TString rootFile = pathToFiles+"/VHTausAnalysis."+sampleName+".root";
        TFile * inFile = new TFile (rootFile);
        TTree * tree = (TTree*) inFile->GetObjectChecked("analysis","TTree");
        TDirectory * aDir = 0;


        for (auto&& mykey : *inFile->GetListOfKeys())
        {
            auto key = (TKey*) mykey;
            TString keyName = key->GetName();
              
            if(key->IsFolder() && keyName.Contains("_") &&!keyName.Contains("Window")) 
            {
                listOfDirs.push_back(keyName);
                if (keyName.Contains("1_")) aDir = inFile->GetDirectory(keyName);
            }

        }


        TList * aList = aDir->GetListOfKeys();

        for (auto&& mykey : *aDir->GetListOfKeys())
        {
            auto key = (TKey*) mykey;
            TString keyName = key->GetName();
            TString keyClass = key->GetClassName();
            if( keyClass=="TH1F" && keyName!="cutflow" ) { listOfHists.push_back(keyName); cout<<"\tfound key: "<<keyName<<endl; }
        }


    } //end protected scope


    for (unsigned int iDir = 0; iDir<listOfDirs.size(); ++iDir)
    {

        TString dirName = listOfDirs.at(iDir); 

        vector<vector<double>> error_TT;

        vector<TList*>  lists_TT;
        vector<TH1F*>  histos_TT;

        for (unsigned int iSample = 0; iSample < sampleList.size(); ++iSample)
        {

            TString sampleName = sampleList.at(iSample);
            TString rootFile = pathToFiles+"/VHTausAnalysis."+sampleName+".root";
            TFile * inFile = new TFile (rootFile);
            TTree * tree = (TTree*) inFile->GetObjectChecked("analysis","TTree");

            if (iSample == 0)
            {
                cout<<endl;
                cout<<dirName<<endl;
                cout<<endl;
                cout<<setw(40)<<"sample name\t\t"<<setw(10)<<"events\t\t"<<setw(15)<<"weight\ttotal"<<endl<<endl;
            }

            for (unsigned int iH = 0; iH<listOfHists.size(); ++iH)
                lists_TT.push_back(new TList);

        
            double nOfEvents = (double) tree->GetEntries();
            double xSection = 0.;                  // in pb
            double integratedLuminosity = 2318.;   // in pb^-1

            if (sampleName.Contains("TT_Tune")) xSection = 831.76 ;
            // if (sampleName.Contains("TTJets_HT-600to800")) xSection = 1.61 * 1.65623 ;
            // if (sampleName.Contains("TTJets_HT-800to1200")) xSection = 0.663 * 1.65623 ;
            // if (sampleName.Contains("TTJets_HT-1200to2500")) xSection = 0.12 * 1.65623 ;
            // if (sampleName.Contains("TTJets_HT-2500toInf")) xSection = 0.00143 * 1.65623 ;
                                    

            double expectedEvents = xSection * integratedLuminosity;
            double scaleEventWeight = expectedEvents / nOfEvents;

            
            for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
            {

                TString histName = listOfHists.at(iHisto);
                TH1F * h_temp = (TH1F*) inFile->GetObjectChecked(dirName+"/"+histName,"TH1F");

                if (histName.Contains("HT")) h_temp->Rebin(5);
                if (histName=="resonance_mass") h_temp->Rebin();
                
                int eventsInHisto = h_temp->GetEntries();

                h_temp->Scale(scaleEventWeight);

                if (sampleName.Contains("TT")) lists_TT.at(iHisto)->Add(h_temp);

                if (iHisto==0) cout<<setw(40)<<sampleName<<"\t"<<setw(10)<<eventsInHisto<<"\t"<<setw(15)<<scaleEventWeight<<
                                   "\t\t"<<eventsInHisto*scaleEventWeight<<endl;               


                if (sampleName.Contains("TT")){

                    if (error_TT.size() < listOfHists.size())
                    {
                        vector<double> vec_temp;
                        for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin)
                            vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));
                        
                        error_TT.push_back(vec_temp);
                    }
                
                    else{
                        for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin)
                            error_TT.at(iHisto).at(iBin) += scaleEventWeight * h_temp->GetBinContent(iBin+1);
                    }
                }

            } //loop over histograms
        }// loop over samples

        cout<<endl;

        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {
            TString histName = listOfHists.at(iHisto);
            histos_TT.push_back( (TH1F*) ((TH1F*) lists_TT.at(iHisto)->First())->Clone(histName) );

            // now reset,merge,make stack plots

            histos_TT.at(iHisto)->Reset();
            histos_TT.at(iHisto)->Merge(lists_TT.at(iHisto));
            histos_TT.at(iHisto)->SetFillColor(kCyan+2);
            histos_TT.at(iHisto)->SetLineColor(kCyan+2);
        }

        TLegend * leg = new TLegend (.65,.87,.85,.95);
        leg->AddEntry(histos_TT.at(0),"t#bar{t} + jets","f");
        
        vector<TCanvas*> c_stack;
            
        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {

            TString histName = listOfHists.at(iHisto);
            TCanvas * c_temp = new TCanvas ("c_"+histName,"c_"+histName);

            TLatex latexLabel1;
            latexLabel1.SetTextSize(0.04);
            latexLabel1.SetNDC();
            latexLabel1.DrawLatex(0.1, 0.96, "CMS");
            TLatex latexLabel3;
            latexLabel3.SetTextSize(0.035);
            latexLabel3.SetTextFont(52);
            latexLabel3.SetNDC();
            latexLabel3.DrawLatex(0.17, 0.96, "Work in progress");

            TLatex latexLabel2;
            latexLabel2.SetTextSize(0.035);
            latexLabel2.SetTextFont(42);
            latexLabel2.SetNDC();
            latexLabel2.DrawLatex(0.72, 0.96, "2.318 fb^{-1} (13 TeV)");


            TPad * p_stack = new TPad ("","",0.0,0.0,1.0,.95);
            p_stack->Draw();
            p_stack->cd();

            p_stack->SetBottomMargin(0.09);
            p_stack->SetTopMargin(0.01);

            histos_TT.at(iHisto)->Draw("hist");
            leg->Draw("same");

            double xmin = histos_TT.at(iHisto)->GetXaxis()->GetXmin();
            double xmax = histos_TT.at(iHisto)->GetXaxis()->GetXmax();

            if (histName=="jet_pT"){
                xmin = 500;
                xmax = 2000;
            }


            if (histName=="jet_mass"){
                xmin = 50;
                xmax = 250;
            }


            if (histName=="jet_Hbbtag"){
                xmin = .6;
                xmax = 1;

                if (histos_TT.at(iHisto)->GetBinCenter( histos_TT.at(iHisto)->GetMaximumBin() ) < 0.6){
                    xmin = -1.;
                    xmax = .6;
                }

            }


            if (histName=="tau1_pT"){
                xmin = 0;
                xmax = 1000;
            }


            if (histName=="tau2_pT"){
                xmin = 0;
                xmax = 500;
            }


            if (histName.Contains("_iso")){
                xmin = -1;
                xmax = 1;
                if (histName.Contains("sum_")){
                    xmin = -2;
                    xmax = 2;
                }
            
            }
        

            if (histName=="MET_ET"){
                xmin = 50;
                xmax = 1500;
            }

            if (histName=="jetmet_DP"){
                xmin = TMath::Pi()/2.;
                xmax = TMath::Pi();
            }

            if (histName=="tautaumet_DP"){
                xmin = 0;
                xmax = TMath::Pi();
            }

            if (histName=="tautau_mass"){
                xmin = 10;
                xmax = 250;
            }

            if (histName=="tautau_pT"){
                xmin = 200;
                xmax = 2000;
            }

            if (histName=="tautau_DR"){
                xmin = .1;
                xmax = 1.;
            }

            if (histName=="taujet_DR"){
                xmin = 1.5;
                xmax = 5;
            }

            if (histName=="resonance_mass"){
                xmin = 600;
                xmax = 1500;
            }


            histos_TT.at(iHisto)->GetXaxis()->SetRangeUser(xmin,xmax);
            
            double max = histos_TT.at(iHisto)->GetMaximum();

            histos_TT.at(iHisto)->SetMinimum(0.);
            histos_TT.at(iHisto)->SetMaximum(1.1*max);

            histos_TT.at(iHisto)->SetTitle("");
            histos_TT.at(iHisto)->GetXaxis()->SetTitleOffset(.8);
            histos_TT.at(iHisto)->GetXaxis()->SetTitleSize(.05);

            c_temp->Modified(); c_temp->Update();
            c_stack.push_back(c_temp);

            if (histName=="resonance_mass_bin")
            {

                ofstream outTxt ("txtFiles/"+dirName+"_binned.txt", ios::out);

                //use all bins in 500,800

                vector<double> mass_range;

                // for (float m = 600; m <= 1500; m += 150)
                //     mass_range.push_back(m);
                for (float m = 600; m <= 1500; m += 90)
                    mass_range.push_back(m);



                for (unsigned int iMassBin = 0; iMassBin < mass_range.size(); ++iMassBin)
                {

                    int iMin = -1;
                    int iMax = -1;

                    if (iMassBin == mass_range.size()-1)
                        iMin = histos_TT.at(iHisto)->FindBin(mass_range.at(0));

                    else 
                        iMin = histos_TT.at(iHisto)->FindBin(mass_range.at(iMassBin));


                    if (iMassBin == mass_range.size()-1)
                        iMax = histos_TT.at(iHisto)->FindBin(mass_range.at(mass_range.size()-1));
                    
                    else iMax = histos_TT.at(iHisto)->FindBin(mass_range.at(iMassBin+1));

                    double massMin = histos_TT.at(iHisto)->GetBinCenter(iMin);
                    double massMax = histos_TT.at(iHisto)->GetBinCenter(iMax-1);

                    massMin -= histos_TT.at(iHisto)->GetBinWidth(1)/2.;
                    massMax += histos_TT.at(iHisto)->GetBinWidth(1)/2.;

                    double TTIntegral = histos_TT.at(iHisto)->Integral(iMin,iMax-1);
                    double err_TTIntegral_2 = 0;
            
                    for (int iBin = iMin-1; iBin<iMax-1; ++iBin)
                        err_TTIntegral_2 += error_TT.at(iHisto).at(iBin);

                    double err_TTIntegral = pow(err_TTIntegral_2, .5);

                    if (!dirName.Contains("SR"))
                        outTxt<<"TT\t"<<massMin<<"\t"<<massMax<<"\t"<<TTIntegral<<"\t"<<err_TTIntegral<<endl;

                    else
                        outTxt<<massMin<<"\t"<<massMax<<"\t"<<TTIntegral<<"\t"<<err_TTIntegral<<endl;

                }//ciau!! :)

            
            }
        }            
    
        TDirectory * c_Dir = outFile->mkdir(dirName);
        c_Dir->cd();

        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {
            c_stack.at(iHisto)->Write();
            delete c_stack.at(iHisto);
        }  


    }//loop over dirs



  outFile->Close();


}//end Macro
