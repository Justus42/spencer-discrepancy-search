#!/usr/bin/env python3
"""ersatz_k2_gegen.py -- unabhaengige Nachrechnung zu ersatz_k2.c (Gegenpruefung 26.11.2026, Orchestrator; numpy, anderer Aufbau).
Fuer einige Spaltenpaare einer Hadamard-Matrix der Ordnung 20: kritische Mengen aus allen x'' sammeln, je Zeilengruppe alle 4^9 Belegungen pruefen.
Mit MINDESTGROESSE > 0 werden nur Bedingungen mit |S| >= MINDESTGROESSE benutzt (dann gibt es Ueberlebende: Kontrolle der Aufzaehlung).
Aufruf: ersatz_k2_gegen.py had20.txt MATRIX MINDESTGROESSE a,b [a,b ...]"""
import sys, numpy as np
b=open(sys.argv[1]).read().strip().split('\n\n'); H=np.array([[int(v) for v in l.split()] for l in b[int(sys.argv[2])].split('\n')]); N=len(H); mg=int(sys.argv[3]); Z=8
for paar in sys.argv[4:]:
    a,c=map(int,paar.split(',')); A=np.delete(H,[a,c],axis=1)
    idx=np.arange(1<<(N-3),dtype=np.uint32); X=1-2*((idx[:,None]>>np.arange(N-3,dtype=np.uint32))&1).astype(np.int16); X=np.concatenate([np.ones((len(idx),1),np.int16),X],axis=1)
    Bm=X@A.T.astype(np.int16); flach=np.abs(Bm).max(1)<=Z-2; Bf=Bm[flach]
    bed=set()
    for r in Bf:
        S=tuple(int(i) for i in np.nonzero(np.abs(r)==Z-2)[0]); sg=tuple(int(np.sign(r[i])) for i in S)
        if sg[0]<0: sg=tuple(-v for v in sg)
        if len(S)>=mg: bed.add((S,sg))
    g=H[:,a]*H[:,c]; erg=[]
    for grp in (1,-1):
        rows=[i for i in range(N) if g[i]==grp]; pos={r:k for k,r in enumerate(rows)}
        C=[(S,sg) for (S,sg) in bed if all(i in pos for i in S)]
        t=np.arange(4**(len(rows)-1),dtype=np.int64); val=np.zeros((len(t),len(rows)),np.int8)
        for k in range(1,len(rows)): val[:,k]=(t>>(2*(k-1)))&3
        ok=np.ones(len(t),bool)
        for (S,sg) in C:
            o=np.zeros(len(t),np.int8)
            for i,s in zip(S,sg): o|=(1<<(val[:,pos[i]]^(3 if s<0 else 0))).astype(np.int8)
            ok&=(o==15)
        erg.append((len(C),int(ok.sum())))
    gem=sum(1 for (S,sg) in bed if len({int(g[i]) for i in S})>1)
    print("mat %s Spalten %d,%d: flache x'' %d, Bedingungen %d/%d, Belegungen %d/%d, ueber beide Gruppen %d"%(sys.argv[2],a,c,int(flach.sum()),erg[0][0],erg[1][0],erg[0][1],erg[1][1],gem))
