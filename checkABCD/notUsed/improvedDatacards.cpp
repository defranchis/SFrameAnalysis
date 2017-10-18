
#include <iostream>
#include <fstream>
#include "TString.h"

int main (){
    
    using namespace std;

    vector<double> QCD_bkg, err_QCD_bkg;
    vector<double> TT_bkg, err_TT_bkg;
    vector<double> WJets_bkg, err_WJets_bkg;
    vector<double> DY_bkg, err_DY_bkg;
    vector<double> ST_bkg, err_ST_bkg;
    vector<double> Diboson_bkg, err_Diboson_bkg;
    vector<int> massMin, massMax;

    TString dir = "txtFiles/";
    ifstream inQCD (dir+"QCD_estimate.txt",ios::in);
    ifstream inTT (dir+"TT_estimate.txt",ios::in);
    ifstream inWJets (dir+"WJets_estimate.txt",ios::in);
    ifstream inDY (dir+"DY_estimate.txt",ios::in);
    ifstream inST (dir+"ST_estimate.txt",ios::in);
    ifstream inDiboson (dir+"Diboson_estimate.txt",ios::in);


    while (!inQCD.eof()){

        double mMin, mMax, bkg, err_bkg;
        inQCD>>mMin>>mMax>>bkg>>err_bkg;

        if (inQCD.eof()) break;

        QCD_bkg.push_back(bkg);
        err_QCD_bkg.push_back(err_bkg);

        massMin.push_back(mMin);
        massMax.push_back(mMax);

    }

    inQCD.close();



    while (!inTT.eof()){

        double mMin, mMax, bkg, err_bkg;
        inTT>>mMin>>mMax>>bkg>>err_bkg;

        if (inTT.eof()) break;

        TT_bkg.push_back(bkg);
        err_TT_bkg.push_back(err_bkg);

    }

    inTT.close();



    while (!inWJets.eof()){

        double mMin, mMax, bkg, err_bkg;
        inWJets>>mMin>>mMax>>bkg>>err_bkg;

        if (inWJets.eof()) break;

        WJets_bkg.push_back(bkg);
        err_WJets_bkg.push_back(err_bkg);

    }

    inWJets.close();



    while (!inDY.eof()){

        double mMin, mMax, bkg, err_bkg;
        inDY>>mMin>>mMax>>bkg>>err_bkg;

        if (inDY.eof()) break;

        DY_bkg.push_back(bkg);
        err_DY_bkg.push_back(err_bkg);

    }

    inDY.close();



    while (!inST.eof()){

        double mMin, mMax, bkg, err_bkg;
        inST>>mMin>>mMax>>bkg>>err_bkg;

        if (inST.eof()) break;

        ST_bkg.push_back(bkg);
        err_ST_bkg.push_back(err_bkg);

    }

    inST.close();



    while (!inDiboson.eof()){

        double mMin, mMax, bkg, err_bkg;
        inDiboson>>mMin>>mMax>>bkg>>err_bkg;

        if (inDiboson.eof()) break;

        Diboson_bkg.push_back(bkg);
        err_Diboson_bkg.push_back(err_bkg);

    }

    inDiboson.close();



    ifstream inSig ("utility/radionList.txt",ios::in);

    while (!inSig.eof()){

        TString fileName;
        inSig>>fileName;

        if (inSig.eof()) break;

        TString massPoint = fileName;
        massPoint.ReplaceAll("RadionTohhTohtatahbb_M-","");

        ifstream inFile (dir+"signal_1pb_M-"+massPoint+".txt", ios::in);
        
        vector<double> sig_integr, err_sig_integr;

        while (!inFile.eof()){

            double mMin, mMax, integr, err_integr;
            inFile>>mMin>>mMax>>integr>>err_integr;

            if (inFile.eof()) break;

            sig_integr.push_back(integr);
            err_sig_integr.push_back(err_integr);

        }


        for (unsigned int iBin = 0; iBin < massMin.size() -1; ++iBin)
        {
            
            TString n; 
            n.Form("%d",iBin+1);

            TString datacardName = "datacards/radion_M"+massPoint+"_"+n+".txt";            

            ofstream outDatacard (datacardName,ios::out);
        
            outDatacard<<"# Simple counting experiment, with one signal and a few background processes "<<endl;
            outDatacard<<"imax 1  number of channels"<<endl;
            outDatacard<<"jmax 6  number of backgrounds"<<endl;
            outDatacard<<"kmax 0  number of nuisance parameters (sources of systematical uncertainties)"<<endl;
            outDatacard<<"------------"<<endl;
            outDatacard<<"# we have just one channel, in which we observe 0 events"<<endl;
            outDatacard<<"bin 1"<<endl;
            outDatacard<<"observation 0"<<endl;
            outDatacard<<"------------"<<endl;
            outDatacard<<"bin\t1\t1\t1\t1\t1\t1\t1"<<endl;
            outDatacard<<"process\tsignal\tQCD\tTT\tWJets\tDYJets\tST\tVV"<<endl;
            outDatacard<<"process\t0\t1\t2\t3\t4\t5\t6"<<endl;
            outDatacard<<"rate\t"<<sig_integr.at(iBin)<<"\t"<<QCD_bkg.at(iBin)<<"\t"<<TT_bkg.at(iBin)<<"\t"<<WJets_bkg.at(iBin)<<"\t"<<
                DY_bkg.at(iBin)<<"\t"<<ST_bkg.at(iBin)<<"\t"<<Diboson_bkg.at(iBin)<<endl;
            // outDatacard<<"rate\t"<<sig_integr.at(iBin)*4.35<<"\t"<<QCD_bkg.at(iBin)*4.35<<"\t"<<TT_bkg.at(iBin)*4.35<<"\t"<<
            //     WJets_bkg.at(iBin)*4.35<<"\t"<<DY_bkg.at(iBin)*4.35<<"\t"<<ST_bkg.at(iBin)*4.35<<"\t"<<Diboson_bkg.at(iBin)*4.35<<endl;
            // outDatacard<<"rate\t"<<sig_integr.at(iBin)*43.5<<"\t"<<QCD_bkg.at(iBin)*43.5<<"\t"<<TT_bkg.at(iBin)*43.5<<"\t"<<
            //     WJets_bkg.at(iBin)*43.5<<"\t"<<DY_bkg.at(iBin)*43.5<<"\t"<<ST_bkg.at(iBin)*43.5<<"\t"<<Diboson_bkg.at(iBin)*43.5<<endl;
            outDatacard<<"------------"<<endl;




        }// loop on bins

    } //loop on masspoints

    inSig.close();

    return 1;

}
