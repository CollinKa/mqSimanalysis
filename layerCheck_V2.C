//negative enery deposit???


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



void layerCheck_V2()
{
    TString fileDir;
    string filename;

    for(int dataNum=10; dataNum < 999; dataNum++) //loop over all withPhoton file
    {
        fileDir="/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(dataNum) + "/MilliQan.root";
        filename = "/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(dataNum) + "/MilliQan.root";

        if (fileExists(filename)) 
        {
            //int numPMTHits = 0; //(TBD)
            int layerNum = -1;
            int rowNum = -1;
            int columnNum = -1;

            int hitN = 0; //channel number
            int numScintHits = 0; //number of slab channal got hit for 
            float Edp = 0; //energy deposit of sigal hit
            bool repeatness = false; //used to check if all hits are at different layers
            
            
            
            
            TFile* file = new TFile(fileDir);
            TTree* tree = (TTree*)file->Get("Events;");
            mqROOTEvent* myROOTEvent = new mqROOTEvent();
            tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
            Long64_t nentries=tree->GetEntries();
            cout << "total entries:" << nentries << endl; //debug


            for(int index = 0; index < nentries; index++)
            {
                float EDEP[48] = {0.0}; // there are 48 slabs for slab detector.
                
                //the following four array is used to save the energy deposit for the pmts
                float EDEPpmt[144] = {0.0}; //
                //int hitN = 0; //pmt channal number
                numPMTHits = 0;



                int layerarray[4] = {0};
                int slapHit = 0; //how many slaps got hit in a event 

                tree->GetEntry(index);
                for (int Eindex = 0; Eindex < 48 ; Eindex ++)
                numScintHits=myROOTEvent->GetScintRHits()->size(); //how many particle hit the slabs in a event
                numPMTHits=myROOTEvent->GetPMTRHits()->size();

                cout << "num of particle hit the slabs in a event: " << numScintHits << endl;//debug
                cout << "num of particle hit the pmts in a event: " << numPMTHits << endl;//debug
                
                //for slabs
                for (int h =0; h < numScintHits; h++)
                {
                    hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo() - 18; // it reach 63, which cause the bug
                    Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                    cout << "Slab no: " << hitN << " energy of this hit:" << Edp << endl; 
                    EDEP[hitN] = EDEP[hitN] + Edp; //sum up the energy for a same slab
                }


                //for scintillator
                for (int h =0; h < numPMTHits; h++)
                {
                    hitN = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber(); // it reach 63, which cause the bug
                    Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                    cout << "Slab no: " << hitN << " energy of this hit:" << Edp << endl; 
                    EDEPpmt[hitN] = EDEPpmt[hitN] + Edp; //sum up the energy for a same slab
                }



                for (int j = 0; j < 48; j++)
                {
                    if (EDEP[j] > 0) 
                    {
                        slapHit++;
                        layerNum = ((hitN-18)/4)%4;
                        cout << "layerNum:" << layerNum << endl;
                        layerarray[layerNum] += 1;
                        //
                    }    
                }
                
                //we only insterest about the event that has less or equal to 4 hits
                if (slapHit < 5)
                {   
                    int count =0;
                    for (int n =0; n < 3; n++)
                    {
                        if (layerarray[n] > 1)
                        {
                            //repeatness = true;
                            //break;
                            count ++;
                        }
                    }

                    if (count == 0)
                    {
                        cout << index << "th event" << " in file " << dataNum << "satisfied the cut(all hits aredifferent layer)" << endl;
                    }
                }



            }





        }

    }

}