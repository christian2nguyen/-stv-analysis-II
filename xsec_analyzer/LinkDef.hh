// ROOT needs these dictionary definitions to be able to manipulate TTree
// branches with specific types, such as a std::vector of TVector3 objects
#ifdef __ROOTCLING__

#include "TVector3.h"
#include "TLorentzVector.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

#pragma link C++ class std::vector<TVector3 >+;
#pragma link C++ class std::vector<TLorentzVector >+;
#pragma link C++ class std::vector<int >+;
#pragma link C++ class std::vector<float >+;
#pragma link C++ class std::vector<double >+;
#pragma link C++ class std::vector<std::vector<float> >+;
#pragma link C++ class std::string+;

#pragma link C++ class std::pair<const std::string, std::vector<double> >+;
#pragma link C++ class std::map<std::string, std::vector<double> >+;

#pragma link C++ class std::pair<const std::string, std::vector<double>* >+;
#pragma link C++ class std::map<std::string, std::vector<double>* >+;


#endif