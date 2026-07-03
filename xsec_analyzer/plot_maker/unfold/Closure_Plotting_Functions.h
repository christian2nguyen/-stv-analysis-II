//Header of GENIE Closure
#pragma once
// Standard library includes
// Standard library includes
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// ROOT includes
#include "TCanvas.h"
#include "TLegend.h"
#include <TStyle.h> // Include TStyle header for gStyle
#include <THStack.h>
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
#include "../include/PlotUtils.hh"
//
//#include "../ConfigMakerUtils.hh"
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


/////////////////////////////////////////////////////////
const std::string MicroBooNEType_string =  "MicroBooNE Tune";
constexpr double BIG_DOUBLE = 1e300;
constexpr bool PrintStatement_Debug = false;
constexpr bool PrintStatement_Uncertainty_Debug = false;

/////////////////////////////////////////////////////////////
// Function
/////////////////////////////////////////////////////////////
void multiply_1d_hist_by_matrix( TMatrixD* mat, TH1* hist ) ;
void IncreaseTitleTH1(TH1& hist, double input);
void printMatrixAsLatexTable(const TMatrixD& matrix, const std::string& fileName);
void unfoldingGENIEClosure();

     double FV_X_MIN_unfold =   21.5;
     double FV_X_MAX_unfold =  234.85;

     double FV_Y_MIN_unfold = -95.0;
     double FV_Y_MAX_unfold =  95.0;

     double FV_Z_MIN_unfold =   21.5;
     double FV_Z_MAX_unfold =  966.8;
 
 
FiducialVolume Fiducial_Volumn_CC0Pi{FV_X_MIN_unfold,
                                    FV_X_MAX_unfold, 
                                    FV_Y_MIN_unfold,
                                    FV_Y_MAX_unfold,
                                    FV_Z_MIN_unfold,
                                    FV_Z_MAX_unfold };
