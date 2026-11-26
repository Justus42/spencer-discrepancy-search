#!/usr/bin/env python3
"""gate_b2.py -- Gate zu Auftrag B2 (Orchestrator). Eigene Rechnung; uebernommen sind nur die Dateiformate.
A  P aus beiden Laeufen gleich? (9 vorzeichenbehaftete Spaltenpermutationen)
B  beide Listen der 19 008 Loesungen als Mengen von 24 Paarklassen ausbauen und vergleichen; der 48er muss in beiden stehen
C  Stichprobe: Ueberdeckungsradius 6 ueber alle 2^20 Punkte (auch fuer Loesungen mit Neunerbahn aus der Positivkontrolle)
D  alle 19 008 Abbildungen auf den Klassenvertreter nachrechnen (Abbildung anwenden, Wortmenge vergleichen)
E  die 20 Abbildungen der Zuordnung (b), (c): Vertreter, 1p3, 2p2, seed4, Treffer aus Auftrag H -- Matrizen direkt aus den Quelldateien
F  eigene Invarianten der neun Vertreter (unter Spaltenpermutation, Spaltenvorzeichen, Zeilenpermutation, Zeilenvorzeichen fest):
   |Zeilenprodukte| ueber 2 und 4 Zeilen, |Spaltenprodukte| ueber 2 und 4 Spalten, je als sortierte Haeufigkeiten.
Wortkodierung: Bit j = 1 heisst Eintrag -1 in Spalte j. Abbildung (pi, t): Bild hat in Spalte pi_j den Wert t_j * w_j."""
import sys, itertools, numpy as np
from collections import Counter
N = 20; FULL = (1 << N) - 1
def wort(r): return sum(1 << j for j, x in enumerate(r) if x == -1)
def bild(g, w):
    pi, t = g; r = 0
    for j in range(N):
        b = (w >> j) & 1
        if t[j] < 0: b ^= 1
        r |= b << pi[j]
    return r
def kl(w): return min(w, w ^ FULL)
def matrix(fn): return [[int(v) for v in l.split()] for l in open(fn) if l.split() and not l.startswith('#')]
def pc(x):
    x = x - ((x >> 1) & 0x5555555555555555); x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333)
    return (((x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101 >> 56) & 0xFF
PTS = np.arange(1 << N, dtype=np.int64)
def radius(klassen):
    best = np.full(1 << N, 99, dtype=np.int64)
    for c in klassen:
        d = pc(PTS ^ c); best = np.minimum(best, np.minimum(d, N - d))
    return int(best.max())
Z = 'runs/bahnen3_zweit/'
# A
P2 = [tuple(tuple(int(v) for v in h.split()) for h in l.split('|')) for l in open(Z + 'P_elemente.txt') if not l.startswith('#')]
P1 = []
for l in open('runs/bahnen3/P_elemente.txt'):
    v = [int(x) for x in l.split()]; P1.append((tuple(v[:N]), tuple(-1 if f else 1 for f in v[N:])))
print("A  P: %d und %d Elemente, als Mengen gleich: %s" % (len(P1), len(P2), set(P1) == set(P2)))
P = P2
# B
bahn = {}
def ausbau(v):
    if v not in bahn: bahn[v] = frozenset(kl(bild(g, v)) for g in P)
    return bahn[v]
L2 = []
for l in open(Z + 'teil1b_B24_liste.txt'):
    reps = [int(x) for x in l.split('|')[2].split()[1:]]; L2.append(frozenset().union(*[ausbau(v) for v in reps]))
L1 = []
for l in open('runs/bahnen3/teil2_klein_B24_liste_4_56.txt'):
    reps = [int(x.split('/')[0]) for x in l.split(':')[1].split()]; L1.append(frozenset().union(*[ausbau(v) for v in reps]))
S1, S2 = set(L1), set(L2)
c48 = frozenset(kl(wort(r)) for r in matrix('controls/had24_1p3_paley12.txt')); c22 = frozenset(kl(wort(r)) for r in matrix('controls/had24_2p2_paley12.txt'))
print("B  Liste erstes Programm: %d Zeilen, %d verschiedene Mengen, Groessen %s" % (len(L1), len(S1), sorted(Counter(len(s) for s in S1).items())))
print("   Liste B2:              %d Zeilen, %d verschiedene Mengen, Groessen %s" % (len(L2), len(S2), sorted(Counter(len(s) for s in S2).items())))
print("   gemeinsam %d, nur erstes %d, nur B2 %d; 1p3 in beiden: %s; 2p2 in beiden: %s" % (len(S1 & S2), len(S1 - S2), len(S2 - S1), c48 in S1 and c48 in S2, c22 in S1 and c22 in S2))
print("   Bahngroessen der vorkommenden Vertreter:", sorted(Counter(len(b) for b in bahn.values()).items()))
# C
rng = np.random.default_rng(20261126); idx = sorted(rng.choice(len(L2), size=12, replace=False).tolist())
print("C  Radius ueber 2^20 Punkte, 12 zufaellige der 19 008 (Nummern %s): %s" % (idx, sorted(Counter(radius(L2[i]) for i in idx).items())))
pos = []
for l in open(Z + 'teil1_positiv_neun_liste.txt'):
    reps = [int(x) for x in l.split('|')[2].split()[1:]]; pos.append(frozenset().union(*[ausbau(v) for v in reps]))
idx = [0, 287, 575]
print("   Positivkontrolle mit Neunerbahn: %d Loesungen, Groessen %s; Radius von Nr. %s: %s" % (len(pos), sorted(Counter(len(s) for s in pos).items()), idx, [radius(pos[i]) for i in idx]))
# D
rep = {}; ok = bad = 0; anz = Counter(); aut = {}
for l in open(Z + 'teil2a_klassen_zuordnung.txt'):
    if l.startswith('#'):
        if 'Vertreter' in l: t = l.split(); rep[int(t[2].rstrip(':'))] = int(t[4].rstrip(','))
        continue
    a, b = l.split('|'); a = [int(x) for x in a.split()]; k, c, o = a[0], a[1], a[2]; g = (a[3:], [int(x) for x in b.split()])
    if frozenset(kl(bild(g, w)) for w in L2[k]) == L2[rep[c]]: ok += 1
    else: bad += 1
    anz[c] += 1; aut[c] = o
print("D  Abbildungen auf den Vertreter: richtig %d, falsch %d; Klassen (Vertreter, |Aut| laut Sitzung, Anzahl): %s" % (ok, bad, [(rep[c], aut[c], anz[c]) for c in sorted(anz)]))
print("   Anzahl * |Aut| je Klasse:", sorted({anz[c] * aut[c] for c in anz}))
# E
quellen = [Z + 'klasse%d_vertreter.txt' % i for i in range(8)] + ['controls/had24_1p3_paley12.txt', 'controls/had24_2p2_paley12.txt', 'runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt'] + ['runs/zerlegung/k3_seed%d.matrix.txt' % i for i in range(1, 9)] + ['runs/zerlegung/a_k12_t10-10_seed1.matrix.txt']
C = [frozenset(kl(wort(r)) for r in matrix(fn)) for fn in quellen]
rep = {}; zeilen = []
for l in open(Z + 'teil2bc_klassen.txt'):
    if l.startswith('#'):
        if 'Vertreter' in l: t = l.split(); rep[int(t[2].rstrip(':'))] = int(t[4].rstrip(','))
        continue
    a, b = l.split('|'); a = [int(x) for x in a.split()]; zeilen.append((a[0], a[1], a[2], (a[3:], [int(x) for x in b.split()])))
print("E  Zuordnung (b), (c): Vertreterdateien gleich den Loesungen der Liste: %s" % all(C[i] in S2 for i in range(8)))
for k, c, o, g in zeilen:
    gut = frozenset(kl(bild(g, w)) for w in C[k]) == C[rep[c]]
    if k >= 8: print("   %-52s -> Klasse %d (|Aut| %d laut Sitzung), Abbildung auf den Vertreter %s" % (quellen[k], c, o, "richtig" if gut else "FALSCH"))
# F
def inv(fn):
    A = np.array(matrix(fn), dtype=np.int64); out = []
    for B, name in ((A, 'Zeilen'), (A.T, 'Spalten')):
        n = len(B); G = np.abs(B @ B.T); iu = np.triu_indices(n, 1); out.append(tuple(sorted(Counter(G[iu].tolist()).items())))
        prod2 = {}; c4 = Counter()
        for i, j in itertools.combinations(range(n), 2): prod2[(i, j)] = B[i] * B[j]
        keys = list(prod2); M2 = np.array([prod2[k] for k in keys])
        for a in range(len(keys)):
            i, j = keys[a]
            sel = [b for b in range(a + 1, len(keys)) if keys[b][0] > j]          # i < j < k < l
            if sel: c4.update(np.abs(M2[sel] @ M2[a]).tolist())
        out.append(tuple(sorted(c4.items())))
    return tuple(out)
V = [Z + 'klasse%d_vertreter.txt' % i for i in range(9)]
I = [inv(fn) for fn in V]
namen = ['Zeilen-2', 'Zeilen-4', 'Spalten-2', 'Spalten-4']
for t in range(4):
    gr = {}
    for i in range(9): gr.setdefault(I[i][t], []).append(i)
    print("F  %-9s trennt in Gruppen: %s" % (namen[t], sorted(gr.values())))
gr = {}
for i in range(9): gr.setdefault(I[i], []).append(i)
print("   alle vier zusammen: %s; getrennte Paare %d von 36" % (sorted(gr.values()), sum(1 for i in range(9) for j in range(i + 1, 9) if I[i] != I[j])))
