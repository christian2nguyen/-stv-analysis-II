#pragma once

#include "TH1.h"
#include "TH2.h"
#include "NamedCategory.hh"
//I want to addd the functionility of NameCategory to make it easy to deal with stacks

// Enum used to label event categories of interest for analysis plots
enum EventCategory {

  // Unable to categorize (e.g., because the event is real data and thus
  // has no MC truth information)
  kUnknown = 0,

  // Signal events broken down by underlying reaction mode
  kSignalCCQE = 1,
  kSignalCCMEC = 2,
  kSignalCCRES = 3,
  kSignalOther = 4,

  // True numu CC event with at least one final-state pion above threshold
  kNuMuCCNpi = 5,

  // True numu CC event with zero final-state pions above threshold and
  // zero final-state protons above threshold
  kNuMuCC0pi0p = 6, 
  //  Will keep  kNuMuCC0pi0p enum but removed from categories and now this type is funnled into signal region
  
  // Any true numu CC event which does not satisfy the criteria for inclusion
  // in one of the other categories above
  kNuMuCCOther = 7,

  // True nue CC event
  kNuECC = 8,

  // True neutral current event for any neutrino flavor
  kNC = 9,

  // True neutrino vertex (any reaction mode and flavor combination) is outside
  // of the fiducial volume
  kOOFV = 10,

  // Seperate numubar CC from kOther
  //kNuMuBarCC = 13,
  // All events that do not fall within any of the other categories (e.g.,)
  kOther = 11,
  
  

  

};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum BDT_Category {
// Categorys for BTD prediction 
  kBDT_Else = 0,
  kBDT_Muon = 1,
  kBDT_Pion = 2,
  kBDT_Proton = 3,
  kBDT_BOGUS = 4
  
  
 // enum class BDTClass { kOther, kMu, kPi, kP, kInvalid };
  
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum FidVol_Category {
// cateogrory if the muon was contained or not 
  kContained  = 0,
  kUnContained  = 1,
  kContainmentBG = 2
  
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////  
enum Particle_type {
  kElectron,
  kPion_neg,
  kPion_pos,
  kPion_pos_neg,
  kPion_0,
  kPion_combine,
  kKaon,
  kProton,
  kNeutron,
  kMuon,
  kGamma,
  kNeutrino_muon,
  kNeutrino_electron,
  kAnti_Neutrino,
  kFourHelium,
  kLamdba,
  kSigma_plus,
  kParticle_OTHER,
  kParticle_N_A,
  kParticle_Total,
  kParticle_neutral,
  kPion_0_Electron_kGamma,
  k_MicroBooNECOSMICs
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum CCZeroPi_type {
// Categorys for BTD prediction 
  kCC_0P_ZeroPi = 0,
  kCC_1P_ZeroPi = 1,
  kCC_2P_ZeroPi = 2,
  kCC_3orGreaterP_ZeroPi = 3,
  kCC_ZeroPi_BG = 4,
  
};
enum Binning2D {

 kProj_Bin1 = 1,
 kProj_Bin2 = 2,
 kProj_Bin3 = 3,
 kProj_Bin4 = 4,
 kProj_Bin5 = 5,
 kProj_Bin6 = 6,
 kProj_Bin7 = 7,
 kProj_Bin8 = 8,
 kProj_Bin9 = 9
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum Binning2DInclusive {

 kProj_InclBin1 = 1,
 kProj_InclBin2 = 2,
 kProj_InclBin3 = 3,
 kProj_InclBin4 = 4,
 kProj_InclBin5 = 5,
 kProj_InclBin6 = 6,
 kProj_InclBin7 = 7,
 kProj_InclBin8 = 8,
 kProj_InclBin9 = 9
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum variable_binning{
kBINNING_Pmu,
kBINNING_Costheta,
kBINNING_Pmu_Proton,
kBINNING_Costheta_Proton,
kBINNING_pn,
kBINNING_delta_alphaT,
kBINNING_delta_pTx,
kBINNING_delta_pTy,
kBINNING_delta_phiT,
kBINNING_delta_pT,
kBINNING_VertexX,
kBINNING_VertexY,
kBINNING_VertexZ,
kBINNING_Mult,
kBINNING_Probability,
kBINNING_trk_distance_v,
kBINNING_trk_len_v,
kBINNING_Cosmic
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum GENIE_MC{
kCCQE,
kCCMEC,
kCCRES,
kCCOTHER
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
enum Particle_top_groups {
kCC_1pi,
kCC_2pi,
kCC_3pi,
kNC_1p,
kCC_1p,
kCC_2p,
kCC_3p,
kCC_1pi_1p,
kCC_2pi_1p,
kCC_1pi_2p,
kCC_1pi_1comsic,
kCC_1pi_2comsic,
kCC_1pi_3comsic,
kCC_2pi_1comsic,
kCC_2pi_2comsic,
kCC_2pi_3comsic,
kCC_1pi_1p_Ncomsic,
kCC_1pi_Neletrons,
kCC_1comsic,
kCC_2comsic,
kCC_3comsic,
kCC_1mu,
kCC_2mu,
kCC_3mu,
kCC_1p_1comsic,
kCC_1p_2comsic,
kCC_1p_3comsic,
kCC_2p_Ncomsic,
kCC_1p_1eletrons,
kCC_1p_2eletrons,
kCC_1p_3eletrons,
kCC_2p_Neletrons,
kCC_OTHER
};




// Singleton class that helps manipulate EventCategory enum values
class EventCategoryInterpreter {

  public:

    // This is a singleton class, so we'll delete the copy constructor
    // the move constructor, and the assignment operators
    EventCategoryInterpreter( const EventCategoryInterpreter& ) = delete;
    EventCategoryInterpreter( EventCategoryInterpreter&& ) = delete;
    EventCategoryInterpreter& operator=( const EventCategoryInterpreter& )
      = delete;
    EventCategoryInterpreter& operator=( EventCategoryInterpreter&& )
      = delete;
  ///////////////////////////////////////////////////////////////////////
  // Making these Public to excess them from the constructor
  ///////////////////////////////////////////////////////////////////////
 std::vector<NamedCategory<EventCategory>>
         EventSelectionGroup_categories_ = {
                 NamedCategory<EventCategory>({kUnknown},        "Unknown"),
                 NamedCategory<EventCategory>({kSignalCCQE},     "Signal-(CCQE)"),
                 NamedCategory<EventCategory>({kSignalCCMEC},    "Signal-(CCMEC)"),
                 NamedCategory<EventCategory>({kSignalCCRES},    "Signal-(CCRES)"),
                 NamedCategory<EventCategory>({kSignalOther},    "Signal-(Other)"),
                 NamedCategory<EventCategory>({kNuMuCCNpi},      "#nu_{#mu}-CCN#pi"),
                 /*NamedCategory<EventCategory>({kNuMuBarCC},       "#bar{#nu_{#mu}}-CC"),*/
                 NamedCategory<EventCategory>({kNuMuCCOther},    "#nu_{#mu}-CCOther"),
                 NamedCategory<EventCategory>({kNuECC},          "#nu_{e}-CC"),
                 NamedCategory<EventCategory>({kNC},             "NC"),
                 NamedCategory<EventCategory>({kOOFV},           "Out FV"),
                 NamedCategory<EventCategory>({kOther},          "Other")
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////    
std::vector<NamedCategory<BDT_Category>>
  BTD_Group_categories_ = {
              NamedCategory<BDT_Category>({kBDT_Else},     "BTD Else"),
              NamedCategory<BDT_Category>({kBDT_Muon},     "BDT Muon"),
              NamedCategory<BDT_Category>({kBDT_Pion},     "BDT Pion"),
              NamedCategory<BDT_Category>({kBDT_Proton},   "BDT Proton"),
              NamedCategory<BDT_Category>({kBDT_BOGUS},    "BDT Failed Prediction")
  };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
std::vector<NamedCategory<FidVol_Category>>
      ContainedGroup_categories_ = {
      NamedCategory<FidVol_Category>({kContained},  "Muon Contained"),
      NamedCategory<FidVol_Category>({kUnContained},"Muon UnContained"),
      NamedCategory<FidVol_Category>({kContainmentBG},"BG")
};
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
std::vector<NamedCategory<Particle_type>>
     ParticleGroup_reduced_categories_ = {
       NamedCategory<Particle_type>({k_MicroBooNECOSMICs},    "Cosmics"),
       NamedCategory<Particle_type>({kParticle_OTHER},        "Other"),
       NamedCategory<Particle_type>({kParticle_neutral},      "Neutral"),
       NamedCategory<Particle_type>({kKaon},                 "K^{#pm}"),
       NamedCategory<Particle_type>({kMuon},                  "#mu"),
       NamedCategory<Particle_type>({kPion_0_Electron_kGamma},"e^{#pm}, #gamma, #pi^{0}"),
       NamedCategory<Particle_type>({kPion_pos_neg},          "#pi^{#pm}"),
       NamedCategory<Particle_type>({kProton},                "p")
         };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
std::vector<NamedCategory<CCZeroPi_type>>
       topology_categories_ = {
           NamedCategory<CCZeroPi_type>({kCC_0P_ZeroPi},          "CC0#pi"),
           NamedCategory<CCZeroPi_type>({kCC_1P_ZeroPi},          "CC1p0#pi"),
           NamedCategory<CCZeroPi_type>({kCC_2P_ZeroPi},          "CC2p0#pi"),
           NamedCategory<CCZeroPi_type>({kCC_3orGreaterP_ZeroPi}, "CC3>p0#pi"),
           NamedCategory<CCZeroPi_type>({kCC_ZeroPi_BG}, "BG")
           
};
///////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
    // Get a const reference to the singleton instance of the
    // EventCategoryInterpreter
    inline static const EventCategoryInterpreter& Instance() {

      // Create the EventCategoryInterpreter object using a static variable.
      // This ensures that the singleton instance is only created once.
      static std::unique_ptr<EventCategoryInterpreter>
        the_instance( new EventCategoryInterpreter() );

      // Return a reference to the singleton instance
      return *the_instance;
    }
    inline const std::vector<EventCategory>  ReturnCategoryVector() const {
    std::vector<EventCategory> output_vector{
    kSignalCCQE,
    kSignalCCMEC,
    kSignalCCRES,
    kSignalOther,
    kNuMuCCNpi,
    kNuMuCCOther, 
    kNuECC,
    kNC,
    kOOFV,
    kOther, 
    kUnknown};
    return output_vector;
    }
    
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline const std::map< EventCategory, std::string >& label_map() const
      { return event_category_to_label_map_; }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline const std::map< FidVol_Category, std::string >& Containment_label_map() const
      { return Containment_to_label_map_; }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline const std::map< CCZeroPi_type, std::string >& Topology_label_map() const
      { return CCZeroPi_type_to_label_map_; }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline const std::map< Particle_type, std::string >& reducedParticle_label_map() const
      { return ReducedParticle_type_to_label_map_; }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline std::string label( EventCategory ec ) const
      { return event_category_to_label_map_.at( ec ); }

inline std::string Hist_label( EventCategory ec ) const
      { return event_category_to_label_HistNamemap_.at( ec ); }
      
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline std::string label( FidVol_Category ec ) const
      { return Containment_to_label_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline std::string label( CCZeroPi_type ec ) const
      { return CCZeroPi_type_to_label_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline std::string label( Particle_type ec ) const
      { return ReducedParticle_type_to_label_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline int color_code( EventCategory ec ) const
      { return event_category_to_color_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline int color_code( FidVol_Category ec ) const
      { return Containment_category_to_color_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
    inline int color_code( CCZeroPi_type ec ) const
      { return CCZeroPi_type_to_color_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
    inline int color_code( Particle_type ec ) const
      { return ReducedParticle_type_to_color_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
    inline int color_code( BDT_Category ec ) const
      { return BDT_category_to_color_map_.at( ec ); }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_mc_histogram_style( EventCategory ec, TH1* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_mc_histogram_style( EventCategory ec, TH2* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_mc_histogram_style( FidVol_Category ec, TH1* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////   
inline void set_mc_histogram_style( FidVol_Category ec, TH2* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////   
inline void set_mc_histogram_style( CCZeroPi_type ec, TH1* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////    
inline void set_mc_histogram_style( CCZeroPi_type ec, TH2* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////   
inline void set_mc_histogram_style( Particle_type ec, TH1* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////    
inline void set_mc_histogram_style( Particle_type ec, TH2* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_mc_histogram_style( BDT_Category ec, TH1* mc_hist ) const
    {
      int color = color_code( ec );
      mc_hist->SetFillColor( color );
      mc_hist->SetLineColor( color );
      mc_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_ext_histogram_style( TH1* ext_hist ) const {
      ext_hist->SetFillColor( 28 );
      ext_hist->SetLineColor( 28 );
      ext_hist->SetLineWidth( 2 );
      ext_hist->SetFillStyle( 3005 );
      ext_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_ext_histogram_style( TH2* ext_hist ) const {
      ext_hist->SetFillColor( 28 );
      ext_hist->SetLineColor( 28 );
      ext_hist->SetLineWidth( 2 );
      ext_hist->SetFillStyle( 3005 );
      ext_hist->SetStats( false );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_bnb_data_histogram_style( TH1* bnb_hist ) const {

      bnb_hist->SetLineColor( kBlack );
      bnb_hist->SetLineWidth( 3 );
      bnb_hist->SetMarkerStyle( kFullCircle );
      bnb_hist->SetMarkerSize( 0.8 );
      bnb_hist->SetStats( false );

      bnb_hist->GetXaxis()->SetTitleOffset( 0.0 );
      bnb_hist->GetXaxis()->SetTitleSize( 0.0 );
      bnb_hist->GetYaxis()->SetTitleSize( 0.05 );
      bnb_hist->GetYaxis()->CenterTitle( true );
      bnb_hist->GetXaxis()->SetLabelSize( 0.0 );

      // This prevents the first y-axis label label (0) to be clipped by the
      // ratio plot
      bnb_hist->SetMinimum( 1e-3 );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////   
inline void set_bnb_data_histogram_style( TH2* bnb_hist ) const {

      bnb_hist->SetLineColor( kBlack );
      bnb_hist->SetLineWidth( 3 );
      bnb_hist->SetMarkerStyle( kFullCircle );
      bnb_hist->SetMarkerSize( 0.8 );
      bnb_hist->SetStats( false );

      bnb_hist->GetXaxis()->SetTitleOffset( 0.0 );
      bnb_hist->GetXaxis()->SetTitleSize( 0.0 );
      bnb_hist->GetYaxis()->SetTitleSize( 0.05 );
      bnb_hist->GetYaxis()->CenterTitle( true );
      bnb_hist->GetXaxis()->SetLabelSize( 0.0 );

      // This prevents the first y-axis label label (0) to be clipped by the
      // ratio plot
      bnb_hist->SetMinimum( 1e-3 );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
inline void set_stat_err_histogram_style( TH1* stat_err_hist ) const {
      stat_err_hist->SetFillColor( kBlack );
      stat_err_hist->SetLineColor( kBlack );
      stat_err_hist->SetLineWidth( 2 );
      stat_err_hist->SetFillStyle( 3004 );
    }
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////   
inline void set_stat_err_histogram_style( TH2* stat_err_hist ) const {
      stat_err_hist->SetFillColor( kBlack );
      stat_err_hist->SetLineColor( kBlack );
      stat_err_hist->SetLineWidth( 2 );
      stat_err_hist->SetFillStyle( 3004 );
    } 
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////    
    // Function to return EventCateogry type but int
/*inline EventCategory returnEventCategoryType( int input) const{
      switch (input)
      {
        case 0:
        return kUnknown;
        
        case 1:
        return kSignalCCQE;
    
        case 2:
        return kSignalCCMEC;
    
        case 3:
        return kSignalCCRES;
    
        case 4:
        return kSignalOther;
    
        case 5:
        return kNuMuCCNpi;
    
        case 6:
        return kNuMuCCOther;
    
        case 7:
        return kNuECC;
    
        case 8:
        return kNC;
    
        case 9:
        return kOOFV;
        
        case 10:
        return kNuMuBarCC;
        
        case 11:
        return kOther;
        
        default:
        std::cout<<"UNknown interaction type Maybesomething is wrong INPUT = "<< input<< std::endl;
        return kOther;
  }
}// End of Function*/
inline EventCategory returnEventCategoryType( int input) const{
      switch (input)
      {
        case 0:
        return kUnknown;
        
        case 1:
        return kSignalCCQE;
    
        case 2:
        return kSignalCCMEC;
    
        case 3:
        return kSignalCCRES;
    
        case 4:
        return kSignalOther;
    
        case 5:
        return kNuMuCCNpi;
    
        case 7:
        return kNuMuCCOther;
    
        case 8:
        return kNuECC;
    
        case 9:
        return kNC;
    
        case 10:
        return kOOFV;
        
        case 11:
        return kOther;
        
        default:
        std::cout<<"UNknown interaction type Maybesomething is wrong INPUT = "<< input<< std::endl;
        return kOther;
  }
}// End of Function
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline BDT_Category returnBDTPredictionType( int input) const{
      switch (input)
      {
        case 0:
        return kBDT_Else;
        
        case 1:
        return kBDT_Muon;
    
        case 2:
        return kBDT_Pion;
    
        case 3:
        return kBDT_Proton;
    
        case 4:
        return kBDT_BOGUS;
    
        default:
        std::cout<<"UNknown BDT type Maybesomething is wrong !!"<< std::endl;
        return kBDT_BOGUS;
  }
}// End of Function
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline Particle_type  GetParticlegroup_type(int pdg) const {

  int count=-999;
  Particle_type Particle;

  //std::cout<< "pdg= "<<pdg<<std::endl;
 if(pdg == 11)                 {count=1;}//MeV electron
  else if (pdg == -11)          {count=2;} // e+
  else if (pdg == 111)          {count=3;} //Pion0
  else if (pdg == 211)          {count=4;} //Pion_plus
  else if (pdg == -211)         {count=5;} //Pion_neg
  else if (pdg == 321)          {count=6;} //Kaon_plus
  else if (pdg == -321)         {count=7;} //Kaon_neg
  else if (pdg == 311)          {count=8;} //Kaon_0)
  else if (pdg == -311)         {count=9;} //anti-Kaon_0)
  else if (pdg == 130)          {count=10;} //Kaon^0_L) //
  else if (pdg == 2212)         {count=11;} //Proton
  else if (pdg == -2212)        {count=12;} //antiProton
  else if (pdg == 2112 || pdg == -2112){count=13;} //neutron
  else if (pdg == 22)           {count=14;} //photon
  else if (pdg == -13)           {count=15;} //muon_plus
  else if (pdg == 13)           {count=16;} //muon_neg
  else if (pdg == 3122||pdg == -3122)         {count=17;} //Lamdba
  else if (pdg == 3222)         {count=18;} //Sigma_plus
  else if (pdg == 3212||pdg == -3212)         {count=19;} //Sigma_0
  else if (pdg == 221)          {count=20;} //eta
  else if (pdg == 1000130270)   {count=21;} //27Al
  else if (pdg == 1000020040)   {count=22;} //4He
  else if (pdg == 1000020030)   {count=23;} //3He
  else if (pdg == 1000010020)   {count=24;} //2 deuterium
  else if (pdg == 1000010060)   {count=25;} //16Oxygen)
  else if (pdg == 1000030060)   {count=26;} //Li6
  else if (pdg == 12)           {count=27;} // electron neutrino
  else if (pdg == -12)           {count=28;} // electron neutrino
  else if (pdg == 14)           {count=29;} // muon neutrino
  else if (pdg == -14)          {count=30;} // muon  antineutrino
  else if (pdg == 1000030070)   {count=31;}  // Li7Nucleus
  else if (pdg == -9999 || pdg == -999 )        {count=0;}  // particle has unknown trajectory thus no particle
  else {count= 0; }



  if (count == 0 ) {Particle = kParticle_N_A;}
  else if (count == 1 || count == 2)  {Particle = kElectron;}
  else if (count == 3) { Particle = kPion_0;}
  else if (count == 4) { Particle = kPion_pos;}
  else if (count == 5) { Particle = kPion_neg;}
  else if (count == 6 || count == 7 || count == 8 || count == 9 || count == 10) {Particle = kKaon;}
  else if (count == 11 || count == 12) {Particle = kProton;}
  else if (count == 13) {Particle = kNeutron;}
  else if (count == 14) {Particle = kGamma;}
  else if (count == 15 || count == 16) {Particle = kMuon;}
  else if ((count > 18 && count < 22) ||(count > 22 && count < 27) || count == 31) {Particle = kParticle_OTHER;}
  else if (count == 27 ) {Particle = kNeutrino_electron;}
  else if (count == 28 || count == 30 ) {Particle = kAnti_Neutrino;}
  else if (count == 29 ) {Particle = kNeutrino_muon;}
  else if (count == 22 ) {Particle = kFourHelium;}
  else if (count == 17 ) {Particle = kLamdba;}
  else if (count == 18 ) {Particle = kSigma_plus;}
  else{Particle = kParticle_N_A;}
  return Particle;
};

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline Particle_type  GetParticlegroup_typeReduced(int pdg) const {

  int count=-999;
  Particle_type Particle;

  //std::cout<< "pdg= "<<pdg<<std::endl;
 if(pdg == 11)                 {count=1;}//MeV electron
  else if (pdg == -11)          {count=2;} // e+
  else if (pdg == 111)          {count=3;} //Pion0
  else if (pdg == 211)          {count=4;} //Pion_plus
  else if (pdg == -211)         {count=5;} //Pion_neg
  else if (pdg == 321)          {count=6;} //Kaon_plus
  else if (pdg == -321)         {count=7;} //Kaon_neg
  else if (pdg == 311)          {count=8;} //Kaon_0)
  else if (pdg == -311)         {count=9;} //anti-Kaon_0)
  else if (pdg == 130)          {count=10;} //Kaon^0_L) //
  else if (pdg == 2212)         {count=11;} //Proton
  else if (pdg == -2212)        {count=12;} //antiProton
  else if (pdg == 2112 || pdg == -2112){count=13;} //neutron
  else if (pdg == 22)           {count=14;} //photon
  else if (pdg == -13)           {count=15;} //muon_plus
  else if (pdg == 13)           {count=16;} //muon_neg
  else if (pdg == 3122||pdg == -3122)         {count=17;} //Lamdba
  else if (pdg == 3222)         {count=18;} //Sigma_plus
  else if (pdg == 3212||pdg == -3212)         {count=19;} //Sigma_0
  else if (pdg == 221)          {count=20;} //eta
  else if (pdg == 1000130270)   {count=21;} //27Al
  else if (pdg == 1000020040)   {count=22;} //4He
  else if (pdg == 1000020030)   {count=23;} //3He
  else if (pdg == 1000010020)   {count=24;} //2 deuterium
  else if (pdg == 1000010060)   {count=25;} //16Oxygen)
  else if (pdg == 1000030060)   {count=26;} //Li6
  else if (pdg == 12)           {count=27;} // electron neutrino
  else if (pdg == -12)           {count=28;} // electron neutrino
  else if (pdg == 14)           {count=29;} // muon neutrino
  else if (pdg == -14)          {count=30;} // muon  antineutrino
  else if (pdg == 1000030070)   {count=31;}  // Li7Nucleus
  else if (pdg == -9999 || pdg == -999 )        {count=0;}  // particle has unknown trajectory thus no particle
  else if (pdg == 0){count = 32;}
  else {count= 0; }


  if (count == 0 ) {Particle = kParticle_OTHER;}
  else if (count == 1 || count == 2||count == 3||count == 14)  {Particle = kPion_0_Electron_kGamma;}
  else if (  count == 13|| count == 27||count == 28 ||count == 29||count == 30) { Particle = kParticle_neutral;}
  else if (count == 4||count == 5) { Particle = kPion_pos_neg;}
  else if (count == 11 || count == 12) {Particle = kProton;}
  else if (count == 15 || count == 16) {Particle = kMuon;}
  else if (count == 6 || count == 7 || count == 8 || count == 9 || count == 10) {Particle = kKaon;}
  else if ( (count > 18 && count < 22) ||(count > 22 && count < 27)
     || count == 31||count == 22||count == 17||count == 18) {Particle = kParticle_OTHER;}
  else if (count == 32){Particle = k_MicroBooNECOSMICs;}
  else{Particle = kParticle_OTHER;}

  return Particle;
}; //end of function
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline FidVol_Category GetContainmentType(bool input ,bool isSignal )const {
    
    if(isSignal){
      if(input == true){return kContained;}
      else{return kUnContained; }  
      }
    
    else{
        return kContainmentBG;
    } 
}//end of function  
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline FidVol_Category GetContainmentType_ForData(bool input  )const {
    
      if(input == true){return kContained;}
      else{return kUnContained; }  

}//end of function
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline CCZeroPi_type returnCCZeroPi_type(int NProton, bool isSignal) const {
  
  if(isSignal){
      switch (NProton)
      {
        case 0:
        return kCC_0P_ZeroPi;
        
        case 1:
        return kCC_1P_ZeroPi;
    
        case 2:
        return kCC_2P_ZeroPi;
    
        default:
        if (NProton >= 3) {return kCC_3orGreaterP_ZeroPi;}
        else{std::cout<<"something is wrong in returnCCZeroPi_type case not real: returning CCgreaterorequal3P0pion "<<std::endl;return kCC_3orGreaterP_ZeroPi;
      };
    }
   }
  
  else {return kCC_ZeroPi_BG;}

}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
inline CCZeroPi_type returnCCZeroPi_type_ForData(int NProton) const {
  
      switch (NProton)
      {
        case 0:
        return kCC_0P_ZeroPi;
        
        case 1:
        return kCC_1P_ZeroPi;
    
        case 2:
        return kCC_2P_ZeroPi;
    
        default:
        if (NProton >= 3) {
        return kCC_3orGreaterP_ZeroPi;}
        else{std::cout<<"something is wrong in returnCCZeroPi_type case not real: returning CCgreaterorequal3P0pion "<<std::endl;return kCC_3orGreaterP_ZeroPi;}
        
      };


}
////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////// 
 inline bool IsSignal(EventCategory input_Event)const {
       
       switch (input_Event)
      {
        case kUnknown:
        return false;
        case kSignalCCQE:
        return true;
        case kSignalCCMEC:
        return true;
        case kSignalCCRES:
        return true;
        case kSignalOther:
        return true;
        case kNuMuCCNpi:
        return false;
        case kNuMuCCOther:
        return false;
        case kNuECC:
        return false;
        case kNC:
        return false;
        case kOOFV:
        return false;
        case kOther:
        return false;
        
        default:
        std::cout<<"UNKNOWN EventCategory when calling:IsSignal() input = "<< input_Event << std::endl;
        return false; 
        };
 }/////////////////////////////////
 //// ENd of Function
 ///////////////////////////
  
//////////////////////////////////////////////////
// Private Function
////////////////////////////////////////////////// 
  private:
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
  EventCategoryInterpreter() {}

    std::map< EventCategory, std::string > event_category_to_label_map_ = {
      { kUnknown, "Unknown" },
      { kSignalCCQE, "Signal (CCQE)" },
      { kSignalCCMEC, "Signal (CCMEC)" },
      { kSignalCCRES, "Signal (CCRES)" },
      { kSignalOther, "Signal (Other)" },
      { kNuMuCCNpi, "#nu_{#mu} CCN#pi" },
      //{kNuMuBarCC , "#bar{#nu_{#mu}}-CC"},
      { kNuMuCCOther, "Other #nu_{#mu} CC" },
      { kNuECC, "#nu_{e} CC" },
      { kNC, "NC" },
      { kOOFV, "Out FV" },
      { kOther, "Other" }
    };
    
    
        std::map< EventCategory, std::string > event_category_to_label_HistNamemap_ = {
      { kUnknown, "Unknown" },
      { kSignalCCQE, "Signal-(CCQE)" },
      { kSignalCCMEC, "Signal-(CCMEC)" },
      { kSignalCCRES, "Signal-(CCRES)" },
      { kSignalOther, "Signal-(Other)" },
      { kNuMuCCNpi, "#nu_{#mu}-CCN#pi" },
      //{kNuMuBarCC , "#bar{#nu_{#mu}}-CC"},
      { kNuMuCCOther, "#nu_{#mu}-CCOther" },
      { kNuECC, "#nu_{e}-CC" },
      { kNC, "NC" },
      { kOOFV, "Out FV" },
      { kOther, "Other" }
    };
    
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////

    std::map< BDT_Category, std::string > BDT_category_to_label_map_ = {
      { kBDT_Else,   "BDT Else" },
      { kBDT_Muon,   "BDT Muon" },
      { kBDT_Pion,   "BDT Charged Pion"},
      { kBDT_Proton, "BDT Proton" },
      { kBDT_BOGUS, "Failed BDT Prediction" }
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
    std::map< FidVol_Category, std::string > Containment_to_label_map_ = {
      { kContained, "Muon Contained" },
      { kUnContained, "Muon Not Contained" },
      { kContainmentBG, "BG" }
      
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
    std::map< CCZeroPi_type, std::string > CCZeroPi_type_to_label_map_ = {      
      {kCC_0P_ZeroPi,          "CC0#pi"},
      {kCC_1P_ZeroPi,          "CC1p0#pi"},
      {kCC_2P_ZeroPi,          "CC2p0#pi"},
      {kCC_3orGreaterP_ZeroPi, "CC3>p0#pi"},
      {kCC_ZeroPi_BG,          "BG"}
    };

//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
    std::map< Particle_type, std::string > ReducedParticle_type_to_label_map_ = {      
        {k_MicroBooNECOSMICs,     "Cosmics"},
        {kParticle_OTHER,         "Other"},
        {kParticle_neutral,       "Neutral"},
        {kKaon,                   "K^{#pm}"},
        {kMuon,                   "#mu"},
        {kPion_0_Electron_kGamma, "e^{#pm}, #gamma, #pi^{0}"},
        {kPion_pos_neg,           "#pi^{#pm}"},
        {kProton,                 "p"}
    };

//////////////////////////////////////////////////
//
//////////////////////////////////////////////////

    std::map< EventCategory, int > event_category_to_color_map_ = {
      { kUnknown, kGray },
      { kSignalCCQE, kGreen },
      { kSignalCCMEC, kGreen + 1 },
      { kSignalCCRES, kGreen + 2 },
      { kSignalOther, kGreen + 3 },
      { kNuMuCCNpi, kAzure - 2 },
     // {kNuMuBarCC , kAzure - 1},
      { kNuMuCCOther, kAzure },
      { kNuECC, kViolet },
      { kNC, kOrange },
      
      { kOOFV, kRed + 3 },
      { kOther, kRed + 1 }
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////    
std::map< FidVol_Category, int > Containment_category_to_color_map_ = {
      { kContained,  kGreen },
      { kUnContained, kGreen + 3 },
      { kContainmentBG, kRed }
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////   
std::map< BDT_Category, int > BDT_category_to_color_map_ = {
      { kBDT_Else,   kAzure  },
      { kBDT_Muon,   kGreen},
      { kBDT_Pion,  kViolet},
      { kBDT_Proton, kRed + 3 },
      { kBDT_BOGUS, kTeal }
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
std::map<CCZeroPi_type, int > CCZeroPi_type_to_color_map_ = {      
      {kCC_0P_ZeroPi,          kGreen + 1},
      {kCC_1P_ZeroPi,          kGreen},
      {kCC_2P_ZeroPi,          kGreen + 2 },
      {kCC_3orGreaterP_ZeroPi, kGreen + 3},
      {kCC_ZeroPi_BG, kRed}
    };
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////    
std::map< Particle_type, int > ReducedParticle_type_to_color_map_ = {      
        {k_MicroBooNECOSMICs,     kRed},
        {kParticle_OTHER,         kOrange},
        {kParticle_neutral,       kGray},
        {kKaon,                   kTeal},
        {kMuon,                   kGreen},
        {kPion_0_Electron_kGamma, kViolet},
        {kPion_pos_neg,           kAzure - 1},
        {kProton,                 kRed + 3}
    };
    
    
};
//////////////////////////////////////////////////
// END of Class
//////////////////////////////////////////////////