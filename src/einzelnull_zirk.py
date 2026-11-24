#!/usr/bin/env python3
"""Auftrag A, Teil 1c: Zirkulante n x n mit Nulldiagonale, Einträge sonst +-1.

Klassen: erste Zeile c_1..c_{n-1} modulo Multiplikatoren t -> u t (ggT(u,n)=1) und Negation.
Diskrepanz exakt über alle x mit x_0 = +1 (ganzzahlig, numpy int64).
Aufruf: python3 src/einzelnull_zirk.py N [N ...]   (schreibt beste Matrix nach runs/einzelnull/zirk_N.txt)
"""
import sys
from math import gcd

import numpy as np


def klassen(n):
    us = [u for u in range(1, n) if gcd(u, n) == 1]
    gesehen = set()
    vertreter = []
    for bits in range(1 << (n - 1)):
        if bits in gesehen:
            continue
        c = [0] + [(-1 if (bits >> (k - 1)) & 1 else 1) for k in range(1, n)]
        bahn = set()
        for u in us:
            for s in (1, -1):
                cc = [0] * n
                for k in range(1, n):
                    cc[(u * k) % n] = s * c[k]
                b = sum(1 << (k - 1) for k in range(1, n) if cc[k] == -1)
                bahn.add(b)
        gesehen |= bahn
        vertreter.append(c)
    return vertreter


def main():
    for n in map(int, sys.argv[1:]):
        V = klassen(n)
        X = np.array([[1] + [(-1 if (b >> k) & 1 else 1) for k in range(n - 1)]
                      for b in range(1 << (n - 1))], dtype=np.int64)
        beste, bc = -1, None
        hist = {}
        for c in V:
            A = np.array([[c[(j - i) % n] for j in range(n)] for i in range(n)], dtype=np.int64)
            disc = int(np.abs(X @ A.T).max(axis=1).min())
            hist[disc] = hist.get(disc, 0) + 1
            if disc > beste:
                beste, bc = disc, A
        print(f"n={n}: {len(V)} Klassen, Maximum {beste}, Verteilung {dict(sorted(hist.items()))}")
        with open(f"runs/einzelnull/zirk_{n}.txt", "w") as f:
            for row in bc:
                f.write(" ".join(f"{int(v):2d}" for v in row) + "\n")


if __name__ == "__main__":
    main()
