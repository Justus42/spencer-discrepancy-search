#!/bin/bash
# Kontrolle Skalierung (22,7), d=2, K=13, Zufallsstart: ein Block = 4 Startwerte je 1800 s.
# Aufruf: run_skal22.sh typ seed_von seed_bis   (z. B. 13,9 1 4). Ausgabe runs/skal22/.
cd "$(dirname "$0")/.."
t=$1; tt=${t/,/-}
python3 src/run_gen.py runs/skal22 K13_typ$tt 22 7 2 13 $t 1800 $2 $3 >> runs/skal22/block_typ${tt}_seed$2-$3.out.txt 2>&1
echo "FERTIG $(date -u +%H:%M:%S)" >> runs/skal22/block_typ${tt}_seed$2-$3.out.txt
