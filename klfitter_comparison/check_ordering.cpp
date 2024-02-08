
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
#include <TObject.h>
#include <TList.h>

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

int main () {
    std::cout << "code has been initiated" << std::endl;
    /* INPUT DATA */
    char* file_path= "/eos/user/m/moriolpe/mphys_project/root_files/matched_klf_ordered.root";
	TFile *input_file = new TFile(file_path, "READ");
	TTree *nominal = (TTree*)input_file->Get("matched");

    /* CREATE ALL NOMINAL BRANCHES */
    std::vector<float>* nominal_jet_pt = 0;
    std::vector<int>* jet_order_pt = 0;
    
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt);     
    nominal->SetBranchAddress("jet_order_pt", &jet_order_pt);     

    Long64_t nentries = nominal->GetEntries();
    int njets;
    
    std::cout << "The total number of events is " << nentries << std::endl;
    
    for(Long64_t i=0; i<20; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        
        nominal->GetEntry(i);
        
        njets = nominal_jet_pt->size();
        std::cout << std::endl;
        for (int j{0}; j<njets; j++) {
            std::cout << (*nominal_jet_pt)[j] << "  "; 
        }
        std::cout << std::endl;
        for (int j{0}; j<njets; j++) {
            std::cout << (*jet_order_pt)[j] << "  "; 
        }
        std::cout << std::endl;
    }  
    return 0;
}
