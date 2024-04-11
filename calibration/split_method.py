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

    
    return {
        # SV Mass for B-enhanced
        "jet_topLep_SV_mass_Electron_basic_final_B": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B", "jet_topLep_SV_mass_Electron_basic_final_B", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_b", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_c", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l", "jet_topLep_SV_mass_Electron_basic_final_B_passWP77_l", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_b", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_c", 25, 0, 6000),
        "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l": ROOT.TH1D("jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l", "jet_topLep_SV_mass_Electron_basic_final_B_failWP77_l", 25, 0, 6000),
        # pT for B-enhanced
        "jet_topLep_pt_Electron_basic_final_B": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B", "jet_topLep_pt_Electron_basic_final_B", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_passWP77": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77", "jet_topLep_pt_Electron_basic_final_B_passWP77", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_passWP77_b": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77_b", "jet_topLep_pt_Electron_basic_final_B_passWP77_b", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_passWP77_c": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77_c", "jet_topLep_pt_Electron_basic_final_B_passWP77_c", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_passWP77_l": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_passWP77_l", "jet_topLep_pt_Electron_basic_final_B_passWP77_l", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_failWP77": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77", "jet_topLep_pt_Electron_basic_final_B_failWP77", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_failWP77_b": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77_b", "jet_topLep_pt_Electron_basic_final_B_failWP77_b", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_failWP77_c": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77_c", "jet_topLep_pt_Electron_basic_final_B_failWP77_c", 56, 20000, 300000),
        "jet_topLep_pt_Electron_basic_final_B_failWP77_l": ROOT.TH1D("jet_topLep_pt_Electron_basic_final_B_failWP77_l", "jet_topLep_pt_Electron_basic_final_B_failWP77_l", 56, 20000, 300000),
        # SV Mass for C/L-enhanced
        "average_SV_mass_Wjets_Muon_basic_final_B": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B", "average_SV_mass_Wjets_Muon_basic_final_B", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_ll", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_lc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_bc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb", "average_SV_mass_Wjets_Muon_basic_final_B_2bTWP77_cb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_ll", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_lc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_bc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb", "average_SV_mass_Wjets_Muon_basic_final_B_0bTWP77_cb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxl", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lxc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bxc", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cxb", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bbx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_ccx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_llx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_blx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lbx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_clx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_lcx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_bcx", 25, 0, 6000),
        "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx": ROOT.TH1D("average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx", "average_SV_mass_Wjets_Muon_basic_final_B_1bTWP77_cbx", 25, 0, 6000),
        # pT for C/L-enhanced
        "jet_W1_pt_Muon_basic_final_B": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B", "jet_W1_pt_Muon_basic_final_B", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_passWP77": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77", "jet_W1_pt_Muon_basic_final_B_passWP77", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_passWP77_b": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77_b", "jet_W1_pt_Muon_basic_final_B_passWP77_b", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_passWP77_c": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77_c", "jet_W1_pt_Muon_basic_final_B_passWP77_c", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_passWP77_l": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_passWP77_l", "jet_W1_pt_Muon_basic_final_B_passWP77_l", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_failWP77": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77", "jet_W1_pt_Muon_basic_final_B_failWP77", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_failWP77_b": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77_b", "jet_W1_pt_Muon_basic_final_B_failWP77_b", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_failWP77_c": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77_c", "jet_W1_pt_Muon_basic_final_B_failWP77_c", 56, 20000, 300000),
        "jet_W1_pt_Muon_basic_final_B_failWP77_l": ROOT.TH1D("jet_W1_pt_Muon_basic_final_B_failWP77_l", "jet_W1_pt_Muon_basic_final_B_failWP77_l", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B", "jet_W2_pt_Muon_basic_final_B", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_passWP77": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77", "jet_W2_pt_Muon_basic_final_B_passWP77", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_passWP77_b": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77_b", "jet_W2_pt_Muon_basic_final_B_passWP77_b", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_passWP77_c": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77_c", "jet_W2_pt_Muon_basic_final_B_passWP77_c", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_passWP77_l": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_passWP77_l", "jet_W2_pt_Muon_basic_final_B_passWP77_l", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_failWP77": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77", "jet_W2_pt_Muon_basic_final_B_failWP77", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_failWP77_b": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77_b", "jet_W2_pt_Muon_basic_final_B_failWP77_b", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_failWP77_c": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77_c", "jet_W2_pt_Muon_basic_final_B_failWP77_c", 56, 20000, 300000),
        "jet_W2_pt_Muon_basic_final_B_failWP77_l": ROOT.TH1D("jet_W2_pt_Muon_basic_final_B_failWP77_l", "jet_W2_pt_Muon_basic_final_B_failWP77_l", 56, 20000, 300000)
    }



def SelectBenhanced(event, reco: pd.DataFrame, index: int):

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]

    if event["jet_isbtagged_DL1dv01_60"][bhad_index]:

def SelectCLenhanced(event, reco: pd.DataFrame, index: int):

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]
    q1_index = reco["q1_index"]
    q2_index = reco["q2_index"]

    if event["jet_DL1dv01"][bhad_index]>2.456 and event["jet_DL1dv01"][blep_index]>2.456:

def ClassifyEvents(root: list, reco: list):
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
                SelectBenhancedMethodB(event, df.loc[[event_index]], event_index)

            if len(event["mu_pt"])==1:
                SelectCLenhancedMethodB(event, df.loc[[event_index]], event_index)


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

    # CREATE DICT CONTAINING HISTOGRAMS
    histograms = CreateHistograms()

    # ORDER DATA IN PANDAS DATAFRAMES
    b_enhanced, cl_enhanced = ClassifyEvents(root=root_file, reco=reco_info)

    # OPEN OUTPUT FILE AND WRITE THE HISTOGRAMS
    output_file = ROOT.TFile.Open(settings.output_file_path, 'RECREATE')
    output_file.cd()
    for key, value in histograms.items():
        output_file.Write(value)
    output_file.Close()


if __name__ == "__main__":
    exit_code = RecoDistributions()
