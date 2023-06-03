//after these cut, one hit per layer is impossible
/*
end of channal looping for one evnet
slab channal:3
slab channal:6
end of channal looping for one evnet


end of channal looping for one evnet
slab channal:0
slab channal:18
end of channal looping for one evnet

there should be one event that pass the hit but cant be seen. how come? possibly didnt pass the energy cut

6-4 loose the cut in one layer can have at most 2 hits, add some debug message

6-5 remove energy check and extract energy distribution to see how does data look like.

*/
#include "TCanvas.h"
#include "TTree.h"
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



void layerCheck_V6()
{
    TString fileDir;
    string filename;
    //ofstream FOUR("fourInalint.txt");
    //ofstream ONE("oneHitperLayer.txt");
    ofstream ONEE("oneHitperE.txt");//record the energy deposit on each slab 
    ofstream FOURHit("fourInalineSlabHits.txt");//record the number of hits on each slab 
    ofstream ONEHIT("oneHitperLayerSlabHits.txt");
    ofstream FourE("FourinLINEE.txt");



    for(int dataNum=10; dataNum < 250; dataNum++) //loop over 25% of withPhoton file
    {
        fileDir="/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(dataNum) + "/MilliQan.root";
        filename = "/net/cms17/cms17r0/schmitz/slabSimMuon/withPhotons/48slab/cosmicdir" + to_string(dataNum) + "/MilliQan.root";

        if (fileExists(filename)) 
        {
            int layerNum = -1;
            int rowNum = -1;
            int columnNum = -1;

            int hitN = 0; //channel number
            int numScintHits = 0; //number of slab channal got hit for a event
            float Edp = 0.0; //energy deposit 
            bool repeatness = false; //used to check if all hits are at different layers
            
            
            
            
            TFile* file = new TFile(fileDir);
            TTree* tree = (TTree*)file->Get("Events;");
            mqROOTEvent* myROOTEvent = new mqROOTEvent();
            tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
            Long64_t nentries=tree->GetEntries();



            for(int index = 0; index < nentries; index++)
            {
                int slabHits[48] = {0}; // there are 48 slabs for slab detector. it register the number of hit on each slab
                float EDEP[48] = {0.0};
                int numPMTHits = 0;//number of pmt hit per event
                int layerarray[4] = {0};
                int columnarray[3] = {0};
                int rowarray[4] = {0};
                int slapHit = 0; //how many slaps got hit in a event


                bool FourInline = false;

                tree->GetEntry(index);

                //for (int Eindex = 0; Eindex < 48 ; Eindex ++) // I dont remember what does this do. I might need to delete it
                numScintHits=myROOTEvent->GetScintRHits()->size(); //how many particle hit the slabs in a event
                numPMTHits = myROOTEvent->GetPMTRHits()->size();
                int numTPMThits = 0; //number of total pmt hits
                int slabChan;


                for(int h=0; h < numPMTHits; h++)
                {
                    slabChan = 0;
                    hitN = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
                    cout << "hitN:" << hitN << endl; //debug
                    //convert the pmt number to corresponding slab channal
                    int j = ((hitN-18)/4);
                    layerNum = j%4;
                    rowNum =(j)/12;
                    columnNum = ((j)%12)/4;
                    slabChan = layerNum + 12 * rowNum + 4 * columnNum; //did I mess this up?
                    cout << "layerNum:" << layerNum << " rowNum:" << rowNum << " columnNum:" << columnNum << endl;//debug
                    cout << "slab number:" <<  slabChan << endl;//debug
                    
                    //if (slabChan > 48)
                    //{
                    //cout << "slabChan:" <<slabChan << endl; //debug
                    //}
                    slabHits[slabChan]++;
                    numTPMThits++;

                }
                
                float totalEdp = 0.0;

                for (int h =0; h < numScintHits; h++)
                {
                    hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo() - 18; 
                    Edp = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                    //cout << "Slab no: " << hitN << " energy of this hit:" << Edp << endl; 
                    EDEP[hitN] = EDEP[hitN] + Edp; //sum up the energy for a same slab
                    //cout<< "EDP:" << Edp << endl;
                    totalEdp = totalEdp + Edp;//in MeV
                        
                }
                //debug
                

                //if((numTPMThits <= 400) && (totalEdp < 1))
                //it took me a while for no seeing any data, change the cut
                if((numTPMThits <= 1000) && (totalEdp <= 1))
                //if((numTPMThits <= 1000) && (totalEdp > 0 ))
                {   
                    cout << "numTPMThits:" << numTPMThits << endl;
                    cout << "totalEdp:" << totalEdp << endl;
                    //debug for loop over slabHits and EDEP array and print the channal
                    for (int j = 0; j < 48; j++)
                    {
                        //if ((slabHits[j] > 0) && (EDEP[j]>0))
                        //{   
                            int slablayer = j % 4;
                            cout << "slab channal:" << j <<" energy of slab:" << EDEP[j] << "slab layer:" << slablayer << " totalHits: "<< slabHits[j] <<endl;
                            // looks like slabHits array assignment has issue
                        //}
                    }
                    cout << "end of channal looping for one evnet" << endl;

                    
                    for (int j = 0; j < 48; j++)
                    {
                        //if (slabHits[j] < 100) //slabHits is the sum of hit of four conneting pmt
                        if (slabHits[j] < 1000)
                        {

                            slapHit++;
                            layerNum = (j)%4;
                            int rowNum =(j)/12;
                            int columnNum = ((j)%12)/4;

                            //cout << "layerNum:" << layerNum << endl;
                            layerarray[layerNum] += 1;
                            columnarray[columnNum] += 1;
                            rowarray[rowNum] += 1;
                        }    
                    }



                    //if (slapHit == 4) //four slabs got hit
                    if (slapHit >= 2)
                    {   
                        int count =0; //check if a single layer has multiple hits
                        bool columnCheck = false;
                        bool rowCheck = false;

                        for (int n =0; n < 4; n++)
                        {
                            if (layerarray[n] >= 3) // loose the cut to allow a layer can have at most 2 hits.
                            {
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

                        


                        //four in a line
                        if ((count == 0) && (FourInline == true))
                        {
                            
                            cout << index << "th event" << " in file " << dataNum << "is four in a line" << endl;
                            //make the graph of energy of three in aline

                            for (int j = 0; j < 48; j++)
                            {
                                if (slabHits[j] > 0)
                                {
                                    FOURHit << slabHits[j] << endl;
                                    FourE << EDEP[j] << endl;
                                    //output totalEdp

                                }                    
                            } 

                            
                        }
                        
                        //one hit per layer
                        if (count == 0)
                        {
                            cout << index << "th event" << " in file " << dataNum << " satisfied the cut(all hits are different layer)" << endl;
                            
                            for (int j = 0; j < 48; j++)
                            {
                            if (slabHits[j] > 0)
                            {
                                ONEHIT << slabHits[j] << endl;
                                ONEE << EDEP[j] << endl;
                                //output totalEdp
                                
                            }
                                    
                            } 
                            
                        }
                        
                        
                    }

                }

            }





        }

    
    cout << "end of file" << endl;
    }
    ONEE.close();
    FOURHit.close();
    ONEHIT.close();
    FourE.close();

}