#!/usr/bin/env python3
"""probe_sylvester32_d11.py -- wie probe_sylvester32.py, aber d = 11 (Rekordgrenze dort 76 Woerter = 38 Zeilen, also 32 Paarzeilen plus bis zu 6 freie Zeilen).
Zaehlt, wie viele Klassen die 64 Woerter (RM(1,5) mit gestrichenen Spalten) bei d = 11 offen lassen. Probe des Orchestrators, nicht gegengerechnet.
Aufruf: probe_sylvester32_d11.py PFAD_ZU_gate_d ARBEITSORDNER"""
import sys, random, subprocess, os
prog, tmp = sys.argv[1], sys.argv[2]
H = [[1 - 2 * (bin(i & j).count('1') & 1) for j in range(16)] for i in range(16)]
def zaehle(weg, d):
    wu = [x for x in weg if x < 16]; ww = [x - 16 for x in weg if x >= 16]
    if len(wu) > len(ww): wu, ww = ww, wu
    cu = [j for j in range(16) if j not in wu]; cw = [j for j in range(16) if j not in ww]; fn = os.path.join(tmp, 'q.paar.txt')
    with open(fn, 'w') as f:
        for i in range(16): f.write(' '.join('%2d' % H[i][j] for j in cu) + ' | ' + ' '.join('%2d' % H[i][j] for j in cw) + '\n')
    w = dict(t.split('=') for t in subprocess.run([prog, fn, str(d)], capture_output=True, text=True).stdout.split())
    return int(w['offen']), int(w['gesamt'])
random.seed(2)
for m, n in ((31, 3), (29, 8), (27, 16), (25, 24), (23, 24)):
    erg = {}
    for _ in range(n):
        weg = tuple(sorted(random.sample(range(32), 32 - m))); o, g = zaehle(weg, 11); erg.setdefault(o, []).append(weg)
    print("Zelle (%d,%d), d = 11, %d Spalten gestrichen, %d zufaellige Lagen; offene Klassen (von %d): %s" % (m, (m - 11) // 2, 32 - m, n, g,
          ", ".join("%d (%d Lagen)" % (k, len(erg[k])) for k in sorted(erg))))
    k = min(erg); print("     beste Lage: gestrichen %s, Anteil offen %.4f %%" % (erg[k][0], 100.0 * k / g))
