#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "THStack.h"
#include "TLegend.h"
#include "TF1.h"
#include "TF2.h"
#include "TLine.h"
#include "TMath.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cmath>

void addGENIE_Wgt1(){
        // Define input and output file paths
        TString inputFile = "/pnfs/uboone/persistent/users/cnguyen/ANNIE_Sample/PhaseIITree_bg2_ntuple.root";
        TString outputFile = "/pnfs/uboone/persistent/users/cnguyen/ANNIE_Sample/PhaseIITree_bg2_ntuple_withweight.root";
        
        // Open input file in READ mode
        TFile *f_in = new TFile(inputFile, "READ");
        if (!f_in || f_in->IsZombie()) {
            std::cerr << "Error: Cannot open input file " << inputFile << std::endl;
            return;
        }
        
        gInterpreter->GenerateDictionary("map<string,vector<double>>", "map;string;vector");
        
        // Get the input tree
        TTree *tTrig_in = (TTree*)f_in->Get("phaseIITriggerTree");
        if (!tTrig_in) {
            std::cerr << "Error: Cannot find phaseIITriggerTree in input file" << std::endl;
            f_in->Close();
            delete f_in;
            return;
        }
        
        // Create output file
        TFile *f_out = new TFile(outputFile, "RECREATE");
        if (!f_out || f_out->IsZombie()) {
            std::cerr << "Error: Cannot create output file " << outputFile << std::endl;
            f_in->Close();
            delete f_in;
            return;
        }
        
        // Clone the tree structure (this copies all branches)
        TTree *tTrig_out = tTrig_in->CloneTree(0);  // 0 means clone structure only, no entries
        
        // Add new branch
        vector<double> *weight_TunedCentralValue_UBGenie = new vector<double>();
        TBranch *weight_TunedCentralValue_UBGenie_branch = tTrig_out->Branch("weight_TunedCentralValue_UBGenie", &weight_TunedCentralValue_UBGenie);
        
        Long64_t nentriesTrig = tTrig_in->GetEntries();
        std::cout << "TriggerTree entries: " << nentriesTrig << std::endl;
        
        // Loop through all entries
        for (Long64_t i = 0; i < nentriesTrig; i++) {
            if(i % 1000 == 0) std::cout << "Processing entry " << i << " / " << nentriesTrig << std::endl;
            
            // Read entry from input tree
            tTrig_in->GetEntry(i);
            
            // Clear and fill the new branch data
            weight_TunedCentralValue_UBGenie->clear();
            double wgt_input = 1.0;
            weight_TunedCentralValue_UBGenie->push_back(wgt_input);
            
            // Fill the output tree (copies all branches from input + new branch)
            tTrig_out->Fill();
        }
        
        std::cout << "Writing output file..." << std::endl;
        
        // Write the output tree
        f_out->cd();
        tTrig_out->Write();
        
        // Copy other objects from input file to output file (if any)
        TIter next(f_in->GetListOfKeys());
        TKey *key;
        while ((key = (TKey*)next())) {
            TString className = key->GetClassName();
            TString keyName = key->GetName();
            
            // Skip the tree we already copied
            if (keyName == "phaseIITriggerTree") continue;
            
            TObject *obj = key->ReadObj();
            if (obj) {
                f_out->cd();
                obj->Write(keyName);
                delete obj;
            }
        }
        
        std::cout << "Closing files..." << std::endl;
        
        // Clean up
        f_out->Close();
        f_in->Close();
        
        delete weight_TunedCentralValue_UBGenie;
        delete f_out;
        delete f_in;
        
        std::cout << "Done! Output saved to: " << outputFile << std::endl;
}