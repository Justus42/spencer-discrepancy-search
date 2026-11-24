# Exakte Suche n=11, d=6, Zeilen mit genau einer Null (Katona-Doppelkugeln). 11 Zeilen, 1024 Punktklassen, je Zeile 112.
# Budget: Summe der Mehrfachueberdeckung = 11*112 - 1024 = 208. Verzweigen am Punkt mit den wenigsten zulaessigen Zeilen.
import numpy as np, itertools, sys, time
n, d, N = int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
X = np.arange(1 << (n - 1), dtype=np.int64)
Xs = 1 - 2 * ((X[:, None] >> np.arange(n)) & 1)
rows = []
for y in range(n):
    oth = [j for j in range(n) if j != y]
    for bits in range(1 << (n - 2)):
        a = np.zeros(n, dtype=np.int64); a[oth[0]] = 1
        for t, j in enumerate(oth[1:]):
            a[j] = -1 if (bits >> t) & 1 else 1
        rows.append(a)
R = np.array(rows)
C = (np.abs(Xs @ R.T) >= d)
Cf = C.astype(np.float32)
SZ = int(C[:, 0].sum()); BUD = N * SZ - len(X)
T0 = time.time(); LIMIT = float(sys.argv[1]); nodes = 0; best = [len(X), None]; done_top = 0
class Timeout(Exception): pass
def dfs(unc, chosen, budget):
    global nodes
    nodes += 1
    if time.time() - T0 > LIMIT: raise Timeout
    U = int(unc.sum())
    if U < best[0]: best[0], best[1] = U, list(chosen)
    if U == 0: return True
    k = N - len(chosen)
    if k == 0: return False
    gains = unc.astype(np.float32) @ Cf                      # neu ueberdeckte Klassen je Zeile
    viable = gains >= max(1, SZ - budget)
    if not viable.any(): return False
    g = np.sort(gains[viable])[::-1][:k]
    if g.sum() < U: return False
    idxU = np.where(unc)[0]
    cnt = Cf[idxU][:, viable].sum(axis=1)
    j = int(np.argmin(cnt))
    if cnt[j] == 0: return False
    p = idxU[j]
    cand = np.where(viable & C[p])[0]
    cand = cand[np.argsort(-gains[cand])]
    for r in cand:
        gr = int(gains[r])
        if dfs(unc & ~C[:, r], chosen + [int(r)], budget - (SZ - gr)): return True
    return False
unc0 = ~C[:, 0]
try:
    ok = dfs(unc0, [0], BUD)
    print("ERGEBNIS vollstaendig:", "LOESUNG" if ok else "keine Loesung", "Knoten", nodes, f"{time.time()-T0:.0f}s")
except Timeout:
    print("Zeitgrenze, Knoten", nodes)
print("kleinste Restzahl (Klassen)", best[0], "mit", len(best[1]), "Zeilen")
if best[0] == 0:
    M = R[best[1]]; np.savetxt("nX_treffer.txt", M, fmt="%2d"); print(M)
# Nebenbei: Verteilung der paarweisen Schnitte mit Zeile 0
inter = (Cf[:, 0] @ Cf).astype(int)
print("Schnittgroessen mit Zeile 0:", sorted(set(inter.tolist())))
