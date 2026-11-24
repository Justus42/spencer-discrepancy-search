import numpy as np
from math import comb
A = np.loadtxt('/home/claude/justus42/spencer-discrepancy-search/controls/ctrl_16.txt', dtype=np.int64)[:, :15]
n = 15
X = ((np.arange(1 << n)[:, None] >> np.arange(n)) & 1) * 2 - 1
P = np.abs(X @ A.T) >= 7           # 2^15 x 16
print("ctrl_16: alle ueberdeckt:", P.any(axis=1).all())
print("offene Punkte nach Streichen je einer Zeile:", sorted({int((~np.delete(P, i, axis=1).any(axis=1)).sum()) for i in range(16)}))
V = lambda m, R: sum(comb(m, k) for k in range(R + 1))
for (m, R, w) in [(15,4,30),(15,4,32),(20,6,40),(20,6,48),(25,8,50),(25,8,64),(30,10,62),(30,10,128)]:
    print((m, R), w, "Woerter: Dichte %.3f" % (w * V(m, R) / 2**m))
