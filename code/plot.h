#ifndef PLOT_H 
#define PLOT_H

#include <vector>
#include <TH1F.h>
#include <TColor.h>
#include <TROOT.h>
#include <TLegend.h>

void plot_hist(TH1F* hist, EColor const color, char* const title, char* const x_label, char* const y_label);

void create_legend(TLegend* legend, std::vector<char*> const labels, std::vector<TH1F*> histograms);

#endif