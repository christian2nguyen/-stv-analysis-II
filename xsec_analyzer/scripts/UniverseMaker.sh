# Number of expected command-line arguments
num_expected=3

if [ "$#" -ne "$num_expected" ]; then
  echo "Usage: UniverseMaker.sh FPM_CONFIG SEL_CONFIG OUTPUT_ROOT_FILE"
#  exit 1
fi

FPM_CONFIG=${1}
BIN_CONFIG=${2}
OUTPUT_ROOT_FILE=${3}
PARALLEL=${4}

if [ ! -f "$FPM_CONFIG" ]; then
  echo "FPM_CONFIG \"${FPM_CONFIG}\" not found"
  exit 1
fi

if [ ! -f "$BIN_CONFIG" ]; then
  echo "BIN_CONFIG \"${BIN_CONFIG}\" not found"
  exit 1
fi

if [ x${PARALLEL} = x ]; then
  echo "time univmake -f ${FPM_CONFIG} -b ${BIN_CONFIG} -o ${OUTPUT_ROOT_FILE} -p 0 1"
#  time univmake -f ${FPM_CONFIG} -b ${BIN_CONFIG} -o ${OUTPUT_ROOT_FILE} -p 0 1
else
  outut_path=${OUTPUT_ROOT_FILE%/*}
  file_output=`basename ${OUTPUT_ROOT_FILE}`
  for i in $(seq 0 `expr ${PARALLEL} - 1`)
  do
    echo "time univmake -f ${FPM_CONFIG} -b ${BIN_CONFIG} -o ${outut_path}/${i}_${file_output} -p $i ${PARALLEL}"
    nohup bash -c "time univmake -f ${FPM_CONFIG} -b ${BIN_CONFIG} -o ${outut_path}/${i}_${file_output} -p $i ${PARALLEL} 2>&1 > ${outut_path}/${i}_${file_output}.log" >> ${outut_path}/${i}_${file_output}.log &
#    time univmake -f ${FPM_CONFIG} -b ${BIN_CONFIG} -o ${outut_path}/${i}_${file_output} -p $i ${PARALLEL}
  done
  sleep 30
fi
rm ${FPM_CONFIG}
