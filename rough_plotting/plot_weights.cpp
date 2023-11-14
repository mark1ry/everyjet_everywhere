/* 	plot_weights.cpp
 *		
 *		This code is used in order to compare the distribution of a dynamic variable applying the
 *		monte carlo weight and not applying it.
 *		This script first produces two TH1F class histograms. A root file is imported and a branch
 *		containing a kinematic variable is selected. This variable is then filled straightaway in one
 *		of the histograms, and in the other histogram it is filled taking into account its MC weight.
 *		These histograms are plotted in the same canvas and saved in the path designated by the user.
 *
 *		Marc Oriol and Eleni Togia - 23th of October 2023
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* IMPORT STATEMENTS */
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
#include <cstdlib>
#include "histogram.h"

/* MAIN CODE*/
int main(){

	/* Open data*/
	TFile *f = new TFile("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root","READ");
	TTree *nominal = (TTree*)f->Get("nominal");

    /* Create variables and assign address */
	std::vector<float> *variable = nullptr;
	nominal->SetBranchAddress("jet_pt",&variable);
    Float_t weight;
	nominal->SetBranchAddress("weight_mc",&weight);
    

    /* Generete canvas and histograms */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *w_hist = new TH1F("w_hist","Weighted",70,0,350);
	TH1F *nw_hist = new TH1F("nw_hist","Not-Weighted",70,0,350);
	std::vector<TH1F*> histograms{nw_hist, w_hist};

	Long64_t nentries = nominal->GetEntries();
	for (Long64_t i=0;i<nentries;i++) {
      	nominal->GetEntry(i);
		int length = variable->size();
		for (int j=0;j<length;j++){
			histograms[0]->Fill((*variable)[j]/1000); // Unweighted hist
			histograms[1]->Fill((*variable)[j]/1000, weight); // Weighted hist
		}
	}

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
	canvas->SetGrid();

	/* Format histograms*/
	char 	*title{"Jet Transverse Momentum Weighted vs Unweighted Distribution"},
			*x_label{"Weights"},
			*y_label{"Entries (Normalized)"};
	format_hist(histograms[0], kBlue, title, x_label, y_label, 0.12);
	format_hist(histograms[1], kGreen, title, x_label, y_label, 0.12);

	/* Create legend */
    TLegend *legend = new TLegend(0.7, 0.68, 0.9, 0.88);
    std::vector<char*> labels {"Unweighted", "Weighted"};
	format_legend(legend, labels, histograms);

	/* Draw and save histograms */
	nw_hist->Draw("HIST");
	w_hist->Draw("HIST same");
	legend->Draw("same");
	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/weighted/jet_pt_weight_compare.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);

	return 0;
}