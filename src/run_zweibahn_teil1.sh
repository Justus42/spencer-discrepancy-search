#!/bin/bash
# Auftrag Z, Teil 1: Gegenrechnung des Vortests mit eigenem Zaehlprogramm. Logs nach runs/zweibahn/teil1/.
# Aufruf aus dem Repo-Wurzelordner: bash src/run_zweibahn_teil1.sh
set -u
Z=build/zweibahn_zaehl; L=${L:-runs/zweibahn/teil1}; mkdir -p $L
lauf() { name=$1; shift; echo "== $name: $*"; ( date -u; timeout 1500 $Z "$@"; echo "Rueckgabe $?" ) > $L/$name.log.txt 2>&1;
         grep -E "Bahnen \(Paare\)|Klassen voller|harte Klassen|kurz [0-9]+ Paare|SUMME|Brute|ZEITGRENZE|UNVOLL|Laufzeit|Rueckgabe" $L/$name.log.txt; }
# a) (15,4) q=7 f=1, alle Zusammensetzungen <= 16 Paare: k=2 (B=2), k=1 (B=9), k=0 (B=16); <= 15: B um 1 kleiner
lauf a16_k2      7 1 4 2 2 -v
lauf a16_k2_nosym 7 1 4 2 2 -n -v
lauf a16_k1      7 1 4 1 9 -v
lauf a16_k1_nosym 7 1 4 1 9 -n -v
lauf a16_k0      7 1 4 0 16 -v
lauf a15_k2      7 1 4 2 1
lauf a15_k2_nosym 7 1 4 2 1 -n
lauf a15_k1      7 1 4 1 8
lauf a15_k0      7 1 4 0 15
# b) (17,5) q=7 f=3: zwei volle + <= 2 feste; + <= 1 festes
lauf b2          7 3 5 2 2 -v -o $L/b2_blaetter.txt
lauf b2_nosym    7 3 5 2 2 -n -v
lauf b1          7 3 5 2 1
lauf b1_nosym    7 3 5 2 1 -n
# c) (20,6) q=10 f=0, alle Zusammensetzungen <= 20 Paare; zwei volle + kurze <= 4 Paare
lauf c20_k2      10 0 6 2 0
lauf c20_k1      10 0 6 1 10
lauf c20_k0      10 0 6 0 20
lauf c_k2_B4     10 0 6 2 4
# d) (20,6) q=9 f=2: zwei volle + <= 2 Paare
lauf d_k2_B2     9 2 6 2 2
# e) (20,6) q=9 f=2: eine volle + kurze <= 11 Paare
lauf e_k1_B11    9 2 6 1 11
