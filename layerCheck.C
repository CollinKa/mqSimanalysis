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


using namespace std;
R__LOAD_LIBRARY(../milliQanSim/build/libMilliQanCore.so)

void layerCheck()
{   
    int SlabHits =0;
    int numPMTHits = 0;
    int layerNum = -1;
    int rowNum = -1;
    int columnNum = -1;
    int hitN = 0; //slab channel number
    int numScintHits = 0;

    //int layerArray[3];  //3 layer got hit, but only one hit per layer
    bool repeatness = false;


    TFile* file = new TFile("/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir12/MilliQan.root");
    TTree* tree = (TTree*)file->Get("Events;8");
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=tree->GetEntries();

    for(int index = 0; index < nentries; index++)
    {
        tree->GetEntry(index);
        //myList.clear();// reset the list for each event
        numScintHits=myROOTEvent->GetScintRHits()->size();
        if (numScintHits <4)
        {   
            //reset the array
            int layerArray[3] = {-1, -1, -1}; //reset the array
            //save the layer into array
            for(int h=0;h<numScintHits;h++) //h is the index of data in an event
            {
				

                hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
				layerNum = ((hitN-18)/4)%4;
                


                //cout << layerNum << endl;
                //cout << index << endl;

                layerArray[h] = layerNum;
			}

            //slab hit check uniquness
            //
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
            }

                
                /*
                // Print the elements of the list
                for (const auto& element : myList)
                {
                    cout << element << " " << endl;
                }
                */
               
        }
    }
}