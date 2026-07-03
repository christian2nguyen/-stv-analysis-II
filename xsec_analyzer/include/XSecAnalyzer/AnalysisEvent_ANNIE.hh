#pragma once

// STV analysis includes
#include "TreeUtils.hh"
#include "FiducialVolume.hh"
#include "Constants.hh"

#include <vector>
#include <map>

#include "TVector3.h"
#include <algorithm>
#include <cassert>

#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <cstddef>
#include <string>
//#include <unordered_map>
#include <utility>
#include <vector>
#include <array>

///////////////////////////
// I think these are the varibles that are tranfered over 
//////////////////////////






class AnalysisANNIE_Event{
 public:
  AnalysisANNIE_Event() {}
  ~AnalysisANNIE_Event() {}
  
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
    
    MyPointer<std::vector<double>> hitX_;
    MyPointer<std::vector<double>> hitY_;
    MyPointer<std::vector<double>> hitZ_;
    MyPointer<std::vector<double>> hitT_;
    MyPointer<std::vector<double>> hitQ_;
    MyPointer<std::vector<double>> hitPE_;
    MyPointer<std::vector<int>> hitDetID_;


    //std::vector<bool>* MRDSide_;
    //std::vector<bool>* MRDStop_;
    //std::vector<bool>* MRDThrough_;
    
    MyPointer<std::vector<bool>> MRDSide_;
    MyPointer<std::vector<bool>> MRDStop_;
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
    int simpleRecoFlag_= BOGUS_INT;
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
    
    int trueNuPDG_= BOGUS_INT;
    double Qij_= BOGUS;
    double promptMuonTotalPE_= BOGUS;
    //bool reco_contained_in_MRD_=false;
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

    // Additional true information
    double trueBJx_ = BOGUS;
    double truey_ = BOGUS;
    double trueq0_ = BOGUS;
    double trueq3_ = BOGUS;
    int trueTargetZ_ = BOGUS_INT;
    double trueW2_ = BOGUS;

    // Weights
    MyPointer<std::vector<double>> weight_All0_UBGenie_;
    MyPointer<std::vector<double>> weight_All1_UBGenie_;
    MyPointer<std::vector<double>> weight_All2_UBGenie_;
    MyPointer<std::vector<double>> weight_All3_UBGenie_;
    MyPointer<std::vector<double>> weight_All4_UBGenie_;
    MyPointer<std::vector<double>> weight_All5_UBGenie_;
    MyPointer<std::vector<double>> weight_AxFFCCQEshape_UBGenie_;
    MyPointer<std::vector<double>> weight_DecayAngMEC_UBGenie_;
    MyPointer<std::vector<double>> weight_NormCCCOH_UBGenie_;
    MyPointer<std::vector<double>> weight_NormNCCOH_UBGenie_;
    MyPointer<std::vector<double>> weight_RPA_CCQE_UBGenie_;
    MyPointer<std::vector<double>> weight_RootinoFix_UBGenie_;
    MyPointer<std::vector<double>> weight_ThetaDelta2NRad_UBGenie_;
    MyPointer<std::vector<double>> weight_Theta_Delta2Npi_UBGenie_;
    MyPointer<std::vector<double>> weight_TunedCentralValue_UBGenie_;
    MyPointer<std::vector<double>> weight_VecFFCCQEshape_UBGenie_;
    MyPointer<std::vector<double>> weight_XSecShape_CCMEC_UBGenie_;
    MyPointer<std::vector<double>> weight_horncurrent_FluxUnisim_;
    MyPointer<std::vector<double>> weight_expskin_FluxUnisim_;
    MyPointer<std::vector<double>> weight_piplus_PrimaryHadronSWCentralSplineVariation_;
    MyPointer<std::vector<double>> weight_piminus_PrimaryHadronSWCentralSplineVariation_;
    MyPointer<std::vector<double>> weight_kplus_PrimaryHadronFeynmanScaling_;
    MyPointer<std::vector<double>> weight_kzero_PrimaryHadronSanfordWang_;
    MyPointer<std::vector<double>> weight_kminus_PrimaryHadronNormalization_;
    MyPointer<std::vector<double>> weight_pioninexsec_FluxUnisim_;
    MyPointer<std::vector<double>> weight_pionqexsec_FluxUnisim_;
    MyPointer<std::vector<double>> weight_piontotxsec_FluxUnisim_;
    MyPointer<std::vector<double>> weight_nucleoninexsec_FluxUnisim_;
    MyPointer<std::vector<double>> weight_nucleonqexsec_FluxUnisim_;
    MyPointer<std::vector<double>> weight_nucleontotxsec_FluxUnisim_;
  
   // New Wgts 
   MyPointer<std::vector<double>> weight_DirtUnc_;
   MyPointer<std::vector<double>> weight_MRDUnc_;
   double MRDEff_=BOGUS;
   double DirtMu_=BOGUS;

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
    bool trueFV_= false;
    bool recoFV_= false;
    bool true_no_followers_ = false;
    //bool mc_is_cc0pi_signal_ = false; 

    // Additional true information
    double trueMuonMomentum_= BOGUS;
    double trueCosTheta_= BOGUS;
    double simpleRecoMomentum_= BOGUS;
    double simpleRecoMomentumCor_= BOGUS;
    bool true_no_mesons_ = false;
    bool reco0pi_contained_in_MRD_ = false;
    //bool reco_0pi_ = false; 
    //bool recoPE_ = false; 
    // Weights (continued)
    MyPointer<std::vector<double>> weight_All_UBGenie_;
    MyPointer<std::vector<double>> weight_flux_all_;
  
};
//////////////////////////////////////////////
/*
class AnalysisANNIE_Event{
 public:
  AnalysisANNIE_Event() {}
  ~AnalysisANNIE_Event() {}
  
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
    std::vector<bool>* MRDSide_;
    std::vector<bool>* MRDStop_;
    std::vector<bool>* MRDThrough_;

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

    // Additional true information
    double trueBJx_ = BOGUS;
    double truey_ = BOGUS;
    double trueq0_ = BOGUS;
    double trueq3_ = BOGUS;
    int trueTargetZ_ = BOGUS_INT;
    double trueW2_ = BOGUS;

    // Weights// removed wgts 


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
    bool trueFV_= false;
    bool recoFV_= false;
    //bool mc_is_cc0pi_signal_ = false; 

    // Additional true information
    double trueMuonMomentum_= BOGUS;
    double trueCosTheta_= BOGUS;
    double simpleRecoMomentum_= BOGUS;
    double simpleRecoMomentumCor_= BOGUS;
    bool true_no_mesons_ = false;
    bool reco_contained_in_MRD_ = false;

    // Weights (continued)
  
};
*/
