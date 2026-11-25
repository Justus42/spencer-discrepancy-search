# Gate zu Auftrag B (Bahnen unter der 3-Sylowgruppe des 48ers), eigene Programme des Orchestrators.
import sys, time, numpy as np, igraph as ig
from sympy.combinatorics import Permutation, PermutationGroup
REPO = '/home/claude/spencer-discrepancy-search'
A = np.loadtxt(f'{REPO}/controls/had24_1p3_paley12.txt', dtype=np.int64); m, n = A.shape; R = 6
def graph(M):
    S = sorted({tuple(z) for z in M.tolist()} | {tuple(-z) for z in M})
    k = [(2*j, 2*j+1) for j in range(n)]; f = [0]*(2*n) + [1]*len(S)
    for i, v in enumerate(S):
        for j, e in enumerate(v): k.append((2*n+i, 2*j if e == 1 else 2*j+1))
    return ig.Graph(n=2*n+len(S), edges=k), f, S
g48, f48, S = graph(A)
gens = [Permutation(x) for x in g48.automorphism_group(color=f48)]
G = PermutationGroup(gens); P = G.sylow_subgroup(3)
print("G", G.order(), "P", P.order(), "Erzeuger von G:", len(gens))
N = 1 << n; xs = np.arange(N, dtype=np.uint32); mask = np.uint32(N - 1)
def act(g):
    a = g.array_form; out = np.zeros(N, dtype=np.uint32)
    for j in range(n):
        img = a[2*j]; out |= (((xs >> j) & 1) ^ (img % 2)).astype(np.uint32) << (img // 2)
    return out
els = [act(g) for g in P.elements]
rep = np.minimum(xs, xs ^ mask)
for t in els: rep = np.minimum(rep, np.minimum(t, t ^ mask))
reps, inv, cnt = np.unique(rep, return_inverse=True, return_counts=True); size = cnt // 2
small = reps[size <= 3]; ssize = size[size <= 3]; sidx = {int(g): i for i, g in enumerate(small)}
orb = [sorted({int(min(t[g], t[g] ^ mask)) for t in els}) for g in small]
C = np.zeros((len(small), len(reps)), dtype=bool)
for i, o in enumerate(orb):
    for w in o:
        dd = np.bitwise_count(np.uint32(w) ^ reps); C[i] |= (dd <= R) | (dd >= n - R)
# --- G1: Liste der 19008 Loesungen
sols = []
for line in open(f'{REPO}/runs/bahnen3/teil2_klein_B24_liste_4_56.txt'):
    parts = line.split(':')[1].split()
    ws = [int(p.split('/')[0]) for p in parts]; gw = [int(p.split('/')[1]) for p in parts]
    ids = tuple(sorted(sidx[int(rep[w])] for w in ws))
    assert all(ssize[i] == g for i, g in zip([sidx[int(rep[w])] for w in ws], gw))
    sols.append(ids)
sset = set(sols)
ok = sum(bool(C[list(s)].any(axis=0).all()) for s in sols)
print(f"G1: {len(sols)} Zeilen, verschieden {len(sset)}, ueberdecken nach meiner Matrix: {ok}, alle aus 8 Dreierbahnen: {all(len(s)==8 and all(ssize[i]==3 for i in s) for s in sols)}")
code = tuple(sorted({sidx[int(rep[sum(1 << j for j in range(n) if v[j] == -1)])] for v in S}))
print("    48er in der Liste:", code in sset)
# --- G2: Abgeschlossenheit unter G und weiteren Normalisator-Elementen
def closed(tab, name):
    img = np.array([sidx.get(int(rep[tab[g]]), -1) for g in small])
    if (img < 0).any(): print(f"    {name}: bildet kleine Bahnen nicht auf kleine ab"); return
    bad = sum(tuple(sorted(img[list(s)])) not in sset for s in sols)
    print(f"    {name}: Loesungen ausserhalb der Liste nach Anwendung: {bad}")
print("G2:")
for i, g in enumerate(gens): closed(act(g), f"G-Erzeuger {i}")
colorb = {}
for o in P.orbits():
    cols = sorted({v // 2 for v in o if v < 2*n})
    if cols: colorb[tuple(cols)] = 1
for cols in colorb:
    closed(xs ^ np.uint32(sum(1 << c for c in cols)), f"Vorzeichenwechsel Spaltenbahn der Laenge {len(cols)}")
# Bahnen der Loesungen unter G
import collections
tabs = [np.array([sidx[int(rep[act(g)[w]])] for w in small]) for g in gens]
seen = set(); klassen = []
for s in sols:
    if s in seen: continue
    comp = {s}; stack = [s]
    while stack:
        u = stack.pop()
        for tb in tabs:
            v = tuple(sorted(tb[list(u)]))
            if v not in comp: comp.add(v); stack.append(v)
    seen |= comp; klassen.append(len(comp))
print("    G-Bahnen der Loesungen:", len(klassen), "Groessen:", dict(collections.Counter(klassen)))
# --- G5: Treffer aus Teil 4
H = np.loadtxt(f'{REPO}/runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt', dtype=np.int64)
gh, fh, Sh = graph(H)
print("G5: Treffer", H.shape, "Woerter", len(Sh), "Gruppenordnung", gh.count_automorphisms(color=fh), "| aequivalent zum 48er (1+3):", gh.isomorphic_bliss(g48, color1=fh, color2=f48), end="")
B2 = np.loadtxt(f'{REPO}/controls/had24_2p2_paley12.txt', dtype=np.int64); g2, f2, _ = graph(B2)
print(" | zum 2+2:", gh.isomorphic_bliss(g2, color1=fh, color2=f2), "| 1+3 aequivalent 2+2:", g48.isomorphic_bliss(g2, color1=f48, color2=f2))
np.save('/home/claude/orch/C_small.npy', C); np.save('/home/claude/orch/ssize.npy', ssize)
