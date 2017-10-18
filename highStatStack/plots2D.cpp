
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TKey.h"
#include "TDirectoryFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
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
    using namespace std;

    TString name = (TString) argv[1]; //tauIso

    ifstream inList ("fullList.txt",ios::in);
    TString pathToFiles = "~/HH/Framework/Analysis/SFrameAnalysis/newRootFiles/";
    vector<TString> sampleList;

    ofstream outTxt ("txtFiles/corrFactor.txt",ios::out);

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
    vector<TList*>  lists_bkg;
    vector<TList*>  lists_Radion;
    vector<TList*>  lists_Data;

    vector<TH2F*>  histos_QCD;
    vector<TH2F*>  histos_DY;
    vector<TH2F*>  histos_ST;
    vector<TH2F*>  histos_TT;
    vector<TH2F*>  histos_WJets;
    vector<TH2F*>  histos_Diboson;
    vector<TH2F*>  histos_bkg;
    vector<TH2F*>  histos_Radion;
    vector<TH2F*>  histos_Data;

    //vector<THStack*> Stacks;


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

                if( keyClass=="TH2F" && (keyName.Contains("_iv")||keyName.Contains("_pt")) ) { 
                    listOfHists.push_back(keyName);
                    cout<<"\tfound key: "<<keyName<<endl;
                }

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
                lists_bkg.push_back(new TList);

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
        if (sampleName.Contains("RadionTohhTohtatahbb")) xSection = .01 ;

        double expectedEvents = xSection * integratedLuminosity;
        double scaleEventWeight = expectedEvents / nOfEvents;


        bool isData = false;
        if (sampleName.Contains("JetHT")) isData = true;

        if (isData) scaleEventWeight = 1.;

        TString dirName;
            
        for (unsigned int iDir = 0; iDir<listOfDirs.size(); ++iDir)
            if (listOfDirs.at(iDir).Contains("NoWindow")) dirName = listOfDirs.at(iDir);

            
        for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
        {
            TString histName = listOfHists.at(iHisto);
            TH2F * h_temp = (TH2F*) inFile->GetObjectChecked(dirName+"/"+histName,"TH2F");

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
            if (!sampleName.Contains("JetHY") && !sampleName.Contains("Radion")) lists_bkg.at(iHisto)->Add(h_temp);
            
            // cout<<sampleName<<":\t n.of events = "<<h_temp->GetEntries()<<endl;


                    

        } //loop over histograms
    }// loop over samples

    cout<<endl;

    for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
    {
        TString histName = listOfHists.at(iHisto);

        histos_QCD.push_back( (TH2F*) ((TH2F*) lists_QCD.at(iHisto)->First())->Clone(histName) );
        histos_DY.push_back( (TH2F*) ((TH2F*) lists_DY.at(iHisto)->First())->Clone(histName) );
        histos_WJets.push_back( (TH2F*) ((TH2F*) lists_WJets.at(iHisto)->First())->Clone(histName) );
        histos_Diboson.push_back( (TH2F*) ((TH2F*) lists_Diboson.at(iHisto)->First())->Clone(histName) );
        histos_ST.push_back( (TH2F*) ((TH2F*) lists_ST.at(iHisto)->First())->Clone(histName) );
        histos_TT.push_back( (TH2F*) ((TH2F*) lists_TT.at(iHisto)->First())->Clone(histName) );
        histos_Radion.push_back( (TH2F*) ((TH2F*) lists_Radion.at(iHisto)->First())->Clone(histName) );
        histos_Data.push_back( (TH2F*) ((TH2F*) lists_Data.at(iHisto)->First())->Clone(histName) );
        histos_bkg.push_back( (TH2F*) ((TH2F*) lists_bkg.at(iHisto)->First())->Clone(histName) );

        // now reset,merge,make stack plots

        histos_Diboson.at(iHisto)->Reset();
        histos_Diboson.at(iHisto)->Merge(lists_Diboson.at(iHisto));

        histos_TT.at(iHisto)->Reset();
        histos_TT.at(iHisto)->Merge(lists_TT.at(iHisto));

        histos_WJets.at(iHisto)->Reset();
        histos_WJets.at(iHisto)->Merge(lists_WJets.at(iHisto));

        histos_DY.at(iHisto)->Reset();
        histos_DY.at(iHisto)->Merge(lists_DY.at(iHisto));

        histos_ST.at(iHisto)->Reset();
        histos_ST.at(iHisto)->Merge(lists_ST.at(iHisto));

        histos_QCD.at(iHisto)->Reset();
        histos_QCD.at(iHisto)->Merge(lists_QCD.at(iHisto));

        histos_Radion.at(iHisto)->Reset();
        histos_Radion.at(iHisto)->Merge(lists_Radion.at(iHisto));

        histos_Data.at(iHisto)->Reset();
        histos_Data.at(iHisto)->Merge(lists_Data.at(iHisto));

        histos_bkg.at(iHisto)->Reset();
        histos_bkg.at(iHisto)->Merge(lists_bkg.at(iHisto));



    }


    vector<TCanvas*> c_stack;
            
    for (unsigned int iHisto = 0; iHisto < listOfHists.size(); ++iHisto)
    {


        TString histName = listOfHists.at(iHisto);
        histName.ReplaceAll("_iv","");

        TCanvas * c_Data = new TCanvas ("c_"+histName+"_Data","c_"+histName+"_Data");
        TString title = histos_Data.at(iHisto)->GetTitle();
        title+=" - Data";
        histos_Data.at(iHisto)->SetTitle(title);
        histos_Data.at(iHisto)->Draw("colz");
        c_Data->Modified(); c_Data->Update();
        c_stack.push_back(c_Data);

        TCanvas * c_Radion = new TCanvas ("c_"+histName+"_Radion","c_"+histName+"_Radion");
        title = histos_Radion.at(iHisto)->GetTitle();
        title+=" - Radion";
        histos_Radion.at(iHisto)->SetTitle(title);
        histos_Radion.at(iHisto)->Draw("colz");
        c_Radion->Modified(); c_Radion->Update();
        c_stack.push_back(c_Radion);

        TCanvas * c_QCD = new TCanvas ("c_"+histName+"_QCD","c_"+histName+"_QCD");
        title = histos_QCD.at(iHisto)->GetTitle();
        title+=" - QCD";
        histos_QCD.at(iHisto)->SetTitle(title);
        histos_QCD.at(iHisto)->Draw("colz");
        c_QCD->Modified(); c_QCD->Update();
        c_stack.push_back(c_QCD);

        TCanvas * c_bkg = new TCanvas ("c_"+histName+"_bkg","c_"+histName+"_bkg");
        title = histos_bkg.at(iHisto)->GetTitle();
        title+=" - all bkg";
        histos_bkg.at(iHisto)->SetTitle(title);
        histos_bkg.at(iHisto)->Draw("colz");
        c_bkg->Modified(); c_bkg->Update();
        c_stack.push_back(c_bkg);

        

        outTxt<<endl<<" -> "<<histName<<endl;
        outTxt<<setprecision(3);

        outTxt<<" - Data"<<"\t\t";
        outTxt<<"rho = "<<histos_Data.at(iHisto)->GetCorrelationFactor()<<"\t significance = "<<
            histos_Data.at(iHisto)->GetCorrelationFactor()/pow((double)histos_Data.at(iHisto)->GetEntries(),-.5)<<endl;

        outTxt<<" - QCD"<<"\t\t";
        outTxt<<"rho = "<<histos_QCD.at(iHisto)->GetCorrelationFactor()<<"\t significance = "<<
            histos_QCD.at(iHisto)->GetCorrelationFactor()/pow((double)histos_QCD.at(iHisto)->GetEntries(),-.5)<<endl;

        outTxt<<" - bkg"<<"\t\t";
        outTxt<<"rho = "<<histos_bkg.at(iHisto)->GetCorrelationFactor()<<"\t significance = "<<
            histos_bkg.at(iHisto)->GetCorrelationFactor()/pow((double)histos_bkg.at(iHisto)->GetEntries(),-.5)<<endl;

        outTxt<<" - rad"<<"\t\t";
        outTxt<<"rho = "<<histos_Radion.at(iHisto)->GetCorrelationFactor()<<"\t significance = "<<
            histos_Radion.at(iHisto)->GetCorrelationFactor()/pow((double)histos_Radion.at(iHisto)->GetEntries(),-.5)<<endl;



    }
        
    cout<<endl;

 
    TFile * outFile = new TFile ("rootFiles/"+name+"_2D.root","recreate");

    for (unsigned int iCanvas = 0; iCanvas < c_stack.size(); ++iCanvas)
        c_stack.at(iCanvas)->Write();


    outFile->Close();


}//end Macro
