#!/usr/bin/env python3
"""gate_spalte.py -- Gate zu Auftrag P, Teil 2 (Orchestrator). Eigene Rechnung, kein Code der Sitzung.
Matrix 16 x 15 ohne Zeile r, Spalte c verdeckt: fuer jede der 2^15 Ergaenzungen a die Zahl offener Klassen
(alle |<Zeile, x>| <= 5). Je Klasse (x_c = +1) erzwingt Zeile i mit Teilsumme s_i: |s_i| <= 4 frei, s_i = +6 -> a_i = -1,
s_i = -6 -> a_i = +1, |s_i| >= 8 nie offen. Gleiche Muster werden zusammengefasst, dann wird ueber alle a gezaehlt.
Aufruf: gate_spalte.py MATRIX [--voll16]"""
import sys, numpy as np
A = np.array([[int(t) for t in l.split()[:15]] for l in open(sys.argv[1]) if l.split()], dtype=np.int64)
assert A.shape == (16, 15) and set(np.unique(A)) == {-1, 1}
X = 1 - 2 * ((np.arange(1 << 14)[:, None] >> np.arange(14)[None, :]) & 1)   # 2^14 x 14
allA = np.arange(1 << 15, dtype=np.int64)
def fall(zeilen, c):
    B = A[zeilen]; m = len(zeilen)
    rest = [j for j in range(15) if j != c]
    S = X @ B[:, rest].T                                   # Klassen x m, x_c = +1
    ok = (np.abs(S) <= 6).all(axis=1); S = S[ok]
    w = 1 << np.arange(m)
    F = ((np.abs(S) == 6) * w).sum(axis=1)                 # erzwungene Stellen
    V = ((S == 6) * w).sum(axis=1)                         # dort a_i = -1 (Bit 1 = -1)
    key, cnt = np.unique(F * (1 << 16) + V, return_counts=True)
    F = key >> 16; V = key & 0xFFFF
    tot = np.zeros(1 << m, dtype=np.int64); aa = np.arange(1 << m, dtype=np.int64)
    for lo in range(0, len(F), 256):
        tot += (((aa[None, :] & F[lo:lo+256, None]) == V[lo:lo+256, None]) * cnt[lo:lo+256, None]).sum(axis=0)
    echt = int(((B[:, c] == -1) * w).sum())
    mn = int(tot.min())
    return mn, int((tot == mn).sum()), int(tot[echt])
if "--voll16" in sys.argv:
    res = [fall(list(range(16)), c) for c in range(15)]
    print("Kontrolle 16 Zeilen, 15 Spalten: (Minimum, Optima, echte Spalte):", sorted(set(res)))
else:
    res = {}
    for r in range(16):
        for c in range(15):
            res[(r, c)] = fall([i for i in range(16) if i != r], c)
    print("240 Faelle: (Minimum, Zahl der Optima, Wert der echten Spalte) ->", {k: list(res.values()).count(k) for k in sorted(set(res.values()))})
