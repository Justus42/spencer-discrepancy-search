#!/usr/bin/env python3
"""Qualitätstest Teil C (Prüfcode): für kleine n jede Klasse exakt rechnen (build/exakt_disc) und mit der
oberen Schranke u aus build/zirkulant vergleichen. Aufruf: zirkulant_qualitaet.py PRAEFIX n [-r]"""
import os, sys, subprocess
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import zirkulant_pruef as zp

praefix, n, rand = sys.argv[1], int(sys.argv[2]), "-r" in sys.argv
zeilen = [l.split() for l in open(praefix + "_alle.txt") if l.strip()]

def eins(i_hu):
    i, (h, u) = i_hu
    return h, int(u), zp.exakt_eins(int(h, 16), n, rand, f"{praefix}_q{i % 4}.txt")

ergebnis = []
with ThreadPoolExecutor(4) as ex:
    # 4 feste Dateinamen, je Thread einer: Aufgaben blockweise verteilen
    bloecke = [[(i, z) for i, z in enumerate(zeilen) if i % 4 == k] for k in range(4)]
    def block(b):
        return [eins(t) for t in b]
    for r in ex.map(block, bloecke):
        ergebnis += r
for k in range(4):
    try: os.remove(f"{praefix}_q{k}.txt")
    except FileNotFoundError: pass
falsch = [e for e in ergebnis if e[1] < e[2]]
ueber = [e for e in ergebnis if e[1] > e[2]]
dmax = max(e[2] for e in ergebnis); umax = max(e[1] for e in ergebnis)
vert = {}
for e in ergebnis: vert[e[2]] = vert.get(e[2], 0) + 1
print(f"n={n}{' mit Rand' if rand else ''}: {len(ergebnis)} Klassen exakt; max exakt {dmax}, u_max {umax}; "
      f"u > exakt in {len(ueber)} Klassen, u < exakt (Fehler) in {len(falsch)}; Verteilung exakt {dict(sorted(vert.items()))}")
