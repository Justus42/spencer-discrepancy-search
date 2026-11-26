#!/usr/bin/env python3
"""typen_fasern.py -- Pruefcode Auftrag K, Schritt 0 b) (eigene Rechnung, teilt keinen Code mit dem Suchcode
oder mit notes/orchestrator/2026-11-26_gegenpruefung/kern3_fasern.py).

Kern D = <1,a,b>, Laenge m, Radius R. Typ = Blockgroessen (n0>=n1>=n2>=n3), mindestens drei Bloecke nichtleer.
Rechenweg (anders als beim Orchestrator): alle 2^m Punkte direkt mit numpy; B_D = Punkte mit Abstand <= R zu D.
B(D) = |B_D| / 8. M[t] = Zahl der Punkte in B_D mit Blockparitaet t. Faser p hat |F_p| Punkte.
K Nebenklassen mit Vertreterparitaeten q_1..q_K koennen nur dann alles decken, wenn fuer jede Faser p
    sum_j M[p xor q_j] >= |F_p|.
Kleinstes K, fuer das ein Multiset q existiert (alle Multisets ueber den moeglichen Paritaeten, ohne q_1 = 0 festzulegen).
Aufruf: typen_fasern.py [m R] ...   (ohne Argumente: (16,4) (18,5) (20,6))
"""
import sys
import itertools
import numpy as np

PC16 = np.array([bin(i).count("1") for i in range(1 << 16)], dtype=np.int32)


def popc(x):
    return PC16[x & 0xFFFF] + PC16[(x >> 16) & 0xFFFF]


def typen(m):
    """Partitionen von m in genau 4 Teile (Nullen erlaubt), absteigend, mindestens 3 Teile > 0 (rekursiv)."""
    aus = []

    def rek(rest, teile, maxv):
        if len(teile) == 4:
            if rest == 0 and sum(1 for t in teile if t > 0) >= 3:
                aus.append(tuple(teile))
            return
        for v in range(min(rest, maxv), -1, -1):
            rek(rest - v, teile + [v], v)

    rek(m, [], m)
    return aus


def analyse(n, R):
    m = sum(n)
    # Blockmasken: Bloecke hintereinander; Block i hat Label (a,b) = (i&1, i>>1)
    masken = []
    off = 0
    for v in n:
        masken.append(((1 << v) - 1) << off)
        off += v
    eins = (1 << m) - 1
    a = masken[1] | masken[3]
    b = masken[2] | masken[3]
    D = [0, a, b, a ^ b]
    D = D + [d ^ eins for d in D]
    x = np.arange(1 << m, dtype=np.int64)
    dmin = np.full(x.shape, 99, dtype=np.int32)
    for d in D:
        dmin = np.minimum(dmin, popc(x ^ d))
    BD = dmin <= R
    nBD = int(BD.sum())
    assert nBD % 8 == 0
    par = np.zeros(x.shape, dtype=np.int64)
    for i, mk in enumerate(masken):
        par |= (popc(x & mk) & 1).astype(np.int64) << i
    M = np.bincount(par[BD], minlength=16)
    F = np.bincount(par, minlength=16)
    moegl = [p for p in range(16) if F[p] > 0]
    kmin = None
    for K in range(1, 9):
        for qs in itertools.combinations_with_replacement(moegl, K):
            if all(sum(int(M[p ^ q]) for q in qs) >= int(F[p]) for p in moegl):
                kmin = K
                break
        if kmin is not None:
            break
    return nBD // 8, kmin


def main():
    args = [int(v) for v in sys.argv[1:]]
    zellen = [(args[i], args[i + 1]) for i in range(0, len(args), 2)] if args else [(16, 4), (18, 5), (20, 6)]
    for (m, R) in zellen:
        T = typen(m)
        erg = {}
        bds = []
        for n in T:
            bd, k = analyse(n, R)
            bds.append(bd)
            erg.setdefault(k, []).append(n)
            print("TYP %d %d %s B(D)=%d K_faser=%s" % (m, R, ",".join(map(str, n)), bd, k), flush=True)
        print("ZELLE (%d,%d): %d Typen, B(D) %d bis %d" % (m, R, len(T), min(bds), max(bds)))
        for k in sorted(erg, key=lambda v: 99 if v is None else v):
            print("   K_faser = %s: %d Typen %s" % (k if k is not None else '>8', len(erg[k]), sorted(erg[k]) if len(erg[k]) <= 12 else ''))
        aus5 = sorted(t for k, v in erg.items() for t in v if k is None or k > 5)
        print("   bei K = 5 ausgeschlossen: %d Typen %s" % (len(aus5), aus5))
        aus6 = sorted(t for k, v in erg.items() for t in v if k is None or k > 6)
        print("   bei K = 6 ausgeschlossen: %d Typen %s" % (len(aus6), aus6))


if __name__ == "__main__":
    main()
