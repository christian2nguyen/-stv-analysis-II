#pragma once

// ROOT includes
#include "TTree.h"
#include "AnalysisEvent.hh"
#include "AnalysisEvent_ANNIE.hh"
#include "AnalysisEvent_ANNIE_nuwro.hh"
#include "TError.h"   // for gErrorIgnoreLevel
bool ExtraUniverses_Tobe_written_TO_ANNE = false; 


bool BranchExists(TTree& tree, const std::string& branchName) {
    // Check if the branch exists
    return (tree.GetBranch(branchName.c_str()) != nullptr);
}

void CheckBranchExists(TTree& tree, const std::string& branchName){

if(!BranchExists(tree, branchName)){std::cout<<"Could not get Branch named:  "<< branchName<< std::endl;}

}

//Works for annue but not for MicroBooNE turnning off for testing 
//void SetBranchAddress(TTree& etree, std::string BranchName, void* Variable) {
//  //std::cout<<"inside::SetBranchAddress , setting branchName = "<<  BranchName<< std::endl;
//  set_object_input_branch_address( etree, BranchName.c_str(),Variable);
//  CheckBranchExists(etree, BranchName.c_str());
//  
//}

// ummm I wonder if this worked for mcc9.10

// works for uboone not for ANNIE
void SetBranchAddress(TTree& etree, std::string BranchName, void* Variable) {
  etree.SetBranchAddress(BranchName.c_str(),Variable);
}



// Helper function to set branch addresses for reading information
// from the Event TTree////////////////////////////////////////
///////////////////////////////////////////////////////////////
void set_event_branch_addresses(TTree& etree, AnalysisEvent& ev)
{
  // Reco PDG code of primary PFParticle in slice (i.e., the neutrino
  // candidate)
  SetBranchAddress(etree, "slpdg", &ev.nu_pdg_ );

  // Number of neutrino slices identified by the SliceID. Allowed values
  // are zero or one.
  //SetBranchAddress(etree,"nslice",&ev.nslice_);
  SetBranchAddress(etree, "nslice", &ev.nslice_ );


   SetBranchAddress(etree, "run", &ev.run_ );
   SetBranchAddress(etree, "sub", &ev.subrun_ );
   SetBranchAddress(etree, "evt", &ev.evt_  );

  // Topological score
  SetBranchAddress(etree, "topological_score", &ev.topological_score_ );
  SetBranchAddress(etree, "CosmicIP", &ev.cosmic_impact_parameter_ );
  //SetBranchAddress(etree, "CosmicIPAll3D", &ev.CosmicIPAll3D_ );
  //  bool has_slice_orig_pass_id = ( etree.GetBranch("slice_orig_pass_id") != nullptr );
  //if ( has_slice_orig_pass_id ) {
  // SetBranchAddress(etree, "slice_orig_pass_id", &ev.slice_orig_pass_id_ );
  // }
  // else {
  //  //ev.slice_orig_pass_id_.reset(nullptr);
  // }
  //for MCC910
   SetBranchAddress(etree, "slice_orig_pass_id", &ev.slice_orig_pass_id_ );
  
  // Reconstructed neutrino vertex position (with corrections for
  // space charge applied)
  SetBranchAddress(etree, "reco_nu_vtx_sce_x", &ev.nu_vx_ );
  SetBranchAddress(etree, "reco_nu_vtx_sce_y", &ev.nu_vy_ );
  SetBranchAddress(etree, "reco_nu_vtx_sce_z", &ev.nu_vz_ );

  // Reconstructed object counts
  SetBranchAddress(etree, "n_pfps", &ev.num_pf_particles_ );
  SetBranchAddress(etree, "n_tracks", &ev.num_tracks_ );
  SetBranchAddress(etree, "n_showers", &ev.num_showers_ );

  // PFParticle properties
  set_object_input_branch_address( etree, "pfp_generation_v",
    ev.pfp_generation_ );

  set_object_input_branch_address( etree, "pfp_trk_daughters_v",
    ev.pfp_trk_daughters_count_ );

  set_object_input_branch_address( etree, "pfp_shr_daughters_v",
    ev.pfp_shr_daughters_count_ );

  set_object_input_branch_address( etree, "trk_score_v", ev.pfp_track_score_ );
  set_object_input_branch_address( etree, "pfpdg", ev.pfp_reco_pdg_ );
  set_object_input_branch_address( etree, "pfnhits", ev.pfp_hits_ );
  set_object_input_branch_address( etree, "pfnplanehits_U", ev.pfp_hitsU_ );
  set_object_input_branch_address( etree, "pfnplanehits_V", ev.pfp_hitsV_ );
  set_object_input_branch_address( etree, "pfnplanehits_Y", ev.pfp_hitsY_ );

  // Backtracked PFParticle properties
  set_object_input_branch_address( etree, "backtracked_pdg", ev.pfp_true_pdg_ );
  set_object_input_branch_address( etree, "backtracked_e", ev.pfp_true_E_ );
  set_object_input_branch_address( etree, "backtracked_px", ev.pfp_true_px_ );
  set_object_input_branch_address( etree, "backtracked_py", ev.pfp_true_py_ );
  set_object_input_branch_address( etree, "backtracked_pz", ev.pfp_true_pz_ );

  // Shower properties
  // These are excluded from some ntuples to ensure blindness for the LEE
  // analyses. We will skip them when not available.
  bool has_shower_branches = ( etree.GetBranch("shr_pfp_id_v") != nullptr );
  if ( has_shower_branches ) {
    set_object_input_branch_address( etree, "shr_pfp_id_v", ev.shower_pfp_id_ );
    set_object_input_branch_address( etree, "shr_start_x_v", ev.shower_startx_ );
    set_object_input_branch_address( etree, "shr_start_y_v", ev.shower_starty_ );
    set_object_input_branch_address( etree, "shr_start_z_v", ev.shower_startz_ );
    // Shower start distance from reco neutrino vertex (pre-calculated for
    // convenience)
    set_object_input_branch_address( etree, "shr_dist_v",
      ev.shower_start_distance_ );
  }
  else {
    // When the shower information is not available, delete the owned vectors
    // to signal that the associated branches should not be written to the
    // output TTree
    ev.shower_pfp_id_.reset( nullptr );
    ev.shower_startx_.reset( nullptr );
    ev.shower_starty_.reset( nullptr );
    ev.shower_startz_.reset( nullptr );
    ev.shower_start_distance_.reset( nullptr );
  }

  // Generator truth
  bool has_generator_truth = (etree.GetBranch("mc_generator_pdg") != nullptr);
  if(has_generator_truth){
    set_object_input_branch_address(etree, "mc_generator_pdg", ev.mc_generator_pdg_);
    set_object_input_branch_address(etree, "mc_generator_mother", ev.mc_generator_mother_);
    set_object_input_branch_address(etree, "mc_generator_rescatter", ev.mc_generator_rescatter_);
    set_object_input_branch_address(etree, "mc_generator_trackid", ev.mc_generator_trackid_);
    set_object_input_branch_address(etree, "mc_generator_statuscode", ev.mc_generator_statuscode_);
    set_object_input_branch_address(etree, "mc_generator_E", ev.mc_generator_E_);
    set_object_input_branch_address(etree, "mc_generator_px", ev.mc_generator_px_);
    set_object_input_branch_address(etree, "mc_generator_py", ev.mc_generator_py_);
    set_object_input_branch_address(etree, "mc_generator_pz", ev.mc_generator_pz_);
  }
  else{
    ev.mc_generator_pdg_.reset(nullptr);
    ev.mc_generator_rescatter_.reset(nullptr);
    ev.mc_generator_mother_.reset(nullptr);
    ev.mc_generator_trackid_.reset(nullptr);
    ev.mc_generator_statuscode_.reset(nullptr);
    ev.mc_generator_E_.reset(nullptr);
    ev.mc_generator_px_.reset(nullptr);
    ev.mc_generator_py_.reset(nullptr);
    ev.mc_generator_pz_.reset(nullptr);
  }

  // Track properties
  set_object_input_branch_address( etree, "trk_pfp_id_v", ev.track_pfp_id_ );
  set_object_input_branch_address( etree, "trk_len_v", ev.track_length_ );
  set_object_input_branch_address( etree, "trk_sce_start_x_v", ev.track_startx_ );
  set_object_input_branch_address( etree, "trk_sce_start_y_v", ev.track_starty_ );
  set_object_input_branch_address( etree, "trk_sce_start_z_v", ev.track_startz_ );

  // Track start distance from reco neutrino vertex (pre-calculated for
  // convenience)
  set_object_input_branch_address( etree, "trk_distance_v",
    ev.track_start_distance_ );


  set_object_input_branch_address( etree, "trk_sce_end_x_v", ev.track_endx_ );
  set_object_input_branch_address( etree, "trk_sce_end_y_v", ev.track_endy_ );
  set_object_input_branch_address( etree, "trk_sce_end_z_v", ev.track_endz_ );

  set_object_input_branch_address( etree, "trk_dir_x_v", ev.track_dirx_ );
  set_object_input_branch_address( etree, "trk_dir_y_v", ev.track_diry_ );
  set_object_input_branch_address( etree, "trk_dir_z_v", ev.track_dirz_ );

  set_object_input_branch_address( etree, "trk_theta_v", ev.track_theta_ );
  set_object_input_branch_address( etree, "trk_phi_v", ev.track_phi_ );

  set_object_input_branch_address( etree, "trk_energy_proton_v",
    ev.track_kinetic_energy_p_ );

  set_object_input_branch_address( etree, "trk_range_muon_mom_v",
    ev.track_range_mom_mu_ );

  set_object_input_branch_address( etree, "trk_mcs_muon_mom_v",
    ev.track_mcs_mom_mu_ );

  // Some ntuples exclude the old proton chi^2 PID score. Only include it
  // in the output if this branch is available.
  bool has_chipr = ( etree.GetBranch("trk_pid_chipr_v") != nullptr );
  if ( has_chipr ) {
    set_object_input_branch_address( etree, "trk_pid_chipr_v",
      ev.track_chi2_proton_ );
  }
  else {
    ev.track_chi2_proton_.reset( nullptr );
  }
  //////////////// Same for muon 
    bool has_chipr_muon = ( etree.GetBranch("trk_pid_chimu_v") != nullptr );
  if ( has_chipr_muon ) {
    set_object_input_branch_address( etree, "trk_pid_chimu_v",
      ev.track_chi2_muon_ );
  }
  else {
    ev.track_chi2_muon_.reset( nullptr );
  }
  
  

 set_object_input_branch_address( etree, "trk_bragg_mu_fwd_preferred_v",
      ev.trk_bragg_mu_fwd_preferred_v_ );

  // Log-likelihood-based particle ID information
  set_object_input_branch_address( etree, "trk_llr_pid_v", ev.track_llr_pid_ );

  set_object_input_branch_address( etree, "trk_llr_pid_u_v",
    ev.track_llr_pid_U_ );

  set_object_input_branch_address( etree, "trk_llr_pid_v_v",
    ev.track_llr_pid_V_ );

  set_object_input_branch_address( etree, "trk_llr_pid_y_v",
    ev.track_llr_pid_Y_ );

  set_object_input_branch_address( etree, "trk_llr_pid_score_v",
    ev.track_llr_pid_score_ );

  // MC truth information for the neutrino
  SetBranchAddress(etree, "nu_pdg", &ev.mc_nu_pdg_ );
  SetBranchAddress(etree, "true_nu_vtx_x", &ev.mc_nu_vx_ );
  //SetBranchAddress(etree, "mc_vx", &ev.mc_nu_vx_ );
  SetBranchAddress(etree, "true_nu_vtx_y", &ev.mc_nu_vy_ );
  SetBranchAddress(etree, "true_nu_vtx_z", &ev.mc_nu_vz_ );
  SetBranchAddress(etree, "nu_e", &ev.mc_nu_energy_ );
  SetBranchAddress(etree, "ccnc", &ev.mc_nu_ccnc_ );
  SetBranchAddress(etree, "interaction", &ev.mc_nu_interaction_type_ );

  //=============================================
  //DB Added to match Samantha's Signal defintion

  SetBranchAddress(etree, "true_nu_vtx_sce_x", &ev.mc_nu_sce_vx_ );
  SetBranchAddress(etree, "true_nu_vtx_sce_y", &ev.mc_nu_sce_vy_ );
  SetBranchAddress(etree, "true_nu_vtx_sce_z", &ev.mc_nu_sce_vz_ );
 //=============================================
 // Blips 
 //=============================================
  bool has_blips = ( etree.GetBranch("blip_x") != nullptr );
  
  if ( has_blips ) {
    SetBranchAddress(etree, "nblips_saved", &ev.nblips_saved_ );
    set_object_input_branch_address( etree, "blip_x", ev.blip_x_ );
    set_object_input_branch_address( etree, "blip_y", ev.blip_y_ );
    set_object_input_branch_address( etree, "blip_z", ev.blip_z_ );
    //set_object_input_branch_address( etree, "blip_size", ev.blip_size_ );
    set_object_input_branch_address( etree, "blip_energy", ev.blip_energy_ );
    
    set_object_input_branch_address( etree, "blip_true_g4id", ev.blip_true_g4id_ );
    set_object_input_branch_address( etree, "blip_true_energy", ev.blip_true_energy_ );
   }
  else {
         //ev.nblips_saved_.reset( nullptr );
         ev.blip_x_.reset( nullptr );
         ev.blip_y_.reset( nullptr );
         ev.blip_z_.reset( nullptr );
         ev.blip_true_g4id_.reset( nullptr );
         ev.blip_true_energy_.reset( nullptr );
  }


 //std::vector<float> *blip_charge = 0;
 //std::vector<int> *blip_nplanes = 0;
 //std::vector<float> *blip_proxtrkdist = 0;
 //std::vector<int> *blip_proxtrkid = 0;
 //std::vector<int> *blip_touchtrk = 0;
 //std::vector<int> *blip_touchtrkid = 0;
 //std::vector<float> *blip_badwirefrac = 0;
 //std::vector<int> *blip_p0_nwires = 0;
 //std::vector<int> *blip_p1_nwires = 0;
 //std::vector<int> *blip_p2_nwires = 0;
 //std::vector<int> *blip_p0_bydeadwire = 0;
 //std::vector<int> *blip_p1_bydeadwire = 0;
 //std::vector<int> *blip_p2_bydeadwire = 0;
 //std::vector<int> *blip_p0_centerwire = 0;
 //std::vector<int> *blip_p1_centerwire = 0;
 //std::vector<int> *blip_p2_centerwire = 0;
 
 
   //=============================================
 
   // MC truth information for the final-state primary particles
  set_object_input_branch_address( etree, "mc_pdg", ev.mc_nu_daughter_pdg_ );
  set_object_input_branch_address( etree, "mc_E", ev.mc_nu_daughter_energy_ );
  set_object_input_branch_address( etree, "mc_px", ev.mc_nu_daughter_px_ );
  set_object_input_branch_address( etree, "mc_py", ev.mc_nu_daughter_py_ );
  set_object_input_branch_address( etree, "mc_pz", ev.mc_nu_daughter_pz_ );



  set_object_input_branch_address( etree, "mc_endx", ev.mc_nu_daughter_endx_ );
  set_object_input_branch_address( etree, "mc_endy", ev.mc_nu_daughter_endy_ );
  set_object_input_branch_address( etree, "mc_endz", ev.mc_nu_daughter_endz_ );

  // GENIE and other systematic variation weights
  bool has_genie_mc_weights = ( etree.GetBranch("weightSpline") != nullptr );
  if ( has_genie_mc_weights ) {
    SetBranchAddress(etree, "weightSpline", &ev.spline_weight_ );
    SetBranchAddress(etree, "weightTune", &ev.tuned_cv_weight_ );
  }

  bool has_weight_map = ( etree.GetBranch("weights") != nullptr );
  if ( has_weight_map ) {
    set_object_input_branch_address( etree, "weights", ev.mc_weights_map_ );
  }
  else {
    ev.mc_weights_map_.reset( nullptr );
  }

  // Purity and completeness of the backtracked hits in the neutrino slice
  bool has_pfp_backtracked_purity = ( etree.GetBranch("nu_purity_from_pfp")
    != nullptr );
  if ( has_pfp_backtracked_purity ) {

    SetBranchAddress(etree, "nu_completeness_from_pfp",
      &ev.nu_completeness_from_pfp_ );

    SetBranchAddress(etree, "nu_purity_from_pfp", &ev.nu_purity_from_pfp_ );

  }

}
///////////////////////////////////////////////////////////////
// Helper function to set branch addresses for the output TTree
void set_event_output_branch_addresses(TTree& out_tree, AnalysisEvent& ev,
  bool create = false)
{
  // Signal definition flags
  set_output_branch_address( out_tree, "is_mc", &ev.is_mc_, create, "is_mc/O" );

  // Event weights
  set_output_branch_address( out_tree, "spline_weight",
    &ev.spline_weight_, create, "spline_weight/F" );

  set_output_branch_address( out_tree, "tuned_cv_weight",
    &ev.tuned_cv_weight_, create, "tuned_cv_weight/F" );



  // If MC weights are available, prepare to store them in the output TTree
  if ( ev.mc_weights_map_ ) {

    // Make separate branches for the various sets of systematic variation
    // weights in the map
    for ( auto& pair : *ev.mc_weights_map_ ) {

      // Prepend "weight_" to the name of the vector of weights in the map
      std::string weight_branch_name = "weight_" + pair.first;

      // Store a pointer to the vector of weights (needed to set the branch
      // address properly) in the temporary map of pointers
      ev.mc_weights_ptr_map_[ weight_branch_name ] = &pair.second;

      // Set the branch address for this vector of weights
      set_object_output_branch_address< std::vector<double> >( out_tree,
        weight_branch_name, ev.mc_weights_ptr_map_.at(weight_branch_name),
        create );
    }
  }

  // Backtracked neutrino purity and completeness
  set_output_branch_address( out_tree, "nu_completeness_from_pfp",
    &ev.nu_completeness_from_pfp_, create, "nu_completeness_from_pfp/F" );

  set_output_branch_address( out_tree, "nu_purity_from_pfp",
    &ev.nu_purity_from_pfp_, create, "nu_purity_from_pfp/F" );

  // Number of neutrino slices identified by the SliceID
  set_output_branch_address( out_tree, "nslice", &ev.nslice_, create,
    "nslice/I" );
    
    set_output_branch_address( out_tree, "slice_orig_pass_id", &ev.slice_orig_pass_id_, create, "slice_orig_pass_id/I" );
    
 if( ev.slice_orig_pass_id_){
  //set_output_branch_address( out_tree, "slice_orig_pass_id", &ev.slice_orig_pass_id_, create, "slice_orig_pass_id/I" );
  }
  
  set_output_branch_address( out_tree, "run", &ev.run_,
  create, "run/I" );

  set_output_branch_address( out_tree, "subrun", &ev.subrun_,
    create, "subrun/I" );

  set_output_branch_address( out_tree, "evt", &ev.evt_,
    create, "evt/I" );

 // *** Branches copied directly from the input ***

  // Cosmic rejection parameters for numu CC inclusive selection
  set_output_branch_address( out_tree, "topological_score",
    &ev.topological_score_, create, "topological_score/F" );

  set_output_branch_address( out_tree, "CosmicIP",
    &ev.cosmic_impact_parameter_, create, "CosmicIP/F" );

  // Reconstructed neutrino vertex position
  set_output_branch_address( out_tree, "reco_nu_vtx_sce_x",
    &ev.nu_vx_, create, "reco_nu_vtx_sce_x/F" );

  set_output_branch_address( out_tree, "reco_nu_vtx_sce_y",
    &ev.nu_vy_, create, "reco_nu_vtx_sce_y/F" );

  set_output_branch_address( out_tree, "reco_nu_vtx_sce_z",
    &ev.nu_vz_, create, "reco_nu_vtx_sce_z/F" );

  // MC truth information for the neutrino
  set_output_branch_address( out_tree, "mc_nu_pdg", &ev.mc_nu_pdg_,
    create, "mc_nu_pdg/I" );

  set_output_branch_address( out_tree, "mc_nu_vtx_x", &ev.mc_nu_vx_,
    create, "mc_nu_vtx_x/F" );

  set_output_branch_address( out_tree, "mc_nu_vtx_y", &ev.mc_nu_vy_,
    create, "mc_nu_vtx_y/F" );

  set_output_branch_address( out_tree, "mc_nu_vtx_z", &ev.mc_nu_vz_,
    create, "mc_nu_vtx_z/F" );

  set_output_branch_address( out_tree, "mc_nu_energy", &ev.mc_nu_energy_,
    create, "mc_nu_energy/F" );

  set_output_branch_address( out_tree, "mc_ccnc", &ev.mc_nu_ccnc_,
    create, "mc_ccnc/I" );

  set_output_branch_address( out_tree, "mc_interaction",
    &ev.mc_nu_interaction_type_, create, "mc_interaction/I" );


  if( ev.mc_generator_pdg_){
    set_object_output_branch_address< std::vector<int> >( out_tree, 
        "mc_generator_pdg", ev.mc_generator_pdg_, create );

    set_object_output_branch_address< std::vector<int> >( out_tree, 
        "mc_generator_rescatter", ev.mc_generator_rescatter_, create );
    set_object_output_branch_address< std::vector<int> >( out_tree, 
        "mc_generator_mother", ev.mc_generator_mother_, create );
    set_object_output_branch_address< std::vector<int> >( out_tree, 
        "mc_generator_trackid", ev.mc_generator_trackid_, create );
    set_object_output_branch_address< std::vector<int> >( out_tree, 
        "mc_generator_statuscode", ev.mc_generator_statuscode_, create );
    set_object_output_branch_address< std::vector<float> >( out_tree, 
        "mc_generator_E", ev.mc_generator_E_, create );
    set_object_output_branch_address< std::vector<float> >( out_tree, 
        "mc_generator_px", ev.mc_generator_px_, create );
    set_object_output_branch_address< std::vector<float> >( out_tree, 
        "mc_generator_py", ev.mc_generator_py_, create );
    set_object_output_branch_address< std::vector<float> >( out_tree, 
        "mc_generator_pz", ev.mc_generator_pz_, create );


  }
  
   if( ev.blip_x_){
  
  set_object_output_branch_address< std::vector<float> >( out_tree, 
    "blip_x", ev.blip_x_, create );
  set_object_output_branch_address< std::vector<float> >( out_tree, 
      "blip_y", ev.blip_y_, create );
  set_object_output_branch_address< std::vector<float> >( out_tree, 
        "blip_z", ev.blip_z_, create );
  //set_object_output_branch_address< std::vector<float> >( out_tree, 
  //    "blip_size", ev.blip_size_, create );
  set_object_output_branch_address< std::vector<float> >( out_tree, 
      "blip_energy", ev.blip_energy_, create );
  set_object_output_branch_address< std::vector<float> >( out_tree, 
       "blip_true_g4id", ev.blip_true_g4id_, create );
  set_object_output_branch_address< std::vector<float> >( out_tree, 
        "blip_true_energy", ev.blip_true_energy_, create );
 }

  // PFParticle properties
  set_object_output_branch_address< std::vector<unsigned int> >( out_tree,
    "pfp_generation_v", ev.pfp_generation_, create );

  set_object_output_branch_address< std::vector<unsigned int> >( out_tree,
    "pfp_trk_daughters_v", ev.pfp_trk_daughters_count_, create );

  set_object_output_branch_address< std::vector<unsigned int> >( out_tree,
    "pfp_shr_daughters_v", ev.pfp_shr_daughters_count_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_score_v", ev.pfp_track_score_, create );

  set_object_output_branch_address< std::vector<int> >( out_tree,
    "pfpdg", ev.pfp_reco_pdg_, create );

  set_object_output_branch_address< std::vector<int> >( out_tree,
    "pfnhits", ev.pfp_hits_, create );

  set_object_output_branch_address< std::vector<int> >( out_tree,
    "pfnplanehits_U", ev.pfp_hitsU_, create );

  set_object_output_branch_address< std::vector<int> >( out_tree,
    "pfnplanehits_V", ev.pfp_hitsV_, create );

  set_object_output_branch_address< std::vector<int> >( out_tree,
    "pfnplanehits_Y", ev.pfp_hitsY_, create );

  // Backtracked PFParticle properties
  set_object_output_branch_address< std::vector<int> >( out_tree,
    "backtracked_pdg", ev.pfp_true_pdg_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "backtracked_e", ev.pfp_true_E_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "backtracked_px", ev.pfp_true_px_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "backtracked_py", ev.pfp_true_py_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "backtracked_pz", ev.pfp_true_pz_, create );

  // Shower properties
  // For some ntuples, reconstructed shower information is excluded.
  // In such cases, skip writing these branches to the output TTree.
  if ( ev.shower_startx_ ) {
    set_object_output_branch_address< std::vector<float> >( out_tree,
      "shr_start_x_v", ev.shower_startx_, create );

    set_object_output_branch_address< std::vector<float> >( out_tree,
      "shr_start_y_v", ev.shower_starty_, create );

    set_object_output_branch_address< std::vector<float> >( out_tree,
      "shr_start_z_v", ev.shower_startz_, create );

    // Shower start distance from reco neutrino vertex (pre-calculated for
    // convenience)
    set_object_output_branch_address< std::vector<float> >( out_tree,
      "shr_dist_v", ev.shower_start_distance_, create );
  }

  // Track properties
  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_len_v", ev.track_length_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_sce_start_x_v", ev.track_startx_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_sce_start_y_v", ev.track_starty_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_sce_start_z_v", ev.track_startz_, create );

  // Track start distance from reco neutrino vertex (pre-calculated for
  // convenience)
  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_distance_v", ev.track_start_distance_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_sce_end_x_v", ev.track_endx_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_sce_end_y_v", ev.track_endy_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_sce_end_z_v", ev.track_endz_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_dir_x_v", ev.track_dirx_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_dir_y_v", ev.track_diry_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_dir_z_v", ev.track_dirz_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_energy_proton_v", ev.track_kinetic_energy_p_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_range_muon_mom_v", ev.track_range_mom_mu_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_mcs_muon_mom_v", ev.track_mcs_mom_mu_, create );

  // Some ntuples exclude the old chi^2 proton PID score. Only include it in
  // the output if it is available.
  if ( ev.track_chi2_proton_ ) {
    set_object_output_branch_address< std::vector<float> >( out_tree,
      "trk_pid_chipr_v", ev.track_chi2_proton_, create );
  }

  // Log-likelihood-based particle ID information
  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_llr_pid_v", ev.track_llr_pid_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_llr_pid_u_v", ev.track_llr_pid_U_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_llr_pid_v_v", ev.track_llr_pid_V_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_llr_pid_y_v", ev.track_llr_pid_Y_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree,
    "trk_llr_pid_score_v", ev.track_llr_pid_score_, create );

  // MC truth information for the final-state primary particles
  set_object_output_branch_address< std::vector<int> >( out_tree, "mc_pdg",
    ev.mc_nu_daughter_pdg_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree, "mc_E",
    ev.mc_nu_daughter_energy_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree, "mc_px",
    ev.mc_nu_daughter_px_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree, "mc_py",
    ev.mc_nu_daughter_py_, create );

  set_object_output_branch_address< std::vector<float> >( out_tree, "mc_pz",
    ev.mc_nu_daughter_pz_, create );
    ////////////////////////
    // new 
    ///////////////////
    
  if ( ev.track_chi2_muon_ ) {
    set_object_output_branch_address< std::vector<float> >( out_tree,
      "trk_pid_chimu_v", ev.track_chi2_muon_, create );
  }
    
    
 set_object_output_branch_address< std::vector<float> >( out_tree,
   "trk_pid_chipi_v", ev.track_chi2_pion_, create );
 
 set_object_output_branch_address< std::vector<float> >( out_tree,
   "trk_pid_chika_v", ev.track_chi2_kaon_, create );
     
 set_object_output_branch_address< std::vector<float> >( out_tree,
   "trk_bragg_mu_v", ev.trk_bragg_mu_v_, create );

 //set_object_output_branch_address< std::vector<float> >( out_tree,
 //       "trk_bragg_pion_v", ev.trk_bragg_pion_v_, create );

 set_object_output_branch_address< std::vector<float> >( out_tree,
       "trk_bragg_p_v", ev.trk_bragg_p_v_, create );
 
 set_object_output_branch_address< std::vector<float> >( out_tree,
       "trk_bragg_mip_v", ev.trk_bragg_mip_v_, create );  
  
    
 set_object_output_branch_address< std::vector<bool> >( out_tree,
       "trk_bragg_mu_fwd_preferred_v", ev.trk_bragg_mu_fwd_preferred_v_, create );  
    
    
}
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
void set_event_branch_addresses(TTree& etree, AnalysisANNIE_Event& ev)
{
   //std::cout<<" "<< std::endl;
   //std::cout<<"Inside :: set_event_branch_addresses"<< std::endl;

    etree.SetBranchAddress("runNumber",    &ev.runNumber_);
    etree.SetBranchAddress("subrunNumber", &ev.subrunNumber_);
    etree.SetBranchAddress("runType",      &ev.runType_);
    //etree.SetBranchAddress("startTime",    &ev.startTime_);
    etree.SetBranchAddress("eventNumber",  &ev.eventNumber_);
    //etree.SetBranchAddress("eventTimeTank", &ev.eventTimeTank_);
    //etree.SetBranchAddress("eventTimeMRD",  &ev.eventTimeMRD_);
    etree.SetBranchAddress("nhits",         &ev.nhits_);
    etree.SetBranchAddress("trigword",      &ev.trigword_);
    etree.SetBranchAddress("HasTank",       &ev.HasTank_);
    etree.SetBranchAddress("HasMRD",        &ev.HasMRD_);
    etree.SetBranchAddress("TankMRDCoinc",  &ev.TankMRDCoinc_);
    etree.SetBranchAddress("NoVeto",        &ev.NoVeto_);
    etree.SetBranchAddress("Extended",      &ev.Extended_);
    etree.SetBranchAddress("beam_pot",      &ev.beam_pot_);
    etree.SetBranchAddress("beam_ok",       &ev.beam_ok_);



    // MRD track information
    etree.SetBranchAddress("numMRDTracks", &ev.numMRDTracks_);
    set_object_input_branch_address( etree,"MRDTrackAngle", ev.MRDTrackAngle_);
    set_object_input_branch_address( etree,"MRDTrackAngleError", ev.MRDTrackAngleError_);
    set_object_input_branch_address( etree,"MRDPenetrationDepth", ev.MRDPenetrationDepth_);
    set_object_input_branch_address( etree,"MRDTrackLength", ev.MRDTrackLength_);
    set_object_input_branch_address( etree,"MRDEntryPointRadius", ev.MRDEntryPointRadius_);
    set_object_input_branch_address( etree,"MRDEnergyLoss", ev.MRDEnergyLoss_);
    set_object_input_branch_address( etree,"MRDEnergyLossError", ev.MRDEnergyLossError_);
    set_object_input_branch_address( etree,"MRDTrackStartX", ev.MRDTrackStartX_);
    set_object_input_branch_address( etree,"MRDTrackStartY", ev.MRDTrackStartY_);
    set_object_input_branch_address( etree,"MRDTrackStartZ", ev.MRDTrackStartZ_);
    set_object_input_branch_address( etree,"MRDTrackStopX", ev.MRDTrackStopX_);
    set_object_input_branch_address( etree,"MRDTrackStopY", ev.MRDTrackStopY_);
    set_object_input_branch_address( etree,"MRDTrackStopZ", ev.MRDTrackStopZ_);
    set_object_input_branch_address( etree,"MRDSide", ev.MRDSide_);
    set_object_input_branch_address( etree,"MRDStop", ev.MRDStop_);
    set_object_input_branch_address( etree,"MRDThrough", ev.MRDThrough_);

    // Reconstructed vertex information
     etree.SetBranchAddress("recoVtxX", &ev.recoVtxX_);
     etree.SetBranchAddress("recoVtxY", &ev.recoVtxY_);
     etree.SetBranchAddress("recoVtxZ", &ev.recoVtxZ_);
     etree.SetBranchAddress("recoVtxTime", &ev.recoVtxTime_);
     etree.SetBranchAddress("recoDirX", &ev.recoDirX_);
     etree.SetBranchAddress("recoDirY", &ev.recoDirY_);
     etree.SetBranchAddress("recoDirZ", &ev.recoDirZ_);
     etree.SetBranchAddress("recoAngle", &ev.recoAngle_);
     etree.SetBranchAddress("recoPhi", &ev.recoPhi_);
     etree.SetBranchAddress("recoVtxFOM", &ev.recoVtxFOM_);
     etree.SetBranchAddress("recoStatus", &ev.recoStatus_);

    // Simple reconstruction information
    etree.SetBranchAddress("simpleRecoFlag", &ev.simpleRecoFlag_);
    etree.SetBranchAddress("simpleRecoEnergy", &ev.simpleRecoEnergy_);
    etree.SetBranchAddress("simpleRecoVtxX", &ev.simpleRecoVtxX_);
    etree.SetBranchAddress("simpleRecoVtxY", &ev.simpleRecoVtxY_);
    etree.SetBranchAddress("simpleRecoVtxZ", &ev.simpleRecoVtxZ_);
    etree.SetBranchAddress("simpleRecoStopVtxX", &ev.simpleRecoStopVtxX_);
    etree.SetBranchAddress("simpleRecoStopVtxY", &ev.simpleRecoStopVtxY_);
    etree.SetBranchAddress("simpleRecoStopVtxZ", &ev.simpleRecoStopVtxZ_);
    etree.SetBranchAddress("simpleRecoCosTheta", &ev.simpleRecoCosTheta_);
    etree.SetBranchAddress("simpleRecoPt", &ev.simpleRecoPt_);
    etree.SetBranchAddress("simpleRecoFV", &ev.simpleRecoFV_);
    etree.SetBranchAddress("simpleRecoMrdEnergyLoss", &ev.simpleRecoMrdEnergyLoss_);
    etree.SetBranchAddress("simpleRecoTrackLengthInMRD", &ev.simpleRecoTrackLengthInMRD_);
    etree.SetBranchAddress("simpleRecoMRDStartX", &ev.simpleRecoMRDStartX_);
    etree.SetBranchAddress("simpleRecoMRDStartY", &ev.simpleRecoMRDStartY_);
    etree.SetBranchAddress("simpleRecoMRDStartZ", &ev.simpleRecoMRDStartZ_);
    etree.SetBranchAddress("simpleRecoMRDStopX", &ev.simpleRecoMRDStopX_);
    etree.SetBranchAddress("simpleRecoMRDStopY", &ev.simpleRecoMRDStopY_);
    etree.SetBranchAddress("simpleRecoMRDStopZ", &ev.simpleRecoMRDStopZ_);
    etree.SetBranchAddress("simpleRecoTrackLengthInTank", &ev.simpleRecoTrackLengthInTank_);

    // Additional event information
    
    bool RCSRPred_check = ( etree.GetBranch("RCSRPred") != nullptr );
    bool RCMRPred_check = ( etree.GetBranch("RCMRPred") != nullptr );
    
    if (RCSRPred_check) etree.SetBranchAddress("RCSRPred", &ev.RCSRPred_);
    if (RCMRPred_check) etree.SetBranchAddress("RCMRPred", &ev.RCMRPred_);
    
    etree.SetBranchAddress("triggerNumber", &ev.triggerNumber_);
    etree.SetBranchAddress("mcEntryNumber", &ev.mcEntryNumber_);

    // True vertex information
    etree.SetBranchAddress("trueVtxX", &ev.trueVtxX_); //wcsim - dont use 
    etree.SetBranchAddress("trueVtxY", &ev.trueVtxY_);
    etree.SetBranchAddress("trueVtxZ", &ev.trueVtxZ_);
    etree.SetBranchAddress("trueVtxTime", &ev.trueVtxTime_);
    etree.SetBranchAddress("trueDirX", &ev.trueDirX_);
    etree.SetBranchAddress("trueDirY", &ev.trueDirY_);
    etree.SetBranchAddress("trueDirZ", &ev.trueDirZ_);
    etree.SetBranchAddress("trueAngle", &ev.trueAngle_);
    etree.SetBranchAddress("truePhi", &ev.truePhi_);
    etree.SetBranchAddress("trueMuonEnergy", &ev.trueMuonEnergy_);
    etree.SetBranchAddress("truePrimaryPdg", &ev.truePrimaryPdg_);
    etree.SetBranchAddress("trueTrackLengthInWater", &ev.trueTrackLengthInWater_);
    etree.SetBranchAddress("trueTrackLengthInMRD", &ev.trueTrackLengthInMRD_);
    etree.SetBranchAddress("trueMultiRing", &ev.trueMultiRing_);
    
    
    if(etree.GetBranch("trueNuPDG") != nullptr ){
        etree.SetBranchAddress("trueNuPDG", &ev.trueNuPDG_);
    }
    
    
    // Particle counts
    etree.SetBranchAddress("Pi0Count", &ev.Pi0Count_);
    etree.SetBranchAddress("PiPlusCount", &ev.PiPlusCount_);
    etree.SetBranchAddress("PiMinusCount", &ev.PiMinusCount_);
    etree.SetBranchAddress("K0Count", &ev.K0Count_);
    etree.SetBranchAddress("KPlusCount", &ev.KPlusCount_);
    etree.SetBranchAddress("KMinusCount", &ev.KMinusCount_);

    // True primary particle information
    set_object_input_branch_address( etree,"truePrimaryPdgs", ev.truePrimaryPdgs_);
    set_object_input_branch_address( etree,"trueNeutCapVtxX", ev.trueNeutCapVtxX_);
    set_object_input_branch_address( etree,"trueNeutCapVtxY", ev.trueNeutCapVtxY_);
    set_object_input_branch_address( etree,"trueNeutCapVtxZ", ev.trueNeutCapVtxZ_);
    set_object_input_branch_address( etree,"trueNeutCapNucleus", ev.trueNeutCapNucleus_);
    set_object_input_branch_address( etree,"trueNeutCapTime", ev.trueNeutCapTime_);
    set_object_input_branch_address( etree,"trueNeutCapGammas", ev.trueNeutCapGammas_);
    set_object_input_branch_address( etree,"trueNeutCapE", ev.trueNeutCapE_);
    set_object_input_branch_address( etree,"trueNeutCapGammaE", ev.trueNeutCapGammaE_);

    // True neutrino information
    etree.SetBranchAddress("trueNeutrinoEnergy", &ev.trueNeutrinoEnergy_);
    etree.SetBranchAddress("trueNeutrinoMomentum_X", &ev.trueNeutrinoMomentum_X_);
    etree.SetBranchAddress("trueNeutrinoMomentum_Y", &ev.trueNeutrinoMomentum_Y_);
    etree.SetBranchAddress("trueNeutrinoMomentum_Z", &ev.trueNeutrinoMomentum_Z_);
    etree.SetBranchAddress("trueNuIntxVtx_X", &ev.trueNuIntxVtx_X_); // use this one 
    etree.SetBranchAddress("trueNuIntxVtx_Y", &ev.trueNuIntxVtx_Y_);
    etree.SetBranchAddress("trueNuIntxVtx_Z", &ev.trueNuIntxVtx_Z_);
    etree.SetBranchAddress("trueNuIntxVtx_T", &ev.trueNuIntxVtx_T_);
    etree.SetBranchAddress("trueFSLVtx_X", &ev.trueFSLVtx_X_);
    etree.SetBranchAddress("trueFSLVtx_Y", &ev.trueFSLVtx_Y_);
    etree.SetBranchAddress("trueFSLVtx_Z", &ev.trueFSLVtx_Z_);
    etree.SetBranchAddress("trueFSLMomentum_X", &ev.trueFSLMomentum_X_);
    etree.SetBranchAddress("trueFSLMomentum_Y", &ev.trueFSLMomentum_Y_);
    etree.SetBranchAddress("trueFSLMomentum_Z", &ev.trueFSLMomentum_Z_);
    etree.SetBranchAddress("trueFSLTime", &ev.trueFSLTime_);
    etree.SetBranchAddress("trueFSLMass", &ev.trueFSLMass_);
    etree.SetBranchAddress("trueFSLPdg", &ev.trueFSLPdg_);
    etree.SetBranchAddress("trueFSLEnergy", &ev.trueFSLEnergy_);

    // Interaction information
    etree.SetBranchAddress("trueQ2", &ev.trueQ2_);
 
 
    //etree.SetBranchAddress("trueCC", &ev.trueCC_);
    etree.SetBranchAddress("trueCC", &ev.trueCC_);
    etree.SetBranchAddress("trueNC", &ev.trueNC_);
    etree.SetBranchAddress("trueQEL", &ev.trueQEL_);
    etree.SetBranchAddress("trueRES", &ev.trueRES_);
    etree.SetBranchAddress("trueDIS", &ev.trueDIS_);
    etree.SetBranchAddress("trueCOH", &ev.trueCOH_);
    etree.SetBranchAddress("trueMEC", &ev.trueMEC_);
    etree.SetBranchAddress("trueNeutrons", &ev.trueNeutrons_);
    etree.SetBranchAddress("trueProtons", &ev.trueProtons_);
    etree.SetBranchAddress("truePi0", &ev.truePi0_);
    etree.SetBranchAddress("truePiPlus", &ev.truePiPlus_);
    etree.SetBranchAddress("truePiPlusCher", &ev.truePiPlusCher_);
    etree.SetBranchAddress("truePiMinus", &ev.truePiMinus_);
    etree.SetBranchAddress("truePiMinusCher", &ev.truePiMinusCher_);
    etree.SetBranchAddress("trueKPlus", &ev.trueKPlus_);
    etree.SetBranchAddress("trueKPlusCher", &ev.trueKPlusCher_);
    etree.SetBranchAddress("trueKMinus", &ev.trueKMinus_);
    etree.SetBranchAddress("trueKMinusCher", &ev.trueKMinusCher_);
    

    // Additional true information
bool CHECK_trueBJx = ( etree.GetBranch("trueBJx") != nullptr );
if(CHECK_trueBJx) etree.SetBranchAddress("trueBJx", &ev.trueBJx_);

bool CHECK_truey = ( etree.GetBranch("truey") != nullptr );
if(CHECK_truey) etree.SetBranchAddress("truey", &ev.truey_);

bool CHECK_trueq0 = ( etree.GetBranch("trueq0") != nullptr );
if(CHECK_trueq0) etree.SetBranchAddress("trueq0", &ev.trueq0_);

bool CHECK_trueq3 = ( etree.GetBranch("trueq3") != nullptr );
if(CHECK_trueq3) etree.SetBranchAddress("trueq3", &ev.trueq3_);

bool CHECK_trueTargetZ = ( etree.GetBranch("trueTargetZ") != nullptr );
if(CHECK_trueTargetZ) etree.SetBranchAddress("trueTargetZ", &ev.trueTargetZ_);

bool CHECK_trueW2 = ( etree.GetBranch("trueW2") != nullptr );
if(CHECK_trueW2) etree.SetBranchAddress("trueW2", &ev.trueW2_);
    
    bool CHECK_weight_weight_TunedCentralValue_UBGenie = ( etree.GetBranch("weight_TunedCentralValue_UBGenie") != nullptr );
    if(CHECK_weight_weight_TunedCentralValue_UBGenie) set_object_input_branch_address( etree,"weight_TunedCentralValue_UBGenie", ev.weight_TunedCentralValue_UBGenie_);
    
    // Weights
    
    bool CHECK_weight_weight_All_UBGenie = ( etree.GetBranch("weight_All_UBGenie") != nullptr );
    bool CHECK_weight_flux_all = ( etree.GetBranch("weight_flux_all") != nullptr );
    
    // Assuming for now if these wgts are there , then the rest of the cross section wgts are there 
    
    if (  CHECK_weight_weight_All_UBGenie && CHECK_weight_flux_all ) {
    
    set_object_input_branch_address( etree,"weight_All_UBGenie", ev.weight_All_UBGenie_);
    set_object_input_branch_address( etree,"weight_flux_all", ev.weight_flux_all_);
    set_object_input_branch_address( etree,"weight_AxFFCCQEshape_UBGenie", ev.weight_AxFFCCQEshape_UBGenie_);
    set_object_input_branch_address( etree,"weight_DecayAngMEC_UBGenie", ev.weight_DecayAngMEC_UBGenie_);
    set_object_input_branch_address( etree,"weight_NormCCCOH_UBGenie", ev.weight_NormCCCOH_UBGenie_);
 
    set_object_input_branch_address( etree,"weight_RPA_CCQE_UBGenie", ev.weight_RPA_CCQE_UBGenie_);
    set_object_input_branch_address( etree,"weight_RootinoFix_UBGenie", ev.weight_RootinoFix_UBGenie_);
    set_object_input_branch_address( etree,"weight_ThetaDelta2NRad_UBGenie", ev.weight_ThetaDelta2NRad_UBGenie_);
    set_object_input_branch_address( etree,"weight_Theta_Delta2Npi_UBGenie", ev.weight_Theta_Delta2Npi_UBGenie_);
    set_object_input_branch_address( etree,"weight_VecFFCCQEshape_UBGenie", ev.weight_VecFFCCQEshape_UBGenie_);
    set_object_input_branch_address( etree,"weight_XSecShape_CCMEC_UBGenie", ev.weight_XSecShape_CCMEC_UBGenie_);
    }
    
    
     bool CHECK_weight_NormNCCOH = ( etree.GetBranch("weight_NormNCCOH_UBGenie") != nullptr );
     bool CHECKII_weight_NormNCCOH = ( etree.GetBranch("weight_Norm_NCCOH_UBGenie") != nullptr );
       
       if(CHECK_weight_NormNCCOH==true &&CHECKII_weight_NormNCCOH==false ){ 
       set_object_input_branch_address( etree,"weight_NormNCCOH_UBGenie", ev.weight_NormNCCOH_UBGenie_);
          }
       else if(CHECK_weight_NormNCCOH==false &&CHECKII_weight_NormNCCOH==true ){
       set_object_input_branch_address( etree,"weight_Norm_NCCOH_UBGenie", ev.weight_NormNCCOH_UBGenie_);
       }
       
       
    
    
     bool weight_All0_UBGenie_check = ( etree.GetBranch("weight_All0_UBGenie") != nullptr );
     bool weight_All1_UBGenie_check = ( etree.GetBranch("weight_All1_UBGenie") != nullptr );
     bool weight_All2_UBGenie_check = ( etree.GetBranch("weight_All2_UBGenie") != nullptr );
     bool weight_All3_UBGenie_check = ( etree.GetBranch("weight_All3_UBGenie") != nullptr );
     bool weight_All4_UBGenie_check = ( etree.GetBranch("weight_All4_UBGenie") != nullptr );
     bool weight_All5_UBGenie_check = ( etree.GetBranch("weight_All5_UBGenie") != nullptr );
    
      if (  weight_All0_UBGenie_check &&
            weight_All2_UBGenie_check &&
            weight_All1_UBGenie_check &&
            weight_All3_UBGenie_check &&
            weight_All4_UBGenie_check &&
            weight_All5_UBGenie_check ) {
            
                set_object_input_branch_address( etree,"weight_All0_UBGenie", ev.weight_All0_UBGenie_);
                set_object_input_branch_address( etree,"weight_All1_UBGenie", ev.weight_All1_UBGenie_);
                set_object_input_branch_address( etree,"weight_All2_UBGenie", ev.weight_All2_UBGenie_);
                set_object_input_branch_address( etree,"weight_All3_UBGenie", ev.weight_All3_UBGenie_);
                set_object_input_branch_address( etree,"weight_All4_UBGenie", ev.weight_All4_UBGenie_);
                set_object_input_branch_address( etree,"weight_All5_UBGenie", ev.weight_All5_UBGenie_);
            
            
            }
    
    //bool weight_All0_UBGenie_check = ( etree.GetBranch("weight_TunedCentralValue_UBGenie") != nullptr );
    
    
    
    
    /*
    bool weight_All0_UBGenie_check = ( etree.GetBranch("weight_All0_UBGenie") != nullptr );
    bool weight_flux_all_check = ( etree.GetBranch("weight_flux_all") != nullptr );
    bool weight_flux_all = ( etree.GetBranch("weight_All_UBGenie") != nullptr );
      
  if (  weight_All0_UBGenie_check &&
            weight_flux_all_check && 
            weight_flux_all ) {
    set_object_input_branch_address( etree,"weight_All0_UBGenie", ev.weight_All0_UBGenie_);
    set_object_input_branch_address( etree,"weight_All1_UBGenie", ev.weight_All1_UBGenie_);
    set_object_input_branch_address( etree,"weight_All2_UBGenie", ev.weight_All2_UBGenie_);
    set_object_input_branch_address( etree,"weight_All3_UBGenie", ev.weight_All3_UBGenie_);
    set_object_input_branch_address( etree,"weight_All4_UBGenie", ev.weight_All4_UBGenie_);
    set_object_input_branch_address( etree,"weight_All5_UBGenie", ev.weight_All5_UBGenie_);

    

    set_object_input_branch_address( etree,"weight_horncurrent_FluxUnisim", ev.weight_horncurrent_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_expskin_FluxUnisim", ev.weight_expskin_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_piplus_PrimaryHadronSWCentralSplineVariation", ev.weight_piplus_PrimaryHadronSWCentralSplineVariation_);
    set_object_input_branch_address( etree,"weight_piminus_PrimaryHadronSWCentralSplineVariation", ev.weight_piminus_PrimaryHadronSWCentralSplineVariation_);
    set_object_input_branch_address( etree,"weight_kplus_PrimaryHadronFeynmanScaling", ev.weight_kplus_PrimaryHadronFeynmanScaling_);
    set_object_input_branch_address( etree,"weight_kzero_PrimaryHadronSanfordWang", ev.weight_kzero_PrimaryHadronSanfordWang_);
    set_object_input_branch_address( etree,"weight_kminus_PrimaryHadronNormalization", ev.weight_kminus_PrimaryHadronNormalization_);
    set_object_input_branch_address( etree,"weight_pioninexsec_FluxUnisim", ev.weight_pioninexsec_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_pionqexsec_FluxUnisim", ev.weight_pionqexsec_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_piontotxsec_FluxUnisim", ev.weight_piontotxsec_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_nucleoninexsec_FluxUnisim", ev.weight_nucleoninexsec_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_nucleonqexsec_FluxUnisim", ev.weight_nucleonqexsec_FluxUnisim_);
    set_object_input_branch_address( etree,"weight_nucleontotxsec_FluxUnisim", ev.weight_nucleontotxsec_FluxUnisim_);
    // Weights (continued)

    }
 */


    if(etree.GetBranch("Qij") != nullptr ){
        
         etree.SetBranchAddress("Qij", &ev.Qij_);
    }

    if(etree.GetBranch("promptMuonTotalPE") != nullptr ){
     
     etree.SetBranchAddress("promptMuonTotalPE", &ev.promptMuonTotalPE_);
    }

    // Seed vertex information
    set_object_input_branch_address( etree,"seedVtxX", ev.seedVtxX_);
    set_object_input_branch_address( etree,"seedVtxY", ev.seedVtxY_);
    set_object_input_branch_address( etree,"seedVtxZ", ev.seedVtxZ_);
    set_object_input_branch_address( etree,"seedVtxFOM", ev.seedVtxFOM_);
    etree.SetBranchAddress("seedVtxTime", &ev.seedVtxTime_);
    
     //etree.SetBranchAddress("reco_0pi", &ev.reco_0pi_);
     //etree.SetBranchAddress("recoPE", &ev.recoPE_);


    // Point information
    set_object_input_branch_address( etree, "hitX",      ev.hitX_  );
    set_object_input_branch_address( etree, "hitY",      ev.hitY_  );
    set_object_input_branch_address( etree, "hitZ",      ev.hitZ_  );
    set_object_input_branch_address( etree, "hitT",      ev.hitT_  );
    set_object_input_branch_address( etree, "hitQ",      ev.hitQ_  );
    set_object_input_branch_address( etree, "hitPE",     ev.hitPE_ );
    set_object_input_branch_address( etree, "hitDetID",  ev.hitDetID_ );
    
    SetBranchAddress(etree,"pointPosX", &ev.pointPosX_);
    SetBranchAddress(etree,"pointPosY", &ev.pointPosY_);
    SetBranchAddress(etree,"pointPosZ", &ev.pointPosZ_);
    SetBranchAddress(etree,"pointPosTime", &ev.pointPosTime_);
    SetBranchAddress(etree,"pointPosFOM", &ev.pointPosFOM_);
    SetBranchAddress(etree,"pointPosStatus", &ev.pointPosStatus_);
    SetBranchAddress(etree,"pointDirX", &ev.pointDirX_);
    SetBranchAddress(etree,"pointDirY", &ev.pointDirY_);
    SetBranchAddress(etree,"pointDirZ", &ev.pointDirZ_);
    SetBranchAddress(etree,"pointDirTime", &ev.pointDirTime_);
    SetBranchAddress(etree,"pointDirStatus", &ev.pointDirStatus_);
    SetBranchAddress(etree,"pointDirFOM", &ev.pointDirFOM_);
    SetBranchAddress(etree,"pointVtxPosX", &ev.pointVtxPosX_);
    SetBranchAddress(etree,"pointVtxPosY", &ev.pointVtxPosY_);
    SetBranchAddress(etree,"pointVtxPosZ", &ev.pointVtxPosZ_);
    SetBranchAddress(etree,"pointVtxTime", &ev.pointVtxTime_);
    SetBranchAddress(etree,"pointVtxDirX", &ev.pointVtxDirX_);
    SetBranchAddress(etree,"pointVtxDirY", &ev.pointVtxDirY_);
    SetBranchAddress(etree,"pointVtxDirZ", &ev.pointVtxDirZ_);
    SetBranchAddress(etree,"pointVtxFOM", &ev.pointVtxFOM_);
    SetBranchAddress(etree,"pointVtxStatus", &ev.pointVtxStatus_);

    // Fiducial volume flags
      
   if(etree.GetBranch("trueFV") != nullptr ){
        etree.SetBranchAddress("trueFV", &ev.trueFV_);
    }
        
    if(etree.GetBranch("recoFV") != nullptr ){
        etree.SetBranchAddress("recoFV", &ev.recoFV_);
    }
    
    if(etree.GetBranch("true_no_followers") != nullptr ){
         etree.SetBranchAddress("true_no_followers", &ev.true_no_followers_);
    }
    
    
    // Additional true information
    etree.SetBranchAddress("trueMuonMomentum", &ev.trueMuonMomentum_);
    
    if(etree.GetBranch("trueCosTheta") != nullptr ){
          etree.SetBranchAddress("trueCosTheta", &ev.trueCosTheta_);
    }
    
   
    
    etree.SetBranchAddress("simpleRecoMomentum", &ev.simpleRecoMomentum_);
    etree.SetBranchAddress("simpleRecoMomentumCor", &ev.simpleRecoMomentumCor_);
    
    
    if(etree.GetBranch("true_no_mesons") != nullptr ){
          etree.SetBranchAddress("true_no_mesons", &ev.true_no_mesons_);
    }
    
      if(etree.GetBranch("reco0pi_contained_in_MRD") != nullptr ){
          etree.SetBranchAddress("reco0pi_contained_in_MRD", &ev.reco0pi_contained_in_MRD_);
    }
    

    
   if(etree.GetBranch("DirtMu") != nullptr ){
         etree.SetBranchAddress("DirtMu", &ev.DirtMu_);
    }
    if(etree.GetBranch("MRDEff") != nullptr ){
         etree.SetBranchAddress("MRDEff", &ev.MRDEff_);
    }
    bool weight_DirtUnc_check = ( etree.GetBranch("weight_DirtUnc") != nullptr );
    if (  weight_DirtUnc_check ) {
            set_object_input_branch_address( etree,"weight_DirtUnc", ev.weight_DirtUnc_);
    }
    bool weight_MRDUnc_check = ( etree.GetBranch("weight_MRDUnc") != nullptr );
    if (  weight_MRDUnc_check ) {
          set_object_input_branch_address( etree,"weight_MRDUnc", ev.weight_MRDUnc_);
      }



     //std::cout<<" "<< std::endl;

}
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
void set_event_output_branch_addresses(TTree& out_tree, AnalysisANNIE_Event& ev,
  bool create = false)
{
  //std::cout<<" "<< std::endl;
  //std::cout<<"Inside :: set_event_output_branch_addresses"<< std::endl;
    
 set_output_branch_address( out_tree, "runNumber", &ev.runNumber_,create,"runNumber/I");
 set_output_branch_address( out_tree, "subrunNumber", &ev.subrunNumber_,create,"subrunNumber/I");
 set_output_branch_address( out_tree, "runType", &ev.runType_,create,"runType/I");
 //set_output_branch_address( out_tree, "startTime", &ev.startTime_,create,"startTime/I");
 set_output_branch_address( out_tree, "eventNumber", &ev.eventNumber_,create,"eventNumber/I");
 //set_output_branch_address( out_tree, "eventTimeTank", &ev.eventTimeTank_,create,"eventTimeTank/I");
 //set_output_branch_address( out_tree, "eventTimeMRD", &ev.eventTimeMRD_,create,"eventTimeMRD/I");
 set_output_branch_address( out_tree, "Pi0Count", &ev.Pi0Count_,create,"Pi0Count/I");
 set_output_branch_address( out_tree, "PiPlusCount", &ev.PiPlusCount_,create,"PiPlusCount/I");
 set_output_branch_address( out_tree, "PiMinusCount", &ev.PiMinusCount_,create,"PiMinusCount/I");
 set_output_branch_address( out_tree, "K0Count", &ev.K0Count_,create,"K0Count/I");
 set_output_branch_address( out_tree, "KPlusCount", &ev.KPlusCount_,create,"KPlusCount/I");
 set_output_branch_address( out_tree, "KMinusCount", &ev.KMinusCount_,create,"KMinusCount/I");
 set_output_branch_address( out_tree, "trueFV", &ev.trueFV_,create,"trueFV/O");
 set_output_branch_address( out_tree, "recoFV", &ev.recoFV_,create,"recoFV/O");
 set_output_branch_address( out_tree, "true_no_followers", &ev.true_no_followers_,create,"true_no_followers/O");
     
     
 set_output_branch_address( out_tree, "nhits",&ev.nhits_,create,"nhits/I");
 set_output_branch_address( out_tree, "trigword",&ev.trigword_,create,"trigword/I");
 set_output_branch_address( out_tree, "HasTank",&ev.HasTank_,create,"HasTank/I");
 set_output_branch_address( out_tree, "HasMRD",&ev.HasMRD_,create,"HasMRD/I");
 set_output_branch_address( out_tree, "TankMRDCoinc",&ev.TankMRDCoinc_,create,"TankMRDCoinc/I");
 set_output_branch_address( out_tree, "NoVeto",&ev.NoVeto_,create,"NoVeto/I");
 set_output_branch_address( out_tree, "Extended",&ev.Extended_,create,"Extended/I");
 set_output_branch_address( out_tree, "beam_ok",&ev.beam_ok_,create,"beam_ok/I");
 set_output_branch_address( out_tree,"beam_pot", &ev.beam_pot_,create,"beam_pot/D");

 set_output_branch_address( out_tree,"DirtMu", &ev.DirtMu_,create,"DirtMu/D");
 set_output_branch_address( out_tree,"MRDEff", &ev.MRDEff_,create,"MRDEff/D");


 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_DirtUnc" , ev.weight_DirtUnc_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_MRDUnc" , ev.weight_MRDUnc_,create) ;


 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackAngle" , ev.MRDTrackAngle_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackAngleError" , ev.MRDTrackAngleError_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDPenetrationDepth" , ev.MRDPenetrationDepth_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackLength" , ev.MRDTrackLength_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDEntryPointRadius" , ev.MRDEntryPointRadius_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDEnergyLoss" , ev.MRDEnergyLoss_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDEnergyLossError" , ev.MRDEnergyLossError_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackStartX" , ev.MRDTrackStartX_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackStartY" , ev.MRDTrackStartY_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackStartZ" , ev.MRDTrackStartZ_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackStopX" , ev.MRDTrackStopX_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackStopY" , ev.MRDTrackStopY_,create) ;
 set_object_output_branch_address< std::vector<double> >( out_tree,"MRDTrackStopZ" , ev.MRDTrackStopZ_,create) ;

 set_object_output_branch_address< std::vector<bool> >( out_tree,"MRDSide" , ev.MRDSide_,create);
 set_object_output_branch_address< std::vector<bool> >( out_tree,"MRDStop" , ev.MRDStop_,create);
 set_object_output_branch_address< std::vector<bool> >( out_tree,"MRDThrough" , ev.MRDThrough_,create);
 
 
 set_output_branch_address( out_tree, "numMRDTracks", &ev.numMRDTracks_,create,"numMRDTracks/I");

    // Reconstructed vertex information
 set_output_branch_address( out_tree, "recoVtxX", &ev.recoVtxX_,create,"recoVtxX/D" );
 set_output_branch_address( out_tree, "recoVtxY", &ev.recoVtxY_,create,"recoVtxY/D" );
 set_output_branch_address( out_tree, "recoVtxZ", &ev.recoVtxZ_,create,"recoVtxZ/D" );
 set_output_branch_address( out_tree, "recoVtxTime", &ev.recoVtxTime_,create,"recoVtxTime/D" );
 set_output_branch_address( out_tree, "recoDirX", &ev.recoDirX_,create,"recoDirX/D" );
 set_output_branch_address( out_tree, "recoDirY", &ev.recoDirY_,create,"recoDirY/D" );
 set_output_branch_address( out_tree, "recoDirZ", &ev.recoDirZ_,create,"recoDirZ/D" );
 set_output_branch_address( out_tree, "recoAngle", &ev.recoAngle_,create,"recoAngle/D" );
 set_output_branch_address( out_tree, "recoPhi", &ev.recoPhi_,create,"recoPhi/D" );
 set_output_branch_address( out_tree, "recoVtxFOM", &ev.recoVtxFOM_,create,"recoVtxFOM/D" );
 set_output_branch_address( out_tree, "recoStatus", &ev.recoStatus_,create,"recoStatus/D" );

    // Simple reconstruction information
 set_output_branch_address( out_tree,"simpleRecoFlag", &ev.simpleRecoFlag_,create,"simpleRecoFlag/D");
 set_output_branch_address( out_tree,"simpleRecoEnergy", &ev.simpleRecoEnergy_,create,"simpleRecoEnergy/D");
 set_output_branch_address( out_tree,"simpleRecoVtxX", &ev.simpleRecoVtxX_,create,"simpleRecoVtxX/D");
 set_output_branch_address( out_tree,"simpleRecoVtxY", &ev.simpleRecoVtxY_,create,"simpleRecoVtxY/D");
 set_output_branch_address( out_tree,"simpleRecoVtxZ", &ev.simpleRecoVtxZ_,create,"simpleRecoVtxZ/D");
 set_output_branch_address( out_tree,"simpleRecoStopVtxX", &ev.simpleRecoStopVtxX_,create,"simpleRecoStopVtxX/D");
 set_output_branch_address( out_tree,"simpleRecoStopVtxY", &ev.simpleRecoStopVtxY_,create,"simpleRecoStopVtxY/D");
 set_output_branch_address( out_tree,"simpleRecoStopVtxZ", &ev.simpleRecoStopVtxZ_,create,"simpleRecoStopVtxZ/D");
 set_output_branch_address( out_tree,"simpleRecoCosTheta", &ev.simpleRecoCosTheta_,create,"simpleRecoCosTheta/D");
 set_output_branch_address( out_tree,"simpleRecoPt", &ev.simpleRecoPt_,create,"simpleRecoPt/D");
 set_output_branch_address( out_tree,"simpleRecoFV", &ev.simpleRecoFV_,create,"simpleRecoFV/I");
 set_output_branch_address( out_tree,"simpleRecoMrdEnergyLoss", &ev.simpleRecoMrdEnergyLoss_,create,"simpleRecoMrdEnergyLoss/D");
 set_output_branch_address( out_tree,"simpleRecoTrackLengthInMRD", &ev.simpleRecoTrackLengthInMRD_,create,"simpleRecoTrackLengthInMRD/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStartX", &ev.simpleRecoMRDStartX_,create,"simpleRecoMRDStartX/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStartY", &ev.simpleRecoMRDStartY_,create,"simpleRecoMRDStartY/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStartZ", &ev.simpleRecoMRDStartZ_,create,"simpleRecoMRDStartZ/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStopX", &ev.simpleRecoMRDStopX_,create,"simpleRecoMRDStopX/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStopY", &ev.simpleRecoMRDStopY_,create,"simpleRecoMRDStopY/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStopZ", &ev.simpleRecoMRDStopZ_,create,"simpleRecoMRDStopZ/D");
 set_output_branch_address( out_tree,"simpleRecoTrackLengthInTank", &ev.simpleRecoTrackLengthInTank_,create,"simpleRecoTrackLengthInTank/D");
     
 
 if(ev.RCSRPred_)  set_output_branch_address( out_tree,"RCSRPred", &ev.RCSRPred_,create,"RCSRPred/D");
 if(ev.RCMRPred_) set_output_branch_address( out_tree,"RCMRPred", &ev.RCMRPred_,create,"RCMRPred/D");
  set_output_branch_address( out_tree,"triggerNumber", &ev.triggerNumber_,create,"triggerNumber/I");
 set_output_branch_address( out_tree,"mcEntryNumber", &ev.mcEntryNumber_,create,"mcEntryNumber/I");

    // True vertex information
 set_output_branch_address( out_tree,"trueVtxX" , &ev.trueVtxX_,create,"trueVtxX/D");
 set_output_branch_address( out_tree,"trueVtxY" , &ev.trueVtxY_,create,"trueVtxY/D");
 set_output_branch_address( out_tree,"trueVtxZ" , &ev.trueVtxZ_,create,"trueVtxZ/D");
 set_output_branch_address( out_tree,"trueVtxTime" , &ev.trueVtxTime_,create,"trueVtxTime/D");
 set_output_branch_address( out_tree,"trueDirX" , &ev.trueDirX_,create,"trueDirX/D");
 set_output_branch_address( out_tree,"trueDirY" , &ev.trueDirY_,create,"trueDirY/D");
 set_output_branch_address( out_tree,"trueDirZ" , &ev.trueDirZ_,create,"trueDirZ/D");
 set_output_branch_address( out_tree,"trueAngle" , &ev.trueAngle_,create,"trueAngle/D");
 set_output_branch_address( out_tree,"truePhi" , &ev.truePhi_,create,"truePhi/D");
 set_output_branch_address( out_tree,"trueMuonEnergy" , &ev.trueMuonEnergy_,create,"trueMuonEnergy/D");
 
 if(ev.trueQ2_) set_output_branch_address( out_tree,"trueQ2", &ev.trueQ2_,create,"trueQ2/D");
 if(ev.trueBJx_) set_output_branch_address( out_tree,"trueBJx", &ev.trueBJx_,create,"trueBJx/D");
 if(ev.truey_) set_output_branch_address( out_tree,"truey", &ev.truey_,create,"truey/D");
 if(ev.trueq0_) set_output_branch_address( out_tree,"trueq0", &ev.trueq0_,create,"trueq0/D");
 if(ev.trueq3_) set_output_branch_address( out_tree,"trueq3", &ev.trueq3_,create,"trueq3/D");
 if(ev.trueW2_) set_output_branch_address( out_tree,"trueW2", &ev.trueW2_,create,"trueW2/D");
 
 
 set_output_branch_address( out_tree,"trueTrackLengthInWater", &ev.trueTrackLengthInWater_,create,"trueTrackLengthInWater/D");
 set_output_branch_address( out_tree,"trueTrackLengthInMRD", &ev.trueTrackLengthInMRD_,create,"trueTrackLengthInMRD/D");
 set_output_branch_address( out_tree,"trueNeutrinoEnergy", &ev.trueNeutrinoEnergy_,create,"trueNeutrinoEnergy/D");
 set_output_branch_address( out_tree,"trueNeutrinoMomentum_X", &ev.trueNeutrinoMomentum_X_,create,"trueNeutrinoMomentum_X/D");
 set_output_branch_address( out_tree,"trueNeutrinoMomentum_Y", &ev.trueNeutrinoMomentum_Y_,create,"trueNeutrinoMomentum_Y/D");
 set_output_branch_address( out_tree,"trueNeutrinoMomentum_Z", &ev.trueNeutrinoMomentum_Z_,create,"trueNeutrinoMomentum_Z/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_X", &ev.trueNuIntxVtx_X_,create,"trueNuIntxVtx_X/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_Y", &ev.trueNuIntxVtx_Y_,create,"trueNuIntxVtx_Y/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_Z", &ev.trueNuIntxVtx_Z_,create,"trueNuIntxVtx_Z/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_T", &ev.trueNuIntxVtx_T_,create,"trueNuIntxVtx_T/D");
 set_output_branch_address( out_tree,"trueFSLVtx_X", &ev.trueFSLVtx_X_,create,"trueFSLVtx_X/D");
 set_output_branch_address( out_tree,"trueFSLVtx_Y", &ev.trueFSLVtx_Y_,create,"trueFSLVtx_Y/D");
 set_output_branch_address( out_tree,"trueFSLVtx_Z", &ev.trueFSLVtx_Z_,create,"trueFSLVtx_Z/D");
 set_output_branch_address( out_tree,"trueFSLMomentum_X", &ev.trueFSLMomentum_X_,create,"trueFSLMomentum_X/D");
 set_output_branch_address( out_tree,"trueFSLMomentum_Y", &ev.trueFSLMomentum_Y_,create,"trueFSLMomentum_Y/D");
 set_output_branch_address( out_tree,"trueFSLMomentum_Z", &ev.trueFSLMomentum_Z_,create,"trueFSLMomentum_Z/D");
 
 set_output_branch_address( out_tree,"trueTargetZ", &ev.trueTargetZ_,create,"trueTargetZ/I"); 
 set_output_branch_address( out_tree,"trueMultiRing", &ev.trueMultiRing_,create,"trueMultiRing/I");
 set_output_branch_address( out_tree,"truePrimaryPdg", &ev.truePrimaryPdg_,create,"truePrimaryPdg/I");
      // True primary particle information
   
   
 set_output_branch_address( out_tree,"trueNuPDG",&ev.trueNuPDG_,create,"trueNuPDG/I");  
 set_output_branch_address( out_tree,"truePrimaryPdgs",&ev.truePrimaryPdgs_,create,"truePrimaryPdgs/I");
 set_output_branch_address( out_tree,"trueFSLPdg",&ev.trueFSLPdg_,create,"trueFSLPdg/I");
 set_output_branch_address( out_tree,"trueNeutCapVtxX",&ev.trueNeutCapVtxX_,create,"trueNeutCapVtxX/D");
 set_output_branch_address( out_tree,"trueNeutCapVtxY",&ev.trueNeutCapVtxY_,create,"trueNeutCapVtxY/D");
 set_output_branch_address( out_tree,"trueNeutCapVtxZ",&ev.trueNeutCapVtxZ_,create,"trueNeutCapVtxZ/D");
 set_output_branch_address( out_tree,"trueNeutCapNucleus",&ev.trueNeutCapNucleus_,create,"trueNeutCapNucleus/D");
 set_output_branch_address( out_tree,"trueNeutCapTime",&ev.trueNeutCapTime_,create,"trueNeutCapTime/D");
 set_output_branch_address( out_tree,"trueNeutCapGammas",&ev.trueNeutCapGammas_,create,"trueNeutCapGammas/D");
 set_output_branch_address( out_tree,"trueNeutCapE",&ev.trueNeutCapE_,create,"trueNeutCapE/D");
 set_output_branch_address( out_tree,"trueNeutCapGammaE",&ev.trueNeutCapGammaE_,create,"trueNeutCapGammaE/D");
    // True neutrino information
    // Interaction information

 set_output_branch_address( out_tree,"trueFSLTime",&ev.trueFSLTime_,create,"trueFSLTime/D");
 set_output_branch_address( out_tree,"trueFSLMass",&ev.trueFSLMass_,create,"trueFSLMass/D");
 set_output_branch_address( out_tree,"trueFSLEnergy",&ev.trueFSLEnergy_,create,"trueFSLEnergy/D");
    
    
 set_output_branch_address( out_tree,"pointPosX", &ev.pointPosX_,create,"pointPosX/D");
 set_output_branch_address( out_tree,"pointPosY", &ev.pointPosY_,create,"pointPosY/D");
 set_output_branch_address( out_tree,"pointPosZ", &ev.pointPosZ_,create,"pointPosZ/D");
 set_output_branch_address( out_tree,"pointPosTime", &ev.pointPosTime_,create,"pointPosTime/D");
 set_output_branch_address( out_tree,"pointPosFOM", &ev.pointPosFOM_,create,"pointPosFOM/D");
 set_output_branch_address( out_tree,"pointDirX", &ev.pointDirX_,create,"pointDirX/D");
 set_output_branch_address( out_tree,"pointDirY", &ev.pointDirY_,create,"pointDirY/D");
 set_output_branch_address( out_tree,"pointDirZ", &ev.pointDirZ_,create,"pointDirZ/D");
 set_output_branch_address( out_tree,"pointDirTime", &ev.pointDirTime_,create,"pointDirTime/D");
 set_output_branch_address( out_tree,"pointDirStatus", &ev.pointDirStatus_,create,"pointDirStatus/D");
 set_output_branch_address( out_tree,"pointDirFOM", &ev.pointDirFOM_,create,"pointDirFOM/D");
 set_output_branch_address( out_tree,"pointVtxPosX", &ev.pointVtxPosX_,create,"pointVtxPosX/D");
 set_output_branch_address( out_tree,"pointVtxPosY", &ev.pointVtxPosY_,create,"pointVtxPosY/D");
 set_output_branch_address( out_tree,"pointVtxPosZ", &ev.pointVtxPosZ_,create,"pointVtxPosZ/D");
 set_output_branch_address( out_tree,"pointVtxTime", &ev.pointVtxTime_,create,"pointVtxTime/D");
 set_output_branch_address( out_tree,"pointVtxDirX", &ev.pointVtxDirX_,create,"pointVtxDirX/D");
 set_output_branch_address( out_tree,"pointVtxDirY", &ev.pointVtxDirY_,create,"pointVtxDirY/D");
 set_output_branch_address( out_tree,"pointVtxDirZ", &ev.pointVtxDirZ_,create,"pointVtxDirZ/D");
 set_output_branch_address( out_tree,"pointVtxFOM", &ev.pointVtxFOM_,create,"pointVtxFOM/D");
 
 set_output_branch_address( out_tree,"trueMuonMomentum", &ev.trueMuonMomentum_,create,"trueMuonMomentum/D");
 set_output_branch_address( out_tree,"trueCosTheta", &ev.trueCosTheta_,create,"trueCosTheta/D");
 set_output_branch_address( out_tree,"simpleRecoMomentum", &ev.simpleRecoMomentum_,create,"simpleRecoMomentum/D");
 set_output_branch_address( out_tree,"simpleRecoMomentumCor", &ev.simpleRecoMomentumCor_,create,"simpleRecoMomentumCor/D");
 

     // Additional true information
  set_output_branch_address( out_tree,"trueCC",&ev.trueCC_,create,"trueCC/I");
  set_output_branch_address( out_tree,"trueNC",&ev.trueNC_,create,"trueNC/I");
  set_output_branch_address( out_tree,"trueQEL",&ev.trueQEL_,create,"trueQEL/I");
  set_output_branch_address( out_tree,"trueRES",&ev.trueRES_,create,"trueRES/I");
  set_output_branch_address( out_tree,"trueDIS",&ev.trueDIS_,create,"trueDIS/I");
  set_output_branch_address( out_tree,"trueCOH",&ev.trueCOH_,create,"trueCOH/I");
  set_output_branch_address( out_tree,"trueMEC",&ev.trueMEC_,create,"trueMEC/I");
  set_output_branch_address( out_tree,"trueNeutrons",&ev.trueNeutrons_,create,"trueNeutrons/I");
  set_output_branch_address( out_tree,"trueProtons",&ev.trueProtons_,create,"trueProtons/I");
  set_output_branch_address( out_tree,"truePi0",&ev.truePi0_,create,"truePi0/I");
  set_output_branch_address( out_tree,"truePiPlus",&ev.truePiPlus_,create,"truePiPlus/I");
  set_output_branch_address( out_tree,"truePiPlusCher",&ev.truePiPlusCher_,create,"truePiPlusCher/I");
  set_output_branch_address( out_tree,"truePiMinus",&ev.truePiMinus_,create,"truePiMinu/I");
  set_output_branch_address( out_tree,"truePiMinusCher",&ev.truePiMinusCher_,create,"truePiMinusCher/I");
  set_output_branch_address( out_tree,"trueKPlus",&ev.trueKPlus_,create,"trueKPlus/I");
  set_output_branch_address( out_tree,"trueKPlusCher",&ev.trueKPlusCher_,create,"trueKPlusCher/I");
  set_output_branch_address( out_tree,"trueKMinus",&ev.trueKMinus_,create,"trueKMinus/I");
  set_output_branch_address( out_tree,"trueKMinusCher",&ev.trueKMinusCher_,create,"trueKMinusCher/I");
   
  set_output_branch_address( out_tree,"true_no_mesons",&ev.true_no_mesons_,create,"true_no_mesons/O");
  set_output_branch_address( out_tree,"reco0pi_contained_in_MRD",&ev.reco0pi_contained_in_MRD_,create,"reco0pi_contained_in_MRD/O");
  set_output_branch_address( out_tree,"pointVtxStatus",&ev.pointVtxStatus_,create,"pointVtxStatus/I");
  set_output_branch_address( out_tree,"pointPosStatus",&ev.pointPosStatus_,create,"pointPosStatus/I");
  
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_TunedCentralValue_UBGenie",ev.weight_TunedCentralValue_UBGenie_,create);
    
    
     //set_output_branch_address( out_tree,"reco_0pi",&ev.reco_0pi_,create,"reco_0pi/O");
    //set_output_branch_address( out_tree,"recoPE",&ev.recoPE_,create,"recoPE/O");
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_All0_UBGenie",ev.weight_All0_UBGenie_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_All1_UBGenie",ev.weight_All1_UBGenie_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_All2_UBGenie",ev.weight_All2_UBGenie_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_All3_UBGenie",ev.weight_All3_UBGenie_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_All4_UBGenie",ev.weight_All4_UBGenie_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_All5_UBGenie",ev.weight_All5_UBGenie_,create);
  
  
 if ( ExtraUniverses_Tobe_written_TO_ANNE ) {

  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_horncurrent_FluxUnisim",ev.weight_horncurrent_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_expskin_FluxUnisim",ev.weight_expskin_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_piplus_PrimaryHadronSWCentralSplineVariation",ev.weight_piplus_PrimaryHadronSWCentralSplineVariation_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_piminus_PrimaryHadronSWCentralSplineVariation",ev.weight_piminus_PrimaryHadronSWCentralSplineVariation_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_kplus_PrimaryHadronFeynmanScaling",ev.weight_kplus_PrimaryHadronFeynmanScaling_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_kzero_PrimaryHadronSanfordWang",ev.weight_kzero_PrimaryHadronSanfordWang_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_kminus_PrimaryHadronNormalization",ev.weight_kminus_PrimaryHadronNormalization_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_pioninexsec_FluxUnisim",ev.weight_pioninexsec_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_pionqexsec_FluxUnisim",ev.weight_pionqexsec_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_piontotxsec_FluxUnisim",ev.weight_piontotxsec_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_nucleoninexsec_FluxUnisim",ev.weight_nucleoninexsec_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_nucleonqexsec_FluxUnisim",ev.weight_nucleonqexsec_FluxUnisim_,create);
  set_object_output_branch_address< std::vector<double> >( out_tree,"weight_nucleontotxsec_FluxUnisim",ev.weight_nucleontotxsec_FluxUnisim_,create);
 }
 
 
  if(ev.weight_All_UBGenie_ && ev.weight_All_UBGenie_ ){
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_AxFFCCQEshape_UBGenie",ev.weight_AxFFCCQEshape_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_DecayAngMEC_UBGenie",ev.weight_DecayAngMEC_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_NormCCCOH_UBGenie",ev.weight_NormCCCOH_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_NormNCCOH_UBGenie",ev.weight_NormNCCOH_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_RPA_CCQE_UBGenie",ev.weight_RPA_CCQE_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_RootinoFix_UBGenie",ev.weight_RootinoFix_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_ThetaDelta2NRad_UBGenie",ev.weight_ThetaDelta2NRad_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_Theta_Delta2Npi_UBGenie",ev.weight_Theta_Delta2Npi_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_VecFFCCQEshape_UBGenie",ev.weight_VecFFCCQEshape_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"weight_XSecShape_CCMEC_UBGenie",ev.weight_XSecShape_CCMEC_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree, "weight_All_UBGenie",ev.weight_All_UBGenie_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree, "weight_flux_all",ev.weight_flux_all_,create);
 }
 
 set_output_branch_address( out_tree,"seedVtxTime", &ev.seedVtxTime_,create,"seedVtxTime/D");
     
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxX",ev.seedVtxX_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxY",ev.seedVtxY_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxZ",ev.seedVtxZ_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxFOM",ev.seedVtxFOM_,create);

    // Additional true information


 if ( ev.Qij_ ) {
  set_output_branch_address( out_tree,"Qij", &ev.Qij_,create,"Qij/D");
 }
 if ( ev.promptMuonTotalPE_ ) {
  set_output_branch_address( out_tree,"promptMuonTotalPE", &ev.promptMuonTotalPE_,create,"promptMuonTotalPE/D");
 }


    // Weights (continued)
  




}
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
void set_event_branch_addresses(TTree& etree, AnalysisANNIE_Event_nuwro& ev)
{
   //std::cout<<" "<< std::endl;
   //std::cout<<"Inside :: set_event_branch_addresses"<< std::endl;

    etree.SetBranchAddress("runNumber",    &ev.runNumber_);
    etree.SetBranchAddress("subrunNumber", &ev.subrunNumber_);
    etree.SetBranchAddress("runType",      &ev.runType_);
    //etree.SetBranchAddress("startTime",    &ev.startTime_);
    etree.SetBranchAddress("eventNumber",  &ev.eventNumber_);
    //etree.SetBranchAddress("eventTimeTank", &ev.eventTimeTank_);
    //etree.SetBranchAddress("eventTimeMRD",  &ev.eventTimeMRD_);
    etree.SetBranchAddress("nhits",         &ev.nhits_);
    etree.SetBranchAddress("trigword",      &ev.trigword_);
    etree.SetBranchAddress("HasTank",       &ev.HasTank_);
    etree.SetBranchAddress("HasMRD",        &ev.HasMRD_);
    etree.SetBranchAddress("TankMRDCoinc",  &ev.TankMRDCoinc_);
    etree.SetBranchAddress("NoVeto",        &ev.NoVeto_);
    etree.SetBranchAddress("Extended",      &ev.Extended_);
    etree.SetBranchAddress("beam_pot",      &ev.beam_pot_);
    etree.SetBranchAddress("beam_ok",       &ev.beam_ok_);



    // MRD track information
    etree.SetBranchAddress("numMRDTracks", &ev.numMRDTracks_);
    set_object_input_branch_address( etree,"MRDTrackAngle", ev.MRDTrackAngle_);
    set_object_input_branch_address( etree,"MRDTrackAngleError", ev.MRDTrackAngleError_);
    set_object_input_branch_address( etree,"MRDPenetrationDepth", ev.MRDPenetrationDepth_);
    set_object_input_branch_address( etree,"MRDTrackLength", ev.MRDTrackLength_);
    set_object_input_branch_address( etree,"MRDEntryPointRadius", ev.MRDEntryPointRadius_);
    set_object_input_branch_address( etree,"MRDEnergyLoss", ev.MRDEnergyLoss_);
    set_object_input_branch_address( etree,"MRDEnergyLossError", ev.MRDEnergyLossError_);
    set_object_input_branch_address( etree,"MRDTrackStartX", ev.MRDTrackStartX_);
    set_object_input_branch_address( etree,"MRDTrackStartY", ev.MRDTrackStartY_);
    set_object_input_branch_address( etree,"MRDTrackStartZ", ev.MRDTrackStartZ_);
    set_object_input_branch_address( etree,"MRDTrackStopX", ev.MRDTrackStopX_);
    set_object_input_branch_address( etree,"MRDTrackStopY", ev.MRDTrackStopY_);
    set_object_input_branch_address( etree,"MRDTrackStopZ", ev.MRDTrackStopZ_);
     set_object_input_branch_address( etree,"MRDSide", ev.MRDSide_);
    //set_object_input_branch_address( etree,"MRDStop", ev.MRDStop);
    //set_object_input_branch_address( etree,"MRDThrough", ev.MRDThrough);
    
    set_object_input_branch_address( etree,"MRDStop", ev.MRDStop_);
    // Reconstructed vertex information
     etree.SetBranchAddress("recoVtxX", &ev.recoVtxX_);
     etree.SetBranchAddress("recoVtxY", &ev.recoVtxY_);
     etree.SetBranchAddress("recoVtxZ", &ev.recoVtxZ_);
     etree.SetBranchAddress("recoVtxTime", &ev.recoVtxTime_);
     etree.SetBranchAddress("recoDirX", &ev.recoDirX_);
     etree.SetBranchAddress("recoDirY", &ev.recoDirY_);
     etree.SetBranchAddress("recoDirZ", &ev.recoDirZ_);
     etree.SetBranchAddress("recoAngle", &ev.recoAngle_);
     etree.SetBranchAddress("recoPhi", &ev.recoPhi_);
     etree.SetBranchAddress("recoVtxFOM", &ev.recoVtxFOM_);
     etree.SetBranchAddress("recoStatus", &ev.recoStatus_);

    // Simple reconstruction information
    etree.SetBranchAddress("simpleRecoFlag", &ev.simpleRecoFlag_);
    etree.SetBranchAddress("simpleRecoEnergy", &ev.simpleRecoEnergy_);
    etree.SetBranchAddress("simpleRecoVtxX", &ev.simpleRecoVtxX_);
    etree.SetBranchAddress("simpleRecoVtxY", &ev.simpleRecoVtxY_);
    etree.SetBranchAddress("simpleRecoVtxZ", &ev.simpleRecoVtxZ_);
    etree.SetBranchAddress("simpleRecoStopVtxX", &ev.simpleRecoStopVtxX_);
    etree.SetBranchAddress("simpleRecoStopVtxY", &ev.simpleRecoStopVtxY_);
    etree.SetBranchAddress("simpleRecoStopVtxZ", &ev.simpleRecoStopVtxZ_);
    etree.SetBranchAddress("simpleRecoCosTheta", &ev.simpleRecoCosTheta_);
    etree.SetBranchAddress("simpleRecoPt", &ev.simpleRecoPt_);
    etree.SetBranchAddress("simpleRecoFV", &ev.simpleRecoFV_);
    etree.SetBranchAddress("simpleRecoMrdEnergyLoss", &ev.simpleRecoMrdEnergyLoss_);
    etree.SetBranchAddress("simpleRecoTrackLengthInMRD", &ev.simpleRecoTrackLengthInMRD_);
    etree.SetBranchAddress("simpleRecoMRDStartX", &ev.simpleRecoMRDStartX_);
    etree.SetBranchAddress("simpleRecoMRDStartY", &ev.simpleRecoMRDStartY_);
    etree.SetBranchAddress("simpleRecoMRDStartZ", &ev.simpleRecoMRDStartZ_);
    etree.SetBranchAddress("simpleRecoMRDStopX", &ev.simpleRecoMRDStopX_);
    etree.SetBranchAddress("simpleRecoMRDStopY", &ev.simpleRecoMRDStopY_);
    etree.SetBranchAddress("simpleRecoMRDStopZ", &ev.simpleRecoMRDStopZ_);
    etree.SetBranchAddress("simpleRecoTrackLengthInTank", &ev.simpleRecoTrackLengthInTank_);

    // Additional event information
    
    bool RCSRPred_check = ( etree.GetBranch("RCSRPred") != nullptr );
    bool RCMRPred_check = ( etree.GetBranch("RCMRPred") != nullptr );
    
    if(RCSRPred_check) etree.SetBranchAddress("RCSRPred", &ev.RCSRPred_);
    if(RCMRPred_check) etree.SetBranchAddress("RCMRPred", &ev.RCMRPred_);
    
    etree.SetBranchAddress("triggerNumber", &ev.triggerNumber_);
    etree.SetBranchAddress("mcEntryNumber", &ev.mcEntryNumber_);

    // True vertex information
    
    etree.SetBranchAddress("trueNuPDG", &ev.trueNuPDG_);
    etree.SetBranchAddress("trueVtxX", &ev.trueVtxX_);
    etree.SetBranchAddress("trueVtxY", &ev.trueVtxY_);
    etree.SetBranchAddress("trueVtxZ", &ev.trueVtxZ_);
    etree.SetBranchAddress("trueVtxTime", &ev.trueVtxTime_);
    etree.SetBranchAddress("trueDirX", &ev.trueDirX_);
    etree.SetBranchAddress("trueDirY", &ev.trueDirY_);
    etree.SetBranchAddress("trueDirZ", &ev.trueDirZ_);
    etree.SetBranchAddress("trueAngle", &ev.trueAngle_);
    etree.SetBranchAddress("truePhi", &ev.truePhi_);
    etree.SetBranchAddress("trueMuonEnergy", &ev.trueMuonEnergy_);
    etree.SetBranchAddress("truePrimaryPdg", &ev.truePrimaryPdg_);
    etree.SetBranchAddress("trueTrackLengthInWater", &ev.trueTrackLengthInWater_);
    etree.SetBranchAddress("trueTrackLengthInMRD", &ev.trueTrackLengthInMRD_);
    etree.SetBranchAddress("trueMultiRing", &ev.trueMultiRing_);

    // Particle counts
    etree.SetBranchAddress("Pi0Count", &ev.Pi0Count_);
    etree.SetBranchAddress("PiPlusCount", &ev.PiPlusCount_);
    etree.SetBranchAddress("PiMinusCount", &ev.PiMinusCount_);
    etree.SetBranchAddress("K0Count", &ev.K0Count_);
    etree.SetBranchAddress("KPlusCount", &ev.KPlusCount_);
    etree.SetBranchAddress("KMinusCount", &ev.KMinusCount_);

    // True primary particle information
    set_object_input_branch_address( etree,"truePrimaryPdgs", ev.truePrimaryPdgs_);
    set_object_input_branch_address( etree,"trueNeutCapVtxX", ev.trueNeutCapVtxX_);
    set_object_input_branch_address( etree,"trueNeutCapVtxY", ev.trueNeutCapVtxY_);
    set_object_input_branch_address( etree,"trueNeutCapVtxZ", ev.trueNeutCapVtxZ_);
    set_object_input_branch_address( etree,"trueNeutCapNucleus", ev.trueNeutCapNucleus_);
    set_object_input_branch_address( etree,"trueNeutCapTime", ev.trueNeutCapTime_);
    set_object_input_branch_address( etree,"trueNeutCapGammas", ev.trueNeutCapGammas_);
    set_object_input_branch_address( etree,"trueNeutCapE", ev.trueNeutCapE_);
    set_object_input_branch_address( etree,"trueNeutCapGammaE", ev.trueNeutCapGammaE_);

    // True neutrino information
    etree.SetBranchAddress("trueNeutrinoEnergy", &ev.trueNeutrinoEnergy_);
    etree.SetBranchAddress("trueNeutrinoMomentum_X", &ev.trueNeutrinoMomentum_X_);
    etree.SetBranchAddress("trueNeutrinoMomentum_Y", &ev.trueNeutrinoMomentum_Y_);
    etree.SetBranchAddress("trueNeutrinoMomentum_Z", &ev.trueNeutrinoMomentum_Z_);
    etree.SetBranchAddress("trueNuIntxVtx_X", &ev.trueNuIntxVtx_X_);
    etree.SetBranchAddress("trueNuIntxVtx_Y", &ev.trueNuIntxVtx_Y_);
    etree.SetBranchAddress("trueNuIntxVtx_Z", &ev.trueNuIntxVtx_Z_);
    //etree.SetBranchAddress("trueNuIntxVtx_T", &ev.trueNuIntxVtx_T_);
    //etree.SetBranchAddress("trueFSLVtx_X", &ev.trueFSLVtx_X_);
    //etree.SetBranchAddress("trueFSLVtx_Y", &ev.trueFSLVtx_Y_);
    //etree.SetBranchAddress("trueFSLVtx_Z", &ev.trueFSLVtx_Z_);
    etree.SetBranchAddress("trueFSLMomentum_X", &ev.trueFSLMomentum_X_);
    etree.SetBranchAddress("trueFSLMomentum_Y", &ev.trueFSLMomentum_Y_);
    etree.SetBranchAddress("trueFSLMomentum_Z", &ev.trueFSLMomentum_Z_);
    //etree.SetBranchAddress("trueFSLTime", &ev.trueFSLTime_);
    //etree.SetBranchAddress("trueFSLMass", &ev.trueFSLMass_);
    etree.SetBranchAddress("trueFSLPdg", &ev.trueFSLPdg_);
    etree.SetBranchAddress("trueFSLEnergy", &ev.trueFSLEnergy_);

    // Interaction information
    etree.SetBranchAddress("trueQ2", &ev.trueQ2_);
 
    etree.SetBranchAddress("IsCCINC", &ev.trueCC_);
    //etree.SetBranchAddress("trueCC", &ev.trueCC_);
    etree.SetBranchAddress("IsNC0pi", &ev.trueNC_);
    
    etree.SetBranchAddress("trueNeutCode", &ev.trueNeutCode_);
    
    
    etree.SetBranchAddress("IsCCQE", &ev.trueQEL_);
    //etree.SetBranchAddress("trueRES", &ev.trueRES_);
    //etree.SetBranchAddress("trueDIS", &ev.trueDIS_);
    //etree.SetBranchAddress("trueCOH", &ev.trueCOH_);
    //etree.SetBranchAddress("trueMEC", &ev.trueMEC_);
    etree.SetBranchAddress("trueNeutrons", &ev.trueNeutrons_);
    etree.SetBranchAddress("trueProtons", &ev.trueProtons_);
    etree.SetBranchAddress("truePi0", &ev.truePi0_);
    etree.SetBranchAddress("truePiPlus", &ev.truePiPlus_);
    etree.SetBranchAddress("truePiPlusCher", &ev.truePiPlusCher_);
    etree.SetBranchAddress("truePiMinus", &ev.truePiMinus_);
    etree.SetBranchAddress("truePiMinusCher", &ev.truePiMinusCher_);
    etree.SetBranchAddress("trueKPlus", &ev.trueKPlus_);
    etree.SetBranchAddress("trueKPlusCher", &ev.trueKPlusCher_);
    etree.SetBranchAddress("trueKMinus", &ev.trueKMinus_);
    etree.SetBranchAddress("trueKMinusCher", &ev.trueKMinusCher_);
    etree.SetBranchAddress("trueTargetZ", &ev.trueTargetZ_);
    /*
    SetBranchAddress( etree,"trueNC", &ev.trueNC_);
    SetBranchAddress( etree,"trueQEL", &ev.trueQEL_);
    SetBranchAddress( etree,"trueRES", &ev.trueRES_);
    SetBranchAddress( etree,"trueDIS", &ev.trueDIS_);
    SetBranchAddress( etree,"trueCOH", &ev.trueCOH_);
    SetBranchAddress( etree,"trueMEC", &ev.trueMEC_);
    SetBranchAddress( etree,"trueNeutrons", &ev.trueNeutrons_);
    SetBranchAddress( etree,"trueProtons", &ev.trueProtons_);
    SetBranchAddress( etree,"truePi0", &ev.truePi0_);
    SetBranchAddress( etree,"truePiPlus", &ev.truePiPlus_);
    SetBranchAddress( etree,"truePiPlusCher", &ev.truePiPlusCher_);
    SetBranchAddress( etree,"truePiMinus", &ev.truePiMinus_);
    SetBranchAddress( etree,"truePiMinusCher", &ev.truePiMinusCher_);
    SetBranchAddress( etree,"trueKPlus", &ev.trueKPlus_);
    SetBranchAddress( etree,"trueKPlusCher", &ev.trueKPlusCher_);
    SetBranchAddress( etree,"trueKMinus", &ev.trueKMinus_);
    SetBranchAddress( etree,"trueKMinusCher", &ev.trueKMinusCher_);
 */
    // Additional true information
    etree.SetBranchAddress("trueBJx", &ev.trueBJx_);
    etree.SetBranchAddress("truey", &ev.truey_);
    etree.SetBranchAddress("trueq0", &ev.trueq0_);
    etree.SetBranchAddress("trueq3", &ev.trueq3_);
    etree.SetBranchAddress("trueW2", &ev.trueW2_);
    etree.SetBranchAddress("fScaleFactor", &ev.fScaleFactor_);
    etree.SetBranchAddress("Qij", &ev.Qij_);
    
 //    etree.SetBranchAddress("true_no_followers", &ev.true_no_followers_);

    // if(etree.GetBranch("Qij") != nullptr ){
    //     etree.SetBranchAddress("Qij", &ev.Qij_); filling this now 
    //   }

    if(etree.GetBranch("promptMuonTotalPE") != nullptr ){
     etree.SetBranchAddress("promptMuonTotalPE", &ev.promptMuonTotalPE_);
    }
 
 
     bool Check_trueFollowerParentPDG = ( etree.GetBranch("trueFollowerParentPDG") != nullptr );
    if(Check_trueFollowerParentPDG){
    //set_object_input_branch_address( etree,"MRDStop", ev.MRDStop_);
    
    auto oldLvl = gErrorIgnoreLevel;
     gErrorIgnoreLevel = kFatal;               
    set_object_input_branch_address( etree,"trueFollowerParentPDG", ev.trueFollowerParentPDG_);
     gErrorIgnoreLevel = oldLvl;
     }
    // Weights
    
    
    set_object_input_branch_address( etree,"hitQ", ev.hitQ_);
    set_object_input_branch_address( etree,"hitT", ev.hitT_);
    set_object_input_branch_address( etree,"hitX", ev.hitX_);
    set_object_input_branch_address( etree,"hitY", ev.hitY_);
    set_object_input_branch_address( etree,"hitZ", ev.hitZ_);
    set_object_input_branch_address( etree,"hitPE", ev.hitPE_);
    set_object_input_branch_address( etree,"hitDetID", ev.hitDetID_);
    
    
       
    

    // Seed vertex information
    set_object_input_branch_address( etree,"seedVtxX", ev.seedVtxX_);
    set_object_input_branch_address( etree,"seedVtxY", ev.seedVtxY_);
    set_object_input_branch_address( etree,"seedVtxZ", ev.seedVtxZ_);
    set_object_input_branch_address( etree,"seedVtxFOM", ev.seedVtxFOM_);
    etree.SetBranchAddress("seedVtxTime", &ev.seedVtxTime_);

    // Point information
    SetBranchAddress(etree,"pointPosX", &ev.pointPosX_);
    SetBranchAddress(etree,"pointPosY", &ev.pointPosY_);
    SetBranchAddress(etree,"pointPosZ", &ev.pointPosZ_);
    SetBranchAddress(etree,"pointPosTime", &ev.pointPosTime_);
    SetBranchAddress(etree,"pointPosFOM", &ev.pointPosFOM_);
    SetBranchAddress(etree,"pointPosStatus", &ev.pointPosStatus_);
    SetBranchAddress(etree,"pointDirX", &ev.pointDirX_);
    SetBranchAddress(etree,"pointDirY", &ev.pointDirY_);
    SetBranchAddress(etree,"pointDirZ", &ev.pointDirZ_);
    SetBranchAddress(etree,"pointDirTime", &ev.pointDirTime_);
    SetBranchAddress(etree,"pointDirStatus", &ev.pointDirStatus_);
    SetBranchAddress(etree,"pointDirFOM", &ev.pointDirFOM_);
    SetBranchAddress(etree,"pointVtxPosX", &ev.pointVtxPosX_);
    SetBranchAddress(etree,"pointVtxPosY", &ev.pointVtxPosY_);
    SetBranchAddress(etree,"pointVtxPosZ", &ev.pointVtxPosZ_);
    SetBranchAddress(etree,"pointVtxTime", &ev.pointVtxTime_);
    SetBranchAddress(etree,"pointVtxDirX", &ev.pointVtxDirX_);
    SetBranchAddress(etree,"pointVtxDirY", &ev.pointVtxDirY_);
    SetBranchAddress(etree,"pointVtxDirZ", &ev.pointVtxDirZ_);
    SetBranchAddress(etree,"pointVtxFOM", &ev.pointVtxFOM_);
    SetBranchAddress(etree,"pointVtxStatus", &ev.pointVtxStatus_);
    
    //etree.SetBranchAddress("reco0pi_contained_in_MRD", &ev.reco0pi_contained_in_MRD_);
    
    // Fiducial volume flags
    //etree.SetBranchAddress("trueFV", &ev.trueFV_);
    //etree.SetBranchAddress("recoFV", &ev.recoFV_);

    // Additional true information
    //etree.SetBranchAddress("trueMuonMomentum", &ev.trueMuonMomentum_);
    //etree.SetBranchAddress("trueCosTheta", &ev.trueCosTheta_);
    //etree.SetBranchAddress("simpleRecoMomentum", &ev.simpleRecoMomentum_);
    //etree.SetBranchAddress("simpleRecoMomentumCor", &ev.simpleRecoMomentumCor_);
    //etree.SetBranchAddress("true_no_mesons", &ev.true_no_mesons_);
    //etree.SetBranchAddress("reco_contained_in_MRD", &ev.reco_contained_in_MRD_);

     //std::cout<<" "<< std::endl;

}
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
void set_event_output_branch_addresses(TTree& out_tree, AnalysisANNIE_Event_nuwro& ev,
  bool create = false)
{
  //std::cout<<" "<< std::endl;
  //std::cout<<"Inside :: set_event_output_branch_addresses"<< std::endl;
    
 set_output_branch_address( out_tree, "runNumber", &ev.runNumber_,create,"runNumber/I");
 set_output_branch_address( out_tree, "subrunNumber", &ev.subrunNumber_,create,"subrunNumber/I");
 set_output_branch_address( out_tree, "runType", &ev.runType_,create,"runType/I");
 //set_output_branch_address( out_tree, "startTime", &ev.startTime_,create,"startTime/I");
 set_output_branch_address( out_tree, "eventNumber", &ev.eventNumber_,create,"eventNumber/I");
 //set_output_branch_address( out_tree, "eventTimeTank", &ev.eventTimeTank_,create,"eventTimeTank/I");
 //set_output_branch_address( out_tree, "eventTimeMRD", &ev.eventTimeMRD_,create,"eventTimeMRD/I");
 set_output_branch_address( out_tree, "Pi0Count", &ev.Pi0Count_,create,"Pi0Count/I");
 set_output_branch_address( out_tree, "PiPlusCount", &ev.PiPlusCount_,create,"PiPlusCount/I");
 set_output_branch_address( out_tree, "PiMinusCount", &ev.PiMinusCount_,create,"PiMinusCount/I");
 set_output_branch_address( out_tree, "K0Count", &ev.K0Count_,create,"K0Count/I");
 set_output_branch_address( out_tree, "KPlusCount", &ev.KPlusCount_,create,"KPlusCount/I");
 set_output_branch_address( out_tree, "KMinusCount", &ev.KMinusCount_,create,"KMinusCount/I");
 //branches not in Nuwro file
 //set_output_branch_address( out_tree, "true_no_followers", &ev.true_no_followers_,create,"true_no_followers/O");
 //set_output_branch_address( out_tree, "trueFV", &ev.trueFV_,create,"trueFV/O");
 //set_output_branch_address( out_tree, "recoFV", &ev.recoFV_,create,"recoFV/O");
     
 set_output_branch_address( out_tree, "nhits",&ev.nhits_,create,"nhits/I");
 set_output_branch_address( out_tree, "trigword",&ev.trigword_,create,"trigword/I");
 set_output_branch_address( out_tree, "HasTank",&ev.HasTank_,create,"HasTank/I");
 set_output_branch_address( out_tree, "HasMRD",&ev.HasMRD_,create,"HasMRD/I");
 set_output_branch_address( out_tree, "TankMRDCoinc",&ev.TankMRDCoinc_,create,"TankMRDCoinc/I");
 set_output_branch_address( out_tree, "NoVeto",&ev.NoVeto_,create,"NoVeto/I");
 set_output_branch_address( out_tree, "Extended",&ev.Extended_,create,"Extended/I");
 set_output_branch_address( out_tree, "beam_ok",&ev.beam_ok_,create,"beam_ok/I");
 set_output_branch_address( out_tree,"beam_pot", &ev.beam_pot_,create,"beam_pot/D");
  
 set_output_branch_address( out_tree, "MRDTrackAngle",&ev.MRDTrackAngle_,create,"MRDTrackAngle/D");
 set_output_branch_address( out_tree, "MRDTrackAngleError",&ev.MRDTrackAngleError_,create,"MRDTrackAngleError/D");
 set_output_branch_address( out_tree, "MRDPenetrationDepth",&ev.MRDPenetrationDepth_,create,"MRDPenetrationDepth/D");
 set_output_branch_address( out_tree, "MRDTrackLength",&ev.MRDTrackLength_,create,"MRDTrackLength/D");
 set_output_branch_address( out_tree, "MRDEntryPointRadius",&ev.MRDEntryPointRadius_,create,"MRDEntryPointRadius/D");
 set_output_branch_address( out_tree, "MRDEnergyLoss",&ev.MRDEnergyLoss_,create,"MRDEnergyLoss/D");
 set_output_branch_address( out_tree, "MRDEnergyLossError",&ev.MRDEnergyLossError_,create,"MRDEnergyLossError/D");
 set_output_branch_address( out_tree, "MRDTrackStartX",&ev.MRDTrackStartX_,create,"MRDTrackStartX/D");
 set_output_branch_address( out_tree, "MRDTrackStartY",&ev.MRDTrackStartY_,create,"MRDTrackStartY/D");
 set_output_branch_address( out_tree, "MRDTrackStartZ",&ev.MRDTrackStartZ_,create,"MRDTrackStartZ/D");
 set_output_branch_address( out_tree, "MRDTrackStopX",&ev.MRDTrackStopX_,create,"MRDTrackStopX/D");
 set_output_branch_address( out_tree, "MRDTrackStopY",&ev.MRDTrackStopY_,create,"MRDTrackStopY/D");
 set_output_branch_address( out_tree, "MRDTrackStopZ",&ev.MRDTrackStopZ_,create,"MRDTrackStopZ/D");
 //set_object_output_branch_address< std::vector<bool> >( out_tree, "MRDStop",ev.MRDStop_,create);
 //set_object_output_branch_address< std::vector<bool> >( out_tree, "MRDSide",ev.MRDSide_,create);
 set_output_branch_address( out_tree, "MRDSide", &ev.MRDSide_,create,"MRDSide/I");
 set_output_branch_address( out_tree, "MRDStop", &ev.MRDStop_,create,"MRDStop/I");
 set_output_branch_address( out_tree, "MRDThrough", &ev.MRDThrough_,create,"MRDThrough/I");
 set_output_branch_address( out_tree, "numMRDTracks", &ev.numMRDTracks_,create,"numMRDTracks/I");
 // set_object_output_branch_address< std::vector<int> >( out_tree, "trueFollowerParentPDG",ev.trueFollowerParentPDG_);



    // Reconstructed vertex information
 set_output_branch_address( out_tree, "recoVtxX", &ev.recoVtxX_,create,"recoVtxX/D" );
 set_output_branch_address( out_tree, "recoVtxY", &ev.recoVtxY_,create,"recoVtxY/D" );
 set_output_branch_address( out_tree, "recoVtxZ", &ev.recoVtxZ_,create,"recoVtxZ/D" );
 set_output_branch_address( out_tree, "recoVtxTime", &ev.recoVtxTime_,create,"recoVtxTime/D" );
 set_output_branch_address( out_tree, "recoDirX", &ev.recoDirX_,create,"recoDirX/D" );
 set_output_branch_address( out_tree, "recoDirY", &ev.recoDirY_,create,"recoDirY/D" );
 set_output_branch_address( out_tree, "recoDirZ", &ev.recoDirZ_,create,"recoDirZ/D" );
 set_output_branch_address( out_tree, "recoAngle", &ev.recoAngle_,create,"recoAngle/D" );
 set_output_branch_address( out_tree, "recoPhi", &ev.recoPhi_,create,"recoPhi/D" );
 set_output_branch_address( out_tree, "recoVtxFOM", &ev.recoVtxFOM_,create,"recoVtxFOM/D" );
 set_output_branch_address( out_tree, "recoStatus", &ev.recoStatus_,create,"recoStatus/D" );

    // Simple reconstruction information
 set_output_branch_address( out_tree,"simpleRecoFlag", &ev.simpleRecoFlag_,create,"simpleRecoFlag/D");
 set_output_branch_address( out_tree,"simpleRecoEnergy", &ev.simpleRecoEnergy_,create,"simpleRecoEnergy/D");
 set_output_branch_address( out_tree,"simpleRecoVtxX", &ev.simpleRecoVtxX_,create,"simpleRecoVtxX/D");
 set_output_branch_address( out_tree,"simpleRecoVtxY", &ev.simpleRecoVtxY_,create,"simpleRecoVtxY/D");
 set_output_branch_address( out_tree,"simpleRecoVtxZ", &ev.simpleRecoVtxZ_,create,"simpleRecoVtxZ/D");
 set_output_branch_address( out_tree,"simpleRecoStopVtxX", &ev.simpleRecoStopVtxX_,create,"simpleRecoStopVtxX/D");
 set_output_branch_address( out_tree,"simpleRecoStopVtxY", &ev.simpleRecoStopVtxY_,create,"simpleRecoStopVtxY/D");
 set_output_branch_address( out_tree,"simpleRecoStopVtxZ", &ev.simpleRecoStopVtxZ_,create,"simpleRecoStopVtxZ/D");
 set_output_branch_address( out_tree,"simpleRecoCosTheta", &ev.simpleRecoCosTheta_,create,"simpleRecoCosTheta/D");
 set_output_branch_address( out_tree,"simpleRecoPt", &ev.simpleRecoPt_,create,"simpleRecoPt/D");
 set_output_branch_address( out_tree,"simpleRecoFV", &ev.simpleRecoFV_,create,"simpleRecoFV/I");
 set_output_branch_address( out_tree,"simpleRecoMrdEnergyLoss", &ev.simpleRecoMrdEnergyLoss_,create,"simpleRecoMrdEnergyLoss/D");
 set_output_branch_address( out_tree,"simpleRecoTrackLengthInMRD", &ev.simpleRecoTrackLengthInMRD_,create,"simpleRecoTrackLengthInMRD/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStartX", &ev.simpleRecoMRDStartX_,create,"simpleRecoMRDStartX/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStartY", &ev.simpleRecoMRDStartY_,create,"simpleRecoMRDStartY/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStartZ", &ev.simpleRecoMRDStartZ_,create,"simpleRecoMRDStartZ/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStopX", &ev.simpleRecoMRDStopX_,create,"simpleRecoMRDStopX/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStopY", &ev.simpleRecoMRDStopY_,create,"simpleRecoMRDStopY/D");
 set_output_branch_address( out_tree,"simpleRecoMRDStopZ", &ev.simpleRecoMRDStopZ_,create,"simpleRecoMRDStopZ/D");
 set_output_branch_address( out_tree,"simpleRecoTrackLengthInTank", &ev.simpleRecoTrackLengthInTank_,create,"simpleRecoTrackLengthInTank/D");
     
 
 if(ev.RCSRPred_) set_output_branch_address( out_tree,"RCSRPred", &ev.RCSRPred_,create,"RCSRPred/D");
 if(ev.RCMRPred_) set_output_branch_address( out_tree,"RCMRPred", &ev.RCMRPred_,create,"RCMRPred/D");
 set_output_branch_address( out_tree,"triggerNumber", &ev.triggerNumber_,create,"triggerNumber/I");
 set_output_branch_address( out_tree,"mcEntryNumber", &ev.mcEntryNumber_,create,"mcEntryNumber/I");

    // True vertex information
    

 set_output_branch_address( out_tree,"trueNuPDG" , &ev.trueNuPDG_,create,"trueNuPDG/D");
 set_output_branch_address( out_tree,"trueVtxX" , &ev.trueVtxX_,create,"trueVtxX/D");
 set_output_branch_address( out_tree,"trueVtxY" , &ev.trueVtxY_,create,"trueVtxY/D");
 set_output_branch_address( out_tree,"trueVtxZ" , &ev.trueVtxZ_,create,"trueVtxZ/D");
 set_output_branch_address( out_tree,"trueVtxTime" , &ev.trueVtxTime_,create,"trueVtxTime/D");
 set_output_branch_address( out_tree,"trueDirX" , &ev.trueDirX_,create,"trueDirX/D");
 set_output_branch_address( out_tree,"trueDirY" , &ev.trueDirY_,create,"trueDirY/D");
 set_output_branch_address( out_tree,"trueDirZ" , &ev.trueDirZ_,create,"trueDirZ/D");
 set_output_branch_address( out_tree,"trueAngle" , &ev.trueAngle_,create,"trueAngle/D");
 set_output_branch_address( out_tree,"truePhi" , &ev.truePhi_,create,"truePhi/D");
 set_output_branch_address( out_tree,"trueMuonEnergy" , &ev.trueMuonEnergy_,create,"trueMuonEnergy/D");
 
 set_output_branch_address( out_tree,"trueQ2", &ev.trueQ2_,create,"trueQ2/D");
 set_output_branch_address( out_tree,"trueBJx", &ev.trueBJx_,create,"trueBJx/D");
 set_output_branch_address( out_tree,"truey", &ev.truey_,create,"truey/D");
 set_output_branch_address( out_tree,"trueq0", &ev.trueq0_,create,"trueq0/D");
 set_output_branch_address( out_tree,"trueq3", &ev.trueq3_,create,"trueq3/D");
 set_output_branch_address( out_tree,"trueW2", &ev.trueW2_,create,"trueW2/D");
 set_output_branch_address( out_tree,"trueTrackLengthInWater", &ev.trueTrackLengthInWater_,create,"trueTrackLengthInWater/D");
 set_output_branch_address( out_tree,"trueTrackLengthInMRD", &ev.trueTrackLengthInMRD_,create,"trueTrackLengthInMRD/D");
 set_output_branch_address( out_tree,"trueNeutrinoEnergy", &ev.trueNeutrinoEnergy_,create,"trueNeutrinoEnergy/D");
 set_output_branch_address( out_tree,"trueNeutrinoMomentum_X", &ev.trueNeutrinoMomentum_X_,create,"trueNeutrinoMomentum_X/D");
 set_output_branch_address( out_tree,"trueNeutrinoMomentum_Y", &ev.trueNeutrinoMomentum_Y_,create,"trueNeutrinoMomentum_Y/D");
 set_output_branch_address( out_tree,"trueNeutrinoMomentum_Z", &ev.trueNeutrinoMomentum_Z_,create,"trueNeutrinoMomentum_Z/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_X", &ev.trueNuIntxVtx_X_,create,"trueNuIntxVtx_X/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_Y", &ev.trueNuIntxVtx_Y_,create,"trueNuIntxVtx_Y/D");
 set_output_branch_address( out_tree,"trueNuIntxVtx_Z", &ev.trueNuIntxVtx_Z_,create,"trueNuIntxVtx_Z/D");
 //set_output_branch_address( out_tree,"trueNuIntxVtx_T", &ev.trueNuIntxVtx_T_,create,"trueNuIntxVtx_T/D");
 // set_output_branch_address( out_tree,"trueFSLVtx_X", &ev.trueFSLVtx_X_,create,"trueFSLVtx_X/D");
 // set_output_branch_address( out_tree,"trueFSLVtx_Y", &ev.trueFSLVtx_Y_,create,"trueFSLVtx_Y/D");
 // set_output_branch_address( out_tree,"trueFSLVtx_Z", &ev.trueFSLVtx_Z_,create,"trueFSLVtx_Z/D");
 set_output_branch_address( out_tree,"trueFSLMomentum_X", &ev.trueFSLMomentum_X_,create,"trueFSLMomentum_X/D");
 set_output_branch_address( out_tree,"trueFSLMomentum_Y", &ev.trueFSLMomentum_Y_,create,"trueFSLMomentum_Y/D");
 set_output_branch_address( out_tree,"trueFSLMomentum_Z", &ev.trueFSLMomentum_Z_,create,"trueFSLMomentum_Z/D");
 
 set_output_branch_address( out_tree,"trueTargetZ", &ev.trueTargetZ_,create,"trueTargetZ/I"); 
 set_output_branch_address( out_tree,"trueMultiRing", &ev.trueMultiRing_,create,"trueMultiRing/I");
 set_output_branch_address( out_tree,"truePrimaryPdg", &ev.truePrimaryPdg_,create,"truePrimaryPdg/I");
      // True primary particle information
   
 set_output_branch_address( out_tree,"truePrimaryPdgs",&ev.truePrimaryPdgs_,create,"truePrimaryPdgs/I");
 set_output_branch_address( out_tree,"trueFSLPdg",&ev.trueFSLPdg_,create,"trueFSLPdg/I");
 set_output_branch_address( out_tree,"trueNeutCapVtxX",&ev.trueNeutCapVtxX_,create,"trueNeutCapVtxX/D");
 set_output_branch_address( out_tree,"trueNeutCapVtxY",&ev.trueNeutCapVtxY_,create,"trueNeutCapVtxY/D");
 set_output_branch_address( out_tree,"trueNeutCapVtxZ",&ev.trueNeutCapVtxZ_,create,"trueNeutCapVtxZ/D");
 set_output_branch_address( out_tree,"trueNeutCapNucleus",&ev.trueNeutCapNucleus_,create,"trueNeutCapNucleus/D");
 set_output_branch_address( out_tree,"trueNeutCapTime",&ev.trueNeutCapTime_,create,"trueNeutCapTime/D");
 set_output_branch_address( out_tree,"trueNeutCapGammas",&ev.trueNeutCapGammas_,create,"trueNeutCapGammas/D");
 set_output_branch_address( out_tree,"trueNeutCapE",&ev.trueNeutCapE_,create,"trueNeutCapE/D");
 set_output_branch_address( out_tree,"trueNeutCapGammaE",&ev.trueNeutCapGammaE_,create,"trueNeutCapGammaE/D");
    // True neutrino information
    // Interaction information

 //set_output_branch_address( out_tree,"trueFSLTime",&ev.trueFSLTime_,create,"trueFSLTime/D");
 //set_output_branch_address( out_tree,"trueFSLMass",&ev.trueFSLMass_,create,"trueFSLMass/D");
 set_output_branch_address( out_tree,"trueFSLEnergy",&ev.trueFSLEnergy_,create,"trueFSLEnergy/D");
    
    
 set_output_branch_address( out_tree,"pointPosX", &ev.pointPosX_,create,"pointPosX/D");
 set_output_branch_address( out_tree,"pointPosY", &ev.pointPosY_,create,"pointPosY/D");
 set_output_branch_address( out_tree,"pointPosZ", &ev.pointPosZ_,create,"pointPosZ/D");
 set_output_branch_address( out_tree,"pointPosTime", &ev.pointPosTime_,create,"pointPosTime/D");
 set_output_branch_address( out_tree,"pointPosFOM", &ev.pointPosFOM_,create,"pointPosFOM/D");
 set_output_branch_address( out_tree,"pointDirX", &ev.pointDirX_,create,"pointDirX/D");
 set_output_branch_address( out_tree,"pointDirY", &ev.pointDirY_,create,"pointDirY/D");
 set_output_branch_address( out_tree,"pointDirZ", &ev.pointDirZ_,create,"pointDirZ/D");
 set_output_branch_address( out_tree,"pointDirTime", &ev.pointDirTime_,create,"pointDirTime/D");
 set_output_branch_address( out_tree,"pointDirStatus", &ev.pointDirStatus_,create,"pointDirStatus/D");
 set_output_branch_address( out_tree,"pointDirFOM", &ev.pointDirFOM_,create,"pointDirFOM/D");
 set_output_branch_address( out_tree,"pointVtxPosX", &ev.pointVtxPosX_,create,"pointVtxPosX/D");
 set_output_branch_address( out_tree,"pointVtxPosY", &ev.pointVtxPosY_,create,"pointVtxPosY/D");
 set_output_branch_address( out_tree,"pointVtxPosZ", &ev.pointVtxPosZ_,create,"pointVtxPosZ/D");
 set_output_branch_address( out_tree,"pointVtxTime", &ev.pointVtxTime_,create,"pointVtxTime/D");
 set_output_branch_address( out_tree,"pointVtxDirX", &ev.pointVtxDirX_,create,"pointVtxDirX/D");
 set_output_branch_address( out_tree,"pointVtxDirY", &ev.pointVtxDirY_,create,"pointVtxDirY/D");
 set_output_branch_address( out_tree,"pointVtxDirZ", &ev.pointVtxDirZ_,create,"pointVtxDirZ/D");
 set_output_branch_address( out_tree,"pointVtxFOM", &ev.pointVtxFOM_,create,"pointVtxFOM/D");
 
 set_output_branch_address( out_tree,"fScaleFactor", &ev.fScaleFactor_,create,"fScaleFactor/D");
 // set_output_branch_address( out_tree,"reco0pi_contained_in_MRD_",&ev.reco0pi_contained_in_MRD_,create,"reco0pi_contained_in_MRD/O");
 set_output_branch_address( out_tree,"trueNeutCode",&ev.trueNeutCode_,create,"trueNeutCode/I");
      
  // set_output_branch_address( out_tree,"trueMuonMomentum", &ev.trueMuonMomentum_,create,"trueMuonMomentum/D");
 //set_output_branch_address( out_tree,"trueCosTheta", &ev.trueCosTheta_,create,"trueCosTheta/D");
 //set_output_branch_address( out_tree,"simpleRecoMomentum", &ev.simpleRecoMomentum_,create,"simpleRecoMomentum/D");
 //set_output_branch_address( out_tree,"simpleRecoMomentumCor", &ev.simpleRecoMomentumCor_,create,"simpleRecoMomentumCor/D");
 
 
     // Additional true information
  set_output_branch_address( out_tree,"trueCC",&ev.trueCC_,create,"trueCC/I");
  set_output_branch_address( out_tree,"trueNC",&ev.trueNC_,create,"trueNC/I");
  set_output_branch_address( out_tree,"trueQEL",&ev.trueQEL_,create,"trueQEL/I");
  //set_output_branch_address( out_tree,"trueRES",&ev.trueRES_,create,"trueRES/I");
  //set_output_branch_address( out_tree,"trueDIS",&ev.trueDIS_,create,"trueDIS/I");
  //set_output_branch_address( out_tree,"trueCOH",&ev.trueCOH_,create,"trueCOH/I");
  //set_output_branch_address( out_tree,"trueMEC",&ev.trueMEC_,create,"trueMEC/I");
  
  set_output_branch_address( out_tree,"trueNeutrons",&ev.trueNeutrons_,create,"trueNeutrons/I");
  set_output_branch_address( out_tree,"trueProtons",&ev.trueProtons_,create,"trueProtons/I");
  set_output_branch_address( out_tree,"truePi0",&ev.truePi0_,create,"truePi0/I");
  set_output_branch_address( out_tree,"truePiPlus",&ev.truePiPlus_,create,"truePiPlus/I");
  set_output_branch_address( out_tree,"truePiPlusCher",&ev.truePiPlusCher_,create,"truePiPlusCher/I");
  set_output_branch_address( out_tree,"truePiMinus",&ev.truePiMinus_,create,"truePiMinu/I");
  set_output_branch_address( out_tree,"truePiMinusCher",&ev.truePiMinusCher_,create,"truePiMinusCher/I");
  set_output_branch_address( out_tree,"trueKPlus",&ev.trueKPlus_,create,"trueKPlus/I");
  set_output_branch_address( out_tree,"trueKPlusCher",&ev.trueKPlusCher_,create,"trueKPlusCher/I");
  set_output_branch_address( out_tree,"trueKMinus",&ev.trueKMinus_,create,"trueKMinus/I");
  set_output_branch_address( out_tree,"trueKMinusCher",&ev.trueKMinusCher_,create,"trueKMinusCher/I");
   
  //set_output_branch_address( out_tree,"true_no_mesons",&ev.true_no_mesons_,create,"true_no_mesons/O");
  //set_output_branch_address( out_tree,"reco_contained_in_MRD",&ev.reco_contained_in_MRD_,create,"reco_contained_in_MRD/O");
  set_output_branch_address( out_tree,"pointVtxStatus",&ev.pointVtxStatus_,create,"pointVtxStatus/I");
  set_output_branch_address( out_tree,"pointPosStatus",&ev.pointPosStatus_,create,"pointPosStatus/I");

 
 set_output_branch_address( out_tree,"seedVtxTime", &ev.seedVtxTime_,create,"seedVtxTime/D");
     
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxX",ev.seedVtxX_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxY",ev.seedVtxY_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxZ",ev.seedVtxZ_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"seedVtxFOM",ev.seedVtxFOM_,create);

 set_object_output_branch_address< std::vector<double> >( out_tree,"hitQ",ev.hitQ_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"hitT",ev.hitT_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"hitX",ev.hitX_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"hitY",ev.hitY_,create);
 set_object_output_branch_address< std::vector<double> >( out_tree,"hitZ",ev.hitZ_,create);

 set_output_branch_address( out_tree,"Qij", &ev.Qij_,create,"Qij/D");





    // Additional true information

 //if ( ev.Qij_ ) {
 // 
 //}
 
 if ( ev.promptMuonTotalPE_ ) {
  set_output_branch_address( out_tree,"promptMuonTotalPE", &ev.promptMuonTotalPE_,create,"promptMuonTotalPE/D");
 }
    // Weights (continued)
  




}
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
