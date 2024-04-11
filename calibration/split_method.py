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

    args = Argparser()

    f = open(args.config,)
    settings = configuration.settings()
    settings.update_settings(f)
    f.close()
    print("CHECKPOINT: Successfully loaded the settings!")

    root_file, reco_info = ReadInData(settings=settings)

    # ORDER DATA IN PANDAS DATAFRAMES
    b_enhanced, cl_enhanced = ClassifyEvents(root=root_file, reco=reco_info)


if __name__ == "__main__":
    exit_code = RecoDistributions()
