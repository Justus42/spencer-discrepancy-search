#!/bin/bash
# Lauf 23.11.2026: n=20 R=6 d=2 K=13 nur Typ (11,9), Startwerte 1-16, je 1800 s, 4 parallel.
# Harte Obergrenze 2,5 h (9000 s). Aufruf aus dem Repo-Wurzelordner.
timeout 9000 bash -c 'seq 1 16 | xargs -P4 -I{} bash -c \
 "python3 src/coset_ladder.py 20 6 2 13 1800 {} 11,9 > runs/2026-11-23_K13_typ11-9_seed{}.log.txt 2>&1"'
echo "fertig $(date -u +%FT%TZ)" > runs/2026-11-23_K13_typ11-9_fertig.txt
