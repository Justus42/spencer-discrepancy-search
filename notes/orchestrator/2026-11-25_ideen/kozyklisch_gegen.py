#!/usr/bin/env python3
"""Gegenprobe zum Sieb kozyklisch.c (Orchestrator, 25.11.2026), unabhaengig aufgebaut, nur ganze Zahlen:
(1) negazyklisch n = 8, 10: Zahl der ersten Zeilen (erster Eintrag +1) mit disc >= d, vollstaendig ueber alle x.
(2) Williamson-Anordnung mit beliebigen zirkulanten 3x3-Bloecken (= Z3 x Q8), n = 12: Zahl mit disc >= 4 und >= 6 (A[0] = +1)."""
import itertools, numpy as np
def disc(M):
    n=M.shape[1]; best=10**9
    X=np.array(list(itertools.product((1,-1),repeat=n-1)),dtype=np.int16); X=np.hstack([np.ones((len(X),1),dtype=np.int16),X])
    return int(np.abs(X@M.T).max(axis=1).min())
def nega(f):
    n=len(f); return np.array([[ (f[j-i] if j>=i else -f[n+j-i]) for j in range(n)] for i in range(n)],dtype=np.int16)
for n,ds in ((8,(4,6)),(10,(4,6))):
    cnt={d:0 for d in ds}
    for rest in itertools.product((1,-1),repeat=n-1):
        dd=disc(nega((1,)+rest))
        for d in ds: cnt[d]+= dd>=d
    print("negazyklisch n =",n,{f"disc>={d}":cnt[d] for d in ds},"von",2**(n-1))
def circ(v): return np.array([[v[(j-i)%len(v)] for j in range(len(v))] for i in range(len(v))],dtype=np.int16)
cnt={4:0,6:0}
for bits in itertools.product((1,-1),repeat=11):
    v=(1,)+bits; A,B,C,D=(circ(v[0:3]),circ(v[3:6]),circ(v[6:9]),circ(v[9:12]))
    M=np.block([[A,B,C,D],[-B,A,-D,C],[-C,D,A,-B],[-D,-C,B,A]]); dd=disc(M)
    for d in cnt: cnt[d]+= dd>=d
print("Williamson-Anordnung n = 12 (Z3 x Q8):",{f"disc>={d}":cnt[d] for d in cnt},"von 2048")
