/*
dont know how to use list yet
find the event that only 3 hit perlayer
//how to get the slab number?
myROOTEvent->GetScintRHits()->at(h)->GetCopyNo()



// save the layer of different hit of slab into the array
//add array
//add array check unique method
//break array assigment if the number of value is biggeer than required number of hits
// see PMTexplore.C as a reference of how to get the slab number and energy

5-22 23 update for the code to get deposit energy & slab number

Q: does myROOTEvent->GetScintRHits()->size() is the same as
myROOTEvent->GetSlabHit()

*** it is easy to build an method of uniqueness check. but this doesnt work in macro function


5-23 remove tchain from Chaintest.c
it has weird bug-finished

need to fixed the hard coding

histogram of energy should between 0-13 mEv.q
*/


#include "TCanvas.h"
#include "TTree.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TChain.h"
#include "/homes/zheng/milliQanSim/include/mqROOTEvent.hh"
#include "TGraph.h"
#include "TVector.h"
#include "TVectorD.h"
#include "TVectorF.h"
#include "TH1.h"
#include "TH1F.h"
#include "TString.h"
#include "TChain.h"
#include "TMultiGraph.h"
#include "/homes/zheng/milliQanSim/include/mqScintRHit.hh"

#include <iostream>
#include <filesystem>

using namespace std;
R__LOAD_LIBRARY(../milliQanSim/build/libMilliQanCore.so)


bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}




void layerCheck()
{
//TFile* file = new TFile("energytest.root", "RECREATE");.
TString fileDir;
string filename;


  for(int dataNum=10; dataNum < 999; dataNum++)
  { 
    fileDir="/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(dataNum) + "/MilliQan.root";
    filename = "/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(dataNum) + "/MilliQan.root";
    
    if (fileExists(filename)) 
    {
    
    TFile* file = new TFile(fileDir);
    TTree* tree = (TTree*)file->Get("Events;");
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=tree->GetEntries();
    //cout << nentries << endl;

    int SlabHits =0;
    int numPMTHits = 0;
    int layerNum = -1;
    int rowNum = -1;
    int columnNum = -1;
    int hitN = 0; //slab channel number
    int numScintHits = 0;
    float Edp = 0;
    bool repeatness = false;
    
    //TH1F* histogram = new TH1F("histogram", "My Histogram", nbins, xmin, xmax);



    //for(int index = 0; index < nentries; index++)
    for(int index = 0; index < 10; index++)   //test
    {
        tree->GetEntry(index);
        numScintHits=myROOTEvent->GetScintRHits()->size(); //get the number scintillator hit in an event
        
        //we want to investigate 4 hit in a event now

        //Ryan：there might be 100 scintHits even if only 4 distinct slabs are hit
        if (numScintHits < 100)
        {  

          //int layerArray[4] = {-1, -2, -3, -4}; //reset the array
          //float EdepArray[4] = {0,0,0,0};
          
          double edeps[100];
          int layerArray[100];

          for (int indexJ = 0, indexJ < 100 ,indexJ++)
          {
            layerArray = -1 * indexJ -1;
          }
          
          for (int h=0, h<numScintHits, h++){
          copyNo=rootEvent->GetScintHits->at(h)->GetCopyNo();
          edep=rootEvent->GetScintHits->at(h)->GetEDep();
          edeps[copyNo]=edeps[copyNo]+edep;
          }
          
          //save the layer into array
          for(int h=0;h<numScintHits;h++) //h is the index of data in an event
          {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();//get the slab number
            Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep(); 
				    layerNum = ((hitN-18)/4)%4;
            layerArray[h] = layerNum;
            EdepArray[h] = Edp;

			    }

            //slab hit check uniquness, check for one hit per layer

            for (int i = 0; i < numScintHits; i++)
            {
                for (int j = 1; j < numScintHits; j++)
                {
                    if(layerArray[j] == layerArray[i])
                    {
                        repeatness = true;
                    }
                }
            }

            
            if (repeatness == false)
            {
                cout << "dataNum"  << dataNum<< " " <<"event:" << index << endl;
                for (int i = 0; i < numScintHits; i++)
                {
                  if (EdepArray[i]>0) {cout << "energy dep(MeV): " << EdepArray[i] << endl;} //in unit of MeV
                }


            }
               
        }
    }

    }
  }
  
}