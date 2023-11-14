#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>

void plot(char* vname1, char* vname2, char* title, char* outputfile) {
    // Open the ROOT file
    TFile* file = new TFile("/eos/user/m/moriolpe/mphys_project/matched_calib_ttbar.root", "READ");

    // Access the histograms
    TH1F* histogram1 = (TH1F*)file->Get(vname1);
	TH1F* histogram2 = (TH1F*)file->Get(vname2);
    // Create a canvas
    TCanvas* canvas = new TCanvas("canvas", "Histograms", 800, 600);
    canvas->cd();

	canvas->SetLeftMargin(0.12);
	canvas->SetRightMargin(0.05);
	canvas->SetTopMargin(0.08);
	canvas->SetBottomMargin(0.12);

    histogram2->SetTitle(title);
    // Draw the histograms on the same canvas
    histogram1->SetFillColor(kBlue);
    histogram1->SetFillStyle(3003);
	histogram2->SetFillColor(kGreen);
	histogram2->SetFillStyle(3003);
    histogram2->Draw("E1");
    histogram2->Draw("HIST SAME");
	histogram1->Draw("E1 SAME");
	histogram1->Draw("HIST SAME");

	TLegend *legend = new TLegend(0.7, 0.68, 0.9, 0.88);
	legend->AddEntry(histogram1, "Reco", "f");
	legend->AddEntry(histogram2, "Truth", "f");
	legend->SetTextSize(0.04);
	legend->SetTextAlign(12);
	legend->Draw();

    canvas->Update();

    // Save the canvas as an image (optional)
    canvas->SaveAs(outputfile);

	delete canvas;
	delete histogram1;
	delete histogram2;
	delete legend;
    // Close the ROOT file
    file->Close();

    return;
}

int main() {
	plot("h_Whad_minv", "h_MC_Whad_minv", "Whad Invariant Mass Distribution", "/eos/user/m/moriolpe/mphys_project/h_both_Whad_minv.png");
	plot("h_thad_minv", "h_MC_thad_minv", "t_had Invariant Mass Distribution", "/eos/user/m/moriolpe/mphys_project/h_both_thad_minv.png");
	plot("h_wjets_angle", "h_MC_wjets_angle", "W-jets Angle Distribution", "/eos/user/m/moriolpe/mphys_project/h_both_wjets_angle.png");
	plot("h_twhad_angle", "h_MC_twhad_angle", "t-Whad Angle Distribution", "/eos/user/m/moriolpe/mphys_project/h_both_twhad_angle.png"); 	
	plot("h_bbbar_angle", "h_MC_bbbar_angle", "top-jets Angle Distribution", "/eos/user/m/moriolpe/mphys_project/h_both_bbbar_angle.png"); 	
	plot("h_blepton_angle", "h_MC_blepton_angle", "b-lepton Angle Distribution", "/eos/user/m/moriolpe/mphys_project/h_both_tlepton_angle.png"); 	
	
	return 0;
}
