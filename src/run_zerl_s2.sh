#!/bin/bash
# Schritt 2 (Auftrag H): d = 9, U = Paley-12 ohne Spalte 0 (12 x 11), W 12 x 12 und 12 x 14.
# Kontrolle m2 = 10 (bekannt unmoeglich), dann je dfs mit Zaehlschranke und ls, 4 parallel, je SEK Sekunden.
SEK=${1:-1200}; E=runs/zerlegung/eingaben; O=runs/zerlegung
./build/zerl_halb dfs $E/paley12_U11.txt 9 10 --lam 0,1,3,6,0 > $O/s2_dfs_m10.log.txt 2>&1
./build/zerl_halb dfs $E/paley12_U11.txt 9 12 --lam 0,1,2,6,0,0 --zeit $SEK --aus $O/s2_dfs_m12.paar.txt > $O/s2_dfs_m12.log.txt 2>&1 &
./build/zerl_halb dfs $E/paley12_U11.txt 9 14 --lam 0,1,2,6,0,0,0 --zeit $SEK --aus $O/s2_dfs_m14.paar.txt > $O/s2_dfs_m14.log.txt 2>&1 &
./build/zerl_halb ls $E/paley12_U11.txt 9 12 --seed 1 --zeit $SEK --aus $O/s2_ls_m12.paar.txt > $O/s2_ls_m12.log.txt 2>&1 &
./build/zerl_halb ls $E/paley12_U11.txt 9 14 --seed 1 --zeit $SEK --aus $O/s2_ls_m14.paar.txt > $O/s2_ls_m14.log.txt 2>&1 &
wait
for f in dfs_m10 dfs_m12 dfs_m14 ls_m12 ls_m14; do echo "$f: $(grep -E '^(LOESUNG|KEINE|ZEITGRENZE)' $O/s2_$f.log.txt)"; done
