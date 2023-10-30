#include <TSystem.h>
#include <TSystemFile.h>
#include <TSystemDirectory.h>
#include <TChain.h>
#include <iostream>
#include <TFile.h>
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


// Declare variables
std::vector<float> *mc_generator_weights = nullptr;
float weight_mc = 0;
float weight_pileup = 0;
float weight_beamspot = 0;
float weight_leptonSF = 0;
float weight_trigger = 0;
float weight_bTagSF_DL1dv01_60 = 0;
float weight_bTagSF_GN120220509_60 = 0;
float weight_jvt = 0;
float weight_pileup_UP = 0;
float weight_pileup_DOWN = 0;
float weight_jvt_UP = 0;
float weight_jvt_DOWN = 0;

// For variables with NULL values, you can declare them as pointers
float *weight_bTagSF_DL1dv01_60_eigenvars_B_up = nullptr;
float *weight_bTagSF_DL1dv01_60_eigenvars_C_up = nullptr;
float *weight_bTagSF_DL1dv01_60_eigenvars_Light_up = nullptr;
float *weight_bTagSF_DL1dv01_60_eigenvars_B_down = nullptr;
float *weight_bTagSF_DL1dv01_60_eigenvars_C_down = nullptr;
float *weight_bTagSF_DL1dv01_60_eigenvars_Light_down = nullptr;
float *weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up = 0;
float *weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down = 0;
float *weight_bTagSF_GN120220509_60_eigenvars_B_up = nullptr;
float *weight_bTagSF_GN120220509_60_eigenvars_C_up = nullptr;
float *weight_bTagSF_GN120220509_60_eigenvars_Light_up = nullptr;
float *weight_bTagSF_GN120220509_60_eigenvars_B_down = nullptr;
float *weight_bTagSF_GN120220509_60_eigenvars_C_down = nullptr;
float *weight_bTagSF_GN120220509_60_eigenvars_Light_down = nullptr;
float *weight_bTagSF_GN120220509_60_extrapolation_from_charm_up = 0;
float *weight_bTagSF_GN120220509_60_extrapolation_from_charm_down = 0;
float *weight_poisson = nullptr;
UInt_t eventNumber = 0;
UInt_t runNumber = 0;
UInt_t randomRunNumber = 0;
UInt_t mcChannelNumber = 0;
float mu = 0;
float mu_actual = 0;

std::vector<float> *el_pt = nullptr;
std::vector<float> *el_eta = nullptr;
std::vector<float> *el_phi = nullptr;
std::vector<int> *el_charge = nullptr;

std::vector<float> *mu_pt = nullptr;
std::vector<float> *mu_eta = nullptr;
std::vector<float> *mu_phi = nullptr;
std::vector<int> *mu_charge = nullptr;

std::vector<float> *jet_pt = nullptr;
std::vector<float> *jet_eta = nullptr;
std::vector<float> *jet_phi = nullptr;
std::vector<float> *jet_e = nullptr;
std::vector<int> *jet_truthflav = nullptr;
std::vector<int> *jet_truthPartonLabel = nullptr;
std::vector<int> *jet_truthflavExtended = nullptr;
std::vector<bool> *jet_isbtagged_DL1dv01_60 = nullptr;
std::vector<bool> *jet_isbtagged_GN120220509_60 = nullptr;
std::vector<float> *jet_GN120220509 = nullptr;
std::vector<float> *jet_GN120220509_pb = nullptr;
std::vector<float> *jet_GN120220509_pc = nullptr;
std::vector<float> *jet_GN120220509_pu = nullptr;
std::vector<float> *jet_DL1dv01 = nullptr;
std::vector<float> *jet_DL1dv01_pb = nullptr;
std::vector<float> *jet_DL1dv01_pc = nullptr;
std::vector<float> *jet_DL1dv01_pu = nullptr;
float met_met = 0;
float met_sumet = 0;
float met_phi = 0;

// Declare variables for klfitter
float *klfitter_logLikelihood = nullptr;
int klfitter_selected = 0;
int *klfitter_minuitDidNotConverge = nullptr;
int *klfitter_model_bhad_jetIndex = nullptr;
int *klfitter_model_blep_jetIndex = nullptr;
int *klfitter_model_lq1_jetIndex = nullptr;
int *klfitter_model_lq2_jetIndex = nullptr;

int ejets_2022_gn = 0;
int ejets_2022_dl1d = 0;
int ejets_2022_gn_lowPt = 0;
int ejets_2022_dl1d_lowPt = 0;
int new_ejets_gn_2022 = 0;
int new_ejets_dl1d_2022 = 0;
int mujets_gn_2022 = 0;
int mujets_dl1d_2022 = 0;
int mujets_2022_gn_lowPt = 0;
int mujets_2022_dl1d_lowPt = 0;
int new_mujets_gn_2022 = 0;
int new_mujets_dl1d_2022 = 0;

std::vector<float> *jet_SV_mass = nullptr;


void ProcessFilesInFolder(const char* folderPath) {

    // Create a TSystem object to work with the file system
    TSystem* sys = gSystem;

    // Create a TChain to combine the files in the folder
    TChain chain_f("file_chain");

    // Create a TChain to combine the nominal TTrees from each file
    TChain *chain_n = new TChain("nominal", "");

    // Create a TChain to combine the truth TTrees from each file
    TChain *chain_t = new TChain("truth", "");


    // Specify the path and name of the output ROOT file
    TString outputPath = "/afs/cern.ch/user/e/etogia/public/output_file.root";

    // Create a new output file where you want to store the combined TTree
    TFile* outputFile = new TFile(outputPath, "RECREATE");


    // Check if the folder exists
    if (!sys->AccessPathName(folderPath)) {
        // Open the folder
        TSystemDirectory dir(folderPath, folderPath);

        // Get the list of files and subdirectories in the folder
        TList* fileList = dir.GetListOfFiles();
        fileList->Sort();  // Sort the list of files

        // Loop over the list of files and subdirectories
        if (fileList) {
            TSystemFile* file; // Declare TSystemFile
            TString fname;
            TIter next(fileList);
            while ((file = (TSystemFile*)next())) {
                fname = file->GetName();
                // Check if it's a directory
                if (!file->IsDirectory()) {
                    // You can access the file name as fname
                    //std::cout<<fname<<std::endl;
                    char* file_path = Form("%s/%s", folderPath, fname.Data());
                    // Add the nominal to the TChain
                    chain_n->Add(file_path);


                    // Set branch addresses for chain_n
                    chain_n->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
                    chain_n->SetBranchAddress("weight_mc", &weight_mc);
                    chain_n->SetBranchAddress("weight_pileup", &weight_pileup);
                    chain_n->SetBranchAddress("weight_beamspot", &weight_beamspot);
                    chain_n->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
                    chain_n->SetBranchAddress("weight_trigger", &weight_trigger);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
                    chain_n->SetBranchAddress("weight_jvt", &weight_jvt);
                    chain_n->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
                    chain_n->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
                    chain_n->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
                    chain_n->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);

                    // For variables with NULL values, set the branch addresses to nullptr
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
                    chain_n->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
                    chain_n->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
                    chain_n->SetBranchAddress("weight_poisson", &weight_poisson);
                    chain_n->SetBranchAddress("eventNumber", &eventNumber);
                    chain_n->SetBranchAddress("runNumber", &runNumber);
                    chain_n->SetBranchAddress("randomRunNumber", &randomRunNumber);
                    chain_n->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
                    chain_n->SetBranchAddress("mu", &mu);
                    chain_n->SetBranchAddress("mu_actual", &mu_actual);

                    chain_n->SetBranchAddress("el_pt", &el_pt);
                    chain_n->SetBranchAddress("el_eta", &el_eta);
                    chain_n->SetBranchAddress("el_phi", &el_phi);
                    chain_n->SetBranchAddress("el_charge", &el_charge);

                    chain_n->SetBranchAddress("mu_pt", &mu_pt);
                    chain_n->SetBranchAddress("mu_eta", &mu_eta);
                    chain_n->SetBranchAddress("mu_phi", &mu_phi);
                    chain_n->SetBranchAddress("mu_charge", &mu_charge);

                    chain_n->SetBranchAddress("jet_pt", &jet_pt);
                    chain_n->SetBranchAddress("jet_eta", &jet_eta);
                    chain_n->SetBranchAddress("jet_phi", &jet_phi);
                    chain_n->SetBranchAddress("jet_e", &jet_e);
                    chain_n->SetBranchAddress("jet_truthflav", &jet_truthflav);
                    chain_n->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);
                    chain_n->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended);
                    chain_n->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);
                    chain_n->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);
                    chain_n->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
                    chain_n->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
                    chain_n->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
                    chain_n->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
                    chain_n->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
                    chain_n->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
                    chain_n->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
                    chain_n->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
                    chain_n->SetBranchAddress("met_met", &met_met);
                    chain_n->SetBranchAddress("met_sumet", &met_sumet);
                    chain_n->SetBranchAddress("met_phi", &met_phi);

                    // Set branch addresses for klfitter
                    chain_n->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
                    chain_n->SetBranchAddress("klfitter_selected", &klfitter_selected);
                    chain_n->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
                    chain_n->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
                    chain_n->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
                    chain_n->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
                    chain_n->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);

                    // Set branch addresses for ejets and mujets variables
                    chain_n->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
                    chain_n->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
                    chain_n->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
                    chain_n->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
                    chain_n->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
                    chain_n->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
                    chain_n->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
                    chain_n->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
                    chain_n->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
                    chain_n->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
                    chain_n->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
                    chain_n->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);

                    // Set branch addresses for jet_SV_mass
                    chain_n->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
                    

                    // Add the truth to the TChain
                    chain_t->Add(file_path);

                    // Set branch addresses for the truth TTree
                    chain_t->SetBranchAddress("MC_Wdecay1_from_t_pdgId", &MC_W1_t_PID);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_t_m", &MC_W1_t_mass);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_t_phi", &MC_W1_t_phi);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_t_eta", &MC_W1_t_eta);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_t_pt", &MC_W1_t_pt);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_t_pdgId", &MC_W2_t_PID);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_t_m", &MC_W2_t_mass);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_t_phi", &MC_W2_t_phi);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_t_eta", &MC_W2_t_eta);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_t_pt", &MC_W2_t_pt);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_tbar_pdgId", &MC_W1_tbar_PID);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_tbar_m", &MC_W1_tbar_mass);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_tbar_phi", &MC_W1_tbar_phi);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_tbar_eta", &MC_W1_tbar_eta);
                    chain_t->SetBranchAddress("MC_Wdecay1_from_tbar_pt", &MC_W1_tbar_pt);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_tbar_pdgId", &MC_W2_tbar_PID);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_tbar_m", &MC_W2_tbar_mass);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_tbar_phi", &MC_W2_tbar_phi);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_tbar_eta", &MC_W2_tbar_eta);
                    chain_t->SetBranchAddress("MC_Wdecay2_from_tbar_pt", &MC_W2_tbar_pt);
                    chain_t->SetBranchAddress("MC_b_from_t_m",&MC_b_t_mass);
                    chain_t->SetBranchAddress("MC_b_from_t_phi",&MC_b_t_phi);
                    chain_t->SetBranchAddress("MC_b_from_t_eta",&MC_b_t_eta);
                    chain_t->SetBranchAddress("MC_b_from_t_pt",&MC_b_t_pt);
                    chain_t->SetBranchAddress("MC_b_from_tbar_m",&MC_b_tbar_mass);
                    chain_t->SetBranchAddress("MC_b_from_tbar_phi",&MC_b_tbar_phi);
                    chain_t->SetBranchAddress("MC_b_from_tbar_eta",&MC_b_tbar_eta);
                    chain_t->SetBranchAddress("MC_b_from_tbar_pt",&MC_b_tbar_pt);
                    chain_t->SetBranchAddress("weight_mc",&MC_weight);
                    chain_t->SetBranchAddress("runNumber",&MC_runNumber);
                    chain_t->SetBranchAddress("eventNumber", &MC_eventNumber);

                }
            }
        }
    }
    
    TTree * truth;
    truth = chain_t;
    truth->Write();
    
    TTree * nominal;
    nominal = chain_n;
    nominal->Write();


    outputFile->Close();

}

int main() {
    // Provide the path to the folder you want to process
    const char* folderPath = "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root";

    ProcessFilesInFolder(folderPath);

    return 0;
}