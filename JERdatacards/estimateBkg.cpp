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

    using namespace std;

    TString name = (TString) argv[1]; //tauIso

    vector<TString> bkg_vec;
    ifstream inList("utility/aList.txt", ios::in);
    ofstream outTex ("txtFiles/"+name+"/bkg.tex",ios::out);
    
    while (!inList.eof()){
        
        TString temp;
        inList>>temp;

        if (inList.eof()) break;
        bkg_vec.push_back(temp);

    }

    for (unsigned int iBkg=0; iBkg<bkg_vec.size(); ++iBkg){

        TString thisBkgType = bkg_vec.at(iBkg);



        ifstream inFile_1("txtFiles/"+name+"/1_bveto_binned.txt");
        ifstream inFile_2 ("txtFiles/"+name+"/2_tauveto_binned.txt");
        ifstream inFile_3 ("txtFiles/"+name+"/3_allveto_binned.txt");

        vector<double> bkg_1, err_bkg_1;
        vector<double> bkg_2, err_bkg_2;
        vector<double> bkg_3, err_bkg_3;
        vector<double> TF, err_TF;
        vector<double> QCD, err_QCD; // NB not necessarily QCD -> all the code

        vector<int> massMin, massMax;


        while (!inFile_1.eof())
        {

            TString bkgType;
            double mMin, mMax, bkg, err_bkg;
            inFile_1>>bkgType>>mMin>>mMax>>bkg>>err_bkg;

            if (inFile_1.eof()) break;
            if (bkgType!=thisBkgType) continue;
        
            bkg_1.push_back(bkg);
            err_bkg_1.push_back(err_bkg);

            massMin.push_back(mMin);
            massMax.push_back(mMax);

        }


        while (!inFile_2.eof())
        {
            TString bkgType;
            double massMin, massMax, bkg, err_bkg;
            inFile_2>>bkgType>>massMin>>massMax>>bkg>>err_bkg;

            if (inFile_2.eof()) break;
            if (bkgType!=thisBkgType) continue;

            bkg_2.push_back(bkg);
            err_bkg_2.push_back(err_bkg);

        }


        while (!inFile_3.eof())
        {

            TString bkgType;
            double massMin, massMax, bkg, err_bkg;
            inFile_3>>bkgType>>massMin>>massMax>>bkg>>err_bkg;

            if (inFile_3.eof()) break;
            if (bkgType!=thisBkgType) continue;

            bkg_3.push_back(bkg);
            err_bkg_3.push_back(err_bkg);

        }



        for (unsigned int iBin = 0; iBin < massMin.size(); ++iBin)
        {
            TF.push_back(bkg_1.at(iBin)/bkg_3.at(iBin));
            err_TF.push_back(TF.at(iBin) * pow( pow(err_bkg_1.at(iBin)/bkg_1.at(iBin),2.) + pow(err_bkg_3.at(iBin)/bkg_3.at(iBin),2.) ,.5));
        }


        cout<<endl;

        for (unsigned int iBin = 0; iBin < massMin.size(); ++iBin)
        {

            double TF_used, err_TF_used;
        
            if ( thisBkgType=="QCD" || thisBkgType=="Diboson" || thisBkgType=="ST" ) // use flat TF
            {
                TF_used = TF.at(TF.size()-1);
                err_TF_used = err_TF.at(err_TF.size()-1);
            }

            else // use binned TF
            {
                TF_used = TF.at(iBin);
                err_TF_used = err_TF.at(iBin);
            }

            cout<<thisBkgType<<"\t"<<massMin.at(iBin)<<"\t"<<massMax.at(iBin)<<"\tTF = "<<TF_used<<" +/- "<<err_TF_used<<endl;

            QCD.push_back(bkg_2.at(iBin) * TF_used);
            err_QCD.push_back(QCD.at(iBin) * pow (pow(err_bkg_2.at(iBin)/bkg_2.at(iBin),2.)+pow(err_TF_used/TF_used,2.),.5));
        }
        
        ofstream outFile ("txtFiles/"+name+"/"+thisBkgType+"_estimate.txt",ios::out);


        outTex<<thisBkgType;

        for (unsigned int iBin = 0; iBin < massMin.size(); ++iBin)
        {
            if (QCD.at(iBin)==0) err_QCD.at(iBin)=0;

            if(iBin<massMin.size()-1) 
                outFile<<massMin.at(iBin)<<"\t"<<massMax.at(iBin)<<"\t"<<QCD.at(iBin)<<"\t"<<err_QCD.at(iBin)<<endl;
            else{

                double sum = 0.;
                double err_sum = 0.;

                for (unsigned int iSum=0; iSum<massMin.size()-1; ++iSum){
                    sum += QCD.at(iSum);
                    err_sum += pow (err_QCD.at(iSum),2.);
                }

                err_sum = pow (err_sum, .5);
                outFile<<massMin.at(iBin)<<"\t"<<massMax.at(iBin)<<"\t"<<sum<<"\t"<<err_sum<<endl;
            }

            
            if(iBin<massMin.size()-1) 
                outTex<<" & $"<<setprecision(2)<<QCD.at(iBin)<<" \\pm "<<setprecision(1)<<err_QCD.at(iBin)<<" $";
        }

        outTex<<" \\\\"<<endl;


    }

    cout<<endl;

    return 1;

}
