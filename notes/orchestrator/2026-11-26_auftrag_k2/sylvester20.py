#!/usr/bin/env python3
"""sylvester20.py -- Orchestrator, 26.11.2026: Kontrollobjekt fuer den Folgeauftrag K2.
Sylvester 32 auf 20 Spalten (SPALTEN unten): 32 x 20, 64 Woerter mit den Komplementen, linear. Schreibt die Matrix und listet die Typen
(Blockgroessen) aller Unterkerne <1,a,b> mit je einem Paar (a,b) als 0/1-Strings (Zeichen j = Spalte j). Nur ganze Zahlen.
Aufruf: sylvester20.py AUSGABEDATEI"""
import sys, itertools
SPALTEN = [1, 2, 3, 4, 5, 6, 9, 10, 11, 13, 14, 15, 16, 17, 19, 22, 23, 25, 27, 29]
rows = [[1 - 2 * (bin(i & j).count('1') & 1) for j in SPALTEN] for i in range(32)]
open(sys.argv[1], 'w').write('\n'.join(' '.join('%2d' % v for v in z) for z in rows) + '\n')
m = 20; eins = (1 << m) - 1
W = set()
for z in rows:
    w = sum(1 << j for j, v in enumerate(z) if v < 0); W.add(w); W.add(w ^ eins)
assert len(W) == 64 and all((x ^ y) in W for x in W for y in W)          # linear, 64 Woerter
typen = {}
for a, b in itertools.combinations(sorted(w for w in W if w not in (0, eins)), 2):
    if b == a ^ eins: continue
    bl = [0, 0, 0, 0]
    for j in range(m): bl[(a >> j & 1) * 2 + (b >> j & 1)] += 1
    t = tuple(sorted(bl, reverse=True)); typen.setdefault(t, (a, b))
s = lambda w: ''.join(str(w >> j & 1) for j in range(m))
print('Sylvester 32 auf den Spalten', SPALTEN, ': 32 x 20, 64 Woerter, linear;', len(typen), 'Typen von <1,a,b>')
for t in sorted(typen, reverse=True): print('Typ %s  a=%s  b=%s' % (','.join(str(v) for v in t), s(typen[t][0]), s(typen[t][1])))
