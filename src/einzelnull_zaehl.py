#!/usr/bin/env python3
"""Auftrag A, Teil 1: Gegenrechnung der Angaben zu Zeilen mit einer Null (n = 11, d = 6).

Nur ganze Zahlen. Punkte modulo Vorzeichen: x in {+-1}^n mit x_0 = +1.
Zeile a trifft x, wenn |<a,x>| >= d.
Aufruf: python3 src/einzelnull_zaehl.py
"""
import itertools
import sys
from collections import Counter

import numpy as np


def punkte(n):
    """Alle x mit x_0 = +1 (Vertreter modulo Vorzeichen)."""
    out = []
    for bits in range(1 << (n - 1)):
        x = [1] + [(-1 if (bits >> k) & 1 else 1) for k in range(n - 1)]
        out.append(x)
    return out


def zeilen(n, traeger):
    """Alle Zeilen mit genau `traeger` Einträgen +-1, modulo Vorzeichen (erster Nicht-Null-Eintrag +1)."""
    out = []
    for supp in itertools.combinations(range(n), traeger):
        for bits in range(1 << (traeger - 1)):
            a = [0] * n
            a[supp[0]] = 1
            for k, j in enumerate(supp[1:]):
                a[j] = -1 if (bits >> k) & 1 else 1
            out.append(tuple(a))
    return out


def treffer_alle(Z, P, d):
    """Trefferm. aller Zeilen als Bitmasken (Python-int), ganzzahlig per numpy int64."""
    A = np.array(Z, dtype=np.int64)
    X = np.array(P, dtype=np.int64)
    S = np.abs(A @ X.T) >= d
    gew = [1 << i for i in range(len(P))]
    out = []
    for zeile in S:
        m = 0
        for i in np.nonzero(zeile)[0]:
            m |= gew[i]
        out.append(m)
    return out


def main():
    n, d = 11, 6
    P = punkte(n)
    print(f"Klassen: {len(P)}")
    erg = {}
    for t in (11, 10, 9, 8, 7, 6, 5):
        Z = zeilen(n, t)
        groessen = Counter(bin(h).count("1") for h in treffer_alle(Z, P, d))
        print(f"Traeger {t}: {len(Z)} Zeilen, Treffergroessen {dict(groessen)}")
        erg[t] = (len(Z), groessen)
    # Schnittgroessen fuer a = (0,1,...,1)
    Z10 = zeilen(n, 10)
    H = dict(zip(Z10, treffer_alle(Z10, P, d)))
    a0 = tuple([0] + [1] * 10)
    h0 = H[a0]
    tab = Counter()
    for b in Z10:
        if b == a0:
            continue
        s = bin(h0 & H[b]).count("1")
        if b[0] == 0:
            dist = sum(1 for j in range(1, n) if b[j] != a0[j])
            typ = ("gleiche Spalte", min(dist, 10 - dist))
        else:
            z = b.index(0)
            common = [j for j in range(1, n) if j != z]
            dist = sum(1 for j in common if b[j] != a0[j])
            typ = ("andere Spalte", min(dist, 9 - dist))
        tab[(typ, s)] += 1
    for (typ, s), c in sorted(tab.items()):
        print(f"  {typ[0]:15s} Abstand {typ[1]}: Schnitt {s:3d}, Anzahl {c}")
    print(f"Schnittgroessen gesamt: {sorted(set(s for (_, s) in tab))}")
    print(f"Budget 11*112-1024 = {11 * 112 - 1024}")
    # Dominanz ungerader Traeger: Trefferm. von a (Traeger t ungerade) in der von a ohne Eintrag j
    for t in (11, 9, 7):
        Z = zeilen(n, t)
        Hz = treffer_alle(Z, P, d)
        kl = zeilen(n, t - 1)
        Hk = dict(zip(kl, treffer_alle(kl, P, d)))
        ok = True
        for a, ha in zip(Z, Hz):
            for j in range(n):
                if a[j] != 0:
                    b = list(a)
                    b[j] = 0
                    s0 = next(v for v in b if v != 0)
                    b = tuple(v * s0 for v in b)  # Vorzeichen normieren
                    if ha & ~Hk[b]:
                        ok = False
        print(f"Traeger {t}: jede Trefferm. in der Trefferm. jeder Zeile mit einem Eintrag weniger: {ok}")


if __name__ == "__main__":
    sys.exit(main())
