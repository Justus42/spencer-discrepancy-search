#!/bin/bash
# Auftrag Z, Teil 2 (d = 9, Rekord bei <= 25 Paaren). Logs nach runs/zweibahn/teil2/. Aufruf: bash src/run_zweibahn_teil2.sh
set -u
Z=build/zweibahn_zaehl; L=runs/zweibahn/teil2; mkdir -p $L
lauf() { name=$1; shift; echo "== $name: $*"; ( date -u; timeout 1700 $Z "$@"; echo "Rueckgabe $?" ) > $L/$name.log.txt 2>&1;
         grep -E "Bahnen \(Paare\)|Klassen voller|harte Klassen|ueberdecken:|kurz [0-9]+ Paare|SUMME|ZEITGRENZE|UNVOLL|Laufzeit|Rueckgabe" $L/$name.log.txt; }
lauf a_k2_B3   11 3 8 2 3  -o $L/a_k2_B3_blaetter.txt
lauf a_k1_B14  11 3 8 1 14 -o $L/a_k1_B14_blaetter.txt
lauf b_k2_B1   12 1 8 2 1  -o $L/b_k2_B1_blaetter.txt
lauf c_k2_B3   11 1 7 2 3  -o $L/c_k2_B3_blaetter.txt
# Nebenergebnis a): kleinste Zahl fester Paare zu zwei vollen Bahnen (alle 16 festen Paare erlaubt)
lauf a_k2_B16  11 3 8 2 16
