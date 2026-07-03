// Standard library includes
#include <iostream>

// XSecAnalyzer includes
#include "XSecAnalyzer/Functions.hh"
#include "XSecAnalyzer/Selections/SelectionBase.hh"
bool Debug = false; 


SelectionBase::SelectionBase( const std::string& sel_name ) {

  selection_name_ = sel_name;
  num_passed_events_ = 0;

  event_number_ = 0;

  fv_true_ = { BOGUS, BOGUS, BOGUS, BOGUS, BOGUS, BOGUS };
  fv_reco_ = { BOGUS, BOGUS, BOGUS, BOGUS, BOGUS, BOGUS };

}

void SelectionBase::setup( TTree* out_tree, bool create_branches ) {

  out_tree_ = out_tree;
  need_to_create_branches_ = create_branches;
  this->setup_tree();
  this->define_category_map();
  this->define_constants();

}

void SelectionBase::apply_selection( AnalysisEvent* event ) {
  this->reset_base();
  this->reset();




  mc_signal_ = this->define_signal( event );
  selected_ = this->selection( event );
  event_category_ = this->categorize_event( event );
  
   

  this->compute_reco_observables( event );
  // Note that event->is_mc_ is set in CategorizeEvent() above
  if ( event->is_mc_ ) {
    this->compute_true_observables( event );
  }


  if ( selected_ ) {
    ++num_passed_events_;
  }

  ++event_number_;
}
////////////////////////////////////////////////////////////////////
void SelectionBase::apply_selection( AnalysisANNIE_Event* event ) {
  this->reset_base();
  this->reset();
  
  
  
  if(Debug)std::cout<<"~~~~~~~~~Start::apply_selection~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  if(Debug)std::cout<<"~~~~~~~~~Starting define_signal~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  mc_signal_ = this->define_signal( event );
  if(Debug)std::cout<<"~~~~~~~~~Starting selection~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  selected_ = this->selection( event );
  if(Debug)std::cout<<"~~~~~~~~~~Starting categorize_event ~~~~~~~~~~~~~~~~~~~"<<std::endl;
  event_category_ = this->categorize_event( event );
  if(Debug)std::cout<<"~~~~~~~~~~ event_category_ =" << event_category_ <<std::endl;


  if(mc_signal_==false && (event_category_ == 1 || event_category_ == 2 || event_category_ == 3 || event_category_ == 4 || event_category_ == 9999)){
  std::cout<<" I Found the bug WHYYYYY ,mc_signal_==false but with Signal label"<< std::endl;
  
  
  }

  else if(mc_signal_==true && (event_category_ == 0 ||event_category_ == 5 || event_category_ == 7 || event_category_ == 8 || event_category_ == 9 ||event_category_ == 10 ||event_category_ == 11 || event_category_ == 9999)){
    std::cout<<" I Found the bug WHYYYYY ,mc_signal_==true but with BG label"<< std::endl;

  
  }
  this->compute_reco_observables( event );
  // Note that event->is_mc_ is set in CategorizeEvent() above
  if ( event->is_mc_ ) {
    this->compute_true_observables( event );
    
  }



  if(Debug)std::cout<<"~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

 if (Debug&&mc_signal_){std::cout<<"~~~~~We Have a true Sigal event~~~"<<std::endl;}

  if ( selected_ ) {
    ++num_passed_events_;
  }

  ++event_number_;
}
////////////////////////////////////////////////////////////////////
void SelectionBase::apply_selection( AnalysisANNIE_Event_nuwro* event ) {
  this->reset_base();
  this->reset();
  
  
  
  if(Debug)std::cout<<"~~~~~~~~~Start::apply_selection~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  if(Debug)std::cout<<"~~~~~~~~~Starting define_signal~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  mc_signal_ = this->define_signal( event );
  if(Debug)std::cout<<"~~~~~~~~~Starting selection~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
  selected_ = this->selection( event );
  if(Debug)std::cout<<"~~~~~~~~~~Starting categorize_event ~~~~~~~~~~~~~~~~~~~"<<std::endl;
  event_category_ = this->categorize_event( event );
  if(Debug)std::cout<<"~~~~~~~~~~ event_category_ =" << event_category_ <<std::endl;


  if(mc_signal_==false && (event_category_ == 1 || event_category_ == 2 || event_category_ == 3 || event_category_ == 4 || event_category_ == 9999)){
  std::cout<<" I Found the bug WHYYYYY"<< std::endl;
  
  
  }

  this->compute_reco_observables( event );
  // Note that event->is_mc_ is set in CategorizeEvent() above
  if ( event->is_mc_ ) {
    this->compute_true_observables( event );
    
  }



  if(Debug)std::cout<<"~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

 if (Debug&&mc_signal_){std::cout<<"~~~~~We Have a true Sigal event~~~"<<std::endl;}

  if ( selected_ ) {
    ++num_passed_events_;
  }

  ++event_number_;
}
////////////////////////////////////////////////////////////////////


void SelectionBase::summary() {
  std::cout << selection_name_ << " has " << num_passed_events_
    << " events which passed\n";
}

void SelectionBase::setup_tree() {

  //this->set_branch_nolabel( &selected_, "Selected" );
  //this->set_branch_nolabel( &mc_signal_, "MC_Signal" );
  //
  // std::string BranchName = selection_name_ +"EventCategory"; //using old naming scheme
  // //std::string BranchName = "category";
  // this->set_branch_nolabel( &event_category_, BranchName );

  this->set_branch( &selected_, "Selected" );
  this->set_branch( &mc_signal_, "MC_Signal" );
  this->set_branch( &event_category_, "EventCategory" );
  //this->set_branch( &event_category_, "category" );
  //this->define_additional_input_branches();
  this->define_output_branches();

}

void SelectionBase::reset_base() {
  selected_ = false;
  mc_signal_ = false;
  event_category_ = BOGUS_INDEX;
}




