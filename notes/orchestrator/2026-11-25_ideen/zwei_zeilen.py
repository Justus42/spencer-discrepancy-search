#!/usr/bin/env python3
"""Vortest zu Astras Vorschlag A (Orchestrator, 25.11.2026): (15,4), r Zeilen des 32ers (ctrl_16) streichen, k freie Zeilen einsetzen.
Notwendige Bedingung fuer JEDE Ersatzzeile mit Eintraegen in [-1,1] (binaer, mit Nullen oder reell) bei Schwelle > 5*sqrt(15/8) = 6,847:
zwei orientierte Punkte derselben Zeilenkappe haben Hamming-Abstand <= 8. Also muessen sich die Lochklassen auf k Zeilen verteilen und
orientieren lassen, so dass je Zeile alle Paare Abstand <= 8 haben. Das ist ein SAT-Problem (Variablen: Zeile und Vorzeichen je Loch).
UNERFUELLBAR  =>  mit diesen 16-r festen Zeilen gibt es keine k Ersatzzeilen, auch keine reellen.  ERFUELLBAR sagt noch nichts.
Aufruf: python3 zwei_zeilen.py ctrl_16.txt r k [Anzahl Faelle je Lochzahl] [Sekunden je Fall] [Teiler Rest]  (Teiler/Rest: nur jede Teiler-te Zeilenmenge, fuer zwei Prozesse)"""
import sys, itertools, time, threading, collections, numpy as np
from pysat.solvers import Solver
A=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()])[:,:15]
r=int(sys.argv[2]); k=int(sys.argv[3]); jeTyp=int(sys.argv[4]) if len(sys.argv)>4 else 2; sek=float(sys.argv[5]) if len(sys.argv)>5 else 120
X=np.array(list(itertools.product((1,-1),repeat=14)),dtype=np.int64); X=np.hstack([np.ones((len(X),1),dtype=np.int64),X])
cov=(np.abs(X@A.T)>=7)
def fall(T):
    H=X[~np.delete(cov,T,axis=1).any(axis=1)]; n=len(H); D=(15-H@H.T)//2
    R=lambda h,t: 1+h*k+t; O=lambda h,t: 1+n*k+h*k+t
    cl=[[R(h,t) for t in range(k)] for h in range(n)]
    iu=np.triu_indices(n,1); d=D[iu]; same=np.where(d<=6)[0]; opp=np.where(d>=9)[0]
    for t in range(k):
        i=iu[0][same]; j=iu[1][same]; ri=-(1+i*k+t); rj=-(1+j*k+t); oi=1+n*k+i*k+t; oj=1+n*k+j*k+t
        cl+=np.column_stack([ri,rj,-oi,oj]).tolist(); cl+=np.column_stack([ri,rj,oi,-oj]).tolist()
        i=iu[0][opp]; j=iu[1][opp]; ri=-(1+i*k+t); rj=-(1+j*k+t); oi=1+n*k+i*k+t; oj=1+n*k+j*k+t
        cl+=np.column_stack([ri,rj,oi,oj]).tolist(); cl+=np.column_stack([ri,rj,-oi,-oj]).tolist()
    cl.append([R(0,0)]); cl.append([-O(0,0)])
    s=Solver(name='cadical153',bootstrap_with=cl); t0=time.time(); res=[None]
    th=threading.Thread(target=lambda: res.__setitem__(0,s.solve())); th.daemon=True; th.start(); th.join(sek)
    if th.is_alive(): return n,"ZEITGRENZE",time.time()-t0,len(cl)
    return n,("ERFUELLBAR" if res[0] else "UNERFUELLBAR"),time.time()-t0,len(cl)
gruppen=collections.defaultdict(list)
for T in itertools.combinations(range(16),r):
    gruppen[int((~np.delete(cov,T,axis=1).any(axis=1)).sum())].append(T)
print(f"r={r} gestrichen, k={k} freie Zeilen; Lochzahlen und Anzahl der Zeilenmengen:",{g:len(v) for g,v in sorted(gruppen.items())},flush=True)
mod=int(sys.argv[6]) if len(sys.argv)>6 else 1; rem=int(sys.argv[7]) if len(sys.argv)>7 else 0; zaehl=collections.Counter()
for g,v in sorted(gruppen.items()):
    for idx,T in enumerate(v[:jeTyp]):
        if idx%mod!=rem: continue
        n,st,dt,ncl=fall(T); zaehl[(n,st)]+=1; print(f"  gestrichen {T}: {n} Lochklassen, {ncl} Klauseln: {st} ({dt:.1f} s)",flush=True)
print("ZUSAMMENFASSUNG (Lochzahl, Ergebnis): Anzahl",dict(zaehl))
