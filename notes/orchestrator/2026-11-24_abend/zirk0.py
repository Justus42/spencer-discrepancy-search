# Zirkulante n x n mit Nulldiagonale (eine Null je Zeile, in jeder Zeile woanders): exakte Diskrepanz aller ersten Zeilen.
import numpy as np, sys, time
def run(n, need):
    t0 = time.time()
    X = 1 - 2 * ((np.arange(1 << (n - 1))[:, None] >> np.arange(n)) & 1)       # x modulo Vorzeichen
    units = [u for u in range(1, n) if np.gcd(u, n) == 1]
    seen = set(); best = 0; hist = {}; cls = 0
    for bits in range(1 << (n - 2)):                                           # a[1] = +1
        a = [0, 1] + [(-1 if (bits >> t) & 1 else 1) for t in range(n - 2)]
        key = min(min(tuple(s * a[(u * j) % n] for j in range(n)) for s in (1, -1)) for u in units)
        if key in seen: continue
        seen.add(key); cls += 1
        A = np.array([[a[(j - i) % n] for j in range(n)] for i in range(n)])
        d = int(np.abs(X @ A.T).max(axis=1).min())
        hist[d] = hist.get(d, 0) + 1; best = max(best, d)
    print(f"n={n}: {cls} Klassen, Diskrepanzen {dict(sorted(hist.items()))}, Maximum {best}, noetig {need}, {time.time()-t0:.0f}s", flush=True)
run(8, 5); run(11, 6); run(14, 7); run(17, 8)
