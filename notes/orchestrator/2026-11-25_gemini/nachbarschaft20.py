#!/usr/bin/env python3
"""Vortest (Orchestrator, 25.11.2026): 48er ohne eine Zeile (23 x 20, 1297 offene Klassen bei (20,6)).
Vollstaendig: alle Aenderungen von genau 1 oder genau 2 Eintraegen EINER Zeile (Vorzeichenwechsel). Kleinste Restzahl. Nur ganze Zahlen."""
import sys, itertools, numpy as np
M=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()]); drop=int(sys.argv[2]); A=np.delete(M,drop,axis=0)
N,m=A.shape; X=np.array(list(itertools.product((1,-1),repeat=m-1)),dtype=np.int8); X=np.hstack([np.ones((len(X),1),dtype=np.int8),X])
val=(X.astype(np.int16)@A.T.astype(np.int16)).astype(np.int8); cov=np.abs(val)>=8; cnt=cov.sum(axis=1).astype(np.int8)
base=int((cnt==0).sum()); print("Start: Zeilen",N,"offene Klassen",base)
best1=best2=base; wo1=wo2=None
for i in range(N):
    kand=np.where(cnt-cov[:,i]==0)[0]            # Klassen, die ohne Zeile i offen waeren
    Xk=X[kand].astype(np.int16); v=val[kand,i].astype(np.int16)
    for j in range(m):
        v1=v-2*A[i,j]*Xk[:,j]; r=int((np.abs(v1)<8).sum())
        if r<best1: best1=r; wo1=(i,j)
        for j2 in range(j+1,m):
            r=int((np.abs(v1-2*A[i,j2]*Xk[:,j2])<8).sum())
            if r<best2: best2=r; wo2=(i,j,j2)
print("ein Eintrag einer Zeile gewechselt: kleinste Restzahl",best1,wo1); print("zwei Eintraege einer Zeile gewechselt: kleinste Restzahl",best2,wo2)
