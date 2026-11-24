# Vortest Ansatz B: (20,6), Codes invariant unter der 3-Sylowgruppe P (Ordnung 9) des 48er-Codes had24_1p3_paley12.
# Teilfamilie: nur Zeilenbahnen der Groesse 1 und 3 (der 48er besteht aus 8 Bahnen der Groesse 3). Exaktes Minimum der Zeilenzahl.
import sys, numpy as np, igraph as ig, time
from sympy.combinatorics import Permutation, PermutationGroup
from ortools.sat.python import cp_model
A = np.loadtxt('/home/claude/justus42/spencer-discrepancy-search/controls/had24_1p3_paley12.txt', dtype=np.int64)
m, n = A.shape; R = 6
S = sorted({tuple(z) for z in A.tolist()} | {tuple(-z) for z in A})
kanten = [(2*j, 2*j+1) for j in range(n)]; farben = [0]*(2*n) + [1]*len(S)
for i, v in enumerate(S):
    for j, e in enumerate(v): kanten.append((2*n+i, 2*j if e == 1 else 2*j+1))
G = PermutationGroup([Permutation(x) for x in ig.Graph(n=2*n+len(S), edges=kanten).automorphism_group(color=farben)])
P = G.sylow_subgroup(3); print("Gruppe", G.order(), "Sylow-3", P.order())
N = 1 << n; xs = np.arange(N, dtype=np.uint32); mask = np.uint32(N - 1)
def act(g):                       # Tabelle: Wort (Bit j = 1 heisst Eintrag -1) -> Bild unter vorzeichenbehafteter Spaltenpermutation
    a = g.array_form; out = np.zeros(N, dtype=np.uint32)
    for j in range(n):
        img = a[2*j]; k, flip = img // 2, img % 2
        out |= (((xs >> j) & 1) ^ flip).astype(np.uint32) << k
    return out
els = [act(g) for g in P.elements]
# Kontrolle: Gruppe erhaelt den Code
code = np.array(sorted(sum(1 << j for j in range(n) if v[j] == -1) for v in S), dtype=np.uint32)
assert all(set(t[code].tolist()) == set(code.tolist()) for t in els)
rep = np.minimum(xs, xs ^ mask)
for t in els: rep = np.minimum(rep, np.minimum(t, t ^ mask))
reps, inv, cnt = np.unique(rep, return_inverse=True, return_counts=True)
size = cnt // 2                    # Bahnlaenge auf Zeilenpaaren
print("Bahnen auf Paaren {v,-v}:", dict(zip(*np.unique(size, return_counts=True))))
small = reps[size <= 3]; ssize = size[size <= 3]
orb = [sorted({int(min(t[g], t[g] ^ mask)) for t in els}) for g in small]
# Ueberdeckung der Punktklassen (dieselben Bahnen als Punkte)
C = np.zeros((len(small), len(reps)), dtype=bool)
for i, o in enumerate(orb):
    for w in o:
        dd = np.bitwise_count(np.uint32(w) ^ reps); C[i] |= (dd <= R) | (dd >= n - R)
codeorb = sorted({int(inv[c]) for c in code})
idx = {int(g): i for i, g in enumerate(small)}
in48 = [idx[int(reps[c])] for c in codeorb if int(reps[c]) in idx]
print("48er: Bahnen", len(codeorb), "davon in Teilfamilie", len(in48), "ueberdeckt alles:", bool(C[in48].any(axis=0).all()))
print("Teilfamilie:", len(small), "Bahnen, Groessen", dict(zip(*np.unique(ssize, return_counts=True))), "| Punktklassen", len(reps))
unc_all = ~C.any(axis=0)
print("von der ganzen Teilfamilie unueberdeckte Punktklassen:", int(unc_all.sum()))
mdl = cp_model.CpModel(); v = [mdl.NewBoolVar(str(i)) for i in range(len(small))]
cols = np.unique(C, axis=1)        # gleiche Spalten zusammenfassen
for c in cols.T: mdl.AddBoolOr([v[i] for i in np.where(c)[0]])
mdl.Minimize(sum(int(s) * x for s, x in zip(ssize, v)))
sv = cp_model.CpSolver(); sv.parameters.max_time_in_seconds = float(sys.argv[1]); sv.parameters.num_workers = 2
t0 = time.time(); st = sv.Solve(mdl)
print("CP-SAT", sv.StatusName(st), "Zeilen min", sv.ObjectiveValue() if st in (cp_model.OPTIMAL, cp_model.FEASIBLE) else None, "untere Schranke", sv.BestObjectiveBound(), f"{time.time()-t0:.0f}s")
if st in (cp_model.OPTIMAL, cp_model.FEASIBLE):
    sel = [i for i in range(len(small)) if sv.Value(v[i])]
    rows = [[-1 if (w >> j) & 1 else 1 for j in range(n)] for i in sel for w in orb[i]]
    np.savetxt("z9_best.txt", np.array(rows), fmt="%2d"); print("Zeilen", len(rows))
