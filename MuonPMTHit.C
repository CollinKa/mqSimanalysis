/*
Origianal author : Ryan Schmitz
Edit by Collin Zheng 
*/
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
//R__LOAD_LIBRARY(../../build/libBenchCore.so)
#include <iostream>
#include <filesystem>

void MuonPMTHit() {
  TChain ch("Events");
  ch.Add("cd109centerNoniso.root"); // some test sim output file

  TFile outputFile("cd109centerNonisoHist.root", "RECREATE");

  TH1F* hNPE18 = new TH1F("hNPE18", "Photoelectrons Detected Per Incident Gamma Chan 18", 100, 0, 100);
  TH1F* hNPE19 = new TH1F("hNPE19", "Photoelectrons Detected Per Incident Gamma Chan 19", 100, 0, 100);
  TH1F* hNPE20 = new TH1F("hNPE20", "Photoelectrons Detected Per Incident Gamma Chan 20", 100, 0, 100);
  TH1F* hNPE21 = new TH1F("hNPE21", "Photoelectrons Detected Per Incident Gamma Chan 21", 100, 0, 100);

  hNPE18->GetXaxis()->SetTitle("Number of Photoelectrons Detected");
  hNPE18->GetYaxis()->SetTitle("Bin Members");
  hNPE19->GetXaxis()->SetTitle("Number of Photoelectrons Detected");
  hNPE19->GetYaxis()->SetTitle("Bin Members");
  hNPE20->GetXaxis()->SetTitle("Number of Photoelectrons Detected");
  hNPE20->GetYaxis()->SetTitle("Bin Members");
  hNPE21->GetXaxis()->SetTitle("Number of Photoelectrons Detected");
  hNPE21->GetYaxis()->SetTitle("Bin Members");

  TCanvas* c1 = new TCanvas("c1", "c1", 0, 400, 600, 300);
  c1->SetFillColor(0);

  hNPE18->SetLineColor(kRed);
  hNPE19->SetLineColor(kBlue);
  hNPE20->SetLineColor(kBlack);
  hNPE21->SetLineColor(kGreen);

  ch.Draw("Sum$(PMTHits->pmtNumber==18)>>hNPE18");
  ch.Draw("Sum$(PMTHits->pmtNumber==19)>>hNPE19", "" ,"SAME");
  ch.Draw("Sum$(PMTHits->pmtNumber==20)>>hNPE20", "" ,"SAME");
  ch.Draw("Sum$(PMTHits->pmtNumber==21)>>hNPE21", "" ,"SAME");

  hNPE18->GetYaxis()->SetRangeUser(0, 60000);
  hNPE19->GetYaxis()->SetRangeUser(0, 60000);
  hNPE20->GetYaxis()->SetRangeUser(0, 60000);
  hNPE21->GetYaxis()->SetRangeUser(0, 60000);

  c1->cd();

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(hNPE18, "chan18", "l");
  legend->AddEntry(hNPE19, "chan19", "l");
  legend->AddEntry(hNPE20, "chan20", "l");
  legend->AddEntry(hNPE21, "chan21", "l");
  legend->Draw();

  hNPE18->Write();
  hNPE19->Write();
  hNPE20->Write();
  hNPE21->Write();
  c1->Write();

  outputFile.Close();
}
