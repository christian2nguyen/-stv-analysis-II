// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/CCXp0piBinScheme.hh"
#include "XSecAnalyzer/Constants.hh"

CCXp0piBinScheme::CCXp0piBinScheme() : BinSchemeBase( "CCXp0piBinScheme" ) {}

void CCXp0piBinScheme::DefineBlocks() {

  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "stv_tree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1 };

  // Prefix for the output bin and slice configuration text files
 
  out_config_prefix_ = "ccxp0pi_";
//#define TKI_1D
#ifdef TKI_1D
  out_config_prefix_ = "ccxp0pi_TKI_1D_";
#endif

//#define TKI_2D
#ifdef TKI_2D
  out_config_prefix_ = "ccxp0pi_TKI_2D_";
#endif
//#define TKI_3D
#ifdef TKI_3D
  out_config_prefix_ = "ccxp0pi_TKI_3D_";
#endif
//#define MUON_PROTON
#ifdef MUON_PROTON
  out_config_prefix_ = "ccxp0pi_MUON_PROTON_";
#endif

//#define MUON_PROTON_0P
#ifdef MUON_PROTON_0P
  out_config_prefix_ = "ccxp0pi_MUON_PROTON_0P_";
#endif
//#define MUON_PROTON_MULTI_P_MOM_COSTHETA
#ifdef MUON_PROTON_MULTI_P_MOM_COSTHETA
  out_config_prefix_ = "ccxp0pi_MUON_PROTON_MULTI_P_";
#endif
//#define MUON_PROTON_MULTI_P_MOM_COSTHETA_0PNP
#ifdef MUON_PROTON_MULTI_P_MOM_COSTHETA_0PNP
  out_config_prefix_ = "ccxp0pi_MUON_PROTON_MULTI_0PNP_";
#endif


#define MUON_1D_AND_2D_BIN_EDGES
#ifdef MUON_1D_AND_2D_BIN_EDGES
#define  PROTON_MULTIPLICITY_BIN_EDGES
  out_config_prefix_ = "ccxp0pi_CCXP0PI_16JAN2025_CM_";
#endif

//#define CCXP0PI_TEST
#ifdef CCXP0PI_TEST
  out_config_prefix_ = "ccxp0pi_CCXP0PI_TEST_";
#endif




  // Selection to use with this binning scheme
  selection_name_ = "CC1muXp0pi";

  // TDirectory file name to use when producing the univmake output histograms
  out_tdir_name_ = "muon_2d_bin";

  /////// Define the blocks of bins in both true and reco space


  // ****** block of proton multiplicity
  std::vector< double > proton_multi_edges = {0, 1.0, 2.0, 3.0, 10.0  };

  Block1D* proton_multi_t = new Block1D("CC1muXp0pi_sig_mc_num_proton_in_momentum_range", "proton multiplicity", "proton multiplicity", proton_multi_edges, "CC1muXp0pi_MC_Signal", kSignalTrueBin);
  Block1D* proton_multi_r = new Block1D("CC1muXp0pi_sel_num_proton_candidates", "proton multiplicity", "proton multiplicity", proton_multi_edges, "CC1muXp0pi_Selected", kOrdinaryRecoBin);
  vect_block.emplace_back( proton_multi_t, proton_multi_r);

  std::string branchexpr, title, textitle, selection;

  // ****** 1D and 2D muon distributions

#ifdef MUON_1D_AND_2D_BIN_EDGES


  // First block: cos_theta_mu and mom_mu in 1D 
  std::vector< double > cos_theta_mu_1D_edges = { -1., -0.85, -0.775, -0.7,
    -0.625, -0.55, -0.475, -0.4, -0.325, -0.25, -0.175, -0.1, -0.025, 0.05,
    0.125, 0.2, 0.275, 0.35, 0.425, 0.5, 0.575, 0.65, 0.725, 0.8, 0.85,
    0.875, 0.9, 0.925, 0.950, 0.975, 1. };
  Block1D* mu_costh_t = new Block1D( "mc_p4_mu->CosTheta()",
    "Xp muon cos#theta", "\\cos\\theta_{\\mu}", cos_theta_mu_1D_edges,
    "CC1muXp0pi_MC_Signal", kSignalTrueBin );
  Block1D* mu_costh_r = new Block1D( "reco_p4mu->CosTheta()",
    "Xp muon cos#theta", "\\cos\\theta_{\\mu}", cos_theta_mu_1D_edges,
    "CC1muXp0pi_Selected", kOrdinaryRecoBin );
  vect_block.emplace_back( mu_costh_t, mu_costh_r );

  // Second block: p_mu in 1D
  std::vector< double > pmu_1D_edges = { 0.1, 0.175, 0.2, 0.225, 0.25,
    0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5, 0.55, 0.6,
    0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.1, 1.2 };

  Block1D* mu_rho_t = new Block1D( "mc_p4_mu->Rho()",
    "Xp p_{#mu}; (GeV)", "p_{\\mu}; (GeV)", pmu_1D_edges,
    "CC1muXp0pi_MC_Signal", kSignalTrueBin );

  Block1D* mu_rho_r = new Block1D( "reco_p4mu->Rho()",
    "Xp p_{#mu}; (GeV)", "p_{\\mu}; (GeV)", pmu_1D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );

  vect_block.emplace_back( mu_rho_t, mu_rho_r );


  std::map< double, std::vector<double> > MUON_2D_BIN_EDGES = {
//
    // No need for an underflow bin: due to the signal definition, all muons
    // with reco momentum below 0.1 GeV/c will be lost
    { 0.1, { -1, -0.55, -0.25, 0., 0.25, 0.45, 0.7, 1.00 }, },
    { 0.24, { -1, -0.55, -0.25, 0., 0.25, 0.45, 0.7, 1.00 } },
    { 0.3,  { -1, -0.4, -0.1, 0.1, 0.35, 0.5, 0.7, 0.85, 1. } },
    { 0.38, { -1, 0, 0.5, 0.65, 0.8, 0.92, 1.00 } },
    { 0.48, { -1, 0.2, 0.5, 0.65, 0.8, 0.875, 0.950, 1.00 } },
    { 0.7, { -1, 0.65, 0.8, 0.875, 0.950, 1.00 } },
    { 0.85, { -1, 0.85, 0.9, 0.950, 1.00 } },

    // Upper edge of the last bin. Due to the signal definition, no overflow
    // bin is needed for muons above 1.2 GeV/c
    { 1.2, {} }

  };

  Block2D *b2dmuon_mom_costheta_true = new Block2D("mc_p4_mu->Rho(); GeV/c; mc_p4_mu->CosTheta(); ", 
      "Xp 2D muon mom; GeV/c;Xp 2D muon cos#theta; ",
      "P_{\\mu}; GeV/c; \\cos\\theta; ",
      MUON_2D_BIN_EDGES, "CC1muXp0pi_MC_Signal", kSignalTrueBin);
  Block2D *b2dmuon_mom_costheta_reco = new Block2D("reco_p4mu->Rho(); GeV/c; reco_p4mu->CosTheta(); ",
      "Xp 2D muon mom; GeV/c;Xp 2D muon cos#theta; ", 
      "P_{\\mu}; GeV/c; \\cos\\theta; ",
      MUON_2D_BIN_EDGES, "CC1muXp0pi_Selected", kOrdinaryRecoBin);
  vect_block.emplace_back(b2dmuon_mom_costheta_true, b2dmuon_mom_costheta_reco);



  // cos_theta_p in 1D; merging all Np channels
  std::map< double, std::vector< double > > proton_multi_cos_theta_Np_2D_edges = {
    {0.0, {-DBL_MAX, DBL_MAX}},
    {1.0, { -1., -0.9, -0.75, -0.6,
  -0.45, -0.3, -0.15, 0.0, 0.15, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.85, 0.9,
  0.925, 0.95, 0.975, 1.0 }},
    {10, {}}
  };

  Block2D* cos_theta_Np_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;mc_p4p->CosTheta();",
    "proton multiplicity ;; Np cos#theta_{P};", "proton multiplicity ;;\\cos\\theta_{p};", proton_multi_cos_theta_Np_2D_edges,
    "CC1muXp0pi_MC_Signal ", kSignalTrueBin );
  Block2D* cos_theta_Np_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4p->CosTheta();",
    "proton multiplicity ;;Np cos#theta_{P};", "proton multiplicity ;;\\cos\\theta_{p};", proton_multi_cos_theta_Np_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( cos_theta_Np_t, cos_theta_Np_r );


// p_p in 1D; merging all Np channels
  std::map< double, std::vector< double > > proton_multi_Np_rho_2D_edges = {
    {0.0, {-DBL_MAX, DBL_MAX}},
    {1.0, { 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0 }},
    {10.0, {}}
  };

  Block2D* Np_rho_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;mc_p4p->Rho();GeV/c",
    "proton multiplicity ;;Np p_{P};", "proton multiplicity ;;p_{P};", proton_multi_Np_rho_2D_edges,
    "CC1muXp0pi_MC_Signal ", kSignalTrueBin );
  Block2D* Np_rho_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4p->Rho();",
    "proton multiplicity ;;Np p_{P};", "proton multiplicity ;;p_{P};", proton_multi_Np_rho_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( Np_rho_t, Np_rho_r );

  // p_p and cosine theta proton in 3D; merging all Np channels
  std::map<double,  std::map< double, std::vector< double > > > proton_multi_Np_rho_3D_edges = {
    {0.0, {{-DBL_MAX, {}}, {DBL_MAX, {}}}},
    {1.0, {
            { 0.250, { -1, 0., 1.0 } },
            { 0.325, { -1, -0.5, 0, 0.5, 0.8, 1.0 } },
            { 0.4,   { -1, -0.6, -0.2, 0.2, 0.5, 0.65, 0.85, 1.0 } },
            { 0.5, { -1, -0.2, 0.2, 0.4, 0.6, 0.7, 0.8, 0.9, 1.0 } },
            { 0.6,   { -1, 0.1, 0.37, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 } },
            { 0.7,   { -1, 0.45, 0.65, 0.75, 0.82, 0.9, 1.0 } },
            { 1., {} }
          }
    },
    {10.0, {{{}}}}
  };

  Block3D* Np_rho_3D_t = new Block3D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;mc_p4p->Rho();GeV/c;mc_p4p->CosTheta();",
      "proton multiplicity ;;Np p_{P};GeV/c; cos#theta_{p};", "proton multiplicity ;;p_{P};GeV/c; cos\\theta_{p};", proton_multi_Np_rho_3D_edges,
      "CC1muXp0pi_MC_Signal ", kSignalTrueBin );
  Block3D* Np_rho_3D_r = new Block3D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4p->Rho();GeV/c;reco_p4p->CosTheta();",
      "proton multiplicity ;;Np p_{P};GeV/c; cos#theta_{p};", "proton multiplicity ;;p_{P};GeV/c; cos\\theta_{p};", proton_multi_Np_rho_3D_edges,
      "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( Np_rho_3D_t, Np_rho_3D_r );


#endif




#ifdef PROTON_MULTIPLICITY_BIN_EDGES

  std::map<double, std::vector< double > > proton_multi_cos_theta_mu_2D_edges = { 
    {0., { -1, 0.275, 0.425, 0.575, 0.725, 0.85, 0.9, 0.95, 1}},
    {1., { -1., -0.85, -0.775, -0.7, -0.625, -0.55, -0.475, -0.4, -0.325, -0.25, -0.175, -0.1, -0.025, 0.05, 0.125, 0.2, 0.275, 0.35, 0.425, 0.5, 0.575, 0.65, 0.725, 0.8, 0.85, 0.875, 0.9, 0.925, 0.950, 0.975, 1. }},
    {2., { -1, 0.275, 0.425, 0.575, 0.725, 0.85, 0.9, 0.95, 1}},
    {3., { -1, 0.275, 0.425, 0.575, 0.725, 0.85, 0.9, 0.95, 1}},
    {10., {}}
  };

  Block2D* mu_costh_0p_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ; mc_p4_mu->CosTheta(); ",
    " proton multiplicity; ; 0p muon cos#theta;", "proton multiplicity ;; \\cos\\theta_{\\mu};", proton_multi_cos_theta_mu_2D_edges,
    "CC1muXp0pi_MC_Signal", kSignalTrueBin );
  Block2D* mu_costh_0p_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;; reco_p4mu->CosTheta(); ",
    "proton multiplicity; ; 0p muon cos#theta;", "proton multiplicity; ; \\cos\\theta_{\\mu};", proton_multi_cos_theta_mu_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( mu_costh_0p_t, mu_costh_0p_r );


  std::map< double , std::vector< double > > proton_multi_pmu_2D_edges = {
    {0., { 0.1, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.2}},
    {1., { 0.1, 0.175, 0.2, 0.225, 0.25, 0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.1, 1.2 }},
    {2., { 0.1, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.2}},
    {3., { 0.1, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.2}},
    {10., {}}
  };

  Block2D* mu_rho_0p_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ;mc_p4_mu->Rho(); GeV/c",
    "proton multiplicity; ;0p p_{#mu}; (GeV)", "proton multiplicity; ;p_{\\mu}; (GeV)", proton_multi_pmu_2D_edges,
    "CC1muXp0pi_MC_Signal", kSignalTrueBin );

  Block2D* mu_rho_0p_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4mu->Rho(); GeV/c",
    "proton multiplicity; ;0p p_{#mu}; (GeV)", "proton multiplicity; ;p_{\\mu}; (GeV)", proton_multi_pmu_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );

  vect_block.emplace_back( mu_rho_0p_t, mu_rho_0p_r );



  // cos_theta_p in 2D
  std::map< double, std::vector< double > > proton_multi_cos_theta_p_2D_edges = {
    {0.0, {-DBL_MAX, DBL_MAX}},
    {1.0, { -1., -0.9, -0.75, -0.6,
  -0.45, -0.3, -0.15, 0.0, 0.15, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.85, 0.9,
  0.925, 0.95, 0.975, 1.0 }},
    {2.0, { -1., -0.9, -0.75, -0.6,
  -0.45, -0.3, -0.15, 0.0, 0.15, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.85, 0.9,
  0.925, 0.95, 0.975, 1.0 }},
    {3.0, { -1., -0.9, -0.75, -0.6,
  -0.45, -0.3, -0.15, 0.0, 0.15, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.85, 0.9,
  0.925, 0.95, 0.975, 1.0 }},
    {10, {}}
  };

  Block2D* cos_theta_p_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;mc_p4p->CosTheta();",
    "proton multiplicity ;; Np cos#theta_{P};", "proton multiplicity ;;\\cos\\theta_{p};", proton_multi_cos_theta_p_2D_edges,
    "CC1muXp0pi_MC_Signal ", kSignalTrueBin );
  Block2D* cos_theta_p_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4p->CosTheta();",
    "proton multiplicity ;;Np cos#theta_{P};", "proton multiplicity ;;\\cos\\theta_{p};", proton_multi_cos_theta_p_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( cos_theta_p_t, cos_theta_p_r );

// p_p in 1D
  std::map< double, std::vector< double > > proton_multi_p_rho_2D_edges = {
    {0.0, {-DBL_MAX, DBL_MAX}},
    {1.0, { 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55,
  0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0 }},
    {2.0, { 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55,
  0.6, 0.9 }},
    {3.0, { 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55,
  0.6, 0.9 }},
    {10.0, {}}
  };

  Block2D* p_rho_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;mc_p4p->Rho();GeV/c",
    "proton multiplicity ;;Np p_{P};", "proton multiplicity ;;p_{P};", proton_multi_p_rho_2D_edges,
    "CC1muXp0pi_MC_Signal ", kSignalTrueBin );
  Block2D* p_rho_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4p->Rho();",
    "proton multiplicity ;;Np p_{P};", "proton multiplicity ;;p_{P};", proton_multi_p_rho_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( p_rho_t, p_rho_r );




  std::map<double, std::vector< double >> proton_multi_pn_2D_edges = { 
    {0, {-DBL_MAX, DBL_MAX}},
    {1.0, {  0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
    {2.0, {  0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.9 }},
    {3.0, {  0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.9 }},
    {10.0, {}}
    };
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ; CC1muXp0pi_mc_lead_proton_pn ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "proton multiplicity; ; leading proton Np p_{n}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multiplicity; ; leading proton  \\p_{n}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal";
  Block2D *b2dt_Np_delta_pn = new Block2D(branchexpr, title, textitle, proton_multi_pn_2D_edges, selection, kSignalTrueBin);
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sel_num_proton_candidates; ; CC1muXp0pi_lead_proton_pn ; GeV/c";
  // the title "title; unit" is used in plot in root style
  // 
  // the tex title "tex title; units" is used in latex format
  // selection
  selection = "CC1muXp0pi_Selected";
  // only the name of branch and the selection is different from true.
  Block2D *b2dr_Np_delta_pn = new Block2D(branchexpr, title, textitle, proton_multi_pn_2D_edges, selection, kOrdinaryRecoBin);
  vect_block.emplace_back(b2dt_Np_delta_pn, b2dr_Np_delta_pn);



  std::map<double, std::vector< double >> proton_multi_pT_2D_edges = { 
    {0, {-DBL_MAX, DBL_MAX}},
    {1.0, {  0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 1.2 }},
    {2.0, {  0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 1.2 }},
    {3.0, {  0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 1.2 }},
    {10.0, {}}
    };
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;CC1muXp0pi_mc_lead_proton_pT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "proton multiplicity; ;leading proton Np p_{T}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multiplicity; ;leading proton  \\p_{T}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal";
  Block2D *b2Dt_Np_delta_pT = new Block2D(branchexpr, title, textitle, proton_multi_pT_2D_edges, selection, kSignalTrueBin);
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sel_num_proton_candidates;;CC1muXp0pi_lead_proton_pT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  // 
  // the tex title "tex title; units" is used in latex format
  // selection
  selection = "CC1muXp0pi_Selected";
  // only the name of branch and the selection is different from true.
  Block2D *b2Dr_Np_delta_pT = new Block2D(branchexpr, title, textitle, proton_multi_pT_2D_edges, selection, kOrdinaryRecoBin);
  vect_block.emplace_back(b2Dt_Np_delta_pT, b2Dr_Np_delta_pT);



  std::map<double, std::vector< double >> proton_multi_delta_alphaT_2D_edges = {
    {0, {-DBL_MAX, DBL_MAX}},
    {1.0, {0, 0.19635, 0.392699, 0.589049, 0.785398, 0.981748, 1.1781, 1.37445, 1.5708, 1.76715, 1.9635, 2.15984, 2.35619, 2.55254, 2.74889, 2.94524, TMath::Pi()}},
    {2.0, {0, 0.19635, 0.392699, 0.589049, 0.785398, 0.981748, 1.1781, 1.37445, 1.5708, 1.76715, 1.9635, 2.15984, 2.35619, 2.55254, 2.74889, 2.94524, TMath::Pi()}},
    {3.0, {0, 0.19635, 0.392699, 0.589049, 0.785398, 0.981748, 1.1781, 1.37445, 1.5708, 1.76715, 1.9635, 2.15984, 2.35619, 2.55254, 2.74889, 2.94524, TMath::Pi()}},
    {10.0, {}}
  };
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sig_mc_num_proton_in_momentum_range ;; CC1muXp0pi_mc_lead_proton_delta_alphaT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "proton multiplicity; ;leading proton Np #delta #alpah_{T}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multiplicity; ;leading proton  \\p_{T}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal";
  Block2D *b2Dt_Np_delta_delta_alphaT = new Block2D(branchexpr, title, textitle, proton_multi_delta_alphaT_2D_edges, selection, kSignalTrueBin);
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sel_num_proton_candidates;; CC1muXp0pi_lead_proton_delta_alphaT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  // 
  // the tex title "tex title; units" is used in latex format
  // selection
  selection = "CC1muXp0pi_Selected ";
  // only the name of branch and the selection is different from true.
  Block2D *b2Dr_Np_delta_delta_alphaT = new Block2D(branchexpr, title, textitle, proton_multi_delta_alphaT_2D_edges, selection, kOrdinaryRecoBin);
  vect_block.emplace_back(b2Dt_Np_delta_delta_alphaT, b2Dr_Np_delta_delta_alphaT);



  std::map<double, std::vector< double > > proton_multi_delta_phiT_2D_edges = {
    {0, {-DBL_MAX, DBL_MAX}},
    {1.0, { 0, 0.0616616, 0.127377, 0.197716, 0.27338, 0.35524, 0.444405, 0.542304, 0.650839, 0.772604, 0.911278, 1.07233, 1.26439, 1.50232, 1.81515, 2.27288, TMath::Pi()}},
    {2.0, { 0, 0.0616616, 0.127377, 0.197716, 0.27338, 0.35524, 0.444405, 0.542304, 0.650839, 0.772604, 0.911278, 1.07233, 1.26439, 1.50232, 1.81515, 2.27288, TMath::Pi()}},
    {3.0, { 0, 0.0616616, 0.127377, 0.197716, 0.27338, 0.35524, 0.444405, 0.542304, 0.650839, 0.772604, 0.911278, 1.07233, 1.26439, 1.50232, 1.81515, 2.27288, TMath::Pi()}},
    {10.0, {}}
  };

  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;; CC1muXp0pi_mc_lead_proton_delta_phiT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "proton multiplicity; ;leading proton  Np #delta #phi_{T}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multiplicity; ;leading proton  \\p_{T}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal ";
  Block2D *b2Dt_Np_delta_delta_phiT = new Block2D(branchexpr, title, textitle, proton_multi_delta_phiT_2D_edges, selection, kSignalTrueBin);
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sel_num_proton_candidates;;CC1muXp0pi_lead_proton_delta_phiT ; GeV/c";
  // the title "title; unit" is used in plot in root style
  // 
  // the tex title "tex title; units" is used in latex format
  // selection
  selection = "CC1muXp0pi_Selected";
  // only the name of branch and the selection is different from true.
  Block2D *b2Dr_Np_delta_delta_phiT = new Block2D(branchexpr, title, textitle, proton_multi_delta_phiT_2D_edges, selection, kOrdinaryRecoBin);
  vect_block.emplace_back(b2Dt_Np_delta_delta_phiT, b2Dr_Np_delta_delta_phiT);



  std::map< double, std::map< double, std::vector< double > > > proton_KE_pn_3D_edges ={

	  // Put the 0p into one bin by this setup.
    { 0, {{-DBL_MAX,  {}}, {DBL_MAX, {}}}},
    {1.0, 
      {{0.03 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.0539999 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.0812775 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.112872 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.150411 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.196664 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.256935 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       { 0.343614, { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       { 0.5, { }}
      }
    },
    {2.0,
      {{0.0, { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.5, {}}
      }
    },
    {10.0, {{{}}}}
  };
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ; (mc_p4p->E() - mc_p4p->M()) ; GeV; CC1muXp0pi_mc_lead_proton_pn ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "proton multi; ; leading proton 1p 2D kinetic energy ; GeV; leading proton 1p 2D p_{n}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multi; ; leading proton  KE; GeV; leading proton  \\p_{n}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal";
  Block3D *b3dt_proton_KE_pn = new Block3D(branchexpr, title, textitle, proton_KE_pn_3D_edges, selection, kSignalTrueBin);
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sel_num_proton_candidates; ;(reco_p4p->E() - reco_p4p->M()) ; GeV; CC1muXp0pi_lead_proton_pn ; GeV/c";
  // the title "title; unit" is used in plot in root style
  // 
  // the tex title "tex title; units" is used in latex format
  // selection
  selection = "CC1muXp0pi_Selected";
  // only the name of branch and the selection is different from true.
  Block3D *b3dr_proton_KE_pn = new Block3D(branchexpr, title, textitle, proton_KE_pn_3D_edges, selection, kOrdinaryRecoBin);
  vect_block.emplace_back(b3dt_proton_KE_pn, b3dr_proton_KE_pn);


#endif


#ifdef MUON_PROTON_MULTI_P_MOM_COSTHETA_0PNP

  std::map<double, std::vector< double > > cos_theta_mu_2D_edges = { 
    {0., { -1, 0.275, 0.425, 0.575, 0.725, 0.85, 0.9, 0.95, 1}},
    {1., { -1., -0.85, -0.775, -0.7, -0.625, -0.55, -0.475, -0.4, -0.325, -0.25, -0.175, -0.1, -0.025, 0.05, 0.125, 0.2, 0.275, 0.35, 0.425, 0.5, 0.575, 0.65, 0.725, 0.8, 0.85, 0.875, 0.9, 0.925, 0.950, 0.975, 1. }},
    {10., {}}
  };

// First block: cos_theta_mu and mom_mu in 1D 
//  std::vector< double > cos_theta_mu_1D_edges = { -1., -0.85, -0.775, -0.7,
//    -0.625, -0.55, -0.475, -0.4, -0.325, -0.25, -0.175, -0.1, -0.025, 0.05,
//    0.125, 0.2, 0.275, 0.35, 0.425, 0.5, 0.575, 0.65, 0.725, 0.8, 0.85,
//    0.875, 0.9, 0.925, 0.950, 0.975, 1. };
  Block2D* mu_costh_0p_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ; mc_p4_mu->CosTheta(); ",
    " proton multiplicity; ; 0p muon cos#theta;", "proton multiplicity ;; \\cos\\theta_{\\mu};", cos_theta_mu_2D_edges,
    "CC1muXp0pi_MC_Signal", kSignalTrueBin );
  Block2D* mu_costh_0p_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;; reco_p4mu->CosTheta(); ",
    "proton multiplicity; ; 0p muon cos#theta;", "proton multiplicity; ; \\cos\\theta_{\\mu};", cos_theta_mu_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( mu_costh_0p_t, mu_costh_0p_r );


  std::map< double , std::vector< double > > pmu_2D_edges = {
    {0., { 0.1, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.2}},
    {1., { 0.1, 0.175, 0.2, 0.225, 0.25, 0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.1, 1.2 }},
    {10., {}}
  };

  // Second block: p_mu in 1D
//  std::vector< double > pmu_1D_edges = { 0.1, 0.175, 0.2, 0.225, 0.25,
//    0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5, 0.55, 0.6,
//    0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0, 1.1, 1.2 };

  Block2D* mu_rho_0p_t = new Block2D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ;mc_p4_mu->Rho(); GeV/c",
    "proton multiplicity; ;0p p_{#mu}; (GeV)", "proton multiplicity; ;p_{\\mu}; (GeV)", pmu_2D_edges,
    "CC1muXp0pi_MC_Signal", kSignalTrueBin );

  Block2D* mu_rho_0p_r = new Block2D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4mu->Rho(); GeV/c",
    "proton multiplicity; ;0p p_{#mu}; (GeV)", "proton multiplicity; ;p_{\\mu}; (GeV)", pmu_2D_edges,
    "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );

  vect_block.emplace_back( mu_rho_0p_t, mu_rho_0p_r );

#endif


#ifdef CCXP0PI_TEST

  std::map< double, std::map< double, std::vector< double > > > proton_KE_pn_3D_edges ={

	  // Put the 0p into one bin by this setup.
    { 0, {{-DBL_MAX,  {}}, {DBL_MAX, {}}}},
    {1.0, 
      {{0.03 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.0539999 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.0812775 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.112872 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.150411 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.196664 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.256935 , { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       { 0.343614, { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       { 0.5, { }}
      }
    },
    {2.0,
      {{0.0, { 0., 0.06, 0.12, 0.18, 0.24, 0.3, 0.45, 0.6, 0.75, 0.9, 1.2 }},
       {0.5, {}}
      }
    },
    {10.0, {{{}}}}
  };
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sig_mc_num_proton_in_momentum_range; ; (mc_p4p->E() - mc_p4p->M()) ; GeV; CC1muXp0pi_mc_lead_proton_pn ; GeV/c";
  // the title "title; unit" is used in plot in root style
  title = "proton multi; ; leading proton 1p 2D kinetic energy ; GeV; leading proton 1p 2D p_{n}; GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "proton multi; ; leading proton  KE; GeV; leading proton  \\p_{n}; GeV/c";
  // selection
  selection = "CC1muXp0pi_MC_Signal";
  Block3D *b3dt_proton_KE_pn = new Block3D(branchexpr, title, textitle, proton_KE_pn_3D_edges, selection, kSignalTrueBin);
  // the branch name of the truth of proton momentum in stv root files; unit is optional with format "branch name; unit"
  branchexpr = "CC1muXp0pi_sel_num_proton_candidates; ;(reco_p4p->E() - reco_p4p->M()) ; GeV; CC1muXp0pi_lead_proton_pn ; GeV/c";
  // the title "title; unit" is used in plot in root style
  // 
  // the tex title "tex title; units" is used in latex format
  // selection
  selection = "CC1muXp0pi_Selected";
  // only the name of branch and the selection is different from true.
  Block3D *b3dr_proton_KE_pn = new Block3D(branchexpr, title, textitle, proton_KE_pn_3D_edges, selection, kOrdinaryRecoBin);
  vect_block.emplace_back(b3dt_proton_KE_pn, b3dr_proton_KE_pn);


  // p_p and cosine theta proton in 3D; merging all Np channels
  std::map<double,  std::map< double, std::vector< double > > > proton_multi_Np_rho_3D_edges = {
    {0.0, {{-DBL_MAX, {}}, {DBL_MAX, {}}}},
    {1.0, {
            { 0.250, { -1, 0., 1.0 } },
            { 0.325, { -1, -0.5, 0, 0.5, 0.8, 1.0 } },
            { 0.4,   { -1, -0.6, -0.2, 0.2, 0.5, 0.65, 0.85, 1.0 } },
            { 0.5, { -1, -0.2, 0.2, 0.4, 0.6, 0.7, 0.8, 0.9, 1.0 } },
            { 0.6,   { -1, 0.1, 0.37, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 } },
            { 0.7,   { -1, 0.45, 0.65, 0.75, 0.82, 0.9, 1.0 } },
            { 1., {} }
          }
    },
    {10.0, {{{}}}}
  };

  Block3D* Np_rho_3D_t = new Block3D( "CC1muXp0pi_sig_mc_num_proton_in_momentum_range;;mc_p4p->Rho();GeV/c;mc_p4p->CosTheta();",
      "proton multiplicity ;;Np p_{P};GeV/c; cos#theta_{p};", "proton multiplicity ;;p_{P};GeV/c; cos\\theta_{p};", proton_multi_Np_rho_3D_edges,
      "CC1muXp0pi_MC_Signal ", kSignalTrueBin );
  Block3D* Np_rho_3D_r = new Block3D( "CC1muXp0pi_sel_num_proton_candidates;;reco_p4p->Rho();GeV/c;reco_p4p->CosTheta();",
      "proton multiplicity ;;Np p_{P};GeV/c; cos#theta_{p};", "proton multiplicity ;;p_{P};GeV/c; cos\\theta_{p};", proton_multi_Np_rho_3D_edges,
      "(CC1muXp0pi_Selected)", kOrdinaryRecoBin );
  vect_block.emplace_back( Np_rho_3D_t, Np_rho_3D_r );

#endif



  // a example to use sideband bins; now we only use sideband bins for reco. 
  // you can definie many blocks of sideband with different selections
  // std::vector< double > test_sideband = {1,2,3,4};

  // Block1D *b1r_sideband = new Block1D("sideband", "sideband", "sideband",test_sideband , "selection", kSidebandRecoBin);

  // vect_sideband.emplace_back(b1r_sideband);

  // CATEGORY is the branchexpr
  // background_index is vector of background categories.

#ifdef TKI_3D
  CATEGORY = "CC1muXp0pi_EventCategory";
  background_index = {0, 1, 2, 3, 4,
    21, 22, 23, 24, 25, 26};
#endif
#ifdef TKI_2D
  CATEGORY = "CC1muXp0pi_EventCategory";
  background_index = {0, 1, 2, 3, 4,
    21, 22, 23, 24, 25, 26};
#endif



  CATEGORY = "CC1muXp0pi_EventCategory";
  background_index = {0, 21, 22, 23, 24, 25, 26};

}
