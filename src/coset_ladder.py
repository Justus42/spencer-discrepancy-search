"""Leiter: Ueberdeckungen als Vereinigung von Nebenklassen eines linearen Codes D = <1,a,b,..>
(Dimension d) mit Einsvektor. Suchkern: src/coset_tabu.c (Walsh-Bewertung). Dieses Skript baut
D und die Paritaetspruefmatrix H, startet den Kern und prueft Treffer unabhaengig per
Brute-Force (Kugelradius ueber alle 2^n Punkte).
Aufruf: coset_ladder.py n R d K sekunden seed [typ ...]   typ = Vielfachheiten, z.B. 7,5,4,4"""
import sys, subprocess, itertools, os, tempfile
import numpy as np

BIN = os.environ.get("COSET_TABU_BIN", "/tmp/claude-0/coset_tabu")

def types_for(n, d):
    """Spaltentypen t in F2^(d-1); bis auf affine Gruppe: Vielfachheiten absteigend sortiert."""
    q = 1 << (d - 1)
    if d == 1: yield (n,); return
    def parts(total, k, mx):
        if k == 0:
            if total == 0: yield ()
            return
        for a in range(min(total, mx), -1, -1):
            for rest in parts(total - a, k - 1, a): yield (a,) + rest
    for p in parts(n, q, n):
        if sum(1 for a in p if a > 0) >= d: yield p   # Rang d: Typen nicht in einer affinen Geraden

def build(n, mult, d):
    """Liefert (Gerade G-Zeilen als Ints, H-Spalten (n Ints), m, Pivot-Positionen, Zeilen von H)."""
    q = 1 << (d - 1); cols = []
    for t, a in enumerate(mult): cols += [t] * a
    # Zeile 0 = Einsvektor, Zeile r>=1: Bit r-1 von t
    G = [(1 << n) - 1] + [sum(((cols[i] >> (r - 1)) & 1) << i for i in range(n)) for r in range(1, d)]
    # Nullraum von G (y mit <g,y>=0): Gauss ueber GF(2)
    rows = G[:]; piv = []
    r = 0
    for c in range(n):
        p = next((k for k in range(r, len(rows)) if (rows[k] >> c) & 1), None)
        if p is None: continue
        rows[r], rows[p] = rows[p], rows[r]
        for k in range(len(rows)):
            if k != r and (rows[k] >> c) & 1: rows[k] ^= rows[r]
        piv.append(c); r += 1
    assert r == d, "Rang von D < d"
    free = [c for c in range(n) if c not in piv]
    Hrows = []
    for fcol in free:
        y = 1 << fcol
        for k, pc in enumerate(piv):
            if (rows[k] >> fcol) & 1: y |= 1 << pc
        Hrows.append(y)
    m = n - d
    # RREF von H, damit Pivotspalten Einheitsvektoren sind (zur Rueckrechnung von Vertretern)
    rr = Hrows[:]; hp = []; r = 0
    for c in range(n):
        p = next((k for k in range(r, m) if (rr[k] >> c) & 1), None)
        if p is None: continue
        rr[r], rr[p] = rr[p], rr[r]
        for k in range(m):
            if k != r and (rr[k] >> c) & 1: rr[k] ^= rr[r]
        hp.append(c); r += 1
    assert r == m
    Hcols = [sum(((rr[k] >> i) & 1) << k for k in range(m)) for i in range(n)]
    return G, Hcols, m, hp, rr

def words(n, G, hp, c_list):
    d = len(G); D = [0]
    for g in G: D += [x ^ g for x in D]
    out = []
    for c in c_list:
        x = 0
        for k, pc in enumerate(hp):
            if (c >> k) & 1: x |= 1 << pc
        out += [x ^ dd for dd in D]
    return out

def covering_radius(n, W):
    X = np.arange(1 << n, dtype=np.uint32)
    dm = np.full(1 << n, n, dtype=np.uint8)
    for w in W: np.minimum(dm, np.bitwise_count(X ^ np.uint32(w)), out=dm)
    return int(dm.max())

def run(n, R, d, K, secs, seed, mult):
    G, Hc, m, hp, rr = build(n, mult, d)
    with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
        f.write(" ".join(map(str, Hc))); fn = f.name
    p = subprocess.run([BIN, str(m), str(n), str(R), str(K), str(seed), str(secs), fn],
                       capture_output=True, text=True)
    os.unlink(fn)
    sol = [l for l in p.stdout.splitlines() if l.startswith("SOLUTION")]
    last = [l for l in p.stderr.splitlines() if l.startswith("[")]
    best = last[-1].split("unueberdeckt")[1].strip() if last else "?"
    if not sol: return None, best, p.stderr.splitlines()[0] if p.stderr else ""
    cl = list(map(int, sol[0].split()[1:]))
    W = words(n, G, hp, cl)
    ok = len(set(W)) == len(W) and all((w ^ ((1 << n) - 1)) in set(W) for w in W)
    rad = covering_radius(n, W)
    return dict(words=W, nwords=len(set(W)), selfcompl=ok, radius=rad), "0", ""

if __name__ == "__main__":
    n, R, d, K, secs, seed = map(int, sys.argv[1:7])
    ts = [tuple(map(int, a.split(","))) for a in sys.argv[7:]] or list(types_for(n, d))
    for mult in ts:
        res, best, info = run(n, R, d, K, secs, seed, mult)
        if res: print(f"n={n} R={R} d={d} K={K} typ={mult}: GELOEST, Woerter={res['nwords']}, selbstkompl={res['selfcompl']}, Radius(Brute-Force)={res['radius']}", flush=True)
        else: print(f"n={n} R={R} d={d} K={K} typ={mult}: nicht geloest, kleinste Restzahl unueberdeckt Syndrome={best}", flush=True)
