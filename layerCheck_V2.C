//negative enery deposit???
//update three in a line(done)
//why I can't see the 4 in a line?

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
    ofstream FOUR("fourInalint.txt");
    ofstream ONEHIT("oneHitperLayer.txt");
    ofstream FOUREn("fourInalintEnergy.txt");
    ofstream ONEHITEn("oneHitperLayerEnergy.txt");

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
            
            
            
            
            TFile* file = new TFile(fileDir);
            TTree* tree = (TTree*)file->Get("Events;");
            mqROOTEvent* myROOTEvent = new mqROOTEvent();
            tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
            Long64_t nentries=tree->GetEntries();
            //cout << "total entries:" << nentries << endl; //debug


            for(int index = 0; index < nentries; index++)
            {
                float EDEP[48] = {0.0}; // there are 48 slabs for slab detector.
                
                //the following four array is used to save the energy deposit for the pmts
                float EDEPpmt[144] = {0.0}; //there are 144 pmts
                //int hitN = 0; //pmt channal number
                int numPMTHits = 0;//number of pmt hit per event



                int layerarray[4] = {0};
                int columnarray[3] = {0};
                int rowarray[4] = {0};

                int slapHit = 0; //how many slaps got hit in a event
                //note although slab and scitilator are the same thing, they are different. slapHit record the number of hits that the correspond to 
                //energy is not zero. and the numScintHits register the hit in a event regardless the amount of energy. Sometime the energy is negative value.

                bool FourInline = false;

                tree->GetEntry(index);

                //for (int Eindex = 0; Eindex < 48 ; Eindex ++) // I dont remember what does this do. I might need to delete it
                numScintHits=myROOTEvent->GetScintRHits()->size(); //how many particle hit the slabs in a event
                

                //cout << "num of particle hit the slabs in a event: " << numScintHits << endl;//debug
                //cout << "num of particle hit the pmts in a event: " << numPMTHits << endl;//debug
                
                //for slabs
                for (int h =0; h < numScintHits; h++)
                {
                    hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo() - 18; 
                    Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                    //cout << "Slab no: " << hitN << " energy of this hit:" << Edp << endl; 
                    EDEP[hitN] = EDEP[hitN] + Edp; //sum up the energy for a same slab

                    
                }


                //current issue add array of pmy hits
                for (int j = 0; j < 48; j++)
                {
                    if (EDEP[j] > 0) 
                    {

                        slapHit++;
                        layerNum = (j)%4;
                        int rowNum =(j)/12;
                        int columnNum = ((j)%12)/4;

                        //cout << "layerNum:" << layerNum << endl;
                        layerarray[layerNum-1] += 1;
                        columnarray[columnNum-1] += 1;
                        rowarray[rowNum-1] += 1;
                    }    
                }


                //debug for why does the four in a line can't be seen
                //check the arrays
                //for (int z, )
                //cout << "layerarray " <<  

                //for pmt //(bug) 
                // how to get the energy of pmt?
                /*

                numPMTHits=myROOTEvent->GetPMTRHits()->size();

                for (int h =0; h < numPMTHits; h++)
                {
                    int hitNpmt = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber(); // need to do conversion like above to fit into the array
                    //one way is to get the row, layer, column and then check if 18,19,20,21 can 
                    //((pmtNumber-18)/4)%4=layerNum , 18 can be swap in 19,20,21

                    //double_t Edppmt = myROOTEvent->GetPMTRHits()->at(h)->GetEventEnergyDeposit();
                    double_t Edppmt = myROOTEvent->at(h)->GetEventEnergyDeposit();
                    cout << "pmt no: " << hitNpmt << " energy of this hit:" << Edppmt << endl; 
                    //EDEPpmt[hitN] = EDEPpmt[hitN] + Edp; //sum up the energy for a same slab
                }
                
                */
                
                


                //---
                
                


                //----
                //we only insterest about the event that has less or equal to 4 hits
                if (slapHit == 4)
                {   
                    int count =0;
                    bool columnCheck = false;
                    bool rowCheck = false;

                    for (int n =0; n < 4; n++)
                    {
                        if (layerarray[n] > 1)
                        {
                            //repeatness = true;
                            //break;
                            count ++;
                        }
                    }

                    for(int z = 0; z < 3 ;z++)
                    {
                        if (columnarray[z] == 4)
                        {
                            columnCheck = true;
                        }
                    }

                    for(int z = 0; z < 4 ;z++)
                    {
                        if (rowarray[z] == 4)
                        {
                            rowCheck = true;
                        }
                    }


                    if ((rowCheck ==  true) && (columnCheck == true))
                    {
                        FourInline = true;
                    }

                    


                    
                    if ((count == 0) && (FourInline == true))
                    {
                        //cout << index << "th event" << " in file " << dataNum << "satisfied the cut(all hits aredifferent layer)" << endl;
                        FOUR << index << "th event" << " in file " << dataNum << "is four in a line" << endl;
                        //make the graph of energy of three in aline

                        for (int j = 0; j < 48; j++)
                        {
                            if (EDEP[j] > 0)
                            {
                                FOUREn << EDEP[j] << endl;
                            }                    
                        } 

                        
                    }
                    
                    
                    if (count == 0)
                    {
                        ONEHIT << index << "th event" << " in file " << dataNum << " satisfied the cut(all hits are different layer)" << endl;
                        
                        for (int j = 0; j < 48; j++)
                        {
                        if (EDEP[j] > 0)
                        {
                            ONEHITEn << EDEP[j] << endl;
                        }
                                   
                        } 
                        
                    }
                    
                    
                    
                    


                    
                }



            }





        }

    

    }
    FOUR.close();
    ONEHIT.close();
    FOUREn.close();
    ONEHITEn.close();

}