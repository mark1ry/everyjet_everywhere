/*	plot_single_hist.cpp
 *
 *		This code is used to plot the distribution of one dynamic variable.
 *		The code creates one histograms and then fills it, using the MC weights.
 *		The histogram is plotted in one canvas and is saved in the path given by the user.
 *
 *	Marc Oriol & Eleni Togia - 12th October 2023
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* IMPORT STATEMENTS */
#include <TFile.h>
#include <TTree.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TVector3.h>
#include <iostream>
#include <string>
#include <TROOT.h>
#include <cstdlib>
#include "histogram.h"

/* MAIN CODE */
int main(){

    /* Open data*/
	TFile *f = new TFile("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root","READ");
	TTree *nominal = (TTree*)f->Get("nominal");

    /* Create variable and assign address */
	std::vector<float> *variable = nullptr;
	nominal->SetBranchAddress("mu_pt",&variable);
	Float_t weight;
	nominal->SetBranchAddress("weight_mc",&weight);

    /* Generete canvas and histogram */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *hist   = new TH1F("hist","lepton distribution",50,0,300);

    /* Fill histogram */
	Long64_t nentries = nominal->GetEntries();
	for (Long64_t i=0;i<nentries;i++) {
      	nominal->GetEntry(i);

		int length = variable->size();
		for (int j{0}; j<length; j++) {
			hist->Fill((*variable)[j]/1000, weight);
		}
	}

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
    canvas->SetGrid();
	canvas->Update();

    /* Format histogram, draw and save */
	char 	*title{"Muon Transvers Momentum Distribution"},
			*x_label{"p_T (GeV/c)"},
			*y_label{"Entries"};
    format_hist(hist, kBlue, title, x_label, y_label, 0.125);
	hist->Draw("HIST");
	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/weighted/mu_pt.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);

	return 0;
}
