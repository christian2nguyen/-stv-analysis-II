#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_WCBinScheme : public BinSchemeBase {

  public:

    JOINTCC0Pi_WCBinScheme();
    virtual void DefineBlocks() override;
};
