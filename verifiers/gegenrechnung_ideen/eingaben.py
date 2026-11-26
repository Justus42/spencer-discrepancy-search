#!/usr/bin/env python3
"""Auftrag C (Sitzung, 26.11.2026): Eingaben fuer die Gegenrechnung. Prueft nur, sucht nichts.

Erzeugt Sylvester 8/16/32, Paley I 12, Paley II 28, Paley-Konferenzmatrizen 26/30, liest controls/
(ctrl_16 mit ergaenzter Spalte, had20_klasse1..3, had24_mckay_hex) und prueft jede Matrix mit ganzen Zahlen
auf H H^T = N I (Konferenz: C C^T = (n-1) I). Schreibt Dateien im Format
    ANZAHL ZEILEN SPALTEN
    (je Matrix ZEILEN Textzeilen mit Eintraegen -1/0/1)
nach ARBEITSORDNER. Eigener Code, keine Zeile aus notes/orchestrator.
Aufruf: python3 eingaben.py REPO ARBEITSORDNER
"""
import sys
from pathlib import Path


def orth(M, diag):
    n = len(M)
    for i in range(n):
        for j in range(n):
            s = sum(a * b for a, b in zip(M[i], M[j]))
            if s != (diag if i == j else 0):
                return False
    return True


def transp(M):
    return [list(r) for r in zip(*M)]


def sylvester(n):
    H = [[1]]
    while len(H) < n:
        H = [r + r for r in H] + [r + [-v for v in r] for r in H]
    return H


def chi_prim(q):
    sq = {(i * i) % q for i in range(1, q)}
    return lambda z: 0 if z % q == 0 else (1 if z % q in sq else -1)


def paley1(q):
    """q = 3 mod 4 Primzahl: H = I + S mit S = [[0, 1^T], [-1, Q]], Q_ij = chi(j - i)."""
    chi = chi_prim(q)
    n = q + 1
    S = [[0] * n for _ in range(n)]
    for j in range(1, n):
        S[0][j] = 1
        S[j][0] = -1
    for i in range(q):
        for j in range(q):
            S[i + 1][j + 1] = chi(j - i)
    return [[S[i][j] + (1 if i == j else 0) for j in range(n)] for i in range(n)]


def konferenz(elemente, minus, chi):
    """Symmetrische Paley-Konferenzmatrix (q = 1 mod 4): [[0, 1^T], [1, Q]], Q_ij = chi(e_j - e_i)."""
    q = len(elemente)
    n = q + 1
    C = [[0] * n for _ in range(n)]
    for j in range(1, n):
        C[0][j] = 1
        C[j][0] = 1
    for i in range(q):
        for j in range(q):
            C[i + 1][j + 1] = chi(minus(elemente[j], elemente[i]))
    return C


def gf25():
    # GF(25) = GF(5)[t]/(t^2 - 2); 2 ist kein Quadrat mod 5
    el = [(a, b) for a in range(5) for b in range(5)]
    def mul(x, y):
        a, b = x; c, d = y
        return ((a * c + 2 * b * d) % 5, (a * d + b * c) % 5)
    quad = {mul(x, x) for x in el if x != (0, 0)}
    assert len(quad) == 12
    chi = lambda z: 0 if z == (0, 0) else (1 if z in quad else -1)
    minus = lambda x, y: ((x[0] - y[0]) % 5, (x[1] - y[1]) % 5)
    return el, minus, chi


def paley2(q):
    """q = 1 mod 4 Primzahl: H = C (x) [[1,1],[1,-1]] + I (x) [[1,-1],[-1,-1]], Ordnung 2(q+1)."""
    chi = chi_prim(q)
    C = konferenz(list(range(q)), lambda a, b: (a - b) % q, chi)
    n = q + 1
    H = [[0] * (2 * n) for _ in range(2 * n)]
    A = [[1, 1], [1, -1]]
    B = [[1, -1], [-1, -1]]
    for i in range(n):
        for j in range(n):
            for a in range(2):
                for b in range(2):
                    H[2 * i + a][2 * j + b] = C[i][j] * A[a][b] + (B[a][b] if i == j else 0)
    return H


def lies(p):
    return [[int(t) for t in z.split()] for z in Path(p).read_text().splitlines() if z.strip()]


def mckay(p):
    out = []
    for z in Path(p).read_text().splitlines():
        if not z.strip():
            continue
        rows = []
        for h in z.split():
            v = int(h, 16)
            rows.append([1 if (v >> (23 - k)) & 1 else -1 for k in range(24)])
        out.append(rows)
    return out


def schreibe(pfad, mats):
    r, s = len(mats[0]), len(mats[0][0])
    with open(pfad, "w") as f:
        f.write(f"{len(mats)} {r} {s}\n")
        for M in mats:
            assert len(M) == r and all(len(z) == s for z in M)
            for z in M:
                f.write(" ".join(str(v) for v in z) + "\n")


def main():
    repo, aus = Path(sys.argv[1]), Path(sys.argv[2])
    aus.mkdir(parents=True, exist_ok=True)
    c = repo / "controls"
    sets = {}
    sets["sylv8"] = [sylvester(8)]
    sets["paley12"] = [paley1(11)]
    sets["sylv16"] = [sylvester(16)]
    sets["sylv32"] = [sylvester(32)]
    # ctrl_16: 16 x 16 mit Nullspalte; fehlende Spalte so ergaenzen, dass H orthogonal wird
    C16 = lies(c / "ctrl_16.txt")
    nullsp = [j for j in range(16) if all(z[j] == 0 for z in C16)]
    assert nullsp == [15], nullsp
    # Spalte v mit <v, Spalte j> = 0 fuer alle j < 15: Gram-Rechnung ueber alle 2^16 Vektoren waere teuer;
    # stattdessen: fuer eine Hadamard-Ergaenzung ist v_i bis auf Vorzeichen eindeutig; Zeilenorthogonalitaet
    # der Zeilen auf 15 Spalten ist <r_i, r_k> = -v_i v_k, also v_k = -v_0 * <r_0, r_k> fuer k > 0.
    v = [1] + [-sum(a * b for a, b in zip(C16[0][:15], C16[k][:15])) for k in range(1, 16)]
    assert all(abs(t) == 1 for t in v), v
    H16 = [C16[i][:15] + [v[i]] for i in range(16)]
    sets["ctrl16voll"] = [H16, transp(H16)]
    sets["had20"] = []
    for k in (1, 2, 3):
        H = lies(c / f"had20_klasse{k}.txt")
        sets["had20"] += [H, transp(H)]
    M = mckay(c / "had24_mckay_hex.txt")
    assert len(M) == 60
    sets["had24"] = M + [transp(H) for H in M]
    sets["paley2_28"] = [paley2(13)]
    el, mi, ch = gf25()
    sets["konf26"] = [konferenz(el, mi, ch)]
    sets["konf30"] = [konferenz(list(range(29)), lambda a, b: (a - b) % 29, chi_prim(29))]
    for name, mats in sets.items():
        n = len(mats[0])
        diag = n - 1 if name.startswith("konf") else n
        ok = all(orth(H, diag) for H in mats)
        sym = all(H == transp(H) for H in mats)
        print(f"{name}: {len(mats)} Matrizen {n}x{len(mats[0][0])}, orthogonal (Diagonale {diag}): {ok}, symmetrisch: {sym}")
        assert ok
        schreibe(aus / f"{name}.txt", mats)
    # Kontrollen fuer Schritt 0 und die freie Zeile (32er ohne Zeile)
    for f in ("ctrl_8", "ctrl_16", "had20_klasse1", "had24_1p3_paley12"):
        schreibe(aus / f"k_{f}.txt", [lies(c / f"{f}.txt")])
    print("geschrieben nach", aus)


if __name__ == "__main__":
    main()
