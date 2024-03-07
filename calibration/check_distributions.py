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
import pandas as pd
import numpy as np
import awkward as ak
from tqdm import tqdm

def Argparser():
    parser = argparse.ArgumentParser(description="Assess calibration methods")

    parser.add_argument('--read_root', required=False, help="Set data to be read from root files")
    parser.add_argument('--read_pkl', required=False, help="Set data to be read from pickle files")

    parser.add_argument('--root_even', type=str, required='--read_root' in sys.argv, help="Path to the root file containing the even data")
    parser.add_argument('--root_odd', type=str, required='--read_root' in sys.argv, help="Path to the root file containing the odd data")
    parser.add_argument('--pkl_even', type=str, required='--read_root' in sys.argv, help="Path to the pickle file containing the even data")
    parser.add_argument('--pkl_odd', type=str, required='--read_root' in sys.argv, help="Path to the pickle file containing the odd data")
    parser.add_argument('--tree_even', type=str, required=False, default='matched_even', help="Name of the tree containing the even data")
    parser.add_argument('--tree_odd', type=str, required=False, default='matched_odd', help="Name of the tree containing the odd data")

    parser.add_argument('--benh_A', type=str, required=(('--read_pkl' in sys.argv) or ('--store_pickles' in sys.argv)), help="Path to the pickle file containing b enhanced sample using method A")
    parser.add_argument('--benh_B', type=str, required=(('--read_pkl' in sys.argv) or ('--store_pickles' in sys.argv)), help="Path to the pickle file containing b enhanced sample using method B")
    parser.add_argument('--clenh_A', type=str, required=(('--read_pkl' in sys.argv) or ('--store_pickles' in sys.argv)), help="Path to the pickle file containing c & l enhanced sample using method A")
    parser.add_argument('--clenh_B', type=str, required=(('--read_pkl' in sys.argv) or ('--store_pickles' in sys.argv)), help="Path to the pickle file containing c & l enhanced sample using method B")
    
    parser.add_argument('--store_pickles', required=False, help="Set classified data to be stored")
    return parser.parse_args()

def ReadInData(args):
    even_data = uproot.open(args.root_even)[args.tree_even]
    odd_data = uproot.open(args.root_odd)[args.tree_odd]

    even_reco = pd.read_pickle(args.pkl_even)
    odd_reco = pd.read_pickle(args.pkl_odd)

    return [even_data, odd_data], [even_reco, odd_reco]


def SelectBenhancedMethodA(event, reco: pd.DataFrame, index: int) -> list:

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]

    if event["jet_isbtagged_DL1dv01_60"][blep_index]:
        return {"index": index, "pt": event["jet_pt"][bhad_index], "eta": event["jet_eta"][bhad_index],
                "SVmass": event["jet_SV_mass"][bhad_index],  "flavour": event["jet_truthflav"][bhad_index]}, True
    else:
        return None, False

def SelectBenhancedMethodB(event, reco: pd.DataFrame, index: int) -> list:

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]

    if event["jet_isbtagged_DL1dv01_60"][bhad_index]:
        return {"index": index, "pt": event["jet_pt"][blep_index], "eta": event["jet_eta"][blep_index],
                "SVmass": event["jet_SV_mass"][blep_index],  "flavour": event["jet_truthflav"][blep_index]}, True
    else:
        return None, False

def SelectCLenhancedMethodA(event, reco: pd.DataFrame, index: int) -> list:

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]
    q1_index = reco["q1_index"]
    q2_index = reco["q2_index"]

    if event["jet_isbtagged_DL1dv01_60"][bhad_index] and event["jet_isbtagged_DL1dv01_60"][blep_index]:
        return [{"index": index, "label": "Wq1", "pt": event["jet_pt"][q1_index], "eta": event["jet_eta"][q1_index],
                "SVmass": event["jet_SV_mass"][q1_index],  "flavour": event["jet_truthflav"][q1_index]},
                {"index": index, "label": "Wq2", "pt": event["jet_pt"][q2_index], "eta": event["jet_eta"][q2_index],
                "SVmass": event["jet_SV_mass"][q2_index],  "flavour": event["jet_truthflav"][q2_index]}], True
    else:
        return None, False

def SelectCLenhancedMethodB(event, reco: pd.DataFrame, index: int) -> list:

    bhad_index = reco["bhad_index"]
    blep_index = reco["blep_index"]
    q1_index = reco["q1_index"]
    q2_index = reco["q2_index"]

    if event["jet_DL1dv01"][bhad_index]>2.456 and event["jet_DL1dv01"][blep_index]>2.456:
        return [{"index": index, "label": "Wq1", "pt": event["jet_pt"][q1_index], "eta": event["jet_eta"][q1_index],
                "SVmass": event["jet_SV_mass"][q1_index],  "flavour": event["jet_truthflav"][q1_index]},
                {"index": index, "label": "Wq2", "pt": event["jet_pt"][q2_index], "eta": event["jet_eta"][q2_index],
                "SVmass": event["jet_SV_mass"][q2_index],  "flavour": event["jet_truthflav"][q2_index]}], True
    else:
        return None, False

def ClassifyEvents(root: list, reco: list):
    # CREATE LISTS TO HOLD RELEVANT INFO
    b_enhanced_A = []
    b_enhanced_B = []
    cl_enhanced_A = []
    cl_enhanced_B = []

    # CYCLE THROUGH EVEN AND ODD FILES
    for file_index in range(len(root)):
        data = root[file_index].arrays()
        df = reco[file_index].set_index('index')

        # CYCLE THROUGH EVENTS AND CLASSIFY THEM
        nentries = len(df.index)
        for event_index in tqdm(range(nentries), desc='Classifying events', unit='event'):
            event = data[event_index]
            if len(event["el_pt"])==1:
                # CHECK METHOD A
                temp_dict, selected = SelectBenhancedMethodA(event=event, reco=df.loc[[event_index]], index=event_index)
                if selected:
                    b_enhanced_A.append(temp_dict)

                # CHECK METHOD B
                temp_dict, selected = SelectBenhancedMethodB(event, df.loc[[event_index]], event_index)
                if selected:
                    b_enhanced_B.append(temp_dict)

            if len(event["mu_pt"])==1:
                # CHECK METHOD A
                temp_dict, selected = SelectCLenhancedMethodA(event=event, reco=df.loc[[event_index]], index=event_index)
                if selected:
                    for element in temp_dict:
                        cl_enhanced_A.append(element)

                # CHECK METHOD B
                temp_dict, selected = SelectCLenhancedMethodB(event, df.loc[[event_index]], event_index)
                if selected:
                    for element in temp_dict:
                        cl_enhanced_B.append(element)

    b_enhanced_A = pd.DataFrame(b_enhanced_A)
    b_enhanced_B = pd.DataFrame(b_enhanced_B)
    cl_enhanced_A = pd.DataFrame(cl_enhanced_A)
    cl_enhanced_B = pd.DataFrame(cl_enhanced_B)

    return [b_enhanced_A, b_enhanced_B], [cl_enhanced_A, cl_enhanced_B]

def GetFromPickle(args):

    b_enhanced_A = pd.read_pickle(args.benh_A)
    b_enhanced_B = pd.read_pickle(args.benh_B)
    cl_enhanced_A = pd.read_pickle(args.clenh_A)
    cl_enhanced_B = pd.read_pickle(args.clenh_B)

    return [b_enhanced_A, b_enhanced_B], [cl_enhanced_A, cl_enhanced_B]

def RecoDistributions():

    args = Argparser()

    # READ IN DATA
    if args.read_pkl:
        b_enhanced, cl_enhanced = GetFromPickle(args=args)

    elif args.read_root:
        root_file, reco_info = ReadInData(args=args)

        # ORDER DATA IN PANDAS DATAFRAMES
        b_enhanced, cl_enhanced = ClassifyEvents(root=root_file, reco=reco_info)

    else:
        print("Either the flag '--read_pkl' or '--read_root' are required")
        print("Please specify the desired option when calling the script")
        return 1

    # STORE DATA IF REQUESTED
    if args.store_pickles:
        b_enhanced[0].to_pickle(args.benh_A)
        b_enhanced[1].to_pickle(args.benh_B)
        cl_enhanced[0].to_pickle(args.clenh_A)
        cl_enhanced[1].to_pickle(args.clenh_B)

    # PRODUCE PLOTS

    return 0

if __name__ == "__main__":
    exit_code = RecoDistributions()
