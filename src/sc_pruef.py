#!/usr/bin/env python3
"""Prüfcode zu Teil 4 neu: selbstkomplementärer Überdeckungscode (Wörter als 0/1-Zeichenketten, eine je Zeile).

Prüft: Länge m einheitlich, Menge unter Komplement abgeschlossen, Überdeckungsradius <= R (alle 2^m Punkte, ganzzahlig).
Schreibt die +-1-Matrix (je Paar ein Wort, Bit 1 = -1) nach AUSGABE; deren Diskrepanz ist dann >= m - 2R.
Aufruf: python3 src/sc_pruef.py WOERTER R AUSGABE
"""
import sys

import numpy as np


def main():
    datei, R, aus = sys.argv[1], int(sys.argv[2]), sys.argv[3]
    W = [l.strip() for l in open(datei) if l.strip()]
    m = len(W[0])
    assert all(len(w) == m and set(w) <= {"0", "1"} for w in W), "Format"
    S = set(W)
    komp = {"".join("1" if ch == "0" else "0" for ch in w) for w in S}
    print(f"{len(W)} Woerter, verschieden {len(S)}, Laenge {m}, komplement-abgeschlossen: {komp == S}")
    C = np.array([[int(ch) for ch in w] for w in sorted(S)], dtype=np.int64)
    X = np.array([[(x >> j) & 1 for j in range(m)] for x in range(1 << m)], dtype=np.int64)
    # Hamming-Abstand = sum |X - C|
    dmin = np.full(1 << m, m + 1, dtype=np.int64)
    for c in C:
        dmin = np.minimum(dmin, np.abs(X - c).sum(axis=1))
    rad = int(dmin.max())
    print(f"Ueberdeckungsradius {rad} (gefordert <= {R}): {'OK' if rad <= R and komp == S else 'FEHLER'}")
    paare, gesehen = [], set()
    for w in sorted(S):
        if w in gesehen:
            continue
        k = "".join("1" if ch == "0" else "0" for ch in w)
        gesehen |= {w, k}
        paare.append(w)
    with open(aus, "w") as f:
        for w in paare:
            f.write(" ".join("-1" if ch == "1" else " 1" for ch in w) + "\n")
    print(f"Matrix {len(paare)} x {m} nach {aus}, Diskrepanz-Erwartung >= {m - 2 * R}")
    return 0 if (rad <= R and komp == S) else 1


if __name__ == "__main__":
    sys.exit(main())
