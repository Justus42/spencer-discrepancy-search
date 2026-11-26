#!/usr/bin/env python3
"""b2_invarianten.py (Auftrag B2, Pruefcode, nur ganze Zahlen)

Invarianten unter Zeilenpermutation, Zeilenvorzeichen, Spaltenpermutation und Spaltenvorzeichen (unabhaengig von b2_aequiv):
  I1 = Verteilung von |<r_a, r_b>| ueber alle Zeilenpaare
  I2 = Verteilung von |sum_r M[r,a] M[r,b] M[r,c] M[r,d]| ueber alle Spaltenquadrupel (gerade Zahl Spalten -> zeilenvorzeichenfrei)
Verschiedene Invarianten beweisen Nicht-Gleichwertigkeit.  Aufruf: b2_invarianten.py MATRIX...
"""
import sys
from itertools import combinations
from collections import Counter

def lies(fn):
    return [[int(x) for x in l.split()] for l in open(fn) if l.strip() and not l.startswith('#')]

inv = {}
for fn in sys.argv[1:]:
    M = lies(fn); R = len(M); C = len(M[0])
    I1 = Counter(abs(sum(x * y for x, y in zip(M[a], M[b]))) for a, b in combinations(range(R), 2))
    cols = [[M[r][c] for r in range(R)] for c in range(C)]
    I2 = Counter()
    # I3: fuer jeden Punkt x in {0,1}^C das Profil (Zahl der Codewoerter im Abstand 0..6); Verteilung der Profile.
    # Spaltenoperationen erhalten Abstaende und bilden Punkte bijektiv ab; Zeilenoperationen aendern die Wortmenge nicht.
    for a, b, c, d in combinations(range(C), 4):
        I2[abs(sum(cols[a][r] * cols[b][r] * cols[c][r] * cols[d][r] for r in range(R)))] += 1
    import numpy as np
    W = set()
    for r in M:
        w = sum(1 << j for j, x in enumerate(r) if x == -1); W.add(w); W.add(w ^ ((1 << C) - 1))
    alle = np.arange(1 << C, dtype=np.int64)
    prof = np.zeros(1 << C, dtype=np.int64)
    for w in W:
        d = np.bitwise_count(alle ^ w).astype(np.int64)
        for k in range(7):
            prof += (d == k).astype(np.int64) * (64 ** k)
    werte, anz = np.unique(prof, return_counts=True)
    I3 = tuple(zip(werte.tolist(), anz.tolist()))
    key = (tuple(sorted(I1.items())), tuple(sorted(I2.items())), I3)
    inv[fn] = key
    import hashlib
    print(fn, "I1:", dict(sorted(I1.items())), "I2:", dict(sorted(I2.items())), "I3: %d Profile, Kennung %s" % (len(I3), hashlib.sha256(repr(I3).encode()).hexdigest()[:12]))
keys = list(inv.values())
print("verschiedene Invariantenpaare: %d von %d Matrizen" % (len(set(keys)), len(keys)))
