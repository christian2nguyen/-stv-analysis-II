/*
 * =====================================================================================
 *
 *       Filename:  EventCategoriesCC1muXp0piFSI.cxx
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/24 14:49:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liang Liu (L. Liu), liangliu@fnal.gov
 *		    Fermi National Accelerator Laboratory
 *  Collaboration:  GENIE
 *
 * =====================================================================================
 */

#include "TH1.h"
#include "XSecAnalyzer/Selections/EventCategoriesCC1muXp0piFSI.hh"


std::map< int, std::pair< std::string, int > > CCXp0pi_FSI = {

  { kUnknown, {"Unknown", kGray }},

  { kNuMuCC0p0pi_CCQE, {"#nu_{#mu} 0p0pi QE", kBlue - 2}},
  { kNuMuCC0p0pi_CCMEC, {"#nu_{#mu} 0p0pi MEC", kBlue - 3}},
  { kNuMuCC0p0pi_CCRES, {"#nu_{#mu} 0p0pi RES", kBlue - 4}},
  { kNuMuCC0p0pi_Other, {"#nu_{#mu} 0p0pi Other", kBlue - 5}},

  { kNuMuCC1p0pi_CCQE_FSI, {"#nu_{#mu} 1p0pi QE (FSI)", kCyan - 2}},
  { kNuMuCC1p0pi_CCMEC_FSI, {"#nu_{#mu} 1p0pi MEC (FSI)", kCyan - 3}},
  { kNuMuCC1p0pi_CCRES_FSI, {"#nu_{#mu} 1p0pi RES (FSI)", kCyan - 4}},
  { kNuMuCC1p0pi_Other_FSI, {"#nu_{#mu} 1p0pi Other (FSI)", kCyan - 5}},

  { kNuMuCC1p0pi_CCQE_NONFSI, {"#nu_{#mu} 1p0pi QE (NONFSI)", kRed - 2}},
  { kNuMuCC1p0pi_CCMEC_NONFSI, {"#nu_{#mu} 1p0pi MEC (NONFSI)", kRed - 3}},
  { kNuMuCC1p0pi_CCRES_NONFSI, {"#nu_{#mu} 1p0pi RES (NONFSI)", kRed - 4}},
  { kNuMuCC1p0pi_Other_NONFSI, {"#nu_{#mu} 1p0pi Other (NONFSI)", kRed - 5}},

  { kNuMuCC2p0pi_CCQE, {"#nu_{#mu} 2p0pi QE", kMagenta - 1}},
  { kNuMuCC2p0pi_CCMEC, {"#nu_{#mu} 2p0pi MEC", kMagenta - 2}},
  { kNuMuCC2p0pi_CCRES, {"#nu_{#mu} 2p0pi RES", kMagenta - 3}},
  { kNuMuCC2p0pi_Other, {"#nu_{#mu} 2p0pi Other", kMagenta - 4}},


  { kNuMuCCMp0pi_CCQE, {"#nu_{#mu} Mp0pi CCQE", kGreen - 1}},
  { kNuMuCCMp0pi_CCMEC, {"#nu_{#mu} Mp0pi MEC", kGreen - 2}},
  { kNuMuCCMp0pi_CCRES, {"#nu_{#mu} Mp0pi RES", kGreen - 3}},
  { kNuMuCCMp0pi_Other, {"#nu_{#mu} Mp0pi Other", kGreen - 4}},

  { kNuMuCCNpi, {"#nu_{#mu} CCN#pi",kAzure - 2 }},
  { kNuMuCCOther, {"Other #nu_{#mu} CC",kAzure }},
  { kNuECC, {"#nu_{e} CC",kViolet }},
  { kNC, {"NC",kOrange }},
  { kOOFV, {"Out FV",kRed + 3 }},
  { kOther, {"Other",kRed + 1 }}


};


