// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/Muon_Pmu_Study_CC0Pi.hh"

Muon_Pmu_Study_CC0Pi::Muon_Pmu_Study_CC0Pi() : BinSchemeBase( "Muon_Pmu_Study_CC0Pi" ) {}

void Muon_Pmu_Study_CC0Pi::DefineBlocks() {

std::cout<<"Starting::Muon_Pmu_Study_CC0Pi"<<std::endl; 
  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "stv_tree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1,2,3 };

  // Prefix for the output bin and slice configuration text files
  out_config_prefix_ = "Muon_MCS_Panels_subpanels";

  // Selection to use with this binning scheme
  selection_name_ = "JOINTCC0pi";

  // TDirectory file name to use when producing the univmake output histograms
  out_tdir_name_ = "JOINTCC0pi";

  /////// Define the blocks of bins in both true and reco space
  std::string branchexpr_reco_scheme1,
  branchexpr_true_scheme2, 
  branchexpr_true_scheme1,
  branchexpr_reco_scheme2, 
  branchexpr_sideband, 
  title, textitle,
  selection_true, selection_reco,branchexpr;

  selection_true = "mc_is_cc0pi_signal &&  mc_num_protons > 0";
  selection_reco = "sel_CC0pi && sel_muon_contained && sel_num_proton_candidates > 0";


  
  const std::string CCNPI_SIDEBAND_SELECTION =
  "sel_reco_vertex_in_FV && sel_pfp_starts_in_PCV"
  " && sel_has_muon_candidate && sel_muon_contained && sel_topo_cut_passed"
  " && sel_no_reco_showers && sel_muon_passed_mom_cuts"
  " && sel_has_pion_candidate"
  " && sel_num_pion_candidates > 0"
  " && sel_n_bdt_other == 0" 
  " && sel_n_bdt_invalid == 0";

  
  
  // First block: cos_theta_mu in 2D
  std::vector<double> Pmu_2d = { 0.1, 0.24, 0.3, 0.38, 0.48, 0.7, 0.85, 1.28, 2.0};
  std::vector<double> trk_len_v_edges = {0, 20, 40, 60, 80, 100 ,120, 140, 160, 180, 200, 250, 300, 350,500,800,1200};
  std::vector<double> costheta = {-1.00, -0.50, 0.00, 0.27, 0.45, 0.62, 0.76, 0.86, 0.94, 1.0};
  std::vector<double> Nprotonsedges{-0.5, 0.5, 1.5, 2.5, 3.5};
  std::vector<double> Sidesedges{-.5, 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5};
    
 std::vector<double> Pmu_1d = Pmu_2d;//{0.1,0.2,0.225,0.25,0.275,0.3,0.325,0.375,0.425,0.5,0.6,0.75,0.95,1.25,2};
 std::vector<double> costheta_1d = costheta; //{ -1.0,-0.775,-0.65,-0.55,-0.45,-0.375,-0.3,-0.2,-0.125,-0.05, 0.025,0.1, 0.175,0.25,0.325,0.4,0.475,0.55,0.625,0.675, 0.725,0.775,0.825,0.85,0.875, 0.9,0.925,0.95,0.975,1.0};



  std::map< double, std::vector<double> > MUON_2D_BIN_EDGES_inclusive = {
// the 2D binning of inclusive but took the max limit of 2 GeV and not 2.5 GeV which its given  
{ -1.00, 
     {0.1, 0.24, 0.3, 2.0} },
{ -0.50,
    {0.1,  0.24, 0.3, 0.38, 2.0} },
 { 0.00, 
   {0.1,   0.24, 0.3, 0.38, 2.0} },
 { 0.27, 
     {0.1,  0.2,  0.3,  0.48, 2.0} },
 { 0.45, 
     {0.1, 0.24, 0.38, 0.48, 2.0} },
 { 0.62, 
     {0.1, 0.24, 0.38, 0.48, 2.0} },
 { 0.76, 
     {0.1, 0.24, 0.38, 0.48, 0.68, 2.0} },
 { 0.86, 
     {0.1, 0.48, 0.68, 0.85,  2.0} },
 { 0.94, 
     {0.1, 0.48, 0.68, 0.85, 1.28, 2.0} },
 { 1.0, {} }
};
   
   
   std::cout<<"1D"<<std::endl; 

   branchexpr = "mc_p3_mu.Mag();GeV/c";
    title = "P_{#mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "P_{\\mu};GeV/c";
  Block1D *b1dt_Pmu_true = new Block1D(branchexpr, title, textitle, Pmu_1d, selection_true, kSignalTrueBin);
  branchexpr = "p3mu_mcs.Mag();GeV/c";
  // only the name of branch and the selection is different from true.
  Block1D *b1dt_Pmu_reco = new Block1D(branchexpr, title, textitle, Pmu_1d, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(b1dt_Pmu_true,b1dt_Pmu_reco);
  
  branchexpr = "mc_p3_mu.CosTheta();";
  title = "cos#theta;";
  // the tex title "tex title; units" is used in latex format
  textitle = "\\cos\\theta;";
  Block1D *b1dt_cos_true = new Block1D(branchexpr, title, textitle, costheta_1d, selection_true, kSignalTrueBin);
 
 branchexpr = "p3mu_mcs.CosTheta();";
  // only the name of branch and the selection is different from true.
  Block1D *b1dt_cos_reco = new Block1D(branchexpr, title, textitle, costheta_1d, selection_reco, kOrdinaryRecoBin);
  //vect_block.emplace_back(b1dt_cos_true,b1dt_cos_reco);
  
  // the title "title; unit" is used in plot in root style
  title ="cos#theta;p_{#mu};GeV/c;";
  // the tex title "tex title; units" is used in latex format
  textitle = "P_{\\mu};GeV/c;\\cos\\theta;";
  
  // selection
 std::cout<<"12D"<<std::endl; 


 branchexpr_reco_scheme2 = "p3mu_mcs.CosTheta();;p3mu_mcs.Mag();GeV/c";
 branchexpr_true_scheme2 = "mc_p3_mu.CosTheta();;mc_p3_mu.Mag();GeV/c";

Block2D *b2dmuon_mom_costheta_true_scheme2 = new Block2D(branchexpr_true_scheme2,
      title,
      textitle,
      MUON_2D_BIN_EDGES_inclusive,
      selection_true, kSignalTrueBin);
      
  Block2D *b2dmuon_mom_costheta_reco_scheme2 = new Block2D(branchexpr_reco_scheme2,
       title,
      textitle,
      MUON_2D_BIN_EDGES_inclusive,
      selection_reco, kOrdinaryRecoBin);
      

  //vect_block.emplace_back(b2dmuon_mom_costheta_true_scheme2, b2dmuon_mom_costheta_reco_scheme2);

 
   // Block2D *b2d_TrackLength_true = new Block2D(branchexpr_reco_scheme1,
   //   "muon mom; GeV/c; TrkLenght_{#mu};cm ", 
   //   "P_{\\mu}; GeV/c; TrkLengh_{\\mu};cm ",
   //   MUON_2D_BIN_EDGES_trackLength, selection_true, kSignalTrueBin);

  //vect_block.emplace_back(b2d_TrackLength_true, b2d_TrackLength_reco);



  
   // Block2D *b2d_TrackLength_true_inclusive = new Block2D(branchexpr_reco_scheme1,
   //   "costheta; GeV/c; TrkLenght_{#mu};cm ", 
   //   "cos#theta;; TrkLengh_{\\mu};cm ",
   //   MUON_2D_BIN_EDGES_inclusive_trklenght, selection_true, kSignalTrueBin);
      
  //vect_block.emplace_back(b2d_TrackLength_true_inclusive, b2d_TrackLength_reco_inclusive);


  // the title "title; unit" is used in plot in root style
  title ="cos#theta; p_{#mu}; GeV/c;";
  // the tex title "tex title; units" is used in latex format
  textitle = "P_{\\mu}; GeV/c; \\cos\\theta; ";
  // selection

//
//  vect_block.emplace_back(b2dmuon_mom_costheta_true_scheme2, b2dmuon_mom_costheta_reco_scheme2);
 branchexpr_sideband = "trk_len_v[ muon_candidate_idx ];cm";
   // the title "title; unit" is used in plot in root style
  title ="TrkLength_{#mu};cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "TrkLength_{\\mu};cm";


  Block1D *b1r_sideband_trklengh = new Block1D(branchexpr_sideband, title, textitle, trk_len_v_edges, selection_reco, kSidebandRecoBin);

   //vect_sideband.emplace_back(b1r_sideband_trklengh);
  
  
   branchexpr_sideband = "sel_PanelClosesttoEndMuonTrk;SideN";
   // the title "title; unit" is used in plot in root style
  title ="SideN;";
  // the tex title "tex title; units" is used in latex format
  textitle = "SideN;";


  Block1D *b1r_sideband_Sides = new Block1D(branchexpr_sideband, title, textitle, Sidesedges, selection_reco, kSidebandRecoBin);
   vect_sideband.emplace_back(b1r_sideband_Sides);
  
  
  branchexpr_sideband = "p3mu_mcs.CosTheta();;trk_len_v[ muon_candidate_idx ];cm";
  title = "cos#theta; ; trklenght;";
  textitle = "cos#theta; ; trklenght;";
  
  
  
  
  
    std::map< double, std::vector<double> > MUON_2D_BIN_EDGES_inclusive_trklenght = {
// the 2D binning of inclusive but took the max limit of 2 GeV and not 2.5 GeV which its given
{ -1.00,
    {0, 10, 20, 30, 40, 50, 60, 70, 100,  1200} },
{ -0.50,
    {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 1200}},
{ 0.00,
    { 0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 1200}},
 { 0.27,
     trk_len_v_edges },
 { 0.45,
     trk_len_v_edges },
 { 0.62,
     trk_len_v_edges },
 { 0.76,
     trk_len_v_edges },
 { 0.86,
     trk_len_v_edges },
 { 0.94,
     trk_len_v_edges },
 { 1.0, {} }
};

  //Block2D *b2r_sideband = new Block2D(branchexpr_sideband, title, textitle, MUON_2D_BIN_EDGES_inclusive_trklenght , selection_reco, kSidebandRecoBin);
  //vect_sideband.emplace_back(b2r_sideband);


/*
  std::vector<double> trk_len_v_edges = {0, 10, 20, 30, 40, 50, 60, 80, 100 ,120, 140, 160, 180, 200, 250, 300, 350};

  std::map< double, std::vector<double> > MUON_2D_BIN_EDGES_inclusive_trklenght = {
// the 2D binning of inclusive but took the max limit of 2 GeV and not 2.5 GeV which its given  
{ -1.00, 
    {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 350} },
{ -0.50,
    {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 350}},
{ 0.00, 
    { 0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 350 }},
 { 0.27, 
     trk_len_v_edges },
 { 0.45, 
     trk_len_v_edges },
 { 0.62, 
     trk_len_v_edges },
 { 0.76, 
     trk_len_v_edges },
 { 0.86, 
     trk_len_v_edges },
 { 0.94, 
     trk_len_v_edges },
 { 1.0, {} }
};

 branchexpr_reco_scheme1 = "p3mu_mcs.CosTheta();GeV/c;trk_len_v[ muon_candidate_idx ];cm";


    Block2D *b2d_TrackLength_reco_inclusive= new Block2D(branchexpr_reco_scheme1,
      "costheta; GeV/c; TrkLenght_{#mu};cm ", 
      "cos#theta;; TrkLengh_{\\mu};cm ",
      MUON_2D_BIN_EDGES_inclusive_trklenght, selection_reco, kOrdinaryRecoBin);

vect_sideband.emplace_back(b2d_TrackLength_reco_inclusive);
 //vect_block.emplace_back(b2d_TrackLength_reco_inclusive,b2d_TrackLength_reco_inclusive);
*/

    branchexpr_reco_scheme1 = "p3mu_mcs.Mag();GeV/c;trk_len_v[ muon_candidate_idx ];cm";
    std::map< double, std::vector<double> > MUON_2D_BIN_EDGES_trackLength = {
    { 0.1, {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 350,1200} },
    { 0.24, {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 350,1200} },
    { 0.3,  {0, 10, 20, 30, 40, 50, 60, 70, 80, 100, 350,1200} },
    { 0.38, trk_len_v_edges },
    { 0.48, trk_len_v_edges },
    { 0.7,  trk_len_v_edges },
    { 0.85, trk_len_v_edges },
    { 1.28, {0, 50, 100, 120, 140, 160, 180, 200, 250, 300, 350,1200}},
    { 1.58, {0, 50, 100, 120, 140, 160, 180, 200, 250, 300, 350,1200} },
    { 2.0, {} }
    };
  
    Block2D *b2d_TrackLength_reco = new Block2D(branchexpr_reco_scheme1,
      "muon mom; GeV/c; TrkLenght_{#mu};cm", 
      "P_{\\mu}; GeV/c; TrkLengh_{\\mu};cm",
      MUON_2D_BIN_EDGES_trackLength, selection_reco, kSidebandRecoBin);
    //vect_sideband.emplace_back(b2d_TrackLength_reco);

    //vect_sideband.emplace_back(b2d_TrackLength_reco);
  
 std::cout <<"LINE 274"<<std::endl; 
  
  
  std::map< double, std::vector<double> > multiply_Pmu  = {
  {-0.5, Pmu_2d}, 
  {0.5, Pmu_2d},
  {1.5, Pmu_2d},
  {2.5, Pmu_2d},
  {3.5, {} }
  }; 
  
  
     branchexpr_reco_scheme1 = "sel_num_proton_candidates;;p3mu_mcs.Mag();GeV/c";
      Block2D *b2d_Mulitply_pmu_reco = new Block2D(branchexpr_reco_scheme1,
      "Nprotons;;P_{#mu};GeV/c", 
      "Nprotons;;P_{#mu};GeV/c",
      multiply_Pmu, selection_reco, kSidebandRecoBin);

      //vect_sideband.emplace_back(b2d_Mulitply_pmu_reco);

     
    //vect_sideband.emplace_back(b2d_Mulitply_pmu_reco);

    std::map< double, std::vector<double> > multiply_trklen = {
  {-0.5, trk_len_v_edges}, 
  {0.5, trk_len_v_edges},
  {1.5, trk_len_v_edges},
  {2.5, trk_len_v_edges},
  {3.5, {} }
  }; 
  
   branchexpr_reco_scheme1 = "sel_num_proton_candidates;;trk_len_v[ muon_candidate_idx ];cm";

        Block2D *b2d_Mulitply_trklen_reco = new Block2D(branchexpr_reco_scheme1,
      "Nprotons;;trkLen;cm", 
      "Nprotons;;trkLen;cm",
      multiply_trklen, selection_reco, kSidebandRecoBin);
 
    // vect_sideband.emplace_back(b2d_Mulitply_trklen_reco);



        std::map< double, std::vector<double> > multiply_Sides_Pmu = {
  {-0.5,Pmu_2d},   
  {0.5, Pmu_2d},   
  {1.5, Pmu_2d},
  {2.5, Pmu_2d},
  {3.5, Pmu_2d},
  {4.5, Pmu_2d},
  {5.5, Pmu_2d},
  {6.5, {} }
  }; 
    
  branchexpr_reco_scheme1 = "sel_PanelClosesttoEndMuonTrk;;p3mu_mcs.Mag();GeV/c";

        Block2D *b2d_Mulitply_ClosestSide_Pmu_reco = new Block2D(branchexpr_reco_scheme1,
      "ClosestSide;;P_{mu};GeV", 
      "ClosestSide;;P_{mu};GeV",
       multiply_Sides_Pmu, selection_reco, kSidebandRecoBin);  
    
    //vect_sideband.emplace_back(b2d_Mulitply_ClosestSide_Pmu_reco);
    
            std::map< double, std::vector<double> > multiply_Sides_costheta = {
  {-0.5, costheta},   
  {0.5, costheta}, 
  {1.5, costheta},
  {2.5, costheta},
  {3.5, costheta},
  {4.5, costheta},
  {5.5, costheta},
  {6.5, {} }
  }; 


  branchexpr_reco_scheme1 = "sel_PanelClosesttoEndMuonTrk;;p3mu_mcs.CosTheta();";

        Block2D *b2d_Mulitply_ClosestSide_costheta_reco = new Block2D(branchexpr_reco_scheme1,
      "ClosestSide;;P_{mu};GeV", 
      "ClosestSide;;P_{mu};GeV",
      multiply_Sides_costheta, selection_reco, kSidebandRecoBin);  
       
      //vect_sideband.emplace_back(b2d_Mulitply_ClosestSide_costheta_reco);
    
            std::map< double, std::vector<double> > multiply_Sides_trackLenght = {
 {-0.5, trk_len_v_edges},   
  {0.5, trk_len_v_edges}, 
  {1.5,  trk_len_v_edges},
  {2.5,  trk_len_v_edges},
  {3.5,  trk_len_v_edges},
  {4.5,  trk_len_v_edges},
  {5.5,  trk_len_v_edges},
  {6.5, {} }
  }; 
    
      branchexpr_reco_scheme1 = "sel_PanelClosesttoEndMuonTrk;;trk_len_v[ muon_candidate_idx ];cm";

        Block2D *b2d_Mulitply_trklenSides_reco = new Block2D(branchexpr_reco_scheme1,
      "ClosestSide;;TkLen_{mu};cm", 
      "ClosestSide;;TkLen_{mu};cm",
       multiply_Sides_trackLenght, selection_reco, kSidebandRecoBin);  
    
    //vect_sideband.emplace_back(b2d_Mulitply_trklenSides_reco);
    
    
    
    std::vector<double> SubPanels_vector;
for (double i = 6.5; i <= 54.5; i += 1.0) {
    SubPanels_vector.push_back(i);
}
    
    
                std::map< double, std::vector<double> > multiply_Sides_SubPanels = {
 {-0.5, SubPanels_vector},   
  {0.5, SubPanels_vector}, 
  {1.5,  SubPanels_vector},
  {2.5,  SubPanels_vector},
  {3.5,  SubPanels_vector},
  {4.5,  SubPanels_vector},
  {5.5,  SubPanels_vector},
  {6.5, {} }
  }; 
    
      branchexpr_reco_scheme1 = "sel_PanelClosesttoEndMuonTrk;;sel_PanelClosestSubPaneltoEndMuonTrk;";

        Block2D *b2d_Mulitply_SubPanels_reco = new Block2D(branchexpr_reco_scheme1,
      "ClosestSide;;subPanels;cm", 
      "ClosestSide;;subPanels;cm",
       multiply_Sides_SubPanels, selection_reco, kSidebandRecoBin);  
    
        vect_sideband.emplace_back(b2d_Mulitply_SubPanels_reco);
    
    
/*
    branchexpr_reco_scheme1 = "trk_len_v[ muon_candidate_idx ];cm;p3mu_mcs.Mag();";
       std::map< double, std::vector<double> >trackLength_BIN_EDGES_Pmu = {
  {0,Pmu_2d},
  {10,Pmu_2d},
  {25,Pmu_2d},
  {40,Pmu_2d},
  {60,Pmu_2d},
  {80,Pmu_2d},
  {100,Pmu_2d},
  {120, Pmu_2d},
  {140,Pmu_2d},
  {160,Pmu_2d},
  {180,Pmu_2d},
  {200,Pmu_2d},
  {250,Pmu_2d},
  {300,Pmu_2d},
  {300, {} }
  };
  
  
     Block2D *b2d_trackLength_BIN_EDGES_Pmu = new Block2D(branchexpr_reco_scheme1,
      "trkLen;cm;P_{#mu};GeV/c", 
      "trkLen;cm;P_{#mu};GeV/c",
      trackLength_BIN_EDGES_Pmu, selection_reco, kOrdinaryRecoBin);

vect_block.emplace_back(b2d_trackLength_BIN_EDGES_Pmu, b2d_trackLength_BIN_EDGES_Pmu);

*/

  // CATEGORY is the branchexpr
  // background_index is vector of background categories.
  //CATEGORY = "category";
  CATEGORY = "JOINTCC0pi_EventCategory";
  background_index = {0, 5, 7, 8, 9, 10, 11};



 //vect_sideband.emplace_back(b2d_TrackLength_reco);


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
/*





  

  
   
    
    vect_sideband.emplace_back(b2d_Mulitply_TrackLength_reco);
    
  
      std::map< double, std::vector<double> > multiply_costheta  = {
  {-0.5, costheta}, 
  {0.5, costheta},
  {1.5, costheta},
  {2.5, costheta},
  {3.5, {} }
  }; 
   branchexpr_reco_scheme1 = "sel_num_proton_candidates;p3mu_mcs.CosTheta();";
      Block2D *b2d_Mulitply_cos_reco = new Block2D(branchexpr_reco_scheme1,
      "Nprotons;;cos#theta;", 
      "Nprotons;;cos#theta;",
      multiply_costheta, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b2d_Mulitply_cos_reco);
  
  
  
  
  

  
  
  vect_sideband.emplace_back(b2d_Mulitply_trklen_reco);  

  
  

  
  vect_sideband.emplace_back(b2d_Mulitply_pmu_reco);
  
  
  */
  

}
