
#include <iostream>
#include <fstream>
#include "TString.h"

// int main (){
int main ( int argc, char * argv[] ){
    
    using namespace std;

    // TString name = "00_code_v7";
    TString name = (TString) argv[1];

    // vector<TString> bkg_vec = {"DY","WJets","TT"};
    vector<TString> massPoints;

    ifstream inMass ("utility/massPoints.txt", ios::in);

    for (TString temp; !inMass.eof(); inMass>>temp)
        if (!inMass.eof()) massPoints.push_back(temp);

    inMass.close();

    for ( TString massPoint : massPoints ){

        if (massPoint == "") continue;

        ifstream inSig ("txtFiles/"+name+"/signal_1pb_M-"+massPoint+".txt",ios::in);

        ifstream inDY ("txtFiles/"+name+"/DY_estimate_"+massPoint+".txt",ios::in);
        ifstream inWJ ("txtFiles/"+name+"/WJets_estimate_"+massPoint+".txt",ios::in);
        ifstream inTT ("txtFiles/"+name+"/TT_estimate_"+massPoint+".txt",ios::in);

        double sig, err_sig;
        
        double DY, err_DY;
        double WJ, err_WJ;
        double TT, err_TT;

        inSig>>sig>>err_sig;

        inDY>>DY>>err_DY;
        inWJ>>WJ>>err_WJ;
        inTT>>TT>>err_TT;

        // cout<<massPoint<<"\t"<<TT<<"\t"<<err_TT<<endl;

        inSig.close();

        inDY.close();
        inWJ.close();
        inTT.close();

        TString datacardName = "datacards/"+name+"/radion_M"+massPoint+".txt";            
        ofstream outDatacard (datacardName,ios::out);
        
        outDatacard<<"# Simple counting experiment, with one signal and a few background processes "<<endl;
        outDatacard<<"imax 1  number of channels"<<endl;
        outDatacard<<"jmax 3  number of backgrounds"<<endl;
        outDatacard<<"kmax "<<4+0<<"  number of nuisance parameters (sources of systematical uncertainties)"<<endl;
        outDatacard<<"------------"<<endl;
        outDatacard<<"# we have just one channel, in which we observe 0 events"<<endl;
        outDatacard<<"bin 1"<<endl;
        outDatacard<<"observation 0"<<endl;
        outDatacard<<"------------"<<endl;
        outDatacard<<"bin\t1\t1\t1\t1"<<endl;
        outDatacard<<"process\tsignal\tDYJets\tWJets\tTT"<<endl;
        outDatacard<<"process\t0\t1\t2\t3"<<endl;

        // now the signal xS it is normalized to 1fb (to make Combined happy)
            
        double LF = 1.; // lumi factor

        outDatacard<<"rate\t"<<sig*LF/100.<<"\t"<<DY*LF<<"\t"<<WJ*LF<<"\t"<<TT*LF<<endl;
            
        outDatacard<<"------------"<<endl;


        outDatacard<<"signal\tlnN\t"<<1+err_sig/sig<<"\t-\t-\t-"<<endl;

        outDatacard<<"DY\tlnN\t-\t"<<1+err_DY/DY<<"\t-\t-"<<endl;
        outDatacard<<"WJ\tlnN\t-\t-\t"<<1+err_WJ/WJ<<"\t-"<<endl;
        outDatacard<<"TT\tlnN\t-\t-\t-\t"<<1+err_TT/TT<<endl;


    } // end loop on masspoints


    return 1;

}
