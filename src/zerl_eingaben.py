#!/usr/bin/env python3
"""Eingaben fuer Auftrag H (25.11.2026), nur ganze Zahlen. Schreibt nach runs/zerlegung/eingaben/:
  paar_48.txt        U = Paley-12 ohne Spalte 0 (12 x 11), W = Paley-12 ohne Spalten 0,1,2 (12 x 9)  (wie src/construct_48.py, 1+3)
  paley12_U11.txt    U allein (12 x 11), paley12_W9.txt  W allein (12 x 9)
  paley12_U11_ohne_zeile<i>.txt  U ohne Zeile i (11 x 11), i = 0, 5, 11
  paley12_voll.txt   Paley-12 (12 x 12)
  ham_U11.txt        8 Hamming-Zeilen (controls/ctrl_8.txt, Spalten 0..6), Spalten 0 und 1 verdreifacht (8 x 11)
  ham_W9.txt         8 Hamming-Zeilen, Spalte 0 verdreifacht (8 x 9)
Aufruf aus dem Wurzelordner: python3 src/zerl_eingaben.py
"""
import os

import numpy as np


def paley12():
    q = 11
    sq = {(x * x) % q for x in range(1, q)}
    chi = lambda a: 0 if a % q == 0 else (1 if a % q in sq else -1)
    S = np.zeros((12, 12), dtype=np.int64)
    S[0, 1:] = 1
    S[1:, 0] = -1
    for i in range(q):
        for j in range(q):
            S[i + 1, j + 1] = chi(i - j)
    H = S + np.eye(12, dtype=np.int64)
    assert (H @ H.T == 12 * np.eye(12, dtype=np.int64)).all()
    return H


def schreibe(pfad, A):
    with open(pfad, "w", encoding="utf-8") as f:
        for z in A:
            f.write(" ".join(f"{int(v):2d}" for v in z) + "\n")


def schreibe_paar(pfad, U, W):
    with open(pfad, "w", encoding="utf-8") as f:
        for a, b in zip(U, W):
            f.write(" ".join(f"{int(v):2d}" for v in a) + " | " + " ".join(f"{int(v):2d}" for v in b) + "\n")


if __name__ == "__main__":
    o = "runs/zerlegung/eingaben"
    os.makedirs(o, exist_ok=True)
    H = paley12()
    U, W = H[:, 1:], H[:, 3:]
    schreibe_paar(f"{o}/paar_48.txt", U, W)
    schreibe(f"{o}/paley12_U11.txt", U)
    schreibe(f"{o}/paley12_W9.txt", W)
    schreibe(f"{o}/paley12_voll.txt", H)
    for i in (0, 5, 11):
        schreibe(f"{o}/paley12_U11_ohne_zeile{i}.txt", np.delete(U, i, axis=0))
    Ham = np.array([[int(t) for t in l.split()] for l in open("controls/ctrl_8.txt") if l.strip()], dtype=np.int64)[:, :7]
    t3 = lambda M, cols: np.hstack([M] + [M[:, [c]] for c in cols for _ in range(2)])
    schreibe(f"{o}/ham_U11.txt", t3(Ham, [0, 1]))
    schreibe(f"{o}/ham_W9.txt", t3(Ham, [0]))
    print("geschrieben:", sorted(os.listdir(o)))
