
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

int main( int argc, char * argv[] ){

    using namespace std;
    gStyle->SetLegendBorderSize(0); 


    ifstream inFile ("utility/massPoints.txt",ios::in);
    TGraph * graph = new TGraph();
    TGraphAsymmErrors * graph_1 = new TGraphAsymmErrors();
    TGraphAsymmErrors * graph_2 = new TGraphAsymmErrors();

    int counter = -1;
    double min, max;

    while (!inFile.eof())
    {
        ++counter;

        int mass;
        TString massPoint;
        double xSection;

        inFile>>mass;
        if (inFile.eof()) break;

        massPoint.Form("%d",mass);

        if (counter==0) {
            min = mass;
            max = mass;
        }

        if (mass<min) min = mass;
        if (mass>max) max = mass;

        TString dirName = "rootFiles";
        TString fileName = "higgsCombine_XHH.Asymptotic.mH"+massPoint+".root";
        TFile * inFile = new TFile (dirName+"/"+fileName);
        TTree * tree = (TTree *) inFile->GetObjectChecked("limit","TTree");

        double limit;
        tree->SetBranchAddress("limit", &limit);

        double down_2, down_1, central, up_1, up_2;

        double BR_Hbb = 0.5809 ;
        double BR_Htautau = 0.06256 ;

        for (int iEntry=0; iEntry<tree->GetEntries()-1; ++iEntry)
        {
            tree->GetEntry(iEntry);

            if (iEntry==0) down_2 = limit / (BR_Hbb*BR_Htautau*2) ;
            if (iEntry==1) down_1 = limit / (BR_Hbb*BR_Htautau*2) ;
            if (iEntry==2) central = limit / (BR_Hbb*BR_Htautau*2) ;
            if (iEntry==3) up_1 = limit / (BR_Hbb*BR_Htautau*2) ;
            if (iEntry==4) up_2 = limit / (BR_Hbb*BR_Htautau*2) ;

        }

        central /= 1E02;
        down_1 /= 1E02;
        down_2 /= 1E02;
        up_1 /= 1E02;
        up_2 /= 1E02;

        graph->SetPoint(counter,mass,central);

        graph_1->SetPoint(counter,mass,central);
        graph_1->SetPointEYhigh(counter,up_1-central);
        graph_1->SetPointEYlow(counter,central-down_1);

        graph_2->SetPoint(counter,mass,central);
        graph_2->SetPointEYhigh(counter,up_2-central);
        graph_2->SetPointEYlow(counter,central-down_2);

        if (mass==max)        
        {

            graph->SetPoint(counter+1,5000,central);

            graph_1->SetPoint(counter+1,5000,central);
            graph_1->SetPointEYhigh(counter+1,up_1-central);
            graph_1->SetPointEYlow(counter+1,central-down_1);

            graph_2->SetPoint(counter+1,5000,central);
            graph_2->SetPointEYhigh(counter+1,up_2-central);
            graph_2->SetPointEYlow(counter+1,central-down_2);

        }
        
    }

    inFile.close();

    graph->SetLineStyle(2);
    graph->SetLineWidth(2);
    graph_1->SetLineStyle(2);
    graph_1->SetLineWidth(2);
    graph_2->SetLineStyle(2);
    graph_2->SetLineWidth(2);

    // graph_1->SetFillColor(kGreen);
    // graph_2->SetFillColor(kYellow);
    // graph_1->SetFillStyle(3001);
    // graph_2->SetFillStyle(3001);

    graph_1->SetFillStyle(1);
    graph_2->SetFillStyle(1);
    graph_1->SetFillColorAlpha(kGreen,0);
    graph_2->SetFillColorAlpha(kYellow,0);


    // graph_1->SetFillStyle(1001);
    // graph_2->SetFillStyle(1001);

    // graph_1->SetFillColorAlpha(kGreen, .35);
    // graph_2->SetFillColorAlpha(kYellow, .35);


    TMultiGraph * multi = new TMultiGraph();
    multi->Add(graph_2);
    multi->Add(graph_1);

    multi->SetTitle(";M_{X} [GeV]; #sigma_{95%} x BR(X->HH) [pb]");


    TCanvas * c = new TCanvas ("limits","limits");

    TLatex latexLabel1;
    latexLabel1.SetTextSize(0.04);
    latexLabel1.SetNDC();
    latexLabel1.DrawLatex(0.1, 0.96, "CMS");
    TLatex latexLabel3;
    latexLabel3.SetTextSize(0.035);
    latexLabel3.SetTextFont(52);
    latexLabel3.SetNDC();
    latexLabel3.DrawLatex(0.17, 0.96, "Work in progress");

    TLatex latexLabel2;
    latexLabel2.SetTextSize(0.035);
    latexLabel2.SetTextFont(42);
    latexLabel2.SetNDC();
    latexLabel2.DrawLatex(0.72, 0.96, "2.318 fb^{-1} (13 TeV)");


    TPad * pad = new TPad ("","",0.0,0.0,1.0,.95);
    pad->Draw();
    pad->cd();
    pad->SetLogy();
    pad->SetGrid();
    pad->SetBottomMargin(0.09);
    pad->SetTopMargin(0.01);

    multi->Draw("a4");
    multi->GetXaxis()->SetRangeUser(min,max-10);
    multi->GetYaxis()->SetRangeUser(3E-2,5);
    c->Modified(); c->Update();
    graph->Draw("PLsame");

    TGraph * th = new TGraph("utility/xS_HH_pb.txt");
    th->SetLineColor(kRed);
    th->SetLineWidth(2);
    th->Draw("L same");

    TLegend * leg = new TLegend (.5,.8,.85,.95);
    leg->AddEntry(graph_1,"Asympt. CL_{s} Expected #pm 1#sigma","lf");
    leg->AddEntry(graph_2,"Asympt. CL_{s} Expected #pm 2#sigma","lf");
    leg->AddEntry(th,"WED Radion (#Lambda_{R} = 1 TeV)","l");
    leg->Draw("same");

    graph->SetName("exclusion_xS");
    graph->SetTitle(multi->GetTitle());

    TFile * outFile = new TFile ("plot/exclusionPlot.root","RECREATE");
    c->Write();
    graph->Write();
    outFile->Close();

    return 1;

}
