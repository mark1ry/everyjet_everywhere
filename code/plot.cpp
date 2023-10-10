#include "plot.h"

void plot_hist(TH1F* hist, EColor const color, char* const title, char* const x_label, char* const y_label) {
    
    /* Normalize the histogram*/
    Double_t scale = 1/(hist->Integral());
    hist->Scale(scale);
    
    hist->SetStats(false);
    
    /* Format histogram line */
    hist->SetLineColor(color);
    hist->SetLineStyle(2);
    hist->SetLineWidth(4);

    /* Format title */
	hist->SetTitle(title);
	//hist->SetTitleSize(5);
	//hist->SetTitleFont(46);
	//hist->SetTitleOffset(0.1);

    /* Format X axis*/
    hist->GetXaxis()->SetTitle(x_label);
	hist->GetXaxis()->SetTitleSize(0.045);
	hist->GetXaxis()->SetLabelSize(0.04);
	hist->GetXaxis()->SetTickSize(0.02);
	hist->GetXaxis()->SetNdivisions(6,0,0);
	hist->GetXaxis()->SetLabelOffset(0.02);
	hist->GetXaxis()->SetTitleOffset(1.15);

    /* Format Y axis */
	hist->GetYaxis()->SetTitle(y_label);
	hist->GetYaxis()->SetTitleSize(0.045);
	hist->GetYaxis()->SetLabelSize(0.04);
	hist->GetYaxis()->SetTickSize(0.02);
	hist->GetYaxis()->SetNdivisions(10,0,0);
	hist->GetYaxis()->SetLabelOffset(0.01);
	hist->GetYaxis()->SetTitleOffset(1.3);
	hist->GetYaxis()->SetRangeUser(0, 0.6);

    return;
}

void create_legend(TLegend* legend, std::vector<char*> const labels, std::vector<TH1F*> histograms) {
	int length = labels.size();

	for (int i{0}; i<length; i++) {
		legend->AddEntry(histograms[i], labels[i], "l");
	}

	legend->SetTextSize(0.04);
	legend->SetTextAlign(12);
}