
#include <iostream>
#include <fstream>
#include "TString.h"

int main (){
    
    using namespace std;

    TString name = "00_code_v7";

    vector<double> QCD_bkg, err_QCD_bkg;
    vector<double> TT_bkg, err_TT_bkg;
    vector<double> WJets_bkg, err_WJets_bkg;
    vector<double> DY_bkg, err_DY_bkg;
    vector<int> massMin, massMax;

    TString dir = "txtFiles/"+name+"/";
    ifstream inQCD (dir+"QCD_estimate.txt",ios::in);
    ifstream inTT (dir+"TT_estimate.txt",ios::in);
    ifstream inWJets (dir+"WJets_estimate.txt",ios::in);
    ifstream inDY (dir+"DY_estimate.txt",ios::in);


    ifstream systList ("utility/systList.txt", ios::in);
    vector <TString> syst;

    while (!systList.eof())
    {
        TString temp;
        systList>>temp;
        
        if (systList.eof()) break;
        syst.push_back(temp);

    }



    vector<vector<double> > QCD_syst;
    for (unsigned int iSyst=0; iSyst<syst.size(); ++iSyst)
    {
        TString systType = syst.at(iSyst);
        ifstream inSyst ("txtFiles/99_syst/QCD_"+systType+"_syst.txt",ios::in);
        
        vector<double> temp;
        
        while (!inSyst.eof())
        {
            double min, max, val;
            inSyst>>min>>max>>val;
            
            if (inSyst.eof()) break;
            
            temp.push_back(val);

        }

        QCD_syst.push_back(temp);
    }


    vector<vector<double> > TT_syst;
    for (unsigned int iSyst=0; iSyst<syst.size(); ++iSyst)
    {
        TString systType = syst.at(iSyst);
        ifstream inSyst ("txtFiles/99_syst/TT_"+systType+"_syst.txt",ios::in);
        
        vector<double> temp;
        
        while (!inSyst.eof())
        {
            double min, max, val;
            inSyst>>min>>max>>val;
            
            if (inSyst.eof()) break;
            
            temp.push_back(val);

        }

        TT_syst.push_back(temp);
    }

    vector<vector<double> > WJets_syst;
    for (unsigned int iSyst=0; iSyst<syst.size(); ++iSyst)
    {
        TString systType = syst.at(iSyst);
        ifstream inSyst ("txtFiles/99_syst/WJets_"+systType+"_syst.txt",ios::in);
        
        vector<double> temp;
        
        while (!inSyst.eof())
        {
            double min, max, val;
            inSyst>>min>>max>>val;
            
            if (inSyst.eof()) break;
            
            temp.push_back(val);

        }

        WJets_syst.push_back(temp);
    }

    vector<vector<double> > DY_syst;
    for (unsigned int iSyst=0; iSyst<syst.size(); ++iSyst)
    {
        TString systType = syst.at(iSyst);
        ifstream inSyst ("txtFiles/99_syst/DY_"+systType+"_syst.txt",ios::in);
        
        vector<double> temp;
        
        while (!inSyst.eof())
        {
            double min, max, val;
            inSyst>>min>>max>>val;
            
            if (inSyst.eof()) break;
            
            temp.push_back(val);

        }

        DY_syst.push_back(temp);
    }


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






    ifstream inSig ("utility/radionList.txt",ios::in);

    while (!inSig.eof()){

        TString fileName;
        inSig>>fileName;

        if (inSig.eof()) break;

        TString massPoint = fileName;
        massPoint.ReplaceAll("RadionTohhTohtatahbb_narrow_M-","");
        massPoint.ReplaceAll("_13TeV-madgraph","");

        ifstream inFile (dir+"signal_1pb_M-"+massPoint+".txt", ios::in);
        
        vector<double> sig_integr, err_sig_integr;

        while (!inFile.eof()){

            double mMin, mMax, integr, err_integr;
            inFile>>mMin>>mMax>>integr>>err_integr;

            if (inFile.eof()) break;

            sig_integr.push_back(integr);
            err_sig_integr.push_back(err_integr);

        }

        vector<vector<double> > sig_syst;

        for (unsigned int iSyst=0; iSyst<syst.size(); ++iSyst)
        {
            TString systType = syst.at(iSyst);
            ifstream inSyst ("txtFiles/99_syst/signal_1pb_M-"+massPoint+"_"+systType+"_syst.txt",ios::in);
        
            vector<double> temp;
        
            while (!inSyst.eof())
            {
                double min, max, val;
                inSyst>>min>>max>>val;
            
                if (inSyst.eof()) break;
            
                temp.push_back(val);

            }

            sig_syst.push_back(temp);
        }


        for (unsigned int iBin = 0; iBin < massMin.size() -1; ++iBin)
        {
            
            TString n; 
            n.Form("%d",iBin+1);

            TString datacardName = "datacards/radion_M"+massPoint+"_"+n+".txt";            
            ofstream outDatacard (datacardName,ios::out);
        
            outDatacard<<"# Simple counting experiment, with one signal and a few background processes "<<endl;
            outDatacard<<"imax 1  number of channels"<<endl;
            outDatacard<<"jmax 4  number of backgrounds"<<endl;
            outDatacard<<"kmax "<<5+syst.size()<<"  number of nuisance parameters (sources of systematical uncertainties)"<<endl;
            outDatacard<<"------------"<<endl;
            outDatacard<<"# we have just one channel, in which we observe 0 events"<<endl;
            outDatacard<<"bin 1"<<endl;
            outDatacard<<"observation 0"<<endl;
            outDatacard<<"------------"<<endl;
            outDatacard<<"bin\t1\t1\t1\t1\t1"<<endl;
            outDatacard<<"process\tsignal\tQCD\tTT\tDYJets\tWJets"<<endl;
            outDatacard<<"process\t0\t1\t2\t3\t4"<<endl;

            // now the signal xS it is normalized to 1fb (to make the combined tool happy)
            
            double lumi_factor = 1.;
            // lumi_factor *= 30./2.318;

            outDatacard<<"rate\t"<<sig_integr.at(iBin)*lumi_factor/100.<<"\t"<<QCD_bkg.at(iBin)*lumi_factor<<"\t"<<TT_bkg.at(iBin)*lumi_factor<<"\t"
                       <<DY_bkg.at(iBin)*lumi_factor<<"\t"<<WJets_bkg.at(iBin)*lumi_factor<<endl;
            
            outDatacard<<"------------"<<endl;

            //outDatacard<<"lumi\tlnN\t"<<1.027<<"\t-\t"<<1.027<<"\t"<<1.027<<"\t"<<1.027<<endl;

            if (sig_integr.at(iBin) > 0)
                outDatacard<<"sig_"<<iBin+1<<"\tlnN\t"<<1+err_sig_integr.at(iBin)/sig_integr.at(iBin)<<"\t-\t-\t-\t-"<<endl;
            else 
                outDatacard<<"sig_"<<iBin+1<<"\tlnN\t-\t-\t-\t-\t-"<<endl;

            outDatacard<<"QCD_"<<iBin+1<<"\tlnN\t-\t"<<1+err_QCD_bkg.at(iBin)/QCD_bkg.at(iBin)<<"\t-\t-\t-"<<endl;
            outDatacard<<"TT_"<<iBin+1<<"\tlnN\t-\t-\t"<<1+err_TT_bkg.at(iBin)/TT_bkg.at(iBin)<<"\t-\t-"<<endl;
            outDatacard<<"DY_"<<iBin+1<<"\tlnN\t-\t-\t-\t"<<1+err_DY_bkg.at(iBin)/DY_bkg.at(iBin)<<"\t-"<<endl;
            outDatacard<<"WJets_"<<iBin+1<<"\tlnN\t-\t-\t-\t-\t"<<1+err_WJets_bkg.at(iBin)/WJets_bkg.at(iBin)<<endl;

            for (unsigned int iSyst = 0; iSyst < syst.size(); ++iSyst)
            {
                outDatacard<<syst.at(iSyst)<<"\tlnN";
                if (sig_syst.at(iSyst).at(iBin) == 1) outDatacard<<"\t-";
                else outDatacard<<"\t"<<sig_syst.at(iSyst).at(iBin);

                if (QCD_syst.at(iSyst).at(iBin) == 1) outDatacard<<"\t-";
                else outDatacard<<"\t"<<QCD_syst.at(iSyst).at(iBin);

                if (TT_syst.at(iSyst).at(iBin) == 1) outDatacard<<"\t-";
                else outDatacard<<"\t"<<TT_syst.at(iSyst).at(iBin);

                if (DY_syst.at(iSyst).at(iBin) == 1) outDatacard<<"\t-";
                else outDatacard<<"\t"<<DY_syst.at(iSyst).at(iBin);

                if (WJets_syst.at(iSyst).at(iBin) == 1) outDatacard<<"\t-";
                else outDatacard<<"\t"<<WJets_syst.at(iSyst).at(iBin);

                outDatacard<<endl;


            }

            // outDatacard<<"tau_reco\tlnN";
            // for (int i=0; i<5; ++i) outDatacard<<"\t1.10";
            // outDatacard<<endl;



        }// loop on bins

    } //loop on masspoints

    inSig.close();

    return 1;

}
