// This iS GENIE OverLay Generator 
//Chrisitan Nguyen 
//cnguyen@fnal.gov
//
#pragma once

#include <TTree.h>
#include <TString.h>



#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>



#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <vector>

#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TTree.h"
#include "TAxis.h"
#include "TROOT.h"
#include "TChain.h"
#include "TLegendEntry.h"
#include <TGraphErrors.h>
#include "TLine.h"
#include "TAttLine.h"
#include "TF1.h"


#include "../include/MnvColors.hh"
#include "../include/NamedCategory.hh"
//#include "../includes/EventCategory.hh"
#include "../include/HistFolio_slim.hh"
#include "../include/HistFolio_slim.cpp"
//#include "HistUtils.hh"
#include "../include/UBTH2Poly.h"
#include "../include/HistUtils_cc0pi.hh"
//#include "../ConfigMakerUtils.hh"

// STV analysis includes
//#include "../DAgostiniUnfolder.hh"
//#include "../FiducialVolume.hh"
//#include "../MatrixUtils.hh"
//#include "../MCC9SystematicsCalculator.hh"
//#include "../NormShapeCovMatrix.hh"
//#include "../PGFPlotsDumpUtils.hh"
//#include "../includes/SliceBinning.hh"
////#include "../SliceHistogram.hh"
//#include "../WienerSVDUnfolder.hh"


#include "../../include/XSecAnalyzer/FilePropertiesManager.hh"
#include "../../include/XSecAnalyzer/MCC9SystematicsCalculator.hh"
#include "../../include/XSecAnalyzer/SliceBinning.hh"
#include "../../include/XSecAnalyzer/SliceHistogram.hh"
#include "../../include/XSecAnalyzer/DAgostiniUnfolder.hh"
#include "../../include/XSecAnalyzer/FiducialVolume.hh"
#include "../../include/XSecAnalyzer/MatrixUtils.hh"
#include "../../include/XSecAnalyzer/NormShapeCovMatrix.hh"
#include "../../include/XSecAnalyzer/PGFPlotsDumpUtils.hh"
#include "../../include/XSecAnalyzer/WienerSVDUnfolder.hh"
#include "../../include/XSecAnalyzer/ConfigMakerUtils.hh"


using namespace std;
void GeneratorOverlay();

TH1D *getHist_true_events_nuisance(std::string nuisance_file, 
std::string SAMPLE_NAME, double conv_factor);
void Filland2DBinWidthNormalize(TH1D &inputSlice, TH1D* Hist_byBinN, std::map< int, std::set< size_t > > bin_map, double BinWidth );
