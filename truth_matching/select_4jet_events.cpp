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
    char* file_path= "/eos/user/m/moriolpe/mphys_project/matched_calib_ttbar.root";
	TFile *input_file = new TFile(file_path, "READ");
	TTree *matched = (TTree*)input_file->Get("matched");

    /* OUTPUT FILE AND TREE */
    TFile* output_file = new TFile("/eos/user/m/moriolpe/mphys_project/matched_exactly4jets_calib_ttbar.root", "RECREATE");
    TTree* output_tree = new TTree("matched", "matched");


    /* CREATE ALL NOMINAL BRANCHES */
    std::vector<float>* mc_generator_weights = 0;
    Float_t weight;
    Float_t weight_pileup;
    Float_t weight_beamspot;
    Float_t weight_leptonSF;
    Float_t weight_trigger;
    Float_t weight_bTagSF_DL1dv01_60;
    Float_t weight_bTagSF_GN120220509_60;
    Float_t weight_jvt;
    Float_t weight_pileup_UP;
    Float_t weight_pileup_DOWN;
    Float_t weight_jvt_UP;
    Float_t weight_jvt_DOWN;
    std::vector<float>* weight_bTagSF_DL1dv01_60_eigenvars_B_up = 0;
    std::vector<float>* weight_bTagSF_DL1dv01_60_eigenvars_C_up = 0;
    std::vector<float>* weight_bTagSF_DL1dv01_60_eigenvars_Light_up = 0;
    std::vector<float>* weight_bTagSF_DL1dv01_60_eigenvars_B_down = 0;
    std::vector<float>* weight_bTagSF_DL1dv01_60_eigenvars_C_down = 0;
    std::vector<float>* weight_bTagSF_DL1dv01_60_eigenvars_Light_down = 0;
    Float_t weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up;
    Float_t weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down;
    std::vector<float>* weight_bTagSF_GN120220509_60_eigenvars_B_up = 0;
    std::vector<float>* weight_bTagSF_GN120220509_60_eigenvars_C_up = 0;
    std::vector<float>* weight_bTagSF_GN120220509_60_eigenvars_Light_up = 0;
    std::vector<float>* weight_bTagSF_GN120220509_60_eigenvars_B_down = 0;
    std::vector<float>* weight_bTagSF_GN120220509_60_eigenvars_C_down = 0;
    std::vector<float>* weight_bTagSF_GN120220509_60_eigenvars_Light_down = 0;
    Float_t weight_bTagSF_GN120220509_60_extrapolation_from_charm_up;
    Float_t weight_bTagSF_GN120220509_60_extrapolation_from_charm_down;
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
    std::vector<char>* jet_isbtagged_DL1dv01_60 = 0;  
    std::vector<char>* jet_isbtagged_GN120220509_60 = 0;  
    std::vector<float>* jet_GN120220509 = 0;
    std::vector<float>* jet_GN120220509_pb = 0;
    std::vector<float>* jet_GN120220509_pc = 0;
    std::vector<float>* jet_GN120220509_pu = 0;
    std::vector<float>* jet_DL1dv01 = 0;
    std::vector<float>* jet_DL1dv01_pb = 0;
    std::vector<float>* jet_DL1dv01_pc = 0;
    std::vector<float>* jet_DL1dv01_pu = 0;
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
    Int_t ejets_2022_dl1d;
    Int_t ejets_2022_gn_lowPt;
    Int_t ejets_2022_dl1d_lowPt;
    Int_t new_ejets_gn_2022;
    Int_t new_ejets_dl1d_2022;
    Int_t mujets_gn_2022;
    Int_t mujets_dl1d_2022;
    Int_t mujets_2022_gn_lowPt;
    Int_t mujets_2022_dl1d_lowPt;
    Int_t new_mujets_gn_2022;
    Int_t new_mujets_dl1d_2022;
    std::vector<double>* jet_SV_mass = 0;
    std::vector<int> jet_match_info;
	Int_t njets;

    matched->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    matched->SetBranchAddress("weight_mc", &weight);
    matched->SetBranchAddress("weight_pileup", &weight_pileup);
    matched->SetBranchAddress("weight_beamspot", &weight_beamspot);
    matched->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    matched->SetBranchAddress("weight_trigger", &weight_trigger);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    matched->SetBranchAddress("weight_jvt", &weight_jvt);
    matched->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    matched->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    matched->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    matched->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    matched->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    matched->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    matched->SetBranchAddress("weight_poisson", &weight_poisson);
    matched->SetBranchAddress("eventNumber", &eventNumber);
    matched->SetBranchAddress("runNumber", &runNumber);
    matched->SetBranchAddress("randomRunNumber", &randomRunNumber);
    matched->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    matched->SetBranchAddress("mu", &mu);
    matched->SetBranchAddress("mu_actual", &mu_actual);
    matched->SetBranchAddress("el_pt", &nominal_el_pt);
    matched->SetBranchAddress("el_eta", &nominal_el_eta);
    matched->SetBranchAddress("el_phi", &nominal_el_phi);
    matched->SetBranchAddress("el_charge", &nominal_el_charge);
    matched->SetBranchAddress("mu_pt", &nominal_mu_pt);
    matched->SetBranchAddress("mu_eta", &nominal_mu_eta);
    matched->SetBranchAddress("mu_phi", &nominal_mu_phi);
    matched->SetBranchAddress("mu_charge", &nominal_mu_charge);
    matched->SetBranchAddress("jet_pt", &nominal_jet_pt);
    matched->SetBranchAddress("jet_eta", &nominal_jet_eta);
    matched->SetBranchAddress("jet_phi", &nominal_jet_phi);
    matched->SetBranchAddress("jet_e", &nominal_jet_e); 
    matched->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    matched->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    matched->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    matched->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    matched->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    matched->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    matched->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    matched->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    matched->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    matched->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    matched->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    matched->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    matched->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    matched->SetBranchAddress("met_met", &met_met);
    matched->SetBranchAddress("met_sumet", &met_sumet);
    matched->SetBranchAddress("met_phi", &met_phi);
    matched->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    matched->SetBranchAddress("klfitter_selected", &klfitter_selected);
    matched->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    matched->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    matched->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    matched->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    matched->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    matched->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    matched->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    matched->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    matched->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    matched->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    matched->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    matched->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    matched->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    matched->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    matched->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    matched->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    matched->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    matched->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
    matched->Branch("jet_truthMatched", &jet_match_info);
	matched->Branch("njets", &njets);
    
    /* CREATE ALL BRANCHES IN THE OUTPUT TREE */
    output_tree->Branch("mc_generator_weights", &mc_generator_weights);
    output_tree->Branch("weight_mc", &weight);
    output_tree->Branch("weight_pileup", &weight_pileup);
    output_tree->Branch("weight_beamspot", &weight_beamspot);
    output_tree->Branch("weight_leptonSF", &weight_leptonSF);
    output_tree->Branch("weight_trigger", &weight_trigger);
    output_tree->Branch("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    output_tree->Branch("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    output_tree->Branch("weight_jvt", &weight_jvt);
    output_tree->Branch("weight_pileup_UP", &weight_pileup_UP);
    output_tree->Branch("weight_pileup_DOWN", &weight_pileup_DOWN);
    output_tree->Branch("weight_jvt_UP", &weight_jvt_UP);
    output_tree->Branch("weight_jvt_DOWN", &weight_jvt_DOWN);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    output_tree->Branch("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    output_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    output_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    output_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    output_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    output_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    output_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    output_tree->Branch("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    output_tree->Branch("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    output_tree->Branch("weight_poisson", &weight_poisson);
    output_tree->Branch("eventNumber", &eventNumber);
    output_tree->Branch("runNumber", &runNumber);
    output_tree->Branch("randomRunNumber", &randomRunNumber);
    output_tree->Branch("mcChannelNumber", &mcChannelNumber);
    output_tree->Branch("mu", &mu);
    output_tree->Branch("mu_actual", &mu_actual);
    output_tree->Branch("el_pt", &nominal_el_pt);
    output_tree->Branch("el_eta", &nominal_el_eta);
    output_tree->Branch("el_phi", &nominal_el_phi);
    output_tree->Branch("el_charge", &nominal_el_charge);
    output_tree->Branch("mu_pt", &nominal_mu_pt);
    output_tree->Branch("mu_eta", &nominal_mu_eta);
    output_tree->Branch("mu_phi", &nominal_mu_phi);
    output_tree->Branch("mu_charge", &nominal_mu_charge);
    output_tree->Branch("jet_pt", &nominal_jet_pt);
    output_tree->Branch("jet_eta", &nominal_jet_eta);
    output_tree->Branch("jet_phi", &nominal_jet_phi);
    output_tree->Branch("jet_e", &nominal_jet_e); 
    output_tree->Branch("jet_truthflav", &jet_truthflav);   
    output_tree->Branch("jet_truthPartonLabel", &jet_truthPartonLabel);   
    output_tree->Branch("jet_truthflavExtended", &jet_truthflavExtended); 
    output_tree->Branch("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    output_tree->Branch("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    output_tree->Branch("jet_GN120220509", &jet_GN120220509);
    output_tree->Branch("jet_GN120220509_pb", &jet_GN120220509_pb);
    output_tree->Branch("jet_GN120220509_pc", &jet_GN120220509_pc);
    output_tree->Branch("jet_GN120220509_pu", &jet_GN120220509_pu);
    output_tree->Branch("jet_DL1dv01", &jet_DL1dv01);
    output_tree->Branch("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    output_tree->Branch("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    output_tree->Branch("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    output_tree->Branch("met_met", &met_met);
    output_tree->Branch("met_sumet", &met_sumet);
    output_tree->Branch("met_phi", &met_phi);
    output_tree->Branch("klfitter_logLikelihood", &klfitter_logLikelihood);
    output_tree->Branch("klfitter_selected", &klfitter_selected);
    output_tree->Branch("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    output_tree->Branch("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    output_tree->Branch("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    output_tree->Branch("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    output_tree->Branch("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    output_tree->Branch("ejets_2022_gn", &ejets_2022_gn);
    output_tree->Branch("ejets_2022_dl1d", &ejets_2022_dl1d);
    output_tree->Branch("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    output_tree->Branch("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    output_tree->Branch("new_ejets_gn_2022", &new_ejets_gn_2022);
    output_tree->Branch("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    output_tree->Branch("mujets_gn_2022", &mujets_gn_2022);
    output_tree->Branch("mujets_dl1d_2022", &mujets_dl1d_2022);
    output_tree->Branch("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    output_tree->Branch("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    output_tree->Branch("new_mujets_gn_2022", &new_mujets_gn_2022);
    output_tree->Branch("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    output_tree->Branch("jet_SV_mass", &jet_SV_mass);
    output_tree->Branch("jet_truthMatched", &jet_match_info);
	output_tree->Branch("njets", &njets);

    Long64_t nentries = matched->GetEntries();
    std::cout << "The total number of events is " << nentries << std::endl;

    for (Long64_t i{0}; i<nentries; i++) {
        matched->GetEntry(i);
        if (nominal_jet_pt->size() == 4) {
            njets = nominal_jet_pt->size();
			output_tree->Fill();
        }
        updateProgressBar(i+1, nentries);
    }

    std::cout << "DONE" << std::endl;
    output_file->cd();
    output_tree->Write();
    output_file->Close();
    delete output_file;

    return 0;
}
