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
    TFile *input_file = new TFile("/eos/user/m/moriolpe/mphys_project/root_files/matched_dl1dselected_4jets.root", "READ");
    TTree *input_tree = (TTree*)input_file->Get("matched");

    TFile* output_file_1 = new TFile("/eos/user/m/moriolpe/mphys_project/matched_dl1d_4jets_even.root", "RECREATE");
    TFile* output_file_2 = new TFile("/eos/user/m/moriolpe/mphys_project/matched_dl1d_4jets_odd.root", "RECREATE");
    TTree* output_tree_1 = new TTree("matched", "matched");
    TTree* output_tree_2 = new TTree("matched", "matched");
    
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
    std::vector<int>* jet_truthMatched = 0;
    Int_t njets;

    input_tree->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    input_tree->SetBranchAddress("weight_mc", &weight);
    input_tree->SetBranchAddress("weight_pileup", &weight_pileup);
    input_tree->SetBranchAddress("weight_beamspot", &weight_beamspot);
    input_tree->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    input_tree->SetBranchAddress("weight_trigger", &weight_trigger);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    input_tree->SetBranchAddress("weight_jvt", &weight_jvt);
    input_tree->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    input_tree->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    input_tree->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    input_tree->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    input_tree->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    input_tree->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
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
    input_tree->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    input_tree->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    input_tree->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    input_tree->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    input_tree->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    input_tree->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    input_tree->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    input_tree->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    input_tree->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    input_tree->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
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
    input_tree->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    input_tree->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    input_tree->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    input_tree->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    input_tree->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    input_tree->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    input_tree->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    input_tree->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    input_tree->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    input_tree->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    input_tree->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    input_tree->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
    input_tree->SetBranchAddress("jet_truthMatched", &jet_truthMatched);
    input_tree->SetBranchAddress("njets", &njets);

    output_tree_1->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    output_tree_1->SetBranchAddress("weight_mc", &weight);
    output_tree_1->SetBranchAddress("weight_pileup", &weight_pileup);
    output_tree_1->SetBranchAddress("weight_beamspot", &weight_beamspot);
    output_tree_1->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    output_tree_1->SetBranchAddress("weight_trigger", &weight_trigger);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    output_tree_1->SetBranchAddress("weight_jvt", &weight_jvt);
    output_tree_1->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    output_tree_1->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    output_tree_1->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    output_tree_1->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    output_tree_1->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    output_tree_1->SetBranchAddress("weight_poisson", &weight_poisson);
    output_tree_1->SetBranchAddress("eventNumber", &eventNumber);
    output_tree_1->SetBranchAddress("runNumber", &runNumber);
    output_tree_1->SetBranchAddress("randomRunNumber", &randomRunNumber);
    output_tree_1->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    output_tree_1->SetBranchAddress("mu", &mu);
    output_tree_1->SetBranchAddress("mu_actual", &mu_actual);
    output_tree_1->SetBranchAddress("el_pt", &nominal_el_pt);
    output_tree_1->SetBranchAddress("el_eta", &nominal_el_eta);
    output_tree_1->SetBranchAddress("el_phi", &nominal_el_phi);
    output_tree_1->SetBranchAddress("el_charge", &nominal_el_charge);
    output_tree_1->SetBranchAddress("mu_pt", &nominal_mu_pt);
    output_tree_1->SetBranchAddress("mu_eta", &nominal_mu_eta);
    output_tree_1->SetBranchAddress("mu_phi", &nominal_mu_phi);
    output_tree_1->SetBranchAddress("mu_charge", &nominal_mu_charge);
    output_tree_1->SetBranchAddress("jet_pt", &nominal_jet_pt);
    output_tree_1->SetBranchAddress("jet_eta", &nominal_jet_eta);
    output_tree_1->SetBranchAddress("jet_phi", &nominal_jet_phi);
    output_tree_1->SetBranchAddress("jet_e", &nominal_jet_e); 
    output_tree_1->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    output_tree_1->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    output_tree_1->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    output_tree_1->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    output_tree_1->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    output_tree_1->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    output_tree_1->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    output_tree_1->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    output_tree_1->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    output_tree_1->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    output_tree_1->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    output_tree_1->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    output_tree_1->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    output_tree_1->SetBranchAddress("met_met", &met_met);
    output_tree_1->SetBranchAddress("met_sumet", &met_sumet);
    output_tree_1->SetBranchAddress("met_phi", &met_phi);
    output_tree_1->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    output_tree_1->SetBranchAddress("klfitter_selected", &klfitter_selected);
    output_tree_1->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    output_tree_1->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    output_tree_1->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    output_tree_1->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    output_tree_1->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    output_tree_1->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    output_tree_1->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    output_tree_1->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    output_tree_1->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    output_tree_1->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    output_tree_1->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    output_tree_1->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    output_tree_1->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    output_tree_1->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    output_tree_1->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    output_tree_1->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    output_tree_1->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    output_tree_1->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
    output_tree_1->SetBranchAddress("jet_truthMatched", &jet_truthMatched);
    output_tree_1->SetBranchAddress("njets", &njets);
    
    output_tree_2->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    output_tree_2->SetBranchAddress("weight_mc", &weight);
    output_tree_2->SetBranchAddress("weight_pileup", &weight_pileup);
    output_tree_2->SetBranchAddress("weight_beamspot", &weight_beamspot);
    output_tree_2->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    output_tree_2->SetBranchAddress("weight_trigger", &weight_trigger);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    output_tree_2->SetBranchAddress("weight_jvt", &weight_jvt);
    output_tree_2->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    output_tree_2->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    output_tree_2->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    output_tree_2->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    output_tree_2->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    output_tree_2->SetBranchAddress("weight_poisson", &weight_poisson);
    output_tree_2->SetBranchAddress("eventNumber", &eventNumber);
    output_tree_2->SetBranchAddress("runNumber", &runNumber);
    output_tree_2->SetBranchAddress("randomRunNumber", &randomRunNumber);
    output_tree_2->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    output_tree_2->SetBranchAddress("mu", &mu);
    output_tree_2->SetBranchAddress("mu_actual", &mu_actual);
    output_tree_2->SetBranchAddress("el_pt", &nominal_el_pt);
    output_tree_2->SetBranchAddress("el_eta", &nominal_el_eta);
    output_tree_2->SetBranchAddress("el_phi", &nominal_el_phi);
    output_tree_2->SetBranchAddress("el_charge", &nominal_el_charge);
    output_tree_2->SetBranchAddress("mu_pt", &nominal_mu_pt);
    output_tree_2->SetBranchAddress("mu_eta", &nominal_mu_eta);
    output_tree_2->SetBranchAddress("mu_phi", &nominal_mu_phi);
    output_tree_2->SetBranchAddress("mu_charge", &nominal_mu_charge);
    output_tree_2->SetBranchAddress("jet_pt", &nominal_jet_pt);
    output_tree_2->SetBranchAddress("jet_eta", &nominal_jet_eta);
    output_tree_2->SetBranchAddress("jet_phi", &nominal_jet_phi);
    output_tree_2->SetBranchAddress("jet_e", &nominal_jet_e); 
    output_tree_2->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    output_tree_2->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    output_tree_2->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    output_tree_2->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    output_tree_2->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    output_tree_2->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    output_tree_2->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    output_tree_2->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    output_tree_2->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    output_tree_2->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    output_tree_2->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    output_tree_2->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    output_tree_2->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    output_tree_2->SetBranchAddress("met_met", &met_met);
    output_tree_2->SetBranchAddress("met_sumet", &met_sumet);
    output_tree_2->SetBranchAddress("met_phi", &met_phi);
    output_tree_2->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    output_tree_2->SetBranchAddress("klfitter_selected", &klfitter_selected);
    output_tree_2->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    output_tree_2->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    output_tree_2->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    output_tree_2->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    output_tree_2->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    output_tree_2->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    output_tree_2->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    output_tree_2->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    output_tree_2->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    output_tree_2->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    output_tree_2->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    output_tree_2->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    output_tree_2->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    output_tree_2->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    output_tree_2->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    output_tree_2->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    output_tree_2->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    output_tree_2->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
    output_tree_2->SetBranchAddress("jet_truthMatched", &jet_truthMatched);
    output_tree_2->SetBranchAddress("njets", &njets);
    
    Long64_t nentries = matched->GetEntries();
    for (Long64_t i=0; i<nentries; i++) {
        matched->GetEntry(i);
        updateProgressBar(i+1, nentries);

        if (randomRunNumber%2 == 0) output_tree_1->Fill();
        else output_tree_2->Fill();
    }

    output_file_1->cd();
    output_tree_1->Write();
    output_file_1->Close();
    delete output_file_1;

    output_file_2->cd();
    output_tree_2->Write();
    output_file_2->Close();
    delete output_file_2;

    return 0;
}