#!/usr/bin/env bash

# change the code in baseline_test.cpp to use 32 threads

export GraphPi_HOME=.
export GraphPi_BIN_MPI=${GraphPi_HOME}/build/bin/run_mpi

export DATA_DIR=${GraphPi_HOME}/data
export QUERY_DIR=${GraphPi_HOME}/queries
export THREAD_COUNT=32

# friendster
get_data_path() {
  echo ${DATA_DIR}/$1.graph.txt
}

get_data_arg() {
  echo `echo $1 | sed -e "s/\b\(.\)/\u\1/g"` `get_data_path $1`
}

get_query_arg() {
  query="0"
  # p1 in paper => q1 in code
  if [ ${1} = "1" ]; then query="1"; fi
  # p2 in paper => q2 in code
  if [ ${1} = "2" ]; then query="2"; fi
  # p3 in paper => q10 in code
  if [ ${1} = "3" ]; then query="10"; fi
  # p4 in paper => q3 in code
  if [ ${1} = "4" ]; then query="3"; fi
  # p5 in paper => q5 in code
  if [ ${1} = "5" ]; then query="5"; fi
  # p6 in paper => q11 in code
  if [ ${1} = "6" ]; then query="11"; fi
  # p7 in paper => q4 in code
  if [ ${1} = "7" ]; then query="4"; fi
  # p8 in paper => q12 in code
  if [ ${1} = "8" ]; then query="12"; fi
  # p9 in paper => q13 in code
  if [ ${1} = "9" ]; then query="13"; fi
  # p10 in paper => q6 in code
  if [ ${1} = "10" ]; then query="6"; fi
  cat ${QUERY_DIR}/q$query.graph.matrix
}

run() {
  if [ $# -lt 3 ]; then echo 'run <dataset> <query_id> <hosts>'; return; fi
  dataset=$1
  query=`get_query_arg ${2}`
  echo pattern = ${2}, query = $query
  hosts=$3
  mpirun -ppn 1 -hosts ${hosts} ${GraphPi_BIN_MPI} `get_data_arg $dataset` $query ${THREAD_COUNT} > p$2.log 2>&1
}

run_all() {
  if [ $# -lt 2 ]; then echo 'run_all <dataset> <hosts>'; return; fi
  dataset=$1
  hosts=$2
  for i in {1..10}; do run $dataset $i $hosts; done
}

print_help() {
  echo Usage:
  echo -n '  ./run_query.sh '; run
}

if [ $# -gt 0 ]; then
  if [ $1 == "-h" ]; then
    print_help
    exit
  fi
  $@
else
  print_help
fi
