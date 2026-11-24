# Gegenrechnung Gemini: Kleitman-Dichte je n (reelle Matrizen), und ctrl_17 = Hadamard 16 + Spalte?
from math import comb, sqrt, floor
import numpy as np
V = lambda n, r: sum(comb(n, i) for i in range(r + 1)) if r >= 0 else 0
def Kl(n, D):
    if D < 0: return 0
    if D >= n: return 2**n
    return V(n, D // 2) if D % 2 == 0 else 2 * V(n - 1, D // 2)
out = []
for n in range(1, 34):
    t = 5 * sqrt(n / 8)
    # disc > t noetig; Kappe {<a,x> >= d} hat Durchmesser <= floor(n - d) < n - t  -> groesstes ganzes D mit D < n - t
    D = int(np.ceil(n - t)) - 1
    dens = 2 * n * Kl(n, D) / 2**n
    out.append((n, D, round(dens, 3)))
print("n, D, noetige Dichte:", out)
print("ausgeschlossen (Dichte < 1):", [n for n, D, d in out if d < 1])
for f in ("ctrl_17", "L2026_17x17", "ctrl_16"):
    A = np.loadtxt(f"/home/claude/justus42/spencer-discrepancy-search/controls/{f}.txt", dtype=np.int64)
    A = A[:, np.any(A != 0, axis=0)]; A = A[np.any(A != 0, axis=1)]
    r, c = A.shape
    res = []
    for j in range(c):
        B = np.delete(A, j, axis=1); G = B @ B.T
        res.append(bool((G == (c - 1) * np.eye(r, dtype=np.int64)).all()))
    G = A @ A.T
    print(f, A.shape, "Zeilen orthogonal nach Streichen einer Spalte:", [j for j, v in enumerate(res) if v], "| Gram-Nebendiagonale:", sorted(set(G[~np.eye(r, dtype=bool)].tolist())))
