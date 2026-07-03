// XSecAnalyzer includes
 #include "XSecAnalyzer/FiducialVolume.hh"



bool FidVol_ANNIE(double x, double y, double z){
        double radius   = 100.;  //cm
        double y_min    = -100.; //cm
        double y_max    = 100.;  //cm
        double z_center = 168.1; //cm
        double y_offset = 14.46; //cm
        if(y+y_offset > y_min && y+y_offset < y_max && radius > std::sqrt((z - z_center)*(z - z_center) + x*x)){
                return true;
        }
        else {
                return false;
        }
}

bool FidVol_ANNIE_mm(double x, double y, double z){
        double radius   = 1000.;  //mm
        double y_min    = -1000.; //mm
        double y_max    = 1000.;  //mm
        double z_center = 1681; //mm
        double y_offset = 144.6; //mm
        if(y+y_offset > y_min && y+y_offset < y_max && radius > std::sqrt((z - z_center)*(z - z_center) + x*x)){
                return true;
        }
        else {
                return false;
        }
}
