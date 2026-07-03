#pragma once

// STV analysis includes
#include "TreeUtils.hh"
#include "FiducialVolume.hh"
#include "Constants.hh"
#include "TMath.h"
#include "TError.h"   // for gErrorIgnoreLevel
#include <vector>
#include <map>

#include "Functions.hh"
#include "TVector3.h"
///////////////////////////
// I think these are the varibles that are tranfered over 
//////////////////////////


class AnalysisANNIE_Event_nuwro{
 public:
  AnalysisANNIE_Event_nuwro() {}
  ~AnalysisANNIE_Event_nuwro() {}
  
  // Signal definition requirements
    bool is_mc_ = true;
    //int category_= BOGUS_INT;
  
    int runNumber_ = BOGUS_INT;
    int subrunNumber_ = BOGUS_INT;
    int runType_ = BOGUS_INT;
    double startTime_;
    int eventNumber_ = BOGUS_INT;
    double eventTimeTank_;
    double eventTimeMRD_;
    int nhits_ = BOGUS_INT;
    int trigword_= BOGUS_INT;
    int HasTank_= BOGUS_INT;
    int HasMRD_= BOGUS_INT;
    int TankMRDCoinc_= BOGUS_INT;
    int NoVeto_= BOGUS_INT;
    int Extended_= BOGUS_INT;
    double beam_pot_= BOGUS;
    int beam_ok_= BOGUS_INT;

    // MRD track information
    int numMRDTracks_= BOGUS_INT;
    MyPointer<std::vector<double>> MRDTrackAngle_;
    MyPointer<std::vector<double>> MRDTrackAngleError_;
    MyPointer<std::vector<double>> MRDPenetrationDepth_;
    MyPointer<std::vector<double>> MRDTrackLength_;
    MyPointer<std::vector<double>> MRDEntryPointRadius_;
    MyPointer<std::vector<double>> MRDEnergyLoss_;
    MyPointer<std::vector<double>> MRDEnergyLossError_;
    MyPointer<std::vector<double>> MRDTrackStartX_;
    MyPointer<std::vector<double>> MRDTrackStartY_;
    MyPointer<std::vector<double>> MRDTrackStartZ_;
    MyPointer<std::vector<double>> MRDTrackStopX_;
    MyPointer<std::vector<double>> MRDTrackStopY_;
    MyPointer<std::vector<double>> MRDTrackStopZ_;
    //std::vector<bool>* MRDSide_;
    MyPointer<std::vector<bool>>MRDStop_;
    MyPointer<std::vector<bool>> MRDSide_;
    MyPointer<std::vector<bool>> MRDThrough_;

    // Reconstructed vertex information
    double recoVtxX_= BOGUS;
    double recoVtxY_= BOGUS;
    double recoVtxZ_= BOGUS;
    double recoVtxTime_= BOGUS;
    double recoDirX_= BOGUS;
    double recoDirY_= BOGUS;
    double recoDirZ_= BOGUS;
    double recoAngle_= BOGUS;
    double recoPhi_= BOGUS;
    double recoVtxFOM_= BOGUS;
    int recoStatus_= BOGUS_INT;

    // Simple reconstruction information
    int simpleRecoFlag_;
    double simpleRecoEnergy_= BOGUS;
    double simpleRecoVtxX_= BOGUS;
    double simpleRecoVtxY_= BOGUS;
    double simpleRecoVtxZ_= BOGUS;
    double simpleRecoStopVtxX_= BOGUS;
    double simpleRecoStopVtxY_= BOGUS;
    double simpleRecoStopVtxZ_= BOGUS;
    double simpleRecoCosTheta_= BOGUS;
    double simpleRecoPt_= BOGUS;
    int simpleRecoFV_= BOGUS_INT;
    double simpleRecoMrdEnergyLoss_= BOGUS;
    double simpleRecoTrackLengthInMRD_= BOGUS;
    double simpleRecoMRDStartX_= BOGUS;
    double simpleRecoMRDStartY_= BOGUS;
    double simpleRecoMRDStartZ_= BOGUS;
    double simpleRecoMRDStopX_= BOGUS;
    double simpleRecoMRDStopY_= BOGUS;
    double simpleRecoMRDStopZ_= BOGUS;
    double simpleRecoTrackLengthInTank_= BOGUS;
     

    // Additional event information
    double RCSRPred_= BOGUS;
    double RCMRPred_= BOGUS;
    int triggerNumber_= BOGUS_INT;
    int mcEntryNumber_= BOGUS_INT;
   
    int trueNuPDG_= BOGUS_INT;
    double promptMuonTotalPE_= BOGUS;


    // True vertex information
    double trueVtxX_= BOGUS;
    double trueVtxY_= BOGUS;
    double trueVtxZ_= BOGUS;
    double trueVtxTime_= BOGUS;
    double trueDirX_= BOGUS;
    double trueDirY_= BOGUS;
    double trueDirZ_= BOGUS;
    double trueAngle_= BOGUS;
    double truePhi_= BOGUS;
    double trueMuonEnergy_= BOGUS;
    int truePrimaryPdg_ = BOGUS_INT;
    double trueTrackLengthInWater_= BOGUS;
    double trueTrackLengthInMRD_= BOGUS;
    int trueMultiRing_ = BOGUS_INT;
    //double trueCosTheta_ = BOGUS;
    // Particle counts
    int Pi0Count_ = BOGUS_INT;
    int PiPlusCount_ = BOGUS_INT;
    int PiMinusCount_ = BOGUS_INT;
    int K0Count_ = BOGUS_INT;
    int KPlusCount_ = BOGUS_INT;
    int KMinusCount_ = BOGUS_INT;

    
    // True primary particle information
    MyPointer<std::vector<int>> truePrimaryPdgs_;
    MyPointer<std::vector<double>> trueNeutCapVtxX_;
    MyPointer<std::vector<double>> trueNeutCapVtxY_;
    MyPointer<std::vector<double>> trueNeutCapVtxZ_;
    MyPointer<std::vector<double>> trueNeutCapNucleus_;
    MyPointer<std::vector<double>> trueNeutCapTime_;
    MyPointer<std::vector<double>> trueNeutCapGammas_;
    MyPointer<std::vector<double>> trueNeutCapE_;
    MyPointer<std::vector<double>> trueNeutCapGammaE_;

    // True neutrino information
    double trueNeutrinoEnergy_= BOGUS;
    double trueNeutrinoMomentum_X_= BOGUS;
    double trueNeutrinoMomentum_Y_= BOGUS;
    double trueNeutrinoMomentum_Z_= BOGUS;
    double trueNuIntxVtx_X_= BOGUS;
    double trueNuIntxVtx_Y_= BOGUS;
    double trueNuIntxVtx_Z_= BOGUS;
    double trueNuIntxVtx_T_= BOGUS;
    double trueFSLVtx_X_= BOGUS;
    double trueFSLVtx_Y_= BOGUS;
    double trueFSLVtx_Z_= BOGUS;
    double trueFSLMomentum_X_= BOGUS;
    double trueFSLMomentum_Y_= BOGUS;
    double trueFSLMomentum_Z_= BOGUS;
    double trueFSLTime_= BOGUS;
    double trueFSLMass_= BOGUS;
    int trueFSLPdg_= BOGUS_INT;
    double trueFSLEnergy_= BOGUS;
    
    MyPointer<std::vector<int>> trueFollowerParentPDG_;
    //std::vector<int>* trueFollowerParentPDG_ = nullptr;
    
    
    //std::vector<double>* hitQ_ = nullptr;
    //std::vector<double>* hitT_ = nullptr;
    //std::vector<double>* hitX_ = nullptr;
    //std::vector<double>* hitY_ = nullptr;
    //std::vector<double>* hitZ_ = nullptr;

   MyPointer<std::vector<double>> hitQ_;
   MyPointer<std::vector<double>> hitT_;
   MyPointer<std::vector<double>> hitX_;
   MyPointer<std::vector<double>> hitY_;
   MyPointer<std::vector<double>> hitZ_;
   MyPointer<std::vector<double>> hitPE_;
   MyPointer<std::vector<int>> hitDetID_;

    // Interaction information
    double trueQ2_= BOGUS;
    int trueCC_= BOGUS_INT;
    int trueNC_= BOGUS_INT;
    int trueQEL_= BOGUS_INT;
    int trueRES_= BOGUS_INT;
    int trueDIS_= BOGUS_INT;
    int trueCOH_= BOGUS_INT;
    int trueMEC_= BOGUS_INT;
    int trueNeutrons_ = BOGUS_INT;
    int trueProtons_ = BOGUS_INT;
    int truePi0_ = BOGUS_INT;
    int truePiPlus_ = BOGUS_INT;
    int truePiPlusCher_ = BOGUS_INT;
    int truePiMinus_ = BOGUS_INT;
    int truePiMinusCher_ = BOGUS_INT;
    int trueKPlus_ = BOGUS_INT;
    int trueKPlusCher_ = BOGUS_INT;
    int trueKMinus_ = BOGUS_INT;
    int trueKMinusCher_ = BOGUS_INT;
    
    int trueNeutCode_= BOGUS_INT;
    double Qij_= BOGUS;
    double tank_bdt_no_pion_score_ = BOGUS;
    double tank_bdt_pion_score_ = BOGUS;
    double tank_bdt_input_promptMuonTotalPE_ = BOGUS;
    double tank_bdt_input_Qij_ = BOGUS;
    double tank_bdt_input_q_asym_ud_ = BOGUS;
    double tank_bdt_input_pe_asym_ud_ = BOGUS;
    double tank_bdt_input_q_asym_tb_ = BOGUS;
    double tank_bdt_input_pe_asym_tb_ = BOGUS;
    double tank_bdt_input_q_r_mean_ = BOGUS;
    double tank_bdt_input_pe_r_mean_ = BOGUS;
    double tank_bdt_input_q_phi_mean_ = BOGUS;
    double tank_bdt_input_pe_phi_mean_ = BOGUS;
    double tank_bdt_input_q_phi_c1_ = BOGUS;
    double tank_bdt_input_q_phi_s1_ = BOGUS;
    double tank_bdt_input_q_phi_a1_ = BOGUS;
    double tank_bdt_input_q_phi_c2_ = BOGUS;
    double tank_bdt_input_q_phi_s2_ = BOGUS;
    double tank_bdt_input_q_phi_a2_ = BOGUS;
    double tank_bdt_input_pe_phi_c1_ = BOGUS;
    double tank_bdt_input_pe_phi_s1_ = BOGUS;
    double tank_bdt_input_pe_phi_a1_ = BOGUS;
    double tank_bdt_input_pe_phi_c2_ = BOGUS;
    double tank_bdt_input_pe_phi_s2_ = BOGUS;
    double tank_bdt_input_pe_phi_a2_ = BOGUS;
    double tank_bdt_input_n_unique_pmts_hit_ = BOGUS;
    double tank_bdt_input_max_pe_fraction_ = BOGUS;
    double tank_bdt_input_top5_pe_fraction_ = BOGUS;
    double tank_bdt_input_pmt_pe_entropy_ = BOGUS;
    double tank_bdt_input_pmt_pe_effective_n_ = BOGUS;
    double tank_bdt_input_q_center_x_ = BOGUS;
    double tank_bdt_input_q_center_y_ = BOGUS;
    double tank_bdt_input_q_center_z_ = BOGUS;
    double tank_bdt_input_q_rms_x_ = BOGUS;
    double tank_bdt_input_q_rms_y_ = BOGUS;
    double tank_bdt_input_q_rms_z_ = BOGUS;
    double tank_bdt_input_pe_center_x_ = BOGUS;
    double tank_bdt_input_pe_center_y_ = BOGUS;
    double tank_bdt_input_pe_center_z_ = BOGUS;
    double tank_bdt_input_pe_rms_x_ = BOGUS;
    double tank_bdt_input_pe_rms_y_ = BOGUS;
    double tank_bdt_input_pe_rms_z_ = BOGUS;
    double tank_bdt_input_hit_t_mean_ = BOGUS;
    double tank_bdt_input_hit_t_stddev_ = BOGUS;
    double tank_bdt_input_hit_q_sum_ = BOGUS;
    double tank_bdt_input_hit_q_mean_ = BOGUS;
    double tank_bdt_input_hit_q_stddev_ = BOGUS;
    double tank_bdt_input_hit_pe_sum_ = BOGUS;
    double tank_bdt_input_hit_pe_mean_ = BOGUS;
    double tank_bdt_input_hit_pe_stddev_ = BOGUS;
    double tank_bdt_input_hit_t_min_ = BOGUS;
    double tank_bdt_input_hit_t_max_ = BOGUS;
    double tank_bdt_input_hit_t_range_ = BOGUS;
    double tank_bdt_input_timing_corr_r_ = BOGUS;
    double tank_bdt_input_timing_corr_z_ = BOGUS;
    // Additional true information
    double trueBJx_ = BOGUS;
    double truey_ = BOGUS;
    double trueq0_ = BOGUS;
    double trueq3_ = BOGUS;
    int trueTargetZ_ = BOGUS_INT;
    double trueW2_ = BOGUS;

    // Weights
    //MyPointer<std::vector<double>> weight_All0_UBGenie_;
    //MyPointer<std::vector<double>> weight_All1_UBGenie_;
    //MyPointer<std::vector<double>> weight_All2_UBGenie_;
    //MyPointer<std::vector<double>> weight_All3_UBGenie_;
    //MyPointer<std::vector<double>> weight_All4_UBGenie_;
    //MyPointer<std::vector<double>> weight_All5_UBGenie_;
    //MyPointer<std::vector<double>> weight_AxFFCCQEshape_UBGenie_;
    //MyPointer<std::vector<double>> weight_DecayAngMEC_UBGenie_;
    //MyPointer<std::vector<double>> weight_NormCCCOH_UBGenie_;
    //MyPointer<std::vector<double>> weight_NormNCCOH_UBGenie_;
    //MyPointer<std::vector<double>> weight_RPA_CCQE_UBGenie_;
    //MyPointer<std::vector<double>> weight_RootinoFix_UBGenie_;
    //MyPointer<std::vector<double>> weight_ThetaDelta2NRad_UBGenie_;
    //MyPointer<std::vector<double>> weight_Theta_Delta2Npi_UBGenie_;
    //MyPointer<std::vector<double>> weight_TunedCentralValue_UBGenie_;
    //MyPointer<std::vector<double>> weight_VecFFCCQEshape_UBGenie_;
    //MyPointer<std::vector<double>> weight_XSecShape_CCMEC_UBGenie_;
    //MyPointer<std::vector<double>> weight_horncurrent_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_expskin_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_piplus_PrimaryHadronSWCentralSplineVariation_;
    //MyPointer<std::vector<double>> weight_piminus_PrimaryHadronSWCentralSplineVariation_;
    //MyPointer<std::vector<double>> weight_kplus_PrimaryHadronFeynmanScaling_;
    //MyPointer<std::vector<double>> weight_kzero_PrimaryHadronSanfordWang_;
    //MyPointer<std::vector<double>> weight_kminus_PrimaryHadronNormalization_;
    //MyPointer<std::vector<double>> weight_pioninexsec_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_pionqexsec_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_piontotxsec_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_nucleoninexsec_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_nucleonqexsec_FluxUnisim_;
    //MyPointer<std::vector<double>> weight_nucleontotxsec_FluxUnisim_;

    // Seed vertex information
    MyPointer<std::vector<double>> seedVtxX_;
    MyPointer<std::vector<double>> seedVtxY_;
    MyPointer<std::vector<double>> seedVtxZ_;
    MyPointer<std::vector<double>> seedVtxFOM_;
    double seedVtxTime_= BOGUS;

    // Point information
    double pointPosX_= BOGUS;
    double pointPosY_= BOGUS;
    double pointPosZ_= BOGUS;
    double pointPosTime_= BOGUS;
    double pointPosFOM_= BOGUS;
    int pointPosStatus_= BOGUS_INT;
    double pointDirX_= BOGUS;
    double pointDirY_= BOGUS;
    double pointDirZ_= BOGUS;
    double pointDirTime_= BOGUS;
    int pointDirStatus_= BOGUS_INT;
    double pointDirFOM_= BOGUS;
    double pointVtxPosX_= BOGUS;
    double pointVtxPosY_= BOGUS;
    double pointVtxPosZ_= BOGUS;
    double pointVtxTime_= BOGUS;
    double pointVtxDirX_= BOGUS;
    double pointVtxDirY_= BOGUS;
    double pointVtxDirZ_= BOGUS;
    double pointVtxFOM_= BOGUS;
    int pointVtxStatus_= BOGUS_INT;

    // Fiducial volume flags
    //bool trueFV_= false;
    //bool recoFV_= false;
    //bool mc_is_cc0pi_signal_ = false; 

    // Additional true information
    //double trueMuonMomentum_= BOGUS;
    //double trueCosTheta_= BOGUS;
    //double simpleRecoMomentum_= BOGUS;
    //double simpleRecoMomentumCor_= BOGUS;
    bool true_no_mesons_ = false;
    double fScaleFactor_ = BOGUS;
    bool reco0pi_contained_in_MRD_ = false;
    // Weights (continued)
    //MyPointer<std::vector<double>> weight_All_UBGenie_;
    //MyPointer<std::vector<double>> weight_flux_all_;
    
  
};

inline double calculateQij(std::vector<double>* hitT, std::vector<double>* hitX, std::vector<double>* hitY, 
                    std::vector<double>* hitZ, std::vector<double>* hitQ,
                    double simplevtxx, double simplevtxy, double simplevtxz,
                    double time_window = 20.0) {
    
    double qval = 0.0;
    double num = 0.0;
    double minT = 99999.0;
    
    // Find minimum time
    for(int j = 0; j < hitT->size(); ++j){
        if(hitT->at(j) <= 0) continue;
        if(minT > hitT->at(j)) minT = hitT->at(j);
    }
    
    // Calculate Qij
    for(int j = 0; j < hitT->size(); ++j){
        if(hitT->at(j) > (minT + time_window)) continue; // "prompt" cluster cut
        qval += hitQ->at(j);
        double qi = hitQ->at(j);
        
        for(int k = 0; k < hitT->size(); ++k){
            if(j == k) continue;
            if(hitT->at(k) > (minT + time_window)) continue; // "prompt" cluster cut
            double qj = hitQ->at(k);
            num += qi * qj * LoC(simplevtxx, simplevtxy + 0.1446, simplevtxz - 1.681,
                                  hitX->at(j), hitY->at(j), hitZ->at(j),
                                  hitX->at(k), hitY->at(k), hitZ->at(k));
        }
    }
    
    
     if(qval == 0.0) return -9999.0;  // Avoid division by zero
      double result = num / qval;
  
    if(std::isnan(result) || std::isinf(result)) {
        return -9999.0;
    }
    
    return result;
}

inline double calculateQij_withcheck(std::vector<double>* hitT, std::vector<double>* hitX, std::vector<double>* hitY, 
                    std::vector<double>* hitZ, std::vector<double>* hitQ,
                    double simplevtxx, double simplevtxy, double simplevtxz,
                    double time_window = 20.0) {
    
    // Check for invalid vertex coordinates
    if(std::isnan(simplevtxx) || std::isnan(simplevtxy) || std::isnan(simplevtxz) ||
       std::isinf(simplevtxx) || std::isinf(simplevtxy) || std::isinf(simplevtxz)) {
        return -9999.0;
    }
        
    double qval = 0.0;
    double num = 0.0;
    double minT = 99999.0;
    
    // Find minimum time
    for(int j = 0; j < hitT->size(); ++j){
        if(hitT->at(j) <= 0) continue;
        if(minT > hitT->at(j)) minT = hitT->at(j);
    }
    
    // Calculate Qij
    for(int j = 0; j < hitT->size(); ++j){
        if(hitT->at(j) > (minT + time_window)) continue; // "prompt" cluster cut
        qval += hitQ->at(j);
        double qi = hitQ->at(j);
        
        for(int k = 0; k < hitT->size(); ++k){
            if(j == k) continue;
            if(hitT->at(k) > (minT + time_window)) continue; // "prompt" cluster cut
            double qj = hitQ->at(k);
            num += qi * qj * LoC(simplevtxx, simplevtxy + 0.1446, simplevtxz - 1.681,
                                  hitX->at(j), hitY->at(j), hitZ->at(j),
                                  hitX->at(k), hitY->at(k), hitZ->at(k));
        }
    }
    
    if(qval == 0.0) return -9999.0;  // Avoid division by zero
    double result = num / qval;
  
    if(std::isnan(result) || std::isinf(result)) {
        return -9999.0;
    }
    
    return result;
}
