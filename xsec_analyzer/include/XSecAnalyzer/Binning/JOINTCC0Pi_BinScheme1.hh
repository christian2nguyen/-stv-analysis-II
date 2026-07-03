#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_BinScheme1 : public BinSchemeBase {

  public:

    JOINTCC0Pi_BinScheme1();
    virtual void DefineBlocks() override;
};
