# Gate zu Auftrag C: die 112 Loesungen (15,4), Ordnung 7, 32 Woerter mit eigenem Code neu erzeugen und nach Aequivalenz sortieren.
import sys, numpy as np, igraph as ig, collections
sys.path.insert(0, '/home/claude/orch')
from sym_enum import run, pairs_with_fixed, rows_of
REPO = '/home/claude/spencer-discrepancy-search'
n = 15
gfull, gfix, Cf, Cx, reps, sig = run(n, 4, 7, 2, "p7")
sols, cnt = pairs_with_fixed(Cf, Cx, 2)
def graph(M):
    M = np.asarray(M); S = sorted({tuple(z) for z in M.tolist()} | {tuple(-z) for z in M})
    k = [(2*j, 2*j+1) for j in range(n)]; f = [0]*(2*n) + [1]*len(S)
    for i, v in enumerate(S):
        for j, e in enumerate(v): k.append((2*n+i, 2*j if e == 1 else 2*j+1))
    return ig.Graph(n=2*n+len(S), edges=k), f
C16 = np.loadtxt(f'{REPO}/controls/ctrl_16.txt', dtype=np.int64); C16 = C16[:, np.any(C16 != 0, axis=0)]
g0, f0 = graph(C16)
cls = []
for S, i, j in sols:
    M = rows_of(n, 7, sig, gfull[i], True) + rows_of(n, 7, sig, gfull[j], True) + [rows_of(n, 7, sig, gfix[s], False)[0] for s in S]
    g, f = graph(M)
    for c in cls:
        if g.isomorphic_bliss(c[0], color1=f, color2=c[1]): c[2] += 1; break
    else: cls.append([g, f, 1, np.array(M)])
print("Loesungen:", len(sols), "| Klassen:", len(cls))
for c in cls:
    G = c[3] @ c[3].T; off = sorted(set(G[~np.eye(16, dtype=bool)].tolist()))
    print("  Klasse mit", c[2], "Loesungen, Gruppenordnung", c[0].count_automorphisms(color=c[1]), "| aequivalent zu ctrl_16:", c[0].isomorphic_bliss(g0, color1=c[1], color2=f0), "| Skalarprodukte der Zeilen:", off)
print("ctrl_16 Gruppenordnung", g0.count_automorphisms(color=f0))
# Zusatz: beide Klassen zu Hadamard-Matrizen der Ordnung 16 ergaenzen und auf Hadamard-Aequivalenz (auch zur Transponierten) pruefen.
def ergaenze(M):
    G = M @ M.T; c = np.ones(16, dtype=np.int64)
    for i in range(1, 16): c[i] = -G[0, i]
    H = np.hstack([M, c[:, None]]); assert (H @ H.T == 16 * np.eye(16, dtype=np.int64)).all(); return H
def hgraph(H):
    m = H.shape[0]; e = [(2*i, 2*i+1) for i in range(2*m)]
    for i in range(m):
        for j in range(m):
            if H[i, j] == 1: e += [(2*i, 2*(m+j)), (2*i+1, 2*(m+j)+1)]
            else: e += [(2*i, 2*(m+j)+1), (2*i+1, 2*(m+j))]
    return ig.Graph(n=4*m, edges=e), [0]*(2*m) + [1]*(2*m)
Hs = [ergaenze(c[3]) for c in cls]; names = ["Klasse ohne ctrl_16", "Klasse mit ctrl_16"]
def syl(k):
    H = np.array([[1]]);
    for _ in range(k): H = np.block([[H, H], [H, -H]])
    return H
gs = [hgraph(H) for H in Hs]; gt = [hgraph(H.T) for H in Hs]; gsy = hgraph(syl(4))
print("Hadamard-aequivalent untereinander:", gs[0][0].isomorphic_bliss(gs[1][0], color1=gs[0][1], color2=gs[1][1]))
print("Klasse A aequivalent zur Transponierten von Klasse B:", gs[0][0].isomorphic_bliss(gt[1][0], color1=gs[0][1], color2=gt[1][1]))
for k in (0, 1):
    print(names[k], "| selbst-transponiert:", gs[k][0].isomorphic_bliss(gt[k][0], color1=gs[k][1], color2=gt[k][1]), "| Sylvester:", gs[k][0].isomorphic_bliss(gsy[0], color1=gs[k][1], color2=gsy[1]), "| Hadamard-Gruppenordnung:", gs[k][0].count_automorphisms(color=gs[k][1]))
