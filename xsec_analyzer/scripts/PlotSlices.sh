# Number of expected command-line arguments
num_expected=5

if [ "$#" -ne "$num_expected" ]; then
  echo "Usage: ./PlotSlices.sh FPM_Config SYST_Config SLICE_Config Univ_File PlotOutputDir"
#  exit 1
fi

run=${1}
FPM_Config=${XSEC_ANALYZER_DIR}/configs/file_properties_fsi_current_${run}.txt
SYST_Config=${XSEC_ANALYZER_DIR}/configs/systcalc.conf
SLICE_Config=${XSEC_ANALYZER_DIR}/configs/ccxp0pi_slice_config.txt
Univ_File="/exp/uboone/data/users/liangliu/workarea/fsi/univmake/univmake.root"
PlotOutputDir=${XSEC_ANALYZER_DIR}/scripts/${run}

if [ ! -f "${FPM_Config}" ]; then
  echo "FPM_Config \"${FPM_Config}\" not found"
  exit 1
fi

if [ ! -f "${SYST_Config}" ]; then
  echo "SYST_Config \"${SYST_Config}\" not found"
  exit 2
fi

if [ ! -f "${SLICE_Config}" ]; then
  echo "SLICE_Config \"${SLICE_Config}\" not found"
  exit 3
fi

if [ ! -f "${Univ_File}" ]; then
  echo "Universe File \"${Univ_File}\" not found"
  exit 4
fi

if [ ! -d "${PlotOutputDir}" ]; then
  echo "Output directory \"${PlotOutputDir}\" not found"
  exit 5
fi

SlicePlots ${FPM_Config} ${SYST_Config} ${SLICE_Config} ${Univ_File} ${PlotOutputDir}
