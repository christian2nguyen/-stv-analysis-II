// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/JOINTCC0Pi_WCBinScheme.hh"

JOINTCC0Pi_WCBinScheme::JOINTCC0Pi_WCBinScheme() : BinSchemeBase( "JOINTCC0Pi_WCBinScheme" ) {}

void JOINTCC0Pi_WCBinScheme::DefineBlocks() {

  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "stv_tree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1 };

  // Prefix for the output bin and slice configuration text files
  out_config_prefix_ = "JOINTCC0pi_WC_Pmu_WithSideBandsIII_Includeuncontainedmuons";

  // Selection to use with this binning scheme
  selection_name_ = "JOINTCC0pi";

  // TDirectory file name to use when producing the univmake output histograms
  out_tdir_name_ = "JOINTCC0pi";

  /////// Define the blocks of bins in both true and reco space
  std::string branchexpr_reco_pmu,
              branchexpr_true_pmu, 
              branchexpr_reco_costheta,
              branchexpr_true_costheta,
              branchexpr_reco_Nproton,
              branchexpr_true_Nproton,
              branchexpr_reco_2D,
              branchexpr_true_2D,
              branchexpr_sideband, 
              title, textitle,
              selection_true, selection_reco;

/*
sel_CC0pi_wc = 
    sel_nu_mu_cc_
    && sel_no_reco_showers_
    && sel_muon_passed_wc_mom_cuts_ 
    && sel_muon_passed_wc_costheta_cut_
    && !sel_has_pion_wc_candidate_
    && sel_muon_contained_
    && !sel_flippedTrack_
    && sel_n_bdt_other_ == 0 && sel_n_bdt_invalid_ == 0;

    
    


*/


  const std::string CC0pi_SELECTION_include_uncontained =
    "sel_nu_mu_cc"
    " && sel_muon_passed_wc_mom_cuts" 
    " && sel_muon_passed_wc_costheta_cut"
    " && !sel_has_pion_wc_candidate"
    " && !sel_flippedTrack"
    " && sel_n_bdt_other == 0 && sel_n_bdt_invalid == 0";
    
    
    const std::string CC0pi_SELECTION_reco = "sel_CC0pi_wc";
  

std::string Gobal_CC0pireco_section = CC0pi_SELECTION_include_uncontained;
  selection_true = "mc_is_cc0pi_wc_signal";
  selection_reco = Gobal_CC0pireco_section;



 std::string selectionleadP_true = "mc_is_cc0pi_wc_signal";
 std::string selectionleadP_reco = Gobal_CC0pireco_section;


 branchexpr_reco_pmu = "p3_mu.Mag();GeV/c";
 branchexpr_true_pmu = "mc_p3_mu.Mag();GeV/c";
 
 branchexpr_reco_costheta = "p3_mu.CosTheta();";
 branchexpr_true_costheta = "mc_p3_mu.CosTheta();";

std::vector<double> Nprotonsedges{-0.5, 0.5, 1.5, 2.5, 3.5};

std::vector<double> PmuBinnEdges_wc{0.60, 0.74, 0.86, 1.00, 1.10, 1.2}; // Annie Binning 
std::vector<double> Costheaedges{.5, .8, 0.95, 1.0};
std::vector<double> costheta = {-1.00, 0.00, 0.27, 0.45,  0.5};
std::vector<double> phasespacepmu_low{0.1, 0.24, 0.38, 0.48, 0.6};
std::vector<double> phasespacepmu_high{1.2, 2.};

  std::map< double, std::vector<double> > MUON_2D_BIN_EDGES = {
// the 2D binning of inclusive but took the max limit of 2 GeV and not 2.5 GeV which its given  
{ .8, {PmuBinnEdges_wc}},
{ .95,{PmuBinnEdges_wc}},
 { 1.0, {} }
};

   std::vector<double> Kp_Edges = {0.25, 0.315, 0.35, 0.42, 0.525, 0.6, 0.8, 1.0};
   std::vector<double> theta_P_Edges = {-1, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

  // the title "title; unit" is used in plot in root style
  title ="p_{#mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "P_{\\mu};GeV/c";
  // selection


  Block1D *b1dt_Pmu_true = new Block1D(branchexpr_true_pmu, title, textitle, PmuBinnEdges_wc, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  Block1D *b1dt_Pmu_reco = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(b1dt_Pmu_true,b1dt_Pmu_reco);
      
        // the title "title; unit" is used in plot in root style
  title ="Cos#theta_{#mu};";
  // the tex title "tex title; units" is used in latex format
  textitle = "Cos\\theta_{\\mu};";
  // selection


  Block1D *b1dt_costheta_true = new Block1D(branchexpr_reco_costheta, title, textitle, Costheaedges, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  Block1D *b1dt_costheta_reco = new Block1D( branchexpr_reco_costheta, title, textitle, Costheaedges, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(b1dt_costheta_true , b1dt_costheta_reco);    
      
      
///////////////////////////////////////////////////////
// 2D blocks 
//////////////////////////////////////////////////////

 branchexpr_reco_2D = "p3_mu.CosTheta();;p3_mu.Mag();GeV/c";
 branchexpr_true_2D = "mc_p3_mu.CosTheta();;mc_p3_mu.Mag();GeV/c";

  // the title "title; unit" is used in plot in root style
  title ="cos#theta; p_{#mu}; GeV/c;";
  // the tex title "tex title; units" is used in latex format
  textitle = "P_{\\mu}; GeV/c; \\cos\\theta; ";
  // selection

Block2D *b2dmuon_pmu_costheta_true = new Block2D(branchexpr_true_2D,
      title,
      textitle,
      MUON_2D_BIN_EDGES,
      selection_true, kSignalTrueBin);
      
  Block2D *b2dmuon_pmu_costheta_reco = new Block2D(branchexpr_reco_2D,
       title,
      textitle,
      MUON_2D_BIN_EDGES,
      selection_reco, kOrdinaryRecoBin);

 //vect_block.emplace_back(b2dmuon_pmu_costheta_true, b2dmuon_pmu_costheta_reco);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
 

 std::string branchexpr_reco_scheme1 = "p3_lead_p.Mag();GeV/c";
 std::string branchexpr_true_scheme1 = "mc_p3_lead_p.Mag();GeV/c";
 
Block1D *b1dt_Kp_true = new Block1D(branchexpr_true_scheme1, "p_{p}; GeV/c", "p_{p};GeV/c", Kp_Edges, selectionleadP_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
Block1D *b1dt_Kp_reco = new Block1D(branchexpr_reco_scheme1, "p_{p}; GeV/c", "p_{p};GeV/c", Kp_Edges, selectionleadP_reco, kOrdinaryRecoBin);
  
vect_block.emplace_back(b1dt_Kp_true, b1dt_Kp_reco);
 
branchexpr_reco_scheme1 = "p3_lead_p.CosTheta();";
branchexpr_true_scheme1 = "mc_p3_lead_p.CosTheta();";

Block1D *b1dt_thetap_true = new Block1D(branchexpr_true_scheme1, "leading theta p;", "#theta_{p};GeV/c", theta_P_Edges, selectionleadP_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
Block1D *b1dt_thetap_reco = new Block1D(branchexpr_reco_scheme1, "leading theta p;", "#theta_{p};GeV/c", theta_P_Edges, selectionleadP_reco, kOrdinaryRecoBin);  
vect_block.emplace_back(b1dt_thetap_true, b1dt_thetap_reco);


          // the title "title; unit" is used in plot in root style
  title ="proton multiplicity;N";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multiplicity;N";
  // selection
 branchexpr_reco_Nproton = "sel_num_proton_candidates;N";
 branchexpr_true_Nproton = "mc_num_protons;N";

//ect_block.emplace_back(branchexpr_reco_Nproton, branchexpr_true_Nproton);
  Block1D *b1dt_Np_true = new Block1D(branchexpr_true_Nproton, title, textitle, Nprotonsedges, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  Block1D *b1dt_Np_reco = new Block1D( branchexpr_reco_Nproton, title, textitle, Nprotonsedges, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(b1dt_Np_true , b1dt_Np_reco);

  // Will make this a side band for now since have to use different cross section extraction scheme 
  //Block1D *b1dt_Np_reco = new Block1D( branchexpr_reco_Nproton, title, textitle, Nprotonsedges, selection_reco, kSidebandRecoBin);
  //vect_sideband.emplace_back(b1dt_Np_reco);


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

  // CATEGORY is the branchexpr
  // background_index is vector of background categories.
  CATEGORY = "category_wc";
  background_index = {0, 5, 7, 8, 9, 10, 11};

  
  const std::string NC_SIDEBAND_SELECTION =
  "sel_presel"
  " && !sel_has_muon_candidate"
  " && sel_has_p_candidate"
  " && sel_protons_contained"
  " && sel_n_bdt_other == 0" 
  " && sel_n_bdt_invalid == 0"
  " && sel_lead_p_passed_mom_cuts"
  " && sel_num_proton_candidates > 0";


const std::string CCNPI_SIDEBAND_SELECTION =
  "sel_nu_mu_cc"
  " && sel_muon_contained"
  " && sel_muon_passed_wc_costheta_cut"
  " && sel_muon_passed_wc_mom_cuts"
  " && sel_has_pion_wc_candidate"
  " && sel_num_pion_wc_candidates > 0"
  " && !sel_flippedTrack"
  " && sel_n_bdt_other == 0" 
  " && sel_n_bdt_invalid == 0";
  
  
  const std::string CC0pi_outsidePmu_SIDEBAND_SELECTION =
    "sel_nu_mu_cc"
    " && !sel_muon_passed_wc_mom_cuts" 
    " && !sel_muon_passed_wc_costheta_cut"
    " && !sel_has_pion_wc_candidate"
    " && sel_muon_contained"
    " && !sel_flippedTrack"
    " && sel_n_bdt_other == 0 && sel_n_bdt_invalid == 0";
  
  
    const std::string CC0pi_outsidecostheta_SIDEBAND_SELECTION =
    "sel_nu_mu_cc"
    " && !sel_muon_passed_wc_mom_cuts" 
    " && !sel_muon_passed_wc_costheta_cut"
    " && !sel_has_pion_wc_candidate"
    " && sel_muon_contained"
    " && !sel_flippedTrack"
    " && sel_n_bdt_other == 0 && sel_n_bdt_invalid == 0";
  
  
    const std::string CC0pi_BDT_SIDEBAND_SELECTION =  "sel_nu_mu_cc"
                                                      " && sel_muon_contained"
                                                      " && !sel_flippedTrack";
  
  
  
  
    //////////////////////////////////////////////////////////////////////////
  
    
   std::vector<double> Pmu_binningSideband = { 0.60, 0.74, 0.86, 1.00, 1.10, 1.2};
   std::vector<double> Costheta_sideBand = {.8, 0.95, 1.0};
   std::vector<double> proton_binningSideband{0.25, 0.28, 0.315, 0.35, 0.42, 0.525, 0.6, 0.8, 1.0}; // Annie Binning  
   std::vector<double> Nprotonsedges_singleBin{0.5, 1.5, 2.5};
  
   std::vector<double> Eavailedges{0.0, .05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, .8};
  title ="E_{avail};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "E_{avail};GeV/c";
  // selection
 std::string branchexpr_reco_Eavail = "Eavail;GeV/c";
 std::string branchexpr_true_Eavail = "mc_Eavail;GeV/c";

  Block1D *b1dt_Eavail_true = new Block1D(branchexpr_true_Eavail, title, textitle, Eavailedges, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  Block1D *b1dt_Eavail_reco = new Block1D( branchexpr_reco_Eavail, title, textitle, Eavailedges, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(b1dt_Eavail_true , b1dt_Eavail_reco);


 branchexpr_sideband = "sel_num_proton_candidates;";
   // the title "title; unit" is used in plot in root style
  title ="N_{proton};";
  // the tex title "tex title; units" is used in latex format
  textitle = "N_{proton};";

  Block1D *b1r_sideband_NC_Np = new Block1D(branchexpr_sideband, title, textitle, Nprotonsedges_singleBin, NC_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_NC_Np);


 branchexpr_sideband = "p3_lead_p.Mag(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{p}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{p}; GeV/c";

  Block1D *b1r_sideband_NC = new Block1D(branchexpr_sideband, title, textitle, proton_binningSideband, NC_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_NC);


 branchexpr_sideband = "p3_mu.Mag(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{#mu}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{\\mu}; GeV/c";

  Block1D *b1r_sideband_Npi_pMu = new Block1D(branchexpr_sideband, title, textitle, Pmu_binningSideband, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Npi_pMu);

 branchexpr_sideband = "p3_mu.CosTheta(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="cos#theta;";
  // the tex title "tex title; units" is used in latex format
  textitle = "\\cos\\theta;";


  Block1D *b1r_sideband_Npi_costheta = new Block1D(branchexpr_sideband, title, textitle, Costheta_sideBand, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Npi_costheta);

   std::vector<double> NPionssedges{0.5, 1.5, 2.5};

 branchexpr_sideband = "sel_num_pion_candidates;";
   // the title "title; unit" is used in plot in root style
  title ="Npions;";
  // the tex title "tex title; units" is used in latex format
  textitle = "Npions;";


  Block1D *b1r_sideband_Npi_NumPions = new Block1D(branchexpr_sideband, title, textitle, NPionssedges, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Npi_NumPions);


 branchexpr_sideband = "p3_mu.Mag();GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{mu};GeV/c";

  Block1D *b1r_sideband_Outside_PmuPhasespace_low = new Block1D(branchexpr_sideband, title, textitle, phasespacepmu_low, CC0pi_outsidePmu_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Outside_PmuPhasespace_low);
  
   Block1D *b1r_sideband_Outside_PmuPhasespace_high = new Block1D(branchexpr_sideband, title, textitle, phasespacepmu_high, CC0pi_outsidePmu_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Outside_PmuPhasespace_high);
  

   branchexpr_sideband = "p3_mu.CosTheta();";
    textitle = "cos_{#theta_{#mu}};";
  
  Block1D *b1r_sideband_Outside_costhetaPhasespace_Anglebelow = new Block1D(branchexpr_sideband, title, textitle, costheta, CC0pi_outsidecostheta_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Outside_costhetaPhasespace_Anglebelow);
  
  
  std::vector<double> BTD_PredictionSideband = {0.0, 0.45, .55,.6,.65,.7,.75,.8,.85 ,.9, .92 ,.94, 1.0};
 branchexpr_sideband = "muon_BDTScore;";
   // the title "title; unit" is used in plot in root style
  title ="BTD Muon score;";
  // the tex title "tex title; units" is used in latex format
  textitle = "BTD Muon score;";

  Block1D *b1r_sideband_BDTPrediction_muon = new Block1D(branchexpr_sideband, title, textitle, BTD_PredictionSideband, CC0pi_BDT_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_BDTPrediction_muon);

  




}
