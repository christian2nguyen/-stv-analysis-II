#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_Bin2DCombined : public BinSchemeBase {

  public:

    JOINTCC0Pi_Bin2DCombined();
    virtual void DefineBlocks() override;
};
