///this is the function for GENIE Closure 
//Christian 
//
#include "Closure_Plotting_Functions.h"

void unfoldingGENIEClosure() {

  //// Initialize the FilePropertiesManager and tell it to treat the NuWro
  //// MC ntuples as if they were data
  //auto& fpm = FilePropertiesManager::Instance();
  //fpm.load_file_properties( "../nuwro_file_properties.txt" );
  
  
  std::string Pdf_name  = "CrossSection_2D_GENIEClosure";
  std::string Name_DATATYPE = "GENIE INPUT Data ";
  char pdf_title[1024];

 std::cout<<"Running : Test unfolding () "<< std::endl;

  //const auto& sample_info = sample_info_map.at( SAMPLE_NAME );
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
  
  //auto generator_truth_map = get_true_events_nuisance_v2(SAMPLE_NAME_2DFlat, 1.0);
  
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
    /*
    for ( const auto& pair : generator_truth_map ) {
      const auto& model_name = pair.first;
      TMatrixD* truth_mat = pair.second;

      TMatrixD ac_temp(A_C, TMatrixD::kMult, *truth_mat );
      *truth_mat = ac_temp;
    }
  */
  
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
    IncreaseTitleTH1(*slice_unf->hist_, .06);
    slice_unf->hist_->Draw( "e" );
    for ( const auto& pair : slice_gen_map ) {
      const auto& name = pair.first;
      const auto* slice_h = pair.second;

      double max = slice_h->hist_->GetMaximum();
      if ( max > ymax ) ymax = max;

      if ( name == "unfolded data" || name == "truth"
        || name == MicroBooNEType_string ) continue;

     // there should be no models 

      //const auto& file_info = truth_file_map.at( name );
      //slice_h->hist_->SetLineColor( file_info.color_ );
      //slice_h->hist_->SetLineStyle( file_info.style_ );
      //slice_h->hist_->SetLineWidth( 4 );

      //slice_h->hist_->Draw( "hist same" );
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
     lg2->SetNColumns(3);
     auto* total_frac_err_hist = frac_uncertainty_hists.at( cov_mat_keys[0] ); //h_Error_total; //
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,
    total_frac_err_hist->GetMaximum() * 1.6 );
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

///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

void IncreaseTitleTH1(TH1& hist, double input) {
    // Increase title size and center it
    gStyle->SetTitleSize(input, "t"); // Adjust the size as needed
    gStyle->SetTitleX(0.5); // Center the title horizontally
}
