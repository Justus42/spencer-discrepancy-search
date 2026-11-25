# Gate L (Orchestrator, 25.11.2026): Ist der 48er aus K=12, Startwert 5, zu einem bekannten 48er gleichwertig?
# Gleichwertig = vorzeichenbehaftete Spaltenpermutation. Farbgraph wie in notes/orchestrator/2026-11-25_gate_bahnen3/gate_b4.py.
import numpy as np, igraph as ig, collections, sys
from sympy.combinatorics import Permutation, PermutationGroup
REPO = sys.argv[1]; n = 20; FULL = (1 << n) - 1
def graph_words(words):
    S = sorted(set(words) | {w ^ FULL for w in words})
    k = [(2*j, 2*j+1) for j in range(n)]; f = [0]*(2*n) + [1]*len(S)
    for i, w in enumerate(S):
        for j in range(n): k.append((2*n+i, 2*j+1 if (w >> j) & 1 else 2*j))
    return ig.Graph(n=2*n+len(S), edges=k), f
def words_of(M): return {sum(1 << j for j in range(n) if r[j] == -1) for r in M.tolist()}
def lade_woerter(fn): return {int(t) for t in open(fn).read().split()}
neu = lade_woerter(f'{REPO}/runs/woerter_n20_R6_d2_K12_typ11-9_k12_seed5.txt')
X = np.arange(1 << n, dtype=np.uint32); d = np.full(1 << n, n, dtype=np.uint8)
for w in neu: np.minimum(d, np.bitwise_count(X ^ np.uint32(w)), out=d)
print("neuer Code:", len(neu), "Woerter, selbstkomplementaer:", all((w ^ FULL) in neu for w in neu), ", Radius (Brute-Force):", int(d.max()))
M = np.loadtxt(f'{REPO}/runs/leiter48/matrix_n20_48w_k12_seed5.txt', dtype=np.int64)
mw = words_of(M); print("Matrixdatei passt zur Wortdatei:", (mw | {w ^ FULL for w in mw}) == neu, M.shape)
gn, fn_ = graph_words(neu); print("Gruppenordnung des neuen Codes:", gn.count_automorphisms(color=fn_))
bek = {"1+3 (controls)": words_of(np.loadtxt(f'{REPO}/controls/had24_1p3_paley12.txt', dtype=np.int64)),
       "2+2 (controls)": words_of(np.loadtxt(f'{REPO}/controls/had24_2p2_paley12.txt', dtype=np.int64)),
       "Tabu-Treffer Bahnen3 Teil 4": words_of(np.loadtxt(f'{REPO}/runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt', dtype=np.int64)),
       "48er aus 52er Startwert 5": lade_woerter(f'{REPO}/runs/woerter_n20_R6_d2_K12_typ11-9_aus52seed5.txt'),
       "48er aus 52er Startwert 6": lade_woerter(f'{REPO}/runs/woerter_n20_R6_d2_K12_typ11-9_aus52seed6.txt')}
for nm, ws in bek.items():
    g, f = graph_words(ws)
    print(f"  {nm}: Gruppenordnung {g.count_automorphisms(color=f)}, gleichwertig zum neuen: {gn.isomorphic_bliss(g, color1=fn_, color2=f)}")
if len(sys.argv) > 2:      # alle 8 Klassen der P-invarianten 48er (wie gate_b4.py)
    A = np.loadtxt(f'{REPO}/controls/had24_1p3_paley12.txt', dtype=np.int64); g48, f48 = graph_words(words_of(A))
    gens = [Permutation(x) for x in g48.automorphism_group(color=f48)]; G = PermutationGroup(gens); P = G.sylow_subgroup(3)
    def actw(g, w):
        a = g.array_form; out = 0
        for j in range(n):
            img = a[2*j]; out |= (((w >> j) & 1) ^ (img % 2)) << (img // 2)
        return out
    Pel = list(P.elements); cp = lambda w: min(w, w ^ FULL)
    sols = [frozenset(cp(actw(g, w)) for w in [int(p.split('/')[0]) for p in line.split(':')[1].split()] for g in Pel) for line in open(f'{REPO}/runs/bahnen3/teil2_klein_B24_liste_4_56.txt')]
    sset = set(sols); seen = set(); reps = []
    for s in sols:
        if s in seen: continue
        comp = {s}; stack = [s]
        while stack:
            u = stack.pop()
            for g in gens:
                v = frozenset(cp(actw(g, w)) for w in u)
                if v not in comp: comp.add(v); stack.append(v)
        seen |= comp; reps.append(s)
    print("G-Bahnen unter den", len(sols), "Loesungen:", len(reps))
    cls = []
    for s in reps:
        g, f = graph_words(s); o = g.count_automorphisms(color=f)
        for c in cls:
            if c[2] == o and g.isomorphic_bliss(c[0], color1=f, color2=c[1]): break
        else: cls.append((g, f, o))
    print("Klassen:", len(cls), "Gruppenordnungen:", sorted(c[2] for c in cls))
    tr = [c[2] for c in cls if gn.isomorphic_bliss(c[0], color1=fn_, color2=c[1])]
    print("neuer Code gleichwertig zu einer der Klassen:", bool(tr), tr)
