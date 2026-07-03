////////////////////////////////////
// Playing this Slice Plots
///////////////////////////////////


// Standard library includes
#include <algorithm>



// ROOT includes
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TLegend.h"
#include "TObjArray.h"


// STV analysis includes
#include "../include/XSecAnalyzer/FilePropertiesManager.hh"
#include "../include/XSecAnalyzer/MCC9SystematicsCalculator.hh"
#include "../include/CC_Joint_cc0pi/PlotUtils.hh"
#include "../include/XSecAnalyzer/SliceBinning.hh"
#include "../include/XSecAnalyzer/SliceHistogram.hh"
#include "TLatex.h"
#include "../include/XSecAnalyzer/ConfigMakerUtils.hh"

using NFT = NtupleFileType;

#define USE_FAKE_DATA ""

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

// I want to print figures to PDF


std::map<std::string, int> ColorMap{
    {"total", kBlack},
    {"flux", 418},        // kGreen + 2
    {"xsec_total", 6},    // magenta
    {"reint", 4},         // blue
    {"detVar_total", 2},  // red
    {"numTargets", 618},  // kMagenta + 2
    {"POT", 398},         // kYellow - 2
    {"MCstats", 820},     // kSpring - 2
    {"EXTstats", 802},    // kOrange + 2
    {"BNBstats", 797}     // kOrange - 3
};



char pdf_title[1024];
char textplace[1024];
std::string Pdf_name = "Measuring_Vars_1D_Figures_Systematics_Errors_CC0piNp";
std::string Pdf_name_1D_NODATAPOINTS = "Measuring_Vars_1D_Figures_Systematics_Errors_NoDataPoints";
std::string Pdf_name_withData = "Measuring_Vars_1D_Figures_Systematics_Errors_1D_Ntracks_May10th";

//auto BinVector = GetProjectBinVector();


std::string Pdf_name2D = "Measuring_Vars_2D_Figures_Systematics_Errors_newTuples_new_sideband";
std::string Pdf_name2_Data = "Measuring_Vars_2D_Figures_Systematics_Errors_Selection_Liang";
std::string Pdf_name2D_inclusive = "Measuring_Vars_2D_inclusive_Figures_Systematics_Errors_newTuples_new_5_20_2024";
std::string Pdf_name2D_inclusive_Data = "Measuring_Vars_2D_inclusive_Figures_Systematics_Errors_Selection_PC_mcs";
//std::string Pdf_name2D_Closure = "Measuring_Vars_2D_inclusive_Figures_Systematics_Errors_GENIEClosure_v12";
std::string MicroBooNE_LegendTitle();
std::string MicroBooNE_LegendTitle_OpenData();
void DrawFakeData_GENIE();
void DrawFakeData_InProgess();
bool TrueOffChisquare = false; 


void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data,
  TH1D* Total_MC,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax, 
  bool Plot_EXT =true);
  
void DrawFractionalError(
CovMatrixMap &matrix_map,
std::vector< std::string > cov_mat_keys,
TH1D* BinSlice_template,
const Slice& slice,
char *XaxisTitle,
char *TotalHistName,
double Ymax,
std::string Pdf_name,
TCanvas *c1);

void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  bool makeNormWidth,
  double ymax,
  bool Plot_EXT= true);

void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  bool makeNormWidth,
   double SliceBinWidth,
  double ymax,
  double WindowZoomscale,
  bool Plot_EXT = true, 
  bool Scaleall= false , 
  double Scaleall_input = 1 );
  
  void DrawStack_WithRatio(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax,
  bool Plot_EXT,
  double &totalChi2,
  double &Totalpvalue,
  int &NDF,
  double SliceBinWidth = 1.0);
  
  void DrawStack_noData(
  TH2D* category_hist_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax,
  bool Plot_EXT );
  
  
void tutorial_slice_plots_withData();
void tutorial_slice_plots_NODATAPOINTS();
void tutorial_slice_plots_2D();
void tutorial_slice_plots_2D_withData();
void tutorial_slice_plots_2D_inclusive();
void tutorial_slice_plots_2D_inclusive_withData();
void tutorial_slice_plots_2D_Closure_Plots(std::string InputFileProperties, 
std::string inputUnimakeFile, std::string BinningFile, bool IsBinningScheme1,std::string Pdf_name2D_Closure );

void DrawFractionalError_GC(
CovMatrixMap &matrix_map,
std::vector< std::string > cov_mat_keys,
char *TotalHistName,
TH1D* BinSlice_template,
const Slice& slice,
std::string BinSliceInfo,
double Ymax,
GridCanvas *GridCanvas_input,
int BinN_toDraw,
bool FillLegend, 
TLegend* lg_input,
bool istotal = false);

void DrawGridCanvas(GridCanvas *GridCanvas_input,
TLegend* lg_input, std::string XaxisTitle, 
std::string YaxisTitle, std::string pdftitle,
double min_YAxis_GridCanvas, double max_YAxis_GridCanvas,
double min_XAxis_GridCanvas, double max_XAxis_GridCanvas );

void IncreaseTitleTH1(TH1& hist, double input);


//void ScaleHistogramsInStack(THStack* stack, double scaleFactor, char *option);

//void AddHistogramsToLegend(THStack* stack, TLegend* legend);

void AddPlotLabel(
        const char* label,
        const double x,
        const double y,
        const double size = 0.05,
        const int color = 1,
        const int font = 62,
        const int align = 22,
        const double angle = 0
      );

//void AddHistoTitle(
//  const char* title,
//  double titleSize,
//  int titleFont
//);



void tutorial_slice_plots() {

   std::cout<<"Starting tutorial_slice_plots"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
   fpm.load_file_properties( "nuwro_file_properties_Tuples_5_13_2024.txt" );
    std::cout<<" passed "<< std::endl;
  #endif

 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
 
 std::cout<<"Starting MCC9SystematicsCalculator"<< std::endl;

 
 
  //UnivMake_FakeData_BDTdecided_1D_v5_pmucorrection.root
  auto* syst_ptr = new MCC9SystematicsCalculator(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_CC0piNp_allConfig.root",
    "systcalc_noNuWro.conf" );
  auto& syst = *syst_ptr;
 //UnivMake_topological_score_v3_realData.root
 //UnivMake_topological_score.root
 //UnivMake_FakeData_BDTdecided_1D_v11_addBogustosideband_noBDTproton_pmucorrection.root
 //UnivMake_NTracks.root
 //UnivMake_FakeData_BDTdecided_1D_v11_addBogustosideband_noBDTproton_pmucorrection
 std::cout<<"Finished  MCC9SystematicsCalculator "<< std::endl; 

  bool Plot_EXT = false;


  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
   reco_bnb_hist->Add(reco_ext_hist,-1);
  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;

  #ifdef USE_FAKE_DATA
    // Add the EXT to the "data" when working with fake data
    //reco_bnb_hist->Add( reco_ext_hist ); Just remove this 
  #endif

  TH2D* category_hist = syst.cv_universe().hist_categ_.get();


  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );


 if(Plot_EXT == false){
   int nBins = reco_mc_plus_ext_hist->GetNbinsX();
     for (int i = 1; i <= nBins; ++i) {
         reco_mc_plus_ext_hist->SetBinContent(i, 0.0);
     }
 }

  // Add in the CV MC prediction
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* sb_ptr = new SliceBinning( "config_files_binning/mybins_mcc8_1D_NTracks.txt" ); //tutorial_reco_slice_config.txt //mybins_mcc8_1D_Topological_Score.txt  mybins_mcc8_1D.txt /mybins_mcc8_1D_NTracks.txt mybins_all.txt
  auto& sb = *sb_ptr;
  ////////////////////////////////
  // Starting to plot
  //////////////////////////////////

  std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
  c1 -> Print(pdf_title);
 /*
   std::vector<std::string> xaxistitles_vector{
   "Cos#theta_{#mu}",
   "p_{#mu} [GeV/c]",
   "Bin N","Bin N" , 
   "p_{#mu} [GeV/c]",
   "p_{#mu} [GeV/c]", 
   "p_{p} [GeV/c]",
   "p_{#mu} [GeV/c]"
 };
 */
 
 // std::vector<std::string> xaxistitles_vector{
 //  "cos#theta_{#mu}",
 //    "p_{#mu} [GeV/c]",
 //    "Bin N"
 //};

 std::vector<std::string> xaxistitles_vector{
  "Proton Multiplicity",
    "Bin N"
 };
 
  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';



    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "Data", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "Total MC", "le" );

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }


   if(Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->Draw( "e" );
    std::string xAxisTitle = slice_bnb->hist_->GetXaxis()->GetTitle();
    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);
      sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    
      sprintf(axisXtitle, "%s", xAxisTitle.c_str());
    
    double chi1,pvalue;
    int NDF;
    
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      Pdf_name,
      "Test",
      axisXtitle,
      c1,
      ymax,
      Plot_EXT,
      chi1,
      pvalue,
      NDF);


    c1->Clear();   
    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    /*
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats", "xsec_NormCCCOH"
    };
  */
  // const std::vector< std::string > cov_mat_keys = {   
  //      "xsec_total",
  //      "xsec_multi",
  //      "xsec_unisim",
  //      "xsec_xsr_scc_Fa3_SCC",
  //      "xsec_xsr_scc_Fv3_SCC",
  //       "POT","MCstats",
  //       "EXTstats", 
  //       "BNBstats"
  //    };
  

  
  
  /* 
   const std::vector< std::string > cov_mat_keys = {  
  };
  
     const std::vector< std::string > cov_mat_keys_cross = {  
                                         "xsec_unisim",
                                         "xsec_AxFFCCQEshape",
                                         "xsec_DecayAngMEC",
                                         "xsec_NormCCCOH",
                                         "xsec_NormNCCOH",
                                         "xsec_RPA_CCQE ",
                                         "xsec_ThetaDelta2NRad",
                                         "xsec_Theta_Delta2Npi",
                                         "xsec_VecFFCCQEshape",
                                         "xsec_XSecShape_CCMEC" };
   
   
   
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
       "xsec_xsr_scc_Fv3_SCC",
       "NuWroGenie"};
     */
    
  
  //detVar_total flux reint xsec_total POT numTargets
  //MCstats EXTstats BNBstats
  
  
  //  const std::vector< std::string > cov_mat_keys = { "xsec_unisim",
  //    "xsec_AxFFCCQEshape", "xsec_DecayAngMEC", "xsec_XSecShape_CCMEC", 
  //    "xsec_NormCCCOH",  "xsec_NormNCCOH", "xsec_RPA_CCQE",
  //     "xsec_ThetaDelta2NRad",  "xsec_Theta_Delta2Npi",
  //     "MCstats", "EXTstats", "BNBstats"
  //  };

   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      ++color;
      if ( color == 3 ) color = kGreen +2;
      if ( color == kGreen + 3) color = 4;
      if ( color == 5 ) color = 6;
      if ( color == 7 ) color = kYellow -2;
      if ( color == kYellow -1 ) color = kMagenta + 2;
      if (color == kMagenta + 3) color = kSpring - 2;
      if (color == kSpring - 1) color = kOrange + 2;
      if (color == kOrange + 3) color = kRed - 6;
      if (color == kRed - 5) color = kAzure + 8;
      if (color == kAzure + 8) color = kOrange - 3;
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend( 0.45, 0.7, 0.8, 0.88 );
    lg2->SetNColumns(2);
    lg2->SetBorderSize(0);
    lg2->SetTextSize(.025); //
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
    //auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    //frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(0.45);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle( "" );
    total_frac_err_hist->GetYaxis()->SetLabelSize(.02);
    total_frac_err_hist->GetXaxis()->SetLabelSize(.025);
    total_frac_err_hist->GetXaxis()->SetTitleSize(0.035);
    total_frac_err_hist->GetXaxis()->CenterTitle(kFALSE);
    total_frac_err_hist->SetTitleOffset (1.01,"Y");
    
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
      << hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    std::cout << "Total frac error in bin #1 = "
      << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);

 DrawFractionalError(
 matrix_map,
  cov_mat_keys,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "total",
 .55,
 pdf_title,
 c1);
 
 
 DrawFractionalError(
 matrix_map,
  cov_mat_keys_cross,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "xsec_unisim",
 .4,
 pdf_title,
 c1);

 DrawFractionalError(
 matrix_map,
  cov_mat_keys_detVar,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "detVar_total",
 .45,
 pdf_title,
 c1);
 
 
 DrawFractionalError(
 matrix_map,
 cov_mat_key_totalsumcross,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "xsec_total",
 .45,
 pdf_title,
 c1);


  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////

 }
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////


  sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
  c1 -> Print(pdf_title);


}
//////////////////////////////////////////////////////////////////////////////
/// NEW FUNCTION  
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_NODATAPOINTS() {

   std::cout<<"Starting tutorial_slice_plots"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
   fpm.load_file_properties( "nuwro_file_properties_Tuples_5_13_2024.txt" );
    std::cout<<" passed "<< std::endl;
  #endif

 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
 
 std::cout<<"Starting MCC9SystematicsCalculator"<< std::endl;

 
 
  //UnivMake_FakeData_BDTdecided_1D_v5_pmucorrection.root
  auto* syst_ptr = new MCC9SystematicsCalculator(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_FakeData_ProtonNTrack.root",
    "systcalc_noNuWro.conf" );
  auto& syst = *syst_ptr;
 //UnivMake_topological_score_v3_realData.root
 //UnivMake_topological_score.root
 //UnivMake_FakeData_BDTdecided_1D_v11_addBogustosideband_noBDTproton_pmucorrection.root
 //UnivMake_NTracks.root
 //UnivMake_FakeData_BDTdecided_1D_v11_addBogustosideband_noBDTproton_pmucorrection
 std::cout<<"Finished  MCC9SystematicsCalculator "<< std::endl; 

  bool Plot_EXT = false;


  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
   reco_bnb_hist->Add(reco_ext_hist,-1);
  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;

  #ifdef USE_FAKE_DATA
    // Add the EXT to the "data" when working with fake data
    //reco_bnb_hist->Add( reco_ext_hist ); Just remove this 
  #endif

  TH2D* category_hist = syst.cv_universe().hist_categ_.get();


  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );


 if(Plot_EXT == false){
   int nBins = reco_mc_plus_ext_hist->GetNbinsX();
     for (int i = 1; i <= nBins; ++i) {
         reco_mc_plus_ext_hist->SetBinContent(i, 0.0);
     }
 }

  // Add in the CV MC prediction
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  auto* sb_ptr = new SliceBinning( "config_files_binning/mybins_mcc8_1D_NTracks.txt" ); //tutorial_reco_slice_config.txt //mybins_mcc8_1D_Topological_Score.txt  mybins_mcc8_1D.txt /mybins_mcc8_1D_NTracks.txt mybins_all.txt
  auto& sb = *sb_ptr;
  ////////////////////////////////
  // Starting to plot
  //////////////////////////////////

  std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name_1D_NODATAPOINTS.c_str());
  c1 -> Print(pdf_title);
 /*
   std::vector<std::string> xaxistitles_vector{
   "Cos#theta_{#mu}",
   "p_{#mu} [GeV/c]",
   "Bin N","Bin N" , 
   "p_{#mu} [GeV/c]",
   "p_{#mu} [GeV/c]", 
   "p_{p} [GeV/c]",
   "p_{#mu} [GeV/c]"
 };
 */
 
 // std::vector<std::string> xaxistitles_vector{
 //  "cos#theta_{#mu}",
 //    "p_{#mu} [GeV/c]",
 //    "Bin N"
 //};

 std::vector<std::string> xaxistitles_vector{
  "Proton Multiplicity",
    "Bin N"
 };
 
  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';



    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "Data", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "Total MC", "le" );

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }


   if(Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->Draw( "e" );
    std::string xAxisTitle = slice_bnb->hist_->GetXaxis()->GetTitle();
    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);
      sprintf(pdf_title, "%s.pdf", Pdf_name_1D_NODATAPOINTS.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    
      sprintf(axisXtitle, "%s", xAxisTitle.c_str());
    
    DrawStack_noData(
      category_hist,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      Pdf_name_1D_NODATAPOINTS,
      "Test",
      axisXtitle,
      c1,
      ymax,
      Plot_EXT);

    c1->Clear();   
    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    /*
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats", "xsec_NormCCCOH"
    };
  */
  // const std::vector< std::string > cov_mat_keys = {   
  //      "xsec_total",
  //      "xsec_multi",
  //      "xsec_unisim",
  //      "xsec_xsr_scc_Fa3_SCC",
  //      "xsec_xsr_scc_Fv3_SCC",
  //       "POT","MCstats",
  //       "EXTstats", 
  //       "BNBstats"
  //    };
  
  /* 
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
                                         "xsec_RPA_CCQE ",
                                         "xsec_ThetaDelta2NRad",
                                         "xsec_Theta_Delta2Npi",
                                         "xsec_VecFFCCQEshape",
                                         "xsec_XSecShape_CCMEC" };
   
   
   
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
       "xsec_xsr_scc_Fv3_SCC",
       "NuWroGenie"};
     */
    
  
  //detVar_total flux reint xsec_total POT numTargets
  //MCstats EXTstats BNBstats
  
  
  //  const std::vector< std::string > cov_mat_keys = { "xsec_unisim",
  //    "xsec_AxFFCCQEshape", "xsec_DecayAngMEC", "xsec_XSecShape_CCMEC", 
  //    "xsec_NormCCCOH",  "xsec_NormNCCOH", "xsec_RPA_CCQE",
  //     "xsec_ThetaDelta2NRad",  "xsec_Theta_Delta2Npi",
  //     "MCstats", "EXTstats", "BNBstats"
  //  };

   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color;
      if ( color >= 10 ) color += 10;
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend( 0.45, 0.7, 0.8, 0.88 );
    lg2->SetNColumns(2);
    lg2->SetBorderSize(0);
    lg2->SetTextSize(.025); //
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
    //auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    //frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(0.45);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle( "" );
    total_frac_err_hist->GetYaxis()->SetLabelSize(.02);
    total_frac_err_hist->GetXaxis()->SetLabelSize(.025);
    total_frac_err_hist->GetXaxis()->SetTitleSize(0.035);
    total_frac_err_hist->GetXaxis()->CenterTitle(kFALSE);
    total_frac_err_hist->SetTitleOffset (1.01,"Y");
    
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
      << hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    std::cout << "Total frac error in bin #1 = "
      << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);

 DrawFractionalError(
 matrix_map,
  cov_mat_keys,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "total",
 .4,
 pdf_title,
 c1);
 
 
 DrawFractionalError(
 matrix_map,
  cov_mat_keys_cross,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "xsec_unisim",
 .1,
 pdf_title,
 c1);

 DrawFractionalError(
 matrix_map,
  cov_mat_keys_detVar,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "detVar_total",
 .2,
 pdf_title,
 c1);
 
 
 DrawFractionalError(
 matrix_map,
 cov_mat_key_totalsumcross,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "xsec_total",
 .3,
 pdf_title,
 c1);


  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////

 }
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////


  sprintf(pdf_title, "%s.pdf)", Pdf_name_1D_NODATAPOINTS.c_str());
  c1 -> Print(pdf_title);


}
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_withData() {

   std::cout<<"Starting tutorial_slice_plots"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
   
    std::cout<<" passed "<< std::endl;
  #endif

 //fpm.load_file_properties( "file_properties_Open_v2.txt" );
 fpm.load_file_properties( "file_properties_May10_OpenData.txt" );
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
 
 std::cout<<"Starting MCC9SystematicsCalculator"<< std::endl;

 
 
  //UnivMake_FakeData_BDTdecided_1D_v5_pmucorrection.root
  auto* syst_ptr = new MCC9SystematicsCalculator(
  "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_Ntracks_v2.root",
  "systcalc_noNuWro_eventrates.conf"
  
  
    /*"/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_8_16_2024/unimake_1D.root",
    "systcalc_noNuWro_eventrates.conf"*/ );
  auto& syst = *syst_ptr;
  //NTracks_contained_v2.root
 //
 //UnivMake_topological_score_v3_realData.root
 //UnivMake_topological_score.root
 //UnivMake_FakeData_BDTdecided_1D_v11_addBogustosideband_noBDTproton_pmucorrection.root
 //UnivMake_NTracks.root
 //UnivMake_FakeData_BDTdecided_1D_v11_addBogustosideband_noBDTproton_pmucorrection
 std::cout<<"Finished  MCC9SystematicsCalculator "<< std::endl; 

  bool Plot_EXT = true;


  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
   
  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;

  #ifdef USE_FAKE_DATA
    // Add the EXT to the "data" when working with fake data
    //reco_bnb_hist->Add( reco_ext_hist ); Just remove this 
  #endif

  TH2D* category_hist = syst.cv_universe().hist_categ_.get();


  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );


 if(Plot_EXT == false){
   int nBins = reco_mc_plus_ext_hist->GetNbinsX();
     for (int i = 1; i <= nBins; ++i) {
         reco_mc_plus_ext_hist->SetBinContent(i, 0.0);
     }
 }

  // Add in the CV MC prediction
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  auto& matrix_map = *matrix_map_ptr;

  //auto* sb_ptr = new SliceBinning( "mybins_Muon_1D_Pmu_v3_slice_config.txt" ); //tutorial_reco_slice_config.txt //mybins_mcc8_1D_Topological_Score.txt mybins_mcc8_1D_TrackScore.txt  mybins_mcc8_1D.txt /mybins_mcc8_1D_NTracks.txt mybins_all.txt
  auto* sb_ptr = new SliceBinning( "mybins_mcc8_1D_NTracks.txt" );
  auto& sb = *sb_ptr;
  ////////////////////////////////
  // Starting to plot
  //////////////////////////////////

  std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name_withData.c_str());
  c1 -> Print(pdf_title);
 /*
   std::vector<std::string> xaxistitles_vector{
   "Cos#theta_{#mu}",
   "p_{#mu} [GeV/c]",
   "Bin N","Bin N" , 
   "p_{#mu} [GeV/c]",
   "p_{#mu} [GeV/c]", 
   "p_{p} [GeV/c]",
   "p_{#mu} [GeV/c]"
 };
 */
 
 // std::vector<std::string> xaxistitles_vector{
 //  "cos#theta_{#mu}",
 //    "p_{#mu} [GeV/c]",
 //    "Bin N"
 //};

 std::vector<std::string> xaxistitles_vector{
  "Proton Multiplicity",
    "Bin N"
 };
 
  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';



    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "Open Data", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "Total MC", "le" );

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }


     lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->Draw( "e" );
    std::string xAxisTitle = slice_bnb->hist_->GetXaxis()->GetTitle();
    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);
      sprintf(pdf_title, "%s.pdf",  Pdf_name_withData.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    
      sprintf(axisXtitle, "%s", xAxisTitle.c_str());
    
    
   //h_bnb_input
          double chi1,pvalue;
      int NDF;

    
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
       Pdf_name_withData,
      "Test",
      axisXtitle,
      c1,
      ymax,
      Plot_EXT ,
      chi1,
      pvalue,
      NDF);
    






    c1->Clear();   
    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    /*
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats", "xsec_NormCCCOH"
    };
  */
  // const std::vector< std::string > cov_mat_keys = {   
  //      "xsec_total",
  //      "xsec_multi",
  //      "xsec_unisim",
  //      "xsec_xsr_scc_Fa3_SCC",
  //      "xsec_xsr_scc_Fv3_SCC",
  //       "POT","MCstats",
  //       "EXTstats", 
  //       "BNBstats"
  //    };
  
  /* 
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
                                         "xsec_RPA_CCQE ",
                                         "xsec_ThetaDelta2NRad",
                                         "xsec_Theta_Delta2Npi",
                                         "xsec_VecFFCCQEshape",
                                         "xsec_XSecShape_CCMEC" };
   
   
   
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
       "xsec_xsr_scc_Fv3_SCC",
       "NuWroGenie"};
     */
    
  
  //detVar_total flux reint xsec_total POT numTargets
  //MCstats EXTstats BNBstats
  
  
  //  const std::vector< std::string > cov_mat_keys = { "xsec_unisim",
  //    "xsec_AxFFCCQEshape", "xsec_DecayAngMEC", "xsec_XSecShape_CCMEC", 
  //    "xsec_NormCCCOH",  "xsec_NormNCCOH", "xsec_RPA_CCQE",
  //     "xsec_ThetaDelta2NRad",  "xsec_Theta_Delta2Npi",
  //     "MCstats", "EXTstats", "BNBstats"
  //  };

   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();
         ++color;
        if ( color == 3 ) color = kGreen +2;
        if ( color == kGreen + 3) color = 4;
        if ( color == 5 ) color = 6;
        if ( color == 7 ) color = kYellow -2;
        if ( color == kYellow -1 ) color = kMagenta + 2;
        if (color == kMagenta + 3) color = kSpring - 2;
        if (color == kSpring - 1) color = kOrange + 2;
        if (color == kOrange + 3) color = kRed - 6;
        if (color == kRed - 5) color = kAzure + 8;
        if (color == kAzure + 8) color = kOrange - 3;
      
      
      
      
      
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend( 0.3, 0.65, 0.8, 0.8 );
    lg2->SetNColumns(3);
    lg2->SetBorderSize(0);
    lg2->SetTextSize(.025); //
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
    //auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    //frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(0.55);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle( "" );
    total_frac_err_hist->GetYaxis()->SetLabelSize(.02);
    total_frac_err_hist->GetXaxis()->SetLabelSize(.025);
    total_frac_err_hist->GetXaxis()->SetTitleSize(0.035);
    total_frac_err_hist->GetXaxis()->CenterTitle(kFALSE);
    total_frac_err_hist->SetTitleOffset (1.01,"Y");
    
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
      << hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    std::cout << "Total frac error in bin #1 = "
      << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf",  Pdf_name_withData.c_str());
      c1 -> Print(pdf_title);

 DrawFractionalError(
 matrix_map,
  cov_mat_keys,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "total",
 .45,
 pdf_title,
 c1);
 
 
 DrawFractionalError(
 matrix_map,
  cov_mat_keys_cross,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "xsec_unisim",
 .35,
 pdf_title,
 c1);

 DrawFractionalError(
 matrix_map,
  cov_mat_keys_detVar,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "detVar_total",
 .15,
 pdf_title,
 c1);
 
 
 DrawFractionalError(
 matrix_map,
 cov_mat_key_totalsumcross,
 reco_mc_plus_ext_hist,
 slice,
 axisXtitle,
 "xsec_total",
 .3,
 pdf_title,
 c1);


  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////

 }
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////


  sprintf(pdf_title, "%s.pdf)",  Pdf_name_withData.c_str());
  c1 -> Print(pdf_title);


}
//////////////////////////////////////////////////////////////////////////////
/// NEW FUNCTION  
//////////////////////////////////////////////////////////////////////////////

void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax,
  bool Plot_EXT )
{
  const auto& EventInterp = EventCategoryInterpreter::Instance();
  TLegend* lg1_stacked = new TLegend(0.35, 0.65, 0.8, 0.85 );
  lg1_stacked->SetNColumns(2);
  lg1_stacked->SetBorderSize(0);
  Double_t defaultTextSize = lg1_stacked->GetTextSize();
  lg1_stacked->SetTextSize(.02); //
  std::string MicroBooneTitle = MicroBooNE_LegendTitle();
  lg1_stacked->SetHeader(MicroBooneTitle.c_str());
  char leg_title[1024];


  TH2D* category_hist = (TH2D*)category_hist_input->Clone("category_hist");
  TH1D* h_Data =(TH1D*)slice_bnb->hist_.get()->Clone("h_Data");
  lg1_stacked->AddEntry(h_Data, "Data", "pe" );
    
  TH1D* h_Total_MC =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone("h_Total_MC");
  TH1D* h_Total_MC_clone =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone("h_Total_MC_clone");
  lg1_stacked->AddEntry(h_Total_MC, "Total MC", "le" );

  TH1D* h_extBNB =(TH1D*)slice_ext->hist_.get()->Clone("h_extBNB");
  EventInterp.set_ext_histogram_style( h_extBNB );
  if(Plot_EXT == true) h_Total_MC_clone->Add(h_extBNB);
  double Total_Area_MC = h_Total_MC_clone->Integral();
  EventInterp.set_ext_histogram_style( slice_ext->hist_.get() );
 //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );

  THStack* slice_pred_stack = new THStack( "mc+ext", "" );
  if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB

  const auto& cat_map = EventInterp.label_map();
  int cat_bin_index = cat_map.size();

  for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
  {
    EventCategory cat = iter->first;
    TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
      cat_bin_index, cat_bin_index );
    temp_mc_hist->SetDirectory( nullptr );
    
    SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
      *temp_mc_hist, slice  );

    EventInterp.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

    slice_pred_stack->Add( temp_slice_mc->hist_.get() );
    std::string lg1_label = EventInterp.label(cat);
    
    double FractionArea = (temp_slice_mc->hist_.get()->Integral() / Total_Area_MC) * 100 ;
    sprintf(leg_title, " %s (%2.1f %)",lg1_label.c_str(), FractionArea );
    lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),leg_title , "f" );
    //std::string cat_col_prefix = "MC" + std::to_string( cat );

    --cat_bin_index;
  }

 bool makeNormWidth = true; 
   h_Data->SetTitle("");



  DrawStackMCandData(
    h_Data, 
    h_Total_MC,
    slice_pred_stack,
    "NEvents",
    Xaxis_title,  
    !makeNormWidth, 
    "title",
    99,
    c1);

    DrawFakeData();
    
    
    double FractionArea_BNB = (h_extBNB->Integral() / Total_Area_MC) * 100 ;
    
    if(Plot_EXT == false) FractionArea_BNB = 0;
    
    sprintf(leg_title, " %s (%2.1f %)",  "EXT BNB", FractionArea_BNB );
  
   if(Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),leg_title , "f" );
   lg1_stacked->Draw( "same" );
  //  std::cout<<"printing chi values "<< std::endl;
  TLatex* text = new TLatex;
    text->SetNDC();
    text->SetTextSize(0.03);
    text->SetTextColor(kRed);
   //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
    //text->DrawLatex(0.15, 0.85, textplace);

    //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    //text->DrawLatex(0.15, 0.80, textplace);
    //AddHistoTitle(TotalTitle.c_str(), .035);
  ////////////////////////////////////////
  ///// Finished with First Plot
  ////////////////////////////////////////

  sprintf(pdf_title, "%s.pdf", pdfTitle.c_str());
  c1 -> Print(pdf_title);


  DrawStackMCandData(
    h_Data, 
    h_Total_MC,
    slice_pred_stack,
    "NEvents / Bin Width",
    Xaxis_title,
    makeNormWidth, 
    "title",
    99,
    c1);
   
    lg1_stacked->Draw( "same" );
   DrawFakeData();
    //AddHistoTitle(TotalTitle.c_str(), .035);
  c1 -> Print(pdf_title);

}

  
//////////////////////////////////////////////////////////////////////////////
///  new 
//////////////////////////////////////////////////////////////////////////////
void DrawStack_WithRatio(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax,
  bool Plot_EXT,
  double &totalChi2,
  double &Totalpvalue,
  int &NDF,
  double SliceBinWidth)
{
    bool makeNormWidth = true;
  const auto& EventInterp = EventCategoryInterpreter::Instance();
  TLegend* lg1_stacked = new TLegend(0.28, 0.38, 0.92, 0.89 );
  std::string MicroBooneTitle = MicroBooNE_LegendTitle_OpenData();
	lg1_stacked->SetHeader(MicroBooneTitle.c_str());
  lg1_stacked->SetNColumns(3);
  lg1_stacked->SetBorderSize(0);
  //lg1_stacked->SetTextColor(0.3);
  
  //Double_t defaultTextSize = lg1_stacked->GetTextSize();
  lg1_stacked->SetTextSize(.035); //
  //std::string MicroBooneTitle = MicroBooNE_LegendTitle();
  //lg1_stacked->SetHeader(MicroBooneTitle.c_str());
  lg1_stacked->SetTextFont(132);
  char leg_title[1024];


  TH2D* category_hist = (TH2D*)category_hist_input->Clone("category_hist");
  TH1D* h_Data =(TH1D*)slice_bnb->hist_.get()->Clone(uniq());
  TH1D* h_Data_RATIO =(TH1D*)slice_bnb->hist_.get()->Clone(uniq());
    h_Data_RATIO->Sumw2();
  if(makeNormWidth)h_Data_RATIO->Scale(1.0/SliceBinWidth,"width");
  TH1D* h_Data_RATIO2 =(TH1D*)slice_bnb->hist_.get()->Clone(uniq());
  
  if(Plot_EXT==false) lg1_stacked->AddEntry(h_Data, "Fake Data [Stat]", "pe" );
  else if(Plot_EXT==true) lg1_stacked->AddEntry(h_Data, "Open Data [Stat]", "pe" );
    
    
  TH1D* h_Total_MC =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
   TH1D* h_Total_MC_cloneRatio =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
   h_Total_MC_cloneRatio->Sumw2();
  TH1D* h_Total_MC_clone =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
  //h_Total_MC_cloneRatio->Divide(h_Data_RATIO2);
  
    //if(Plot_EXT == true) h_Total_MC_clone->Add(h_extBNB);
  double Total_Area_MC = h_Total_MC_clone->Integral();
  
  if(makeNormWidth)h_Total_MC_clone->Scale(1.0/SliceBinWidth,"width");
  h_Data_RATIO->Divide(h_Total_MC_clone);
  //TH1D* h_Total_MC_clone2 = (TH1D*)h_Total_MC->Clone(uniq()); 
  //h_Total_MC_clone2->SetLineColorAlpha(kBlack, .8);
  //h_Total_MC_clone2->SetLineStyle(2); // Dashed line for upper bound
  //h_Total_MC_clone2->SetLineWidth( 4 );
  lg1_stacked->AddEntry(h_Total_MC, "#muBooNE Tune [Sys+Stats]", "l" );

  TH1D* h_extBNB =(TH1D*)slice_ext->hist_.get()->Clone(uniq());
  EventInterp.set_ext_histogram_style( h_extBNB );

  EventInterp.set_ext_histogram_style( slice_ext->hist_.get() );
  //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );

  THStack* slice_pred_stack = new THStack( "mc+ext", "" );
  if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB

  const auto& cat_map = EventInterp.label_map();
  int cat_bin_index = cat_map.size();

  for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
  {
    EventCategory cat = iter->first;
    TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
      cat_bin_index, cat_bin_index );
    temp_mc_hist->SetDirectory( nullptr );
    
    SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
      *temp_mc_hist, slice  );

    EventInterp.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

    slice_pred_stack->Add( temp_slice_mc->hist_.get() );
    std::string lg1_label = EventInterp.label(cat);
    
    double FractionArea = (temp_slice_mc->hist_.get()->Integral() / Total_Area_MC) * 100 ;
    sprintf(leg_title, " %s (%2.1f%)",lg1_label.c_str(), FractionArea );
    lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),leg_title , "f" );
    //std::string cat_col_prefix = "MC" + std::to_string( cat );

    --cat_bin_index;
  }

 //gPad->SetTicks(1, 1);
 gPad->RedrawAxis();
  
   

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0); //  old 0, 0.2, 1, 1.0
    pad1->SetBottomMargin(.0); // Upper and lower plot are joined
    pad1->SetRightMargin(.02);
    pad1->SetLeftMargin(.08);
  //pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();  
   /////////////
   //// REmove Binning title 
   /////////
  //h_Data->SetTitle("");

  h_Data->GetYaxis()->SetLabelSize(.08);

  //h_Data->GetYaxis()->SetRangeUser( 0., 1 );

  DrawStackMCandData(
    h_Data, 
    h_Total_MC,
    slice_pred_stack,
    "NEvents (#times 10^{3}) / Bin Width",
   "",  
    makeNormWidth, 
    "title",
    99,
    c1,
    SliceBinWidth,
    .001);

  if(Plot_EXT==false) DrawFakeData_GENIE(); //DrawFakeData();
    
    gPad->RedrawAxis();
    
    //DrawFakeData_GENIE();
    
    
    double FractionArea_BNB = (h_extBNB->Integral() / Total_Area_MC) * 100 ;
    
    if(Plot_EXT == false) FractionArea_BNB = 0;
    
    sprintf(leg_title, " %s (%2.1f%)",  "EXT BNB", FractionArea_BNB );
   if(Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),leg_title , "f" );
   
   lg1_stacked->Draw( "same" );
  //  std::cout<<"printing chi values "<< std::endl;
  TLatex* text = new TLatex;
    text->SetNDC();
    text->SetTextSize(0.04);
    text->SetTextColor(kRed);
    
    if(!TrueOffChisquare==true){
    auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
    text->DrawLatex(0.1, 0.75, textplace);
    sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    text->DrawLatex(0.1, 0.70, textplace);
    
    totalChi2 = chi2_result.chi2_;
    Totalpvalue = chi2_result.p_value_;
    NDF = chi2_result.num_bins_;
    
    
    
    }
    else {
    totalChi2 = -999;
    Totalpvalue = -999;
    NDF = -999;
    }

   // AddCutArrow(
   //    .15,
   //    0,
   //    80000,
   //    .05,
   //    false);

    
   //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
   // text->DrawLatex(0.15, 0.85, textplace);

    //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    //text->DrawLatex(0.15, 0.80, textplace);
    //AddHistoTitle(TotalTitle.c_str(), .035);
  ////////////////////////////////////////
  ///// Finished with First Plot
  ////////////////////////////////////////
    c1->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.3);
    pad2->SetTopMargin(.0);
    pad2->SetRightMargin(.02);
    pad2->SetLeftMargin(.08);
    pad2->SetBottomMargin(0.3);
    pad2->SetGridx(); // vertical grid
    //pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();
    //h_Data_RATIO->GetXaxis()->CenterTitle();
     h_Data_RATIO->GetXaxis()->SetTitle(Xaxis_title);
     h_Data_RATIO->GetYaxis()->SetTitle("Data/MC");
     //h_Data_RATIO->GetXaxis()->SetTitle("cos#theta_{#mu}");
     h_Data_RATIO->GetYaxis()->SetLabelSize(.15);
     h_Data_RATIO->GetXaxis()->SetLabelSize(.15);
     h_Data_RATIO->GetXaxis()->SetTitleSize(0.14);
     h_Data_RATIO->GetYaxis()->SetTitleSize(0.15);
     h_Data_RATIO->SetTitle("");

    
    // Optional: Adjust the title size if needed
    

    h_Total_MC_cloneRatio->SetFillColorAlpha(kRed, 0.35);
    h_Total_MC_cloneRatio->SetFillStyle(3001);
    h_Total_MC_cloneRatio->SetMarkerStyle(0);


  //h_Total_MC->SetFillColor(0);
  //h_Total_MC->SetLineColor(2);
  //h_Total_MC->SetLineStyle(1);
  //h_Total_MC->SetLineWidth(2);
     h_Data_RATIO->GetYaxis()->SetNdivisions(503);
     h_Data_RATIO->GetXaxis()->SetNdivisions(510);
     //RatioHist["DATA"]->GetXaxis()->CenterTitle();
     h_Data_RATIO->GetYaxis()->CenterTitle();
     //h_Data_RATIO->GetXaxis()->CenterTitle();
     h_Data_RATIO->GetYaxis()->SetTitleOffset(.2);
     h_Data_RATIO->GetXaxis()->SetTitleOffset(.95);
     h_Data_RATIO->SetMinimum(.2);
     h_Data_RATIO->SetMaximum(2.1);
     h_Data_RATIO->Draw("e");
     h_Total_MC_cloneRatio->Divide(h_Total_MC_cloneRatio);
     h_Total_MC_cloneRatio->GetXaxis()->SetTitle("");
     h_Total_MC_cloneRatio->Draw("SAME E2");
    //h_Total_MC_cloneRatio->Draw("SAME AXIS");
     TLine* line = new TLine(h_Data_RATIO->GetXaxis()->GetXmin(), 1.0, h_Data_RATIO->GetXaxis()->GetXmax(), 1.0);
    line->SetLineColor(kBlack); // Set line color to black
    line->SetLineStyle(2);      // Set line style to dashed

    // Draw the line on the canvas
    line->Draw("same");
    h_Data_RATIO->Draw("e same");

  sprintf(pdf_title, "%s.pdf", pdfTitle.c_str());
  c1 -> Print(pdf_title);

  c1 ->Clear(); 
 /**
  DrawStackMCandData(
    h_Data, 
    h_Total_MC,
    slice_pred_stack,
    "NEvents / Bin Width",
    Xaxis_title,
    makeNormWidth, 
    "title",
    99,
    c1);
   
    lg1_stacked->Draw( "same" );
   DrawFakeData();
    //AddHistoTitle(TotalTitle.c_str(), .035);
  c1 -> Print(pdf_title);
  */


}
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  bool makeNormWidth,
  double ymax,
  bool Plot_EXT)
{
  const auto& EventInterp = EventCategoryInterpreter::Instance();

  TH2D* category_hist = (TH2D*)category_hist_input->Clone("category_hist");
  TH1D* h_Data =(TH1D*)slice_bnb->hist_.get()->Clone("h_Data");
    
  TH1D* h_Total_MC =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone("h_Total_MC");
  TH1D* h_Total_MC_clone =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone("h_Total_MC");
  

  TH1D* h_extBNB =(TH1D*)slice_ext->hist_.get()->Clone("h_extBNB");
  EventInterp.set_ext_histogram_style( h_extBNB );
  if(Plot_EXT == true) h_Total_MC_clone->Add(h_extBNB);
  double Total_Area_MC = h_Total_MC_clone->Integral();
  EventInterp.set_ext_histogram_style( slice_ext->hist_.get() );
  //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );

  THStack* slice_pred_stack = new THStack( "mc+ext", "" );
  slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB

  const auto& cat_map = EventInterp.label_map();
  int cat_bin_index = cat_map.size();

  for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
  {
    EventCategory cat = iter->first;
    TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
      cat_bin_index, cat_bin_index );
    temp_mc_hist->SetDirectory( nullptr );
    
    SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
      *temp_mc_hist, slice  );

    EventInterp.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

    slice_pred_stack->Add( temp_slice_mc->hist_.get() );

    
    //std::string cat_col_prefix = "MC" + std::to_string( cat );

    --cat_bin_index;
  }


   h_Data->SetTitle("");
       
  DrawStackMCandData(
  h_Data, 
    h_Total_MC,
    slice_pred_stack,
    makeNormWidth, 
    ymax);     
       
       

  //  std::cout<<"printing chi values "<< std::endl;
  //TLatex* text = new TLatex;
  //  text->SetNDC();
  //  text->SetTextSize(0.03);
  //  text->SetTextColor(kRed);
   // sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
    //text->DrawLatex(0.15, 0.85, textplace);

    //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    //text->DrawLatex(0.15, 0.80, textplace);
    //AddHistoTitle(TotalTitle.c_str(), .035);
  ////////////////////////////////////////
  ///// Finished with First Plot
  ////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  bool makeNormWidth,
  double SliceBinWidth,
  double ymax,
  double WindowZoomscale,
  bool Plot_EXT,
  bool Scaleall, 
  double Scaleall_input)
{
  const auto& EventInterp = EventCategoryInterpreter::Instance();

  TH2D* category_hist = (TH2D*)category_hist_input->Clone(uniq());
  TH1D* h_Data =(TH1D*)slice_bnb->hist_.get()->Clone(uniq());
    
  TH1D* h_Total_MC =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
  TH1D* h_Total_MC_clone =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
  

  TH1D* h_extBNB =(TH1D*)slice_ext->hist_.get()->Clone(uniq());
  EventInterp.set_ext_histogram_style( h_extBNB );
  if(Plot_EXT==true) h_Total_MC_clone->Add(h_extBNB);
  double Total_Area_MC = h_Total_MC_clone->Integral();
  EventInterp.set_ext_histogram_style( slice_ext->hist_.get() );
  //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );

  THStack* slice_pred_stack = new THStack(uniq(), "" );
  if(Plot_EXT==true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB

  const auto& cat_map = EventInterp.label_map();
  int cat_bin_index = cat_map.size();

  for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
  {
    EventCategory cat = iter->first;
    TH1D* temp_mc_hist = category_hist->ProjectionY( uniq(),
      cat_bin_index, cat_bin_index );
    temp_mc_hist->SetDirectory( nullptr );
    
    SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
      *temp_mc_hist, slice  );

    EventInterp.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

    slice_pred_stack->Add( temp_slice_mc->hist_.get() );

    
    //std::string cat_col_prefix = "MC" + std::to_string( cat );

    --cat_bin_index;
  }


   h_Data->SetTitle("");
std::cout<<"SliceBinWidth = "<< SliceBinWidth << std::endl;
  ///////////////////////
  // Zoom in on figure 
  /////////////////////
if(makeNormWidth){
  //h_Data->GetYaxis()->SetNoExponent(kFALSE);
   h_Data->Scale(1.0/SliceBinWidth,"width");
   h_Total_MC->Scale(1.0/SliceBinWidth,"width");
   h_extBNB->Scale(1.0/SliceBinWidth,"width");
   BinWidthNormTHStack(slice_pred_stack, SliceBinWidth);
}

if(Scaleall){

   h_Data->Scale(Scaleall_input);
   h_Total_MC->Scale(Scaleall_input);
   h_extBNB->Scale(Scaleall_input);
   scaleTHStack(slice_pred_stack, Scaleall_input);
}





   h_Data->Scale(WindowZoomscale);
   h_Total_MC->Scale(WindowZoomscale);
   h_extBNB->Scale(WindowZoomscale);
   scaleTHStack(slice_pred_stack, WindowZoomscale);
            
  h_Data->GetYaxis()->SetNdivisions(503);
  h_Data->GetXaxis()->SetNdivisions(505);
       
       
  DrawStackMCandData_withBand(
    h_Data, 
    h_Total_MC,
    h_extBNB,
    slice_pred_stack,
    false, 
    ymax);     
       
       

  //  std::cout<<"printing chi values "<< std::endl;
  //TLatex* text = new TLatex;
  //  text->SetNDC();
  //  text->SetTextSize(0.03);
  //  text->SetTextColor(kRed);
   // sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
    //text->DrawLatex(0.15, 0.85, textplace);

    //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    //text->DrawLatex(0.15, 0.80, textplace);
    //AddHistoTitle(TotalTitle.c_str(), .035);
  ////////////////////////////////////////
  ///// Finished with First Plot
  ////////////////////////////////////////
}
///////////// new function 
void DrawStack_noData(
  TH2D* category_hist_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  char *Xaxis_title,
  TCanvas *c1,
  double ymax,
  bool Plot_EXT )
{
    double shade = .35;
    int color = kRed;

  const auto& EventInterp = EventCategoryInterpreter::Instance();
  TLegend* lg1_stacked = new TLegend(0.4, 0.28, 0.88, 0.86 );
  lg1_stacked->SetNColumns(2);
  lg1_stacked->SetBorderSize(0);
  Double_t defaultTextSize = lg1_stacked->GetTextSize();
  lg1_stacked->SetTextSize(.02); //
  //std::string MicroBooneTitle = MicroBooNE_LegendTitle();
  //lg1_stacked->SetHeader(MicroBooneTitle.c_str());
  char leg_title[1024];


  TH2D* category_hist = (TH2D*)category_hist_input->Clone("category_hist");
  TH1D* h_Data =(TH1D*)slice_bnb->hist_.get()->Clone("h_Data");
  //lg1_stacked->AddEntry(h_Data, "Data", "pe" );
    
  TH1D* h_Total_MC =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone("h_Total_MC");
  
  h_Total_MC->SetFillColorAlpha(color, shade);
  h_Total_MC->SetFillStyle(3001);
  h_Total_MC->SetMarkerStyle(0);
  h_Total_MC->DrawCopy("SAME E2");
  h_Total_MC->Draw("SAME AXIS");
  
  TH1D* h_Total_MC_clone =(TH1D*)slice_mc_plus_ext->hist_.get()->Clone("h_Total_MC_clone");
  lg1_stacked->AddEntry(h_Total_MC, "Total MC [Stat+Sys]", "lf" );

  TH1D* h_extBNB =(TH1D*)slice_ext->hist_.get()->Clone("h_extBNB");
  EventInterp.set_ext_histogram_style( h_extBNB );
  if(Plot_EXT == true) h_Total_MC_clone->Add(h_extBNB);
  double Total_Area_MC = h_Total_MC_clone->Integral();
  EventInterp.set_ext_histogram_style( slice_ext->hist_.get() );
 //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );

  THStack* slice_pred_stack = new THStack( "mc+ext", "" );
  if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB

  const auto& cat_map = EventInterp.label_map();
  int cat_bin_index = cat_map.size();

  for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
  {
    EventCategory cat = iter->first;
    TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
      cat_bin_index, cat_bin_index );
    temp_mc_hist->SetDirectory( nullptr );
    
    SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
      *temp_mc_hist, slice  );

    EventInterp.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

    slice_pred_stack->Add( temp_slice_mc->hist_.get() );
    std::string lg1_label = EventInterp.label(cat);
    
    double FractionArea = (temp_slice_mc->hist_.get()->Integral() / Total_Area_MC) * 100 ;
    sprintf(leg_title, " %s",lg1_label.c_str(), FractionArea );
    lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),leg_title , "f" );
    //std::string cat_col_prefix = "MC" + std::to_string( cat );

    --cat_bin_index;
  }

 bool makeNormWidth = true; 
   h_Data->SetTitle("");



  DrawStackMC(
    h_Total_MC,
    slice_pred_stack,
    "NEvents",
    Xaxis_title,  
    !makeNormWidth, 
    "title",
    99,
    shade,
    color);

   DrawFakeData_InProgess();
    
    
    double FractionArea_BNB = (h_extBNB->Integral() / Total_Area_MC) * 100 ;
    
    if(Plot_EXT == false) FractionArea_BNB = 0;
    
    sprintf(leg_title, " %s (%2.1f %)",  "EXT BNB", FractionArea_BNB );
  
   if(Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),leg_title , "f" );
   lg1_stacked->Draw( "same" );
  //  std::cout<<"printing chi values "<< std::endl;
  TLatex* text = new TLatex;
    text->SetNDC();
    text->SetTextSize(0.03);
    text->SetTextColor(kRed);
   //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
    //text->DrawLatex(0.15, 0.85, textplace);

    //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
    //text->DrawLatex(0.15, 0.80, textplace);
    //AddHistoTitle(TotalTitle.c_str(), .035);
  ////////////////////////////////////////
  ///// Finished with First Plot
  ////////////////////////////////////////

  sprintf(pdf_title, "%s.pdf", pdfTitle.c_str());
  c1 -> Print(pdf_title);


  DrawStackMC(
    h_Total_MC,
    slice_pred_stack,
    "NEvents / Bin Width",
    Xaxis_title,
    makeNormWidth, 
    "title",
    99,
    shade,
    color);
   
    lg1_stacked->Draw( "same" );
   DrawFakeData_InProgess();
    //AddHistoTitle(TotalTitle.c_str(), .035);
  c1 -> Print(pdf_title);

}

//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_2D()
{
   std::cout<<"Starting tutorial_slice_plots 2D"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
    fpm.load_file_properties( "nuwro_file_properties_Tuples_5_13_2024.txt" );
    std::cout<<" passed "<< std::endl;
  #endif

 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
 
 std::cout<<"Starting MCC9SystematicsCalculator"<< std::endl;

  auto* syst_ptr = new MCC9SystematicsCalculator(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_FakeData_2D_binningscheme1_pmucorrection_v3.root",
    "systcalc_noNuWro.conf" );
  auto& syst = *syst_ptr;
  //



   std::cout<<"Finished  MCC9SystematicsCalculator "<< std::endl; 
  //old 
  ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_v4_pmucorrection.root
  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  // Notes :: I think everything is projected by bin Number in 1D and the slicer will get it 
  bool Plot_EXT = false; 
  
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();

  reco_bnb_hist->Add(reco_ext_hist,-1);


  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;

  #ifdef USE_FAKE_DATA
    // Add the EXT to the "data" when working with fake data
    if(Plot_EXT == true) reco_bnb_hist->Add( reco_ext_hist );
  #endif

  TH2D* category_hist = syst.cv_universe().hist_categ_.get();

  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );

 // Zero out if not plotting 
 if(Plot_EXT == false){
   int nBins = reco_mc_plus_ext_hist->GetNbinsX();
     for (int i = 1; i <= nBins; ++i) {
         reco_mc_plus_ext_hist->SetBinContent(i, 0.0);
     }
 }
  // Add in the CV MC prediction
  
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  //auto& matrix_map = *matrix_map_ptr;

   CovMatrixMap &matrix_map = *matrix_map_ptr;


  auto* sb_ptr = new SliceBinning( "mybins_mcc9_2D_muon_new3.txt" ); //tutorial_reco_slice_config.txt
  auto& sb = *sb_ptr;
  ////////////////////////////////
  // Starting to plotte
  //////////////////////////////////

  //std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name2D.c_str());
  c1 -> Print(pdf_title);

  //std::vector<std::string> xaxistitles_vector{"Cos#theta_{#mu}","p_{#mu} [GeV/c]", "Bin N" };

  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


  
    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "NuWro Fake Data [Stat]", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "#muBooNE Tune [Sys+Stat]", "le" );

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }


   if(Plot_EXT == true)lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->GetYaxis()->SetLabelSize(.02);
    slice_bnb->hist_->Draw( "e" );

    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      if(sl_idx !=12){
      
      auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
      << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
      << " p-value = " << chi2_result.p_value_ << '\n';
      sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      text->DrawLatex(0.15, 0.85, textplace);

      sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      text->DrawLatex(0.15, 0.80, textplace);
      }
      
      
      sprintf(pdf_title, "%s.pdf", Pdf_name2D.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
   
    if(10 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else{sprintf(axisXtitle, "%s", "cos#theta_{#mu}");}
  
    
      double chi1,pvalue;
      int NDF;
    
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      Pdf_name2D,
      "Test",
      axisXtitle,
      c1,
      ymax,
      Plot_EXT, 
      chi1,
      pvalue,
      NDF);






    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    /*
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats", "xsec_NormCCCOH"
    };
   */


   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      
      
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color;
      if ( color >= 10 ) color += 10;
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend(0.3, 0.75, 0.82, 0.88 );
    lg2->SetNColumns(3);
    lg2->SetBorderSize(0);
    lg2->SetTextFont(132);
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   // std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
   // auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.45);
    total_frac_err_hist->SetMinimum(0.0);
    //total_frac_err_hist->SetMaximum(.5);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    if(9 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else if(10 ==sl_idx){  sprintf(axisXtitle, "%s", "p_{p} [GeV/c]");}
    else if(11 ==sl_idx){  sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
    else{sprintf(axisXtitle, "%s", "cos#theta_{#mu}");}
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Uncertainty" );
    //total_frac_err_hist->SetTitle(" ");
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "Total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;
      if(name == "EXTstats" && Plot_EXT == false) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
      << hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    std::cout << "Total frac error in bin #1 = "
      << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);


  } 
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////
 std::vector<size_t> SliceBins{0,1,2,3,4,5,6,7,8};
 auto BinVector = GetProjectBinVector();
 
 auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES, "p_{#mu} [GeV/c]");
 
 ///////////////////////////////////////////////////
 /// Making Grid Figure here 
 ///////////////////////////////////////////////////
 std::vector<double> WindowZoomScale{2,4,4,2,1,1,1,4,5};


 double min_XAxis_GridCanvas = -1.0;
 double max_XAxis_GridCanvas = 1.0;
 
 double min_YAxis_GridCanvas = 0.0;
 double max_YAxis_GridCanvas = 7500.0;
 
 GridCanvas *GC_Stack = new GridCanvas(uniq(), 3, 4, 800, 550);
 GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);

   GC_Stack->SetBottomMargin(.00);
   GC_Stack->SetTopMargin(.02);
   GC_Stack->SetRightMargin(.05);

   Stack_FracError->SetBottomMargin(.00);
   Stack_FracError->SetTopMargin(.02);
   Stack_FracError->SetRightMargin(.05);




    TLegend* lg1_Grid= new TLegend( 0.41, 0.08, 0.96, 0.2 );
    lg1_Grid->SetNColumns(2);
    lg1_Grid->SetBorderSize(0);
    //lg1_Grid->SetTextSize(defaultTextSize*1.05); //

  for ( auto sl_idx : SliceBins  )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;
    int GridBins = sl_idx + 1; 
    
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();
      
      if(sl_idx==0){
          lg1_Grid->AddEntry(slice_bnb->hist_.get(), "NuWro Fake Data", "pe" );
          auto histclone = (TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
          
           histclone->SetFillColorAlpha(kRed, 0.8);
				   histclone->SetFillStyle(3001);
				   histclone->SetMarkerStyle(0);
				   histclone->SetLineWidth(0);
                    
          lg1_Grid->AddEntry(histclone, "#muBoonE Tune [Stat+Syst]", "f" );
      }


   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );
      
      if(sl_idx==0){
      std::string lg1_label = eci.label(cat);
      lg1_Grid->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      }
      
      //std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }

   if(sl_idx==0){
    if(Plot_EXT == true) lg1_Grid->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );
   }

    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");


   //  std::cout<<"printing chi values "<< std::endl;

      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);

    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    sprintf(axisXtitle, "%s", "");
    
    GC_Stack->cd(GridBins); 
    
     DrawStack(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      false,
      99,
      ymax,
      WindowZoomScale.at(sl_idx),
      Plot_EXT );
    std::cout<<"Zoom In Times " << WindowZoomScale.at(sl_idx) << std::endl;
    
    
     drawString(BinStringMap[BinVector.at(sl_idx)], .02, false );
    
    		if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Stack->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .03,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(42);
			la2->SetTextSize(0.02);
			la2->Draw();
		}
    
    
    
    
  }// End of Canvus Bins
   ///////////////////////////////////////// 
  lg1_Grid->Draw("same");
    
  GC_Stack->SetYLabel_Size(.015);
	GC_Stack->SetXLabel_Size(.015);
  GC_Stack->SetInterpadSpace(.005);
	  //GC_Stack->SetRightMargin(0.05);
	  //GC_Stack->SetLeftMargin(0.08);
	  
  GC_Stack->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GC_Stack->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
  GC_Stack->ResetPads();
	GC_Stack->SetXTitle("cos#theta_{#mu}");
	GC_Stack->SetYTitleSize(25);
	GC_Stack->SetXTitleSize(20);  
	GC_Stack->SetYTitle("NEvents");
	GC_Stack->SetTitleAlignmentFor6Hist();
  //leg->Draw("SAME");
 GC_Stack->Modified();
   sprintf(pdf_title, "%s.pdf", Pdf_name2D.c_str());
 GC_Stack->Print(pdf_title);
  ///////////////////////////////////////////////////

 GridCanvas *GridCanvas_TotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg1_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg1_FracError_Grid->SetNColumns(2);
 lg1_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
  {

  const auto& slice = sb.slices_.at( sl_idx );
 
 bool FillLegend = (sl_idx==0) ? true : false;

 int GridBins = sl_idx + 1; 
 
 
 DrawFractionalError_GC(
 matrix_map,
 cov_mat_keys,
 "total",
 reco_mc_plus_ext_hist,
 slice,
 BinStringMap[BinVector.at(sl_idx)],
 1.0,
 GridCanvas_TotalFracError,
 GridBins,
 FillLegend, 
 lg1_FracError_Grid);

 }

 std::cout<<"FInished"<< std::endl;
 
 DrawGridCanvas(GridCanvas_TotalFracError,
 lg1_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.3,
 -1., 1. );
 
 
 delete GridCanvas_TotalFracError;
 delete lg1_FracError_Grid; 
 ///////////////////////////
 GridCanvas *GridCanvas_DetFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg2_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg2_FracError_Grid->SetNColumns(2);
 lg2_FracError_Grid->SetBorderSize(0);
 

 for ( auto sl_idx : SliceBins  )
  {

  const auto& slice = sb.slices_.at( sl_idx );
 
 bool FillLegend = (sl_idx==0) ? true : false;

 int GridBins = sl_idx + 1; 


  DrawFractionalError_GC(
  matrix_map,
  cov_mat_keys_detVar,
  "detVar_total",
  reco_mc_plus_ext_hist,
  slice,
  BinStringMap[BinVector.at(sl_idx)],
  1.0,
  GridCanvas_DetFracError,
  GridBins,
  FillLegend, 
  lg2_FracError_Grid);
  
 }
 
 DrawGridCanvas(GridCanvas_DetFracError,
 lg2_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.25,
 -1., 1. );
 
 delete GridCanvas_DetFracError;
 delete lg2_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg3_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg3_FracError_Grid->SetNColumns(2);
 lg3_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
   DrawFractionalError_GC(
   matrix_map,
   cov_mat_keys_cross,
   "xsec_unisim",
   reco_mc_plus_ext_hist,
   slice,
   BinStringMap[BinVector.at(sl_idx)],
   1.0,
   GridCanvas_CrossFracError,
   GridBins,
   FillLegend, 
   lg3_FracError_Grid);
   
   }
 
 DrawGridCanvas(GridCanvas_CrossFracError,
 lg3_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.08,
 -1.0, 1.0 );
 
 delete GridCanvas_CrossFracError;
 delete lg3_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossTotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg4_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg4_FracError_Grid->SetNColumns(2);
 lg4_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
   DrawFractionalError_GC(
   matrix_map,
   cov_mat_key_totalsumcross,
   "xsec_total",
   reco_mc_plus_ext_hist,
   slice,
   BinStringMap[BinVector.at(sl_idx)],
   1.0,
   GridCanvas_CrossTotalFracError,
   GridBins,
   FillLegend, 
   lg4_FracError_Grid);
   
   }
 
 DrawGridCanvas(GridCanvas_CrossTotalFracError,
 lg4_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.3,
 -1., 1.0 );
 
 delete GridCanvas_CrossTotalFracError;
 delete lg4_FracError_Grid; 
 
 
    // ENd of 
     /////////////////////
     // ENd of slices 
     ////////////////////
 
    sprintf(pdf_title, "%s.pdf)", Pdf_name2D.c_str());
    c1 -> Print(pdf_title);
 
 
 
 
}/////End of tutorial_slice_plots_2D
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_2D_inclusive() {

   std::cout<<"Starting tutorial_slice_plots 2D"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
    fpm.load_file_properties( "nuwro_file_properties_Tuples_5_13_2024.txt" );
    std::cout<<" passed "<< std::endl;
  #endif

 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
  bool Plot_EXT = false; 
 std::cout<<"Created syst_ptr a  MCC9SystematicsCalculator"<< std::endl;

  auto* syst_ptr = new MCC9SystematicsCalculator(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_FakeData_2D_binningscheme2_pmucorrection_v2.root",
    "systcalc_noNuWro.conf" );
    ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_inclusive_v7_pmucorrection.root
  auto& syst = *syst_ptr;
 //UnivMake_FakeData_BDTdecided_2D_inclusive_v3_pmucorrection.root
 std::cout<<"Intizing Done:   MCC9SystematicsCalculator "<< std::endl; 

  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  // Notes :: I think everything is projected by bin Number in 1D and the slicer will get it 
  
  
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
   reco_bnb_hist->Add(reco_ext_hist,-1);
  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;
 std::cout<< " reco_bnb_hist Nbins = "<< reco_bnb_hist->GetNbinsX()<< std::endl; 


  #ifdef USE_FAKE_DATA
    // Add the EXT to the "data" when working with fake data
    if( Plot_EXT == true)reco_bnb_hist->Add( reco_ext_hist );
  #endif

 std::cout<<"added reco bnb to ext "<< std::endl;
  TH2D* category_hist = syst.cv_universe().hist_categ_.get();
 std::cout<<"Finished  category_hist "<< std::endl; 
  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );


 // Zero out if not plotting 
 if(Plot_EXT == false){
   int nBins = reco_mc_plus_ext_hist->GetNbinsX();
     for (int i = 1; i <= nBins; ++i) {
         reco_mc_plus_ext_hist->SetBinContent(i, 0.0);
     }
 }


  // Add in the CV MC prediction
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  //auto& matrix_map = *matrix_map_ptr;

   CovMatrixMap &matrix_map = *matrix_map_ptr;

  std::cout<<"Making slice from inclusive "<< std::endl;

  auto* sb_ptr = new SliceBinning( "mybins_mcc9_2D_muon_inclusive_newbinning9_newtuples.txt" ); //tutorial_reco_slice_config.txt
  auto& sb = *sb_ptr;

  
  ////////////////////////////////
  // Starting to plot
  //////////////////////////////////

  std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name2D_inclusive.c_str());
  c1 -> Print(pdf_title);

  //std::vector<std::string> xaxistitles_vector{"Cos#theta_{#mu}","p_{#mu} [GeV/c]", "Bin N" };

  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if( Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "NuWro Fake Data", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "Total MC", "le" );

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }


   if( Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );

    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->Draw( "e" );
    if(sl_idx<9) slice_bnb->hist_->GetXaxis()->SetRangeUser( 0., 2.0 ); // Setting X range for inclusive 0,2 GeV
    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

     // sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
     // text->DrawLatex(0.15, 0.80, textplace);
      sprintf(pdf_title, "%s.pdf", Pdf_name2D_inclusive.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    if(10 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else{sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
  
      double chi1,pvalue;
      int NDF;
    
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      Pdf_name2D_inclusive,
      "Test",
      axisXtitle,
      c1,
      ymax,
       Plot_EXT,
       chi1,
       pvalue,
      NDF);


    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    /*
    const std::vector< std::string > cov_mat_keys = { "total",
      "detVar_total", "flux", "reint", "xsec_total", "POT", "numTargets",
      "MCstats", "EXTstats", "BNBstats", "xsec_NormCCCOH"
    };
   */

  

  
  
    //  const std::vector< std::string > cov_mat_keys = { "xsec_unisim",
    //    "xsec_AxFFCCQEshape", "xsec_DecayAngMEC", "xsec_XSecShape_CCMEC", 
    //    "xsec_NormCCCOH",  "xsec_NormNCCOH", "xsec_RPA_CCQE",
    //     "xsec_ThetaDelta2NRad",  "xsec_Theta_Delta2Npi",
    //     "MCstats", "EXTstats", "BNBstats"
    //  };

   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      
      
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color;
      if ( color >= 10 ) color += 10;
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend( 0.45, 0.7, 0.8, 0.88 );
    lg2->SetNColumns(2);
    lg2->SetBorderSize(0);
    lg2->SetTextSize(.025); //
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
   // auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(.45);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    if(9 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else if(10 ==sl_idx){  sprintf(axisXtitle, "%s", "p_{p} [GeV/c]");}
    else if(11 ==sl_idx){sprintf(axisXtitle, "%s", "cos#theta_{#mu}");}
    else{sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle(" ");
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "Total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      //std::cout << name << " frac err in bin #1 = "
      //<< hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    //std::cout << "Total frac error in bin #1 = "
    //  << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);


  } 
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////
 std::vector<size_t> SliceBins{0,1,2,3,4,5,6,7,8};
 auto BinVector = GetProjectBinVector();
 auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES_inclusive, "cos#theta_{#mu}");
 
 ///////////////////////////////////////////////////
 /// Making Grid Figure here 
 ///////////////////////////////////////////////////
 std::vector<double> WindowZoomScale{2,4,4,2,1,1,1,1,1};


 double min_XAxis_GridCanvas = 0;
 double max_XAxis_GridCanvas = 2.0;
 
 double min_YAxis_GridCanvas = 0.0;
 double max_YAxis_GridCanvas = 7350.0;
 
 GridCanvas *GC_Stack = new GridCanvas(uniq(), 3, 4, 800, 550);
   
   GC_Stack->SetBottomMargin(.00);
   GC_Stack->SetTopMargin(.02);
   GC_Stack->SetRightMargin(.05);

    TLegend* lg1_Grid= new TLegend( 0.41, 0.08, 0.96, 0.2 );
    lg1_Grid->SetNColumns(2);
    lg1_Grid->SetBorderSize(0);
    //lg1_Grid->SetTextSize(defaultTextSize*1.05); //

  for ( auto sl_idx : SliceBins  )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;
    int GridBins = sl_idx + 1; 
    
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if( Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();
      
      if(sl_idx==0){
          lg1_Grid->AddEntry(slice_bnb->hist_.get(), "NuWro Fake Data", "pe" );
          auto histclone = (TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
          
           histclone->SetFillColorAlpha(kRed, 0.8);
				   histclone->SetFillStyle(3001);
				   histclone->SetMarkerStyle(0);
				   histclone->SetLineWidth(0);
                    
          lg1_Grid->AddEntry(histclone, "Total MC [Stat+Syst]", "f" );
      }


   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );
      
      if(sl_idx==0){
      std::string lg1_label = eci.label(cat);
      lg1_Grid->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      }
      
      //std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }

   if(sl_idx==0){
     if( Plot_EXT == true) lg1_Grid->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );
   }

    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");


   //  std::cout<<"printing chi values "<< std::endl;

      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);

    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());

    
    GC_Stack->cd(GridBins); 
    
     DrawStack(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      false,
      99,
      ymax,
      WindowZoomScale.at(sl_idx), 
      Plot_EXT );
    
   
     drawString(BinStringMap[BinVector.at(sl_idx)], .02, false );
    
    		if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Stack->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .03,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(42);
			la2->SetTextSize(0.02);
			la2->Draw();
		}
    
    
    
    
  }// End of Canvus Bins
   ///////////////////////////////////////// 
  lg1_Grid->Draw("same");
    
  GC_Stack->SetYLabel_Size(.015);
	GC_Stack->SetXLabel_Size(.015);
  GC_Stack->SetInterpadSpace(.005);
	  //GC_Stack->SetRightMargin(0.05);
	  //GC_Stack->SetLeftMargin(0.08);
	  
  GC_Stack->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GC_Stack->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
  GC_Stack->ResetPads();
	GC_Stack->SetXTitle("p_{#mu} [GeV/c]");
	GC_Stack->SetYTitleSize(25);
	GC_Stack->SetXTitleSize(20);  
	GC_Stack->SetYTitle("NEvents");
	GC_Stack->SetTitleAlignmentFor6Hist();
  //leg->Draw("SAME");
 GC_Stack->Modified();
   sprintf(pdf_title, "%s.pdf", Pdf_name2D_inclusive.c_str());
 GC_Stack->Print(pdf_title);
  // ENd of 
   delete GC_Stack;
  /////////////////////
  // ENd of slices 
  ////////////////////

 GridCanvas *GridCanvas_TotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg1_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg1_FracError_Grid->SetNColumns(2);
 lg1_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
 DrawFractionalError_GC(
 matrix_map,
 cov_mat_keys,
 "total",
 reco_mc_plus_ext_hist,
 slice,
 BinStringMap[BinVector.at(sl_idx)],
 1.0,
 GridCanvas_TotalFracError,
 GridBins,
 FillLegend, 
 lg1_FracError_Grid);
 
 }

 std::cout<<"FInished"<< std::endl;
 
 DrawGridCanvas(GridCanvas_TotalFracError,
 lg1_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.3,
 .1, 2.0 );
 
 
 delete GridCanvas_TotalFracError;
 delete lg1_FracError_Grid; 
 ///////////////////////////
 GridCanvas *GridCanvas_DetFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg2_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg2_FracError_Grid->SetNColumns(2);
 lg2_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
   DrawFractionalError_GC(
   matrix_map,
   cov_mat_keys_detVar,
   "detVar_total",
   reco_mc_plus_ext_hist,
   slice,
   BinStringMap[BinVector.at(sl_idx)],
   1.0,
   GridCanvas_DetFracError,
   GridBins,
   FillLegend, 
   lg2_FracError_Grid);
  
  }

 DrawGridCanvas(GridCanvas_DetFracError,
 lg2_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.3,
 .1, 2.0 );
 
 delete GridCanvas_DetFracError;
 delete lg2_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg3_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg3_FracError_Grid->SetNColumns(2);
 lg3_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 


  DrawFractionalError_GC(
  matrix_map,
  cov_mat_keys_cross,
  "xsec_unisim",
  reco_mc_plus_ext_hist,
  slice,
  BinStringMap[BinVector.at(sl_idx)],
  1.0,
  GridCanvas_CrossFracError,
  GridBins,
  FillLegend, 
  lg3_FracError_Grid);
  
  }

 DrawGridCanvas(GridCanvas_CrossFracError,
 lg3_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.15,
 .1, 2.0 );
 
 delete GridCanvas_CrossFracError;
 delete lg3_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossTotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg4_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg4_FracError_Grid->SetNColumns(2);
 lg4_FracError_Grid->SetBorderSize(0);


 for ( auto sl_idx : SliceBins  )
  {

  const auto& slice = sb.slices_.at( sl_idx );
 
 bool FillLegend = (sl_idx==0) ? true : false;

 int GridBins = sl_idx + 1; 


  DrawFractionalError_GC(
  matrix_map,
  cov_mat_key_totalsumcross,
  "xsec_total",
  reco_mc_plus_ext_hist,
  slice,
  BinStringMap[BinVector.at(sl_idx)],
  1.0,
  GridCanvas_CrossTotalFracError,
  GridBins,
  FillLegend, 
  lg4_FracError_Grid);
  
  }

 DrawGridCanvas(GridCanvas_CrossTotalFracError,
 lg4_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.3,
 .1, 2.0 );
 
 delete GridCanvas_CrossTotalFracError;
 delete lg4_FracError_Grid; 



 //////////////////////////////////////////////////////////////////////////////
 ///  End of PDF print
 //////////////////////////////////////////////////////////////////////////////
    sprintf(pdf_title, "%s.pdf)", Pdf_name2D_inclusive.c_str());
   c1 -> Print(pdf_title);
}/////End of tutorial_slice_plots_2D
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_2D_Closure_Plots(std::string InputFileProperties, 
std::string inputUnimakeFile, std::string BinningFile, 
bool IsBinningScheme1,std::string Pdf_name2D_Closure ) {

   std::cout<<"Starting tutorial_slice_plots 2D"<< std::endl;
 // #ifdef USE_FAKE_DATA
 //   // Initialize the FilePropertiesManager and tell it to treat the NuWro
 //   // MC ntuples as if they were data
 //   auto& fpm = FilePropertiesManager::Instance();
 //   std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
 //   
 //   std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
 //   std::cout<<"trying to apply load_file_properties"<< std::endl;
 //   fpm.load_file_properties( "nuwro_file_properties_Tuples_4_26_2024_Closure.txt" );
 //   std::cout<<" passed "<< std::endl;
 // #endif



auto& fpm = FilePropertiesManager::Instance();
  fpm.load_file_properties(InputFileProperties );

 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
  bool Plot_EXT = false; 
 std::cout<<"Created syst_ptr a  MCC9SystematicsCalculator"<< std::endl;

  auto* syst_ptr = new MCC9SystematicsCalculator(
    inputUnimakeFile,
    "systcalc_GENIEClosure.conf" );
    ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_inclusive_v7_pmucorrection.root
  auto& syst = *syst_ptr;
 //UnivMake_FakeData_BDTdecided_2D_inclusive_v3_pmucorrection.root
 std::cout<<"Intizing Done:   MCC9SystematicsCalculator "<< std::endl; 

  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  // Notes :: I think everything is projected by bin Number in 1D and the slicer will get it 
  
  
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();

///// I think I got to remove the ext

  reco_bnb_hist->Add(reco_ext_hist,-1);


  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;
 std::cout<< " reco_bnb_hist Nbins = "<< reco_bnb_hist->GetNbinsX()<< std::endl; 


  #ifdef USE_FAKE_DATA
    // Add the EXT to the "data" when working with fake data
  //  if( Plot_EXT == true)reco_bnb_hist->Add( reco_ext_hist );
  #endif

 std::cout<<"added reco bnb to ext "<< std::endl;
  TH2D* category_hist = syst.cv_universe().hist_categ_.get();
 std::cout<<"Finished  category_hist "<< std::endl; 
  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );


 // Zero out if not plotting 
  reco_mc_plus_ext_hist->Reset();

  // Add  MC TRUE  prediction
  //////////////////////////
  // Getting TRUE 
  ///////////////////////////////
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );
  //reco_mc_plus_ext_hist->Add(reco_ext_hist,-1);
  
// TH1D* genie_cv_truth = syst.cv_universe().hist_true_.get();
  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  //auto& matrix_map = *matrix_map_ptr;

   CovMatrixMap &matrix_map = *matrix_map_ptr;

  std::cout<<"Making slice from inclusive "<< std::endl;

  auto* sb_ptr = new SliceBinning( BinningFile ); //tutorial_reco_slice_config.txt
  auto& sb = *sb_ptr;

  
  ////////////////////////////////
  // Starting to plot
  //////////////////////////////////

  std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name2D_Closure.c_str());
  c1 -> Print(pdf_title);

  //std::vector<std::string> xaxistitles_vector{"Cos#theta_{#mu}","p_{#mu} [GeV/c]", "Bin N" };

  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    std::cout<<"Getting Slice"<< std::endl;
    const auto& slice = sb.slices_.at( sl_idx );
    std::cout<<"Got Slice"<< std::endl;
    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("MCstats") );

    std::cout<<"Constucted SliceHistograms:finsihed: slice_bnb"<< std::endl;
    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("MCstats") );
      
    std::cout<<"Constucted SliceHistograms:finsihed: EXTstats"<< std::endl;

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );

    std::cout<<"Constucted SliceHistograms:finsihed: total"<< std::endl;

   std::cout<<"Constucted SliceHistograms"<< std::endl;

    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if( Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "Data", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "Total MC", "le" );

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }


   if( Plot_EXT == true) lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );

    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->Draw( "e" );
    if(sl_idx<9 && IsBinningScheme1==false) slice_bnb->hist_->GetXaxis()->SetRangeUser( 0., 2.0 ); // Setting X range for inclusive 0,2 GeV
    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

     // sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
     // text->DrawLatex(0.15, 0.80, textplace);
      sprintf(pdf_title, "%s.pdf", Pdf_name2D_Closure.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    if(10 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else{sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
      double chi1,pvalue;
    int NDF;
    
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      Pdf_name2D_Closure,
      "Test",
      axisXtitle,
      c1,
      ymax,
      Plot_EXT,
      chi1,
      pvalue,
      NDF);
      


    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
    
    const std::vector< std::string > cov_mat_keys = { "total", "MCstats"};
   

   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      
      
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

      if ( color <= 9 ) ++color;
      if ( color == 5 ) ++color;
      if ( color >= 10 ) color += 10;
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend( 0.45, 0.7, 0.8, 0.88 );
    lg2->SetNColumns(2);
    lg2->SetBorderSize(0);
    lg2->SetTextSize(.025); //
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
   // auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    //frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    //frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(.45);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle(" ");
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "Total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      //std::cout << name << " frac err in bin #1 = "
      //<< hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    //std::cout << "Total frac error in bin #1 = "
    //  << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);


  } 
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////
 std::vector<size_t> SliceBins{0,1,2,3,4,5,6,7,8};
 auto BinVector = GetProjectBinVector();
 
 std::map<Binning2D , std::string > BinStringMap;
 
 if(IsBinningScheme1==true){  BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES, "p_{#mu} [GeV/c]");}
 else if(IsBinningScheme1==false){  BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES_inclusive, "cos#theta_{#mu}");}
 

 
 ///////////////////////////////////////////////////
 /// Making Grid Figure here 
 ///////////////////////////////////////////////////
 std::vector<double> WindowZoomScale{2,4,4,2,1,1,1,1,1};


 double min_XAxis_GridCanvas = 0;
 double max_XAxis_GridCanvas = 2.0;
 
 double min_YAxis_GridCanvas = 0.0;
 double max_YAxis_GridCanvas = 7350.0;
 
 GridCanvas *GC_Stack = new GridCanvas(uniq(), 3, 4, 800, 550);


    TLegend* lg1_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
    lg1_Grid->SetNColumns(2);
    lg1_Grid->SetBorderSize(0);
    //lg1_Grid->SetTextSize(defaultTextSize*1.05); //

  for ( auto sl_idx : SliceBins  )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;
    int GridBins = sl_idx + 1; 
    
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("MCstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("MCstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if( Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();
      
      if(sl_idx==0){
          lg1_Grid->AddEntry(slice_bnb->hist_.get(), "GENIE Fake Data", "pe" );
          auto histclone = (TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
          
           histclone->SetFillColorAlpha(kRed, 0.8);
				   histclone->SetFillStyle(3001);
				   histclone->SetMarkerStyle(0);
				   histclone->SetLineWidth(0);
                    
          lg1_Grid->AddEntry(histclone, "Total MC [Stat]", "f" );
      }


   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );
      
      if(sl_idx==0){
      std::string lg1_label = eci.label(cat);
      lg1_Grid->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      }
      
      //std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }

   if(sl_idx==0){
     if( Plot_EXT == true) lg1_Grid->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );
   }

    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");


   //  std::cout<<"printing chi values "<< std::endl;

      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);

    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());

    
    GC_Stack->cd(GridBins); 
    
     DrawStack(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      false,
      99,
      ymax,
      WindowZoomScale.at(sl_idx), 
      Plot_EXT );
    
   
     drawString(BinStringMap[BinVector.at(sl_idx)], .02, false );
    
    		if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Stack->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .03,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(42);
			la2->SetTextSize(0.02);
			la2->Draw();
		}
    
    
    
    
  }// End of Canvus Bins
   ///////////////////////////////////////// 
  lg1_Grid->Draw("same");
    
  GC_Stack->SetYLabel_Size(.015);
	GC_Stack->SetXLabel_Size(.015);
  GC_Stack->SetInterpadSpace(.005);
	  //GC_Stack->SetRightMargin(0.05);
	  //GC_Stack->SetLeftMargin(0.08);
	  
  GC_Stack->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GC_Stack->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
  GC_Stack->ResetPads();
	GC_Stack->SetXTitle("p_{#mu} [GeV/c]");
	GC_Stack->SetYTitleSize(25);
	GC_Stack->SetXTitleSize(20);  
	GC_Stack->SetYTitle("NEvents");
	GC_Stack->SetTitleAlignmentFor6Hist();
  //leg->Draw("SAME");
 GC_Stack->Modified();
   sprintf(pdf_title, "%s.pdf", Pdf_name2D_Closure.c_str());
 GC_Stack->Print(pdf_title);
  // ENd of 
   delete GC_Stack;
  /////////////////////
  // ENd of slices 
  ////////////////////

 GridCanvas *GridCanvas_TotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg1_FracError_Grid= new TLegend( 0.41, 0.1, 0.96, 0.25 );
 lg1_FracError_Grid->SetNColumns(2);
 lg1_FracError_Grid->SetBorderSize(0);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
 DrawFractionalError_GC(
 matrix_map,
 cov_mat_keys,
 "total",
 reco_mc_plus_ext_hist,
 slice,
 BinStringMap[BinVector.at(sl_idx)],
 1.0,
 GridCanvas_TotalFracError,
 GridBins,
 FillLegend, 
 lg1_FracError_Grid);
 
 }

 std::cout<<"FInished"<< std::endl;
 
 DrawGridCanvas(GridCanvas_TotalFracError,
 lg1_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.3,
 .1, 2.0 );
 
 
 delete GridCanvas_TotalFracError;
 delete lg1_FracError_Grid; 

 



//////////////////////////////////////////////////////////////////////////////
///  End of PDF print
//////////////////////////////////////////////////////////////////////////////
   sprintf(pdf_title, "%s.pdf)", Pdf_name2D_Closure.c_str());
   c1 -> Print(pdf_title);
}/////End of tutorial_slice_plots_2D



int main() {


std::cout<<" Testing Slice Plots "<< std::endl;

 // tutorial_slice_plots();
  //tutorial_slice_plots_NODATAPOINTS();
  //tutorial_slice_plots_withData();
  tutorial_slice_plots_2D_withData();
  //tutorial_slice_plots_2D_inclusive_withData();
  //tutorial_slice_plots_2D();
  //tutorial_slice_plots_2D_inclusive();
  //
  //tutorial_slice_plots_2D_Closure_Plots("nuwro_file_properties_Tuples_5_13_2024_Closure.txt", 
  //"/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_FakeData_2D_binningscheme1_pmucorrection_v1_Closure.root", 
  //"mybins_mcc9_2D_muon_new3.txt", true,
  //"GENIE_Clourse_files_scheme1_check");
  ////
  //  tutorial_slice_plots_2D_Closure_Plots("nuwro_file_properties_Tuples_5_13_2024_Closure.txt", 
  //"/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_5_13_2024/UnivMake_FakeData_2D_binningscheme2_pmucorrection_v1_Closure.root", 
  //"mybins_mcc9_2D_muon_inclusive_newbinning9_newtuples.txt", false,
  //"GENIE_Clourse_files_scheme2");
  
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
std::string MicroBooNE_LegendTitle(){
return get_legend_title( 4.54e19 );
}// End of Funtion


std::string MicroBooNE_LegendTitle_OpenData(){
return get_legend_title( 1.42E+20 );
}// End of Funtion

void DrawFractionalError(
CovMatrixMap &matrix_map,
std::vector< std::string > cov_mat_keys,
TH1D* BinSlice_template,
const Slice& slice,
char *XaxisTitle,
char *TotalHistName,
double Ymax,
std::string Pdf_name,
TCanvas *c1)
{

 //CovMatrixMap &matrix_map = *matrix_map_ptr;
 
 auto* fr_unc_hists = new std::map< std::string, TH1* >();
 auto& frac_uncertainty_hists = *fr_unc_hists;
 
 int color = 1;
 int loop_count=0;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *BinSlice_template, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

              ++color;
        if ( color == 3 ) color = kGreen +2;
        if ( color == kGreen + 3) color = 4;
        if ( color == 5 ) color = 6;
        if ( color == 7 ) color = kYellow -2;
        if ( color == kYellow -1 ) color = kMagenta + 2;
        if (color == kMagenta + 3) color = kSpring - 2;
        if (color == kSpring - 1) color = kOrange + 2;
        if (color == kOrange + 3) color = kRed - 6;
        if (color == kRed - 5) color = kAzure + 8;
        if (color == kAzure + 8) color = kOrange - 3;
      slice_for_syst->hist_->SetLineColor( color );
      slice_for_syst->hist_->SetLineWidth( 3 );

    if(key=="BNBstats")frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    else if(key=="EXTstats" )frac_uncertainty_hists.at( "EXTstats"  )->SetLineStyle(2);
    else if(key=="MCstats" )frac_uncertainty_hists.at( "MCstats"  )->SetLineStyle(2);

    }

 TLegend* lg2 = new TLegend( 0.15, 0.6, 0.75, 0.88 );
    lg2->SetNColumns(2);
    lg2->SetBorderSize(0);
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    //auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
    auto* total_frac_err_hist = frac_uncertainty_hists.at(TotalHistName );
    //  std::cout<<" Finshed " << std::endl;


    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(Ymax);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    total_frac_err_hist->GetXaxis()->SetTitle( XaxisTitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle( "" );
    total_frac_err_hist->GetYaxis()->SetLabelSize(.02);
    total_frac_err_hist->GetXaxis()->SetLabelSize(.025);
    total_frac_err_hist->GetXaxis()->SetTitleSize(0.035);
    total_frac_err_hist->GetXaxis()->CenterTitle(kFALSE);
    total_frac_err_hist->SetTitleOffset (1.01,"Y");
    
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, TotalHistName, "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == TotalHistName ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
      << hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );
     //DrawFakeData();
    std::cout << "Total frac error in bin #1 = "
      << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      sprintf(pdf_title, "%s", Pdf_name.c_str());
      c1 -> Print(pdf_title);






}
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void DrawFractionalError_GC(
CovMatrixMap &matrix_map,
std::vector< std::string > cov_mat_keys,
char *TotalHistName,
TH1D* BinSlice_template,
const Slice& slice,
std::string BinSliceInfo,
double Ymax,
GridCanvas *GridCanvas_input,
int BinN_toDraw,
bool FillLegend, 
TLegend* lg_input, 
bool istotal)
{

  GridCanvas_input->cd(BinN_toDraw); 
 //CovMatrixMap &matrix_map = *matrix_map_ptr;
 
 auto* fr_unc_hists = new std::map< std::string, TH1* >();
 auto& frac_uncertainty_hists = *fr_unc_hists;
 
 int color = 1;
 int loop_count=0;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *BinSlice_template, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

         ++color;
        if ( color == 3 ) color = kGreen +2;
        if ( color == kGreen + 3) color = 4;
        if ( color == 5 ) color = 6;
        if ( color == 7 ) color = kYellow -2;
        if ( color == kYellow -1 ) color = kMagenta + 2;
        if (color == kMagenta + 3) color = kSpring - 2;
        if (color == kSpring - 1) color = kOrange + 2;
        if (color == kOrange + 3) color = kRed - 6;
        if (color == kRed - 5) color = kAzure + 8;
        if (color == kAzure + 8) color = kOrange - 3;
        
      
      if(istotal==true){slice_for_syst->hist_->SetLineColor( ColorMap[key] );}
      else{slice_for_syst->hist_->SetLineColor( color );}
      
      
      slice_for_syst->hist_->SetLineWidth( 2 );

    if(key=="BNBstats")frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    else if(key=="EXTstats" )frac_uncertainty_hists.at( "EXTstats"  )->SetLineStyle(2);
    else if(key=="MCstats" )frac_uncertainty_hists.at( "MCstats"  )->SetLineStyle(2);

    }


    auto* total_frac_err_hist = frac_uncertainty_hists.at(TotalHistName );
    //  std::cout<<" Finshed " << std::endl;

    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(Ymax);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 2 );
    total_frac_err_hist->SetTitle( "" );
    total_frac_err_hist->GetYaxis()->SetNdivisions(505);
    total_frac_err_hist->GetXaxis()->SetNdivisions(505);
    total_frac_err_hist->Draw( "hist" );
     
                 


    if(FillLegend == true)  {
    lg_input->AddEntry( total_frac_err_hist, TotalHistName, "l" );
    }
   
   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == TotalHistName ) continue;

          if(FillLegend == true)  {
            lg_input->AddEntry( hist, name.c_str(), "l" );
          }
      
        hist->Draw( "same hist" );
    }
    //std::cout<<"END error leg names"<<std::endl;

   drawString(BinSliceInfo, .035, false );


  return; 


}//////End of Function
////////////////////////////////////////////////////////////////////  
/// 
////////////////////////////////////////////////////////////////////  
void DrawGridCanvas(GridCanvas *GridCanvas_input,
TLegend* lg_input, std::string XaxisTitle, 
std::string YaxisTitle, std::string pdftitle,
double min_YAxis_GridCanvas, double max_YAxis_GridCanvas,
double min_XAxis_GridCanvas, double max_XAxis_GridCanvas ){
char TitleInput[1024];


  GridCanvas_input->SetYLabel_Size(.03);
	GridCanvas_input->SetXLabel_Size(.03);
  //GridCanvas_input->SetInterpadSpace(.005);
	  //GridCanvas_input->SetRightMargin(0.05);
	  //GridCanvas_input->SetLeftMargin(0.08);
	  
  GridCanvas_input->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GridCanvas_input->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
  GridCanvas_input->ResetPads();
   sprintf(TitleInput, "%s", XaxisTitle.c_str());
	GridCanvas_input->SetXTitle(TitleInput);
	GridCanvas_input->SetYTitleSize(25);
	GridCanvas_input->SetXTitleSize(20);  
	sprintf(TitleInput, "%s", YaxisTitle.c_str());
	GridCanvas_input->SetYTitle(TitleInput);
	GridCanvas_input->SetTitleAlignmentFor6Hist();
 GridCanvas_input->Modified();
 
 lg_input->Draw("Same");
 
  sprintf(TitleInput, "%s", pdftitle.c_str());
 GridCanvas_input->Print(TitleInput);
  




}


void DrawFakeData_GENIE(){
    TLatex* textfake = new TLatex;
    textfake->SetNDC();
    textfake->SetTextSize(0.03);
    textfake->SetTextColor(kBlue);
    textfake->DrawLatex(0.12, 0.86, "USING [GENIE] FAKE DATA");

}

void DrawFakeData_InProgess(){
    TLatex* textfake = new TLatex;
    textfake->SetNDC();
    textfake->SetTextSize(0.03);
    textfake->SetTextColor(kBlue);
    textfake->DrawLatex(0.12, 0.86, "MicroBooNE Simulation, In Progress");

}


///////////////////////////////////////////////////////////////////////////////
// With Data 
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_2D_withData()
{
   std::cout<<"Starting tutorial_slice_plots 2D"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
    fpm.load_file_properties( "file_properties.txt" );
    std::cout<<" passed "<< std::endl;
  #endif
  
 fpm.load_file_properties( "/exp/uboone/app/users/liangliu/analysis-code/tutorial/xsec_analyzer_eaf/configs/file_properties_cthorpe_current.txt" );
 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
auto binwidthMap = Projection9Bins_width(MUON_2D_BIN_EDGES);
 std::cout<<"Starting MCC9SystematicsCalculator"<< std::endl;

//UnivMake_OPEN_Data_2D_binningscheme1_CC0piNp.root

  auto* syst_ptr = new MCC9SystematicsCalculator(
    "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Liang_unimake/unimake_binningScheme1.root",
    "systcalc_noNuWro_eventrates.conf" );
  auto& syst = *syst_ptr;
  ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme1_v1.root
  //nimake_scheme1_MCS_pmu_FC_OPEN.root
  //Unimak_scheme1_RANGE_FC_Open.root //Unimak_scheme1_RANGE_PC_Open.root
  
  //unimake_scheme1_MCS_pmu_FC_OPEN.root
  
   //Unimak_scheme1_RANGE_PC_Open.root
   // unimake_scheme1_MCS_pmu_FC_OPEN.root
   // unimak_scheme1_MCS_pmu_PC.root


  
  //unimake_MCS_pmu_contained_v2_OPEN.root
  //
  // unimake_MCS_pmu_contained_v2_OPEN.root
// 
// Unimak_scheme2_RANGEPmu_uncontained_v3_Open.root
// UniMake_scheme1_RANGE_Contained_v2_OPEN.root
  
  
  //UnivMake_scheme1_PC_muonsOnly.root
  //unimake_MCS_pmu_contained_v2.root
// unimak_MCS_pmu_uncontained_v2.root
  //UnivMake_scheme1_pionSidebandCondition_Muons_contained.root
  //  UnivMake_OPEN_Data_2D_binningscheme1_pmucorrection_v7_newbins.root

char LegendMasterTitle[1024];

   std::cout<<"Finished  MCC9SystematicsCalculator "<< std::endl; 
  //old 
  ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_v4_pmucorrection.root
  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  // Notes :: I think everything is projected by bin Number in 1D and the slicer will get it 
  bool Plot_EXT = true; 
  
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();

  //reco_bnb_hist->Add(reco_ext_hist,-1);


  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;


  TH2D* category_hist = syst.cv_universe().hist_categ_.get();

  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );

 // Zero out if not plotting 

  // Add in the CV MC prediction
  
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  //auto& matrix_map = *matrix_map_ptr;

   CovMatrixMap &matrix_map = *matrix_map_ptr;


  auto* sb_ptr = new SliceBinning( "mybins_mcc9_2D_muon_v1_july3_2024.txt" ); //tutorial_reco_slice_config.txt
  auto& sb = *sb_ptr;
  ////////////////////////////////
  // Starting to plotte
  //////////////////////////////////

  //std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;

  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(", Pdf_name2_Data.c_str());
  c1 -> Print(pdf_title);

  //std::vector<std::string> xaxistitles_vector{"Cos#theta_{#mu}","p_{#mu} [GeV/c]", "Bin N" };
   int BinNSlice = 9; 
	TLegend *legendMaster = new TLegend(0.05, 0.05, 0.98, 0.98);
	  std::string MicroBooneTitle = MicroBooNE_LegendTitle_OpenData();
	legendMaster->SetNColumns(2);
	legendMaster->SetBorderSize(0);
	
	
  TLegend* lg1_Grid= new TLegend( 0.21, 0.01, 0.99, 0.2 );
  lg1_Grid->SetNColumns(3);
  lg1_Grid->SetBorderSize(0);
//  lg1_Grid->SetTextSize(defaultTextSize*1.05); //
	
	 auto BinVector = GetProjectBinVector();
	
double Totalamount = 9999;
std::string FullChi_sqted; 



  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;
    //if(sl_idx>10)continue; 
    TLegend* lg1_stacked = new TLegend( 0.35, 0.7, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );
    //if(sl_idx>10)continue;
    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );
     

    if(BinNSlice==sl_idx){
    Totalamount = slice_mc_plus_ext->hist_.get()->Integral() +slice_ext->hist_.get()->Integral() ;
    }

 

  
    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "Open Data [Stat]", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "#muBooNE Tune [Sys+Stat]", "le" );

    if(BinNSlice==sl_idx){
         legendMaster->AddEntry(slice_bnb->hist_.get(), "Open Data [Stat]", "pe" );
         legendMaster->AddEntry(slice_mc_plus_ext->hist_.get(), "#muBooNE Tune [Sys+Stat]", "le" );
         
         TH1D* clonebnb = (TH1D*)slice_bnb->hist_.get()->Clone(uniq());
         TH1D* cloneext = (TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
         
         lg1_Grid->AddEntry(clonebnb, "Open Data [Stat]", "pe" );
         lg1_Grid->AddEntry(cloneext, "#muBooNE Tune [Sys+Stat]", "le" );
    }

   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );
     
     if(BinNSlice==sl_idx){
     double amount = temp_slice_mc->hist_.get()->Integral();
     double input = ( amount / Totalamount ) * 100.0 ; 
     sprintf(LegendMasterTitle, "%s (%2.1f%)",lg1_label.c_str(), input );
     legendMaster->AddEntry(temp_slice_mc->hist_.get(),LegendMasterTitle , "f" );
     
     TH1D*clone = (TH1D*)temp_slice_mc->hist_.get()->Clone(uniq());
     lg1_Grid->AddEntry(clone,LegendMasterTitle , "f" );
     } 


      --cat_bin_index;
    }


   lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );

  if(BinNSlice==sl_idx){
    double amount = slice_ext->hist_.get()->Integral();
     double input = ( amount/ Totalamount ) * 100.0 ; 
     sprintf(LegendMasterTitle, "EXT BNB (%2.1f%)", input );
     legendMaster->AddEntry(slice_ext->hist_.get(),LegendMasterTitle , "f" );
     TH1D*clone = (TH1D*)slice_ext->hist_.get()->Clone(uniq());
     lg1_Grid->AddEntry(clone,LegendMasterTitle , "f" );
    }


    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->GetYaxis()->SetLabelSize(.02);
    slice_bnb->hist_->Draw( "e" );

    const char* Error_title = slice_bnb->hist_->GetTitle();
    slice_pred_stack->Draw( "hist same" );

    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      if(sl_idx !=12){
      
      auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
      << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
      << " p-value = " << chi2_result.p_value_ << '\n';
      sprintf(textplace, "#chi^{2}/ndf: %.2f/%i",chi2_result.chi2_ , chi2_result.num_bins_ );
      text->DrawLatex(0.15, 0.85, textplace);
      if(BinNSlice==sl_idx){
      FullChi_sqted = ": " + std::string(textplace); 
      }
      sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      text->DrawLatex(0.15, 0.80, textplace);
      if(BinNSlice==sl_idx){
      FullChi_sqted = FullChi_sqted  + " : " + std::string(textplace); 
      }
            
      }
      
      
      sprintf(pdf_title, "%s.pdf", Pdf_name2_Data.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    if(9 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else{sprintf(axisXtitle, "%s", "cos#theta_{#mu}");}
  
    IncreaseTitleTH1(*h_bnb_input, .06);
    
      double chi1;
      double pvalue;
      int NDF;
    double SliceBinWidth = 1.0;
    if(sl_idx < 9) SliceBinWidth = binwidthMap[BinVector.at(sl_idx)];
    
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      Pdf_name2_Data,
      "Test",
      axisXtitle,
      c1,
      ymax,
      Plot_EXT, 
      chi1,
       pvalue,
      NDF,
      SliceBinWidth);

    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.
   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      
      
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

       ++color;
       if ( color == 3 ) color = kGreen +2;
       if ( color == kGreen + 3) color = 4;
       if ( color == 5 ) color = 6;
       if ( color == 7 ) color = kYellow -2;
       if ( color == kYellow -1 ) color = kMagenta + 2;
       if (color == kMagenta + 3) color = kSpring - 2;
       if (color == kSpring - 1) color = kOrange + 2;
       if (color == kOrange + 3) color = kRed - 6;
       if (color == kRed - 5) color = kAzure + 8;
       if (color == kAzure + 8) color = kOrange - 3;
       
      slice_for_syst->hist_->SetLineColor( ColorMap[key] );
      slice_for_syst->hist_->SetLineWidth( 3 );
    std::cout<<"key = "<< key << " color = "<< color << std::endl;


    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend(0.3, 0.6, 0.82, 0.88 );
    lg2->SetNColumns(3);
    lg2->SetBorderSize(0);
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   // std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
   // auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    //frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.45);
    total_frac_err_hist->SetMinimum(0.0);
    //total_frac_err_hist->SetMaximum(.5);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    if(9 == sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else if(10 ==sl_idx){  sprintf(axisXtitle, "%s", "p_{p} [GeV/c]");}
    else if(11 ==sl_idx){  sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
    else{sprintf(axisXtitle, "%s", "cos#theta_{#mu}");}
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Uncertainty" );
    total_frac_err_hist->SetTitle(Error_title);
    IncreaseTitleTH1(*total_frac_err_hist, .06);
    double maxx = total_frac_err_hist->GetMaximum();
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,maxx*1.4);
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "Total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if(name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      std::cout << name << " frac err in bin #1 = "
      << hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    std::cout << "Total frac error in bin #1 = "
      << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);


  } 

    c1->cd();
    c1->Clear(); 
    
    MicroBooneTitle = MicroBooneTitle + FullChi_sqted;
    legendMaster->SetHeader(MicroBooneTitle.c_str());
    lg1_Grid->SetHeader(MicroBooneTitle.c_str());
    
    
    legendMaster->Draw();
    c1 -> Print(pdf_title);
  
  
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////
 std::vector<size_t> SliceBins{0,1,2,3,4,5,6,7,8};
 //auto BinVector = GetProjectBinVector();
 auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES, "p_{#mu} [GeV/c]");

 ///////////////////////////////////////////////////
 /// Making Grid Figure here 
 ///////////////////////////////////////////////////
// std::vector<double> WindowZoomScale{2,1,1,1,1,1,2,10,25}; contained
 std::vector<double> WindowZoomScale{2,1,1,1,1,1,2,15,50};

 double min_XAxis_GridCanvas = -1.0;
 double max_XAxis_GridCanvas = 1.0;
 
 double min_YAxis_GridCanvas = 0.0;
 double max_YAxis_GridCanvas = 23.5;
 
 GridCanvas *GC_Stack = new GridCanvas(uniq(), 3, 4, 800, 550);
 GridCanvas *Stack_FracError = new GridCanvas(uniq(), 3, 4, 800, 550);

   GC_Stack->SetBottomMargin(.00);
   GC_Stack->SetTopMargin(.02);
   GC_Stack->SetRightMargin(.01);
   GC_Stack->SetLeftMargin(.08);

   Stack_FracError->SetBottomMargin(.00);
   Stack_FracError->SetTopMargin(.02);
   Stack_FracError->SetRightMargin(.02);

    //TLegend* lg1_Grid= new TLegend( 0.21, 0.02, 0.99, 0.22 );
    //lg1_Grid->SetNColumns(3);
    //lg1_Grid->SetBorderSize(0);
    //lg1_Grid->SetHeader(MicroBooneTitle.c_str());
    //lg1_Grid->SetTextSize(defaultTextSize*1.05); //

  for ( auto sl_idx : SliceBins  )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;
    int GridBins = sl_idx + 1; 
    
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    if(Plot_EXT == true) slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();
      
      if(sl_idx==0){
          //lg1_Grid->AddEntry(slice_bnb->hist_.get(), "Open Data", "pe" );
          auto histclone = (TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
          
           histclone->SetFillColorAlpha(kRed, 0.8);
				   histclone->SetFillStyle(3001);
				   histclone->SetMarkerStyle(0);
				   histclone->SetLineWidth(0);
                    
          //lg1_Grid->AddEntry(histclone, "#muBoonE Tune [Stat+Syst]", "f" );
      }


   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );
      
      if(sl_idx==0){
      std::string lg1_label = eci.label(cat);
      //lg1_Grid->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      }
      
      //std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }

   if(sl_idx==0){

      double amount = slice_ext->hist_.get()->Integral();
     double input = ( amount/ Totalamount ) * 100.0 ; 
     sprintf(LegendMasterTitle, "EXT BNB (%2.1f%)", input );
     //lg1_Grid->AddEntry(slice_ext->hist_.get(),LegendMasterTitle , "f" );
     
     
   }

    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");


   //  std::cout<<"printing chi values "<< std::endl;

      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);

    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    sprintf(axisXtitle, "%s", "");
    double SliceBinWidth = 1.0;
    if(sl_idx < 10) SliceBinWidth = binwidthMap[BinVector.at(sl_idx)];
     bool DoScaledown= true;
      double Scaledown = .001;
    GC_Stack->cd(GridBins); 
    
     DrawStack(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      true,
      SliceBinWidth,
      ymax,
      WindowZoomScale.at(sl_idx),
      Plot_EXT,
      DoScaledown,
      Scaledown
      );
      
    std::cout<<"Zoom In Times " << WindowZoomScale.at(sl_idx) << std::endl;
    
    
     drawString(BinStringMap[BinVector.at(sl_idx)], .038, false );
    
    		if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Stack->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .06,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(62);
			la2->SetTextSize(0.04);
			la2->Draw();
		}
    
    
    
    
  }// End of Canvus Bins
   ///////////////////////////////////////// 
  lg1_Grid->Draw("same");
    
  GC_Stack->SetYLabel_Size(.04);
	GC_Stack->SetXLabel_Size(.04);
  //GC_Stack->SetInterpadSpace(.00);
	  //GC_Stack->SetRightMargin(0.05);
	  //GC_Stack->SetLeftMargin(0.08);
	  
  GC_Stack->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GC_Stack->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
 
	GC_Stack->SetXTitle("cos#theta_{#mu}");
	GC_Stack->SetYTitleSize(22);
	GC_Stack->SetXTitleSize(20);  
	GC_Stack->SetYTitle("NEvents (#times 10^{3}) / [BinWidth]");
	GC_Stack->SetTitleAlignmentFor6Hist();
	GC_Stack->SetYTitle_offset(.1);
  //leg->Draw("SAME");]
  GC_Stack->Modified();
   GC_Stack->ResetPads();
   sprintf(pdf_title, "%s.pdf", Pdf_name2_Data.c_str());
 GC_Stack->Print(pdf_title);
  ///////////////////////////////////////////////////

 GridCanvas *GridCanvas_TotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg1_FracError_Grid= new TLegend(  0.22, 0.015, 0.99, 0.2  );
 lg1_FracError_Grid->SetNColumns(2);
 lg1_FracError_Grid->SetBorderSize(0);
 
   GridCanvas_TotalFracError->SetBottomMargin(.00);
   GridCanvas_TotalFracError->SetTopMargin(.02);
   GridCanvas_TotalFracError->SetRightMargin(.01);
   GridCanvas_TotalFracError->SetLeftMargin(.08);
 
 
 for ( auto sl_idx : SliceBins  )
  {

  const auto& slice = sb.slices_.at( sl_idx );
 
 bool FillLegend = (sl_idx==0) ? true : false;

 int GridBins = sl_idx + 1; 
 
 
 DrawFractionalError_GC(
 matrix_map,
 cov_mat_keys,
 "total",
 reco_mc_plus_ext_hist,
 slice,
 BinStringMap[BinVector.at(sl_idx)],
 1.0,
 GridCanvas_TotalFracError,
 GridBins,
 FillLegend, 
 lg1_FracError_Grid, true);

 }

 std::cout<<"FInished"<< std::endl;
 
 DrawGridCanvas(GridCanvas_TotalFracError,
 lg1_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.333,
 -1., 1. );
 
 
 delete GridCanvas_TotalFracError;
 delete lg1_FracError_Grid; 
 ///////////////////////////
 GridCanvas *GridCanvas_DetFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg2_FracError_Grid= new TLegend(  0.22, 0.015, 0.99, 0.2  );
 lg2_FracError_Grid->SetNColumns(2);
 lg2_FracError_Grid->SetBorderSize(0);
    GridCanvas_DetFracError->SetBottomMargin(.00);
   GridCanvas_DetFracError->SetTopMargin(.02);
   GridCanvas_DetFracError->SetRightMargin(.01);
   GridCanvas_DetFracError->SetLeftMargin(.08);

 for ( auto sl_idx : SliceBins  )
  {

  const auto& slice = sb.slices_.at( sl_idx );
 
 bool FillLegend = (sl_idx==0) ? true : false;

 int GridBins = sl_idx + 1; 


  DrawFractionalError_GC(
  matrix_map,
  cov_mat_keys_detVar,
  "detVar_total",
  reco_mc_plus_ext_hist,
  slice,
  BinStringMap[BinVector.at(sl_idx)],
  1.0,
  GridCanvas_DetFracError,
  GridBins,
  FillLegend, 
  lg2_FracError_Grid);
  
 }
 
 DrawGridCanvas(GridCanvas_DetFracError,
 lg2_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.258,
 -1., 1. );
 
 delete GridCanvas_DetFracError;
 delete lg2_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg3_FracError_Grid= new TLegend(  0.22, 0.015, 0.99, 0.2  );
 lg3_FracError_Grid->SetNColumns(2);
 lg3_FracError_Grid->SetBorderSize(0);
   GridCanvas_CrossFracError->SetBottomMargin(.00);
   GridCanvas_CrossFracError->SetTopMargin(.02);
   GridCanvas_CrossFracError->SetRightMargin(.01);
   GridCanvas_CrossFracError->SetLeftMargin(.08);
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
   DrawFractionalError_GC(
   matrix_map,
   cov_mat_keys_cross,
   "xsec_unisim",
   reco_mc_plus_ext_hist,
   slice,
   BinStringMap[BinVector.at(sl_idx)],
   1.0,
   GridCanvas_CrossFracError,
   GridBins,
   FillLegend, 
   lg3_FracError_Grid);
   
   }
 
 DrawGridCanvas(GridCanvas_CrossFracError,
 lg3_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.0888,
 -1.0, 1.0 );
 
 delete GridCanvas_CrossFracError;
 delete lg3_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossTotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg4_FracError_Grid= new TLegend( 0.22, 0.012, 0.99, 0.2  );
 lg4_FracError_Grid->SetNColumns(2);
 lg4_FracError_Grid->SetBorderSize(0);
    GridCanvas_CrossTotalFracError->SetBottomMargin(.00);
   GridCanvas_CrossTotalFracError->SetTopMargin(.02);
   GridCanvas_CrossTotalFracError->SetRightMargin(.01);
   GridCanvas_CrossTotalFracError->SetLeftMargin(.08);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
   DrawFractionalError_GC(
   matrix_map,
   cov_mat_key_totalsumcross,
   "xsec_total",
   reco_mc_plus_ext_hist,
   slice,
   BinStringMap[BinVector.at(sl_idx)],
   1.0,
   GridCanvas_CrossTotalFracError,
   GridBins,
   FillLegend, 
   lg4_FracError_Grid);
   
   }
 
 DrawGridCanvas(GridCanvas_CrossTotalFracError,
 lg4_FracError_Grid, "cos#theta_{#mu}", 
 "Fractional Error", pdf_title,
 .0,.333,
 -1., 1.0 );
 
 delete GridCanvas_CrossTotalFracError;
 delete lg4_FracError_Grid; 
 
 
    // ENd of 
     /////////////////////
     // ENd of slices 
     ////////////////////
 
    sprintf(pdf_title, "%s.pdf)", Pdf_name2_Data.c_str());
    c1 -> Print(pdf_title);
 
 
}/////End of tutorial_slice_plots_2D
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void tutorial_slice_plots_2D_inclusive_withData() {

   std::cout<<"Starting tutorial_slice_plots 2D"<< std::endl;
  #ifdef USE_FAKE_DATA
    // Initialize the FilePropertiesManager and tell it to treat the NuWro
    // MC ntuples as if they were data
    auto& fpm = FilePropertiesManager::Instance();
    std::cout<<"OutPut: Path : " << fpm.analysis_path()<< std::endl;
    
    std::cout<<"Finished:FilePropertiesManager::Instance() "<<std::endl; 
    std::cout<<"trying to apply load_file_properties"<< std::endl;
    fpm.load_file_properties( "file_properties.txt" );
    std::cout<<" passed "<< std::endl;
  #endif

 fpm.load_file_properties( "file_properties_May10_OpenData.txt" );
 
 //
 
 //
 char axisXtitle[1024];
 /// Taking from my area
 //  uboone/data/users/cnguyen/RootFiles_tutorial/tutorial_univmake_output.root
  bool Plot_EXT = true; 
 std::cout<<"Created syst_ptr a  MCC9SystematicsCalculator"<< std::endl;

  auto* syst_ptr = new MCC9SystematicsCalculator(
  "/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_8_16_2024/unimak_Scheme2_MCS_pmu_PC_Open.root"
    , "systcalc_noNuWro_eventrates.conf" );
    //UnivMake_scheme2_pionSidebandCondition_Muons_contained_v1.root
    // UniMake_scheme2_MCS_FC_Open.root
//"/exp/uboone/data/users/cnguyen/CC0Pi_Selection/Anaylzer_unimakeoutputPanos/Unimake_BinningScheme2.root"
////exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_8_16_2024/univmake_FCMuons_scheme2_v5_newBinning.root
//"
  //
  //
  // unimake_MCS_pmu_contained_v2_OPEN.root
// 
// Unimak_scheme2_RANGEPmu_uncontained_v3_Open.root
// UniMake_scheme1_RANGE_Contained_v2_OPEN.root
  
  
  //UnivMake_scheme1_PC_muonsOnly.root
  //unimake_MCS_pmu_contained_v2.root
// unimak_MCS_pmu_uncontained_v2.root
    
    ///exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_2_27_24_PmuCorrection/UnivMake_FakeData_BDTdecided_2D_inclusive_v7_pmucorrection.root
  auto& syst = *syst_ptr;
 //UnivMake_FakeData_BDTdecided_2D_inclusive_v3_pmucorrection.root
 std::cout<<"Intizing Done:   MCC9SystematicsCalculator "<< std::endl; 

char LegendMasterTitle[1024];
  // Get access to the relevant histograms owned by the SystematicsCalculator
  // object. These contain the reco bin counts that we need to populate the
  // slices below.
  // Notes :: I think everything is projected by bin Number in 1D and the slicer will get it 
double Totalamount = 9999; 
double Totalchisqrt = 9999; 
double Totalpvalue = 9999; 
int TotalNDF = 9999; 
char words_char[1024];
auto binwidthMap = Projection9Bins_width(MUON_2D_BIN_EDGES_inclusive);
auto BinVector = GetProjectBinVector();
  TH1D* reco_bnb_hist = syst.data_hists_.at( NFT::kOnBNB ).get();
  TH1D* reco_ext_hist = syst.data_hists_.at( NFT::kExtBNB ).get();
   
  std::cout<<"Total N Data = "<< reco_bnb_hist->Integral()<<std::endl;
 std::cout<< " reco_bnb_hist Nbins = "<< reco_bnb_hist->GetNbinsX()<< std::endl; 



 std::cout<<"added reco bnb to ext "<< std::endl;
  TH2D* category_hist = syst.cv_universe().hist_categ_.get();
 std::cout<<"Finished  category_hist "<< std::endl; 
  // Total MC+EXT prediction in reco bin space. Start by getting EXT.
  TH1D* reco_mc_plus_ext_hist = dynamic_cast< TH1D* >(
    reco_ext_hist->Clone("reco_mc_plus_ext_hist") );
  reco_mc_plus_ext_hist->SetDirectory( nullptr );


  // Add in the CV MC prediction
  reco_mc_plus_ext_hist->Add( syst.cv_universe().hist_reco_.get() );

  // Keys are covariance matrix types, values are CovMatrix objects that
  // represent the corresponding matrices
  auto* matrix_map_ptr = syst.get_covariances().release();
  //auto& matrix_map = *matrix_map_ptr;

   CovMatrixMap &matrix_map = *matrix_map_ptr;

  std::cout<<"Making slice from inclusive "<< std::endl;

  auto* sb_ptr = new SliceBinning( "mybins_mcc9_2D_muon_inclusive_v1_july3_2024.txt" ); //tutorial_reco_slice_config.txt
  auto& sb = *sb_ptr;
//
 // mybins_mcc9_2D_muon_inclusive_v1_Oct16_2024.txt
  ////////////////////////////////
  // Starting to plot
  //////////////////////////////////

  std::cout<< " Number of Slices to Plot : "<< sb.slices_.size() << std::endl;
   
  TCanvas *c1 = new TCanvas("c1");
  sprintf(pdf_title, "%s.pdf(",  Pdf_name2D_inclusive_Data.c_str());
  c1 -> Print(pdf_title);

  //std::vector<std::string> xaxistitles_vector{"Cos#theta_{#mu}","p_{#mu} [GeV/c]", "Bin N" };
   int BinNSlice = 9; 
	TLegend *legendMaster = new TLegend(0.05, 0.05, 0.98, 0.98);
	std::string MicroBooneTitle = MicroBooNE_LegendTitle_OpenData();
	//legendMaster->SetHeader(MicroBooneTitle.c_str());
	legendMaster->SetNColumns(2);
	legendMaster->SetBorderSize(0);
	std::string FullChi_sqted; 
	
	  TLegend* lg1_Grid= new TLegend(0.22, 0.01, 0.99, 0.2 );
    lg1_Grid->SetNColumns(3);
    lg1_Grid->SetBorderSize(0);
    //lg1_Grid->SetHeader(MicroBooneTitle.c_str());
	



  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;

    TLegend* lg1_stacked = new TLegend( 0.32, 0.65, 0.8, 0.88 );
    lg1_stacked->SetNColumns(2);
    lg1_stacked->SetBorderSize(0);
    Double_t defaultTextSize = lg1_stacked->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize << std::endl;
    lg1_stacked->SetTextSize(defaultTextSize*1.05); //
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );

   

    if(BinNSlice==sl_idx){
    Totalamount = slice_mc_plus_ext->hist_.get()->Integral() +slice_ext->hist_.get()->Integral() ;
    }



    auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
    slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();

    lg1_stacked->AddEntry(slice_bnb->hist_.get(), "Open Data", "pe" );
    lg1_stacked->AddEntry(slice_mc_plus_ext->hist_.get(), "Total MC", "le" );

    if(BinNSlice==sl_idx){
         legendMaster->AddEntry(slice_bnb->hist_.get(), "Open Data [Stat]", "pe" );
         legendMaster->AddEntry(slice_mc_plus_ext->hist_.get(), "#muBooNE Tune [Sys+Stat]", "le" );
         
       TH1D*test =  (TH1D*)slice_bnb->hist_.get()->Clone();
       lg1_Grid->AddEntry(test, "Open Data [Stat]", "pe" );
        auto histclone = (TH1D*)slice_mc_plus_ext->hist_.get()->Clone(uniq());
          
        histclone->SetFillColorAlpha(kRed, 0.8);
				histclone->SetFillStyle(3001);
				histclone->SetMarkerStyle(0);
				histclone->SetLineWidth(0);
                 
                 
                 
       lg1_Grid->AddEntry(histclone, "Total MC [Stat+Syst]", "f" );
       
       

    }


   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );

      std::string lg1_label = eci.label(cat);
      lg1_stacked->AddEntry(temp_slice_mc->hist_.get(),lg1_label.c_str() , "f" );
      std::string cat_col_prefix = "MC" + std::to_string( cat );

     if(BinNSlice==sl_idx){
     double amount = temp_slice_mc->hist_.get()->Integral();
     double input = ( amount/ Totalamount ) * 100.0 ; 
     sprintf(LegendMasterTitle, "%s (%2.1f%)",lg1_label.c_str(), input );
     legendMaster->AddEntry(temp_slice_mc->hist_.get(),LegendMasterTitle , "f" );
      TH1D*test2 =  (TH1D*)temp_slice_mc->hist_.get()->Clone();
        sprintf(LegendMasterTitle, "%s (%2.1f%)",lg1_label.c_str(), input );
         lg1_Grid->AddEntry(test2,LegendMasterTitle , "f" );
     
     } 



      --cat_bin_index;
    }


    lg1_stacked->AddEntry(slice_ext->hist_.get(),"EXT BNB" , "f" );


   if(BinNSlice==sl_idx){
    double amount = slice_ext->hist_.get()->Integral();
     double input = ( amount/ Totalamount ) * 100.0 ; 
     sprintf(LegendMasterTitle, "EXT BNB (%2.1f%)", input );
     legendMaster->AddEntry(slice_ext->hist_.get(),LegendMasterTitle , "f" );
     
      TH1D*test3 =  (TH1D*)slice_ext->hist_.get()->Clone();
     lg1_Grid->AddEntry(test3,LegendMasterTitle, "f" );
     
  }

    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.85;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );

    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");

    slice_bnb->hist_->Draw( "e" );
    if(sl_idx<9) slice_bnb->hist_->GetXaxis()->SetRangeUser( 0.1, 2.0 ); // Setting X range for inclusive 0,2 GeV
    slice_pred_stack->Draw( "hist same" );
    const char* Error_title = slice_bnb->hist_->GetTitle();
    slice_mc_plus_ext->hist_->SetLineWidth( 3 );
    slice_mc_plus_ext->hist_->Draw( "same hist e" );

    slice_bnb->hist_->Draw( "same e" );

    lg1_stacked->Draw( "same" );
   //  std::cout<<"printing chi values "<< std::endl;
    TLatex* text = new TLatex;
      text->SetNDC();
      text->SetTextSize(0.03);
      text->SetTextColor(kRed);
      sprintf(textplace, "#chi^{2}/ndf: %.2f/%i",chi2_result.chi2_ , chi2_result.num_bins_ );
      text->DrawLatex(0.15, 0.85, textplace);
      if(BinNSlice==sl_idx){
      FullChi_sqted = ": " + std::string(textplace); 
      }
      sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      text->DrawLatex(0.15, 0.80, textplace);
      if(BinNSlice==sl_idx){
      FullChi_sqted = FullChi_sqted  + " : " + std::string(textplace); 
      }
            
      sprintf(pdf_title, "%s.pdf",  Pdf_name2D_inclusive_Data.c_str());
      c1 -> Print(pdf_title);
    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");

    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());
    if(9 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else{sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
    double SliceBinWidth = 1.0;
   if(sl_idx<9 ){SliceBinWidth = binwidthMap[BinVector.at(sl_idx)];}
  
    IncreaseTitleTH1(*h_bnb_input, .07);
    
    h_bnb_input->GetXaxis()->SetRangeUser( .1, 2.0 );
    
        double chi1;
       double pvalue;
         int NDF;
         
    DrawStack_WithRatio(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
       Pdf_name2D_inclusive_Data,
      "Test",
      axisXtitle,
      c1,
      ymax,
       Plot_EXT, 
       chi1,
       pvalue,
      NDF,
      SliceBinWidth);
     /*
      if (NDF == 43){
      Totalchisqrt = chi1;
      Totalpvalue = pvalue;
      TotalNDF = NDF;
      

       	sprintf( words_char ,     "#chi^{2}/ndf = %.2f / %i",Totalchisqrt, TotalNDF);
				lg1_Grid->AddEntry((TObject*)0,  words_char, ""); // to put fit param
        	sprintf( words_char ,     "Pvalue = %.3f",Totalpvalue);
				lg1_Grid->AddEntry((TObject*)0,  words_char, ""); // to put fit param
      
      
      }
*/





    ////////////////////////////////////////
    ///// Finished with First Plot
    ////////////////////////////////////////

   //if(sl_idx==3)continue; 

    //std::cout<<"Starting Making error Plot  "<< std::endl;

    TH1* slice_hist = dynamic_cast< TH1* >(
      slice.hist_->Clone("slice_hist") );

      slice_hist->SetDirectory( nullptr );

    // Keys are labels, values are fractional uncertainty histograms
    auto* fr_unc_hists = new std::map< std::string, TH1* >();
    auto& frac_uncertainty_hists = *fr_unc_hists;

    // Show fractional uncertainties computed using these covariance matrices
    // in the ROOT plot. All configured fractional uncertainties will be
    // included in the output pgfplots file regardless of whether they appear
    // in this vector.

   //  std::cout<<"Looping over the various systematic uncertainties "<< std::endl;
    int loop_count=0;
    // Loop over the various systematic uncertainties
    int color = 1;
    for ( const auto& pair : matrix_map )
      {
      //std::cout<< "loop_count = " << loop_count<< std::endl;
      loop_count++;
      const auto& key = pair.first;
      const auto& cov_matrix = pair.second;

      SliceHistogram* slice_for_syst = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &cov_matrix );

      // The SliceHistogram object already set the bin errors appropriately
      // based on the slice covariance matrix. Just change the bin contents
      // for the current histogram to be fractional uncertainties. Also set
      // the "uncertainties on the uncertainties" to zero.
      // TODO: revisit this last bit, possibly assign bin errors here
      //std::cout<<" this loop starts for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      for ( const auto& bin_pair : slice.bin_map_ )
      {
        int global_bin_idx = bin_pair.first;
        double y = slice_for_syst->hist_->GetBinContent( global_bin_idx );
        double err = slice_for_syst->hist_->GetBinError( global_bin_idx );
        double frac = 0.;
        if ( y > 0. ) frac = err / y;
        slice_for_syst->hist_->SetBinContent( global_bin_idx, frac );
        slice_for_syst->hist_->SetBinError( global_bin_idx, 0. );
      }
      //std::cout<<" this loop Ends for ( const auto& bin_pair : slice.bin_map_ )"<< std::endl;
      // Check whether the current covariance matrix name is present in
      // the vector defined above this loop. If it isn't, don't bother to
      // plot it, and just move on to the next one.
      
      
      auto cbegin = cov_mat_keys.cbegin();
      auto cend = cov_mat_keys.cend();
      auto iter = std::find( cbegin, cend, key );
      if ( iter == cend ) continue;
      
      //std::cout<<"Universe : "<< key<< std::endl;
      
      frac_uncertainty_hists[ key ] = slice_for_syst->hist_.get();

            ++color;
          if ( color == 3 ) color = kGreen +2;
          if ( color == kGreen + 3) color = 4;
          if ( color == 5 ) color = 6;
          if ( color == 7 ) color = kYellow -2;
          if ( color == kYellow -1 ) color = kMagenta + 2;
          if (color == kMagenta + 3) color = kSpring - 2;
          if (color == kSpring - 1) color = kOrange + 2;
          if (color == kOrange + 3) color = kRed - 6;
          if (color == kRed - 5) color = kAzure + 8;
          if (color == kAzure + 8) color = kOrange - 3;
      slice_for_syst->hist_->SetLineColor( ColorMap[key] );
      slice_for_syst->hist_->SetLineWidth( 3 );

    }

     //TCanvas* c2 = new TCanvas;
    TLegend* lg2 = new TLegend( 0.3, 0.6, 0.82, 0.88 );
    lg2->SetNColumns(3);
    lg2->SetBorderSize(0);
    //Double_t defaultTextSize_lg2 = lg2->GetTextSize();
    //std::cout<<"set text size "<< defaultTextSize_lg2 << std::endl;
    //lg2->SetTextSize(defaultTextSize_lg2*1.05);

   //  std::cout<<" trying to call frac_uncertainty_hists.at( Total Error ) " << std::endl;
    auto* total_frac_err_hist = frac_uncertainty_hists.at( "total" );
   // auto* total_frac_err_hist = frac_uncertainty_hists.at( "xsec_unisim" );
    //  std::cout<<" Finshed " << std::endl;
    //frac_uncertainty_hists.at( "BNBstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "EXTstats" )->SetLineStyle(2);
    frac_uncertainty_hists.at( "MCstats" )->SetLineStyle(2);
    total_frac_err_hist->SetStats( false );
    //total_frac_err_hist->SetMaximum(total_frac_err_hist->GetMaximum() * 1.5);
    total_frac_err_hist->SetMinimum(0.0);
    total_frac_err_hist->SetMaximum(.45);
    total_frac_err_hist->SetLineColor( kBlack );
    total_frac_err_hist->SetLineWidth( 3 );
    if(9 ==sl_idx || 12 == sl_idx  ){  sprintf(axisXtitle, "%s", "Bin N");}
    else if(10 ==sl_idx){  sprintf(axisXtitle, "%s", "p_{p} [GeV/c]");}
    else if(11 ==sl_idx){sprintf(axisXtitle, "%s", "cos#theta_{#mu}");}
    else{sprintf(axisXtitle, "%s", "p_{#mu} [GeV/c]");}
    total_frac_err_hist->GetXaxis()->SetTitle( axisXtitle);
    total_frac_err_hist->GetYaxis()->SetTitle( "Fractional Error" );
    total_frac_err_hist->SetTitle(Error_title);
    IncreaseTitleTH1(*total_frac_err_hist, .06);
    double maxx = total_frac_err_hist->GetMaximum();
    total_frac_err_hist->GetYaxis()->SetRangeUser( 0.,maxx*1.4);
    
    
    
    total_frac_err_hist->Draw( "hist" );

    lg2->AddEntry( total_frac_err_hist, "Total", "l" );

   //  std::cout<<"starting error leg names"<<std::endl;
    //////////////////////////////////  
    /// Drawing the Sysmatics 
    //////////////////////////////////  
    for ( auto& pair : frac_uncertainty_hists )
    {
      const auto& name = pair.first;
      TH1* hist = pair.second;
      // We already plotted the "total" one above
      if ( name == "total" ) continue;

      lg2->AddEntry( hist, name.c_str(), "l" );
      hist->Draw( "same hist" );

      //std::cout << name << " frac err in bin #1 = "
      //<< hist->GetBinContent( 1 )*100. << "%\n";
    }
    //std::cout<<"END error leg names"<<std::endl;
    lg2->Draw( "same" );

    //std::cout << "Total frac error in bin #1 = "
    //  << total_frac_err_hist->GetBinContent( 1 )*100. << "%\n";
      //sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
      c1 -> Print(pdf_title);


  } 
  
    c1->cd();
    c1->Clear(); 
    
    MicroBooneTitle = MicroBooneTitle + FullChi_sqted;
    legendMaster->SetHeader(MicroBooneTitle.c_str());
    lg1_Grid->SetHeader(MicroBooneTitle.c_str());
    
    
    legendMaster->Draw();
    c1 -> Print(pdf_title);
  
  
  
  //////////////////////////////////
  // End of slices Loop 
  //////////////////////////////////
 std::vector<size_t> SliceBins{0,1,2,3,4,5,6,7,8};
// 
 auto BinStringMap = Projection9Bins_StringMap(MUON_2D_BIN_EDGES_inclusive, "cos#theta_{#mu}");

 ///////////////////////////////////////////////////
 /// Making Grid Figure here 
 ///////////////////////////////////////////////////
 //std::vector<double> WindowZoomScale{5,2,2,1,2,1,1,1,1}; contained
std::vector<double> WindowZoomScale{5,2,2,2,2,1,1,1,1};

 double min_XAxis_GridCanvas = .1;
 double max_XAxis_GridCanvas = 2.0;
 
 double min_YAxis_GridCanvas = 0.0;
 double max_YAxis_GridCanvas = 35.5;
 
 double Scaledown = .001;
 bool DoScaledown = true; 
 
 

 
 GridCanvas *GC_Stack = new GridCanvas(uniq(), 3, 4, 800, 550);
   
   GC_Stack->SetBottomMargin(.00);
   GC_Stack->SetTopMargin(.02);
   GC_Stack->SetRightMargin(.01);
   GC_Stack->SetLeftMargin(.08);

    //lg1_Grid->SetTextSize(defaultTextSize*1.05); //

  for ( auto sl_idx : SliceBins  )
  {
    std::cout<<"SLICE : "<< sl_idx <<std::endl;
    int GridBins = sl_idx + 1; 
    
    const auto& slice = sb.slices_.at( sl_idx );

    // We now have all of the reco bin space histograms that we need as input.
    // Use them to make new histograms in slice space.
    SliceHistogram* slice_bnb = SliceHistogram::make_slice_histogram(
      *reco_bnb_hist, slice, &matrix_map.at("BNBstats") );

    SliceHistogram* slice_ext = SliceHistogram::make_slice_histogram(
      *reco_ext_hist, slice, &matrix_map.at("EXTstats") );

    SliceHistogram* slice_mc_plus_ext = SliceHistogram::make_slice_histogram(
      *reco_mc_plus_ext_hist, slice, &matrix_map.at("total") );


    //auto chi2_result = slice_bnb->get_chi2( *slice_mc_plus_ext );
    //std::cout << "Slice " << sl_idx << ": \u03C7\u00b2 = "
    //  << chi2_result.chi2_ << '/' << chi2_result.num_bins_ << " bins,"
    //  << " p-value = " << chi2_result.p_value_ << '\n';


    // Build a stack of categorized central-value MC predictions plus the
    // extBNB contribution in slice space
    const auto& eci = EventCategoryInterpreter::Instance();
    eci.set_ext_histogram_style( slice_ext->hist_.get() );

    THStack* slice_pred_stack = new THStack( "mc+ext", "" );
     slice_pred_stack->Add( slice_ext->hist_.get() ); // extBNB


    const auto& cat_map = eci.label_map();

    // Go in reverse so that signal ends up on top. Note that this index is
    // one-based to match the ROOT histograms
    int cat_bin_index = cat_map.size();
      
   //  std::cout<<"stack Loop removed r crbegin() crend() "<< std::endl;
    for ( auto iter = cat_map.crbegin(); iter != cat_map.crend(); ++iter )
    {
      EventCategory cat = iter->first;
      TH1D* temp_mc_hist = category_hist->ProjectionY( "temp_mc_hist",
        cat_bin_index, cat_bin_index );
      temp_mc_hist->SetDirectory( nullptr );

      SliceHistogram* temp_slice_mc = SliceHistogram::make_slice_histogram(
        *temp_mc_hist, slice  );

      eci.set_mc_histogram_style( cat, temp_slice_mc->hist_.get() );

      slice_pred_stack->Add( temp_slice_mc->hist_.get() );
      
      
      //std::string cat_col_prefix = "MC" + std::to_string( cat );

      --cat_bin_index;
    }

    slice_bnb->hist_->SetLineColor( kBlack );
    slice_bnb->hist_->SetLineWidth( 3 );
    slice_bnb->hist_->SetMarkerStyle( kFullCircle );
    slice_bnb->hist_->SetMarkerSize( 0.8 );
    slice_bnb->hist_->SetStats( false );
    double ymax = std::max( slice_bnb->hist_->GetMaximum(),
      slice_mc_plus_ext->hist_->GetMaximum() ) * 1.6;
    slice_bnb->hist_->GetYaxis()->SetRangeUser( 0., ymax );
    slice_bnb->hist_->GetYaxis()->SetTitle( "NEvent" );
    slice_bnb->hist_->SetTitleOffset (1.01,"Y");


   //  std::cout<<"printing chi values "<< std::endl;

      //sprintf(textplace, "#chi^{2}/ndf = %.2f / %i",chi2_result.chi2_ , chi2_result.num_bins_ );
      //text->DrawLatex(0.15, 0.85, textplace);

      //sprintf(textplace, "p-value = %.4f",chi2_result.p_value_ );
      //text->DrawLatex(0.15, 0.80, textplace);

    //  std::cout<<"FINISHED printing chi values "<< std::endl;

    // Get the binning and axis labels for the current slice by cloning the
    // (empty) histogram owned by the Slice object
    /////////////////////////////////////
    // testing Drawing Function
    ////////////////////////////////////
    TH1D* h_bnb_input = (TH1D*)reco_bnb_hist->Clone("slice_bnb_input");
    TH1D* h_mc_plus_ext_input = (TH1D*)reco_mc_plus_ext_hist->Clone("slice_mc_plus_ext_input");
    TH1D* h_ext_input = (TH1D*)reco_ext_hist->Clone("slice_ext_input");


    //char axisXtitle = reco_mc_plus_ext_hist->GetXaxis()->GetTitle();
   //sprintf(axisXtitle, "%s", xaxistitles_vector.at(sl_idx).c_str());

    
    GC_Stack->cd(GridBins); 
    
     DrawStack(
      category_hist,
      h_bnb_input,
      h_mc_plus_ext_input,
      h_ext_input,
       slice_bnb,
       slice_ext,
       slice_mc_plus_ext,
      slice,
      true,
      binwidthMap[BinVector.at(sl_idx)],
      ymax,
      WindowZoomScale.at(sl_idx), 
      Plot_EXT,
      DoScaledown,
      Scaledown
      );
    
   
     drawString(BinStringMap[BinVector.at(sl_idx)], .038, false );
    
    		if ( WindowZoomScale.at(sl_idx) != 1)
		{
			auto pad = GC_Stack->cd(GridBins);
			TLatex *la2 = new TLatex(1 - pad->GetRightMargin() - 0.02,
				1 - pad->GetTopMargin() - .1,
				TString::Format("#times %.1f", WindowZoomScale.at(sl_idx)));
			la2->SetTextAlign(33);	// top right
			la2->SetNDC();
			la2->SetTextFont(62);
			la2->SetTextSize(0.04);
			la2->Draw();
		}
    
     
    
  }// End of Canvus Bins
   ///////////////////////////////////////// 
  lg1_Grid->Draw("same");
    
  GC_Stack->SetYLabel_Size(.04);
	GC_Stack->SetXLabel_Size(.04);
  //GC_Stack->SetInterpadSpace(.005);
	  //GC_Stack->SetRightMargin(0.05);
	  //GC_Stack->SetLeftMargin(0.08);
	GC_Stack->SetYTitle_offset(0.5);
  GC_Stack->SetYLimits(min_YAxis_GridCanvas, max_YAxis_GridCanvas);
  GC_Stack->SetXLimits(min_XAxis_GridCanvas, max_XAxis_GridCanvas);
  GC_Stack->ResetPads();
	GC_Stack->SetXTitle("p_{#mu} [GeV/c]");
	GC_Stack->SetYTitleSize(22);
	GC_Stack->SetXTitleSize(20);  
	GC_Stack->SetYTitle("NEvents (#times 10^{3}) / [BinWidth]");
	GC_Stack->SetTitleAlignmentFor6Hist();
  //leg->Draw("SAME");
  
  GC_Stack->Modified();
	GC_Stack->ResetPads();

   sprintf(pdf_title, "%s.pdf",  Pdf_name2D_inclusive_Data.c_str());
 GC_Stack->Print(pdf_title);
  // ENd of 
   delete GC_Stack;
  /////////////////////
  // ENd of slices 
  ////////////////////

 GridCanvas *GridCanvas_TotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg1_FracError_Grid= new TLegend(0.22, 0.01, 0.99, 0.2  );
 lg1_FracError_Grid->SetNColumns(2);
 lg1_FracError_Grid->SetBorderSize(0);
   GridCanvas_TotalFracError->SetBottomMargin(.00);
   GridCanvas_TotalFracError->SetTopMargin(.02);
   GridCanvas_TotalFracError->SetRightMargin(.01);
   GridCanvas_TotalFracError->SetLeftMargin(.08);
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
 DrawFractionalError_GC(
 matrix_map,
 cov_mat_keys,
 "total",
 reco_mc_plus_ext_hist,
 slice,
 BinStringMap[BinVector.at(sl_idx)],
 1.0,
 GridCanvas_TotalFracError,
 GridBins,
 FillLegend, 
 lg1_FracError_Grid, true);
 
 }

 std::cout<<"FInished"<< std::endl;
 
 
 DrawGridCanvas(GridCanvas_TotalFracError,
 lg1_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.78,
 .1, 2.0 );
 
 
 delete GridCanvas_TotalFracError;
 delete lg1_FracError_Grid; 
 ///////////////////////////
 GridCanvas *GridCanvas_DetFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg2_FracError_Grid= new TLegend( 0.22, 0.01, 0.99, 0.2  );
 lg2_FracError_Grid->SetNColumns(2);
 lg2_FracError_Grid->SetBorderSize(0);
 
   GridCanvas_DetFracError->SetBottomMargin(.00);
   GridCanvas_DetFracError->SetTopMargin(.02);
   GridCanvas_DetFracError->SetRightMargin(.01);
   GridCanvas_DetFracError->SetLeftMargin(.08);
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 
 
 
   DrawFractionalError_GC(
   matrix_map,
   cov_mat_keys_detVar,
   "detVar_total",
   reco_mc_plus_ext_hist,
   slice,
   BinStringMap[BinVector.at(sl_idx)],
   1.0,
   GridCanvas_DetFracError,
   GridBins,
   FillLegend, 
   lg2_FracError_Grid);
  
  }
  GridCanvas_DetFracError->SetYLabel_Size(.04);
	GridCanvas_DetFracError->SetXLabel_Size(.04);
 DrawGridCanvas(GridCanvas_DetFracError,
 lg2_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.33,
 .1, 2.0 );
 
 delete GridCanvas_DetFracError;
 delete lg2_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg3_FracError_Grid= new TLegend( 0.22, 0.01, 0.99, 0.2 );
 lg3_FracError_Grid->SetNColumns(2);
 lg3_FracError_Grid->SetBorderSize(0);
 
    GridCanvas_CrossFracError->SetBottomMargin(.00);
   GridCanvas_CrossFracError->SetTopMargin(.02);
   GridCanvas_CrossFracError->SetRightMargin(.01);
   GridCanvas_CrossFracError->SetLeftMargin(.08);
 
 
 for ( auto sl_idx : SliceBins  )
   {
 
   const auto& slice = sb.slices_.at( sl_idx );
  
  bool FillLegend = (sl_idx==0) ? true : false;
 
 int GridBins = sl_idx + 1; 


  DrawFractionalError_GC(
  matrix_map,
  cov_mat_keys_cross,
  "xsec_unisim",
  reco_mc_plus_ext_hist,
  slice,
  BinStringMap[BinVector.at(sl_idx)],
  1.0,
  GridCanvas_CrossFracError,
  GridBins,
  FillLegend, 
  lg3_FracError_Grid);
  
  }

  GridCanvas_CrossFracError->SetYLabel_Size(.04);
	GridCanvas_CrossFracError->SetXLabel_Size(.04);

 DrawGridCanvas(GridCanvas_CrossFracError,
 lg3_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.17,
 .1, 2.0 );
 
 delete GridCanvas_CrossFracError;
 delete lg3_FracError_Grid; 
 ////////////////////////////////////////
 GridCanvas *GridCanvas_CrossTotalFracError = new GridCanvas(uniq(), 3, 4, 800, 550);
 TLegend* lg4_FracError_Grid= new TLegend( 0.22, 0.01, 0.99, 0.2  );
 lg4_FracError_Grid->SetNColumns(2);
 lg4_FracError_Grid->SetBorderSize(0);
     GridCanvas_CrossTotalFracError->SetBottomMargin(.00);
   GridCanvas_CrossTotalFracError->SetTopMargin(.02);
   GridCanvas_CrossTotalFracError->SetRightMargin(.01);
   GridCanvas_CrossTotalFracError->SetLeftMargin(.08);


 for ( auto sl_idx : SliceBins  )
  {

  const auto& slice = sb.slices_.at( sl_idx );
 
 bool FillLegend = (sl_idx==0) ? true : false;

 int GridBins = sl_idx + 1; 


  DrawFractionalError_GC(
  matrix_map,
  cov_mat_key_totalsumcross,
  "xsec_total",
  reco_mc_plus_ext_hist,
  slice,
  BinStringMap[BinVector.at(sl_idx)],
  1.0,
  GridCanvas_CrossTotalFracError,
  GridBins,
  FillLegend, 
  lg4_FracError_Grid);
  
  }


  GridCanvas_CrossTotalFracError->SetYLabel_Size(.04);
	GridCanvas_CrossTotalFracError->SetXLabel_Size(.04);

 DrawGridCanvas(GridCanvas_CrossTotalFracError,
 lg4_FracError_Grid, "p_{#mu} [GeV/c]", 
 "Fractional Error", pdf_title,
 .0,.33,
 .1, 2.0 );
 
 delete GridCanvas_CrossTotalFracError;
 delete lg4_FracError_Grid; 



 //////////////////////////////////////////////////////////////////////////////
 ///  End of PDF print
 //////////////////////////////////////////////////////////////////////////////
    sprintf(pdf_title, "%s.pdf)",  Pdf_name2D_inclusive_Data.c_str());
   c1 -> Print(pdf_title);
}/////End of tutorial_slice_plots_2D
//////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////
void IncreaseTitleTH1(TH1& hist, double input) {
    // Increase title size and center it
    gStyle->SetTitleSize(input, "t"); // Adjust the size as needed
    gStyle->SetTitleX(0.5); // Center the title horizontally
}
