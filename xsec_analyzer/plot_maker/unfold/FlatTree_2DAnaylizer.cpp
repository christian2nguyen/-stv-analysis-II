/**
 *This Code is to Anaylizer flat trees
 *
 * **/

#include "FlatTreeAnalyzer.h"


double POT_inter = 6.66577e+11;


void FlatTreeAnalyzer::Loop(std::string inputTreeType) {

	//----------------------------------------//	

	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;

	double Units = 1E38; // so that the extracted cross-section is in 10^{-38} cm^{2}
	double A = 40.; // so that we can have xsecs per nucleus

	int NInte = 6; // Interaction processes: All, QE, MEC, RES, DIS, COH
	std::vector<TString> InteractionLabels = {"","_interaction_QE","_interaction_MEC","_interaction_RES","_interaction_DIS","_interaction_COH"};

	//----------------------------------------//	

        // Output file

	TString FileNameAndPath = fOutputFile + ".root";
	TFile* file = new TFile(FileNameAndPath,"recreate");

	std::cout << std::endl << "------------------------------------------------" << std::endl << std::endl;
	std::cout << "File " << FileNameAndPath << " to be created" << std::endl << std::endl;
	
	//----------------------------------------//

	// Plot declaration


   UBTH2Poly* h_costheta_Pmu_UBTH2Poly  = new UBTH2Poly("h_costheta_Pmu_UBTH2Poly", "h_costheta_Pmu_UBTH2Poly",  MUON_2D_BIN_EDGES, true); 
   UBTH2Poly* h_costheta_Pmu_UBTH2Poly_inclusive  = new UBTH2Poly("h_costheta_Pmu_UBTH2Poly_inclusive", "h_costheta_Pmu_UBTH2Poly_inclusive",  MUON_2D_BIN_EDGES_inclusive, false);

   UBTH2Poly* UBTH2Poly_scheme1_CrossSection[NInte];
   UBTH2Poly* UBTH2Poly_scheme2_CrossSection[NInte];

	//TH1D* TrueMuonCosThetaPlot[NInte];
    TH1D* TrueMuon_PmuCosThetaPlotBinningscheme1[NInte];
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme2[NInte];
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme1_noBinWidthNorm[NInte];
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme2_noBinWidthNorm[NInte];
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme1_NoNorm[NInte];
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme2_NoNorm[NInte];
	
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme1_CrossSection[NInte];
	TH1D* TrueMuon_PmuCosThetaPlotBinningscheme2_CrossSection[NInte];
	
	
	int NBin_Scheme1 = 45; 
    int NBin_Scheme2 = 39;   
	// Loop over the interaction processes
    // Better to fill by Bin Number 
    // Binning starts at 1 

	// Loop over the interaction processes




	for (int inte = 0; inte < NInte; inte++) {

	  //--------------------------------------------------//
	  //TrueMuonCosThetaPlot[inte] = new TH1D(InteractionLabels[inte]+"TrueMuonCosThetaPlot",";cos(#theta_{#mu})",10,-1.,1.);
      TrueMuon_PmuCosThetaPlotBinningscheme1[inte] = new TH1D("TrueMuonPmuCosTheta_binN_scheme1"+InteractionLabels[inte],";Bin N",NBin_Scheme1,1.,NBin_Scheme1+1);
      TrueMuon_PmuCosThetaPlotBinningscheme2[inte] = new TH1D("TrueMuonPmuCosTheta_binN_scheme2"+InteractionLabels[inte],";Bin N",NBin_Scheme2,1.,NBin_Scheme2+1);

	  TrueMuon_PmuCosThetaPlotBinningscheme1_noBinWidthNorm[inte] = new TH1D("TrueMuonPmuCosTheta_binN_scheme1_noBinWidthNorm"+InteractionLabels[inte],";Bin N",NBin_Scheme1,1.,NBin_Scheme1+1);
      TrueMuon_PmuCosThetaPlotBinningscheme2_noBinWidthNorm[inte] = new TH1D("TrueMuonPmuCosTheta_binN_scheme2_noBinWidthNorm"+InteractionLabels[inte],";Bin N",NBin_Scheme2,1.,NBin_Scheme2+1);

      TrueMuon_PmuCosThetaPlotBinningscheme1_NoNorm[inte] = new TH1D("TrueMuonPmuCosTheta_binN_scheme1_NoNorm"+InteractionLabels[inte],";Bin N",NBin_Scheme1,1.,NBin_Scheme1+1);
      TrueMuon_PmuCosThetaPlotBinningscheme2_NoNorm[inte] = new TH1D("TrueMuonPmuCosTheta_binN_scheme2_NoNorm"+InteractionLabels[inte],";Bin N",NBin_Scheme2,1.,NBin_Scheme2+1);
	  
	  
	    UBTH2Poly_scheme1_CrossSection[inte] = new UBTH2Poly("h_costheta_Pmu_UBTH2Poly" + InteractionLabels[inte] , "h_costheta_Pmu_UBTH2Poly",  MUON_2D_BIN_EDGES, true); 
        UBTH2Poly_scheme2_CrossSection[inte] = new UBTH2Poly("h_costheta_Pmu_UBTH2Poly_inclusive" + InteractionLabels[inte], "h_costheta_Pmu_UBTH2Poly_inclusive",  MUON_2D_BIN_EDGES_inclusive, false);

        TrueMuon_PmuCosThetaPlotBinningscheme1_CrossSection[inte]= new TH1D("TrueMuonPmuCosTheta_binN_scheme1_CrossSection"+InteractionLabels[inte],";Bin N",NBin_Scheme1,1.,NBin_Scheme1+1);
        TrueMuon_PmuCosThetaPlotBinningscheme2_CrossSection[inte]= new TH1D("TrueMuonPmuCosTheta_binN_scheme2_CrossSection"+InteractionLabels[inte],";Bin N",NBin_Scheme2,1.,NBin_Scheme2+1);
	  
	  
	  //--------------------------------------------------//

	} // End of the loop over the interaction processes							

	//----------------------------------------//

	// Counters

	int CounterEventsPassedSelection = 0;
	int CounterQEEventsPassedSelection = 0;
	int CounterMECEventsPassedSelection = 0;
	int CounterRESEventsPassedSelection = 0;
	int CounterDISEventsPassedSelection = 0;
	int CounterCOHEventsPassedSelection = 0;	

	//----------------------------------------//
	
	// Loop over the events

	for (Long64_t jentry=0; jentry<nentries;jentry++) {

	  //----------------------------------------//	

	
	  Long64_t ientry = LoadTree(jentry);
	  if (ientry < 0) break; nb = fChain->GetEntry(jentry); nbytes += nb;
	  if (jentry%100000 == 0) std::cout << jentry/100000 << " 100 k " << std::setprecision(3) << double(jentry)/nentries*100. << " %"<< std::endl;

	  //----------------------------------------//	
		
	  double weight = fScaleFactor*Units*A*Weight; //*Weight	
       //if(inputTreeType=="GiBUU_2023"){weight=weight/81.0;}
       //weight=weight/81.0;
       weight=weight/150.0;
     // double weight =fScaleFactor*Units*A  /  POT_inter;

     //double weight =fScaleFactor * ( A ) *  1e38 / POT_inter ;
	  double weight_noUnits = fScaleFactor;// *Weight
	  //if(inputTreeType=="GiBUU_2023"){weight_noUnits=weight_noUnits/81.0;}
	  weight_noUnits=weight_noUnits/81.0;
	  weight_noUnits=weight_noUnits/150.0;
	  //----------------------------------------//	
      //std::cout<<"fScaleFactor = "<< fScaleFactor << std::endl;
	  // Signal definition

	  if (PDGLep != 13) { continue; } // make sure that we have only a muon in the final state
	  if (cc != 1) { continue; } // make sure that we have only CC interactions		

	  int ProtonTagging = 0, ChargedPionTagging = 0, NeutralPionTagging = 0, MuonTagging = 0;
          int ElectronTagging = 0, PhotonTagging = 0;
	  //std::vector <int> ProtonID; ProtonID.clear();
	  std::vector <int> MuonID; MuonID.clear();		

	  // Example selection with CC0pi (units in GeV/c)
	  // Loop over final state particles
		double Pmu =0; 
	  for (int i = 0; i < nfsp; i++) {
		
	    double pf = TMath::Sqrt( px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i]);

	    if (pdg[i] == 13 && (pf >= 0.1 /*GeV*/ && pf <= 2.0 /*GeV*/)) {

	      MuonTagging ++;
	      MuonID.push_back(i);
          Pmu = pf;
	    }


	    if (fabs(pdg[i]) == 211 && pf > 0.07)  {

	      ChargedPionTagging ++;

	    }

	    if (pdg[i] == 111)  {

	      NeutralPionTagging ++;

	    }

	    if (fabs(pdg[i]) == 11)  {

	      ElectronTagging ++;

	    }

	    if (fabs(pdg[i]) == 22)  {

	      PhotonTagging ++;

	    }


	  } // End of the loop over the final state particles

	  // If the signal definition is not satisfied, continue

	  if (
		ChargedPionTagging != 0 || 
		NeutralPionTagging != 0 || 
		MuonTagging !=1 ||
		ElectronTagging != 0 ||
		PhotonTagging != 0 )
		{ continue; }

	  //----------------------------------------//	

	  // https://arxiv.org/pdf/2106.15809.pdf

	  CounterEventsPassedSelection++;
	
	  // Classify the events based on the interaction type

	  int genie_mode = -1.;
	  if (TMath::Abs(Mode) == 1) { CounterQEEventsPassedSelection++; genie_mode = 1; } // QE
	  else if (TMath::Abs(Mode) == 2) { CounterMECEventsPassedSelection++; genie_mode = 2; } // MEC
	  else if (
		   TMath::Abs(Mode) == 10 ||
		   TMath::Abs(Mode) == 11 || TMath::Abs(Mode) == 12 || TMath::Abs(Mode) == 13 ||
		   TMath::Abs(Mode) == 17 || TMath::Abs(Mode) == 22 || TMath::Abs(Mode) == 23
		   ) { CounterRESEventsPassedSelection++; genie_mode = 3; } // RES
	  else if (TMath::Abs(Mode) == 21 || TMath::Abs(Mode) == 26) { CounterDISEventsPassedSelection++; genie_mode = 4; } // DIS
	  else if (TMath::Abs(Mode) == 16) { CounterCOHEventsPassedSelection++; genie_mode = 5;} // COH
	  else { continue; }  

	  // Feb 8 2022: Only case that is not covered is 15 = diffractive

	  //----------------------------------------//

	  // filling in the histo regardless of interaction mode

	  //TrueMuonCosThetaPlot[0]->Fill(CosLep,weight);


	  //----------------------------------------//

	  // filling in the histo based on the interaction mode

	  //TrueMuonCosThetaPlot[genie_mode]->Fill(CosLep,weight);

      int binNscheme1 = h_costheta_Pmu_UBTH2Poly->Fill(CosLep,Pmu,weight);
	  int binNscheme2 = h_costheta_Pmu_UBTH2Poly_inclusive->Fill(CosLep,Pmu,weight);

   if(binNscheme1>52 || binNscheme1 <=0 ){std::cout<<"Something is not good iwth scheme 1 "<< binNscheme1 << std::endl; }
   if(binNscheme2>60 || binNscheme2 <=0 ){std::cout<<"Something is not good with scheme 2 "<< binNscheme2 << std::endl; }



      UBTH2Poly_scheme1_CrossSection[0]->Fill(CosLep,Pmu,weight);
      UBTH2Poly_scheme2_CrossSection[0]->Fill(CosLep,Pmu,weight);

      //std::cout<<"binNscheme1 = "<< binNscheme1<< std::endl;


      TrueMuon_PmuCosThetaPlotBinningscheme1[0]->Fill(binNscheme1,weight);
      TrueMuon_PmuCosThetaPlotBinningscheme2[0]->Fill(binNscheme2,weight);
      
      TrueMuon_PmuCosThetaPlotBinningscheme1_noBinWidthNorm[0]->Fill(binNscheme1,weight);
      TrueMuon_PmuCosThetaPlotBinningscheme2_noBinWidthNorm[0]->Fill(binNscheme2,weight);
	  //----------------------------------------//

      TrueMuon_PmuCosThetaPlotBinningscheme1_NoNorm[0]->Fill(binNscheme1,weight_noUnits);
      TrueMuon_PmuCosThetaPlotBinningscheme2_NoNorm[0]->Fill(binNscheme2,weight_noUnits);


	  // filling in the histo based on the interaction mode


	  TrueMuon_PmuCosThetaPlotBinningscheme1[genie_mode]->Fill(binNscheme1,weight);
	  TrueMuon_PmuCosThetaPlotBinningscheme2[genie_mode]->Fill(binNscheme2,weight);

	  TrueMuon_PmuCosThetaPlotBinningscheme1_noBinWidthNorm[genie_mode]->Fill(binNscheme1,weight);
	  TrueMuon_PmuCosThetaPlotBinningscheme2_noBinWidthNorm[genie_mode]->Fill(binNscheme2,weight);
	  
	  TrueMuon_PmuCosThetaPlotBinningscheme1_NoNorm[genie_mode]->Fill(binNscheme1,weight_noUnits);
      TrueMuon_PmuCosThetaPlotBinningscheme2_NoNorm[genie_mode]->Fill(binNscheme2,weight_noUnits);
	  
	     
      UBTH2Poly_scheme1_CrossSection[genie_mode]->Fill(CosLep,Pmu,weight);
      UBTH2Poly_scheme2_CrossSection[genie_mode]->Fill(CosLep,Pmu,weight);
	  //----------------------------------------//
	
	} // End of the loop over the events

	//----------------------------------------//	







	//std::cout << "Percetage of events passing the selection cuts = " << 
	//double(CounterEventsPassedSelection)/ double(nentries)*100. << " %" << std::endl; std::cout << std::endl;
//
	//std::cout << "Success percetage in selecting QE events = " << 
	//double(CounterQEEventsPassedSelection)/ double(CounterEventsPassedSelection)*100. << " %" << std::endl; std::cout << std::endl;
//
	//std::cout << "Success percetage in selecting MEC events = " << 
	//double(CounterMECEventsPassedSelection)/ double(CounterEventsPassedSelection)*100. << " %" << std::endl; std::cout << std::endl;
//
	//std::cout << "Success percetage in selecting RES events = " << 
	//double(CounterRESEventsPassedSelection)/ double(CounterEventsPassedSelection)*100. << " %" << std::endl; std::cout << std::endl;
//
	//std::cout << "Success percetage in selecting DIS events = " << 
	//double(CounterDISEventsPassedSelection)/ double(CounterEventsPassedSelection)*100. << " %" << std::endl; std::cout << std::endl;
//
	//std::cout << "Success percetage in selecting COH events = " << 
	//double(CounterCOHEventsPassedSelection)/ double(CounterEventsPassedSelection)*100. << " %" << std::endl; std::cout << std::endl;	
//
	//----------------------------------------//	
	//----------------------------------------//	

	// Division by bin width to get the cross sections	
	// Loop over the interaction processes
std::cout<<"BinWidth Normalizing"<< std::endl;
	for (int inte = 0; inte < NInte; inte++) {

		//----------------------------------------//
	
		//Reweight(TrueMuonCosThetaPlot[inte]);
        Reweight_2DBinning(*TrueMuon_PmuCosThetaPlotBinningscheme1[inte], h_costheta_Pmu_UBTH2Poly);
        Reweight_2DBinning(*TrueMuon_PmuCosThetaPlotBinningscheme2[inte], h_costheta_Pmu_UBTH2Poly_inclusive);
        
        
        Reweight_2DBinning(*TrueMuon_PmuCosThetaPlotBinningscheme1_NoNorm[inte], h_costheta_Pmu_UBTH2Poly);
        Reweight_2DBinning(*TrueMuon_PmuCosThetaPlotBinningscheme2_NoNorm[inte], h_costheta_Pmu_UBTH2Poly_inclusive);
        
		//----------------------------------------//
       UBTH2Poly_scheme1_CrossSection[inte]->Scale(1, "width");
       UBTH2Poly_scheme2_CrossSection[inte]->Scale(1, "width");



	} // End of the loop over the interaction processes		

std::cout<<"Full 1D Cross Section"<<std::endl;




	//----------------------------------------//		
		for (int inte = 0; inte < NInte; inte++) {
		std::cout<<"There is this many Bins : "<< UBTH2Poly_scheme1_CrossSection[inte]->GetNumberOfBins() << std::endl;
		for(int binj = 1; binj <= UBTH2Poly_scheme1_CrossSection[inte]->GetNumberOfBins(); binj++){
		std::cout<<"Filling Bin: "<< binj<< std::endl;
		TrueMuon_PmuCosThetaPlotBinningscheme1_CrossSection[inte]->SetBinContent(binj,UBTH2Poly_scheme1_CrossSection[inte]->GetBinContent(binj)); 
		TrueMuon_PmuCosThetaPlotBinningscheme1_CrossSection[inte]->SetBinError(binj,UBTH2Poly_scheme1_CrossSection[inte]->GetBinError(binj)); 
		
		}
		
		
		std::cout<<"There is this many Bins : "<< UBTH2Poly_scheme2_CrossSection[inte]->GetNumberOfBins() << std::endl;
		for(int binj = 1; binj <= UBTH2Poly_scheme2_CrossSection[inte]->GetNumberOfBins(); binj++){
		std::cout<<"Filling Bin: "<< binj<< std::endl;
		TrueMuon_PmuCosThetaPlotBinningscheme2_CrossSection[inte]->SetBinContent(binj,UBTH2Poly_scheme2_CrossSection[inte]->GetBinContent(binj)); 
		TrueMuon_PmuCosThetaPlotBinningscheme2_CrossSection[inte]->SetBinError(binj,UBTH2Poly_scheme2_CrossSection[inte]->GetBinError(binj)); 
		}
		
		}
		
		

		
		std::cout<<"Writing Files "<< std::endl; 
		
		
		
	file->cd();
	file->Write();
	fFile->Close();

char Histtitle[1024];
   TCanvas *can = new TCanvas(uniq());
    gStyle->SetOptStat(0); 
    int precision = 2;  // Change this value based on your requirement
	gStyle->SetPaintTextFormat(Form(".%df", precision));
	float textSize = 0.01;  // Adjust this value based on your requirement
	gStyle->SetTextSize(textSize);
    char text_title_pdf1[2024];
  sprintf(Histtitle, "Model: %s binning Scheme 1",fPDF_Name.c_str());
h_costheta_Pmu_UBTH2Poly->SetTitle(Histtitle); 
h_costheta_Pmu_UBTH2Poly->GetXaxis()->SetTitle("Cos(#theta_{#mu})");
h_costheta_Pmu_UBTH2Poly->GetYaxis()->SetTitle("P_{#mu} [Gev/c]");
h_costheta_Pmu_UBTH2Poly->GetZaxis()->SetLabelSize (0.017);
h_costheta_Pmu_UBTH2Poly->Draw("colz text");
 sprintf(text_title_pdf1, "FlatTreePlots.pdf");
  can -> Print(text_title_pdf1);
h_costheta_Pmu_UBTH2Poly_inclusive->GetXaxis()->SetTitle("Cos(#theta_{#mu})");
h_costheta_Pmu_UBTH2Poly_inclusive->GetYaxis()->SetTitle("P_{#mu} [Gev/c]");
  sprintf(Histtitle, "Model: %s binning Scheme 1",fPDF_Name.c_str());
h_costheta_Pmu_UBTH2Poly_inclusive->SetTitle(Histtitle); 
h_costheta_Pmu_UBTH2Poly_inclusive->Draw("colz text");
  can -> Print(text_title_pdf1);
  
 h_costheta_Pmu_UBTH2Poly->Scale(1, "width");
 h_costheta_Pmu_UBTH2Poly->Draw("colz text");
 can -> Print(text_title_pdf1);
 h_costheta_Pmu_UBTH2Poly_inclusive->Scale(1, "width");
 h_costheta_Pmu_UBTH2Poly_inclusive->GetZaxis()->SetTitle("#frac{d#sigma}{dp_{#mu}} [ 10^{-38} #frac{cm^{2}}{GeV/c Ar} ]");
 h_costheta_Pmu_UBTH2Poly_inclusive->Draw("colz text");
 can -> Print(text_title_pdf1);
  can->Close();   
	//delete can; 
	std::cout << std::endl;
	std::cout << "File " << FileNameAndPath +" has been created created " << std::endl; 
	std::cout << std::endl;

	std::cout << std::endl << "------------------------------------------------" << std::endl << std::endl;
	std::cout << std::endl << "End of Loop " << std::endl << std::endl;
	//----------------------------------------//		
return; 
} // End of the program
//////////////////////////////////////////////////////////
//	
//////////////////////////////////////////////////////////
void Reweight(TH1D* h) {

  int NBins = h->GetXaxis()->GetNbins();

  for (int i = 0; i < NBins; i++) {

    double CurrentEntry = h->GetBinContent(i+1);
    double NewEntry = CurrentEntry / h->GetBinWidth(i+1);

    double CurrentError = h->GetBinError(i+1);
    double NewError = CurrentError / h->GetBinWidth(i+1);

    h->SetBinContent(i+1,NewEntry); 
    h->SetBinError(i+1,NewError); 
    //h->SetBinError(i+1,0.000001); 

  }

}
//////////////////////////////////////////////////////////
//	
//////////////////////////////////////////////////////////
void run_FlatTreeAnaylizer(std::string rootfileName_input, std::string outputName, std::string PDF_name){



std::cout<<"Running FlatTreeAnaylizer For: "<<PDF_name.c_str() << std::endl;

TString in(rootfileName_input);
TString out(outputName);



FlatTreeAnalyzer FlatTreeAnalyzer_object(in, out, PDF_name );

FlatTreeAnalyzer_object.Loop(rootfileName_input);


}


//----------------------------------------//		
//////////////////////////////////////////////////////////
//	
//////////////////////////////////////////////////////////
void Reweight_2DBinning(TH1D &h, UBTH2Poly* UBTH2Poly_input) {

  int NBins = h.GetXaxis()->GetNbins();

  for (int i = 0; i < NBins; i++) {

    double CurrentEntry = h.GetBinContent(i+1);
    double NewEntry = CurrentEntry / UBTH2Poly_input->GetBinWidth(i+1);

    double CurrentError = h.GetBinError(i+1);
    double NewError = CurrentError / UBTH2Poly_input->GetBinWidth(i+1);

    h.SetBinContent(i+1,NewEntry); 
    h.SetBinError(i+1,NewError); 
    //h->SetBinError(i+1,0.000001); 

  }

}
//////////////////////////////////////////////////////////
//	
//////////////////////////////////////////////////////////

int main() {
std::vector<std::string> WhichSample;
std::vector<std::string> WhichName;

   UBTH2Poly instance;
   gInterpreter->Declare("#include \"/exp/uboone/app/users/cnguyen/stv-analysis-new/includes/UBTH2Poly.h\""); 

std::vector<std::string> input{"NuWro.flat","GENIE_v3_0_6_G18_10a_02a.flat","GiBUU.flat","NEUT.flat"};
//WhichSample.push_back("/pnfs/uboone/persistent/users/mastbaum/tuning2022/mc/bnb_ub/flat/bnb.ub.num.genie_v3_00_06.flat"); WhichName.push_back("GENIE_v3_0_6");
//WhichSample.push_back("/pnfs/uboone/persistent/users/mastbaum/tuning2022/mc/bnb_ub/flat/bnb.ub.num.genie_v2_12_10.mec.flat"); WhichName.push_back("GENIE_v2_12_10_MEC");
//WhichSample.push_back("/pnfs/uboone/persistent/users/mastbaum/tuning2022/mc/bnb_ub/flat/bnb.ub.num.genie_v2_12_10.flat"); WhichName.push_back("GENIE_v2_12_10");
//WhichSample.push_back("/pnfs/uboone/persistent/users/mastbaum/tuning2022/mc/bnb_ub/flat/bnb.ub.num.nuwro_19_02_1.flat"); WhichName.push_back("NuWro_19_02_1");
//WhichSample.push_back("/pnfs/uboone/persistent/users/mastbaum/tuning2022/mc/bnb_ub/flat/bnb.ub.num.neut_5_4_0_1.flat"); WhichName.push_back("NEUT_5_4_0_1");
//WhichSample.push_back("/pnfs/uboone/persistent/users/bbogart/GiBUU_samples/GiBUU_noOset/GiBUU_2023_noOset.flat"); WhichName.push_back("GiBUU_2023");
WhichSample.push_back("/pnfs/uboone/persistent/users/bbogart/GiBUU_samples/GiBUU_flagInMedium/GiBUU_flagInMedium.flat"); WhichName.push_back("GiBUU_2023");



/*
for(auto rootname:input ){


std::string inputTfile = "mySamples/" + rootname;
run_FlatTreeAnaylizer(inputTfile, name);
}
*/

 TCanvas *can1 = new TCanvas(uniq());
 
  can1 -> Print("FlatTreePlots.pdf(");

for(int i =0 ; i < WhichSample.size(); ++i ){


std::string name = "OutputFiles/CC0Pi_Flat_2D_" + WhichName.at(i);

run_FlatTreeAnaylizer(WhichSample.at(i), name, WhichName.at(i) );
std::cout<<"Finished FlatTreeAnaylizer for ::  "<< WhichName.at(i).c_str()<< std::endl;


}

can1 -> Print("FlatTreePlots.pdf)");
can1->Close(); 
delete can1;
  return 0;
}
