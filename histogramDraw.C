//author Collin Zheng      zheng.1947@buckeyemail.osu.edu
// this one doesn't work for bench data extraction, use muonpmthit.c

#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TChain.h"
#include "../include/mqROOTEvent.hh" 
#include "TGraph.h"
#include "TVector.h"
#include "TVectorD.h"
#include "TVectorF.h"
#include "TH1.h"
#include "TH1F.h"
#include "TString.h"
#include "TChain.h"
#include "TMultiGraph.h"
#include <TH1D.h>
#include "../include/mqScintRHit.hh"


#include <iostream>
#include <filesystem>

using namespace std;
//R__LOAD_LIBRARY(libBenchCore.so) //no need to worry about this



void histogramDraw()
{
    TFile* file = new TFile("Sim_Am241frontMilliQan.root");
    TTree* tree = (TTree*)file->Get("Events;");
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
    int NbOfPMTHits;
    tree->SetBranchAddress("NbOfPMTHits", &NbOfPMTHits);
    Long64_t nentries=tree->GetEntries();
    TCanvas* canvas = new TCanvas("canvas", "pmtHits", 800, 600);
    canvas->SetFillColor(0);
    TFile outputFile("Sim_Am241frontMilliQanHist.root", "RECREATE");
    TH1D* histogram18 = new TH1D("histogram18", "chan18 pmtHits ", 90, 0, 90);
    TH1D* histogram19 = new TH1D("histogram19", "chan19 pmtHits", 90, 0, 90);
    TH1D* histogram20 = new TH1D("histogram20", "chan20 pmtHits", 90, 0, 90);
    TH1D* histogram21 = new TH1D("histogram21", "chan21 pmtHits", 90, 0, 90);
    

    

    for(int index = 0; index < nentries; index++)
    {
        tree->GetEntry(index);
        int pmt18E = 0;//number  of hit of different channal in each event
        int pmt19E = 0;
        int pmt20E = 0;
        int pmt21E = 0;
        int numPMTHits=myROOTEvent->GetPMTRHits()->size(); //try to figure out why does this doesnt work
        //cout << numPMTHits << endl;
        for(int h=0;h<numPMTHits;h++)
        {
            if(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber()==18)
            //if(myROOTEvent->GetPMTRHits()->GetPMTNumber()==18)
            {
                //pmt18E++;
                histogram18->Fill(NbOfPMTHits);
                
            }
            
            if(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber()==19)
            //if(myROOTEvent->GetPMTRHits()->GetPMTNumber()==19)
            {
                //pmt19E++;
                histogram19->Fill(NbOfPMTHits);
            }

            if(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber()==20)
            //if(myROOTEvent->GetPMTRHits()->GetPMTNumber()==20)
            {
                //pmt20E++;
                histogram20->Fill(NbOfPMTHits);
            }
            if(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber()==21)
            //if(myROOTEvent->GetPMTRHits()->GetPMTNumber()==21)
            {
                //pmt21E++;
                histogram21->Fill(NbOfPMTHits);
            }
            
            

        }
        //histogram18->Fill(pmt18E);
        //histogram19->Fill(pmt19E);
        //histogram20->Fill(pmt20E);
        //histogram21->Fill(pmt21E);
    }
    canvas->cd(); 
    histogram18->Draw();
    histogram19->Draw("same");
    histogram20->Draw("same");
    histogram21->Draw("same");
    histogram18->SetLineColor(kRed);
    histogram19->SetLineColor(kBlue);
    histogram20->SetLineColor(kBlack);
    histogram21->SetLineColor(kGreen);
    histogram18->GetXaxis()->SetTitle("pmt hits");
    histogram19->GetXaxis()->SetTitle("pmt hits");
    histogram20->GetXaxis()->SetTitle("pmt hits");
    histogram21->GetXaxis()->SetTitle("pmt hits");
    histogram18->GetYaxis()->SetRangeUser(0, 60000);
    histogram19->GetYaxis()->SetRangeUser(0, 60000);
    histogram20->GetYaxis()->SetRangeUser(0, 60000);
    histogram21->GetYaxis()->SetRangeUser(0, 60000);

    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(histogram18, "chan18", "l");
    legend->AddEntry(histogram19, "chan19", "l");
    legend->AddEntry(histogram20, "chan20", "l");
    legend->AddEntry(histogram21, "chan21", "l");
    legend->Draw();
    histogram18->Write();
    histogram19->Write();
    histogram20->Write();
    histogram21->Write();
    canvas->Update();
    canvas->Print();
    canvas->Write(); 
    outputFile.Close();

}