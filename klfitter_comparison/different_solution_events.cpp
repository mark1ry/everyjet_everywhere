
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
    int dl1_b_tagged_jets = 0;
    int gn_b_tagged_jets = 0;
    std::vector<int> n_0_btags(5);
    std::vector<int> n_1_btags(5);
    std::vector<int> n_2_btags(5);
    std::vector<int> n_3_btags(5);
    std::vector<int> n_more_btags(5);
    std::vector<int> nw_0_btags(5);
    std::vector<int> nw_1_btags(5);
    std::vector<int> nw_2_btags(5);
    std::vector<int> nw_3_btags(5);
    std::vector<int> nw_more_btags(5);
    
    std::cout << "The total number of events is " << nentries << std::endl;
    bool manual = false;
    
    int array_index = 0;
    
    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        
        nominal->GetEntry(i);
        updateProgressBar(i+1, nentries);

        njets = nominal_jet_pt->size();
        dl1_b_tagged_jets = 0;
        for (int j{0}; j<njets; j++) {
            if (manual) {
                if ((*jet_DL1dv01)[j]>4.854) {
                    dl1_b_tagged_jets += 1;
                }
            } else {
                if ((*jet_isbtagged_DL1dv01_60)[j]) {
                    dl1_b_tagged_jets += 1;
                }
            }
        }
        
        gn_b_tagged_jets = 0;
        for (int k{0}; k<njets; k++) {
            if ((*jet_isbtagged_GN120220509_60)[k]) { gn_b_tagged_jets += 1; }
        }
        
        if (gn_b_tagged_jets == 0) {array_index = 0;}
        else if (gn_b_tagged_jets == 1) {array_index = 1;}
        else if (gn_b_tagged_jets == 2) {array_index = 2;}
        else if (gn_b_tagged_jets == 3) {array_index = 3;}
        else {array_index = 4;}
        
        if (dl1_b_tagged_jets == 0) {n_0_btags[array_index] += 1; nw_0_btags[array_index] += abs(weight);}
        else if (dl1_b_tagged_jets == 1) {n_1_btags[array_index] += 1; nw_1_btags[array_index] += abs(weight);}
        else if (dl1_b_tagged_jets == 2) {n_2_btags[array_index] += 1; nw_2_btags[array_index] += abs(weight);}
        else if (dl1_b_tagged_jets == 3) {n_3_btags[array_index] += 1; nw_3_btags[array_index] += abs(weight);}
        else {n_more_btags[array_index] += 1; nw_more_btags[array_index] += abs(weight);}
            
    }
    
    
    std::cout << std::endl << "B-TAGGED JETS DISTRIBUTION:" << std::endl;
    std::cout << "0 b-tags:  ";
    for (int index{0}; index<5; index++) {std::cout << n_0_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "1 b-tag:   ";
    for (int index{0}; index<5; index++) {std::cout << n_1_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "2 b-tags:  ";
    for (int index{0}; index<5; index++) {std::cout << n_2_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "3 b-tags:  ";
    for (int index{0}; index<5; index++) {std::cout << n_3_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "4+ b-tags: ";
    for (int index{0}; index<5; index++) {std::cout << n_more_btags[index] << "  ";}
    std::cout << std::endl;
    
    std::cout << std::endl << "WEIGHTED B-TAGGED JETS DISTRIBUTION:" << std::endl;
    std::cout << "0 b-tags:  ";
    for (int index{0}; index<5; index++) {std::cout << nw_0_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "1 b-tag:   ";
    for (int index{0}; index<5; index++) {std::cout << nw_1_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "2 b-tags:  ";
    for (int index{0}; index<5; index++) {std::cout << nw_2_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "3 b-tags:  ";
    for (int index{0}; index<5; index++) {std::cout << nw_3_btags[index] << "  ";}
    std::cout << std::endl;
    std::cout << "4+ b-tags: ";
    for (int index{0}; index<5; index++) {std::cout << nw_more_btags[index] << "  ";}
    std::cout << std::endl;

    return 0;
}
