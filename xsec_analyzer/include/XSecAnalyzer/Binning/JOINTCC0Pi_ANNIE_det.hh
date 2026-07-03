#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_ANNIE_det : public BinSchemeBase {

  public:

    JOINTCC0Pi_ANNIE_det();
    virtual void DefineBlocks() override;
};
