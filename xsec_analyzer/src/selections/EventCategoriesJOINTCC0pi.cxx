// ROOT includes
#include "TH1.h"



// XSecAnalyzer includes
#include "XSecAnalyzer/Selections/EventCategoriesJOINTCC0pi.hh"





std::map< int, std::pair< std::string, int > > JOINTCC0Pi_MAP = {

      { kUnknown, {"Unknown", kGray }},
      { kSignalCCQE, {"Signal (CCQE)" ,kGreen }},
      { kSignalCCMEC, {"Signal (CCMEC)",kGreen + 1  }},
      { kSignalCCRES, {"Signal (CCRES)",kGreen + 2  }},
      { kSignalOther, {"Signal (Other)",kGreen + 3  }},
      { kNuMuCCNpi, {"#nu_{#mu} CCN#pi",kAzure - 2 }},
      //{ kNuMuBarCC ,{ "#bar{#nu_{#mu}}-CC",kAzure - 3}},
      { kNuMuCCOther, {"Other #nu_{#mu} CC",kAzure }},
      { kNuECC, {"#nu_{e} CC",kViolet }},
      { kNC, {"NC",kOrange }},
      { kOOFV, {"Out FV",kRed + 3 }},
      { kOther, {"Other",kRed + 1 }}
  
  
};



MRDRecoResult ComputeMRDReco(
    double startx, double starty, double startz,
    double stopx,  double stopy,  double stopz,
    bool mrd_stop)
{
  MRDRecoResult result;

  result.trackLengthInMRD = sqrt(pow(stopx-startx,2) + pow(stopy-starty,2) + pow(stopz-startz,2));

  double diffx = stopx - startx;
  double diffy = stopy - starty;
  double diffz = stopz - startz;
  double startz_c = startz - 1.681;

  // Intersection with inner cylinder (radius = 1.0)
  double a = pow(diffx,2) + pow(diffz,2);
  double b = -2*diffx*startx - 2*diffz*startz_c;
  double c = pow(startx,2) + pow(startz_c,2) - 1.0*1.0;

  double t1 = (-b + sqrt(b*b - 4*a*c)) / (2*a);
  double t2 = (-b - sqrt(b*b - 4*a*c)) / (2*a);
  double t = 0;
  if      (t1 < 0) t = t2;
  else if (t2 < 0) t = t1;
  else             t = (t1 < t2) ? t1 : t2;

  result.exitx = startx - t*diffx;
  result.exity = starty - t*diffy;
  result.exitz = startz - t*diffz;

  double mag = sqrt(diffx*diffx + diffy*diffy + diffz*diffz);
  result.dirx = diffx / mag;
  result.diry = diffy / mag;
  result.dirz = diffz / mag;
  result.cosTheta = result.dirz;

  result.dist_mrd   = sqrt(pow(startx-stopx,2)          + pow(starty-stopy,2)          + pow(startz-stopz,2));
  result.dist_air   = sqrt(pow(startx-result.exitx,2)   + pow(starty-result.exity,2)   + pow(startz-result.exitz,2));
  result.dist_air_x = sqrt(pow(startx-result.exitx,2));
  result.dist_air_y = sqrt(pow(starty-result.exity,2));
  result.dist_air_z = sqrt(pow(startz-result.exitz,2));

  // Intersection with outer cylinder (radius = 1.5)
  double ap = pow(diffx,2) + pow(diffz,2);
  double bp = -2*diffx*startx - 2*diffz*startz_c;
  double cp = pow(startx,2) + pow(startz_c,2) - 1.5*1.5;

  double t1p = (-bp + sqrt(bp*bp - 4*ap*cp)) / (2*ap);
  double t2p = (-bp - sqrt(bp*bp - 4*ap*cp)) / (2*ap);
  double tp = 0;
  if      (t1p < 0) tp = t2p;
  else if (t2p < 0) tp = t1p;
  else              tp = (t1p < t2p) ? t1p : t2p;

  double exitxp = startx - tp*diffx;
  double exityp = starty - tp*diffy;
  double exitzp = startz - tp*diffz;

  double diff_exit_x = exitxp - result.exitx;
  double diff_exit_y = exityp - result.exity;
  double diff_exit_z = exitzp - result.exitz;

  result.dist_pmtvol_tank = sqrt(pow(diff_exit_x,2) + pow(diff_exit_y,2) + pow(diff_exit_z,2));

  // Check for stopping track
  if (!mrd_stop) {
    result.tank_exit = false;
    //std::cout<<"SimpleReconstruction: Event does not have a stopping track in the MRD! Abort reconstruction"<<std::endl;
    
  } else {
    result.tank_exit = true;
  }

  return result;
}
