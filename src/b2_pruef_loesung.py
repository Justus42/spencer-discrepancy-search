#!/usr/bin/env python3
"""b2_pruef_loesung.py (Auftrag B2, Pruefcode, nur ganze Zahlen)

Prueft Loesungszeilen aus `b2_zaehl` unabhaengig: Bahnvertreter (Woerter) werden mit allen Elementen von P
ausgebaut (Paare {w,-w}), Gewicht = Zahl der Paare, und der Ueberdeckungsradius der Wortmenge (Paare und Negative)
wird ueber alle 2^20 Punkte bestimmt (numpy, ganze Zahlen).
Aufruf: b2_pruef_loesung.py P_ELEMENTE LISTE [ANZAHL [SEED]]   (ANZAHL: Stichprobe, sonst alle)
"""
import sys, random
import numpy as np
N = 20; MASK = (1 << N) - 1

def lies_P(fn):
    el = []
    for line in open(fn):
        if line.startswith('#') or not line.strip():
            continue
        a, b = line.split('|')
        el.append(([int(x) for x in a.split()], [int(x) for x in b.split()]))
    assert len(el) == 9
    return el

def bild(g, w):
    pi, t = g; r = 0
    for j in range(N):
        b = (w >> j) & 1
        if t[j] == -1:
            b ^= 1
        r |= b << pi[j]
    return r

def main():
    P = lies_P(sys.argv[1])
    zeilen = [l for l in open(sys.argv[2]) if l.startswith('L ')]
    if len(sys.argv) > 3:
        random.seed(int(sys.argv[4]) if len(sys.argv) > 4 else 1)
        zeilen = random.sample(zeilen, min(int(sys.argv[3]), len(zeilen)))
    alle = np.arange(1 << N, dtype=np.int64)
    ok = 0
    for l in zeilen:
        teile = l.split('|')
        gew = int(teile[0].split()[1])
        vert = [int(x) for x in teile[2].split()[1:]]
        paare = set()
        for v in vert:
            for g in P:
                x = bild(g, v)
                paare.add(min(x, x ^ MASK))
        woerter = set()
        for x in paare:
            woerter.add(x); woerter.add(x ^ MASK)
        dmin = np.full(1 << N, 99, dtype=np.int64)
        for x in woerter:
            dmin = np.minimum(dmin, np.bitwise_count(alle ^ x).astype(np.int64))
        rad = int(dmin.max())
        gut = (len(paare) == gew and rad <= 6)
        ok += gut
        if not gut:
            print("FEHLER:", l.strip(), "Paare", len(paare), "Radius", rad)
    print("geprueft %d, in Ordnung %d (Gewicht = Zahl der Paare, Radius <= 6)" % (len(zeilen), ok))

main()
