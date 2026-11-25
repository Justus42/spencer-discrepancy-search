#!/usr/bin/env python3
"""Prüfcode zu Auftrag G, Teil 4 (kein Suchcode, nur ganze Zahlen; ohne Rückgriff auf notes/orchestrator/.../kern3.py).

Kern D = <1, a, b> in F_2^20 (8 Wörter). Typ = Größen der vier Spaltenklassen nach (a_i, b_i) in {00,01,10,11}, absteigend sortiert,
mindestens drei Klassen nicht leer (sonst hat D nicht Dimension 3). B(D) = Zahl der Nebenklassen von D mit Minimalgewicht <= 6, also
Zahl der verschiedenen Nebenklassen unter allen 60 460 Vektoren vom Gewicht <= 6. Nebenklasse = Vektor nach Reduktion mit einer
reduzierten Staffelbasis von D (Pivotbits gelöscht), das ist ein eindeutiger Vertreter.
Schranke: 2^17 / B(D) (Zahl der Nebenklassen insgesamt durch B(D)); Vergleich mit 5 (nötige Zahl der Nebenklassen).

Aufruf: python3 src/kern_nebenklassen.py [AUSGABE.csv]
"""
import sys
from fractions import Fraction
from itertools import combinations

L, R = 20, 6


def partitionen(n, teile):
    """absteigende Zerlegungen von n in genau `teile` nichtnegative Summanden"""
    def rek(rest, k, maxv):
        if k == 1:
            if rest <= maxv:
                yield (rest,)
            return
        for v in range(min(rest, maxv), -1, -1):
            for t in rek(rest - v, k - 1, v):
                yield (v,) + t
    return list(rek(n, teile, n))


def basis(typ):
    """Wörter 1, a, b für Klassen 00, 01, 10, 11 mit Größen typ (in dieser Reihenfolge belegt)."""
    pos, start = {}, 0
    for kl, s in enumerate(typ):
        pos[kl] = range(start, start + s)
        start += s
    assert start == L
    eins = (1 << L) - 1
    a = sum(1 << p for kl in (2, 3) for p in pos[kl])      # a_i = 1 in den Klassen 10 und 11
    b = sum(1 << p for kl in (1, 3) for p in pos[kl])      # b_i = 1 in den Klassen 01 und 11
    return [eins, a, b]


def staffel(vs):
    """reduzierte Staffelbasis: Liste (Pivot, Vektor), jedes Pivotbit nur im eigenen Vektor"""
    B = []
    for v in vs:
        for p, w in B:
            if (v >> p) & 1:
                v ^= w
        if v == 0:
            raise AssertionError("Wörter 1, a, b linear abhängig")
        p = v.bit_length() - 1
        B = [(q, (w ^ v) if (w >> p) & 1 else w) for q, w in B]
        B.append((p, v))
    return B


def main():
    masken = [sum(1 << i for i in c) for k in range(R + 1) for c in combinations(range(L), k)]
    assert len(masken) == 60460
    typen = sorted({t for t in partitionen(L, 4) if sum(1 for s in t if s) >= 3}, reverse=True)
    erg = []
    for t in typen:
        B = staffel(basis(t))
        ps = [(1 << p, w) for p, w in B]
        klassen = set()
        for v in masken:
            for bit, w in ps:
                if v & bit:
                    v ^= w
            klassen.add(v)
        erg.append((t, len(klassen)))
    print(f"Typen: {len(erg)}")
    gross = max(erg, key=lambda e: e[1])
    klein = min(erg, key=lambda e: e[1])
    d5 = dict(erg)[(5, 5, 5, 5)]
    print(f"größtes B(D) = {gross[1]} bei Typ {gross[0]}")
    print(f"B(D) bei (5,5,5,5) = {d5}")
    print(f"kleinstes B(D) = {klein[1]} bei Typ {klein[0]}")
    schr = [(Fraction(1 << 17, b), t, b) for t, b in erg]
    print(f"Schranke 2^17/B(D): kleinste {float(min(schr)[0]):.4f} (Typ {min(schr)[1]}), größte {float(max(schr)[0]):.4f} (Typ {max(schr)[1]}); "
          f"Typen mit Schranke >= 5: {sum(1 for s in schr if s[0] >= 5)}")
    if len(sys.argv) > 1:
        with open(sys.argv[1], "w") as f:
            f.write("typ;B;2^17/B\n")
            for t, b in erg:
                f.write(f"{t};{b};{(1 << 17) / b:.4f}\n")


if __name__ == "__main__":
    main()
