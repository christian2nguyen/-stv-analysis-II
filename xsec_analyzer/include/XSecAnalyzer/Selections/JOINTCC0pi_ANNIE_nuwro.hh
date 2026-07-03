#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Selections/SelectionBase.hh"

class JOINTCC0pi_ANNIE_nuwro : public SelectionBase {

public:

  struct TankBDTFeatureOutputs {
    float prompt_muon_total_pe = -9999.0f;
    float qij = -9999.0f;
    float q_asym_ud = -9999.0f;
    float pe_asym_ud = -9999.0f;
    float q_asym_tb = -9999.0f;
    float pe_asym_tb = -9999.0f;
    float q_r_mean = -9999.0f;
    float pe_r_mean = -9999.0f;
    float q_phi_mean = -9999.0f;
    float pe_phi_mean = -9999.0f;
    float q_phi_c1 = -9999.0f;
    float q_phi_s1 = -9999.0f;
    float q_phi_a1 = -9999.0f;
    float q_phi_c2 = -9999.0f;
    float q_phi_s2 = -9999.0f;
    float q_phi_a2 = -9999.0f;
    float pe_phi_c1 = -9999.0f;
    float pe_phi_s1 = -9999.0f;
    float pe_phi_a1 = -9999.0f;
    float pe_phi_c2 = -9999.0f;
    float pe_phi_s2 = -9999.0f;
    float pe_phi_a2 = -9999.0f;
    float n_unique_pmts_hit = -9999.0f;
    float max_pe_fraction = -9999.0f;
    float top5_pe_fraction = -9999.0f;
    float pmt_pe_entropy = -9999.0f;
    float pmt_pe_effective_n = -9999.0f;
    float q_center_x = -9999.0f;
    float q_center_y = -9999.0f;
    float q_center_z = -9999.0f;
    float q_rms_x = -9999.0f;
    float q_rms_y = -9999.0f;
    float q_rms_z = -9999.0f;
    float pe_center_x = -9999.0f;
    float pe_center_y = -9999.0f;
    float pe_center_z = -9999.0f;
    float pe_rms_x = -9999.0f;
    float pe_rms_y = -9999.0f;
    float pe_rms_z = -9999.0f;
    float hit_t_mean = -9999.0f;
    float hit_t_stddev = -9999.0f;
    float hit_q_sum = -9999.0f;
    float hit_q_mean = -9999.0f;
    float hit_q_stddev = -9999.0f;
    float hit_pe_sum = -9999.0f;
    float hit_pe_mean = -9999.0f;
    float hit_pe_stddev = -9999.0f;
    float hit_t_min = -9999.0f;
    float hit_t_max = -9999.0f;
    float hit_t_range = -9999.0f;
    float timing_corr_r = -9999.0f;
    float timing_corr_z = -9999.0f;
  };

  JOINTCC0pi_ANNIE_nuwro();

  ~JOINTCC0pi_ANNIE_nuwro() {
    std::cout << "Destructor: Memory deallocated." << std::endl;
} 

  virtual int categorize_event( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual bool selection( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual bool define_signal( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual void compute_reco_observables( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual void compute_true_observables( AnalysisANNIE_Event_nuwro* event ) override final;


  virtual int categorize_event( AnalysisANNIE_Event* event ) override final;
  virtual bool selection( AnalysisANNIE_Event* event ) override final;
  virtual bool define_signal( AnalysisANNIE_Event* event ) override final;
  virtual void compute_reco_observables( AnalysisANNIE_Event* event ) override final;
  virtual void compute_true_observables( AnalysisANNIE_Event* event ) override final;
  
  //// for now I Think I have hae these in here
  virtual int categorize_event( AnalysisEvent* event ) override final;
  virtual bool selection( AnalysisEvent* event ) override final;
  virtual bool define_signal( AnalysisEvent* event ) override final;
  virtual void compute_reco_observables( AnalysisEvent* event ) override final;
  virtual void compute_true_observables( AnalysisEvent* event ) override final;

  
  virtual void define_output_branches() override final;
  virtual void define_constants() override final;
  virtual void define_category_map() override final;
  virtual void reset() override final;
  virtual void define_additional_input_branches(TTree& etree) override final;
 
      double muon_m = 105.66;
 
      bool mc_is_cc0pi_signal_;
      bool sel_CC0pi_wc_;
      bool sel_nu_mu_cc_;
      
      bool mc_vertex_in_FV_;
      bool mc_muon_in_mom_range_;
      bool mc_neutrino_is_numu_;
      bool mc_no_charged_pi_above_threshold_;
      bool mc_no_fs_mesons_;
      bool mc_no_fs_pi0_;
      bool mc_muon_costheta_threshold_;
      bool mc_true_no_followers_;
      

      bool sel_muon_passed_mom_cuts_;
      bool sel_has_pion_candidate_;
      bool sel_costheta_threshold_;
      bool sel_contained_in_MRD_;
      bool sel_vertex_in_FV_;
      bool recoFV_;
      
      bool reco_contained_in_MRD_;
      
      double simpleRecoMomentumCor_;
      double simpleRecoMomentum_;
      
      int category_;
      int mc_num_protons_;
      int mc_num_neutrons_;
      int mc_num_charged_pions_;
      int mc_num_wc_charged_pions_;
      int mc_num_neutral_pions_;
      int mc_num_kaons_;
      int mc_num_wc_kaons_;
      
      double single_ring_score_;
      double multi_ring_score_;
      
      double mc_pmu_gev_;
      double pmu_gev_;
      double vertexR_;
      double vertexRR_;
      
      double vertexX_;
      double vertexY_;
      double vertexZ_;
      double tracklenghtMRD_cm_; 
      double lenghtvertextoStartMRD_cm_; 

      //double Qij_;
      bool sel_has_no_pion_candidate_Qij_; 
      bool sel_trigword_; 
      bool sel_HasTank_; 
      
      bool sel_HasMRD_;
      bool sel_TankMRDCoinc_;
      bool sel_noVeto_;
      bool sel_simpleRecoFlag_;
      bool sel_promptMuonTotalPE_;
      bool sel_Atleast1track_MRDSTOP_;
      bool mc_interaction_withO16_;
      
      //double trueAngle_
      double trueCosTheta_;
      //std::vector<double> weight_TunedCentralValue_UBGenie_;
      double tank_bdt_no_pion_score_ = -9999.0;
      double tank_bdt_pion_score_ = -9999.0;
      TankBDTFeatureOutputs tank_bdt_feature_outputs_;
      
       //MyPointer< std::vector<double> > weight_TunedCentralValue_UBGenie_;
      
      private:
};

