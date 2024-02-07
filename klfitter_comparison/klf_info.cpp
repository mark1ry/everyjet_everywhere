
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
    
    Int_t mujets_dl1d_2022;
    Int_t mujets_2022_dl1d_lowPt;
    Int_t ejets_dl1d_2022;
    Int_t ejets_2022_dl1d_lowPt;
    
    nominal->SetBranchAddress("weight_mc", &weight);
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt); 
    nominal->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);
    nominal->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);
    nominal->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);

    nominal->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal->SetBranchAddress("ejets_2022_dl1d", &ejets_dl1d_2022);
    nominal->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);            

    // Getvector of jets
    // Create vector of Lorentz objects
    // Create vector of matched labels
    // Match jets to each truth object
    // Set unmatched jets to unmatched value
    Long64_t nentries = nominal->GetEntries();
    int njets = 0;
    int b_tagged_jets = 0;
    std::vector<int> jets_freq = {0, 0, 0, 0, 0};
    std::vector<int> solution_frequency_noGN = {0, 0, 0};
    
    std::cout << "The total number of events is " << nentries << std::endl;
    bool manual = false;
    
    int counter = 0;
    int value;
    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        
        nominal->GetEntry(i);
        updateProgressBar(i+1, nentries);
        
        if (mujets_dl1d_2022 || mujets_2022_dl1d_lowPt || ejets_dl1d_2022 || ejets_2022_dl1d_lowPt) {
            njets = nominal_jet_pt->size();
            if (njets == 4) { jets_freq[0] += 1;}
            if (njets == 5) { jets_freq[1] += 1;}
            if (njets == 6) { jets_freq[2] += 1;}
            if (njets == 7) { jets_freq[3] += 1;}
            else { jets_freq[4] += 1; }
            
            
            /*
            if (klfitter_logLikelihood->size()!=0) {
                for (int j{0}; j<njets; j++) {
                    if ((*jet_isbtagged_DL1dv01_60)[j]) {
                        value = 1;
                    } else { value = 0;}
                    std::cout << (*nominal_jet_pt)[j] << " (" << value << ")  ";
                }
                std::cout << std::endl;
                counter += 1;
            */
        }
    }
    std::cout << std::endl;
    for( int j{0}; j<5; j++ ) {
        std::cout << jets_freq[j] << std::endl;
    }
    
    return 0;
}
