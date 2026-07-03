//author :: christian nguyen 
#include "XSecAnalyzer/MCSTools.hh"

//Constuctor

/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void MCSTools::InitializePanelMaps(){

    
        SideSpaceing_Z_Y = {
    { 0,     {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX} },
    { 210.0, {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX} },
    { 420.0, {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX} },
    { 630.0, {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX} },
    { 840.0, {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX}},
    { 1050.0, {} }
    };


    SideSpaceing_Z_X = {
    { 0,     {TPC_limit_X_MIN, 125.0,   TPC_limit_X_MAX} },
    { 210.0, {TPC_limit_X_MIN, 125.0,   TPC_limit_X_MAX} },
    { 420.0, {TPC_limit_X_MIN, 125.0,   TPC_limit_X_MAX} },
    { 630.0, {TPC_limit_X_MIN, 125.0,   TPC_limit_X_MAX} },
    { 840.0, {TPC_limit_X_MIN, 125.0,   TPC_limit_X_MAX}},
    { 1050.0, {} }
    };


  SideSpaceing_X_Y = {
    { TPC_limit_X_MIN, {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX} },
    { 125,             {TPC_limit_Y_MIN, 0.0,  TPC_limit_Y_MAX} },
    { TPC_limit_X_MAX, {} }
    };
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
Int_t MCSTools::ClosestSide(double x, double y, double z){
    // Compute distances to each side of the cuboid
    
    double dx_min = std::abs(x - TPC_limit_X_MIN);
    double dx_max = std::abs(x - TPC_limit_X_MAX);
    double dy_min = std::abs(y - TPC_limit_Y_MIN);
    double dy_max = std::abs(y - TPC_limit_Y_MAX);
    double dz_min = std::abs(z - TPC_limit_Z_MIN);
    double dz_max = std::abs(z - TPC_limit_Z_MAX);

    
  // Find Closeest wwall 
  int min_position =  FindMinimum(dx_min, dx_max, dy_min, dy_max, dz_min, dz_max);
  Int_t returnvalue = min_position;
   return returnvalue;
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
  Int_t MCSTools::PanelSingle_TB(double vectex_X,double vectex_Z)const {
    Int_t trueBininclusive = h_TBsides_ ->Fill(vectex_Z, vectex_X,1); 
  return trueBininclusive;
  }
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
Int_t MCSTools::PanelSingle_LR(double vectex_Y,double vectex_Z)const {
     Int_t trueBininclusive = h_LRsides_->Fill(vectex_Z, vectex_Y,1); 
  return trueBininclusive; 
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
Int_t MCSTools::PanelSingle_FB(double vectex_X,double vectex_Y)const {
     Int_t trueBininclusive = h_FBsides_->Fill(vectex_X, vectex_Y,1); 
  return trueBininclusive;
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int  MCSTools::FindMinimum(double a, double b, double c, double d, double e, double f) {
    double numbers[] = {a, b, c, d, e, f};
    double min_value = numbers[0];
    int min_index = 0;

    for (int i = 0; i < 6; ++i) {
        if (numbers[i] < min_value) {
            min_value = numbers[i];
            min_index = i;
        }
    }

    return  min_index+1; // Returning position as 1-based index
}

/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
MCSTools::MuonExitingPanel MCSTools::returnSINGLEPanel(int  MuonExitingPanel,
                                              int SmallerSide_X_Z,
                                              int SmallerSide_Y_Z,
                                              int SmallerSide_X_Y){
 if(LeftPanel == MuonExitingPanel) {
    if(SmallerSide_Y_Z==1) return LSide1; 
    else if (SmallerSide_Y_Z==2) return LSide2; 
    else if (SmallerSide_Y_Z==3) return LSide3; 
    else if (SmallerSide_Y_Z==4) return LSide4; 
    else if (SmallerSide_Y_Z==5) return LSide5; 
    else if (SmallerSide_Y_Z==6) return LSide6; 
    else if (SmallerSide_Y_Z==7) return LSide7; 
    else if (SmallerSide_Y_Z==8) return LSide8; 
    else if (SmallerSide_Y_Z==9) return LSide9; 
    else if (SmallerSide_Y_Z==10) return LSide10; 
    else {return NoPanel; }
  }
 else if(RightPanel == MuonExitingPanel){
    if(SmallerSide_Y_Z==1) return RSide1; 
    else if (SmallerSide_Y_Z==2) return RSide2; 
    else if (SmallerSide_Y_Z==3) return RSide3; 
    else if (SmallerSide_Y_Z==4) return RSide4; 
    else if (SmallerSide_Y_Z==5) return RSide5; 
    else if (SmallerSide_Y_Z==6) return RSide6; 
    else if (SmallerSide_Y_Z==7) return RSide7; 
    else if (SmallerSide_Y_Z==8) return RSide8; 
    else if (SmallerSide_Y_Z==9) return RSide9; 
    else if (SmallerSide_Y_Z==10) return RSide10; 
    else {return NoPanel; }
 }
  else if(TopPanel == MuonExitingPanel){
     if (SmallerSide_X_Z==1) return TSide1;
     else if (SmallerSide_X_Z==2) return TSide2;
     else if (SmallerSide_X_Z==3) return TSide3;
     else if (SmallerSide_X_Z==4) return TSide4;
     else if (SmallerSide_X_Z==5) return TSide5;
     else if (SmallerSide_X_Z==6) return TSide6;
     else if (SmallerSide_X_Z==7) return TSide7;
     else if (SmallerSide_X_Z==8) return TSide8;
     else if (SmallerSide_X_Z==9) return TSide9;
     else if (SmallerSide_X_Z==10) return TSide10;
     else {return NoPanel; }
  }
  else if( BottomPanel == MuonExitingPanel){
      if (SmallerSide_X_Z==1) return BSide1;
      else if (SmallerSide_X_Z==2) return BSide2;
     else if (SmallerSide_X_Z==3) return BSide3;
     else if (SmallerSide_X_Z==4) return BSide4;
     else if (SmallerSide_X_Z==5) return BSide5;
     else if (SmallerSide_X_Z==6) return BSide6;
     else if (SmallerSide_X_Z==7) return BSide7;
     else if (SmallerSide_X_Z==8) return BSide8;
     else if (SmallerSide_X_Z==9) return BSide9;
     else if (SmallerSide_X_Z==10) return BSide10;
     else {return NoPanel; }
  }
  else if(BackPanel == MuonExitingPanel){
      if(SmallerSide_X_Y==1) return BkSide1;
    else if (SmallerSide_X_Y==2) return BkSide2;
    else if (SmallerSide_X_Y==3) return BkSide3;
    else if (SmallerSide_X_Y==4) return BkSide4;
    else {return NoPanel; }
  }
  else if(FrontPanel == MuonExitingPanel){
      if(SmallerSide_X_Y==1) return FSide1;
    else if (SmallerSide_X_Y==2) return FSide2;
    else if (SmallerSide_X_Y==3) return FSide3;
    else if (SmallerSide_X_Y==4) return FSide4;
    else {return NoPanel; }
  }
 else {return NoPanel; }
 
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
MCSTools::MuonExitingPanel MCSTools::returnSINGLEPanel(double endtrack_x,double endtrack_y,double endtrack_z){
    int Outpanel = this->ClosestSide(endtrack_x, endtrack_y, endtrack_z); 
    int bin_Z_X =  this->PanelSingle_TB(endtrack_x,endtrack_z);
    int bin_Z_Y =  this->PanelSingle_LR(endtrack_y,endtrack_z);
    int bin_X_Y =  this->PanelSingle_FB(endtrack_x,endtrack_y);
          return this->returnSINGLEPanel(Outpanel, bin_Z_X, bin_Z_Y, bin_X_Y);
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
int MCSTools::returnSINGLEPanel_int(double endtrack_x,double endtrack_y,double endtrack_z){
  MuonExitingPanel Paneltype = this->returnSINGLEPanel(endtrack_x, endtrack_y, endtrack_z);
 int output_int = this->getEnumValue(Paneltype);
 return output_int;
}
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////