//rewrite the code into multiple method
//(not complete yet)
//add the file loading code(TBD), check if the the method make sense(TBD)
//mutiple variable are missing
//try to isolate pure array manipulation

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

void processScintHits(mqROOTEvent* myROOTEvent, float EDEP[48])
{
    int numScintHits = myROOTEvent->GetScintRHits()->size();
    for (int h = 0; h < numScintHits; h++) {
        int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo() - 18;
        float Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
        EDEP[hitN] += Edp;
    }
}


/*
it has bug see v2
void processPMTHits(mqROOTEvent* myROOTEvent, float EDEPpmt[144])
{
    int numPMTHits = myROOTEvent->GetPMTRHits()->size();
    for (int h = 0; h < numPMTHits; h++) {
        int hitNpmt = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
        double_t Edppmt = myROOTEvent->GetPMTRHits()->at(h)->GetEventEnergyDeposit();
        // Process PMT hits
        // EDEPpmt[hitNpmt] += Edppmt;
    }
}
*/


void processSlapHits(mqROOTEvent* myROOTEvent, float EDEP[48])
{
    int slapHit = 0;
    int layerarray[4] = {0};
    int columnarray[3] = {0};
    int rowarray[4] = {0};

    for (int j = 0; j < 48; j++) {
        if (EDEP[j] > 0) {
            slapHit++;
            int layerNum = ((j - 18) / 4) % 4;
            int rowNum = (j - 18) / 12;
            int columnNum = j - 18;

            layerarray[layerNum]++;
            columnarray[columnNum]++;
            rowarray[rowNum]++;
        }
    }

    // Additional processing based on slapHits, layerarray, columnarray, rowarray
    // ...
}

//----------------------------------code at above are automatically generated
//this method assign the value arrays related to slab
void SlabArrays(mqROOTEvent* myROOTEvent,float EDEP[48],int layerarray[4],int rowarray[4],numScintHits,inslapHit)
{   
    for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo() - 18; 
            Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            EDEP[hitN] = EDEP[hitN] + Edp;
            if (EDEP[j] > 0) // in some cases energy is negative
            {
                slapHit++;
                //((slabNum-18)%12)/4=columnNum

                //I suspect the follow three array might not be correct. (bug)
                int layerNum = (hitN-18) % 4;
                int rowNum =((hitN-18) / 4 % 3);
                int columnNum = (slabNum-18)- 4*layerNum - 12 * rowNum;

                layerarray[layerNum] += 1;
                columnarray[columnNum] += 1;
                rowarray[rowNum] += 1;
            }
        }
}



void layerCheck_V3()
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
            float Edp = 0; //energy deposit 
            bool repeatness = false; //used to check if all hits are at different layers
    
            for(int index = 0; index < nentries; index++)
            {
                float EDEP[48] = {0.0}; // there are 48 slabs for slab detector.
                int layerarray[4] = {-1};
                int columnarray[3] = {-1};
                int rowarray[4] = {-1};

                int slapHit = 0; //how many slaps got hit in a event 

                bool FourInline = false;

                tree->GetEntry(index);


                //loop over events
                for (int Eindex = 0; Eindex < 48 ; Eindex ++)
                {
                    SlabArrays(mqROOTEvent* myROOTEvent,float EDEP[48],int layerarray[4],int rowarray[4],numScintHits,inslapHit);
                    //one hit per layer & 3 in line check TBD
                }











    float EDEP[48] = {0};

    for (int index = 0; index < nentries; index++) {


        processScintHits(myROOTEvent, EDEP);

        processSlapHits(myROOTEvent, EDEP);


    }


}
