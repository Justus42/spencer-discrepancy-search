#!/usr/bin/env python3
"""gate_v_bauregel.py -- Gate zu Auftrag V: Vergleich der Leiterstufen mit der Bauregel "geloeste Matrix ohne eine / ohne zwei Zeilen".
Eigener Zaehler. Fuer jede geloeste Kontrollmatrix (0 offen) und die Bauregel-Matrizen: kleinste Zahl offener Klassen nach Streichen
einer Zeile (alle k Faelle) und zweier Zeilen (alle k(k-1)/2 Faelle). Dazu: Sind die Ausgaben verschiedener Startwerte gleich?
Aufruf: gate_v_bauregel.py REPO"""
import sys, os, re, glob, itertools
import numpy as np
repo = sys.argv[1]; D = os.path.join(repo, 'runs/verdopplung')
def lies(p): return np.array([[int(v) for v in l.split()] for l in open(p) if l.strip()], dtype=np.int8)
def masken(M, R):
    k, m = M.shape; d = m - 2 * R; out = []
    for u in M:
        a = np.array([u[0]], dtype=np.int8)
        for j in range(1, m): a = np.concatenate((a + u[j], a - u[j]))
        out.append(np.packbits(np.abs(a) >= d))            # gedeckte Klassen je Zeile
    return out
def streich(M, R):
    z = masken(M, R); k = len(z); n = 1 << (M.shape[1] - 1)
    cnt = np.zeros(n, dtype=np.uint8)
    for q in z: cnt += np.unpackbits(q)[:n]
    assert int((cnt == 0).sum()) == 0
    b = [np.unpackbits(q)[:n].astype(bool) for q in z]
    eins = cnt == 1; zwei = cnt == 2
    e1 = [int((eins & b[i]).sum()) for i in range(k)]      # nur von Zeile i gedeckt
    best2 = min(e1[i] + e1[j] + int((zwei & b[i] & b[j]).sum()) for i, j in itertools.combinations(range(k), 2))
    return min(e1), best2
print('1. Bauregel: geloeste Matrix ohne 1 Zeile / ohne 2 Zeilen, kleinste Zahl offener Klassen')
for (m, R, k, blk) in ((19, 6, 16, '0b'), (17, 5, 16, '0b'), (22, 7, 24, '0c'), (24, 8, 24, '0c')):
    erg = []
    for p in sorted(glob.glob(os.path.join(D, 'out', '%s_m%dR%dk%d_s*.txt' % (blk, m, R, k)))):
        M = lies(p)
        try: erg.append((os.path.basename(p),) + streich(M, R))
        except AssertionError: pass
    if m >= 22:
        p = os.path.join(D, 'bauregel_24x%d.txt' % m); erg.append((os.path.basename(p),) + streich(lies(p), R))
    for e in erg: print('   (%d,%d) %s: ohne 1 Zeile min %d, ohne 2 Zeilen min %d' % (m, R, e[0], e[1], e[2]))
print('2. Ausgaben verschiedener Startwerte: verschiedene Dateiinhalte / verschiedene Zeilenmengen bis auf Vorzeichen und Reihenfolge')
for pat in ('2_m19R6k15', '2_m19R6k14', '2_m17R5k15', '2_m17R5k14', '1_m19R6k10', '1_m17R5k10'):
    fs = sorted(glob.glob(os.path.join(D, 'out', pat + '_s*.txt')))
    roh = {open(f).read() for f in fs}
    mengen = {frozenset(tuple(int(v) * (1 if int(l.split()[0]) > 0 else -1) for v in l.split()) for l in open(f) if l.strip()) for f in fs}
    print('   %s: %d Dateien, %d verschiedene Inhalte, %d verschiedene Zeilenmengen' % (pat, len(fs), len(roh), len(mengen)))
