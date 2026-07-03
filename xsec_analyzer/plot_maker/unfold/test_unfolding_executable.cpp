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

//#include "../DAgostiniUnfolder.hh"

//#include "../MatrixUtils.hh"
//#include "../MCC9SystematicsCalculator.hh"
//#include "../NormShapeCovMatrix.hh"
//#include "../PGFPlotsDumpUtils.hh"
//#include "../includes/SliceBinning.hh"
//#include "../SliceHistogram.hh"
//#include "../WienerSVDUnfolder.hh"

#include "../include/PlotUtils.hh"

//#include "../ConfigMakerUtils.hh"
#include "Closure_Plotting_Functions.h"

double calculateChiSquare(TH1D* observed, TH1D* expected, int &NDF);
TMatrixD covarianceToCorrelation(const TMatrixD& covarianceMatrix);
struct TruthFileInfo {
  TruthFileInfo() {}
  TruthFileInfo( const std::string& file_name, int color, int style )
    : file_name_( file_name ), color_( color ), style_( style ) {}

  std::string file_name_;
  int color_;
  int style_;
};

void visualizeMatrix(const TMatrixD & matrix, std::string title, 
                     char *pdfname, std::string XaxisTitle = "", std::string YaxisTitle="");
void visualizeMatrix(const TMatrixD & matrix, std::string title,  
                     char *pdfname, std::string XaxisTitle, 
                     std::string YaxisTitle, double Zmax,  double Zmin = 99 );
                     
void visualizeMatrix(const TMatrixD & matrix, std::string title,  char *pdfname, std::string XaxisTitle,
 std::string YaxisTitle, double Zmax, double Zmin, std::map<std::string, double> lines );
void printMatrixAsLatexTable(const TMatrixD& matrix, const std::string& fileName);
TMatrixD RemoveLastNEntries(const TMatrixD& matrix, int N);
void IncreaseTitleTH1(TH1& hist, double input);
TH1D *ConstuctBinN_FromModelSlices(SliceBinning &sb, std::string ModelName, std::string RootPath);
 void Filland2DBinWidthNormalize(TH1D &inputSlice, TH1D* Hist_byBinN,
std::map< int, std::set< size_t > > bin_map, double BinWidth );
void test_unfolding_ExtraModels();
void test_unfolding_ExtraModels_Inclusive();
void test_unfolding_ExtraModels_Inclusive_noData();
TH1D *ConstuctBinN_FromModelSlices(SliceBinning &sb, std::string ModelName, std::string RootPath);

std::map<std::string, TH1D*> get_true_events_nuisance_BinNum_vector(
SliceBinning &sb, std::vector<std::string> ModelName_Vector, std::string RootPath);

 std::map< std::string, TMatrixD* >CreatedModel_BinNMatrix(SliceBinning &sb,
 std::vector<std::string> ModelName_Vector,std::string RootPath);


std::map< std::string, TMatrixD* > get_true_events_nuisance_v2(
  std::string  SAMPLE_NAME, double conv_factor );

void DrawSlice(
  SliceHistogram* SliceH_Input,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax);

std::map< std::string, TMatrixD* > get_true_events_nuisance_FlatTrees(
 std::map< std::string, TruthFileInfo > FileInfo,const Slice& slice_input,
 double BinWidth );
 
 std::map< std::string, TMatrixD* > get_true_events_nuisance_FlatTrees_BinNum(
 std::map< std::string, TruthFileInfo > FileInfo);
 
 
 void DrawGridCanvas(GridCanvas *GridCanvas_input,
TLegend* lg_input, std::string XaxisTitle, 
std::string YaxisTitle, std::string pdftitle,
double min_YAxis_GridCanvas, double max_YAxis_GridCanvas,
double min_XAxis_GridCanvas, double max_XAxis_GridCanvas );

void unfoldingGENIEClosure(std::string inputFile, std::string inputBinningFile, std::string pdf_type,int BinNSlice, std::map<std::string, double> BinN_lineMap);
void unfolding_NuWroClosure_tests(std::string inputFile, std::string inputBinningFile, std::string pdf_type, bool removebins, int number, std::map<std::string, double> lines);
//void MultplyMatrixDMap_bySingleMatrix(std::map< std::string, TMatrixD* > &InputMap, TMatrixD *SmearingMatrix );
void drawVerticalLinesWithText(std::map<std::string, double> lines, double maxh) ;

   //  double FV_X_MIN_unfold =   21.5;
   //  double FV_X_MAX_unfold =  234.85;
//
   //  double FV_Y_MIN_unfold = -95.0;
   //  double FV_Y_MAX_unfold =  95.0;
//
   //  double FV_Z_MIN_unfold =   21.5;
   //  double FV_Z_MAX_unfold =  966.8;
 
 
//FiducialVolume Fiducial_Volumn_CC0Pi{FV_X_MIN_unfold,
//                                    FV_X_MAX_unfold, 
//                                    FV_Y_MIN_unfold,
//                                    FV_Y_MAX_unfold,
//                                    FV_Z_MIN_unfold,
//                                    FV_Z_MAX_unfold };
//

// Wiener-SVD includes
//#include "svd/include/WienerSVD.h"

// RooUnfold includes
//#include "RooUnfold/src/RooUnfoldResponse.h"
//#include "RooUnfold/src/RooUnfoldBayes.h"

//std::unique_ptr< RooUnfoldResponse > get_test_response(
//  const SystematicsCalculator& sc )
//{
//  // Make a TH2D containing the response (or "smearceptance") matrix elements
//  // in the format expected by RooUnfoldResponse.
//  // Note that RooUnfoldResponse uses the axis convention reco <-> x,
//  // true <-> y for defining the response matrix. In the TMatrixD
//  // representation that I use here, reco <-> row and true <-> column.
//  // Note also that the RooUnfoldResponse constructor clones the input
//  // histograms (rather than taking ownership). In light of that behavior, we
//  // will use temporary histograms that will go out of scope when this function
//  // exits.
//  auto smearcept = sc.get_cv_smearceptance_matrix();
//  auto true_signal = sc.get_cv_true_signal();
//
//  int num_ordinary_reco_bins = smearcept->GetNrows();
//  int num_true_signal_bins = smearcept->GetNcols();
//
//  TH2D resp( "resp", "response matrix", num_ordinary_reco_bins, 0.,
//    num_ordinary_reco_bins, num_true_signal_bins, 0., num_true_signal_bins );
//
//  for ( int r = 0; r < num_ordinary_reco_bins; ++r ) {
//    for ( int t = 0; t < num_true_signal_bins; ++t ) {
//      double elem = smearcept->operator()( r, t );
//      // RooUnfold expects a response matrix normalized in terms of event
//      // counts (as opposed to event fractions). We multiply here by the
//      // CV true signal prediction to convert from one to the other.
//      elem *= true_signal->operator()( t, 0 );
//      // Note that TMatrixD objects have zero-based indices while TH2D objects
//      // have one-based indices. We correct for that explicitly here.
//      resp.SetBinContent( r + 1, t + 1, elem );
//    }
//  }
//
//  // Now prepare TH1D objects for the prior on the true events and the measured
//  // (background-subtracted) reco events
//  TH1D sig( "sig", "true signal", num_true_signal_bins, 0.,
//    num_true_signal_bins );
//
//  for ( int t = 0; t < num_true_signal_bins; ++t ) {
//    double elem = true_signal->operator()( t, 0 );
//    // Note that TMatrixD objects have zero-based indices while TH1D objects
//    // have one-based indices. We correct for that explicitly here.
//    sig.SetBinContent( t + 1, elem );
//  }
//
//  // Get the background-subtracted measurement
//  auto meas = sc.get_measured_events();
//  const auto& reco_signal = meas.reco_signal_;
//
//  TH1D reco( "reco", "background-subtracted data", num_ordinary_reco_bins, 0.,
//    num_ordinary_reco_bins );
//
//  for ( int r = 0; r < num_ordinary_reco_bins; ++r ) {
//    double elem = reco_signal->operator()( r, 0 );
//    // Note that TMatrixD objects have zero-based indices while TH1D objects
//    // have one-based indices. We correct for that explicitly here.
//    reco.SetBinContent( r + 1, elem );
//  }
//
//  auto result = std::make_unique< RooUnfoldResponse >( &reco, &sig, &resp,
//    "MyTestResponse", "test response" );
//
//  return result;
//}
constexpr bool DEBUG_PLOTS = false;
//constexpr bool PrintStatement_Debug = false;
//constexpr bool PrintStatement_Uncertainty_Debug = false;
//constexpr double BIG_DOUBLE = 1e300;
const double DATA_POT = 4.54e+19;

void multiply_1d_hist_by_matrix( TMatrixD* mat, TH1* hist ) {
  // Copy the histogram contents into a column vector
  int num_bins = mat->GetNcols();
  TMatrixD hist_mat( num_bins, 1 );
  for ( int r = 0; r < num_bins; ++r ) {
    hist_mat( r, 0 ) = hist->GetBinContent( r + 1 );
  }

  // Multiply the column vector by the input matrix
  // TODO: add error handling here related to matrix dimensions
  TMatrixD hist_mat_transformed( *mat, TMatrixD::EMatrixCreatorsOp2::kMult,
    hist_mat );

  // Update the input histogram contents with the new values
  for ( int r = 0; r < num_bins; ++r ) {
    double val = hist_mat_transformed( r, 0 );
    hist->SetBinContent( r + 1, val );
  }

}

                                
const std::string SAMPLE_NAME = "MicroBooNE_CC1MuNp_XSec_2D_PmuCosmu_nu_MC";
//const std::string SAMPLE_NAME = "MicroBooNE_CC1MuNp_XSec_2D_PpCosp_nu_MC";
const std::string SAMPLE_NAME_2DFlat = "TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm"; //TrueMuon_PmuCosThetaPlotBinningscheme1_NoNorm//TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm // TrueMuonPmuCosTheta_binN_scheme1
const std::string SAMPLE_NAME_2DFlat_BinScheme2 = "TrueMuonPmuCosTheta_binN_scheme2_noBinWidthNorm";
//const std::string MicroBooNEType_string =  "#muBooN";
// TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm

// Keys are generator names and versions, values are TruthFileInfo objects
// describing nuiscomp output files containing the differential cross-section
// predictions in each true bin
std::map< std::string, TruthFileInfo > truth_file_map = {
  { "GENIE 2.12.10",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv2.root", kBlue, 1 } },
  { "GENIE 3.0.6",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3.root", kBlack, 2} },
  { "GENIE 3.2.0 G18_02a",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3-g1802a.root", kBlack, 2} },
  { "GENIE 3.2.0 G21_11a",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3-g2111a.root", kBlack, 2} },
  { "GENIE 3.2.0 G21_11b",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3-g2111b.root", kBlack, 2} },
  { "NEUT 5.6.0",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-neut.root", kRed, 9} },
  { "NuWro 19.02.2",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-nuwro.root", kViolet, 7} },
 { "GiBUU 2021.1",
    {"/exp/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/mygibuu3.root", kGreen, 10} },
};

std::map< std::string, std::string > samples_to_hist_names {
  { "unfolded data", "UnfData" },
  { "MicroBooNE Tune", "uBTune" },
  { "truth", "FakeData" },
  { "GENIE 2.12.10", "gv2" },
  { "GENIE 3.0.6", "gv3" },
  { "GENIE 3.2.0 G18_02a", "g1802a" },
  { "GENIE 3.2.0 G21_11a", "g2111a" },
  { "GENIE 3.2.0 G21_11b", "g2111b" },
  { "NEUT 5.6.0", "neut" },
  { "NuWro 19.02.2", "nuwro" },
  { "GiBUU 2021.1", "gibuu" },
};


std::map< std::string, TruthFileInfo > truth_CC0Pifile_map = {
  { "GENIE 3.0.6",
    {"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/CC0Pi_Flat_2D_GENIE_v3_0_6.root", kBlue, 1 } },
  { "GENIE 2.12.10",
    {"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/CC0Pi_Flat_2D_GENIE_v2_12_10.root", kGreen+3, 1} },
  { "NuWro 19.02.1",
    {"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/CC0Pi_Flat_2D_NEUT_5_4_0_1.root", kRed, 9} },
  { "NEUT 5.4.0.1",
    {"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/CC0Pi_Flat_2D_NuWro_19_02_1.root", kViolet, 7}},
  { "GiBUU 2023.3",
    {"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/CC0Pi_Flat_2D_GiBUU_2023.root", kTeal, 1}}
};

// REmoved this model 
  //{ "GENIE 2.12.10(MEC)",
   // {"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/CC0Pi_Flat_2D_GENIE_v2_12_10_MEC.root", kGreen, 2} },
   //"GENIEv2_12_10_MEC",

std::vector<std::string> ModelName_Global{"GENIE_v3_0_6","GENIEv2_12_10","NuWro_19_02_1","NEUT_5_4_0_1","GiBUU_2023_3"}; 

std::vector<double> BinWidth_PmuProjection{0.23,0.06,0.08,0.1,0.22,0.15,0.43,0.3,0.42,1.0}; // ADDED A EXTRA 1.0 FOR BIN N PLOT


const std::vector< std::string > cov_mat_keys = {  
   "total",
   "flux",
   "xsec_total",
   "reint",
   "detVar_total",
    "numTargets",
    "POT",
    "MCstats",
    "EXTstats", 
    "BNBstats"};
  
const std::vector< std::string > cov_mat_keys_cross = {  
     "xsec_unisim",
     "xsec_AxFFCCQEshape",
     "xsec_DecayAngMEC",
     "xsec_NormCCCOH",
     "xsec_NormNCCOH",

     "xsec_ThetaDelta2NRad",
     "xsec_Theta_Delta2Npi",
     "xsec_VecFFCCQEshape"
     };
   
   //     "xsec_RPA_CCQE ","Xsec_XSecShape_CCMEC"
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
 "xsec_xsr_scc_Fv3_SCC"/*,
 "NuWroGenie"*/};


const std::vector<std::vector< std::string > > cov_mat_KEYS{cov_mat_keys, cov_mat_keys_cross, cov_mat_keys_detVar, cov_mat_key_totalsumcross};


struct SampleInfo {

  SampleInfo() {}

  SampleInfo( const std::string& resp, const std::string& width,
    const std::string& sb ) : respmat_file_( resp ), widths_file_( width ),
    sb_file_( sb ) {}

  // File containing the output of respmat.C with the same true binning
  // as was used in NUISANCE to make theoretical predictions
  std::string respmat_file_;

  // NUISANCE data file in which the bin widths (along each relevant axis) are
  // stored. These files are used by the preliminary NUISANCE implementation of
  // this cross-section measurement.
  std::string widths_file_;

  // SliceBinning configuration file (needs to be consistent with the bin
  // definitions used in the other two files)
  std::string sb_file_;
};

// Keys are NUISANCE sample names, values are SampleInfo objects that give
// the corresponding input file paths needed for this script
std::map< std::string, SampleInfo > sample_info_map = {

  // 2D muon measurement
  { "MicroBooNE_CC1MuNp_XSec_2D_PmuCosmu_nu_MC", { "/uboone/data/users"
    "/gardiner/respmat-test-new-Muon2D.root", "/exp/uboone/app/users/gardiner"
    "/stv/mc/nuisance/data/MicroBooNE/mybins2Dmuon.txt",
    "mybins_mcc9_2D_muon.txt" } },
        
  // 2D proton measurement
  { "MicroBooNE_CC1MuNp_XSec_2D_PpCosp_nu_MC", { "/uboone/data/users"
    "/gardiner/respmat-test-new-Proton2D.root", "/exp/uboone/app/users/gardiner"
    "/stv/mc/nuisance/data/MicroBooNE/mybins2Dproton.txt",
    "mybins_mcc9_2D_proton.txt" } },

};

// Multiplying by the conversion factor conv_factor should change a total cross
// section value (in 10^{-38} cm^2 / Ar) into an expected number of true event
// counts. The value of the factor can be obtained by multiplying the number of
// Ar targets in the fiducial volume by the integrated numu flux (for the
// measured POT exposure) in the fiducial volume.
//
// This function returns a map in which the keys are legend labels for each
// generator model. The values are TMatrixD column vectors containing the
// expected true event counts (directly comparable to unfolded event counts
// from the data).
std::map< std::string, TMatrixD* > get_true_events_nuisance(
  const SampleInfo& info, double conv_factor )
{
  // We'll create one prediction per generator model in the truth_file_map
  std::map< std::string, TMatrixD* > truth_counts_map;
  for ( const auto& pair : truth_file_map ) {
    // First retrieve the raw NUISANCE histogram. It is expressed as a
    // total cross section with true bin number along the x-axis
    std::string generator_label = pair.first;
    std::cout<<"Making Nuisance Hists Label : "<< generator_label<< std::endl;
    const auto& file_info = pair.second;
    std::string nuisance_file = file_info.file_name_;
    TFile temp_in_file( nuisance_file.c_str(), "read" );
    TH1D* temp_hist = nullptr;
    temp_in_file.GetObject( SAMPLE_NAME.c_str(), temp_hist );

    // Set the associated directory to a null pointer. That way this histogram
    // won't be auto-deleted when the corresponding TFile object goes out of
    // scope
    temp_hist->SetDirectory( nullptr );

    // Set the style of the histogram to match the configuration in the
    // TruthFileInfo object
    temp_hist->SetLineColor( file_info.color_ );
    temp_hist->SetLineStyle( file_info.style_ );

    // Disable displaying the stats box
    temp_hist->SetStats( false );

    // Convert the content (and error) of each bin to an expected true event
    // count. Do this using the input conversion factor (integrated numu
    // flux * number of Ar targets in the fiducial volume) and the 2D bin
    // width.
    size_t num_bins = temp_hist->GetNbinsX();
    
    for ( size_t b = 0u; b < num_bins; ++b ) {
      double xsec = temp_hist->GetBinContent( b + 1 );
      double err = temp_hist->GetBinError( b + 1 );
      temp_hist->SetBinContent( b + 1, xsec * conv_factor );
      temp_hist->SetBinError( b + 1, err * conv_factor );
    }

    // Now change the TH1D into a TMatrixD column vector
    TMatrixD* temp_mat = new TMatrixD( num_bins, 1 );
    for ( size_t b = 0u; b < num_bins; ++b ) {
      temp_mat->operator()( b, 0 ) = temp_hist->GetBinContent( b + 1 );
    }

    // The conversion is done, so add the finished true event counts histogram
    // to the map
    truth_counts_map[ generator_label ] = temp_mat;
  }

  return truth_counts_map;
}
///////////////////////////////////////////////////////////////////////////
std::map< std::string, TMatrixD* > get_true_events_nuisance_v2(
  std::string  SAMPLE_NAME, double conv_factor )
{
  // We'll create one prediction per generator model in the truth_file_map
  std::map< std::string, TMatrixD* > truth_counts_map;
  for ( const auto& pair : truth_CC0Pifile_map ) {
    // First retrieve the raw NUISANCE histogram. It is expressed as a
    // total cross section with true bin number along the x-axis
    std::string generator_label = pair.first;
    std::cout<<"Making Nuisance Hists Label : "<< generator_label<< std::endl;
    const auto& file_info = pair.second;
    std::string nuisance_file = file_info.file_name_;
    TFile temp_in_file( nuisance_file.c_str(), "read" );
    TH1D* temp_hist = nullptr;
    temp_in_file.GetObject( SAMPLE_NAME.c_str(), temp_hist );

    // Set the associated directory to a null pointer. That way this histogram
    // won't be auto-deleted when the corresponding TFile object goes out of
    // scope
    temp_hist->SetDirectory( nullptr );

    // Set the style of the histogram to match the configuration in the
    // TruthFileInfo object
    temp_hist->SetLineColor( file_info.color_ );
    temp_hist->SetLineStyle( file_info.style_ );

    // Disable displaying the stats box
    temp_hist->SetStats( false );

    // Convert the content (and error) of each bin to an expected true event
    // count. Do this using the input conversion factor (integrated numu
    // flux * number of Ar targets in the fiducial volume) and the 2D bin
    // width.
    size_t num_bins = temp_hist->GetNbinsX();
    std::cout<< "get_true_events_nuisance_v2 :: NBins =  "<< num_bins << std::endl;
    for ( size_t b = 0u; b < num_bins; ++b ) {
      double xsec = temp_hist->GetBinContent( b + 1 );
      double err = temp_hist->GetBinError( b + 1 );
      temp_hist->SetBinContent( b + 1, xsec * conv_factor );
      temp_hist->SetBinError( b + 1, err * conv_factor );
    }

    // Now change the TH1D into a TMatrixD column vector
    TMatrixD* temp_mat = new TMatrixD( num_bins, 1 );
    for ( size_t b = 0u; b < num_bins; ++b ) {
      temp_mat->operator()( b, 0 ) = temp_hist->GetBinContent( b + 1 );
    }

    // The conversion is done, so add the finished true event counts histogram
    // to the map
    truth_counts_map[ generator_label ] = temp_mat;
  }

  return truth_counts_map;
}
///////////////////////////////////////////////////////////////////////////
void Filland2DBinWidthNormalize(TH1D &inputSlice, TH1D* Hist_byBinN,
std::map< int, std::set< size_t > > bin_map, double BinWidth ){


  for (auto map1:bin_map)
    {
      for(auto set:map1.second ){
        inputSlice.SetBinContent(map1.first, Hist_byBinN->GetBinContent(set+1));
        inputSlice.SetBinError(map1.first, Hist_byBinN->GetBinError(set+1) );
        
     std::cout<<"map key Bin To Set: "<< map1.first<< " get Contenct from "<< set << " Rate : "<< Hist_byBinN->GetBinContent(set+1)<< std::endl;
   // std::cout<< set <<" ,";
   }
    //std::cout<<std::endl;
    
  }

    inputSlice.Scale(1.0 / BinWidth);
    inputSlice.Scale(1.0,"width"); 
    
}
///////////////////////////////////////////////////////////////////////////
std::map< std::string, TMatrixD* > get_true_events_nuisance_FlatTrees(
 std::map< std::string, TruthFileInfo > FileInfo, const Slice& slice_input,
 double BinWidth )
{
  // We'll create one prediction per generator model in the truth_file_map
  std::map< std::string, TMatrixD* > truth_counts_map;
  for ( const auto& pair : FileInfo ) {
  
    // First retrieve the raw NUISANCE histogram. It is expressed as a
    // total cross section with true bin number along the x-axis
    std::string generator_label = pair.first;
    std::cout<<"Making Nuisance Hists Label : "<< generator_label<< std::endl;
    const auto& file_info = pair.second;
    std::string nuisance_file = file_info.file_name_;
    TFile temp_in_file( nuisance_file.c_str(), "read" );
    TH1D* temp_hist_BinN = nullptr;
    temp_in_file.GetObject( SAMPLE_NAME_2DFlat.c_str(), temp_hist_BinN );
    // By Bin Number 
    std::cout << "temp_hist_BinN Number of Bins = "<< temp_hist_BinN->GetNbinsX()<< std::endl;
    
    TH1D* temp_hist = (TH1D*)slice_input.hist_->Clone(uniq());
  
    temp_hist->SetDirectory( nullptr );

    // Set the style of the histogram to match the configuration in the
    // TruthFileInfo object
    temp_hist->SetLineColor( file_info.color_ );
    temp_hist->SetLineStyle( file_info.style_ );

    // Disable displaying the stats box
    temp_hist->SetStats( false );
    
    Filland2DBinWidthNormalize(*temp_hist, temp_hist_BinN, slice_input.bin_map_, BinWidth );

  //TH1D* temp_hist = h_Slice->Clone(uniq());

    // Set the associated directory to a null pointer. That way this histogram
    // won't be auto-deleted when the corresponding TFile object goes out of
    // scope


    // Convert the content (and error) of each bin to an expected true event
    // count. Do this using the input conversion factor (integrated numu
    // flux * number of Ar targets in the fiducial volume) and the 2D bin
    // width.
    size_t num_bins = temp_hist->GetNbinsX();
        std::cout << "temp_hist of Bins = "<< num_bins<< std::endl;

    // Now change the TH1D into a TMatrixD column vector
    TMatrixD* temp_mat = new TMatrixD( num_bins, 1 );
    for ( size_t b = 0u; b < num_bins; ++b ) {
    std::cout<< "b =" << b << "temp_hist->GetBinContent( b + 1 ) " << temp_hist->GetBinContent( b + 1 )<< std::endl; 
      temp_mat->operator()( b, 0 ) = temp_hist->GetBinContent( b + 1 );
      //temp_mat(b, 0) = temp_hist->GetBinContent(b + 1);
    }

    // The conversion is done, so add the finished true event counts histogram
    // to the map
    truth_counts_map[ generator_label ] = temp_mat;
  }

  return truth_counts_map;
}
//////////////////////////////////////////////////////////////////////////
std::map< std::string, TMatrixD* > get_true_events_nuisance_FlatTrees_BinNum(
 std::map< std::string, TruthFileInfo > FileInfo, double conv_factor )
{
  // We'll create one prediction per generator model in the truth_file_map
  std::map< std::string, TMatrixD* > truth_counts_map;
  for ( const auto& pair : FileInfo ) {
  
    // First retrieve the raw NUISANCE histogram. It is expressed as a
    // total cross section with true bin number along the x-axis
    std::string generator_label = pair.first;
    std::cout<<"Making Nuisance Hists Label : "<< generator_label<< std::endl;
    const auto& file_info = pair.second;
    
    std::string nuisance_file = file_info.file_name_;
    TFile temp_in_file( nuisance_file.c_str(), "read" );
    TH1D* temp_hist= nullptr;
    temp_in_file.GetObject( SAMPLE_NAME_2DFlat.c_str(), temp_hist );
    // By Bin Number 
    //std::cout << "temp_hist_BinN Number of Bins = "<< temp_hist_BinN->GetNbinsX()<< std::endl;
    
    //TH1D* temp_hist = (TH1D*)slice_input.hist_->Clone(uniq());
  
    temp_hist->SetDirectory( nullptr );

    // Set the style of the histogram to match the configuration in the
    // TruthFileInfo object
    temp_hist->SetLineColor( file_info.color_ );
    temp_hist->SetLineStyle( file_info.style_ );

    // Disable displaying the stats box
    temp_hist->SetStats( false );
    
   // Filland2DBinWidthNormalize(*temp_hist, temp_hist_BinN, slice_input.bin_map_, BinWidth );

  //TH1D* temp_hist = h_Slice->Clone(uniq());

    // Set the associated directory to a null pointer. That way this histogram
    // won't be auto-deleted when the corresponding TFile object goes out of
    // scope


    // Convert the content (and error) of each bin to an expected true event
    // count. Do this using the input conversion factor (integrated numu
    // flux * number of Ar targets in the fiducial volume) and the 2D bin
    // width.
    size_t num_bins = temp_hist->GetNbinsX();
    std::cout << "temp_hist of Bins = "<< num_bins<< std::endl;

    // Now change the TH1D into a TMatrixD column vector
    TMatrixD* temp_mat = new TMatrixD( num_bins, 1 );
    for ( size_t b = 0u; b < num_bins; ++b ) {
    std::cout<< "b =" << b << "temp_hist->GetBinContent( b + 1 ) " << temp_hist->GetBinContent( b + 1 )<< std::endl; 
      temp_mat->operator()( b, 0 ) = temp_hist->GetBinContent( b + 1 );
      //temp_mat(b, 0) = temp_hist->GetBinContent(b + 1);
    }

    // The conversion is done, so add the finished true event counts histogram
    // to the map
    truth_counts_map[ generator_label ] = temp_mat;
  }

  return truth_counts_map;
}
////////////////////////////////////////////////////////////////////////////


void dump_slice_errors_local( const std::string& hist_col_prefix,
  const Slice& slice, const std::map< std::string,
  std::unique_ptr<SliceHistogram> >& slice_hist_cov_matrix_map,
  std::map< std::string, std::vector<double> >& pgf_plots_hist_table )
{
  for ( const auto& pair : slice_hist_cov_matrix_map ) {
    std::string err_name = pair.first;
    std::string err_col_name = hist_col_prefix + '_' + err_name + "_error";
    pgf_plots_hist_table[ err_col_name ] = std::vector<double>();
  }

  for ( const auto& bin_pair : slice.bin_map_ ) {
    // TODO: revisit for multi-dimensional slices
    int global_bin_idx = bin_pair.first;

    for ( const auto& err_pair : slice_hist_cov_matrix_map ) {

      std::string err_name = err_pair.first;
      std::string err_col_name = hist_col_prefix + '_' + err_name + "_error";

      const auto* hist = err_pair.second->hist_.get();
      double err = hist->GetBinError( global_bin_idx );

      pgf_plots_hist_table.at( err_col_name ).push_back( err );
    }

  } // slice bins

  // Add a (presumably empty) overflow bin to get certain PGFPlots styles to
  // look right.
  for ( const auto& err_pair : slice_hist_cov_matrix_map ) {
    std::string err_name = err_pair.first;
    std::string err_col_name = hist_col_prefix + '_' + err_name + "_error";

    pgf_plots_hist_table.at( err_col_name ).push_back( 0. );
  }

}

// Helper function that dumps a lot of the results to simple text files.
// The events_to_xsec_factor is a constant that converts expected true event
// counts to a total cross section (10^{-38} cm^2 / Ar) via multiplication.
void dump_overall_results_local( const UnfoldedMeasurement& result,
  const std::map< std::string, std::unique_ptr<TMatrixD> >& unf_cov_matrix_map,
  double events_to_xsec_factor, const TMatrixD& genie_cv_true_events,
  const TMatrixD& fake_data_true_events,
  const std::map< std::string, TMatrixD* >& generator_truth_map,
  bool using_fake_data )
{
  // Dump the unfolded flux-averaged total cross sections (by converting
  // the units on the unfolded signal event counts)
  TMatrixD unf_signal = *result.unfolded_signal_;
  unf_signal *= events_to_xsec_factor;
  dump_text_column_vector( "dump/vec_table_unfolded_signal.txt", unf_signal );

  // Dump similar tables for each of the theoretical predictions (and the fake
  // data truth if applicable). Note that this function expects that the
  // additional smearing matrix A_C has not been applied to these predictions.
  TMatrixD temp_genie_cv = genie_cv_true_events;
  temp_genie_cv *= events_to_xsec_factor;
  dump_text_column_vector( "dump/vec_table_uBTune.txt", temp_genie_cv );

  if ( using_fake_data ) {
    TMatrixD temp_fake_truth = fake_data_true_events;
    temp_fake_truth *= events_to_xsec_factor;
    dump_text_column_vector( "dump/vec_table_FakeData.txt", temp_fake_truth );
  }

  for ( const auto& gen_pair : generator_truth_map ) {
    std::string gen_short_name = samples_to_hist_names.at( gen_pair.first );
    TMatrixD temp_gen = *gen_pair.second;
    temp_gen *= events_to_xsec_factor;
    dump_text_column_vector( "dump/vec_table_" + gen_short_name + ".txt",
      temp_gen );
  }

  // No unit conversions are necessary for the unfolding, error propagation,
  // and additional smearing matrices since they are dimensionless
  dump_text_matrix( "dump/mat_table_unfolding.txt", *result.unfolding_matrix_ );
  dump_text_matrix( "dump/mat_table_err_prop.txt", *result.err_prop_matrix_ );
  dump_text_matrix( "dump/mat_table_add_smear.txt", *result.add_smear_matrix_ );

  // Convert units on the covariance matrices one-by-one and dump them
  for ( const auto& cov_pair : unf_cov_matrix_map ) {
    const auto& name = cov_pair.first;
    TMatrixD temp_cov_matrix = *cov_pair.second;
    // Note that we need to square the unit conversion factor for the
    // covariance matrix elements
    temp_cov_matrix *= std::pow( events_to_xsec_factor, 2 );
    dump_text_matrix( "dump/mat_table_cov_" + name + ".txt", temp_cov_matrix );
  }

  // Finally, dump a summary table of the flux-averaged total cross section
  // measurements and their statistical and total uncertainties
  TMatrixD temp_stat_cov = *unf_cov_matrix_map.at( "DataStats" );
  TMatrixD temp_total_cov = *unf_cov_matrix_map.at( "total" );
  temp_stat_cov *= std::pow( events_to_xsec_factor, 2 );
  temp_total_cov *= std::pow( events_to_xsec_factor, 2 );

  // Open the output file and set up the output stream so that full numerical
  // precision is preserved in the ascii text representation
  std::ofstream out_summary_file( "dump/xsec_summary_table.txt" );
  out_summary_file << std::scientific
    << std::setprecision( std::numeric_limits<double>::max_digits10 );

  int num_bins = unf_signal.GetNrows();
  out_summary_file << "numXbins " << num_bins;

  for ( int bin = 0; bin < num_bins; ++bin ) {
    double xsec = unf_signal( bin, 0 );
    double stat_err = std::sqrt( std::max(0., temp_stat_cov(bin, bin)) );
    double total_err = std::sqrt( std::max(0., temp_total_cov(bin, bin)) );
    out_summary_file << '\n' << bin << "  " << xsec << "  " << stat_err
      << "  " << total_err;
  }
}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void test_unfolding() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  //auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties.txt" );
  
  
  std::string Pdf_name  = "CrossSection_2D_test";
  
  char pdf_title[1024];
  
 std::cout<<"Running : Test unfolding () "<< std::endl;

  const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

    //const std::string respmat_file_name(
    //  "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name(
    "/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root" );
  //UnivMake_FakeData_BDTdecided_1D_v10_noBDTproton_pmucorrection.root
  
  //-rw-r--r-- 1 cnguyen microboone  398895883 Mar  9 03:11
  //-rw-r--r-- 1 cnguyen microboone  591920210 Mar  9 03:18 UnivMake_FakeData_2D_inclusive_v2_NoBDTprotron_pmucorrection.root
  //-rw-r--r-- 1 cnguyen microboone  689790553 Mar  9 19:06 UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root


  // Do the systematics calculations in preparation for unfolding
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc_unfold_fd.conf" );
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc.conf" );
  auto& syst = *syst_ptr;

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );

  UnfoldedMeasurement result = unfolder->unfold( syst );

  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );

  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;

  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    auto temp_cov_mat = matrix_pair.second.get_matrix();

    TMatrixD temp_mat( *temp_cov_mat, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );


  std::cout<<"Checking what keys are in unfolded_cov_matrix_map" << std::endl;






  //// Test against RooUnfold implementation of the D'Agostini method
  //auto test_response = get_test_response( mcc9 );
  //RooUnfoldBayes roounfold_unfolder( test_response.get(),
  //  test_response->Hmeasured(), NUM_DAGOSTINI_ITERATIONS );
  //auto measured = mcc9.get_measured_events();
  //roounfold_unfolder.SetMeasuredCov( *measured.cov_matrix_ );
  //roounfold_unfolder.IncludeSystematics( 1 );

  //auto* roo_hist = roounfold_unfolder.Hreco(
  //  RooUnfold::ErrorTreatment::kCovariance );

  //auto roo_cov = roounfold_unfolder.Ereco(
  //  RooUnfold::ErrorTreatment::kCovariance );

  //for ( int t = 0; t < num_true_signal_bins; ++t ) {
  //  double mine = result.unfolded_signal_->operator()( t, 0 );
  //  double my_err = std::sqrt( std::max(0.,
  //    result.cov_matrix_->operator()( t, t )) );
  //  double theirs = roo_hist->GetBinContent( t + 1 );
  //  double their_err = std::sqrt( std::max(0., roo_cov(t, t)) );

  //  std::cout << "t = " << t << ' ' << mine << " ± " << my_err
  //    << "  " << theirs << " ± " << their_err << "  "
  //    << (mine - theirs) / theirs << " ± "
  //    << (my_err - their_err) / their_err << '\n';
  //}

  //auto smearcept = mcc9.get_cv_smearceptance_matrix();
  //auto true_signal = syst.get_cv_true_signal();

  //// Sanity check (these were equal to each other as expected)
  //// int num_reco_bins = smearcept->GetNrows();
  ////TMatrixD reco_signal( *smearcept, TMatrixD::kMult, *true_signal );
  ////auto data_sig = mcc9.get_cv_ordinary_reco_signal();
  ////for ( int r = 0; r < num_reco_bins; ++r ) {
  ////  double r1 = reco_signal( r, 0 );
  ////  double r2 = data_sig->operator()( r, 0 );
  ////  std::cout << "r = " << r << ", r1 = " << r1 << ", r2 = " << r2
  ////    << ", diff = " << r1 - r2 << '\n';
  ////}

  //auto meas = mcc9.get_measured_events();
  //// ASIMOV TEST: USE CV RECO SIGNAL PREDICTION INSTEAD OF
  //// BACKGROUND-SUBTRACTED DATA
  ////const auto& data_signal = meas.reco_signal_;
  //auto data_signal = mcc9.get_cv_ordinary_reco_signal();
  //const auto& data_covmat = meas.cov_matrix_;

  //auto result = unfolder->unfold( *data_signal, *data_covmat,
  //  *smearcept, *true_signal );

  ////// Test with original implementation
  ////TVectorD true_sig_vec( num_true_bins );
  ////for ( int t = 0; t < num_true_bins; ++t ) {
  ////  true_sig_vec( t ) = true_signal->operator()( t, 0 );
  ////}

  ////TVectorD data_sig_vec( num_ordinary_reco_bins );
  ////for ( int r = 0; r < num_ordinary_reco_bins; ++r ) {
  ////  data_sig_vec( r ) = data_signal->operator()( r, 0 );
  ////}

  ////TMatrixD A_C_svd( num_true_bins, num_true_bins );
  ////TVectorD WF_svd( num_true_bins );
  ////TMatrixD UnfoldCov_svd( num_true_bins, num_true_bins );

  ////TVectorD wsvd_unfolded_signal = WienerSVD( *smearcept, true_sig_vec,
  ////  data_sig_vec, *data_covmat, 0, 0, A_C_svd, WF_svd, UnfoldCov_svd, 0. );

  //for ( int t = 0; t < num_true_bins; ++t ) {
  //  double t_evts = true_signal->operator()( t, 0 );
  //  double evts = result.unfolded_signal_->operator()( t, 0 );
  //  double error = std::sqrt( std::max(0.,
  //    result.cov_matrix_->operator()( t, t )) );
  //  //double evts_svd = wsvd_unfolded_signal( t );
  //  //double error_svd = std::sqrt( std::max(0.,
  //  //  UnfoldCov_svd( t, t )) );
  //  std::cout << "t = " << t << ", t_evts = " << t_evts
  //    << ", evts = " << evts << " ± " << error
  //    << ", diff = " << t_evts - evts << '\n';
  //}

  // Set the event counts in each bin of the histogram that displays the
  // unfolded result. Note that we don't care about the background true bins
  // (which are assumed to follow all of the signal true bins) since we've
  // subtracted out an estimate of the background before unfolding.
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
    }
  }

  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }

  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );

  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );

    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  }

  TLegend* lg = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg->AddEntry( unfolded_events, "unfolded", "l" );
  lg->AddEntry( genie_cv_truth, "uB tune", "l" );
  if ( using_fake_data ) {
    lg->AddEntry( fake_data_truth_hist, "truth", "l" );
  }

  lg->Draw( "same" );

  // Plot slices of the unfolded result
  auto* sb_ptr = new SliceBinning( "../mybins_mcc9_2D_muon_nosidebands.txt");
  auto& sb = *sb_ptr;
  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Get the factors needed to convert to cross-section units
  double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);

  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  // Retrieve the true-space expected event counts from NUISANCE output files
  // for each available generator model
  double conv_factor = ( num_Ar * integ_flux ) / 1e38;
  std::cout<< " about to run: get_true_events_nuisance"<< std::endl;
  
  auto generator_truth_map = get_true_events_nuisance( sample_info,
    conv_factor );
std::cout<< " Finished"<< std::endl;
  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  // truth-level prediction TMatrixD objects via multiplication by A_C
  dump_overall_results_local( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
    genie_cv_truth_vec, fake_data_truth, generator_truth_map,
    using_fake_data );

  if ( USE_ADD_SMEAR ) {

    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;

    // Start with the fake data truth if present
    if ( using_fake_data ) {
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
    }

    // Also transform the GENIE CV model
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;

    // Now do the other generator predictions
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;

      TMatrixD ac_temp( A_C, TMatrixD::kMult, *truth_mat );
      *truth_mat = ac_temp;
    }
  }



  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);




  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    const auto& slice = sb.slices_.at( sl_idx );

    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;

      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[ "unfolded data" ] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;

    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;

      SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );

      slice_gen_map[ model_name ] = temp_slice;
      
      
      
    }

    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
    }

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " (10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar)";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      slice_h->transform( trans_mat );
      slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
    }

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
 
    
    
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      slice_h->transform( trans_mat );
      
      
      
      
    }






    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == "unfolded data" ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at( "unfolded data" );
      }

      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
    }

    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    slice_unf->hist_->Draw( "e" );
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == "unfolded data" || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_file_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 4 );

      slice_h->hist_->Draw( "hist same" );
    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    slice_cv->hist_->Draw( "hist same" );

    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 5 );
      slice_truth->hist_->Draw( "hist same" );
    }

    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    slice_unf->hist_->Draw( "e same" );

    TLegend* lg = new TLegend( 0.15, 0.6, 0.5, 0.88 );
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;
      std::ostringstream oss;
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) oss << 's';

      if ( name != "unfolded data" ) {
        label += ": #chi^{2} = " + oss.str();
      }

      lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
    }

    lg->Draw( "same" );

    // Dump the unfolded results to text files compatible with PGFPlots
    std::map< std::string, std::vector<double> > slice_hist_table;
    std::map< std::string, std::string > slice_params_table;

    dump_slice_variables( sb, sl_idx, slice_params_table );

    for ( const auto& pair : slice_gen_map ) {
      const auto hist_name = samples_to_hist_names.at( pair.first );
      const auto* slice_hist = pair.second;
      bool include_x_coords = ( hist_name == "UnfData" );
      bool include_y_error = include_x_coords;
      dump_slice_histogram( hist_name, *slice_hist, slice, slice_hist_table,
        include_y_error, include_x_coords );
    }

    dump_slice_plot_limits( *slice_unf, *slice_cv, slice, slice_params_table );

    dump_slice_errors_local( "UnfData", slice, sh_cov_map, slice_hist_table );

    // Dump the chi^2 test results
    for ( const auto& chi2_pair : chi2_map ) {
      const auto hist_name = samples_to_hist_names.at( chi2_pair.first );
      const auto& chi2_result = chi2_pair.second;

      // Comparing the data histogram to itself is trivial, so skip it
      if ( hist_name == "UnfData" ) continue;
      else {
        slice_params_table[ hist_name + "_chi2" ]
          = std::to_string( chi2_result.chi2_ );
        slice_params_table[ hist_name + "_pvalue" ]
          = std::to_string( chi2_result.p_value_ );
      }
    }

    // Dump the total data POT and number of bins in the slice
    slice_params_table[ "bnb_data_pot" ] = std::to_string( total_pot );
    slice_params_table[ "num_bins" ] = std::to_string( num_slice_bins );

    // Dump a LaTeX title for the y-axis
    slice_params_table[ "y_axis_title" ] = slice_y_latex_title;

    // Before moving on to the next slice, dump information about the
    // current one to new pgfplots files that can be used for offline plotting
    std::string output_file_prefix = "dump/pgfplots_slice_";
    // Use at least three digits for numbering the slice output files
    if ( sl_idx < 10 ) output_file_prefix += '0';
    if ( sl_idx < 100 ) output_file_prefix += '0';
    output_file_prefix += std::to_string( sl_idx );

    write_pgfplots_files( output_file_prefix, slice_hist_table,
      slice_params_table );


  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);

  } // slices
  
    sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
    c1 -> Print(pdf_title);
  return;

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "uB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1 -> Print(pdf_title);

}

///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void test_unfoldingTest() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  //auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties.txt" );
  
  
  std::string Pdf_name  = "CrossSection_2D_NoModels";
  
  char pdf_title[1024];

 std::cout<<"Running : Test unfolding () "<< std::endl;

  const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  //  const std::string respmat_file_name(
  //    "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name(
    "/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root" );
  //UnivMake_FakeData_BDTdecided_1D_v10_noBDTproton_pmucorrection.root
  
  //-rw-r--r-- 1 cnguyen microboone  398895883 Mar  9 03:11
  //-rw-r--r-- 1 cnguyen microboone  591920210 Mar  9 03:18 UnivMake_FakeData_2D_inclusive_v2_NoBDTprotron_pmucorrection.root
  //-rw-r--r-- 1 cnguyen microboone  689790553 Mar  9 19:06 UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root


  // Do the systematics calculations in preparation for unfolding
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc_unfold_fd.conf" );
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc.conf" );
  auto& syst = *syst_ptr;

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );

  UnfoldedMeasurement result = unfolder->unfold( syst );




  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );

  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;

  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    auto temp_cov_mat = matrix_pair.second.get_matrix();

    TMatrixD temp_mat( *temp_cov_mat, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );

  //// Test against RooUnfold implementation of the D'Agostini method
  //auto test_response = get_test_response( mcc9 );
  //RooUnfoldBayes roounfold_unfolder( test_response.get(),
  //  test_response->Hmeasured(), NUM_DAGOSTINI_ITERATIONS );
  //auto measured = mcc9.get_measured_events();
  //roounfold_unfolder.SetMeasuredCov( *measured.cov_matrix_ );
  //roounfold_unfolder.IncludeSystematics( 1 );

  //auto* roo_hist = roounfold_unfolder.Hreco(
  //  RooUnfold::ErrorTreatment::kCovariance );

  //auto roo_cov = roounfold_unfolder.Ereco(
  //  RooUnfold::ErrorTreatment::kCovariance );

  //for ( int t = 0; t < num_true_signal_bins; ++t ) {
  //  double mine = result.unfolded_signal_->operator()( t, 0 );
  //  double my_err = std::sqrt( std::max(0.,
  //    result.cov_matrix_->operator()( t, t )) );
  //  double theirs = roo_hist->GetBinContent( t + 1 );
  //  double their_err = std::sqrt( std::max(0., roo_cov(t, t)) );

  //  std::cout << "t = " << t << ' ' << mine << " ± " << my_err
  //    << "  " << theirs << " ± " << their_err << "  "
  //    << (mine - theirs) / theirs << " ± "
  //    << (my_err - their_err) / their_err << '\n';
  //}

  //auto smearcept = mcc9.get_cv_smearceptance_matrix();
  //auto true_signal = syst.get_cv_true_signal();

  //// Sanity check (these were equal to each other as expected)
  //// int num_reco_bins = smearcept->GetNrows();
  ////TMatrixD reco_signal( *smearcept, TMatrixD::kMult, *true_signal );
  ////auto data_sig = mcc9.get_cv_ordinary_reco_signal();
  ////for ( int r = 0; r < num_reco_bins; ++r ) {
  ////  double r1 = reco_signal( r, 0 );
  ////  double r2 = data_sig->operator()( r, 0 );
  ////  std::cout << "r = " << r << ", r1 = " << r1 << ", r2 = " << r2
  ////    << ", diff = " << r1 - r2 << '\n';
  ////}

  //auto meas = mcc9.get_measured_events();
  //// ASIMOV TEST: USE CV RECO SIGNAL PREDICTION INSTEAD OF
  //// BACKGROUND-SUBTRACTED DATA
  ////const auto& data_signal = meas.reco_signal_;
  //auto data_signal = mcc9.get_cv_ordinary_reco_signal();
  //const auto& data_covmat = meas.cov_matrix_;

  //auto result = unfolder->unfold( *data_signal, *data_covmat,
  //  *smearcept, *true_signal );

  ////// Test with original implementation
  ////TVectorD true_sig_vec( num_true_bins );
  ////for ( int t = 0; t < num_true_bins; ++t ) {
  ////  true_sig_vec( t ) = true_signal->operator()( t, 0 );
  ////}

  ////TVectorD data_sig_vec( num_ordinary_reco_bins );
  ////for ( int r = 0; r < num_ordinary_reco_bins; ++r ) {
  ////  data_sig_vec( r ) = data_signal->operator()( r, 0 );
  ////}

  ////TMatrixD A_C_svd( num_true_bins, num_true_bins );
  ////TVectorD WF_svd( num_true_bins );
  ////TMatrixD UnfoldCov_svd( num_true_bins, num_true_bins );

  ////TVectorD wsvd_unfolded_signal = WienerSVD( *smearcept, true_sig_vec,
  ////  data_sig_vec, *data_covmat, 0, 0, A_C_svd, WF_svd, UnfoldCov_svd, 0. );

  //for ( int t = 0; t < num_true_bins; ++t ) {
  //  double t_evts = true_signal->operator()( t, 0 );
  //  double evts = result.unfolded_signal_->operator()( t, 0 );
  //  double error = std::sqrt( std::max(0.,
  //    result.cov_matrix_->operator()( t, t )) );
  //  //double evts_svd = wsvd_unfolded_signal( t );
  //  //double error_svd = std::sqrt( std::max(0.,
  //  //  UnfoldCov_svd( t, t )) );
  //  std::cout << "t = " << t << ", t_evts = " << t_evts
  //    << ", evts = " << evts << " ± " << error
  //    << ", diff = " << t_evts - evts << '\n';
  //}

  // Set the event counts in each bin of the histogram that displays the
  // unfolded result. Note that we don't care about the background true bins
  // (which are assumed to follow all of the signal true bins) since we've
  // subtracted out an estimate of the background before unfolding.
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
    }
  }

  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }

  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );

  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );

    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  }

  TLegend* lg = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg->AddEntry( unfolded_events, "unfolded", "l" );
  lg->AddEntry( genie_cv_truth, "uB tune", "l" );
  if ( using_fake_data ) {
    lg->AddEntry( fake_data_truth_hist, "truth", "l" );
  }

  lg->Draw( "same" );

  // Plot slices of the unfolded result
  auto* sb_ptr = new SliceBinning( "../mybins_mcc9_2D_muon_nosidebands.txt");
  auto& sb = *sb_ptr;
  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Get the factors needed to convert to cross-section units
  double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);

  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  // Retrieve the true-space expected event counts from NUISANCE output files
  // for each available generator model
  double conv_factor = ( num_Ar * integ_flux ) / 1e38;
  
  
  std::cout<< " About to make :: generator_truth_map"<< std::endl;
  //auto generator_truth_map = get_true_events_nuisance( sample_info,
    //conv_factor );
  
  auto generator_truth_map = get_true_events_nuisance_v2(SAMPLE_NAME_2DFlat, 1.0);
  
  std::cout<< " Finished"<< std::endl;

  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  //// truth-level prediction TMatrixD objects via multiplication by A_C
  //dump_overall_results( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
  //  genie_cv_truth_vec, fake_data_truth, generator_truth_map,
  //  using_fake_data );

  if ( USE_ADD_SMEAR ) {

    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;

    // Start with the fake data truth if present
    if ( using_fake_data ) {
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
    }

    // Also transform the GENIE CV model
    
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;
 
    // Now do the other generator predictions
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;

      TMatrixD ac_temp(A_C, TMatrixD::kMult, *truth_mat );
      *truth_mat = ac_temp;
    }
  
  
  }



  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);




  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    const auto& slice = sb.slices_.at( sl_idx );

    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;

      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[ "unfolded data" ] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;

 /*
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;

      SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );

      slice_gen_map[ model_name ] = temp_slice;
    }
 */
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
    }

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " (10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar)";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      slice_h->transform( trans_mat );
      slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
    }

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      slice_h->transform( trans_mat );
    }

    // Keys are generator legend labels, values are the result of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == "unfolded data" ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at( "unfolded data" );
      }

      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
    }

    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    slice_unf->hist_->Draw( "e" );
    
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == "unfolded data" || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_file_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 4 );

      slice_h->hist_->Draw( "hist same" );
    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    slice_cv->hist_->Draw( "hist same" );

    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 5 );
      slice_truth->hist_->Draw( "hist same" );
    }

    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
   slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.85 );
    slice_unf->hist_->Draw( "e same" );

    TLegend* lg = new TLegend( 0.15, 0.6, 0.5, 0.88 );
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;
      std::ostringstream oss;
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) oss << 's';

      if ( name != "unfolded data" ) {
        label += ": #chi^{2} = " + oss.str();
      }

      lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
    }

    lg->Draw( "same" );

    // Dump the unfolded results to text files compatible with PGFPlots
    std::map< std::string, std::vector<double> > slice_hist_table;
    std::map< std::string, std::string > slice_params_table;

    dump_slice_variables( sb, sl_idx, slice_params_table );

    for ( const auto& pair : slice_gen_map ) {
      const auto hist_name = samples_to_hist_names.at( pair.first );
      const auto* slice_hist = pair.second;
      bool include_x_coords = ( hist_name == "UnfData" );
      bool include_y_error = include_x_coords;
      dump_slice_histogram( hist_name, *slice_hist, slice, slice_hist_table,
        include_y_error, include_x_coords );
    }

    dump_slice_plot_limits( *slice_unf, *slice_cv, slice, slice_params_table );

    dump_slice_errors_local( "UnfData", slice, sh_cov_map, slice_hist_table );

    // Dump the chi^2 test results
    for ( const auto& chi2_pair : chi2_map ) {
      const auto hist_name = samples_to_hist_names.at( chi2_pair.first );
      const auto& chi2_result = chi2_pair.second;

      // Comparing the data histogram to itself is trivial, so skip it
      if ( hist_name == "UnfData" ) continue;
      else {
        slice_params_table[ hist_name + "_chi2" ]
          = std::to_string( chi2_result.chi2_ );
        slice_params_table[ hist_name + "_pvalue" ]
          = std::to_string( chi2_result.p_value_ );
      }
    }

    // Dump the total data POT and number of bins in the slice
    slice_params_table[ "bnb_data_pot" ] = std::to_string( total_pot );
    slice_params_table[ "num_bins" ] = std::to_string( num_slice_bins );

    // Dump a LaTeX title for the y-axis
    slice_params_table[ "y_axis_title" ] = slice_y_latex_title;

    // Before moving on to the next slice, dump information about the
    // current one to new pgfplots files that can be used for offline plotting
    std::string output_file_prefix = "dump/pgfplots_slice_";
    // Use at least three digits for numbering the slice output files
    if ( sl_idx < 10 ) output_file_prefix += '0';
    if ( sl_idx < 100 ) output_file_prefix += '0';
    output_file_prefix += std::to_string( sl_idx );

    write_pgfplots_files( output_file_prefix, slice_hist_table,
      slice_params_table );


  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);

  } // slices
  
   // sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
    //c1 -> Print(pdf_title);
  
  
  //return;

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "uB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1 -> Print(pdf_title);


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);

}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void IncreaseTitleTH1(TH1& hist, double input) {
    // Increase title size and center it
    gStyle->SetTitleSize(input, "t"); // Adjust the size as needed
    gStyle->SetTitleX(0.5); // Center the title horizontally
}



///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void test_unfolding_ExtraModels() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  auto& fpm = FilePropertiesManager::Instance();
  fpm.load_file_properties( "../file_properties_May10_OpenData.txt" );
  
    
  std::string Pdf_name  = "CrossSection_2D_ExtraModels_FCMuons_WithOpenData";
  std::string DataType_string =  "Unfolded Open Data";
  double POT_input =0;
  char pdf_title[1024];
    TCanvas *c1 = new TCanvas("c1");
    c1->cd(); 
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);
 std::cout<<"Running : Test unfolding () "<< std::endl;

  //const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  //  const std::string respmat_file_name(
  //    "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme1_v1.root" );
  //UnivMake_FakeData_BDTdecided_1D_v10_noBDTproton_pmucorrection.root
  
  //UnivMake_FakeData_BDTdecided_2D_v3_nosidebands_pmucorrection.root
  
  //UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root
  
  //-rw-r--r-- 1 cnguyen microboone  398895883 Mar  9 03:11
  //-rw-r--r-- 1 cnguyen microboone  591920210 Mar  9 03:18 UnivMake_FakeData_2D_inclusive_v2_NoBDTprotron_pmucorrection.root
  //-rw-r--r-- 1 cnguyen microboone  689790553 Mar  9 19:06 UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root


  // Do the systematics calculations in preparation for unfolding
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc_new_removeNuWro.conf" ); //../ systcalc_unfold_fd.conf
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "" );
  auto& syst = *syst_ptr;


 double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);
  POT_input = total_pot;
  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  


  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );

  UnfoldedMeasurement result = unfolder->unfold( syst );

  //auto inv_cov_mat = invert_matrix(*result.cov_matrix_, 1e-4 );


  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );


  auto Ncols_err_prop = err_prop.GetNcols();
  auto Nrows_err_prop = err_prop.GetNrows();
  
  std::cout<< " err_prop_tr:   Ncols = "<< Ncols_err_prop<< " Nrows = "<<Nrows_err_prop << std::endl;


  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;
  std::map< std::string, CovMatrix > matrix_map_error; 



  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    
    auto temp_cov_mat = matrix_pair.second.get_matrix();
     
     auto Ncols_temp = temp_cov_mat->GetNcols();
     auto Nrows_temp = temp_cov_mat->GetNrows();
     
     
     auto Ncols_temp_error = err_prop_tr.GetNcols();
     auto Nrows_temp_error = err_prop_tr.GetNrows();
     
     std::cout<<"matrix_key = "<< matrix_key << " Ncols = "<< Ncols_temp<< " Nrows = "<< Nrows_temp << std::endl;
     std::cout<<"matrix_key(After) = " << " Ncols = "<< Ncols_temp-16<< " Nrows = "<< Nrows_temp-16 << std::endl;
     std::cout<< " Ncols (error) = "<< Ncols_temp_error<< " Nrows (error) = "<< Nrows_temp_error << std::endl;
    
    
    auto mat_temp = RemoveLastNEntries(*temp_cov_mat, 16);
    //temp_cov_mat
    TMatrixD temp_mat( mat_temp , TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
      
      sprintf(pdf_title, "temp_mat:: Matrix Key : %s  ", matrix_key.c_str());
      if(DEBUG_PLOTS==true) visualizeMatrix(temp_mat, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
      
      sprintf(pdf_title, "err_prop::Matrix Key : %s  ", matrix_key.c_str());
          if(DEBUG_PLOTS==true) visualizeMatrix(err_prop, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
     // CovMatrix Matrix = CovMatrix( *unfolded_cov_matrix_map[ matrix_key ] );
      

      
      
  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );
  
  const TMatrixD& NORM_Matrix = bd_ns_covmat.norm_;
  const TMatrixD& Mixed_Matrix = bd_ns_covmat.mixed_;
  const TMatrixD& Shape_Matrix = bd_ns_covmat.shape_;
  
  TMatrixD CombinedNorm( NORM_Matrix, TMatrixD::EMatrixCreatorsOp2::kPlus, Mixed_Matrix);  
  visualizeMatrix(CombinedNorm, "Total Blockewise covariance matrix Norm + Mixed ",  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf","Bin N", "Bin N");
  unfolded_cov_matrix_map[ "total_combined_norm" ] = std::make_unique< TMatrixD >(CombinedNorm);
  
  TMatrixD TOTAL_Cov( CombinedNorm, TMatrixD::EMatrixCreatorsOp2::kPlus, Shape_Matrix);  
  TMatrixD CorrelationMatrix =  covarianceToCorrelation(TOTAL_Cov);
  
  
   sprintf(pdf_title, "Unfolded Total 2D Binning Scheme 1:Covariance Matrix");
   visualizeMatrix(TOTAL_Cov, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf","Bin N", "Bin N");
  
    printMatrixAsLatexTable(TOTAL_Cov,  "TOTAL2D_Covariance_Table.txt");
  
  
  sprintf(pdf_title, "Unfolded Total 2D Binning Scheme 1:Correlation Matrix");
  visualizeMatrix(CorrelationMatrix, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Shape_Matrix =  covarianceToCorrelation(Shape_Matrix);
  sprintf(pdf_title, "Unfolded  Shape Matrix 2D Binning Scheme 1:Correlation Matrix ");
  visualizeMatrix(Correlation_Shape_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Norm_Matrix =  covarianceToCorrelation(NORM_Matrix);
  sprintf(pdf_title, "Unfolded  Norm Matrix 2D Binning Scheme 1:Correlation Matrix ");
  visualizeMatrix(Correlation_Norm_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Mix_Matrix =  covarianceToCorrelation(Mixed_Matrix);
  sprintf(pdf_title, "Unfolded  Mixed Matrix 2D Binning Scheme 1:Correlation Matrix ");
  visualizeMatrix(Correlation_Mix_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "Bin N", "Bin N" , 1,-1);
  
  
  
  ////////////////////////////////////////////////////////////
  // Panel Plot
  //////////////////////////
   GridCanvas *GC_Models = new GridCanvas(uniq(), 3, 4, 800, 550);
 //GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);
   GC_Models->SetBottomMargin(.00);
   GC_Models->SetTopMargin(.02);
   GC_Models->SetRightMargin(.01);
   
    GridCanvas *GC_Models_ERROR = new GridCanvas(uniq(), 3, 4, 800, 550);
 //GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);
   GC_Models_ERROR->SetBottomMargin(.00);
   GC_Models_ERROR->SetTopMargin(.02);
   GC_Models_ERROR->SetRightMargin(.01);  
   
 std::vector<double> WindowZoomScale{5,2,2,2,1,1,1,2,4};


 auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES, "p_{#mu} [GeV/c]");
 auto BinVector = GetProjectBinVector();

 std::string crossSectionYaxis = " "; 
  double openPOT = 1.41645e+20;
    TLegend* lg1_Grid= new TLegend( 0.22, 0.05, 0.96, 0.22 );
    lg1_Grid->SetNColumns(2);
    lg1_Grid->SetBorderSize(0);
    std::string MicroBooneTitle = get_legend_title(openPOT );
     lg1_Grid->SetHeader(MicroBooneTitle.c_str());
     
   TLegend* lg1_Grid_Error = new TLegend( 0.25, 0.05, 0.96, 0.22 );
    lg1_Grid_Error->SetNColumns(3);
    lg1_Grid_Error->SetBorderSize(0);

     //lg1_Grid_Error->SetHeader(MicroBooneTitle.c_str());
     
     
  c1->cd();
  
  
  
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
    }
  }

 if(DEBUG_PLOTS==true) visualizeMatrix(fake_data_truth, "fake_data_truth for CV",  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }
 if(DEBUG_PLOTS==true) visualizeMatrix(genie_cv_truth_vec, "genie_cv_truth_vec for CV",  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );

  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
   
    //visualizeMatrix(*fake_data_truth_hist, "fake_data_truth_hist",  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );



    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  }
  




  TLegend* lg = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg->AddEntry( unfolded_events, DataType_string.c_str(), "l" );
  //TH1D *h_unfolded_events_clone = (TH1D*)unfolded_events->Clone(uniq());
  //lg1_Grid->AddEntry( h_unfolded_events_clone, DataType_string.c_str(), "l" );
  lg->AddEntry( genie_cv_truth, "#muB tune", "l" );
  //TH1D *genie_cv_truth_clone = (TH1D*)genie_cv_truth->Clone(uniq());
  //lg1_Grid->AddEntry( genie_cv_truth_clone, "uB tune", "l" );
  if ( using_fake_data ) {
    lg->AddEntry( fake_data_truth_hist, "truth", "l" );
     // TH1D *fake_data_truth_hist_clone = (TH1D*)fake_data_truth_hist->Clone(uniq());
    //lg1_Grid->AddEntry( fake_data_truth_hist_clone, "truth", "l" );
  }

  lg->Draw( "same" );
  
  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);
 //////////////////////////////////////

  // Plot slices of the unfolded result
  auto* sb_ptr = new SliceBinning( "../mybins_mcc9_2D_muon_v1_july3_2024_noSideBands.txt");
  auto& sb = *sb_ptr;
  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Retrieve the true-space expected event counts from NUISANCE output files
  // for each available generator model
  double conv_factor = ( num_Ar * integ_flux ) / 1e38;

  
  std::cout<< " About to make :: generator_truth_map"<< std::endl;
  
  //auto generator_truth_map = get_true_events_nuisance( truth_CC0Pifile_map, conv_factor2 );
    
    
    //std::map< std::string, TMatrixD* > generator_truth_map =  CreatedModel_BinNMatrix(sb,
    //ModelName_Global,"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/Models_slices.root");
    
    auto generator_truth_map = get_true_events_nuisance_v2(SAMPLE_NAME_2DFlat, 1.0);
    
   //auto generator_truth_map = get_true_events_nuisance_FlatTrees_BinNum(truth_CC0Pifile_map);
    
  //std::map< std::string, TMatrixD* > generator_truth_map = CreatedModel_BinNMatrix(sb,
    //ModelName_Global, "/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/Models_slices.root");
    
 
    
    
    
    
  std::cout<< " Finished"<< std::endl;

  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  //// truth-level prediction TMatrixD objects via multiplication by A_C
  //dump_overall_results( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
  //  genie_cv_truth_vec, fake_data_truth, generator_truth_map,
  //  using_fake_data );

  if ( USE_ADD_SMEAR ) {


      std::map<std::string, double> BinN_lineMap;
    
      
      BinN_lineMap.insert(std::pair<std::string, double>("Slice1", 3 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice2", 10 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice3", 18 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice4", 24 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice5", 31 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice6", 36 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice7", 40 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice8", 43 ));
      BinN_lineMap.insert(std::pair<std::string, double>("Slice9", 45 ));


    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;

     visualizeMatrix(A_C, "Regularization Matrix : A_{C}",  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "TRUE Bin N", "TRUE Regularized Bin N", 1.0, -1.0, BinN_lineMap); // 
    // Start with the fake data truth if present
    if ( using_fake_data ) {
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
    }

    // Also transform the GENIE CV model
    
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;
 
    // Now do the other generator predictions
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<" Inside generator_truth_map :: A_C Tranformation "<< std::endl;
     sprintf(pdf_title, "Model : %s  Generator prediction BEFORE A_{C} Smearing Matrix", model_name.c_str());
     std::cout<< "Tranformationg Model :: "<< pdf_title << std::endl;
      //if(DEBUG_PLOTS==true) 
      
      visualizeMatrix(*truth_mat, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "","",2);


      TMatrixD ac_temp( A_C, TMatrixD::kMult, *truth_mat );
       sprintf(pdf_title, "Model : %s  Generator prediction After A_{C} Smearing Matrix", model_name.c_str());
       //if(DEBUG_PLOTS==true) 
       
      *truth_mat = ac_temp;
      visualizeMatrix(*truth_mat, pdf_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "","",2);
    }
    
  }

 //////////////////////////////////////////////////////////////////////////
 // Ploting  slices 
 //////////////////////////////////////////////////////////////////////////
// Looking the covarience matrix before slicing 

  /*
      for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
        const auto& uc_name = uc_pair.first;
        const auto& uc_matrix = uc_pair.second;
         std::string matrix_title = "Name of Martix: "+  uc_name; 
  
        visualizeMatrix(*uc_matrix, matrix_title,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", "","",2);
  
  }
  */


 for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    if(sl_idx==9) continue; 
     int GridBins = sl_idx + 1; 
    const auto& slice = sb.slices_.at( sl_idx );
    std::cout<<"Making Slice : "<< sl_idx << std::endl;
    // Make a histogram showing the unfolded true event counts in the current
    // slice
    std::map<std::string, TH1D*> RatioHist; 
    
    
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    ///////////////// Maybe this combines the universe ?? 
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;
      std::cout<<"Inside line 2773: uc_name = "<< uc_name<<std::endl;
      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );
      
       if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice, "CrossSection_2D_ExtraModels_FCMuons_WithOpenData","Slice cv","bins",c1,60);

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
        
     if(DEBUG_PLOTS==true) DrawSlice(slice_truth,slice, "CrossSection_2D_ExtraModels_FCMuons_WithOpenData","Slice fake data before unfolding","bins",c1,60);
        
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[DataType_string] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;

     
     
      if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", "CV Slice","bins",c1,60);
      if(DEBUG_PLOTS==true) DrawSlice(slice_unf,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", "unfolded Slice","bins",c1,60);
 
   //auto generator_truth_map = get_true_events_nuisance_FlatTrees(truth_CC0Pifile_map, slice, BinWidth_PmuProjection.at(sl_idx));
 
     

  
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<"Adding " << model_name << " to  slice_gen_map"<< std::endl;
      //SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
      //  *truth_mat, slice, nullptr );
      
       SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );
        std::string modeltitle = model_name + " First generator_truth_map loop  ";
        
        if(DEBUG_PLOTS==true) DrawSlice(temp_slice,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", modeltitle,"bins",c1,60);
        
        if(DEBUG_PLOTS==true) visualizeMatrix(*truth_mat, model_name,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
      

      slice_gen_map[ model_name ] = temp_slice;
    }
 
 
 
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    TMatrixD trans_unit( num_slice_bins, num_slice_bins );
    
    
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
      trans_unit(b, b) = 1.0 / width;
    } 

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "#frac{d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma}{" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += "} [10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar]";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

       if(sl_idx ==0){
          crossSectionYaxis += slice_y_title;
       }
      

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      const auto& name = pair.first;
       
       std::string inputtitle = "Before Tranfermation , Model:" + name;
       
        if(DEBUG_PLOTS==true) DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", inputtitle,"bins",c1,60);
       
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );
           
           
           
           }
           else {
           slice_h->transform( trans_unit );
           }
      
          slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
      
      
        std::string inputtitleout = "After Tranfermation , Model:" + name;
       if(DEBUG_PLOTS==true)  DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", inputtitleout,"bins",c1,60);
      
      
      
    }

  //CovMatrixMap Uncern_CovMap;

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      
      const auto& name = sh_cov_pair.first;
      std::cout<<"Inside Loop for sh_cov_map:: name =  "<< name << std::endl;
      
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string ) {
           slice_h->transform( trans_mat );

           }
      else {
      slice_h->transform( trans_unit );
      }
      
    }
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) { 
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == DataType_string ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at(DataType_string );
      }
       
       std::cout<<"Chi2Result: for Name = "<< name<<std::endl;
      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
      
    }  
    
    
    std::cout<<"Finished slice_gen_map "<< std::endl;
   ///////////////////////////////////////////////////////////
   // Starting to Draw 
   ///////////////////////////////////////////////////////////
    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    c1->cd();
    
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.25, 1, 1.0); //  old 0, 0.2, 1, 1.0
    pad1->SetBottomMargin(.0); // Upper and lower plot are joined
  //pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();    

    TH1D *h_unfoledData_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    TH1D *h_unfoledData_clone_Error = (TH1D*)slice_unf->hist_->Clone(uniq());
    TH1D *h_unfoledData_Ratio = (TH1D*)slice_unf->hist_->Clone(uniq());
    slice_unf->hist_->GetXaxis()->SetTitle("");
    slice_unf->hist_->GetYaxis()->SetTitleSize(0.05);
    slice_unf->hist_->GetYaxis()->SetTitleOffset(.8);
    slice_unf->hist_->Draw( "e" );
    
    
    
    GC_Models->cd(GridBins);

    
    RatioHist.insert(std::pair<std::string, TH1D*>("DATA",h_unfoledData_Ratio)); 
   // slice_unf->hist_->Draw( "e" );
   if(WindowZoomScale.at(sl_idx) != 1){
   
   h_unfoledData_clone->Scale(WindowZoomScale.at(sl_idx));
   
   }
   
   
    h_unfoledData_clone->SetLineWidth( 2 );
    h_unfoledData_clone->SetTitle("");
    h_unfoledData_clone->Draw( "e" );
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_CC0Pifile_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 4 );
      c1->cd();
      pad1->cd(); 
      slice_h->hist_->Draw( "hist same" );
      GC_Models->cd(GridBins);
      TH1D *slice_h_clone = (TH1D*)slice_h->hist_->Clone(uniq());
      TH1D *slice_h_RAIO = (TH1D*)slice_h->hist_->Clone(uniq());
      slice_h_clone->SetLineWidth( 2 );
      
      RatioHist.insert(std::pair<std::string, TH1D*>(name,slice_h_RAIO)); 
      
         if(WindowZoomScale.at(sl_idx) != 1){
         slice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
   
      slice_h_clone->Draw( "hist same" );
    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    c1->cd();
    pad1->cd();
    slice_cv->hist_->Draw( "hist same" );
    
    
    TH1D *CVslice_h_clone = (TH1D*)slice_cv->hist_->Clone(uniq());
    TH1D *CVslice_h_clone2 = (TH1D*)slice_cv->hist_->Clone(uniq());
    
    RatioHist.insert(std::pair<std::string, TH1D*>(MicroBooNEType_string,CVslice_h_clone2)); 
    
    CVslice_h_clone->SetLineWidth( 2 );
        if(WindowZoomScale.at(sl_idx) != 1){
         CVslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
    
    GC_Models->cd(GridBins);
    CVslice_h_clone->Draw( "hist same" );

    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 5 );
      c1->cd();
      pad1->cd(); 
      slice_truth->hist_->Draw( "hist same" );
      GC_Models->cd(GridBins);
      TH1D *truthslice_h_clone = (TH1D*)slice_truth->hist_->Clone(uniq());
      TH1D *truthslice_h_clone2 = (TH1D*)slice_truth->hist_->Clone(uniq());
      
      RatioHist.insert(std::pair<std::string, TH1D*>("Truth",truthslice_h_clone2)); 
      
      truthslice_h_clone->SetLineWidth( 2 );
        if(WindowZoomScale.at(sl_idx) != 1){
         truthslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
      
      truthslice_h_clone->Draw( "hist same" );
    }

        
     std::cout<<"~~~~~~~~~~~~"<< std::endl;
    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*2.5 );
     c1->cd();
     pad1->cd(); 
    slice_unf->hist_->Draw( "e same" );
    TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    GC_Models->cd(GridBins);
     TH1D *unfslice_h_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
     unfslice_h_clone->SetLineWidth( 2 );

      if(WindowZoomScale.at(sl_idx) != 1){
         unfslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
     
    unfslice_h_clone->Draw( "e same" );
    drawString(BinStringMap[BinVector.at(sl_idx)], .038, false );
    
    
  if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Models->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .055,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(62);
			la2->SetTextSize(0.04);
			la2->Draw();
		}
    
    
    
    c1->cd();
    
    char HistName[1024];
    TLegend* lg = new TLegend( 0.15, 0.48, 0.82, 0.89 );
    lg->SetTextFont(132);
    lg->SetBorderSize(0);
  
    
    std::cout<<"Starting to Make Legend"<< std::endl;
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;

      std::ostringstream oss;
      
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss << "";

      if ( name != DataType_string ) {
        label += ": #chi^{2}/ndf = " + oss.str() + " p-value = " + to_string_with_precision(chi2_result.p_value_);
      }
      
      

       lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
         
    }
  ///////////////////////////////////////
  // Drawing Ratio 
  //////////////////////////////////////
    
    
    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.25);
    pad2->SetTopMargin(.0);
    pad2->SetBottomMargin(0.22);
    //pad2->SetLeftMargin(0.1); 
    pad2->SetGridx(); // vertical grid
    //pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();
    TH1D* h_MicroBooNETune = (TH1D*)RatioHist[MicroBooNEType_string]->Clone("MicroBooNETune"); 
     RatioHist["DATA"]->Divide(h_MicroBooNETune);
     RatioHist["DATA"]->GetYaxis()->SetTitle("Ratio to #muBooNE");
     RatioHist["DATA"]->GetXaxis()->SetTitle("cos#theta_{#mu}");
     RatioHist["DATA"]->GetYaxis()->SetLabelSize(.1);
     RatioHist["DATA"]->GetXaxis()->SetLabelSize(.1);
     RatioHist["DATA"]->GetXaxis()->SetTitleSize(0.12);
     RatioHist["DATA"]->GetYaxis()->SetTitleSize(0.1);
     RatioHist["DATA"]->SetTitle("");
    
     //RatioHist["DATA"]->GetXaxis()->CenterTitle();
     RatioHist["DATA"]->GetYaxis()->CenterTitle();
     RatioHist["DATA"]->GetYaxis()->SetTitleOffset(.4);
     RatioHist["DATA"]->GetXaxis()->SetTitleOffset(.75);
     RatioHist["DATA"]->SetMinimum(0);
     RatioHist["DATA"]->SetMaximum(3.5);
     RatioHist["DATA"]->Draw("e");
     
    for(auto h_model:RatioHist ){
       if(h_model.first == "DATA" ) continue;
      h_model.second->Divide(h_MicroBooNETune);
      h_model.second->SetLineWidth( 2 );
      h_model.second->Draw("Same Hist");
    }
    
    
   ///////////////////////////////////////////
   // Draw Norm Uncernity 
   //////////////////////////////////////////
   TH1D* h_normUncern = (TH1D*)slice_unf->hist_->Clone(uniq());
   //h_normUncern->Sumw2();
   auto CovMatrix_NormCombined = unfolded_cov_matrix_map[ "total_combined_norm" ].get(); 


   for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double widthNorm = h_normUncern->GetBinWidth( global_bin_idx );
        widthNorm *= other_var_width;
        Double_t element = (*CovMatrix_NormCombined)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * widthNorm*widthNorm);
        double Uncern_value = sqrt(Var);
        // if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        std::cout<<"Bin : "<< Bin_matrix << " var =  "<< Var << " Uncern_value = "<<Uncern_value<< std::endl;
       // std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
        //h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        //h_normUncern->SetBinError( global_bin_idx, 0. );
      
      
        if( Uncern_value > 0 && std::isnan(Uncern_value)==false){
        h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        h_normUncern->SetBinError( global_bin_idx, 0. );
        }
      else{
           h_normUncern->SetBinContent( global_bin_idx,0);
           h_normUncern->SetBinError( global_bin_idx, 0. );
      }  
      
    }


  h_normUncern->SetFillColor(12);
  h_normUncern->SetLineWidth(0);
  h_normUncern->SetLineColor(0);
  h_normUncern->SetFillStyle(3001);
  TH1D* h_normUncern_clone = (TH1D*)h_normUncern->Clone(uniq());
  lg->AddEntry( h_normUncern, "Norm unc.", "f" );
  c1->cd();
  pad1->cd();
  //h_normUncern->Draw("hist same");
  
  THStack *stack = new THStack("stack", "Stacked Histograms");
  stack->Add(h_normUncern);
  stack->Draw("HISTF same");
  lg->Draw( "same" );


  GC_Models->cd(GridBins); 
  if(WindowZoomScale.at(sl_idx) != 1){
    h_normUncern_clone->Scale(WindowZoomScale.at(sl_idx));
  }
 
  THStack *stack_panel = new THStack("stack_panel", "stack_panel Histograms");
  stack_panel->Add(h_normUncern_clone);
  stack_panel->Draw("HISTF same");
 
 

   c1->cd();
   sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
   c1 -> Print(pdf_title);
  /*
  continue; 
  
    // Dump the unfolded results to text files compatible with PGFPlots
    std::map< std::string, std::vector<double> > slice_hist_table;
    std::map< std::string, std::string > slice_params_table;

    dump_slice_variables( sb, sl_idx, slice_params_table );

    for ( const auto& pair : slice_gen_map ) {
      const auto hist_name = samples_to_hist_names.at( pair.first );
      const auto* slice_hist = pair.second;
      bool include_x_coords = ( hist_name == "UnfData" );
      bool include_y_error = include_x_coords;
      dump_slice_histogram( hist_name, *slice_hist, slice, slice_hist_table,
        include_y_error, include_x_coords );
    }

    dump_slice_plot_limits( *slice_unf, *slice_cv, slice, slice_params_table );

    dump_slice_errors_local( "UnfData", slice, sh_cov_map, slice_hist_table );

    // Dump the chi^2 test results
    for ( const auto& chi2_pair : chi2_map ) {
      const auto hist_name = samples_to_hist_names.at( chi2_pair.first );
      const auto& chi2_result = chi2_pair.second;

      // Comparing the data histogram to itself is trivial, so skip it
      if ( hist_name == "UnfData" ) continue;
      else {
        slice_params_table[ hist_name + "_chi2" ]
          = std::to_string( chi2_result.chi2_ );
        slice_params_table[ hist_name + "_pvalue" ]
          = std::to_string( chi2_result.p_value_ );
      }
    }

    // Dump the total data POT and number of bins in the slice
    slice_params_table[ "bnb_data_pot" ] = std::to_string( total_pot );
    slice_params_table[ "num_bins" ] = std::to_string( num_slice_bins );

    // Dump a LaTeX title for the y-axis
    slice_params_table[ "y_axis_title" ] = slice_y_latex_title;

    // Before moving on to the next slice, dump information about the
    // current one to new pgfplots files that can be used for offline plotting
    std::string output_file_prefix = "dump/pgfplots_slice_";
    // Use at least three digits for numbering the slice output files
    if ( sl_idx < 10 ) output_file_prefix += '0';
    if ( sl_idx < 100 ) output_file_prefix += '0';
    output_file_prefix += std::to_string( sl_idx );

    write_pgfplots_files( output_file_prefix, slice_hist_table,
      slice_params_table );
   */
   ////////////////////////////////////////////////////////////////////////////
   //// Drawing Uncernity 
   ////////////////////////////////////////////////////////////////////////////


    //TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    
     //h_unfolded_slice_clone->SetDirectory( nullptr );

    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.

   const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };


    
    //cov_mat_keys_cross
    //cov_mat_keys_detVar
    //cov_mat_key_totalsumcross
    //cov_mat_keys_detVar
    
    int color = 1;
    for ( auto CovMatrixName:cov_mat_keys  ) {
      
        const auto& key = CovMatrixName;

    std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;

    auto CovMatrix_ = unfolded_cov_matrix_map[ CovMatrixName ].get(); 
    std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;
    std::string Name =  "Covmartirx: " + key;
    //visualizeMatrix(*CovMatrix_, Name ,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf", " Bin N", "Bin N");

    TH1D* h_Error = (TH1D*)h_unfoledData_clone_Error->Clone(uniq());
   

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
         double y = h_unfoledData_clone_Error->GetBinContent( global_bin_idx );
        double width = h_unfoledData_clone_Error->GetBinWidth( global_bin_idx );
        width *= other_var_width;
        Double_t element = (*CovMatrix_)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * width*width);
      
                double frac = 0.;
        if ( y > 0. ) frac = sqrt(Var) / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        
        std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
        if(isnan(frac)) frac = 0;
        
        h_Error->SetBinContent( global_bin_idx, frac );
        h_Error->SetBinError( global_bin_idx, 0. );
      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = h_Error;

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

     if(key == "BNBstats" ||
     key == "EXTstats" ||
     key == "MCstats" ||
     key == "DataStats"){
     frac_uncertainty_hists[ key ]->SetLineStyle(2);}


      frac_uncertainty_hists[ key ]->SetLineColor( color );
      frac_uncertainty_hists[ key ]->SetLineWidth( 3 );
    }
    
    
    
  

   
    TLegend* lg2 = new TLegend( 0.15, 0.7, 0.8, 0.89 );
     lg2->SetNColumns(3);
     auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] ); //h_Error_total; //
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.6 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
     GC_Models_ERROR->cd(GridBins);
    TH1D* total_frac_err_hist_clone = (TH1D*)total_frac_err_hist->Clone(uniq());
    total_frac_err_hist_clone->SetLineWidth( 2 );
    total_frac_err_hist_clone->SetTitle("");
    total_frac_err_hist_clone->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty"); 
     
    lg2->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    if(sl_idx==1) lg1_Grid_Error->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    


    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0] ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
       if(sl_idx==1) lg1_Grid_Error->AddEntry(hist, name.c_str(), "l");
      c1->cd();
      hist->Draw( "same hist" );
      
      GC_Models_ERROR->cd(GridBins);
      TH1D* hist_clone = (TH1D*)hist->Clone(uniq());
      hist_clone->SetLineWidth( 2 );
      hist_clone->Draw( "same hist" );
      
      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }

    c1->cd();
    lg2->Draw( "same" );

    GC_Models_ERROR->cd(GridBins);
    drawString(BinStringMap[BinVector.at(sl_idx)], .03, false );
 
    c1->cd();
    sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
    c1 -> Print(pdf_title);
    
 } ////////////////////////////////////////
 ////////////////////////////////////////
 ////////////////////////////////////////
 // ENd of slices
 ////////////////////////////////////////
  
  
 ///////////////////////////////////////
 // Make Plot for Bin Number 
 //////////////////////////////////// 
  {
    c1->cd();
    TLegend* lg_binN = new TLegend( 0.15, 0.6, 0.8, 0.89 );
    lg_binN->SetBorderSize(0);
    const auto& slice = sb.slices_.at( 9 );
    std::cout<<"Making Slice (should be by Bin Number ) : "<< 10 << std::endl;
    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;
         std::cout<< "line 3416::"<< uc_name << std::endl;
      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );
      
       if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice, "CrossSection_2D_ExtraModels_FCMuons_WithOpenData","Slice cv","bins",c1,60);

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
        
     if(DEBUG_PLOTS==true) DrawSlice(slice_truth,slice, "CrossSection_2D_ExtraModels_FCMuons_WithOpenData","Slice fake data before unfolding","bins",c1,60);
        
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[DataType_string ] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;
     
      if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", "CV Slice","bins",c1,60);
      if(DEBUG_PLOTS==true) DrawSlice(slice_unf,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", "unfolded Slice","bins",c1,60);
 
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<"Adding " << model_name << " to  slice_gen_map"<< std::endl;
      //SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
      //  *truth_mat, slice, nullptr );
      
       SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );
        std::string modeltitle = model_name + " First generator_truth_map loop  ";
        
        if(DEBUG_PLOTS==true) DrawSlice(temp_slice,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", modeltitle,"bins",c1,60);
        
        if(DEBUG_PLOTS==true) visualizeMatrix(*truth_mat, model_name,  "CrossSection_2D_ExtraModels_FCMuons_WithOpenData.pdf");
      

      slice_gen_map[ model_name ] = temp_slice;
    }
 
 
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    TMatrixD trans_unit( num_slice_bins, num_slice_bins );
    
    
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
      trans_unit(b, b) = 1.0 / width;
    } 

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " [10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar]";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";



    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      const auto& name = pair.first;
       
       std::string inputtitle = "Before Tranfermation , Model:" + name;
       
        if(DEBUG_PLOTS==true) DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", inputtitle,"bins",c1,60);
       
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );
           }
           else {
           slice_h->transform( trans_unit );
           }
      
          slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
      
      
        std::string inputtitleout = "After Tranfermation , Model:" + name;
       if(DEBUG_PLOTS==true)  DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_FCMuons_WithOpenData", inputtitleout,"bins",c1,60);
      
    }

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    
    //CovMatrixMap Uncern_CovMap_BinN;
    
    
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      
      const auto& name = sh_cov_pair.first;
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );         
           }
           
      else {
        slice_h->transform( trans_unit );
      }
    }
    
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) { 
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == DataType_string ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at(DataType_string );
      }
       
       std::cout<<"Chi2Result: for Name = "<< name<<std::endl;
      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << 10 << ", Bin N "  << " Chi2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
      
    }  
    
    
    std::cout<<"Finished slice_gen_map "<< std::endl;

    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 2 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    slice_unf->hist_->Draw( "e" );
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_CC0Pifile_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 2 );
      slice_h->hist_->Draw( "hist same" );
    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 2 );
    slice_cv->hist_->SetLineStyle( 5 );
    slice_cv->hist_->Draw( "hist same" );
    
    
    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 2 );
      slice_truth->hist_->Draw( "hist same" );
    }
     std::cout<<"~~~~~~~~~~~~"<< std::endl;
    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.8 );
    slice_unf->hist_->Draw( "e same" );
    
    std::cout<<"Starting to Make Legend"<< std::endl;
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;

      std::ostringstream oss;
      
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_  << "";
      if ( chi2_result.num_bins_ > 1 ) oss << "";

      if ( name != DataType_string ) {
        label += ": #chi^{2}/ndf = " + oss.str() + " p-value = " + to_string_with_precision(chi2_result.p_value_);
      }

       lg_binN->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
       TH1D *slice_h_clone = (TH1D*)slice_h->hist_.get()->Clone(uniq());
       lg1_Grid->AddEntry( slice_h_clone, label.c_str(), "l" );
       
         
    }


    TH1D* h_normUncern = (TH1D*)slice_unf->hist_->Clone(uniq());
   //h_normUncern->Sumw2();
   auto CovMatrix_NormCombined = unfolded_cov_matrix_map[ "total_combined_norm" ].get(); 
   
   for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double widthNorm = h_normUncern->GetBinWidth( global_bin_idx );
        widthNorm *= other_var_width;
        Double_t element = (*CovMatrix_NormCombined)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * widthNorm*widthNorm);
        double Uncern_value = sqrt(Var);
        // if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        std::cout<<"Bin : "<< Bin_matrix << "var =  "<< Var << " Uncern_value = "<<Uncern_value<< std::endl;
       // std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
       // h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        //h_normUncern->SetBinError( global_bin_idx, 0. );
      
      
        if( Uncern_value > 0 && std::isnan(Uncern_value)==false){
        h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        h_normUncern->SetBinError( global_bin_idx, 0. );
        }
      else{
           h_normUncern->SetBinContent( global_bin_idx,0);
           h_normUncern->SetBinError( global_bin_idx, 0. );
      }  
      
    }


   h_normUncern->SetFillColor(12);
   h_normUncern->SetLineWidth(0);
   h_normUncern->SetLineColor(0);
   h_normUncern->SetFillStyle(3001);
   
   TH1D* h_normUncern_clone = (TH1D*)h_normUncern->Clone(uniq());
   lg_binN->AddEntry( h_normUncern, "Norm unc.", "f" );
   lg1_Grid->AddEntry( h_normUncern_clone, "Norm unc.", "f" );
   THStack *stack_binN = new THStack(uniq(), "Stacked Histograms");
   stack_binN->Add(h_normUncern);
   stack_binN->Draw("HISTF same");
 
   lg_binN->Draw( "same" );

  
     sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
     c1 -> Print(pdf_title);
  
   /////////////////////////////////////////////////////////////
   // Fractional Uncerinty by Bin Number
   ////////////////////////////////////////////////////////////
       ////////////////////////////////////////////////////////////////////////////
    //// Drawing Uncernity 
    ////////////////////////////////////////////////////////////////////////////
 
 
     //TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    
     //h_unfolded_slice_clone->SetDirectory( nullptr );

    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };



    int color = 1;
    for ( const auto& pair : matrix_map ) {

      const auto& key = pair.first;
      const auto &cov_matrix = pair.second;

    std::cout<<"Inside: matrix_map_error :: key:: "<< key<< std::endl;


      auto& uc_ptr = sh_cov_map[ key ];
      SliceHistogram* slice_for_syst = uc_ptr.get();


      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
                double frac = 0.;
        if ( y > 0. ) frac = err / y;
        
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< err << " frac = "<< frac<< std::endl;
        

        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

        if(key == "BNBstats" ||
        key == "EXTstats" ||
        key == "MCstats" ||
        key == "DataStats"){slice_for_syst->hist_->SetLineStyle(2);}


      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );
    }


    TLegend* lg4 = new TLegend(  0.15, 0.65, 0.8, 0.89 );
    lg4->SetNColumns(3);

    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.7 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty");  
    lg4->AddEntry( total_frac_err_hist, "total", "l" );

    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg4->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }

    lg4->Draw( "same" );

   c1->cd();
   sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
   c1 -> Print(pdf_title);
 



 }
 ///////////////////////////////////// 
  
  
  

 GC_Models->cd(1);
 lg1_Grid->Draw("same");
 

 
 
 DrawGridCanvas(GC_Models,
 lg1_Grid, "cos#theta_{#mu}", 
 crossSectionYaxis, pdf_title,
 0,100,
 -1., 1. );
  
  
   GC_Models_ERROR->cd(1);
  lg1_Grid_Error->Draw("same");

 DrawGridCanvas(GC_Models_ERROR,
 lg1_Grid_Error, "cos#theta_{#mu}", 
 "Fractional Uncertainity", pdf_title,
 0,1.0,
  -1., 1.);
  
   // sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
    //c1 -> Print(pdf_title);
  
  
  //return;

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "uB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1->cd();
  c1 -> Print(pdf_title);


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);

}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void test_unfolding_ExtraModels_Inclusive() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties_Tuples_5_13_2024.txt" );
 
 fpm.load_file_properties("../file_properties_May10_OpenData.txt" );
  
  
  //  #ifdef USE_FAKE_DATA
  //  // Initialize the FilePropertiesManager and tell it to treat the NuWro
  //  // MC ntuples as if they were data
  //  auto& fpm = FilePropertiesManager::Instance();
  //  std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
  //  
  //  std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
  //  std::cout<<"trying to apply load_file_properties"<< std::endl;
  //  fpm.load_file_properties( "nuwro_file_properties_pmucorrection.txt" );
  //  std::cout<<" passed "<< std::endl;
  //#endif
  
  /////////////////////////
  /// 
  /////////////////////////
      const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };
  
  
    //    const std::vector< std::string > cov_mat_keys = { "total", "xsec_total", 
    //  "MCstats", "EXTstats"
    //};
  

  
  std::string Pdf_name  = "CrossSection_2D_ExtraModels_Inclusive_OPENDATA";
  std::string DataType_string =  "Unfolded OPEN Data"; 
  double POT_input =0;
  char pdf_title[1024];
    TCanvas *c1 = new TCanvas("c1");
    c1->cd(); 
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);
 std::cout<<"Running : Test unfolding () "<< std::endl;

  //const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  //  const std::string respmat_file_name(
  //    "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name_inclusive(
 "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme2.root" );
  //UnivMake_FakeData_BDTdecided_1D_v10_noBDTproton_pmucorrection.root
   // "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_8_16_2024/univmake_FCMuons_scheme2_v5_newBinning.root
  
  // old // /exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_inclusive_v5_pmucorrection.root
  //UnivMake_FakeData_BDTdecided_2D_v3_nosidebands_pmucorrection.root
  
  ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_scheme2_pionSidebandCondition_Uncontained_v1.root
  
  
  //UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root
  
  //-rw-r--r-- 1 cnguyen microboone  398895883 Mar  9 03:11
  //-rw-r--r-- 1 cnguyen microboone  591920210 Mar  9 03:18 UnivMake_FakeData_2D_inclusive_v2_NoBDTprotron_pmucorrection.root
  //-rw-r--r-- 1 cnguyen microboone  689790553 Mar  9 19:06 UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root

  auto* sb_ptr = new SliceBinning( "../mybins_mcc9_2D_muon_inclusive_v1_Oct16_2024_nosidebands.txt");
  auto& sb = *sb_ptr;
  // Do the systematics calculations in preparation for unfolding
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name_inclusive, "../systcalc_unfold_fd.conf" );
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name_inclusive, "../systcalc_new_removeNuWro.conf" );
  auto& syst = *syst_ptr;


  double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);
  POT_input = total_pot;
  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();
std::cout<<"num_true_bins = "<< num_true_bins<< std::endl;
  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );

  UnfoldedMeasurement result = unfolder->unfold( syst );
  //std::unique_ptr< TMatrixD > UnFolded_err_prop_Matrix = result.err_prop_matrix_;
  //auto inv_cov_mat = invert_matrix(*result.cov_matrix_, 1e-4 );

   //TMatrixD* MatrixD_UnFolded_errProp = result.err_prop_matrix_.get();
  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );




  auto Ncols_err_prop = err_prop.GetNcols();
  auto Nrows_err_prop = err_prop.GetNrows();
  
  std::cout<< " err_prop_tr:   Ncols = "<< Ncols_err_prop<< " Nrows = "<<Nrows_err_prop << std::endl;


  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;
  std::map< std::string, CovMatrix > matrix_map_error; 



  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    
    auto temp_cov_mat = matrix_pair.second.get_matrix();
     
     auto Ncols_temp = temp_cov_mat->GetNcols();
     auto Nrows_temp = temp_cov_mat->GetNrows();
     
     
     auto Ncols_temp_error = err_prop_tr.GetNcols();
     auto Nrows_temp_error = err_prop_tr.GetNrows();
     
     std::cout<<"matrix_key = "<< matrix_key << " Ncols = "<< Ncols_temp<< " Nrows = "<< Nrows_temp << std::endl;
     std::cout<< " Ncols (error) = "<< Ncols_temp_error<< " Nrows (error) = "<< Nrows_temp_error << std::endl;
    
    
    auto mat_temp1 = RemoveLastNEntries(*temp_cov_mat, 16);
      auto Ncols_temp_error_after = mat_temp1.GetNcols();
     auto Nrows_temp_error_after = mat_temp1.GetNrows();
    
    //auto error_temp = RemoveLastNEntries(*err_prop_tr, 16);
    //temp_cov_mat
        std::cout<< " Ncols (error)(After) = "<< Ncols_temp_error_after<< " Nrows (error) = "<< Nrows_temp_error_after << std::endl;
    
    TMatrixD temp_mat( mat_temp1, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
      
      sprintf(pdf_title, "temp_mat:: Matrix Key : %s  ", matrix_key.c_str());
      if(DEBUG_PLOTS==true) visualizeMatrix(temp_mat, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");
      
      
      
      sprintf(pdf_title, "err_prop::Matrix Key : %s  ", matrix_key.c_str());
      if(DEBUG_PLOTS==true) visualizeMatrix(err_prop, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");
     // CovMatrix Matrix = CovMatrix( *unfolded_cov_matrix_map[ matrix_key ] );      
  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );

  const TMatrixD& NORM_Matrix = bd_ns_covmat.norm_;
  const TMatrixD& Mixed_Matrix = bd_ns_covmat.mixed_;
  const TMatrixD& Shape_Matrix = bd_ns_covmat.shape_;
  
  TMatrixD CombinedNorm( NORM_Matrix, TMatrixD::EMatrixCreatorsOp2::kPlus, Mixed_Matrix);  
  visualizeMatrix(CombinedNorm, "Total Blockewise covariance matrix Norm + Mixed ",  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf","Bin N", "Bin N");
  unfolded_cov_matrix_map[ "total_combined_norm" ] = std::make_unique< TMatrixD >(CombinedNorm);



 TMatrixD TOTAL_Cov( CombinedNorm, TMatrixD::EMatrixCreatorsOp2::kPlus, Shape_Matrix);  
  TMatrixD CorrelationMatrix =  covarianceToCorrelation(TOTAL_Cov);
  
  
   sprintf(pdf_title, "Unfolded Total 2D Binning Scheme 2:Covariance Matrix");
   visualizeMatrix(TOTAL_Cov, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf","Bin N", "Bin N");
  
  printMatrixAsLatexTable(TOTAL_Cov,  "InclusiveTOTAL_Covariance.txt");
  
  sprintf(pdf_title, "Unfolded Total 2D Binning Scheme 2:Correlation Matrix");
  visualizeMatrix(CorrelationMatrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Shape_Matrix =  covarianceToCorrelation(Shape_Matrix);
  sprintf(pdf_title, "Unfolded  Shape Matrix 2D Binning Scheme 2:Correlation Matrix ");
  visualizeMatrix(Correlation_Shape_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Norm_Matrix =  covarianceToCorrelation(NORM_Matrix);
  sprintf(pdf_title, "Unfolded  Norm Matrix 2D Binning Scheme 2:Correlation Matrix ");
  visualizeMatrix(Correlation_Norm_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Mix_Matrix =  covarianceToCorrelation(Mixed_Matrix);
  sprintf(pdf_title, "Unfolded  Mixed Matrix 2D Binning Scheme 2:Correlation Matrix ");
  visualizeMatrix(Correlation_Mix_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf", "Bin N", "Bin N" , 1,-1);
  

  // Set the event counts in each bin of the histogram that displays the
  // unfolded result. Note that we don't care about the background true bins
  // (which are assumed to follow all of the signal true bins) since we've
  // subtracted out an estimate of the background before unfolding.
  
  ////////////////////////////////////////////////////////////
  // Panel Plot
  //////////////////////////
   GridCanvas *GC_Models = new GridCanvas(uniq(), 3, 4, 800, 550);
  //GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);
   GC_Models->SetBottomMargin(.00);
   GC_Models->SetTopMargin(.02);
   GC_Models->SetRightMargin(.01);
   
   
  GridCanvas *GC_Models_ERROR = new GridCanvas(uniq(), 3, 4, 800, 550);
  //GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);
   GC_Models_ERROR->SetBottomMargin(.00);
   GC_Models_ERROR->SetTopMargin(.02);
   GC_Models_ERROR->SetRightMargin(.01);  
   
   std::vector<double> WindowZoomScale{5,2,2,2,1,1,1,1,1};


  auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES_inclusive, "cos#theta_{#mu}");
  //auto BinVector = GetProjectInclusiveBinVector();
  auto BinVector = GetProjectBinVector();
  std::string crossSectionYaxis = " "; 
  
    TLegend* lg1_Grid= new TLegend( 0.25, 0.05, 0.96, 0.22 );
    lg1_Grid->SetNColumns(2);
    lg1_Grid->SetBorderSize(0);
    double openPOT = 1.41645e+20;
    std::string MicroBooneTitle = get_legend_title(openPOT );
     lg1_Grid->SetHeader(MicroBooneTitle.c_str());
      c1->cd();
  
  
      TLegend* lg1_Grid_Error = new TLegend( 0.25, 0.05, 0.96, 0.22 );
    lg1_Grid_Error->SetNColumns(3);
    lg1_Grid_Error->SetBorderSize(0);
    
     //lg1_Grid_Error->SetHeader(MicroBooneTitle.c_str());

  
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
    }
  }

  if(DEBUG_PLOTS==true) visualizeMatrix(fake_data_truth, "fake_data_truth for CV",  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");
  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }
    if(DEBUG_PLOTS==true) visualizeMatrix(genie_cv_truth_vec, "genie_cv_truth_vec for CV",  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");
  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );

  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
   
    //visualizeMatrix(*fake_data_truth_hist, "fake_data_truth_hist",  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );



    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  }
  

  TLegend* lg1 = new TLegend( 0.15, 0.68, 0.3, 0.86 );
  lg1->AddEntry( unfolded_events, DataType_string.c_str(), "l" );
  //TH1D *h_unfolded_events_clone = (TH1D*)unfolded_events->Clone(uniq());
  //lg1_Grid->AddEntry( h_unfolded_events_clone, DataType_string.c_str(), "l" );
  lg1->AddEntry( genie_cv_truth, "uB tune", "l" );
  //TH1D *genie_cv_truth_clone = (TH1D*)genie_cv_truth->Clone(uniq());
  //lg1_Grid->AddEntry( genie_cv_truth_clone, "uB tune", "l" );
  if ( using_fake_data ) {
    lg1->AddEntry( fake_data_truth_hist, "truth", "l" );
     // TH1D *fake_data_truth_hist_clone = (TH1D*)fake_data_truth_hist->Clone(uniq());
    //lg1_Grid->AddEntry( fake_data_truth_hist_clone, "truth", "l" );
  }

  lg1->Draw( "same" );
  
  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);
    //////////////////////////////////////

  // Plot slices of the unfolded result

  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Get the factors needed to convert to cross-section units

  double conv_factor = ( num_Ar * integ_flux ) / 1e38;

  
  std::cout<< " About to make :: generator_truth_map"<< std::endl;
  
  //auto generator_truth_map = get_true_events_nuisance( truth_CC0Pifile_map, conv_factor2 );
    
    
    //std::map< std::string, TMatrixD* > generator_truth_map =  CreatedModel_BinNMatrix(sb,
    //ModelName_Global,"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/Models_slices.root");
    
    auto generator_truth_map = get_true_events_nuisance_v2(SAMPLE_NAME_2DFlat_BinScheme2, 1.0);
      std::cout<< " Finished"<< std::endl;

  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  //// truth-level prediction TMatrixD objects via multiplication by A_C
  //dump_overall_results( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
  //  genie_cv_truth_vec, fake_data_truth, generator_truth_map,
  //  using_fake_data );

  if ( USE_ADD_SMEAR ) {


  
  std::map<std::string, double> BinN_lineMap_scheme2;

  
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice1", 3 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice2", 8 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice3", 13 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice4", 17 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice5", 21 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice6", 26 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice7", 31 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice8", 35 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice9", 39 ));
  int BinNSlice = 10; 


    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;
//xxyyxx
     visualizeMatrix(A_C, "Regularization Matrix : A_{C}",  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf", "TRUE Bin N", "TRUE Regularized Bin N",1.0, -1.0,BinN_lineMap_scheme2);
    // Start with the fake data truth if present
    if ( using_fake_data ) {
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
    }

    // Also transform the GENIE CV model
    
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;
 
    // Now do the other generator predictions
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
     sprintf(pdf_title, "Model : %s  Generator prediction Before A_{C} Smearing Matrix", model_name.c_str());
      //if(DEBUG_PLOTS==true)
      visualizeMatrix(*truth_mat, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf","","", 2.0);


      TMatrixD ac_temp( A_C, TMatrixD::kMult, *truth_mat );
       sprintf(pdf_title, "Model : %s  Generator prediction After A_{C} Smearing Matrix", model_name.c_str());
       //if(DEBUG_PLOTS==true)
     
      *truth_mat = ac_temp;
        visualizeMatrix(*truth_mat, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf","","",2.0);
    }
  
  
  }

 //////////////////////////////////////////////////////////////////////////
 // Ploting  slices 
 //////////////////////////////////////////////////////////////////////////



  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    if(sl_idx>=9) continue; 
     int GridBins = sl_idx + 1; 
    const auto& slice = sb.slices_.at( sl_idx );
    std::cout<<"Making Slice : "<< sl_idx << std::endl;
    // Make a histogram showing the unfolded true event counts in the current
    // slice
        std::map<std::string, TH1D*> RatioHist; 
    
    
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    ///////////////// Maybe this combines the universe ?? 
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;
      //std::cout<<"Inside line 2773: uc_name = "<< uc_name<<std::endl;
      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );
      
       if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice, "CrossSection_2D_ExtraModels_Inclusive_OPENDATA","Slice cv","bins",c1,60);

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
        
     if(DEBUG_PLOTS==true) DrawSlice(slice_truth,slice, "CrossSection_2D_ExtraModels_Inclusive_OPENDATA","Slice fake data before unfolding","bins",c1,60);
        
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[DataType_string] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;

     
     
      if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", "CV Slice","bins",c1,60);
      if(DEBUG_PLOTS==true) DrawSlice(slice_unf,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", "unfolded Slice","bins",c1,60);
 
   
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<"Adding " << model_name << " to  slice_gen_map"<< std::endl;
      //SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
      //  *truth_mat, slice, nullptr );
      
       SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );
        std::string modeltitle = model_name + " First generator_truth_map loop  ";
        
        if(DEBUG_PLOTS==true) DrawSlice(temp_slice,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", modeltitle,"bins",c1,60);
        
        if(DEBUG_PLOTS==true) visualizeMatrix(*truth_mat, model_name,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");
      

      slice_gen_map[ model_name ] = temp_slice;
    }
 
 
 
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    TMatrixD trans_unit( num_slice_bins, num_slice_bins );
    //TMatrixD trans_Cov( num_slice_bins, num_slice_bins );
    
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
      trans_unit(b, b) = 1.0 / width;
      //trans_Cov(b,b) = (1e38*1e38) / ( width*width * integ_flux*integ_flux*  num_Ar*num_Ar );
    } 

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "#frac{d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma}{" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += "} [10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar]";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

       if(sl_idx ==0){
          crossSectionYaxis += slice_y_title;
       }
      

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      const auto& name = pair.first;
       
       std::string inputtitle = "Before Tranfermation , Model:" + name;
       
        if(DEBUG_PLOTS==true) DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", inputtitle,"bins",c1,60);
       
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );
           
           }
           else {
           slice_h->transform( trans_unit );
           }
      
          slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
      
      
        std::string inputtitleout = "After Tranfermation , Model:" + name;
       if(DEBUG_PLOTS==true)  DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", inputtitleout,"bins",c1,60);
     }

     //CovMatrixMap Uncern_CovMap;
 
     // Also transform all of the unfolded data slice histograms which have
     // specific covariance matrices
     for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      
      const auto& name = sh_cov_pair.first;
      std::cout<<"Inside Loop for sh_cov_map:: name =  "<< name << std::endl;
      
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string ) {
           slice_h->transform( trans_mat );

           }
      else {
      slice_h->transform( trans_unit );
      }
      
    }
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) { 
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == DataType_string ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at(DataType_string );
      }
       
       std::cout<<"Chi2Result: for Name = "<< name<<std::endl;
      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
      
    }  
    
    
    std::cout<<"Finished slice_gen_map "<< std::endl;
   ///////////////////////////////////////////////////////////
   // Starting to Draw 
   ///////////////////////////////////////////////////////////
    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    c1->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.25, 1, 1.0); //  old 0, 0.2, 1, 1.0
    pad1->SetBottomMargin(.0); // Upper and lower plot are joined
    //pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();    
    
     TH1D *h_unfoledData_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    TH1D *h_unfoledData_clone_Error = (TH1D*)slice_unf->hist_->Clone(uniq());
    TH1D *h_unfoledData_Ratio = (TH1D*)slice_unf->hist_->Clone(uniq());
    slice_unf->hist_->GetXaxis()->SetTitle("");
    slice_unf->hist_->GetYaxis()->SetTitleSize(0.05);
    slice_unf->hist_->GetYaxis()->SetTitleOffset(.8);
    slice_unf->hist_->Draw( "e" );
   
       GC_Models->cd(GridBins);
   
      RatioHist.insert(std::pair<std::string, TH1D*>("DATA",h_unfoledData_Ratio)); 
   
   
   
   
   
   if(WindowZoomScale.at(sl_idx) != 1){
      h_unfoledData_clone->Scale(WindowZoomScale.at(sl_idx));
   }
   
   
    h_unfoledData_clone->SetLineWidth( 2 );
    h_unfoledData_clone->SetTitle("");
    h_unfoledData_clone->Draw( "e" );
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_CC0Pifile_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 4 );
      
      c1->cd();
      pad1->cd();
      slice_h->hist_->Draw( "hist same" );
      GC_Models->cd(GridBins);
      
       TH1D *slice_h_clone = (TH1D*)slice_h->hist_->Clone(uniq());
       TH1D *slice_h_RAIO = (TH1D*)slice_h->hist_->Clone(uniq());
      slice_h_clone->SetLineWidth( 2 );
      RatioHist.insert(std::pair<std::string, TH1D*>(name,slice_h_RAIO)); 
      
         if(WindowZoomScale.at(sl_idx) != 1){
         slice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
   
      slice_h_clone->Draw( "hist same" );
    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    c1->cd();
    pad1->cd();
    slice_cv->hist_->Draw( "hist same" );
    
    
    
    TH1D *CVslice_h_clone = (TH1D*)slice_cv->hist_->Clone(uniq());
    TH1D *CVslice_h_clone2 = (TH1D*)slice_cv->hist_->Clone(uniq());
    
    RatioHist.insert(std::pair<std::string, TH1D*>(MicroBooNEType_string,CVslice_h_clone2)); 
    
    CVslice_h_clone->SetLineWidth( 2 );
    
        if(WindowZoomScale.at(sl_idx) != 1){
         CVslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
         
         GC_Models->cd(GridBins);
         CVslice_h_clone->Draw( "hist same" );

    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 5 );
      c1->cd();
       pad1->cd(); 
      slice_truth->hist_->Draw( "hist same" );
      GC_Models->cd(GridBins);
      TH1D *truthslice_h_clone = (TH1D*)slice_truth->hist_->Clone(uniq());
      TH1D *truthslice_h_clone2 = (TH1D*)slice_truth->hist_->Clone(uniq());
      RatioHist.insert(std::pair<std::string, TH1D*>("Truth",truthslice_h_clone2)); 
      truthslice_h_clone->SetLineWidth( 2 );
        if(WindowZoomScale.at(sl_idx) != 1){
         truthslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
      
      truthslice_h_clone->Draw( "hist same" );
    }
     std::cout<<"~~~~~~~~~~~~"<< std::endl;
    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*2.0 );
     c1->cd();
      pad1->cd();
    slice_unf->hist_->Draw( "e same" );
    TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    GC_Models->cd(GridBins);
     TH1D *unfslice_h_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
     unfslice_h_clone->SetLineWidth( 2 );

      if(WindowZoomScale.at(sl_idx) != 1){
         unfslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
     
    unfslice_h_clone->Draw( "e same" );
    drawString(BinStringMap[BinVector.at(sl_idx)], .035, false );

    
   if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Models->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .055,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(62);
			la2->SetTextSize(0.04);
			
			la2->Draw();
		}
    
    
    
    c1->cd();
    
    char HistName[1024];
    
    TLegend* lg = new TLegend( 0.15, 0.55, 0.82, 0.89 );
    lg->SetTextFont(132);
    lg->SetBorderSize(0);
    
    std::cout<<"Starting to Make Legend"<< std::endl;
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;

      std::ostringstream oss;
      
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss << "";


      if ( name != DataType_string ) {
        label += ": #chi^{2}/ndf = " + oss.str() + " p-value = " + to_string_with_precision(chi2_result.p_value_);
      }
      
      

       lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
       

    }
    ///////////////////////////////////////
   // Drawing Ratio 
   //////////////////////////////////////
    
    
    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.25);
    pad2->SetTopMargin(.0);
    pad2->SetBottomMargin(0.22);
    //pad2->SetLeftMargin(0.1); 
    pad2->SetGridx(); // vertical grid
    //pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();
    TH1D* h_MicroBooNETune = (TH1D*)RatioHist[MicroBooNEType_string]->Clone("MicroBooNETune"); 
    
    
     RatioHist["DATA"]->Divide(h_MicroBooNETune);
     RatioHist["DATA"]->GetYaxis()->SetTitle("Ratio to #muBooNE");
     RatioHist["DATA"]->GetXaxis()->SetTitle("p_{#mu} [GeV/c]");
     RatioHist["DATA"]->GetYaxis()->SetLabelSize(.1);
     RatioHist["DATA"]->GetXaxis()->SetLabelSize(.1);
     RatioHist["DATA"]->GetXaxis()->SetTitleSize(0.12);
     RatioHist["DATA"]->GetYaxis()->SetTitleSize(0.1);
     RatioHist["DATA"]->SetTitle("");
    
     //RatioHist["DATA"]->GetXaxis()->CenterTitle();
     RatioHist["DATA"]->GetYaxis()->CenterTitle();
     RatioHist["DATA"]->GetYaxis()->SetTitleOffset(.4);
     RatioHist["DATA"]->GetXaxis()->SetTitleOffset(.75);
     RatioHist["DATA"]->SetMinimum(0);
     RatioHist["DATA"]->SetMaximum(3.5);
     RatioHist["DATA"]->Draw("e");
     
    for(auto h_model:RatioHist ){
       if(h_model.first == "DATA") continue;
      h_model.second->Divide(h_MicroBooNETune);
      h_model.second->SetLineWidth( 2 );
      h_model.second->Draw("Same Hist");
    }
  
    
    ///////////////////////////////////////////
    // Draw Norm Uncernity 
    //////////////////////////////////////////
           TH1D* h_normUncern = (TH1D*)slice_unf->hist_->Clone(uniq());
           //h_normUncern->Sumw2();
          auto CovMatrix_NormCombined = unfolded_cov_matrix_map[ "total_combined_norm" ].get(); 
 

   for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double widthNorm = h_normUncern->GetBinWidth( global_bin_idx );
        widthNorm *= other_var_width;
        Double_t element = (*CovMatrix_NormCombined)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * widthNorm*widthNorm);
        double Uncern_value = sqrt(Var);
        // if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        std::cout<<"Bin : "<< Bin_matrix << "var =  "<< Var << " Uncern_value = "<<Uncern_value<< std::endl;
       // std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
       if( Uncern_value > 0 && std::isnan(Uncern_value)==false){
        h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        h_normUncern->SetBinError( global_bin_idx, 0. );
        }
        
      else{
      h_normUncern->SetBinContent( global_bin_idx,0);
      h_normUncern->SetBinError( global_bin_idx, 0. );}
        
      }


   h_normUncern->SetFillColor(12);
   h_normUncern->SetLineWidth(0);
   h_normUncern->SetLineColor(0);
   h_normUncern->SetFillStyle(3001);
   TH1D* h_normUncern_clone = (TH1D*)h_normUncern->Clone(uniq());
   lg->AddEntry( h_normUncern, "Norm unc.", "f" );
   c1->cd();
   pad1->cd();
   //h_normUncern->Draw("hist same");
   
   THStack *stack = new THStack("stack", "Stacked Histograms");
   stack->Add(h_normUncern);
   stack->Draw("HISTF same");
   lg->Draw( "same" );    
   
   
   
   GC_Models->cd(GridBins); 
   if(WindowZoomScale.at(sl_idx) != 1){
     h_normUncern_clone->Scale(WindowZoomScale.at(sl_idx));
   }
 
  THStack *stack_panel = new THStack("stack_panel", "stack_panel Histograms");
  stack_panel->Add(h_normUncern_clone);
  stack_panel->Draw("HISTF same");
 
   
   
   
   c1->cd();
   sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
   c1 -> Print(pdf_title);
   
   
   
  
   ////////////////////////////////////////////////////////////////////////////
   //// Drawing Uncernity 
   ////////////////////////////////////////////////////////////////////////////


    //TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    
     //h_unfolded_slice_clone->SetDirectory( nullptr );

    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };

  
   //     const std::vector< std::string > cov_mat_keys = { "total", "xsec_total", 
   //   "MCstats", "EXTstats"
   // };


 //////////////////////////////////////////////////////////
 // Plotting Error 
 //////////////////////////////////////////////////////
 
    int color = 1;

  for(auto CovMatrixName: cov_mat_keys){
      const auto& key = CovMatrixName;
      //const auto &cov_matrix = pair.second;
    auto CovMatrix_ = unfolded_cov_matrix_map[ CovMatrixName ].get(); 
    std::cout<<"Inside: or(auto CovMatrixName: cov_mat_keys) loop - Making matrix_map_error :: key::"<< key<< std::endl;
    

    TH1D* h_Error = (TH1D*)h_unfoledData_clone_Error->Clone(uniq());
   
      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        const auto& bin_set = bin_pair.second;
              int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double y = h_unfoledData_clone_Error->GetBinContent( global_bin_idx );
        double width = h_unfoledData_clone_Error->GetBinWidth( global_bin_idx );
        width *= other_var_width;
        //double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        Double_t element = (*CovMatrix_)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * width*width);
                
        //1e38 / ( width * integ_flux * num_Ar );

        
            double frac = 0.;
        if ( y > 0. ) frac = sqrt(Var) / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        
        std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
        h_Error->SetBinContent( global_bin_idx, frac );
        h_Error->SetBinError( global_bin_idx, 0. );

      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = h_Error;

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

     if(key == "BNBstats" ||
     key == "EXTstats" ||
     key == "MCstats" ||
     key == "DataStats"){
     
     frac_uncertainty_hists[ key ]->SetLineStyle(2);}


      frac_uncertainty_hists[ key ]->SetLineColor( color );
      frac_uncertainty_hists[ key ]->SetLineWidth( 3 );
    }


    TLegend* lg2 = new TLegend( 0.15, 0.7, 0.8, 0.89 );
     lg2->SetNColumns(3);


    auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] ); //h_Error_total; //
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.6 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
    GC_Models_ERROR->cd(GridBins);
    TH1D* total_frac_err_hist_clone = (TH1D*)total_frac_err_hist->Clone(uniq());
    total_frac_err_hist_clone->SetLineWidth( 2 );
    total_frac_err_hist_clone->SetTitle("");
    total_frac_err_hist_clone->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty"); 
     
    lg2->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    if(sl_idx==1) lg1_Grid_Error->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    
    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0] ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
     if(sl_idx==1) lg1_Grid_Error->AddEntry(  hist, name.c_str(), "l" );
      c1->cd();
      hist->Draw( "same hist" );
      
      GC_Models_ERROR->cd(GridBins);
      TH1D* hist_clone = (TH1D*)hist->Clone(uniq());
      hist_clone->SetLineWidth( 2 );
      hist_clone->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }
    c1->cd();
    lg2->Draw( "same" );


  GC_Models_ERROR->cd(GridBins);
  drawString(BinStringMap[BinVector.at(sl_idx)], .03, false );
 
  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);



 } ////////////////////////////////////////
 ////////////////////////////////////////
 ////////////////////////////////////////
 // ENd of slices
 ////////////////////////////////////////
  
  
  std::cout<<"Starting to make inclusive Bin N Plot"<< std::endl;
  
  ///////////////////////////////////////
  // Make Plot for Bin Number 
  ////////////////////////////////////

 {
 
  c1->cd();
  TLegend* lg_binN = new TLegend( 0.15, 0.65, 0.8, 0.89 );
  lg_binN->SetNColumns(1);
  const auto& slice = sb.slices_.at( 9 );
    std::cout<<"Making Slice (should be by Bin Number ) : "<< 10 << std::endl;
    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;
         //std::cout<< "line 5192::"<< uc_name << std::endl;
      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );
      
       if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice, "CrossSection_2D_ExtraModels_Inclusive_OPENDATA","Slice cv","bins",c1,60);

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
        
     if(DEBUG_PLOTS==true) DrawSlice(slice_truth,slice, "CrossSection_2D_ExtraModels_Inclusive_OPENDATA","Slice fake data before unfolding","bins",c1,60);
        
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[DataType_string ] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;
     
      if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", "CV Slice","bins",c1,60);
      if(DEBUG_PLOTS==true) DrawSlice(slice_unf,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", "unfolded Slice","bins",c1,60);
 
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<"Adding " << model_name << " to  slice_gen_map"<< std::endl;
      //SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
      //  *truth_mat, slice, nullptr );
      
       SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );
        std::string modeltitle = model_name + " First generator_truth_map loop  ";
        
        if(DEBUG_PLOTS==true) 
        DrawSlice(temp_slice,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", modeltitle,"bins",c1,60);
        
        if(DEBUG_PLOTS==true) 
        visualizeMatrix(*truth_mat, model_name,  "CrossSection_2D_ExtraModels_Inclusive_OPENDATA.pdf");

      slice_gen_map[ model_name ] = temp_slice;
    }
 
 
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    TMatrixD trans_unit( num_slice_bins, num_slice_bins );
    
    
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
      trans_unit(b, b) = 1.0 / width;
    } 

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " [10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar]";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";



    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      const auto& name = pair.first;
       
       std::string inputtitle = "Before Tranfermation , Model:" + name;
       
        if(DEBUG_PLOTS==true) DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", inputtitle,"bins",c1,60);
       
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );
           }
           else {
           slice_h->transform( trans_unit );
           }
      
          slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
      
      
        std::string inputtitleout = "After Tranfermation , Model:" + name;
       if(DEBUG_PLOTS==true)  DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive_OPENDATA", inputtitleout,"bins",c1,60);
      
    }

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    
    //CovMatrixMap Uncern_CovMap_BinN;
    
    
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      
      const auto& name = sh_cov_pair.first;
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string ) {
           slice_h->transform( trans_mat );
           
           if(name == DataType_string){
           /// maybe I can extract the uncenrity on the data here 
           //CovMatrix orig_cov = *slice_h.cmat_;
           //Uncern_CovMap_BinN[name]=orig_cov
           }
           }
           
      else {
      slice_h->transform( trans_unit );
      } 
    }
    
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) { 
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      
      if ( name == DataType_string ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at(DataType_string );
      }
       
      std::cout<<"Chi2Result: for Name = "<< name<<std::endl;
     // // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );
      
//
     // std::cout << "Slice " << 10 << ", Bin N "  << " Chi2 = "
     //   << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
     // if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
     // std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
      
    }  
    
    
    std::cout<<"Finished slice_gen_map "<< std::endl;

    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 2 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax =slice_unf->hist_->GetMaximum();
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_CC0Pifile_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 2 );
      //slice_h->hist_->Draw( "hist same" );
    }
    
    
    
    slice_unf->hist_->SetMaximum(1.85*ymax);
    c1->Modified();
    c1->Update();
    slice_unf->hist_->Draw( "e" );
       for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      slice_h->hist_->Draw( "hist same" );
    }
    
    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 2 );
    slice_cv->hist_->SetLineStyle( 5 );
    slice_cv->hist_->Draw( "hist same" );
    
    
    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 2 );
      slice_truth->hist_->Draw( "hist same" );
    }
     std::cout<<"~~~~~~~~~~~~"<< std::endl;
    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.8 );
    slice_unf->hist_->Draw( "e same" );
    
    std::cout<<"Starting to Make Legend"<< std::endl;
    TH1D* slice_unf_clone = (TH1D*)slice_unf->hist_->Clone(uniq()); 
    
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;

      std::ostringstream oss;
      
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss << "";

      Double_t  chi2;
      Int_t ndf, isgood;
      int ndf_test; 

   //
   //  if ( name != DataType_string ) {
   //   TH1D* model_clone = (TH1D*)slice_h->hist_->Clone(uniq()); 
   //   auto NBins_test = slice_unf_clone->GetNbinsX();
   //   auto NBins_test2 = model_clone->GetNbinsX();
   //   
   //   //double chisqt_local = calculateChiSquare(slice_unf_clone, model_clone, ndf_test);
   //   
   //     double pValue_new = TMath::Prob(chisqt_local, NBins_test);
   //     double pvalue = slice_unf_clone->Chi2TestX(model_clone, chi2, ndf,isgood);
   //     std::cout<<"Nbins1 = "<< NBins_test << " NBins2 = "<< NBins_test2 << std::endl;
   //     std::cout<<'Chisqt isGood = '<< isgood << "ndf = "<< ndf << " ChiSqt = "<< chi2<< " Local Chi ="<<  chisqt_local<< "pvalue = "<< pvalue<< " pvalue_new ="<< pValue_new <<  std::endl;
   //     label += ": #chi^{2}/ndf = " + to_string_with_precision(chisqt_local) +" / "+  std::to_string(NBins_test2) + " p-value = " + to_string_with_precision(pvalue);
   //   }


         
        if ( name != DataType_string ) {
       label += ": #chi^{2}/ndf = " + oss.str() + " p-value = " + to_string_with_precision(chi2_result.p_value_);
     }
      
        lg_binN->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
        TH1D *slice_h_clone = (TH1D*)slice_h->hist_.get()->Clone(uniq());
       lg1_Grid->AddEntry( slice_h_clone, label.c_str(), "l" );
 
         
    }
    
    
    TH1D* h_normUncern = (TH1D*)slice_unf->hist_->Clone(uniq());
   //h_normUncern->Sumw2();
   auto CovMatrix_NormCombined = unfolded_cov_matrix_map[ "total_combined_norm" ].get(); 
    for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double widthNorm = h_normUncern->GetBinWidth( global_bin_idx );
        widthNorm *= other_var_width;
        Double_t element = (*CovMatrix_NormCombined)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * widthNorm*widthNorm);
        double Uncern_value = sqrt(Var);
        // if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        std::cout<<"Bin : "<< Bin_matrix << "var =  "<< Var << " Uncern_value = "<<Uncern_value<< std::endl;
       // std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
       // h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        //h_normUncern->SetBinError( global_bin_idx, 0. );
      
      
        if( Uncern_value > 0 && std::isnan(Uncern_value)==false){
        h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        h_normUncern->SetBinError( global_bin_idx, 0. );
        }
      else{
           h_normUncern->SetBinContent( global_bin_idx,0);
           h_normUncern->SetBinError( global_bin_idx, 0. );
      }  
      
    }


  h_normUncern->SetFillColor(12);
  h_normUncern->SetLineWidth(0);
  h_normUncern->SetLineColor(0);
  h_normUncern->SetFillStyle(3001);
  
  TH1D* h_normUncern_clone = (TH1D*)h_normUncern->Clone(uniq());
   lg_binN->AddEntry( h_normUncern, "Norm unc.", "f" );
     THStack *stack_binN = new THStack(uniq(), "Stacked Histograms");
      stack_binN->Add(h_normUncern);
      stack_binN->Draw("HISTF same");

   
   lg_binN->Draw( "same" );

  
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);
  
  
  std::cout<<"Drawing Fractional Uncerinity "<< std::endl;
  
  /////////////////////////////////////////////////////////////
  // Fractional Uncerinty by Bin Number
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
   //// Drawing Uncernity 
   ////////////////////////////////////////////////////////////////////////////


    //TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    
     //h_unfolded_slice_clone->SetDirectory( nullptr );

    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };




  //   for ( auto& sh_cov_pair : sh_cov_map ) {
  //    auto& slice_h = sh_cov_pair.second;
  //    
  //    const auto& name = sh_cov_pair.first;
  //    std::cout<<"Inside Loop for sh_cov_map:: name =  "<< name << std::endl;
  //    
  //     if ( name == DataType_string ||
  //          name == "truth" || 
  //         name == MicroBooNEType_string ) {
  //         slice_h->transform( trans_mat );
  //         
  //          if(name == DataType_string){
  //         /// maybe I can extract the uncenrity on the data here 
  //          TMatrixD Matrix_temp;
  //          Matrix_temp = slice_h->get_TMatrixD();
  //         
  //         CovMatrix Matrix_temp_input(Matrix_temp);
  //        Uncern_CovMap.insert(namMatrix_temp_input);
  //         
  //         }
  //         
  //         }
  //    else {
  //    slice_h->transform( trans_unit );
  //    }
  //    
  //  }


  //
    int color = 1;
    for ( const auto& pair : matrix_map ) {

      const auto& key = pair.first;
      const auto &cov_matrix = pair.second;

      std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;

      auto& uc_ptr = sh_cov_map[ key ];
      SliceHistogram* slice_for_syst = uc_ptr.get();

    
      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
                double frac = 0.;
        if ( y > 0. ) frac = err / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< err << " frac = "<< frac<< std::endl;
        

        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

        if(key == "BNBstats" ||
        key == "EXTstats" ||
        key == "MCstats" ||
        key == "DataStats"){slice_for_syst->hist_->SetLineStyle(2);}


      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );
    }


    TLegend* lg4 = new TLegend(  0.15, 0.65, 0.8, 0.89 );
    lg4->SetNColumns(3);
    auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] );
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.7 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty");  
    lg4->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );

    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0].c_str() ) continue;

      lg4->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }

    lg4->Draw( "same" );

  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);




 }

 ///////////////////////////////////// 
  
  
 GC_Models->cd(1);
 lg1_Grid->Draw("same");
 
 DrawGridCanvas(GC_Models,
 lg1_Grid, "p_{#mu} [GeV/c]", 
 crossSectionYaxis, pdf_title,
 0,69,
 .1, 2. );
  

  GC_Models_ERROR->cd(1);
  lg1_Grid_Error->Draw("same");

 DrawGridCanvas(GC_Models_ERROR,
 lg1_Grid_Error, "p_{#mu} [GeV/c]", 
 "Fractional Uncertainity", pdf_title,
 0,.8,
 .1, 2.);

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "uB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1->cd();
  c1 -> Print(pdf_title);


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);

}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
int main() {
  //test_unfolding();
  //test_unfoldingTest();
  //test_unfolding_ExtraModels();
  test_unfolding_ExtraModels_Inclusive();
  //test_unfolding_ExtraModels_Inclusive_noData();
  std::string input_directory = "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/";
  //std::string GenietupleName = input_directory + "UnivMake_FakeData_2D_binningscheme2_pmucorrection_v1_nosideband_closure.root";
  
  
  std::map<std::string, double> BinN_lineMap;
  std::map<std::string, double> BinN_lineMap_scheme2;
  
  BinN_lineMap.insert(std::pair<std::string, double>("Slice1", 3 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice2", 10 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice3", 18 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice4", 24 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice5", 31 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice6", 36 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice7", 40 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice8", 43 ));
  BinN_lineMap.insert(std::pair<std::string, double>("Slice9", 45 ));
  int BinNSlice = 10; 
  
  //unfoldingGENIEClosure("/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme1_Closure.root",
  //"../mybins_mcc9_2D_muon_v1_july3_2024_noSideBands.txt", "_scheme1_new3",BinNSlice, BinN_lineMap);
                       
                       
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice1", 3 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice2", 8 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice3", 13 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice4", 17 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice5", 22 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice6", 28 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice7", 33 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice8", 38 ));
  //BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice9", 44 ));
  
   
  
  
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice1", 3 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice2", 8 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice3", 13 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice4", 17 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice5", 21 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice6", 26 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice7", 31 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice8", 35 ));
  BinN_lineMap_scheme2.insert(std::pair<std::string, double>("Slice9", 39 ));
  
                       
                       
    //std::string NuwrotupleName = input_directory + "UnivMake_FakeData_2D_binningscheme2_pmucorrection_v3_Closure.root";
  
  std::string NuwrotupleName = "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme1_nuwroFakedata.root";
  
     //unfolding_NuWroClosure_tests(NuwrotupleName,  "../mybins_mcc9_2D_muon_v1_july3_2024_noSideBands.txt","BinScheme1_NuWro_v5" ,true, 16, BinN_lineMap);
  
                                   //mybins_mcc9_2D_muon_v1_july3_2024_noSideBands.txt                   
    
    
    //std::string NuwrotupleName2 = input_directory + "UnivMake_FakeData_2D_binningscheme1_pmucorrection_v2_Closure.root";
  std::string NuwrotupleName2 = "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme2_nuwroFake.root";
  
    //unfolding_NuWroClosure_tests(NuwrotupleName2,  "../mybins_mcc9_2D_muon_inclusive_v1_Oct16_2024_nosidebands.txt","BinScheme2_NuWro_v6", true, 16,BinN_lineMap_scheme2);
  
  
    //unfoldingGENIEClosure("/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme2_Closure.root",
    // "../mybins_mcc9_2D_muon_inclusive_v1_Oct16_2024_nosidebands.txt", "binning_scheme2_new3",BinNSlice, BinN_lineMap_scheme2);
  
  return 0;
}




TH1D *ConstuctBinN_FromModelSlices(SliceBinning &sb, std::string ModelName, std::string RootPath){
 int NBins = 44; 
 char HistName[1024];
 TH1D* h_binN = new TH1D("h_binN",";Bin N",NBins,1.,NBins+1);
 TFile *TFile_ = new TFile(RootPath.c_str());
std::cout<<"Inside::ConstuctBinN_FromModelSlices"<< std::endl;


  std::cout << "Inside Slice Loop "<< std::endl;
  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {
    
    
    sprintf(HistName, "%s_Slice_%i", ModelName.c_str(),sl_idx);
    TH1D* Hist_slice =  GetTH1DHist(*TFile_, HistName );
    const auto& slice = sb.slices_.at( sl_idx );
    for (auto map1:slice.bin_map_)
    {
     for(auto set:map1.second ){
     std::cout<<"set "<< set << "map1.first "<< map1.first << std::endl;
     h_binN->SetBinContent(set+1, Hist_slice->GetBinContent(map1.first));
     
     
     }
    }
  }// End of Loop 



return h_binN; 
}// End of Function 



std::map<std::string, TH1D*> get_true_events_nuisance_BinNum_vector(
SliceBinning &sb, std::vector<std::string> ModelName_Vector, std::string RootPath){

std::map<std::string, TH1D*> OutPutMap; 

for(auto name:ModelName_Vector ){

TH1D *OutPutHist = ConstuctBinN_FromModelSlices(sb, name, RootPath);
OutPutMap.insert(std::pair<std::string, TH1D*>(name,OutPutHist));
}// End of Loop 


return OutPutMap;

}// End of Function 

///////////////////////////////////////////////////////////////////////////

 std::map< std::string, TMatrixD* >CreatedModel_BinNMatrix(SliceBinning &sb,
 std::vector<std::string> ModelName_Vector, std::string RootPath){
 
 std::cout<<"Inside : CreatedModel_BinNMatrix"<< std::endl;
 
 std::map< std::string, TMatrixD* > truth_counts_map;
 
 
 std::map<std::string, TH1D*> hist_map =  get_true_events_nuisance_BinNum_vector(sb,
                                              ModelName_Vector, RootPath);
 
 for(auto model_slice:hist_map){
 
 int num_bins = model_slice.second->GetNbinsX();
 std::string  generator_label = model_slice.first;
   TMatrixD* temp_mat = new TMatrixD( num_bins, 1 );
    
    for ( size_t b = 0u; b < num_bins; ++b ) {
       std::cout<<"generator_label: "<< generator_label<< "   b =" << b << "temp_hist->GetBinContent( b + 1 ) " << model_slice.second->GetBinContent( b + 1 )<< std::endl; 
      temp_mat->operator()( b, 0 ) = model_slice.second->GetBinContent( b + 1 );
    }
 
 
 std::cout<<"Checking Matrix Nrow = " << temp_mat->GetNrows() << " Ncols = "<< temp_mat->GetNcols()<< std::endl;
 std::string title_input = "temp_mat dditional Inside:CreatedModel_BinNMatrix :: model" + generator_label;
  visualizeMatrix(*temp_mat, title_input,  "CrossSection_2D_ExtraModels.pdf");
   truth_counts_map[ generator_label ] = temp_mat;
 
 
 
 }
 
 
 return truth_counts_map; 
 
 
 
 }// end of function
 /////////////////////////////////////////////////////////
 
void visualizeMatrix(const TMatrixD & matrix, std::string title,  
char *pdfname, std::string XaxisTitle, std::string YaxisTitle ) {
    // Get the number of rows and columns in the matrix
    Int_t numRows = matrix.GetNrows();
    Int_t numCols = matrix.GetNcols();
    char name[1024];
    sprintf(name, "%s" ,title.c_str()); 
    std::cout<<"inside::isualizeMatrix"<< "numRows = "<< numRows<< "numCols = "<< numCols<< std::endl;
    // Create a TH2D histogram to visualize the matrix
    TH2D* hist = new TH2D("hist", "Matrix Visualization", numCols, 0, numCols, numRows, 0, numRows);

    // Fill the histogram with the matrix elements
    for (Int_t i = 0; i < numRows; ++i) {
        for (Int_t j = 0; j < numCols; ++j) {
            hist->SetBinContent(j + 1, i + 1, matrix[i][j]); // SetBinContent takes bin indices starting from 1
        }
    }

    // Create a canvas and draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "Matrix Visualization Canvas");
    
    gStyle->SetOptStat(0);
    //canvas.cd(); 
    hist->GetXaxis()->SetTitle(XaxisTitle.c_str()); 
    hist->GetYaxis()->SetTitle(YaxisTitle .c_str());
      
    hist->SetTitle(name);
    hist->Draw("COLZ"); // COLZ option for a colored 2D plot
    canvas -> Print(pdfname);
    // Run the event loop
    //canvas->Modified();
    //canvas->Update();
    //canvas->Draw(pdfname);

    // Keep the program running to interact with the plot
    canvas->WaitPrimitive();

    // Clean up
    delete hist;
    delete canvas;
}
/////////////////////////////////////////////////////////////////////////////////
 void visualizeMatrix(const TMatrixD & matrix, std::string title,  char *pdfname, std::string XaxisTitle,
 std::string YaxisTitle, double Zmax, double Zmin ) {
    // Get the number of rows and columns in the matrix
    Int_t numRows = matrix.GetNrows();
    Int_t numCols = matrix.GetNcols();
    char name[1024];
    sprintf(name, "%s" ,title.c_str()); 
    std::cout<<"inside::isualizeMatrix"<< "numRows = "<< numRows<< "numCols = "<< numCols<< std::endl;
    // Create a TH2D histogram to visualize the matrix
    TH2D* hist = new TH2D("hist", "Matrix Visualization", numCols, 0, numCols, numRows, 0, numRows);

    // Fill the histogram with the matrix elements
    for (Int_t i = 0; i < numRows; ++i) {
        for (Int_t j = 0; j < numCols; ++j) {
            hist->SetBinContent(j + 1, i + 1, matrix[i][j]); // SetBinContent takes bin indices starting from 1
        }
    }

    // Create a canvas and draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "Matrix Visualization Canvas");
    
    gStyle->SetOptStat(0);
    //canvas.cd(); 
    hist->GetXaxis()->SetTitle(XaxisTitle.c_str()); 
    hist->GetYaxis()->SetTitle(YaxisTitle .c_str());
    hist->SetMaximum(Zmax);
    if(Zmin != 99){hist->SetMinimum(Zmin);}
    hist->SetTitle(name);
    hist->Draw("COLZ"); // COLZ option for a colored 2D plot
    canvas -> Print(pdfname);
    // Run the event loop
    //canvas->Modified();
    //canvas->Update();
    //canvas->Draw(pdfname);

    // Keep the program running to interact with the plot
    canvas->WaitPrimitive();

    // Clean up
    delete hist;
    delete canvas;
}
 ///////////////////////////////////////////////////////////////////////////////
 ////
///////////////////////////////////////////////////////////////////////////////
 void visualizeMatrix(const TMatrixD & matrix, std::string title,  char *pdfname, std::string XaxisTitle,
 std::string YaxisTitle, double Zmax, double Zmin, std::map<std::string, double> lines ) {
    // Get the number of rows and columns in the matrix
    Int_t numRows = matrix.GetNrows();
    Int_t numCols = matrix.GetNcols();
    char name[1024];
    sprintf(name, "%s" ,title.c_str()); 
    std::cout<<"inside::isualizeMatrix"<< "numRows = "<< numRows<< "numCols = "<< numCols<< std::endl;
    // Create a TH2D histogram to visualize the matrix
    TH2D* hist = new TH2D("hist", "Matrix Visualization", numCols, 0, numCols, numRows, 0, numRows);

    // Fill the histogram with the matrix elements
    for (Int_t i = 0; i < numRows; ++i) {
        for (Int_t j = 0; j < numCols; ++j) {
            hist->SetBinContent(j + 1, i + 1, matrix[i][j]); // SetBinContent takes bin indices starting from 1
        }
    }

    // Create a canvas and draw the histogram
    TCanvas* canvas = new TCanvas("canvas", "Matrix Visualization Canvas");
    
    gStyle->SetOptStat(0);
    //canvas.cd(); 
    hist->GetXaxis()->SetTitle(XaxisTitle.c_str()); 
    hist->GetYaxis()->SetTitle(YaxisTitle .c_str());
    hist->SetMaximum(Zmax);
    if(Zmin != 99){hist->SetMinimum(Zmin);}
    hist->SetTitle(name);
    hist->Draw("COLZ"); // COLZ option for a colored 2D plot
    
    double x_min =  hist->GetXaxis()->GetXmin();


    // Create a new TPad over the TH2D
    TPad *pad = new TPad("pad", "Overlay pad", 0, 0, 1, 1);
    pad->SetFillStyle(0); // Make pad transparent
    pad->SetFrameFillStyle(0);
    pad->Draw();
    pad->cd();
  
   // drawing a line on th2d seems to be troublesome trying this method
     Double_t bm = gPad->GetBottomMargin();
     Double_t lm = gPad->GetLeftMargin();
     Double_t rm = gPad->GetRightMargin();
     Double_t tm = gPad->GetTopMargin();


     Double_t xx1 = hist->GetXaxis()->GetXmin();
     Double_t yy1 = hist->GetYaxis()->GetXmin();
     Double_t xx2 = hist->GetXaxis()->GetXmax();
     Double_t yy2 = hist->GetYaxis()->GetXmax();


     pad->Range( xx1-(xx2-xx1)*(lm/(1-rm-lm)),
                  yy1-(yy2-yy1)*(bm/(1-tm-bm)),
                  xx2+(xx2-xx1)*(rm/(1-rm-lm)),
                  yy2+(yy2-yy1)*(tm/(1-tm-bm)));



    gPad->Update();


    
    // Iterate through the map and draw lines and text
    for (const auto &entry : lines) {
        const std::string &text = entry.first;
        double pos = entry.second;

        // Draw vertical line
        TLine *vline = new TLine(pos, 0, pos, pos);
        vline->SetLineColor(kMagenta);  // Set line color to black
        vline->SetLineWidth(2);       // Set line thickness
        vline->Draw("SAME");

        // Draw horizontal line
        TLine *hline = new TLine(0, pos, pos, pos);
        hline->SetLineColor(kMagenta);  // Set line color to black
        hline->SetLineWidth(2);       // Set line thickness
        hline->Draw("SAME");


        // Draw text on the bottom side of the horizontal line
        TText *t_h = new TText(x_min+.1 , pos + 1, text.c_str()); //- 0.05 * (x_max - x_min)
        t_h->SetTextAlign(12); // Left-align the text to the left of the line
        t_h->SetTextSize(0.03); // Adjust text size if needed
        t_h->Draw("SAME");
        t_h->SetTextColor(kMagenta);
    }

    pad->Modified();
    pad->Update();

    canvas->cd();
    canvas->Modified();
    canvas->Update();

    
    canvas -> Print(pdfname);
    // Run the event loop
    //canvas->Modified();
    //canvas->Update();
    //canvas->Draw(pdfname);

    // Keep the program running to interact with the plot
    canvas->Close();

    // Clean up
    delete hist;
    delete canvas;
}


 ///////////////////////////////////////////////////////////////////////////////
 ////
  ///////////////////////////////////////////////////////////////////////////////
 void DrawSlice(
  SliceHistogram* SliceH_Input,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax)
{
  char pdf_title[1024];
  TLegend* lg1_stacked = new TLegend(0.35, 0.65, 0.8, 0.85 );
  lg1_stacked->SetNColumns(2);
  lg1_stacked->SetBorderSize(0);
  Double_t defaultTextSize = lg1_stacked->GetTextSize();
  lg1_stacked->SetTextSize(.02); //
  char leg_title[1024];

  TH1D* h_Data =(TH1D*)SliceH_Input->hist_.get()->Clone("h_Data");
  lg1_stacked->AddEntry(h_Data, "InputSlice", "pe" );
  h_Data->SetMaximum(h_Data->GetMaximum()*1.7); 
  h_Data->Draw("Hist");
    sprintf(pdf_title, "%s.pdf", pdfTitle.c_str());
       lg1_stacked->Draw( "same" );
   c1 -> Print(pdf_title);
  
  auto matrix = SliceH_Input->get_col_vect();
 //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );

  visualizeMatrix(matrix, TotalTitle,  pdf_title);


 


  //  std::cout<<"printing chi values "<< std::endl;

   //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
    //text->DrawLatex(0.15, 0.85, textplace);

    //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    //text->DrawLatex(0.15, 0.80, textplace);
    //AddHistoTitle(TotalTitle.c_str(), .035);
  ////////////////////////////////////////
  ///// Finished with First Plot
  ////////////////////////////////////////

return; 
}
////////////////////////////////////////////////////////////////////  
/// 
////////////////////////////////////////////////////////////////////  
void DrawGridCanvas(GridCanvas *GridCanvas_input,
TLegend* lg_input, std::string XaxisTitle, 
std::string YaxisTitle, std::string pdftitle,
double min_YAxis_GridCanvas, double max_YAxis_GridCanvas,
double min_XAxis_GridCanvas, double max_XAxis_GridCanvas ){
char TitleInput[1024];

 lg_input->Draw("same");
  GridCanvas_input->SetYLabel_Size(.017);
	GridCanvas_input->SetXLabel_Size(.02);
  GridCanvas_input->SetInterpadSpace(.005);
	  //GridCanvas_input->SetRightMargin(0.05);
	  //GridCanvas_input->SetLeftMargin(0.08);
	 //GridCanvas_input->SetBottomMargin(.08);
	 double bottom  = GridCanvas_input->GetBottomMargin();
	 double top  = GridCanvas_input->GetTopMargin();
	 double right  = GridCanvas_input->GetRightMargin();
	 
	 std::cout<<"Bottom margin is set to = "<< bottom<< std::endl;
	 std::cout<<"Top margin is set to = "<< top<< std::endl;
	 std::cout<<"Right margin is set to = "<< right<< std::endl;
  GridCanvas_input->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GridCanvas_input->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
  GridCanvas_input->ResetPads();
   sprintf(TitleInput, "%s", XaxisTitle.c_str());
	GridCanvas_input->SetXTitle(TitleInput);
	GridCanvas_input->SetYTitleSize(18);
	GridCanvas_input->SetXTitleSize(20);  
	sprintf(TitleInput, "%s", YaxisTitle.c_str());
	GridCanvas_input->SetYTitle(TitleInput);
	GridCanvas_input->SetTitleAlignmentFor6Hist();
 GridCanvas_input->Modified();
 

 
  sprintf(TitleInput, "%s", pdftitle.c_str());
 GridCanvas_input->Print(TitleInput);
}
////////////////////////////////////////////////////////////////////  
/// 
//////////////////////////////////////////////////////////////////// 
TMatrixD RemoveLastNEntries(const TMatrixD& matrix, int N) {
    if (N <= 0 || matrix.GetNrows() < N || matrix.GetNcols() < N) {
        std::cerr << "Invalid input: N is non-positive or larger than matrix dimensions." << std::endl;
        return matrix; // Return the original matrix if input is invalid
    }

    int newRows = matrix.GetNrows() - N;
    int newCols = matrix.GetNcols() - N;

    // Create a new matrix with the resized dimensions
    TMatrixD newMatrix(newRows, newCols);

    // Copy the content from the original matrix to the new one
    for (int i = 0; i < newRows; ++i) {
        for (int j = 0; j < newCols; ++j) {
            newMatrix(i, j) = matrix(i, j);
        }
    }

    return newMatrix;
}
////////////////////////////////////////////////////////////////////  
/// 
//////////////////////////////////////////////////////////////////// 

double calculateChiSquare(TH1D* observed, TH1D* expected, int &NDF) {
    // Check if histograms have the same number of bins
    if (observed->GetNbinsX() != expected->GetNbinsX()) {
        std::cerr << "Error: Histograms have different number of bins" << std::endl;
        return -1.0;
    }

    double chiSquare = 0.0;
    Int_t numBins = observed->GetNbinsX();
    NDF = numBins;
    // Loop through each bin and calculate the Chi-square contribution
    for (Int_t i = 1; i <= numBins; ++i) {
        // Calculate observed and expected values for this bin
        double obs = observed->GetBinContent(i);
        double exp = expected->GetBinContent(i);

        // Calculate the Chi-square contribution for this bin
        if (exp > 0.0) { // Avoid division by zero
            chiSquare += pow(obs - exp, 2) / exp;
        }
    }

    return chiSquare;
}
////////////////////////////////////////////////////////////////////  
/// 
//////////////////////////////////////////////////////////////////// 

TMatrixD covarianceToCorrelation(const TMatrixD& covarianceMatrix) {
    // Clone the covariance matrix
    TMatrixD clonedMatrix = TMatrixD(covarianceMatrix); // Using copy constructor to clone
    int n = clonedMatrix.GetNrows();
    TMatrixD correlationMatrix(n, n);

    for (int i = 0; i < n; ++i) {
        double var_i = clonedMatrix(i, i);
        for (int j = 0; j < n; ++j) {
            double cov_ij = clonedMatrix(i, j);
            double var_j = clonedMatrix(j, j);
            double correlation_ij = cov_ij / sqrt(var_i * var_j);
            correlationMatrix(i, j) = correlation_ij;
        }
    }

    return correlationMatrix;
}

///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void printMatrixAsLatexTable(const TMatrixD& matrix, const std::string& fileName) {
    // Open the file for writing
    std::ofstream outputFile(fileName);

    // Check if the file is open
    if (outputFile.is_open()) {
        // Get the dimensions of the matrix
        int numRows = matrix.GetNrows();
        int numCols = matrix.GetNcols();

        // Write the LaTeX table header
        outputFile << "\\begin{tabular}{";
        for (int j = 0; j < numCols; ++j) {
            outputFile << "c";
            if (j < numCols - 1) {
                outputFile << " ";
            }
        }
        outputFile << "}" << std::endl;

        // Write the matrix elements
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                outputFile << matrix(i, j);
                if (j < numCols - 1) {
                    outputFile << " & ";
                } else {
                    outputFile << " \\\\";
                }
            }
            outputFile << std::endl;
        }

        // Write the LaTeX table footer
        outputFile << "\\end{tabular}" << std::endl;

        // Close the file
        outputFile.close();
    } else {
        std::cerr << "Error: Unable to open the file '" << fileName << "'." << std::endl;
    }
}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void unfoldingGENIEClosure(std::string inputFile, 
          std::string inputBinningFile,
          std::string pdf_type, 
          int BinNSlice,
          std::map<std::string, double> BinN_lineMap) {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  //auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties.txt" );
  
  auto& fpm = FilePropertiesManager::Instance();
  fpm.load_file_properties( "../nuwro_file_properties_Tuples_5_13_2024_Closure.txt" );
  
  std::string Pdf_name  = "CrossSection_2D_GENIEClosure_" + pdf_type;
  std::string Name_DATATYPE = "Fake Data (TRUTH)";
  char pdf_title[1024];

 std::cout<<"Running : Test unfolding () "<< std::endl;

  //const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  //  const std::string respmat_file_name(
  //    "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name(inputFile);
  

  // Do the systematics calculations in preparation for unfolding
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc_GENIEClosure.conf" );
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc.conf" );
  auto& syst = *syst_ptr;

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );

  UnfoldedMeasurement result = unfolder->unfold( syst );




  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );

  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;

  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    auto temp_cov_mat = matrix_pair.second.get_matrix();
   //std::cout<<"Nrow = "<< temp_cov_mat.GetNrows()<< std::endl;
     auto mat_temp = RemoveLastNEntries(*temp_cov_mat, 16);

    
    TMatrixD temp_mat( mat_temp, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );


  // Set the event counts in each bin of the histogram that displays the
  // unfolded result. Note that we don't care about the background true bins
  // (which are assumed to follow all of the signal true bins) since we've
  // subtracted out an estimate of the background before unfolding.
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
    }
  }

  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }

  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  
 ///////////////////
 // ONly apply smearing on the Truth Genie
 //////////////////
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );



  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );

    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  
  }

  TLegend* lg = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg->AddEntry( unfolded_events, "unfolded GENIE", "l" );
  lg->AddEntry( genie_cv_truth, "#muB tune", "l" );
  if ( using_fake_data ) {
    lg->AddEntry( fake_data_truth_hist, "Truth", "l" );
  }

  lg->Draw( "same" );

  // Plot slices of the unfolded result
  auto* sb_ptr = new SliceBinning( inputBinningFile);
  auto& sb = *sb_ptr;
  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Get the factors needed to convert to cross-section units
  double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);

  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  // Retrieve the true-space expected event counts from NUISANCE output files
  // for each available generator model
  double conv_factor = ( num_Ar * integ_flux ) / 1e38;
  
  


  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  //// truth-level prediction TMatrixD objects via multiplication by A_C
  //dump_overall_results( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
  //  genie_cv_truth_vec, fake_data_truth, generator_truth_map,
  //  using_fake_data );

  if ( USE_ADD_SMEAR ) {

    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;

    // Start with the fake data truth if present
    if ( using_fake_data){
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
    }

    // Also transform the GENIE CV model
    
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;
   
  }



  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);




  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    const auto& slice = sb.slices_.at( sl_idx );

    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;

      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
       
       //slice_truth = SliceHistogram::make_slice_histogram(
      //genie_cv_truth_vec, slice, nullptr );
       
       
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[ Name_DATATYPE] = slice_unf;
    
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    
    //slice_gen_map[ MicroBooNEType_string ] = slice_cv;

    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
        std::cout<<"Bin "<< b+1 << "  width = "<< width << " other_var_width =  "<< other_var_width << "  width * other_var_width =  "<<  width * other_var_width <<std::endl;
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
    }

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " (10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar)";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      slice_h->transform( trans_mat );
      slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
    }

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      slice_h->transform( trans_mat );
    }

    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == Name_DATATYPE) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at( Name_DATATYPE );
      }

      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
    }
    
    double ymax = -DBL_MAX;
  for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      double max2 = slice_unf->hist_->GetMaximum();
      if ( max > max2 ) ymax = max;
      else{ymax = max2;}

      if ( name == Name_DATATYPE || name == "Truth"
        || name == MicroBooNEType_string ) continue;

    }

    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    IncreaseTitleTH1(*slice_unf->hist_, .06);
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.75 );
    if(BinNSlice==BinNSlice){
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*2.25 );
    }
    
    
    slice_unf->hist_->Draw( "e" );
    
    
    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    //slice_cv->hist_->Draw( "hist" );
    //double area_cv = slice_cv->hist_->Integral() ;
    double area_truth = slice_truth->hist_->Integral() ;
    if ( using_fake_data ) {
      slice_truth->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.65 );
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 5 );
      slice_truth->hist_->Draw( "hist same " );
      //double area_truth = slice_truth->hist_->Integral() ;
    }

    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
   //slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.85 );
    slice_unf->hist_->Draw( "e same" );
   double area_unfolded = slice_unf->hist_->Integral() ;
   double ratio = area_unfolded / area_truth;
    TLegend* lg = new TLegend( 0.2, 0.6, 0.8, 0.89 );
    lg->SetBorderSize(0);
    //lg->
      std::string areaName;
      std::string p_valueName; 
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;
    
      std::ostringstream oss;
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << "/"
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss <<"";

      if ( name != Name_DATATYPE ) {
      
        label += ":#chi^{2}/ndf: " + oss.str();
        areaName =  "#frac{Unfolded GENIE Area}{GENIE Area} = " + to_string_with_precision(ratio,2);
        p_valueName = " p-value: " + to_string_with_precision(chi2_result.p_value_,2);
      }

      lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
     
    }

    lg->AddEntry("", p_valueName.c_str(), ""); // Add a text-only entry
    lg->AddEntry("", areaName.c_str(), ""); // Add a text-only entry
    lg->Draw( "same" );


  if(BinNSlice==BinNSlice){
  
    drawVerticalLinesWithText(BinN_lineMap, 1.75) ;
  
  }





  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);

  TH1D *h_unfoledData_clone_Error = (TH1D*)slice_unf->hist_->Clone(uniq());


 ////////////////////////////////////////
 //
 //////////////////////////////////////
 auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.

    const std::vector< std::string > cov_mat_keys = { "total", "MCstats"};

    
    //cov_mat_keys_cross
    //cov_mat_keys_detVar
    //cov_mat_key_totalsumcross
    //cov_mat_keys_detVar
    
    int color = 1;
    for ( auto CovMatrixName:cov_mat_keys  ) {
      
        const auto& key = CovMatrixName;

    std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;

    auto CovMatrix_ = unfolded_cov_matrix_map[ CovMatrixName ].get(); 
    std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;
    

    TH1D* h_Error = (TH1D*)h_unfoledData_clone_Error->Clone(uniq());
   

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
         double y = h_unfoledData_clone_Error->GetBinContent( global_bin_idx );
        double width = h_unfoledData_clone_Error->GetBinWidth( global_bin_idx );
        width *= other_var_width;
        Double_t element = (*CovMatrix_)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * width*width);
      
                double frac = 0.;
        if ( y > 0. ) frac = sqrt(Var) / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        
        std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
        h_Error->SetBinContent( global_bin_idx, frac );
        h_Error->SetBinError( global_bin_idx, 0. );
      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = h_Error;

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

     if(key == "BNBstats" ||
     key == "EXTstats" ||
     key == "MCstats" ||
     key == "DataStats"){
     frac_uncertainty_hists[ key ]->SetLineStyle(2);}


      frac_uncertainty_hists[ key ]->SetLineColor( color );
      frac_uncertainty_hists[ key ]->SetLineWidth( 3 );
    }
    
    
    
  

   
    TLegend* lg2 = new TLegend( 0.15, 0.7, 0.8, 0.89 );
     lg2->SetNColumns(1);
     auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] ); //h_Error_total; //
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,.3 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
    //GC_Models_ERROR->cd(GridBins);
    TH1D* total_frac_err_hist_clone = (TH1D*)total_frac_err_hist->Clone(uniq());
    total_frac_err_hist_clone->SetLineWidth( 2 );
    total_frac_err_hist_clone->SetTitle("");
    total_frac_err_hist_clone->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty"); 
     
    lg2->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    //if(sl_idx==1) lg1_Grid_Error->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    


    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0] ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
       //if(sl_idx==1) lg1_Grid_Error->AddEntry(hist, name.c_str(), "l");
      c1->cd();
      hist->Draw( "same hist" );
      
      //GC_Models_ERROR->cd(GridBins);
      //TH1D* hist_clone = (TH1D*)hist->Clone(uniq());
      //hist_clone->SetLineWidth( 2 );
      //hist_clone->Draw( "same hist" );
      
      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }

    c1->cd();
    lg2->Draw( "same" );

    //GC_Models_ERROR->cd(GridBins);
    //drawString(BinStringMap[BinVector.at(sl_idx)], .03, false );
 
    c1->cd();
    sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
    c1 -> Print(pdf_title);
    


  } // slices
  
   // sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
    //c1 -> Print(pdf_title);
  
  
  //return;

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "#muB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1 -> Print(pdf_title);


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);

}

///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void unfolding_NuWroClosure_tests(std::string inputFile,
std::string inputBinningFile, std::string pdf_type,
bool removebins, int Removesideband_number, std::map<std::string, double> lines) {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  //auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties.txt" );
  
  auto& fpm = FilePropertiesManager::Instance();
  fpm.load_file_properties( "../nuwro_file_properties.txt" );
  
  std::string Pdf_name  = "CrossSection_2D_NuWro_Closure_" + pdf_type;
  std::string Name_DATATYPE = "NuWro  Unfolded: 2ndDeriv";
  std::string Name_DATATYPE_1stDev = "NuWro Unfolded: 1stDeriv";
  char pdf_title[1024];


  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);



 std::cout<<"Running : Test unfolding () "<< std::endl;

  //const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  //  const std::string respmat_file_name(
  //    "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name(inputFile);
  

  // Do the systematics calculations in preparation for unfolding
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc_unfold_fd.conf" ); //systcalc_NuWroClosure_v2.conf
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc.conf" );
  auto& syst = *syst_ptr;

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder_SVDSecondDeriv (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );


 //  std::unique_ptr< Unfolder > unfolder_DAGOSTINI_ITERATIONS (
 //    new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
 //    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
 //      //::FigureOfMerit, 0.025 )
 //
 //  );
 //
 //  std::unique_ptr< Unfolder > unfolder_DAGOSTINI_ConvergenceCriterion (
 //
 //    new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion::FigureOfMerit, 0.025 )
 //
 //  );
 //
  std::unique_ptr< Unfolder > unfolder_SVDFirstDeriv (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kFirstDeriv )
  );



  UnfoldedMeasurement result = unfolder_SVDSecondDeriv->unfold( syst );
  UnfoldedMeasurement result_1st_derv = unfolder_SVDFirstDeriv->unfold( syst );
  //UnfoldedMeasurement result_SVDFirstDeriv = unfolder_SVDFirstDeriv->Unfolder( syst );
  //UnfoldedMeasurement result_DagoIterations = unfolder_DAGOSTINI_ITERATIONS->Unfolder( syst );
  //UnfoldedMeasurement result_DagoCriterion = unfolder_DAGOSTINI_ConvergenceCriterion->Unfolder( syst );


  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );

  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;

  //std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map2;
  
  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    auto temp_cov_mat = matrix_pair.second.get_matrix();

     //
    //temp_cov_mat
    if(removebins==true) {
    
    auto mat_temp = RemoveLastNEntries(*temp_cov_mat, Removesideband_number);
    
        
    TMatrixD temp_mat( mat_temp, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
    
    
    }
    else {
    
        TMatrixD temp_mat( *temp_cov_mat, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
    
    
    
    }

    

  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );


  //NormShapeCovMatrix bd_ns_covmat2 = make_block_diagonal_norm_shape_covmat(
  //  *result_1st_derv.unfolded_signal_, *result_1st_derv.cov_matrix_, syst.true_bins_ );
  //
    //
    //  // Add the blockwise decomposed matrices into the map
    //unfolded_cov_matrix_map2[ "total_blockwise_norm" ]
    //  = std::make_unique< TMatrixD >( bd_ns_covmat2.norm_ );
  //
    //unfolded_cov_matrix_map2[ "total_blockwise_shape" ]
    //  = std::make_unique< TMatrixD >( bd_ns_covmat2.shape_ );
  //
    //unfolded_cov_matrix_map2[ "total_blockwise_mixed" ]
    //  = std::make_unique< TMatrixD >( bd_ns_covmat2.mixed_ );
  
  
  
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  TMatrixD fake_data_truth_1stDev( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
      fake_data_truth_1stDev( b, 0 ) = true_evts;
    }
  }

  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }

  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  
 ///////////////////
 // ONly apply smearing on the Truth Genie
 //////////////////
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );



  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );

    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  
  }

  TLegend* lg = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg->AddEntry( unfolded_events, "unfolded GENIE", "l" );
  lg->AddEntry( genie_cv_truth, "#muB tune", "l" );
  if ( using_fake_data ) {
    lg->AddEntry( fake_data_truth_hist, "truth", "l" );
  }

  lg->Draw( "same" );

  // Plot slices of the unfolded result
  auto* sb_ptr = new SliceBinning( inputBinningFile);
  auto& sb = *sb_ptr;
  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Get the factors needed to convert to cross-section units
  double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);

  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  // Retrieve the true-space expected event counts from NUISANCE output files
  // for each available generator model
  double conv_factor = ( num_Ar * integ_flux ) / 1e38;
  
  


  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  //// truth-level prediction TMatrixD objects via multiplication by A_C
  //dump_overall_results( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
  //  genie_cv_truth_vec, fake_data_truth, generator_truth_map,
  //  using_fake_data );

  if ( USE_ADD_SMEAR ) {

    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;
    sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
    visualizeMatrix(A_C, "Regularization Matrix: kSecondDeriv : A_{C}",  pdf_title, "TRUE Bin N", "TRUE Regularized Bin N",1.0, -1.0,lines);

    const TMatrixD& A_C_1stDev = *result_1st_derv.add_smear_matrix_;

    visualizeMatrix(A_C_1stDev, "Regularization Matrix: kFirstDeriv : A_{C}",  pdf_title, "TRUE Bin N", "TRUE Regularized Bin N",1.0,-1.0,lines);


    // Start with the fake data truth if present
    if ( using_fake_data ) {
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      TMatrixD ac_truth_1st( A_C_1stDev, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
      fake_data_truth_1stDev = ac_truth_1st;
      
    }

    // Also transform the GENIE CV model
    
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;
 
    // Now do the other generator predictions
    /*
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;

      TMatrixD ac_temp(A_C, TMatrixD::kMult, *truth_mat );
      *truth_mat = ac_temp;
    }
  */
  
  }


  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    const auto& slice = sb.slices_.at( sl_idx );

    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    SliceHistogram* slice_unf_1stDev = SliceHistogram::make_slice_histogram(
      *result_1st_derv.unfolded_signal_, slice, result_1st_derv.cov_matrix_.get() );



    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;

      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    SliceHistogram* slice_truth_1stDev = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
      
       slice_truth_1stDev = SliceHistogram::make_slice_histogram( fake_data_truth_1stDev,
        slice, nullptr );
        
        
    }


    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    


   slice_unf_1stDev->hist_->SetLineColor( kBlack );
   slice_unf_1stDev->hist_->SetLineWidth( 3 );
   slice_unf_1stDev->hist_->SetMarkerStyle(  kOpenTriangleDown );
   slice_unf_1stDev->hist_->SetMarkerSize( 0.8 );
   


    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

   auto* slice_gen_map2_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map2 = *slice_gen_map2_ptr;


    slice_gen_map[Name_DATATYPE] = slice_unf;
    slice_gen_map2[Name_DATATYPE_1stDev] = slice_unf_1stDev;
    
    
    if ( using_fake_data ) {
      slice_gen_map[ "1st Deriv" ] = slice_truth_1stDev;
      slice_gen_map2[ "2nd Deriv" ] = slice_truth;
      
    }
    
    //slice_gen_map[ MicroBooNEType_string ] = slice_cv;





    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
    }

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " (10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar)";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      slice_h->transform( trans_mat );
      slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
    }

   for ( auto& pair : slice_gen_map2 ) {
      auto* slice_h = pair.second;
      slice_h->transform( trans_mat );
      slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
    }


    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      slice_h->transform( trans_mat );
    }
    
     /*  for ( auto& sh_cov_pair : sh_cov_map2 ) {
      auto& slice_h = sh_cov_pair.second;
      slice_h->transform( trans_mat );
    }
   */
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == Name_DATATYPE) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at( Name_DATATYPE );
      }

      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
    }
   ///////////////////////////////////
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map2;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map2 ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == Name_DATATYPE_1stDev) {
        chi2_map2[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map2.at( Name_DATATYPE_1stDev );
      }

      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map2[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
    }


   slice_unf->hist_->SetStats( false );
   slice_unf_1stDev->hist_->SetStats( false );


    //TCanvas* c1 = new TCanvas;




    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    
      const char* Error_title = slice_unf->hist_->GetTitle();
    slice_unf->hist_->Draw( "e" );
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      double max2 = slice_unf->hist_->GetMaximum();
      if ( max > max2 ) ymax = max;
      else{ymax = max2;}

      if ( name == Name_DATATYPE || name == "1st Deriv"
        || name == MicroBooNEType_string || name == Name_DATATYPE_1stDev  ) continue;

    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    //slice_cv->hist_->Draw( "hist same" );

    if ( using_fake_data ) {
      slice_truth->hist_->GetYaxis()->SetRangeUser( 0., ymax*2 );
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 2 );
      slice_truth->hist_->Draw( "hist" );
      
      slice_truth_1stDev->hist_->SetLineColor( kGreen );
      slice_truth_1stDev->hist_->SetLineStyle(2);
      slice_truth_1stDev->hist_->SetLineWidth( 2 );
      slice_truth_1stDev->hist_->SetStats( false );
      slice_truth_1stDev->hist_->Draw( "hist SAME" );
      
    }

    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
   //slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.85 );
    slice_unf->hist_->Draw( "e same" );
    slice_unf_1stDev->hist_->Draw( "e same" );

    TLegend* lg = new TLegend( 0.39, 0.58, 0.88, 0.88 );
    //lg->SetBorderSize(0); 
    //lg->SetTextFont(132);
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;
      std::ostringstream oss;
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 2 ) << chi2_result.chi2_ << "/"
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss <<"";

      if ( name != Name_DATATYPE ) {
        label += ": #chi^{2}/ndf: " + oss.str() + " p-value:" + to_string_with_precision(chi2_result.p_value_,2);
        lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
      }
    else {
    
    slice_h->hist_->SetLineColor( kBlack );
    slice_h->hist_->SetLineWidth( 2 );
    slice_h->hist_->SetMarkerStyle( kFullCircle );
    lg->AddEntry( slice_h->hist_.get(), label.c_str(), "p" );
    
    }

      
    }


  for ( const auto& pair : slice_gen_map2 ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;
      std::ostringstream oss;
      const auto& chi2_result = chi2_map2.at( name );
      oss << std::setprecision( 2 ) << chi2_result.chi2_ << "/"
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss <<"";

      if ( name != Name_DATATYPE_1stDev ) {
        label += ": #chi^{2}/ndf: " + oss.str() + " p-value:" + to_string_with_precision(chi2_result.p_value_,2);
      lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
      }
    else{
        slice_h->hist_->SetLineColor( kBlack );
    slice_h->hist_->SetLineWidth( 2 );
    slice_h->hist_->SetMarkerStyle(  kOpenTriangleDown );
    lg->AddEntry( slice_h->hist_.get(), label.c_str(), "p" );
    }


      
    }


    lg->Draw( "same" );

  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);

  TH1D *h_unfoledData_clone_Error = (TH1D*)slice_unf->hist_->Clone(uniq());


 ////////////////////////////////////////
 //
 //////////////////////////////////////
 auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.

    const std::vector< std::string > cov_mat_keys = { "total", "MCstats", "xsec_multi", "xsec_unisim", /*" xsec_xsr_scc_Fa3_SCC",
                                                       "xsec_xsr_scc_Fv3_SCC",*/ }; // "NuWroGenie"

    
    
    //cov_mat_keys_cross
    //cov_mat_keys_detVar
    //cov_mat_key_totalsumcross
    //cov_mat_keys_detVar
    
    int color = 1;
    for ( auto CovMatrixName:cov_mat_keys  ) {
      
        const auto& key = CovMatrixName;

    std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;

    auto CovMatrix_ = unfolded_cov_matrix_map[ CovMatrixName ].get(); 
    std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;
    

    TH1D* h_Error = (TH1D*)h_unfoledData_clone_Error->Clone(uniq());
   

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
         double y = h_unfoledData_clone_Error->GetBinContent( global_bin_idx );
        double width = h_unfoledData_clone_Error->GetBinWidth( global_bin_idx );
        width *= other_var_width;
        Double_t element = (*CovMatrix_)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * width*width);
      
                double frac = 0.;
        if ( y > 0. ) frac = sqrt(Var) / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        
        std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
        h_Error->SetBinContent( global_bin_idx, frac );
        h_Error->SetBinError( global_bin_idx, 0. );
      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = h_Error;

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

     if(key == "BNBstats" ||
     key == "EXTstats" ||
     key == "MCstats" ||
     key == "DataStats"){
     frac_uncertainty_hists[ key ]->SetLineStyle(2);}


      frac_uncertainty_hists[ key ]->SetLineColor( color );
      frac_uncertainty_hists[ key ]->SetLineWidth( 3 );
    }
    
    
    
  

   
    TLegend* lg2 = new TLegend( 0.3, 0.68, 0.8, 0.88);
     lg2->SetNColumns(2);
     //lg2->SetBorderSize(0); 
     auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] ); //h_Error_total; //
     
    double maxx = total_frac_err_hist->GetMaximum();
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,maxx*1.4);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->SetTitle(Error_title); 
    total_frac_err_hist->Draw( "hist" );
    //GC_Models_ERROR->cd(GridBins);
    

    
    TH1D* total_frac_err_hist_clone = (TH1D*)total_frac_err_hist->Clone(uniq());
    total_frac_err_hist_clone->SetLineWidth( 2 );
    total_frac_err_hist_clone->SetTitle(Error_title);
    IncreaseTitleTH1(*total_frac_err_hist_clone, .06);
    total_frac_err_hist_clone->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty"); 
     
    lg2->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    //if(sl_idx==1) lg1_Grid_Error->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    


    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0] ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
       //if(sl_idx==1) lg1_Grid_Error->AddEntry(hist, name.c_str(), "l");
      c1->cd();
      hist->Draw( "same hist" );
      
      //GC_Models_ERROR->cd(GridBins);
      //TH1D* hist_clone = (TH1D*)hist->Clone(uniq());
      //hist_clone->SetLineWidth( 2 );
      //hist_clone->Draw( "same hist" );
      
      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }

    c1->cd();
    lg2->Draw( "same" );

    //GC_Models_ERROR->cd(GridBins);
    //drawString(BinStringMap[BinVector.at(sl_idx)], .03, false );
 
    c1->cd();
    sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
    c1 -> Print(pdf_title);
    


  } // slices
  
   // sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
    //c1 -> Print(pdf_title);
  
  
  //return;

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "#muB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1 -> Print(pdf_title);


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);

}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void test_unfolding_ExtraModels_Inclusive_noData() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  auto& fpm = FilePropertiesManager::Instance();
  fpm.load_file_properties( "../nuwro_file_properties_Tuples_5_13_2024.txt" );
  
  
  
  //  #ifdef USE_FAKE_DATA
  //  // Initialize the FilePropertiesManager and tell it to treat the NuWro
  //  // MC ntuples as if they were data
  //  auto& fpm = FilePropertiesManager::Instance();
  //  std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
  //  
  //  std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
  //  std::cout<<"trying to apply load_file_properties"<< std::endl;
  //  fpm.load_file_properties( "nuwro_file_properties_pmucorrection.txt" );
  //  std::cout<<" passed "<< std::endl;
  //#endif
  
  /////////////////////////
  /// 
  /////////////////////////
      const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };
  
  
    //    const std::vector< std::string > cov_mat_keys = { "total", "xsec_total", 
    //  "MCstats", "EXTstats"
    //};
  

  
  std::string Pdf_name  = "CrossSection_2D_ExtraModels_Inclusive_noDataPointsONBinN";
  std::string DataType_string =  "Unfolded FAKE Data"; 
  double POT_input =0;
  char pdf_title[1024];
    TCanvas *c1 = new TCanvas("c1");
    c1->cd(); 
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);
 std::cout<<"Running : Test unfolding () "<< std::endl;

  //const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  //  const std::string respmat_file_name(
  //    "/uboone/data/users/cnguyen/CC0Pi_Selection/unfolding/23-sept10-all-universes.root" );
  //
  
  const std::string respmat_file_name_inclusive(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_FakeData_2D_binningscheme2_pmucorrection_v2.root" );
  //UnivMake_FakeData_BDTdecided_1D_v10_noBDTproton_pmucorrection.root
  // old // /exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_inclusive_v5_pmucorrection.root
  //UnivMake_FakeData_BDTdecided_2D_v3_nosidebands_pmucorrection.root
  
  //UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root
  
  //-rw-r--r-- 1 cnguyen microboone  398895883 Mar  9 03:11
  //-rw-r--r-- 1 cnguyen microboone  591920210 Mar  9 03:18 UnivMake_FakeData_2D_inclusive_v2_NoBDTprotron_pmucorrection.root
  //-rw-r--r-- 1 cnguyen microboone  689790553 Mar  9 19:06 UnivMake_FakeData_2D_v2_NoBDTproton_pmucorrection.root

  auto* sb_ptr = new SliceBinning( "../mybins_mcc9_2D_muon_inclusive_newbinning9_newtuples_noSideBands.txt");
  auto& sb = *sb_ptr;
  // Do the systematics calculations in preparation for unfolding
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name_inclusive, "../systcalc_unfold_fd.conf" );
  auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name_inclusive, "../systcalc_new.conf" );
  auto& syst = *syst_ptr;


  double total_pot = syst.total_bnb_data_pot_;
  double integ_flux = integrated_numu_flux_in_FV( total_pot );
  double num_Ar = num_Ar_targets_in_FV(Fiducial_Volumn_CC0Pi);
  POT_input = total_pot;
  std::cout << "INTEGRATED numu FLUX = " << integ_flux << '\n';
  std::cout << "NUM Ar atoms in fiducial volume = " << num_Ar << '\n';

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  // While we're at it, clone the histogram and zero it out. We'll fill this
  // one with our unfolded result for easy comparison
  TH1D* unfolded_events = dynamic_cast< TH1D* >(
    genie_cv_truth->Clone("unfolded_events") );
  unfolded_events->Reset();

  // If present, then get the fake data event counts in each true bin
  // (including the background true bins). We hope to approximately reproduce
  // these event counts in the signal true bins via unfolding the fake data.
  const auto& fake_data_univ = syst.fake_data_universe();
  TH1D* fake_data_truth_hist = nullptr;

  bool using_fake_data = false;
  if ( fake_data_univ ) {
    using_fake_data = true;
    fake_data_truth_hist = fake_data_univ->hist_true_.get();
  }

  int num_ordinary_reco_bins = 0;
  int num_sideband_reco_bins = 0;
  for ( int b = 0; b < syst.reco_bins_.size(); ++b ) {
    const auto& rbin = syst.reco_bins_.at( b );
    if ( rbin.type_ == kSidebandRecoBin ) ++num_sideband_reco_bins;
    else ++num_ordinary_reco_bins;
  }

  int num_true_signal_bins = 0;
  for ( int t = 0; t < syst.true_bins_.size(); ++t ) {
    const auto& tbin = syst.true_bins_.at( t );
    if ( tbin.type_ == kSignalTrueBin ) ++num_true_signal_bins;
  }

  std::cout << "NUM ORDINARY RECO BINS = " << num_ordinary_reco_bins << '\n';
  std::cout << "NUM TRUE SIGNAL BINS = " << num_true_signal_bins << '\n';

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* cov_mat = matrix_map.at( "total" ).cov_matrix_.get();

  constexpr int NUM_DAGOSTINI_ITERATIONS = 2;
  constexpr bool USE_ADD_SMEAR = true;

  std::unique_ptr< Unfolder > unfolder (
    //new DAgostiniUnfolder( NUM_DAGOSTINI_ITERATIONS )
    //new DAgostiniUnfolder( DAgostiniUnfolder::ConvergenceCriterion
      //::FigureOfMerit, 0.025 )
    new WienerSVDUnfolder( true,
      WienerSVDUnfolder::RegularizationMatrixType::kSecondDeriv )
  );

  UnfoldedMeasurement result = unfolder->unfold( syst );
  //std::unique_ptr< TMatrixD > UnFolded_err_prop_Matrix = result.err_prop_matrix_;
  //auto inv_cov_mat = invert_matrix(*result.cov_matrix_, 1e-4 );

   //TMatrixD* MatrixD_UnFolded_errProp = result.err_prop_matrix_.get();
  // For real data only, add some new covariance matrices in which only the
  // signal response or the background is varied. We could calculate these
  // for the fake data, but it seems unnecessary at this point.
  if ( !using_fake_data ) {
    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlyBackground );
    auto* bkgd_matrix_map_ptr = syst.get_covariances().release();
    auto& bkgd_matrix_map = *bkgd_matrix_map_ptr;

    syst.set_syst_mode( MCC9SystematicsCalculator
      ::SystMode::VaryOnlySignalResponse );
    auto* sigresp_matrix_map_ptr = syst.get_covariances().release();
    auto& sigresp_matrix_map = *sigresp_matrix_map_ptr;

    for ( const auto& m_pair : bkgd_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "bkgd_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }

    for ( const auto& m_pair : sigresp_matrix_map ) {
      auto& my_temp_cov_mat = matrix_map[ "sigresp_only_" + m_pair.first ];
      my_temp_cov_mat += m_pair.second;
    }
  }

  // Propagate all defined covariance matrices through the unfolding procedure
  const TMatrixD& err_prop = *result.err_prop_matrix_;
  TMatrixD err_prop_tr( TMatrixD::kTransposed, err_prop );


  auto Ncols_err_prop = err_prop.GetNcols();
  auto Nrows_err_prop = err_prop.GetNrows();
  
  std::cout<< " err_prop_tr:   Ncols = "<< Ncols_err_prop<< " Nrows = "<<Nrows_err_prop << std::endl;


  std::map< std::string, std::unique_ptr<TMatrixD> > unfolded_cov_matrix_map;
  std::map< std::string, CovMatrix > matrix_map_error; 



  for ( const auto& matrix_pair : matrix_map ) {
    const std::string& matrix_key = matrix_pair.first;
    
    auto temp_cov_mat = matrix_pair.second.get_matrix();
     
     auto Ncols_temp = temp_cov_mat->GetNcols();
     auto Nrows_temp = temp_cov_mat->GetNrows();
     
     
     auto Ncols_temp_error = err_prop_tr.GetNcols();
     auto Nrows_temp_error = err_prop_tr.GetNrows();
     
     std::cout<<"matrix_key = "<< matrix_key << " Ncols = "<< Ncols_temp<< " Nrows = "<< Nrows_temp << std::endl;
     std::cout<< " Ncols (error) = "<< Ncols_temp_error<< " Nrows (error) = "<< Nrows_temp_error << std::endl;
    
    //std::cout<<"Nrow = "<< temp_cov_mat.GetNrows()<< std::endl;
    auto mat_temp = RemoveLastNEntries(*temp_cov_mat, 16);
    //auto error_temp = RemoveLastNEntries(*err_prop_tr, 16);
    //temp_cov_mat
    TMatrixD temp_mat( mat_temp, TMatrixD::EMatrixCreatorsOp2::kMult,
      err_prop_tr );

    unfolded_cov_matrix_map[ matrix_key ] = std::make_unique< TMatrixD >(
      err_prop, TMatrixD::EMatrixCreatorsOp2::kMult, temp_mat );
      
      sprintf(pdf_title, "temp_mat:: Matrix Key : %s  ", matrix_key.c_str());
      if(DEBUG_PLOTS==true) visualizeMatrix(temp_mat, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf");
      
      
      
      sprintf(pdf_title, "err_prop::Matrix Key : %s  ", matrix_key.c_str());
      if(DEBUG_PLOTS==true) visualizeMatrix(err_prop, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf");
     // CovMatrix Matrix = CovMatrix( *unfolded_cov_matrix_map[ matrix_key ] );      
  }

  // Decompose the block-diagonal pieces of the total covariance matrix
  // into normalization, shape, and mixed components (for later plotting
  // purposes)
  NormShapeCovMatrix bd_ns_covmat = make_block_diagonal_norm_shape_covmat(
    *result.unfolded_signal_, *result.cov_matrix_, syst.true_bins_ );

  // Add the blockwise decomposed matrices into the map
  unfolded_cov_matrix_map[ "total_blockwise_norm" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.norm_ );

  unfolded_cov_matrix_map[ "total_blockwise_shape" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.shape_ );

  unfolded_cov_matrix_map[ "total_blockwise_mixed" ]
    = std::make_unique< TMatrixD >( bd_ns_covmat.mixed_ );

  const TMatrixD& NORM_Matrix = bd_ns_covmat.norm_;
  const TMatrixD& Mixed_Matrix = bd_ns_covmat.mixed_;
  const TMatrixD& Shape_Matrix = bd_ns_covmat.shape_;
  
  TMatrixD CombinedNorm( NORM_Matrix, TMatrixD::EMatrixCreatorsOp2::kPlus, Mixed_Matrix);  
  visualizeMatrix(CombinedNorm, "Total Blockewise covariance matrix Norm + Mixed ",  "CrossSection_2D_ExtraModels_Inclusive.pdf","Bin N", "Bin N");
  unfolded_cov_matrix_map[ "total_combined_norm" ] = std::make_unique< TMatrixD >(CombinedNorm);



 TMatrixD TOTAL_Cov( CombinedNorm, TMatrixD::EMatrixCreatorsOp2::kPlus, Shape_Matrix);  
  TMatrixD CorrelationMatrix =  covarianceToCorrelation(TOTAL_Cov);
  
  
   sprintf(pdf_title, "Unfolded Total 2D Binning Scheme 2:Covariance Matrix");
   visualizeMatrix(TOTAL_Cov, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf","Bin N", "Bin N");
  
  printMatrixAsLatexTable(TOTAL_Cov,  "InclusiveTOTAL_Covariance.txt");
  
  sprintf(pdf_title, "Unfolded Total 2D Binning Scheme 2:Correlation Matrix");
  visualizeMatrix(CorrelationMatrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Shape_Matrix =  covarianceToCorrelation(Shape_Matrix);
  sprintf(pdf_title, "Unfolded  Shape Matrix 2D Binning Scheme 2:Correlation Matrix ");
  visualizeMatrix(Correlation_Shape_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Norm_Matrix =  covarianceToCorrelation(NORM_Matrix);
  sprintf(pdf_title, "Unfolded  Norm Matrix 2D Binning Scheme 2:Correlation Matrix ");
  visualizeMatrix(Correlation_Norm_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf", "Bin N", "Bin N" , 1,-1);
  
  TMatrixD Correlation_Mix_Matrix =  covarianceToCorrelation(Mixed_Matrix);
  sprintf(pdf_title, "Unfolded  Mixed Matrix 2D Binning Scheme 2:Correlation Matrix ");
  visualizeMatrix(Correlation_Mix_Matrix, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf", "Bin N", "Bin N" , 1,-1);
  

  // Set the event counts in each bin of the histogram that displays the
  // unfolded result. Note that we don't care about the background true bins
  // (which are assumed to follow all of the signal true bins) since we've
  // subtracted out an estimate of the background before unfolding.
  
  ////////////////////////////////////////////////////////////
  // Panel Plot
  //////////////////////////
   GridCanvas *GC_Models = new GridCanvas(uniq(), 3, 4, 800, 550);
  //GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);
   GC_Models->SetBottomMargin(.00);
   GC_Models->SetTopMargin(.02);
   GC_Models->SetRightMargin(.01);
   
   
  GridCanvas *GC_Models_ERROR = new GridCanvas(uniq(), 3, 4, 800, 550);
  //GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);
   GC_Models_ERROR->SetBottomMargin(.00);
   GC_Models_ERROR->SetTopMargin(.02);
   GC_Models_ERROR->SetRightMargin(.01);  
   
   std::vector<double> WindowZoomScale{5,4,4,2,1,1,1,1,1};


  auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES_inclusive, "cos#theta_{#mu}");
  //auto BinVector = GetProjectInclusiveBinVector();
  auto BinVector = GetProjectBinVector();
  std::string crossSectionYaxis = " "; 
  
    TLegend* lg1_Grid= new TLegend( 0.25, 0.05, 0.96, 0.22 );
    lg1_Grid->SetNColumns(2);
    lg1_Grid->SetBorderSize(0);
    std::string MicroBooneTitle = get_legend_title(DATA_POT );
     lg1_Grid->SetHeader(MicroBooneTitle.c_str());
      c1->cd();
  
  
      TLegend* lg1_Grid_Error = new TLegend( 0.25, 0.05, 0.96, 0.22 );
    lg1_Grid_Error->SetNColumns(3);
    lg1_Grid_Error->SetBorderSize(0);
    
     //lg1_Grid_Error->SetHeader(MicroBooneTitle.c_str());

  
  
  for ( int t = 0; t < num_true_bins; ++t ) {
    double evts = 0.;
    double error = 0.;
    if ( t < num_true_signal_bins ) {
      evts = result.unfolded_signal_->operator()( t, 0 );
      error = std::sqrt( std::max(0., result.cov_matrix_->operator()( t, t )) );
    }

    // We need to use one-based indices while working with TH1D bins
    unfolded_events->SetBinContent( t + 1, evts );
    unfolded_events->SetBinError( t + 1, error );
  }

  unfolded_events->SetStats( false );
  unfolded_events->SetLineColor( kBlack );
  unfolded_events->SetLineWidth( 3 );
  unfolded_events->GetXaxis()->SetRangeUser( 0, num_true_signal_bins );

  // Save the fake data truth (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD fake_data_truth( num_true_signal_bins, 1 );
  if ( using_fake_data ) {
    for ( int b = 0; b < num_true_signal_bins; ++b ) {
      double true_evts = fake_data_truth_hist->GetBinContent( b + 1 );
      fake_data_truth( b, 0 ) = true_evts;
    }
  }

  if(DEBUG_PLOTS==true) visualizeMatrix(fake_data_truth, "fake_data_truth for CV",  "CrossSection_2D_ExtraModels_Inclusive.pdf");
  // Save the GENIE CV model (before A_C multiplication) using a column vector
  // of event counts
  TMatrixD genie_cv_truth_vec( num_true_signal_bins, 1 );
  for ( int b = 0; b < num_true_signal_bins; ++b ) {
    double true_evts = genie_cv_truth->GetBinContent( b + 1 );
    genie_cv_truth_vec( b, 0 ) = true_evts;
  }
    if(DEBUG_PLOTS==true) visualizeMatrix(genie_cv_truth_vec, "genie_cv_truth_vec for CV",  "CrossSection_2D_ExtraModels_Inclusive.pdf");
  // Multiply the truth-level GENIE prediction histogram by the additional
  // smearing matrix
  TMatrixD* A_C = result.add_smear_matrix_.get();
  multiply_1d_hist_by_matrix( A_C, genie_cv_truth );

  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  unfolded_events->Draw( "e" );
  genie_cv_truth->Draw( "hist same" );

  if ( using_fake_data ) {

    // Multiply the fake data truth histogram by the additional smearing matrix
   
    //visualizeMatrix(*fake_data_truth_hist, "fake_data_truth_hist",  "CrossSection_2D_ExtraModels_Inclusive.pdf");
    multiply_1d_hist_by_matrix( A_C, fake_data_truth_hist );



    fake_data_truth_hist->SetStats( false );
    fake_data_truth_hist->SetLineColor( kBlue );
    fake_data_truth_hist->SetLineWidth( 3 );
    fake_data_truth_hist->SetLineStyle( 2 );
    fake_data_truth_hist->Draw( "hist same" );
  }
  

  TLegend* lg1 = new TLegend( 0.15, 0.68, 0.3, 0.86 );
  lg1->AddEntry( unfolded_events, DataType_string.c_str(), "l" );
  //TH1D *h_unfolded_events_clone = (TH1D*)unfolded_events->Clone(uniq());
  //lg1_Grid->AddEntry( h_unfolded_events_clone, DataType_string.c_str(), "l" );
  lg1->AddEntry( genie_cv_truth, "MicroBooNE Tune", "l" );
  //TH1D *genie_cv_truth_clone = (TH1D*)genie_cv_truth->Clone(uniq());
  //lg1_Grid->AddEntry( genie_cv_truth_clone, "uB tune", "l" );
  if ( using_fake_data ) {
    lg1->AddEntry( fake_data_truth_hist, "truth", "l" );
     // TH1D *fake_data_truth_hist_clone = (TH1D*)fake_data_truth_hist->Clone(uniq());
    //lg1_Grid->AddEntry( fake_data_truth_hist_clone, "truth", "l" );
  }

  lg1->Draw( "same" );
  
  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);
    //////////////////////////////////////

  // Plot slices of the unfolded result

  //myconfig_mcc8_CC0pi_1D_NoBDTproton_new.txt"
  //
  // Get the factors needed to convert to cross-section units

  double conv_factor = ( num_Ar * integ_flux ) / 1e38;

  
  std::cout<< " About to make :: generator_truth_map"<< std::endl;
  
  //auto generator_truth_map = get_true_events_nuisance( truth_CC0Pifile_map, conv_factor2 );
    
    
    //std::map< std::string, TMatrixD* > generator_truth_map =  CreatedModel_BinNMatrix(sb,
    //ModelName_Global,"/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/Models_slices.root");
    
    auto generator_truth_map = get_true_events_nuisance_v2(SAMPLE_NAME_2DFlat_BinScheme2, 1.0);
      std::cout<< " Finished"<< std::endl;

  // Dump overall results to text files. Total cross section units (10^{-38}
  // cm^2 / Ar) will be used throughout. Do this before adjusting the
  //// truth-level prediction TMatrixD objects via multiplication by A_C
  //dump_overall_results( result, unfolded_cov_matrix_map, 1.0 / conv_factor,
  //  genie_cv_truth_vec, fake_data_truth, generator_truth_map,
  //  using_fake_data );

  if ( USE_ADD_SMEAR ) {

    // Get access to the additional smearing matrix
    const TMatrixD& A_C = *result.add_smear_matrix_;

     visualizeMatrix(A_C, "Regularization Matrix : A_{C}",  "CrossSection_2D_ExtraModels_Inclusive.pdf", "TRUE Bin N", "TRUE Regularized Bin N");
    // Start with the fake data truth if present
    if ( using_fake_data ) {
      TMatrixD ac_truth( A_C, TMatrixD::kMult, fake_data_truth );
      fake_data_truth = ac_truth;
    }

    // Also transform the GENIE CV model
    
    TMatrixD genie_cv_temp( A_C, TMatrixD::kMult, genie_cv_truth_vec );
    genie_cv_truth_vec = genie_cv_temp;
 
    // Now do the other generator predictions
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
     sprintf(pdf_title, "Model : %s  Generator prediction Before A_{C} Smearing Matrix", model_name.c_str());
      //if(DEBUG_PLOTS==true)
      visualizeMatrix(*truth_mat, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf","","", 2.0);


      TMatrixD ac_temp( A_C, TMatrixD::kMult, *truth_mat );
       sprintf(pdf_title, "Model : %s  Generator prediction After A_{C} Smearing Matrix", model_name.c_str());
       //if(DEBUG_PLOTS==true)
     
      *truth_mat = ac_temp;
        visualizeMatrix(*truth_mat, pdf_title,  "CrossSection_2D_ExtraModels_Inclusive.pdf","","",2.0);
    }
  
  
  }

 //////////////////////////////////////////////////////////////////////////
 // Ploting  slices 
 //////////////////////////////////////////////////////////////////////////



  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {

    if(sl_idx==9) continue; 
     int GridBins = sl_idx + 1; 
    const auto& slice = sb.slices_.at( sl_idx );
    std::cout<<"Making Slice : "<< sl_idx << std::endl;
    // Make a histogram showing the unfolded true event counts in the current
    // slice
        std::map<std::string, TH1D*> RatioHist; 
    
    
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    ///////////////// Maybe this combines the universe ?? 
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;
      std::cout<<"Inside line 2773: uc_name = "<< uc_name<<std::endl;
      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );
      
       if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice, "CrossSection_2D_ExtraModels_Inclusive","Slice cv","bins",c1,60);

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
        
     if(DEBUG_PLOTS==true) DrawSlice(slice_truth,slice, "CrossSection_2D_ExtraModels_Inclusive","Slice fake data before unfolding","bins",c1,60);
        
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[DataType_string] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;

     
     
      if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice,"CrossSection_2D_ExtraModels_Inclusive", "CV Slice","bins",c1,60);
      if(DEBUG_PLOTS==true) DrawSlice(slice_unf,slice,"CrossSection_2D_ExtraModels_Inclusive", "unfolded Slice","bins",c1,60);
 
   
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<"Adding " << model_name << " to  slice_gen_map"<< std::endl;
      //SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
      //  *truth_mat, slice, nullptr );
      
       SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );
        std::string modeltitle = model_name + " First generator_truth_map loop  ";
        
        if(DEBUG_PLOTS==true) DrawSlice(temp_slice,slice,"CrossSection_2D_ExtraModels_Inclusive", modeltitle,"bins",c1,60);
        
        if(DEBUG_PLOTS==true) visualizeMatrix(*truth_mat, model_name,  "CrossSection_2D_ExtraModels_Inclusive.pdf");
      

      slice_gen_map[ model_name ] = temp_slice;
    }
 
 
 
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    TMatrixD trans_unit( num_slice_bins, num_slice_bins );
    //TMatrixD trans_Cov( num_slice_bins, num_slice_bins );
    
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
      trans_unit(b, b) = 1.0 / width;
      //trans_Cov(b,b) = (1e38*1e38) / ( width*width * integ_flux*integ_flux*  num_Ar*num_Ar );
    } 

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "#frac{d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma}{" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += "} [10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar]";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";

       if(sl_idx ==0){
          crossSectionYaxis += slice_y_title;
       }
      

    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      const auto& name = pair.first;
       
       std::string inputtitle = "Before Tranfermation , Model:" + name;
       
        if(DEBUG_PLOTS==true) DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive", inputtitle,"bins",c1,60);
       
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );
           
           }
           else {
           slice_h->transform( trans_unit );
           }
      
          slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
      
      
        std::string inputtitleout = "After Tranfermation , Model:" + name;
       if(DEBUG_PLOTS==true)  DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive", inputtitleout,"bins",c1,60);
     }

     //CovMatrixMap Uncern_CovMap;
 
     // Also transform all of the unfolded data slice histograms which have
     // specific covariance matrices
     for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      
      const auto& name = sh_cov_pair.first;
      std::cout<<"Inside Loop for sh_cov_map:: name =  "<< name << std::endl;
      
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string ) {
           slice_h->transform( trans_mat );

           }
      else {
      slice_h->transform( trans_unit );
      }
      
    }
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    for ( const auto& pair : slice_gen_map ) { 
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      if ( name == DataType_string ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at(DataType_string );
      }
       
       std::cout<<"Chi2Result: for Name = "<< name<<std::endl;
      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << sl_idx << ", " << name << ": \u03C7\u00b2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
      
    }  
    
    
    std::cout<<"Finished slice_gen_map "<< std::endl;
   ///////////////////////////////////////////////////////////
   // Starting to Draw 
   ///////////////////////////////////////////////////////////
    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 3 );
    slice_unf->hist_->SetMarkerStyle( kFullCircle );
    slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    c1->cd();
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.25, 1, 1.0); //  old 0, 0.2, 1, 1.0
    pad1->SetBottomMargin(.0); // Upper and lower plot are joined
  //pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();    
    
  TH1D *h_unfoledData_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    TH1D *h_unfoledData_clone_Error = (TH1D*)slice_unf->hist_->Clone(uniq());
    TH1D *h_unfoledData_Ratio = (TH1D*)slice_unf->hist_->Clone(uniq());
    slice_unf->hist_->GetXaxis()->SetTitle("");
    slice_unf->hist_->GetYaxis()->SetTitleSize(0.05);
    slice_unf->hist_->GetYaxis()->SetTitleOffset(.8);
    slice_unf->hist_->Draw( "e" );
   
       GC_Models->cd(GridBins);
   
      RatioHist.insert(std::pair<std::string, TH1D*>("DATA",h_unfoledData_Ratio)); 
   
   
   
   
   
   if(WindowZoomScale.at(sl_idx) != 1){
      h_unfoledData_clone->Scale(WindowZoomScale.at(sl_idx));
   }
   
   
    h_unfoledData_clone->SetLineWidth( 2 );
    h_unfoledData_clone->SetTitle("");
    h_unfoledData_clone->Draw( "e" );
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_CC0Pifile_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 4 );
      
      c1->cd();
      pad1->cd();
      slice_h->hist_->Draw( "hist same" );
      GC_Models->cd(GridBins);
      
       TH1D *slice_h_clone = (TH1D*)slice_h->hist_->Clone(uniq());
       TH1D *slice_h_RAIO = (TH1D*)slice_h->hist_->Clone(uniq());
      slice_h_clone->SetLineWidth( 2 );
      RatioHist.insert(std::pair<std::string, TH1D*>(name,slice_h_RAIO)); 
      
         if(WindowZoomScale.at(sl_idx) != 1){
         slice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
   
      slice_h_clone->Draw( "hist same" );
    }

    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 5 );
    slice_cv->hist_->SetLineStyle( 5 );
    c1->cd();
    pad1->cd();
    slice_cv->hist_->Draw( "hist same" );
    
    
    
    TH1D *CVslice_h_clone = (TH1D*)slice_cv->hist_->Clone(uniq());
    TH1D *CVslice_h_clone2 = (TH1D*)slice_cv->hist_->Clone(uniq());
    
    RatioHist.insert(std::pair<std::string, TH1D*>(MicroBooNEType_string,CVslice_h_clone2)); 
    
    CVslice_h_clone->SetLineWidth( 2 );
    
        if(WindowZoomScale.at(sl_idx) != 1){
         CVslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
         
         GC_Models->cd(GridBins);
         CVslice_h_clone->Draw( "hist same" );

    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 5 );
      c1->cd();
       pad1->cd(); 
      slice_truth->hist_->Draw( "hist same" );
      GC_Models->cd(GridBins);
      TH1D *truthslice_h_clone = (TH1D*)slice_truth->hist_->Clone(uniq());
      TH1D *truthslice_h_clone2 = (TH1D*)slice_truth->hist_->Clone(uniq());
      RatioHist.insert(std::pair<std::string, TH1D*>("Truth",truthslice_h_clone2)); 
      truthslice_h_clone->SetLineWidth( 2 );
        if(WindowZoomScale.at(sl_idx) != 1){
         truthslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
      
      truthslice_h_clone->Draw( "hist same" );
    }
     std::cout<<"~~~~~~~~~~~~"<< std::endl;
    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*2.0 );
     c1->cd();
      pad1->cd();
    slice_unf->hist_->Draw( "e same" );
    TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    GC_Models->cd(GridBins);
     TH1D *unfslice_h_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
     unfslice_h_clone->SetLineWidth( 2 );

      if(WindowZoomScale.at(sl_idx) != 1){
         unfslice_h_clone->Scale(WindowZoomScale.at(sl_idx));
         }
     
    unfslice_h_clone->Draw( "e same" );
    drawString(BinStringMap[BinVector.at(sl_idx)], .035, false );

    
  if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Models->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .04,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(42);
			la2->SetTextSize(0.03);
			la2->Draw();
		}
    
    
    
    c1->cd();
    
    char HistName[1024];
    
    TLegend* lg = new TLegend( 0.15, 0.55, 0.82, 0.89 );
    lg->SetTextFont(132);
  lg->SetBorderSize(0);
    
    std::cout<<"Starting to Make Legend"<< std::endl;
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;

      std::ostringstream oss;
      
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_ << "";
      if ( chi2_result.num_bins_ > 1 ) oss << "";


      if ( name != DataType_string ) {
        label += ": #chi^{2}/ndf = " + oss.str() + " p-value = " + to_string_with_precision(chi2_result.p_value_);
      }
      
      

       lg->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
       

    }
    ///////////////////////////////////////
  // Drawing Ratio 
  //////////////////////////////////////
    
    
    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.25);
    pad2->SetTopMargin(.0);
    pad2->SetBottomMargin(0.22);
    //pad2->SetLeftMargin(0.1); 
    pad2->SetGridx(); // vertical grid
    //pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();
    
     RatioHist["DATA"]->Divide(RatioHist["Truth"]);
     RatioHist["DATA"]->GetYaxis()->SetTitle("Ratio to Truth");
     RatioHist["DATA"]->GetXaxis()->SetTitle("cos#theta_{#mu}");
     RatioHist["DATA"]->GetYaxis()->SetLabelSize(.1);
     RatioHist["DATA"]->GetXaxis()->SetLabelSize(.1);
     RatioHist["DATA"]->GetXaxis()->SetTitleSize(0.12);
     RatioHist["DATA"]->GetYaxis()->SetTitleSize(0.1);
     RatioHist["DATA"]->SetTitle("");
    
     //RatioHist["DATA"]->GetXaxis()->CenterTitle();
     RatioHist["DATA"]->GetYaxis()->CenterTitle();
     RatioHist["DATA"]->GetYaxis()->SetTitleOffset(.4);
     RatioHist["DATA"]->GetXaxis()->SetTitleOffset(.75);
     RatioHist["DATA"]->SetMinimum(0);
     RatioHist["DATA"]->SetMaximum(2.0);
     RatioHist["DATA"]->Draw("e");
     
    for(auto h_model:RatioHist ){
       if(h_model.first == "DATA" || h_model.first == "Truth") continue;
      h_model.second->Divide(RatioHist["Truth"]);
      h_model.second->SetLineWidth( 2 );
      h_model.second->Draw("Same Hist");
    }
  
    
    ///////////////////////////////////////////
    // Draw Norm Uncernity 
    //////////////////////////////////////////
           TH1D* h_normUncern = (TH1D*)slice_unf->hist_->Clone(uniq());
           //h_normUncern->Sumw2();
          auto CovMatrix_NormCombined = unfolded_cov_matrix_map[ "total_combined_norm" ].get(); 
 

   for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double widthNorm = h_normUncern->GetBinWidth( global_bin_idx );
        widthNorm *= other_var_width;
        Double_t element = (*CovMatrix_NormCombined)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * widthNorm*widthNorm);
        double Uncern_value = sqrt(Var);
        // if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        std::cout<<"Bin : "<< Bin_matrix << "var =  "<< Var << " Uncern_value = "<<Uncern_value<< std::endl;
       // std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
       if( Uncern_value > 0 && std::isnan(Uncern_value)==false){
        h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        h_normUncern->SetBinError( global_bin_idx, 0. );
        }
        
      else{
      h_normUncern->SetBinContent( global_bin_idx,0);
      h_normUncern->SetBinError( global_bin_idx, 0. );}
        
      }


   h_normUncern->SetFillColor(12);
   h_normUncern->SetLineWidth(0);
   h_normUncern->SetLineColor(0);
   h_normUncern->SetFillStyle(3001);
   TH1D* h_normUncern_clone = (TH1D*)h_normUncern->Clone(uniq());
   lg->AddEntry( h_normUncern, "Norm unc.", "f" );
   c1->cd();
   pad1->cd();
   //h_normUncern->Draw("hist same");
   
   THStack *stack = new THStack("stack", "Stacked Histograms");
   stack->Add(h_normUncern);
   stack->Draw("HISTF same");
   lg->Draw( "same" );    
   
   
   
  GC_Models->cd(GridBins); 
  if(WindowZoomScale.at(sl_idx) != 1){
    h_normUncern_clone->Scale(WindowZoomScale.at(sl_idx));
  }
 
  THStack *stack_panel = new THStack("stack_panel", "stack_panel Histograms");
  stack_panel->Add(h_normUncern_clone);
  stack_panel->Draw("HISTF same");
 
   
   
   
   c1->cd();
   sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
   c1 -> Print(pdf_title);
   
   
   
  
   ////////////////////////////////////////////////////////////////////////////
   //// Drawing Uncernity 
   ////////////////////////////////////////////////////////////////////////////


    //TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    
     //h_unfolded_slice_clone->SetDirectory( nullptr );

    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };

  
    //    const std::vector< std::string > cov_mat_keys = { "total", "xsec_total", 
    //  "MCstats", "EXTstats"
    //};


 //////////////////////////////////////////////////////////
 // Plotting Error 
 //////////////////////////////////////////////////////
 
    int color = 1;

  for(auto CovMatrixName: cov_mat_keys){
      const auto& key = CovMatrixName;
      //const auto &cov_matrix = pair.second;
    auto CovMatrix_ = unfolded_cov_matrix_map[ CovMatrixName ].get(); 
    std::cout<<"Inside: or(auto CovMatrixName: cov_mat_keys) loop - Making matrix_map_error :: key::"<< key<< std::endl;
    

    TH1D* h_Error = (TH1D*)h_unfoledData_clone_Error->Clone(uniq());
   
      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        const auto& bin_set = bin_pair.second;
              int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double y = h_unfoledData_clone_Error->GetBinContent( global_bin_idx );
        double width = h_unfoledData_clone_Error->GetBinWidth( global_bin_idx );
        width *= other_var_width;
        //double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        Double_t element = (*CovMatrix_)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * width*width);
                
        //1e38 / ( width * integ_flux * num_Ar );

        
            double frac = 0.;
        if ( y > 0. ) frac = sqrt(Var) / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        
        std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
        h_Error->SetBinContent( global_bin_idx, frac );
        h_Error->SetBinError( global_bin_idx, 0. );

      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = h_Error;

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

     if(key == "BNBstats" ||
     key == "EXTstats" ||
     key == "MCstats" ||
     key == "DataStats"){
     
     frac_uncertainty_hists[ key ]->SetLineStyle(2);}


      frac_uncertainty_hists[ key ]->SetLineColor( color );
      frac_uncertainty_hists[ key ]->SetLineWidth( 3 );
    }


    TLegend* lg2 = new TLegend( 0.15, 0.7, 0.8, 0.89 );
     lg2->SetNColumns(3);


    auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] ); //h_Error_total; //
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.6 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
    GC_Models_ERROR->cd(GridBins);
    TH1D* total_frac_err_hist_clone = (TH1D*)total_frac_err_hist->Clone(uniq());
    total_frac_err_hist_clone->SetLineWidth( 2 );
    total_frac_err_hist_clone->SetTitle("");
    total_frac_err_hist_clone->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty"); 
     
    lg2->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    if(sl_idx==1) lg1_Grid_Error->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );
    
    
    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0] ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
     if(sl_idx==1) lg1_Grid_Error->AddEntry(  hist, name.c_str(), "l" );
      c1->cd();
      hist->Draw( "same hist" );
      
      GC_Models_ERROR->cd(GridBins);
      TH1D* hist_clone = (TH1D*)hist->Clone(uniq());
      hist_clone->SetLineWidth( 2 );
      hist_clone->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }
    c1->cd();
    lg2->Draw( "same" );


  GC_Models_ERROR->cd(GridBins);
  drawString(BinStringMap[BinVector.at(sl_idx)], .03, false );
 
  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);



 } ////////////////////////////////////////
 ////////////////////////////////////////
 ////////////////////////////////////////
 // ENd of slices
 ////////////////////////////////////////
  
  
  std::cout<<"Starting to make inclusive Bin N Plot"<< std::endl;
  
  ///////////////////////////////////////
  // Make Plot for Bin Number 
  ////////////////////////////////////

 {
 
  c1->cd();
  TLegend* lg_binN = new TLegend( 0.15, 0.55, 0.8, 0.89 );
  lg_binN->SetNColumns(1);
  lg_binN->SetBorderSize(0);
  const auto& slice = sb.slices_.at( 9 );
    std::cout<<"Making Slice (should be by Bin Number ) : "<< 10 << std::endl;
    // Make a histogram showing the unfolded true event counts in the current
    // slice
    SliceHistogram* slice_unf = SliceHistogram::make_slice_histogram(
      *result.unfolded_signal_, slice, result.cov_matrix_.get() );

    // Temporary copies of the unfolded true event count slices with
    // different covariance matrices
    std::map< std::string, std::unique_ptr<SliceHistogram> > sh_cov_map;
    for ( const auto& uc_pair : unfolded_cov_matrix_map ) {
      const auto& uc_name = uc_pair.first;
      const auto& uc_matrix = uc_pair.second;
         std::cout<< "line 9665::"<< uc_name << std::endl;
      auto& uc_ptr = sh_cov_map[ uc_name ];
      uc_ptr.reset(
        SliceHistogram::make_slice_histogram( *result.unfolded_signal_, slice,
        uc_matrix.get() )
      );
    }

    // Also use the GENIE CV model to do the same
    SliceHistogram* slice_cv = SliceHistogram::make_slice_histogram(
      genie_cv_truth_vec, slice, nullptr );
      
       if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice, "CrossSection_2D_ExtraModels_Inclusive","Slice cv","bins",c1,60);

    // If present, also use the truth information from the fake data to do the
    // same
    SliceHistogram* slice_truth = nullptr;
    if ( using_fake_data ) {
      slice_truth = SliceHistogram::make_slice_histogram( fake_data_truth,
        slice, nullptr );
        
     if(DEBUG_PLOTS==true) DrawSlice(slice_truth,slice, "CrossSection_2D_ExtraModels_Inclusive","Slice fake data before unfolding","bins",c1,60);
        
    }

    // Keys are legend labels, values are SliceHistogram objects containing
    // true-space predictions from the corresponding generator models
    auto* slice_gen_map_ptr = new std::map< std::string, SliceHistogram* >();
    auto& slice_gen_map = *slice_gen_map_ptr;

    slice_gen_map[DataType_string ] = slice_unf;
    if ( using_fake_data ) {
      slice_gen_map[ "truth" ] = slice_truth;
    }
    slice_gen_map[ MicroBooNEType_string ] = slice_cv;
     
      if(DEBUG_PLOTS==true) DrawSlice(slice_cv,slice,"CrossSection_2D_ExtraModels_Inclusive", "CV Slice","bins",c1,60);
      if(DEBUG_PLOTS==true) DrawSlice(slice_unf,slice,"CrossSection_2D_ExtraModels_Inclusive", "unfolded Slice","bins",c1,60);
 
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;
      std::cout<<"Adding " << model_name << " to  slice_gen_map"<< std::endl;
      //SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
      //  *truth_mat, slice, nullptr );
      
       SliceHistogram* temp_slice = SliceHistogram::make_slice_histogram(
        *truth_mat, slice, nullptr );
        std::string modeltitle = model_name + " First generator_truth_map loop  ";
        
        if(DEBUG_PLOTS==true) 
        DrawSlice(temp_slice,slice,"CrossSection_2D_ExtraModels_Inclusive", modeltitle,"bins",c1,60);
        
        if(DEBUG_PLOTS==true) 
        visualizeMatrix(*truth_mat, model_name,  "CrossSection_2D_ExtraModels_Inclusive.pdf");

      slice_gen_map[ model_name ] = temp_slice;
    }
 
 
    int var_count = 0;
    std::string diff_xsec_denom;
    std::string diff_xsec_units_denom;
    std::string diff_xsec_denom_latex;
    std::string diff_xsec_units_denom_latex;
    double other_var_width = 1.;
    for ( const auto& ov_spec : slice.other_vars_ ) {
      double high = ov_spec.high_bin_edge_;
      double low = ov_spec.low_bin_edge_;
      const auto& var_spec = sb.slice_vars_.at( ov_spec.var_index_ );
      if ( high != low && std::abs(high - low) < BIG_DOUBLE ) {
        ++var_count;
        other_var_width *= ( high - low );
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;
        const std::string& temp_units = var_spec.units_;
        if ( !temp_units.empty() ) {
          diff_xsec_units_denom += " / " + temp_units;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    for ( size_t av_idx : slice.active_var_indices_ ) {
      const auto& var_spec = sb.slice_vars_.at( av_idx );
      const std::string& temp_name = var_spec.name_;
      if ( temp_name != "true bin number" ) {
        var_count += slice.active_var_indices_.size();
        diff_xsec_denom += 'd' + var_spec.name_;
        diff_xsec_denom_latex += " d" + var_spec.latex_name_;

        if ( !var_spec.units_.empty() ) {
          diff_xsec_units_denom += " / " + var_spec.units_;
          diff_xsec_units_denom_latex += " / " + var_spec.latex_units_;
        }
      }
    }

    // NOTE: This currently assumes that each slice is a 1D histogram
    // TODO: revisit as needed
    int num_slice_bins = slice_unf->hist_->GetNbinsX();
    TMatrixD trans_mat( num_slice_bins, num_slice_bins );
    TMatrixD trans_unit( num_slice_bins, num_slice_bins );
    
    
    for ( int b = 0; b < num_slice_bins; ++b ) {
      double width = slice_unf->hist_->GetBinWidth( b + 1 );
      width *= other_var_width;
      trans_mat( b, b ) = 1e38 / ( width * integ_flux * num_Ar );
      trans_unit(b, b) = 1.0 / width;
    } 

    std::string slice_y_title;
    std::string slice_y_latex_title;
    if ( var_count > 0 ) {
      slice_y_title += "d";
      slice_y_latex_title += "{$d";
      if ( var_count > 1 ) {
        slice_y_title += "^{" + std::to_string( var_count ) + "}";
        slice_y_latex_title += "^{" + std::to_string( var_count ) + "}";
      }
      slice_y_title += "#sigma/" + diff_xsec_denom;
      slice_y_latex_title += "\\sigma / " + diff_xsec_denom_latex;
    }
    else {
      slice_y_title += "#sigma";
      slice_y_latex_title += "\\sigma";
    }
    slice_y_title += " [10^{-38} cm^{2}" + diff_xsec_units_denom + " / Ar]";
    slice_y_latex_title += "\\text{ }(10^{-38}\\text{ cm}^{2}"
      + diff_xsec_units_denom_latex + " / \\mathrm{Ar})$}";



    // Convert all slice histograms from true event counts to differential
    // cross-section units
    for ( auto& pair : slice_gen_map ) {
      auto* slice_h = pair.second;
      const auto& name = pair.first;
       
       std::string inputtitle = "Before Tranfermation , Model:" + name;
       
        if(DEBUG_PLOTS==true) DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive", inputtitle,"bins",c1,60);
       
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string )
           {
           slice_h->transform( trans_mat );
           }
           else {
           slice_h->transform( trans_unit );
           }
      
          slice_h->hist_->GetYaxis()->SetTitle( slice_y_title.c_str() );
      
      
        std::string inputtitleout = "After Tranfermation , Model:" + name;
       if(DEBUG_PLOTS==true)  DrawSlice(slice_h,slice,"CrossSection_2D_ExtraModels_Inclusive", inputtitleout,"bins",c1,60);
      
    }

    // Also transform all of the unfolded data slice histograms which have
    // specific covariance matrices
    
    //CovMatrixMap Uncern_CovMap_BinN;
    
    
    for ( auto& sh_cov_pair : sh_cov_map ) {
      auto& slice_h = sh_cov_pair.second;
      
      const auto& name = sh_cov_pair.first;
       
       if ( name == DataType_string ||
            name == "truth" || 
           name == MicroBooNEType_string ) {
           slice_h->transform( trans_mat );
           
           if(name == DataType_string){
           /// maybe I can extract the uncenrity on the data here 
           //CovMatrix orig_cov = *slice_h.cmat_;
           //Uncern_CovMap_BinN[name]=orig_cov
           }
           }
           
      else {
      slice_h->transform( trans_unit );
      } 
    }
    
    // Keys are generator legend labels, values are the results of a chi^2
    // test compared to the unfolded data (or, in the case of the unfolded
    // data, to the fake data truth)
    
    std::map< std::string, SliceHistogram::Chi2Result > chi2_map;
    std::cout << '\n';
    /*
    for ( const auto& pair : slice_gen_map ) { 
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      // Decide what other slice histogram should be compared to this one,
      // then calculate chi^2
      SliceHistogram* other = nullptr;
      // We don't need to compare the unfolded data to itself, so just skip to
      // the next SliceHistogram and leave a dummy Chi2Result object in the map
      
      if ( name == DataType_string ) {
        chi2_map[ name ] = SliceHistogram::Chi2Result();
        continue;
      }
      // Compare all other distributions to the unfolded data
      else {
        other = slice_gen_map.at(DataType_string );
      }
       
       std::cout<<"Chi2Result: for Name = "<< name<<std::endl;
      // Store the chi^2 results in the map
      const auto& chi2_result = chi2_map[ name ] = slice_h->get_chi2( *other );

      std::cout << "Slice " << 10 << ", Bin N "  << " Chi2 = "
        << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bin";
      if ( chi2_result.num_bins_ > 1 ) std::cout << 's';
      std::cout << ", p-value = " << chi2_result.p_value_ << '\n';
      
    }  
    
    */
    
    std::cout<<"Finished slice_gen_map "<< std::endl;

    //TCanvas* c1 = new TCanvas;
    slice_unf->hist_->SetLineColor( kBlack );
    slice_unf->hist_->SetLineWidth( 2 );
    //slice_unf->hist_->SetMarkerStyle( kFullCircle );
    //slice_unf->hist_->SetMarkerSize( 0.8 );
    slice_unf->hist_->SetStats( false );

    double ymax = -DBL_MAX;
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    slice_unf->hist_->Draw( "hist" );
    
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == DataType_string || name == "truth"
        || name == MicroBooNEType_string ) continue;

      const auto& file_info = truth_CC0Pifile_map.at( name );
      slice_h->hist_->SetLineColor( file_info.color_ );
      slice_h->hist_->SetLineStyle( file_info.style_ );
      slice_h->hist_->SetLineWidth( 2 );
      slice_h->hist_->Draw( "hist same" );
    }



    slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*2 );
    slice_cv->hist_->SetStats( false );
    slice_cv->hist_->SetLineColor( kAzure - 7 );
    slice_cv->hist_->SetLineWidth( 2 );
    slice_cv->hist_->SetLineStyle( 5 );
    //slice_cv->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.8 );
    slice_cv->hist_->Draw( "hist same" );
    
    
    if ( using_fake_data ) {
      slice_truth->hist_->SetStats( false );
      slice_truth->hist_->SetLineColor( kOrange );
      slice_truth->hist_->SetLineWidth( 2 );
      slice_truth->hist_->Draw( "hist same" );
    }
     std::cout<<"~~~~~~~~~~~~"<< std::endl;
    //
    //if(ymax> 5) slice_unf->hist_->GetYaxis()->SetRangeUser( 0., 55 );
    //else slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.07 );
    //slice_unf->hist_->GetYaxis()->SetRangeUser( 0., ymax*1.6 );
    //slice_unf->hist_->Draw( "e same" );
    
    std::cout<<"Starting to Make Legend"<< std::endl;
    TH1D* slice_unf_clone = (TH1D*)slice_unf->hist_->Clone(uniq()); 
    
    /*
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      std::string label = name;

      std::ostringstream oss;
      
      const auto& chi2_result = chi2_map.at( name );
      oss << std::setprecision( 3 ) << chi2_result.chi2_ << " / "
        << chi2_result.num_bins_  << "";
      if ( chi2_result.num_bins_ > 1 ) oss << "";

      Double_t  chi2;
      Int_t ndf, isgood;
      int ndf_test; 

   
      //if ( name != DataType_string ) {
      //TH1D* model_clone = (TH1D*)slice_h->hist_->Clone(uniq()); 
      //auto NBins_test = slice_unf_clone->GetNbinsX();
      //auto NBins_test2 = model_clone->GetNbinsX();
      //
      ////double chisqt_local = calculateChiSquare(slice_unf_clone, model_clone, ndf_test);
      //
      //  double pValue_new = TMath::Prob(chisqt_local, NBins_test);
      //  double pvalue = slice_unf_clone->Chi2TestX(model_clone, chi2, ndf,isgood);
      //  std::cout<<"Nbins1 = "<< NBins_test << " NBins2 = "<< NBins_test2 << std::endl;
      //  std::cout<<'Chisqt isGood = '<< isgood << "ndf = "<< ndf << " ChiSqt = "<< chi2<< " Local Chi ="<<  chisqt_local<< "pvalue = "<< pvalue<< " pvalue_new ="<< pValue_new <<  std::endl;
      //  label += ": #chi^{2}/ndf = " + to_string_with_precision(chisqt_local) +" / "+  std::to_string(NBins_test2) + " p-value = " + to_string_with_precision(pvalue);
      //}


         
          if ( name != DataType_string ) {
         label += ": #chi^{2}/ndf = " + oss.str() + " p-value = " + to_string_with_precision(chi2_result.p_value_);
       }
      
     // if(name == DataType_string){
     // lg_binN->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
      
     // } 
      
        lg_binN->AddEntry( slice_h->hist_.get(), label.c_str(), "l" );
        TH1D *slice_h_clone = (TH1D*)slice_h->hist_.get()->Clone(uniq());
       lg1_Grid->AddEntry( slice_h_clone, label.c_str(), "l" );
 
         
    }
    */
    
    TH1D* h_normUncern = (TH1D*)slice_unf->hist_->Clone(uniq());
   //h_normUncern->Sumw2();
   auto CovMatrix_NormCombined = unfolded_cov_matrix_map[ "total_combined_norm" ].get(); 
    for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        const auto& bin_set = bin_pair.second;
          int Bin_matrix; 
            for (size_t element : bin_set) {
            // Use the element from the set
            std::cout << "global_bin_idx= "<< global_bin_idx <<  " element: " << element << std::endl;
            Bin_matrix = element;
        }
        
        double widthNorm = h_normUncern->GetBinWidth( global_bin_idx );
        widthNorm *= other_var_width;
        Double_t element = (*CovMatrix_NormCombined)(Bin_matrix, Bin_matrix);
        double Var1 = (element);
        double Var = (Var1 * 1e38 * 1e38) / (integ_flux*integ_flux * num_Ar*num_Ar * widthNorm*widthNorm);
        double Uncern_value = sqrt(Var);
        // if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< (element*element) << " frac = "<< frac<< std::endl;
        std::cout<<"Bin : "<< Bin_matrix << "var =  "<< Var << " Uncern_value = "<<Uncern_value<< std::endl;
       // std::cout<<" y =  "<< y << " var1 = " << Var1 << " Var = "<< Var<< "Var sqrted = "<< (Var*Var) << " Frac = " << frac << std::endl;
       // h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        //h_normUncern->SetBinError( global_bin_idx, 0. );
      
      
        if( Uncern_value > 0 && std::isnan(Uncern_value)==false){
        h_normUncern->SetBinContent( global_bin_idx, Uncern_value);
        h_normUncern->SetBinError( global_bin_idx, 0. );
        }
      else{
           h_normUncern->SetBinContent( global_bin_idx,0);
           h_normUncern->SetBinError( global_bin_idx, 0. );
      }  
      
    }


  h_normUncern->SetFillColor(12);
  h_normUncern->SetLineWidth(0);
  h_normUncern->SetLineColor(0);
  h_normUncern->SetFillStyle(3001);
  
  TH1D* h_normUncern_clone = (TH1D*)h_normUncern->Clone(uniq());
   lg_binN->AddEntry( h_normUncern, "Norm unc.", "f" );
     THStack *stack_binN = new THStack(uniq(), "Stacked Histograms");
      stack_binN->Add(h_normUncern);
      stack_binN->Draw("HISTF same");

   
   lg_binN->Draw( "same" );

  
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);
  
  
  std::cout<<"Drawing Fractional Uncerinity "<< std::endl;
  
  /////////////////////////////////////////////////////////////
  // Fractional Uncerinty by Bin Number
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
   //// Drawing Uncernity 
   ////////////////////////////////////////////////////////////////////////////


    //TH1D* h_unfolded_slice_clone = (TH1D*)slice_unf->hist_->Clone(uniq());
    
     //h_unfolded_slice_clone->SetDirectory( nullptr );

    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats"
    };


   //for ( auto& sh_cov_pair : sh_cov_map ) {
   //   auto& slice_h = sh_cov_pair.second;
   //   
   //   const auto& name = sh_cov_pair.first;
   //   std::cout<<"Inside Loop for sh_cov_map:: name =  "<< name << std::endl;
   //   
   //    if ( name == DataType_string ||
   //         name == "truth" || 
   //        name == MicroBooNEType_string ) {
   //        slice_h->transform( trans_mat );
   //        
   //         if(name == DataType_string){
   //        /// maybe I can extract the uncenrity on the data here 
   //         TMatrixD Matrix_temp;
   //         Matrix_temp = slice_h->get_TMatrixD();
   //        
   //        CovMatrix Matrix_temp_input(Matrix_temp);
   //       Uncern_CovMap.insert(namMatrix_temp_input);
   //        
   //        }
   //        
   //        }
   //   else {
   //   slice_h->transform( trans_unit );
   //   }
   //   
   // }
//

  //
    int color = 1;
    for ( const auto& pair : matrix_map ) {

      const auto& key = pair.first;
      const auto &cov_matrix = pair.second;

      std::cout<<"Inside: matrix_map_error :: key::"<< key<< std::endl;

      auto& uc_ptr = sh_cov_map[ key ];
      SliceHistogram* slice_for_syst = uc_ptr.get();

    
      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      for ( const auto& bin_pair : slice.bin_map_ ) {
        int global_bin_idx = bin_pair.first;
        
        
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
                double frac = 0.;
        if ( y > 0. ) frac = err / y;
         if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Inside Bin Mapping Global Bin  Index:  "<< global_bin_idx<< " y = "<< y << " error = "<< err << " frac = "<< frac<< std::endl;
        

        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }

      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      if(PrintStatement_Uncertainty_Debug==true) std::cout<<"Key = "<< key << std::endl;
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color; 
      if ( color >= 10 ) color += 10;

        if(key == "BNBstats" ||
        key == "EXTstats" ||
        key == "MCstats" ||
        key == "DataStats"){slice_for_syst->hist_->SetLineStyle(2);}


      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );
    }


    TLegend* lg4 = new TLegend(  0.15, 0.65, 0.8, 0.89 );
    lg4->SetNColumns(3);
    auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] );
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.7 );
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->Draw( "hist" );
    total_frac_err_hist->GetYaxis()->SetTitle("Fractional Uncertainty");  
    lg4->AddEntry( total_frac_err_hist, cov_mat_keys[0].c_str(), "l" );

    for ( auto& pair : frac_uncertainty_hists ) {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == cov_mat_keys[0].c_str() ) continue;

      lg4->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
        << hist->GetBinContent( 1 )*100. << "%\n";
    }

    lg4->Draw( "same" );

  c1->cd();
  sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
  c1 -> Print(pdf_title);




 }

 ///////////////////////////////////// 
  
  
 GC_Models->cd(1);
 lg1_Grid->Draw("same");
 
 DrawGridCanvas(GC_Models,
 lg1_Grid, "p_{#mu} [GeV/c]", 
 crossSectionYaxis, pdf_title,
 0,80,
 .1, 2. );
  

  GC_Models_ERROR->cd(1);
  lg1_Grid_Error->Draw("same");

 DrawGridCanvas(GC_Models_ERROR,
 lg1_Grid_Error, "p_{#mu} [GeV/c]", 
 "Fractional Uncertainity", pdf_title,
 0,1.5,
 .1, 2.);

  // ******* Also look at reco-space results
  TH1D* reco_data_hist = dynamic_cast< TH1D* >(
    syst.data_hists_.at( NFT::kOnBNB )->Clone( "reco_data_hist" )
  );
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
  const auto& cv_univ = syst.cv_universe();
  int num_reco_bins = reco_data_hist->GetNbinsX();

  // Clone the reco data hist twice. We will fill the clones with the CV
  // MC+EXT prediction and the constrained one
  TH1D* reco_mc_and_ext_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_mc_and_ext_hist" )
  );
  reco_mc_and_ext_hist->Reset();
  reco_mc_and_ext_hist->Add( reco_ext_hist );
  reco_mc_and_ext_hist->Add( cv_univ.hist_reco_.get() );

  TH1D* reco_constrained_hist = dynamic_cast< TH1D* >(
    reco_data_hist->Clone( "reco_constrained_hist" )
  );
  reco_constrained_hist->Reset();

  // Get the post-constraint event counts and covariance matrix in the
  // signal region
  auto meas = syst.get_measured_events();

  for ( int rb = 0; rb < num_reco_bins; ++rb ) {

    double mcc9_err = std::sqrt(
      std::max( 0., cov_mat->GetBinContent(rb + 1, rb + 1) )
    );
    reco_mc_and_ext_hist->SetBinError( rb + 1, mcc9_err );

    if ( rb >= num_ordinary_reco_bins ) {
      double data_evts = reco_data_hist->GetBinContent( rb + 1 );
      reco_constrained_hist->SetBinContent( rb + 1, data_evts );
      reco_constrained_hist->SetBinError( rb + 1, 0. );
    }
    else {
      double constr_pred = meas.reco_mc_plus_ext_->operator()( rb, 0 );
      double constr_err = std::sqrt(
        std::max( 0., meas.cov_matrix_->operator()(rb, rb) )
      );

      reco_constrained_hist->SetBinContent( rb + 1, constr_pred );
      reco_constrained_hist->SetBinError( rb + 1, constr_err );
    }

  }

  //TCanvas* c2 = new TCanvas;

  reco_data_hist->SetLineColor( kBlack );
  reco_data_hist->SetLineWidth( 5 );

  reco_mc_and_ext_hist->SetLineColor( kRed );
  reco_mc_and_ext_hist->SetLineStyle( 2 );
  reco_mc_and_ext_hist->SetLineWidth( 4 );

  reco_constrained_hist->SetLineColor( kBlue );
  reco_constrained_hist->SetLineStyle( 9 );
  reco_constrained_hist->SetLineWidth( 4 );

  reco_data_hist->Draw( "e" );
  reco_mc_and_ext_hist->Draw( "same hist e" );
  reco_constrained_hist->Draw( "same hist e" );

  reco_data_hist->Draw( "same e" );

  TLegend* lg2 = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg2->AddEntry( reco_data_hist, using_fake_data ? "fake data" : "data",
    "l" );
  lg2->AddEntry( reco_mc_and_ext_hist, "uB tune + EXT", "l" );
  lg2->AddEntry( reco_constrained_hist, "post-constraint", "l" );

  lg2->Draw( "same" );
  c1->cd();
  c1 -> Print(pdf_title);


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);

}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void drawVerticalLinesWithText(std::map<std::string, double> lines, double maxh) {
    // Iterate through the map and draw lines and text
    for (const auto &entry : lines) {
        const std::string &text = entry.first;
        double x = entry.second;

        // Draw vertical line
        TLine *line = new TLine(x, 0, x, maxh);
        line->SetLineColor(kBlack);  // Set line color to black
        line->SetLineWidth(3);       // Set line thickness
        line->Draw("samwe");

        TText *t = new TText(x + 0.15, maxh * 1.08, text.c_str());
        t->SetTextAlign(31); // Right-align the text to the left of the line
        t->SetTextSize(0.02); // Adjust text size if needed


        t->Draw("same");
    }
    
}
