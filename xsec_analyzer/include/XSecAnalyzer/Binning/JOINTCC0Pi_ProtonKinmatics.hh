#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Binning/BinSchemeBase.hh"

class JOINTCC0Pi_ProtonKinmatics : public BinSchemeBase {

  public:

    JOINTCC0Pi_ProtonKinmatics();
    virtual void DefineBlocks() override;
};
