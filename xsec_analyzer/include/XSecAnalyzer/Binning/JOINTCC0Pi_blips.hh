#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_blips : public BinSchemeBase {

  public:

    JOINTCC0Pi_blips();
    virtual void DefineBlocks() override;
};
