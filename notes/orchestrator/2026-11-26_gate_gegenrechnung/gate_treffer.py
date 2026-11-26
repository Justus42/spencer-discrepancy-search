#!/usr/bin/env python3
"""gate_treffer.py -- Gate zu Auftrag C (Orchestrator, 26.11.2026). Nur ganze Zahlen.
1. Vergleicht fallweise die Trefferliste der Sitzung (Ordnung 24 ohne 4 Spalten, disc >= 8;
   runs/gegenrechnung_ideen/p2_c4_treffer.txt) mit der des Orchestrators
   (notes/orchestrator/2026-11-26_ideen/treffer_had24_c4_teil{0,1}.txt). Die Matrizen werden ueber ihren
   Inhalt zugeordnet, nicht ueber ihre Nummer.
2. Schreibt einige Treffer (ohne 2 und ohne 4 Spalten) als Matrixdateien fuer verifiers/pruefe_matrix.py.
Aufruf: gate_treffer.py REPO ORCH_HAD24 AUSGABEORDNER   (ORCH_HAD24 = had24.txt aus had_eingaben.py)"""
import sys, os
repo, orch, aus = sys.argv[1:4]
def bloecke(p):
    return [tuple(tuple(int(v) for v in z.split()) for z in b.strip().split('\n')) for b in open(p).read().strip().split('\n\n')]
mein = bloecke(orch)
z = open(os.path.join(repo, 'runs/gegenrechnung_ideen/eingaben/had24.txt')).read().split()
anz, nz, ns = int(z[0]), int(z[1]), int(z[2]); w = [int(v) for v in z[3:]]
sitz = [tuple(tuple(w[(k*nz+i)*ns:(k*nz+i)*ns+ns]) for i in range(nz)) for k in range(anz)]
assert len(mein) == 120 and anz == 120
assert all(mein[2*k] == sitz[k] and mein[2*k+1] == sitz[60+k] for k in range(60))
zu = [k//2 + 60*(k % 2) for k in range(120)]     # Blocknummer Orchestrator -> Nummer Sitzung (Inhalt oben geprueft)
print('Zuordnung: Orchestrator-Block 2k = Sitzung k (Original), Block 2k+1 = Sitzung 60+k (Transponierte), Inhalt gleich: ja')
orte = {}
for k, M in enumerate(sitz): orte.setdefault(M, []).append(k)
dopp = sorted(v for v in orte.values() if len(v) > 1)
assert all(len(v) == 2 and v[0] < 60 <= v[1] for v in dopp)
print('verschiedene Matrizen unter den 120: %d; %d Transponierte stehen woertlich schon unter den 60 Originalen' % (len(orte), len(dopp)))
print('Originale, deren Transponierte NICHT woertlich in der Liste steht:', [k for k in range(60) if tuple(zip(*sitz[k])) not in sitz[:60]])
A = set()
for teil in (0, 1):
    for nr, l in enumerate(open(os.path.join(repo, 'notes/orchestrator/2026-11-26_ideen/treffer_had24_c4_teil%d.txt' % teil))):
        t = l.split(); assert t[0] == 'mat' and int(t[1]) == nr and int(t[2]) == len(t) - 3
        for h in t[3:]: A.add((zu[2*nr + teil], int(h, 16)))
B = set()
for l in open(os.path.join(repo, 'runs/gegenrechnung_ideen/p2_c4_treffer.txt')):
    k, h = l.split(); B.add((int(k), int(h, 16)))
assert all(bin(m).count('1') == 4 for _, m in A | B)
print('ohne 4 Spalten: Orchestrator %d Faelle, Sitzung %d Faelle, gemeinsam %d, nur Orchestrator %d, nur Sitzung %d'
      % (len(A), len(B), len(A & B), len(A - B), len(B - A)))
print('FALLWEISE GLEICH:', A == B)
jeB = {}
for k, m in B: jeB.setdefault(k, set()).add(m)
print('woertlich doppelte Matrizen haben dieselbe Treffermenge (Sitzung):', all(jeB.get(u, set()) == jeB.get(v, set()) for u, v in dopp))
print('Matrizen mit Treffern, Originale:', sorted((k, len(v)) for k, v in jeB.items() if k < 60))
print('Matrizen mit Treffern, Transponierte (als Nummer des gleichen Originals, wo woertlich vorhanden):',
      sorted(((orte[sitz[k]][0] if len(orte[sitz[k]]) > 1 else 'T%d' % (k - 60), len(v)) for k, v in jeB.items() if k >= 60), key=str))
C2 = [(int(l.split()[0]), int(l.split()[1], 16)) for l in open(os.path.join(repo, 'runs/gegenrechnung_ideen/p2_c2_treffer.txt'))]
assert all(bin(m).count('1') == 2 for _, m in C2)
je = {}
for k, m in sorted(B): je.setdefault(k, []).append(m)
wahl = [(k, je[k][0]) for k in sorted(je) if len(je[k]) < 100] + [(0, je[0][0]), (0, je[0][-1]), (60, je[60][1319])]
wahl += [C2[0], C2[len(C2)//2], C2[-1]]
for k, m in wahl:
    name = 'had24_m%03d_ohne_%06x.txt' % (k, m)
    with open(os.path.join(aus, name), 'w') as f:
        for r in sitz[k]: f.write(' '.join(str(v) for j, v in enumerate(r) if not m >> j & 1) + '\n')
print('geschrieben:', len(wahl), 'Matrizen nach', aus)
