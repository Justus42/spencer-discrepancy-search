#!/bin/bash
# Beide Pruefskripte auf die 48er-Folgecodes (Laenge 22/24/26), Ausgaben je Datei in runs/ext48/.
cd "$(dirname "$0")/../.."
for s in 5 6; do for n in 22 24 26; do
  b=runs/matrix_ext48_n${n}_typ11-9_aus52seed${s}; v=$b.txt; [ -f ${b}_pad.txt ] && v=${b}_pad.txt
  echo "python3 src/check_disc.py $b.txt > runs/ext48/check_disc_n${n}_seed${s}.txt 2>&1"
  echo "python3 verifiers/verify_spencer.py $v > runs/ext48/verify_n${n}_seed${s}.txt 2>&1"
done; done | xargs -P 4 -I{} bash -c '{}'
echo FERTIG > runs/ext48/fertig.txt
