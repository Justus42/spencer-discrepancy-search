#!/bin/bash
# run_kern3_folge_gate.sh -- Treiber Auftrag K2 (kein Pruefcode). Startet build/kern3_gate (OMP_NUM_THREADS=1) je Typ, 4 Prozesse gleichzeitig.
# Aufruf: src/run_kern3_folge_gate.sh m R K SEK PRAEFIX TYP...   (TYP als n0,n1,n2,n3). Logs runs/kern3_folge/PRAEFIX_n0-n1-n2-n3.log.txt
set -u
M=$1; R=$2; K=$3; SEK=$4; PR=$5; shift 5
cd "$(dirname "$0")/.."
mkdir -p runs/kern3_folge/typen_gate
job() {
    t=$1; tn=${t//,/-}
    echo "${t//,/-}" > runs/kern3_folge/typen_gate/$tn.txt
    log=runs/kern3_folge/${PR}_${tn}.log.txt
    { echo "START $(date -u +%H:%M:%S) kern3_gate m=$M R=$R Typ=$t K=$K SEK=$SEK"
      TYPEN=runs/kern3_folge/typen_gate/$tn.txt OMP_NUM_THREADS=1 ./build/kern3_gate alle $M $R $K $SEK
      echo "ENDE $(date -u +%H:%M:%S)"; } > $log 2>&1
}
export -f job; export M R K SEK PR
printf '%s\n' "$@" | xargs -P 4 -I{} bash -c 'job {}'
echo "FERTIG $(date -u +%H:%M:%S) $PR"
