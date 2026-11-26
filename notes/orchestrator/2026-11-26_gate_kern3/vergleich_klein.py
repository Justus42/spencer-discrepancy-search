#!/usr/bin/env python3
"""vergleich_klein.py -- Gate zu Auftrag K: kleinstes K je Typ aus kern3_gate (Modus kmin) gegen die Brute-Force-Zahlen der Sitzung
(runs/kern3/s0_klein_brute_*.log.txt, klein_brute.py: ohne Symmetrie und Schranken). Aufruf: vergleich_klein.py REPO KERN3_GATE"""
import sys, re, subprocess, os
repo, prog = sys.argv[1], sys.argv[2]
soll = {}
for f in ('s0_klein_brute_10_2.log.txt', 's0_klein_brute_10_2_k6.log.txt', 's0_klein_brute_11-12_3.log.txt'):
    for l in open(os.path.join(repo, 'runs/kern3', f)):
        t = re.match(r'\((\d+),(\d+)\) Typ \((\d+), (\d+), (\d+), (\d+)\) brute(?: Kmin=(\d+)|: kein K <= (\d+))', l)
        if not t: continue
        k = tuple(int(v) for v in t.groups()[:6])
        if t.group(7): soll[k] = ('min', int(t.group(7)))
        elif k not in soll: soll[k] = ('gt', int(t.group(8)))
ges = ab = 0
for (m, R, kmax) in ((10, 2, 6), (11, 3, 4), (12, 3, 4)):
    out = subprocess.run([prog, 'kmin', str(m), str(R), str(kmax), '120'], capture_output=True, text=True).stdout
    for l in out.splitlines():
        t = re.match(r'\((\d+),(\d+),(\d+),(\d+)\) \|S\|=\d+ (?:Kmin=(\d+)|(Kmin>KMAX)|(ZEIT))', l)
        k = (m, R) + tuple(int(v) for v in t.groups()[:4]); art, w = soll[k]; ges += 1
        ok = (t.group(5) and art == 'min' and int(t.group(5)) == w) or (t.group(6) and art == 'gt' and w >= kmax) or (t.group(5) and art == 'gt' and int(t.group(5)) > w)
        ab += not ok
        if not ok: print('ABWEICHUNG', k, l, soll[k])
    print('(%d,%d): %d Typen verglichen' % (m, R, sum(1 for k in soll if k[:2] == (m, R))))
print('Typen der Sitzung: %d, verglichen: %d, Abweichungen: %d' % (len(soll), ges, ab))
