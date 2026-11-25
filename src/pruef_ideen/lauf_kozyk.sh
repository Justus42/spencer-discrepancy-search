#!/bin/bash
# Aufruf: lauf_kozyk.sh GRUPPE n LCEIL LSIEB   (4 Teile parallel; Belege in runs/pruef_ideen/kozyklisch/)
G=$1; NN=$2; LC=$3; LS=$4
D=runs/pruef_ideen/kozyklisch
T=${TMPDIR_P:-/tmp}
for s in 0 1 2 3; do ./build/kozyk "$G" "$LC" "$LS" $s 4 "$D/${G}_n$NN" "$T" > "$D/${G}_n$NN.shard$s.log.txt" & done
wait
cat "$D/${G}_n$NN".shard?.log.txt > "$D/${G}_n$NN.zusammen.log.txt"
