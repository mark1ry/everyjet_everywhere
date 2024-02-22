// Open nominal tree -select appropiate variables
// Open truth tree -select appropriate variables
// Loop over events
//      Calculate Lorentz vector for events in both trees
//      Match each true event with the closest jet using DeltaR
//      Store information of each event in a vector
// Store match information in a new branch (i.e. will need to create a new tree)


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
    chain->Add("/eos/user/m/moriolpe/mphys_project/root_files/combined_data.root");


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

void print_2D_array(int** v, int length) {
    std::cout << std::endl;
    for (int x{0}; x<4; x++) {
        std::cout << "[ ";
        for (int y{0}; y<length; y++) {
            std::cout << v[x][y] << " ";
        }
        std::cout << "]" << std::endl;
    }
    return;
}

void print_2D_array(Double_t** v, int length) {
    std::cout << std::endl;
    for (int x{0}; x<4; x++) {
        std::cout << "[ ";
        for (int y{0}; y<length; y++) {
            std::cout << v[x][y] << " ";
        }
        std::cout << "]" << std::endl;
    }
    return;
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

bool selection_criteria_truth (bool W_t, bool W_tbar) {
    if (MC_b_t_pt<20000 || MC_b_t_eta>2.5 || MC_b_t_eta<-2.5) {return false;}
    if (MC_b_tbar_pt<20000 || MC_b_tbar_eta>2.5 || MC_b_tbar_eta<-2.5) {return false;}
    if (W_t) {
        if (MC_W1_t_pt<20000 || MC_W1_t_eta>2.5 || MC_W1_t_eta<-2.5) {return false;}
        if (MC_W2_t_pt<20000 || MC_W2_t_eta>2.5 || MC_W2_t_eta<-2.5) {return false;}
        if (MC_W1_tbar_PID!=11 && MC_W1_tbar_PID!=13) { return false; }
        if (MC_W1_tbar_pt<27000 || MC_W1_tbar_eta>2.5 || MC_W1_tbar_eta<-2.5) {return false;}
    } else if (W_tbar) {
        if (MC_W1_tbar_pt<20000 || MC_W1_tbar_eta>2.5 || MC_W1_tbar_eta<-2.5) {return false;}
        if (MC_W2_tbar_pt<20000 || MC_W2_tbar_eta>2.5 || MC_W2_tbar_eta<-2.5) {return false;}
        if (MC_W2_t_PID!=-11 && MC_W2_t_PID!=-13) { return false; }
        if (MC_W2_t_pt<27000 || MC_W2_t_eta>2.5 || MC_W2_t_eta<-2.5) {return false;}
    }
    return true;
}

bool select_hadronic_decay(bool * W_t, bool * W_tbar) {
    if (MC_W1_t_PID<10) {*W_t=true;}
    else if (MC_W1_t_PID>10) {*W_t=false;}
    else {return false;}
    if (MC_W1_tbar_PID<10) {*W_tbar=true;}
    else if (MC_W1_tbar_PID>10) {*W_tbar=false;}
    else {return false;}
    if (*W_t==*W_tbar) {return false;}
    return true;
}

int vector_sum(int* v, int length) {
    int sum = 0;
    for (int k{0}; k<length; k++) {
        sum += v[k];
    }
    return sum;
}

int find_index (int* v, int length) {
    for (int l{0}; l<length; l++) {
        if (v[l] == 1) {
            return l;
        }
    }
    return -99;
}

bool sort_algorithm (int* matched_jets, int** truth_match_indices, Double_t** truth_match_deltas, int length) {
    int index_of_match;
    bool match_found;
    int sum_all_truths, sum_one_truth;
    do {
        match_found=false;
        sum_all_truths = 0;
        for (int k{0}; k<4; k++) {
            sum_one_truth = vector_sum(truth_match_indices[k], length);
            sum_all_truths += sum_one_truth;
            if (sum_one_truth == 1) {
                index_of_match = find_index(truth_match_indices[k], length);
                matched_jets[index_of_match] = k+1;
                match_found=true;
                for (int l{0}; l<4; l++) {
                    truth_match_indices[l][index_of_match] = 0;
                }
            }
        }
        if (vector_sum(matched_jets, length)==10) {directly_matched+=1; return true;}
    } while (match_found);
    if (sum_all_truths<4 && sum_all_truths>0){
        int temp_index;
        Double_t min_DeltaR = 0.5;
        for (int k{0}; k<4; k++) {
            if (vector_sum(truth_match_indices[k], length)!=0) {
                while (find_index(truth_match_indices[k], length)!=-99) {
                    temp_index = find_index(truth_match_indices[k], length);
                    truth_match_indices[k][temp_index] = 0;
                    if (truth_match_deltas[k][temp_index] < min_DeltaR) {
                        index_of_match = temp_index;
                        min_DeltaR = truth_match_deltas[k][temp_index];
                    }
                }
                matched_jets[index_of_match] += k+1;
                if (vector_sum(matched_jets, length)==10) {
                    indirectly_matched+=1;
                    return true;
                }
            }
        }
    }
    if (sum_all_truths==0) {
        unmatchable+=1;
    } else if (sum_all_truths>3) {
        more_complex+=1;
    }
    
    return false;
}

void set_vector_to_zero (int* v, int length) {
    for (int k{0}; k<length; k++) {
        v[k] = 0;
    }
    return;
}

void set_vector_to_zero (Double_t* v, int length) {
    for (int k{0}; k<length; k++) {
        v[k] = 99.;
    }
    return;
}

std::vector<int> array_to_vector(int* a, int length) {
    std::vector<int> v (length);
    for (int k{0}; k<length; k++) {
        v[k] = a[k];
    }
    return v;
}

void histogram_writing (TFile* output_file, TH1F** v, int length) {
    /* FORMAT HISTOGRAMS */
    format_hist(v[0], kBlue, "MC W_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.4);
    format_hist(v[1], kBlue, "MC W_lep Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.4);
    format_hist(v[2], kBlue, "MC t_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.4);
    format_hist(v[3], kBlue, "MC t_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.4);
    format_hist(v[4], kBlue, "MC W Jets Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(v[5], kBlue, "MC W Leptons Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(v[6], kBlue, "MC W_had to b Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(v[7], kBlue, "MC W_lep to b Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(v[8], kBlue, "MC Top Jets Angle Distribution", "Angle", "Entries", 0.1);

    format_hist(v[9], kBlue, "W_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.15);
    format_hist(v[10], kBlue, "t_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.15);
    format_hist(v[11], kBlue, "W Jets Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(v[12], kBlue, "W_had to b Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(v[13], kBlue, "Top Jets Angle Distribution", "Angle", "Entries", 0.1);

    output_file->cd();
    output_file->Delete("h_MC_Whad_minv;*");
    output_file->Delete("h_MC_Wlep_minv;*");
    output_file->Delete("h_MC_thad_minv;*");
    output_file->Delete("h_MC_tlep_minv;*");
    output_file->Delete("h_MC_wjets_angle;*");
    output_file->Delete("h_MC_wleptons_angle;*");
    output_file->Delete("h_MC_twhad_angle;*");
    output_file->Delete("h_MC_twlep_angle;*");
    output_file->Delete("h_MC_bbbar_angle;*");
    output_file->Delete("h_Whad_minv;*");
    output_file->Delete("h_thad_minv;*");
    output_file->Delete("h_wjets_angle;*");
    output_file->Delete("h_twhad_angle;*");
    output_file->Delete("h_bbbar_angle;*");
    
    
    for (int k=0; k<length; k++) {
        std::cout << "Here" << std::endl;
        v[k]->Write();
    }

    std::cout << "Here" << std::endl;
    output_file->Close();
    delete output_file;

    return;
}

void get_indices(int* v, int* jet_match, int length) {
    int holder;
    for (int k{0}; k<length; k++) {
        holder = jet_match[k];
        if (holder==1) v[0] = k;
        else if (holder==2) v[1] = k;
        else if (holder==3) v[2] = k;
        else if (holder==4) v[3] = k;
    }
}

std::vector<int> find_pt_order(std::vector<float> jet_pt)  {
     float max_pt;
     std::vector<int> ordered (4);
     if (jet_pt.size()!=4) { std::cout << "!!! EVENT HAD MORE THAN FOUR JETS, COULD NOT ORDER !!!" << std::endl; return ordered; }
     for (int l{0}; l<4; l++) {
         max_pt = 0;
         for (int m{0}; m<4; m++) {
             if (jet_pt[m] > max_pt) { max_pt = jet_pt[m]; ordered[l] = m; }
         }
         jet_pt[ordered[l]] = 0;
     }
     return ordered;
}

int main () {
    std::cout << "code has been initiated" << std::endl;
    /* INPUT DATA */
    char* file_path= "/eos/user/m/moriolpe/mphys_project/root_files/combined_data.root";
	TFile *input_file = new TFile(file_path, "READ");
	TTree *nominal = (TTree*)input_file->Get("nominal");
    TTree* truth = 0;
    truth = SetTruthTree(file_path);

    /* OUTPUT FILE AND TREE */
    TFile* output_file = new TFile("/eos/user/m/moriolpe/mphys_project/root_files/matched_dl1dselected_alljets.root", "RECREATE");
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

    nominal->SetBranchAddress("mc_generator_weights", &mc_generator_weights);
    nominal->SetBranchAddress("weight_mc", &weight);
    nominal->SetBranchAddress("weight_pileup", &weight_pileup);
    nominal->SetBranchAddress("weight_beamspot", &weight_beamspot);
    nominal->SetBranchAddress("weight_leptonSF", &weight_leptonSF);
    nominal->SetBranchAddress("weight_trigger", &weight_trigger);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60", &weight_bTagSF_DL1dv01_60);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60", &weight_bTagSF_GN120220509_60);
    nominal->SetBranchAddress("weight_jvt", &weight_jvt);
    nominal->SetBranchAddress("weight_pileup_UP", &weight_pileup_UP);
    nominal->SetBranchAddress("weight_pileup_DOWN", &weight_pileup_DOWN);
    nominal->SetBranchAddress("weight_jvt_UP", &weight_jvt_UP);
    nominal->SetBranchAddress("weight_jvt_DOWN", &weight_jvt_DOWN);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_up", &weight_bTagSF_DL1dv01_60_eigenvars_B_up);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_up", &weight_bTagSF_DL1dv01_60_eigenvars_C_up);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_up", &weight_bTagSF_DL1dv01_60_eigenvars_Light_up);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_B_down", &weight_bTagSF_DL1dv01_60_eigenvars_B_down);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_C_down", &weight_bTagSF_DL1dv01_60_eigenvars_C_down);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_eigenvars_Light_down", &weight_bTagSF_DL1dv01_60_eigenvars_Light_down);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_up);
    nominal->SetBranchAddress("weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down", &weight_bTagSF_DL1dv01_60_extrapolation_from_charm_down);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_up", &weight_bTagSF_GN120220509_60_eigenvars_B_up);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_up", &weight_bTagSF_GN120220509_60_eigenvars_C_up);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_up", &weight_bTagSF_GN120220509_60_eigenvars_Light_up);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_B_down", &weight_bTagSF_GN120220509_60_eigenvars_B_down);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_C_down", &weight_bTagSF_GN120220509_60_eigenvars_C_down);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_eigenvars_Light_down", &weight_bTagSF_GN120220509_60_eigenvars_Light_down);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_up", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_up);
    nominal->SetBranchAddress("weight_bTagSF_GN120220509_60_extrapolation_from_charm_down", &weight_bTagSF_GN120220509_60_extrapolation_from_charm_down);
    nominal->SetBranchAddress("weight_poisson", &weight_poisson);
    nominal->SetBranchAddress("eventNumber", &eventNumber);
    nominal->SetBranchAddress("runNumber", &runNumber);
    nominal->SetBranchAddress("randomRunNumber", &randomRunNumber);
    nominal->SetBranchAddress("mcChannelNumber", &mcChannelNumber);
    nominal->SetBranchAddress("mu", &mu);
    nominal->SetBranchAddress("mu_actual", &mu_actual);
    nominal->SetBranchAddress("el_pt", &nominal_el_pt);
    nominal->SetBranchAddress("el_eta", &nominal_el_eta);
    nominal->SetBranchAddress("el_phi", &nominal_el_phi);
    nominal->SetBranchAddress("el_charge", &nominal_el_charge);
    nominal->SetBranchAddress("mu_pt", &nominal_mu_pt);
    nominal->SetBranchAddress("mu_eta", &nominal_mu_eta);
    nominal->SetBranchAddress("mu_phi", &nominal_mu_phi);
    nominal->SetBranchAddress("mu_charge", &nominal_mu_charge);
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal->SetBranchAddress("jet_e", &nominal_jet_e); 
    nominal->SetBranchAddress("jet_truthflav", &jet_truthflav);   
    nominal->SetBranchAddress("jet_truthPartonLabel", &jet_truthPartonLabel);   
    nominal->SetBranchAddress("jet_truthflavExtended", &jet_truthflavExtended); 
    nominal->SetBranchAddress("jet_isbtagged_DL1dv01_60", &jet_isbtagged_DL1dv01_60);  
    nominal->SetBranchAddress("jet_isbtagged_GN120220509_60", &jet_isbtagged_GN120220509_60);  
    nominal->SetBranchAddress("jet_GN120220509", &jet_GN120220509);
    nominal->SetBranchAddress("jet_GN120220509_pb", &jet_GN120220509_pb);
    nominal->SetBranchAddress("jet_GN120220509_pc", &jet_GN120220509_pc);
    nominal->SetBranchAddress("jet_GN120220509_pu", &jet_GN120220509_pu);
    nominal->SetBranchAddress("jet_DL1dv01", &jet_DL1dv01);
    nominal->SetBranchAddress("jet_DL1dv01_pb", &jet_DL1dv01_pb);
    nominal->SetBranchAddress("jet_DL1dv01_pc", &jet_DL1dv01_pc);
    nominal->SetBranchAddress("jet_DL1dv01_pu", &jet_DL1dv01_pu);
    nominal->SetBranchAddress("met_met", &met_met);
    nominal->SetBranchAddress("met_sumet", &met_sumet);
    nominal->SetBranchAddress("met_phi", &met_phi);
    nominal->SetBranchAddress("klfitter_logLikelihood", &klfitter_logLikelihood);
    nominal->SetBranchAddress("klfitter_selected", &klfitter_selected);
    nominal->SetBranchAddress("klfitter_minuitDidNotConverge", &klfitter_minuitDidNotConverge);
    nominal->SetBranchAddress("klfitter_model_bhad_jetIndex", &klfitter_model_bhad_jetIndex);
    nominal->SetBranchAddress("klfitter_model_blep_jetIndex", &klfitter_model_blep_jetIndex);
    nominal->SetBranchAddress("klfitter_model_lq1_jetIndex", &klfitter_model_lq1_jetIndex);
    nominal->SetBranchAddress("klfitter_model_lq2_jetIndex", &klfitter_model_lq2_jetIndex);
    nominal->SetBranchAddress("ejets_2022_gn", &ejets_2022_gn);
    nominal->SetBranchAddress("ejets_2022_dl1d", &ejets_2022_dl1d);
    nominal->SetBranchAddress("ejets_2022_gn_lowPt", &ejets_2022_gn_lowPt);
    nominal->SetBranchAddress("ejets_2022_dl1d_lowPt", &ejets_2022_dl1d_lowPt);
    nominal->SetBranchAddress("new_ejets_gn_2022", &new_ejets_gn_2022);
    nominal->SetBranchAddress("new_ejets_dl1d_2022", &new_ejets_dl1d_2022);
    nominal->SetBranchAddress("mujets_gn_2022", &mujets_gn_2022);
    nominal->SetBranchAddress("mujets_dl1d_2022", &mujets_dl1d_2022);
    nominal->SetBranchAddress("mujets_2022_gn_lowPt", &mujets_2022_gn_lowPt);
    nominal->SetBranchAddress("mujets_2022_dl1d_lowPt", &mujets_2022_dl1d_lowPt);
    nominal->SetBranchAddress("new_mujets_gn_2022", &new_mujets_gn_2022);
    nominal->SetBranchAddress("new_mujets_dl1d_2022", &new_mujets_dl1d_2022);
    nominal->SetBranchAddress("jet_SV_mass", &jet_SV_mass);
    
    /* CREATE ALL BRANCHES IN THE OUTPUT TREE */
    std::vector<int> jet_match_info;
    Int_t njets;
    
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

    /* CREATE HISTOGRAMS */
    int number_of_histograms = 16;
    TH1F** h_vector = new TH1F*[number_of_histograms];

    // Truth histograms
    h_vector[0] = new TH1F("h_MC_Whad_minv", "h_MC_Whad_minv", 50, 20, 120);
    h_vector[1] = new TH1F("h_MC_Wlep_minv", "h_MC_Wlep_minv", 50, 20, 120);
    h_vector[2] = new TH1F("h_MC_thad_minv", "h_MC_thad_minv", 50, 120, 220);
    h_vector[3] = new TH1F("h_MC_tlep_minv", "h_MC_tlep_minv", 30, 120, 220);
    h_vector[4] = new TH1F("h_MC_wjets_angle", "h_MC_wjets_angle", 30, 0, 3.15);
    h_vector[5] = new TH1F("h_MC_wleptons_angle", "h_MC_wleptons_angle", 30, 0, 3.15);
    h_vector[6] = new TH1F("h_MC_twhad_angle", "h_MC_twhad_angle", 30, 0, 3.15);
    h_vector[7] = new TH1F("h_MC_twlep_angle", "h_MC_twlep_angle", 30, 0, 3.15);
    h_vector[8] = new TH1F("h_MC_bbbar_angle", "h_MC_bbbar_angle", 30, 0, 3.15);
	h_vector[9] = new TH1F("h_MC_blepton_angle", "h_MC_blepton_angle", 30, 0, 3.15);

    h_vector[10] = new TH1F("h_Whad_minv", "h_Whad_minv", 50, 20, 120);
    h_vector[11] = new TH1F("h_thad_minv", "h_thad_minv", 50, 120, 220);
    h_vector[12] = new TH1F("h_wjets_angle", "h_wjets_angle", 30, 0, 3.15);
    h_vector[13] = new TH1F("h_twhad_angle", "h_twhad_angle", 30, 0, 3.15);
    h_vector[14] = new TH1F("h_bbbar_angle", "h_bbbar_angle", 30, 0, 3.15);
	h_vector[15] = new TH1F("h_blepton_angle", "h_blepton_angle", 30, 0, 3.15);

    // Getvector of jets
    // Create vector of Lorentz objects
    // Create vector of matched labels
    // Match jets to each truth object
    // Set unmatched jets to unmatched value
    Long64_t nentries = nominal->GetEntries();
    Double_t DeltaR_max = 0.4;
    int selected = 0;
    int matched = 0;
	bool tagged = 0;
    std::cout << "The total number of events is " << nentries << std::endl;
    
    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        
        nominal->GetEntry(i);
        truth->GetEntryWithIndex(runNumber,eventNumber);
        
        updateProgressBar(i+1, nentries);
		njets = nominal_jet_pt->size();

        if (runNumber!=MC_runNumber) continue;
        if (eventNumber!=MC_eventNumber) continue;

        bool W_t_is_hadronic, W_tbar_is_hadronic;
        if (!select_hadronic_decay(&W_t_is_hadronic, &W_tbar_is_hadronic)) continue;

        if (!selection_criteria_truth(W_t_is_hadronic, W_tbar_is_hadronic)) continue;
        
		tagged = false;
	    for (int jet{0}; jet<njets; jet++) {
		    if ((*jet_isbtagged_DL1dv01_60)[jet] == true){
			    tagged = true;
			}
	    }
        if (!tagged) continue;
        selected+=1;

        /* CREATE TRUTH LORENTZ VECTORS */
        TLorentzVector truth_b_t;
        truth_b_t.SetPtEtaPhiM(MC_b_t_pt, MC_b_t_eta, MC_b_t_phi, MC_b_t_mass);
        TLorentzVector truth_b_tbar;
        truth_b_tbar.SetPtEtaPhiM(MC_b_tbar_pt, MC_b_tbar_eta, MC_b_tbar_phi, MC_b_tbar_mass);
        TLorentzVector truth_q1_w;
        TLorentzVector truth_q2_w;
        TLorentzVector truth_l_w;
        TLorentzVector truth_n_w;
        if (W_t_is_hadronic) {
            truth_q1_w.SetPtEtaPhiM(MC_W1_t_pt, MC_W1_t_eta, MC_W1_t_phi, MC_W1_t_mass);
            truth_q2_w.SetPtEtaPhiM(MC_W2_t_pt, MC_W2_t_eta, MC_W2_t_phi, MC_W2_t_mass);
        } else {
            truth_n_w.SetPtEtaPhiM(MC_W1_t_pt, MC_W1_t_eta, MC_W1_t_phi, MC_W1_t_mass);
            truth_l_w.SetPtEtaPhiM(MC_W2_t_pt, MC_W2_t_eta, MC_W2_t_phi, MC_W2_t_mass);
        }
        if (W_tbar_is_hadronic) {
            truth_q1_w.SetPtEtaPhiM(MC_W1_tbar_pt, MC_W1_tbar_eta, MC_W1_tbar_phi, MC_W1_tbar_mass);
            truth_q2_w.SetPtEtaPhiM(MC_W2_tbar_pt, MC_W2_tbar_eta, MC_W2_tbar_phi, MC_W2_tbar_mass);
        } else {
            truth_l_w.SetPtEtaPhiM(MC_W1_tbar_pt, MC_W1_tbar_eta, MC_W1_tbar_phi, MC_W1_tbar_mass);
            truth_n_w.SetPtEtaPhiM(MC_W2_tbar_pt, MC_W2_tbar_eta, MC_W2_tbar_phi, MC_W2_tbar_mass);
        }
        
        /* MATCH TRUTH AND NOMINAL CHARGED LEPTON */
        TLorentzVector nominal_lepton;
        if (nominal_el_pt->size()==1) {
            nominal_lepton.SetPtEtaPhiM((*nominal_el_pt)[0], (*nominal_el_eta)[0], (*nominal_el_phi)[0], ELECTRON_MASS);
        } else if (nominal_mu_pt->size()==1) {
            nominal_lepton.SetPtEtaPhiM((*nominal_mu_pt)[0], (*nominal_mu_eta)[0], (*nominal_mu_phi)[0], MUON_MASS);
        } else {
            continue;
        }
        
        if (nominal_lepton.DeltaR(truth_l_w)>0.4) {
            continue;
        }

        /* RECO MATCHING WITH TRUTH */
        TLorentzVector* truth_lorentz_v = new TLorentzVector[4];
        truth_lorentz_v[0] = truth_b_t;
        truth_lorentz_v[1] = truth_b_tbar;
        truth_lorentz_v[2] = truth_q1_w;
        truth_lorentz_v[3] = truth_q2_w;

        /* Create nominal Lorentz vectors */
        TLorentzVector* nominal_jet_lorentz_v = new TLorentzVector[njets];
        for (int j{0}; j<njets; j++) {
            TLorentzVector temp;
            temp.SetPtEtaPhiE((*nominal_jet_pt)[j], (*nominal_jet_eta)[j], (*nominal_jet_phi)[j], (*nominal_jet_e)[j]);
            nominal_jet_lorentz_v[j] = temp;
        }

        /* Create matrix holding the matched indices and the DeltaR values */
        Double_t holder;
        int** match_indices = new int*[4];
        Double_t** match_deltas = new Double_t*[4];
        for (int m=0; m<4; m++) {
            match_indices[m] = new int[njets];
            set_vector_to_zero(match_indices[m], njets);
            match_deltas[m] = new Double_t[njets];
            set_vector_to_zero(match_deltas[m], njets);
        }
        
        /* Loop over all truths and over all nominal to find matches */
        bool match_not_found = false;
        for (int m{0}; m<4; m++) {
            for (int j{0}; j<njets; j++) {
                holder = truth_lorentz_v[m].DeltaR(nominal_jet_lorentz_v[j]);
                if (holder<DeltaR_max) {
                    match_indices[m][j]=1;
                    match_deltas[m][j]=holder;
                }
            }
            if (vector_sum(match_indices[m], njets)==0) {match_not_found=true;}
        }
        if (match_not_found) continue;
        


        /* Find the best matching of truth and nominal if possible
           If a match is found then it is pushed to the array containing all events matched */
        int* jet_match_holder = new int[njets];
        set_vector_to_zero(jet_match_holder, njets);
        if(sort_algorithm(jet_match_holder, match_indices, match_deltas, njets)) {
            matched +=1;
            jet_match_info = array_to_vector(jet_match_holder, njets);
            output_tree->Fill();

            /* FILL MC HISTOGRAMS */
            TLorentzVector truth_Whad = truth_q1_w + truth_q2_w;
            TLorentzVector truth_Wlep = truth_l_w + truth_n_w;
            h_vector[0]->Fill(truth_Whad.M()/1000, MC_weight);
            h_vector[1]->Fill(truth_Wlep.M()/1000, MC_weight);
            h_vector[4]->Fill(truth_q1_w.Angle(truth_q2_w.Vect()), MC_weight);
            h_vector[5]->Fill(truth_l_w.Angle(truth_n_w.Vect()), MC_weight);
            h_vector[8]->Fill(truth_b_t.Angle(truth_b_tbar.Vect()), MC_weight);

            if (W_t_is_hadronic) {
                h_vector[2]->Fill((truth_Whad + truth_b_t).M()/1000, MC_weight);
                h_vector[6]->Fill(truth_Whad.Angle(truth_b_t.Vect()), MC_weight);
            } else {
                h_vector[3]->Fill((truth_Wlep + truth_b_t).M()/1000, MC_weight);
                h_vector[7]->Fill(truth_Wlep.Angle(truth_b_t.Vect()), MC_weight);
				h_vector[9]->Fill(truth_b_t.Angle(truth_l_w.Vect()), MC_weight);
            }
            if (W_tbar_is_hadronic) {
                h_vector[2]->Fill((truth_Whad + truth_b_tbar).M()/1000, MC_weight);
                h_vector[6]->Fill(truth_Wlep.Angle(truth_b_tbar.Vect()), MC_weight);
            } else {
                h_vector[3]->Fill((truth_Wlep + truth_b_tbar).M()/1000, MC_weight);
                h_vector[7]->Fill(truth_Whad.Angle(truth_b_tbar.Vect()), MC_weight);
				h_vector[9]->Fill(truth_b_tbar.Angle(truth_l_w.Vect()), MC_weight);
            }

            /* FILL RECO HISTOGRAMS */
            int* indices = new int[4];
            get_indices(indices, jet_match_holder, njets);

            TLorentzVector nominal_Whad = nominal_jet_lorentz_v[indices[2]] + nominal_jet_lorentz_v[indices[3]];
            h_vector[10]->Fill(nominal_Whad.M()/1000, weight);
            h_vector[12]->Fill((nominal_jet_lorentz_v[indices[2]]).Angle((nominal_jet_lorentz_v[indices[3]]).Vect()), weight);
            h_vector[14]->Fill((nominal_jet_lorentz_v[indices[0]]).Angle((nominal_jet_lorentz_v[indices[1]]).Vect()), weight);
            if (W_t_is_hadronic) {
                h_vector[11]->Fill((nominal_jet_lorentz_v[indices[0]] + nominal_Whad).M()/1000, weight);
                h_vector[13]->Fill((nominal_jet_lorentz_v[indices[0]]).Angle(nominal_Whad.Vect()), weight);
            } else {
				h_vector[15]->Fill((nominal_jet_lorentz_v[indices[0]]).Angle(nominal_lepton.Vect()), weight);
			}
            if (W_tbar_is_hadronic) {
                h_vector[11]->Fill((nominal_jet_lorentz_v[indices[1]] + nominal_Whad).M()/1000, weight);
                h_vector[13]->Fill((nominal_jet_lorentz_v[indices[1]]).Angle(nominal_Whad.Vect()), weight);
            } else {
				h_vector[15]->Fill((nominal_jet_lorentz_v[indices[1]]).Angle(nominal_lepton.Vect()), weight);
			}
        }

       /* Correctly disposes of dynamically allocated arrays */
        for (int j{0}; j<4; j++) {
            delete[] match_indices[j];
            delete[] match_deltas[j];
        }
        delete[] match_indices;
        delete[] match_deltas;
        delete[] jet_match_holder;
        delete[] nominal_jet_lorentz_v;
        delete[] truth_lorentz_v;
        jet_match_holder = nullptr;

    }
    
    std::cout << "The total number of selected events is " << selected << std::endl;
    std::cout << "The number of matched events is " << matched << std::endl;
    std::cout << "The number of directly matched events is " << directly_matched << std::endl;
    std::cout << "The number of indirectly matched events is " << indirectly_matched << std::endl;
    std::cout << "The number of unmatchable events is " << unmatchable << std::endl;
    std::cout << "The number of more complex events is " << more_complex << std::endl;

    output_file->cd();
    //output_file->Delete("matched;*");
    
    format_hist(h_vector[0], kBlue, "MC t_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.5);
    format_hist(h_vector[1], kBlue, "MC t_lep Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.5);
    format_hist(h_vector[2], kBlue, "MC W_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.5);
    format_hist(h_vector[3], kBlue, "MC W_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.5);
    format_hist(h_vector[4], kBlue, "MC W Jets Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(h_vector[5], kBlue, "MC W Leptons Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(h_vector[6], kBlue, "MC W_had to b Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(h_vector[7], kBlue, "MC W_lep to b Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(h_vector[8], kBlue, "MC Top Jets Angle Distribution", "Angle", "Entries", 0.1);
	format_hist(h_vector[9], kBlue, "MC b-lepton Angle Distribution", "Angle", "Entries", 0.1);

    format_hist(h_vector[10], kBlue, "W_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.1);
    format_hist(h_vector[11], kBlue, "t_had Invariant Mass Distribution", "m_inv [GeV/c^2]", "Entries", 0.08);
    format_hist(h_vector[12], kBlue, "W Jets Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(h_vector[13], kBlue, "W_had to b Angle Distribution", "Angle", "Entries", 0.1);
    format_hist(h_vector[14], kBlue, "Top Jets Angle Distribution", "Angle", "Entries", 0.1);
	format_hist(h_vector[15], kBlue, "b-lepton Angle Distribution", "Angle", "Entries", 0.1);

    for (int k{0}; k<number_of_histograms; k++) {
        h_vector[k]->Write();
    }
    
    std::cout << "Here" << std::endl;
    output_tree->Write();

    std::cout << "Here" << std::endl;
    output_file->Close();
    delete output_file;
    
    delete[] h_vector;
    
    return 0;
}
