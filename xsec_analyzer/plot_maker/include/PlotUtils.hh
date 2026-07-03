#pragma once

// Standard library includes
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>


// ROOT includes
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TFile.h"
#include "THStack.h"
#include "TLegend.h"
#include "TObjArray.h"
#include "TMatrixD.h"
#include "TStyle.h"
#include <fstream>
#include "TLatex.h"
#include "TLine.h"
#include "TArrow.h"
#include "HistUtils_cc0pi.hh"
#include "GridCanvas.hh"
  
#include "TH2Poly.h"  
//#include "UBTH2Poly.h"  
#include "/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/include/XSecAnalyzer/UBTH2Poly.hh"
#include "EventCategory.hh"
//#include "/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/include/XSecAnalyzer/SliceBinning.hh"
//#include "/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/include/XSecAnalyzer/SliceHistogram.hh"
//#include "../SliceBinning.hh"
//#include "../SliceHistogram.hh"
  
template<typename T>
T findMax(const T& a, const T& b) {
    return std::max(a, b);
}  
  
  
// Helper function that produces the standard MicroBooNE plot legend title
// with the BNB POT displayed
std::string get_legend_title( double bnb_pot );
TString uniq();

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 3)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
// Helper function that dumps 1D histogram contents to a map of pgfplotstable
// columns
void dump_1d_histogram( 
const std::string& hist_col_prefix,
  const TH1D& hist,
  std::map< std::string, std::vector<std::string> >& pgf_plots_hist_table,
  bool include_yerror = true, bool include_x_coords = false );
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
// Helper function that dumps TGraph contents to a map of pgfplotstable columns
// TODO: add support for TGraphErrors
void dump_tgraph( const std::string& col_prefix, const TGraph& graph,
  std::map< std::string, std::vector<std::string> >& pgf_plots_table );
 ////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////// 
void write_pgfplots_file( const std::string& out_filename,
  std::map< std::string, std::vector<std::string> >& pgfplots_table );
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void colNormalize(TH2& hist, bool includeFlows);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void rowNormalize(TH2& hist, bool includeFlows);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void ScaleHistogramsInStack(THStack* stack, double scaleFactor, char *option);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void ScaleHistogramsInStack(THStack* stack, double scaleFactor );
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
double GetMaxFromProjectionY(TH2 *h1, TH2 *h2, bool checkBinwidth);
double GetMaxFromProjectionX(TH2 *h1,TH2 *h2, bool checkBinwidth);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
double GetMaxFromProjectionY(TH2 *h1, bool checkBinwidth);
double GetMaxFromProjectionX(TH2 * h1, bool checkBinwidth);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void drawBinRange(TH2* h, int axis, int bin, const char* varName, double text_size, 
const char* numFormatStr=".2f", bool left=false );
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void drawString(std::string inputString, double text_size,  bool left );
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
THStack* CreateStackFromTObjArray(TObjArray* histArray);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void AddCutArrow(
       double cut_location,
       double y1,
       double y2,
       double arrow_length,
        bool ArrowHeadGoLeft,
       int arrow_line_width=2,
       int arrow_line_style=1,
       int arrow_line_color=4);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


void Draw_heatMap(
  TH2D *h_migration_input, 
  const char* xaxislabel,
  const char* yaxislabel,
  const char* Title,
  const char* pdf,
  int rownormtype,
  TCanvas *can,
  bool includeFlows  );
  
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////  
  
  void Draw_heatMap_notext(
  TH2D *h_migration_input, 
  const char* xaxislabel,
  const char* yaxislabel,
  const char* Title,
  const char* pdf,
  int rownormtype,
  TCanvas *can,
  bool includeFlows);
  
  
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
  
  void Draw_heatMap_BinCheck(
  TH2D *h_migration_input, 
  const char* xaxislabel,
  const char* yaxislabel,
  const char* Title,
  const char* pdf,
  TCanvas *can,
  bool includeFlows);
  
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Draw_HIST_Resolution(
  TH1D *hist_1_input,
   char *histotitle,
   std::string xaxislabel,
   std::string yaxislabel,
   std::string pdf_name,
   bool NormArea, 
   bool Setgrid,
   bool BinWidthNorm,
   double Ymax,
   TCanvas *cE);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DrawStackMCandData(
    TH1* h_data_input, 
    TH1* h_MC_Total_input,
    THStack* Stack_input,
    char *Yaxis_title,
    char *Xaxis_title,
    bool DoBinWidthNorm, 
    char *Title,
    double YMax,
    TCanvas *Canvas,
    double SliceBinWidth=1.0,
    double NormalizeAfterBinwith =1.0);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
 void DrawStackMC( 
    TH1* h_MC_Total_input,
    THStack* Stack_input,
    char *Yaxis_title,
    char *Xaxis_title,
    bool DoBinWidthNorm, 
    char *Title,
    double YMax,
    double Errorshade =.35,
    int Color = kRed);   
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DrawStackMCandData_WithMCBand(
    TH1* h_data_input, 
    TH1* h_MC_Total_input,
    THStack* Stack_input,
    char *Yaxis_title,
    char *Xaxis_title,
    bool DoBinWidthNorm, 
    char *Title,
    double YMax,
    TCanvas *Canvas);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DrawStackMCandData(
    TH1* h_data_input, 
    TH1* h_MC_Total_input,
    THStack* Stack_input,
    bool DoBinWidthNorm, 
    double YMax);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
    void DrawStackMCandData_withBand(
    TH1* h_data_input, 
    TH1* h_MC_Total_input,
    TH1* BG_beamOFF_input,
    THStack* Stack_input,
    bool DoBinWidthNorm, 
    double YMax);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DrawStack(
    TH1* h_input, 
    THStack* Stack_input,
    char *Yaxis_title,
    char *Xaxis_title,
    bool DoBinWidthNorm, 
    char *Title,
    double YMax);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
    void DrawStack(
    TH1* h_input, 
    THStack* Stack_input,
    bool DoBinWidthNorm, 
    char *Title,
    double YMax);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void PlotDataStackedMC2D_ProjY(
	TH2* data,
	TH2* BG_beamOFF_input,
	TH2* Total_MC_input,
	TObjArray stack_input,
	std::vector<int> fillColors,
	char *pdf_label, char *histotitle, char *xaxislabel,
	char *yaxislabel, char *zaxislabel_units,
	double Ymax, bool setMaxY, bool doMultipliers,
	std::vector<double> YMultipliers,
	bool do_bin_width_norm,
	double text_size,
	double POT_DATA);
	
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void PlotDataStackedMC2D_ProjX(
	TH2* data,
	TH2* BG_beamOFF_input,
	TH2* Total_MC_input,
	TObjArray stack_input,
	std::vector<int> fillColors,
	char *pdf_label, char *histotitle, char *xaxislabel,
	char *yaxislabel, char *zaxislabel_units,
	double Ymax, bool setMaxY, bool doMultipliers,
	std::vector<double> YMultipliers,
	bool do_bin_width_norm,
	double text_size,
	double POT_DATA);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Draw_heatMap(
  TH2Poly *h_migration_input, 
  const char* xaxislabel,
  const char* yaxislabel,
  const char* Title,
  const char* pdf,
  int rownormtype,
  TCanvas *can,
  bool includeFlows);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
/*
void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data,
  TH1D* Total_MC,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  std::string pdfTitle,
  std::string TotalTitle,
  TCanvas *c1,double ymax
);
*/
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DrawOverlayCanvas(UBTH2Poly* originalPlot, 
char *drawoption,  double X_windowPos, double Y_windowPos,
double X_windowSize, double Y_windowSize);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Draw_heatMap_4DMigration(
  TH2D *h_migration_input, 
  const char* xaxislabel,
  const char* yaxislabel,
  const char* Title,
  const char* pdf,
  int rownormtype,
  TCanvas *can,
  bool includeFlows,
  bool IsInclusive,
  std::map<double,std::vector<double>> Bin_edges, 
double X1_window, double Y1_window, double X2_window, double Y2_window);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
 void PlotMC2D(
 std::map<Binning2D, TH1D*> InputHistmap,
 std::map<Binning2D, std::string > BinStringMap,
	char *pdf_label, char *histotitle, char *xaxislabel,
	char *legendTitle, char *zaxislabel_units,
	double Ymax, bool setMaxY, double Ymin, bool doMultipliers,
	std::vector<double> YMultipliers,
	bool do_bin_width_norm,
	double text_size,
	double POT_DATA);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
 void PlotMC2D_Stack(
 std::map<Binning2D, TH1D*> InputHistmap_DATA,
 std::map<Binning2D, TH1D*> InputHistmap_DATA_BeamOff,
 std::map<Binning2D, TH1D*> InputHistmap_MC,
 std::map<std::pair<Binning2D, EventCategory>, TH1D*> StackMap_input,
 std::map<Binning2D , std::string > BinStringMap,
	char *pdf_label, char *histotitle, char *xaxislabel,
	char *legendTitle, char *zaxislabel_units,
	double Ymax, bool setMaxY, double Ymin,  bool doMultipliers,
	std::vector<double> YMultipliers,
	bool dontDo_bin_width_norm,
	double text_size,
	float POT_DATA, 
float POT_scaler_MC,
float BG_Trigger_scaler);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void scaleTHStack(THStack* stack, double scaleFactor);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void BinWidthNormTHStack(THStack* stack, double scaleFactor);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void DrawFakeData();
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
 void PlotMC2D_purity_Eff(
 std::map<Binning2D, TH1D*> InputHistmap1,
 std::map<Binning2D, TH1D*> InputHistmap2,
 std::map<Binning2D , std::string > BinStringMap,
	char *pdf_label, char *histotitle, char *xaxislabel,
	char *zaxislabel_units,
	double Ymax, bool setMaxY, double Ymin,  bool doMultipliers,
	std::vector<double> YMultipliers,
	bool do_bin_width_norm,
	double text_size);
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
/*
void DrawStack(
  TH2D* category_hist_input,
  TH1D* Data_input,
  TH1D* Total_MC_input,
  TH1D* extBNB_input,
  SliceHistogram* slice_bnb,
  SliceHistogram* slice_ext,
  SliceHistogram* slice_mc_plus_ext,
  const Slice& slice,
  bool makeNormWidth,
  double SliceBinWidth,
  double ymax,
  double WindowZoomscale,
  bool Plot_EXT,
  bool Scaleall, 
  double Scaleall_input);	
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
	void DrawPanelplot(std::vector<double> WindowZoomScale,
                   std::map<Binning2D , std::string > BinStringMap,
                   std::string x_axis_title,
                   std::string y_axis_title,
                   std::string Pdf_Title,
                    double min_XAxis_GridCanvas,
                    double max_XAxis_GridCanvas,
                    double min_YAxis_GridCanvas,
                    double max_YAxis_GridCanvas,
                    std::vector<size_t> SliceBins,
                    SliceBinning &SliceBins_input,
                    std::map<Binning2D , double>  binwidthMap ,
                    TH1D* reco_bnb_hist,
                    TH1D* reco_ext_hist,
                    TH1D* reco_mc_plus_ext_hist,
                    TH2D* category_hist,
                    CovMatrixMap &matrix_map,
                    bool DoScaledown,
                    double Scaledown,
                    bool Plot_EXT);*/