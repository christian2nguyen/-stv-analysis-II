
#include "GeneratorOverlay_executable.h"

void GeneratorOverlay() {

	//------------------------------//
std::vector<double> SliceNorm{.23,.06,.08,.1,.22,.15,.43,.3,.42};
	TH1D::SetDefaultSumw2();
	TH2D::SetDefaultSumw2();
	gStyle->SetOptStat(0);

	int FontStyle = 132;
	double TextSize = 0.06;
	double TextSize_legend = 0.025;
	std::string Pdf_name = "GeneratorOverLay";
	std::string OutPutRootName = "OutputFiles/Models_slices.root";
	TString OutFilePath = "/exp/uboone/app/users/cnguyen/stv-analysis-new/unfold/OutputFiles/";

	//------------------------------//

	// Event generators

	std::vector<TString> Names; std::vector<TString> Labels; std::vector<int> Colors;
	std::vector<std::string> Labels_string;
	Names.push_back(OutFilePath+"CC0Pi_Flat_2D_GENIE_v3_0_6.root"); 
	Labels.push_back("GENIE v3_0_6_G18_10a_02a");
	Labels_string.push_back("GENIE_v3_0_6");
	Colors.push_back(kBlue+2);	

	Names.push_back(OutFilePath+"CC0Pi_Flat_2D_GENIE_v2_12_10_MEC.root"); 
	Labels.push_back("GENIE v2_12_10_MEC");
	Labels_string.push_back("GENIEv2_12_10_MEC");
	Colors.push_back(kGreen+1);	

	Names.push_back(OutFilePath+"CC0Pi_Flat_2D_GENIE_v2_12_10.root"); 
	Labels.push_back("GENIE v2_12_10");
	Labels_string.push_back("GENIEv2_12_10");
	Colors.push_back(kGreen);	


	Names.push_back(OutFilePath+"CC0Pi_Flat_2D_NuWro_19_02_1.root"); 
	Labels.push_back("NuWro 19.02.1");
	Labels_string.push_back("NuWro_19_02_1");
	Colors.push_back(kRed);

	Names.push_back(OutFilePath+"CC0Pi_Flat_2D_NEUT_5_4_0_1.root"); 
	Labels.push_back("NEUT 5.4.0.1");
	Labels_string.push_back("NEUT_5_4_0_1");
	Colors.push_back(kOrange+7);

	//Names.push_back(OutFilePath+"CC0Pi_Flat_2D_GiBUU.root"); 
	//Labels.push_back("GiBUU");
	//Colors.push_back(kBlue+6);
	
	char pdf_title[1024];
	char HistName[1024];
    TCanvas *c1 = new TCanvas("c1");
    sprintf(pdf_title, "%s.pdf(", Pdf_name.c_str());
    c1 -> Print(pdf_title);

	const int NSamples = Names.size();
	std::vector<TFile*> Files; Files.resize(NSamples);

	//------------------------------//

	// Plots to overlay

	std::vector<TString> PlotNames;

	PlotNames.push_back("TrueMuonPmuCosTheta_binN_scheme1");
	PlotNames.push_back("TrueMuonPmuCosTheta_binN_scheme2");
	
	PlotNames.push_back("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm");
	PlotNames.push_back("TrueMuonPmuCosTheta_binN_scheme2_noBinWidthNorm");
	
	
	const int NPlots = PlotNames.size();

	//------------------------------//	

	// Loop over the samples to open the files and the TTree

	for (int iSample = 0; iSample < NSamples; iSample++) {

		Files[iSample] = new TFile(Names[iSample],"readonly");

	} // End of the loop over the samples

	//------------------------------//

	// Loop over the plots to be compared

	for (int iPlot = 0; iPlot < NPlots; iPlot++) {

		TString CanvasName = "Canvas_" + PlotNames[iPlot];
		TCanvas* PlotCanvas = new TCanvas(CanvasName,CanvasName,205,34,1024,768);
		PlotCanvas->cd();
		PlotCanvas->SetTopMargin(0.12);
		PlotCanvas->SetLeftMargin(0.15);
		PlotCanvas->SetBottomMargin(0.15);		
		PlotCanvas->Draw();	

		TLegend* leg = new TLegend(0.2,0.7,0.7,0.83);
		leg->SetBorderSize(0);
		leg->SetNColumns(2);
		leg->SetTextSize(TextSize_legend);	
		leg->SetTextFont(FontStyle);						




		// Loop over the samples to open the files and to get the corresponding plot

		std::vector<TH1D*> Histos; Histos.resize(NSamples);

		for (int iSample = 0; iSample < NSamples; iSample++) {	

		   Histos[iSample] = (TH1D*)(Files[iSample]->Get(PlotNames[iPlot]));

			Histos[iSample]->SetLineWidth(4);
			Histos[iSample]->SetLineColor( Colors.at(iSample) );	

			Histos[iSample]->GetXaxis()->SetTitleFont(FontStyle);
			Histos[iSample]->GetXaxis()->SetLabelFont(FontStyle);
			Histos[iSample]->GetXaxis()->SetNdivisions(8);
			Histos[iSample]->GetXaxis()->SetLabelSize(TextSize);
			Histos[iSample]->GetXaxis()->SetTitleSize(TextSize);	
			Histos[iSample]->GetXaxis()->SetTitleOffset(1.1);					
			Histos[iSample]->GetXaxis()->CenterTitle();						

			Histos[iSample]->GetYaxis()->SetTitleFont(FontStyle);
			Histos[iSample]->GetYaxis()->SetLabelFont(FontStyle);
			Histos[iSample]->GetYaxis()->SetNdivisions(6);
			Histos[iSample]->GetYaxis()->SetLabelSize(TextSize);
			Histos[iSample]->GetYaxis()->SetTitle("Cross Section [10^{-38} cm^{2}/Ar]");
			Histos[iSample]->GetYaxis()->SetTitleSize(TextSize);
			Histos[iSample]->GetYaxis()->SetTitleOffset(1.3);
			Histos[iSample]->GetYaxis()->SetTickSize(0);
			Histos[iSample]->GetYaxis()->CenterTitle();	
			Histos[iSample]->SetTitle(PlotNames[iPlot]);


			double imax = TMath::Max(Histos[iSample]->GetMaximum(),Histos[0]->GetMaximum());			
			Histos[iSample]->GetYaxis()->SetRangeUser(0.,1.1*imax);
			Histos[0]->GetYaxis()->SetRangeUser(0.,1.1*imax);			

			PlotCanvas->cd();
			Histos[iSample]->Draw("hist same");
			Histos[0]->Draw("hist same");	

			leg->AddEntry(Histos[iSample],Labels[iSample],"l");
			
			//----------------------------------------//					

		} // End of the loop over the samples grabing the plots	

		PlotCanvas->cd();
		leg->Draw();
    sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
    PlotCanvas -> Print(pdf_title);
	} // End of the loop over the plots

	//------------------------------//


////////////////////////////////////////////////////////////////
// testing Binning
////////////////////////////////////
c1->cd(); 
  auto* sb_ptr = new SliceBinning( "../mybins_mcc9_2D_muon_nosidebands.txt"); //tutorial_reco_slice_config.txt
  auto& sb = *sb_ptr;
  char title[1024];
  
  // I want to save all this to a root file, 
  auto outFile = TFile::Open(OutPutRootName.c_str(), "RECREATE");
   outFile->cd();   

  
  
  //TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm
  TH1D* BinNsheme0 = (TH1D*)Files[0]->Get("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm");
  TH1D* BinNsheme1 = (TH1D*)Files[1]->Get("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm");
  TH1D* BinNsheme2 = (TH1D*)Files[2]->Get("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm");
  TH1D* BinNsheme3 = (TH1D*)Files[3]->Get("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm");
  TH1D* BinNsheme4 = (TH1D*)Files[4]->Get("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm");
  std::cout << "Inside Slice Loop "<< std::endl;
  for ( size_t sl_idx = 0u; sl_idx < sb.slices_.size(); ++sl_idx ) {
    
    std::cout << "Slice : "<<sl_idx<< std::endl;

    const auto& slice = sb.slices_.at( sl_idx );

    TH1D* HistSlice0 = (TH1D*)slice.hist_->Clone(uniq());
    TH1D* HistSlice1 = (TH1D*)slice.hist_->Clone(uniq());  
    TH1D* HistSlice2 = (TH1D*)slice.hist_->Clone(uniq());  
    TH1D* HistSlice3 = (TH1D*)slice.hist_->Clone(uniq());
    TH1D* HistSlice4 = (TH1D*)slice.hist_->Clone(uniq());
  //std::cout<<"slice.hist_.size() = " <<  slice.hist_->size()<< std::endl;
  
  sprintf(title,"Slice %i",sl_idx);
   slice.hist_->SetTitle(title);
   
   


    std::cout<<" Prinining set "<< std::endl; 
    
    for (auto map1:slice.bin_map_)
    {
    
   // std::cout<<"map key:"<< map1.first<< std::endl;     
     for(auto set:map1.second ){
        //HistSlice->SetBinContent(map1.first, BinNsheme1->GetBinContent(set+1));
        HistSlice0->SetBinContent(map1.first, BinNsheme0->GetBinContent(set));
        HistSlice1->SetBinContent(map1.first, BinNsheme1->GetBinContent(set));
        HistSlice2->SetBinContent(map1.first, BinNsheme2->GetBinContent(set));
        HistSlice3->SetBinContent(map1.first, BinNsheme3->GetBinContent(set));
        HistSlice4->SetBinContent(map1.first, BinNsheme4->GetBinContent(set));
    double content1 = BinNsheme1->GetBinContent(set);
     double content2 = BinNsheme1->GetBinContent(set+1);
     
     std::cout<<"map key1: "<< map1.first<< " set:"<< set << " Rate(set): "<< content1<< " Rate(set+1) " <<  content2 << std::endl;
     
   // std::cout<< set <<" ,";
   }
    //std::cout<<std::endl;
    
  }

 std::cout<<" "<< std::endl;   
    //HistSlice->Scale(1.0/SliceNorm.at(sl_idx));
    //HistSlice->Scale(1.0,"width"); 
    
  Filland2DBinWidthNormalize(*HistSlice0, BinNsheme0, slice.bin_map_,SliceNorm.at(sl_idx) );
  Filland2DBinWidthNormalize(*HistSlice1, BinNsheme1, slice.bin_map_,SliceNorm.at(sl_idx) );  
  Filland2DBinWidthNormalize(*HistSlice2, BinNsheme2, slice.bin_map_,SliceNorm.at(sl_idx) );
  Filland2DBinWidthNormalize(*HistSlice3, BinNsheme3, slice.bin_map_,SliceNorm.at(sl_idx) );
  Filland2DBinWidthNormalize(*HistSlice4, BinNsheme4, slice.bin_map_,SliceNorm.at(sl_idx) );
    
    //HistSlice2->Scale(1.0/SliceNorm.at(sl_idx));
    //HistSlice2->Scale(1.0,"width"); 
    // /SliceNorm.at(sl_idx)
   // HistSlice->Scale(1/SliceNorm.at(sl_idx));

   HistSlice0->SetLineColor( Colors.at(0) );	
   HistSlice1->SetLineColor( Colors.at(1) );
   HistSlice2->SetLineColor( Colors.at(2) );
   HistSlice3->SetLineColor( Colors.at(3) );
   HistSlice4->SetLineColor( Colors.at(4) );
  HistSlice0->SetLineWidth(4);
  HistSlice1->SetLineWidth(4);
  HistSlice2->SetLineWidth(4);
  HistSlice3->SetLineWidth(4);
   HistSlice4->SetLineWidth(4);
   
   
   HistSlice0->SetMaximum(HistSlice1->GetMaximum()*1.75);
   HistSlice0->Draw("Hist");
   HistSlice1->Draw("hist same");
   HistSlice2->Draw("hist same");
   HistSlice3->Draw("hist same");
	HistSlice4->Draw("hist same");
	TLegend* leg = new TLegend(0.2,0.7,0.7,0.83);
		leg->SetBorderSize(0);
		leg->SetNColumns(2);
		leg->SetTextSize(TextSize_legend);	
		leg->SetTextFont(FontStyle);						

	leg->AddEntry(HistSlice0,Labels[0],"l");
	leg->AddEntry(HistSlice1,Labels[1],"l");
	leg->AddEntry(HistSlice2,Labels[2],"l");
	leg->AddEntry(HistSlice3,Labels[3],"l");
	leg->AddEntry(HistSlice4,Labels[4],"l");
	
	leg->Draw("same");
   sprintf(pdf_title, "%s.pdf", Pdf_name.c_str());
     c1 -> Print(pdf_title);

std::string slide_id = std::to_string(sl_idx);

    sprintf(HistName, "%s_Slice_%s",Labels_string[0].c_str(),slide_id.c_str());
    HistSlice0->Write(HistName);
    sprintf(HistName, "%s_Slice_%s",Labels_string[1].c_str(),slide_id.c_str());
    HistSlice1->Write(HistName);
    sprintf(HistName, "%s_Slice_%s",Labels_string[2].c_str(),slide_id.c_str());
    HistSlice2->Write(HistName);
    sprintf(HistName, "%s_Slice_%s",Labels_string[3].c_str(),slide_id.c_str());
    HistSlice3->Write(HistName);
    sprintf(HistName, "%s_Slice_%s",Labels_string[4].c_str(),slide_id.c_str());
    HistSlice4->Write(HistName);


  }// end of slices

     sprintf(pdf_title, "%s.pdf)", Pdf_name.c_str());
    c1 -> Print(pdf_title);
    outFile->Close();
} // End of the program


////////////////////////
// Main function
//////////////////////
int main() {
  //test_unfolding();
  GeneratorOverlay();
  return 0;
}



////////////////////////////////////////////////////////////////
// Get Nuisance and convert to TMatrix
////////////////////////////////////
/*
 TMatrixD* get_true_events_nuisance(
  std::string nuisance_file, std::string SAMPLE_NAME, double conv_factor )
{
  // We'll create one prediction per generator model in the truth_file_map
  std::map< std::string, TMatrixD* > truth_counts_map;
  for ( const auto& pair : truth_file_map ) {
    // First retrieve the raw NUISANCE histogram. It is expressed as a
    // total cross section with true bin number along the x-axis
    std::string generator_label = pair.first;
    std::cout<<"Making Nuisance Hists Label : "<< generator_label<< std::endl;
    const auto& file_info = pair.second;
    std::string nuisance_file = file_info.file_name_;
    TFile temp_in_file( nuisance_file.c_str(), "read" );
    TH1D* temp_hist = nullptr;
    temp_in_file.GetObject( SAMPLE_NAME.c_str(), temp_hist );

    // Set the associated directory to a null pointer. That way this histogram
    // won't be auto-deleted when the corresponding TFile object goes out of
    // scope
    temp_hist->SetDirectory( nullptr );

    // Set the style of the histogram to match the configuration in the
    // TruthFileInfo object
    temp_hist->SetLineColor( file_info.color_ );
    temp_hist->SetLineStyle( file_info.style_ );

    // Disable displaying the stats box
    temp_hist->SetStats( false );

    // Convert the content (and error) of each bin to an expected true event
    // count. Do this using the input conversion factor (integrated numu
    // flux * number of Ar targets in the fiducial volume) and the 2D bin
    // width.
    size_t num_bins = temp_hist->GetNbinsX();
    for ( size_t b = 0u; b < num_bins; ++b ) {
      double xsec = temp_hist->GetBinContent( b + 1 );
      double err = temp_hist->GetBinError( b + 1 );
      temp_hist->SetBinContent( b + 1, xsec * conv_factor );
      temp_hist->SetBinError( b + 1, err * conv_factor );
    }

    // Now change the TH1D into a TMatrixD column vector
    TMatrixD* temp_mat = new TMatrixD( num_bins, 1 );
    for ( size_t b = 0u; b < num_bins; ++b ) {
      temp_mat->operator()( b, 0 ) = temp_hist->GetBinContent( b + 1 );
    }

    // The conversion is done, so add the finished true event counts histogram
    // to the map
    truth_counts_map[ generator_label ] = temp_mat;
  }





  return truth_counts_map;
}
*/

TH1D *getHist_true_events_nuisance(std::string nuisance_file, 
std::string SAMPLE_NAME, double conv_factor){
 
 TFile temp_in_file( nuisance_file.c_str(), "read" );
 TH1D* temp_hist = nullptr;
 temp_in_file.GetObject( SAMPLE_NAME.c_str(), temp_hist );
 
 // Set the associated directory to a null pointer. That way this histogram
// won't be auto-deleted when the corresponding TFile object goes out of
// scope
 temp_hist->SetDirectory( nullptr );
    // Disable displaying the stats box
    temp_hist->SetStats( false );

    // Convert the content (and error) of each bin to an expected true event
    // count. Do this using the input conversion factor (integrated numu
    // flux * number of Ar targets in the fiducial volume) and the 2D bin
    // width.
    // No BinWidth applied yet
    size_t num_bins = temp_hist->GetNbinsX();
    for ( size_t b = 0u; b < num_bins; ++b ) {
      double xsec = temp_hist->GetBinContent( b + 1 );
      double err = temp_hist->GetBinError( b + 1 );
      temp_hist->SetBinContent( b + 1, xsec * conv_factor );
      temp_hist->SetBinError( b + 1, err * conv_factor );
    }


 return temp_hist;
}

/*


TH1D * GetCrossSectionSlice(TH1D *AllBins, Slice& slice){





}
*/


void Filland2DBinWidthNormalize(TH1D &inputSlice, TH1D* Hist_byBinN, std::map< int, std::set< size_t > > bin_map, double BinWidth ){

std::cout<<"inside::Filland2DBinWidthNormalize"<< std::endl;
  for (auto map1:bin_map)
    {
      for(auto set:map1.second ){
        inputSlice.SetBinContent(map1.first, Hist_byBinN->GetBinContent(set+1));
     std::cout<<"map: first key : "<< map1.first<< " Second Key:  "<< set << " Rate (set + 1) : "<< Hist_byBinN->GetBinContent(set+1)<<  " Rate(set) =  "<<  Hist_byBinN->GetBinContent(set) << std::endl;
   // std::cout<< set <<" ,";
   }
    //std::cout<<std::endl;
    
  }

    inputSlice.Scale(1.0 / BinWidth);
    inputSlice.Scale(1.0,"width"); 
    
}