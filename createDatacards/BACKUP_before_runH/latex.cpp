
#include <iostream>
#include <fstream>
#include "TString.h"
#include <iomanip>

int main (){
    
    using namespace std;

    ifstream inSyst ("utility/systList.txt", ios::in);
    ifstream inBkg ("utility/aList.txt",ios::in);

    vector<TString> syst_vec, bkg_vec;

    bkg_vec.push_back("signal_1pb_M-2000");

    while (!inBkg.eof()){
        TString temp;
        inBkg>>temp;

        if (inBkg.eof()) break;

        bkg_vec.push_back(temp);
    }

    while (!inSyst.eof()){
        TString temp;
        inSyst>>temp;

        if (inSyst.eof()) break;

        syst_vec.push_back(temp);
    }


    TString aFileName = bkg_vec.at(0)+"_"+syst_vec.at(0)+"_syst.txt";
    ifstream testFile ("txtFiles/99_syst/"+aFileName, ios::in);
    
    unsigned int nBins = 0;
    while (!testFile.eof()){
        double min, max, val;
        testFile>>min>>max>>val;

        if (testFile.eof()) break;
        ++nBins;
    }

    ofstream outFile ("txtFiles/syst_summary.tex", ios::out);

    for ( unsigned int iSyst = 0; iSyst < syst_vec.size(); ++iSyst )
    {
        outFile<<syst_vec.at(iSyst);

        for ( unsigned int iBkg = 0; iBkg < bkg_vec.size(); ++iBkg  )
        {
            TString fileName = bkg_vec.at(iBkg)+"_"+syst_vec.at(iSyst)+"_syst.txt";
            ifstream inFile ("txtFiles/99_syst/"+fileName, ios::in);
            
            double syst = 0;

            for (unsigned int iBin=0; !inFile.eof(); ++iBin){
                double min, max, val;
                inFile>>min>>max>>val;

                if (inFile.eof()) break;
                if (iBin == nBins -1) syst = val; 
            }

            syst = (syst-1.)*100.;
            outFile<<fixed;
            if (syst == 0) outFile<<" & -";
            else if (syst > 7) outFile<<setprecision(1)<<" & \\textbf{"<<syst<<"}";
            else outFile<<setprecision(1)<<" & "<<syst;

            if (iBkg == bkg_vec.size()-1) outFile<<" \\\\"<<endl;

        }
            
    }


    return 1;


}
