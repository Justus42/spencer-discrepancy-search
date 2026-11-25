#!/bin/bash
# Auftrag A, Teil 3 gemischt: alle Profile (q, s) mit 40q+80s <= 208, (q,s) != (0,0), billigste zuerst, ein Block.
# Aufruf: bash src/run_profile.sh BLOCK_SEKUNDEN
set -u
cd "$(dirname "$0")/.."
BLOCK=$1; START=$(date +%s)
for PR in "5 0" "3 1" "1 2" "4 0" "2 1" "0 2" "3 0" "1 1" "2 0" "0 1" "1 0"; do
  set -- $PR
  F=runs/einzelnull/fertig_10_8_6_q$1s$2.txt
  if [ -f $F ] && [ "$(wc -l < $F)" -ge 10 ]; then continue; fi
  REST=$(( BLOCK - ( $(date +%s) - START ) ))
  if [ $REST -lt 30 ]; then break; fi
  bash src/run_einzelnull.sh 10,8,6 $REST $1 $2 > /dev/null
  if grep -q "LOESUNG" runs/einzelnull/block_10_8_6_q$1s$2.log.txt; then echo "LOESUNG Profil $1 $2"; break; fi
done
for PR in "5 0" "3 1" "1 2" "4 0" "2 1" "0 2" "3 0" "1 1" "2 0" "0 1" "1 0"; do set -- $PR; F=runs/einzelnull/fertig_10_8_6_q$1s$2.txt; echo "Profil q=$1 s=$2: $( [ -f $F ] && wc -l < $F || echo 0 ) von 10 Typen fertig"; done
