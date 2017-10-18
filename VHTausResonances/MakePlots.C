#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TString.h"
//
void MakePlots(TString Plot ){
  //TString FileName ="VHTausAnalysis.ZHtautauqq.600";
TString FileName ="VHTausAnalysis.HHtautauqq.1000";


  TFile *_fileA = TFile::Open(FileName+".root");

 
  gStyle->SetOptStat(00000);
  TH1::SetDefaultSumw2();

  vector<TString> channels;
  channels.push_back("mutau");
  channels.push_back("eletau");
  channels.push_back("tautau");
  for( unsigned int ch=0;ch <   channels.size();ch++){

    TH1F *histA1 = (TH1F*)_fileA->Get(channels[ch]+"_NoWindow_NoTau21_SubjetPreTag/"+Plot);
    TCanvas c1;

    histA1->Draw("");
 
    gPad->SetBottomMargin(0.7); 
    c1.SaveAs("TriggerPlots/"+ FileName +"_"+Plot+"_"+channels[ch]+".pdf"); 
  }

  

}
