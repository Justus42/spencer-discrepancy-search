#!/usr/bin/env python3
"""gate_v.py -- Gate zu Auftrag V (Orchestrator, 26.11.2026). Eigener Zaehler (wie gate_f.py), nicht der Bewerter, kein Code der Sitzung.
Offene Klassen einer +-1-Matrix (k Zeilen, m Spalten) bei Radius R: x mit x_0 = +1 und |<u_i, x>| < m - 2R fuer alle Zeilen.
1. alle Dateien runs/verdopplung/out/*.txt gegen runs/verdopplung/nachzaehlung.log.txt (m, R, k aus dem Dateinamen)
2. die beiden Bauregel-Matrizen
3. je Zelle und Paarzahl: kleinste und groesste Restzahl, Zahl der Laeufe mit 0
Aufruf: gate_v.py REPO"""
import sys, os, re, glob
import numpy as np
repo = sys.argv[1]; D = os.path.join(repo, 'runs/verdopplung')
def lies(p): return np.array([[int(v) for v in l.split()] for l in open(p) if l.strip()], dtype=np.int8)
def offen(M, R):
    k, m = M.shape; d = m - 2 * R; o = None
    for u in M:
        a = np.array([u[0]], dtype=np.int8)
        for j in range(1, m): a = np.concatenate((a + u[j], a - u[j]))
        z = np.abs(a) < d; o = z if o is None else o & z
    return int(o.sum())
soll = {}
for l in open(os.path.join(D, 'nachzaehlung.log.txt')):
    t = re.match(r'(\S+\.txt) archiv (\d+) neu (\d+) (\w+)', l)
    if t: soll[os.path.basename(t.group(1))] = (int(t.group(2)), int(t.group(3)), t.group(4))
dateien = sorted(os.path.basename(p) for p in glob.glob(os.path.join(D, 'out', '*.txt')))
print('1. Ausgabedateien: %d im Ordner, %d im Log der Sitzung, gleiche Namen: %s' % (len(dateien), len(soll), set(dateien) == set(soll)))
gl = 0; je = {}
for f in dateien:
    t = re.match(r'(\w+?)_m(\d+)R(\d+)k(\d+)_s(\d+)\.txt', f); m, R, k = int(t.group(2)), int(t.group(3)), int(t.group(4))
    M = lies(os.path.join(D, 'out', f)); assert M.shape == (k, m) and set(np.unique(M)) <= {-1, 1}, (f, M.shape)
    e = offen(M, R); a, n, ok = soll[f]; gl += (e == a == n)
    if not e == a == n: print('   ABWEICHUNG %s: eigen %d, Archiv %d, Sitzung neu %d' % (f, e, a, n))
    je.setdefault((m, R, k, t.group(1)), []).append(e)
print('   eigener Zaehler = Archiv = Nachzaehlung der Sitzung: %d von %d' % (gl, len(dateien)))
print('2. Bauregel')
for name, R in (('bauregel_24x22.txt', 7), ('bauregel_24x24.txt', 8)):
    M = lies(os.path.join(D, name)); print('   %s: %d x %d, R=%d, offen %d' % (name, M.shape[0], M.shape[1], R, offen(M, R)))
print('3. je Zelle, Paarzahl, Block: Laeufe, davon 0 offen, kleinste, groesste Restzahl')
for (m, R, k, bl), w in sorted(je.items()):
    print('   (%d,%d) k=%d Block %s: %d Laeufe, %d mit 0, min %d, max %d' % (m, R, k, bl, len(w), sum(v == 0 for v in w), min(w), max(w)))
