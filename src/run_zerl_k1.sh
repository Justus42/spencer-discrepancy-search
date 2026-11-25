#!/bin/bash
# K1 (Auftrag H): Halbproblem auf dem 48er, beide Richtungen, Startwerte 1-4, je hoechstens 600 s, 4 parallel.
# Aufruf aus dem Wurzelordner: bash src/run_zerl_k1.sh RICHTUNG   (W = U fest, W gesucht; U = W fest, U gesucht)
E=runs/zerlegung/eingaben; O=runs/zerlegung
for s in 1 2 3 4; do
  if [ "$1" = W ]; then
    ./build/zerl_halb ls $E/paley12_U11.txt 8 9 --seed $s --zeit 600 --aus $O/k1_W_seed$s.paar.txt > $O/k1_W_seed$s.log.txt 2>&1 &
  else
    ./build/zerl_halb ls $E/paley12_W9.txt 8 11 --fest-rechts --seed $s --zeit 600 --aus $O/k1_U_seed$s.paar.txt > $O/k1_U_seed$s.log.txt 2>&1 &
  fi
done
wait
for s in 1 2 3 4; do echo "seed $s: $(tail -n 1 $O/k1_$1_seed$s.log.txt)"; python3 src/zerl_offen.py offen $O/k1_$1_seed$s.paar.txt 8; done
