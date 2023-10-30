#include <TFile.h>
#include <TTree.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TVector3.h>
#include <iostream>
#include <TROOT.h>
#include <TLegend.h>
#include <vector>
#include <TChain.h>
#include <TLorentzVector.h>
#include "histogram.h"


void updateProgressBar(int progress, int total, int barWidth = 50) {
    float fraction = static_cast<float>(progress) / total;
    int numChars = static_cast<int>(fraction * barWidth);

    std::cout << "\r[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < numChars) std::cout << "#";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(fraction * 100.0) << "%";
    std::cout.flush();
}
/*
int main () {
    std::cout << "code has been initiated" << std::endl;

    char* file_path1= "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000002.output_pflow.root";
    char* file_path2= "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000003.output_pflow.root";
	
    TFile *f1 = new TFile(file_path1, "READ");
    TFile *f2 = new TFile(file_path2, "READ");
	//TTree *nominal = (TTree*)f->Get("nominal");
    TTree *truth1 = (TTree*)f1->Get("truth");
    TTree *truth2 = (TTree*)f2->Get("truth");

    ULong64_t eventNumber1; 
    ULong64_t eventNumber2; 

    truth1->SetBranchAddress("eventNumber", &eventNumber1);
    truth2->SetBranchAddress("eventNumber", &eventNumber2);

    Long64_t nentries1 = truth1->GetEntries();
    Long64_t nentries2 = truth2->GetEntries();

    
    for(Long64_t i=0; i<nentries1; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        truth1->GetEntry(i);
        

        for(Long64_t j=0; j<nentries2; j++){
            updateProgressBar(j+1, nentries2);
            truth2->GetEntry(j);
            if(eventNumber1 == eventNumber2){
                std::cout<<"There is a clash on the event with i = "<<i<<" and j = "<<j<<std::endl;
            }

        }

    }
    
    return 0;
}*/


#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <unordered_set>

int main () {
    std::cout << "Code has been initiated" << std::endl;
    

    char* file_path1= "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000004.output_pflow.root";
    char* file_path2= "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000005.output_pflow.root";
    
    TFile *f1 = new TFile(file_path1, "READ");
    TFile *f2 = new TFile(file_path2, "READ");
    
    TTree *truth1 = (TTree*)f1->Get("truth");
    TTree *truth2 = (TTree*)f2->Get("truth");

    ULong64_t eventNumber1;
    ULong64_t eventNumber2;

    truth1->SetBranchAddress("eventNumber", &eventNumber1);
    truth2->SetBranchAddress("eventNumber", &eventNumber2);

    Long64_t nentries1 = truth1->GetEntries();
    Long64_t nentries2 = truth2->GetEntries();

    std::unordered_set<ULong64_t> eventNumbersSet;

    // Store event numbers from truth2 in a set for quick lookup
    for (Long64_t j = 0; j < nentries2; j++) {
        if (j==0) { std::cout << "Entered the first loop" << std::endl; }
        truth2->GetEntry(j);
        eventNumbersSet.insert(eventNumber2);
        updateProgressBar(j+1, nentries2);
    }

    for (Long64_t i = 0; i < nentries1; i++) {
        if (i==0) { std::cout << "Entered the second loop" << std::endl; }
        truth1->GetEntry(i);
        updateProgressBar(i+1, nentries1);
        // Check if the event number from truth1 exists in the set
        if (eventNumbersSet.find(eventNumber1) != eventNumbersSet.end()) {
            std::cout << "There is a clash on the event with i = " << i << std::endl;
        }
    }

    return 0;
}
