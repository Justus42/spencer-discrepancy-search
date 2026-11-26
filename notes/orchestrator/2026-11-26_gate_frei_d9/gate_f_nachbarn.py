#!/usr/bin/env python3
"""gate_f_nachbarn.py -- Gate zu Auftrag F (Orchestrator, 26.11.2026). Eigener Zaehler wie gate_f.py.
Fuer eine Matrix alle Nachbarn mit genau einem umgedrehten Eintrag zaehlen (k*m Faelle, vollstaendig):
Wie viele haben weniger offene Klassen, wie viele gleich viele, was ist der beste Wert?
Aufruf: gate_f_nachbarn.py MATRIX R [ZEILE_WEG]   (ZEILE_WEG 1-basiert: diese Zeile vorher streichen)"""
import sys
import numpy as np
M = np.array([[int(v) for v in l.split()] for l in open(sys.argv[1]) if l.strip()], dtype=np.int8); R = int(sys.argv[2])
if len(sys.argv) > 3: M = np.delete(M, int(sys.argv[3]) - 1, axis=0)
k, m = M.shape; d = m - 2 * R
def maske(u):
    a = np.array([u[0]], dtype=np.int8)
    for j in range(1, m): a = np.concatenate((a + u[j], a - u[j]))
    return np.abs(a) < d
z = [maske(u) for u in M]
vor = [None] * (k + 1); nach = [None] * (k + 1)          # Praefix- und Suffix-Schnitte
vor[0] = np.ones(1 << (m - 1), dtype=bool); nach[k] = vor[0]
for i in range(k): vor[i + 1] = vor[i] & z[i]
for i in range(k - 1, -1, -1): nach[i] = nach[i + 1] & z[i]
basis = int(vor[k].sum()); besser = gleich = 0; best = None
for i in range(k):
    andere = vor[i] & nach[i + 1]
    for j in range(m):
        u = M[i].copy(); u[j] = -u[j]; w = int((andere & maske(u)).sum())
        besser += w < basis; gleich += w == basis
        if best is None or w < best[0]: best = (w, i + 1, j + 1)
print('%s%s: %d x %d, R=%d, offen %d; %d Nachbarn mit einem umgedrehten Eintrag: %d besser, %d gleich, bester %d (Zeile %d, Spalte %d)'
      % (sys.argv[1].split('/')[-1], ' ohne Zeile ' + sys.argv[3] if len(sys.argv) > 3 else '', k, m, R, basis, k * m, besser, gleich, best[0], best[1], best[2]))
