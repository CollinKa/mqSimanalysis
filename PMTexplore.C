// this file is used to explore how to get the npe hits at different PMTs 


/*

5-20 how to use GetEDep() in mqScinRHit.hh?  how to get scint number(see Ryan's note)?
the get energy deposit from mqScintRHit.hh are the same as the one in mqROOTEvent.hh

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

using namespace std;
R__LOAD_LIBRARY(../milliQanSim/build/libMilliQanCore.so)

int PMTexplore()
{   int numPMTHits=0;
    int numSciHits =0;
    Double_t DE = 0;
    Int_t hitN=0;
    int h = 0; //index of element inside a event
    int NbSlabChan = -1; //slab channal number
    TFile* file = new TFile("/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir12/MilliQan.root");
    TTree* tree = (TTree*)file->Get("Events;8");
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    mqScintRHit* myScintRHit = new mqScintRHit();
    tree->SetBranchAddress("ROOTEvent", &myROOTEvent);

    Long64_t nentries=tree->GetEntries();
    tree->GetEntry(50);

    //numPMTHits = myROOTEvent->GetPMTRHits()->size(); // this return the number of total hits in a event
    //numSciHits = myROOTEvent->GetScintRHits()->size();
    //cout << numSciHits << endl; //it return 2 hits?????
    DE = myROOTEvent->GetScintRHits()->at(h)->GetEDep(); // from mqScinRHit.hh get the energy deposit, got to figure out how to use this
    //the one in mqScitRHit.hh return same thing.
    cout << DE << "(DE)" << endl; 
    

    NbSlabChan = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
    cout << NbSlabChan << "NbSlabChan" << endl; 
    /*
    for(int index = 0; index < nentries; index++)
    {
        tree->GetEntry(index);
        //barHits = myROOTEvent->GetBarHit();
        //cout << barHits << endl;
        DE = myROOTEvent->GetEventEnergyDeposit();
        if (DE > 0) 
        {
            cout << DE << " "<< index <<"(DE & index)" << endl; 
        }
    }
    */

    /*
    for(int h=0;h<numPMTHits;h++){
		hitN=myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
        DE = myROOTEvent->GetEventEnergyDeposit();
        cout << hitN << endl;//print out which the correspond pmtnumber for each hit
		cout << DE << endl;
        //PMTHits[hitN]++;
        // with pmt number I can do layer check
    }
    */
    
    
    
    

    return 0;
    
}