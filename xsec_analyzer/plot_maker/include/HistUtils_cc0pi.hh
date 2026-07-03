#pragma once

#include <algorithm>

// ROOT includes
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TLegend.h"
#include "TObjArray.h"


// STV analysis includes

//#include "SliceBinning.hh"
//#include "SliceHistogram.hh"
#include "TLatex.h"
#include "TLine.h"
#include "TH2Poly.h"
//#include "UBTH2Poly.h" 
#include "GridCanvas.hh"
#include "/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/include/XSecAnalyzer/UBTH2Poly.hh"
//#include "ConfigMakerUtils.hh"
#include "EventCategory.hh"
#include "NamedCategory.hh"
#include "PlotUtils.hh"
#include "HistFolio_slim.hh"






struct BinMap {
double xlow;
double xhigh; 
double ylow; 
double yhigh; 
double centerx; 
double centery; 
};


struct TKI_parameters{

double Pproton;
double Pproton_mc;

double PCostheta;
double PCostheta_mc;

double pT;
double pT_mc;

double delta_alphaT;
double delta_alphaT_mc;

double delta_pTx;
double delta_pTx_mc;

double delta_pTy;
double delta_pTy_mc;

double delta_phiT;
double delta_phiT_mc;

double pn;
double pn_mc;

EventCategory category_type; 

};


TKI_parameters TKI_stuct(
double Pproton_input,
double Pproton_mc_input,
double PCostheta_input,
double PCostheta_mc_input,
double pT_input,
double pT_mc_input,
double delta_alphaT_input,
double delta_alphaT_mc_input,
double delta_pTx_input,
double delta_pTx_mc_input,
double delta_pTy_input,
double delta_pTy_mc_input,
double delta_phiT_input,
double delta_phiT_mc_input,
double pn_input,
double pn_mc_input,
EventCategory category_type_input
);

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////

class Vertex_XYZ {
 public:
  double x;
  double y;
  double z;

  Vertex_XYZ(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

  bool operator==(const Vertex_XYZ& vector) const {
    return (x == vector.x && y == vector.y && z == vector.z);
  }

  Vertex_XYZ& operator=(const Vertex_XYZ& vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
    return *this;
  }

  Vertex_XYZ operator+(const Vertex_XYZ& input) const {
    return Vertex_XYZ(x + input.x, y + input.y, z + input.z);
  }

  Vertex_XYZ operator-(const Vertex_XYZ& input) const {
    return Vertex_XYZ(x - input.x, y - input.y, z - input.z);
  }

  double DotProduct(const Vertex_XYZ& vector1, const Vertex_XYZ& vector2) {
    return vector1.x * vector2.x + vector1.y * vector2.y +
           vector1.z * vector2.z;
  }

  Vertex_XYZ CrossProduct(const Vertex_XYZ& vector1,
                          const Vertex_XYZ& vector2) {
    Vertex_XYZ inputVector;
    inputVector.x = vector1.y * vector2.z - vector1.z * vector2.y;
    inputVector.y = vector1.z * vector2.x - vector1.x * vector2.z;
    inputVector.z = vector1.x * vector2.y - vector1.y * vector2.x;
    return inputVector;
  }
};




std::vector<double> GetCCZeroPi_Binning(variable_binning inputVar);




class TKI_Hists{
#include "HistFolio_slim.hh"
public:
    TKI_Hists(const std::vector<NamedCategory<EventCategory>>& categories_input, std::string Name, int trackN)
        :Name_(Name), Ntracks_(trackN) {
        
char Histname[1024];

std::vector<double> Resolution_bins{-10,-5,-4,-2,-1.5,-1.0,-0.5,-.25,0,.25,0.5,1.0,1.5,2,4,5,10};

std::vector<double> h_PbinEdges =  GetCCZeroPi_Binning(kBINNING_Pmu_Proton);            
sprintf(Histname, "h_Pproton_Track_%i",trackN);                                
h_Pproton_ = new TH1D(Histname, Histname, h_PbinEdges.size() - 1, h_PbinEdges.data());
sprintf(Histname, "h_Pproton_Track_%i_TRUE",trackN);  
h_Pproton_TRUE_ = new TH1D(Histname, Histname, h_PbinEdges.size() - 1, h_PbinEdges.data());
sprintf(Histname, "h_Pproton_Track_%i_TRUE_RECO",trackN); 
h_Pproton_TRUE_RECO_ = new TH1D(Histname, Histname, h_PbinEdges.size() - 1, h_PbinEdges.data());
sprintf(Histname, "h_Pproton_Track_%i_Mig",trackN); 
h_Pproton_Mig_ = new TH2D(Histname, Histname, h_PbinEdges.size() - 1, h_PbinEdges.data(),h_PbinEdges.size() - 1, h_PbinEdges.data());
sprintf(Histname, "h_Pproton_Track_%i_EventCategory",trackN); 
h_Pproton_EventCategory_ = HistFolio<TH1D, EventCategory> (categories_input, Histname, h_PbinEdges, " ;Events");
sprintf(Histname, "h_Pproton_Resolution_Track_%i",trackN); 
h_Pproton_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());


h_Pproton_->SetDirectory(0);            h_Pproton_->Sumw2();
h_Pproton_TRUE_->SetDirectory(0); h_Pproton_TRUE_->Sumw2();
h_Pproton_TRUE_RECO_->SetDirectory(0); h_Pproton_TRUE_RECO_->Sumw2();
h_Pproton_Mig_->SetDirectory(0); h_Pproton_Mig_->Sumw2();


std::vector<double> proton_Costheta_binEdges =  GetCCZeroPi_Binning(kBINNING_Costheta_Proton);  
sprintf(Histname, "h_PCostheta_Track_%i",trackN); 
h_CosThetaproton_ = new TH1D(Histname, Histname, proton_Costheta_binEdges.size() - 1, proton_Costheta_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_TRUE",trackN); 
h_CosThetaproton_TRUE_  = new TH1D(Histname, Histname, proton_Costheta_binEdges.size() - 1, proton_Costheta_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_TRUE_RECO",trackN); 
h_CosThetaproton_TRUE_RECO_ = new TH1D(Histname, Histname, proton_Costheta_binEdges.size() - 1, proton_Costheta_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_Mig",trackN); 
h_CosThetaproton_Mig_ = new TH2D(Histname, Histname, proton_Costheta_binEdges.size() - 1, proton_Costheta_binEdges.data(),proton_Costheta_binEdges.size() - 1, proton_Costheta_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_EventCategory",trackN); 
h_CosThetaproton_EventCategory_= HistFolio<TH1D, EventCategory>(categories_input, Histname,proton_Costheta_binEdges, " ;Events");
sprintf(Histname, "h_pT_Resolution_Track_%i",trackN); 
h_CosThetaproton_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());
sprintf(Histname, "h_CosThetaproton_Resolution_Track_%i",trackN); 
h_CosThetaproton_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());



std::vector<double> pT_binEdges =  GetCCZeroPi_Binning(kBINNING_delta_pT);

sprintf(Histname, "h_PCostheta_Track_%i",trackN); 
h_delta_pT_= new TH1D(Histname, Histname, pT_binEdges.size() - 1, pT_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_TRUE",trackN);
h_delta_pT_TRUE_ = new TH1D(Histname, Histname, pT_binEdges.size() - 1, pT_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_TRUE_RECO",trackN); 
h_delta_pT_TRUE_RECO_ = new TH1D(Histname, Histname, pT_binEdges.size() - 1, pT_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_Mig",trackN); 
h_delta_pT_Mig_= new TH2D(Histname, Histname, pT_binEdges.size() - 1, pT_binEdges.data(),pT_binEdges.size() - 1, pT_binEdges.data());
sprintf(Histname, "h_PCostheta_Track_%i_EventCategory",trackN); 
h_pT_EventCategory_ = HistFolio<TH1D, EventCategory>(categories_input, Histname, pT_binEdges, " ;Events");
sprintf(Histname, "h_pT_Resolution_Track_%i",trackN); 
h_pT_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());




std::vector<double> delta_alphaT_binEdges =  GetCCZeroPi_Binning(kBINNING_delta_alphaT);
sprintf(Histname, "h_delta_alphaT_Track_%i",trackN); 
h_delta_alphaT_= new TH1D(Histname, Histname, delta_alphaT_binEdges.size() - 1, delta_alphaT_binEdges.data());
sprintf(Histname, "h_delta_alphaT_Track_%i_TRUE",trackN); 
h_delta_alphaT_TRUE_ = new TH1D(Histname, Histname, delta_alphaT_binEdges.size() - 1, delta_alphaT_binEdges.data());
sprintf(Histname, "h_delta_alphaT_Track_%i_TRUE_RECO",trackN); 
h_delta_alphaT_TRUE_RECO_ = new TH1D(Histname, Histname, delta_alphaT_binEdges.size() - 1, delta_alphaT_binEdges.data());
sprintf(Histname, "h_delta_alphaT_Track_%i_Mig",trackN); 
h_delta_alphaT_Mig_ = new TH2D(Histname, Histname, delta_alphaT_binEdges.size() - 1, delta_alphaT_binEdges.data(),delta_alphaT_binEdges.size() - 1, delta_alphaT_binEdges.data());
sprintf(Histname, "h_delta_alphaT_Track_%i_EventCategory",trackN); 
h_delta_alphaT_EventCategory_ = HistFolio<TH1D, EventCategory>(categories_input, Histname,delta_alphaT_binEdges, " ;Events");
sprintf(Histname, "h_delta_alphaT_Resolution_Track_%i",trackN); 
h_delta_alphaT_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());



std::vector<double> delta_pTx_binEdges =  GetCCZeroPi_Binning(kBINNING_delta_pTx);
sprintf(Histname, "h_delta_pTx_Track_%i",trackN); 
h_delta_pTx_ = new TH1D(Histname, Histname, delta_pTx_binEdges.size() - 1, delta_pTx_binEdges.data());
sprintf(Histname, "h_delta_pTx_Track_%i_TRUE",trackN); 
h_delta_pTx_TRUE_= new TH1D(Histname, Histname, delta_pTx_binEdges.size() - 1, delta_pTx_binEdges.data());
sprintf(Histname, "h_delta_pTx_Track_%i_TRUE_RECO",trackN);
h_delta_pTx_TRUE_RECO_= new TH1D(Histname, Histname, delta_pTx_binEdges.size() - 1, delta_pTx_binEdges.data());
sprintf(Histname, "h_delta_pTx_Track_%i_Mig",trackN); 
h_delta_pTx_Mig_= new TH2D(Histname, Histname, delta_pTx_binEdges.size() - 1, delta_pTx_binEdges.data(),delta_pTx_binEdges.size() - 1, delta_pTx_binEdges.data());
sprintf(Histname, "h_delta_pTx_Track_%i_EventCategory",trackN); 
h_delta_pTx_EventCategory_ = HistFolio<TH1D, EventCategory>(categories_input, Histname, delta_pTx_binEdges, " ;Events");
sprintf(Histname, "h_delta_pTx_Resolution_Track_%i",trackN); 
h_delta_pTx_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());



std::vector<double> delta_pTy_binEdges =  GetCCZeroPi_Binning(kBINNING_delta_pTy);
sprintf(Histname, "h_delta_pTy_Track_%i",trackN); 
h_delta_pTy_ = new TH1D(Histname, Histname, delta_pTy_binEdges.size() - 1, delta_pTy_binEdges.data());
sprintf(Histname, "h_delta_pTy_Track_%i_TRUE",trackN); 
h_delta_pTy_TRUE_ = new TH1D(Histname, Histname, delta_pTy_binEdges.size() - 1, delta_pTy_binEdges.data());
sprintf(Histname, "h_delta_pTy_Track_%i_TRUE_RECO",trackN);
h_delta_pTy_TRUE_RECO_ = new TH1D(Histname, Histname, delta_pTy_binEdges.size() - 1, delta_pTy_binEdges.data());
sprintf(Histname, "h_delta_pTy_Track_%i_Mig",trackN); 
h_delta_pTy_Mig_ = new TH2D(Histname, Histname, delta_pTy_binEdges.size() - 1, delta_pTy_binEdges.data(),delta_pTy_binEdges.size() - 1, delta_pTy_binEdges.data());
sprintf(Histname, "h_delta_pTy_Track_%i_EventCategory",trackN); 
h_delta_pTy_EventCategory_ = HistFolio<TH1D, EventCategory> (categories_input, Histname, delta_pTy_binEdges, " ;Events");
sprintf(Histname, "h_delta_pTy_Resolution_Track_%i",trackN); 
h_delta_pTy_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());



std::vector<double> delta_phiT_binEdges =  GetCCZeroPi_Binning(kBINNING_delta_phiT);
sprintf(Histname, "h_delta_phiT_Track_%i",trackN);
h_delta_phiT_  = new TH1D(Histname, Histname, delta_phiT_binEdges.size() - 1, delta_phiT_binEdges.data());
sprintf(Histname, "h_delta_phiT_Track_%i_TRUE",trackN); 
h_delta_phiT_TRUE_ = new TH1D(Histname, Histname, delta_phiT_binEdges.size() - 1, delta_phiT_binEdges.data());
sprintf(Histname, "h_delta_phiT_Track_%i_TRUE_RECO",trackN);
h_delta_phiT_TRUE_RECO_  = new TH1D(Histname, Histname, delta_phiT_binEdges.size() - 1, delta_phiT_binEdges.data());
sprintf(Histname, "h_delta_phiT_Track_%i_Mig",trackN); 
h_delta_phiT_Mig_ = new TH2D(Histname, Histname, delta_phiT_binEdges.size() - 1, delta_phiT_binEdges.data(),delta_phiT_binEdges.size() - 1, delta_phiT_binEdges.data());
sprintf(Histname, "h_delta_phiT_Track_%i_EventCategory",trackN); 
h_delta_phiT_EventCategory_  = HistFolio<TH1D, EventCategory>(categories_input, Histname, delta_phiT_binEdges, " ;Events");
sprintf(Histname, "h_delta_phiT_Resolution_Track_%i",trackN); 
h_delta_phiT_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());



std::vector<double> pn_binEdges =  GetCCZeroPi_Binning(kBINNING_pn);
sprintf(Histname, "h_pn_Track_%i",trackN);
h_pn_= new TH1D(Histname, Histname, pn_binEdges.size() - 1, pn_binEdges.data());
sprintf(Histname, "h_pn_Track_%i_TRUE",trackN); 
h_pn_TRUE_= new TH1D(Histname, Histname, pn_binEdges.size() - 1, pn_binEdges.data());
sprintf(Histname, "h_pn_Track_%i_TRUE_RECO",trackN);
h_pn_TRUE_RECO_= new TH1D(Histname, Histname, pn_binEdges.size() - 1, pn_binEdges.data());
sprintf(Histname, "h_pn_Track_%i_Mig",trackN); 
h_pn_Mig_  = new TH2D(Histname, Histname, pn_binEdges.size() - 1, pn_binEdges.data(),pn_binEdges.size() - 1, pn_binEdges.data());
sprintf(Histname, "h_pn_Track_%i_EventCategory",trackN); 
h_pn_EventCategory_ = HistFolio<TH1D, EventCategory>(categories_input, Histname, pn_binEdges, " ;Events");       
sprintf(Histname, "h_pn_Resolution_Track_%i",trackN); 
h_pn_Resolution_ = new TH1D(Histname, Histname, Resolution_bins.size() - 1, Resolution_bins.data());



h_CosThetaproton_->SetDirectory(0);
h_CosThetaproton_TRUE_->SetDirectory(0);
h_CosThetaproton_TRUE_RECO_ ->SetDirectory(0);
h_CosThetaproton_Mig_->SetDirectory(0);
h_delta_alphaT_->SetDirectory(0);
h_delta_alphaT_TRUE_->SetDirectory(0);
h_delta_alphaT_TRUE_RECO_->SetDirectory(0);
h_delta_alphaT_Mig_->SetDirectory(0);
h_delta_pTx_->SetDirectory(0);
h_delta_pTx_TRUE_->SetDirectory(0);
h_delta_pTx_TRUE_RECO_->SetDirectory(0);
h_delta_pTx_Mig_->SetDirectory(0);
h_delta_pTy_->SetDirectory(0);
h_delta_pTy_TRUE_->SetDirectory(0);
h_delta_pTy_TRUE_RECO_->SetDirectory(0);
h_delta_pTy_Mig_->SetDirectory(0);
h_delta_phiT_->SetDirectory(0);
h_delta_phiT_TRUE_->SetDirectory(0);
h_delta_phiT_TRUE_RECO_->SetDirectory(0);
h_delta_phiT_Mig_->SetDirectory(0);
h_pn_->SetDirectory(0);
h_pn_TRUE_->SetDirectory(0);
h_pn_TRUE_RECO_->SetDirectory(0);
h_pn_Mig_->SetDirectory(0);

h_Pproton_Resolution_->SetDirectory(0);
h_CosThetaproton_Resolution_->SetDirectory(0);
h_pT_Resolution_->SetDirectory(0);
h_delta_alphaT_Resolution_->SetDirectory(0);
h_delta_pTx_Resolution_->SetDirectory(0);
h_delta_pTy_Resolution_->SetDirectory(0);
h_delta_phiT_Resolution_->SetDirectory(0);
h_pn_Resolution_->SetDirectory(0);


//////
h_CosThetaproton_->Sumw2();
h_CosThetaproton_TRUE_->Sumw2();
h_CosThetaproton_TRUE_RECO_ ->Sumw2();
h_CosThetaproton_Mig_->Sumw2();
h_delta_alphaT_->Sumw2();
h_delta_alphaT_TRUE_->Sumw2();
h_delta_alphaT_TRUE_RECO_->Sumw2();
h_delta_alphaT_Mig_->Sumw2();
h_delta_pTx_->Sumw2();
h_delta_pTx_TRUE_->Sumw2();
h_delta_pTx_TRUE_RECO_->Sumw2();
h_delta_pTx_Mig_->Sumw2();
h_delta_pTy_->Sumw2();
h_delta_pTy_TRUE_->Sumw2();
h_delta_pTy_TRUE_RECO_->Sumw2();
h_delta_pTy_Mig_->Sumw2();
h_delta_phiT_->Sumw2();
h_delta_phiT_TRUE_->Sumw2();
h_delta_phiT_TRUE_RECO_->Sumw2();
h_delta_phiT_Mig_->Sumw2();
h_pn_->Sumw2();
h_pn_TRUE_->Sumw2();
h_pn_TRUE_RECO_->Sumw2();
h_pn_Mig_->Sumw2();

h_Pproton_Resolution_->Sumw2();
h_CosThetaproton_Resolution_->Sumw2();
h_pT_Resolution_->Sumw2();
h_delta_alphaT_Resolution_->Sumw2();
h_delta_pTx_Resolution_->Sumw2();
h_delta_pTy_Resolution_->Sumw2();
h_delta_phiT_Resolution_->Sumw2();
h_pn_Resolution_->Sumw2();



}


void FillRECOHist(TKI_parameters input, double wgt);
void FillTRUEHist(TKI_parameters input, double wgt);
void FillTRUE_RECOHist(TKI_parameters input, double wgt);
void WriteAll(TFile &outfile) const;

 private:
 std::string Name_; 
 int Ntracks_; 
 std::vector<EventCategory> categories;
 
 TH1D *h_Pproton_;
 TH1D *h_CosThetaproton_;
 TH1D *h_delta_pT_;
 TH1D *h_delta_alphaT_;
 TH1D *h_delta_pTx_;
 TH1D *h_delta_pTy_;
 TH1D *h_delta_phiT_;
 TH1D *h_pn_;
 
 TH1D *h_Pproton_TRUE_;
 TH1D *h_CosThetaproton_TRUE_;
 TH1D *h_delta_pT_TRUE_;
 TH1D *h_delta_alphaT_TRUE_;
 TH1D *h_delta_pTx_TRUE_;
 TH1D *h_delta_pTy_TRUE_;
 TH1D *h_delta_phiT_TRUE_;
 TH1D *h_pn_TRUE_;   
     
 TH1D *h_Pproton_TRUE_RECO_;
 TH1D *h_CosThetaproton_TRUE_RECO_;
 TH1D *h_delta_pT_TRUE_RECO_;
 TH1D *h_delta_alphaT_TRUE_RECO_;
 TH1D *h_delta_pTx_TRUE_RECO_;
 TH1D *h_delta_pTy_TRUE_RECO_;
 TH1D *h_delta_phiT_TRUE_RECO_;
 TH1D *h_pn_TRUE_RECO_;       
     
 TH2D *h_Pproton_Mig_;
 TH2D *h_CosThetaproton_Mig_;
 TH2D *h_delta_pT_Mig_;
 TH2D *h_delta_alphaT_Mig_;
 TH2D *h_delta_pTx_Mig_;
 TH2D *h_delta_pTy_Mig_;
 TH2D *h_delta_phiT_Mig_;
 TH2D *h_pn_Mig_;  
     
 HistFolio<TH1D, EventCategory> h_Pproton_EventCategory_;
 HistFolio<TH1D, EventCategory> h_CosThetaproton_EventCategory_;
 HistFolio<TH1D, EventCategory> h_pT_EventCategory_;
 HistFolio<TH1D, EventCategory> h_delta_alphaT_EventCategory_;
 HistFolio<TH1D, EventCategory> h_delta_pTx_EventCategory_;
 HistFolio<TH1D, EventCategory> h_delta_pTy_EventCategory_;
 HistFolio<TH1D, EventCategory> h_delta_phiT_EventCategory_;
 HistFolio<TH1D, EventCategory> h_pn_EventCategory_;    
    
    
TH1D *h_Pproton_Resolution_;
TH1D *h_CosThetaproton_Resolution_;
TH1D *h_pT_Resolution_;
TH1D *h_delta_alphaT_Resolution_;
TH1D *h_delta_pTx_Resolution_;
TH1D *h_delta_pTy_Resolution_;
TH1D *h_delta_phiT_Resolution_;
TH1D *h_pn_Resolution_;    
    
    
    


    
    
};



typedef std::map<Binning2D, std::vector<int>> PROJECTION_Bin_Map;
typedef std::map<Binning2DInclusive, std::vector<int>> PROJECTION_InclusBin_Map;

PROJECTION_Bin_Map GetBin_ProjectionMap(); 
PROJECTION_InclusBin_Map GetInclusvieBin_ProjectionMap(); 


std::vector<Binning2D> GetProjectBinVector(); 
std::vector<Binning2DInclusive> GetProjectInclusiveBinVector();
std::map<Binning2D , std::string > Projection9Bins_StringMap(std::map< double, std::vector<double> > InputBins, std::string Par_name);
std::map<Binning2D , double> Projection9Bins_width(std::map< double, std::vector<double> > InputBins);
////////////////////////////////////////////////////////////////////////////////
TH1D* GetTH1DHist(TFile& fin, const char* name);
////////////////////////////////////////////////////////////////////////////////
TH1D* GetTH1DHist(TFile& fin, std::string name );
////////////////////////////////////////////////////////////////////////////////
TH2D* GetTH2DHist(TFile& fin, const char* name) ;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
TH2D* GetTH2DHist(TFile& fin, std::string name) ;
////////////////////////////////////////////////////////////////////////////////
std::map<Binning2D, TH1D*> ConstructProjectionMap(
TFile& fin,std::vector<Binning2D> Bin_vector, char *BaseName );
////////////////////////////////////////////////////////////////////////////////
std::map<std::pair<Binning2D, EventCategory>, TH1D*> Construct_CategoryProjectionMap(
 TFile& fin,
 std::vector<Binning2D> Bin_vector,
 std::vector<EventCategory> Stack_category, 
 char *BaseName );
////////////////////////////////////////////////////////////////////////////////
TH2Poly* Make2DHist(std::map<double,std::vector<double>> Bin_edges, std::map<int , BinMap> &TH1Poly_binMap, char *name);
////////////////////////////////////////////////////////////////////////////////
UBTH2Poly* Make2DHist_UB(std::map<double,std::vector<double>> Bin_edges, std::map<int , BinMap> &TH1Poly_binMap, char *name);
////////////////////////////////////////////////////////////////////////////////
TH2Poly* Make2DHist_inclusive(std::map<double,std::vector<double>> Bin_edges, std::map<int , BinMap> &TH1Poly_binMap, char *name);
////////////////////////////////////////////////////////////////////////////////
UBTH2Poly* Make2DHist_inclusive_UB(std::map<double,std::vector<double>> Bin_edges, std::map<int , BinMap> &TH1Poly_binMap, char *name);
////////////////////////////////////////////////////////////////////////////////
UBTH2Poly* Make2DHist_UB( std::map<double,std::vector<double>> Bin_edges, char *name);
////////////////////////////////////////////////////////////////////////////////
UBTH2Poly* Make2DHist_UB_inclusive( std::map<double,std::vector<double>> Bin_edges, char *name);
////////////////////////////////////////////////////////////////////////////////
bool checkConditions_ProtonBDT( std::vector<float> vector1_BDT_prediction, 
 std::vector<int> vector2_BDT_PID,
float threshold1, int pdg_type);
////////////////////////////////////////////////////////////////////////////////
template <typename EnumType>
std::map<EnumType, TH2D*> MakeNamedCategoryMap_TH2D(TFile& fin, 
std::string histBase_name, const std::vector<NamedCategory<EnumType>> &categories )
{
std::map<EnumType, TH2D*> OutMap_result;
char Histtitle[1024];
   std::cout << "Processing categories:" << std::endl;
    for (const auto& namedCategory : categories) {
    std::string HistName= histBase_name + "_" + namedCategory.m_name;
    const char* name_char = namedCategory.m_name.c_str();
    TH2D* HistInput= GetTH2DHist(fin, HistName);
    HistInput->SetTitle(name_char);
    OutMap_result[namedCategory.m_value]=HistInput;
     std::cout << "Category: " << static_cast<int>(namedCategory.m_value) << ", Name: " <<  HistName << std::endl;
     
    }

return OutMap_result;

}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////////////////////
 
template <typename EnumType>
std::map<EnumType, TH1D*> MakeNamedCategoryMap_TH1D(TFile& fin, 
std::string histBase_name, const std::vector<NamedCategory<EnumType>> &categories )
{
std::map<EnumType, TH1D*> OutMap_result;
char Histtitle[1024];
   std::cout << "Processing categories:" << std::endl;
    for (const auto& namedCategory : categories) {
    std::string HistName= histBase_name + "_" + namedCategory.m_name;
    const char* name_char = namedCategory.m_name.c_str();
    TH1D* HistInput= GetTH1DHist(fin, HistName);
    HistInput->SetTitle(name_char);
    OutMap_result[namedCategory.m_value]=HistInput;
     std::cout << "Category: " << static_cast<int>(namedCategory.m_value) << ", Name: " <<  HistName << std::endl;
     
    }

return OutMap_result;

}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////////////////////
//std::vector<double> get_bin_low_edges( double xmin, double xmax, int Nbins );
////////////////////////////////////////////////////////////////////////////////
std::vector<double> generateBins();
////////////////////////////////////////////////////////////////////////////////
std::vector<double> generateBins(int binsPerInterval, int numIntervals, float spacing  );
////////////////////////////////////////////////////////////////////////////////
void DrawBinningInfo(std::map<int , BinMap> TH1Poly_binMap_input);
void DrawBinningInfo_Area(std::map<int , BinMap> TH1Poly_binMap_input);
////////////////////////////////////////////////////////////////////////////////
void DrawBinningNum(std::map<int , BinMap> TH1Poly_binMap_input);
////////////////////////////////////////////////////////////////////////////////
UBTH2Poly *Make2DHist_UB_inclusive_BinNumMap( std::map<double,std::vector<double>> Bin_edges, char *name);
////////////////////////////////////////////////////////////////////////////////
void saveUBTH2PolyToTextFile(const UBTH2Poly& hist, char* fileName);
////////////////////////////////////////////////////////////////////////////////
double MaxYofMap(std::map<Binning2D, TH1D*> inputMap);
////////////////////////////////////////////////////////////////////////////////
void BinNormalizeTOFractionOF_Events(TObjArray &input_Array);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_X_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ> input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_Y_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_R_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_RR_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_X_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ> input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_R_vs_Y_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_RR_vs_Y_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_R_vs_Y_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_RR_vs_X_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_R_vs_X_Tgraph_fromVector(std::vector<Vertex_XYZ>input);
////////////////////////////////////////////////////////////////////////////////
GENIE_MC GENIE_Type(int input, bool combined);
Particle_top_groups return_Particle_top_groups(int Npion, int Nprotons, int NComsics, int Neletrons, int NOther, int NMuon);
void addTextToLastEmptyLine(const std::string& filename, const std::string& content);
void recreateFileWithFirstLine(const std::string& filename, const std::string& firstLineContent);
////////////////////////////////////////////////////////////////////////////////
MuonExitingPanel ExitingLoction(float Exit_X,float Exit_Y,float Exit_Z);