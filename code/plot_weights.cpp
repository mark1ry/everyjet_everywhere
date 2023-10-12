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
	nominal->SetBranchAddress("jet_pt",&variable);
    std::vector<Float_t>* weight = nullptr;
    TBranch* w_branch = nominal->GetBranch("weight_mc");
    w_branch->SetAddress(&weight);

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
			histograms[0]->Fill((*variable)[j]/1000);
			histograms[1]->Fill((*variable)[j]/1000, (*weight)[j]);
		}
	}

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
	canvas->SetGrid();

	/* Format histograms*/
	char 	*title{"Jet Transverse Momentum Weighted vs Unweighted"},
			*x_label{"p_T (GeV/c)"},
			*y_label{"Entries (Normalized)"};
	plot_hist(histograms[0], kBlue, title, x_label, y_label);
	plot_hist(histograms[1], kGreen, title, x_label, y_label);

	/* Create legend */
    TLegend *legend = new TLegend(0.7, 0.68, 0.9, 0.88);
    std::vector<char*> labels {"Unweighted", "Weighted"};
	create_legend(legend, labels, histograms);

	/* Draw and save histogram */
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