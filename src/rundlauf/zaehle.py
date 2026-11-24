#!/usr/bin/env python3
"""Prüfcode (kein Suchcode): Zahl und Form der Matrizen je Familie (Auftrag Punkt 2).
Aufruf: python3 src/rundlauf/zaehle.py [Schlüssel ...]   (ohne Argument: alle 16, 4 Prozesse)
Je Familie: Anzahl der gelieferten Matrizen, Zeilen x Spalten (muss bei allen gleich sein), Einträge nur +-1, Namen verschieden.
"""
import sys, os, time
from multiprocessing import Pool
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from familien import FAMILIEN, lade

def zaehle(k):
    t0 = time.time()
    n = 0; formen = {}; namen = set(); fehler = 0; doppelt = 0
    for name, M in lade(k)():
        n += 1
        if name in namen: doppelt += 1
        namen.add(name)
        form = (len(M), len(set(len(r) for r in M)) == 1 and len(M[0]))
        formen[form] = formen.get(form, 0) + 1
        if any(v not in (-1, 1) for r in M for v in r): fehler += 1
    return k, n, formen, fehler, doppelt, time.time() - t0

if __name__ == "__main__":
    ks = sys.argv[1:] or list(FAMILIEN)
    ok = True
    with Pool(4) as p:
        for k, n, formen, fehler, doppelt, t in p.imap_unordered(zaehle, ks):
            F = FAMILIEN[k]
            soll = (F["zeilen"], F["spalten"])
            gut = (n == F["anzahl"] and list(formen) == [soll] and fehler == 0 and doppelt == 0)
            ok &= gut
            print(f"{k}: {n} Matrizen (erwartet {F['anzahl']}), Formen {formen} (erwartet {soll}), Einträge nicht +-1: {fehler}, doppelte Namen: {doppelt}, {t:.1f} s -> {'OK' if gut else 'ABWEICHUNG'}", flush=True)
    sys.exit(0 if ok else 1)
