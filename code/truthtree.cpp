/* IMPORT STATEMENTS */
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TVector3.h>
#include <iostream>
#include <TROOT.h>
#include <TLegend.h>
#include <vector>
#include <cstdlib>
#include "histogram.h"
#include <math.h>
#include <TLorentzVector.h>

int main(){

    Float_t Wdecay_pt;
    Float_t Wdecay_phi;
    Float_t Wdecay_eta;
    Float_t Wdecay_m;

    TChain * chain = new TChain("truth","");

	chain->Add("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root");
	chain->Print();

    chain->SetBranchAddress("MC_Wdecay1_from_tbar_pt", &Wdecay_pt);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_phi", &Wdecay_phi);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_eta", &Wdecay_eta);
    chain->SetBranchAddress("MC_Wdecay1_from_tbar_m", &Wdecay_m);
	// Create the hash table that connect
	//    runNumber (x) eventNumber
	// to the tree entry number. A return code less than 0 indicated failure.
	if (chain->BuildIndex("eventNumber") < 0){
		std::cerr << "Could not build particle level index!" << std::endl;
		std::abort();
	}

    TLorentzVector Wdecay_particle;
	Wdecay_particle.SetPtEtaPhiM(Wdecay_pt, Wdecay_eta, Wdecay_phi, Wdecay_m);

    // Calculate ΔR
    Float_t DeltaR = Wdecay_particle.DeltaR(Wdecay_particle);

    return 0;
}