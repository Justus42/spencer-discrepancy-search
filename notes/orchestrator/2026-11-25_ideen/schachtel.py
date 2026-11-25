#!/usr/bin/env python3
"""Vortest (Orchestrator, 25.11.2026): 'verschachtelte optimale Codes' in der Kern-<1,a>-Familie, Typ (11,9), d = 8.
U = 8 Hamming-Zeilen (ctrl_8, 7 Spalten) mit verdreifachten Spalten -> 8 x 11 (jedes y hat eine Zeile mit |u| >= 5),
W = 8 Hamming-Zeilen mit einer verdreifachten Spalte -> 8 x 9 (jedes z hat eine Zeile mit |v| >= 5).
Woerter: (U_i, +-W_pi(i)) und Komplemente = 32 Woerter. Alle 8! Zuordnungen pi; gezaehlt werden offene (y,z)-Klassen
(max_i(|u_i|+|v_pi(i)|) < 8), von 1024*256 = 262144. Danach gierig: wie viel bleibt nach 1 und 2 weiteren Nebenklassen offen
(2 weitere = 40 Woerter = Rekordgrenze). Nur ganze Zahlen."""
import sys, itertools, numpy as np
def punkte(m):
    X=np.array(list(itertools.product((1,-1),repeat=m-1)),dtype=np.int64); return np.hstack([np.ones((len(X),1),dtype=np.int64),X])
Ham=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()])[:,:7]
P11,P9=punkte(11),punkte(9)
def test(U,W,name):
    au=np.abs(P11@U.T); av=np.abs(P9@W.T)
    assert (au.max(axis=1)>=5).all() and (av.max(axis=1)>=5).all()
    B=[[np.packbits((au[:,i][:,None]+av[:,j][None,:]>=8).ravel()) for j in range(8)] for i in range(8)]
    tab=np.array([bin(k).count("1") for k in range(256)],dtype=np.int64)
    best=None
    for pi in itertools.permutations(range(8)):
        o=B[0][pi[0]].copy()
        for i in range(1,8): o|=B[i][pi[i]]
        op=262144-int(tab[o].sum())
        if best is None or op<best[0]: best=(op,pi)
    op,pi=best; print(f"{name}: kleinste Zahl offener Klassen ueber alle 8! Zuordnungen: {op} von 262144 (Zuordnung {pi})")
    cov=np.zeros((1024,256),dtype=bool)
    for i in range(8): cov|= (au[:,i][:,None]+av[:,pi[i]][None,:]>=8)
    O=(~cov).astype(np.float32)
    A11=np.abs(P11@P11.T); A9=np.abs(P9@P9.T)       # A11[a][y] = |<a,y>|
    for runde in (1,2):
        cnt=np.zeros((1024,256),dtype=np.float32)
        for t in (1,3,5,7,9,11):
            Ya=(A11==t).astype(np.float32); Zb=(A9>=8-t).astype(np.float32)
            cnt+= Ya@O@Zb.T
        a,b=np.unravel_index(int(cnt.argmax()),cnt.shape)
        O[(A11[a][:,None]+A9[b][None,:])>=8]=0
        print(f"   nach {runde} weiteren Nebenklasse(n), gierig gewaehlt: offen {int(O.sum())}")
t3=lambda M,cols: np.hstack([M]+[M[:,[c]] for c in cols for _ in range(2)])
test(t3(Ham,[0,1]),t3(Ham,[0]),"U: Spalten 0,1 verdreifacht; W: Spalte 0 verdreifacht")
test(np.hstack([Ham]+[Ham[:,[0]]]*4),t3(Ham,[0]),"U: Spalte 0 verfuenffacht; W: Spalte 0 verdreifacht")
