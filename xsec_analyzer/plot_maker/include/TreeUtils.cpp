#include "TreeUtils.hh"

void set_output_branch_address( TTree& out_tree, const std::string& branch_name,
  void* address, bool create, const std::string& leaf_spec )
{
  if ( create ) {
    if ( leaf_spec != "" ) {
      out_tree.Branch( branch_name.c_str(), address, leaf_spec.c_str() );
    }
    else {
      out_tree.Branch( branch_name.c_str(), address );
    }
  }
  else {
    out_tree.SetBranchAddress( branch_name.c_str(), address );
  }
}
