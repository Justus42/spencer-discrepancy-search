#!/usr/bin/env python3
"""gate_f.py -- Gate zu Auftrag F (Orchestrator, 26.11.2026). Eigener Zaehler, kein Code der Sitzung, nicht der Bewerter.
Offene Klassen einer +-1-Matrix (k Zeilen, m Spalten) bei Radius R: Zahl der x in {+-1}^m mit x_0 = +1 und
|<u_i, x>| < d = m - 2R fuer alle Zeilen i (x und -x sind gleichwertig, also 2^(m-1) Klassen). Nur ganze Zahlen.
1. alle Ausgabedateien runs/frei_d9/*_s*.txt gegen runs/frei_d9/auswertung_bewerter.log.txt
2. Messlatte: die bekannten 64er (runs/leiter_n/c25-8_k16, c23-7_k16) ohne je eine Zeile
3. Abstand der Ausgaben von Schritt 3 zur Startmatrix (Zahl geaenderter Eintraege)
Aufruf: gate_f.py REPO"""
import sys, os, re, glob
import numpy as np
repo = sys.argv[1]; D = os.path.join(repo, 'runs/frei_d9')
def lies(p): return np.array([[int(v) for v in l.split()] for l in open(p) if l.strip()], dtype=np.int8)
def masken(M, R):
    k, m = M.shape; d = m - 2 * R; out = []
    for u in M:
        a = np.array([u[0]], dtype=np.int8)
        for j in range(1, m): a = np.concatenate((a + u[j], a - u[j]))
        out.append(np.abs(a) < d)
    return out
def offen(M, R):
    o = None
    for z in masken(M, R): o = z if o is None else o & z
    return int(o.sum())
soll = {}
for l in open(os.path.join(D, 'auswertung_bewerter.log.txt')):
    t = re.match(r'(\S+\.txt): offen=(\d+) klassen=(\d+) k=(\d+) m=(\d+) R=(\d+)', l)
    if t: soll[t.group(1)] = tuple(int(v) for v in t.groups()[1:])
dateien = sorted(os.path.basename(p) for p in glob.glob(os.path.join(D, '*_s[0-9].txt')))
print('1. Ausgabedateien: %d im Ordner, %d im Log der Sitzung, gleiche Namen: %s' % (len(dateien), len(soll), set(dateien) == set(soll)))
gl = 0
for f in dateien:
    o, kl, k, m, R = soll[f]; M = lies(os.path.join(D, f)); assert M.shape == (k, m) and set(np.unique(M)) <= {-1, 1} and kl == 2 ** (m - 1)
    e = offen(M, R); gl += e == o
    print('   %-40s k=%d m=%d R=%d  eigen %d  Sitzung %d  %s' % (f, k, m, R, e, o, 'gleich' if e == o else 'ABWEICHUNG'))
print('   gleich: %d von %d' % (gl, len(dateien)))
print('2. Messlatte: bekannter 64er ohne je eine Zeile (Zeilen 1-basiert)')
start = {}
for name, R in (('c25-8_k16', 8), ('c23-7_k16', 7)):
    M = lies(os.path.join(repo, 'runs/leiter_n/%s.matrix.txt' % name)); start[M.shape[1]] = M
    z = masken(M, R); alle = z[0].copy()
    for q in z[1:]: alle &= q
    w = []
    for i in range(len(z)):
        o = None
        for j in range(len(z)):
            if j != i: o = z[j].copy() if o is None else o & z[j]
        w.append(int(o.sum()))
    mn = min(w)
    print('   %s (%d x %d, R=%d): voll %d offen; ohne Zeile: min %d (Zeilen %s), max %d, Mittel %.0f' %
          (name, M.shape[0], M.shape[1], R, int(alle.sum()), mn, [i + 1 for i, v in enumerate(w) if v == mn], max(w), sum(w) / len(w)))
print('3. Schritt 3: geaenderte Eintraege gegenueber der Startmatrix (64er ohne die genannte Zeile, gleiche Zeilenfolge)')
for tag, m, zeile in (('s3_p026_m25R8k31', 25, 5), ('s4_3_p026_m23R7k31', 23, 17)):
    S = np.delete(start[m], zeile - 1, axis=0)
    for s in range(1, 5):
        A = lies(os.path.join(D, '%s_s%d.txt' % (tag, s)))
        if A.shape != S.shape: print('   %s_s%d: Form %s' % (tag, s, A.shape)); continue
        df = A != S; zl = int(df.any(axis=1).sum())
        print('   %s_s%d: %d Eintraege in %d Zeilen geaendert' % (tag, s, int(df.sum()), zl))
