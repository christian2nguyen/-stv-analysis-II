// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/JOINTCC0Pi_ANNIE_det.hh"

#include <cstdlib>
#include <fstream>
#include <initializer_list>
#include <map>
#include <set>
#include <sstream>
#include <string>

namespace {

struct SidebandConfig {
  std::set<std::string> enabled_keys;
  std::map<std::string, std::string> selection_profiles;
  std::string output_name;
};

std::string trim_copy(const std::string& input) {
  const std::string whitespace = " \t\r\n";
  const size_t first = input.find_first_not_of(whitespace);
  if (first == std::string::npos) return "";
  const size_t last = input.find_last_not_of(whitespace);
  return input.substr(first, last - first + 1);
}

std::string basename_copy(const std::string& input) {
  const size_t slash_pos = input.find_last_of("/\\");
  if (slash_pos == std::string::npos) return input;
  return input.substr(slash_pos + 1);
}

std::string remove_extension_copy(const std::string& input) {
  const size_t dot_pos = input.find_last_of('.');
  if (dot_pos == std::string::npos) return input;
  return input.substr(0, dot_pos);
}

std::string make_output_prefix_from_config_path(
    const std::string& config_path,
    const std::string& fallback_prefix) {
  const std::string config_name = basename_copy(config_path);
  const std::string config_stem = remove_extension_copy(config_name);
  return config_stem.empty() ? fallback_prefix : config_stem;
}

SidebandConfig load_sideband_config(
    const std::string& config_path,
    const std::initializer_list<std::string>& default_keys) {
  std::ifstream config_stream(config_path);
  SidebandConfig config;
  if (!config_stream) {
    config.enabled_keys =
        std::set<std::string>(default_keys.begin(), default_keys.end());
    return config;
  }

  std::string line;
  while (std::getline(config_stream, line)) {
    const size_t comment_pos = line.find('#');
    if (comment_pos != std::string::npos) line.erase(comment_pos);

    const std::string trimmed_line = trim_copy(line);
    if (trimmed_line.empty()) continue;

    std::istringstream line_stream(trimmed_line);
    std::string key;
    std::string selection_profile;
    line_stream >> key;
    line_stream >> selection_profile;

    if (key.empty()) continue;
    if (key == "output_name") {
      const std::string output_name = trim_copy(selection_profile);
      if (!output_name.empty()) config.output_name = output_name;
      continue;
    }
    config.enabled_keys.insert(key);
    if (!selection_profile.empty()) {
      config.selection_profiles[key] = selection_profile;
    }
  }

  return config;
}

}  // namespace

JOINTCC0Pi_ANNIE_det::JOINTCC0Pi_ANNIE_det() : BinSchemeBase( "JOINTCC0Pi_ANNIE_det" ) {}

void JOINTCC0Pi_ANNIE_det::DefineBlocks() {

  /////// Set some standard variables before managing the blocks

  // TTree name for the post-processed ntuples
  ntuple_ttree_name_ = "phaseIITriggerTree";

  // Run numbers to use when plotting migration matrices
  runs_to_use_ = { 1 };

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

  const char* analyzer_dir = std::getenv("XSEC_ANALYZER_DIR");
  const std::string sideband_config_path = analyzer_dir
    ? std::string(analyzer_dir) + "/configs/JOINTCC0Pi_ANNIE_det_sidebands.conf"
    : "configs/JOINTCC0Pi_ANNIE_det_sidebands.conf";
  const auto sideband_config = load_sideband_config(
      sideband_config_path, {"bdt_no_pion_score", "pion_control_pmu"});
  out_config_prefix_ = !sideband_config.output_name.empty()
      ? sideband_config.output_name
      : make_output_prefix_from_config_path(
            sideband_config_path, "JOINTCC0pi_ANNIE_BDT_Sidebands_v1");
  auto sideband_enabled =
      [&sideband_config](const std::string& key,
                           std::initializer_list<std::string> groups = {}) {
        if (sideband_config.enabled_keys.count("all") > 0u ||
            sideband_config.enabled_keys.count(key) > 0u) {
          return true;
        }

        for (const auto& group : groups) {
          if (sideband_config.enabled_keys.count(group) > 0u) return true;
        }

        return false;
      };
  std::map<std::string, std::string> selection_profiles;
  auto resolve_selection =
      [&](const std::string& key, const std::string& default_selection,
          std::initializer_list<std::string> groups = {}) -> std::string {
        const auto direct_it = sideband_config.selection_profiles.find(key);
        if (direct_it != sideband_config.selection_profiles.end()) {
          const auto profile_it = selection_profiles.find(direct_it->second);
          if (profile_it != selection_profiles.end()) return profile_it->second;
        }

        for (const auto& group : groups) {
          const auto group_it = sideband_config.selection_profiles.find(group);
          if (group_it == sideband_config.selection_profiles.end()) continue;

          const auto profile_it = selection_profiles.find(group_it->second);
          if (profile_it != selection_profiles.end()) return profile_it->second;
        }

        const auto all_it = sideband_config.selection_profiles.find("all");
        if (all_it != sideband_config.selection_profiles.end()) {
          const auto profile_it = selection_profiles.find(all_it->second);
          if (profile_it != selection_profiles.end()) return profile_it->second;
        }

        return default_selection;
      };
  auto add_sideband_1d = [&](const std::string& key, const std::string& branch_expr,
                             const std::string& plot_title,
                             const std::string& plot_textitle,
                             const std::vector<double>& edges,
                             const std::string& default_selection,
                             std::initializer_list<std::string> groups = {}) {
    if (!sideband_enabled(key, groups)) return;
    const std::string selection =
        resolve_selection(key, default_selection, groups);
    Block1D* sideband = new Block1D(branch_expr, plot_title, plot_textitle,
        edges, selection, kSidebandRecoBin);
    vect_sideband.emplace_back(sideband);
  };


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
   std::vector<double> BDTUnitInterval{0.0, 0.1, 0.2, 0.3, 0.4, 0.5,
       0.6, 0.7, 0.8, 0.9, 1.0};
   std::vector<double> BDTSignedUnit{ -1.0, -0.8, -0.6, -0.4, -0.2, 0.0,
       0.2, 0.4, 0.6, 0.8, 1.0 };
   std::vector<double> BDTPhi{-0.75, -0.5, -0.25, 0.0, 0.25, 0.5, 0.75};
   std::vector<double> BDTCenterCoord{-1.5, -1.25, -1.0, -0.75, -0.5, -0.25,
       0.0, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 2.25, 2.5};
   std::vector<double> BDTRadiusLike{0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5,
       1.75, 2.0, 2.25, 2.5, 2.75, 3.0};
   std::vector<double> BDTRmsCoord{0.0, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6,
       0.7, 0.8, 0.9, 1.0, 1.2};
   std::vector<double> BDTCountEdges{0, 5, 10, 20, 30, 40, 50, 70, 90, 110,
       130};
   std::vector<double> BDTEntropy{0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0,
       4.5, 5.0};
   std::vector<double> BDTEffectiveN{0, 2, 4, 6, 8, 10, 12, 15, 20, 30, 40,
       55, 70};
   std::vector<double> BDTTimeMean{-50, 0, 25, 40, 50, 60, 75, 100, 150, 200,
       250, 300};
   std::vector<double> BDTTimeSpread{0, 5, 7.5, 10, 15, 20, 30, 40, 60, 80,
       100, 150, 250, 400, 600, 1000, 2000};
   std::vector<double> BDTQij{0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0,
       4.5, 5.0, 6.0};
   std::vector<double> BDTPromptPESum{0, 25, 50, 100, 200, 400, 800, 1200,
       1600, 2000, 3000, 4500, 6000};
   std::vector<double> BDTQChargeSum{0.0, 0.001, 0.005, 0.01, 0.05, 0.1, 0.2,
       0.5, 1.0, 2.0, 4.0, 6.0};
   std::vector<double> BDTQChargeMean{0.0, 0.001, 0.002, 0.005, 0.01, 0.015,
       0.02, 0.03, 0.04, 0.06};
   std::vector<double> BDTQChargeSpread{0.0, 0.001, 0.002, 0.005, 0.01, 0.015,
       0.02, 0.03, 0.04, 0.06};
   std::vector<double> BDTPEChargeSum{0, 25, 50, 100, 200, 400, 800, 1200,
       1600, 2000, 3000, 4500, 6000};
   std::vector<double> BDTPEChargeMean{0.0, 0.5, 1.0, 2.0, 4.0, 8.0, 12.0,
       15.0, 20.0, 25.0, 35.0};
   std::vector<double> BDTPEChargeSpread{0.0, 0.5, 1.0, 2.0, 4.0, 8.0, 12.0,
       15.0, 25.0, 40.0, 50.0, 60.0};

       // the title "title; unit" is used in plot in root style
  title ="p_{mu};GeV/c";
  // the tex title "tex title; units" is used in latex format
  textitle = "p_{mu};GeV/c";
  // selection
    double zShift_center = 168.1;
  selection_true = "mc_is_cc0pi_signal";
  selection_reco = "sel_CC0pi_wc  && tank_bdt_no_pion_score > 0.8";
  const std::string selection_Sidebands = "sel_CC0pi_wc";
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

      const std::string SELECTION_CC0PI_LOW_BDT =
  "sel_CC0pi_wc"
  " && tank_bdt_no_pion_score < 0.8";

  selection_profiles["nominal"] = selection_Sidebands;
  selection_profiles["selection_reco"] = selection_Sidebands;
  selection_profiles["bdt_nominal"] = selection_Sidebands;
  selection_profiles["pion_control"] = SELECTION_Pion;
  selection_profiles["cc0pi_low_bdt"] = SELECTION_CC0PI_LOW_BDT;
  

  
  const std::string branchexpr_reco_costheta = "simpleRecoCosTheta;";

  add_sideband_1d("phase_space_pmu_no_pmu", branchexpr_reco_pmu,
      "p_{mu};GeV/c", "p_{mu};GeV/c", PmuBinnEdges_wc_extra, SELECTION_noPmu,
      {"phase_space"});
  add_sideband_1d("phase_space_pmu_no_pmu_no_costheta", branchexpr_reco_pmu,
      "p_{mu};GeV/c", "p_{mu};GeV/c", PmuBinnEdges_wc_extra,
      SELECTION_noPmuandnocostheta, {"phase_space"});
  add_sideband_1d("phase_space_costheta_nominal", branchexpr_reco_costheta,
      "cos#theta_{#mu};", "cos#theta_{#mu};", costheta, selection_Sidebands,
      {"phase_space"});
  add_sideband_1d("phase_space_costheta_no_cut", branchexpr_reco_costheta,
      "cos#theta_{#mu};", "cos#theta_{#mu};", costheta, SELECTION_noCostheta,
      {"phase_space"});
  add_sideband_1d("tagged_fmv_pmu", branchexpr_reco_pmu, "p_{mu};GeV/c",
      "p_{mu};GeV/c", PmuBinnEdges_wc, SELECTION_Taged_FMV, {"fmv"});

  add_sideband_1d("bdt_input_track_to_mrd", "lenghtvertextoStartMRD_cm;",
      "tracklenght;[cm]", "tracklenght;[cm]",
      MRDtracklenght_vertex_toMRD, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_track_in_mrd", "tracklenghtMRD_cm;",
      "tracklenght;[cm]", "tracklenght;[cm]", MRDtracklenght, selection_Sidebands,
      {"bdt_inputs"});
  add_sideband_1d("bdt_input_num_mrd_tracks", "numMRDTracks;", "Ntrks;[cm]",
      "Ntrks;[cm]", NMRDtracks, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_vertex_x_no_fv", "vertexX;cm", "Vertex X;cm",
      "Vertex X;cm", VertexX, SELECTION_noFV, {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_vertex_x", "vertexX;cm", "Vertex X;cm",
      "Vertex X;cm", VertexX, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_vertex_y_no_fv", "vertexY;cm", "Vertex Y;cm",
      "Vertex Y;cm", VertexY, SELECTION_noFV, {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_vertex_y", "vertexY;cm", "Vertex Y;cm",
      "Vertex Y;cm", VertexY, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_vertex_r_no_fv", "vertexR;cm", "Vertex R;cm",
      "Vertex R;cm", VertexR, SELECTION_noFV, {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_vertex_r", "vertexR;cm", "Vertex R;cm",
      "Vertex R;cm", VertexR, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_vertex_rr_no_fv", "vertexRR;cm",
      "Vertex R^{2};cm", "Vertex R^{2};cm", VertexRR, SELECTION_noFV,
      {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_vertex_rr", "vertexRR;cm", "Vertex R^{2};cm",
      "Vertex R^{2};cm", VertexRR, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_vertex_z_no_fv", "vertexZ;cm", "Vertex Z;cm",
      "Vertex Z;cm", VertexZ, SELECTION_noFV, {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_vertex_z", "vertexZ;cm", "Vertex Z;cm",
      "Vertex Z;cm", VertexZ, selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_prompt_pe_no_cut", "promptMuonTotalPE;",
      "Prompt #mu PE;", "Prompt #mu PE;", PromptPE, SELECTION_noPrompt,
      {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_prompt_pe", "promptMuonTotalPE;",
      "Prompt #mu PE;", "Prompt #mu PE;", PromptPE, selection_Sidebands,
      {"bdt_inputs"});
  add_sideband_1d("bdt_input_qij_no_cut", "Qij;", "Q_{ij};", "Q_{ij};", Qij,
      SELECTION_noQij, {"bdt_inputs_relaxed"});
  add_sideband_1d("bdt_input_qij", "Qij;", "Q_{ij};", "Q_{ij};", Qij,
      selection_Sidebands, {"bdt_inputs"});
  add_sideband_1d("bdt_input_tank_prompt_pe", "tank_bdt_input_promptMuonTotalPE;",
      "BDT prompt #mu PE;", "BDT prompt #mu PE;", BDTPromptPESum, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_raw"});
  add_sideband_1d("bdt_input_tank_qij", "tank_bdt_input_Qij;",
      "BDT Q_{ij};", "BDT Q_{ij};", BDTQij, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_raw"});
  add_sideband_1d("bdt_input_q_asym_ud", "tank_bdt_input_q_asym_ud;",
      "BDT q asym UD;", "BDT q asym UD;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_asym_ud", "tank_bdt_input_pe_asym_ud;",
      "BDT pe asym UD;", "BDT pe asym UD;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_asym_tb", "tank_bdt_input_q_asym_tb;",
      "BDT q asym TB;", "BDT q asym TB;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_asym_tb", "tank_bdt_input_pe_asym_tb;",
      "BDT pe asym TB;", "BDT pe asym TB;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_r_mean_tank", "tank_bdt_input_q_r_mean;",
      "BDT q r mean;", "BDT q r mean;", BDTRadiusLike, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_r_mean_tank", "tank_bdt_input_pe_r_mean;",
      "BDT pe r mean;", "BDT pe r mean;", BDTRadiusLike, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_mean", "tank_bdt_input_q_phi_mean;",
      "BDT q #phi mean;", "BDT q #phi mean;", BDTPhi, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_mean", "tank_bdt_input_pe_phi_mean;",
      "BDT pe #phi mean;", "BDT pe #phi mean;", BDTPhi, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_c1", "tank_bdt_input_q_phi_c1;",
      "BDT q #phi c1;", "BDT q #phi c1;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_s1", "tank_bdt_input_q_phi_s1;",
      "BDT q #phi s1;", "BDT q #phi s1;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_a1", "tank_bdt_input_q_phi_a1;",
      "BDT q #phi a1;", "BDT q #phi a1;", BDTUnitInterval, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_c2", "tank_bdt_input_q_phi_c2;",
      "BDT q #phi c2;", "BDT q #phi c2;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_s2", "tank_bdt_input_q_phi_s2;",
      "BDT q #phi s2;", "BDT q #phi s2;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_phi_a2", "tank_bdt_input_q_phi_a2;",
      "BDT q #phi a2;", "BDT q #phi a2;", BDTUnitInterval, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_c1", "tank_bdt_input_pe_phi_c1;",
      "BDT pe #phi c1;", "BDT pe #phi c1;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_s1", "tank_bdt_input_pe_phi_s1;",
      "BDT pe #phi s1;", "BDT pe #phi s1;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_a1", "tank_bdt_input_pe_phi_a1;",
      "BDT pe #phi a1;", "BDT pe #phi a1;", BDTUnitInterval, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_c2", "tank_bdt_input_pe_phi_c2;",
      "BDT pe #phi c2;", "BDT pe #phi c2;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_s2", "tank_bdt_input_pe_phi_s2;",
      "BDT pe #phi s2;", "BDT pe #phi s2;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pe_phi_a2", "tank_bdt_input_pe_phi_a2;",
      "BDT pe #phi a2;", "BDT pe #phi a2;", BDTUnitInterval, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_n_unique_pmts_hit",
      "tank_bdt_input_n_unique_pmts_hit;", "BDT unique PMTs hit;",
      "BDT unique PMTs hit;", BDTCountEdges, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_max_pe_fraction",
      "tank_bdt_input_max_pe_fraction;", "BDT max pe fraction;",
      "BDT max pe fraction;", BDTUnitInterval, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_top5_pe_fraction",
      "tank_bdt_input_top5_pe_fraction;", "BDT top5 pe fraction;",
      "BDT top5 pe fraction;", BDTUnitInterval, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pmt_pe_entropy",
      "tank_bdt_input_pmt_pe_entropy;", "BDT PMT pe entropy;",
      "BDT PMT pe entropy;", BDTEntropy, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_pmt_pe_effective_n",
      "tank_bdt_input_pmt_pe_effective_n;", "BDT PMT pe effective N;",
      "BDT PMT pe effective N;", BDTEffectiveN, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_pmt"});
  add_sideband_1d("bdt_input_q_center_x", "tank_bdt_input_q_center_x;",
      "BDT q center x;cm", "BDT q center x;cm", BDTCenterCoord,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_q_center_y", "tank_bdt_input_q_center_y;",
      "BDT q center y;cm", "BDT q center y;cm", BDTCenterCoord,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_q_center_z", "tank_bdt_input_q_center_z;",
      "BDT q center z;cm", "BDT q center z;cm", BDTCenterCoord,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_q_rms_x", "tank_bdt_input_q_rms_x;",
      "BDT q rms x;cm", "BDT q rms x;cm", BDTRmsCoord, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_q_rms_y", "tank_bdt_input_q_rms_y;",
      "BDT q rms y;cm", "BDT q rms y;cm", BDTRmsCoord, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_q_rms_z", "tank_bdt_input_q_rms_z;",
      "BDT q rms z;cm", "BDT q rms z;cm", BDTRmsCoord, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_pe_center_x", "tank_bdt_input_pe_center_x;",
      "BDT pe center x;cm", "BDT pe center x;cm", BDTCenterCoord,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_pe_center_y", "tank_bdt_input_pe_center_y;",
      "BDT pe center y;cm", "BDT pe center y;cm", BDTCenterCoord,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_pe_center_z", "tank_bdt_input_pe_center_z;",
      "BDT pe center z;cm", "BDT pe center z;cm", BDTCenterCoord,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_pe_rms_x", "tank_bdt_input_pe_rms_x;",
      "BDT pe rms x;cm", "BDT pe rms x;cm", BDTRmsCoord, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_pe_rms_y", "tank_bdt_input_pe_rms_y;",
      "BDT pe rms y;cm", "BDT pe rms y;cm", BDTRmsCoord, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_pe_rms_z", "tank_bdt_input_pe_rms_z;",
      "BDT pe rms z;cm", "BDT pe rms z;cm", BDTRmsCoord, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_geom"});
  add_sideband_1d("bdt_input_hit_t_mean", "tank_bdt_input_hit_t_mean;",
      "BDT hit t mean;", "BDT hit t mean;", BDTTimeMean, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_t_stddev", "tank_bdt_input_hit_t_stddev;",
      "BDT hit t stddev;", "BDT hit t stddev;", BDTTimeSpread, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_q_sum", "tank_bdt_input_hit_q_sum;",
      "BDT hit q sum;", "BDT hit q sum;", BDTQChargeSum, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_q_mean", "tank_bdt_input_hit_q_mean;",
      "BDT hit q mean;", "BDT hit q mean;", BDTQChargeMean, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_q_stddev", "tank_bdt_input_hit_q_stddev;",
      "BDT hit q stddev;", "BDT hit q stddev;", BDTQChargeSpread,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_pe_sum", "tank_bdt_input_hit_pe_sum;",
      "BDT hit pe sum;", "BDT hit pe sum;", BDTPEChargeSum, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_pe_mean", "tank_bdt_input_hit_pe_mean;",
      "BDT hit pe mean;", "BDT hit pe mean;", BDTPEChargeMean, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_pe_stddev", "tank_bdt_input_hit_pe_stddev;",
      "BDT hit pe stddev;", "BDT hit pe stddev;", BDTPEChargeSpread,
      selection_Sidebands, {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_t_min", "tank_bdt_input_hit_t_min;",
      "BDT hit t min;", "BDT hit t min;", BDTTimeMean, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_t_max", "tank_bdt_input_hit_t_max;",
      "BDT hit t max;", "BDT hit t max;", BDTTimeMean, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_hit_t_range", "tank_bdt_input_hit_t_range;",
      "BDT hit t range;", "BDT hit t range;", BDTTimeSpread, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_timing_corr_r",
      "tank_bdt_input_timing_corr_r;", "BDT timing corr r;",
      "BDT timing corr r;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});
  add_sideband_1d("bdt_input_timing_corr_z",
      "tank_bdt_input_timing_corr_z;", "BDT timing corr z;",
      "BDT timing corr z;", BDTSignedUnit, selection_Sidebands,
      {"bdt_inputs", "bdt_inputs_hits"});

  add_sideband_1d("throughgoing_pmu", branchexpr_reco_pmu, "p_{mu};GeV/c",
      "p_{mu};GeV/c", PmuBinnEdges_wc_extra, SELECTION_thoughtgoing,
      {"throughgoing"});
  add_sideband_1d("throughgoing_track_to_mrd", "lenghtvertextoStartMRD_cm;",
      "tracklenght;[cm]", "tracklenght;[cm]",
      MRDtracklenght_vertex_toMRD, SELECTION_thoughtgoing, {"throughgoing"});
  add_sideband_1d("throughgoing_track_in_mrd", "tracklenghtMRD_cm;",
      "tracklenght;[cm]", "tracklenght;[cm]", MRDtracklenght,
      SELECTION_thoughtgoing, {"throughgoing"});
  add_sideband_1d("throughgoing_prompt_pe", "promptMuonTotalPE;",
      "Prompt #mu PE;", "Prompt #mu PE;", PromptPE, SELECTION_thoughtgoing,
      {"throughgoing"});
  add_sideband_1d("throughgoing_qij", "Qij;", "Q_{ij};", "Q_{ij};", Qij,
      SELECTION_thoughtgoing, {"throughgoing"});
  add_sideband_1d("throughgoing_vertex_x", "vertexX;cm", "Vertex X;cm",
      "Vertex X;cm", VertexX, SELECTION_thoughtgoing, {"throughgoing"});
  add_sideband_1d("throughgoing_vertex_y", "vertexY;cm", "Vertex Y;cm",
      "Vertex Y;cm", VertexY, SELECTION_thoughtgoing, {"throughgoing"});
  add_sideband_1d("throughgoing_vertex_r", "vertexR;cm", "Vertex R;cm",
      "Vertex R;cm", VertexR, SELECTION_thoughtgoing, {"throughgoing"});
  add_sideband_1d("throughgoing_vertex_rr", "vertexRR;cm",
      "Vertex R^{2};cm", "Vertex R^{2};cm", VertexRR, SELECTION_thoughtgoing,
      {"throughgoing"});
  add_sideband_1d("throughgoing_vertex_z", "vertexZ;cm", "Vertex Z;cm",
      "Vertex Z;cm", VertexZ, SELECTION_thoughtgoing, {"throughgoing"});

  add_sideband_1d("bdt_no_pion_score", "tank_bdt_no_pion_score;",
      "BDT score;", "BDT score;", BTD_PredictionSideband, selection_Sidebands,
      {"bdt_outputs"});
  add_sideband_1d("bdt_pion_score", "tank_bdt_pion_score;", "BDT pion score;",
      "BDT pion score;", BTD_PredictionSideband, selection_Sidebands,
      {"bdt_outputs"});
  add_sideband_1d("pion_control_pmu", branchexpr_reco_pmu, "p_{mu};GeV/c",
      "p_{mu};GeV/c", PmuBinnEdges_wc, SELECTION_Pion, {"pion_control"});
  
}
