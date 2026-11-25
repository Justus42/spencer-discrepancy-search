#!/bin/bash
# Aufruf: lauf_kern.sh MATRIX r k PREFIX RESTARTS SEKUNDEN   (4 Teile parallel, Ergebnis PREFIX_zertifikate.txt, sortiert nach Index)
M=$1; R=$2; K=$3; P=$4; RS=${5:-60}; SEK=${6:-1500}
for s in 0 1 2 3; do ./build/kern_r3 "$M" "$R" "$K" "${P}.teil$s.txt" $s 4 "$RS" $((s+11)) "$SEK" & done
wait
cat "${P}".teil?.txt | sort -n > "${P}_zertifikate.txt"
