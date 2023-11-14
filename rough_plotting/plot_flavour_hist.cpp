/*	plot_flavour_hist.cpp
 *
 *		This code is used to compare the distribution of dynamic variables
 * 		according to their flavour.
 *		The code creates three histograms and the fills each one, using the
 *		MC weights. It fills each of the histograms with the jets of the
 *		corresponding flavour. The histograms are plotted in one canvas and
 *		is saved in the path given by the user.
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
#include <TROOT.h>
#include <TLegend.h>
#include <vector>
#include <cstdlib>
#include "histogram.h"

/* MAIN CODE */
int main(){
	/* Open data*/
	TFile *f = new TFile("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root","READ");
	TTree *nominal = (TTree*)f->Get("nominal");

    /* Create variable and assign address */
	std::vector<float> *variable = nullptr;
	std::vector<int> *flavour = nullptr;
	Float_t weight;
	nominal->SetBranchAddress("jet_GN120220509",&variable);
	nominal->SetBranchAddress("jet_truthflav", &flavour);
	nominal->SetBranchAddress("weight_mc", &weight);

    /* Generete canvas and histograms */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *b_hist = new TH1F("b_hist","b jets",50,-10,18);
	TH1F *c_hist = new TH1F("c_hist","c jets",50,-10,18);
	TH1F *l_hist = new TH1F("l_hist","light jets",50,-10,18);
	std::vector<TH1F*> histograms{l_hist, c_hist, b_hist};

	Long64_t nentries = nominal->GetEntries();
	for (Long64_t i=0;i<nentries;i++) {
      	nominal->GetEntry(i);

		int length = variable->size();
		for (int j=0;j<length;j++){
			if ((*flavour)[j]==0) histograms[0]->Fill((*variable)[j],weight);
			else if ((*flavour)[j]==4) histograms[1]->Fill((*variable)[j],weight);
			else if ((*flavour)[j]==5) histograms[2]->Fill((*variable)[j],weight);
		}
	}

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
	canvas->SetGrid();

	/* Format histograms*/
	char 	*title{"Weighted Jet GN Discriminant Distribution"},
			*x_label{"GN Discriminant"},
			*y_label{"Entries"};
	format_hist(histograms[0], kRed, title, x_label, y_label, 0.2);
	format_hist(histograms[1], kGreen, title, x_label, y_label, 0.2);
	format_hist(histograms[2], kBlue, title, x_label, y_label, 0.2);

	/* Create legend */
    TLegend *legend = new TLegend(0.7, 0.68, 0.9, 0.88);
    std::vector<char*> labels {"light jets", "c-jets", "b-jets"};
	format_legend(legend, labels, histograms);

	/* Draw and save histogram */
	l_hist->Draw("HIST");
	c_hist->Draw("HIST same");
	b_hist->Draw("HIST same");
	legend->Draw("same");
	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/weighted/jet_GN.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);

	return 0;
}
