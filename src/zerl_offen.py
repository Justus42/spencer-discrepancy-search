#!/usr/bin/env python3
"""Pruefcode Auftrag H (25.11.2026): Kern-<1,a>-Codes als Paar (U,W). Nur ganze Zahlen, kein Suchcode.

Paardatei: K Zeilen, je "U_i | W_i" (Eintraege +1/-1, Leerraum-getrennt, Trennstrich '|').
Woerter des Codes: (U_i, W_i), (U_i, -W_i) und deren Komplemente (4K Woerter). Mit u = U y, v = W z:
    Ueberdeckung mit d  <=>  fuer alle y, z:  max_i (|u_i| + |v_i|) >= d.
Gezaehlt werden Klassen (y,z) mit y_0 = z_0 = +1 (2^(m1-1) * 2^(m2-1) Stueck).

Aufrufe:
  zerl_offen.py offen PAAR D [--direkt]      offene Klassen; --direkt zaehlt zusaetzlich direkt auf der 2K x (m1+m2)-Matrix
  zerl_offen.py matrix PAAR AUSGABE          schreibt die 2K x (m1+m2)-Matrix (Zeilen (U_i,W_i), (U_i,-W_i)) fuer pruefe_matrix.py
  zerl_offen.py profile PAAR D               Profile |U y| und |W z| (sortiert), kritische Klassen, Anforderungsvektoren
  zerl_offen.py schachtel UDATEI WDATEI D    offene Klassen fuer alle K! Zeilenzuordnungen (U_i, W_pi(i)); U, W als einfache Matrixdateien
  zerl_offen.py zerlege MATRIX               sucht in einer Vorzeichenmatrix (Zeilen = Woerter bis auf Komplement) einen Verschiebungskern
                                             {0,a,1+a,1} und schreibt das Paar (U,W) nach stdout (Spalten nach a geteilt)
"""
import itertools
import sys
from collections import Counter

import numpy as np


def lies_matrix(pfad):
    zeilen = []
    for roh in open(pfad, encoding="utf-8"):
        roh = roh.strip()
        if not roh or roh.startswith("#"):
            continue
        zeilen.append([int(t) for t in roh.split()])
    A = np.array(zeilen, dtype=np.int64)
    assert A.ndim == 2 and len({len(z) for z in zeilen}) == 1
    return A


def lies_paar(pfad):
    U, W = [], []
    for roh in open(pfad, encoding="utf-8"):
        roh = roh.strip()
        if not roh or roh.startswith("#"):
            continue
        links, rechts = roh.split("|")
        U.append([int(t) for t in links.split()])
        W.append([int(t) for t in rechts.split()])
    U = np.array(U, dtype=np.int64)
    W = np.array(W, dtype=np.int64)
    assert U.ndim == 2 and W.ndim == 2 and U.shape[0] == W.shape[0]
    assert set(np.unique(U)) <= {-1, 1} and set(np.unique(W)) <= {-1, 1}
    return U, W


def punkte(m):
    """Alle x in {+1,-1}^m mit x_0 = +1, als (2^(m-1)) x m."""
    n = 1 << (m - 1)
    X = np.ones((n, m), dtype=np.int64)
    idx = np.arange(n, dtype=np.int64)
    for j in range(1, m):
        X[:, j] = 1 - 2 * ((idx >> (j - 1)) & 1)
    return X


def offen(U, W, d):
    au = np.abs(punkte(U.shape[1]) @ U.T)          # 2^(m1-1) x K
    av = np.abs(punkte(W.shape[1]) @ W.T)          # 2^(m2-1) x K
    tot = 0
    schritt = max(1, (1 << 22) // max(1, av.shape[0] * U.shape[0]))
    for s in range(0, au.shape[0], schritt):
        blk = au[s:s + schritt]
        summe = blk[:, None, :] + av[None, :, :]   # y x z x K
        tot += int((summe.max(axis=2) < d).sum())
    return tot


def matrix(U, W):
    zeilen = []
    for i in range(U.shape[0]):
        zeilen.append(np.concatenate([U[i], W[i]]))
        zeilen.append(np.concatenate([U[i], -W[i]]))
    return np.array(zeilen, dtype=np.int64)


def offen_direkt(M, d):
    """Zweiter Weg: x in {+-1}^n mit x_0 = +1; offen, wenn max_r |<M_r, x>| < d. Zaehlt Punkte (ganz, nicht Klassen)."""
    n = M.shape[1]
    tot = 0
    rest = n - 1
    hoch = max(0, rest - 16)              # obere Bits in einer Python-Schleife
    tief = rest - hoch
    Xt = punkte(tief + 1)[:, 1:]          # 2^tief x tief
    for h in range(1 << hoch):
        oben = np.array([1 - 2 * ((h >> b) & 1) for b in range(hoch)], dtype=np.int64)
        basis = M[:, 0] + (M[:, 1 + tief:] @ oben if hoch else 0)
        S = Xt @ M[:, 1:1 + tief].T + basis[None, :]
        tot += int((np.abs(S).max(axis=1) < d).sum())
    return tot


def schreibe_matrix(pfad, A):
    with open(pfad, "w", encoding="utf-8") as f:
        for z in A:
            f.write(" ".join(f"{int(v):2d}" for v in z) + "\n")


def profil_text(p):
    teile = []
    for wert, anz in sorted(Counter(p).items(), key=lambda t: -t[0]):
        teile.append(f"{wert}" if anz == 1 else f"{wert}^{anz}")
    return "(" + ",".join(teile) + ")"


def profile(A, B, d, name):
    """Profile |A x| fuer x mit x_0=+1; kritisch, wenn jede Anforderung d-|a_i| groesser als der kleinste Wert von |B x'| ist."""
    au = np.abs(punkte(A.shape[1]) @ A.T)
    vmin = B.shape[1] % 2                 # kleinster moeglicher Betrag auf der Gegenseite (Paritaet)
    c = Counter(tuple(sorted((int(t) for t in a), reverse=True)) for a in au)
    print(f"{name}: {A.shape[0]} x {A.shape[1]}, {au.shape[0]} Klassen; kleinster Betrag der Gegenseite {vmin}")
    krit_vek = set()
    for p, n in sorted(c.items(), key=lambda t: (-t[1], t[0])):
        krit = all(d - t > vmin for t in p)
        print(f"   {profil_text(p)}: {n}{'  KRITISCH' if krit else ''}")
    for a in au:
        r = tuple(int(d - t) for t in a)
        if all(t > vmin for t in r):
            krit_vek.add(r)
    print(f"   kritische Klassen: {sum(1 for a in au if all(d - int(t) > vmin for t in a))}, verschiedene Anforderungsvektoren: {len(krit_vek)}")
    sortiert = Counter(tuple(sorted(r)) for r in krit_vek)
    for r, n in sorted(sortiert.items()):
        print(f"   Anforderung (sortiert) {r}: {n} Vektoren")


def schachtel(U, W, d):
    K = U.shape[0]
    au = np.abs(punkte(U.shape[1]) @ U.T)
    av = np.abs(punkte(W.shape[1]) @ W.T)
    ny, nz = au.shape[0], av.shape[0]
    B = [[np.packbits((au[:, i][:, None] + av[:, j][None, :] >= d).ravel()) for j in range(K)] for i in range(K)]
    tab = np.array([bin(k).count("1") for k in range(256)], dtype=np.int64)
    zaehl = Counter()
    for pi in itertools.permutations(range(K)):
        o = B[0][pi[0]].copy()
        for i in range(1, K):
            o |= B[i][pi[i]]
        zaehl[ny * nz - int(tab[o].sum())] += 1
    return zaehl, ny * nz


def zerlege(M):
    """M: Zeilen = Woerter (bis auf Komplement), Eintraege +-1. Sucht a (Spaltenmenge) mit: zu jeder Zeile x ist x*a
    (Vorzeichen auf a umgedreht) bis auf Vorzeichen wieder eine Zeile. Gibt alle solchen a zurueck (ohne leere und volle Menge)."""
    N, n = M.shape
    zeilen = {tuple(int(t) for t in z) for z in M} | {tuple(int(-t) for t in z) for z in M}
    gefunden = []
    # Kandidaten fuer a: Spaltenmengen, auf denen zwei Zeilen uebereinstimmen bzw. sich unterscheiden (x*x' bestimmt a bis auf Komplement)
    kand = set()
    for i in range(N):
        for j in range(N):
            if i == j:
                continue
            diff = tuple(int(c) for c in np.nonzero(M[i] != M[j])[0])
            kand.add(diff)
    for a in sorted(kand, key=len):
        if len(a) == 0 or len(a) == n:
            continue
        f = np.ones(n, dtype=np.int64)
        f[list(a)] = -1
        if all(tuple(int(t) for t in (z * f)) in zeilen for z in M):
            gefunden.append(a)
    return gefunden


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(2)
    modus = sys.argv[1]
    if modus == "offen":
        U, W = lies_paar(sys.argv[2])
        d = int(sys.argv[3])
        o = offen(U, W, d)
        print(f"{sys.argv[2]}: K={U.shape[0]}, Typ ({U.shape[1]},{W.shape[1]}), d={d}: offene Klassen {o} von {(1 << (U.shape[1] - 1)) * (1 << (W.shape[1] - 1))}")
        if "--direkt" in sys.argv:
            M = matrix(U, W)
            od = offen_direkt(M, d)
            # direkt zaehlt Punkte x mit x_0=+1 in {+-1}^(m1+m2): je Klasse (y,z) zwei Punkte (z und -z), also od = 2*o
            print(f"   direkt auf der {M.shape[0]} x {M.shape[1]}-Matrix: offene Punkte (x_0=+1) {od}, halbiert {od // 2}; "
                  f"{'stimmt' if od == 2 * o else 'ABWEICHUNG'}")
            if od != 2 * o:
                sys.exit(1)
    elif modus == "matrix":
        U, W = lies_paar(sys.argv[2])
        M = matrix(U, W)
        schreibe_matrix(sys.argv[3], M)
        print(f"{sys.argv[3]}: {M.shape[0]} x {M.shape[1]}")
    elif modus == "profile":
        U, W = lies_paar(sys.argv[2])
        d = int(sys.argv[3])
        profile(U, W, d, "U (|U y|)")
        profile(W, U, d, "W (|W z|)")
    elif modus == "schachtel":
        U = lies_matrix(sys.argv[2])
        W = lies_matrix(sys.argv[3])
        d = int(sys.argv[4])
        zaehl, tot = schachtel(U, W, d)
        print(f"U {U.shape}, W {W.shape}, d={d}: offene Klassen ueber alle {sum(zaehl.values())} Zuordnungen (von {tot}): "
              + ", ".join(f"{k}: {v} Zuordnungen" for k, v in sorted(zaehl.items())))
    elif modus == "zerlege":
        M = lies_matrix(sys.argv[2])
        assert set(np.unique(M)) <= {-1, 1}
        kerne = zerlege(M)
        print(f"# {sys.argv[2]}: {M.shape[0]} x {M.shape[1]}; Kernelemente a (Spaltenmengen): {[len(a) for a in kerne]}")
        for a in kerne:
            print(f"#   a = {list(a)}")
        if kerne:
            a = list(kerne[0])
            rest = [j for j in range(M.shape[1]) if j not in a]
            f = np.ones(M.shape[1], dtype=np.int64)
            f[a] = -1
            # je Nebenklasse {x, x*a, -x, -x*a} eine Zeile; normieren: erster Eintrag des U-Teils +1
            gesehen = set()
            print(f"# Paar: U = Spalten {rest}, W = Spalten {a}")
            for z in M:
                kl = {tuple(int(t) for t in z), tuple(int(t) for t in z * f), tuple(int(-t) for t in z), tuple(int(-t) for t in z * f)}
                schl = min(kl)
                if schl in gesehen:
                    continue
                gesehen.add(schl)
                zz = z if z[rest[0]] == 1 else -z
                print(" ".join(f"{int(zz[j]):2d}" for j in rest) + " | " + " ".join(f"{int(zz[j]):2d}" for j in a))
    else:
        print(__doc__)
        sys.exit(2)


if __name__ == "__main__":
    main()
