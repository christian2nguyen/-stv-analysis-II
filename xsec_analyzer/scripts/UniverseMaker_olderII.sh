# Number of expected command-line arguments
num_expected=3

if [ "$#" -ne "$num_expected" ]; then
  echo "Usage: UniverseMaker.sh file_properties_CONFIG Binning_CONFIG OUTPUT_ROOT_FILE"
  exit 1
fi

FPM_CONFIG=$1
BIN_CONFIG=$2
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
univmake ${FPM_CONFIG} ${BIN_CONFIG} ${output_dir}/${OUTPUT_ROOT_FILE} ${FPM_CONFIG}
