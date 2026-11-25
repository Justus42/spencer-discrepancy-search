#!/usr/bin/env python3
"""Pruefcode Auftrag S (teilt keinen Code mit spann_zaehl.py / spann_dfs.c / spann_ls.c).

  spann_pruef.py radius MATRIX            Ueberdeckungsradius von Zeilen und Komplementen (Breitensuche ueber alle 2^m Punkte)
  spann_pruef.py matrix WOERTER m ZIEL    schreibt eine +-1-Matrix aus Wortzahlen (Bit j = 1 -> Eintrag -1 in Spalte j)

Nur ganze Zahlen. Nullspalten der Matrix werden ignoriert (sie aendern den Radius nicht, nur m).
"""
import sys
import numpy as np


def radius(pfad):
    A = [[int(t) for t in z.split()] for z in open(pfad) if z.strip()]
    spalten = [j for j in range(len(A[0])) if any(r[j] != 0 for r in A)]
    m = len(spalten)
    for r in A:
        assert all(r[j] in (1, -1) for j in spalten), "Eintrag ausser +-1 in einer Nicht-Nullspalte"
    W = sorted({sum(1 << i for i, j in enumerate(spalten) if r[j] < 0) for r in A})
    alle = (1 << m) - 1
    Q = sorted(set(W) | {w ^ alle for w in W})
    dist = np.full(1 << m, -1, dtype=np.int16)
    front = np.array(Q, dtype=np.int64)
    dist[front] = 0
    t = 0
    while True:
        nb = np.unique(np.concatenate([front ^ (1 << j) for j in range(m)]))
        nb = nb[dist[nb] < 0]
        if nb.size == 0:
            break
        t += 1
        dist[nb] = t
        front = nb
    assert (dist >= 0).all()
    print(f"{pfad}: {len(A)} Zeilen, m={m}, verschiedene Woerter mit Komplementen {len(Q)}, Ueberdeckungsradius {int(dist.max())}, "
          f"Punkte im Abstand max: {int((dist == dist.max()).sum())}")
    return int(dist.max())


def matrix(woerter, m, ziel):
    W = [int(x) for x in woerter.replace(',', ' ').split()]
    with open(ziel, 'w') as f:
        for w in W:
            f.write(" ".join("-1" if (w >> j) & 1 else "1" for j in range(m)) + "\n")
    print(f"{ziel}: {len(W)} x {m}")


if __name__ == '__main__':
    if sys.argv[1] == 'radius':
        radius(sys.argv[2])
    elif sys.argv[1] == 'matrix':
        matrix(sys.argv[2], int(sys.argv[3]), sys.argv[4])
