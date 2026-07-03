// Executable for generating systematic universe files for later analysis. It
// has been adapted from a similar ROOT macro.

// Standard library includes
#include <stdexcept>

// Gnu Portability library (Gnulib) includes
#include <getopt.h>

// ROOT includes
#include "TBranch.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

// XSecAnalyzer includes
#include "XSecAnalyzer/FilePropertiesManager.hh"
#include "XSecAnalyzer/MCC9SystematicsCalculator.hh"
#include "XSecAnalyzer/UniverseMaker.hh"

static int verbose_flag;

// Helper function that checks whether a given ROOT file represents an ntuple
// from a reweightable MC sample. This is done by checking for the presence of
// a branch whose name matches the TUNE_WEIGHT_NAME string defined in
// UniverseMaker.hh. Central-value GENIE MC samples are expected to have
// this branch. Real data, detector variation systematics samples and MC
// samples prepared using alternative generators (e.g., NuWro) are not expected
// to have this branch.
bool is_reweightable_mc_ntuple( const std::string& input_file_name ) {
  TFile temp_file( input_file_name.c_str(), "read" );
  TTree* stv_tree = nullptr;
  temp_file.GetObject( "stv_tree", stv_tree );
  if ( !stv_tree ) throw std::runtime_error( "Missing TTree \"stv_tree\" in"
    " the input ROOT file " + input_file_name );

  TBranch* cv_weight_br = stv_tree->GetBranch( TUNE_WEIGHT_NAME.c_str() );
  bool has_cv_weights = ( cv_weight_br != nullptr );
  return has_cv_weights;
}

int main( int argc, char* argv[] ) {

  int c;

  std::string list_file_name;
  std::string univmake_config_file_name;
  std::string output_file_name;
  std::vector<int> index_parallel;
  int iparallel = 0, nparallel = 1;

  while ( true ) {

    static struct option long_options[] =
    {
      // These options set a flag
      {"files", required_argument, 0, 'f'},
      {"bin", required_argument, 0, 'b'},
      {"output", required_argument, 0, 'o'},
      {"parallel", required_argument, 0, 'p'},
      {"help", required_argument, 0, 'h'},

      {0, 0, 0, 0}
    };
    // getopt_long stores the option index here
    int option_index = 0;

    c = getopt_long( argc, argv, "f:b:o:p:h", long_options, &option_index );

    if( c == -1 ) break;

    // Detect the end of the options
    switch ( c )
    {
      case 'f':
        // file properties list file name and type
        list_file_name = std::string(optarg);
        break;
      case 'b':
        // binning configuration
        univmake_config_file_name = std::string(optarg);
        break;
      case 'o':
        // output univmake files
        output_file_name = std::string(optarg);
        break;
      case 'p':
        // a option to run the unimake in parallel
        index_parallel.push_back(atoi(optarg));
        while (optind < argc && argv[optind][0] != '-'){
          index_parallel.push_back(atoi(argv[optind]));
          optind++;
        }
        break;
      case 'h':
      case '?':
      default:
        std::cout << "Usage: \n";
        std::cout << "Options: \n";
        std::cout << "    -f, --files; File properties."
          << " list the input files \n";
        std::cout << "    -b, --bin;   Binning configuration"
          << " \n";
        std::cout << "    -o, --output;   Output of the univmake \n";
        std::cout << "    -p, --parallel;   Run univmake in parallel \n";
        std::cout << "    -h, --help;   Print this help information. \n";
        return 0;
    }

  } // option parsing loop

  // Instead of reporting '--verbose' and '--brief' as they are encountered,
  // we report the final status resulting from them
  if ( verbose_flag ) puts( "verbose flag is set" );

  // Print any remaining command line arguments (not options)
  bool set_bin_scheme_name = false;
  std::string bin_scheme_name;

  if ( optind < argc ) {
    printf( "non-option ARGV-elements: " );
    while ( optind < argc ) {
      if ( !set_bin_scheme_name ) {
        bin_scheme_name = argv[ optind ];
        set_bin_scheme_name = true;
      }
      printf( "%s ", argv[optind++] );
    }
    putchar( '\n' );
  }
  if ( argc <= 1 ) {
    std::cout << "Usage: \n";
    std::cout << "Options: \n";
    std::cout << "    -f, --files; File properties."
      << " list the input files \n";
    std::cout << "    -b, --bin;   Binning configuration"
      << " \n";
    std::cout << "    -o, --output;   Output of the univmake \n";
    std::cout << "    -p, --parallel;   Run univmake in parallel \n";
    std::cout << "    -h, --help;   Print this help information. \n";
    abort();
  }


  std::cout << "\nRunning univmake.C with options:\n";
  std::cout << "\tlist_file_name: " << list_file_name << '\n';
  std::cout << "\tunivmake_config_file_name: "
    << univmake_config_file_name << '\n';
  std::cout << "\toutput_file_name: " << output_file_name << '\n';

  if(index_parallel.size() == 2 && index_parallel[1] > index_parallel[0] && index_parallel[1] > 0){
    iparallel = index_parallel[0];
    nparallel = index_parallel[1];
    std::cout << "\tparallel index: " << iparallel << "  " << nparallel << "\n";
  }
  else{
    std::cerr << "error setup of parallel \n";
    throw;
  }

  // Simultaneously check that we can write to the output file directory, and wipe any information within that file
  TFile* temp_file = new TFile(output_file_name.c_str(), "recreate");
  if (!temp_file || temp_file->IsZombie()) {
    std::cerr << "Could not write to output file: "
      << output_file_name << '\n';
    throw;
  }
  delete temp_file;

  // If the user specified an (optional) non-default configuration file for the
  // FilePropertiesManager on the command line, then load it here. Note that the
  // only place where the FilePropertiesManager configuration is relevant is in
  // the use of MCC9SystematicsCalculator to compute total event count
  // histograms (see below).
  auto& fpm = FilePropertiesManager::Instance();
  if ( argc == 5 ) {
    std::cout << "\tfile_properties_name: " << argv[4] << '\n';
    fpm.load_file_properties( argv[4] );
  }

  // Regardless of whether the default was used or not, retrieve the
  // name of the FilePropertiesManager configuration file that was
  // actually used
  std::string fp_config_file_name = fpm.config_file_name();
  std::cout << "\nLoaded FilePropertiesManager configuration from: "
	    << fp_config_file_name << '\n';

  // Read in the complete list of input ntuple files that should be processed
  std::ifstream in_file( list_file_name );
  std::vector< std::string > input_files;
  std::string temp_line;
  while ( std::getline(in_file, temp_line) ) {
    // Ignore lines that begin with the '#' character (this allows for
    // comments in the normalization table file
    if ( temp_line.front() == '#' ) continue;

    // Read in the ntuple file name from the beginning of the current line of
    // the list file. Any trailing line contents separated from the name by
    // whitespace will be ignored.
    std::string file_name;
    std::istringstream temp_ss( temp_line );
    temp_ss >> file_name;

    input_files.push_back( file_name );
  }

  std::cout << "Processing systematic universes for a total of "
	    << input_files.size() << " input ntuple files\n";

  //ROOT::EnableImplicitMT();

  // Store the name of the root TDirectoryFile created by the UniverseMaker
  // objects below. We will use it to ensure that the MCC9SystematicsCalculator
  // object used to calculate the total event counts will always be working with
  // the correct sets of universes.
  std::string tdirfile_name;
  bool set_tdirfile_name = false;

  std::cout << "\nCalculating systematic universes for ntuple input file:\n";

  int counter = 0;
  for ( const auto& input_file_name : input_files ) {
    std::cout << '\t' << counter << '/' << input_files.size() << " - "
      << input_file_name << '\n';
    std::cout << fpm.ntuple_type_to_string(fpm.get_ntuple_file_type(input_file_name)) << std::endl;
    std::cout << "has event weight: " << ntuple_type_is_reweightable_mc(fpm.get_ntuple_file_type(input_file_name)) << std::endl;

    UniverseMaker univ_maker( univmake_config_file_name );

    univ_maker.add_input_file( input_file_name.c_str() );
    univ_maker.setup_parallel(iparallel, nparallel);

    //univ_maker.build_universes();
    //univ_maker.build_universes_memory_efficient(&output_file_name, &input_file_name);



    //bool has_event_weights = is_reweightable_mc_ntuple( input_file_name );
    bool has_event_weights = ntuple_type_is_reweightable_mc(fpm.get_ntuple_file_type(input_file_name));

    if ( has_event_weights ) {
      // If the check above was successful, then run all of the histogram
      // calculations in the usual way
      // univ_maker.build_universes();
      univ_maker.build_universes_memory_efficient(&output_file_name, &input_file_name);
    }
    else {
      // Passing in the fake list of explicit branch names below instructs
      // the UniverseMaker class to ignore all event weights while
      // processing the current ntuple
      univ_maker.build_universes( { "FAKE_BRANCH_NAME" } );
      univ_maker.save_histograms( output_file_name, input_file_name );
    }


    // The root TDirectoryFile name is the same across all iterations of this
    // loop, so just set it once on the first iteration
    if ( !set_tdirfile_name ) {
      tdirfile_name = univ_maker.dir_name();
      set_tdirfile_name = true;
    }

    counter += 1;
  } // loop over input files


  // Use a temporary MCC9SystematicsCalculator object to automatically calculate
  // the total event counts in each universe across all input files. Since the
  // get_covariances() member function is never called, the specific systematics
  // configuration file used doesn't matter. The empty string passed as the
  // second argument to the constructor just instructs the
  // MCC9SystematicsCalculator class to use the default systematics
  // configuration file.

  // std::cout << "\nCalculating total event counts using all input files:\n";
  // MCC9SystematicsCalculator unfolder( output_file_name, "", tdirfile_name );
  std::cout << "Completing the creation of the universe!" << std::endl;

  return 0;
}
