
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
    char* file_path= "/eos/user/m/moriolpe/mphys_project/root_files/combined_data.root";
	TFile *input_file = new TFile(file_path, "READ");
	TTree *nominal = (TTree*)input_file->Get("nominal");

    /* CREATE ALL NOMINAL BRANCHES */
    float_t weight;
    std::vector<float>* nominal_jet_pt = 0;
    std::vector<char>* jet_isbtagged_DL1dv01_60 = 0;
    std::vector<float>* jet_DL1dv01 = 0;
    std::vector<float>* jet_isbtagged_GN120220509_60 = 0;
    std::vector<unsigned int>* klfitter_model_bhad_jetIndex = 0;
    
    Int_t mujets_dl1d_2022;
    Int_t mujets_2022_dl1d_lowPt;
    Int_t ejets_dl1d_2022;
    Int_t ejets_2022_dl1d_lowPt;
    
    nominal->SetBranchAddress("weight_mc", &weight);
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt); 
    nominal->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);
    nominal->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);
    nominal->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);

    nominal->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal->SetBranchAddress("ejets_dl1d_2022", &ejets_dl1d_2022);
    nominal->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);            

    // Getvector of jets
    // Create vector of Lorentz objects
    // Create vector of matched labels
    // Match jets to each truth object
    // Set unmatched jets to unmatched value
    Long64_t nentries = nominal->GetEntries();
    int njets = 0;
    int b_tagged_jets = 0;
    int n_0_btags = 0;
    int n_1_btags = 0;
    int n_2_btags = 0;
    int n_3_btags = 0;
    int n_more_btags = 0;
    unsigned long nw_0_btags = 0;
    unsigned long nw_1_btags = 0;
    unsigned long nw_2_btags = 0;
    unsigned long nw_3_btags = 0;
    unsigned long nw_more_btags = 0;
    
    int Gn_0_btags = 0;
    int Gn_1_btags = 0;
    int Gn_2_btags = 0;
    int Gn_3_btags = 0;
    int Gn_more_btags = 0;
    unsigned long Gnw_0_btags = 0;
    unsigned long Gnw_1_btags = 0;
    unsigned long Gnw_2_btags = 0;
    unsigned long Gnw_3_btags = 0;
    unsigned long Gnw_more_btags = 0;
    
    std::cout << "The total number of events is " << nentries << std::endl;
    bool manual = false;
    
    int counter = 0;
    
    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        
        nominal->GetEntry(i);
        updateProgressBar(i+1, nentries);
        //if (mujets_dl1d_2022 || mujets_2022_dl1d_lowPt || ejets_dl1d_2022 || ejets_2022_dl1d_lowPt) {
        counter += 1;
        njets = nominal_jet_pt->size();
        b_tagged_jets = 0;
        for (int j{0}; j<njets; j++) {
            if (manual) {
                if ((*jet_DL1dv01)[j]>4.854) {
                    b_tagged_jets += 1;
                }
            } else {
                if ((*jet_isbtagged_DL1dv01_60)[j]) {
                    b_tagged_jets += 1;
                }
            }
        }
        if (b_tagged_jets == 0) {n_0_btags += 1; nw_0_btags += abs(weight);}
        else if (b_tagged_jets == 1) {n_1_btags += 1; nw_1_btags += abs(weight);}
        else if (b_tagged_jets == 2) {n_2_btags += 1; nw_2_btags += abs(weight);}
        else if (b_tagged_jets == 3) {n_3_btags += 1; nw_3_btags += abs(weight);}
        else {n_more_btags += 1; nw_more_btags += abs(weight);}
        
        if (b_tagged_jets == 0) {
            b_tagged_jets = 0;
            for (int k{0}; k<njets; k++) {
                if ((*jet_isbtagged_GN120220509_60)[k]) { b_tagged_jets += 1; }
            }
            if (b_tagged_jets == 0) {Gn_0_btags += 1; Gnw_0_btags += abs(weight);}
            else if (b_tagged_jets == 1) {Gn_1_btags += 1; Gnw_1_btags += abs(weight);}
            else if (b_tagged_jets == 2) {Gn_2_btags += 1; Gnw_2_btags += abs(weight);}
            else if (b_tagged_jets == 3) {Gn_3_btags += 1; Gnw_3_btags += abs(weight);}
            else {Gn_more_btags += 1; Gnw_more_btags += abs(weight);}
            
        }
                
                
        //}
    }
    
    std::cout << std::endl << "number of selected events found: " << counter << std::endl;
    
    std::cout << std::endl << "B-TAGGED JETS DISTRIBUTION:" << std::endl;
    std::cout << "0 b-tags:  " << n_0_btags << std::endl;
    std::cout << "1 b-tag:   " << n_1_btags << std::endl;
    std::cout << "2 b-tags:  " << n_2_btags << std::endl;
    std::cout << "3 b-tags:  " << n_3_btags << std::endl;
    std::cout << "4+ b-tags: " << n_more_btags << std::endl;
    
    std::cout << std::endl << "WEIGHTED B-TAGGED JETS DISTRIBUTION:" << std::endl;
    std::cout << "0 b-tags:  " << nw_0_btags << std::endl;
    std::cout << "1 b-tag:   " << nw_1_btags << std::endl;
    std::cout << "2 b-tags:  " << nw_2_btags << std::endl;
    std::cout << "3 b-tags:  " << nw_3_btags << std::endl;
    std::cout << "4+ b-tags: " << nw_more_btags << std::endl;
    
    std::cout << std::endl << "EVENTS WITH 0 DL1d B-TAGGINGS:" << std::endl;   
    
    std::cout << std::endl << "B-TAGGED JETS DISTRIBUTION:" << std::endl;
    std::cout << "0 b-tags:  " << Gn_0_btags << std::endl;
    std::cout << "1 b-tag:   " << Gn_1_btags << std::endl;
    std::cout << "2 b-tags:  " << Gn_2_btags << std::endl;
    std::cout << "3 b-tags:  " << Gn_3_btags << std::endl;
    std::cout << "4+ b-tags: " << Gn_more_btags << std::endl;
    
    std::cout << std::endl << "WEIGHTED B-TAGGED JETS DISTRIBUTION:" << std::endl;
    std::cout << "0 b-tags:  " << Gnw_0_btags << std::endl;
    std::cout << "1 b-tag:   " << Gnw_1_btags << std::endl;
    std::cout << "2 b-tags:  " << Gnw_2_btags << std::endl;
    std::cout << "3 b-tags:  " << Gnw_3_btags << std::endl;
    std::cout << "4+ b-tags: " << Gnw_more_btags << std::endl;
    
    return 0;
}
