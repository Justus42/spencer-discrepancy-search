#!/bin/bash
# Auftrag S, Teil 2b: Rang-9-Vertreter, exakte Suche mit k Paaren, Zeitlimit je Fall.
# Aufruf: bash src/run_spann_teil2b.sh K SEK [Namen...]
K=$1; SEK=$2; shift 2
P=runs/spann/probleme
NAMEN=${@:-"ctrl16_r9_y40 ctrl16_r9_y1 ctrl16_r9_y3 ctrl16_r9_y7 l111_r9_y1 l111_r9_y384 l111_r9_y128 l111_r9_y385 l111_r9_y129"}
for n in $NAMEN; do
  LOG=runs/spann/teil2b_${n}_k${K}.log.txt
  (date; build/spann_dfs $P/$n.txt $K $SEK 3 0 $P/$n.gen; date) 2>&1 | grep -v Fortschritt > $LOG
  echo "$n: $(grep ERGEBNIS $LOG) | $(grep Knoten $LOG)"
done
