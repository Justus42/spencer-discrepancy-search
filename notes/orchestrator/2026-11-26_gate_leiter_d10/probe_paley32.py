#!/usr/bin/env python3
"""probe_paley32.py -- Probe des Orchestrators im Gate D (nicht gegengerechnet): Paley-Hadamard-Matrix der Ordnung 32 (q = 31), Spalten gestrichen, d = 10.
Erst Eichung des Zaehlers an Sylvester 32 ohne 2 Spalten (muss 4 * 107520 / 2 = 215040 ergeben: je Klasse (y,z) vier Punkte, bis auf Vorzeichen zwei).
Aufruf: probe_paley32.py PFAD_ZU_offen_allg ARBEITSORDNER"""
import sys, subprocess, os, random
prog, tmp = sys.argv[1], sys.argv[2]; q = 31
qr = {(a * a) % q for a in range(1, q)}; chi = lambda a: 0 if a % q == 0 else (1 if a % q in qr else -1)
P = [[0] * 32 for _ in range(32)]
for j in range(1, 32): P[0][j] = 1; P[j][0] = -1
for i in range(1, 32):
    for j in range(1, 32): P[i][j] = chi(i - j)
for i in range(32): P[i][i] = 1
assert all(sum(P[i][k] * P[j][k] for k in range(32)) == (32 if i == j else 0) for i in range(32) for j in range(32))
S16 = [[1 - 2 * (bin(i & j).count('1') & 1) for j in range(32)] for i in range(32)]
def offen(M, weg, d=10):
    fn = os.path.join(tmp, 'm.txt')
    with open(fn, 'w') as f:
        for z in M: f.write(' '.join(str(z[j]) for j in range(32) if j not in weg) + '\n')
    return subprocess.run([prog, fn, str(d)], capture_output=True, text=True).stdout.strip()
print("Eichung Sylvester 32 ohne Spalten (16,17):", offen(S16, (16, 17)))
for weg in ((0, 1), (5, 20)): print("Paley 32 ohne Spalten %s:" % (weg,), offen(P, weg))
random.seed(3)
for _ in range(4):
    weg = tuple(sorted(random.sample(range(32), 4))); print("Paley 32 ohne Spalten %s:" % (weg,), offen(P, weg))
