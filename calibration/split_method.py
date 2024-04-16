"""
Want to study the flavour, pT, eta, SVMass distributions of the objects in the
events reconstructed by Hyper to assess the viability of the calibration.

First of all, we want to divide the sample of events in two different orthogonal
samples. In one of them a set of selection criteria will be applied to enhance the
number of b-jets present, while the other will be b-jet suppressed. The
orthogonality will be achieved by selecting the first sample to be the events
containing an electron, and the second to contain events with a muon.

For each of these samples we want to try two different sets of selection criteria
in order to assess which one works best. This will be done by plotting the flavour
fraction of jets of interest, as well as some kinematic variables such as the pT,
eta and SVMass.

Note that the data to be used by us in our study was divided in two datasets,
one containing events with an even eventnumber and the other with odd eventnumber.
This was done to avoid overtraining, and the datasets were cross-trained and
evaluated. For this reason, this code requires to provide two datasets, and carries
the analysis joining the data from both.

Marc Oriol Pérez
07/03/2024
"""

# IMPORT STATEMENTS
import argparse
import sys
import uproot
import ROOT
import pandas as pd
import numpy as np
import awkward as ak
from tqdm import tqdm

import plot
import configuration

def Argparser():
    parser = argparse.ArgumentParser(description="Assess calibration methods")

    parser.add_argument('-c', '--config', type=str, required=True, help="Set path to the configuration file")

    return parser.parse_args()

def ReadInData(settings):
    even_data = uproot.open(settings.even_root_path)[settings.even_tree_name]
    odd_data = uproot.open(settings.odd_root_path)[settings.odd_tree_name]

    even_reco = pd.read_pickle(settings.even_pkl_path)
    odd_reco = pd.read_pickle(settings.odd_pkl_path)

    return [even_data, odd_data], [even_reco, odd_reco]

def CreateHistograms() -> dict:
    sv_nbins = 24
    sv_xmin = 0
    sv_xmax = 6000
    pt_nbins = 56
    pt_xmin = 20000
    pt_xmax = 300000

    return {
        # SV Mass for B-enhanced
        "jet_topLep_SV_mass_Electron_basic_final_B": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B", "jet_topLep_SV_mass_Electron_basic_final_B", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c", sv_nbins, sv_xmin, sv_xmax),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l", sv_nbins, sv_xmin, sv_xmax),
        # pT for B-enhanced
        "jet_topLep_pt_Electron_basic_final_B": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B", "jet_topLep_pt_Electron_basic_final_B", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_passWP77": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77", "jet_topLep_pt_Electron_basic_final_B_passWP77", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_passWP77_b": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77_b", "jet_topLep_pt_Electron_basic_final_B_passWP77_b", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_passWP77_c": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77_c", "jet_topLep_pt_Electron_basic_final_B_passWP77_c", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_passWP77_l": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77_l", "jet_topLep_pt_Electron_basic_final_B_passWP77_l", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_failWP77": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77", "jet_topLep_pt_Electron_basic_final_B_failWP77", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_failWP77_b": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77_b", "jet_topLep_pt_Electron_basic_final_B_failWP77_b", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_failWP77_c": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77_c", "jet_topLep_pt_Electron_basic_final_B_failWP77_c", pt_nbins, pt_xmin, pt_xmax),
        "jet_topLep_pt_Electron_basic_final_B_failWP77_l": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77_l", "jet_topLep_pt_Electron_basic_final_B_failWP77_l", pt_nbins, pt_xmin, pt_xmax),
        # SV Mass for C/L-enhanced
        "average_SV_mass_Wjets_Muon_basic_final_B": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B", "average_SV_mass_Wjets_Muon_basic_final_B", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx", sv_nbins, sv_xmin, sv_xmax),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx", sv_nbins, sv_xmin, sv_xmax),
        # pT for C/L-enhanced
        "jet_W1_pt_Muon_basic_final_B": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B", "jet_W1_pt_Muon_basic_final_B", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_passWP77": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77", "jet_W1_pt_Muon_basic_final_B_passWP77", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_passWP77_b": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77_b", "jet_W1_pt_Muon_basic_final_B_passWP77_b", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_passWP77_c": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77_c", "jet_W1_pt_Muon_basic_final_B_passWP77_c", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_passWP77_l": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77_l", "jet_W1_pt_Muon_basic_final_B_passWP77_l", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_failWP77": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77", "jet_W1_pt_Muon_basic_final_B_failWP77", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_failWP77_b": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77_b", "jet_W1_pt_Muon_basic_final_B_failWP77_b", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_failWP77_c": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77_c", "jet_W1_pt_Muon_basic_final_B_failWP77_c", pt_nbins, pt_xmin, pt_xmax),
        "jet_W1_pt_Muon_basic_final_B_failWP77_l": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77_l", "jet_W1_pt_Muon_basic_final_B_failWP77_l", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B", "jet_W2_pt_Muon_basic_final_B", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_passWP77": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77", "jet_W2_pt_Muon_basic_final_B_passWP77", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_passWP77_b": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77_b", "jet_W2_pt_Muon_basic_final_B_passWP77_b", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_passWP77_c": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77_c", "jet_W2_pt_Muon_basic_final_B_passWP77_c", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_passWP77_l": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77_l", "jet_W2_pt_Muon_basic_final_B_passWP77_l", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_failWP77": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77", "jet_W2_pt_Muon_basic_final_B_failWP77", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_failWP77_b": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77_b", "jet_W2_pt_Muon_basic_final_B_failWP77_b", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_failWP77_c": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77_c", "jet_W2_pt_Muon_basic_final_B_failWP77_c", pt_nbins, pt_xmin, pt_xmax),
        "jet_W2_pt_Muon_basic_final_B_failWP77_l": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77_l", "jet_W2_pt_Muon_basic_final_B_failWP77_l", pt_nbins, pt_xmin, pt_xmax)
    }


def SelectBenhanced(event, reco: pd.DataFrame, index: int, histograms: dict) -> dict:

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]

    if event["jet_isbtagged_DL1dv01_60"][bhad_index]:

        # Fill the histogram with the total SV mass before splitting (we want hist_total=hist_total_check)
        histograms["jet_topLep_SV_mass_Electron_basic_final_B"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
        histograms["jet_topLep_pt_Electron_basic_final_B"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])
        
        #Need to split the data into 2 when they pass/fail the blep tag 77% WP
        if event["jet_DL1dv01"][blep_index] > 2.456:

            # Fill the histogram with events that are tagged
            histograms["jet_topLep_SV_mass_Electron_basic_final_B_passWP77"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
            histograms["jet_topLep_pt_Electron_basic_final_B_passWP77"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

            # Split according to truth flavour and fill histograms
            if event["jet_truthflav"][blep_index] == 5:
                histograms["jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
                histograms["jet_topLep_pt_Electron_basic_final_B_passWP77_b"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

            elif event["jet_truthflav"][blep_index] == 4:
                histograms["jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
                histograms["jet_topLep_pt_Electron_basic_final_B_passWP77_c"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

            elif event["jet_truthflav"][blep_index] == 0:
                histograms["jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
                histograms["jet_topLep_pt_Electron_basic_final_B_passWP77_l"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

        else:

            # Fill the histogram with all the events not b-tagged
            histograms["jet_topLep_SV_mass_Electron_basic_final_B_failWP77"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
            histograms["jet_topLep_pt_Electron_basic_final_B_failWP77"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

            # Split events accotding to their truth flavour and fill histograms
            if event["jet_truthflav"][blep_index] == 5:
                histograms["jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
                histograms["jet_topLep_pt_Electron_basic_final_B_failWP77_b"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

            elif event["jet_truthflav"][blep_index] == 4:
                histograms["jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
                histograms["jet_topLep_pt_Electron_basic_final_B_failWP77_c"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])

            elif event["jet_truthflav"][blep_index] == 0:
                histograms["jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l"].Fill(event["jet_SV_mass"][blep_index][0], event["weight_mc"])
                histograms["jet_topLep_pt_Electron_basic_final_B_failWP77_l"].Fill(event["jet_pt"][blep_index][0], event["weight_mc"])
            
    return histograms
    


def SelectCLenhanced(event, reco: pd.DataFrame, index: int, histograms: dict) -> dict:

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]
    q1_index = reco["q1_index"]
    q2_index = reco["q2_index"]

    if event["jet_truthflav"][q2_index] != 0:
        if event["jet_truthflav"][q2_index][0] ==0:
            print("AAAAA")

    if event["jet_DL1dv01"][bhad_index]>2.456 and event["jet_DL1dv01"][blep_index]>2.456:

        # Fill histograms before splitting
        histograms["average_SV_mass_Wjets_Muon_basic_final_B"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
        histograms["jet_W1_pt_Muon_basic_final_B"].Fill((event["jet_pt"][q1_index][0]), event["weight_mc"])
        histograms["jet_W2_pt_Muon_basic_final_B"].Fill((event["jet_pt"][q2_index][0]), event["weight_mc"])

        q1_and_q2 = (event["jet_truthflav"][q1_index], event["jet_truthflav"][q2_index])

        # Split the data into 2-btags, 1 b-tag and 0 b-tags (targets now are the q1 and q2)
        # 2 B-TAGS
        if event["jet_DL1dv01"][q1_index]>2.456 and event["jet_DL1dv01"][q2_index]>2.456:
            # Fill the histogram with 2 b-tags
            histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])        
            # Split flavour permutations
            if q1_and_q2 == (5,5):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (5,4):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (5,0):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (4,5):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (4,4):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (4,0):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (0,5):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (0,4):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (0,0):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])

        # 1 B-TAG
        elif event["jet_DL1dv01"][q1_index]>2.456 or event["jet_DL1dv01"][q2_index]>2.456:
            # Fill the histogram with 1 b-tag
            histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            # Divide the events by whether the first is b-tagged or the second is
            if event["jet_DL1dv01"][q1_index]>2.456:
                # Split flavour permutations
                if q1_and_q2 == (5,5):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (5,4):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (5,0):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (4,5):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (4,4):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (4,0):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (0,5):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (0,4):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (0,0):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                
            elif event["jet_DL1dv01"][q2_index]>2.456:
                # Split flavour permutations
                if q1_and_q2 == (5,5):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (5,4):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (5,0):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (4,5):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (4,4):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (4,0):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (0,5):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (0,4):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
                elif q1_and_q2 == (0,0):
                    histograms["average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])

        # 0 B-TAGS
        else:
            # Fill the histogram with 0 b-tags
            histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            # Split flavour permutations
            if q1_and_q2 == (5,5):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (5,4):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (5,0):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (4,5):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (4,4):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (4,0):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (0,5):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (0,4):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])
            elif q1_and_q2 == (0,0):
                histograms["average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll"].Fill((event["jet_SV_mass"][q1_index][0] + event["jet_SV_mass"][q2_index][0])/2, event["weight_mc"])


        # FILL PT PLOTS FOR Q1 AND Q2 INDEPENDENTLY
        # Q1
        if event["jet_DL1dv01"][q1_index] > 2.456:
            # Fill the histogram w          ith tagged jets
            histograms["jet_W1_pt_Muon_basic_final_B_passWP77"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
            # Split by flavour
            if event["jet_truthflav"][q1_index] == 5:
                histograms["jet_W1_pt_Muon_basic_final_B_passWP77_b"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q1_index] == 4:
                histograms["jet_W1_pt_Muon_basic_final_B_passWP77_c"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q1_index] == 0:
                histograms["jet_W1_pt_Muon_basic_final_B_passWP77_l"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
        else:
            # Fill histogam with non-tagged jets
            histograms["jet_W1_pt_Muon_basic_final_B_failWP77"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
            # Split by flavour
            if event["jet_truthflav"][q1_index] == 5:
                histograms["jet_W1_pt_Muon_basic_final_B_failWP77_b"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q1_index] == 4:
                histograms["jet_W1_pt_Muon_basic_final_B_failWP77_c"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q1_index] == 0:
                histograms["jet_W1_pt_Muon_basic_final_B_failWP77_l"].Fill(event["jet_pt"][q1_index][0], event["weight_mc"])

        # Q2
        if event["jet_DL1dv01"][q2_index] > 2.456:
            # Fill the histogram with tagged jets
            histograms["jet_W2_pt_Muon_basic_final_B_passWP77"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
            # match statement starts here .
            if event["jet_truthflav"][q2_index] == 5:
                histograms["jet_W2_pt_Muon_basic_final_B_passWP77_b"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q2_index] == 4:
                histograms["jet_W2_pt_Muon_basic_final_B_passWP77_c"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q2_index] == 0:
                histograms["jet_W2_pt_Muon_basic_final_B_passWP77_l"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
        else:
            # Fill the histogram with non-tagged jets
            histograms["jet_W2_pt_Muon_basic_final_B_failWP77"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
            # match statement starts here .
            if event["jet_truthflav"][q2_index] == 5:
                histograms["jet_W2_pt_Muon_basic_final_B_failWP77_b"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q2_index] == 4:
                histograms["jet_W2_pt_Muon_basic_final_B_failWP77_c"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
            elif event["jet_truthflav"][q2_index] == 0:
                histograms["jet_W2_pt_Muon_basic_final_B_failWP77_l"].Fill(event["jet_pt"][q2_index][0], event["weight_mc"])
    return histograms


def ClassifyEvents(root: list, reco: list, histograms: dict) -> dict:
    # CREATE LISTS TO HOLD RELEVANT INFO
    b_enhanced = []
    cl_enhanced = []

    # CYCLE THROUGH EVEN AND ODD FILES
    for file_index in range(len(root)):
        data = root[file_index].arrays()
        df = reco[file_index].set_index('index')

        # CYCLE THROUGH EVENTS AND CLASSIFY THEM ACCORDING TO LEPTON FLAVOUR
        nentries = len(df.index)
        for event_index in tqdm(range(nentries), desc='Classifying events', unit='event'):
            event = data[event_index]
            if len(event["el_pt"])==1:
                histograms = SelectBenhanced(event, df.loc[[event_index]], event_index, histograms)

            if len(event["mu_pt"])==1:
                histograms = SelectCLenhanced(event, df.loc[[event_index]], event_index, histograms)

    return histograms


def RecoDistributions():

    # PARSE ARGUMENTS
    args = Argparser()

    # OPEN AND READ CONFIGURATION FILE
    f = open(args.config,)
    settings = configuration.settings_split()
    settings.update_settings(f)
    f.close()
    print("CHECKPOINT: Successfully loaded the settings!")

    # OPEN INPUT FILES
    root_file, reco_info = ReadInData(settings=settings)
    print("CHECKPOINT: Successfully loaded the data!")

    # CREATE DICT CONTAINING HISTOGRAMS
    histograms = CreateHistograms()

    # ORDER DATA IN PANDAS DATAFRAMES
    histograms = ClassifyEvents(root=root_file, reco=reco_info, histograms=histograms)
    print("CHECKPOINT: The histograms have been filled!")

    # OPEN OUTPUT FILE AND WRITE THE HISTOGRAMS
    output_file = ROOT.TFile.Open(settings.output_root_path, 'RECREATE')
    output_file.cd()
    for key, value in histograms.items():
        value.Write()
    output_file.Close()
    print("CHECKPOINT: The root file has been written out!")

    return


if __name__ == "__main__":
    exit_code = RecoDistributions()