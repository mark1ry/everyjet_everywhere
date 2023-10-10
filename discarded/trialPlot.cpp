#include <iostream>
#include <TFile.h>
#include <TBranch.h>
#include <TTree.h> 
#include <TH1F.h>
#include <TCanvas.h>


int trialPlot() {
    TCanvas* canvas = new TCanvas("canvas", "Particle Momentum", 800, 600);

    TFile* file = new TFile("/eos/user/v/vvecchio/cjets_calib/user.vvecchio.601229.PhPy8EG.DAOD_PHYS.e8453_s3873_r13829_p5631.SVmass6Julyttbar22_output_pflow_root/user.vvecchio.33977303._000001.output_pflow.root", "read");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Could not open the ROOT file." << std::endl;
        return 1; // Exit with an error code
    }

    TTree* tree = (TTree*)file->Get("nominal");
    if (!tree) {
        std::cerr << "Error: Could not find the TTree in the ROOT file." << std::endl;
        file->Close();
        return 1; // Exit with an error code
    }
    
    int eNumber;
    TBranch* event = tree->GetBranch("eventNumber");
    if (!event) {
        std::cerr << "Error: Could not find the TBranch for el_pt." << std::endl;
        file->Close();
        return 1; // Exit with an error code
    }

    TH1F* hist = new TH1F("hist", "Transvers Momentum Distribution", 100, 0, 500000);

    event->SetAddress(&eNumber);
    std::cout << "Here!" << std::endl;
    float num;
    int entries = tree->GetEntries();
    for (int i=0; i<1; i++) {
        tree->GetEntry(i) <= 0;
        std::cout << "eNumber = " << eNumber << std::endl;
        num = eNumber;
        std::cout << num << std::endl;
        hist->Fill(num);
    }

    std::cout << "Here!" << std::endl;
    hist->Draw();
    canvas->SaveAs("/afs/cern.ch/user/m/moriolpe/private/mphys_project/plot.jpg");

    return 0;

}