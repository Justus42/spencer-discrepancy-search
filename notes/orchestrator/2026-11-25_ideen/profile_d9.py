#!/usr/bin/env python3
"""Vortest (Orchestrator, 25.11.2026): Kern-<1,a>-Familie bei d = 9 mit K = 12 Nebenklassen (48 Woerter <= 50, Zellen (21,6), (23,7), (25,8)).
Fuer U = Paley-12 ohne k Spalten (k = 0..3): Profile |U y| (sortiert) mit Haeufigkeit; kritische Profile bei d = 9 (keine Zeile mit d - |u| <= kleinster Wert von |v|);
Zahl verschiedener Zeilenmuster. Dazu fuer U = H (12 x 12) die Volumenrechnung fuer W (12 x m2). Nur ganze Zahlen."""
import itertools, collections, numpy as np
from math import comb
def paley1(q):
    sq={(x*x)%q for x in range(1,q)}; chi=lambda a: 0 if a%q==0 else (1 if a%q in sq else -1)
    n=q+1; S=np.zeros((n,n),dtype=np.int64); S[0,1:]=1; S[1:,0]=-1
    for i in range(q):
        for j in range(q): S[i+1,j+1]=chi(i-j)
    return S+np.eye(n,dtype=np.int64)
def punkte(m):
    X=np.array(list(itertools.product((1,-1),repeat=m-1)),dtype=np.int64); return np.hstack([np.ones((len(X),1),dtype=np.int64),X])
V=lambda m,r: sum(comb(m,k) for k in range(r+1)) if r>=0 else 0
H=paley1(11)
for k in range(4):
    U=H[:,k:]; m1=U.shape[1]; au=np.abs(punkte(m1)@U.T); vmin=1-(m1%2)   # d=9 ungerade: m2 hat die andere Paritaet; kleinster Wert von |v| ist 1 (m2 ungerade) oder 0
    c=collections.Counter(tuple(sorted((int(t) for t in a),reverse=True)) for a in au)
    print(f"U = H ohne {k} Spalten ({12} x {m1}); {len(au)} Klassen y; kleinster moeglicher Wert von |v|: {vmin}")
    for p,n in sorted(c.items(),key=lambda t:-t[1]):
        krit=all(9-t>vmin for t in p); muster=len({tuple(int(t) for t in a) for a in au if tuple(sorted((int(t) for t in a),reverse=True))==p})
        print(f"   Profil {p}: {n} Klassen, {muster} Zeilenmuster, {'KRITISCH: Anforderung an |v| je Zeile '+str(tuple(9-t for t in p)) if krit else 'immer erfuellt'}")
print()
print("U = H (12 x 12), d = 9. Bedingung an W (12 x m2, m2 ungerade), falls alle 220 Dreiermengen als 6er-Zeilen und als Nullzeilen vorkommen (oben: Zeilenmuster je 220):")
print("  fuer jedes z:  (eine Zeile mit |v| >= 9)  oder  (mindestens 4 Zeilen mit |v| >= 5  und  (eine Zeile mit |v| >= 7 oder hoechstens 2 Zeilen mit |v| = 1)).")
for m2 in (9,11,13):
    r5=(m2-5)//2; r9=(m2-9)//2; tot=2**m2; b9=24*V(m2,r9); inz=24*V(m2,r5); need=b9+4*(tot-b9)
    print(f"  m2={m2} (Zelle ({12+m2},{(12+m2-9)//2})): Punkte {tot}; hoechstens {b9} Punkte mit |v|>=9; Inzidenzen |v|>=5: {inz}; noetig mindestens {need}; Spielraum {inz-need}")
