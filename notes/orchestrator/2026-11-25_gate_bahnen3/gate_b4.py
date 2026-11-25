# Aequivalenzklassen der 19008 Loesungen: erst G-Bahnen (654), dann paarweiser Isomorphietest je Gruppenordnung.
import numpy as np, igraph as ig, collections
from sympy.combinatorics import Permutation, PermutationGroup
REPO = '/home/claude/spencer-discrepancy-search'; n = 20; FULL = (1 << n) - 1
def graph_words(words):
    S = sorted(set(words) | {w ^ FULL for w in words})
    k = [(2*j, 2*j+1) for j in range(n)]; f = [0]*(2*n) + [1]*len(S)
    for i, w in enumerate(S):
        for j in range(n): k.append((2*n+i, 2*j+1 if (w >> j) & 1 else 2*j))
    return ig.Graph(n=2*n+len(S), edges=k), f
def words_of(M): return {sum(1 << j for j in range(n) if r[j] == -1) for r in M.tolist()}
A = np.loadtxt(f'{REPO}/controls/had24_1p3_paley12.txt', dtype=np.int64)
g48, f48 = graph_words(words_of(A))
gens = [Permutation(x) for x in g48.automorphism_group(color=f48)]
G = PermutationGroup(gens); P = G.sylow_subgroup(3)
def actw(g, w):
    a = g.array_form; out = 0
    for j in range(n):
        img = a[2*j]; out |= (((w >> j) & 1) ^ (img % 2)) << (img // 2)
    return out
Pel = list(P.elements)
def canonpair(w): return min(w, w ^ FULL)
sols = []
for line in open(f'{REPO}/runs/bahnen3/teil2_klein_B24_liste_4_56.txt'):
    ws = [int(p.split('/')[0]) for p in line.split(':')[1].split()]
    sols.append(frozenset(canonpair(actw(g, w)) for w in ws for g in Pel))
assert all(len(s) == 24 for s in sols); sset = set(sols)
seen = set(); reps = []
for s in sols:
    if s in seen: continue
    comp = {s}; stack = [s]
    while stack:
        u = stack.pop()
        for g in gens:
            v = frozenset(canonpair(actw(g, w)) for w in u)
            assert v in sset
            if v not in comp: comp.add(v); stack.append(v)
    seen |= comp; reps.append((s, len(comp)))
print("G-Bahnen:", len(reps))
buckets = collections.defaultdict(list)
for s, sz in reps:
    g, f = graph_words(s); buckets[g.count_automorphisms(color=f)].append((g, f, sz, s))
bek = {nm: graph_words(words_of(np.loadtxt(f'{REPO}/{pf}', dtype=np.int64))) for nm, pf in (("1+3", 'controls/had24_1p3_paley12.txt'), ("2+2", 'controls/had24_2p2_paley12.txt'), ("Teil-4-Treffer", 'runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt'))}
total = 0
for order in sorted(buckets, reverse=True):
    cls = []
    for g, f, sz, s in buckets[order]:
        for c in cls:
            if g.isomorphic_bliss(c[0], color1=f, color2=c[1]): c[2] += sz; break
        else: cls.append([g, f, sz])
    names = [[nm for nm, (bg, bf) in bek.items() if c[0].isomorphic_bliss(bg, color1=c[1], color2=bf)] for c in cls]
    total += len(cls)
    print(f"Gruppenordnung {order}: {len(cls)} Klassen, Loesungen je Klasse {sorted(c[2] for c in cls)}, bekannte darin: {[x for x in names if x]}")
print("Aequivalenzklassen insgesamt:", total)
