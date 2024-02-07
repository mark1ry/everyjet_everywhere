
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
    std::vector<float>* jet_isbtagged_GN120220509_60 = 0;
    std::vector<float>* klfitter_logLikelihood = 0;
    
    std::vector<float>* el_pt = 0;
    std::vector<float>* mu_pt = 0;
    
    nominal->SetBranchAddress("weight_mc", &weight);
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt); 
    nominal->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);
    nominal->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);
    nominal->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    
    nominal->SetBranchAddress("el_pt", &el_pt);
    nominal->SetBranchAddress("mu_pt", &mu_pt);      

    // DECLARATION OF CONTAINERS
    Long64_t nentries = nominal->GetEntries();
    int njets = 0;
    int dl1_b_tagged_jets = 0;
    int gn_b_tagged_jets = 0;
    std::vector<int> n_0_btags(5, 0);
    std::vector<int> n_1_btags(5, 0);
    std::vector<int> n_2_btags(5, 0);
    std::vector<int> n_3_btags(5, 0);
    std::vector<int> n_more_btags(5, 0);
    std::vector<int> nw_0_btags(5, 0);
    std::vector<int> nw_1_btags(5, 0);
    std::vector<int> nw_2_btags(5, 0);
    std::vector<int> nw_3_btags(5, 0);
    std::vector<int> nw_more_btags(5, 0);
    
    std::vector<int> njet_dist(5, 0);
    int nmuons = 0;
    int nelectrons = 0;
    
    std::cout << "The total number of events is " << nentries << std::endl;
    bool manual = false;
    
    int array_index = 0;
    int counter = 0;
    
    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        
        nominal->GetEntry(i);
        updateProgressBar(i+1, nentries);

        njets = nominal_jet_pt->size();
        dl1_b_tagged_jets = 0;
        gn_b_tagged_jets = 0;
        
        // SELECT EVENTS WITH TWO DIFFERENT SOLUTIONS
        if (klfitter_logLikelihood->size()==2) {
            if ((*klfitter_logLikelihood)[0] != (*klfitter_logLikelihood)[1]) {
                counter += 1;
            
                // CHECK B-TAGGING FREQUENCY IN THE EVENTS
                for (int j{0}; j<njets; j++) {
                    if ((*jet_isbtagged_DL1dv01_60)[j]) { dl1_b_tagged_jets += 1; }
                    if ((*jet_isbtagged_GN120220509_60)[j]) { gn_b_tagged_jets += 1; }
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
                
                // CHECK NUMBER OF JETS IN THE EVENT
                if (njets==4) { njet_dist[0] += 1; }
                else if (njets==5) { njet_dist[1] += 1; }
                else if (njets==6) { njet_dist[2] += 1; }
                else if (njets==7) { njet_dist[3] += 1; }
                else { njet_dist[4] += 1; }
                
                // CHECK LEPTON FLAVOUR IN THE EVENT
                if (el_pt->size()==1  && mu_pt->size()==0) { nelectrons += 1; }
                else if (el_pt->size()==0  && mu_pt->size()==1) { nmuons += 1; }
                else { std::cout << "!!! CLASH CLASH CLASH !!!" << std::endl; }
                
            }
        }
          
    }
    
    // PRINT THE RESULTS OBTAINED
    std::cout << std::endl << "The number of events with two different solutions is: " << counter << std::endl << std::endl;
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
    
    std::cout << std::endl << "LEPTON FLAVOUR FREQUENCY:" << std::endl;
    std::cout << "Electrons: " << nelectrons << std::endl;
    std::cout << "Muons:     " << nmuons << std::endl;
    std::cout << std::endl;
    
    std::cout << std::endl << "NUMBER OF JETS DISTRIBUTION:" << std::endl;
    std::cout << "4 jets:  " << njet_dist[0] << std::endl;
    std::cout << "5 jets:  " << njet_dist[1] << std::endl;
    std::cout << "6 jets:  " << njet_dist[2] << std::endl;
    std::cout << "7 jets:  " << njet_dist[3] << std::endl;
    std::cout << "8+ jets: " << njet_dist[4] << std::endl;
    

    return 0;
}
