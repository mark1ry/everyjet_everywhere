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

int main () {



    /* Generete canvas and histogram */
	TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600); 
	TH1F *hist1   = new TH1F("hist1","MC b from t distribution",50, 0, 300);
    TH1F *hist2   = new TH1F("hist2","MC b from t distribution",50, 0, 300);
    TH1F *hist3   = new TH1F("hist3","MC b from t distribution",50, 0, 300);
    TH1F *hist4   = new TH1F("hist4","MC b from t distribution",50, 0, 300);
    TH1F *hist5   = new TH1F("hist5","MC b from t distribution",50, 0, 300);
    TH1F *hist6   = new TH1F("hist6","MC b from t distribution",50, 0, 300);

    std::vector<TH1F*> histograms{hist1, hist2, hist3, hist4, hist5, hist6};
    int variable;
 
    histograms[0]->Fill(variable);
    histograms[1]->Fill(variable);
    histograms[2]->Fill(variable);
    histograms[3]->Fill(variable);
    histograms[4]->Fill(variable);
    histograms[5]->Fill(variable);
            

    //std::cout<<"The number of hadronically decaying w from t is "<<hadronic_w<<std::endl;

	/* Format canvas */
	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.08); //before it was 0.05
 	canvas->SetTopMargin(0.08);
   	canvas->SetBottomMargin(0.12);
    canvas->SetGrid();
	canvas->Update();

    /* Format histograms*/
	char 	*title{"Hadronic Decay of W from t Transverse Momentum Distribution File 000003"},
			*x_label{"p_T (GeV/c)"},
			*y_label{"Entries"};
	format_hist(histograms[0], kRed, title, x_label, y_label, 0.06);
	format_hist(histograms[1], kGreen, title, x_label, y_label, 0.06);
	format_hist(histograms[2], kBlue, title, x_label, y_label, 0.06);
    format_hist(histograms[3], kViolet, title, x_label, y_label, 0.06);
	format_hist(histograms[4], kBlack, title, x_label, y_label, 0.06);
	format_hist(histograms[5], kOrange, title, x_label, y_label, 0.06);

	/* Create legend */
    TLegend *legend = new TLegend(0.7, 0.56, 0.9, 0.88);
    std::vector<char*> labels {"u #bar{d}", "u #bar{s}", "u #bar{b}", "c #bar{d}", "c #bar{s}", "c #bar{b}"};
	format_legend(legend, labels, histograms);

	/* Draw and save histogram */
	hist1->Draw("HIST");
	hist2->Draw("HIST same");
	hist3->Draw("HIST same");
    hist4->Draw("HIST same");
	hist5->Draw("HIST same");
    hist6->Draw("HIST same");
	
	legend->Draw("same");

	const char* cwd = std::getenv("PWD");
	const char* relative_path = "../results/truth/hadronic_decays_w_t_with_flavours_000003.png";
	size_t full_path_length = std::strlen(cwd) + 2 + std::strlen(relative_path);
	char full_path[full_path_length];
	std::snprintf(full_path, full_path_length, "%s/%s", cwd, relative_path);
	canvas->SaveAs(full_path);
    

    return 0;
}
