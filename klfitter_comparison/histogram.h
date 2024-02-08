#ifndef PLOT_H 
#define PLOT_H

#include <vector>
#include <TH1F.h>
#include <TColor.h>
#include <TROOT.h>
#include <TLegend.h>

void format_hist(TH1F* hist, EColor const color, char* const title, char* const xlabel,
				char* const ylabel, int const xticks, int const yticks, double const ylim);

void format_hist(TH1F* hist, EColor const color, char* const title, char* const xlabel,
				char* const ylabel, double const ylim);

void format_legend(TLegend* legend, std::vector<char*> const labels, std::vector<TH1F*> const histograms, 
					double const text_size, int const text_align);

void format_legend(TLegend* legend, std::vector<char*> const labels, std::vector<TH1F*> const histograms);
#endif