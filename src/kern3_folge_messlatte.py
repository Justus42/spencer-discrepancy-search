#!/usr/bin/env python3
"""kern3_folge_messlatte.py -- Hilfsskript Auftrag K2 (kein Such- und kein Pruefcode).
Liest die VERTRETER-Zeilen eines lsw-Logs (K = 8 Treffer), streicht j = 1..JMAX Nebenklassen in allen Moeglichkeiten und
zaehlt die offenen Klassen mit `build/kern3_such eval` (getrennter Prozess je Fall). Ausgabe: je j Anzahl Faelle, kleinste/groesste Restzahl.
Aufruf: kern3_folge_messlatte.py m R n0,n1,n2,n3 LOG JMAX [PROZESSE]
"""
import itertools, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
m, R, typ, log, jmax = int(sys.argv[1]), int(sys.argv[2]), sys.argv[3], sys.argv[4], int(sys.argv[5])
procs = int(sys.argv[6]) if len(sys.argv) > 6 else 4
vs = [l.split()[1] for l in open(log) if l.startswith("VERTRETER")]
assert len(vs) == 8, len(vs)

def offen(drop):
    sel = [v for i, v in enumerate(vs) if i not in drop]
    r = subprocess.run(["build/kern3_such", "eval", str(m), str(R)] + typ.split(","), input="\n".join(sel) + "\n",
                       capture_output=True, text=True, check=True)
    return int(r.stdout.split()[1])

fälle = [d for j in range(1, jmax + 1) for d in itertools.combinations(range(8), j)]
with ThreadPoolExecutor(procs) as ex:
    res = list(ex.map(offen, fälle))
for j in range(1, jmax + 1):
    w = [r for d, r in zip(fälle, res) if len(d) == j]
    print(f"Typ {typ} m={m} R={R} ohne {j}: Fälle {len(w)}, kleinste Restzahl {min(w)}, größte {max(w)}, alle {sorted(w)}" if len(w) <= 8
          else f"Typ {typ} m={m} R={R} ohne {j}: Fälle {len(w)}, kleinste Restzahl {min(w)}, größte {max(w)}")
