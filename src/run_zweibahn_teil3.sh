#!/bin/bash
# Auftrag Z, Teil 3 (d = 10, Rekord bei <= 31 Paaren). Logs nach runs/zweibahn/teil3/.
# Aufruf: bash src/run_zweibahn_teil3.sh FALL...   (Faelle: c13 c11 a b)
set -u
Z=build/zweibahn_zaehl; L=runs/zweibahn/teil3; mkdir -p $L
lauf() { name=$1; shift; echo "== $name: $*"; ( date -u; timeout 1750 $Z "$@" -z 1700; echo "Rueckgabe $?" ) > $L/$name.log.txt 2>&1;
         grep -E "Bahnen \(Paare\)|Klassen voller|harte Klassen|ueberdecken:|kurz [0-9]+ Paare|SUMME|ZEITGRENZE|UNVOLL|Laufzeit|Rueckgabe" $L/$name.log.txt; }
for fall in "$@"; do case $fall in
  c13) lauf c_q13_k2_B5  13 0 8 2 5 ;;            # (26,8) q=13 f=0: zwei volle + kurze (nur 2 feste Paare)
  c11) lauf c_q11_k2_B9  11 4 8 2 9 ;;            # (26,8) q=11 f=4: zwei volle + <= 9 feste
  a)   lauf a_k2_B32     13 4 10 2 32 ;;          # (30,10) q=13 f=4: harte Klassen wie bei B=5; Verteilung liefert <=5 und das Minimum
  a2)  lauf a_k2_B32_ab2926 13 4 10 2 32 -s 2926 ;;
  b)   lauf b_k2_B5      13 2 9 2 5 ;;            # (28,9) q=13 f=2: zwei volle + <= 5 feste
esac; done
# Fortsetzung (30,10) nach Zeitgrenze: bash src/run_zweibahn_teil3.sh a2   (Klassen ab 2926)
