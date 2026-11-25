#!/usr/bin/env python3
"""Vortest (Orchestrator, 25.11.2026): kleines Zertifikat fuer 'drei Zeilen des 32ers lassen sich nicht durch zwei Zeilen ersetzen'.
Gesucht: eine kleine Menge S von Lochklassen, die sich schon allein nicht auf zwei Zeilen verteilen und orientieren laesst
(je Zeile alle Paare orientiert im Abstand <= 8). Kern per SAT (Annahmen je Loch) und Streichen ueberfluessiger Loecher;
danach PRUEFUNG OHNE SAT: alle 2^|S| Zeilenverteilungen, je Zeile Zweifaerbung der Vorzeichen (Abstand <= 6 gleich, >= 9 verschieden)."""
import sys, itertools, numpy as np
from pysat.solvers import Solver
A=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()])[:,:15]
X=np.array(list(itertools.product((1,-1),repeat=14)),dtype=np.int64); X=np.hstack([np.ones((len(X),1),dtype=np.int64),X])
cov=(np.abs(X@A.T)>=7); k=2
def kern(T):
    H=X[~np.delete(cov,T,axis=1).any(axis=1)]; n=len(H); D=(15-H@H.T)//2
    R=lambda h,t: 1+h*k+t; O=lambda h,t: 1+n*k+h*k+t; S=lambda h: 1+2*n*k+h
    cl=[[-S(h)]+[R(h,t) for t in range(k)] for h in range(n)]
    iu=np.triu_indices(n,1); d=D[iu]; same=np.where(d<=6)[0]; opp=np.where(d>=9)[0]
    for t in range(k):
        i=iu[0][same]; j=iu[1][same]; ri=-(1+i*k+t); rj=-(1+j*k+t); oi=1+n*k+i*k+t; oj=1+n*k+j*k+t
        cl+=np.column_stack([ri,rj,-oi,oj]).tolist(); cl+=np.column_stack([ri,rj,oi,-oj]).tolist()
        i=iu[0][opp]; j=iu[1][opp]; ri=-(1+i*k+t); rj=-(1+j*k+t); oi=1+n*k+i*k+t; oj=1+n*k+j*k+t
        cl+=np.column_stack([ri,rj,oi,oj]).tolist(); cl+=np.column_stack([ri,rj,-oi,-oj]).tolist()
    s=Solver(name='glucose4',bootstrap_with=cl)
    assert not s.solve(assumptions=[S(h) for h in range(n)])
    core=[a-1-2*n*k for a in s.get_core()]
    ch=True
    while ch:
        ch=False
        for h in list(core):
            rest=[g for g in core if g!=h]
            if not s.solve(assumptions=[S(g) for g in rest]): core=rest; ch=True
    return H[core],D[np.ix_(core,core)]
def pruefe(Dk):     # ohne SAT: gibt es eine Verteilung auf zwei Zeilen mit widerspruchsfreien Vorzeichen?
    m=len(Dk)
    def zweifarb(idx):
        col={}
        for st in idx:
            if st in col: continue
            col[st]=0; stack=[st]
            while stack:
                u=stack.pop()
                for v in idx:
                    if v==u: continue
                    if Dk[u][v]<=6: need=col[u]
                    elif Dk[u][v]>=9: need=1-col[u]
                    else: continue
                    if v not in col: col[v]=need; stack.append(v)
                    elif col[v]!=need: return False
        return True
    for mask in range(1<<(m-1)):
        a=[i for i in range(m) if (mask>>i)&1==0]; b=[i for i in range(m) if (mask>>i)&1]
        if zweifarb(a) and zweifarb(b): return True
    return False
for T in [(0,1,2),(0,1,5),(3,8,13),(2,9,14)]:
    Hk,Dk=kern(T); print(f"gestrichen {T}: Kern aus {len(Hk)} Lochklassen; Pruefung ohne SAT: verteilbar = {pruefe(Dk)} (Soll False)")
    print("   Abstandsmatrix des Kerns:",Dk.tolist())
