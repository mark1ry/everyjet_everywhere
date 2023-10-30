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

TTree * SetTruthTree(char* file_path) {
    TChain *chain = new TChain("truth", "");
    chain->Add("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000003.output_pflow.root");


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
    } else if (W_tbar) {
        if (MC_W1_tbar_pt<20000 || MC_W1_tbar_eta>2.5 || MC_W1_tbar_eta<-2.5) {return false;}
        if (MC_W2_tbar_pt<20000 || MC_W2_tbar_eta>2.5 || MC_W2_tbar_eta<-2.5) {return false;}
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
        if (vector_sum(matched_jets, length)==10) return true;
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
                    return true;
                }
            }
        }
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


int main () {
    std::cout << "code has been initiated" << std::endl;
    /* Open data*/
    char* file_path= "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000003.output_pflow.root";
	TFile *f = new TFile(file_path, "READ");
	TTree *nominal = (TTree*)f->Get("nominal");
    TTree* truth = 0;
    truth = SetTruthTree(file_path);

    UInt_t runNumber;
    ULong64_t eventNumber;
    std::vector<float>* nominal_jet_pt = 0;
    std::vector<float>* nominal_jet_phi = 0;
    std::vector<float>* nominal_jet_eta = 0;
    std::vector<float>* nominal_jet_e = 0;    

    nominal->SetBranchAddress("eventNumber", &eventNumber);
    nominal->SetBranchAddress("runNumber", &runNumber);
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal->SetBranchAddress("jet_e", &nominal_jet_e);

    // Getvector of jets
    // Create vector of Lorentz objects
    // Create vector of matched labels
    // Match jets to each truth object
    // Set unmatched jets to unmatched value
    Long64_t nentries = nominal->GetEntries();
    Double_t DeltaR_max = 0.4;
    std::vector<std::vector<int>> matched_events;
    int counter = 0;
    int matched = 0;
    std::cout << "The total number of events is " << nentries << std::endl;

    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the loop" << std::endl; }
        nominal->GetEntry(i);
        truth->GetEntryWithIndex(runNumber,eventNumber);
        
        updateProgressBar(i+1, nentries);

        if (runNumber!=MC_runNumber) continue;
        if (eventNumber!=MC_eventNumber) continue;

        bool W_t_is_hadronic, W_tbar_is_hadronic;
        if (!select_hadronic_decay(&W_t_is_hadronic, &W_tbar_is_hadronic)) continue;

        if (!selection_criteria_truth(W_t_is_hadronic, W_tbar_is_hadronic)) continue;
        counter+=1;

        /* Create truth Lorentz vectors*/
        TLorentzVector truth_b_t;
        truth_b_t.SetPtEtaPhiM(MC_b_t_pt, MC_b_t_eta, MC_b_t_phi, MC_b_t_mass);
        TLorentzVector truth_b_tbar;
        truth_b_tbar.SetPtEtaPhiM(MC_b_tbar_pt, MC_b_tbar_eta, MC_b_tbar_phi, MC_b_tbar_mass);
        TLorentzVector truth_q1_w;
        TLorentzVector truth_q2_w;
        TLorentzVector truth_l1_w;
        TLorentzVector truth_l2_w;
        if (W_t_is_hadronic) {
            truth_q1_w.SetPtEtaPhiM(MC_W1_t_pt, MC_W1_t_eta, MC_W1_t_phi, MC_W1_t_mass);
            truth_q2_w.SetPtEtaPhiM(MC_W2_t_pt, MC_W2_t_eta, MC_W2_t_phi, MC_W2_t_mass);
        } else {
            truth_l1_w.SetPtEtaPhiM(MC_W1_t_pt, MC_W1_t_eta, MC_W1_t_phi, MC_W1_t_mass);
            truth_l2_w.SetPtEtaPhiM(MC_W2_t_pt, MC_W2_t_eta, MC_W2_t_phi, MC_W2_t_mass);
        }
        if (W_tbar_is_hadronic) {
            truth_q1_w.SetPtEtaPhiM(MC_W1_tbar_pt, MC_W1_tbar_eta, MC_W1_tbar_phi, MC_W1_tbar_mass);
            truth_q2_w.SetPtEtaPhiM(MC_W2_tbar_pt, MC_W2_tbar_eta, MC_W2_tbar_phi, MC_W2_tbar_mass);
        } else {
            truth_l1_w.SetPtEtaPhiM(MC_W1_tbar_pt, MC_W1_tbar_eta, MC_W1_tbar_phi, MC_W1_tbar_mass);
            truth_l2_w.SetPtEtaPhiM(MC_W2_tbar_pt, MC_W2_tbar_eta, MC_W2_tbar_phi, MC_W2_tbar_mass);
        }

        TLorentzVector* truth_lorentz_v = new TLorentzVector[4];
        truth_lorentz_v[0] = truth_b_t;
        truth_lorentz_v[1] = truth_b_tbar;
        truth_lorentz_v[2] = truth_q1_w;
        truth_lorentz_v[3] = truth_q2_w;

        /* Create nominal Lorentz vectors */
        int njets = nominal_jet_pt->size();
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
            matched_events.push_back(array_to_vector(jet_match_holder, njets));
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
    
    std::cout << "The total number of  events is " << counter << std::endl;
    std::cout << "The number of matched jets is " << matched << std::endl;

    /* Sanity check */
    /*for (int i{0}; i<200; i++) {
        std::cout << "[ ";
        for (int j{0}; j<matched_events[i].size(); j++) {
            std::cout << matched_events[i][j] << " ";
        }
        std::cout << "]" << " \n";
    }*/
    
    return 0;
}