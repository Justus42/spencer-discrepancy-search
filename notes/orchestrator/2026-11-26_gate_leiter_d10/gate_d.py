#!/usr/bin/env python3
"""gate_d.py -- Gate zu Auftrag D: ruft den eigenen Zaehler gate_d (C) auf jedem gespeicherten Paar aus runs/leiter_d10 auf und vergleicht
mit der Restzahl in der Kopfzeile der Datei (geloeste Paare: 0). Aufruf aus dem Repo-Wurzelordner: gate_d.py PFAD_ZUM_PROGRAMM"""
import sys, glob, os, re, subprocess
from collections import defaultdict
ab = n = gel = 0; best = defaultdict(list); orth = defaultdict(lambda: [0, 0, 0])
for fn in sorted(glob.glob('runs/leiter_d10/*.paar.txt')):
    kopf = open(fn).readline(); name = os.path.basename(fn)[:-9]
    d = int(re.search(r'd=(\d+)', kopf).group(1)); r = re.search(r'Restzahl (\d+)', kopf); soll = int(r.group(1)) if r else 0
    aus = subprocess.run([sys.argv[1], fn, str(d)], capture_output=True, text=True).stdout
    w = dict(t.split('=') for t in aus.split()); ist = int(w['offen']); n += 1; gel += ist == 0
    if ist != soll: ab += 1; print("ABWEICHUNG", name, soll, ist)
    key = re.sub(r'_seed\d+$', '', name); best[key].append(ist); o = orth[key]; o[0] += 1; o[1] += int(w['U_orthogonal']); o[2] += int(w['W_orthogonal'])
    if not r: print("Kopf ohne Restzahl (geloest):", name, kopf.strip()[:70], "-> gezaehlt", ist)
for k in sorted(best):
    print("%-26s Laeufe %2d  kleinste Restzahl %9d  groesste %9d   U orthogonal in %d, W orthogonal in %d" % (k, len(best[k]), min(best[k]), max(best[k]), orth[k][1], orth[k][2]))
print("Paare: %d, Abweichungen: %d, geloest (0 offen): %d" % (n, ab, gel))
