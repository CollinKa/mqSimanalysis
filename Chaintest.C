/*
5-23 some new issue, due to the branch name is not exa
*/


#include "TChain.h"
#include "TH1.h"
#include "TCanvas.h"
#include "/homes/zheng/milliQanSim/include/mqROOTEvent.hh"
using namespace std;
R__LOAD_LIBRARY(../milliQanSim/build/libMilliQanCore.so)


void Chaintest()
{

  TChain rootEvents("Events");

  TString fileDir;
 
  for(int i=10;i<100;i++)
  { 
        fileDir="/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(i) + "/MilliQan.root";
		rootEvents.Add(fileDir);
  }

    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    rootEvents.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=rootEvents.GetEntries();
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
    TFile* file = new TFile("energytest.root", "RECREATE");
    //TH1F* histogram = new TH1F("histogram", "My Histogram", nbins, xmin, xmax);



    for(int index = 0; index < nentries; index++)
    {
        rootEvents.GetEntry(index);
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
                  cout << "energy dep: " << EdepArray[i] << endl; //in unit of MeV
                }


            }
               
        }
    }


  
}