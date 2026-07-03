#!/bin/sh

#################################
# userSetup_template.sh
#--------------------------------
#  source this script and it will:
#  setup the minerva environment For MacroAnalysis for Helium
#  set up Root 6 and Cmake 
#--------------------------------
# Christian Nguyen  - Oct 29, 2024
#################################


###########################################
# You can pick the setup script and tag
###########################################
setupscript="/cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh"   #This source is to  setup MicroBooNE Tu  
tag=""                                                        
debug=""                                                                
WorkingArea="/exp/uboone/app/users/cnguyen/"

#sourceArea="/exp/uboone/app/users/apapadop/BuildEventGenerators/"

source /cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh
#cd $sourceArea
#source setup_generators.sh
cd $WorkingArea


#setup uboonecode v08_00_00_52 -q e17:prof 
#old release , newest relase as of Nov 29 2023
setup uboonecode v08_00_00_84 -q e17:prof

THIS_DIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export STV_ANALYSIS_DIR=${THIS_DIRECTORY}
export PATH=${PATH}:${STV_ANALYSIS_DIR}
export LD_LIBRARY_PATH=/exp/uboone/app/users/mastbaum/xgboost/build/lib64:$LD_LIBRARY_PATH


#setting up proxy for grid submission
kx509
cigetcert --institution 'Fermi National Accelerator Laboratory'
voms-proxy-init -rfc --voms=fermilab:/fermilab/minerva/Role=Analysis --noregen -valid 24:0
