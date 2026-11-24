import sys, time, numpy as np
sys.path.insert(0, '/home/claude/orch')
from sym_enum import *
# Wie nah kommt die Z7-Familie mit 30 Woertern? kleinste Zahl unueberdeckter Punkte
n, R = 15, 4
gfull, gfix, Cf, Cx, reps, sig = run(n, R, 7, 2, "p7 7+7+1")
xs, sig2, reps2, fixed = setup(n, 7, 2)
w = np.where(fixed[reps], 2, 14).astype(np.float32)
for s in range(4):
    U = (~Cf & ~Cx[s][None, :]).astype(np.float32)
    M = (U * w) @ U.T
    np.fill_diagonal(M, 1e9)
    print("feste Zeile", s, "kleinste Restzahl", int(M.min()), "Anzahl Paare mit Minimum", int((M == M.min()).sum() // 2))
# Vergleich: ctrl_16 hat einen Automorphismus der Ordnung 7 vom Typ 7+7+1? -> pruefen, ob eine der 112 Loesungen zu ctrl_16 aequivalent ist (Abstandsverteilung der Zeilen)
