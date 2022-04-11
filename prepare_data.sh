#!/usr/bin/env bash

export GraphPi_HOME=.
export GraphPi_BIN_REORDER=${GraphPi_HOME}/build/bin/reorder

export DATA_DIR=${GraphPi_HOME}/data

# mkdir data
if [[ ! -d "${DATA_DIR}" ]]; then
  echo "[Run] mkdir ${DATA_DIR}"
  mkdir ${DATA_DIR}
  echo "[Done] mkdir ${DATA_DIR}"
fi

# download
if [[ ! -f "${DATA_DIR}/com-friendster.ungraph.txt.gz" ]] && [[ ! -f "${DATA_DIR}/com-friendster.ungraph.txt" ]]; then
  echo "[Run] download friendster graph to ${DATA_DIR}"
  cd ${DATA_DIR} && wget https://snap.stanford.edu/data/bigdata/communities/com-friendster.ungraph.txt.gz && gunzip com-friendster.ungraph.txt.gz && cd ..
  echo "[Done] download friendster graph to ${DATA_DIR}"
fi

# reorder
if [[ -f "${DATA_DIR}/com-friendster.ungraph.txt" ]]; then
  echo "[Run] reorder friendster graph"
  ${GraphPi_BIN_REORDER} ${DATA_DIR}/com-friendster.ungraph.txt > ${DATA_DIR}/friendster.graph.txt
  echo "[Done] reorder friendster graph"
fi
