#!/usr/bin/env python3
"""gate_daten.py -- Gate zu Auftrag LZ (Orchestrator). Liest die Matrix und die Wortliste aus den erzeugten Lean-Datendateien (als Text)
und vergleicht sie mit den Quelldateien in controls/. Dazu der Radius ueber alle 2^18 Punkte mit eigenem Zaehler."""
import re, numpy as np
def lean(name):
    t = open('lean/Ueberdeckung/Daten/%s.lean' % name, encoding='utf-8').read()
    m = re.search(r'def matrix : List \(List Int\) := \[(.*?)\n\]', t, re.S); rows = re.findall(r'\[([^\[\]]*)\]', m.group(1))
    M = [[int(v) for v in r.split(',')] for r in rows]
    w = re.search(r'def woerter : List Nat := \[(.*?)\]', t, re.S); W = [int(v) for v in re.findall(r'\d+', w.group(1))]
    return M, W
def quelle(fn): return [[int(v) for v in l.split()] for l in open(fn) if l.split() and not l.startswith('#')]
PC = np.array([bin(i).count('1') for i in range(512)], dtype=np.int8); PTS = np.arange(1 << 18, dtype=np.int64)
def radius(W):
    best = np.full(1 << 18, 99, dtype=np.int8)
    for c in W:
        x = PTS ^ c; best = np.minimum(best, PC[x & 511] + PC[x >> 9])
    return int(best.max()), int((best > 5).sum()), [int(p) for p in PTS[best > 5][:8]]
for name, fn, weg in (('C18_5_50w', 'controls/c18_5_50w.txt', None), ('C18_5_52w', 'controls/c18_5_52w.txt', None), ('C18_5_50w_ohne24', 'controls/c18_5_50w.txt', 24)):
    M, W = lean(name); Q = quelle(fn)
    if weg is not None: Q = Q[:weg] + Q[weg + 1:]
    eig = [sum(1 << j for j, e in enumerate(z) if e == -1) for z in M]; eig = eig + [w ^ ((1 << 18) - 1) for w in eig]
    r = radius(W)
    print("%-17s Matrix in Lean = Quelle: %s (%d Zeilen x %d); Wortliste in Lean = Zeilen und Komplemente (als Menge): %s, %d Woerter, verschieden %d; groesster Abstand %d, Punkte ueber 5: %d %s"
          % (name, M == Q, len(M), len(M[0]), sorted(W) == sorted(eig), len(W), len(set(W)), r[0], r[1], r[2] if r[1] else ''))
