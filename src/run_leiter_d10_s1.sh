#!/bin/bash
# Auftrag D, Schritt 1: wie run_leiter_d10.sh, aber mit Speichermessung (VmHWM aus /proc, alle 2 s gelesen; /usr/bin/time gibt es hier nicht).
# Aufruf: bash src/run_leiter_d10_s1.sh NAME K m1 m2 d SEK SEED1 [SEED2 ...]
NAME=$1; K=$2; M1=$3; M2=$4; D=$5; SEK=$6; shift 6
O=runs/leiter_d10; mkdir -p $O
PIDS=()
for s in "$@"; do
  ./build/zerl_halb alt $K $M1 $M2 $D --seed $s --zeit $SEK --aus $O/${NAME}_seed$s.paar.txt --neustart 20 > $O/${NAME}_seed$s.log.txt 2>&1 &
  PIDS+=($!)
done
declare -A HWM
while kill -0 ${PIDS[0]} 2>/dev/null || { for p in "${PIDS[@]}"; do kill -0 $p 2>/dev/null && exit_loop=0 && break; done; [ "$exit_loop" = 0 ]; }; do
  exit_loop=1
  for p in "${PIDS[@]}"; do v=$(awk '/VmHWM/{print $2}' /proc/$p/status 2>/dev/null); [ -n "$v" ] && HWM[$p]=$v; done
  sleep 2
done
wait
i=0
for s in "$@"; do
  p=${PIDS[$i]}; i=$((i+1))
  echo "$NAME seed $s: $(grep -E '^(LOESUNG|ZEITGRENZE)' $O/${NAME}_seed$s.log.txt | head -1) | Speicher max ${HWM[$p]:-?} kB"
  if [ -f $O/${NAME}_seed$s.paar.txt ]; then
    t0=$(date +%s.%N); python3 src/zerl_offen.py offen $O/${NAME}_seed$s.paar.txt $D; t1=$(date +%s.%N)
    echo "  zerl_offen.py Dauer $(echo "$t1 - $t0" | bc) s"
  else echo "  (kein Paar gespeichert)"; fi
done
