#!/usr/bin/env python3
"""pflanz.py -- Gate zu Auftrag K: Positivkontrolle der eigenen Suche in der Zielzelle.
Liest eine +-1-Matrix (Zeilen = Codewoerter bis auf Komplement), bestimmt selbst den Kern (alle k mit C + k = C), waehlt darin
<1,a,b> mit dem verlangten Typ, bringt die Spalten in die Anordnung von kern3_gate.c und ruft dessen Modus "plant" auf:
erst wird geprueft, dass die K Klassen ueberdecken, dann werden die letzten r gestrichen und neu gesucht.
Aufruf: pflanz.py KERN3_GATE MATRIX R n1 n2 n3 n4 r"""
import sys, subprocess, itertools
prog, pfad, R = sys.argv[1], sys.argv[2], int(sys.argv[3]); typ = [int(v) for v in sys.argv[4:8]]; r = int(sys.argv[8])
rows = [[int(v) for v in l.split()] for l in open(pfad) if l.strip()]; m = len(rows[0]); eins = (1 << m) - 1
W = set()
for z in rows:
    w = sum(1 << j for j, v in enumerate(z) if v < 0); W.add(w); W.add(w ^ eins)
w0 = next(iter(W)); kern = [k for k in (w0 ^ w for w in W) if all((w ^ k) in W for w in W)]
assert eins in kern
gef = None
for a, b in itertools.combinations([k for k in kern if k not in (0, eins)], 2):
    if b in (a ^ eins,): continue
    bl = [[], [], [], []]
    for j in range(m): bl[(a >> j & 1) * 2 + (b >> j & 1)].append(j)
    bl.sort(key=len, reverse=True)
    if [len(x) for x in bl] == typ: gef = bl; break
assert gef, 'kein Paar a, b mit diesem Typ im Kern (Kerngroesse %d)' % len(kern)
neu = {}; pos = 0                                   # alte Spalte -> neue Stelle
for j in range(4):
    for s in (gef[j][1:] if j < 3 else gef[j]): neu[s] = pos; pos += 1
for j in range(3): neu[gef[j][0]] = m - 3 + j
blk = {neu[s]: j for j in range(4) for s in gef[j]}
def klasse(w):
    x = sum(1 << neu[j] for j in range(m) if w >> j & 1)
    f = [x >> (m - 3 + j) & 1 for j in range(3)]; f.append(f[0] ^ f[1] ^ f[2])
    for p in range(m):
        if f[blk[p]]: x ^= 1 << p
    assert x < 1 << (m - 3); return x
kl = sorted({klasse(w) for w in W}); assert len(kl) * 8 == len(W), (len(kl), len(W))
kl = [c ^ kl[0] for c in kl]                        # Verschiebung: erste Klasse = 0
print('%s: %d Woerter, Kern %d Elemente, %d Klassen' % (pfad.split('/')[-1], len(W), len(kern), len(kl)))
print(subprocess.run([prog, 'plant', str(m), str(R)] + [str(v) for v in typ] + [str(r)] + [str(c) for c in kl], capture_output=True, text=True).stdout.strip())
