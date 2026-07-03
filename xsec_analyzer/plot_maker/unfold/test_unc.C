// Standard library includes
#include <iomanip>
#include <iostream>
#include <sstream>

// ROOT includes
#include "TCanvas.h"
#include "TLegend.h"

// STV analysis includes
#include "../DAgostiniUnfolder.hh"
#include "../FiducialVolume.hh"
#include "../MCC9SystematicsCalculator.hh"
#include "../NormShapeCovMatrix.hh"
#include "../PGFPlotsDumpUtils.hh"
#include "../SliceBinning.hh"
#include "../SliceHistogram.hh"
#include "../TruthSystematicsCalculator.hh"
#include "../WienerSVDUnfolder.hh"

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

constexpr double BIG_DOUBLE = 1e300;

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

//const std::string SAMPLE_NAME = "MicroBooNE_CC1MuNp_XSec_2D_PmuCosmu_nu_MC";
const std::string SAMPLE_NAME = "MicroBooNE_CC1MuNp_XSec_2D_PpCosp_nu_MC";

struct TruthFileInfo {
  TruthFileInfo() {}
  TruthFileInfo( const std::string& file_name, int color, int style )
    : file_name_( file_name ), color_( color ), style_( style ) {}

  std::string file_name_;
  int color_;
  int style_;
};

// Keys are generator names and versions, values are TruthFileInfo objects
// describing nuiscomp output files containing the differential cross-section
// predictions in each true bin
std::map< std::string, TruthFileInfo > truth_file_map = {
  { "GENIE 2.12.10",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv2.root", kBlue, 1 } },
  { "GENIE 3.0.6",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3.root", kBlack, 2} },
  { "GENIE 3.2.0 G18_02a",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3-g1802a.root", kBlack, 2} },
  { "GENIE 3.2.0 G21_11a",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3-g2111a.root", kBlack, 2} },
  { "GENIE 3.2.0 G21_11b",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-gv3-g2111b.root", kBlack, 2} },
  { "NEUT 5.4.0.1",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-neut.root", kRed, 9} },
  { "NuWro 19.02.1",
    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/ubmc/comp-all/comp-nuwro.root", kViolet, 7} },
// { "GiBUU 2021",
//    {"/uboone/app/users/gardiner/temp-gen/BuildEventGenerators/nuisance/build/myout.root", kGreen, 10} },
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
  { "NEUT 5.4.0.1", "neut" },
  { "NuWro 19.02.1", "nuwro" },
  { "GiBUU 2021", "gibuu" },
};

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
    "/gardiner/respmat-test-new-Muon2D.root", "/uboone/app/users/gardiner"
    "/stv/mc/nuisance/data/MicroBooNE/mybins2Dmuon.txt",
    "mybins_mcc9_2D_muon.txt" } },

  // 2D proton measurement
  { "MicroBooNE_CC1MuNp_XSec_2D_PpCosp_nu_MC", { "/uboone/data/users"
    "/gardiner/respmat-test-new-Proton2D.root", "/uboone/app/users/gardiner"
    "/stv/mc/nuisance/data/MicroBooNE/mybins2Dproton.txt",
    "mybins_mcc9_2D_proton.txt" } },

};

// Multiplying by the conversion factor conv_factor should change a total cross
// section value (in cm^2 / Ar) into an expected number of true event counts.
// The value of the factor can be obtained by multiplying the number of Ar
// targets in the fiducial volume by the integrated numu flux (for the measured
// POT exposure) in the fiducial volume.
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


void dump_slice_errors( const std::string& hist_col_prefix,
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

void get_cv_model_and_covariance( const std::string& respmat_file_name,
  const std::string& syst_config_file_name, TH1D*& event_hist,
  CovMatrix*& covariance )
{
  // This function expects to be passed null pointers. If the pointers are
  // non-null, make them null by deleting what was there before
  if ( event_hist ) delete event_hist;
  if ( covariance ) delete covariance;

  // Calculate the systematic uncertainties on the true bin counts
  TruthSystematicsCalculator syst( respmat_file_name, syst_config_file_name );

  // Get the CV prediction in each true bin (including the
  // background true bins)
  TH1D* cv_truth = dynamic_cast< TH1D* >(
    syst.cv_universe().hist_true_->Clone( "cv_truth" )
  );
  cv_truth->SetDirectory( nullptr );

  int num_true_bins = cv_truth->GetNbinsX();

  // Get a pointer to the map that stores the total and individual
  // covariance matrices on the CV prediction
  std::unique_ptr< CovMatrixMap > matrix_map_ptr = syst.get_covariances();

  // Get a std::unique_ptr< TMatrixD >
  const CovMatrix& cov = matrix_map_ptr->at( "total" );
  std::unique_ptr< TMatrixD > cov_mat = cov.get_matrix();
  //  = matrix_map_ptr->at( "total" ).get_matrix();

  for ( int b = 0; b < num_true_bins; ++b ) {
    double err2 = cov_mat->operator()( b, b );
    double err = std::sqrt( std::max(0., err2) );
    cv_truth->SetBinError( b + 1, err );
  }


}

void test_unc() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  //auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties.txt" );

  const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
  //const auto& respmat_file_name = sample_info.respmat_file_;

  const std::string respmat_file_name( "/uboone/data/users/gardiner/myuniverses-all.root" );

  // Do the systematics calculations in preparation for unfolding
  auto* syst_ptr = new TruthSystematicsCalculator( respmat_file_name, "../systcalc_unfold_fd.conf" );
  //auto* syst_ptr = new MCC9SystematicsCalculator( respmat_file_name, "../systcalc.conf" );
  auto& syst = *syst_ptr;

  // Get the tuned GENIE CV prediction in each true bin (including the
  // background true bins)
  TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  int num_true_bins = genie_cv_truth->GetNbinsX();

  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto cov_mat = matrix_map.at( "total" ).get_matrix();

  for ( int b = 0; b < num_true_bins; ++b ) {
    double err2 = cov_mat->operator()( b, b );
    double err = std::sqrt( std::max(0., err2) );
    genie_cv_truth->SetBinError( b + 1, err );
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

  genie_cv_truth->SetStats( false );
  genie_cv_truth->SetLineColor( kRed );
  genie_cv_truth->SetLineWidth( 3 );
  genie_cv_truth->SetLineStyle( 9 );

  genie_cv_truth->Draw( "e" );

  TLegend* lg = new TLegend( 0.15, 0.7, 0.3, 0.85 );
  lg->AddEntry( genie_cv_truth, "uB tune", "l" );

  lg->Draw( "same" );
}

int main() {
  test_unc();
  return 0;
}
