#!/bin/bash
# Auftrag A, Teil 4 neu: Leiter selbstkomplementaerer Codes bei (13,3). Treiber, rechnet nichts selbst.
# Aufruf: bash src/run_sc13.sh BLOCK_SEKUNDEN WORTZAHL...   (je Stufe 4 Startwerte parallel, je 600 s)
# Stopp bei der ersten Stufe ohne Loesung. Log runs/sc13/leiter.log.txt, Loesungen runs/sc13/w<W>_s<seed>.txt
set -u
cd "$(dirname "$0")/.."
BLOCK=$1; shift
D=runs/sc13; mkdir -p $D
LOG=$D/leiter.log.txt
START=$(date +%s)
echo "### Block Start $(date -u '+%F %T') UTC" >> $LOG
for W in "$@"; do
  if [ -f $D/stufe_$W.ok ]; then continue; fi
  if [ -f $D/stufe_$W.fehl ]; then echo "### Stufe $W schon ohne Loesung, Stopp" >> $LOG; break; fi
  REST=$(( BLOCK - ( $(date +%s) - START ) ))
  if [ $REST -lt 600 ]; then echo "### Blockzeit reicht nicht fuer Stufe $W" >> $LOG; break; fi
  P=$(( W / 2 ))
  for s in 1 2 3 4; do ./build/sc13_tabu -m 13 -R 3 -P $P -s $s -z 600 -o $D/w${W}_s$s.txt > $D/w${W}_s$s.out 2>&1 & done
  wait
  cat $D/w${W}_s?.out >> $LOG
  OK=0
  for s in 1 2 3 4; do
    if [ -f $D/w${W}_s$s.txt ]; then
      python3 src/sc_pruef.py $D/w${W}_s$s.txt 3 $D/w${W}_s${s}_matrix.txt >> $LOG 2>&1 && OK=1
    fi
  done
  if [ $OK = 1 ]; then touch $D/stufe_$W.ok; echo "### Stufe $W: geloest" >> $LOG
  else touch $D/stufe_$W.fehl; echo "### Stufe $W: OHNE Loesung, Stopp" >> $LOG; break; fi
done
echo "### Block Ende $(date -u '+%F %T') UTC" >> $LOG
