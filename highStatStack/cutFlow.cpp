
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

    gStyle->SetOptStat(0000);
    gStyle->SetLegendBorderSize(0); 
    gStyle->SetOptTitle(0);
    using namespace std;

    TString name = (TString) argv[1]; //tauIso

    ifstream inList ("fullList.txt",ios::in);
    TString pathToFiles = "~/HH/Framework/Analysis/SFrameAnalysis/newRootFiles/";
    vector<TString> sampleList;


    pathToFiles += name;


    while (!inList.eof())
    {
        TString sampleName;
        inList>>sampleName;
        if (!inList.eof()) sampleList.push_back(sampleName);
    }

    cout<<endl;

    vector<TString> listOfDirs;
    vector<TString> listOfHists;

    vector<TList*>  lists_QCD;
    vector<TList*>  lists_DY;
    vector<TList*>  lists_ST;
    vector<TList*>  lists_TT;
    vector<TList*>  lists_WJets;
    vector<TList*>  lists_Diboson;
    vector<TList*>  lists_Radion;
    vector<TList*>  lists_Data;

    vector<TH1F*>  histos_QCD;
    vector<TH1F*>  histos_DY;
    vector<TH1F*>  histos_ST;
    vector<TH1F*>  histos_TT;
    vector<TH1F*>  histos_WJets;
    vector<TH1F*>  histos_Diboson;
    vector<TH1F*>  histos_Radion;
    vector<TH1F*>  histos_Data;

    vector<THStack*> Stacks;


    for (unsigned int iSample = 0; iSample < sampleList.size(); ++iSample)
    {

        TString sampleName = sampleList.at(iSample);
        TString rootFile = pathToFiles+"/VHTausAnalysis."+sampleName+".root";
        TFile * inFile = new TFile (rootFile);
        TTree * tree = (TTree*) inFile->GetObjectChecked("analysis","TTree");

        if (iSample==0)
        {

            TDirectory * aDir = 0;

            for (auto&& mykey : *inFile->GetListOfKeys())
            {
                auto key = (TKey*) mykey;
                TString keyName = key->GetName();
              
                if(key->IsFolder() && keyName.Contains("tautau_")) 
                {
                    listOfDirs.push_back(keyName);
                    if (keyName.Contains("NoWindow")) aDir = inFile->GetDirectory(keyName);
                }

            }

            
            TList * aList = aDir->GetListOfKeys();

            for (auto&& mykey : *aDir->GetListOfKeys())
            {
                auto key = (TKey*) mykey;
                TString keyName = key->GetName();
                TString keyClass = key->GetClassName();
                if( keyClass=="TH1F" /*&& keyName!="cutflow"*/ ) { listOfHists.push_back(keyName); cout<<"\tfound key: "<<keyName<<endl; }

            }

            cout<<endl;
            cout<<setw(40)<<"sample name\t\t"<<setw(10)<<"events\t\t"<<setw(15)<<"weight\ttotal"<<endl<<endl;

            for (unsigned int iH = 0; iH<listOfHists.size(); ++iH)
            {
                lists_QCD.push_back(new TList);
                lists_DY.push_back(new TList);
                lists_ST.push_back(new TList);
                lists_TT.push_back(new TList);
                lists_WJets.push_back(new TList);
                lists_Diboson.push_back(new TList);
                lists_Radion.push_back(new TList);
                lists_Data.push_back(new TList);
            }

        } // if first sample



        double nOfEvents = (double) tree->GetEntries();
        double xSection = 0.;                  // in pb
        double integratedLuminosity = 2318.;   // in pb^-1

        if (sampleName.Contains("QCD_HT700to1000")) xSection = 6831. ;
        if (sampleName.Contains("QCD_HT1000to1500")) xSection = 1207. ;
        if (sampleName.Contains("QCD_HT1500to2000")) xSection = 119.9 ;
        if (sampleName.Contains("QCD_HT2000toInf")) xSection = 25.24 ;
        if (sampleName.Contains("TT_Mtt-700to1000")) xSection = 76.605 ;
        if (sampleName.Contains("TT_Mtt-1000toInf")) xSection = 20.578 ;
        if (sampleName.Contains("WJetsToQQ_HT-600ToInf")) xSection = 95.14 ;
        if (sampleName.Contains("WJetsToLNu_HT-400To600")) xSection = 48.91*1.21 ;
        if (sampleName.Contains("WJetsToLNu_HT-600To800")) xSection = 12.05*1.21 ;
        if (sampleName.Contains("WJetsToLNu_HT-800To1200")) xSection = 5.501*1.21 ;
        if (sampleName.Contains("WJetsToLNu_HT-1200To2500")) xSection = 1.329*1.21 ;
        if (sampleName.Contains("WJetsToLNu_HT-2500ToInf")) xSection = 0.03216*1.21 ;
        if (sampleName.Contains("ZJetsToQQ_HT600toInf")) xSection = 5.67 ;
        if (sampleName.Contains("DYJetsToLL_M-50_HT-400to600")) xSection = 5.678*1.23 ;
        if (sampleName.Contains("DYJetsToLL_M-50_HT-600toInf")) xSection = 2.198*1.23 ;
        if (sampleName.Contains("DYJetsToLL_M-5to50_HT-400to600")) xSection = 3.581 ;
        if (sampleName.Contains("DYJetsToLL_M-5to50_HT-600toInf")) xSection = 1.124 ;
        if (sampleName.Contains("ST_s-channel_4f_leptonDecays")) xSection = 3.36 ;
        if (sampleName.Contains("ST_t-channel_antitop_4f_leptonDecays")) xSection = 26.38 ;
        if (sampleName.Contains("ST_t-channel_top_4f_leptonDecays")) xSection = 44.33 ;
        if (sampleName.Contains("ST_tW_antitop_5f_inclusiveDecays")) xSection = 35.6 ;
        if (sampleName.Contains("ST_tW_top_5f_inclusiveDecays")) xSection = 35.6 ;
        if (sampleName.Contains("WW")) xSection = 118.7 ;
        if (sampleName.Contains("WZ")) xSection = 47.13 ;
        if (sampleName.Contains("ZZ")) xSection = 16.523 ;
        //if (sampleName.Contains("RadionTohhTohtatahbb")) xSection = .01 ;
        if (sampleName.Contains("RadionTohhTohtatahbb")) xSection = 3.548 * 1E-03 ; // 3.548 fb


        double expectedEvents = xSection * integratedLuminosity;
        double scaleEventWeight = expectedEvents / nOfEvents;


        bool isData = false;
        if (sampleName.Contains("JetHT")) isData = true;

        bool isSignal = false;
        if (sampleName.Contains("Radion")) isSignal = true;


        if (isData) scaleEventWeight = 1.;

        TString dirName;
            
        for (unsigned int iDir = 0; iDir<listOfDirs.size(); ++iDir)
            if (listOfDirs.at(iDir).Contains("NoWindow")) dirName = listOfDirs.at(iDir);

            
        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {
            TString histName = listOfHists.at(iHisto);
            TH1F * h_temp = (TH1F*) inFile->GetObjectChecked(dirName+"/"+histName,"TH1F");

            int eventsInHisto = h_temp->GetEntries();

            if(!isData) h_temp->Scale(scaleEventWeight);

            if (iHisto==0 && !sampleName.Contains("Run")) cout<<setw(40)<<sampleName<<"\t"<<setw(10)<<eventsInHisto<<"\t"<<
                                                              setw(15)<<scaleEventWeight<<"\t\t"<<eventsInHisto*scaleEventWeight<<endl;               

            if (sampleName.Contains("QCD")) lists_QCD.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("DY") || sampleName.Contains("ZJets")) lists_DY.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("WW") || sampleName.Contains("WZ") || sampleName.Contains("ZZ")) lists_Diboson.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("ST")) lists_ST.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("TT")) lists_TT.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("WJets")) lists_WJets.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("Radion")) lists_Radion.at(iHisto)->Add(h_temp);
            if (sampleName.Contains("JetHT")) lists_Data.at(iHisto)->Add(h_temp);

            // cout<<sampleName<<":\t n.of events = "<<h_temp->GetEntries()<<endl;


                    

        } //loop over histograms
    }// loop over samples

    cout<<endl;

    for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
    {
        TString histName = listOfHists.at(iHisto);

        histos_QCD.push_back( (TH1F*) ((TH1F*) lists_QCD.at(iHisto)->First())->Clone(histName) );
        histos_DY.push_back( (TH1F*) ((TH1F*) lists_DY.at(iHisto)->First())->Clone(histName) );
        histos_WJets.push_back( (TH1F*) ((TH1F*) lists_WJets.at(iHisto)->First())->Clone(histName) );
        histos_Diboson.push_back( (TH1F*) ((TH1F*) lists_Diboson.at(iHisto)->First())->Clone(histName) );
        histos_ST.push_back( (TH1F*) ((TH1F*) lists_ST.at(iHisto)->First())->Clone(histName) );
        histos_TT.push_back( (TH1F*) ((TH1F*) lists_TT.at(iHisto)->First())->Clone(histName) );
        histos_Radion.push_back( (TH1F*) ((TH1F*) lists_Radion.at(iHisto)->First())->Clone(histName) );
        histos_Data.push_back( (TH1F*) ((TH1F*) lists_Data.at(iHisto)->First())->Clone(histName) );

        // now reset,merge,make stack plots

        THStack * tempStack = new THStack (histName,histName);

        histos_Diboson.at(iHisto)->Reset();
        histos_Diboson.at(iHisto)->Merge(lists_Diboson.at(iHisto));
        histos_Diboson.at(iHisto)->SetFillColor(kRed);
        histos_Diboson.at(iHisto)->SetLineColor(kRed);
        tempStack->Add(histos_Diboson.at(iHisto));

        histos_TT.at(iHisto)->Reset();
        histos_TT.at(iHisto)->Merge(lists_TT.at(iHisto));
        histos_TT.at(iHisto)->SetFillColor(kCyan+2);
        histos_TT.at(iHisto)->SetLineColor(kCyan+2);
        tempStack->Add(histos_TT.at(iHisto));

        histos_WJets.at(iHisto)->Reset();
        histos_WJets.at(iHisto)->Merge(lists_WJets.at(iHisto));
        histos_WJets.at(iHisto)->SetFillColor(kYellow+1);
        histos_WJets.at(iHisto)->SetLineColor(kYellow+1);
        tempStack->Add(histos_WJets.at(iHisto));

        histos_DY.at(iHisto)->Reset();
        histos_DY.at(iHisto)->Merge(lists_DY.at(iHisto));
        histos_DY.at(iHisto)->SetFillColor(kGreen+2);
        histos_DY.at(iHisto)->SetLineColor(kGreen+2);
        tempStack->Add(histos_DY.at(iHisto));

        histos_ST.at(iHisto)->Reset();
        histos_ST.at(iHisto)->Merge(lists_ST.at(iHisto));
        histos_ST.at(iHisto)->SetFillColor(kGreen);
        histos_ST.at(iHisto)->SetLineColor(kGreen);
        tempStack->Add(histos_ST.at(iHisto));

        histos_QCD.at(iHisto)->Reset();
        histos_QCD.at(iHisto)->Merge(lists_QCD.at(iHisto));
        histos_QCD.at(iHisto)->SetFillColor(kGray);
        histos_QCD.at(iHisto)->SetLineColor(kGray);
        tempStack->Add(histos_QCD.at(iHisto));

        histos_Radion.at(iHisto)->Reset();
        histos_Radion.at(iHisto)->Merge(lists_Radion.at(iHisto));
        histos_Radion.at(iHisto)->SetLineColor(kBlack);

        histos_Data.at(iHisto)->Reset();
        histos_Data.at(iHisto)->Merge(lists_Data.at(iHisto));
        histos_Data.at(iHisto)->SetMarkerColor(kBlack);
        histos_Data.at(iHisto)->SetLineColor(kBlack);
        histos_Data.at(iHisto)->SetMarkerStyle(7);


        Stacks.push_back(tempStack);

    }


    //TLegend * leg = new TLegend (.62,.60,.87,.87);
    TLegend * leg = new TLegend (.6,.6,.85,.95);
    leg->AddEntry(histos_QCD.at(0),"QCD","f");
    leg->AddEntry(histos_Diboson.at(0),"SM diboson","f");
    leg->AddEntry(histos_TT.at(0),"t#bar{t}","f");
    leg->AddEntry(histos_WJets.at(0),"W + Jets","f");
    leg->AddEntry(histos_DY.at(0),"Drell-Yan + Jets","f");
    leg->AddEntry(histos_ST.at(0),"single top","f");
    leg->AddEntry(histos_Radion.at(0),"sig. M=2TeV #sigma=10fb","f");



    vector<TCanvas*> c_stack;
            
    for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
    {

        TString histName = listOfHists.at(iHisto);

        bool isBlindVariable = false;
        if (histName.Contains("mass")) isBlindVariable = true;
        if (histName=="jet_mass" || histName=="tautau_mass")
        {
                    
            int minBin = histos_Data.at(iHisto)->FindBin(105);
            int maxBin = histos_Data.at(iHisto)->FindBin(135);
    
            for (int iBin = minBin; iBin <= maxBin; ++iBin)
                histos_Data.at(iHisto)->SetBinContent(iBin, -1.);

            isBlindVariable = false;

        }



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
        //p_stack->SetLogy();

        p_stack->SetBottomMargin(0.09);
        p_stack->SetTopMargin(0.01);

        Stacks.at(iHisto)->Draw("hist");
        histos_Radion.at(iHisto)->Draw("hist same");
        //if (!isBlindVariable) histos_Data.at(iHisto)->Draw("same");

        if (histName.Contains("_pT") || histName.Contains("_ET")) leg->Draw("same");
        

        if (histName == "resonance_mass"){

            TLegend * leg_2 = (TLegend*) leg->Clone("leg_2");
            leg_2->SetX1(.14);
            leg_2->SetX2(.39);
            leg_2->Draw("same");
        }


        // if (histName=="jet_pT")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(500,1400);

        // if (histName=="jet_mass")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(105,135);

        // if (histName=="jet_Hbbtag"){

        //     if ( histos_Radion.at(iHisto)->GetBinCenter( histos_Radion.at(iHisto)->GetMaximumBin() ) > 0.3 )
        //         Stacks.at(iHisto)->GetXaxis()->SetRangeUser(.3,1);
        //     else
        //         Stacks.at(iHisto)->GetXaxis()->SetRangeUser(-1,.3);
        // }


        // if (histName=="tau1_pT")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(0,1000);

        // if (histName=="tau2_pT")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(0,500);

        // if (histName.Contains("_iso")){

        //     if ( histos_Radion.at(iHisto)->GetBinCenter( histos_Radion.at(iHisto)->GetMaximumBin() ) > 0. )
        //         Stacks.at(iHisto)->GetXaxis()->SetRangeUser(-.5,1);
        //     else
        //         Stacks.at(iHisto)->GetXaxis()->SetRangeUser(-1,-.5);
        // }
        

        // if (histName=="MET_ET")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(50,1200);

        // if (histName=="jetmet_DP")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(TMath::Pi()/2.,TMath::Pi());

        // if (histName=="tautaumet_DP")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(0,TMath::Pi()/2.);

        // if (histName=="tautau_mass")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(60,160);

        // if (histName=="tautau_pT")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(200,1800);

        // if (histName=="tautau_DR")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(.1,1);

        // if (histName=="taujet_DR")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(1.5,5);

        // if (histName=="resonance_mass")
        //     Stacks.at(iHisto)->GetXaxis()->SetRangeUser(800,2400);

        double xmin = Stacks.at(iHisto)->GetXaxis()->GetXmin();
        double xmax = Stacks.at(iHisto)->GetXaxis()->GetXmax();

        if (histName=="jet_pT"){
            xmin = 500;
            xmax = 1400;
        }


        if (histName=="jet_mass"){
            xmin = 105;
            xmax = 135;
        }


        if (histName=="jet_Hbbtag"){
            xmin = .3;
            xmax = 1;

            if (histos_Radion.at(iHisto)->GetBinCenter( histos_Radion.at(iHisto)->GetMaximumBin() ) < 0.3){
                xmin = -1.;
                xmax = .3;
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

            // xmin = -.5;
            // xmax = 1;

            // if ( histos_Radion.at(iHisto)->GetBinCenter( histos_Radion.at(iHisto)->GetMaximumBin() ) < 0. ){
            //     xmin = -1;
            //     xmax = -.5;
            // }
        }
        

        if (histName=="MET_ET"){
            xmin = 50;
            xmax = 1200;
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
            xmin = 60;
            xmax = 160;
        }

        if (histName=="tautau_pT"){
            xmin = 200;
            xmax = 1800;
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
            xmin = 800;
            xmax = 2400;
        }

        Stacks.at(iHisto)->GetXaxis()->SetRangeUser(xmin,xmax);

            
        double max = Stacks.at(iHisto)->GetMaximum();
        if (histos_Radion.at(iHisto)->GetMaximum() > max) max = histos_Radion.at(iHisto)->GetMaximum();
        Stacks.at(iHisto)->SetMinimum(0.);
        Stacks.at(iHisto)->SetMaximum(1.1*max);

        Stacks.at(iHisto)->SetTitle(histos_Radion.at(iHisto)->GetTitle());
        Stacks.at(iHisto)->GetXaxis()->SetTitle(histos_Radion.at(iHisto)->GetXaxis()->GetTitle());
        Stacks.at(iHisto)->SetTitle();
        
        c_temp->Modified(); c_temp->Update();
        c_stack.push_back(c_temp);

        if (histName.Contains("jet_pT")){
            cout<<setprecision(3);
            cout<<"   MC integral = "<<((TH1F*)Stacks.at(iHisto)->GetStack()->Last())->Integral()<<endl;
            cout<<"   signal integral = "<<histos_Radion.at(iHisto)->Integral()<<endl;
            cout<<endl;
       } 
 

       //  if (histName.Contains("resonance_mass")){
       //      cout<<setprecision(4);
       //      int nBins = ((TH1F*)Stacks.at(iHisto)->GetStack()->Last())->GetNbinsX();
       //      for (int iBin = 1; iBin<=nBins; ++iBin)
       //      {
       //          float mass = ((TH1F*)Stacks.at(iHisto)->GetStack()->Last())->GetBinCenter(iBin);
       //          float content = ((TH1F*)Stacks.at(iHisto)->GetStack()->Last())->GetBinContent(iBin);
       //          cout<<"mass = "<<mass<<"\t GeV \t MC = "<<content<<endl;
       //      }
       //      cout<<endl;
       // } 


        if (histName=="resonance_mass")
        {
            int iMin = histos_Radion.at(iHisto)->FindBin(1000);
            int iMax = histos_Radion.at(iHisto)->GetNbinsX();
            cout<<endl;
            cout<<"\t -> MC bkg integral in [1000,inf) GeV =\t"<<((TH1F*)Stacks.at(iHisto)->GetStack()->Last())->Integral(iMin,iMax)<<endl;
            cout<<"\t -> signal integral in [1000,inf) GeV =\t"<<histos_Radion.at(iHisto)->Integral(iMin,iMax)<<endl;
            cout<<endl<<endl;
        }


        // delete p_stack;
        // delete p_ratio;
        // delete h_ratio;


    }            


            

    TFile * outFile = new TFile ("rootFiles/"+name+".root","recreate");

    for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        c_stack.at(iHisto)->Write();

    for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        Stacks.at(iHisto)->Write();



    outFile->Close();


}//end Macro
