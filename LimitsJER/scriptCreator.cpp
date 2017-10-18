
#include <iostream>
#include <fstream>
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TStyle.h"


int main(){

    using namespace std;

    ifstream inFile ("utility/massPoints.txt", ios::in);
    ofstream outFile ("Combine.sh", ios::out);

    outFile<<endl<<"#!/bin/bash"<<endl<<endl;

    while (!inFile.eof())
    {

        TString massPoint;
        inFile>>massPoint;
        if (inFile.eof()) break;

        outFile<<"python combineCards.py ";

        for (int iBin = 1; iBin <= 3; ++iBin)
        {
            TString n;
            n.Form("%d",iBin);

            outFile<<"bin"<<n<<"=datacards/radion_M"<<massPoint<<"_"<<n<<".txt ";

        }

        outFile<<"> combinedDatacards/radion_M"<<massPoint<<".txt &"<<endl;

    }


    outFile<<endl<<"wait"<<endl;



    return 1;
}
