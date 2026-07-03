# Number of expected command-line arguments
num_expected=3

if [ "$#" -ne "$num_expected" ]; then
  echo "Usage: UniverseMaker.sh file_properties_CONFIG Binning_CONFIG OUTPUT_ROOT_FILE"
  exit 1
fi

input_dir=/exp/uboone/app/users/cnguyen/stv-analysis-II/xsec_analyzer/configs
FPM_CONFIG=${input_dir}/$1
BIN_CONFIG=${input_dir}/$2
OUTPUT_ROOT_FILE=$3
output_dir=/exp/uboone/data/users/cnguyen/CC0Pi_Selection/EventSelection_12_21_2024

if [ ! -f "$FPM_CONFIG" ]; then
  echo "FPM_CONFIG \"${FPM_CONFIG}\" not found"
  exit 1
fi

if [ ! -f "$BIN_CONFIG" ]; then
  echo "BIN_CONFIG \"${BIN_CONFIG}\" not found"
  exit 1
fi
univmake -f ${FPM_CONFIG} -b ${BIN_CONFIG} -o ${output_dir}/${OUTPUT_ROOT_FILE}
