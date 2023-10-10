import argparse
import uproot
import numpy as np
import os

# - - - - - - - FUNCTIONS - - - - - - - #


def get_data():
    '''
    Reads in the root files containing the LHCb data of D0 decays into two hadrons. It takes into
    account the year and size requested by the user, and handles the different scenarios appropiately.
    
    It only reads a set of specified variables given in the main body of the code.
    
    Returns an array of size 2 with the data using the up polarity as the first argument, and the
    down polarity as the second argument.
    '''
    tree_name = "nominal"

    # get path to directories of the requested year
    directory = "/eos/user/v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root"
    
    
    # reads the data from the files requested by the user and concatanates it
    data = uproot.concatenate(directory+":"+tree_name)
    
    print('checkpoint: data has been read')
    
    return data

# - - - - - - - MAIN CODE - - - - - - - #

# read data in
raw_data = get_data()

print(len(raw_data["eventNumber"]))