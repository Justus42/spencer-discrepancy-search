#!/usr/bin/env python3
"""gate_18_5.py -- Gate zu Auftrag N (Orchestrator). Eigene Rechnung, kein Code der Sitzung.
Teil A: den Treffer K = 13 bei (18,5) unabhaengig pruefen: Matrix 26 x 18 aus dem Paar (U | W) bauen (Zeilen (U_i, W_i) und (U_i, -W_i)),
        52 Woerter (Zeilen und Negative) als 18-Bit-Zahlen, fuer ALLE 2^18 Punkte den kleinsten Hamming-Abstand zu einem Wort bestimmen.
Teil B: aus jedem Zustand mit K = 12 und Restzahl 1 einen Code mit 50 Woertern bauen: Die eine offene Klasse (y, z) steht fuer die vier
        Punkte (+-y, +-z). Das Wort w = (y, z') mit z' = z an vier Stellen gewechselt hat Abstand 4 zu (y, z) und 5 zu (y, -z);
        -w erledigt die beiden anderen. Also 24 Zeilen des Zustands plus die Zeile w: 25 Zeilen, 50 Woerter. Pruefung wie in Teil A.
Aufruf: gate_18_5.py VERZEICHNIS AUSGABEVERZEICHNIS"""
import sys, glob, os, numpy as np
M_ = 18
PC = np.array([bin(i).count("1") for i in range(1 << 9)], dtype=np.int8)
def lies(fn):
    rows = [l for l in open(fn) if "|" in l and not l.startswith("#")]
    U = np.array([[int(t) for t in l.split("|")[0].split()] for l in rows]); W = np.array([[int(t) for t in l.split("|")[1].split()] for l in rows])
    return U, W
def radius(Mx):
    """groesster Abstand eines Punkts zum naechsten Wort; Woerter = Zeilen und ihre Negative"""
    w = [(int(sum(1 << j for j in range(M_) if r[j] == -1))) for r in Mx]; w = w + [x ^ ((1 << M_) - 1) for x in w]
    pts = np.arange(1 << M_, dtype=np.int64); best = np.full(1 << M_, 99, dtype=np.int8)
    for c in w:
        x = pts ^ c
        best = np.minimum(best, PC[x & 511] + PC[x >> 9])
    return len(set(w)), int(best.max()), int((best > 5).sum())
def schreibe(Mx, fn):
    open(fn, "w").write("\n".join(" ".join("%2d" % v for v in r) for r in Mx) + "\n")
d, aus = sys.argv[1], sys.argv[2]
print("Teil A: Treffer K = 13, Typ (9,9)")
for fn in sorted(glob.glob(d + "/s2c18-5_k13_t9-9_seed*.paar.txt")):
    U, W = lies(fn); Mx = np.vstack([np.hstack([U, W]), np.hstack([U, -W])])
    nw, rad, off = radius(Mx)
    print("  %s: Matrix %d x %d, verschiedene Woerter %d, Ueberdeckungsradius %d, Punkte mit Abstand > 5: %d" % (os.path.basename(fn), Mx.shape[0], Mx.shape[1], nw, rad, off))
    if fn.endswith("seed1.paar.txt"):
        schreibe(Mx, aus + "/gate_c18-5_52w.matrix.txt")
        S = np.array([[int(t) for t in l.split()] for l in open(d + "/c18-5_k13_t9-9.matrix.txt") if l.split() and not l.startswith("#")])
        print("    Zeilenmenge gleich der Matrix der Sitzung:", sorted(map(tuple, Mx.tolist())) == sorted(map(tuple, S.tolist())))
print("Teil B: 50 Woerter aus den Zustaenden mit K = 12 und Restzahl 1")
Y = 1 - 2 * ((np.arange(256)[:, None] >> np.arange(8)[None, :]) & 1); Y = np.hstack([np.ones((256, 1), dtype=np.int64), Y])
erst = True
for fn in sorted(glob.glob(d + "/s*c18-5*_k12_t9-9_seed*.paar.txt")):
    U, W = lies(fn); A = np.abs(Y @ U.T); B = np.abs(Y @ W.T)
    offen = np.argwhere((A[:, None, :] + B[None, :, :]).max(axis=2) < 8)
    if len(offen) != 1: print("  %s: %d offene Klassen, uebersprungen" % (os.path.basename(fn), len(offen))); continue
    y, z = Y[offen[0][0]], Y[offen[0][1]]; z2 = z.copy(); z2[:4] *= -1
    Mx = np.vstack([np.hstack([U, W]), np.hstack([U, -W]), np.hstack([y, z2])[None, :]])
    nw, rad, off = radius(Mx)
    print("  %s: offene Klasse y=%s z=%s -> Matrix %d x %d, verschiedene Woerter %d, Radius %d, Punkte mit Abstand > 5: %d" % (os.path.basename(fn), "".join("+" if v > 0 else "-" for v in y), "".join("+" if v > 0 else "-" for v in z), Mx.shape[0], Mx.shape[1], nw, rad, off))
    if erst and off == 0: schreibe(Mx, aus + "/gate_c18-5_50w.matrix.txt"); erst = False; print("    gespeichert als gate_c18-5_50w.matrix.txt (aus %s)" % os.path.basename(fn))
