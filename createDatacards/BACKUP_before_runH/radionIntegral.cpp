
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

    ifstream inList ("utility/radionList.txt",ios::in);
    TString pathToFiles = "~/HH/8_X_Framework/Analysis/SFrameAnalysis/rootFiles/";
    vector<TString> sampleList;

    ofstream outXS_HH ("utility/xS_HH_pb.txt",ios::out);
    ofstream outXS_bbtautau ("utility/xS_bbtautau_pb.txt",ios::out);

    int do_lumi_shift = 0;
    int do_tauReco_shift = 0;

    TString temp = name;
    if (temp.Contains("_xS_")) temp = "00_code_v7";

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


    while (!inList.eof())
    {
        TString sampleName;
        inList>>sampleName;
        if (!inList.eof()) sampleList.push_back(sampleName);
    }

    //cout<<endl;

    vector<TString> listOfDirs;
    vector<TString> listOfHists;


    for (unsigned int iSample = 0; iSample < sampleList.size(); ++iSample)
    {

        TString sampleName = sampleList.at(iSample);
        TString rootFile = pathToFiles+"/VHTausAnalysis."+sampleName+".root";
        TFile * inFile = new TFile (rootFile);
        TTree * tree = (TTree*) inFile->GetObjectChecked("analysis","TTree");

        TString massPoint = sampleName;
        massPoint.ReplaceAll("RadionTohhTohtatahbb_narrow_M-","");
        massPoint.ReplaceAll("_13TeV-madgraph","");

        // massPoint.ReplaceAll("RadionTohhTohtatahbb_M-","");

        ofstream outTxt ("txtFiles/"+name+"/signal_1pb_M-"+massPoint+".txt",ios::out);

        double nOfEvents = (double) tree->GetEntries();
        double xSection = 0.;                  // in fb
        double integratedLuminosity = 27.22;   // in fb^-1

        integratedLuminosity += do_lumi_shift * .027 * integratedLuminosity;

        double BR_Hbb = 0.5809 ;
        double BR_Htautau = 0.06256 ;
        double LambdaR_factor = 9. ;

        if (sampleName.Contains("M-1000")) xSection = 63.1753 ;
        if (sampleName.Contains("M-1200")) xSection = 36.3232 ;
        if (sampleName.Contains("M-1400")) xSection = 21.8032 ;
        if (sampleName.Contains("M-1600")) xSection = 13.4682 ;
        if (sampleName.Contains("M-1800")) xSection = 8.48921 ;
        if (sampleName.Contains("M-2000")) xSection = 5.42388 ;
        if (sampleName.Contains("M-2500")) xSection = 1.81755 ;
        if (sampleName.Contains("M-3000")) xSection = 0.603699;


        if (xSection != 0)
        {
            xSection *= LambdaR_factor * 1.E-03 ;       //in pb
            outXS_HH<<massPoint<<"\t"<<xSection<<endl;
            xSection *= 2 * BR_Hbb * BR_Htautau ;       //in pb
            outXS_bbtautau<<massPoint<<"\t"<<xSection<<endl;
        }

        xSection = 1.E03; // 1pb

        double expectedEvents = xSection * integratedLuminosity;
        double scaleEventWeight = expectedEvents / nOfEvents;
        scaleEventWeight *= 1 + do_tauReco_shift * .1;
            

        // TH1F * h_temp = (TH1F*) inFile->GetObjectChecked("0_SR/resonance_mass_bin","TH1F");
        TH1F * h_temp = (TH1F*) inFile->GetObjectChecked("HT800_0_SR/resonance_mass_bin","TH1F");
        int eventsInHisto = h_temp->GetEntries();
        
        h_temp->Scale(scaleEventWeight);

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
                iMin = h_temp->FindBin(mass_range.at(0));

            else 
                iMin = h_temp->FindBin(mass_range.at(iMassBin));

            if (iMassBin >= mass_range.size()-1)
                iMax = h_temp->GetNbinsX();
                    
            else iMax = h_temp->FindBin(mass_range.at(iMassBin+1));

            double massMin = h_temp->GetBinCenter(iMin);
            double massMax = h_temp->GetBinCenter(iMax-1);
            
            massMin -= h_temp->GetBinWidth(1)/2.;
            massMax += h_temp->GetBinWidth(1)/2.;

            double signalIntegral = h_temp->Integral(iMin,iMax-1);
            double err_signalIntegral = scaleEventWeight * pow(signalIntegral,.5);

            outTxt<<massMin<<"\t"<<massMax<<"\t"<<signalIntegral<<"\t"<<err_signalIntegral<<endl;
        }
              
    }// loop over samples

    //cout<<endl;
    
}//end Macro
