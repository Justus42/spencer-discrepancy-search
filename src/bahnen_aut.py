"""Pruefcode Teil A3 (24.11.2026): Ordnung der Gruppe der vorzeichenbehafteten Spaltenpermutationen,
die die Menge S = {Zeilen} u {negierte Zeilen} einer Matrix erhalten (igraph/bliss).

Graph: je Spalte j zwei Literal-Knoten j+ und j- (Farbe 0, Kante j+ -- j-), je Vektor v in S ein
Knoten (Farbe 1), Kante zu j+ falls v_j = +1, zu j- falls v_j = -1. Da S als Menge genommen wird,
haben verschiedene Vektor-Knoten verschiedene Nachbarschaften; die Automorphismengruppe ist damit
genau die gesuchte Gruppe. Ein Automorphismus der Ordnung 5 existiert genau dann, wenn 5 die
Gruppenordnung teilt (Cauchy).
"""
import sys
import igraph as ig
import numpy as np


def gruppenordnung(A):
    A = np.asarray(A, dtype=np.int64)
    m, n = A.shape
    S = sorted({tuple(int(v) for v in z) for z in A} | {tuple(int(-v) for v in z) for z in A})
    kanten = [(2 * j, 2 * j + 1) for j in range(n)]
    farben = [0] * (2 * n) + [1] * len(S)
    for i, v in enumerate(S):
        k = 2 * n + i
        for j, e in enumerate(v):
            if e == 1:
                kanten.append((k, 2 * j))
            elif e == -1:
                kanten.append((k, 2 * j + 1))
            elif e != 0:
                raise ValueError("nur Eintraege -1, 0, 1")
    g = ig.Graph(n=2 * n + len(S), edges=kanten)
    return g.count_automorphisms(color=farben), len(S)


if __name__ == "__main__":
    for d in sys.argv[1:]:
        A = np.loadtxt(d, dtype=np.int64, ndmin=2)
        o, s = gruppenordnung(A)
        print(f"{d}: {A.shape[0]} x {A.shape[1]}, |S| = {s}, Gruppenordnung {o}, "
              f"durch 5 teilbar: {o % 5 == 0} -> {'Automorphismus der Ordnung 5 existiert' if o % 5 == 0 else 'kein Automorphismus der Ordnung 5'}")
