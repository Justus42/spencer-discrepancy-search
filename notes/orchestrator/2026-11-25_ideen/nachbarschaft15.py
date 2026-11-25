#!/usr/bin/env python3
"""Vortest (Orchestrator, 25.11.2026): ctrl_16 ohne eine Zeile (15 x 15, 233 offene Klassen = 466 Punkte).
Vollstaendig: jede Aenderung von hoechstens 3 Eintraegen EINER Zeile auf Werte in {-1,0,1}.
Schwelle je Zeile: Traeger ungerade -> 7, gerade -> 8 (N = 15). Ausgabe: kleinste Restzahl je Aenderungsgroesse."""
import sys, itertools, numpy as np
rows=[list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()]
A=np.array(rows)[:,:15]; drop=int(sys.argv[2]); A=np.delete(A,drop,axis=0); N,m=A.shape
P=1<<(m-1); X=np.array([[(-1 if (x>>j)&1 else 1) for j in range(m)] for x in range(P)],dtype=np.int16)
def thr(s): return 7 if s%2 else 8
val=X@A.T.astype(np.int16)                      # P x N
cov=np.abs(val)>=np.array([thr(int(np.count_nonzero(A[i]))) for i in range(N)])
cnt=cov.sum(axis=1); base=int((cnt==0).sum()); print("Start: Zeilen",N,"offene Klassen",base, "Punkte",2*base)
best={1:(base,None),2:(base,None),3:(base,None)}
for i in range(N):
    rest=cnt-cov[:,i]
    for k in (1,2,3):
        for cols in itertools.combinations(range(m),k):
            for newv in itertools.product((-1,0,1),repeat=k):
                if any(newv[t]==A[i][cols[t]] for t in range(k)): continue
                v=val[:,i].copy(); s=int(np.count_nonzero(A[i]))
                for t in range(k):
                    v+= (newv[t]-A[i][cols[t]])*X[:,cols[t]]; s+= int(newv[t]!=0)-int(A[i][cols[t]]!=0)
                r=int(((rest==0)&(np.abs(v)<thr(s))).sum())
                if r<best[k][0]: best[k]=(r,(i,cols,newv))
for k in (1,2,3): print("Aenderung von genau",k,"Eintraegen einer Zeile: kleinste Restzahl",best[k][0],"(Klassen)",best[k][1])
