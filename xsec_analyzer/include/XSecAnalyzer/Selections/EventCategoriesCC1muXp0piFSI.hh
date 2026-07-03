#pragma once

// Standard library includes
#include <map>
#include <string>

// Enum used to label event categories of interest for analysis plots in
// the CC0pi 1p/2p/Np/Xp analyses
enum EventCategoryCCXp0piFSI {

  // Unable to categorize (e.g., because the event is real data and thus
  // has no MC truth information)
  kUnknown = 0,

  // Signal events broken down by underlying reaction mode
  // a test to study the 1p fsi model
  kNuMuCC0p0pi_CCQE,  // 1
  kNuMuCC0p0pi_CCMEC, // 2
  kNuMuCC0p0pi_CCRES, // 3
  kNuMuCC0p0pi_Other, // 4


  kNuMuCC1p0pi_CCQE_FSI,  // 5
  kNuMuCC1p0pi_CCMEC_FSI, // 6
  kNuMuCC1p0pi_CCRES_FSI, // 7
  kNuMuCC1p0pi_Other_FSI, // 8

  kNuMuCC1p0pi_CCQE_NONFSI,  // 9
  kNuMuCC1p0pi_CCMEC_NONFSI, // 10
  kNuMuCC1p0pi_CCRES_NONFSI, // 11
  kNuMuCC1p0pi_Other_NONFSI, // 12

  kNuMuCC2p0pi_CCQE, // 13
  kNuMuCC2p0pi_CCMEC, // 14
  kNuMuCC2p0pi_CCRES, // 15
  kNuMuCC2p0pi_Other, // 16

  // M > 2
  kNuMuCCMp0pi_CCQE,  // 17
  kNuMuCCMp0pi_CCMEC, // 18
  kNuMuCCMp0pi_CCRES, // 19
  kNuMuCCMp0pi_Other, // 20

  // True numu CC event with at least one final-state pion above threshold
  kNuMuCCNpi, // 21

  // True numu CC event with zero final-state pions above threshold and
  // zero final-state protons above threshold
  //  Will keep  kNuMuCC0pi0p enum but removed from categories and now this type is funnled into signal region
  
  // Any true numu CC event which does not satisfy the criteria for inclusion
  // in one of the other categories above
  kNuMuCCOther, // 22

  // True nue CC event
  kNuECC,  //  23

  // True neutral current event for any neutrino flavor
  kNC, //  24

  // True neutrino vertex (any reaction mode and flavor combination) is outside
  // of the fiducial volume
  kOOFV, // 25

  // Seperate numubar CC from kOther
  //kNuMuBarCC = 13,
  // All events that do not fall within any of the other categories (e.g.,)
  kOther,
  
};

extern std::map< int, std::pair< std::string, int > > CCXp0pi_FSI;



