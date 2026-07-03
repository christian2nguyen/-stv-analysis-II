//// This code is to run and test making a joint convariance matrix 
//Author : Chrisitan Nguyen
//date: 2/6/25
//


// Executable for generating systematic universe files for later analysis. It
// has been adapted from a similar ROOT macro.

// Standard library includes
#include <stdexcept>

// ROOT includes
#include "TBranch.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

// STV analysis includes
#include "XSecAnalyzer/FilePropertiesManager.hh"
#include "XSecAnalyzer/MCC9SystematicsCalculator.hh"
#include "XSecAnalyzer/UniverseMaker.hh"
// Standard library includes
#include <algorithm>

// ROOT includes
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TLegend.h"

// XSecAnalyzer includes
#include "XSecAnalyzer/PlotUtils.hh"
#include "XSecAnalyzer/SliceBinning.hh"
#include "XSecAnalyzer/SliceHistogram.hh"

///////////////////////////////////////////
//Function 
///////////////////////////
void testCovariance();

const std::vector< std::string > cov_mat_keys = {  
   "total", 
   "detVar_total",
    "flux",
    "reint",
   "xsec_total",
   "POT",
    "numTargets",
    "MCstats",
    "EXTstats"
    /*, "BNBstats" removing BNB stats because the uncern of the model shouldn't include it */};
  
const std::vector< std::string > cov_mat_keys_cross = {  
     "xsec_DecayAngMEC",
     "xsec_NormCCCOH",
     "xsec_NormNCCOH",
     "xsec_RPA_CCQE ",
     "xsec_ThetaDelta2NRad",
     "xsec_Theta_Delta2Npi",
     "xsec_VecFFCCQEshape",
     "xsec_AxFFCCQEshape",
     "Xsec_XSecShape_CCMEC",
     "xsec_unisim"
     };
   
   
    const std::vector< std::string > cov_mat_keys_detVar = { 
    "detVar_total",
    "detVarLYatten",
    "detVarLYdown",
    "detVarLYrayl",
    "detVarRecomb2",
    "detVarSCE",
    "detVarWMAngleXZ",
    "detVarWMAngleYZ",
    "detVarWMX",
    "detVarWMYZ"};
    
    
  const std::vector< std::string > cov_mat_key_totalsumcross = { 
 "xsec_total",
 "xsec_multi",
 "xsec_unisim",
 "xsec_xsr_scc_Fa3_SCC",
 "xsec_xsr_scc_Fv3_SCC"
 /*,"NuWroGenie" removed this  */};

////////////////////////////////
// testing covariance
///////////////////////////////


void testCovariance(){


}




int main() {




return 0; 
}
