# Vortest: n=11, d=6. Zeilen in {-1,0,1}^11 mit geradem Traeger (10, 8 oder 6 Eintraege). 11 Zeilen, jedes x braucht |<a,x>| >= 6.
import numpy as np, itertools, sys, time
from ortools.sat.python import cp_model
n, d, N = 11, 6, 11
supports = [int(s) for s in sys.argv[1].split(',')]
X = np.arange(1 << (n - 1), dtype=np.int64)               # x mit x_10 = +1 (Klassen modulo Vorzeichen)
Xs = 1 - 2 * ((X[:, None] >> np.arange(n)) & 1)            # 1024 x 11
rows = []
for k in supports:
    for S in itertools.combinations(range(n), k):
        for bits in range(1 << (k - 1)):                   # erstes Vorzeichen +1
            a = np.zeros(n, dtype=np.int64)
            for t, j in enumerate(S):
                a[j] = 1 if t == 0 or not (bits >> (t - 1)) & 1 else -1
            rows.append(a)
R = np.array(rows)
C = (np.abs(Xs @ R.T) >= d)                                 # 1024 x Zeilenzahl
print("Zeilenkandidaten", len(R), "Punkte-Klassen", len(X), "Ueberdeckung je Zeile", sorted(set(C.sum(axis=0).tolist())), flush=True)
m = cp_model.CpModel()
v = [m.NewBoolVar(f"r{i}") for i in range(len(R))]
for p in range(len(X)):
    m.AddBoolOr([v[i] for i in np.where(C[p])[0]])
m.Add(sum(v) <= N)
# Symmetriebrechung: x = (1,...,1) wird von einer Zeile ueberdeckt, deren Eintraege (nach Vorzeichenwechsel der Spalten) ... hier nur: Klasse 0 durch Zeile mit kleinstem Index
s = cp_model.CpSolver(); s.parameters.max_time_in_seconds = float(sys.argv[2]); s.parameters.num_workers = 2
t = time.time(); st = s.Solve(m)
print("Status", s.StatusName(st), f"{time.time()-t:.0f}s")
if st in (cp_model.OPTIMAL, cp_model.FEASIBLE):
    sel = [i for i in range(len(R)) if s.Value(v[i])]
    M = R[sel]; print(M)
    np.savetxt("n11_treffer.txt", M, fmt="%2d")
