#include "histogram.h"

/* 	void format_hist(TH1F*, EColor, char*, char*, char*, int, int, double)
 *
 *		Formats a given TH1F object, changing its title, labels, y range and
 *		setting the amount and size of ticks in the x and y axes. It also normalizes
 *		the histogram and sets the stats to false.
 *
 *	Marc Oriol - 12th October 2023
 */
void format_hist(TH1F* hist, EColor const color, char* const title, char* const xlabel,
				char* const ylabel, int const xticks, int const yticks, double const ylim) {
    
    /* Normalize the histogram*/
    Double_t scale = 1/(hist->Integral());
    hist->Scale(scale);
    
    hist->SetStats(false);
    
    /* Format histogram line */
	hist->SetLineColor(kBlack);
	hist->SetFillColor(kBlack);
	hist->SetFillStyle(3003);
	hist->SetLineWidth(2);
	hist->SetMarkerStyle(20);
	hist->SetMarkerSize(0.2);
	hist->SetMarkerColor(kBlack);

    /* Format title */
	hist->SetTitle(title); // TO DO: ADD TITLE FORMATTING

    /* Format X axis*/
    hist->GetXaxis()->SetTitle(xlabel);
	hist->GetXaxis()->SetTitleSize(0.045);
	hist->GetXaxis()->SetLabelSize(0.04);
	hist->GetXaxis()->SetTickSize(0.02);
	hist->GetXaxis()->SetNdivisions(xticks,0,0);
	hist->GetXaxis()->SetLabelOffset(0.02);
	hist->GetXaxis()->SetTitleOffset(1.15);

    /* Format Y axis */
	hist->GetYaxis()->SetTitle(ylabel);
	hist->GetYaxis()->SetTitleSize(0.045);
	hist->GetYaxis()->SetLabelSize(0.04);
	hist->GetYaxis()->SetTickSize(0.02);
	hist->GetYaxis()->SetNdivisions(yticks,0,0);
	hist->GetYaxis()->SetLabelOffset(0.01);
	hist->GetYaxis()->SetTitleOffset(1.3);
	hist->GetYaxis()->SetRangeUser(0, ylim);

    return;
}

/*	void format_hist (TH1F*, EColor, char*, char*, char*, double)
 *
 *		Overloaded function to format the histogram. This calls the main
 *		formatting function with the predefined values xticks=6 and yticks=10.
 *
 *		Marc Oriol 12th October 2023
 */
void format_hist(TH1F* hist, EColor const color, char* const title, char* const xlabel,
				char* const ylabel, double const ylim) {
    format_hist(hist, color, title, xlabel, ylabel, 6, 10, ylim);
}

/*	void format_legend (TLegend*, std::vector<char*>, std::vector<TH1F*>, double, int)
 *
 *		It takes a vector of char* objects containing labels and a vector of TH1F*
 *		objects containing histograms. These 2 vectors must be the same size. It adds
 *		an entry to the legend for each of the histograms with the corresponding label.
 *		It also formats the text size and alignment.
 *
 *	Marc Oriol - 12th October 2023
 */
void format_legend(TLegend* legend, std::vector<char*> const labels, std::vector<TH1F*> const histograms, 
					double const text_size, int const text_align) {
	int length = labels.size();

	for (int i{0}; i<length; i++) {
		legend->AddEntry(histograms[i], labels[i], "l");
	}

	legend->SetTextSize(text_size);
	legend->SetTextAlign(text_align);
}


/*	void format_legend (TLegend*, std::vector<char*>, std::vector<TH1F*>)
 *
 *		Overloaded function to format the legend. This calls the main formatting
 *		function with the predefined values text_size=0.04 and text_align=12.
 *
 *		Marc Oriol 12th October 2023
 */
void format_legend(TLegend* legend, std::vector<char*> const labels, std::vector<TH1F*> const histograms) {
	format_legend(legend, labels, histograms, 0.04, 12);
}
