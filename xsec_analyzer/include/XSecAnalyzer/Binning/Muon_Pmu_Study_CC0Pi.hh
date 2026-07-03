#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class Muon_Pmu_Study_CC0Pi : public BinSchemeBase {

  public:

    Muon_Pmu_Study_CC0Pi();
    virtual void DefineBlocks() override;
};
