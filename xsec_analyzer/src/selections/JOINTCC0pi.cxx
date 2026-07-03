// XSecAnalyzer includes
#include "XSecAnalyzer/FiducialVolume.hh"
#include "XSecAnalyzer/Functions.hh"
#include "XSecAnalyzer/TreeUtils.hh"
//#include "XSecAnalyzer/MCSTools.hh"

#include "XSecAnalyzer/Selections/JOINTCC0pi.hh"
#include "XSecAnalyzer/Selections/EventCategoriesJOINTCC0pi.hh"

// XGBoost
//#include <xgboost/c_api.h>

// XGBoost model
//BoosterHandle* booster;

JOINTCC0pi::JOINTCC0pi() : SelectionBase("JOINTCC0pi"), MCS_TOOL_(FV_X_MIN, FV_X_MAX ,FV_Y_MIN, FV_Y_MAX ,FV_Z_MIN, FV_Z_MAX) {
  //CalcType = kOpt1;
  booster = new BoosterHandle;
  XGBoosterCreate(NULL, 0, booster);
  assert(XGBoosterLoadModel(*booster, "/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/lib/cc0pi_fstrack_pid_softprob.json") == 0);
  std::cout<<"Got XGBoosterCreate  "<<std::endl;
  std::cout<<"Finished with Constuctor  "<<std::endl;
  xgb_pid_vec_.reset( new std::vector<int>() );
  this->define_category_map();  
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::define_constants() {

  this->define_true_FV(FV_X_MIN,FV_X_MAX ,FV_Y_MIN,FV_Y_MAX,FV_Z_MIN,FV_Z_MAX);
  this->define_reco_FV(FV_X_MIN,FV_X_MAX ,FV_Y_MIN,FV_Y_MAX,FV_Z_MIN,FV_Z_MAX); // old 10.,246.,-105.,105.,10.,1026.
  std::cout<<"Finished ::DefineConstants "<< std::endl;
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::compute_reco_observables(AnalysisEvent* event){

 if(verbosal > 0) std::cout<< "FInished truth observables"<< std::endl;
  // In cases where we failed to find a muon candidate, check whether there are
  // at least two generation == 2 PFParticles. If there are, then compute the
  // usual observables using the longest track as the muon candidate and the
  // second-longest track as the leading proton candidate. This will enable
  // sideband studies of NC backgrounds in the STV phase space.
  if ( !sel_has_muon_candidate_ ) {

    float max_trk_len = LOW_FLOAT;
    int max_trk_idx = BOGUS_INDEX;

    float next_to_max_trk_len = LOW_FLOAT;
    int next_to_max_trk_idx = BOGUS_INDEX;
    
    if(verbosal > 0) std::cout<< "Starting Loop (line 53 JOINTCC0pi) ;  num_pf_particles_ = "<<event->num_pf_particles_<< std::endl;
   // std::cout<<"event->num_pf_particles_ = " << event->num_pf_particles_ << " event->pfp_generation_->size() = " <<  event->pfp_generation_->size()<<std::endl;

   // if (event->num_pf_particles_ != event->pfp_generation_->size()) return; 

   if(event->pfp_generation_->size()==0)return; 

    for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {

      // Only include direct neutrino daughters (generation == 2)
     unsigned int generation = event->pfp_generation_->at( p );
     if ( generation != 2 ) continue;

      float trk_len = event->track_length_->at( p );

      if ( trk_len > next_to_max_trk_len ) {

        next_to_max_trk_len = trk_len;
        next_to_max_trk_idx = p;

        if ( next_to_max_trk_len > max_trk_len ) {

          next_to_max_trk_len = max_trk_len;
          next_to_max_trk_idx = max_trk_idx;

          max_trk_len = trk_len;
          max_trk_idx = p;
        }
      }
    }
    
   if(verbosal > 0) std::cout<< "Finished  num_pf_particles_ loop "<< std::endl;
    // If we found at least two usable PFParticles, then assign the indices to
    // be used below
    if ( max_trk_idx != BOGUS_INDEX && next_to_max_trk_idx != BOGUS_INDEX ) {
      muon_candidate_idx_ = max_trk_idx;
      lead_p_candidate_idx_ = next_to_max_trk_idx;
    }
  }

  // Abbreviate some of the calculations below by using these handy
  // references to the muon and leading proton 3-momenta
      if(verbosal > 0)std::cout<< "Setting p3mu   "<< std::endl;
  auto& p3mu = *p3_mu_;
      if(verbosal > 0) std::cout<< "Setting p3mu_rangle   "<< std::endl;
  auto& p3mu_range = *p3_mu_range_;
    if(verbosal > 0)  std::cout<< "Setting p3mu_mcs   "<< std::endl;
  auto& p3mu_mcs = *p3_mu_mcs_;
  auto& p3p = *p3_lead_p_;
 if(verbosal > 0)  std::cout<< "muon_candidate_idx_ =  "<< muon_candidate_idx_<< std::endl;
 if(verbosal > 0)  std::cout<< "lead_p_candidate_idx_ =  "<< lead_p_candidate_idx_<< std::endl;
  // Set the reco 3-momentum of the muon candidate if we found one
  bool muon = muon_candidate_idx_ != BOGUS_INDEX;
  if ( muon ) {
  if(verbosal > 0) std::cout<<"inside is muon"<< std::endl;
  
    float mu_dirx = event->track_dirx_->at( muon_candidate_idx_ );
    float mu_diry = event->track_diry_->at( muon_candidate_idx_ );
    float mu_dirz = event->track_dirz_->at( muon_candidate_idx_ );

 if(verbosal > 0) std::cout<<"inside is muon:line 107"<< std::endl;

    p3mu = TVector3( mu_dirx, mu_diry, mu_dirz );
    p3mu_mcs = TVector3( mu_dirx, mu_diry, mu_dirz );
    p3mu_range= TVector3( mu_dirx, mu_diry, mu_dirz );
    // The selection flag indicating whether the muon candidate is contained
    // was already set when the selection was applied. Use it to choose the
    // best momentum estimator to use.
    if(verbosal > 0) std::cout<<"inside is muon:line 115"<< std::endl;
    float muon_mom = LOW_FLOAT;
    float muon_muon_range = event->track_range_mom_mu_->at( muon_candidate_idx_ );
    float muon_muon_mcs = event->track_mcs_mom_mu_->at( muon_candidate_idx_ );
    if(verbosal > 0) std::cout<<"inside is muon:line 119"<< std::endl;
    /// Set Muon Tracklen
    muontrklen_ = event->track_length_->at( muon_candidate_idx_ );
    muondistancetovectex_ =  event->track_start_distance_->at( muon_candidate_idx_ );
    muon_llr_pid_score_ =  event->track_llr_pid_score_->at( muon_candidate_idx_ );

   if(verbosal > 0) std::cout<<"inside is muon:line 125"<< std::endl;
    muon_tkscore_ = event->pfp_track_score_->at( muon_candidate_idx_ );

    muon_trkstart_x_ = event->track_startx_->at(muon_candidate_idx_);
    muon_trkstart_y_ = event->track_starty_->at(muon_candidate_idx_);
    muon_trkstart_z_ = event->track_startz_->at(muon_candidate_idx_);

   if(verbosal > 0) std::cout<<"inside is muon:line 132"<< std::endl;
    muon_trkend_x_ = event->track_endx_->at(muon_candidate_idx_);
    muon_trkend_y_ = event->track_endy_->at(muon_candidate_idx_);
    muon_trkend_z_ = event->track_endz_->at(muon_candidate_idx_);

   if(verbosal > 0) std::cout<<"inside is muon:line 137"<< std::endl;
 
   if(verbosal > 0) std::cout<<"event->track_chi2_muon_->Size() = "<< event->track_chi2_muon_->size()<<std::endl;
 
 
    muon_trkchi2muon_ = event->track_chi2_muon_->at(muon_candidate_idx_);

  if( verbosal > 0 ) std::cout<< "Finished getting float muon_mom  float muon_muon_range loat muon_muon_mcs   "<< std::endl;

    
    PanelProjection_X_Z_ = MCS_TOOL_.PanelSingle_TB(muon_trkend_x_,muon_trkend_z_);
    PanelProjection_Y_Z_ = MCS_TOOL_.PanelSingle_LR(muon_trkend_y_,muon_trkend_z_);
    PanelProjection_X_Y_ = MCS_TOOL_.PanelSingle_FB(muon_trkend_x_,muon_trkend_y_);
     
     sel_PanelClosesttoEndMuonTrk_ =   MCS_TOOL_.ClosestSide(muon_trkend_x_, muon_trkend_y_, muon_trkend_z_);
     sel_PanelClosestSubPaneltoEndMuonTrk_ =   MCS_TOOL_.returnSINGLEPanel_int(muon_trkend_x_, muon_trkend_y_, muon_trkend_z_);


 if( verbosal > 0 ) std::cout<<"(muon_trkend_x, muon_trkend_y, muon_trkend_z) =   (" << muon_trkend_x_<< " , "<< muon_trkend_y_<< " , " <<muon_trkend_z_ << " ) "<< std::endl;
 if( verbosal > 0 ) std::cout<<"Checking  sel_PanelClosesttoEndMuonTrk_ = " << sel_PanelClosesttoEndMuonTrk_ << std::endl;
 if( verbosal > 0 ) std::cout<<"Checking  sel_PanelClosestSubPaneltoEndMuonTrk_ = " << sel_PanelClosestSubPaneltoEndMuonTrk_ << std::endl;
 
 if( verbosal > 0 ) std::cout<<"Checking  PanelProjection_X_Z_ = " << PanelProjection_X_Z_ << std::endl;
 if( verbosal > 0 ) std::cout<<"Checking  PanelProjection_Y_Z_ = " << PanelProjection_Y_Z_ << std::endl;
 if( verbosal > 0 ) std::cout<<"Checking  PanelProjection_X_Y_ = " << PanelProjection_X_Y_ << std::endl;




   ////No Corrections//////
   /*if ( sel_muon_contained_ ) {
       muon_mom = track_range_mom_mu_->at( muon_candidate_idx_ );
    }
    else{
       muon_mom = track_mcs_mom_mu_->at( muon_candidate_idx_ );
    }*/

    if ( sel_muon_contained_ ) {
    // Unsure how to if I should apply it since not all root files have these branches
    if (event->track_length_->size() == 1 && event->trk_bragg_mu_fwd_preferred_v_->at(muon_candidate_idx_) == 0 && 6. < event->track_chi2_muon_->at(muon_candidate_idx_)){
            //std::cout<<"Rejecting flipped tracks"<<std::endl;
             muon_mom = LOW_FLOAT;
            }
    else{
       muon_mom = event->track_range_mom_mu_->at( muon_candidate_idx_ );
       }
     }

    else{
    if(p3mu.CosTheta() > -0.9){


          if (event->track_mcs_mom_mu_->at( muon_candidate_idx_ ) < 0.11){
              muon_mom = LOW_FLOAT;
          }
          else{
              muon_mom = event->track_mcs_mom_mu_->at( muon_candidate_idx_ ) - 0.0361*event->track_mcs_mom_mu_->at( muon_candidate_idx_ ) + 0.04;
          }
    }
    else{
    muon_mom = LOW_FLOAT;
    }

    }

    if(verbosal > 0)std::cout<<"  muon_mom = "<<  muon_mom << std::endl;



    p3mu = p3mu.Unit() * muon_mom;
    p3mu_mcs = p3mu.Unit() * muon_muon_mcs;
    p3mu_range = p3mu.Unit() * muon_muon_range;

    if(verbosal > 0) std::cout<<" stating distance_FV_surface_" << std::endl;
    distance_FV_surface_ = this->reco_distance_to_FV_Surface(event);

    if(verbosal > 0)std::cout<<"  Finsished ::distance_FV_surface_ =" << distance_FV_surface_<<std::endl;

  }
  // Set the reco 3-momentum of the leading proton candidate if we found one
  bool lead_p = lead_p_candidate_idx_ != BOGUS_INDEX;
  if ( lead_p ) {
     if(verbosal > 0) std::cout<<"inside lead proton "<< std::endl;
    float p_dirx = event->track_dirx_->at( lead_p_candidate_idx_ );
    float p_diry = event->track_diry_->at( lead_p_candidate_idx_ );
    float p_dirz = event->track_dirz_->at( lead_p_candidate_idx_ );
    float KEp = event->track_kinetic_energy_p_->at( lead_p_candidate_idx_ );
    float p_mom = real_sqrt( KEp*KEp + 2.*PROTON_MASS*KEp );

    p3p = TVector3( p_dirx, p_diry, p_dirz );
    p3p = p3p.Unit() * p_mom;
    
    
  }

  // Reset the vector of reconstructed proton candidate 3-momenta
  p3_p_vec_->clear();

  // Set the reco 3-momenta of all proton candidates (i.e., all generation == 2
  // tracks except the muon candidate) assuming we found both a muon candidate
  // and at least one proton candidate.
   if ( muon && lead_p ) {
    for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {
      // Skip the muon candidate
      if ( p == muon_candidate_idx_ ) continue;

      // Only include direct neutrino daughters (generation == 2)
      unsigned int generation = event->pfp_generation_->at( p );
      if ( generation != 2u ) continue;

      float p_dirx = event->track_dirx_->at( p );
      float p_diry = event->track_diry_->at( p );
      float p_dirz = event->track_dirz_->at( p );
      float KEp = event->track_kinetic_energy_p_->at( p );
      float p_mom = real_sqrt( KEp*KEp + 2.*PROTON_MASS*KEp );

      TVector3 p3_temp( p_dirx, p_diry, p_dirz );
      p3_temp = p3_temp.Unit() * p_mom;

      p3_p_vec_->push_back( p3_temp );
    }


    // TODO: reduce code duplication by just getting the leading proton
    // 3-momentum from this sorted vector
    // Sort the reco proton 3-momenta in order from highest to lowest magnitude
    std::sort( p3_p_vec_->begin(), p3_p_vec_->end(), [](const TVector3& a,
      const TVector3& b) -> bool { return a.Mag() > b.Mag(); } );
  }

  // Compute reco STVs if we have both a muon candidate
  // and a leading proton candidate in the event
  if ( muon && lead_p ) {
    this->compute_stvs( p3mu, p3p, delta_pT_, delta_phiT_,
      delta_alphaT_, delta_pL_, pn_, delta_pTx_,delta_pTy_ );

    theta_mu_p_ = std::acos( p3mu.Dot(p3p) / p3mu.Mag() / p3p.Mag() );
  }


}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::compute_true_observables(AnalysisEvent* event){

 // If this is not an MC event, then just return without doing anything
 if(verbosal > 0){std::cout<<"inside::compute_true_observables"<<std::endl;
  std::cout<<"is_mc_ = "<< event->is_mc_ <<std::endl;}
 
  if ( !event->is_mc_ ) return;

  size_t num_mc_daughters = event->mc_nu_daughter_pdg_->size();

  // Set the true 3-momentum of the final-state muon if there is one
  bool true_muon = ( mc_neutrino_is_numu_ && event->mc_nu_ccnc_ == CHARGED_CURRENT );
  if ( true_muon ) {
    // Loop over the MC neutrino daughters, find the muon, and get its
    // true 3-momentum. Note that we assume there is only one muon in
    // this loop.
    bool found_muon = false;
    for ( size_t d = 0u; d < num_mc_daughters; ++d ) {
      int pdg = event->mc_nu_daughter_pdg_->at( d );
      if ( pdg == MUON ) {
        found_muon = true;
        float px = event->mc_nu_daughter_px_->at( d );
        float py = event->mc_nu_daughter_py_->at( d );
        float pz = event->mc_nu_daughter_pz_->at( d );
        *mc_p3_mu_ = TVector3( px, py, pz );
        
        if(verbosal > 0){std::cout << "mc_nu_vx: " << event->mc_nu_vx_ << "\n"
          << "mc_nu_vy: " << event->mc_nu_vy_ << "\n"
          << "mc_nu_vz: " << event->mc_nu_vz_ << "\n"
          << "mc_nu_daughter_endx[" << d << "]: " << event->mc_nu_daughter_endx_->at(d) << "\n"
          << "mc_nu_daughter_endy[" << d << "]: " << event->mc_nu_daughter_endy_->at(d) << "\n"
          << "mc_nu_daughter_endz[" << d << "]: " << event->mc_nu_daughter_endz_->at(d) << std::endl;
           }
        
        mc_muontrklen_ = distanceBetweentwopoint(
                          event->mc_nu_vx_,
                          event->mc_nu_vy_,
                          event->mc_nu_vz_,
                          event->mc_nu_daughter_endx_->at( d ),
                          event->mc_nu_daughter_endy_->at( d ),
                          event->mc_nu_daughter_endz_->at( d ));

        break;
      }
    }

    if ( !found_muon ) {
      std::cout << "WARNING: Missing muon in MC signal event!\n";
      return;
    }
  }

  // Reset the vector of true MC proton 3-momenta
  mc_p3_p_vec_->clear();

  // Set the true 3-momentum of the leading proton (if there is one)
  float max_mom = LOW_FLOAT;
  float max_mom_Pion = LOW_FLOAT;
  float mc_Eavail = 0.0;
  if(verbosal > 0){
  std::cout<<" num_mc_daughters = "<<num_mc_daughters<< std::endl;
  }

  
  for ( int p = 0; p < num_mc_daughters; ++p ) {
  
      int pdg = event->mc_nu_daughter_pdg_->at( p );
      // ignore muon and neutrons
      if(abs(pdg) == 13 || pdg == 2112) continue;
     float energy = event->mc_nu_daughter_energy_->at( p );
     
      float px = event->mc_nu_daughter_px_->at( p );
      float py = event->mc_nu_daughter_py_->at( p );
      float pz = event->mc_nu_daughter_pz_->at( p );
      TVector3 temp_p3 = TVector3( px, py, pz );

      mc_p3_p_vec_->push_back( temp_p3 );
      float mom = temp_p3.Mag();
     
     if(mom > energy ) continue; // // Ben saw some weirdness with GiBUU, so lets replicate his checks
     
    if ( pdg == PROTON )
    {
      
       float KE_proton = real_sqrt( std::pow(energy, 2) - std::pow(PROTON_MASS, 2) );
       if(KE_proton >= 0.035 )  mc_Eavail += KE_proton; // using inclusive thresholds for protons // 35 MeV threhold
       if(verbosal > 0) std::cout<<"mc_Eavail = "<< mc_Eavail <<  " KE_proton  = "<< KE_proton<<std::endl;
      if ( mom > max_mom ) {
        max_mom = mom;
        *mc_p3_lead_p_ = temp_p3;
      }
    }
    else if(abs(pdg) == PI_PLUS){
     float KE_pion = real_sqrt( std::pow(energy, 2) - std::pow(PI_PLUS_MASS, 2) );
     
     if(KE_pion >= 0.01) mc_Eavail += KE_pion;  // using inclusive thresholds for pions  // 10 MeV threhold
     if(verbosal > 0) std::cout<<"mc_Eavail = "<< mc_Eavail <<  " KE_pion;  = "<< KE_pion<<std::endl;
      
      if ( mom > max_mom_Pion ) {
        max_mom_Pion = mom;
        *mc_p3_lead_pi_ = temp_p3;
      }  
    }
    else{
         mc_Eavail +=energy;
    } 
    
  }
         
   mc_Eavail_= mc_Eavail;
    if(verbosal > 0) std::cout<<"mc_Eavail_ = "<< mc_Eavail <<  " mc_Eavail = "<< mc_Eavail<<std::endl;
  // TODO: reduce code duplication by just getting the leading proton
  // 3-momentum from this sorted vector
  // Sort the true proton 3-momenta in order from highest to lowest magnitude
  std::sort( mc_p3_p_vec_->begin(), mc_p3_p_vec_->end(), [](const TVector3& a,
    const TVector3& b) -> bool { return a.Mag() > b.Mag(); } );

  mc_distance_FV_surface_ = this->mc_distance_to_FV_Surface(event);


  // If the event contains a leading proton, then set the 3-momentum
  // accordingly
  bool true_lead_p = ( max_mom != LOW_FLOAT );
  if ( !true_lead_p && mc_is_signal_ ) {
    // If it doesn't for a signal event, then something is wrong.
    std::cout << "WARNING: Missing leading proton in MC signal event!\n";
    return;
  }

  // Compute true STVs if the event contains both a muon and a leading
  // proton
  if ( true_muon && true_lead_p ) {
    this->compute_stvs( *mc_p3_mu_, *mc_p3_lead_p_, mc_delta_pT_, mc_delta_phiT_,
      mc_delta_alphaT_, mc_delta_pL_, mc_pn_, mc_delta_pTx_, mc_delta_pTy_ );

    mc_theta_mu_p_ = std::acos( mc_p3_mu_->Dot(*mc_p3_lead_p_)
      / mc_p3_mu_->Mag() / mc_p3_lead_p_->Mag() );
  }

 return;

}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
int JOINTCC0pi::categorize_event(AnalysisEvent* event){

  // Real data has a bogus true neutrino PDG code that is not one of the
  // allowed values (±12, ±14, ±16)
  int abs_mc_nu_pdg = std::abs( event->mc_nu_pdg_ );
  event->is_mc_ = ( abs_mc_nu_pdg == ELECTRON_NEUTRINO
    || abs_mc_nu_pdg == MUON_NEUTRINO || abs_mc_nu_pdg == TAU_NEUTRINO );
  if ( !event->is_mc_ ) return kUnknown;

  if ( !mc_vertex_in_FV_ ) {
    mc_is_signal_ = false;
    mc_is_cc0pi_signal_ = false;
    mc_is_cc0pi_wc_signal_ = false;
    return kOOFV;
  }
  else if ( event->mc_nu_ccnc_ == NEUTRAL_CURRENT ) {
    mc_is_signal_ = false;
    mc_is_cc0pi_signal_ = false;
    mc_is_cc0pi_wc_signal_ = false;
    return kNC;
  }
  else if ( !mc_neutrino_is_numu_ ) {
    mc_is_signal_ = false;
    mc_is_cc0pi_signal_ = false;
    mc_is_cc0pi_wc_signal_ = false;
    if ( event->mc_nu_pdg_ == ELECTRON_NEUTRINO
      && event->mc_nu_ccnc_ == CHARGED_CURRENT ) return kNuECC;
    else return kOther;
  }


  // Sort signal by interaction mode
  if ( mc_is_cc0pi_signal_ ) {
    if ( event->mc_nu_interaction_type_ == 0 ) return kSignalCCQE; // QE
    else if ( event->mc_nu_interaction_type_ == 10 ) return kSignalCCMEC; // MEC
    else if ( event->mc_nu_interaction_type_ == 1 ) return kSignalCCRES; // RES
    //else if ( event->mc_nu_interaction_type_ == 2 ) // DIS
    //else if ( event->mc_nu_interaction_type_ == 3 ) // COH
    else return kSignalOther;
  }
  else if ( !mc_no_fs_pi0_ || !mc_no_charged_pi_above_threshold_ ) {
    return kNuMuCCNpi;
  }
  /*else if ( !mc_lead_p_in_mom_range_ ) {
    return kNuMuCC0pi0p;
  }*/
  else return kNuMuCCOther;

}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
bool JOINTCC0pi::define_signal(AnalysisEvent* event){

  // Set flags to their default values here
  mc_muon_in_mom_range_ = false;
  mc_muon_in_wc_mom_range_ = false;
  mc_muon_in_wc_costheta_threshold_ = false;
  mc_lead_p_in_mom_range_ = false;
  mc_no_fs_pi0_ = true;
  mc_no_charged_pi_above_threshold_ = true;
  mc_no_charged_pi_above_wc_threshold_ = true;
  mc_no_fs_mesons_ = true;
  mc_vertex_in_FV_ = this->mc_vertex_inside_FV(event);
  mc_neutrino_is_numu_ = ( event->mc_nu_pdg_ == MUON_NEUTRINO );


  double lead_p_mom = LOW_FLOAT;

  for ( size_t p = 0u; p < event->mc_nu_daughter_pdg_->size(); ++p ) {
    int pdg = event->mc_nu_daughter_pdg_->at( p );
    float energy = event->mc_nu_daughter_energy_->at( p );

    // Do the general check for (anti)mesons first before considering
    // any individual PDG codes
    if ( is_meson_or_antimeson(pdg) ) {
      mc_no_fs_mesons_ = false;
    }

    // Check that the muon has a momentum within the allowed range
    if ( pdg == MUON ) {
      double mom = real_sqrt( std::pow(energy, 2) - std::pow(MUON_MASS, 2) );
      if ( mom >= MUON_P_MIN_MOM_CUT_jointcc0pi && mom <= MUON_P_MAX_MOM_CUT_jointcc0pi ) {
        mc_muon_in_mom_range_ = true;

      }
      
      if ( mom >= MUON_P_MIN_WC_MOM_CUT_jointcc0pi && mom <= MUON_P_MAX_WC_MOM_CUT_jointcc0pi ) {
        mc_muon_in_wc_mom_range_ = true;
        }
      
      if(mc_p3_mu_->CosTheta() > MUON_Costheta_MIN_WC_CUT_jointcc0pi ){
        mc_muon_in_wc_costheta_threshold_ = true;
      }
      
      
      
      
    }
    else if ( pdg == PROTON ) {
      double mom = real_sqrt( std::pow(energy, 2) - std::pow(PROTON_MASS, 2) );
      if ( mom > lead_p_mom ){
        lead_p_mom = mom;
        mc_num_protons_++;
      }
    }
    else if ( pdg == NEUTRON ) {
      mc_num_neutrons_++;
    }
    else if ( pdg == PI_ZERO ) {
      mc_no_fs_pi0_ = false;
    }
    else if ( std::abs(pdg) == PI_PLUS ) {
      double mom = real_sqrt( std::pow(energy, 2) - std::pow(PI_PLUS_MASS, 2) );
      if ( mom > CHARGED_PI_MOM_CUT_jointcc0pi ) {
        mc_no_charged_pi_above_threshold_ = false;
        mc_num_charged_pions_++;
      }
      if ( mom > CHARGED_PI_WC_MOM_CUT_jointcc0pi ) {
        mc_no_charged_pi_above_wc_threshold_ = false;
        mc_num_wc_charged_pions_++;
      }
    }
  }

  // Check that the leading proton has a momentum within the allowed range
  if ( lead_p_mom >= LEAD_P_MIN_MOM_CUT && lead_p_mom <= LEAD_P_MAX_MOM_CUT ) {
    mc_lead_p_in_mom_range_ = true;
  }

  mc_is_signal_ = mc_vertex_in_FV_ && mc_neutrino_is_numu_
    && mc_muon_in_mom_range_ && mc_lead_p_in_mom_range_
    && mc_no_fs_mesons_;

  mc_is_cc0pi_signal_ = mc_vertex_in_FV_ && mc_neutrino_is_numu_
    && mc_muon_in_mom_range_ && mc_no_fs_pi0_
    && mc_no_charged_pi_above_threshold_;

    if(verbosal > 0){
    
      std::cout << "mc_vertex_in_FV_: " << mc_vertex_in_FV_ << "\n"
      << "mc_neutrino_is_numu_: " << mc_neutrino_is_numu_ << "\n"
      << "mc_muon_in_mom_range_: " << mc_muon_in_mom_range_ << "\n"
      << "mc_no_fs_pi0_: " << mc_no_fs_pi0_ << "\n"
      << "mc_no_charged_pi_above_threshold_: " << mc_no_charged_pi_above_threshold_ << "\n"
      << "mc_is_cc0pi_signal_: " << (mc_vertex_in_FV_ && mc_neutrino_is_numu_
          && mc_muon_in_mom_range_ && mc_no_fs_pi0_
          && mc_no_charged_pi_above_threshold_) << std::endl;
    
    }

  mc_is_cc0pi_wc_signal_ = mc_vertex_in_FV_ && mc_neutrino_is_numu_
    && mc_muon_in_wc_mom_range_ && mc_no_fs_pi0_
    && mc_no_charged_pi_above_wc_threshold_
    && mc_muon_in_wc_costheta_threshold_;

  //std::cout << "DEBUG : " << __FILE__ << "  "  << __LINE__ << "  " << mc_is_cc0pi_signal_ << std::endl;

  return mc_is_cc0pi_signal_;
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
bool JOINTCC0pi::selection(AnalysisEvent* event){

  int reco_shower_count = 0;
   if(verbosal > 0) std::cout<< "Starting loop "<<std::endl;
   
   if (0 == event->pfp_generation_->size()) return false;  // I thhink this will help the full loop 
   
  // std::cout<< "event->num_pf_particles_ "<< event->num_pf_particles_<<" event->pfp_generation_.size()"<< event->pfp_generation_->size()<<std::endl;
  for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {

    // Only check direct neutrino daughters (generation == 2)
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2u ) continue;

    float tscore = event->pfp_track_score_->at( p );
    if ( tscore <= TRACK_SCORE_CUT ) ++reco_shower_count;
  }

 if(verbosal > 0) std::cout<< "Finished loop "<<std::endl;
  // Check the shower cut
  sel_no_reco_showers_ = ( reco_shower_count == 0 );

  /////////////////////////////
  // Apply Numu CC Selection
  ///////////////////////////
 this->apply_numu_CC_selection(event);

 if(verbosal > 0) std::cout<< "Finished apply_numu_CC_selection"<<std::endl;

  // Set flags that default to true here
  sel_passed_proton_pid_cut_ = true;
  sel_protons_contained_ = true;

  // Set flags that default to false here
  sel_muon_passed_mom_cuts_ = false;
  sel_muon_passed_wc_mom_cuts_ = false;
  sel_muon_passed_wc_costheta_ = false;
  sel_muon_contained_ = false;
  sel_has_pion_candidate_ = false;
  sel_has_pion_wc_candidate_ = false;

  sel_num_proton_candidates_ = 0;
  sel_num_pion_candidates_ = 0;
  sel_num_pion_wc_candidates_ = 0;
  float Eavail = 0.0;
   if(verbosal > 0) std::cout<< "Eavail  = "<< Eavail<<std::endl;

  if(verbosal > 0)std::cout<< "Starting loop Line 520 "<<std::endl;
 /*event->num_pf_particles_*/
  for ( int p = 0; p < event->pfp_generation_->size(); ++p ) { 
      // Only make predictions for events passing the BDT pre-selection cuts
      if (!sel_presel_) continue;
      if(verbosal > 0) std::cout<<"p  = "<< p<< std::endl;

     // Only worry about direct neutrino daughters (PFParticles considered
     // daughters of the reconstructed neutrino)     
    float track_score =  event->pfp_track_score_->at( p );
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2 || track_score <= TRACK_SCORE_CUT_jointcc0pi ) continue;
     
        
     // Check that we can find a muon candidate in the event. If more than
     // one is found, also fail the cut.
    if ( p == muon_candidate_idx_ ) {
     if(verbosal > 0) std::cout<<" muon_candidate_idx_  "<<muon_candidate_idx_ << std::endl;
      // Check whether the muon candidate is contained. Use the same
      // containment volume as the protons. TODO: revisit this as needed.
      float endx = event->track_endx_->at( p );
      float endy = event->track_endy_->at( p );
      float endz = event->track_endz_->at( p );
      bool end_contained = this->in_proton_containment_vol( endx, endy, endz );
      if ( end_contained ) sel_muon_contained_ = true;

      auto& p3mu = *p3_mu_;
      float mu_dirx = event->track_dirx_->at( muon_candidate_idx_ );
      float mu_diry = event->track_diry_->at( muon_candidate_idx_ );
      float mu_dirz = event->track_dirz_->at( muon_candidate_idx_ );
       p3mu = TVector3( mu_dirx, mu_diry, mu_dirz );
       double angle = p3mu.CosTheta();
      float muon_mom = LOW_FLOAT;
      float range_muon_mom = event->track_range_mom_mu_->at( p );
      float mcs_muon_mom = event->track_mcs_mom_mu_->at( p );

      ///////No Corrections////////
      /*if ( sel_muon_contained_ ){
         muon_mom = range_muon_mom;
      }
      else {
         muon_mom = mcs_muon_mom;
      }*/

      if ( sel_muon_contained_ ){

           if (event->track_length_->size() == 1 && event->trk_bragg_mu_fwd_preferred_v_->at(muon_candidate_idx_) == 0 && 6. < event->track_chi2_muon_->at(muon_candidate_idx_)){
            //std::cout<<"Rejecting flipped tracks"<<std::endl;
            muon_mom = LOW_FLOAT;
           }

     else{
          muon_mom = range_muon_mom;
          }
      }
      else{
          if(p3mu.CosTheta() > -0.9){

                if (mcs_muon_mom < 0.11){
                    muon_mom = LOW_FLOAT;
                }
                else{
                    muon_mom = mcs_muon_mom - 0.0361*mcs_muon_mom + 0.04;
                    }
          }
          else{
                muon_mom = LOW_FLOAT;
              }
        }
      if ( muon_mom >= MUON_P_MIN_MOM_CUT_jointcc0pi && muon_mom <= MUON_P_MAX_MOM_CUT_jointcc0pi ) {
        sel_muon_passed_mom_cuts_ = true;
      }

      if ( muon_mom >= MUON_P_MIN_WC_MOM_CUT_jointcc0pi && muon_mom <= MUON_P_MAX_WC_MOM_CUT_jointcc0pi ) {
        sel_muon_passed_wc_mom_cuts_ = true;
      }
     
      if (p3mu.CosTheta() > MUON_Costheta_MIN_WC_CUT_jointcc0pi){
      sel_muon_passed_wc_costheta_ = true; 
      }

      // Apply muon candidate quality cut by comparing MCS and range-based
      // momentum estimators. Default to failing the cut.
      sel_muon_quality_ok_ = false;

      double frac_diff_range_mcs = std::abs( range_muon_mom - mcs_muon_mom );
      if ( range_muon_mom > 0. ) {
        frac_diff_range_mcs /= range_muon_mom;
        if ( frac_diff_range_mcs < MUON_MOM_QUALITY_CUT ) {
          sel_muon_quality_ok_ = true;
        }
      }
    }
    // Pion candidate
    else if (xgb_pid_vec_->at(p) == (int) BDTClass::kMu ||
             xgb_pid_vec_->at(p) == (int) BDTClass::kPi) {

      float track_length = event->track_length_->at( p );
      if ( track_length <= 0. ) continue;

      float endx = event->track_endx_->at( p );
      float endy = event->track_endy_->at( p );
      float endz = event->track_endz_->at( p );
      bool end_contained = this->in_proton_containment_vol( endx, endy, endz );

      float pi_mom;
      if ( end_contained ) {
        pi_mom = event->track_range_mom_mu_->at(p);
      }
      else {
        pi_mom = event->track_mcs_mom_mu_->at(p);
      }
      
      float output_Eavail = event->track_kinetic_energy_p_->at( p );
      Eavail +=  output_Eavail;
   
     if(verbosal > 0) std::cout<<"output_Evvai(pion) l= "<< output_Eavail <<  " Eavail  = "<< Eavail<<std::endl;

      if (pi_mom > CHARGED_PI_MOM_CUT_jointcc0pi) {
        sel_num_pion_candidates_++;
        sel_has_pion_candidate_ = true;
      }

      if (pi_mom > CHARGED_PI_WC_MOM_CUT_jointcc0pi) {
        sel_num_pion_wc_candidates_++;
        sel_has_pion_wc_candidate_ = true;
      }
    }

    else if (xgb_pid_vec_->at(p) == (int) BDTClass::kP) {
      float track_length = event->track_length_->at( p );
      if ( track_length <= 0. ) continue;

      sel_has_p_candidate_ = true;
      sel_passed_proton_pid_cut_ = true;
      sel_num_proton_candidates_++;

      // Check whether the current proton candidate fails the containment cut
      float endx = event->track_endx_->at( p );
      float endy = event->track_endy_->at( p );
      float endz = event->track_endz_->at( p );
      bool end_contained = this->in_proton_containment_vol( endx, endy, endz );
      if ( !end_contained ) sel_protons_contained_ = false;
      float output_Eavail = event->track_kinetic_energy_p_->at( p );
      Eavail +=  output_Eavail;
   
     if(verbosal > 0) std::cout<<"output_Eavail(kP)=  "<< output_Eavail <<  " Eavail  = "<< Eavail<<std::endl;
      
      
    }

    else if (xgb_pid_vec_->at(p) == (int) BDTClass::kOther) {    
     float track_length = event->track_length_->at( p );
      if ( track_length <= 0. ) continue;
      float output_Eavail = event->track_kinetic_energy_p_->at( p );
      Eavail +=  output_Eavail;
   
     if(verbosal > 0) std::cout<<"output_Eavail (Other)= "<< output_Eavail <<  " Eavail  = "<< Eavail<<std::endl; 
    }
    else if (xgb_pid_vec_->at(p) == (int) BDTClass::kInvalid) {}
    else {
      // hmm
    }
  }
  if(verbosal > 0) std::cout<< "Finished loop:line1945 "<<std::endl;
        

   
    
  Eavail_= Eavail;
   if(verbosal > 0) std::cout<<"Eavail_ = "<< Eavail_ <<  " Eavail  = "<< Eavail<<std::endl;
  
  sel_CC0pi_ = sel_nu_mu_cc_ && sel_no_reco_showers_
    && sel_muon_passed_mom_cuts_ && !sel_has_pion_candidate_
    && sel_n_bdt_other_ == 0 && sel_n_bdt_invalid_ == 0;

  sel_CC0pi_wc_ = sel_nu_mu_cc_ && sel_no_reco_showers_
    && sel_muon_passed_wc_mom_cuts_ && sel_muon_passed_wc_costheta_
    && !sel_has_pion_wc_candidate_
    && sel_n_bdt_other_ == 0 && sel_n_bdt_invalid_ == 0;
  // Don't bother to apply the cuts that involve the leading
  // proton candidate if we don't have one
  if ( !sel_has_p_candidate_ ) {
    sel_CCNp0pi_ = false;
  }
  else {
    // All that remains is to apply the leading proton candidate cuts. We could
    // search for it above, but doing it here makes the code more readable (with
    // likely negligible impact on performance)
    if(verbosal > 0) std::cout<<" find_lead_p_candidate  "<<std::endl;
    if(sel_num_proton_candidates_>0) this->find_lead_p_candidate(event);

    // Check the range-based reco momentum for the leading proton candidate
    float lead_p_KE = event->track_kinetic_energy_p_->at( lead_p_candidate_idx_ );
    float range_mom_lead_p = real_sqrt( lead_p_KE*lead_p_KE
      + 2.*PROTON_MASS*lead_p_KE );
    if ( range_mom_lead_p >= LEAD_P_MIN_MOM_CUT
      && range_mom_lead_p <= LEAD_P_MAX_MOM_CUT )
    {
      sel_lead_p_passed_mom_cuts_ = true;
    }

    // All right, we've applied all selection cuts. Set the flag that indicates
    // whether all were passed (and thus the event is selected as a CCNp0pi
    // candidate)
    sel_CCNp0pi_ = sel_nu_mu_cc_ && sel_no_reco_showers_
      && sel_muon_passed_mom_cuts_ && sel_muon_contained_ && sel_muon_quality_ok_
      && sel_has_p_candidate_ && sel_passed_proton_pid_cut_
      && sel_protons_contained_ && sel_lead_p_passed_mom_cuts_;
  }

 return sel_CC0pi_;
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::define_output_branches(){

  // Signal definition flags

  set_branch( &mc_neutrino_is_numu_, "mc_neutrino_is_numu" );
  set_branch( &mc_vertex_in_FV_, "mc_vertex_in_FV" );
  set_branch( &mc_muon_in_mom_range_, "mc_muon_in_mom_range" );
  set_branch( &mc_lead_p_in_mom_range_, "mc_lead_p_in_mom_range" );
  set_branch( &mc_no_fs_pi0_, "mc_no_fs_pi0" );
  set_branch( &mc_no_charged_pi_above_threshold_, "mc_no_charged_pi_above_threshold" );

  set_branch( &mc_num_protons_, "mc_num_protons" );
  set_branch( &mc_num_neutrons_, "mc_num_neutrons" );
  set_branch( &mc_num_charged_pions_, "mc_num_charged_pions" );
  set_branch( &mc_num_wc_charged_pions_, "mc_num_wc_charged_pions" );


  set_branch( &mc_no_fs_mesons_, "mc_no_fs_mesons" );
  set_branch( &mc_is_signal_, "mc_is_signal" );
  set_branch( &mc_is_cc0pi_signal_, "mc_is_cc0pi_signal" );
  set_branch( &mc_is_cc0pi_wc_signal_, "mc_is_cc0pi_wc_signal" );

  // MC event category
   set_branch( &sel_nu_mu_cc_, "sel_nu_mu_cc" );
   set_branch( &sel_reco_vertex_in_FV_, "sel_reco_vertex_in_FV" );
   set_branch( &sel_topo_cut_passed_, "sel_topo_cut_passed" );
   set_branch( &sel_cosmic_ip_cut_passed_, "sel_cosmic_ip_cut_passed" );
   set_branch( &sel_pfp_starts_in_PCV_, "sel_pfp_starts_in_PCV" );
   set_branch( &sel_no_reco_showers_, "sel_no_reco_showers" );
   set_branch( &sel_has_muon_candidate_, "sel_has_muon_candidate" );
   set_branch( &sel_muon_contained_, "sel_muon_contained" );
   set_branch( &sel_muon_passed_mom_cuts_, "sel_muon_passed_mom_cuts" );
   set_branch( &sel_muon_quality_ok_, "sel_muon_quality_ok" );
   set_branch( &sel_has_p_candidate_, "sel_has_p_candidate" );
   set_branch( &sel_passed_proton_pid_cut_, "sel_passed_proton_pid_cut" );
   set_branch( &sel_protons_contained_, "sel_protons_contained" );
   set_branch( &sel_lead_p_passed_mom_cuts_, "sel_lead_p_passed_mom_cuts" );
   set_branch( &sel_CCNp0pi_, "sel_CCNp0pi" );
   set_branch( &sel_presel_, "sel_presel" );
   set_branch( &sel_has_pion_candidate_, "sel_has_pion_candidate" );
   set_branch( &sel_CC0pi_, "sel_CC0pi" );
   set_branch( &sel_CC0pi_wc_, "sel_CC0pi_wc" );

  // Still on the fence if I should make the WC conditions as a different selection
  set_branch( &sel_muon_passed_wc_mom_cuts_, "sel_muon_passed_wc_mom_cuts" );
  set_branch( &sel_muon_passed_wc_costheta_, "sel_muon_passed_wc_costheta" );
  set_branch( &sel_CCNp0pi_wc_, "sel_CCNp0pi_wc" );
  set_branch( &sel_has_pion_wc_candidate_, "sel_has_pion_wc_candidate" );
  set_branch( &mc_muon_in_wc_mom_range_, "mc_muon_in_wc_mom_range" );
  set_branch( &mc_muon_in_wc_costheta_threshold_, "mc_muon_in_wc_costheta_threshold" );
  set_branch( &mc_no_charged_pi_above_wc_threshold_,
    "mc_no_charged_pi_above_wc_threshold" );




  // XGBoost outputs

  set_branch( &sel_n_bdt_other_, "sel_n_bdt_other" );
  set_branch( &sel_n_bdt_muon_, "sel_n_bdt_muon" );
  set_branch( &sel_n_bdt_pion_, "sel_n_bdt_pion" );
  set_branch( &sel_n_bdt_proton_, "sel_n_bdt_proton" );
  set_branch( &sel_n_bdt_invalid_, "sel_n_bdt_invalid" );


  // CC0pi
  set_branch( &sel_num_proton_candidates_, "sel_num_proton_candidates" );
  set_branch( &sel_num_pion_candidates_, "sel_num_pion_candidates" );
  set_branch( &sel_num_pion_wc_candidates_, "sel_num_pion_wc_candidates" );
  set_branch( &muon_candidate_idx_, "muon_candidate_idx" );
  set_branch( &lead_p_candidate_idx_, "lead_p_candidate_idx" );


  // Index for the leading proton candidate in the vectors of PFParticles

   set_branch( &distance_FV_surface_, "distance_FV_surface" );
   set_branch( &muontrklen_, "muontrklen" );
   set_branch( &muondistancetovectex_, "muondistancetovectex" );
   set_branch( &muon_llr_pid_score_, "muon_llr_pid_score" );
   set_branch( &muon_tkscore_, "muon_tkscore_" );

   set_branch( &muon_trkend_x_, "muon_trkend_x" );
   set_branch( &muon_trkend_y_, "muon_trkend_y" );
   set_branch( &muon_trkend_z_, "muon_trkend_z" );

   set_branch( &muon_trkstart_x_, "muon_trkstart_x" );
   set_branch( &muon_trkstart_y_, "muon_trkstart_y" );
   set_branch( &muon_trkstart_z_, "muon_trkstart_z" );
   set_branch( &muon_trkchi2muon_, "muon_trkchi2muon");
   set_branch( &muon_BDTScore_,    "muon_bdtscore");
   set_branch( &lead_p_BDTScore_,  "lead_p_bdtscore");
   set_branch( &Eavail_,       "Eavail" );
   
   
  

   set_branch( &PanelProjection_X_Z_, "PanelProjection_X_Z" );
   set_branch( &PanelProjection_Y_Z_, "PanelProjection_Y_Z" );
   set_branch( &PanelProjection_X_Y_, "PanelProjection_X_Y" );
   set_branch( &sel_PanelClosesttoEndMuonTrk_, "sel_PanelClosesttoEndMuonTrk" );
   set_branch( &sel_PanelClosestSubPaneltoEndMuonTrk_, "sel_PanelClosestSubPaneltoEndMuonTrk" );

  // Reco 3-momenta (muon, leading proton)

     set_branch( p3_mu_, "p3_mu" );
     
     set_branch( p3_mu_mcs_, "p3mu_mcs" );
     set_branch( p3_mu_range_, "p3mu_range" );
     set_branch( p3_lead_p_, "p3_lead_p" );
     set_branch( p3_lead_pi_, "p3_lead_pi" );

  // Reco 3-momenta (all proton candidates, ordered from highest to lowest
  // magnitude)

    set_branch( p3_p_vec_, "p3_p_vec" );
    set_branch( mc_p3_mu_, "mc_p3_mu" );

    set_branch( mc_p3_lead_p_, "mc_p3_lead_p" );
    set_branch( mc_p3_lead_pi_, "mc_p3_lead_pi" );
    set_branch( mc_p3_p_vec_, "mc_p3_p_vec" );
  // True 3-momenta (muon, leading proton)

    set_branch( &mc_muontrklen_, "mc_muontrklen" );
    set_branch( &mc_Eavail_,       "mc_Eavail" );

  // True 3-momenta (all protons, ordered from highest to lowest magnitude)


  // XGBoost scores
   set_branch( xgb_pid_vec_, "xgb_pid_vec" );
   set_branch( xgb_score_vec_, "xgb_score_vec" );

  // Reco STVs
   set_branch( &delta_pT_, "delta_pT" );
   set_branch( &delta_phiT_, "delta_phiT" );
   set_branch( &delta_alphaT_, "delta_alphaT" );
   set_branch( &delta_pL_, "delta_pL" );
   set_branch( &pn_, "pn" );
   set_branch( &delta_pTx_, "delta_pTx" );

   set_branch( &delta_pTy_, "delta_pTy" );
   set_branch( &theta_mu_p_, "theta_mu_p" );
     // MC STVs (only filled for signal events)
   set_branch( &mc_delta_pT_, "mc_delta_pT" );
   set_branch( &mc_delta_phiT_, "mc_delta_phiT" );
   set_branch( &mc_delta_alphaT_, "mc_delta_alphaT" );
   set_branch( &mc_delta_pL_, "mc_delta_pL" );
   set_branch( &mc_pn_, "mc_pn" );
   set_branch( &mc_delta_pTx_, "mc_delta_pTx" );
   set_branch( &mc_delta_pTy_, "mc_delta_pTy" );
   set_branch( &mc_theta_mu_p_, "mc_theta_mu_p" );
   set_branch( &mc_distance_FV_surface_, "mc_distance_FV_surface" );
  // *** Branches copied directly from the input ***

  // Cosmic rejection parameters for numu CC inclusive selection

 // MC truth information for the neutrino



  // Log-likelihood-based particle ID information
  // MC truth information for the final-state primary particles



}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::define_category_map(){
  // Use the shared category map for 1p/2p/Np/Xp
  categ_map_ = JOINTCC0Pi_MAP;
  std::cout<<"Finished :: DefineCategoryMap "<< std::endl;
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::classify_tracks(AnalysisEvent* event){
if(verbosal > 0) std::cout<<" inside ::classify_tracks  "<< std::endl;
  xgb_pid_vec_->clear();
  xgb_pid_vec_->resize(event->pfp_generation_->size(), (int) BDTClass::kInvalid);

  xgb_score_vec_->clear();
  xgb_score_vec_->resize(event->pfp_generation_->size());

  sel_n_bdt_other_ = 0;
  sel_n_bdt_muon_ = 0;
  sel_n_bdt_pion_ = 0;
  sel_n_bdt_proton_ = 0;
  sel_n_bdt_invalid_ = 0;

  // Only make predictions for events passing the BDT pre-selection cuts
  if (!sel_presel_) return;

  // Classify all FS objects
  for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {
    // Only consider generation 2 tracks
    float track_score =  event->pfp_track_score_->at( p );
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2 || track_score <= TRACK_SCORE_CUT_jointcc0pi ) continue;

    // Observables vector
    float pmcs = event->track_mcs_mom_mu_->at(p);
    float prange = event->track_range_mom_mu_->at(p);
    float trk_relative_dif_mcs_range = (pmcs - prange) / prange;

    float vx = event->track_endx_->at(p);
    float vy = event->track_endy_->at(p);
    float vz = event->track_endz_->at(p);
    
    
    //std::cout<<" vx = "<< vx<<" vy = "<< vy<<" vz = "<< vz<<std::endl;
    bool trk_contained = point_inside_FV( this->reco_FV(), vx, vy, vz );

    unsigned int num_daughters = event->pfp_trk_daughters_count_->at(p) +
                                 event->pfp_shr_daughters_count_->at(p);

    float fs_v[] = {
      event->track_start_distance_->at(p),
      event->track_llr_pid_score_->at(p),
      event->track_chi2_proton_->at(p),
      event->track_kinetic_energy_p_->at(p),
      (float) trk_contained,
      (float) num_daughters,
      trk_relative_dif_mcs_range
    };

    // Check for inf/nans
    bool fs_v_ok = true;
    for (size_t i=0; i<7; i++) {
      if (std::isnan(fs_v[i]) || std::isinf(fs_v[i])) {
        fs_v_ok = false;
        break;
      }
    }

    if (fs_v_ok) {
      int r;  // XGBoost return codes

      // Create input DMatrix
      DMatrixHandle dmat;
      r = XGDMatrixCreateFromMat(fs_v, 1, 7, -1, &dmat);
      if (r != 0) std::cout << XGBGetLastError() << std::endl;
      assert(r == 0);

      // Load trained model from JSON
      const char* c_json_config = "{\"type\": 0,\"training\": false,\"iteration_begin\": 0,\"iteration_end\": 0,\"strict_shape\": false}";
      const unsigned long int* out_shape;
      unsigned long int out_dim;
      const float* out_result;

      r = XGBoosterPredictFromDMatrix(*booster, dmat, c_json_config,
                                      &out_shape, &out_dim, &out_result);
      if (r != 0) std::cout << XGBGetLastError() << std::endl;
      assert(r == 0);

      XGDMatrixFree(dmat);

      int idx_max = (int) BDTClass::kInvalid;
      float v_max = -1;

      //for (int k=0; k<out_dim; k++) {
      for (int k=0; k < 4; k++) {
      if(verbosal > 0) std::cout<<" inside ::classify_tracks::filling::xgb_score_vec_ : k =  "<< k <<  "out_result[k] = "<< out_result[k]<< std::endl;
          xgb_score_vec_->at(p).push_back(out_result[k]);
          
          if (out_result[k] > v_max) {
             v_max = out_result[k];
             idx_max = k;
          }
       }
       
      xgb_pid_vec_->at(p) = idx_max;

      switch ((BDTClass) idx_max) {
        case BDTClass::kOther: sel_n_bdt_other_++; break;
        case BDTClass::kMu: sel_n_bdt_muon_++; break;
        case BDTClass::kPi: sel_n_bdt_pion_++; break;
        case BDTClass::kP: sel_n_bdt_proton_++; break;
        case BDTClass::kInvalid: sel_n_bdt_invalid_++; break;
      }
    }
  }
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::find_muon_candidate(AnalysisEvent* event){

  if(verbosal > 0) std::cout<<" inside ::find_muon_candidate  "<< std::endl;
  std::vector<int> muon_candidate_indices;
  std::vector<int> muon_pid_scores;

  if (sel_presel_) {
    for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {
      // Only direct neutrino daughters (generation == 2) will be considered as
      // possible muon candidates
    float track_score =  event->pfp_track_score_->at( p );
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2 || track_score <= TRACK_SCORE_CUT_jointcc0pi ) continue;

      if (xgb_pid_vec_->at(p) == (int) BDTClass::kMu) {
        muon_candidate_indices.push_back( p );
        muon_pid_scores.push_back( xgb_score_vec_->at(p)[1] );
        muon_BDTScore_ = xgb_score_vec_->at(p)[1];
      }
    }
  }

  size_t num_candidates = muon_candidate_indices.size();
  if ( num_candidates > 0u ) sel_has_muon_candidate_ = true;

  if ( num_candidates == 1u ) {
    muon_candidate_idx_ = muon_candidate_indices.front();
    
  }
  else if ( num_candidates > 1u ) {
    // In the case of multiple muon candidates, choose the one with the highest
    // PID score (most muon-like) as the one to use
    float highest_score = LOW_FLOAT;
    int chosen_index = BOGUS_INDEX;
    for ( size_t c = 0; c < num_candidates; ++c ) {
      float score = muon_pid_scores.at( c );
      if ( highest_score < score ) {
        highest_score = score;
        chosen_index = muon_candidate_indices.at( c );
        muon_BDTScore_ = score;
      }
    }
    
    muon_candidate_idx_ = chosen_index;
  }
  else {
    muon_candidate_idx_ = BOGUS_INDEX;
  }






}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::find_lead_p_candidate(AnalysisEvent* event){
  
  // Only make predictions for events passing the BDT pre-selection cuts
  if (!sel_presel_) return;
  
  float lead_p_track_length = LOW_FLOAT;
  size_t lead_p_index = 0u;

  
  for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {
    

    // Only check direct neutrino daughters (generation == 2)
     // Skip PFParticles that are shower-like (track scores near 0)
    float track_score =  event->pfp_track_score_->at( p );
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2 || track_score <= TRACK_SCORE_CUT_jointcc0pi ) continue;

    // Skip the muon candidate reco track (this function assumes that it has
    // already been found)
    if ( p == muon_candidate_idx_ ) continue;


    // All non-muon-candidate reco tracks are considered proton candidates
    float track_length = event->track_length_->at( p );
    if ( track_length <= 0. ) continue;
    if(xgb_score_vec_->at(p).size()==0)continue; // if size zero no perdiction for this particle , skip then 
         if(verbosal > 0) std::cout<< "  xgb_score_vec_->at(p).size() = "<< xgb_score_vec_->at(p).size() << std::endl;
     if(verbosal > 0) std::cout<<" p =  "<< p <<  " xgb_score_vec_->at(p)[3] "<<xgb_score_vec_->at(p)[3] << std::endl;
    if ( track_length > lead_p_track_length  && xgb_score_vec_->at(p).size() >3 ) {
      lead_p_track_length = track_length;
      lead_p_index = p;
       lead_p_BDTScore_ = xgb_score_vec_->at(p)[3];
    }
  }

  // If the leading proton track length changed from its initial
  // value, then we found one. Set the index appropriately.
  if ( lead_p_track_length != LOW_FLOAT ) lead_p_candidate_idx_ = lead_p_index;
  // Otherwise, set the index to BOGUS_INDEX
  else lead_p_candidate_idx_ = BOGUS_INDEX;
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::compute_stvs( const TVector3& p3mu,
   const TVector3& p3p, float& delta_pT,
  float& delta_phiT, float& delta_alphaT,
  float& delta_pL, float& pn,
  float& delta_pTx, float& delta_pTy )
{
  delta_pT = (p3mu + p3p).Perp();

  delta_phiT = std::acos( (-p3mu.X()*p3p.X() - p3mu.Y()*p3p.Y())
    / (p3mu.XYvector().Mod() * p3p.XYvector().Mod()) );

  TVector2 delta_pT_vec = (p3mu + p3p).XYvector();
  delta_alphaT = std::acos( (-p3mu.X()*delta_pT_vec.X()
    - p3mu.Y()*delta_pT_vec.Y())
    / (p3mu.XYvector().Mod() * delta_pT_vec.Mod()) );

  float Emu = std::sqrt(std::pow(MUON_MASS, 2) + p3mu.Mag2());
  float Ep = std::sqrt(std::pow(PROTON_MASS, 2) + p3p.Mag2());
  float R = TARGET_MASS + p3mu.Z() + p3p.Z() - Emu - Ep;

  // Estimated mass of the final remnant nucleus (CCQE assumption)
  float mf = TARGET_MASS - NEUTRON_MASS + BINDING_ENERGY;
  delta_pL = 0.5*R - (std::pow(mf, 2) + std::pow(delta_pT, 2)) / (2.*R);

  pn = std::sqrt( std::pow(delta_pL, 2) + std::pow(delta_pT, 2) );

  // Components of the 2D delta_pT vector (see arXiv:1910.08658)

  // We assume that the neutrino travels along the +z direction (also done
  // in the other expressions above)
  TVector3 zUnit( 0., 0., 1. );

  // Defines the x direction for the components of the delta_pT vector
  TVector2 xTUnit = zUnit.Cross( p3mu ).XYvector().Unit();

  delta_pTx = xTUnit.X()*delta_pT_vec.X() + xTUnit.Y()*delta_pT_vec.Y();

  // Defines the y direction for the components of the delta_T vector
  TVector2 yTUnit = ( -p3mu ).XYvector().Unit();

  delta_pTy = yTUnit.X()*delta_pT_vec.X() + yTUnit.Y()*delta_pT_vec.Y();
}
////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
float JOINTCC0pi::point_distance_to_FV( float x, float y, float z )
{
    double FV_X_MIN =   21.5;
    double FV_X_MAX =  234.85;

    double FV_Y_MIN = -95.0;
    double FV_Y_MAX =  95.0;

    double FV_Z_MIN =   21.5;
    double FV_Z_MAX =  966.8;
    // Distances to the surfaces along each axis
    float dist_to_x_min = x - FV_X_MIN;
    float dist_to_x_max = FV_X_MAX - x;
    float dist_to_y_min = y - FV_Y_MIN;
    float dist_to_y_max = FV_Y_MAX - y;
    float dist_to_z_min = z - FV_Z_MIN;
    float dist_to_z_max = FV_Z_MAX - z;

    // Find the minimum distance to any surface
    float min_distance = std::min({
        dist_to_x_min, dist_to_x_max,
        dist_to_y_min, dist_to_y_max,
        dist_to_z_min, dist_to_z_max
    });

    return min_distance;
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::apply_numu_CC_selection(AnalysisEvent* event)
{   
  if (0 == event->pfp_generation_->size()) return; 
    if(verbosal > 0) std::cout<<" inside ::apply_numu_CC_selection  "<< std::endl;
  sel_reco_vertex_in_FV_ = this->reco_vertex_inside_FV(event);
  sel_topo_cut_passed_ = event->topological_score_ > TOPO_SCORE_CUT_jointcc0pi;
  sel_cosmic_ip_cut_passed_ = event->cosmic_impact_parameter_ > COSMIC_IP_CUT_jointcc0pi;

  // Apply the containment cut to the starting positions of all
  // reconstructed tracks and showers. Pass this cut by default.
  sel_pfp_starts_in_PCV_ = true;

  // Loop over each PFParticle in the event
  for ( int p = 0; p < event->pfp_generation_->size(); ++p ) {

    // Only check direct neutrino daughters (generation == 2)
    unsigned int generation = event->pfp_generation_->at( p );
    if ( generation != 2) continue;

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
    
    sel_pfp_starts_in_PCV_ &= this->in_proton_containment_vol( x, y, z );
  }

  //sel_presel_ = (event->nslice_ == 1 &&
  //               sel_reco_vertex_in_FV_ &&
  //               sel_pfp_starts_in_PCV_ &&
  //               sel_topo_cut_passed_ &&
  //               sel_no_reco_showers_);
  
 /// Changed for mcc10
  sel_presel_ = (event->slice_orig_pass_id_ == 1 &&
                 sel_reco_vertex_in_FV_ &&
                 sel_pfp_starts_in_PCV_ &&
                 sel_topo_cut_passed_ &&
                 sel_no_reco_showers_);
                 
                 
               

                 if(verbosal > 0){std::cout << "event->nslice_: " << event->nslice_ << "\n"
  << "sel_reco_vertex_in_FV_: " << sel_reco_vertex_in_FV_ << "\n"
  << "sel_pfp_starts_in_PCV_: " << sel_pfp_starts_in_PCV_ << "\n"
  << "sel_topo_cut_passed_: " << sel_topo_cut_passed_ << "\n"
  << "sel_no_reco_showers_: " << sel_no_reco_showers_ << "\n"
  << "sel_presel_: " << (event->nslice_ == 1 &&
                         sel_reco_vertex_in_FV_ &&
                         sel_pfp_starts_in_PCV_ &&
                         sel_topo_cut_passed_ &&
                         sel_no_reco_showers_) << std::endl;}


  // Apply BDT to classify final state tracks
  this->classify_tracks(event);

  // Sets the sel_has_muon_candidate_ flag as appropriate. The threshold check
  // is handled later.
  this->find_muon_candidate(event);

  sel_nu_mu_cc_ = sel_presel_ && sel_has_muon_candidate_;
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////

bool JOINTCC0pi::reco_vertex_inside_FV(AnalysisEvent* event) {
  if(verbosal > 0){
     std::cout<<"Inside::::reco_vertex_inside_FV"<< std::endl;
     std::cout << "nu_vx: " << event->nu_vx_
    << ", nu_vy: " << event->nu_vy_
    << ", nu_vz: " << event->nu_vz_ << std::endl;}
      return point_inside_FV( this->reco_FV(),event->nu_vx_, event->nu_vy_, event->nu_vz_ );
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
float JOINTCC0pi::reco_distance_to_FV_Surface(AnalysisEvent* event){
   return point_distance_to_FV( event->nu_vx_, event->nu_vy_, event->nu_vz_  );
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////

bool JOINTCC0pi::mc_vertex_inside_FV(AnalysisEvent* event) {
  if(verbosal > 0){
    std::cout<<"Inside::::mc_vertex_inside_FV"<< std::endl;
    std::cout << "mc_nu_vx: " << event->mc_nu_vx_
   << ", mc_nu_vy: " << event->mc_nu_vy_
   << ", mc_nu_vz: " << event->mc_nu_vz_ << std::endl;}

      return point_inside_FV( this->true_FV(),event->mc_nu_vx_, event->mc_nu_vy_, event->mc_nu_vz_ );
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
float JOINTCC0pi::mc_distance_to_FV_Surface(AnalysisEvent* event){
       return point_distance_to_FV( event->mc_nu_vx_, event->mc_nu_vy_, event->mc_nu_vz_  );
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
bool JOINTCC0pi::in_proton_containment_vol( float x, float y, float z ) {
      bool x_inside_PCV = ( PCV_X_MIN < x ) && ( x < PCV_X_MAX );
      bool y_inside_PCV = ( PCV_Y_MIN < y ) && ( y < PCV_Y_MAX );
      bool z_inside_PCV = ( PCV_Z_MIN < z ) && ( z < PCV_Z_MAX );
      return ( x_inside_PCV && y_inside_PCV && z_inside_PCV );
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
float JOINTCC0pi::distanceBetweentwopoint(float x1, float y1, float z1,
  float x2, float y2, float z2)
{
    // Calculate the differences in each coordinate
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    // Calculate the Euclidean distance
    return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
}

void JOINTCC0pi::reset() {

  mc_neutrino_is_numu_ = false;
  mc_vertex_in_FV_ = false;
  mc_muon_in_mom_range_ = false;
  mc_lead_p_in_mom_range_ = false;
  mc_no_fs_pi0_ = false;
  mc_no_charged_pi_above_threshold_ = false;

  mc_num_protons_ = BOGUS_INDEX;
  mc_num_neutrons_ = BOGUS_INDEX;
  mc_num_charged_pions_ = BOGUS_INDEX;
  mc_num_wc_charged_pions_ = BOGUS_INDEX;

  mc_no_fs_mesons_ = false;
  mc_is_signal_ = false;
  mc_is_cc0pi_signal_ = false;
  mc_is_cc0pi_wc_signal_ = false;

  sel_nu_mu_cc_ = false;
  sel_reco_vertex_in_FV_ = false;
  sel_topo_cut_passed_ = false;
  sel_cosmic_ip_cut_passed_ = false;
  sel_pfp_starts_in_PCV_ = false;
  sel_no_reco_showers_ = false;
  sel_has_muon_candidate_ = false;
  sel_muon_contained_ = false;
  sel_muon_passed_mom_cuts_ = false;
  sel_muon_quality_ok_ = false;
  sel_has_p_candidate_ = false;
  sel_passed_proton_pid_cut_ = false;
  sel_protons_contained_ = false;
  sel_lead_p_passed_mom_cuts_ = false;
  sel_CCNp0pi_ = false;
  sel_presel_ = false;
  sel_has_pion_candidate_ = false;
  sel_CC0pi_ = false;
  sel_CC0pi_wc_ = false;

  sel_muon_passed_wc_mom_cuts_ = false;
  sel_muon_passed_wc_costheta_ = false; 
  sel_CCNp0pi_wc_ = false;
  sel_has_pion_wc_candidate_ = false;
  mc_muon_in_wc_mom_range_ = false;
  mc_no_charged_pi_above_wc_threshold_ = false;

  // XGBoost outputs
  sel_n_bdt_other_ = BOGUS_INDEX;
  sel_n_bdt_muon_ = BOGUS_INDEX;
  sel_n_bdt_pion_ = BOGUS_INDEX;
  sel_n_bdt_proton_ = BOGUS_INDEX;
  sel_n_bdt_invalid_ = BOGUS_INDEX;

  sel_num_proton_candidates_ = BOGUS_INDEX;
  sel_num_pion_candidates_ = BOGUS_INDEX;
  sel_num_pion_wc_candidates_ = BOGUS_INDEX;
  muon_candidate_idx_ = BOGUS_INDEX;
  lead_p_candidate_idx_ = BOGUS_INDEX;

  distance_FV_surface_ = BOGUS;
  lead_p_BDTScore_ = BOGUS; 
  muontrklen_ = BOGUS;
  muondistancetovectex_ = BOGUS;
  muon_llr_pid_score_ = BOGUS;
  muon_tkscore_ = BOGUS;

  muon_trkend_x_ = BOGUS;
  muon_trkend_y_ = BOGUS;
  muon_trkend_z_ = BOGUS;

  muon_trkstart_x_ = BOGUS;
  muon_trkstart_y_ = BOGUS;
  muon_trkstart_z_ = BOGUS;
  muon_trkchi2muon_ = BOGUS;
  muon_BDTScore_    = BOGUS;
  Eavail_ = BOGUS;
  
  PanelProjection_X_Z_ = BOGUS_INDEX;
  PanelProjection_Y_Z_ = BOGUS_INDEX;
  PanelProjection_X_Y_ = BOGUS_INDEX;
  
  sel_PanelClosesttoEndMuonTrk_ = BOGUS;
  sel_PanelClosestSubPaneltoEndMuonTrk_ = BOGUS;

  *p3_mu_ = TVector3();
  *p3_lead_p_ = TVector3();
  *p3_lead_pi_ = TVector3();

  *p3_mu_mcs_= TVector3();
  *p3_mu_range_= TVector3();
  
  p3_p_vec_ ->clear();
  *mc_p3_mu_ = TVector3();

  *mc_p3_lead_p_ = TVector3();
  *mc_p3_lead_pi_ = TVector3();
  mc_p3_p_vec_->clear();

  mc_muontrklen_ = BOGUS;
  xgb_pid_vec_->clear();
  xgb_score_vec_->clear();

  // Reco STVs
  delta_pT_ = BOGUS;
  delta_phiT_ = BOGUS;
  delta_alphaT_ = BOGUS;
  delta_pL_ = BOGUS;
  pn_ = BOGUS;
  delta_pTx_ = BOGUS;

  delta_pTy_ = BOGUS;
  theta_mu_p_ = BOGUS;

  // MC STVs (only filled for signal events)
  mc_Eavail_ = BOGUS;
  mc_delta_pT_ = BOGUS;
  mc_delta_phiT_ = BOGUS;
  mc_delta_alphaT_ = BOGUS;
  mc_delta_pL_ = BOGUS;
  mc_pn_ = BOGUS;
  mc_delta_pTx_ = BOGUS;
  mc_delta_pTy_ = BOGUS;
  mc_theta_mu_p_ = BOGUS;
  mc_distance_FV_surface_ = BOGUS;
}
/////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
void JOINTCC0pi::define_additional_input_branches(TTree& etree){
}



 
  // Implementing pure virtual functions
    int JOINTCC0pi::categorize_event(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return 0;  // Return a default integer value
    }

    bool JOINTCC0pi::selection(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return false;  // Return a default boolean value
    }

    bool JOINTCC0pi::define_signal(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
        return false;  // Return a default boolean value
    }

    void JOINTCC0pi::compute_reco_observables(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
    }

    void JOINTCC0pi::compute_true_observables(AnalysisANNIE_Event_nuwro* event)  {
        // Do nothing
    }