// XSecAnalyzer includes
#include "XSecAnalyzer/TreeUtils.hh"
#include "XSecAnalyzer/Functions.hh"

#include "XSecAnalyzer/Selections/CC1muXp0pi.hh"
#include "XSecAnalyzer/Selections/EventCategoriesCC1muXp0piFSI.hh"

CC1muXp0pi::CC1muXp0pi() : SelectionBase( "CC1muXp0pi" ) {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;
  CalcType = kOpt4;
  booster = new BoosterHandle;
  XGBoosterCreate(NULL, 0, booster);
  assert(XGBoosterLoadModel(*booster, "/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/lib/cc0pi_fstrack_pid_softprob.json") == 0);
  std::cout<<"Got XGBoosterCreate  "<<std::endl;
  std::cout<<"Finished with Constuctor  "<<std::endl;

  // xgb_pid_vec_.reset( new std::vector<int>() );
  this->define_category_map();

}


void CC1muXp0pi::define_constants() {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;
  this->define_true_FV( 21.5, 234.85, -95.0, 95.0, 21.5, 966.8 );
  this->define_reco_FV( 21.5, 234.85, -95.0, 95.0, 21.5, 966.8 );
}


////////////////////////////////////////////////////////////////
//   define signal
////////////////////////////////////////////////////////////////
bool CC1muXp0pi::define_signal(AnalysisEvent* event){
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;

  // there are five criteria:
  // 1. A muon neutrino undergoes a CC interaction with an argon nucleus. Interaction 
  //    vertex should be within FV.
  // 2. There are no mesons in final states.
  // 3. The momentum of the outgoing muon lies within the interval [0.1, 1.2] GeV/c.
  //
  // vertex inside FV
  // with tight requirement
  this->define_true_FV( 21.5, 234.85, -95.0, 95.0, 21.5, 966.8 );
  this->define_reco_FV( 21.5, 234.85, -95.0, 95.0, 21.5, 966.8 );
  sig_in_fv_ = point_inside_FV(this->true_FV(), event->mc_nu_vx_, event->mc_nu_vy_, event->mc_nu_vz_);
  // with a loose requirement
  this->define_true_FV(10.,246.,-105.,105.,10.,1026.);
  this->define_reco_FV(10.,246.,-105.,105.,10.,1026.);
  sig_in_fv_loose_ = point_inside_FV(this->true_FV(), event->mc_nu_vx_, event->mc_nu_vy_, event->mc_nu_vz_);


  // initial state is a muon neutrion
  // CC or NC channel
  sig_is_numu_ = (event->mc_nu_pdg_ == MUON_NEUTRINO);
  is_cc_ = (event->mc_nu_ccnc_ == CHARGED_CURRENT);

  // only one muon in momnetum range
  sig_mc_num_muon_in_momentum_range_ = 0;
  sig_mc_num_muon_ = 0;
  // count how many protons in momentum range
  // 0p channel has no proton in momentum range
  sig_mc_num_proton_in_momentum_range_ = 0;
  sig_mc_num_proton_ = 0;

  sig_mc_num_charged_pi_ = 0;
  sig_mc_num_charged_pi_above_threshold_ = 0;

  // there are some different definition of meson
  // 1. no any other type of fs particle except proton and neutron
  // 2. no charged pion and neutral pion
  // 3. no charged pion above the momentum threshold
  sig_mc_no_fs_mesons_ = true;
  sig_mc_no_fs_pi0_ = true;
  sig_mc_no_fs_charged_pi_ = true;
  sig_mc_no_fs_additional_ = true;
  sig_mc_no_fs_charged_pi_above_threshold_ = true;
  TLorentzVector p4;
  sig_mc_LeadProtonMomentum = -1;
  for ( size_t p = 0u; p < event->mc_nu_daughter_pdg_->size(); ++p ) {
    int pdg = event->mc_nu_daughter_pdg_->at( p );
    float energy = event->mc_nu_daughter_energy_->at( p );
    float px = event->mc_nu_daughter_px_->at( p );
    float py = event->mc_nu_daughter_py_->at( p );
    float pz = event->mc_nu_daughter_pz_->at( p );
    p4.SetPxPyPzE(px, py, pz, energy);

    // Do the general check for (anti)mesons first before considering
    // any individual PDG codes
    if ( is_meson_or_antimeson(pdg) ) {
      sig_mc_no_fs_mesons_ = false;
    }

    if (!(pdg == PROTON || pdg == NEUTRON || pdg == MUON )){
      sig_mc_no_fs_additional_ = false;
    }

    if(pdg == MUON){
      sig_mc_num_muon_++;
      if(p4.Rho() >= MUON_P_MIN_MOM_CUT && p4.Rho() <= MUON_P_MAX_MOM_CUT){
        sig_mc_num_muon_in_momentum_range_++;
      }
    }

    else if(pdg == PROTON){
      sig_mc_num_proton_++;
      if(p4.Rho() >= LEAD_P_MIN_MOM_CUT && p4.Rho() <= LEAD_P_MAX_MOM_CUT){
        if(p4.Rho() > sig_mc_LeadProtonMomentum) sig_mc_LeadProtonMomentum = p4.Rho();
        sig_mc_num_proton_in_momentum_range_++;
      }
    }
    else if (pdg == PI_ZERO){
      sig_mc_no_fs_pi0_ = false;
    }
    else if (std::abs(pdg) == PI_PLUS){
      sig_mc_num_charged_pi_++;
      if(p4.Rho() > CHARGED_PI_MOM_CUT){
        sig_mc_no_fs_charged_pi_above_threshold_ = false;
        sig_mc_num_charged_pi_above_threshold_++;
      }
    }
  }

  mc_is_signal_charged_pi_above_threshold_ =  sig_in_fv_ && sig_is_numu_ && is_cc_ && sig_mc_num_muon_in_momentum_range_ && sig_mc_no_fs_charged_pi_above_threshold_;
  mc_is_signal_no_meson_ = sig_in_fv_ && sig_is_numu_ && is_cc_ && sig_mc_num_muon_in_momentum_range_ == 1 && sig_mc_no_fs_mesons_;
  mc_is_signal_ = sig_in_fv_ && sig_is_numu_ && is_cc_ && sig_mc_num_muon_in_momentum_range_ == 1 && sig_mc_no_fs_additional_;

  return mc_is_signal_;
}

////////////////////////////////////////////////////////////////
//   selection criteria
////////////////////////////////////////////////////////////////

bool CC1muXp0pi::selection( AnalysisEvent* event ) {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;

  FiducialVolume PCV;
  PCV.X_Min = 10.;
  PCV.X_Max = 246.35;
  PCV.Y_Min = -106.5;
  PCV.Y_Max = 106.5;
  PCV.Z_Min = 10.;
  PCV.Z_Max = 1026.8;

  sel_reco_vertex_in_fv_ = point_inside_FV( this->reco_FV(),
      event->nu_vx_, event->nu_vy_, event->nu_vz_ );

  // not be used in selection; set different topo cut for different channels
  // sel_topo_cut_passed_ = event->topological_score_ > TOPO_SCORE_CUT;
  // not be used in selection;
  // sel_cosmic_ip_cut_passed_ = event->cosmic_impact_parameter_ > COSMIC_IP_CUT;

  // Apply the containment cut to the starting positions of all
  // reconstructed tracks and showers. Pass this cut by default.
  sel_pfp_starts_in_PCV_ = true;

  // Loop over each PFParticle in the event
  for ( int p = 0; p < event->num_pf_particles_; ++p ) {

    // Only check direct neutrino daughters (generation == 2)
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2u ) continue;

    // Use the track reconstruction results to get the start point for
    // every PFParticle for the purpose of verifying containment. We could
    // in principle differentiate between tracks and showers here, but
    // (1) we cut out all showers later on in the selection anyway, and
    // (2) the blinded PeLEE data ntuples do not include shower information.
    // We therefore apply the track reconstruction here unconditionally.
    float x = event->track_startx_->at( p );
    float y = event->track_starty_->at( p );
    float z = event->track_startz_->at( p );

    // Verify that the start of the PFParticle lies within the containment
    // volume.
    // TODO: revisit which containment volume to use for PFParticle start
    // positions. See https://stackoverflow.com/a/2488507 for an explanation
    // of the use of &= here. Don't worry, it's type-safe since both operands
    // are bool.
    sel_pfp_starts_in_PCV_ &= point_inside_FV(PCV, x, y, z );
  }

  // Sets the sel_has_muon_candidate_ flag as appropriate. The threshold check
  // is handled later.

  for ( int p = 0; p < event->num_pf_particles_; ++p ) {
    // Only direct neutrino daughters (generation == 2) will be considered as
    // possible muon candidates
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2u ) continue;

    float track_score = event->pfp_track_score_->at( p );
    float start_dist = event->track_start_distance_->at( p );
    float track_length = event->track_length_->at( p );
    float pid_score = event->track_llr_pid_score_->at( p );

    if ( track_score > MUON_TRACK_SCORE_CUT
        && start_dist < MUON_VTX_DISTANCE_CUT
        && track_length > MUON_LENGTH_CUT
        && pid_score > MUON_PID_CUT )
    {
      sel_muon_candidate_indices_.push_back( p );
      sel_muon_pid_scores_.push_back( pid_score );
    }
  }

  sel_has_muon_candidate_ = false;
  sel_muon_contained_ = false;
  sel_muon_quality_ok_ = false;
  sel_muon_passed_mom_cuts_ = false;
  sel_no_reco_showers_ = true;
  sel_no_reco_meson_ = true;
  sel_topo_cut_passed_ = false;
  sel_num_proton_candidates_ = -1;
  sel_reject_flipped_track_ = false;
  size_t num_candidates = sel_muon_candidate_indices_.size();

  sel_num_muon_candidates_ = num_candidates;
  if ( num_candidates > 0u ) sel_has_muon_candidate_ = true;

  if ( num_candidates == 1u ) {
    sel_muon_candidate_idx_ = sel_muon_candidate_indices_.front();
  }
  else if ( num_candidates > 1u ) {
    // In the case of multiple muon candidates, choose the one with the highest
    // PID score (most muon-like) as the one to use
    float highest_score = LOW_FLOAT;
    int chosen_index = BOGUS_INDEX;
    for ( size_t c = 0; c < num_candidates; ++c ) {
      float score = sel_muon_pid_scores_.at( c );
      if ( highest_score < score ) {
        highest_score = score;
        chosen_index = sel_muon_candidate_indices_.at( c );
      }
    }
    sel_muon_candidate_idx_ = chosen_index;
  }
  else {
    sel_muon_candidate_idx_ = BOGUS_INDEX;
  }

  // finish the standard muon selection
  // sel_nu_mu_cc_ = sel_reco_vertex_in_FV_ && sel_pfp_starts_in_PCV_
  //   && sel_has_muon_candidate_ && sel_topo_cut_passed_;

  // 

  for ( int p = 0; p < event->num_pf_particles_; ++p ) {
    // Only worry about direct neutrino daughters (PFParticles considered
    // aughters of the reconstructed neutrino)
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2u ) continue;
    // check the quality of muon
    // 1. muon canidate is contained.
    // 2. the different between range momentum and contained momentum
    if ( p == sel_muon_candidate_idx_ ) {
      float endx = event->track_endx_->at( p );
      float endy = event->track_endy_->at( p );
      float endz = event->track_endz_->at( p );
      bool end_contained = point_inside_FV(PCV, endx, endy, endz );
      if ( end_contained ) sel_muon_contained_ = true;
      float muon_mom = LOW_FLOAT;
      float range_muon_mom = event->track_range_mom_mu_->at( p );
      float mcs_muon_mom = event->track_mcs_mom_mu_->at( p );
      double frac_diff_range_mcs = std::abs( range_muon_mom - mcs_muon_mom );
      if ( range_muon_mom > 0. ) {
        frac_diff_range_mcs /= range_muon_mom;
        if ( frac_diff_range_mcs < MUON_MOM_QUALITY_CUT ) {
          sel_muon_quality_ok_ = true;
        }
      }
      if ( sel_muon_contained_ ) muon_mom = range_muon_mom;
      else muon_mom = mcs_muon_mom;

      if ( muon_mom >= MUON_P_MIN_MOM_CUT && muon_mom <= MUON_P_MAX_MOM_CUT ) {
        sel_muon_passed_mom_cuts_ = true;
      }
      sel_track_length_size_ = event->track_length_->size();
      sel_trk_bragg_mu_fwd_preferred_ = event->trk_bragg_mu_fwd_preferred_v_->at(p);
      sel_track_chi2_muon_ = event->track_chi2_muon_->at(p);
      if(sel_track_length_size_ == 1 && sel_trk_bragg_mu_fwd_preferred_ == 0 && sel_track_chi2_muon_ > 6.0){
        sel_reject_flipped_track_ = true;
      }

    }
    else{
      // instead of select a lead proton candidate, 
      // we remove the shower and non-proton tracks
      // if we find any shower or non-proton tracks 
      // in an event, than we kill this event.a

      // if the events have a shower, than we set sel_has_non_proton_particles_ to be true
      float track_score = event->pfp_track_score_->at( p );
      float llr_pid_score = event->track_llr_pid_score_->at( p );
      float track_length = event->track_length_->at( p );
      if ( track_score <= TRACK_SCORE_CUT ){
        sel_no_reco_showers_ = false;
      }

      // Check whether the current proton candidate fails the proton PID cut
      if ( llr_pid_score > proton_pid_cut(track_length) ) {
        sel_no_reco_meson_ = false;
      }     

      // Check whether the current proton candidate fails the containment cut
      float endx = event->track_endx_->at( p );
      float endy = event->track_endy_->at( p );
      float endz = event->track_endz_->at( p );
      bool end_contained = point_inside_FV(PCV, endx, endy, endz );

      float p_dirx = event->track_dirx_->at( p );
      float p_diry = event->track_diry_->at( p );
      float p_dirz = event->track_dirz_->at( p );
      float KEp = event->track_kinetic_energy_p_->at( p );
      float p_mom = real_sqrt( KEp*KEp + 2.*PROTON_MASS*KEp );
      TVector3 v3p(p_dirx, p_diry, p_dirz);
      v3p = v3p.Unit() * p_mom;
      if( track_length > 0. && end_contained ){
        if(p_mom > 0.25 && p_mom < 1.0){
          proton_index.insert(std::pair<double, int>(track_length, p));
          proton_energy.insert(std::pair<double, double>(track_length, KEp + PROTON_MASS));
          proton_px.insert(std::pair<double, double>(track_length, v3p.X()));
          proton_py.insert(std::pair<double, double>(track_length, v3p.Y()));
          proton_pz.insert(std::pair<double, double>(track_length, v3p.Z()));
        }
      }
    }
  }

  size_t num_proton_candidates = proton_index.size();
  sel_num_proton_candidates_ = num_proton_candidates;

  // If there are protons in the final state, the one with longest 
  // track length is set to be the leading proton track.

  lead_p_candidate_idx_ = BOGUS_INDEX;
  if(num_proton_candidates > 0u){
    lead_p_candidate_idx_ = proton_index.cbegin()->second;
  }

  // We set different requirement of topological cut according to the 
  // proton multiplicity. Cosmic rays are likely to survive the previous 
  // event selection as 0p (or 1p) events.

  if(num_proton_candidates == 0u){
    sel_topo_cut_passed_ = (event->topological_score_ > TOPO_SCORE_CUT * 2) ? true : false; // FIXME: need to optimize this selection
  }
  else if(num_proton_candidates == 1u){
    sel_topo_cut_passed_ = (event->topological_score_ > TOPO_SCORE_CUT * 2) ? true : false; // FIXME: need to optimize this selection
  }
  else{
    sel_topo_cut_passed_ = (event->topological_score_ > TOPO_SCORE_CUT * 0.0) ? true : false;
  }
  sel_nu_mu_cc_ = false;
  sel_ccxp0meson_ = false;
  sel_nu_mu_cc_ = sel_reco_vertex_in_fv_ && sel_pfp_starts_in_PCV_ && sel_has_muon_candidate_;
  sel_ccxp0meson_ = sel_nu_mu_cc_ && sel_muon_contained_ && sel_muon_quality_ok_ && sel_no_reco_showers_ && sel_no_reco_meson_ && sel_topo_cut_passed_ && !sel_reject_flipped_track_;

  if(sel_ccxp0meson_){
    for ( int p = 0; p < event->num_pf_particles_; ++p ) {
      // Only worry about direct neutrino daughters (PFParticles considered
      // aughters of the reconstructed neutrino)
      unsigned int generation = event->pfp_generation_->at( p );
      if ( generation != 2u ){
        sel_xsec_fs_pdg_->push_back(0);
      }
      else{
        if ( p == sel_muon_candidate_idx_ ) {
          sel_xsec_fs_pdg_->push_back(MUON);
        }
        else{
          sel_xsec_fs_pdg_->push_back(PROTON);
        }
      }
    }
  }

  return sel_ccxp0meson_;
}



void CC1muXp0pi::compute_true_observables( AnalysisEvent* event ) {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;
  // If this is not an MC event, then just return without doing anything
  if ( !event->is_mc_ ) return;
  bool true_muon = (sig_is_numu_ && is_cc_);
  size_t num_mc_daughters = event->mc_nu_daughter_pdg_->size();
  if ( true_muon ) {
    // Loop over the MC neutrino daughters, find the muon, and get its
    // true 3-momentum. Note that we assume there is only one muon in
    // this loop.
    bool found_muon = false;
    mc_p4_mu_vec_->clear();
    float max_mom = LOW_FLOAT;
    for ( size_t d = 0u; d < num_mc_daughters; ++d ) {
      int pdg = event->mc_nu_daughter_pdg_->at( d );
      if ( pdg == MUON ) {
        found_muon = true;
        float px = event->mc_nu_daughter_px_->at( d );
        float py = event->mc_nu_daughter_py_->at( d );
        float pz = event->mc_nu_daughter_pz_->at( d );
        float energy = event->mc_nu_daughter_energy_->at( d );
        TLorentzVector temp_p4_mu_ = TLorentzVector( px, py, pz, energy );
        mc_p4_mu_vec_->push_back(temp_p4_mu_);
        float mom = temp_p4_mu_.Rho();
        if(mom > max_mom){
          max_mom = mom;
          *mc_p4_mu_ = temp_p4_mu_;
        }
      }
    }

    if ( !found_muon ) {
      std::cout << "WARNING: Missing muon in MC signal event!\n";
      return;
    }
  }


  // Reset the vector of true MC proton 3-momenta
  mc_p4_proton_vec_->clear();

  // Set the true 3-momentum of the leading proton (if there is one)
  float max_mom = LOW_FLOAT;
  for ( int p = 0; p < num_mc_daughters; ++p ) {
    int pdg = event->mc_nu_daughter_pdg_->at( p );
    if ( pdg == PROTON )
    {
      float px = event->mc_nu_daughter_px_->at( p );
      float py = event->mc_nu_daughter_py_->at( p );
      float pz = event->mc_nu_daughter_pz_->at( p );
      float energy = event->mc_nu_daughter_energy_->at( p );
      TLorentzVector temp_p4 = TLorentzVector( px, py, pz, energy );

      mc_p4_proton_vec_->push_back( temp_p4 );

      float mom = temp_p4.Rho();
      if ( mom > max_mom ) {
        max_mom = mom;
        *mc_p4p_ = temp_p4;
      }
    }
  }

  // TODO: reduce code duplication by just getting the leading proton
  // 3-momentum from this sorted vector
  // Sort the true proton 3-momenta in order from highest to lowest magnitude
  std::sort( mc_p4_proton_vec_->begin(), mc_p4_proton_vec_->end(), [](const TLorentzVector& a,
        const TLorentzVector& b) -> bool { return a.E() > b.E(); } );

  // If the event contains a leading proton, then set the 3-momentum
  // accordingly
  bool true_lead_p = ( max_mom != LOW_FLOAT );
  if ( true_muon && true_lead_p ) {
    STV_Tools stv_tools;
    stv_tools.CalculateSTVs(mc_p4_mu_->Vect(), mc_p4p_->Vect(), mc_p4_mu_->E(), mc_p4p_->E(), CalcType);
    mc_lead_proton_pT_ = stv_tools.ReturnPt();
    mc_lead_proton_delta_phiT_ = stv_tools.ReturnDeltaPhiT() * TMath::Pi()/180.;
    mc_lead_proton_delta_alphaT_ = stv_tools.ReturnDeltaAlphaT() * TMath::Pi()/180.;
    mc_lead_proton_delta_alpha3Dq_ = stv_tools.ReturnDeltaAlpha3Dq() * TMath::Pi()/180.;
    mc_lead_proton_delta_alpha3DMu_ = stv_tools.ReturnDeltaAlpha3DMu() * TMath::Pi()/180.;
    mc_lead_proton_delta_phi3D_ = stv_tools.ReturnDeltaPhi3D() * TMath::Pi()/180.;
    mc_lead_proton_pL_ = stv_tools.ReturnPL();
    mc_lead_proton_pn_ = stv_tools.ReturnPn();
    mc_lead_proton_pTx_ = stv_tools.ReturnPtx();
    mc_lead_proton_pTy_ = stv_tools.ReturnPty();
    mc_lead_proton_theta_mu_p_ = stv_tools.ReturnThetaMuHadron();

    stv_tools.CalculateSTVs(*mc_p4_mu_, *mc_p4_proton_vec_, CalcType);

    mc_hadron_pT_ = stv_tools.ReturnPt();
    mc_hadron_delta_phiT_ = stv_tools.ReturnDeltaPhiT() * TMath::Pi()/180.;
    mc_hadron_delta_alphaT_ = stv_tools.ReturnDeltaAlphaT() * TMath::Pi()/180.;
    mc_hadron_delta_alpha3Dq_ = stv_tools.ReturnDeltaAlpha3Dq() * TMath::Pi()/180.;
    mc_hadron_delta_alpha3DMu_ = stv_tools.ReturnDeltaAlpha3DMu() * TMath::Pi()/180.;
    mc_hadron_delta_phi3D_ = stv_tools.ReturnDeltaPhi3D() * TMath::Pi()/180.;
    mc_hadron_pL_ = stv_tools.ReturnPL();
    mc_hadron_pn_ = stv_tools.ReturnPn();
    mc_hadron_pTx_ = stv_tools.ReturnPtx();
    mc_hadron_pTy_ = stv_tools.ReturnPty();
    mc_hadron_theta_mu_p_ = stv_tools.ReturnThetaMuHadron();
  }
}

void CC1muXp0pi::compute_reco_observables( AnalysisEvent* event ) {

  if(sel_muon_candidate_idx_ != BOGUS_INDEX){

    float p_dirx = event->track_dirx_->at( sel_muon_candidate_idx_ );
    float p_diry = event->track_diry_->at( sel_muon_candidate_idx_ );
    float p_dirz = event->track_dirz_->at( sel_muon_candidate_idx_ );

    float range_muon_mom = event->track_range_mom_mu_->at( sel_muon_candidate_idx_ );
    float mcs_muon_mom = event->track_mcs_mom_mu_->at( sel_muon_candidate_idx_ );
    float muon_mom = LOW_FLOAT;
    if(sel_muon_contained_)  muon_mom = range_muon_mom;
    else muon_mom = mcs_muon_mom;

    TVector3 direction(p_dirx, p_diry, p_dirz);

    double MuonEnergy = real_sqrt(muon_mom*muon_mom + MUON_MASS*MUON_MASS);
    reco_p4mu_->SetPxPyPzE(direction.Unit().X() * muon_mom,
        direction.Unit().Y() * muon_mom,
        direction.Unit().Z() * muon_mom,
        MuonEnergy);
  }

  if(proton_index.size() > 0){
    double temp_energy = -9999.;
    for(auto & itp: proton_index){
      TLorentzVector p4p(proton_px.at(itp.first), proton_py.at(itp.first), 
          proton_pz.at(itp.first), proton_energy.at(itp.first));
      reco_p4_p_vec_->push_back(p4p);
      
      if(proton_energy.at(itp.first) > temp_energy){
        temp_energy = proton_energy.at(itp.first);
        *reco_p4p_ = p4p;
      }
    }
  }

  if ( sel_num_muon_candidates_ != BOGUS_INDEX && reco_p4_p_vec_->size() > 0) {
    STV_Tools stv_tools;

    stv_tools.CalculateSTVs(reco_p4mu_->Vect(),reco_p4p_->Vect(),reco_p4mu_->E(), reco_p4p_->E(), CalcType);
    lead_proton_pT_ = stv_tools.ReturnPt();
    lead_proton_delta_phiT_ = stv_tools.ReturnDeltaPhiT() * TMath::Pi()/180.;
    lead_proton_delta_alphaT_ = stv_tools.ReturnDeltaAlphaT() * TMath::Pi()/180.;
    lead_proton_delta_alpha3Dq_ = stv_tools.ReturnDeltaAlpha3Dq() * TMath::Pi()/180.;
    lead_proton_delta_alpha3DMu_ = stv_tools.ReturnDeltaAlpha3DMu() * TMath::Pi()/180;
    lead_proton_delta_phi3D_ = stv_tools.ReturnDeltaPhi3D() * TMath::Pi()/180.;
    lead_proton_pL_ = stv_tools.ReturnPL();
    lead_proton_pn_ = stv_tools.ReturnPn();
    lead_proton_pTx_ = stv_tools.ReturnPtx();
    lead_proton_pTy_ = stv_tools.ReturnPty();
    lead_proton_theta_mu_p_ = stv_tools.ReturnThetaMuHadron();


    stv_tools.CalculateSTVs(*reco_p4mu_, *reco_p4_p_vec_, CalcType);
    hadron_pT_ = stv_tools.ReturnPt();
    hadron_delta_phiT_ = stv_tools.ReturnDeltaPhiT() * TMath::Pi()/180.;
    hadron_delta_alphaT_ = stv_tools.ReturnDeltaAlphaT() * TMath::Pi()/180.;
    hadron_delta_alpha3Dq_ = stv_tools.ReturnDeltaAlpha3Dq() * TMath::Pi()/180.;
    hadron_delta_alpha3DMu_ = stv_tools.ReturnDeltaAlpha3DMu() * TMath::Pi()/180;
    hadron_delta_phi3D_ = stv_tools.ReturnDeltaPhi3D() * TMath::Pi()/180.;
    hadron_pL_ = stv_tools.ReturnPL();
    hadron_pn_ = stv_tools.ReturnPn();
    hadron_pTx_ = stv_tools.ReturnPtx();
    hadron_pTy_ = stv_tools.ReturnPty();
    hadron_theta_mu_p_ = stv_tools.ReturnThetaMuHadron();

  }

}

int CC1muXp0pi::categorize_event(AnalysisEvent* event) {
  // Real data has a bogus true neutrino PDG code that is not one of the
  // allowed values (±12, ±14, ±16)

  int abs_mc_nu_pdg = std::abs( event->mc_nu_pdg_ );
  event->is_mc_ = ( abs_mc_nu_pdg == ELECTRON_NEUTRINO || abs_mc_nu_pdg == MUON_NEUTRINO || abs_mc_nu_pdg == TAU_NEUTRINO );

  if ( !event->is_mc_ ) {
    return kUnknown;
  }

  bool MCVertexInFV = point_inside_FV( this->true_FV(),
      event->mc_nu_vx_, event->mc_nu_vy_, event->mc_nu_vz_ );
  if ( !MCVertexInFV ) {
    return kOOFV;
  }

  bool isNC = ( event->mc_nu_ccnc_ == NEUTRAL_CURRENT );
  // DB Currently only one NC category is supported so test first. Will likely
  // want to change this in the future
  if ( isNC ) return kNC;


  if ( event->mc_nu_pdg_ == ELECTRON_NEUTRINO ) {
    return kNuECC;
  }
  if ( !(event->mc_nu_pdg_ == MUON_NEUTRINO) ) {
    return kOther;
  }

  if(this->is_event_mc_signal()){
    switch (sig_mc_num_proton_in_momentum_range_){
      case 0:{
               switch(event->mc_nu_interaction_type_){
                 case 0: return kNuMuCC0p0pi_CCQE;
                 case 1: return kNuMuCC0p0pi_CCRES;
                 case 10: return kNuMuCC0p0pi_CCMEC;
                 default: return kNuMuCC0p0pi_Other;
               }
             }
      case 1:{
               //     std::cout << __FILE__ << "  " << __LINE__ << std::endl;
               //     std::cout << __FILE__ << "  " << __LINE__  << "  " << event->mc_nu_daughter_pdg_->size() << std::endl;
               if(event->mc_generator_pdg_){
                 //     std::cout << __FILE__ << "  " << __LINE__ << std::endl;
                 double output_proton_energy = 0;
                 int mother_id = -1;
                 for ( size_t p = 0u; p < event->mc_nu_daughter_pdg_->size(); ++p ) {
                   int pdg = event->mc_nu_daughter_pdg_->at( p );
                   float energy = event->mc_nu_daughter_energy_->at( p );
                   if(pdg == 2212 && energy > sqrt(PROTON_MASS * PROTON_MASS + 0.25*0.25)){
                     //std::cout << "uboone energy : " << energy << std::endl;
                     output_proton_energy = energy;
                   }
                 }

                 for(int p = 0; p < event->mc_generator_pdg_->size(); p++){
                   if(event->mc_generator_pdg_->at(p) == 2212 && event->mc_generator_statuscode_->at(p) == 1){
                     //std::cout << "genie energy : " << event->mc_generator_E_->at(p) << std::endl;
                     if(fabs(event->mc_generator_E_->at(p) - output_proton_energy) < 0.0001){
                       //std::cout << "genie energy : " << event->mc_generator_E_->at(p) << std::endl;
                       mother_id = event->mc_generator_mother_->at(p);
                     }
                   }
                 }
                 bool fsi = false;
                 while(mother_id!=-1){
                   //std::cout << "mother energy : " << event->mc_generator_E_->at(mother_id) << std::endl;
                   //std::cout << "mother scatter : " << event->mc_generator_rescatter_->at(mother_id) << std::endl;
                   if(event->mc_generator_rescatter_->at(mother_id) > 1)
                     fsi=true;
                   mother_id = event->mc_generator_mother_->at(mother_id);
                 }
                 if(fsi){
                   switch(event->mc_nu_interaction_type_){
                     case 0: return kNuMuCC1p0pi_CCQE_FSI;
                     case 1: return kNuMuCC1p0pi_CCRES_FSI;
                     case 10: return kNuMuCC1p0pi_CCMEC_FSI;
                     default: return kNuMuCC1p0pi_Other_FSI;
                   }
                 }
                 else{
                   switch(event->mc_nu_interaction_type_){
                     case 0: return kNuMuCC1p0pi_CCQE_NONFSI;
                     case 1: return kNuMuCC1p0pi_CCRES_NONFSI;
                     case 10: return kNuMuCC1p0pi_CCMEC_NONFSI;
                     default: return kNuMuCC1p0pi_Other_NONFSI;
                   }
                 }
               }
               else{
                 switch(event->mc_nu_interaction_type_){
                   case 0: return kNuMuCC1p0pi_CCQE_FSI;
                   case 1: return kNuMuCC1p0pi_CCRES_FSI;
                   case 10: return kNuMuCC1p0pi_CCMEC_FSI;
                   default: return kNuMuCC1p0pi_Other_FSI;
                 }
               }
             }
      case 2:{
               switch(event->mc_nu_interaction_type_){
                 case 0: return kNuMuCC2p0pi_CCQE;
                 case 1: return kNuMuCC2p0pi_CCRES;
                 case 10: return kNuMuCC2p0pi_CCMEC;
                 default: return kNuMuCC2p0pi_Other;
               }
             }
      default:{
                switch(event->mc_nu_interaction_type_){
                  case 0: return kNuMuCCMp0pi_CCQE;
                  case 1: return kNuMuCCMp0pi_CCRES;
                  case 10: return kNuMuCCMp0pi_CCMEC;
                  default: return kNuMuCCMp0pi_Other;
                }
              }
    }
  }
  else if(!sig_mc_no_fs_mesons_ ){
    return kNuMuCCNpi;
  }
  return kNuMuCCOther;
}

void CC1muXp0pi::define_output_branches() {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;

  set_branch(&sig_in_fv_, "sig_in_fv");
  set_branch(&sig_in_fv_loose_, "sig_in_fv_loose");
  set_branch(&sig_is_numu_, "sig_is_numu");
  set_branch(&is_cc_, "is_cc");

  set_branch(&sig_mc_num_muon_in_momentum_range_, "sig_mc_num_muon_in_momentum_range");
  set_branch(&sig_mc_num_muon_, "sig_mc_num_muon");
  set_branch(&sig_mc_num_proton_in_momentum_range_, "sig_mc_num_proton_in_momentum_range");
  set_branch(&sig_mc_num_proton_, "sig_mc_num_proton");
  set_branch(&sig_mc_num_charged_pi_above_threshold_, "sig_mc_num_charged_pi_above_threshold");
  set_branch(&sig_mc_num_charged_pi_, "sig_mc_num_charged_pi");
  set_branch(&sig_mc_no_fs_mesons_, "sig_mc_no_fs_mesons");
  set_branch(&sig_mc_no_fs_pi0_, "sig_mc_no_fs_pi0");
  set_branch(&sig_mc_no_fs_charged_pi_, "sig_mc_no_fs_charged_pi");
  set_branch(&sig_mc_no_fs_charged_pi_above_threshold_, "sig_mc_no_fs_charged_pi_above_threshold");
  set_branch(&sig_mc_no_fs_additional_, "sig_mc_no_fs_additional");

  set_branch(&sig_mc_LeadProtonMomentum, "sig_mc_LeadProtonMomentum");
  set_branch(&mc_is_signal_charged_pi_above_threshold_, "mc_is_signal_charged_pi_above_threshold");
  set_branch(&mc_is_signal_no_meson_, "mc_is_signal_no_meson");
  set_branch(&mc_is_signal_, "mc_is_signal");

  set_branch(&sel_reco_vertex_in_fv_, "sel_reco_vertex_in_fv");
  set_branch(&sel_pfp_starts_in_PCV_, "sel_pfp_starts_in_PCV");
  set_branch(&sel_muon_candidate_idx_, "sel_muon_candidate_idx");
  set_branch(&sel_num_muon_candidates_, "sel_num_muon_candidates");
  set_branch(&sel_num_proton_candidates_, "sel_num_proton_candidates");
  set_branch(&lead_p_candidate_idx_, "lead_p_candidate_idx");
  set_branch(&sel_has_muon_candidate_, "sel_has_muon_candidate");
  set_branch(&sel_muon_contained_, "sel_muon_contained");
  set_branch(&sel_muon_quality_ok_, "sel_muon_quality_ok");
  set_branch(&sel_muon_passed_mom_cuts_, "sel_muon_passed_mom_cuts");
  set_branch(&sel_no_reco_showers_, "sel_no_reco_showers");
  set_branch(&sel_no_reco_meson_, "sel_no_reco_meson");
  set_branch(&sel_topo_cut_passed_, "sel_topo_cut_passed");
  set_branch(&sel_reject_flipped_track_, "sel_reject_flipped_track");
  set_branch(&sel_nu_mu_cc_, "sel_nu_mu_cc");
  set_branch(&sel_ccxp0meson_, "sel_ccxp0meson");

  set_branch(sel_xsec_fs_pdg_, "sel_xsec_fs_pdg");
  set_branch(&sel_track_length_size_, "sel_track_length_size");
  set_branch(&sel_trk_bragg_mu_fwd_preferred_, "sel_trk_bragg_mu_fwd_preferred");
  set_branch(&sel_track_chi2_muon_, "sel_track_chi2_muon");


  set_branch(mc_p4_mu_, "mc_p4_mu");
  set_branch(mc_p4p_, "mc_p4p");
  set_branch(mc_p4_proton_vec_, "mc_p4_proton_vec");
  set_branch(mc_p4_mu_vec_, "mc_p4_mu_vec");

  set_branch(&mc_hadron_pT_, "mc_hadron_pT");
  set_branch(&mc_hadron_delta_phiT_, "mc_hadron_delta_phiT");
  set_branch(&mc_hadron_delta_alphaT_, "mc_hadron_delta_alphaT");
  set_branch(&mc_hadron_delta_alpha3Dq_, "mc_hadron_delta_alpha3Dq");
  set_branch(&mc_hadron_delta_alpha3DMu_, "mc_hadron_delta_alpha3DMu");
  set_branch(&mc_hadron_delta_phi3D_, "mc_hadron_delta_phi3D");
  set_branch(&mc_hadron_pL_, "mc_hadron_pL");
  set_branch(&mc_hadron_pn_, "mc_hadron_pn");
  set_branch(&mc_hadron_pTx_, "mc_hadron_pTx");
  set_branch(&mc_hadron_pTy_, "mc_hadron_pTy");
  set_branch(&mc_hadron_theta_mu_p_, "mc_hadron_theta_mu_p");

  set_branch(&mc_lead_proton_pT_, "mc_lead_proton_pT");
  set_branch(&mc_lead_proton_delta_phiT_, "mc_lead_proton_delta_phiT");
  set_branch(&mc_lead_proton_delta_alphaT_, "mc_lead_proton_delta_alphaT");
  set_branch(&mc_lead_proton_delta_alpha3Dq_, "mc_lead_proton_delta_alpha3Dq");
  set_branch(&mc_lead_proton_delta_alpha3DMu_, "mc_lead_proton_delta_alpha3DMu");
  set_branch(&mc_lead_proton_delta_phi3D_, "mc_lead_proton_delta_phi3D");
  set_branch(&mc_lead_proton_pL_, "mc_lead_proton_pL");
  set_branch(&mc_lead_proton_pn_, "mc_lead_proton_pn");
  set_branch(&mc_lead_proton_pTx_, "mc_lead_proton_pTx");
  set_branch(&mc_lead_proton_pTy_, "mc_lead_proton_pTy");
  set_branch(&mc_lead_proton_theta_mu_p_, "mc_lead_proton_theta_mu_p");

  set_branch(reco_p4mu_, "reco_p4mu");
  set_branch(reco_p4p_, "reco_p4p");
  set_branch(reco_p4_p_vec_, "reco_p4_p_vec");

  set_branch(&hadron_pT_, "hadron_pT");
  set_branch(&hadron_delta_phiT_, "hadron_delta_phiT");
  set_branch(&hadron_delta_alphaT_, "hadron_delta_alphaT");
  set_branch(&hadron_delta_alpha3Dq_, "hadron_delta_alpha3Dq");
  set_branch(&hadron_delta_alpha3DMu_, "hadron_delta_alpha3DMu");
  set_branch(&hadron_delta_phi3D_, "hadron_delta_phi3D");
  set_branch(&hadron_pL_, "hadron_pL");
  set_branch(&hadron_pn_, "hadron_pn");
  set_branch(&hadron_pTx_, "hadron_pTx");
  set_branch(&hadron_pTy_, "hadron_pTy");
  set_branch(&hadron_theta_mu_p_, "hadron_theta_mu_p");

  set_branch(&lead_proton_pT_, "lead_proton_pT");
  set_branch(&lead_proton_delta_phiT_, "lead_proton_delta_phiT");
  set_branch(&lead_proton_delta_alphaT_, "lead_proton_delta_alphaT");
  set_branch(&lead_proton_delta_alpha3Dq_, "lead_proton_delta_alpha3Dq");
  set_branch(&lead_proton_delta_alpha3DMu_, "lead_proton_delta_alpha3DMu");
  set_branch(&lead_proton_delta_phi3D_, "lead_proton_delta_phi3D");
  set_branch(&lead_proton_pL_, "lead_proton_pL");
  set_branch(&lead_proton_pn_, "lead_proton_pn");
  set_branch(&lead_proton_pTx_, "lead_proton_pTx");
  set_branch(&lead_proton_pTy_, "lead_proton_pTy");
  set_branch(&lead_proton_theta_mu_p_, "lead_proton_theta_mu_p");


}

void CC1muXp0pi::reset() {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;
  // variable for signal definition
  sig_in_fv_ = false;
  sig_in_fv_loose_ = false;
  sig_is_numu_ = false;
  is_cc_ = false;

  sig_mc_num_muon_in_momentum_range_ = BOGUS;
  sig_mc_num_muon_ = BOGUS;
  sig_mc_num_proton_in_momentum_range_ = BOGUS;
  sig_mc_num_proton_ = BOGUS;
  sig_mc_num_charged_pi_above_threshold_ = BOGUS;
  sig_mc_num_charged_pi_ = BOGUS;
  sig_mc_LeadProtonMomentum = BOGUS;

  sig_mc_no_fs_mesons_ = false;
  sig_mc_no_fs_pi0_ = false;
  sig_mc_no_fs_charged_pi_ = false;
  sig_mc_no_fs_charged_pi_above_threshold_ = false;
  sig_mc_no_fs_additional_ = false;

  mc_is_signal_charged_pi_above_threshold_ = false;
  mc_is_signal_no_meson_ = false;
  mc_is_signal_ = false;

  // variables for event selection
  sel_reco_vertex_in_fv_ = false;
  sel_pfp_starts_in_PCV_ = false;
  sel_muon_candidate_indices_.clear();
  sel_muon_pid_scores_.clear();
  sel_xsec_fs_pdg_->clear();
  sel_muon_candidate_idx_ = BOGUS_INDEX;
  sel_num_muon_candidates_ = BOGUS_INDEX;
  sel_num_proton_candidates_ = BOGUS;
  lead_p_candidate_idx_ = BOGUS;
  sel_has_muon_candidate_ = false;
  sel_muon_contained_ = false;
  sel_muon_quality_ok_ = false;
  sel_muon_passed_mom_cuts_ = false;
  sel_no_reco_showers_ = false;
  sel_no_reco_meson_ = false;
  sel_topo_cut_passed_ = false;
  sel_nu_mu_cc_ = false;
  sel_ccxp0meson_ = false;
  sel_reject_flipped_track_ = false;
  sel_track_length_size_ = BOGUS;
  sel_trk_bragg_mu_fwd_preferred_ = false;
  sel_track_chi2_muon_ = BOGUS;

  proton_index.clear();
  proton_energy.clear();
  proton_px.clear();
  proton_py.clear();
  proton_pz.clear();

  *mc_p4_mu_ = TLorentzVector();
  *mc_p4p_ = TLorentzVector();
  mc_p4_proton_vec_->clear();
  mc_p4_mu_vec_->clear();

  mc_hadron_pT_ = BOGUS;
  mc_hadron_delta_phiT_ = BOGUS;
  mc_hadron_delta_alphaT_ = BOGUS;
  mc_hadron_delta_alpha3Dq_ = BOGUS;
  mc_hadron_delta_alpha3DMu_ = BOGUS;
  mc_hadron_delta_phi3D_ = BOGUS;
  mc_hadron_pL_ = BOGUS;
  mc_hadron_pn_ = BOGUS;
  mc_hadron_pTx_ = BOGUS;
  mc_hadron_pTy_ = BOGUS;
  mc_hadron_theta_mu_p_ = BOGUS;

  mc_lead_proton_pT_ = BOGUS;
  mc_lead_proton_delta_phiT_ = BOGUS;
  mc_lead_proton_delta_alphaT_ = BOGUS;
  mc_lead_proton_delta_alpha3Dq_ = BOGUS;
  mc_lead_proton_delta_alpha3DMu_ = BOGUS;
  mc_lead_proton_delta_phi3D_ = BOGUS;
  mc_lead_proton_pL_ = BOGUS;
  mc_lead_proton_pn_ = BOGUS;
  mc_lead_proton_pTx_ = BOGUS;
  mc_lead_proton_pTy_ = BOGUS;
  mc_lead_proton_theta_mu_p_ = BOGUS;


  // reconstructed variables


  *reco_p4mu_ = TLorentzVector();
  *reco_p4p_ = TLorentzVector();
  reco_p4_p_vec_->clear();


  hadron_pT_ = BOGUS;
  hadron_delta_phiT_ = BOGUS;
  hadron_delta_alphaT_ = BOGUS;
  hadron_delta_alpha3Dq_ = BOGUS;
  hadron_delta_alpha3DMu_ = BOGUS;
  hadron_delta_phi3D_ = BOGUS;
  hadron_pL_ = BOGUS;
  hadron_pn_ = BOGUS;
  hadron_pTx_ = BOGUS;
  hadron_pTy_ = BOGUS;
  hadron_theta_mu_p_ = BOGUS;

  lead_proton_pT_ = BOGUS;
  lead_proton_delta_phiT_ = BOGUS;
  lead_proton_delta_alphaT_ = BOGUS;
  lead_proton_delta_alpha3Dq_ = BOGUS;
  lead_proton_delta_alpha3DMu_ = BOGUS;
  lead_proton_delta_phi3D_ = BOGUS;
  lead_proton_pL_ = BOGUS;
  lead_proton_pn_ = BOGUS;
  lead_proton_pTx_ = BOGUS;
  lead_proton_pTy_ = BOGUS;
  lead_proton_theta_mu_p_ = BOGUS;

}

void CC1muXp0pi::define_category_map() {
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;
  // Use the shared category map for 1p/2p/Np/Xp
  categ_map_ = CCXp0pi_FSI;
}

/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void CC1muXp0pi::define_additional_input_branches(TTree& etree){
  //std::cout << __FUNCTION__ << "  " << __LINE__ << std::endl;
}

////////////////////////////////////////////////////////////////
// use xgbooster to classify particles
////////////////////////////////////////////////////////////////
void CC1muXp0pi::classify_tracks(AnalysisEvent* event){

}



 // Implementing pure virtual functions
    int CC1muXp0pi::categorize_event(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return 0;  // Return a default integer value
    }

    bool CC1muXp0pi::selection(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return false;  // Return a default boolean value
    }

    bool CC1muXp0pi::define_signal(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return false;  // Return a default boolean value
    }

    void CC1muXp0pi::compute_reco_observables(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
    }

    void CC1muXp0pi::compute_true_observables(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
    }