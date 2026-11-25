#!/bin/bash
# Aufruf: lauf_pruef_kozyk.sh GRUPPE n   -- prueft Gegenbeispiele (Stufe 6) und rechnet Ueberlebende mit eigenen Matrizen exakt nach
G=$1; NN=$2
D=runs/pruef_ideen/kozyklisch
python3 -I verifiers/pruef_ideen/pruef_kozyk.py "$G" 6 "$D/${G}_n$NN" 4 --exakt "$D/${G}_n$NN" "${TMPDIR_P:-/tmp}" 6 > "$D/${G}_n$NN.pruefung.log.txt" 2>&1
