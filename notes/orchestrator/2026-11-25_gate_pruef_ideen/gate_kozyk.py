#!/usr/bin/env python3
"""gate_kozyk.py -- Gate zu Auftrag P, Teil 3 (Orchestrator). Eigene Gruppentafeln (Praesentationen, ganze Zahlen),
eigene Matrix, alle gespeicherten Gegenbeispiele der Sitzung nachgerechnet (max_i |<M_i,x>| <= 6).
Uebernommen ist nur die dokumentierte Nummerierung (Kopf von pruef_kozyk.py): Direktprodukt Index f0 + |F0|*(f1 + |F1|*f2);
DIC(m): a^i b^j -> i + 2m*j; DIH(m): r^i s^j -> i + m*j; Vertreter = kleinerer Index von {g, zg};
Funktionsnummer F: Bit k-1 = 1 heisst f(Vertreter k) = -1; x: Bit h = 1 heisst -1; 3 Bytes je Funktion, Position = F.
Zusatz: die Dateien in beste/ werden mit der eigenen Matrix verglichen.
Aufruf: gate_kozyk.py VERZEICHNIS"""
import sys, gzip, glob, os, re, itertools, numpy as np
def cyc(m):  return m, (lambda a, b: (a + b) % m)
def dic(m):
    def mul(a, b):
        i, j, k, l = a % (2*m), a // (2*m), b % (2*m), b // (2*m)
        e = (i - k) if j else (i + k)
        if j + l == 2: e += m
        return e % (2*m) + 2*m*((j + l) % 2)
    return 4*m, mul
def dih(m):
    def mul(a, b):
        i, j, k, l = a % m, a // m, b % m, b // m
        return ((i - k) if j else (i + k)) % m + m*((j + l) % 2)
    return 2*m, mul
def prod(*fs):
    sizes = [f[0] for f in fs]
    def split(a):
        out = []
        for s in sizes: out.append(a % s); a //= s
        return out
    def mul(a, b):
        r, w = 0, 1
        for (s, mu), x, y in zip(fs, split(a), split(b)):
            r += w * mu(x, y); w *= s
        return r
    def idx(parts):
        r, w = 0, 1
        for s, p in zip(sizes, parts): r += w*p; w *= s
        return r
    return int(np.prod(sizes)), mul, idx
G = {  # Name: (Faktoren, z als Komponenten)
 "Z36": ([cyc(36)], [18]), "Dic9": ([dic(9)], [9]), "Z3xDic3": ([cyc(3), dic(3)], [0, 3]),
 "Z40": ([cyc(40)], [20]), "Z5xQ8": ([cyc(5), dic(2)], [0, 2]), "Z5xD4": ([cyc(5), dih(4)], [0, 2]),
 "Z5xZ4xZ2": ([cyc(5), cyc(4), cyc(2)], [0, 2, 0]), "Dic10": ([dic(10)], [10]), "D20": ([dih(20)], [10]) }
def tafel(name):
    fs, zc = G[name]; o, mul, idx = prod(*fs); z = idx(zc)
    T = np.array([[mul(a, b) for b in range(o)] for a in range(o)])
    assert all(sorted(T[a]) == list(range(o)) for a in range(o)) and (T[0] == np.arange(o)).all()
    assert (T[T] == T[:, T]).all()                      # (ab)c = a(bc) fuer alle Tripel
    assert z != 0 and T[z, z] == 0 and (T[z] == T[:, z]).all()   # z zentral, Ordnung 2
    inv = T.argmin(axis=1)
    n = o // 2; rep = [g for g in range(o) if g < T[z, g]]; rang = {g: k for k, g in enumerate(rep)}
    assert len(rep) == n and rep[0] == 0
    IDX = np.zeros((n, n), dtype=np.int64); SGN = np.zeros((n, n), dtype=np.int8)
    for a in range(n):
        for b in range(n):
            e = T[inv[rep[a]], rep[b]]
            IDX[a, b], SGN[a, b] = (rang[e], 1) if e in rang else (rang[T[z, e]], -1)
    nichtabelsch = not (T == T.T).all()
    ordn = {}
    for g in range(o):
        k, h = 1, g
        while h != 0: h = T[h, g]; k += 1
        ordn[k] = ordn.get(k, 0) + 1
    return n, IDX, SGN, nichtabelsch, dict(sorted(ordn.items()))
def matrix(F, n, IDX, SGN):
    f = np.ones(n, dtype=np.int64); f[1:] = 1 - 2*((F >> np.arange(n-1)) & 1)
    return SGN * f[IDX]
d = sys.argv[1]; allesok = True
for name in G:
    n, IDX, SGN, nab, ordn = tafel(name); N = 1 << (n-1)
    dat = b"".join(gzip.open("%s/%s_n%d.shard%d.bin.gz" % (d, name, n, s), "rb").read() for s in range(4))
    assert len(dat) == 3*N, (name, len(dat))
    xs = np.frombuffer(dat, dtype=np.uint8).reshape(N, 3).astype(np.int64); xs = xs[:, 0] | (xs[:, 1] << 8) | (xs[:, 2] << 16)
    fehl = int((xs >= (1 << n)).sum()); hist = {}
    schlecht = 0
    for lo in range(0, N, 1 << 16):
        Fs = np.arange(lo, min(N, lo + (1 << 16)), dtype=np.int64)
        fv = np.ones((len(Fs), n), dtype=np.int16); fv[:, 1:] = 1 - 2*((Fs[:, None] >> np.arange(n-1)[None, :]) & 1)
        x = (1 - 2*((xs[lo:lo+len(Fs), None] >> np.arange(n)[None, :]) & 1)).astype(np.int16)
        mx = np.zeros(len(Fs), dtype=np.int16)
        for g in range(n):
            mx = np.maximum(mx, np.abs((fv[:, IDX[g]] * SGN[g].astype(np.int16) * x).sum(axis=1)))
        schlecht += int((mx > 6).sum())
        for v, c in zip(*np.unique(mx, return_counts=True)): hist[int(v)] = hist.get(int(v), 0) + int(c)
    ok = (schlecht == 0 and fehl == 0); allesok &= ok
    print("%-9s n=%d %s Elementordnungen %s | Funktionen %d, Fehlstellen %d, max > 6: %d, Maxima %s -> %s"
          % (name, n, "nichtabelsch" if nab else "abelsch", ordn, N, fehl, schlecht, hist, "bestaetigt" if ok else "FEHLER"))
    for fn in sorted(f for f in glob.glob("%s/beste/%s_n%d_F*.txt" % (d, name, n)) if "pruefung" not in f):
        F = int(re.search(r"_F(\d+)", fn).group(1))
        Mf = np.array([[int(t) for t in l.split()] for l in open(fn) if l.split()])
        Mi = matrix(F, n, IDX, SGN)
        had = bool((Mi @ Mi.T == n*np.eye(n, dtype=np.int64)).all())
        print("   %s: gleich der eigenen Matrix: %s, Hadamard: %s" % (os.path.basename(fn), bool(Mf.shape == Mi.shape and (Mf == Mi).all()), had))
print("GESAMT:", "alle Gegenbeispiele bestaetigt" if allesok else "FEHLER")
