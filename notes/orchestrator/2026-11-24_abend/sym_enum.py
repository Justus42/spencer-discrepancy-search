# Orchestrator-Vortest 24.11.2026: selbstkomplementaere Ueberdeckungscodes, invariant unter
# einer Spaltenpermutation sigma von Primzahlordnung p (c Zyklen der Laenge p, f feste Spalten).
# Vollstaendige Aufzaehlung fuer "a volle Zeilenbahnen + b feste Zeilen", a in {1,2}.
import numpy as np, itertools, sys, time

def setup(n, p, c):
    N = 1 << n
    xs = np.arange(N, dtype=np.uint32)
    perm = list(range(n))
    for k in range(c):
        for j in range(p):
            perm[k*p + j] = k*p + (j+1) % p
    sig = np.zeros(N, dtype=np.uint32)
    for j in range(n):
        sig |= ((xs >> j) & 1) << perm[j]
    mask = np.uint32(N - 1)
    rep = np.minimum(xs, xs ^ mask)
    cur = xs.copy()
    size = np.ones(N, dtype=np.int32)
    for _ in range(p - 1):
        cur = sig[cur]
        rep = np.minimum(rep, np.minimum(cur, cur ^ mask))
    fixed = sig == xs           # sigma-feste Woerter (blockkonstant)
    reps = np.unique(rep)
    return xs, sig, reps, fixed

def cover_matrix(n, R, p, sig, gens, xreps, full):
    """bool [len(gens), len(xreps)]: x-Klasse von Bahn(gen) ueberdeckt"""
    C = np.zeros((len(gens), len(xreps)), dtype=bool)
    cur = gens.copy()
    for _ in range(p if full else 1):
        d = np.bitwise_count(cur[:, None] ^ xreps[None, :])
        C |= (d <= R) | (d >= n - R)
        cur = sig[cur]
    return C

def run(n, R, p, c, label=""):
    t0 = time.time()
    xs, sig, reps, fixed = setup(n, p, c)
    isfix = fixed[reps]
    gfull = reps[~isfix]            # Vertreter voller Bahnen (p Zeilen = 2p Woerter)
    gfix = reps[isfix]              # feste Zeilen (je 2 Woerter)
    w = np.where(isfix, 2, 2*p)     # Punkte je x-Klasse
    assert w.sum() == 1 << n
    Cf = cover_matrix(n, R, p, sig, gfull, reps, True)
    Cx = cover_matrix(n, R, p, sig, gfix, reps, False)
    print(f"[{label}] n={n} R={R} p={p} Zyklen={c} fest={n-p*c}: x-Klassen {len(reps)}, volle Bahnen {len(gfull)}, feste Zeilen {len(gfix)}, Aufbau {time.time()-t0:.1f}s", flush=True)
    return gfull, gfix, Cf, Cx, reps, sig

def pairs_with_fixed(Cf, Cx, b):
    """zaehlt Loesungen: 2 volle Bahnen + genau die b festen Zeilen einer b-Menge; liefert Liste (S, i, j)"""
    sols = []; cnt = {}
    for S in itertools.combinations(range(len(Cx)), b):
        cov = np.zeros(Cf.shape[1], dtype=bool)
        for s in S: cov |= Cx[s]
        U = (~Cf[:, ~cov]).astype(np.float32)       # unueberdeckt je Bahn, nur relevante Spalten
        M = U @ U.T
        ii, jj = np.where(np.triu(M < 0.5, 1))
        cnt[S] = len(ii)
        for i, j in zip(ii, jj): sols.append((S, int(i), int(j)))
        dd = np.where(np.diag(M) < 0.5)[0]
        for i in dd: sols.append((S, int(i), int(i)))
    return sols, cnt

def one_with_fixed(Cf, Cx, b):
    sols = []
    combos = list(itertools.combinations(range(len(Cx)), b))
    covS = np.zeros((len(combos), Cf.shape[1]), dtype=bool)
    for k, S in enumerate(combos):
        for s in S: covS[k] |= Cx[s]
    for i in range(len(Cf)):
        need = ~Cf[i]
        ok = covS[:, need].all(axis=1)
        for k in np.where(ok)[0]: sols.append((combos[k], i))
    return sols

def rows_of(n, p, sig, g, full):
    out = []; cur = np.array([g], dtype=np.uint32)
    for _ in range(p if full else 1):
        v = int(cur[0]); out.append([-1 if (v >> j) & 1 else 1 for j in range(n)]); cur = sig[cur]
    return out
