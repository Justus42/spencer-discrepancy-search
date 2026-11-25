#!/bin/bash
# Auftrag N: ein Block = ein Typ, Startwerte parallel (je SEK s), Suche mit src/zerl_halb.c (unveraendert).
# Aufruf: bash src/run_leiter_n.sh NAME K m1 m2 d SEK SEED1 [SEED2 ...] [-- weitere Optionen fuer zerl_halb alt]
# Logs runs/leiter_n/NAME_seed<s>.log.txt, Paare NAME_seed<s>.paar.txt; danach Pruefcode zerl_offen.py je Paar (Zusammenfassung stdout).
NAME=$1; K=$2; M1=$3; M2=$4; D=$5; SEK=$6; shift 6
SEEDS=(); while [ $# -gt 0 ] && [ "$1" != "--" ]; do SEEDS+=($1); shift; done; [ "$1" = "--" ] && shift
O=runs/leiter_n
mkdir -p $O
for s in "${SEEDS[@]}"; do
  ./build/zerl_halb alt $K $M1 $M2 $D --seed $s --zeit $SEK --aus $O/${NAME}_seed$s.paar.txt "$@" > $O/${NAME}_seed$s.log.txt 2>&1 &
done
wait
for s in "${SEEDS[@]}"; do
  echo "$NAME seed $s: $(grep -E '^(LOESUNG|ZEITGRENZE)' $O/${NAME}_seed$s.log.txt | head -1)"
  if [ -f $O/${NAME}_seed$s.paar.txt ]; then python3 src/zerl_offen.py offen $O/${NAME}_seed$s.paar.txt $D; else echo "  (kein Paar gespeichert)"; fi
done
