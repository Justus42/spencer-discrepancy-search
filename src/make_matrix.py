"""Woerter-Datei (selbstkomplementaer, Laenge n) -> Matrix (je ein Wort pro Paar) und mit Nullspalten
auf quadratisch aufgefuellte Variante fuer verify_spencer.py. Aufruf: make_matrix.py n woerter.txt ausgabe_praefix"""
import sys
n = int(sys.argv[1]); W = list(map(int, open(sys.argv[2]).read().split())); full = (1 << n) - 1
S = set(W); assert len(S) == len(W) and all(w ^ full in S for w in W)
rows = sorted(w for w in W if not (w >> (n - 1)) & 1); N = len(rows); assert 2 * N == len(W)
def line(c, pad): return " ".join(f"{-1 if (c >> j) & 1 else 1:2d}" for j in range(n)) + "  0" * pad
open(sys.argv[3] + ".txt", "w").write("\n".join(line(c, 0) for c in rows) + "\n")
if N > n: open(sys.argv[3] + "_pad.txt", "w").write("\n".join(line(c, N - n) for c in rows) + "\n")
