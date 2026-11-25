#!/usr/bin/env python3
"""gate_evo.py -- Gate zu Auftrag E (Orchestrator). Eigener Zaehler, kein Code der Sitzung.
Fuer jede Zeile des Archivs runs/evo/archiv.tsv die Ausgabedatei runs/evo/out/rNNN_PROG_mMRrkK_sS.txt lesen und die offenen Klassen
neu zaehlen: Klasse {x,-x} offen, wenn fuer alle Zeilen |<Zeile, x>| < m - 2R (kein Wort und kein Komplement im Abstand <= R).
Alle 2^(m-1) Klassen, ganze Zahlen. Zusatz: Betraege der Zeilen-Skalarprodukte fuer geloeste Laeufe.
Aufruf: gate_evo.py ARCHIV OUTVERZEICHNIS"""
import sys, os, numpy as np
from collections import Counter
X = {}
def klassen(m):
    if m not in X:
        a = np.arange(1 << (m - 1), dtype=np.int64)
        M = 1 - 2 * ((a[:, None] >> np.arange(m - 1)[None, :]) & 1)
        X[m] = np.hstack([np.ones((len(a), 1), dtype=np.int64), M]).astype(np.int8)
    return X[m]
def offen(A, m, R):
    Xm = klassen(m); d = m - 2 * R; n = 0
    for lo in range(0, len(Xm), 1 << 16):
        n += int((np.abs(Xm[lo:lo + (1 << 16)].astype(np.int16) @ A.T.astype(np.int16)).max(axis=1) < d).sum())
    return n
zeilen = [l.rstrip("\n").split("\t") for l in open(sys.argv[1])][1:]
ab = fehl = 0; nach = Counter()
for z in zeilen:
    rn, prog, elter, insel, m, R, k, s, sek, soll = int(z[0]), z[1], z[2], z[3], int(z[4]), int(z[5]), int(z[6]), int(z[7]), z[8], int(z[9])
    fn = os.path.join(sys.argv[2], "r%03d_%s_m%dR%dk%d_s%d.txt" % (rn, prog, m, R, k, s))
    if not os.path.exists(fn): fehl += 1; print("FEHLT", fn); continue
    A = np.array([[int(t) for t in l.split()[:m]] for l in open(fn) if l.split() and not l.startswith("#")], dtype=np.int64)
    assert A.shape == (k, m) and set(np.unique(A)) <= {-1, 1}, (fn, A.shape)
    ist = offen(A, m, R)
    if ist != soll: ab += 1; print("ABWEICHUNG", fn, soll, ist)
    nach[(m, k, ist)] += 1
    if ist == 0:
        G = np.abs(A @ A.T); iu = np.triu_indices(k, 1)
        print("geloest: Runde %d %s (%s) m=%d Paare=%d Startwert %d; |Zeilenprodukte| %s" % (rn, prog, insel, m, k, s, dict(sorted(Counter(G[iu].tolist()).items()))))
print("Laeufe im Archiv: %d, Dateien fehlen: %d, Abweichungen: %d" % (len(zeilen), fehl, ab))
for (m, k) in sorted({(a, b) for (a, b, c) in nach}):
    print("  m=%d, %d Paare: " % (m, k) + ", ".join("%d x %d" % (c, nach[(m, k, c)]) for c in sorted(c for (a, b, c) in nach if (a, b) == (m, k))))
