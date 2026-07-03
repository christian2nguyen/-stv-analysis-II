// Post-processing program for the MicroBooNE xsec_analyzer framework. This is
// currently designed for use with the PeLEE group's "searchingfornues" ntuples
//
// Updated 24 September 2024
// Steven Gardiner <gardiner@fnal.gov>
// Daniel Barrow <daniel.barrow@physics.ox.ac.uk>

// Standard library includes
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <thread>

// ROOT includes
#include "TChain.h"
#include "TFile.h"
#include "TBranch.h"
#include "TParameter.h"
#include "TTree.h"
#include "TVector3.h"
#include "TROOT.h"
#include "TFileMerger.h"
#include "TMemFile.h"
#include "ROOT/RConfig.hxx"
#include "ROOT/TSeq.hxx"
#include "ROOT/TBufferMerger.hxx"
#include <functional>
#include <memory>
#include <mutex>
#include <queue>

std::mutex m;

// XSecAnalyzer includes
#include "XSecAnalyzer/AnalysisEvent.hh"
#include "XSecAnalyzer/Branches.hh"
#include "XSecAnalyzer/Constants.hh"
#include "XSecAnalyzer/Functions.hh"

#include "XSecAnalyzer/Selections/SelectionBase.hh"
#include "XSecAnalyzer/Selections/SelectionFactory.hh"

void analyze( const std::vector< std::string >& in_file_names,
  const std::vector< std::string >& selection_names,
  const std::string& output_filename )
{
  std::cout << "\nRunning ProcessNTuples with options:\n";
  std::cout << "\toutput_filename: " << output_filename << '\n';
  std::cout << "\tinput_file_names:\n";
  for ( size_t i = 0u; i < in_file_names.size(); ++i ) {
    std::cout << "\t\t- " << in_file_names[i] << '\n';
  }
  std::cout << "\n\nselection names:\n";
  for ( const auto& sel_name : selection_names ) {
    std::cout << "\t\t- " << sel_name << '\n';
  }

  ROOT::EnableThreadSafety();

  // Get the TTrees containing the event ntuples and subrun POT information
  // Use TChain objects for simplicity in manipulating multiple files
  TChain events_ch( "nuselection/NeutrinoSelectionFilter" );
  TChain subruns_ch( "nuselection/SubRun" );

  for ( const auto& f_name : in_file_names ) {
    events_ch.Add( f_name.c_str() );
    subruns_ch.Add( f_name.c_str() );
  }

  // OUTPUT TTREE
  // Make an output TTree for plotting (one entry per event)
  //std::unique_ptr<TFile> out_file(new TFile( output_filename.c_str(), "recreate" ));
  ROOT::TBufferMerger merger(output_filename.c_str(), "recreate");
  auto out_file = merger.GetFile();
  out_file->cd();
  //std::unique_ptr<TFile> uniquePtr_out_file(std::move(out_file));
 // ROOT::TBufferMerger merger(out_file);

  // Get the total POT from the subruns TTree. Save it in the output
  // TFile as a TParameter<float>. Real data doesn't have this TTree,
  // so check that it exists first.
  float pot;
  float summed_pot = 0.;
  bool has_pot_branch = ( subruns_ch.GetBranch("pot") != nullptr );
  if ( has_pot_branch ) {
    subruns_ch.SetBranchAddress( "pot", &pot );
    for ( int se = 0; se < subruns_ch.GetEntries(); ++se ) {
      subruns_ch.GetEntry( se );
      summed_pot += pot;
    }
  }
  TParameter<float>* summed_pot_param = new TParameter<float>( "summed_pot",
    summed_pot );

  summed_pot_param->Write();
  out_file->Write();

  const size_t nWorkers = 30;

  auto work_function = [&](int seed) {


  // Get the TTrees containing the event ntuples and subrun POT information
  // Use TChain objects for simplicity in manipulating multiple files
  TChain events_ch( "nuselection/NeutrinoSelectionFilter" );

  for ( const auto& f_name : in_file_names ) {
    events_ch.Add( f_name.c_str() );
  }


	  m.lock();

    std::cout << "Thread : " << seed << std::endl;

    auto f = merger.GetFile();
    f->cd();
    TTree* out_tree = new TTree( "stv_tree", "STV analysis tree" );

    std::vector< std::unique_ptr<SelectionBase> > selections;

    SelectionFactory sf;
    for ( const auto& sel_name : selection_names ) {
      selections.emplace_back().reset( sf.CreateSelection(sel_name) );
    }
    for ( auto& sel : selections ) {
      sel->setup( out_tree );
    }

    m.unlock();
    // EVENT LOOP
    // TChains can potentially be really big (and spread out over multiple
    // files). When that's the case, calling TChain::GetEntries() can be very
    // slow. I get around this by using a while loop instead of a for loop.
    int total_entries = events_ch.GetEntries();

    int entries_per_thread = (total_entries + nWorkers) / nWorkers;


    bool created_output_branches = false;
    long events_entry = 0;
    events_entry = seed * entries_per_thread;

    while ( true ) {

      if ( events_entry > ( (seed + 1) * entries_per_thread - 1 ) ) break;

      if ( events_entry % 1000 == 0 ) {
        std::cout << "Processing event #" << events_entry << '\n';
      }

      // Create a new AnalysisEvent object. This will reset all analysis
      // variables for the current event.
      AnalysisEvent cur_event;

      // Set branch addresses for the member variables that will be read
      // directly from the Event TTree.
      set_event_branch_addresses( events_ch, cur_event );

      // TChain::LoadTree() returns the entry number that should be used with
      // the current TTree object, which (together with the TBranch objects
      // that it owns) doesn't know about the other TTrees in the TChain.
      // If the return value is negative, there was an I/O error, or we've
      // attempted to read past the end of the TChain.
      int local_entry = events_ch.LoadTree( events_entry );

      // If we've reached the end of the TChain (or encountered an I/O error),
      // then terminate the event loop
      if ( local_entry < 0 ) break;

      // Load all of the branches for which we've called
      // TChain::SetBranchAddress() above
      events_ch.GetEntry( events_entry );

      // Set the output TTree branch addresses, creating the branches if needed
      // (during the first event loop iteration)
      bool create_them = false;
      if ( !created_output_branches ) {
        create_them = true;
        created_output_branches = true;
      }
      set_event_output_branch_addresses(*out_tree, cur_event, create_them );

      for ( auto& sel : selections ) {
        sel->apply_selection( &cur_event );
      }

      // We're done. Save the results and move on to the next event.
      out_tree->Fill();
      ++events_entry;
    }

	  m.lock();
    for ( auto& sel : selections ) {
      sel->summary();
    }
    std::cout << "Wrote output to:" << output_filename << std::endl;

    for ( auto& sel : selections ) {
      sel->final_tasks();
    }
	  m.unlock();

    f->Write();
  };


  // Create worker threads
  std::vector<std::thread> workers;

  for (auto i : ROOT::TSeqI(nWorkers))
    workers.emplace_back(work_function, i); // seed==0 means random seed :)
  // Make sure workers are done
  for (auto &&worker : workers)
    worker.join();

  //out_file->Close();
 // delete out_file;
}

void analyzer( const std::string& in_file_name,
    const std::vector< std::string > selection_names,
    const std::string& output_filename)
{
  std::vector< std::string > in_files = { in_file_name };
  analyze( in_files, selection_names, output_filename );
}

int main( int argc, char* argv[] ) {

  if ( argc != 4 ) {
    std::cout << "Usage: " << argv[0]
      << " INPUT_PELEE_NTUPLE_FILE SELECTION_NAMES OUTPUT_FILE\n";
    return 1;
  }

  std::string input_file_name( argv[1] );
  std::string output_file_name( argv[3] );

  std::vector< std::string > selection_names;

  std::stringstream sel_ss( argv[2] );
  std::string sel_name;
  while ( std::getline(sel_ss, sel_name, ',') ) {
    selection_names.push_back( sel_name );
  }

  analyzer( input_file_name, selection_names, output_file_name );

  return 0;
}
