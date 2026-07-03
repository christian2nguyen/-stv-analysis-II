// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/JOINTCC0Pi_ANNIE.hh"

JOINTCC0Pi_ANNIE::JOINTCC0Pi_ANNIE() : BinSchemeBase( "JOINTCC0Pi_ANNIE" ) {}

void JOINTCC0Pi_ANNIE::DefineBlocks() {

  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "stv_tree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1 };

  // Prefix for the output bin and slice configuration text files
  out_config_prefix_ = "JOINTCC0pi_UBooNE_SIDEBands_v3";

  // Selection to use with this binning scheme
  selection_name_ = "JOINTCC0pi";

  // TDirectory file name to use when producing the univmake output histograms
  out_tdir_name_ = "JOINTCC0pi";

  /////// Define the blocks of bins in both true and reco space
  std::string 
  title, textitle,
  selection_true, 
  selection_reco,
  branchexpr_reco_pmu,
  branchexpr_true_pmu,branchexpr_sideband;


   //std::vector<double> Eavailedges{0.0, .05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, .8};
//    std::vector<double> Xp_edges{-0.5, 0.5, 0.5};
   std::vector<double> PmuBinnEdges_wc{0.60, 0.74, 0.86, 1.00, 1.10, 1.2}; // Annie Binning 
   std::vector<double> costheta = {-1.00, 0.00, 0.27, 0.45, 0.62, 0.76, 0.8};
   std::vector<double> NPionssedges{0.5, 1.5, 2.5};
   std::vector<double> NC_Nprotonsedges{0.5, 1.5, 2.5};
   std::vector<double> Nprotonsedges{-0.5,0.5, 1.5, 2.5, 3.5};
   std::vector<double> phasespacepmu_low{0.1, 0.24, 0.38, 0.48, 0.6};
   std::vector<double> phasespacepmu_high{1.2, 2.};
   std::vector<double> proton_binningSideband{0.25, 0.28, 0.315, 0.35, 0.42, 0.525, 0.6, 0.8, 1.0}; // Annie Binning  
       // the title "title; unit" is used in plot in root style
  title ="p_{mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{mu};GeV/c";
  // selection

  selection_true = "mc_is_cc0pi_wc_signal";
  selection_reco = "sel_CC0pi_wc";
  branchexpr_reco_pmu = "p3_mu.Mag();GeV/c" ;
  branchexpr_true_pmu= "mc_p3_mu.Mag();GeV/c" ;


  Block1D *Pmu_true = new Block1D(branchexpr_true_pmu, title, textitle, PmuBinnEdges_wc, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  Block1D *Pmu_reco = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(Pmu_true , Pmu_reco);


  // CATEGORY is the branchexpr
  // background_index is vector of background categories.
  CATEGORY ="category_wc";
  //CATEGORY = "category";
  background_index = {0, 5, 7, 8, 9, 10, 11};


//const std::string CCNPI_SIDEBAND_SELECTION =
//  "JOINTCC0pi_ANNIE_sel_nu_mu_cc"
//  " && JOINTCC0pi_ANNIE_sel_has_pion_candidate";
    const std::string NC_SIDEBAND_SELECTION =
  "sel_reco_vertex_in_FV && sel_pfp_starts_in_PCV"
  " && !sel_has_muon_candidate && sel_topo_cut_passed"
  " && sel_no_reco_showers && sel_has_p_candidate"
  " && sel_protons_contained"
  " && sel_n_bdt_other == 0" 
  " && sel_n_bdt_invalid == 0"
  " && sel_lead_p_passed_mom_cuts"
  " && sel_num_proton_candidates > 0";
  
  
  const std::string CCNPI_SIDEBAND_SELECTION =
  "sel_reco_vertex_in_FV"
  " && sel_pfp_starts_in_PCV"
  " && sel_has_muon_candidate" 
  " && sel_muon_passed_wc_mom_cuts"
  " && sel_muon_passed_wc_costheta_cut"
  " && sel_topo_cut_passed"
  " && sel_no_reco_showers"
  " && sel_muon_passed_mom_cuts"
  " && sel_has_pion_candidate"
  " && sel_num_pion_candidates > 0"
  " && sel_n_bdt_other == 0" 
  " && sel_n_bdt_invalid == 0";
  
  
  const std::string CC0pi_outsidePmu_SIDEBAND_SELECTION =
    " sel_nu_mu_cc"
    " && sel_no_reco_showers"
    " && !sel_muon_passed_wc_mom_cuts" 
    " && sel_muon_passed_wc_costheta_cut"
    " && !sel_has_pion_wc_candidate"
    " && sel_muon_contained"
    " && !sel_flippedTrack"
    " && sel_n_bdt_other == 0 && sel_n_bdt_invalid == 0";
  
  
    const std::string CC0pi_outsidecostheta_SIDEBAND_SELECTION =
    "sel_nu_mu_cc"
    " && sel_no_reco_showers"
    " && sel_muon_passed_wc_mom_cuts" 
    " && !sel_muon_passed_wc_costheta_cut"
    " && !sel_has_pion_wc_candidate"
    " && sel_muon_contained"
    " && !sel_flippedTrack"
    " && sel_n_bdt_other == 0 && sel_n_bdt_invalid == 0";
  
    //////////////////////////////////////////////////////////////////////////
  
            // the title "title; unit" is used in plot in root style
  title ="proton multiplicity;N";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multiplicity;N";
  // selection
 std::string branchexpr_reco_Nproton = "sel_num_proton_candidates;N";
std::string  branchexpr_true_Nproton = "mc_num_protons;N";

  //Block1D *b1dt_Np_true = new Block1D(branchexpr_true_Nproton, title, textitle, Nprotonsedges, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  //Block1D *b1dt_Np_reco = new Block1D( branchexpr_reco_Nproton, title, textitle, Nprotonsedges, selection_reco, kOrdinaryRecoBin);
  //vect_block.emplace_back(b1dt_Np_true , b1dt_Np_reco);

  // Will make this a side band for now since have to use different cross section extraction scheme 
  Block1D *b1dt_Np_reco = new Block1D( branchexpr_reco_Nproton, title, textitle, Nprotonsedges, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1dt_Np_reco);
  
  

 branchexpr_sideband = "sel_num_proton_candidates;";
   // the title "title; unit" is used in plot in root style
  title ="N_{proton};";
  // the tex title "tex title; units" is used in latex format
  textitle = "N_{proton};";

  Block1D *b1r_sideband_NC_Np = new Block1D(branchexpr_sideband, title, textitle, NC_Nprotonsedges, NC_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_NC_Np);
  
 branchexpr_sideband = "p3_lead_p.Mag(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{p}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{p}; GeV/c";

  Block1D *b1r_sideband_NC = new Block1D(branchexpr_sideband, title, textitle, proton_binningSideband, NC_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_NC);
  

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
  
  
  

 branchexpr_sideband = "p3_mu.Mag();GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{mu};GeV/c";

  Block1D *b1r_sideband_Npi_pMu = new Block1D(branchexpr_sideband, title, textitle, PmuBinnEdges_wc, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Npi_pMu);

 branchexpr_sideband = "sel_num_pion_wc_candidates;";
   // the title "title; unit" is used in plot in root style
  title ="Npions;";
  // the tex title "tex title; units" is used in latex format
  textitle = "Npions;";
    Block1D *b1r_sideband_Npi_NumPions = new Block1D(branchexpr_sideband, title, textitle, NPionssedges, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);

   vect_sideband.emplace_back(b1r_sideband_Npi_NumPions);
  
}
