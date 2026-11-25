#!/usr/bin/env python3
"""Vortest (Orchestrator, 25.11.2026): Halbproblem 'finde W zu festem U' als reines SAT (CaDiCaL ueber python-sat). SUCHCODE.
Variable x[i][c]: Zeile i von W ist das Wort c (c aus allen Woertern der Laenge m2 mit erstem Eintrag +1).
Jede Loesung wird mit zerlegung.offen() nachgezaehlt. Aufruf: python3 zerlegung_sat.py MODUS [Sekunden]"""
import sys, time, threading, itertools, numpy as np
from pysat.solvers import Solver
src=open(__file__.replace('zerlegung_sat.py','zerlegung.py')).read().split('if __name__')[0]
src=src.replace('from ortools.sat.python import cp_model','')
exec(src)
def finde_W_sat(U,m2,d,sek,erste_fest=True,name='cadical153'):
    K=U.shape[0]; req,nall=anforderungen(U,d); Z=punkte(m2); C=punkte(m2); A=np.abs(Z@C.T); nC=len(C)
    var=lambda i,c: 1+i*nC+c; nv=K*nC; cl=[]
    for i in range(K):
        cl.append([var(i,c) for c in range(nC)])
        # hoechstens eines: Leiterkodierung
        aux=[nv+1+t for t in range(nC-1)]; nv+=nC-1
        for c in range(nC-1):
            cl.append([-var(i,c),aux[c]]);
            if c>0: cl.append([-aux[c-1],aux[c]])
            cl.append([-aux[c],-var(i,c+1)])
    if erste_fest: cl.append([var(0,0)])
    thr=sorted({t for r in req for t in r}); g={}
    for zi in range(len(Z)):
        for t in thr:
            good=[c for c in range(nC) if A[zi][c]>=t]
            for i in range(K):
                nv+=1; g[(zi,i,t)]=nv; cl.append([-nv]+[var(i,c) for c in good])
        for r in req: cl.append([g[(zi,i,r[i])] for i in range(K)])
    s=Solver(name=name,bootstrap_with=cl); t0=time.time()
    timer=threading.Timer(sek,lambda: s.interrupt()); timer.start()
    try: res=s.solve_limited(expect_interrupt=True)
    except Exception as e: res=s.solve()
    timer.cancel(); dt=time.time()-t0
    if res is None: return "ZEITGRENZE",None,dt,len(req),nall,len(cl)
    if res is False: return "UNMOEGLICH",None,dt,len(req),nall,len(cl)
    mdl=set(l for l in s.get_model() if l>0)
    Wn=np.array([C[[c for c in range(nC) if var(i,c) in mdl][0]] for i in range(K)],dtype=np.int64)
    return "LOESUNG",Wn,dt,len(req),nall,len(cl)
if __name__=="__main__":
    modus=sys.argv[1]; sek=float(sys.argv[2]) if len(sys.argv)>2 else 120
    H=paley1(11); U12=H[:,1:]; W12=H[:,3:]
    faelle={"kontrolle":[[]],"k11":[[11],[0]],"k10":[[10,11],[0,1],[3,7]]}[modus]
    for weg in faelle:
        U=np.delete(U12,weg,axis=0) if weg else U12
        st,Wn,dt,nr,na,ncl=finde_W_sat(U,9,8,sek,name='glucose4')
        print(f"K={U.shape[0]}, U = (H ohne Spalte 0) ohne Zeilen {weg}, Typ (11,9), d=8: {st} in {dt:.1f} s; Anforderungsvektoren {nr} maximal von {na}; Klauseln {ncl}",flush=True)
        if Wn is not None: print("  nachgezaehlt offene Klassen:",offen(U,Wn,8)); print(Wn)
