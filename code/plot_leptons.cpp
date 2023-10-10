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
#include "plot.h"


int main(){

    /* Open data*/
	TFile *f = new TFile("/eos/home-v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root","READ");
	TTree *nominal = (TTree*)f->Get("nominal");

    /* Create variable and assign address */
	std::vector<float> *variable = nullptr;
	nominal->SetBranchAddress("jet_pt",&variable);

    /* Generete canvas and histogram */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *hist   = new TH1F("hist","lepton distribution",12,0,12);

    /* Fill histogram */
	Long64_t nentries = nominal->GetEntries();
	for (Long64_t i=0;i<nentries;i++) {
      	nominal->GetEntry(i);

		int length = variable->size();
		hist->Fill(length);
	
	}

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
    canvas->SetGrid();
	canvas->Update();

    /* Format histogram, draw and save */
	char 	*title{"Number of Jets Distribution"},
			*x_label{"Number of jets"},
			*y_label{"Entries"};
    plot_hist(hist, kRed, title, x_label, y_label);
	hist->Draw("HIST");
	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/trial.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);

	return 0;
}