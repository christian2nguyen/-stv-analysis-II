///////////////////////////
////Author:Christiain Nguyen 
//mcs tools for anaylzing muon tracks 
//date Nov 22 
///////////////////////
#pragma once

#include <iostream> // Example: Include standard libraries
#include <string>   // Example: Include string if needed
#include "UBTH2Poly.hh"




// Documentation
/**
 * @class MCSTools
 * @brief Brief description of the class.
 *
 * Detailed description of the class, its purpose, and usage.
 */
class MCSTools {

public:
    
    
    ///Constants I got from 
    
    static constexpr double volactivex = 256.35; //cm 
    static constexpr double volactivey = 233.0; //cm 
    static constexpr double volactivez = 1036.8;//cm 
     
    static constexpr double FV_X_MIN =   11.5;
    static constexpr double FV_X_MAX =  244.85;

    static constexpr double FV_Y_MIN = -105.0;
    static constexpr double FV_Y_MAX =  105.0;

    static constexpr double FV_Z_MIN =   10.5;
    static constexpr double FV_Z_MAX =  1026.8;
     
    static constexpr double TPC_limit_X_MIN =   0.0;
    static constexpr double TPC_limit_X_MAX =  250.0;

    static constexpr double TPC_limit_Y_MIN = -125.0;
    static constexpr double TPC_limit_Y_MAX =  125.0;

    static constexpr double TPC_limit_Z_MIN =   0.0;
    static constexpr double TPC_limit_Z_MAX =  1050.0;
    
    enum MuonExitingPanel{
// Special case
    NoPanel,

    // Primary panels
    LeftPanel,
    RightPanel,
    BottomPanel,
    TopPanel,
    FrontPanel,
    BackPanel,

    // RSide panels
    RSide1, RSide2, RSide3, RSide4, RSide5,
    RSide6, RSide7, RSide8, RSide9, RSide10,

    // LSide panels
    LSide1, LSide2, LSide3, LSide4, LSide5,
    LSide6, LSide7, LSide8, LSide9, LSide10,

    // TSide panels
    TSide1, TSide2, TSide3, TSide4, TSide5,
    TSide6, TSide7, TSide8, TSide9, TSide10,

    // BSide panels
    BSide1, BSide2, BSide3, BSide4, BSide5,
    BSide6, BSide7, BSide8, BSide9, BSide10,

    // BkSide panels
    BkSide1, BkSide2, BkSide3, BkSide4,

    // FSide panels
    FSide1, FSide2, FSide3, FSide4
};

    
    
    
    // Constructors and Destructor
    //MCSTools(double FV_X_min, double FV_X_max, double FV_Y_min, double FV_Y_max, double FV_Z_min, double FV_Z_max);                       // Default constructor
    MCSTools(double FV_X_min, double FV_X_max, double FV_Y_min, double FV_Y_max, double FV_Z_min, double FV_Z_max) :
       FV_X_range_low_(FV_X_min),
       FV_X_range_high_(FV_X_max),
       FV_Y_range_low_(FV_Y_min),
       FV_Y_range_high_(FV_Y_max),
       FV_Z_range_low_(FV_Z_min),
       FV_Z_range_high_(FV_Z_max){

       this->InitializePanelMaps();
     
     h_TBsides_  = new UBTH2Poly("h_TBsides_", "h_TBsides_",  SideSpaceing_Z_X, false);
     h_LRsides_  = new UBTH2Poly("h_LRsides_", "h_LRsides_",  SideSpaceing_Z_Y, false);
     h_FBsides_  = new UBTH2Poly("h_FBsides_", "h_FBsides_",  SideSpaceing_X_Y, false);
     
     
     
  
    }
    
    ~MCSTools(){std::cout<<"Destuctor MCSTools "<< std::endl;};                      // Destructor

    // Accessors (Getters)


    // Mutators (Setters)
    void SetFV(double FV_X_min,double FV_X_max, double FV_Y_min, double FV_Y_max, double FV_Z_min, double FV_Z_max){
       FV_X_range_low_ = FV_X_min;
       FV_X_range_high_ = FV_X_max;
       FV_Y_range_low_ = FV_Y_min;
       FV_Y_range_high_ = FV_Y_max;
       FV_Z_range_low_ = FV_Z_min;
       FV_Z_range_high_ = FV_Z_max;
       FVSet_= true; 
       std::cout<<"Set FV in MCS tool"<< std::endl;
    };
    
    // Other Public Methods
    Int_t ClosestSide(double x, double y, double z); 
    Int_t PanelSingle_TB(double vectex_X,double vectex_Z)const ;
    Int_t PanelSingle_LR(double vectex_Y,double vectex_Z)const ;
    Int_t PanelSingle_FB(double vectex_X,double vectex_Y)const ;
    int FindMinimum(double a, double b, double c, double d, double e, double f);
    void InitializePanelMaps();
    
    MuonExitingPanel returnSINGLEPanel(int  MuonExitingPanel, int SmallerSide_X_Z,
                                        int SmallerSide_Y_Z, int SmallerSide_X_Y);
   MuonExitingPanel returnSINGLEPanel(double endtrack_x,double endtrack_y,double endtrack_z);
    int returnSINGLEPanel_int(double endtrack_x, double endtrack_y, double endtrack_z);
    int getEnumValue(MuonExitingPanel panel) {
                           return static_cast<int>(panel); // Explicitly cast to int
                    }
private:
    // Member Variables
 
    bool FVSet_ = false; 
    double FV_X_range_low_;
    double FV_X_range_high_;
    
    double FV_Y_range_low_;
    double FV_Y_range_high_;
    
    double FV_Z_range_low_;
    double FV_Z_range_high_;

    UBTH2Poly* h_TBsides_= nullptr;  
    UBTH2Poly* h_LRsides_= nullptr; 
    UBTH2Poly* h_FBsides_= nullptr;
    
    std::map< double, std::vector<double> > SideSpaceing_Z_Y;
    std::map< double, std::vector<double> > SideSpaceing_Z_X;
    std::map< double, std::vector<double> > SideSpaceing_X_Y;




};

