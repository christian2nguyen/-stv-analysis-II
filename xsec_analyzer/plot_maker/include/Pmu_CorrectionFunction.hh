///////////////////
/////// Pmu Correction Function From Panos : Christian's Version 1/10/24
//////////////////
//
// See for more Info on Study https://microboone-docdb.fnal.gov/cgi-bin/sso/RetrieveFile?docid=41275&filename=PanagiotisEnglezos_12_12_uB_xsec_meeting.pdf&version=1
//
//////////////////
#pragma once
// Standard library includes
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// ROOT includes
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTreeFormula.h"

// STV analysis includes
#include "EventCategory.hh"
//#include "TreeUtils.hh"
//#include "WeightHandler.hh"

/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
namespace FittedPars_Pmu_MuonContained {
// the functional form will be 
// A +Bx + Cx^2 +Dx^3 + ... 
static const double  Range1_A =   0.59; //   0.58546;
static const double  Range1_B = - 6.89; // - 6.88745;
static const double  Range1_C =  20.63; //  20.6256;
//
static const double  Range2_A =   0.03; //   0.0321782;
static const double  Range2_B = - 0.010; // - 0.010168;
static const double  Range2_C = - 0.014; // - 0.0136456;
//
//
static const double  Range3_A = 1.96;//  1.96012 ;
static const double  Range3_B =-3.35;// -3.34577 ;
static const double  Range3_C = 1.97;//  1.96595 ;
static const double  Range3_D =-0.41;// -0.411269;


}

/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

namespace FittedPars_Pmu_Muon_NOTContained {
// the functional form will be 
// A +Bx + Cx^2 +Dx^3 + ... 

static const double  Range1_A =   .18;
static const double  Range1_B =   -.15;
static const double  Range1_C =   -.02;

static const double  Range2_A =   -0.98;
static const double  Range2_B =  1.63;
static const double  Range2_C = - 0.73;

static const double  Range3_A =  1.9;
static const double  Range3_B =   -1.34;
static const double  Range3_C =  .03;

static const double  Range4_A =   3.42119 ;
static const double  Range4_B = - 2.19198 ;
static const double  Range4_C =   0.160376 ;

}
/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
double polynomialDegree2(double Input, double A, double B, double C){
 return A + B * Input + C * std::pow(Input,2); 
}
/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
double polynomialDegree3(double Input, double A, double B, double C, double D){
 return A + B * Input + C * std::pow(Input,2)+ D * std::pow(Input,3); 
}

/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
double  Pmu_correction_Muon_Contained(double Pmu_input, bool muon_quality){

if(Pmu_input <= 0.2 && !muon_quality){
return 0;
}
	else if (Pmu_input <= 0.2 && muon_quality){
	        return polynomialDegree2(Pmu_input, 
	                FittedPars_Pmu_MuonContained::Range1_A,
					FittedPars_Pmu_MuonContained::Range1_B,
					FittedPars_Pmu_MuonContained::Range1_C);
		}
		
	else if ( 0.2 < Pmu_input && Pmu_input  < 1.5){
					return polynomialDegree2(Pmu_input, 
					        FittedPars_Pmu_MuonContained::Range2_A,
							FittedPars_Pmu_MuonContained::Range2_B,
							FittedPars_Pmu_MuonContained::Range2_C);
                    }
                    
    else if ( 1.5 <= Pmu_input ){
	return polynomialDegree3(Pmu_input, 
	          FittedPars_Pmu_MuonContained::Range3_A,
			  FittedPars_Pmu_MuonContained::Range3_B,
			  FittedPars_Pmu_MuonContained::Range3_C,
			  FittedPars_Pmu_MuonContained::Range3_D);
			}

    else{
     std::cout<<"Out of Range for Pmu_correction_MuonContained this shouldn't happen something is wrong"<< std::endl;
     assert(false);
     }
}
/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
/*
double  Pmu_correction_Muon_NOTContained_old(double Pmu_input){
	if (Pmu_input <= 0.3){
	        return polynomialDegree2(Pmu_input,
	                    FittedPars_Pmu_Muon_NOTContained::Range1_A,
						FittedPars_Pmu_Muon_NOTContained::Range1_B,
						FittedPars_Pmu_Muon_NOTContained::Range1_C);
		}	
	else if ( 0.3 < Pmu_input && Pmu_input  < 1.2){
					return polynomialDegree2(Pmu_input, 
					        FittedPars_Pmu_Muon_NOTContained::Range2_A,
							FittedPars_Pmu_Muon_NOTContained::Range2_B,
							FittedPars_Pmu_Muon_NOTContained::Range2_C);
                    }
                    
    else if ( 1.2 <= Pmu_input && Pmu_input  < 1.5 ){
				return polynomialDegree2(Pmu_input,
					FittedPars_Pmu_Muon_NOTContained::Range3_A,
					FittedPars_Pmu_Muon_NOTContained::Range3_B,
					FittedPars_Pmu_Muon_NOTContained::Range3_C);
            }
    else if ( 1.5 <= Pmu_input ){
			return polynomialDegree2(Pmu_input,
					FittedPars_Pmu_Muon_NOTContained::Range4_A,
					FittedPars_Pmu_Muon_NOTContained::Range4_B,
					FittedPars_Pmu_Muon_NOTContained::Range4_C);
			}						    
	else{
      std::cout<<"Out of Range for Pmu_correction_Muon NOT Contained this shouldn't happen something is wrong"<< std::endl;
      assert(false);
      }
      
}
*/
/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
double  Pmu_correction_Muon_NOTContained(double Pmu_input){
	if (Pmu_input <= 0.2){
	        return 0.0;
		}	
		
	else if ( 0.2 < Pmu_input && Pmu_input  < 1.2){
					return polynomialDegree2(Pmu_input, 
					        FittedPars_Pmu_Muon_NOTContained::Range1_A,
							FittedPars_Pmu_Muon_NOTContained::Range1_B,
							FittedPars_Pmu_Muon_NOTContained::Range1_C);
                    }
                    
    else if ( 1.2 <= Pmu_input && Pmu_input  < 2.2 ){
				return polynomialDegree2(Pmu_input,
					FittedPars_Pmu_Muon_NOTContained::Range2_A,
					FittedPars_Pmu_Muon_NOTContained::Range2_B,
					FittedPars_Pmu_Muon_NOTContained::Range2_C);
            }
    else if ( 2.2 <= Pmu_input ){
			return polynomialDegree2(Pmu_input,
					FittedPars_Pmu_Muon_NOTContained::Range3_A,
					FittedPars_Pmu_Muon_NOTContained::Range3_B,
					FittedPars_Pmu_Muon_NOTContained::Range3_C);
			}						    
	else{
      std::cout<<"Out of Range for Pmu_correction_Muon NOT Contained this shouldn't happen something is wrong"<< std::endl;
      assert(false);
      }
      
}
/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////     
double  Pmu_PANOS_Correction(double Pmu_input, bool MuonIscontained, bool muon_quality){

if(MuonIscontained==true){return Pmu_correction_Muon_Contained(Pmu_input, muon_quality);}
else{return Pmu_correction_Muon_NOTContained(Pmu_input);}

}