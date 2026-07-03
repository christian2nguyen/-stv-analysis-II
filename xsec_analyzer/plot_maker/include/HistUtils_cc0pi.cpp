

 #include "HistUtils_cc0pi.hh"
//#include "UBTH2Poly.h"  


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
EventCategory category_type_input){
TKI_parameters output; 

 output.Pproton = Pproton_input ;
 output.Pproton_mc = Pproton_mc_input;
 output.PCostheta =  PCostheta_input ;
 output.PCostheta_mc =  PCostheta_mc_input ;
 output.pT =pT_input;
 output.pT_mc =pT_mc_input;
 output.delta_alphaT =  delta_alphaT_input ;
 output.delta_alphaT_mc =  delta_alphaT_mc_input ;
 output.delta_pTx =  delta_pTx_input ;
 output.delta_pTx_mc = delta_pTx_mc_input;
 output.delta_pTy =   delta_pTy_input;
 output.delta_pTy_mc =  delta_pTy_mc_input ;
 output.delta_phiT =  delta_phiT_input ;
 output.delta_phiT_mc = delta_phiT_mc_input  ;
 output.pn =  pn_input ;
 output.pn_mc  = pn_mc_input ;
 output.category_type = category_type_input; 

return output;


}


TH1D* GetTH1DHist(TFile& fin, const char* name) {

  TH1D* h=(TH1D*)fin.Get(name);
  if (h==0) {
      const char* fileName = fin.GetName();
  std::cout << "Could not get 1D TH1D hist :  " << name << "  From TFILE named : "<<fileName << "\n";
  
  }
  return h;
}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////////////////////
TH1D* GetTH1DHist(TFile& fin, std::string name ){
  const char* name_char = name.c_str();
  TH1D* hist = GetTH1DHist(fin, name_char);
  return hist;
}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////////////////////
TH2D* GetTH2DHist(TFile& fin, const char* name) {
  TH2D* h=(TH2D*)fin.Get(name);
  if (h==0) std::cout << "Could not get 2D TH2D hist " << name << "\n";
  return h;
}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
TH2D* GetTH2DHist(TFile& fin, std::string name) {
  const char* name_char = name.c_str();
  TH2D* hist = GetTH2DHist(fin, name_char);
  return hist;
}//////////////////////////////// End of Function 

TH2Poly * Make2DHist(std::map<double,std::vector<double>> Bin_edges, 
std::map<int , BinMap> &TH1Poly_binMap, char *name){

	TH2Poly *h2p = new TH2Poly();
	h2p->SetName(name);

 for (auto it = Bin_edges.begin(); it != Bin_edges.end(); ++it) {
        double currentKey = it->first;
        auto  Xvector = it->second;
        int vector_size = Xvector.size();   
        // Check if there is a next element
        auto nextIt = std::next(it);
        if (nextIt != Bin_edges.end()) {
            double nextKey = nextIt->first;
            //if(nextKey ==2)continue;
            //std::string nextValue = nextIt->second;
            //std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
            // Access current key, value, next key, and next value
            //std::cout << "Current: Key=" << currentKey  << std::endl;
            //std::cout << "Next: Key=" << nextKey  << std::endl;
            //std::cout<<"Bins: ";
            for (int k = 0; k <vector_size-1; k++ )
            {
            //std::cout<<Xvector.at(k)<< ", "<< Xvector.at(k+1)<< std::endl;
            double avg_X = .5 * (Xvector.at(k) +Xvector.at(k+1) );
            double avg_Y = .5 * (currentKey + nextKey); 
            
            BinMap BinMapSingle{currentKey,nextKey,Xvector.at(k),Xvector.at(k+1),avg_X,avg_Y};
            int binN = h2p->AddBin(Xvector.at(k),currentKey, Xvector.at(k+1),nextKey);
            //std::cout<<"BinN = "<< binN<< std::endl;
            TH1Poly_binMap.insert(std::pair<int,BinMap >(binN,BinMapSingle)); 
            }
            //std::cout<< " "<< std::endl;
            
            
        } else {
            // If there is no next element, handle accordingly
            //std::cout << "Finished last bin  element with Key=" << currentKey << std::endl;
        }
    
   // std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
    
    
}


return h2p; 

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
UBTH2Poly *Make2DHist_UB(std::map<double,std::vector<double>> Bin_edges, 
std::map<int , BinMap> &TH1Poly_binMap, char *name){

	UBTH2Poly *h2p = new UBTH2Poly();

	h2p->SetName(name);

	for (auto it = Bin_edges.begin(); it != Bin_edges.end(); ++it) {
        double currentKey = it->first;
        auto  Xvector = it->second;
        int vector_size = Xvector.size();   
        // Check if there is a next element
        auto nextIt = std::next(it);
        if (nextIt != Bin_edges.end()) {
            double nextKey = nextIt->first;
            //if(nextKey ==2)continue;
            //std::string nextValue = nextIt->second;
             //std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
            // Access current key, value, next key, and next value
            //std::cout << "Current: Key=" << currentKey  << std::endl;
            //std::cout << "Next: Key=" << nextKey  << std::endl;
            //std::cout<<"Bins: ";
            for (int k = 0; k <vector_size-1; k++ )
            {
            //std::cout<<Xvector.at(k)<< ", "<< Xvector.at(k+1)<< std::endl;
            double avg_X = .5 * (Xvector.at(k) +Xvector.at(k+1) );
            double avg_Y = .5 * (currentKey + nextKey); 
            
            BinMap BinMapSingle{currentKey,nextKey,Xvector.at(k),Xvector.at(k+1),avg_X,avg_Y};
            Double_t x1 = Xvector.at(k);
            Double_t y1 = currentKey;
            Double_t x2 = Xvector.at(k+1);
            Double_t y2 = nextKey;
            
            
            int binN = h2p->AddBin(x1,y1, x2,y2);
            //std::cout<<"BinN = "<< binN<< std::endl;
            TH1Poly_binMap.insert(std::pair<int,BinMap >(binN,BinMapSingle)); 
            }
            //std::cout<< " "<< std::endl;
            
            
        } else {
            // If there is no next element, handle accordingly
            //std::cout << "Finished last bin  element with Key=" << currentKey << std::endl;
        }
    
   // std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
    
    
}
  return h2p; 

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
TH2Poly * Make2DHist_inclusive(std::map<double,std::vector<double>> Bin_edges,
std::map<int , BinMap> &TH1Poly_binMap, char *name){

	TH2Poly *h2p = new TH2Poly();

	h2p->SetName(name);

	for (auto it = Bin_edges.begin(); it != Bin_edges.end(); ++it) {
        double currentKey = it->first;
        auto  Xvector = it->second;
        int vector_size = Xvector.size();   
        // Check if there is a next element
        auto nextIt = std::next(it);
        if (nextIt != Bin_edges.end()) {
            double nextKey = nextIt->first;
            //if(nextKey ==2)continue;
            //std::string nextValue = nextIt->second;
            //std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
            // Access current key, value, next key, and next value
            //std::cout << "Current: Key=" << currentKey  << std::endl;
            //std::cout << "Next: Key=" << nextKey  << std::endl;
            //std::cout<<"Bins: ";
            for (int k = 0; k <vector_size-1; k++ )
            {
            //std::cout<<Xvector.at(k)<< ", "<< Xvector.at(k+1)<< std::endl;
            double avg_Y = .5 * (Xvector.at(k) +Xvector.at(k+1) );
            double avg_X = .5 * (currentKey + nextKey); 
            
            BinMap BinMapSingle{Xvector.at(k),Xvector.at(k+1),currentKey,nextKey,avg_X,avg_Y};
            
            Double_t x1 = currentKey;
            Double_t y1 = Xvector.at(k);
            Double_t x2 = nextKey;
            Double_t y2 = Xvector.at(k+1);
            
            int binN = h2p->AddBin(x1,y1, x2,y2);
            //std::cout<<"BinN = "<< binN<< std::endl;
            TH1Poly_binMap.insert(std::pair<int,BinMap >(binN,BinMapSingle)); 
            }
            //std::cout<< " "<< std::endl;
            
            
        } else {
            // If there is no next element, handle accordingly
            //std::cout << "Finished last bin  element with Key=" << currentKey << std::endl;
        }
    
   // std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
        
}

 return h2p; 

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
UBTH2Poly * Make2DHist_inclusive_UB(std::map<double,std::vector<double>> Bin_edges, 
std::map<int , BinMap> &TH1Poly_binMap, char *name){

	UBTH2Poly *h2p = new UBTH2Poly();

	h2p->SetName(name);

	for (auto it = Bin_edges.begin(); it != Bin_edges.end(); ++it) {
        double currentKey = it->first;
        auto  Xvector = it->second;
        int vector_size = Xvector.size();   
        // Check if there is a next element
        auto nextIt = std::next(it);
        if (nextIt != Bin_edges.end()) {
            double nextKey = nextIt->first;
            //if(nextKey ==2)continue;
            //std::string nextValue = nextIt->second;
            //std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
            // Access current key, value, next key, and next value
            //std::cout << "Current: Key=" << currentKey  << std::endl;
            //std::cout << "Next: Key=" << nextKey  << std::endl;
            //std::cout<<"Bins: ";
            for (int k = 0; k <vector_size-1; k++ )
            {
            //std::cout<<Xvector.at(k)<< ", "<< Xvector.at(k+1)<< std::endl;
            double avg_Y = .5 * (Xvector.at(k) +Xvector.at(k+1) );
            double avg_X = .5 * (currentKey + nextKey); 
            
            BinMap BinMapSingle{Xvector.at(k),Xvector.at(k+1),currentKey,nextKey,avg_X,avg_Y};
            int binN = h2p->AddBin(currentKey,Xvector.at(k), nextKey, Xvector.at(k+1));
            //std::cout<<"BinN = "<< binN<< std::endl;
            TH1Poly_binMap.insert(std::pair<int,BinMap >(binN,BinMapSingle)); 
            }
            //std::cout<< " "<< std::endl;
            
            
        } else {
            // If there is no next element, handle accordingly
            //std::cout << "Finished last bin  element with Key=" << currentKey << std::endl;
        }
    
   // std::cout<<"~~~~~~~~~~~~~~~~~"<< std::endl;
    
    
 }


 return h2p; 

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
UBTH2Poly *Make2DHist_UB( std::map<double,std::vector<double>> Bin_edges, char *name){
    std::map<int , BinMap> Input_place; 
    UBTH2Poly *h2p_test = Make2DHist_UB(Bin_edges,Input_place, name);

    return h2p_test;

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////

UBTH2Poly *Make2DHist_UB_inclusive( std::map<double,std::vector<double>> Bin_edges, char *name){
    std::map<int , BinMap> Input_place; 
    UBTH2Poly *h_UBPolyth2 = Make2DHist_inclusive_UB(Bin_edges, Input_place, name);
    
    return h_UBPolyth2;

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
UBTH2Poly *Make2DHist_UB_inclusive_BinNumMap( std::map<double,std::vector<double>> Bin_edges, char *name){
    std::map<int , BinMap> BinningMap; 
    UBTH2Poly *h2p_test = Make2DHist_inclusive_UB(Bin_edges, BinningMap, name);
    h2p_test->GetXaxis()->SetTitle("Cos(#theta_{#mu})");
    h2p_test->GetXaxis()->CenterTitle();
    h2p_test->GetYaxis()->SetTitle("P_{#mu} [GeV/c]");
    h2p_test->GetYaxis()->CenterTitle();
    h2p_test->SetMarkerSize(0);
    h2p_test->Draw("text");

    DrawBinningInfo(BinningMap);
    DrawBinningNum(BinningMap);

    return h2p_test;

}/// End of Function
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
bool checkConditions_ProtonBDT( std::vector<float> vector1_BDT_prediction, 
 std::vector<int> vector2_BDT_PID,
float threshold1, int pdg_type) {
    // Check if both vectors have the same size
    if (vector1_BDT_prediction.size() != vector2_BDT_PID.size()) {
        std::cerr << "Error: Vectors must have the same size." << std::endl;
        return false;
    }

    // Iterate through the vectors
    for (size_t i = 0; i < vector1_BDT_prediction.size(); ++i) {
        // Check the conditions for each pair of elements
        
      if(vector2_BDT_PID[i] == pdg_type){
        if (vector1_BDT_prediction[i] < threshold1) {
            // If either condition fails, return false
            return false;
        }
        }
        
    }

    // If all pairs pass the conditions, return true
    return true;
}// New of Function
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
/*
std::vector<double> get_bin_low_edges( double xmin, double xmax, int Nbins )
{
  std::vector<double> bin_low_edges;
  double bin_step = ( xmax - xmin ) / Nbins;
  for ( int b = 0; b <= Nbins; ++b ) {
    double low_edge = xmin + b*bin_step;
    bin_low_edges.push_back( low_edge );
  }

  return bin_low_edges;
}
*/
//////////////////////////////// End of Function 
std::vector<double> generateBins() {
    std::vector<double> bins;

    // Number of bins per 0.1 value
    const int binsPerInterval = 4;

    // Number of intervals (0 to 1)
    const int numIntervals = 10;

    for (int i = 0; i <= numIntervals; ++i) {
        for (int j = 0; j < binsPerInterval; ++j) {
            double bin = i * 0.1 + j * 0.1 / binsPerInterval;
            bins.push_back(bin);
            if(bin==1) break; 
        }
    }

    return bins;
}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
std::vector<double> generateBins(int binsPerInterval, int numIntervals, float spacing  ) {
    std::vector<double> bins;

   
    for (int i = 0; i < numIntervals; ++i) {
        for (int j = 0; j < binsPerInterval; ++j) {
            double bin = i * spacing + j * spacing / binsPerInterval;
            bins.push_back(bin);
        }
    }
    
    bins.push_back(numIntervals*spacing);

    return bins;
}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////

void DrawBinningInfo(std::map<int , BinMap> TH1Poly_binMap_input){
 for(auto binmap :TH1Poly_binMap_input){
  //double centerx; 
  //double centery; 
    Double_t xLowEdge =binmap.second.xlow;
    Double_t xUpEdge = binmap.second.xhigh;
    Double_t yLowEdge = binmap.second.ylow;
    Double_t yUpEdge =binmap.second.yhigh;

    // Offset values for demonstration
    Double_t offset1 = 0.025;
    Double_t offset2 = -0.034;
    Double_t offset2y = -0.01;
    
     TText* textrange1 = new TText(binmap.second.centerx,
                             binmap.second.centery + offset2 + offset2y,
                             Form("X (%1.2f,%1.2f)",xLowEdge,xUpEdge));
            textrange1->SetTextSize(0.005);
            textrange1->SetTextAlign(22);  // Centered
            textrange1->SetTextColor(kBlue);
            textrange1->Draw(); 
            
            TText* textrange = new TText(binmap.second.centerx,
                                     binmap.second.centery + offset2 ,
                                     Form("(%1.2f,%1.2f)",yLowEdge,yUpEdge));
            textrange->SetTextSize(0.005);
            textrange->SetTextAlign(22);  // Centered
            textrange->SetTextColor(kBlue );
            textrange->Draw();             
 }// End of Loop 
  
 return; 

}// End of fucntion 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////

void DrawBinningNum(std::map<int , BinMap> TH1Poly_binMap_input){
 
 for(auto binmap :TH1Poly_binMap_input){
  //double centerx; 
  //double centery; 
    Double_t xLowEdge =binmap.second.xlow;
    Double_t xUpEdge = binmap.second.xhigh;
    Double_t yLowEdge = binmap.second.ylow;
    Double_t yUpEdge =binmap.second.yhigh;

    // Offset values for demonstration
    Double_t offset1 = 0.025;
    Double_t offset2 = -0.034;
    Double_t offset2y = -0.01;
    
     TText* textrange1 = new TText(binmap.second.centerx,
                             binmap.second.centery,
                             Form("%i",binmap.first));
            textrange1->SetTextSize(0.02);
            textrange1->SetTextAlign(22);  // Centered
            textrange1->SetTextColor(kBlack);
            textrange1->Draw(); 
                       
 }// End of Loop 
  
 return; 

}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
void DrawBinningInfo_Area(std::map<int , BinMap> TH1Poly_binMap_input){
 for(auto binmap :TH1Poly_binMap_input){
  //double centerx; 
  //double centery; 
    Double_t xLowEdge =binmap.second.xlow;
    Double_t xUpEdge = binmap.second.xhigh;
    Double_t yLowEdge = binmap.second.ylow;
    Double_t yUpEdge =binmap.second.yhigh;

    // Offset values for demonstration
    Double_t offset1 = 0.025;
    Double_t offset2 = -0.034;
    Double_t offset2y = -0.01;
    
     TText* textrange1 = new TText(binmap.second.centerx,
                             binmap.second.centery + offset2 + offset2y,
                             Form("X (%1.2f,%1.2f)",xLowEdge,xUpEdge));
            textrange1->SetTextSize(0.005);
            textrange1->SetTextAlign(22);  // Centered
            textrange1->SetTextColor(kBlue);
            textrange1->Draw(); 
            
            TText* textrange = new TText(binmap.second.centerx,
                                     binmap.second.centery + offset2 ,
                                     Form("(%1.2f,%1.2f)",yLowEdge,yUpEdge));
            textrange->SetTextSize(0.005);
            textrange->SetTextAlign(22);  // Centered
            textrange->SetTextColor(kBlue );
            textrange->Draw();             
 }// End of Loop 
  
 return; 

}// End of fucntion 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////





void saveUBTH2PolyToTextFile(const UBTH2Poly& hist, char* fileName) {
    // Open the file for writing
    std::ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open the file " << fileName << " for writing." << std::endl;
        return;
    }
    Int_t Nbins = hist.GetNumberOfBins();
    // Iterate over bins and write bin number and content to the file
    for (Int_t bin = 1; bin <= Nbins; ++bin) {
            Double_t binContent = hist.GetBinContent(bin);
            // Write to the file
            outputFile << bin << "," << binContent << std::endl;
        }
    // Close the file
    outputFile.close();
}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////


PROJECTION_Bin_Map GetBin_ProjectionMap(){

PROJECTION_Bin_Map Map{ 
{kProj_Bin1, {1,2,3,4}},
{kProj_Bin2, {5,6,7,8,9,10,11}},
{kProj_Bin3, {12,13,14,15,16,17,18,19}},
{kProj_Bin4, {20,21,22,23,24,25}},
{kProj_Bin5, {26,27,28,29,30,31,32}},
{kProj_Bin6, {33,34,35,36,37}},
{kProj_Bin7, {38,39,40,41}},
{kProj_Bin8, {42,43}},
{kProj_Bin9, {44,45}} 
};



return  Map;

} //////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////

PROJECTION_InclusBin_Map GetInclusvieBin_ProjectionMap(){


PROJECTION_InclusBin_Map Map{ 
{kProj_InclBin1, {1,2,3}},
{kProj_InclBin2, {4,5,6,7}},
{kProj_InclBin3, {8,9,10,11,12}},
{kProj_InclBin4, {13,14,15,16}},
{kProj_InclBin5, {17,18,19,20}},
{kProj_InclBin6, {21,22,23,24,25}},
{kProj_InclBin7, {26,27,28,29,30}},
{kProj_InclBin8, {31,32,33,34}},
{kProj_InclBin9, {35,36,37,38,39}} 
};


/*
PROJECTION_InclusBin_Map Map{ 
{kProj_InclBin1, {0,1,2}},
{kProj_InclBin2, {3,4,5,6,7}},
{kProj_InclBin3, {8,9,10,11,12}},
{kProj_InclBin4, {13,14,15,16}},
{kProj_InclBin5, {17,18,19,20}},
{kProj_InclBin6, {21,22,23,24,25}},
{kProj_InclBin7, {26,27,28,29,30}},
{kProj_InclBin8, {31,32,33,34}},
{kProj_InclBin9, {35,36,37,38,39}} 
};
*/

return Map; 



}//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////


std::map<Binning2D, TH1D*> ConstructProjectionMap(
TFile& fin,std::vector<Binning2D> Bin_vector, char *BaseName ){

std::map<Binning2D, TH1D*> OutPutMap; 
char Name[1024];

for(auto bin: Bin_vector){
  sprintf(Name, "%s_Bin_%i", BaseName, bin);
   TH1D* Hist = GetTH1DHist( fin, Name );
    OutPutMap.insert(std::pair<Binning2D, TH1D*>(bin,Hist)); 
}


return OutPutMap;

}//////////////////////////////// End of Function 
std::map<std::pair<Binning2D, EventCategory>, TH1D*> Construct_CategoryProjectionMap(
TFile& fin,std::vector<Binning2D> Bin_vector,
std::vector<EventCategory> Stack_category, char *BaseName ){



std::map<std::pair<Binning2D, EventCategory>, TH1D*> OutPutMap; 
char Name[1024];
auto& eci = EventCategoryInterpreter::Instance();
for(auto bin: Bin_vector){
  for (auto stack:Stack_category){
     std::string label  = eci.Hist_label( stack );

  sprintf(Name, "%s_EventCategory_%s_Bin_%i", BaseName,label.c_str(), bin);
   
   TH1D* Hist = GetTH1DHist( fin, Name );
    OutPutMap[{bin,stack}] = Hist; 
  }

}


return OutPutMap;

}



////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
std::vector<Binning2D> GetProjectBinVector(){
std::vector<Binning2D> ReturnVector; 

ReturnVector.push_back(kProj_Bin1);
ReturnVector.push_back(kProj_Bin2);
ReturnVector.push_back(kProj_Bin3);
ReturnVector.push_back(kProj_Bin4);
ReturnVector.push_back(kProj_Bin5);
ReturnVector.push_back(kProj_Bin6);
ReturnVector.push_back(kProj_Bin7);
ReturnVector.push_back(kProj_Bin8);
ReturnVector.push_back(kProj_Bin9);

return ReturnVector; 

}
//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////

std::vector<Binning2DInclusive> GetProjectInclusiveBinVector(){

std::vector<Binning2DInclusive> ReturnVector; 

ReturnVector.push_back(kProj_InclBin1);
ReturnVector.push_back(kProj_InclBin2);
ReturnVector.push_back(kProj_InclBin3);
ReturnVector.push_back(kProj_InclBin4);
ReturnVector.push_back(kProj_InclBin5);
ReturnVector.push_back(kProj_InclBin6);
ReturnVector.push_back(kProj_InclBin7);
ReturnVector.push_back(kProj_InclBin8);
ReturnVector.push_back(kProj_InclBin9);

return ReturnVector; 


}
//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
std::map<Binning2D , double> Projection9Bins_width(std::map< double, std::vector<double> > InputBins){
 std::map<Binning2D , double> returnMap; 
 auto BinVector = GetProjectBinVector();

for(int i = 0 ; i <InputBins.size(); i++ ){
    
    if (i == InputBins.size()-1) continue; 
    auto lower_edge = InputBins.begin();
    auto upper_edge = InputBins.begin();
    std::advance(lower_edge, i );
    std::advance(upper_edge, i + 1 );
    
    double low = lower_edge->first; 
    double up = upper_edge->first; 
    double width  = up - low;
    
        returnMap.insert(std::pair<Binning2D , double>(BinVector.at(i), width ));
}

return returnMap;

}





std::map<Binning2D , std::string > Projection9Bins_StringMap(std::map< double, std::vector<double> > InputBins, std::string Par_name){

std::map<Binning2D , std::string > returnMap; 
auto BinVector = GetProjectBinVector();


for(int i = 0 ; i <InputBins.size(); i++ ){
    
    if (i == InputBins.size()-1) continue; 
    auto lower_edge = InputBins.begin();
    auto upper_edge = InputBins.begin();
    std::advance(lower_edge, i );
    std::advance(upper_edge, i + 1 );
    
    double low = lower_edge->first; 
    double up = upper_edge->first; 
    char name[1024];
    sprintf(name, " %.2f < %s < %.2f", low, Par_name.c_str(), up);
    
    std::string BinString(name);
    
    returnMap.insert(std::pair<Binning2D , std::string>(BinVector.at(i),BinString ));
    
    }
    
 return returnMap;
    
}// End of function
//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////

double MaxYofMap(std::map<Binning2D, TH1D*> inputMap){

double returnMax = -1.0; 

for(auto Bin: inputMap){
    double maxBin =  Bin.second ->GetMaximum();
    if(maxBin > returnMax)
    {
      returnMax = maxBin;
    }
  }
  
    return returnMax;
}
//////////////////////////////// End of Function 
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
std::vector<double> GetCCZeroPi_Binning(variable_binning inputVar){
    std::vector<double> bins_vec;
 
 
 switch (inputVar) {

    case kBINNING_Pmu:
    bins_vec = { 0.1, 0.24, 0.3, 0.38, 0.48, 0.7, 0.85, 1.28, 1.58, 2.0};
    return bins_vec;

    case kBINNING_Costheta:
    bins_vec = {-1.00, -0.50, 0.00,  0.27,  0.45, 0.62, 0.76, 0.86, 0.94, 1.0};
    return bins_vec;

    case kBINNING_Pmu_Proton:
    bins_vec = {0.25, 0.41, 0.495, 0.56, 0.62, 0.68, 0.74, 0.8, 0.87, 0.93, 1.2 };
    return bins_vec;

    case kBINNING_Costheta_Proton:
    bins_vec = {-1.0, -0.5, 0.0, 0.27, 0.45, 0.62, 0.76, 0.86, 0.94, 1.0 };

    case kBINNING_pn:
    bins_vec = {0., 0.125, 0.225, 0.325, 0.425, 0.525, 0.65, 0.85 };
    return bins_vec;

    case kBINNING_delta_alphaT:
    bins_vec = { 0, 0.35, 0.85, 1.35, 1.85, 2.3, 2.7, 2.95};
    return bins_vec;

    case kBINNING_delta_pTx:
    bins_vec = { -0.6, -0.45, -0.35, -0.25, -0.15, -0.075, 0, 0.075, 0.15, 0.25,  0.35, 0.45, 0.6};
    return bins_vec;

    case kBINNING_delta_pTy:
    bins_vec = { -0.8, -0.55, -0.39, -0.2125, -0.05, 0.1, 0.225, 0.3375, 0.5};
    return bins_vec;

    case kBINNING_delta_phiT:
    bins_vec = {0., 0.075, 0.2, 0.35, 0.5, 0.7, 0.9, 1.15, 1.4, 1.65, 1.9, 2.35, 2.8};
    return bins_vec;

    case kBINNING_delta_pT:
    bins_vec = {0, 0.1, 0.2, 0.3, 0.4, 0.525, 0.675, 0.9};
    return bins_vec;

    case kBINNING_VertexX:
    bins_vec = {-10,0,25,50,100,125,150,175,200,225,250,275,300,325, 350 };
    return bins_vec;

    case kBINNING_VertexY:
    bins_vec = {-200,-175,-150,-125,-100.,-75,-50,-25,0,25,50,100,125,150,175,200 };
    return bins_vec;

    case kBINNING_VertexZ:
    bins_vec = {-10,0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,1000,1050,1100,1150 ,1200};
    return bins_vec;

    case kBINNING_Mult:
    bins_vec = {-0.5, 0.5, 1.5, 2.5, 3.5, 4.5};
    return bins_vec;

    case kBINNING_Probability:
    bins_vec = generateBins();
    return bins_vec;
    
    case kBINNING_trk_distance_v:
    bins_vec = generateBins(4,15, 1 );
    return bins_vec;

    case kBINNING_trk_len_v:
    bins_vec = generateBins(10,10, 10);
    return bins_vec;

    case kBINNING_Cosmic:
    bins_vec = generateBins(1, 40, 5);
    return bins_vec;

    default:
    std::cout << "ERROR: unknown Playlist!" << std::endl;
    return {9999};
  };


}

////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////


void TKI_Hists::FillRECOHist(TKI_parameters input, double wgt){

h_Pproton_->Fill(input.Pproton,wgt);
h_CosThetaproton_->Fill(input.PCostheta,wgt);
h_delta_alphaT_->Fill(input.delta_alphaT,wgt);
h_delta_pTx_->Fill(input.delta_pTx,wgt);
h_delta_pTy_->Fill(input.delta_pTy,wgt);
h_delta_phiT_->Fill(input.delta_phiT,wgt);
h_pn_->Fill(input.pn,wgt);


h_Pproton_Resolution_->Fill(input.Pproton - input.Pproton_mc ,wgt);
h_CosThetaproton_Resolution_->Fill(input.PCostheta - input.PCostheta_mc ,wgt);
h_delta_alphaT_Resolution_->Fill(input.delta_alphaT - input.delta_alphaT_mc ,wgt);
h_delta_pTx_Resolution_->Fill(input.delta_pTx - input.delta_pTx_mc ,wgt);
h_delta_pTy_Resolution_->Fill(input.delta_pTy - input.delta_pTy_mc ,wgt);
h_delta_phiT_Resolution_->Fill(input.delta_phiT - input.delta_phiT_mc ,wgt);
h_pn_Resolution_->Fill(input.pn - input.pn_mc ,wgt);


//h_Pproton_EventCategory_.GetComponentHist(input.category_type)->Fill(input.Pproton,wgt);
//h_CosThetaproton_EventCategory_.GetComponentHist(input.category_type)->Fill(input.PCostheta,wgt);
//h_delta_alphaT_EventCategory_.GetComponentHist(input.category_type)->Fill(input.delta_alphaT,wgt);
//h_delta_pTx_EventCategory_.GetComponentHist(input.category_type)->Fill(input.delta_pTx,wgt);
//h_delta_pTy_EventCategory_.GetComponentHist(input.category_type)->Fill(input.delta_pTy,wgt);
//h_delta_phiT_EventCategory_.GetComponentHist(input.category_type)->Fill(input.delta_phiT,wgt);
//h_pn_EventCategory_.GetComponentHist(input.category_type)->Fill(input.pn,wgt); 

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////


void TKI_Hists::FillTRUEHist(TKI_parameters input, double wgt){


 h_Pproton_TRUE_->Fill(input.Pproton_mc,wgt);
 h_CosThetaproton_TRUE_->Fill(input.PCostheta_mc,wgt);
 h_delta_alphaT_TRUE_->Fill(input.delta_alphaT_mc,wgt);
 h_delta_pTx_TRUE_->Fill(input.delta_pTx_mc,wgt);
 h_delta_pTy_TRUE_->Fill(input.delta_pTy_mc,wgt);
 h_delta_phiT_TRUE_->Fill(input.delta_phiT_mc,wgt);
 h_pn_TRUE_->Fill(input.pn_mc,wgt);   

}

////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////



void TKI_Hists::FillTRUE_RECOHist(TKI_parameters input, double wgt){


 h_Pproton_TRUE_RECO_->Fill(input.Pproton_mc,wgt);
 h_CosThetaproton_TRUE_RECO_->Fill(input.PCostheta_mc,wgt);
 h_delta_alphaT_TRUE_RECO_->Fill(input.delta_alphaT_mc,wgt);
 h_delta_pTx_TRUE_RECO_->Fill(input.delta_pTx_mc,wgt);
 h_delta_pTy_TRUE_RECO_->Fill(input.delta_pTy_mc,wgt);
 h_delta_phiT_TRUE_RECO_->Fill(input.delta_phiT_mc,wgt);
 h_pn_TRUE_RECO_->Fill(input.pn_mc,wgt);   

 h_Pproton_Mig_->Fill(input.Pproton_mc, input.Pproton, wgt);
 h_CosThetaproton_Mig_ ->Fill(input.PCostheta_mc, input.PCostheta, wgt);  
 h_delta_alphaT_Mig_->Fill(input.delta_alphaT_mc, input.delta_alphaT, wgt);
 h_delta_pTx_Mig_->Fill(input.delta_pTx_mc, input.delta_pTx, wgt);
 h_delta_pTy_Mig_->Fill(input.delta_pTy_mc, input.delta_pTy, wgt);
 h_delta_phiT_Mig_->Fill(input.delta_phiT_mc, input.delta_phiT, wgt);
 h_pn_Mig_->Fill(input.pn_mc, input.pn, wgt);   

}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////


void TKI_Hists::WriteAll(TFile &outfile) const{

outfile.cd();

 h_Pproton_->Write();
 h_CosThetaproton_->Write();
 h_delta_alphaT_->Write();
 h_delta_pTx_->Write();
 h_delta_pTy_->Write();
 h_delta_phiT_->Write();
 h_pn_->Write();
 
 h_Pproton_TRUE_->Write();
 h_CosThetaproton_TRUE_->Write();
 h_delta_alphaT_TRUE_->Write();
 h_delta_pTx_TRUE_->Write();
 h_delta_pTy_TRUE_->Write();
 h_delta_phiT_TRUE_->Write();
 h_pn_TRUE_->Write();     
 h_Pproton_TRUE_RECO_->Write();
 h_CosThetaproton_TRUE_RECO_->Write();
 h_delta_alphaT_TRUE_RECO_->Write();
 h_delta_pTx_TRUE_RECO_->Write();
 h_delta_pTy_TRUE_RECO_->Write();
 h_delta_phiT_TRUE_RECO_->Write();
 h_pn_TRUE_RECO_->Write();        
 h_Pproton_Mig_->Write();
 h_CosThetaproton_Mig_->Write();    
 h_delta_alphaT_Mig_->Write();
 h_delta_pTx_Mig_->Write();
 h_delta_pTy_Mig_->Write();
 h_delta_phiT_Mig_->Write();
 h_pn_Mig_->Write();  


//h_Pproton_EventCategory_.WriteToFile(outfile);
//h_CosThetaproton_EventCategory_.WriteToFile(outfile);
//h_delta_alphaT_EventCategory_.WriteToFile(outfile);
//h_delta_pTx_EventCategory_.WriteToFile(outfile);
//h_delta_pTy_EventCategory_.WriteToFile(outfile);
//h_delta_phiT_EventCategory_.WriteToFile(outfile);
//h_pn_EventCategory_.WriteToFile(outfile);


h_Pproton_Resolution_->Write();  
h_CosThetaproton_Resolution_->Write();  
h_delta_alphaT_Resolution_->Write();  
h_delta_pTx_Resolution_->Write();  
h_delta_pTy_Resolution_->Write();  
h_delta_phiT_Resolution_->Write();  
h_pn_Resolution_->Write();  


}
////////////////////////////////////////////////////////////////
////
//////////////////////////////////////////////////////////////////
void BinNormalizeTOFractionOF_Events(TObjArray &input_Array){

  int NEntries = input_Array.GetEntries();

  TH1D* TotalH = (TH1D*)input_Array.At(0)->Clone(uniq());

  for(int i = 1 ; i < NEntries; ++i){
    TotalH->Add((TH1D*)input_Array.At(i),1.0);
  }
  
  for(int i = 0 ; i < NEntries; ++i){
    ((TH1D*)input_Array.At(i))->Divide((TH1D*)input_Array.At(i),TotalH);
  }


}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_X_vs_Y_Tgraph_fromVector(std::vector<Vertex_XYZ> input)
{
  const int size = input.size();
  double YY[size];
  double XX[size];

  for(unsigned int j =0; j<size;j++){
  XX[j]=input.at(j).x;
  YY[j]=input.at(j).y;
  }

  TGraph *Tg_result = new TGraph(size,XX,YY);

    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_X_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ> input)
{
  const int size = input.size();
  double ZZ[size];
  double XX[size];

  for(unsigned int j =0; j<size;j++){
  XX[j]=input.at(j).x;
  ZZ[j]=input.at(j).z;
  }

  TGraph *Tg_result = new TGraph(size,ZZ,XX);

    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_Y_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{

  const int size = input.size();
  double ZZ[size];
  double YY[size];

  for(unsigned int j =0; j<size;j++){
  YY[j]=input.at(j).y;
  ZZ[j]=input.at(j).z;
  }

  TGraph *Tg_result = new TGraph(size,ZZ,YY);


  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("Z [mm]");
  Tg_result->GetYaxis()->SetTitle("Y [mm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);


    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
TGraph  *Make_R_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{
  const int size = input.size();
  double ZZ[size];
  double RR[size];

  for(unsigned int j =0; j<size;j++){
  RR[j]=sqrt(pow(input.at(j).x,2)+pow(input.at(j).z,2));
  ZZ[j]=input.at(j).z;
  }

  TGraph *Tg_result = new TGraph(size,ZZ,RR);

  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("Z [mm]");
  Tg_result->GetYaxis()->SetTitle("R [mm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);
    return Tg_result;

}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

TGraph  *Make_RR_vs_Z_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{

  const int size = input.size();
  double ZZ[size];
  double RR[size];

  for(unsigned int j =0; j<size;j++){
  RR[j]=pow(input.at(j).z,2) + pow(input.at(j).x,2);
  ZZ[j]=input.at(j).z;
  }

  TGraph *Tg_result = new TGraph(size,ZZ,RR);

  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("Z [mm]");
  Tg_result->GetYaxis()->SetTitle("R^{2} [mm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);
    
    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

TGraph  *Make_R_vs_Y_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{

  const int size = input.size();
  double YY[size];
  double RR[size];

  for(unsigned int j =0; j<size;j++){
  RR[j]=sqrt(pow(input.at(j).z,2) + pow(input.at(j).x,2));
  YY[j]=input.at(j).y;
  }

  TGraph *Tg_result = new TGraph(size,YY,RR);

  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("Z [mm]");
  Tg_result->GetYaxis()->SetTitle("R [mm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);
  Tg_result->SetMinimum(0);

    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

TGraph  *Make_RR_vs_Y_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{

  const int size = input.size();
  double YY[size];
  double RR[size];

  for(unsigned int j =0; j<size;j++){
  RR[j]=pow(input.at(j).z,2) + pow(input.at(j).x,2);
  YY[j]=input.at(j).y;
  }

  TGraph *Tg_result = new TGraph(size,YY,RR);

  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("T [cm]");
  Tg_result->GetYaxis()->SetTitle("R^{2} [cm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);


    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

TGraph  *Make_RR_vs_X_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{

  const int size = input.size();
  double X[size];
  double RR[size];

  for(unsigned int j =0; j<size;j++){
  RR[j]=pow(input.at(j).z,2) + pow(input.at(j).x,2);
  X[j]=input.at(j).x;
  }

  TGraph *Tg_result = new TGraph(size,X,RR);

  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("X [mm]");
  Tg_result->GetYaxis()->SetTitle("R^{2} [mm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);

    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

TGraph  *Make_R_vs_X_Tgraph_fromVector(std::vector<Vertex_XYZ>input)
{

  const int size = input.size();
  double XX[size];
  double RR[size];

  for(unsigned int j =0; j<size;j++){
  RR[j]=sqrt(pow(input.at(j).z,2) + pow(input.at(j).x,2));
  XX[j]=input.at(j).x;
  }

  TGraph *Tg_result = new TGraph(size,RR,XX);

  Tg_result->GetXaxis()->CenterTitle();
  Tg_result->GetYaxis()->CenterTitle();
  Tg_result->GetXaxis()->SetTitle("X [cm]");
  Tg_result->GetYaxis()->SetTitle("R [cm]");
  Tg_result->GetXaxis()->SetTitleSize(0.038);
  Tg_result->GetYaxis()->SetTitleSize(0.038);
  Tg_result->SetLineColor(2);
  Tg_result->SetLineWidth(4);
  Tg_result->SetMarkerColor(1);
  Tg_result->SetMarkerSize(1);
  Tg_result->SetMarkerStyle(20);


    return Tg_result;
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
GENIE_MC GENIE_Type(int input, bool combined){

  if(combined == false){
  switch (input)
      {
        case 0:
        return kCCQE;
        
        case 10:
        return kCCMEC;
        
        case 1: 
        return kCCRES;
        
        case 2:
        return kCCDIS;
        
        case 3: 
        return kCCCOH;
        
        default:
        std::cout<<"UNknown interaction type Maybesomething is wrong INPUT = "<< input<< std::endl;
        return kCCOTHER;   
      }
    } 
  else {
       switch (input)
           {
             case 0:
             return kCCQE;
             
             case 10:
             return kCCMEC;
             
             case 1: 
             return kCCRES;
             
             case 2:
             return kCCOTHER;
             
             case 3: 
             return kCCOTHER;
             
             default:
             std::cout<<"UNknown interaction type Maybesomething is wrong INPUT = "<< input<< std::endl;
             return kCCOTHER;   
           }

     }
}
////////////////////////////////////////////////////////////////////////
Particle_top_groups return_Particle_top_groups(int Npion, int Nprotons, int NComsics, int Neletrons, int NOther, int NMuon){

if(NOther>0 && Nprotons==0 && NComsics==0 && Neletrons ==0 && Npion==0 && NMuon ==0) {return kCC_OTHER; }
else if(Nprotons==0 && NComsics==0 && Neletrons ==0 && NOther==0 && Npion>=1 && NMuon ==1){
 if(Npion==1){return kCC_1pi; }
 else if(Npion==2){return kCC_2pi; }
 else if(Npion>=3){return kCC_3pi; }
}
else if(Nprotons==1 && NComsics==0 && Neletrons ==0 && NOther==0 && Npion==0 && NMuon ==0 ){return kNC_1p;}
else if(NComsics==0 && Npion>=1 &&  Neletrons==0 && Nprotons>=1 && NMuon ==1 ){
 if(Npion==1 && Nprotons ==1 ){return kCC_1pi_1p; }
 else if(Npion==2 && Nprotons ==1 ){return kCC_2pi_1p; }
 else if(Npion==1 && Nprotons ==2 ){return kCC_1pi_2p; }
}
else if(NComsics==0 && Npion==0 &&  Neletrons==0 && Nprotons>=1 && NMuon ==1 ){
 if(Nprotons ==1 ){return kCC_1p; }
 else if(Nprotons ==2 ){return kCC_2p; }
 else if(Nprotons >=3 ){return kCC_3p; }
}
else if(NComsics>=1 && Npion==0 &&  Neletrons==0 && Nprotons>=1 && Neletrons==0 && NMuon ==1){
  if(Nprotons ==1 ){
       if(NComsics==1){return kCC_1p_1comsic; }
       else if(NComsics==2){return kCC_1p_2comsic;}
       else if(NComsics>=3){return kCC_1p_3comsic;}
       else if(Nprotons ==2){
  return kCC_2p_Ncomsic;
  }
}
}
else if(NComsics==1 && Npion==0 &&  Neletrons>=1 && Nprotons>=1 && NMuon ==1){
  if(Nprotons ==1 ){
       if(Neletrons==1){return kCC_1p_1eletrons; }
       else if(Neletrons==2){return kCC_1p_2eletrons;}
       else if(Neletrons>=3){return kCC_1p_3eletrons;}
  else if(Nprotons == 2){return kCC_2p_Neletrons;}
}
}
else if(Nprotons==0 && Npion>=1 && Neletrons==0 && NMuon ==1 ){
  if(Npion==1 ){
    if (Neletrons>=1 && NComsics ==0){return kCC_1pi_Neletrons; }
    else if(NComsics==1){return kCC_1pi_1comsic; }
    else if(NComsics==2){return kCC_1pi_2comsic;}
    else if(NComsics>=3){return kCC_1pi_3comsic;}
 }
  if(Npion==2 ){
     if(NComsics==1){return kCC_2pi_1comsic; }
     else if(NComsics==2){return kCC_2pi_2comsic;}
     else if(NComsics>=3){return kCC_2pi_3comsic;}
   }
}
else if(NComsics >=1 && Neletrons ==0 && Npion==0 && Nprotons==0 && NMuon ==1){
  if(NComsics==1){return kCC_1comsic; }
   else if(NComsics==2){return kCC_2comsic;}
   else if(NComsics>=3){return kCC_3comsic;}
}
else if (Npion==1 && Nprotons==1 && NComsics>=1 && Neletrons==0 && NMuon ==1 ){return kCC_1pi_1p_Ncomsic; }
else if (NMuon>=1 && Nprotons==0 && NComsics==0 && Npion==0 && Neletrons==0){
 if(NMuon==1) return kCC_1mu;
   else if(NMuon==2) return kCC_2mu; 
   else if (NMuon>2) return kCC_3mu; 
}
else if (NMuon==2 && Nprotons>=0 && NComsics==0 && Npion>=0 && Neletrons==0){return kCC_2muNpMpi;}
else if (NMuon==3 && Nprotons>=0 && NComsics==0 && Npion>=0 && Neletrons==0){return kCC_3muNpMpi;}
else if (NMuon==2 && Nprotons==0 && NComsics>0 && Npion==0 && Neletrons==0){return kCC_2muNcomsic;}
else if (NMuon==3 && Nprotons==0 && NComsics>0 && Npion==0 && Neletrons==0){return kCC_3muNcomsic;}

else if(NMuon==0 && Nprotons==0 && NComsics==1 && Npion==0 && Neletrons==0){return k1comsic;}
else if(NMuon==0 && Nprotons==0 && NComsics==2 && Npion==0 && Neletrons==0){return k2comsic;}
else if(NMuon==0 && Nprotons==0 && NComsics>2 && Npion==0 && Neletrons==0){return k3comsic;}
else if(NMuon==0 && Nprotons > 0 && NComsics==0 && Npion==0 && Neletrons==0){return k_Np;}
else if(NMuon==0 && Nprotons == 1 && NComsics>0 && Npion==0 && Neletrons==0){return k_1p_Ncomsic;}
else if(NMuon==0 && Nprotons == 2 && NComsics>0 && Npion==0 && Neletrons==0){return k_2p_Ncomsic;}
else if(NMuon==0 && Nprotons == 2 && NComsics==0 && Npion>0 && Neletrons==0){return k_1p_Npion;}
else return kCC_OTHER;


}


void addTextToLastEmptyLine(const std::string& filename, const std::string& content) {
    std::ifstream infile(filename); // Open the file for reading

    if (!infile) {
        std::cerr << "Error: Cannot open file for reading: " << filename << std::endl;
        return;
    }

    // Read all lines into a vector
    std::vector<std::string> lines;
    std::string line;
    int lastEmptyLineIndex = -1;

    while (std::getline(infile, line)) {
        if (line.empty()) {
            lastEmptyLineIndex = lines.size(); // Keep track of the last empty line index
        }
        lines.push_back(line);
    }

    infile.close(); // Close the file after reading

    // If no empty line is found, append the content at the end
    if (lastEmptyLineIndex == -1) {
        lines.push_back(content);
    } else {
        lines[lastEmptyLineIndex] = content;
    }

    // Write all lines back to the file
    std::ofstream outfile(filename); // Open the file for writing

    if (!outfile) {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& ln : lines) {
        outfile << ln << std::endl;
    }

    outfile.close(); // Close the file after writing
}

void recreateFileWithFirstLine(const std::string& filename, const std::string& firstLineContent) {
    // Open the file in write mode to recreate it
    std::ofstream outfile(filename);

    if (!outfile) {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        return;
    }

    // Write the string to the first line of the file
    outfile << firstLineContent << std::endl;

    // The file is automatically recreated with only the new content
    outfile.close(); // Close the file after writing
}



MuonExitingPanel ExitingLoction(float Exit_X,float Exit_Y,float Exit_Z){

double FV_X_MIN =  10.0; // 21.5;
double FV_X_MAX = 230.0;// 234.85;

double FV_Y_MIN = -105;//-95.0;
double FV_Y_MAX =  105;//95.0;

double FV_Z_MIN = 10; // 21.5;
double FV_Z_MAX = 1000;//966.5;

if(Exit_X >  FV_X_MIN &&
   Exit_X <  FV_X_MAX &&
   Exit_Y >  FV_Y_MIN &&
   Exit_Y <  FV_Y_MAX &&
   Exit_Z > FV_Z_MAX)
   {return BackPanel;}
else if( (Exit_X > FV_X_MIN || Exit_X < FV_X_MIN) &&
        (Exit_X <  FV_X_MAX || Exit_X > FV_X_MAX )  &&
        Exit_Y > FV_Y_MAX &&
        (Exit_Z < FV_Z_MAX || Exit_Z > FV_Z_MAX ) &&
        (Exit_Z > FV_Z_MIN || Exit_Z < FV_Z_MIN))
        {return TopPanel;}
else if((Exit_X >  FV_X_MIN || Exit_X <  FV_X_MIN) &&
        (Exit_X <  FV_X_MAX ||Exit_X  >  FV_X_MAX) &&
        Exit_Y < FV_Y_MIN &&
        (Exit_Z < FV_Z_MAX || Exit_Z > FV_Z_MAX) &&
        (Exit_Z > FV_Z_MIN || Exit_Z < FV_Z_MIN))
        {return BottomPanel;}
else if(Exit_X > FV_X_MAX &&
        Exit_Y > FV_Y_MIN &&
        Exit_Y < FV_Y_MAX &&
        (Exit_Z < FV_Z_MAX || Exit_Z > FV_Z_MAX) &&
        (Exit_Z > FV_Z_MIN || Exit_Z < FV_Z_MIN))
        {return LeftPanel;}
else if(Exit_X <  FV_X_MIN &&
        Exit_Y > FV_Y_MIN &&
        Exit_Y < FV_Y_MAX &&
        (Exit_Z < FV_Z_MAX || Exit_Z > FV_Z_MAX) &&
        (Exit_Z > FV_Z_MIN || Exit_Z < FV_Z_MIN) )
     {return RightPanel;}
else if( Exit_X >  FV_X_MIN &&
         Exit_X <  FV_X_MAX &&
         Exit_Y >  FV_Y_MIN &&
         Exit_Y <  FV_Y_MAX &&
         Exit_Z < FV_Z_MIN)
       {return FrontPanel;}       
else{return NoPanel; }



}