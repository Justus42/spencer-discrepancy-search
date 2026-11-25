#!/usr/bin/env python3
"""gate_zerl.py -- Gate zu Auftrag H (Orchestrator). Eigener Zaehler, kein Code der Sitzung.
Liest jedes gespeicherte Paar (U | W) aus runs/zerlegung/*.paar.txt, zaehlt die offenen Klassen (y, z) mit y_0 = z_0 = +1,
also max_i (|U_i y| + |W_i z|) < d, und vergleicht mit der Restzahl in der Kopfzeile (bei geloesten Paaren: 0).
Zusatz: Betraege der Zeilen-Skalarprodukte von U und W (zeigt, ob eine Haelfte Hadamard-artig ist).
Aufruf: gate_zerl.py VERZEICHNIS"""
import sys, glob, os, re, numpy as np
from collections import Counter
def halb(M):
    K, m = M.shape
    Y = 1 - 2 * ((np.arange(1 << (m - 1))[:, None] >> np.arange(m - 1)[None, :]) & 1)
    Y = np.hstack([np.ones((len(Y), 1), dtype=np.int64), Y])
    return np.abs(Y @ M.T).astype(np.int16)                       # Klassen x K
def offen(U, W, d):
    A, B = halb(U), halb(W); n = 0
    for lo in range(0, len(A), 64):
        n += int(((A[lo:lo+64, None, :] + B[None, :, :]).max(axis=2) < d).sum())
    return n
def gram(M):
    G = np.abs(M @ M.T); iu = np.triu_indices(len(M), 1)
    return dict(sorted(Counter(G[iu].tolist()).items()))
ab = 0; zeilen = []
for fn in sorted(glob.glob(sys.argv[1] + "/*.paar.txt")):
    kopf = open(fn).readline()
    rows = [l for l in open(fn) if "|" in l and not l.startswith("#")]
    U = np.array([[int(t) for t in l.split("|")[0].split()] for l in rows], dtype=np.int64)
    W = np.array([[int(t) for t in l.split("|")[1].split()] for l in rows], dtype=np.int64)
    assert set(np.unique(U)) <= {-1, 1} and set(np.unique(W)) <= {-1, 1}
    md = re.search(r"d=(\d+)", kopf); mr = re.search(r"Restzahl (\d+)", kopf)
    name = os.path.basename(fn)[:-9]
    if not md:                                                     # Kopf ohne d: aus dem Namen
        md_val = 7 if name.startswith("k1b") else (9 if name.startswith(("c_", "s2_")) else 8)
    else: md_val = int(md.group(1))
    soll = int(mr.group(1)) if mr else 0
    ist = offen(U, W, md_val)
    if ist != soll: ab += 1
    zeilen.append((name, U.shape[0], U.shape[1], W.shape[1], md_val, soll, ist, gram(U), gram(W)))
for z in zeilen:
    print("%-22s K=%2d (%2d,%2d) d=%d  Kopf %6d  gezaehlt %6d  %s   |U-Gram| %s  |W-Gram| %s" % (z[0], z[1], z[2], z[3], z[4], z[5], z[6], "gleich" if z[5] == z[6] else "ABWEICHUNG", z[7], z[8]))
print("Paare: %d, Abweichungen: %d, geloest (0 offen): %d" % (len(zeilen), ab, sum(1 for z in zeilen if z[6] == 0)))
