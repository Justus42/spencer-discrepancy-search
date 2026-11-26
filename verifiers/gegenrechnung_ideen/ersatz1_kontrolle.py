#!/usr/bin/env python3
"""Kontrolle fuer `gr ersatz1`: an kleinen Zufallsmatrizen (10 x 10, +-1) die Zahl der guten Ersatzspalten direkt zaehlen.
Fuer jede Spalte j und jede neue Spalte c in {+-1}^10 die exakte Diskrepanz der ersetzten Matrix (alle 2^10 x) und zaehlen,
wie oft sie >= Z ist. Bei ungerader Restspaltenzahl ist die Bedingung in gr.c exakt, die Zahlen muessen gleich sein.
Aufruf: python3 ersatz1_kontrolle.py ARBEITSORDNER GR_PROGRAMM"""
import itertools
import subprocess
import sys
from pathlib import Path

import numpy as np

aus, gr = Path(sys.argv[1]), sys.argv[2]
rng = np.random.default_rng(20261126)
N = 10
X = np.array(list(itertools.product([1, -1], repeat=N)), dtype=np.int64)  # alle x
C = np.array(list(itertools.product([1, -1], repeat=N)), dtype=np.int64)  # alle c
alle_gleich = True
for nr in range(3):
    H = rng.choice([-1, 1], size=(N, N))
    p = aus / f"zufall10_{nr}.txt"
    p.write_text(f"1 {N} {N}\n" + "\n".join(" ".join(str(v) for v in z) for z in H) + "\n")
    for Z in (4, 6):
        out = subprocess.run([gr, "ersatz1", str(p), str(Z)], capture_output=True, text=True).stdout
        prog = [int(z.split("(von 2^10): ")[1]) for z in out.splitlines() if "ohne Spalte" in z]
        brute = []
        for j in range(N):
            rest = np.delete(H, j, axis=1)
            B = X[:, :N - 1] @ rest.T            # b fuer jedes x' (erste N-1 Koordinaten von x), Zeilen
            s = X[:, N - 1:N]                    # Vorzeichen der neuen Spalte
            gut = 0
            for c in C:
                S = B + s * c[None, :]
                if np.abs(S).max(axis=1).min() >= Z:
                    gut += 1
            brute.append(gut)
        gleich = prog == brute
        alle_gleich &= gleich
        print(f"Matrix {nr}, Ziel {Z}: gr {prog}  direkt {brute}  gleich: {gleich}")
print("ALLE GLEICH:", alle_gleich)
