import sys, numpy as np, igraph as ig
from sympy.combinatorics import Permutation, PermutationGroup
from collections import Counter
def gruppe(A):
    A = np.asarray(A, dtype=np.int64); A = A[:, np.any(A != 0, axis=0)]
    m, n = A.shape
    S = sorted({tuple(int(v) for v in z) for z in A} | {tuple(int(-v) for v in z) for z in A})
    kanten = [(2*j, 2*j+1) for j in range(n)]
    farben = [0]*(2*n) + [1]*len(S)
    for i, v in enumerate(S):
        for j, e in enumerate(v):
            kanten.append((2*n+i, 2*j if e == 1 else 2*j+1))
    g = ig.Graph(n=2*n+len(S), edges=kanten)
    gens = g.automorphism_group(color=farben)
    return n, len(S), S, PermutationGroup([Permutation(x) for x in gens])
def typ(perm, n, S):
    a = perm.array_form
    cols = Permutation([a[2*j]//2 for j in range(n)])
    # Zeilenpaare: Vektor-Knoten modulo Negation
    idx = {v: i for i, v in enumerate(S)}
    pair = {i: min(i, idx[tuple(-x for x in v)]) for i, v in enumerate(S)}
    seen = set(); cyc = []
    for i in range(len(S)):
        p = pair[i]
        if p in seen: continue
        L = 0; cur = i
        while pair[cur] not in seen:
            seen.add(pair[cur]); L += 1; cur = a[2*n+cur] - 2*n
        cyc.append(L)
    return tuple(sorted(Counter(len(c) for c in cols.full_cyclic_form).items())), tuple(sorted(Counter(cyc).items()))
for f in sys.argv[1:]:
    A = np.loadtxt(f, dtype=np.int64, ndmin=2)
    n, s, S, G = gruppe(A)
    print(f, A.shape, "Ordnung", G.order())
    for p in (3, 7):
        if G.order() % p: continue
        P = G.sylow_subgroup(p)
        print("  Sylow", p, "Ordnung", P.order(), "abelsch", P.is_abelian, "zyklisch", P.is_cyclic)
        types = Counter()
        for el in P.elements:
            if el.order() == 1: continue
            types[(el.order(),) + typ(el, n, S)] += 1
        for k, v in types.items(): print("    Ordnung", k[0], "Spaltenzyklen", k[1], "Zeilenpaar-Zyklen", k[2], "x", v)
        # Bahnen der ganzen Sylowgruppe auf Spalten
        orb = P.orbits()
        print("    Bahnen der Sylowgruppe: Spalten", sorted(Counter(len(o) for o in orb if min(o) < 2*n).items()), "(Literale)", " Vektoren", sorted(Counter(len(o) for o in orb if min(o) >= 2*n).items()))
