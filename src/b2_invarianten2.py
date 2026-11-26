#!/usr/bin/env python3
"""b2_invarianten2.py (Auftrag B2, Pruefcode, nur ganze Zahlen)

Feinere Invarianten (unabhaengig von b2_aequiv), invariant unter Zeilen-/Spaltenpermutation und -vorzeichen:
  J1 = Multimenge der Spaltensignaturen; Signatur von Spalte c = Verteilung von |S(c,b,d,e)| ueber alle Tripel {b,d,e} ohne c,
       S = sum_r M[r,c]M[r,b]M[r,d]M[r,e].
  J2 = Multimenge der Spaltenpaar-Signaturen; Signatur von {a,b} = (Signatur a, Signatur b sortiert,
       Verteilung von |S(a,b,d,e)| ueber Paare {d,e}).
  J3 = 6er-Summen: Verteilung von |sum_r prod_{6 Spalten} M[r,.]| (gerade Zahl von Spalten).
Aufruf: b2_invarianten2.py MATRIX...
"""
import sys, hashlib
from itertools import combinations
from collections import Counter
import numpy as np

def lies(fn):
    return np.array([[int(x) for x in l.split()] for l in open(fn) if l.strip() and not l.startswith('#')], dtype=np.int64)

erg = {}
for fn in sys.argv[1:]:
    M = lies(fn); R, C = M.shape
    S4 = {}
    for q in combinations(range(C), 4):
        S4[q] = abs(int((M[:, q[0]] * M[:, q[1]] * M[:, q[2]] * M[:, q[3]]).sum()))
    sig = {}
    for c in range(C):
        sig[c] = tuple(sorted(Counter(v for q, v in S4.items() if c in q).items()))
    J1 = tuple(sorted(Counter(sig.values()).items()))
    J2c = Counter()
    for a, b in combinations(range(C), 2):
        d = tuple(sorted(Counter(v for q, v in S4.items() if a in q and b in q).items()))
        J2c[(tuple(sorted((sig[a], sig[b]))), d)] += 1
    J2 = tuple(sorted(J2c.items()))
    J3 = Counter()
    for q in combinations(range(C), 6):
        J3[abs(int(np.prod(M[:, list(q)], axis=1).sum()))] += 1
    J3 = tuple(sorted(J3.items()))
    key = (J1, J2, J3)
    erg[fn] = key
    h = lambda x: hashlib.sha256(repr(x).encode()).hexdigest()[:10]
    print(fn, "J1", h(J1), "J2", h(J2), "J3", dict(J3))
print("verschiedene Schluessel: %d von %d" % (len(set(erg.values())), len(erg)))
