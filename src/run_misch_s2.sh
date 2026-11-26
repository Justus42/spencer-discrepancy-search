#!/bin/bash
# Auftrag M, Schritt 2: Zustaende mit zerl_halb alt (unveraendert) erzeugen, je Zustand misch_frei (f <= 2, exakt).
# Aufruf: bash src/run_misch_s2.sh K m1 m2 SEK NEUSTART SEED1 SEED2 ...    (4 Laeufe gleichzeitig)
# Logs und Paare runs/mischform/s2/k<K>_t<m1>-<m2>_z<SEK>_n<NEUSTART>_seed<s>.*; Zusammenfassung stdout.
K=$1; M1=$2; M2=$3; SEK=$4; NEU=$5; shift 5
O=runs/mischform/s2; mkdir -p $O
NAME=k${K}_t${M1}-${M2}_z${SEK}_n${NEU}
printf '%s\n' "$@" | xargs -P 4 -I{} sh -c "./build/zerl_halb alt $K $M1 $M2 8 --seed {} --zeit $SEK --neustart $NEU --aus $O/${NAME}_seed{}.paar.txt > $O/${NAME}_seed{}.log.txt 2>&1"
for s in "$@"; do
  P=$O/${NAME}_seed$s.paar.txt
  ./build/misch_frei paar $P 8 2 > $O/${NAME}_seed$s.frei.txt
  rz=$(grep -oE 'Restzahl [0-9]+' $P | head -1)
  op=$(grep -oE 'offene Punkte [0-9]+' $O/${NAME}_seed$s.frei.txt | head -1)
  mc=$(grep -oE 'hoechstens [0-9]+' $O/${NAME}_seed$s.frei.txt)
  f1=$(grep -E '^f=1: (JA|NEIN)' $O/${NAME}_seed$s.frei.txt | cut -c1-20)
  f2=$(grep -E '^f=2: (JA|NEIN)' $O/${NAME}_seed$s.frei.txt | cut -c1-20)
  echo "$NAME seed $s: $rz, $op, maxcov ${mc#hoechstens }, $f1, $f2"
done
