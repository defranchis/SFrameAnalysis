
// fare variazioni al posto che stimare contributo

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

    TString name = (TString) argv[1];

    int do_xS_shift = 0;
    int do_lumi_shift = 0;
    int do_tauReco_shift = 0;


    ifstream inList ("utility/fullList.txt",ios::in);
    TString pathToFiles = "~/HH/8_X_Framework/Analysis/SFrameAnalysis/rootFiles/";
    vector<TString> sampleList;

    TString temp = name;
    if (temp.Contains("_xS_")){

        temp = "00_code_v7";
        if (name.Contains("up")) do_xS_shift = 1;
        else if (name.Contains("down")) do_xS_shift = -1;

    }

    else if (temp.Contains("_lumi_")){

        temp = "00_code_v7";
        if (name.Contains("up")) do_lumi_shift = 1;
        else if (name.Contains("down")) do_lumi_shift = -1;

    }

    else if (temp.Contains("_tauReco_")){

        temp = "00_code_v7";
        if (name.Contains("up")) do_tauReco_shift = 1;
        else if (name.Contains("down")) do_tauReco_shift = -1;

    }


    pathToFiles += "code_v7/"+temp;

    vector<TString> listOfDirs;
    vector<TString> listOfHists;

    TFile * outFile = new TFile ("rootFiles/"+name+"/"+name+".root","recreate");

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
                if (keyName.Contains("HT800")) listOfDirs.push_back(keyName);
                if (keyName.Contains("1_") && keyName.Contains("HT800")) aDir = inFile->GetDirectory(keyName);
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

        vector<vector<double>> error_MC;
        vector<vector<double>> error_data;
        vector<vector<double>> error_TT;
        vector<vector<double>> error_WJets;
        vector<vector<double>> error_DY;
        // vector<vector<double>> error_ST;
        // vector<vector<double>> error_Diboson;

        vector<TList*>  lists_QCD;
        vector<TList*>  lists_DY;
        // vector<TList*>  lists_ST;
        vector<TList*>  lists_TT;
        vector<TList*>  lists_WJets;
        // vector<TList*>  lists_Diboson;
        vector<TList*>  lists_Radion;
        vector<TList*>  lists_Data;
        
        vector<TH1F*>  histos_QCD;
        vector<TH1F*>  histos_DY;
        // vector<TH1F*>  histos_ST;
        vector<TH1F*>  histos_TT;
        vector<TH1F*>  histos_WJets;
        // vector<TH1F*>  histos_Diboson;
        vector<TH1F*>  histos_Radion;
        vector<TH1F*>  histos_Data;
        
        vector<THStack*> Stacks;



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
            {
                lists_QCD.push_back(new TList);
                lists_DY.push_back(new TList);
                // lists_ST.push_back(new TList);
                lists_TT.push_back(new TList);
                lists_WJets.push_back(new TList);
                // lists_Diboson.push_back(new TList);
                lists_Radion.push_back(new TList);
                lists_Data.push_back(new TList);
            }
        
            double nOfEvents = (double) tree->GetEntries();
            double xSection = 0.;                  // in pb
            double err_xSection = 0.;
            double integratedLuminosity = 27.22 * 1E03;   // in pb^-1

            integratedLuminosity += do_lumi_shift * .027 * integratedLuminosity;

            if (sampleName.Contains("QCD_HT500to700")) xSection = 32100. ;
            if (sampleName.Contains("QCD_HT700to1000")) xSection = 6831. ;
            if (sampleName.Contains("QCD_HT1000to1500")) xSection = 1207. ;
            if (sampleName.Contains("QCD_HT1500to2000")) xSection = 119.9 ;
            if (sampleName.Contains("QCD_HT2000toInf")) xSection = 25.24 ;
            if (sampleName.Contains("TTJets_HT-600to800")) xSection = 1.61 ;
            if (sampleName.Contains("TTJets_HT-800to1200")) xSection = 0.663 ;
            if (sampleName.Contains("TTJets_HT-1200to2500")) xSection = 0.12 ;
            if (sampleName.Contains("TTJets_HT-2500toInf")) xSection = 0.00143 ;
            if (sampleName.Contains("TTJets_Tune")) xSection = 831.76 - (1.61 + 0.663 + 0.12 + 0.00143) ;
            if (sampleName.Contains("WJetsToQQ_HT-600ToInf")) xSection = 95.14 ;
            if (sampleName.Contains("WJetsToLNu_HT-400To600")) xSection = 48.91*1.21 ;
            if (sampleName.Contains("WJetsToLNu_HT-600To800")) xSection = 12.05*1.21 ;
            if (sampleName.Contains("WJetsToLNu_HT-800To1200")) xSection = 5.501*1.21 ;
            if (sampleName.Contains("WJetsToLNu_HT-1200To2500")) xSection = 1.329*1.21 ;
            if (sampleName.Contains("WJetsToLNu_HT-2500ToInf")) xSection = 0.03216*1.21 ;
            if (sampleName.Contains("ZJetsToQQ_HT600toInf")) xSection = 5.67 ;
            if (sampleName.Contains("DYJetsToLL_M-50_HT-400to600")) xSection = 5.678*1.23 ;
            if (sampleName.Contains("DYJetsToLL_M-50_HT-600to800")) xSection = 1.367*1.23 ;
            if (sampleName.Contains("DYJetsToLL_M-50_HT-800to1200")) xSection = 0.6304*1.23 ;
            if (sampleName.Contains("DYJetsToLL_M-50_HT-1200to2500")) xSection = 0.1514*1.23 ;
            if (sampleName.Contains("DYJetsToLL_M-50_HT-2500toInf")) xSection = 0.003565*1.23 ;
            if (sampleName.Contains("DYJetsToLL_M-5to50_HT-400to600")) xSection = 3.581 ;
            if (sampleName.Contains("DYJetsToLL_M-5to50_HT-600toInf")) xSection = 1.124 ;

            // if (sampleName.Contains("DYJetsToLL_M-50_HT-600toInf")) xSection = 2.198*1.23 ;
            // if (sampleName.Contains("ST_s-channel_4f_leptonDecays")) xSection = 3.36 ;
            // if (sampleName.Contains("ST_t-channel_antitop_4f_leptonDecays")) xSection = 26.38 ;
            // if (sampleName.Contains("ST_t-channel_top_4f_leptonDecays")) xSection = 44.33 ;
            // if (sampleName.Contains("ST_tW_antitop_5f_inclusiveDecays")) xSection = 35.6 ;
            // if (sampleName.Contains("ST_tW_top_5f_inclusiveDecays")) xSection = 35.6 ;
            // if (sampleName.Contains("WW")) xSection = 118.7 ;
            // if (sampleName.Contains("WZ")) xSection = 47.13 ;
            // if (sampleName.Contains("ZZ")) xSection = 16.523 ;
            if (sampleName.Contains("RadionTohhTohtatahbb")) xSection = .001 ;

            // if (sampleName.Contains("TT_Tune")) err_xSection = 45.63 ;
            // if (sampleName.Contains("WJetsToQQ_HT-600ToInf")) err_xSection = 3.57 ;
            // if (sampleName.Contains("WJetsToLNu_HT-400To600")) err_xSection = 0.072*1.21 ;
            // if (sampleName.Contains("WJetsToLNu_HT-600To800")) err_xSection = 0.0073*1.21 ;
            // if (sampleName.Contains("WJetsToLNu_HT-800To1200")) err_xSection = 0.017*1.21 ;
            // if (sampleName.Contains("WJetsToLNu_HT-1200To2500")) err_xSection = 0.0025*1.21 ;
            // if (sampleName.Contains("WJetsToLNu_HT-2500ToInf")) err_xSection = 0.000104*1.21 ;
            // if (sampleName.Contains("ZJetsToQQ_HT600toInf")) err_xSection = 0. ; //can't find this info
            // if (sampleName.Contains("DYJetsToLL_M-50_HT-400to600")) err_xSection = 0.005*1.23 ;
            // if (sampleName.Contains("DYJetsToLL_M-50_HT-600toInf")) err_xSection = 0.002*1.23 ;
            // if (sampleName.Contains("DYJetsToLL_M-5to50_HT-400to600")) err_xSection = 0.118 ;
            // if (sampleName.Contains("DYJetsToLL_M-5to50_HT-600toInf")) err_xSection = 0.038 ;
            // if (sampleName.Contains("ST_s-channel_4f_leptonDecays")) err_xSection = 0.13 ;
            // if (sampleName.Contains("ST_t-channel_antitop_4f_leptonDecays")) err_xSection = 1.32 ;
            // if (sampleName.Contains("ST_t-channel_top_4f_leptonDecays")) err_xSection = 1.76 ;
            // if (sampleName.Contains("ST_tW_antitop_5f_inclusiveDecays")) err_xSection = 1.92 ;
            // if (sampleName.Contains("ST_tW_top_5f_inclusiveDecays")) err_xSection = 1.92 ;


            xSection += do_xS_shift * err_xSection;

            double expectedEvents = xSection * integratedLuminosity;
            double scaleEventWeight = expectedEvents / nOfEvents;
            scaleEventWeight *= 1 + do_tauReco_shift * .1;

            // double err_expectedEvents_xS = err_xSection * integratedLuminosity;


            bool isData = false;
            if (sampleName.Contains("JetHT")) isData = true;

            bool isSignal = false;
            if (sampleName.Contains("Radion")) isSignal = true;

            if (isData) scaleEventWeight = 1.;




            
            for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
            {
                TString histName = listOfHists.at(iHisto);
                TH1F * h_temp = (TH1F*) inFile->GetObjectChecked(dirName+"/"+histName,"TH1F");

                int eventsInHisto = h_temp->GetEntries();

                if(!isData) h_temp->Scale(scaleEventWeight);

                if (!(dirName.Contains("_SR") && isData))
                {
                    if (iHisto==0) cout<<setw(40)<<sampleName<<"\t"<<setw(10)<<eventsInHisto<<"\t"<<setw(15)<<scaleEventWeight<<
                                       "\t\t"<<eventsInHisto*scaleEventWeight<<endl;               
                }

                if (sampleName.Contains("QCD")) lists_QCD.at(iHisto)->Add(h_temp);
                if (sampleName.Contains("DY") || sampleName.Contains("ZJets")) lists_DY.at(iHisto)->Add(h_temp);
                // if (sampleName.Contains("WW") || sampleName.Contains("WZ") || sampleName.Contains("ZZ")) lists_Diboson.at(iHisto)->Add(h_temp);
                // if (sampleName.Contains("ST")) lists_ST.at(iHisto)->Add(h_temp);
                if (sampleName.Contains("TT")) lists_TT.at(iHisto)->Add(h_temp);
                if (sampleName.Contains("WJets")) lists_WJets.at(iHisto)->Add(h_temp);
                if (sampleName.Contains("Radion")) lists_Radion.at(iHisto)->Add(h_temp);
                if (sampleName.Contains("JetHT")) lists_Data.at(iHisto)->Add(h_temp);


                if (isData)
                {

                    if (error_data.size() < listOfHists.size())
                    {

                        vector<double> vec_temp;
                        for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin)
                            vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));

                        error_data.push_back(vec_temp);

                    }
                
                    else
                        for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin)
                            error_data.at(iHisto).at(iBin) += scaleEventWeight * h_temp->GetBinContent(iBin+1);

                }
             
                else if (!isSignal)
                {

                    if (error_MC.size() < listOfHists.size())
                    {
                
                        vector<double> vec_temp;
                        //vector<double> vec_temp_xS;

                        for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){

                            vec_temp.push_back( scaleEventWeight * h_temp->GetBinContent(iBin+1) );
                            //vec_temp_xS.push_back( err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) );
                        }

                        error_MC.push_back(vec_temp);
                        // error_MC_xS.push_back(vec_temp_xS);

                    }
                
                    else{

                        for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                            error_MC.at(iHisto).at(iBin) +=  scaleEventWeight * h_temp->GetBinContent(iBin+1) ;
                            // error_MC_xS.at(iHisto).at(iBin) += err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) ;
                        }

                    }


                    if (sampleName.Contains("WJets")){

                        if (error_WJets.size() < listOfHists.size())
                        {
                
                            vector<double> vec_temp;
                            //vector<double> vec_temp_xS;

                            for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                                vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));
                                //vec_temp_xS.push_back( err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) );
                            }

                            error_WJets.push_back(vec_temp);
                            // error_WJets_xS.push_back(vec_temp_xS);

                        }
                
                        else{

                            for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                                error_WJets.at(iHisto).at(iBin) +=  scaleEventWeight * h_temp->GetBinContent(iBin+1) ;
                                // error_WJets_xS.at(iHisto).at(iBin) +=  err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) ;
                            }

                        }

                    }


                    if (sampleName.Contains("TT")){

                        if (error_TT.size() < listOfHists.size())
                        {
                
                            vector<double> vec_temp;
                            //vector<double> vec_temp_xS;

                            for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                                vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));
                                //vec_temp_xS.push_back( err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) );
                            }

                            error_TT.push_back(vec_temp);
                            // error_TT_xS.push_back(vec_temp_xS);

                        }
                
                        else{

                            for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                                error_TT.at(iHisto).at(iBin) +=  scaleEventWeight * h_temp->GetBinContent(iBin+1) ;
                                // error_TT_xS.at(iHisto).at(iBin) +=  err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) ;
                            }

                        }

                    }

                    if (sampleName.Contains("DY") || sampleName.Contains("ZJets")){
                            
                        if (error_DY.size() < listOfHists.size())
                        {
                
                            vector<double> vec_temp;
                            //vector<double> vec_temp_xS;

                            for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                                vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));
                                //vec_temp_xS.push_back( err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) );
                            }

                            error_DY.push_back(vec_temp);
                            // error_DY_xS.push_back(vec_temp_xS);                            

                        }
                        
                        else{

                            for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                                error_DY.at(iHisto).at(iBin) +=  scaleEventWeight * h_temp->GetBinContent(iBin+1) ;
                                // error_DY_xS.at(iHisto).at(iBin) +=  err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) ;
                            }
                            
                        }
                        
                    }

                    // if (sampleName.Contains("ST")){
                        
                    //     if (error_ST.size() < listOfHists.size())
                    //     {
                            
                    //         vector<double> vec_temp;
                    //         //vector<double> vec_temp_xS;

                    //         for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                    //             vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));
                    //             //vec_temp_xS.push_back( err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) );
                    //         }
                            
                    //         error_ST.push_back(vec_temp);
                    //         // error_ST_xS.push_back(vec_temp_xS);                                                        
                    //     }
                        
                    //     else{
                            
                    //         for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                    //             error_ST.at(iHisto).at(iBin) +=  scaleEventWeight * h_temp->GetBinContent(iBin+1) ;
                    //             // error_ST_xS.at(iHisto).at(iBin) +=  err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) ;
                    //         }
                            
                    //     }
                        
                        
                    // }

                    // if (sampleName.Contains("WW") || sampleName.Contains("WZ") || sampleName.Contains("ZZ")){

                    //     if (error_Diboson.size() < listOfHists.size())
                    //     {
                
                    //         vector<double> vec_temp;
                    //         //vector<double> vec_temp_xS;

                    //         for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                    //             vec_temp.push_back(scaleEventWeight * h_temp->GetBinContent(iBin+1));
                    //             //vec_temp_xS.push_back( err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) );
                    //         }

                    //         error_Diboson.push_back(vec_temp);
                    //         // error_Diboson_xS.push_back(vec_temp_xS);                                                        
                    //     }
                
                    //     else{

                    //         for (unsigned int iBin=0; iBin<h_temp->GetNbinsX(); ++iBin){
                    //             error_Diboson.at(iHisto).at(iBin) +=  scaleEventWeight * h_temp->GetBinContent(iBin+1) ;
                    //             // error_Diboson_xS.at(iHisto).at(iBin) +=  err_expectedEvents_xS/expectedEvents * h_temp->GetBinContent(iBin+1) ;
                    //         }

                    //     }


                    // }

                }

            } //loop over histograms
        }// loop over samples

        cout<<endl;

        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {
            TString histName = listOfHists.at(iHisto);

            histos_QCD.push_back( (TH1F*) ((TH1F*) lists_QCD.at(iHisto)->First())->Clone(histName) );
            histos_DY.push_back( (TH1F*) ((TH1F*) lists_DY.at(iHisto)->First())->Clone(histName) );
            histos_WJets.push_back( (TH1F*) ((TH1F*) lists_WJets.at(iHisto)->First())->Clone(histName) );
            // histos_Diboson.push_back( (TH1F*) ((TH1F*) lists_Diboson.at(iHisto)->First())->Clone(histName) );
            // histos_ST.push_back( (TH1F*) ((TH1F*) lists_ST.at(iHisto)->First())->Clone(histName) );
            histos_TT.push_back( (TH1F*) ((TH1F*) lists_TT.at(iHisto)->First())->Clone(histName) );
            histos_Radion.push_back( (TH1F*) ((TH1F*) lists_Radion.at(iHisto)->First())->Clone(histName) );
            histos_Data.push_back( (TH1F*) ((TH1F*) lists_Data.at(iHisto)->First())->Clone(histName) );

            // now reset,merge,make stack plots

            THStack * tempStack = new THStack (histName,histName);

            // histos_Diboson.at(iHisto)->Reset();
            // histos_Diboson.at(iHisto)->Merge(lists_Diboson.at(iHisto));
            // histos_Diboson.at(iHisto)->SetFillColor(kOrange+10);
            // histos_Diboson.at(iHisto)->SetLineColor(kOrange+10);
            // tempStack->Add(histos_Diboson.at(iHisto));

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

            // histos_ST.at(iHisto)->Reset();
            // histos_ST.at(iHisto)->Merge(lists_ST.at(iHisto));
            // histos_ST.at(iHisto)->SetFillColor(kGreen);
            // histos_ST.at(iHisto)->SetLineColor(kGreen);
            // tempStack->Add(histos_ST.at(iHisto));

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

        TLegend * leg = new TLegend (.65,.6,.85,.95);
        leg->AddEntry(histos_QCD.at(0),"QCD","f");
        // leg->AddEntry(histos_Diboson.at(0),"SM diboson","f");
        leg->AddEntry(histos_TT.at(0),"t#bar{t}","f");
        leg->AddEntry(histos_WJets.at(0),"W+Jets","f");
        leg->AddEntry(histos_DY.at(0),"Z+Jets","f");
        // leg->AddEntry(histos_ST.at(0),"single top","f");
        leg->AddEntry(histos_Radion.at(0),"sig. M=2TeV #sigma=1fb","f");
        leg->AddEntry(histos_Data.at(0),"data","pl");

        vector<TCanvas*> c_stack;
            
        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {

            TString histName = listOfHists.at(iHisto);
            TH1D * h_ratio = new TH1D("","",histos_Radion.at(iHisto)->GetNbinsX(),
                                      histos_Radion.at(iHisto)->GetXaxis()->GetXmin(),histos_Radion.at(iHisto)->GetXaxis()->GetXmax());

            h_ratio->GetYaxis()->SetTitle("data/MC");
            h_ratio->GetYaxis()->SetTitleOffset(.2);
            h_ratio->GetYaxis()->SetTitleSize(.15);
            h_ratio->GetYaxis()->SetLabelSize(.13);
            h_ratio->GetXaxis()->SetLabelSize(0);
            h_ratio->GetYaxis()->SetTickLength(.01);
            h_ratio->GetXaxis()->SetTickSize(.01);

            h_ratio->SetMarkerStyle(7);
            h_ratio->SetLineColor(kBlack);
        

            h_ratio->SetMaximum(3);
            h_ratio->SetMinimum(-1);
            h_ratio->GetYaxis()->SetNdivisions(5);


            for (unsigned int iBin = 1; iBin <= histos_Radion.at(iHisto)->GetNbinsX(); ++iBin )
            {

                double MC = ((TH1F*)Stacks.at(iHisto)->GetStack()->Last())->GetBinContent(iBin);
                double data = histos_Data.at(iHisto)->GetBinContent(iBin);
                double err_data_2 = error_data.at(iHisto).at(iBin-1);
                double err_MC_2 = error_MC.at(iHisto).at(iBin-1);

                double ratio = data/MC;
                double err_ratio = ratio*pow(err_data_2/(data*data)+err_MC_2/(MC*MC),.5);

                if (data!=0 && MC!=0 && data!=-1)
                {
                    h_ratio->SetBinContent(iBin,ratio);
                    h_ratio->SetBinError(iBin,err_ratio);
                }

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
            latexLabel2.DrawLatex(0.72, 0.96, "27.22 fb^{-1} (13 TeV)");


            TPad * p_stack = new TPad ("","",0.0,0.2,1.0,.95);
            TPad * p_ratio = new TPad ("","",0.0,0.0,1.0,0.2);

            if (dirName.Contains("_SR")) p_stack = new TPad ("","",0.0,0.0,1.0,.95);



            p_stack->Draw();
            if (!dirName.Contains("_SR")) {
                p_ratio->Draw();
                p_ratio->SetGrid();
            }
            p_stack->cd();
            //p_stack->SetLogy();

            p_stack->SetBottomMargin(0.09);
            p_stack->SetTopMargin(0.01);

            Stacks.at(iHisto)->Draw("hist");
            histos_Radion.at(iHisto)->Draw("hist same");
            if (!dirName.Contains("_SR")) histos_Data.at(iHisto)->Draw("same");
            if(histName.Contains("pT") || histName.Contains("ET") || histName=="resonance_mass") leg->Draw("same");

            double xmin = Stacks.at(iHisto)->GetXaxis()->GetXmin();
            double xmax = Stacks.at(iHisto)->GetXaxis()->GetXmax();

            if (histName=="jet_pT"){
                xmin = 500;
                xmax = 2000;
            }


            if (histName=="jet_mass"){
                xmin = 105;
                xmax = 135;
            }


            if (histName=="jet_Hbbtag"){
                xmin = .6;
                xmax = 1;

                if (histos_Radion.at(iHisto)->GetBinCenter( histos_Radion.at(iHisto)->GetMaximumBin() ) < 0.6){
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
                xmin = 60;
                xmax = 160;
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
                xmin = 800;
                xmax = 3000;
            }


            Stacks.at(iHisto)->GetXaxis()->SetRangeUser(xmin,xmax);
            h_ratio->GetXaxis()->SetRangeUser(xmin,xmax);
            TLine * line = new TLine (xmin, 1., xmax, 1.);
            
            double max = Stacks.at(iHisto)->GetMaximum();
            if (histos_Radion.at(iHisto)->GetMaximum() > max) max = histos_Radion.at(iHisto)->GetMaximum();
            if (!dirName.Contains("_SR"))
            {
                double max_data_plus_err = histos_Data.at(iHisto)->GetMaximum() + pow(histos_Data.at(iHisto)->GetMaximum(),.5);
                if (max_data_plus_err > max) max = max_data_plus_err;
            }

            Stacks.at(iHisto)->SetMinimum(0.);
            Stacks.at(iHisto)->SetMaximum(1.1*max);

            Stacks.at(iHisto)->SetTitle(histos_Radion.at(iHisto)->GetTitle());
            Stacks.at(iHisto)->GetXaxis()->SetTitle(histos_Radion.at(iHisto)->GetXaxis()->GetTitle());
            Stacks.at(iHisto)->SetTitle();

            if (histName=="resonance_mass")
                Stacks.at(iHisto)->GetXaxis()->SetTitle("mass(HH) [GeV]");

            Stacks.at(iHisto)->GetXaxis()->SetTitleOffset(.8);
            Stacks.at(iHisto)->GetXaxis()->SetTitleSize(.05);

            if (!dirName.Contains("_SR"))
            {
                p_ratio->cd();
                h_ratio->Draw("E");

                line->SetLineColor(kRed);
                line->SetLineWidth(2);
                line->Draw("same");
                h_ratio->Draw("E same");

            }


            c_temp->Modified(); c_temp->Update();
            c_stack.push_back(c_temp);


            if (histName=="resonance_mass_bin")
            {

                TString temp_dirName = dirName;
                if (temp_dirName.Contains("HT") && temp_dirName.Contains("00")){
                    temp_dirName.ReplaceAll("HT800_","");
                    temp_dirName.ReplaceAll("HT900_","");
                }
                    

                ofstream outTxt ("txtFiles/"+name+"/"+temp_dirName+"_binned.txt", ios::out);

                vector<double> mass_range;
                // mass_range.push_back(800.);
                // mass_range.push_back(1100.);
                // mass_range.push_back(1430.);
                // for ( double m = 800. ; m <= 2000. ; m += 50. )
                //     mass_range.push_back(m);


                // for ( double m = 800. ; m <= 1500. ; m += 10. )
                //     mass_range.push_back(m);

                // for ( double m = mass_range.at(mass_range.size()-1)+50. ; m <= 2000. ; m += 50. )
                //     mass_range.push_back(m);

                // for ( double m = mass_range.at(mass_range.size()-1)+100. ; m <= 2500. ; m += 100. )
                //     mass_range.push_back(m);

                for ( double m = 800. ; m <= 1000. ; m += 50. )
                    mass_range.push_back(m);

                for ( double m = mass_range.at(mass_range.size()-1)+50. ; m <= 1500. ; m += 100. )
                    mass_range.push_back(m);


                for ( double m = mass_range.at(mass_range.size()-1)+100. ; m <= 2500. ; m += 500. )
                    mass_range.push_back(m);



                for (unsigned int iMassBin = 0; iMassBin < mass_range.size()+1; ++iMassBin)
                {

                    int iMin = -1;
                    int iMax = -1;

                    if (iMassBin == mass_range.size())
                        iMin = histos_Data.at(iHisto)->FindBin(mass_range.at(0));

                    else 
                        iMin = histos_Data.at(iHisto)->FindBin(mass_range.at(iMassBin));


                    if (iMassBin >= mass_range.size()-1)
                        iMax = histos_Data.at(iHisto)->GetNbinsX();
                    
                    else iMax = histos_Data.at(iHisto)->FindBin(mass_range.at(iMassBin+1));

                    double massMin = histos_Data.at(iHisto)->GetBinCenter(iMin);
                    double massMax = histos_Data.at(iHisto)->GetBinCenter(iMax-1);

                    massMin -= histos_Data.at(iHisto)->GetBinWidth(1)/2.;
                    massMax += histos_Data.at(iHisto)->GetBinWidth(1)/2.;


                    double dataIntegral = histos_Data.at(iHisto)->Integral(iMin,iMax-1);

                    double TTIntegral = histos_TT.at(iHisto)->Integral(iMin,iMax-1);
                    double WJetsIntegral = histos_WJets.at(iHisto)->Integral(iMin,iMax-1);
                    // double STIntegral = histos_ST.at(iHisto)->Integral(iMin,iMax-1);
                    double DYIntegral = histos_DY.at(iHisto)->Integral(iMin,iMax-1);
                    // double DibosonIntegral = histos_Diboson.at(iHisto)->Integral(iMin,iMax-1);

                    // double nonQCDIntegral = TTIntegral+WJetsIntegral+STIntegral+DYIntegral+DibosonIntegral;
                    double nonQCDIntegral = TTIntegral + DYIntegral + WJetsIntegral ;
                    
                    double estimatedQCD = dataIntegral - ( nonQCDIntegral );


                    double err_dataIntegral_2 = dataIntegral;

                    double err_TTIntegral_2 = 0;
            
                    for (int iBin = iMin-1; iBin<iMax-1; ++iBin){
                        err_TTIntegral_2 += error_TT.at(iHisto).at(iBin);
                    }

                    double err_WJetsIntegral_2 = 0;
            
                    for (int iBin = iMin-1; iBin<iMax-1; ++iBin){
                        err_WJetsIntegral_2 += error_WJets.at(iHisto).at(iBin);
                    }

                    // double err_STIntegral_2 = 0;
            
                    // for (int iBin = iMin-1; iBin<iMax-1; ++iBin){
                    //     err_STIntegral_2 += error_ST.at(iHisto).at(iBin);
                    // }

                    double err_DYIntegral_2 = 0;
            
                    for (int iBin = iMin-1; iBin<iMax-1; ++iBin){
                        err_DYIntegral_2 += error_DY.at(iHisto).at(iBin);
                    }

                    // double err_DibosonIntegral_2 = 0;
                    // // double err_DibosonIntegral_xS = 0;
            
                    // for (int iBin = iMin-1; iBin<iMax-1; ++iBin){
                    //     err_DibosonIntegral_2 += error_Diboson.at(iHisto).at(iBin);
                    // }


                    double err_dataIntegral = pow(err_dataIntegral_2 , .5);

                    double err_TTIntegral = pow(err_TTIntegral_2 , .5);
                    double err_WJetsIntegral = pow(err_WJetsIntegral_2 , .5);
                    // double err_STIntegral = pow(err_STIntegral_2 , .5);
                    double err_DYIntegral = pow(err_DYIntegral_2 , .5);
                    // double err_DibosonIntegral = pow(err_DibosonIntegral_2 , .5);



                    // double err_estimatedQCD_2 = err_dataIntegral_2 + err_TTIntegral_2+err_WJetsIntegral_2+err_STIntegral_2
                    //                                                   +err_DYIntegral_2+err_DibosonIntegral_2 ;

                    double err_estimatedQCD_2 = err_dataIntegral_2 + err_TTIntegral_2 + err_DYIntegral_2 + err_WJetsIntegral_2 ;
                    double err_estimatedQCD = pow( err_estimatedQCD_2 , .5);

                    // if (!dirName.Contains("_SR")){

                        if (!dirName.Contains("_SR")) outTxt<<"QCD\t"<<massMin<<"\t"<<massMax<<"\t"<<estimatedQCD<<"\t"<<err_estimatedQCD<<endl;

                        outTxt<<"TT\t"<<massMin<<"\t"<<massMax<<"\t"<<TTIntegral<<"\t"<<err_TTIntegral<<endl;
                        outTxt<<"WJets\t"<<massMin<<"\t"<<massMax<<"\t"<<WJetsIntegral<<"\t"<<err_WJetsIntegral<<endl;
                        outTxt<<"DY\t"<<massMin<<"\t"<<massMax<<"\t"<<DYIntegral<<"\t"<<err_DYIntegral<<endl;

                        // outTxt<<"Diboson\t"<<massMin<<"\t"<<massMax<<"\t"<<DibosonIntegral<<"\t"<<err_DibosonIntegral<<endl;
                        // outTxt<<"ST\t"<<massMin<<"\t"<<massMax<<"\t"<<STIntegral<<"\t"<<err_STIntegral<<endl;
                    // }

                    // else 
                    //     outTxt<<endl<<"this file is intentionally empty"<<endl;

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

        TDirectory * s_Dir = outFile->mkdir("stack_"+dirName);
        s_Dir->cd();

        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
            Stacks.at(iHisto)->Write();



    }//loop over dirs



    outFile->Close();


}//end Macro
