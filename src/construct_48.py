"""Konstruktion der 48-Woerter-Codes ohne Suche (Teil B, 24.11.2026).

H = Paley-Hadamard-Matrix der Ordnung 12 (Typ I, q = 11), M = [[H,H],[H,-H]] (24 x 24).
Bauregel 1+3: links Spalte 0, rechts Spalten 0,1,2 streichen -> 24 x 20, Diskrepanz 8.
Beispiel 2+2: links {0,1}, rechts {0,2} (Ueberlappung 1) -> 24 x 20, Diskrepanz 8.
Nur ganze Zahlen. Schreibt nach controls/ je eine Datei 24 x 20 (check_disc.py) und eine
24 x 24 mit Nullspalten (`_pad`, fuer verify_spencer.py, das m <= n verlangt).

Aufruf (aus dem Wurzelordner): python3 src/construct_48.py
"""
import os
import numpy as np


def paley1(q):
    """Paley-Hadamard-Matrix Typ I der Ordnung q+1 (q Primzahl, q = 3 mod 4)."""
    assert q % 4 == 3
    sq = {(x * x) % q for x in range(1, q)}
    chi = lambda a: 0 if a % q == 0 else (1 if a % q in sq else -1)
    n = q + 1
    S = np.zeros((n, n), dtype=np.int64)
    S[0, 1:] = 1
    S[1:, 0] = -1
    for i in range(q):
        for j in range(q):
            S[i + 1, j + 1] = chi(i - j)
    H = S + np.eye(n, dtype=np.int64)
    assert (H @ H.T == n * np.eye(n, dtype=np.int64)).all()
    return H


def doubling(H):
    """M = [[H,H],[H,-H]]."""
    return np.block([[H, H], [H, -H]])


def streiche(M, links, rechts):
    """Streicht in der linken Haelfte die Spalten `links`, in der rechten `rechts` (0-basiert je Haelfte)."""
    h = M.shape[1] // 2
    weg = set(links) | {h + j for j in rechts}
    keep = [j for j in range(M.shape[1]) if j not in weg]
    return M[:, keep]


def schreibe(pfad, A):
    with open(pfad, "w") as f:
        for z in A:
            f.write(" ".join(f"{int(v):2d}" for v in z) + "\n")


def pad_quadratisch(A):
    z, s = A.shape
    return np.hstack([A, np.zeros((z, z - s), dtype=A.dtype)])


if __name__ == "__main__":
    H = paley1(11)
    M = doubling(H)
    faelle = {
        "had24_1p3_paley12": ([0], [0, 1, 2]),   # 1+3, Ueberlappung (Spalte 0 in beiden)
        "had24_2p2_paley12": ([0, 1], [0, 2]),   # 2+2, Ueberlappung 1 (Spalte 0)
    }
    os.makedirs("controls", exist_ok=True)
    for name, (l, r) in faelle.items():
        A = streiche(M, l, r)
        assert A.shape == (24, 20)
        schreibe(f"controls/{name}.txt", A)
        schreibe(f"controls/{name}_pad.txt", pad_quadratisch(A))
        print(name, "links", l, "rechts", r, "->", A.shape)
