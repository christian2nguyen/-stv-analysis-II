#ifndef __CC1muXp0pi_hh__
#define __CC1muXp0pi_hh__


#include "XSecAnalyzer/Selections/SelectionBase.hh"


// XGBoost
#include <xgboost/c_api.h>

#define DEFAULT_NUM 20

class CC1muXp0pi : public SelectionBase {
 public:
  CC1muXp0pi();
  ~CC1muXp0pi(){
    XGBoosterFree(*booster);
    std::cout << "Destructor: Memory deallocated." << std::endl;
  }
 
  virtual int categorize_event( AnalysisEvent* event ) override final;
  virtual void compute_reco_observables( AnalysisEvent* event ) override final;
  virtual void compute_true_observables( AnalysisEvent* event ) override final;
  virtual void define_category_map() override final;
  virtual void define_constants() override final;
  virtual void define_output_branches() override final;
  virtual bool define_signal( AnalysisEvent* event ) override final;
  virtual void reset() override final;
  virtual bool selection( AnalysisEvent* event ) override final;
  virtual void define_additional_input_branches(TTree& etree) override final;
  
  
  // Implementing pure virtual functions
    int categorize_event(AnalysisANNIE_Event* event) override final {
        // Do nothing
        return 0;  // Return a default integer value
    }

    bool selection(AnalysisANNIE_Event* event) override final {
        // Do nothing
        return false;  // Return a default boolean value
    }

    bool define_signal(AnalysisANNIE_Event* event) override final {
        // Do nothing
        return false;  // Return a default boolean value
    }

    void compute_reco_observables(AnalysisANNIE_Event* event) override final {
        // Do nothing
    }

    void compute_true_observables(AnalysisANNIE_Event* event) override final {
        // Do nothing
    }

    virtual int categorize_event( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual bool selection( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual bool define_signal( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual void compute_reco_observables( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual void compute_true_observables( AnalysisANNIE_Event_nuwro* event ) override final;

private:
  // xgbooster from Christain and Panos
  BoosterHandle* booster;
  void classify_tracks(AnalysisEvent* event);
  // variable for signal definition
  bool sig_in_fv_;
  bool sig_in_fv_loose_;
  bool sig_is_numu_;
  bool is_cc_;

  int sig_mc_num_muon_in_momentum_range_;
  int sig_mc_num_muon_;
  int sig_mc_num_proton_in_momentum_range_;
  int sig_mc_num_proton_;
  int sig_mc_num_charged_pi_above_threshold_;
  int sig_mc_num_charged_pi_;
  bool sig_mc_no_fs_mesons_;
  bool sig_mc_no_fs_pi0_;
  bool sig_mc_no_fs_charged_pi_;
  bool sig_mc_no_fs_charged_pi_above_threshold_;
  bool sig_mc_no_fs_additional_;

  float sig_mc_LeadProtonMomentum;
  bool mc_is_signal_charged_pi_above_threshold_;
  bool mc_is_signal_no_meson_;
  bool mc_is_signal_;

  // variables for event selection
  bool sel_reco_vertex_in_fv_;
  bool sel_pfp_starts_in_PCV_;
  int sel_muon_candidate_idx_;
  std::vector<int> sel_muon_candidate_indices_;
  std::vector<int> sel_muon_pid_scores_;
  MyPointer<std::vector<int>> sel_xsec_fs_pdg_;
  int sel_num_muon_candidates_;
  int sel_num_proton_candidates_;
  int lead_p_candidate_idx_;
  bool sel_has_muon_candidate_;
  bool sel_muon_contained_;
  bool sel_muon_quality_ok_;
  bool sel_muon_passed_mom_cuts_;
  bool sel_no_reco_showers_;
  bool sel_no_reco_meson_;
  bool sel_topo_cut_passed_;
  bool sel_nu_mu_cc_;
  bool sel_ccxp0meson_;

  std::map<double, int> proton_index;
  std::map<double, double> proton_energy, proton_px, proton_py, proton_pz;

  int sel_track_length_size_;
  bool sel_trk_bragg_mu_fwd_preferred_;
  double sel_track_chi2_muon_;
  bool sel_reject_flipped_track_;


// truth variables
        
  MyPointer<TLorentzVector> mc_p4_mu_;
  MyPointer<TLorentzVector> mc_p4p_;
  MyPointer<std::vector<TLorentzVector>> mc_p4_proton_vec_;
  MyPointer<std::vector<TLorentzVector>> mc_p4_mu_vec_;
  
  double mc_hadron_pT_;
  double mc_hadron_delta_phiT_;
  double mc_hadron_delta_alphaT_;
  double mc_hadron_delta_alpha3Dq_;
  double mc_hadron_delta_alpha3DMu_;
  double mc_hadron_delta_phi3D_;
  double mc_hadron_pL_;
  double mc_hadron_pn_;
  double mc_hadron_pTx_;
  double mc_hadron_pTy_;
  double mc_hadron_theta_mu_p_;

  double mc_lead_proton_pT_;
  double mc_lead_proton_delta_phiT_;
  double mc_lead_proton_delta_alphaT_;
  double mc_lead_proton_delta_alpha3Dq_;
  double mc_lead_proton_delta_alpha3DMu_;
  double mc_lead_proton_delta_phi3D_;
  double mc_lead_proton_pL_;
  double mc_lead_proton_pn_;
  double mc_lead_proton_pTx_;
  double mc_lead_proton_pTy_;
  double mc_lead_proton_theta_mu_p_;


// reconstructed variables


  MyPointer<TLorentzVector> reco_p4mu_;
  MyPointer<TLorentzVector> reco_p4p_;
  MyPointer<std::vector<TLorentzVector>> reco_p4_p_vec_;


  double hadron_pT_;
  double hadron_delta_phiT_;
  double hadron_delta_alphaT_;
  double hadron_delta_alpha3Dq_;
  double hadron_delta_alpha3DMu_;
  double hadron_delta_phi3D_;
  double hadron_pL_;
  double hadron_pn_;
  double hadron_pTx_;
  double hadron_pTy_;
  double hadron_theta_mu_p_;

  double lead_proton_pT_;
  double lead_proton_delta_phiT_;
  double lead_proton_delta_alphaT_;
  double lead_proton_delta_alpha3Dq_;
  double lead_proton_delta_alpha3DMu_;
  double lead_proton_delta_phi3D_;
  double lead_proton_pL_;
  double lead_proton_pn_;
  double lead_proton_pTx_;
  double lead_proton_pTy_;
  double lead_proton_theta_mu_p_;








  double muon_energy_;
  double muon_costh_;
  double muon_p_;

  double mc_muon_energy_;
  double mc_muon_costh_;
  double mc_muon_p_;

  double mc_lead_proton_energy_;
  double mc_lead_proton_costh_;
  double mc_lead_proton_p_;

  double mc_hadron_energy_;
  double mc_hadron_costh_;
  double mc_hadron_p_;
  STVCalcType CalcType;
};

#endif
