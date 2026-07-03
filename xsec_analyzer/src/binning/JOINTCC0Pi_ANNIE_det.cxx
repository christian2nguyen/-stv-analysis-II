// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/JOINTCC0Pi_ANNIE_det.hh"

JOINTCC0Pi_ANNIE_det::JOINTCC0Pi_ANNIE_det() : BinSchemeBase( "JOINTCC0Pi_ANNIE_det" ) {}

void JOINTCC0Pi_ANNIE_det::DefineBlocks() {

  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "phaseIITriggerTree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1 };

  // Prefix for the output bin and slice configuration text files
  out_config_prefix_ = "JOINTCC0pi_ANNIE_BDT_Sidebands_v1";

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
  std::vector<double> PmuBinnEdges_wc_extra{.5,0.60, 0.74, 0.86, 1.00, 1.10, 1.2,1.5}; // Annie Binning 

   std::vector<double> costheta = {0.7, 0.8, .9, 1.0};
   std::vector<double> Nprotonsedges{-0.5,0.5, 1.5, 2.5, 3.5};
   std::vector<double> MRDtracklenght{0,45,50,60,65,70,90,110,120,125,135,200,250};
   std::vector<double> MRDtracklenght_vertex_toMRD{0,80,120,160,200,240,280,320,360,380,420,460,500,540,580,620,700,800};
   std::vector<double> PromptPE{0,200,400,600,800,1000,1200,1400,1600,1800,2000,2200,2400,2600,2800,3000,3200,3400,3600};
   std::vector<double> VertexX{-125,-100,-75,-50,-25,0.0, 25,50,75,100,125};
   std::vector<double> VertexY{-125,-100,-75,-50,-25,0.0, 25,50,75,100,125};
   std::vector<double> VertexZ{-125,-100,-75,-50,-25,0.0, 25,50,75,100,125};
   
   std::vector<double> VertexR{0,20,40,60,80,100,110,125};
   std::vector<double> VertexRR{0,400,1600,3600,6400,10000,12100,15625};
   std::vector<double> Qij{0,.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0};
  std::vector<double> NMRDtracks{0.5, 1.5, 2.5};
  
   std::vector<double> BTD_PredictionSideband = {0.0,.2,.4, 0.45, .55,.6,.65,.7,.75,.76,.77,.78,.79,.8,.81,.82,.83,.84,.85,.86,.87,.88,.89 ,.9, .925};

       // the title "title; unit" is used in plot in root style
  title ="p_{mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{mu};GeV/c";
  // selection
    double zShift_center = 168.1;
  selection_true = "mc_is_cc0pi_signal";
  selection_reco = "sel_CC0pi_wc  && tank_bdt_no_pion_score > 0.8";
  branchexpr_reco_pmu = "pmu_gev;GeV/c" ;
  branchexpr_true_pmu= "mc_pmu_gev;GeV/c" ;
  
  
  


  Block1D *Pmu_true = new Block1D(branchexpr_true_pmu, title, textitle, PmuBinnEdges_wc, selection_true, kSignalTrueBin);
  // only the name of branch and the selection is different from true.
  Block1D *Pmu_reco = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc, selection_reco, kOrdinaryRecoBin);
  vect_block.emplace_back(Pmu_true , Pmu_reco);
/////////////////////////////////


  // CATEGORY is the branchexpr
  // background_index is vector of background categories.
  CATEGORY ="category";
  //CATEGORY = "category";
  background_index = {0, 5, 7, 8, 9, 10, 11};


//const std::string CCNPI_SIDEBAND_SELECTION =
//  "JOINTCC0pi_ANNIE_sel_nu_mu_cc"
//  " && JOINTCC0pi_ANNIE_sel_has_pion_candidate";
    const std::string SELECTION =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_promptMuonTotalPE" 
  " && sel_costheta_threshold"
  " && sel_muon_passed_mom_cuts";
  
  const std::string SELECTION_noFV =
   "sel_nu_mu_cc"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_promptMuonTotalPE" 
  " && sel_costheta_threshold"
  " && sel_muon_passed_mom_cuts";
  
    const std::string SELECTION_noQij =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_promptMuonTotalPE" 
  " && sel_costheta_threshold"
  " && sel_muon_passed_mom_cuts";
  
    const std::string SELECTION_noPrompt =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_costheta_threshold"
  " && sel_muon_passed_mom_cuts";
  
      const std::string SELECTION_noCostheta =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_promptMuonTotalPE" 
  " && sel_muon_passed_mom_cuts";
  
        const std::string SELECTION_Taged_FMV =
        "sel_simpleRecoFlag &&"
        "sel_contained_in_MRD && "
        "sel_trigword  && "
        "sel_HasTank && "
        "sel_HasMRD && "
        "sel_TankMRDCoinc && "
        "!sel_noVeto"
        " && sel_muon_passed_mom_cuts";
  
        const std::string SELECTION_noPmu =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_promptMuonTotalPE"
  " && sel_costheta_threshold"
  ;
  
          const std::string SELECTION_noPmuandnocostheta =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score > 0.8"
  " && sel_promptMuonTotalPE"
  ;
  
  
      const std::string SELECTION_thoughtgoing =
   "trigword ==5"
  " && HasTank==1"
  " && HasMRD ==1"
  " && TankMRDCoinc==1" 
  " && NoVeto==0"
  " && 30 < tracklenghtMRD_cm ";
  
  
      const std::string SELECTION_BDT =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && sel_promptMuonTotalPE" 
  " && sel_costheta_threshold"
  " && sel_muon_passed_mom_cuts";
  
      const std::string SELECTION_Pion =
  "sel_nu_mu_cc"
  " && sel_vertex_in_FV"
  " && tank_bdt_no_pion_score < 0.8"
  " && sel_promptMuonTotalPE" 
  " && sel_costheta_threshold"
  " && sel_muon_passed_mom_cuts";
  

  
    //Block1D *Pmu_reco_FMV_tag = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc, SELECTION_Taged_FMV, kSidebandRecoBin);
    //vect_sideband.emplace_back(Pmu_reco_FMV_tag);
  
    //////////////////////////////////////////////////////////////////////////
    
    /*
    Block1D *b1dt_pmu_reco_nopmu = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc_extra, SELECTION_noPmu, kSidebandRecoBin);
    vect_sideband.emplace_back(b1dt_pmu_reco_nopmu);
  
    Block1D *b1dt_pmu_reco_nopmuandnocosetheta = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc_extra, SELECTION_noPmuandnocostheta, kSidebandRecoBin);
    vect_sideband.emplace_back(b1dt_pmu_reco_nopmuandnocosetheta);
  
  
  
            // the title "title; unit" is used in plot in root style
  title ="cos#theta_{#mu};";
  // the tex title "tex title; units" is used in latex format
   textitle = "cos//theta_{//mu};";
  // selection
  std::string branchexpr_reco_costheta = "simpleRecoCosTheta;";
  std::string  branchexpr_true_costheta = "trueCosTheta;";

  // Will make this a side band for now since have to use different cross section extraction scheme 
  Block1D *b1dt_costheta_reco = new Block1D( branchexpr_reco_costheta, title, textitle, costheta, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1dt_costheta_reco);
  
    Block1D *b1dt_costheta_reco_nocostheta = new Block1D( branchexpr_reco_costheta, title, textitle, costheta, SELECTION_noCostheta, kSidebandRecoBin);
  vect_sideband.emplace_back(b1dt_costheta_reco_nocostheta);
  
  //didnt see events when !sel_noVeto
  //Block1D *b1dt_costheta_reco_FMV_tag = new Block1D( branchexpr_reco_costheta, title, textitle, costheta, SELECTION_Taged_FMV, kSidebandRecoBin);
  //vect_sideband.emplace_back(b1dt_costheta_reco_FMV_tag);
  
 branchexpr_sideband = "lenghtvertextoStartMRD_cm;";
   // the title "title; unit" is used in plot in root style
  title ="tracklenght;[cm]";
  // the tex title "tex title; units" is used in latex format
  textitle = "tracklenght;[cm]";
///////////////////////////////////////////////
  Block1D *b1r_sideband_trklengintank = new Block1D(branchexpr_sideband, title, textitle, MRDtracklenght_vertex_toMRD, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_trklengintank);
  
  branchexpr_sideband = "tracklenghtMRD_cm;";
  
  Block1D *b1r_sideband_trklengInMRD = new Block1D(branchexpr_sideband, title, textitle, MRDtracklenght, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_trklengInMRD);
///////////////////////////////////////////////
 branchexpr_sideband = "numMRDTracks;";
   // the title "title; unit" is used in plot in root style
title ="Ntrks;[cm]";
  // the tex title "tex title; units" is used in latex format
textitle = "Ntrks;[cm]";
Block1D *b1dt_NMRDtracks = new Block1D( branchexpr_sideband, title, textitle, NMRDtracks, selection_reco, kSidebandRecoBin);
vect_sideband.emplace_back(b1dt_NMRDtracks);
  ////////////////////////////////////////////
 branchexpr_sideband = "vertexX;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex X;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex X;cm";
  Block1D *b1r_sideband_vertexX_noFV = new Block1D(branchexpr_sideband, title, textitle, VertexX, SELECTION_noFV, kSidebandRecoBin);
  Block1D *b1r_sideband_vertexX = new Block1D(branchexpr_sideband, title, textitle, VertexX, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexX_noFV);
  vect_sideband.emplace_back(b1r_sideband_vertexX);
//////////////////////////////////////////////////////////////

 branchexpr_sideband = "vertexY;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex Y;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex Y;cm";
  Block1D *b1r_sideband_vertexY_noFV = new Block1D(branchexpr_sideband, title, textitle, VertexY, SELECTION_noFV, kSidebandRecoBin);
  Block1D *b1r_sideband_vertexY = new Block1D(branchexpr_sideband, title, textitle, VertexY, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexY_noFV);
  vect_sideband.emplace_back(b1r_sideband_vertexY);

//////////////////////////////////////////////////////////////
 branchexpr_sideband = "vertexR;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex R;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex R;cm";
  Block1D *b1r_sideband_vertexR_noFV = new Block1D(branchexpr_sideband, title, textitle, VertexR, SELECTION_noFV, kSidebandRecoBin);
  Block1D *b1r_sideband_vertexR = new Block1D(branchexpr_sideband, title, textitle, VertexR, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexR_noFV);
  vect_sideband.emplace_back(b1r_sideband_vertexR);
//////////////////////////////////////////////////////////////
 branchexpr_sideband = "vertexRR;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex R^{2};cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex R^{2};cm";
  Block1D *b1r_sideband_vertexRR_noFV = new Block1D(branchexpr_sideband, title, textitle, VertexRR, SELECTION_noFV, kSidebandRecoBin);
  Block1D *b1r_sideband_vertexRR = new Block1D(branchexpr_sideband, title, textitle, VertexRR, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexRR_noFV);
  vect_sideband.emplace_back(b1r_sideband_vertexRR);
//////////////////////////////////////////////////////////////

 branchexpr_sideband = "vertexZ;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex Z;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex Z;cm";
  Block1D *b1r_sideband_vertexZ_noFV = new Block1D(branchexpr_sideband, title, textitle, VertexZ, SELECTION_noFV, kSidebandRecoBin);
  Block1D *b1r_sideband_vertexZ = new Block1D(branchexpr_sideband, title, textitle, VertexZ, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexZ_noFV);
  vect_sideband.emplace_back(b1r_sideband_vertexZ);
//////////////////////////////////////////////////////////////

 branchexpr_sideband = "promptMuonTotalPE;";
   // the title "title; unit" is used in plot in root style
  title ="Prompt #mu PE;";
  // the tex title "tex title; units" is used in latex format
  textitle = "Prompt #mu PE;";
  Block1D *b1r_sideband_prompt_noprompt = new Block1D(branchexpr_sideband, title, textitle, PromptPE, SELECTION_noPrompt, kSidebandRecoBin);
  Block1D *b1r_sideband_prompt = new Block1D(branchexpr_sideband, title, textitle, PromptPE, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_prompt_noprompt);
  vect_sideband.emplace_back(b1r_sideband_prompt);
//////////////////////////////////////////////////////////////
  
   branchexpr_sideband = "Qij;";
   // the title "title; unit" is used in plot in root style
  title ="Q_{ij};";
  // the tex title "tex title; units" is used in latex format
  textitle = "Q_{ij};";
  Block1D *b1r_sideband_Qji_noQji = new Block1D(branchexpr_sideband, title, textitle, Qij, SELECTION_noQij, kSidebandRecoBin);
  Block1D *b1r_sideband_Qji = new Block1D(branchexpr_sideband, title, textitle, Qij, selection_reco, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Qji_noQji);
  vect_sideband.emplace_back(b1r_sideband_Qji);
//////////////////////////////////////////////////////////////
  */
  
  // Throught going muons selection 
  /*
  Block1D *b1dt_pmu_reco_nopmu = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc_extra, SELECTION_thoughtgoing, kSidebandRecoBin);
    vect_sideband.emplace_back(b1dt_pmu_reco_nopmu);
  
   branchexpr_sideband = "lenghtvertextoStartMRD_cm;";
   // the title "title; unit" is used in plot in root style
  title ="tracklenght;[cm]";
  // the tex title "tex title; units" is used in latex format
  textitle = "tracklenght;[cm]";
  
  
    Block1D *b1r_sideband_trklengintank = new Block1D(branchexpr_sideband, title, textitle, MRDtracklenght_vertex_toMRD, SELECTION_thoughtgoing, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_trklengintank);
  
  branchexpr_sideband = "tracklenghtMRD_cm;";
  
  Block1D *b1r_sideband_trklengInMRD = new Block1D(branchexpr_sideband, title, textitle, MRDtracklenght, SELECTION_thoughtgoing, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_trklengInMRD);
  
  
  
   branchexpr_sideband = "promptMuonTotalPE;";
   // the title "title; unit" is used in plot in root style
  title ="Prompt #mu PE;";
  // the tex title "tex title; units" is used in latex format
  textitle = "Prompt #mu PE;";
  //Block1D *b1r_sideband_prompt_noprompt = new Block1D(branchexpr_sideband, title, textitle, PromptPE, SELECTION_noPrompt, kSidebandRecoBin);
  Block1D *b1r_sideband_prompt = new Block1D(branchexpr_sideband, title, textitle, PromptPE, SELECTION_thoughtgoing, kSidebandRecoBin);
  //vect_sideband.emplace_back(b1r_sideband_prompt_noprompt);
  vect_sideband.emplace_back(b1r_sideband_prompt);
  
  
  
     branchexpr_sideband = "Qij;";
   // the title "title; unit" is used in plot in root style
  title ="Q_{ij};";
  // the tex title "tex title; units" is used in latex format
  textitle = "Q_{ij};";
  Block1D *b1r_sideband_Qji = new Block1D(branchexpr_sideband, title, textitle, Qij, SELECTION_thoughtgoing, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_Qji);
  
    ////////////////////////////////////////////
 branchexpr_sideband = "vertexX;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex X;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex X;cm";
  
  Block1D *b1r_sideband_vertexX = new Block1D(branchexpr_sideband, title, textitle, VertexX, SELECTION_thoughtgoing, kSidebandRecoBin);
  
  vect_sideband.emplace_back(b1r_sideband_vertexX);
//////////////////////////////////////////////////////////////

 branchexpr_sideband = "vertexY;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex Y;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex Y;cm";

  Block1D *b1r_sideband_vertexY = new Block1D(branchexpr_sideband, title, textitle, VertexY, SELECTION_thoughtgoing, kSidebandRecoBin);

  vect_sideband.emplace_back(b1r_sideband_vertexY);

//////////////////////////////////////////////////////////////
 branchexpr_sideband = "vertexR;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex R;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex R;cm";
  Block1D *b1r_sideband_vertexR = new Block1D(branchexpr_sideband, title, textitle, VertexR, SELECTION_thoughtgoing, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexR);
//////////////////////////////////////////////////////////////
 branchexpr_sideband = "vertexRR;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex R^{2};cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex R^{2};cm";
  Block1D *b1r_sideband_vertexRR = new Block1D(branchexpr_sideband, title, textitle, VertexRR, SELECTION_thoughtgoing, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexRR);
//////////////////////////////////////////////////////////////

 branchexpr_sideband = "vertexZ;cm";
   // the title "title; unit" is used in plot in root style
  title ="Vertex Z;cm";
  // the tex title "tex title; units" is used in latex format
  textitle = "Vertex Z;cm";
  Block1D *b1r_sideband_vertexZ = new Block1D(branchexpr_sideband, title, textitle, VertexZ, SELECTION_thoughtgoing, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_vertexZ);
  
*/

    // std::vector<double> BTD_PredictionSideband = {0.0,.2,.4, 0.45, .55,.6,.65,.7,.75,.76,.77,.78,.79,.8,.81,.82,.83,.84,.85,.86,.87,.88,.89 ,.9, .925};

  
   branchexpr_sideband = "tank_bdt_no_pion_score;";
   // the title "title; unit" is used in plot in root style
  title ="BDT score;";
  // the tex title "tex title; units" is used in latex format
  textitle = "BDT score;";
  Block1D *b1r_sideband_BDT = new Block1D(branchexpr_sideband, title, textitle, BTD_PredictionSideband, SELECTION_BDT, kSidebandRecoBin);
  vect_sideband.emplace_back(b1r_sideband_BDT);
  
  
  
   // the title "title; unit" is used in plot in root style
  title ="p_{mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{mu};GeV/c";
    Block1D *Pmu_reco_pion_sideband = new Block1D( branchexpr_reco_pmu, title, textitle, PmuBinnEdges_wc, SELECTION_Pion, kSidebandRecoBin);
  vect_sideband.emplace_back(Pmu_reco_pion_sideband);
  
}
