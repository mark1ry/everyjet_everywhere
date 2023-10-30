// Open nominal tree -select appropiate variables
// Open truth tree -select appropriate variables
// Loop over events
//      Calculate Lorentz vector for events in both trees
//      Match each true event with the closest jet using DeltaR
//      Store information of each event in a vector
// Store match information in a new branch (i.e. will need to create a new tree)

#include <TLatex.h>
#include <TMathText.h>
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

UInt_t MC_runNumber;
ULong64_t MC_eventNumber;

Float_t weight;



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
    chain->SetBranchAddress("weight_mc",&weight);
    chain->SetBranchAddress("runNumber",&MC_runNumber);
    chain->SetBranchAddress("eventNumber", &MC_eventNumber);

    TTree * truth;
    truth = chain;

    return truth;
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
    /* Open data*/
    char* file_path= "/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000003.output_pflow.root";
	TFile *f = new TFile(file_path, "READ");
	//TTree *nominal = (TTree*)f->Get("nominal");
    TTree *truth = (TTree*)f->Get("truth");

    //TTree* truth = 0;
    //truth = SetTruthTree(file_path);

    UInt_t runNumber;
    ULong64_t eventNumber;
    std::vector<float>* nominal_jet_pt = 0;
    std::vector<float>* nominal_jet_phi = 0;
    std::vector<float>* nominal_jet_eta = 0;
    std::vector<float>* nominal_jet_e = 0;    

    /*
    nominal->SetBranchAddress("eventNumber", &eventNumber);
    nominal->SetBranchAddress("runNumber", &runNumber);
    nominal->SetBranchAddress("jet_pt", &nominal_jet_pt);
    nominal->SetBranchAddress("jet_phi", &nominal_jet_phi);
    nominal->SetBranchAddress("jet_eta", &nominal_jet_eta);
    nominal->SetBranchAddress("jet_e", &nominal_jet_e);
    */

    // Getvector of jets
    // Create vector of Lorentz objects
    // Create vector of matched labels
    // Match jets to each truth object
    // Set unmatched jets to unmatched value

    //Long64_t nentries = nominal->GetEntries();
    truth->SetBranchAddress("MC_Wdecay1_from_t_pdgId", &MC_W1_t_PID);
    truth->SetBranchAddress("MC_Wdecay1_from_t_m", &MC_W1_t_mass);
    truth->SetBranchAddress("MC_Wdecay1_from_t_phi", &MC_W1_t_phi);
    truth->SetBranchAddress("MC_Wdecay1_from_t_eta", &MC_W1_t_eta);
    truth->SetBranchAddress("MC_Wdecay1_from_t_pt", &MC_W1_t_pt);
    truth->SetBranchAddress("MC_Wdecay2_from_t_pdgId", &MC_W2_t_PID);
    truth->SetBranchAddress("MC_Wdecay2_from_t_m", &MC_W2_t_mass);
    truth->SetBranchAddress("MC_Wdecay2_from_t_phi", &MC_W2_t_phi);
    truth->SetBranchAddress("MC_Wdecay2_from_t_eta", &MC_W2_t_eta);
    truth->SetBranchAddress("MC_Wdecay2_from_t_pt", &MC_W2_t_pt);
    truth->SetBranchAddress("MC_Wdecay1_from_tbar_pdgId", &MC_W1_tbar_PID);
    truth->SetBranchAddress("MC_Wdecay1_from_tbar_m", &MC_W1_tbar_mass);
    truth->SetBranchAddress("MC_Wdecay1_from_tbar_phi", &MC_W1_tbar_phi);
    truth->SetBranchAddress("MC_Wdecay1_from_tbar_eta", &MC_W1_tbar_eta);
    truth->SetBranchAddress("MC_Wdecay1_from_tbar_pt", &MC_W1_tbar_pt);
    truth->SetBranchAddress("MC_Wdecay2_from_tbar_pdgId", &MC_W2_tbar_PID);
    truth->SetBranchAddress("MC_Wdecay2_from_tbar_m", &MC_W2_tbar_mass);
    truth->SetBranchAddress("MC_Wdecay2_from_tbar_phi", &MC_W2_tbar_phi);
    truth->SetBranchAddress("MC_Wdecay2_from_tbar_eta", &MC_W2_tbar_eta);
    truth->SetBranchAddress("MC_Wdecay2_from_tbar_pt", &MC_W2_tbar_pt);
    truth->SetBranchAddress("MC_b_from_t_m", &MC_b_t_mass);
    truth->SetBranchAddress("MC_b_from_t_phi", &MC_b_t_phi);
    truth->SetBranchAddress("MC_b_from_t_eta", &MC_b_t_eta);
    truth->SetBranchAddress("MC_b_from_t_pt", &MC_b_t_pt);
    truth->SetBranchAddress("MC_b_from_tbar_m", &MC_b_tbar_mass);
    truth->SetBranchAddress("MC_b_from_tbar_phi", &MC_b_tbar_phi);
    truth->SetBranchAddress("MC_b_from_tbar_eta", &MC_b_tbar_eta);
    truth->SetBranchAddress("MC_b_from_tbar_pt", &MC_b_tbar_pt);
    truth->SetBranchAddress("weight_mc", &weight);
    truth->SetBranchAddress("runNumber", &MC_runNumber);
    truth->SetBranchAddress("eventNumber", &MC_eventNumber);


    Long64_t nentries = truth->GetEntries();
    std::cout<<"The number of entries is "<<nentries<<std::endl;
    Long64_t hadronic_w = 0; 

    int q1_flavour = 0;
    int q2_flavour = 0;
    int c = 4;
    int s = 3;
    int b = 5;
    int u = 2;
    int d = 1;

    /*
    // Find the minimum and maximum values of MC_eventNumber
    Float_t minMC_eventNumber = std::numeric_limits<Float_t>::max();
    Float_t maxMC_eventNumber = std::numeric_limits<Float_t>::lowest();

    for (Long64_t i = 0; i < nentries; i++) {
        if (i==0) { std::cout << "Entered the first loop" << std::endl; }
        truth->GetEntry(i);
        //truth->GetEntryWithIndex(runNumber, eventNumber);
        updateProgressBar(i+1, nentries);

        if (MC_eventNumber < minMC_eventNumber) {
            minMC_eventNumber = MC_eventNumber;
        }
        if (MC_eventNumber > maxMC_eventNumber) {
            maxMC_eventNumber = MC_eventNumber;
        }
    }

    // Calculate the range and use it as the number of bins
    Float_t range = maxMC_eventNumber - minMC_eventNumber;
    std::cout<<"Upper limit is "<<maxMC_eventNumber<<std::endl;
    Int_t numBins = static_cast<Int_t>(range);
    */

    /* Generete canvas and histogram */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *hist1   = new TH1F("hist1","MC b from t distribution",50, 0, 300);
    TH1F *hist2   = new TH1F("hist2","MC b from t distribution",50, 0, 300);

    std::vector<TH1F*> histograms{hist1, hist2};
    
    for(Long64_t i=0; i<nentries; i++) {
        if (i==0) { std::cout << "Entered the second loop" << std::endl; }
        truth->GetEntry(i);
        updateProgressBar(i+1, nentries);
        //truth->GetEntryWithIndex(runNumber,eventNumber);

        bool W_t_is_hadronic, W_tbar_is_hadronic;
        if (!select_hadronic_decay(&W_t_is_hadronic, &W_tbar_is_hadronic)) continue;


        /* Create truth Lorentz vectors*/
        
        /*TLorentzVector truth_b_t;
        truth_b_t.SetPtEtaPhiM(MC_b_t_pt, MC_b_t_eta, MC_b_t_phi, MC_b_t_mass);
        TLorentzVector truth_b_tbar;
        truth_b_tbar.SetPtEtaPhiM(MC_b_tbar_pt, MC_b_tbar_eta, MC_b_tbar_phi, MC_b_tbar_mass);*/
        TLorentzVector truth_q1_w;
        TLorentzVector truth_q2_w;
        TLorentzVector truth_q_w_t;    
        TLorentzVector truth_q_w_tbar; 


        if (W_t_is_hadronic) {
            truth_q1_w.SetPtEtaPhiM(MC_W1_t_pt, MC_W1_t_eta, MC_W1_t_phi, MC_W1_t_mass);
            truth_q2_w.SetPtEtaPhiM(MC_W2_t_pt, MC_W2_t_eta, MC_W2_t_phi, MC_W2_t_mass);
            truth_q_w_t = truth_q1_w + truth_q2_w;
            //q1_flavour = MC_W1_t_PID;
            //q2_flavour = MC_W2_t_PID;
            //std::cout<<q1_flavour<<std::endl;


            
            /*if(((q1_flavour == -d))){
                histograms[0]->Fill((truth_q1_w.Pt()/1000), weight);
            }

            if(((q1_flavour == -s))){
                histograms[1]->Fill((truth_q1_w.Pt()/1000), weight);
            }*/

            /*
            if(((q2_flavour == -d))){
                histograms[0]->Fill((MC_W2_t_pt/1000), weight);
            }


            if(((q2_flavour == -s))){
                histograms[1]->Fill((MC_W2_t_pt/1000), weight);
            }
            */


            //hist->Fill((truth_q_w_t.Pt()/1000), weight);
        }
        if (W_tbar_is_hadronic) {
            truth_q1_w.SetPtEtaPhiM(MC_W1_tbar_pt, MC_W1_tbar_eta, MC_W1_tbar_phi, MC_W1_tbar_mass);
            truth_q2_w.SetPtEtaPhiM(MC_W2_tbar_pt, MC_W2_tbar_eta, MC_W2_tbar_phi, MC_W2_tbar_mass);
            truth_q_w_tbar = truth_q1_w + truth_q2_w;
            q1_flavour = MC_W1_t_PID;
            q2_flavour = MC_W2_t_PID;

            if(((q2_flavour == -c))){
                histograms[0]->Fill((truth_q1_w.Pt()/1000), weight);
            }

            if(((q2_flavour == -u))){
                histograms[1]->Fill((truth_q1_w.Pt()/1000), weight);
            }

            //histograms[0]->Fill((truth_q1_w.Pt()/1000), weight);
            //histograms[1]->Fill((truth_q2_w.Pt()/1000), weight);
            
            hadronic_w++;
        } 
    }

    //std::cout<<"The number of hadronically decaying w from t is "<<hadronic_w<<std::endl;

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.08); //before it was 0.05
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
    canvas->SetGrid();
	canvas->Update();

    /* Format histograms*/
	char 	*title{"Hadronic Decay of W from t bar Transverse Momentum Distribution File 000003"},
			*x_label{"p_T (GeV/c)"},
			*y_label{"Entries"};
	format_hist(histograms[0], kBlue, title, x_label, y_label, 0.12);
	format_hist(histograms[1], kGreen, title, x_label, y_label, 0.12);


	/* Create legend */
    TLegend *legend = new TLegend(0.7, 0.68, 0.9, 0.88);
    std::vector<char*> labels {"#bar{c}", " #bar{u}"};
	format_legend(legend, labels, histograms);

	/* Draw and save histogram */
	hist1->Draw("HIST");
	hist2->Draw("HIST same");
	
	legend->Draw("same");

	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/truth/cu_w_tbar_000003.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);
    

    return 0;
}
