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

const float ELECTRON_MASS = 0.510999;
const float MUON_MASS = 105.658;

// Define truth attributes:
Int_t MC_W2_t_PID;
Float_t MC_W2_t_pt;
Float_t MC_W2_t_phi;
Float_t MC_W2_t_eta;
Float_t MC_W2_t_mass;

Int_t MC_W1_t_PID;
Float_t MC_W1_t_phi;
Float_t MC_W1_t_eta;
Float_t MC_W1_t_pt;
Float_t MC_W1_t_mass;

Int_t MC_W2_tbar_PID;
Float_t MC_W2_tbar_pt;
Float_t MC_W2_tbar_phi;
Float_t MC_W2_tbar_eta;
Float_t MC_W2_tbar_mass;

Int_t MC_W1_tbar_PID;
Float_t MC_W1_tbar_phi;
Float_t MC_W1_tbar_pt;
Float_t MC_W1_tbar_eta;
Float_t MC_W1_tbar_mass;

Float_t MC_b_t_phi;
Float_t MC_b_t_pt;
Float_t MC_b_t_eta;
Float_t MC_b_t_mass;

Float_t MC_b_tbar_phi;
Float_t MC_b_tbar_pt;
Float_t MC_b_tbar_mass;
Float_t MC_b_tbar_eta;

Float_t MC_weight;
UInt_t MC_runNumber;
ULong64_t MC_eventNumber;

int directly_matched=0;
int indirectly_matched=0;
int unmatchable=0;
int more_complex=0;

TTree * SetTruthTree(char* file_path) {
    TChain *chain = new TChain("truth", "");
    chain->Add(file_path);


    chain->BuildIndex("runNumber","eventNumber");

    chain->SetBranchAddress("MC_Wdecay1_from_t_pdgId",&MC_W1_t_PID);
    chain->SetBranchAddress("MC_Wdecay1_from_t_m",&MC_W1_t_mass);
    chain->SetBranchAddress("MC_Wdecay1_from_t_phi",&MC_W1_t_phi);
    chain->SetBranchAddress("MC_Wdecay1_from_t_eta",&MC_W1_t_eta);
    chain->SetBranchAddress("MC_Wdecay1_from_t_pt",&MC_W1_t_pt);
    chain->SetBranchAddress("MC_Wdecay2_from_t_pdgId",&MC_W2_t_PID);
    chain->SetBranchAddress("MC_Wdecay2_from_t_m",&MC_W2_t_mass);
    chain->SetBranchAddress("MC_Wdecay2_from_t_phi",&MC_W2_t_phi);
    chain->SetBranchAddress("MC_Wdecay2_from_t_eta",&MC_W2_t_eta);
    chain->SetBranchAddress("MC_Wdecay2_from_t_pt",&MC_W2_t_pt);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_pdgId",&MC_W1_tbar_PID);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_m",&MC_W1_tbar_mass);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_phi",&MC_W1_tbar_phi);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_eta",&MC_W1_tbar_eta);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_pt",&MC_W1_tbar_pt);
    chain->SetBranchAddress("MC_Wdecay2_from_tbar_pdgId",&MC_W2_tbar_PID);
    chain->SetBranchAddress("MC_Wdecay2_from_tbar_m",&MC_W2_tbar_mass);
    chain->SetBranchAddress("MC_Wdecay2_from_tbar_phi",&MC_W2_tbar_phi);
    chain->SetBranchAddress("MC_Wdecay2_from_tbar_eta",&MC_W2_tbar_eta);
    chain->SetBranchAddress("MC_Wdecay2_from_tbar_pt",&MC_W2_tbar_pt);
    chain->SetBranchAddress("MC_b_from_t_m",&MC_b_t_mass);
    chain->SetBranchAddress("MC_b_from_t_phi",&MC_b_t_phi);
    chain->SetBranchAddress("MC_b_from_t_eta",&MC_b_t_eta);
    chain->SetBranchAddress("MC_b_from_t_pt",&MC_b_t_pt);
    chain->SetBranchAddress("MC_b_from_tbar_m",&MC_b_tbar_mass);
    chain->SetBranchAddress("MC_b_from_tbar_phi",&MC_b_tbar_phi);
    chain->SetBranchAddress("MC_b_from_tbar_eta",&MC_b_tbar_eta);
    chain->SetBranchAddress("MC_b_from_tbar_pt",&MC_b_tbar_pt);
    chain->SetBranchAddress("weight_mc",&MC_weight);
    chain->SetBranchAddress("runNumber",&MC_runNumber);
    chain->SetBranchAddress("eventNumber", &MC_eventNumber);


    TTree * truth;
    truth = chain;

    return truth;
}

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
    char* file_path_1 = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root";
    char* file_path_2 = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000002.output_pflow.root";
    char* file_path_3 = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000003.output_pflow.root";
    char* file_path_4 = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000004.output_pflow.root";
    char* file_path_5 = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000005.output_pflow.root";
    char* file_path_6 = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000006.output_pflow.root";

    TFile *input_file_1 = new TFile(file_path_1, "READ");
	TFile *input_file_2 = new TFile(file_path_2, "READ");
	TFile *input_file_3 = new TFile(file_path_3, "READ");
	TFile *input_file_4 = new TFile(file_path_4, "READ");
	TFile *input_file_5 = new TFile(file_path_5, "READ");
	TFile *input_file_6 = new TFile(file_path_6, "READ");

    TTree *nominal_1 = (TTree*)input_file_1->Get("nominal");
	TTree *nominal_2 = (TTree*)input_file_2->Get("nominal");
	TTree *nominal_3 = (TTree*)input_file_3->Get("nominal");
	TTree *nominal_4 = (TTree*)input_file_4->Get("nominal");
	TTree *nominal_5 = (TTree*)input_file_5->Get("nominal");
	TTree *nominal_6 = (TTree*)input_file_6->Get("nominal");
    
    TTree* truth_1 = 0;
    TTree* truth_2 = 0;
    TTree* truth_3 = 0;
    TTree* truth_4 = 0;
    TTree* truth_5 = 0;
    TTree* truth_6 = 0;

    truth_1 = SetTruthTree(file_path_1);
    truth_2 = SetTruthTree(file_path_2);
    truth_3 = SetTruthTree(file_path_3);
    truth_4 = SetTruthTree(file_path_4);
    truth_5 = SetTruthTree(file_path_5);
    truth_6 = SetTruthTree(file_path_6);

    /* OUTPUT FILE AND TREE */
    TFile* output_file = new TFile("/eos/user/m/moriolpe/mphys_project/combined_data.root", "RECREATE");
    TTree* output_nominal_tree = new TTree("nominal", "nominal");
    TTree* output_truth_tree = new TTree("truth", "truth");


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

    nominal_1->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal_1->SetBranchAddress("weight_mc", &weight);
    nominal_1->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal_1->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal_1->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal_1->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal_1->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal_1->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal_1->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal_1->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal_1->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal_1->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal_1->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal_1->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal_1->SetBranchAddress("eventNumber", &eventNumber);
    nominal_1->SetBranchAddress("runNumber", &runNumber);
    nominal_1->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal_1->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal_1->SetBranchAddress("mu", &mu);
    nominal_1->SetBranchAddress("mu_actual", &mu_actual);
    nominal_1->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal_1->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal_1->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal_1->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal_1->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal_1->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal_1->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal_1->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal_1->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal_1->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal_1->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal_1->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal_1->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal_1->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal_1->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal_1->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal_1->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal_1->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal_1->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal_1->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal_1->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal_1->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal_1->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal_1->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal_1->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal_1->SetBranchAddress("met_met", &met_met);
    nominal_1->SetBranchAddress("met_sumet", &met_sumet);
    nominal_1->SetBranchAddress("met_phi", &met_phi);
    nominal_1->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal_1->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal_1->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal_1->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal_1->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal_1->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal_1->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal_1->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal_1->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal_1->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal_1->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal_1->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal_1->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal_1->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal_1->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal_1->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal_1->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal_1->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal_1->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal_1->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
    
    nominal_2->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal_2->SetBranchAddress("weight_mc", &weight);
    nominal_2->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal_2->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal_2->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal_2->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal_2->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal_2->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal_2->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal_2->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal_2->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal_2->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal_2->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal_2->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal_2->SetBranchAddress("eventNumber", &eventNumber);
    nominal_2->SetBranchAddress("runNumber", &runNumber);
    nominal_2->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal_2->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal_2->SetBranchAddress("mu", &mu);
    nominal_2->SetBranchAddress("mu_actual", &mu_actual);
    nominal_2->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal_2->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal_2->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal_2->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal_2->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal_2->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal_2->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal_2->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal_2->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal_2->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal_2->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal_2->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal_2->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal_2->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal_2->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal_2->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal_2->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal_2->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal_2->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal_2->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal_2->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal_2->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal_2->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal_2->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal_2->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal_2->SetBranchAddress("met_met", &met_met);
    nominal_2->SetBranchAddress("met_sumet", &met_sumet);
    nominal_2->SetBranchAddress("met_phi", &met_phi);
    nominal_2->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal_2->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal_2->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal_2->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal_2->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal_2->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal_2->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal_2->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal_2->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal_2->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal_2->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal_2->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal_2->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal_2->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal_2->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal_2->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal_2->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal_2->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal_2->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal_2->SetBranchAddress("jet_SV_mass", &jet_SV_mass);

    nominal_3->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal_3->SetBranchAddress("weight_mc", &weight);
    nominal_3->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal_3->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal_3->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal_3->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal_3->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal_3->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal_3->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal_3->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal_3->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal_3->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal_3->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal_3->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal_3->SetBranchAddress("eventNumber", &eventNumber);
    nominal_3->SetBranchAddress("runNumber", &runNumber);
    nominal_3->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal_3->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal_3->SetBranchAddress("mu", &mu);
    nominal_3->SetBranchAddress("mu_actual", &mu_actual);
    nominal_3->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal_3->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal_3->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal_3->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal_3->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal_3->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal_3->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal_3->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal_3->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal_3->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal_3->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal_3->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal_3->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal_3->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal_3->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal_3->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal_3->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal_3->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal_3->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal_3->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal_3->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal_3->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal_3->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal_3->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal_3->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal_3->SetBranchAddress("met_met", &met_met);
    nominal_3->SetBranchAddress("met_sumet", &met_sumet);
    nominal_3->SetBranchAddress("met_phi", &met_phi);
    nominal_3->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal_3->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal_3->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal_3->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal_3->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal_3->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal_3->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal_3->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal_3->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal_3->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal_3->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal_3->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal_3->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal_3->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal_3->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal_3->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal_3->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal_3->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal_3->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal_3->SetBranchAddress("jet_SV_mass", &jet_SV_mass);

    nominal_4->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal_4->SetBranchAddress("weight_mc", &weight);
    nominal_4->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal_4->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal_4->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal_4->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal_4->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal_4->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal_4->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal_4->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal_4->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal_4->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal_4->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal_4->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal_4->SetBranchAddress("eventNumber", &eventNumber);
    nominal_4->SetBranchAddress("runNumber", &runNumber);
    nominal_4->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal_4->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal_4->SetBranchAddress("mu", &mu);
    nominal_4->SetBranchAddress("mu_actual", &mu_actual);
    nominal_4->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal_4->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal_4->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal_4->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal_4->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal_4->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal_4->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal_4->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal_4->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal_4->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal_4->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal_4->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal_4->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal_4->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal_4->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal_4->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal_4->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal_4->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal_4->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal_4->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal_4->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal_4->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal_4->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal_4->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal_4->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal_4->SetBranchAddress("met_met", &met_met);
    nominal_4->SetBranchAddress("met_sumet", &met_sumet);
    nominal_4->SetBranchAddress("met_phi", &met_phi);
    nominal_4->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal_4->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal_4->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal_4->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal_4->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal_4->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal_4->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal_4->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal_4->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal_4->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal_4->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal_4->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal_4->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal_4->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal_4->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal_4->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal_4->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal_4->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal_4->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal_4->SetBranchAddress("jet_SV_mass", &jet_SV_mass);

    nominal_5->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal_5->SetBranchAddress("weight_mc", &weight);
    nominal_5->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal_5->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal_5->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal_5->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal_5->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal_5->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal_5->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal_5->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal_5->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal_5->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal_5->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal_5->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal_5->SetBranchAddress("eventNumber", &eventNumber);
    nominal_5->SetBranchAddress("runNumber", &runNumber);
    nominal_5->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal_5->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal_5->SetBranchAddress("mu", &mu);
    nominal_5->SetBranchAddress("mu_actual", &mu_actual);
    nominal_5->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal_5->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal_5->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal_5->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal_5->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal_5->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal_5->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal_5->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal_5->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal_5->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal_5->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal_5->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal_5->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal_5->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal_5->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal_5->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal_5->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal_5->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal_5->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal_5->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal_5->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal_5->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal_5->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal_5->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal_5->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal_5->SetBranchAddress("met_met", &met_met);
    nominal_5->SetBranchAddress("met_sumet", &met_sumet);
    nominal_5->SetBranchAddress("met_phi", &met_phi);
    nominal_5->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal_5->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal_5->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal_5->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal_5->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal_5->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal_5->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal_5->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal_5->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal_5->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal_5->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal_5->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal_5->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal_5->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal_5->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal_5->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal_5->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal_5->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal_5->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal_5->SetBranchAddress("jet_SV_mass", &jet_SV_mass);

    nominal_6->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal_6->SetBranchAddress("weight_mc", &weight);
    nominal_6->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal_6->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal_6->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal_6->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal_6->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal_6->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal_6->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal_6->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal_6->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal_6->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal_6->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal_6->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal_6->SetBranchAddress("eventNumber", &eventNumber);
    nominal_6->SetBranchAddress("runNumber", &runNumber);
    nominal_6->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal_6->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal_6->SetBranchAddress("mu", &mu);
    nominal_6->SetBranchAddress("mu_actual", &mu_actual);
    nominal_6->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal_6->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal_6->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal_6->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal_6->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal_6->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal_6->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal_6->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal_6->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal_6->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal_6->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal_6->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal_6->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal_6->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal_6->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal_6->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal_6->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal_6->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal_6->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal_6->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal_6->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal_6->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal_6->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal_6->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal_6->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal_6->SetBranchAddress("met_met", &met_met);
    nominal_6->SetBranchAddress("met_sumet", &met_sumet);
    nominal_6->SetBranchAddress("met_phi", &met_phi);
    nominal_6->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal_6->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal_6->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal_6->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal_6->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal_6->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal_6->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal_6->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal_6->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal_6->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal_6->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal_6->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal_6->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal_6->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal_6->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal_6->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal_6->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal_6->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal_6->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal_6->SetBranchAddress("jet_SV_mass", &jet_SV_mass);

    /* CREATE ALL BRANCHES IN THE OUTPUT TREE */
    output_nominal_tree->Branch("mc_generator_weights", &mc_generator_weights);
    output_nominal_tree->Branch("weight_mc", &weight);
    output_nominal_tree->Branch("weight_pileup", &weight_pileup);
    output_nominal_tree->Branch("weight_beamspot", &weight_beamspot);
    output_nominal_tree->Branch("weight_leptonSF", &weight_leptonSF);
    output_nominal_tree->Branch("weight_trigger", &weight_trigger);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    output_nominal_tree->Branch("weight_jvt", &weight_jvt);
    output_nominal_tree->Branch("weight_pileup_UP", &weight_pileup_UP);
    output_nominal_tree->Branch("weight_pileup_DOWN", &weight_pileup_DOWN);
    output_nominal_tree->Branch("weight_jvt_UP", &weight_jvt_UP);
    output_nominal_tree->Branch("weight_jvt_DOWN", &weight_jvt_DOWN);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    output_nominal_tree->Branch("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    output_nominal_tree->Branch("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    output_nominal_tree->Branch("weight_poisson", &weight_poisson);
    output_nominal_tree->Branch("eventNumber", &eventNumber);
    output_nominal_tree->Branch("runNumber", &runNumber);
    output_nominal_tree->Branch("randomRunNumber", &randomRunNumber);
    output_nominal_tree->Branch("mcChannelNumber", &mcChannelNumber);
    output_nominal_tree->Branch("mu", &mu);
    output_nominal_tree->Branch("mu_actual", &mu_actual);
    output_nominal_tree->Branch("el_pt", &nominal_el_pt);
    output_nominal_tree->Branch("el_eta", &nominal_el_eta);
    output_nominal_tree->Branch("el_phi", &nominal_el_phi);
    output_nominal_tree->Branch("el_charge", &nominal_el_charge);
    output_nominal_tree->Branch("mu_pt", &nominal_mu_pt);
    output_nominal_tree->Branch("mu_eta", &nominal_mu_eta);
    output_nominal_tree->Branch("mu_phi", &nominal_mu_phi);
    output_nominal_tree->Branch("mu_charge", &nominal_mu_charge);
    output_nominal_tree->Branch("jet_pt", &nominal_jet_pt);
    output_nominal_tree->Branch("jet_eta", &nominal_jet_eta);
    output_nominal_tree->Branch("jet_phi", &nominal_jet_phi);
    output_nominal_tree->Branch("jet_e", &nominal_jet_e); 
    output_nominal_tree->Branch("jet_truthflav", &jet_truthflav);   
    output_nominal_tree->Branch("jet_truthPartonLabel", &jet_truthPartonLabel);   
    output_nominal_tree->Branch("jet_truthflavExtended", &jet_truthflavExtended); 
    output_nominal_tree->Branch("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    output_nominal_tree->Branch("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    output_nominal_tree->Branch("jet_GN120220509", &jet_GN120220509);
    output_nominal_tree->Branch("jet_GN120220509_pb", &jet_GN120220509_pb);
    output_nominal_tree->Branch("jet_GN120220509_pc", &jet_GN120220509_pc);
    output_nominal_tree->Branch("jet_GN120220509_pu", &jet_GN120220509_pu);
    output_nominal_tree->Branch("jet_DL1dv01", &jet_DL1dv01);
    output_nominal_tree->Branch("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    output_nominal_tree->Branch("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    output_nominal_tree->Branch("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    output_nominal_tree->Branch("met_met", &met_met);
    output_nominal_tree->Branch("met_sumet", &met_sumet);
    output_nominal_tree->Branch("met_phi", &met_phi);
    output_nominal_tree->Branch("klfitter_logLikelihood", &klfitter_logLikelihood);
    output_nominal_tree->Branch("klfitter_selected", &klfitter_selected);
    output_nominal_tree->Branch("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    output_nominal_tree->Branch("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    output_nominal_tree->Branch("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    output_nominal_tree->Branch("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    output_nominal_tree->Branch("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    output_nominal_tree->Branch("ejets_2022_gn", &ejets_2022_gn);
    output_nominal_tree->Branch("ejets_2022_dl1d", &ejets_2022_dl1d);
    output_nominal_tree->Branch("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    output_nominal_tree->Branch("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    output_nominal_tree->Branch("new_ejets_gn_2022", &new_ejets_gn_2022);
    output_nominal_tree->Branch("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    output_nominal_tree->Branch("mujets_gn_2022", &mujets_gn_2022);
    output_nominal_tree->Branch("mujets_dl1d_2022", &mujets_dl1d_2022);
    output_nominal_tree->Branch("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    output_nominal_tree->Branch("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    output_nominal_tree->Branch("new_mujets_gn_2022", &new_mujets_gn_2022);
    output_nominal_tree->Branch("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    output_nominal_tree->Branch("jet_SV_mass", &jet_SV_mass);

    output_truth_tree->Branch("MC_Wdecay1_from_t_pdgId",&MC_W1_t_PID);
    output_truth_tree->Branch("MC_Wdecay1_from_t_m",&MC_W1_t_mass);
    output_truth_tree->Branch("MC_Wdecay1_from_t_phi",&MC_W1_t_phi);
    output_truth_tree->Branch("MC_Wdecay1_from_t_eta",&MC_W1_t_eta);
    output_truth_tree->Branch("MC_Wdecay1_from_t_pt",&MC_W1_t_pt);
    output_truth_tree->Branch("MC_Wdecay2_from_t_pdgId",&MC_W2_t_PID);
    output_truth_tree->Branch("MC_Wdecay2_from_t_m",&MC_W2_t_mass);
    output_truth_tree->Branch("MC_Wdecay2_from_t_phi",&MC_W2_t_phi);
    output_truth_tree->Branch("MC_Wdecay2_from_t_eta",&MC_W2_t_eta);
    output_truth_tree->Branch("MC_Wdecay2_from_t_pt",&MC_W2_t_pt);
    output_truth_tree->Branch("MC_Wdecay1_from_tbar_pdgId",&MC_W1_tbar_PID);
    output_truth_tree->Branch("MC_Wdecay1_from_tbar_m",&MC_W1_tbar_mass);
    output_truth_tree->Branch("MC_Wdecay1_from_tbar_phi",&MC_W1_tbar_phi);
    output_truth_tree->Branch("MC_Wdecay1_from_tbar_eta",&MC_W1_tbar_eta);
    output_truth_tree->Branch("MC_Wdecay1_from_tbar_pt",&MC_W1_tbar_pt);
    output_truth_tree->Branch("MC_Wdecay2_from_tbar_pdgId",&MC_W2_tbar_PID);
    output_truth_tree->Branch("MC_Wdecay2_from_tbar_m",&MC_W2_tbar_mass);
    output_truth_tree->Branch("MC_Wdecay2_from_tbar_phi",&MC_W2_tbar_phi);
    output_truth_tree->Branch("MC_Wdecay2_from_tbar_eta",&MC_W2_tbar_eta);
    output_truth_tree->Branch("MC_Wdecay2_from_tbar_pt",&MC_W2_tbar_pt);
    output_truth_tree->Branch("MC_b_from_t_m",&MC_b_t_mass);
    output_truth_tree->Branch("MC_b_from_t_phi",&MC_b_t_phi);
    output_truth_tree->Branch("MC_b_from_t_eta",&MC_b_t_eta);
    output_truth_tree->Branch("MC_b_from_t_pt",&MC_b_t_pt);
    output_truth_tree->Branch("MC_b_from_tbar_m",&MC_b_tbar_mass);
    output_truth_tree->Branch("MC_b_from_tbar_phi",&MC_b_tbar_phi);
    output_truth_tree->Branch("MC_b_from_tbar_eta",&MC_b_tbar_eta);
    output_truth_tree->Branch("MC_b_from_tbar_pt",&MC_b_tbar_pt);
    output_truth_tree->Branch("weight_mc",&MC_weight);
    output_truth_tree->Branch("runNumber",&MC_runNumber);
    output_truth_tree->Branch("eventNumber", &MC_eventNumber);
    
    Long64_t nentries_1 = nominal_1->GetEntries();
    for(Long64_t i=0; i<nentries_1; i++) {
        if (i==0) { std::cout << "FILE 1:" << std::endl; }
        
        nominal_1->GetEntry(i);
        truth_1->GetEntryWithIndex(runNumber,eventNumber);

        output_nominal_tree->Fill();
        output_truth_tree->Fill();

        updateProgressBar(i+1, nentries_1);
    }

    Long64_t nentries_2 = nominal_2->GetEntries();
    for(Long64_t i=0; i<nentries_2; i++) {
        if (i==0) { std::cout << "FILE 2:" << std::endl; }
        
        nominal_2->GetEntry(i);
        truth_2->GetEntryWithIndex(runNumber,eventNumber);

        output_nominal_tree->Fill();
        output_truth_tree->Fill();

        updateProgressBar(i+1, nentries_2);
    }

    Long64_t nentries_3 = nominal_3->GetEntries();
    for(Long64_t i=0; i<nentries_3; i++) {
        if (i==0) { std::cout << "FILE 3:" << std::endl; }
        
        nominal_3->GetEntry(i);
        truth_3->GetEntryWithIndex(runNumber,eventNumber);

        output_nominal_tree->Fill();
        output_truth_tree->Fill();

        updateProgressBar(i+1, nentries_3);

    }

    Long64_t nentries_4 = nominal_4->GetEntries();
    for(Long64_t i=0; i<nentries_4; i++) {
        if (i==0) { std::cout << "FILE 4:" << std::endl; }
        
        nominal_4->GetEntry(i);
        truth_4->GetEntryWithIndex(runNumber,eventNumber);

        output_nominal_tree->Fill();
        output_truth_tree->Fill();

        updateProgressBar(i+1, nentries_4);
    }

    Long64_t nentries_5 = nominal_5->GetEntries();
    for(Long64_t i=0; i<nentries_5; i++) {
        if (i==0) { std::cout << "FILE 5:" << std::endl; }
        
        nominal_5->GetEntry(i);
        truth_5->GetEntryWithIndex(runNumber,eventNumber);

        output_nominal_tree->Fill();
        output_truth_tree->Fill();

        updateProgressBar(i+1, nentries_5);
    }

    Long64_t nentries_6 = nominal_6->GetEntries();
    for(Long64_t i=0; i<nentries_6; i++) {
        if (i==0) { std::cout << "FILE 6:" << std::endl; }
        
        nominal_6->GetEntry(i);
        truth_6->GetEntryWithIndex(runNumber,eventNumber);

        output_nominal_tree->Fill();
        output_truth_tree->Fill();

        updateProgressBar(i+1, nentries_6);
    }
    std::cout << "DONE" << std::endl;
    output_file->cd();
    output_nominal_tree->Write();
    output_truth_tree->Write();
    output_file->Close();
    delete output_file;

    return 0;
}