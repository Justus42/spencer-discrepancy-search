#!/usr/bin/env python3
"""probe_sylvester32.py -- Gate zu Auftrag D, Probe des Orchestrators (nicht Teil des Auftrags, von keiner Sitzung gegengerechnet).
Bauregel wie beim 48er: M = [[H,H],[H,-H]] mit H = Sylvester-Hadamard 16, also Kern-Paar U = W = H mit K = 16 (die 64 Woerter sind RM(1,5)),
dann Spalten streichen. Gezaehlt werden die offenen Klassen bei d = 10 mit dem eigenen Zaehler gate_d.
Aufruf: probe_sylvester32.py PFAD_ZU_gate_d ARBEITSORDNER"""
import sys, itertools, random, subprocess, os
prog, tmp = sys.argv[1], sys.argv[2]
H = [[1 - 2 * (bin(i & j).count('1') & 1) for j in range(16)] for i in range(16)]
def zaehle(weg_u, weg_w, d=10):
    cu = [j for j in range(16) if j not in weg_u]; cw = [j for j in range(16) if j not in weg_w]; fn = os.path.join(tmp, 'p.paar.txt')
    with open(fn, 'w') as f:
        for i in range(16): f.write(' '.join('%2d' % H[i][j] for j in cu) + ' | ' + ' '.join('%2d' % H[i][j] for j in cw) + '\n')
    w = dict(t.split('=') for t in subprocess.run([prog, fn, str(d)], capture_output=True, text=True).stdout.split())
    return len(cu), len(cw), int(w['offen']), int(w['gesamt'])
print("Zelle (30,10), 2 Spalten gestrichen (alle 2-Mengen sind unter AGL(5,2) gleichwertig; drei Lagen zur Kontrolle):")
for wu, ww in (((), (0, 1)), ((), (5, 10)), ((3,), (12,))): print("   U ohne %s, W ohne %s: Typ (%d,%d), offene Klassen %d von %d" % ((wu, ww) + zaehle(wu, ww)))
print("Zelle (28,9), 4 Spalten gestrichen (zwei Bahnen: Summe der vier Punkte 0 oder nicht):")
for wu, ww in (((), (0, 1, 2, 3)), ((), (0, 1, 2, 4)), ((0, 1), (0, 1)), ((0, 1), (0, 2)), ((0,), (1, 2, 4))): print("   U ohne %s, W ohne %s: Typ (%d,%d), offene Klassen %d von %d" % ((wu, ww) + zaehle(wu, ww)))
print("Zelle (26,8), 6 Spalten gestrichen, 40 zufaellige Lagen (Startwert 1), Verteilung der offenen Klassen:")
random.seed(1); erg = {}
for _ in range(40):
    s = random.sample(range(32), 6); wu = tuple(sorted(x for x in s if x < 16)); ww = tuple(sorted(x - 16 for x in s if x >= 16))
    if len(wu) > len(ww): wu, ww = ww, wu
    r = zaehle(wu, ww); erg.setdefault(r[2], []).append((wu, ww))
for k in sorted(erg): print("   offene Klassen %8d: %2d Lagen, z. B. U ohne %s, W ohne %s" % (k, len(erg[k]), erg[k][0][0], erg[k][0][1]))
