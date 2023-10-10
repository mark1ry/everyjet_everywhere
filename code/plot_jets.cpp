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
#include "plot.h"


int main(){

	/* Open data*/
	TFile *f = new TFile("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root","READ");
	TTree *nominal = (TTree*)f->Get("nominal");

    /* Create variable and assign address */
	std::vector<float> *variable = nullptr;
	std::vector<int> *flavour = nullptr;
	nominal->SetBranchAddress("klfitter_logLikelihood",&variable);
	nominal->SetBranchAddress("jet_truthflav", &flavour);

    /* Generete canvas and histograms */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *b_hist = new TH1F("b_hist","b jets",60,-80,-20);
	TH1F *c_hist = new TH1F("c_hist","c jets",60,-80,-20);
	TH1F *l_hist = new TH1F("l_hist","light jets",60,-80,-20);
	std::vector<TH1F*> histograms{l_hist, c_hist, b_hist};

	Long64_t nentries = nominal->GetEntries();
	for (Long64_t i=0;i<nentries;i++) {
      	nominal->GetEntry(i);

		int length = variable->size();
		for (int j=0;j<length;j++){
			if ((*flavour)[j]==0) histograms[0]->Fill((*variable)[j]);
			else if ((*flavour)[j]==4) histograms[1]->Fill((*variable)[j]);
			else if ((*flavour)[j]==5) histograms[2]->Fill((*variable)[j]);
		}
	}

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
	canvas->SetGrid();

	/* Format histograms*/
	char 	*title{"Jet KLFitter Likelihood Distribution"},
			*x_label{"ln(Likelihood)"},
			*y_label{"Entries"};
	plot_hist(histograms[0], kRed, title, x_label, y_label);
	plot_hist(histograms[1], kGreen, title, x_label, y_label);
	plot_hist(histograms[2], kBlue, title, x_label, y_label);

	/* Create legend */
    TLegend *legend = new TLegend(0.7, 0.68, 0.9, 0.88);
    std::vector<char*> labels {"light jets", "c-jets", "b-jets"};
	create_legend(legend, labels, histograms);

	/* Draw and save histogram */
	l_hist->Draw("HIST");
	c_hist->Draw("HIST same");
	b_hist->Draw("HIST same");
	legend->Draw("same");
	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/KLF_likelihood.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);

	return 0;
}
