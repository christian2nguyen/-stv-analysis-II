#pragma once

// XSecAnalyzer includes
#include "XSecAnalyzer/Selections/SelectionBase.hh"

class DummySelection : public SelectionBase {

public:

  DummySelection();

  virtual int categorize_event( AnalysisEvent* event ) override final;
  virtual bool selection( AnalysisEvent* event ) override final;
  virtual bool define_signal( AnalysisEvent* event ) override final;
  virtual void compute_reco_observables( AnalysisEvent* event ) override final;
  virtual void compute_true_observables( AnalysisEvent* event ) override final;
  virtual void define_output_branches() override final;
  virtual void define_constants() override final;
  virtual void define_category_map() override final;
  virtual void reset() override final;
  virtual void define_additional_input_branches(TTree& etree) override final;
  virtual int categorize_event( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual bool selection( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual bool define_signal( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual void compute_reco_observables( AnalysisANNIE_Event_nuwro* event ) override final;
  virtual void compute_true_observables( AnalysisANNIE_Event_nuwro* event ) override final;

  //virtual int categorize_event( AnalysisANNIE_Event* event ) override final;
  //virtual bool selection( AnalysisANNIE_Event* event ) override final;
  //virtual bool define_signal( AnalysisANNIE_Event* event ) override final;
  //virtual void compute_reco_observables( AnalysisANNIE_Event* event ) override final;
  //virtual void compute_true_observables( AnalysisANNIE_Event* event ) override final;

  // Implementing pure virtual functions
    int categorize_event(AnalysisANNIE_Event* event) override final {
        // Do nothing
        return 0;  // Return a default integer value
    }

    bool selection(AnalysisANNIE_Event* event) override final {
        // Do nothing
        return false;  // Return a default boolean value
    }

    bool define_signal(AnalysisANNIE_Event* event) override final {
        // Do nothing
        return false;  // Return a default boolean value
    }

    void compute_reco_observables(AnalysisANNIE_Event* event) override final {
        // Do nothing
    }

    void compute_true_observables(AnalysisANNIE_Event* event) override final {
        // Do nothing
    }

  

};
