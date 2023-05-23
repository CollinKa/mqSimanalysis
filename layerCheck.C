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


  for(int i=10;i<100;i++)
  { 
    fileDir="/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(i) + "/MilliQan.root";
    filename = "/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(i) + "/MilliQan.root";
    
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



    for(int index = 0; index < nentries; index++)
    {
        tree->GetEntry(index);
        numScintHits=myROOTEvent->GetScintRHits()->size(); //get the number scintillator hit in an event
        if (numScintHits <4)
        {   
          int layerArray[4] = {-1, -2, -3, -4}; //reset the array
          float EdepArray[3] = {0,0,0};
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
                cout << "event:" << index << endl;
                for (int i = 0; i < numScintHits; i++)
                {
                  cout << "energy dep(MeV): " << EdepArray[i] << endl; //in unit of MeV
                }


            }
               
        }
    }

    }
  }
  
}