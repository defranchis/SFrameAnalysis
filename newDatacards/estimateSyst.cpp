
#include <iostream>
#include <fstream>
#include "TString.h"

int main (){
    
    using namespace std;

    TString name = "00_code_v7";

    ifstream inBkg  ("utility/allList.txt",ios::in);
    ifstream inSyst ("utility/systList.txt", ios::in);
    ifstream inDirs ("utility/listOfDirs.txt", ios::in);

    vector<TString> syst, dirs;

    while (!inSyst.eof())
    {
        TString temp;
        inSyst>>temp;
        
        if (inSyst.eof()) break;

        if ( !temp.Contains("#") ) syst.push_back(temp);

    }


    while (!inDirs.eof())
    {
        TString temp;
        inDirs>>temp;
        
        if (inDirs.eof()) break;
        if ( !temp.Contains("#") ) dirs.push_back(temp);

    }


    while (!inBkg.eof())
    {
        TString bkgType;
        inBkg>>bkgType;

        if (inBkg.eof()) break;

        TString central = "00_code_v7";

        for (unsigned int iSyst = 0; iSyst < syst.size(); ++iSyst)
        {
            TString systType = syst.at(iSyst);
            TString up, down;

            ofstream outSyst ("txtFiles/99_syst/"+bkgType+"_"+systType+"_syst.txt");

            for (unsigned int iDir = 0; iDir< dirs.size(); ++iDir)
            {
                TString dirName = dirs.at(iDir);

                if (dirName.Contains(systType))
                {
                    if (dirName.Contains("_up")) up = dirName;
                    else down = dirName;
                }

            }

            // now up/down are initialized with corresponding dirNames

            TString suffix = "_estimate";
            if ( bkgType.Contains("signal") ) suffix = "";

            ifstream inCentral ("txtFiles/"+central+"/"+bkgType+suffix+".txt");
            ifstream inUp ("txtFiles/"+up+"/"+bkgType+suffix+".txt");
            ifstream inDown ("txtFiles/"+down+"/"+bkgType+suffix+".txt");

            while (!inCentral.eof())
            {
                double min_c, max_c, bkg_c, err_c;
                double min_u, max_u, bkg_u, err_u;
                double min_d, max_d, bkg_d, err_d;

                inCentral>>min_c>>max_c>>bkg_c>>err_c;
                inUp>>min_u>>max_u>>bkg_u>>err_u;
                inDown>>min_d>>max_d>>bkg_d>>err_d;

                if (inCentral.eof()) break;

                outSyst<<min_c<<"\t"<<max_c<<"\t";
                
                double syst_u = fabs( bkg_u-bkg_c );
                double syst_d = fabs( bkg_d-bkg_c );

                double syst = syst_u;
                if (syst_d > syst_u) syst = syst_d;

                if (bkg_c > 0) outSyst<<1+syst/bkg_c<<endl;
                else outSyst<<1<<endl;

            }


        }

        

    }





    return 1;


}
