#!/bin/bash
# run_kern3_folge.sh -- Treiber Auftrag K2 (Kopie von run_kern3.sh, Ausgabeordner runs/kern3_folge) (kein Pruefcode). Startet build/kern3_such fuer mehrere Typen, 4 gleichzeitig.
# Aufruf: src/run_kern3.sh MODUS m R K SEK PRAEFIX TYP... [SEED fuer ls ueber Umgebungsvariable SEED, Standard 1]
#   MODUS dfs, ls oder lsw; TYP als n0,n1,n2,n3. Logs runs/kern3_folge/PRAEFIX_n0-n1-n2-n3.log.txt, Matrix bei Treffer *.matrix.txt.
#   Bei einem Treffer mit 4K < 2*... (K = 5) schreibt der Job runs/kern3_folge/STOPP; danach startet kein neuer Job.
set -u
MODUS=$1; M=$2; R=$3; K=$4; SEK=$5; PR=$6; shift 6
SEED=${SEED:-1}
cd "$(dirname "$0")/.."
mkdir -p runs/kern3_folge
job() {
    t=$1; tn=${t//,/-}
    if [ -e runs/kern3_folge/STOPP ]; then echo "UEBERSPRUNGEN (STOPP) $t" > runs/kern3_folge/${PR}_${tn}.log.txt; return; fi
    log=runs/kern3_folge/${PR}_${tn}.log.txt; mat=runs/kern3_folge/${PR}_${tn}.matrix.txt
    { echo "START $(date -u +%H:%M:%S) $MODUS m=$M R=$R Typ=$t K=$K SEK=$SEK SEED=$SEED"
      if [ "$MODUS" = dfs ]; then ./build/kern3_such dfs $M $R ${t//,/ } $K $SEK $mat
      else ./build/kern3_such $MODUS $M $R ${t//,/ } $K $SEK $SEED $mat; fi
      echo "ENDE $(date -u +%H:%M:%S)"; } > $log 2>&1
    if grep -q "ERGEBNIS GEFUNDEN" $log && [ "$K" -le 5 ]; then touch runs/kern3_folge/STOPP; fi
}
export -f job; export MODUS M R K SEK PR SEED
printf '%s\n' "$@" | xargs -P 4 -I{} bash -c 'job {}'
echo "FERTIG $(date -u +%H:%M:%S) $PR"
