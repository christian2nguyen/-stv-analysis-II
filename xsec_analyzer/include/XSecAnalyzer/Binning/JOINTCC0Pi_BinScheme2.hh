#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_BinScheme2 : public BinSchemeBase {

  public:

    JOINTCC0Pi_BinScheme2();
    virtual void DefineBlocks() override;
};
