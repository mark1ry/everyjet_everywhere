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
    TFile *input_file = new TFile("/eos/user/m/moriolpe/mphys_project/root_files/final_files/matched_gn_pythia_4jets.root", "READ");
    TTree *input_tree = (TTree*)input_file->Get("matched");
    
	std::vector<float>* mc_generator_weights = 0;
    Float_t weight;
    Float_t weight_pileup;
    Float_t weight_beamspot;
    Float_t weight_leptonSF;
    Float_t weight_trigger;
    Float_t weight_jvt;
    Float_t weight_pileup_UP;
    Float_t weight_pileup_DOWN;
    Float_t weight_jvt_UP;
    Float_t weight_jvt_DOWN;
    std::vector<int>* weight_poisson = 0;
    ULong64_t eventNumber;
    UInt_t runNumber;
    UInt_t randomRunNumber;
    UInt_t mcChannelNumber;
    Float_t mu;
    Float_t mu_actual;

    std::vector<float>* nominal_el_pt = 0;
    std::vector<float>* nominal_el_eta = 0;
    std::vector<float>* nominal_el_phi = 0;
    std::vector<float>* nominal_el_charge = 0;
    std::vector<float>* nominal_mu_pt = 0;
    std::vector<float>* nominal_mu_eta = 0;
    std::vector<float>* nominal_mu_phi = 0;
    std::vector<float>* nominal_mu_charge = 0;
    std::vector<float>* nominal_jet_pt = 0;
    std::vector<float>* nominal_jet_phi = 0;
    std::vector<float>* nominal_jet_eta = 0;
    std::vector<float>* nominal_jet_e = 0; 
    
    std::vector<int>* jet_truthflav = 0;
    std::vector<int>* jet_truthPartonLabel = 0;
    std::vector<int>* jet_truthflavExtended = 0;
    std::vector<char>* jet_isbtagged_GN2v01_65 = 0;
    std::vector<char>* jet_isbtagged_GN2v01_70 = 0;
    std::vector<float>* jet_GN2v01 = 0;
    std::vector<float>* jet_GN2v01_pb = 0;
    std::vector<float>* jet_GN2v01_pc = 0;
    std::vector<float>* jet_GN2v01_pu = 0;
    std::vector<float>* jet_GN2v01_ptau = 0;
	Float_t met_met;
    Float_t met_sumet;
    Float_t met_phi;
    std::vector<float>* klfitter_logLikelihood = 0;
    Short_t klfitter_selected;
    std::vector<short>* klfitter_minuitDidNotConverge = 0;
    std::vector<unsigned int>* klfitter_model_bhad_jetIndex = 0;
    std::vector<unsigned int>* klfitter_model_blep_jetIndex = 0;
    std::vector<unsigned int>* klfitter_model_lq1_jetIndex = 0;
    std::vector<unsigned int>* klfitter_model_lq2_jetIndex = 0;
    Int_t ejets_2022_gn;
    Int_t ejets_2022_gn_lowPt;
    Int_t new_ejets_gn_2022;
    Int_t mujets_gn_2022;
    Int_t mujets_2022_gn_lowPt;
    Int_t new_mujets_gn_2022;
    std::vector<double>* jet_SV_mass = 0;
    std::vector<int>* jet_truthMatched = 0;
    Int_t njets;

    input_tree->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    input_tree->SetBranchAddress("weight_mc", &weight);
    input_tree->SetBranchAddress("weight_pileup", &weight_pileup);
    input_tree->SetBranchAddress("weight_beamspot", &weight_beamspot);
    input_tree->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    input_tree->SetBranchAddress("weight_trigger", &weight_trigger);
    input_tree->SetBranchAddress("weight_jvt", &weight_jvt);
    input_tree->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    input_tree->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    input_tree->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    input_tree->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    input_tree->SetBranchAddress("weight_poisson", &weight_poisson);
    input_tree->SetBranchAddress("eventNumber", &eventNumber);
    input_tree->SetBranchAddress("runNumber", &runNumber);
    input_tree->SetBranchAddress("randomRunNumber", &randomRunNumber);
    input_tree->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    input_tree->SetBranchAddress("mu", &mu);
    input_tree->SetBranchAddress("mu_actual", &mu_actual);
    input_tree->SetBranchAddress("el_pt", &nominal_el_pt);
    input_tree->SetBranchAddress("el_eta", &nominal_el_eta);
    input_tree->SetBranchAddress("el_phi", &nominal_el_phi);
    input_tree->SetBranchAddress("el_charge", &nominal_el_charge);
    input_tree->SetBranchAddress("mu_pt", &nominal_mu_pt);
    input_tree->SetBranchAddress("mu_eta", &nominal_mu_eta);
    input_tree->SetBranchAddress("mu_phi", &nominal_mu_phi);
    input_tree->SetBranchAddress("mu_charge", &nominal_mu_charge);
    input_tree->SetBranchAddress("jet_pt", &nominal_jet_pt);
    input_tree->SetBranchAddress("jet_eta", &nominal_jet_eta);
    input_tree->SetBranchAddress("jet_phi", &nominal_jet_phi);
    input_tree->SetBranchAddress("jet_e", &nominal_jet_e); 
    input_tree->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    input_tree->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    input_tree->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    input_tree->SetBranchAddress("jet_isbtagged_GN2v01_65", &jet_isbtagged_GN2v01_65);  
    input_tree->SetBranchAddress("jet_isbtagged_GN2v01_70", &jet_isbtagged_GN2v01_70);  
    input_tree->SetBranchAddress("jet_GN2v01", &jet_GN2v01);
    input_tree->SetBranchAddress("jet_GN2v01_pb", &jet_GN2v01_pb);
    input_tree->SetBranchAddress("jet_GN2v01_pc", &jet_GN2v01_pc);
    input_tree->SetBranchAddress("jet_GN2v01_pu", &jet_GN2v01_pu);
    input_tree->SetBranchAddress("jet_GN2v01_pu", &jet_GN2v01_ptau);
	input_tree->SetBranchAddress("met_met", &met_met);
    input_tree->SetBranchAddress("met_sumet", &met_sumet);
    input_tree->SetBranchAddress("met_phi", &met_phi);
    input_tree->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    input_tree->SetBranchAddress("klfitter_selected", &klfitter_selected);
    input_tree->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    input_tree->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    input_tree->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    input_tree->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    input_tree->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    input_tree->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    input_tree->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    input_tree->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    input_tree->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    input_tree->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    input_tree->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    input_tree->SetBranchAddress("jet_SV_mass", &jet_SV_mass);   
    input_tree->SetBranchAddress("jet_truthMatched", &jet_truthMatched);
	input_tree->SetBranchAddress("njets", &njets);

    Long64_t nentries = input_tree->GetEntries();
    std::cout << std::endl << "THE TOTAL NUMBER OF ENTRIES IS " << nentries << std::endl;
    int is_odd;
    
    TFile* output_file_1 = new TFile("/eos/user/m/moriolpe/mphys_project/root_files/final_files/matched_gn_pythia_4jets_even.root", "RECREATE");
    TTree* output_tree_1 = new TTree("matched_even", "matched_even");
    
    output_tree_1->Branch("mc_generator_weights", &mc_generator_weights);
    output_tree_1->Branch("weight_mc", &weight);
    output_tree_1->Branch("weight_pileup", &weight_pileup);
    output_tree_1->Branch("weight_beamspot", &weight_beamspot);
    output_tree_1->Branch("weight_leptonSF", &weight_leptonSF);
    output_tree_1->Branch("weight_trigger", &weight_trigger);
    output_tree_1->Branch("weight_jvt", &weight_jvt);
    output_tree_1->Branch("weight_pileup_UP", &weight_pileup_UP);
    output_tree_1->Branch("weight_pileup_DOWN", &weight_pileup_DOWN);
    output_tree_1->Branch("weight_jvt_UP", &weight_jvt_UP);
    output_tree_1->Branch("weight_jvt_DOWN", &weight_jvt_DOWN);
    output_tree_1->Branch("weight_poisson", &weight_poisson);
    output_tree_1->Branch("eventNumber", &eventNumber);
    output_tree_1->Branch("runNumber", &runNumber);
    output_tree_1->Branch("randomRunNumber", &randomRunNumber);
    output_tree_1->Branch("mcChannelNumber", &mcChannelNumber);
    output_tree_1->Branch("mu", &mu);
    output_tree_1->Branch("mu_actual", &mu_actual);
    output_tree_1->Branch("el_pt", &nominal_el_pt);
    output_tree_1->Branch("el_eta", &nominal_el_eta);
    output_tree_1->Branch("el_phi", &nominal_el_phi);
    output_tree_1->Branch("el_charge", &nominal_el_charge);
    output_tree_1->Branch("mu_pt", &nominal_mu_pt);
    output_tree_1->Branch("mu_eta", &nominal_mu_eta);
    output_tree_1->Branch("mu_phi", &nominal_mu_phi);
    output_tree_1->Branch("mu_charge", &nominal_mu_charge);
    output_tree_1->Branch("jet_pt", &nominal_jet_pt);
    output_tree_1->Branch("jet_eta", &nominal_jet_eta);
    output_tree_1->Branch("jet_phi", &nominal_jet_phi);
    output_tree_1->Branch("jet_e", &nominal_jet_e); 
    output_tree_1->Branch("jet_truthflav", &jet_truthflav);   
    output_tree_1->Branch("jet_truthPartonLabel", &jet_truthPartonLabel);   
    output_tree_1->Branch("jet_truthflavExtended", &jet_truthflavExtended); 
    output_tree_1->Branch("jet_isbtagged_GN2v01_65", &jet_isbtagged_GN2v01_65);  
    output_tree_1->Branch("jet_isbtagged_GN2v01_70", &jet_isbtagged_GN2v01_70);  
    output_tree_1->Branch("jet_GN2v01", &jet_GN2v01);
    output_tree_1->Branch("jet_GN2v01_pb", &jet_GN2v01_pb);
    output_tree_1->Branch("jet_GN2v01_pc", &jet_GN2v01_pc);
    output_tree_1->Branch("jet_GN2v01_pu", &jet_GN2v01_pu);
    output_tree_1->Branch("jet_GN2v01_pu", &jet_GN2v01_ptau);
	output_tree_1->Branch("met_met", &met_met);
    output_tree_1->Branch("met_sumet", &met_sumet);
    output_tree_1->Branch("met_phi", &met_phi);
    output_tree_1->Branch("klfitter_logLikelihood", &klfitter_logLikelihood);
    output_tree_1->Branch("klfitter_selected", &klfitter_selected);
    output_tree_1->Branch("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    output_tree_1->Branch("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    output_tree_1->Branch("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    output_tree_1->Branch("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    output_tree_1->Branch("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    output_tree_1->Branch("ejets_2022_gn", &ejets_2022_gn);
    output_tree_1->Branch("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    output_tree_1->Branch("new_ejets_gn_2022", &new_ejets_gn_2022);
    output_tree_1->Branch("mujets_gn_2022", &mujets_gn_2022);
    output_tree_1->Branch("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    output_tree_1->Branch("new_mujets_gn_2022", &new_mujets_gn_2022);
    output_tree_1->Branch("jet_SV_mass", &jet_SV_mass);   
    output_tree_1->Branch("jet_truthMatched", &jet_truthMatched);
	output_tree_1->Branch("njets", &njets);
   
    for (Long64_t i=0; i<nentries; i++) {
        input_tree->GetEntry(i);
        updateProgressBar(i+1, nentries);
        
        is_odd = eventNumber%2;
        if (is_odd!=1) {
            output_tree_1->Fill();
        }
    }

    output_file_1->cd();
    output_tree_1->Write();
    output_file_1->Close();
    delete output_file_1;
    
    TFile* output_file_2 = new TFile("/eos/user/m/moriolpe/mphys_project/root_files/final_files/matched_gn_pythia_4jets_odd.root", "RECREATE");
    TTree* output_tree_2 = new TTree("matched_odd", "matched_odd");
    
    output_tree_2->Branch("mc_generator_weights", &mc_generator_weights);
    output_tree_2->Branch("weight_mc", &weight);
    output_tree_2->Branch("weight_pileup", &weight_pileup);
    output_tree_2->Branch("weight_beamspot", &weight_beamspot);
    output_tree_2->Branch("weight_leptonSF", &weight_leptonSF);
    output_tree_2->Branch("weight_trigger", &weight_trigger);
    output_tree_2->Branch("weight_jvt", &weight_jvt);
    output_tree_2->Branch("weight_pileup_UP", &weight_pileup_UP);
    output_tree_2->Branch("weight_pileup_DOWN", &weight_pileup_DOWN);
    output_tree_2->Branch("weight_jvt_UP", &weight_jvt_UP);
    output_tree_2->Branch("weight_jvt_DOWN", &weight_jvt_DOWN);
    output_tree_2->Branch("weight_poisson", &weight_poisson);
    output_tree_2->Branch("eventNumber", &eventNumber);
    output_tree_2->Branch("runNumber", &runNumber);
    output_tree_2->Branch("randomRunNumber", &randomRunNumber);
    output_tree_2->Branch("mcChannelNumber", &mcChannelNumber);
    output_tree_2->Branch("mu", &mu);
    output_tree_2->Branch("mu_actual", &mu_actual);
    output_tree_2->Branch("el_pt", &nominal_el_pt);
    output_tree_2->Branch("el_eta", &nominal_el_eta);
    output_tree_2->Branch("el_phi", &nominal_el_phi);
    output_tree_2->Branch("el_charge", &nominal_el_charge);
    output_tree_2->Branch("mu_pt", &nominal_mu_pt);
    output_tree_2->Branch("mu_eta", &nominal_mu_eta);
    output_tree_2->Branch("mu_phi", &nominal_mu_phi);
    output_tree_2->Branch("mu_charge", &nominal_mu_charge);
    output_tree_2->Branch("jet_pt", &nominal_jet_pt);
    output_tree_2->Branch("jet_eta", &nominal_jet_eta);
    output_tree_2->Branch("jet_phi", &nominal_jet_phi);
    output_tree_2->Branch("jet_e", &nominal_jet_e); 
    output_tree_2->Branch("jet_truthflav", &jet_truthflav);   
    output_tree_2->Branch("jet_truthPartonLabel", &jet_truthPartonLabel);   
    output_tree_2->Branch("jet_truthflavExtended", &jet_truthflavExtended); 
    output_tree_2->Branch("jet_isbtagged_GN2v01_65", &jet_isbtagged_GN2v01_65);  
    output_tree_2->Branch("jet_isbtagged_GN2v01_70", &jet_isbtagged_GN2v01_70);  
    output_tree_2->Branch("jet_GN2v01", &jet_GN2v01);
    output_tree_2->Branch("jet_GN2v01_pb", &jet_GN2v01_pb);
    output_tree_2->Branch("jet_GN2v01_pc", &jet_GN2v01_pc);
    output_tree_2->Branch("jet_GN2v01_pu", &jet_GN2v01_pu);
    output_tree_2->Branch("jet_GN2v01_pu", &jet_GN2v01_ptau);
	output_tree_2->Branch("met_met", &met_met);
    output_tree_2->Branch("met_sumet", &met_sumet);
    output_tree_2->Branch("met_phi", &met_phi);
    output_tree_2->Branch("klfitter_logLikelihood", &klfitter_logLikelihood);
    output_tree_2->Branch("klfitter_selected", &klfitter_selected);
    output_tree_2->Branch("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    output_tree_2->Branch("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    output_tree_2->Branch("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    output_tree_2->Branch("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    output_tree_2->Branch("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    output_tree_2->Branch("ejets_2022_gn", &ejets_2022_gn);
    output_tree_2->Branch("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    output_tree_2->Branch("new_ejets_gn_2022", &new_ejets_gn_2022);
    output_tree_2->Branch("mujets_gn_2022", &mujets_gn_2022);
    output_tree_2->Branch("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    output_tree_2->Branch("new_mujets_gn_2022", &new_mujets_gn_2022);
    output_tree_2->Branch("jet_SV_mass", &jet_SV_mass);   
    output_tree_2->Branch("jet_truthMatched", &jet_truthMatched);
	output_tree_2->Branch("njets", &njets);

    for (Long64_t i=0; i<nentries; i++) {
        input_tree->GetEntry(i);
        updateProgressBar(i+1, nentries);
        
        is_odd = eventNumber%2;
        if (is_odd==1) {
            output_tree_2->Fill();
        }
    }
    
    output_file_2->cd();
    output_tree_2->Write();
    output_file_2->Close();
    delete output_file_2;
    
    return 0;
}
