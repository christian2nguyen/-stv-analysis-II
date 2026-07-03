// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/JOINTCC0Pi_blips.hh"

JOINTCC0Pi_blips::JOINTCC0Pi_blips() : BinSchemeBase( "JOINTCC0Pi_blips" ) {}

void JOINTCC0Pi_blips::DefineBlocks() {

  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "stv_tree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1 };

  // Prefix for the output bin and slice configuration text files
  out_config_prefix_ = "JOINTCC0pi_mc910_noblips";

  // Selection to use with this binning scheme
  selection_name_ = "JOINTCC0pi";

  // TDirectory file name to use when producing the univmake output histograms
  out_tdir_name_ = "JOINTCC0pi";

  /////// Define the blocks of bins in both true and reco space
  std::string branchexpr_reco_scheme1,
  branchexpr_true_scheme2, 
  branchexpr_true_scheme1,
  branchexpr_reco_scheme2,
  branchexpr_reco_Nproton,
  branchexpr_true_Nproton,
  branchexpr_sideband, 
  title, textitle,
  selection_true, 
  selection_reco;

  std::vector<double> PmuBinnEdges = { 0.1, 0.24, 0.3, 0.38, 0.48, 0.7, 0.85, 1.28, 2.0};
  std::vector<double> trk_len_v_edges = {0, 20, 40, 60, 80, 100 ,120, 140, 160, 180, 200, 250, 300, 350,500,800,1200};
  std::vector<double> costheta = {-1.00, -0.50, 0.00, 0.27, 0.45, 0.62, 0.76, 0.86, 0.94, 1.0};


  selection_true = "mc_is_cc0pi_signal";
  selection_reco = "sel_CC0pi && sel_muon_contained";
 // selection_reco_leadingP = "sel_CC0pi && sel_num_proton_candidates > 0 && sel_muon_contained";



   title ="p_{#mu};GeV/c";
   // the tex title "tex title; units" is used in latex format
   textitle = "p_{\\mu};GeV/c";
   // selection
 
    std::string branchexpr_reco_pmu = "p3_mu.Mag();GeV/c";
    std::string branchexpr_true_pmu = "mc_p3_mu.Mag();GeV/c";
 
   Block1D *b1dt_Pmu_true = new Block1D(branchexpr_true_pmu, title, textitle, PmuBinnEdges, selection_true, kSignalTrueBin);
   // only the name of branch and the selection is different from true.
   Block1D *b1dt_Pmu_reco = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges, selection_reco, kOrdinaryRecoBin);
   vect_block.emplace_back(b1dt_Pmu_true,b1dt_Pmu_reco);
     

   title ="cos#theta_{#mu};GeV/c";
   // the tex title "tex title; units" is used in latex format
   textitle = "p_{\\mu};GeV/c";
   // selection
 
   std::string branchexpr_reco_costheta = "p3_mu.CosTheta();";
   std::string branchexpr_true_costheta = "mc_p3_mu.CosTheta();";
 
   Block1D *b1dt_Costheta_true = new Block1D(branchexpr_true_costheta, title, textitle, costheta, selection_true, kSignalTrueBin);
   // only the name of branch and the selection is different from true.
   Block1D *b1dt_Costheta_reco = new Block1D( branchexpr_reco_costheta, title, textitle, costheta, selection_reco, kOrdinaryRecoBin);
   vect_block.emplace_back(b1dt_Costheta_true,b1dt_Costheta_reco);
     

     
     
  // CATEGORY is the branchexpr
  // background_index is vector of background categories.
  CATEGORY = "category";
 




  //vect_block.emplace_back(b2dmuon_mom_costheta_true_scheme2, b2dmuon_mom_costheta_reco_scheme2);

  // CATEGORY is the branchexpr
  // background_index is vector of background categories.

  //CATEGORY = "category";
  background_index = {0, 5, 7, 8, 9, 10, 11};


/*

 will leave out for now
 std::vector< double > Mult= {-0.5, 0.5, 1.5, 2.5, 3.5};

  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_true_lead_proton_delta_pT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "leading proton #Delta p_{T}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "leading proton \\Delta \\p_{T}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal && CC1muXp0pi_nProtons_in_Momentum_range > 0";

  Block1D *b1dt_delta_pT = new Block1D(branchexpr, title, textitle, delta_pT_1D_edges, selection, kSignalTrueBin);

*/


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
  "sel_reco_vertex_in_FV && sel_pfp_starts_in_PCV"
  " && sel_has_muon_candidate && sel_topo_cut_passed"
  " && sel_no_reco_showers && sel_muon_passed_mom_cuts"
  " && sel_has_pion_candidate"
  " && sel_num_pion_candidates > 0"
  " && sel_n_bdt_other == 0" 
  " && sel_n_bdt_invalid == 0";
  
    
   std::vector<double> Pmu_binningSideband = { 0.1, 0.24, 0.3, 0.38, 0.48, 0.7, 0.85, 1.28, 1.58, 2.0};
   std::vector<double> Costheta_sideBand = {-1.00, -0.50, 0.00, 0.27,  0.45, 0.62, 0.76, 0.86, 0.94, 1.0};
   std::vector<double> proton_binningSideband = {0.25, 0.41, 0.495, 0.56, 0.62, 0.68, 0.74, 0.8, 0.87, 0.93, 1.0, 1.2};
   std::vector<double> BTD_PredictionSideband = {0.0, 0.45, .55,.6,.65,.7,.75,.8,.85 ,.9, .925 ,.95, .975, 1.0};
   std::vector<double> NPionssedges{0.5, 1.5, 2.5};
   
   std::vector<double> NBlipssedges{-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5, 10.5};
   std::vector< double > VertexX_edges = {-10,0,25,50,100,125,150,175,200,225,250,275,300,325, 350 };
   std::vector< double > VertexY_edges= {-200,-175,-150,-125,-100.,-75,-50,-25,0,25,50,100,125,150,175,200 };
   std::vector< double > VertexZ_edges= {-10,0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,1000,1050,1100,1150 ,1200};

   
   


 branchexpr_sideband = "p3_lead_p.Mag(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{p}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{p}; GeV/c";

  Block1D *b1r_sideband_NC = new Block1D(branchexpr_sideband, title, textitle,proton_binningSideband, NC_SIDEBAND_SELECTION, kSidebandRecoBin);
  //vect_sideband.emplace_back(b1r_sideband_NC);



 branchexpr_sideband = "p3_mu.Mag(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="p_{#mu}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{\\mu}; GeV/c";


  Block1D *b1r_sideband_Npi_pMu = new Block1D(branchexpr_sideband, title, textitle, Pmu_binningSideband, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  //vect_sideband.emplace_back(b1r_sideband_Npi_pMu);


 branchexpr_sideband = "p3_mu.CosTheta(); GeV/c";
   // the title "title; unit" is used in plot in root style
  title ="cos#theta;";
  // the tex title "tex title; units" is used in latex format
  textitle = "\\cos\\theta;";


  Block1D *b1r_sideband_Npi_costheta = new Block1D(branchexpr_sideband, title, textitle, Costheta_sideBand, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Npi_costheta);


 branchexpr_sideband = "sel_num_pion_candidates;";
   // the title "title; unit" is used in plot in root style
  title ="Npions;";
  // the tex title "tex title; units" is used in latex format
  textitle = "Npions;";

  Block1D *b1r_sideband_Npi_NumPions = new Block1D(branchexpr_sideband, title, textitle, NPionssedges, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Npi_NumPions);


  branchexpr_sideband = "nblips_saved;";
  // the title "title; unit" is used in plot in root style
 title ="nblips;";
 // the tex title "tex title; units" is used in latex format
 textitle = "nblips;";

 Block1D *b1r_sideband_NBlips = new Block1D(branchexpr_sideband, title, textitle, NBlipssedges, CCNPI_SIDEBAND_SELECTION, kSidebandRecoBin);
 //vect_sideband.emplace_back(b1r_sideband_NBlips);

}
