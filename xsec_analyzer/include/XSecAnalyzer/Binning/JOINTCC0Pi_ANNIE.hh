#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_ANNIE : public BinSchemeBase {

  public:

    JOINTCC0Pi_ANNIE();
    virtual void DefineBlocks() override;
};
