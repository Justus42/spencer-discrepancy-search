#!/usr/bin/env python3
"""gate_b2_inv2.py -- Zusatz zu gate_b2.py, Teil F: fuer jeden Vertreter die Haeufigkeiten ueber alle 2^20 Punkte von
(kleinster Abstand zum Code, Zahl der Woerter in diesem Abstand, Zahl der Woerter im Abstand <= 6). Invariant unter
Spaltenpermutationen und Spaltenvorzeichen, weil diese die Punktmenge auf sich und den Code auf den Code abbilden."""
import numpy as np
from collections import Counter
N = 20; FULL = (1 << N) - 1
def pc(x):
    x = x - ((x >> 1) & 0x5555555555555555); x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333)
    return (((x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101 >> 56) & 0xFF
PTS = np.arange(1 << N, dtype=np.int64); I = []
for i in range(9):
    rows = [[int(v) for v in l.split()] for l in open('runs/bahnen3_zweit/klasse%d_vertreter.txt' % i) if l.split() and not l.startswith('#')]
    W = [sum(1 << j for j, x in enumerate(r) if x == -1) for r in rows]; W = W + [w ^ FULL for w in W]
    D = np.stack([pc(PTS ^ w).astype(np.int8) for w in W])            # 48 x 2^20
    mn = D.min(axis=0); nmin = (D == mn[None, :]).sum(axis=0); n6 = (D <= 6).sum(axis=0)
    key = mn.astype(np.int64) * 10000 + nmin * 100 + n6
    u, c = np.unique(key, return_counts=True); I.append(tuple(zip(u.tolist(), c.tolist())))
    print("Vertreter %d: %d verschiedene Punkttypen, groesster Abstand %d, hoechstens %d Woerter im Abstand <= 6" % (i, len(u), int(mn.max()), int(n6.max())))
gr = {}
for i in range(9): gr.setdefault(I[i], []).append(i)
print("Gruppen nach dieser Invariante:", sorted(gr.values()))
print("trennt 0 von 7: %s; trennt 5 von 6: %s" % (I[0] != I[7], I[5] != I[6]))
