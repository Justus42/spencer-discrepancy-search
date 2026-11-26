#!/usr/bin/env python3
"""loch_buendel.py -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
Sind die Loecher von Hadamard 24 ohne 1 Spalte bei d = 9 gebuendelt? Fuer eine Matrix/Spalte: Loecher (max|s| <= 7) aufzaehlen,
dann gierig mit Kugeln vom Radius 7 (mit Gegenkugel) ueberdecken. Kandidaten fuer Mittelpunkte: alle Loecher und ihre Nachbarn im Abstand 1 und 2 (Stichprobe).
Ausgabe: Zahl der Loecher (bis auf Vorzeichen), groesste Einzelkugel, gierige Kugelzahl. Kein Beweis einer unteren Schranke.
Aufruf: loch_buendel.py had24.txt MATRIXINDEX SPALTE"""
import sys, numpy as np
b=open(sys.argv[1]).read().strip().split('\n\n'); H=np.array([[int(v) for v in l.split()] for l in b[int(sys.argv[2])].split('\n')]); sp=int(sys.argv[3])
A=np.delete(H,sp,axis=1).astype(np.int8); m=23
# alle x mit x_0=+1: 2^22
idx=np.arange(1<<22,dtype=np.uint32)
L=[]
for teil in np.array_split(idx,64):
    X=1-2*((teil[:,None]>>np.arange(22,dtype=np.uint32))&1).astype(np.int8); X=np.concatenate([np.ones((len(teil),1),np.int8),X],axis=1)
    S=np.abs(X.astype(np.int16)@A.T.astype(np.int16)); L.append(X[S.max(1)<=7])
L=np.concatenate(L); n=len(L); print("Loecher bis auf Vorzeichen:",n)
rng=np.random.default_rng(1)
kand=[L]
for t in (1,2):
    K=np.repeat(L,6,axis=0).copy()
    for _ in range(t):
        j=rng.integers(0,m,len(K)); K[np.arange(len(K)),j]*=-1
    kand.append(K)
K=np.concatenate(kand); print("Kandidaten:",len(K))
offen=np.ones(n,bool); kugeln=0; erste=None
Lf=L.astype(np.int16)
while offen.any():
    best=-1; bi=-1
    for a in range(0,len(K),20000):
        P=np.abs(K[a:a+20000].astype(np.int16)@Lf[offen].T)>=9
        c=P.sum(1); i=int(c.argmax())
        if c[i]>best: best=int(c[i]); bi=a+i
    if erste is None: erste=best
    cov=np.abs(Lf@K[bi].astype(np.int16))>=9; offen&=~cov; kugeln+=1
    if kugeln<=12 or not offen.any(): print("  Kugel %d deckt %d neue, offen %d"%(kugeln,best,int(offen.sum())))
print("groesste Einzelkugel: %d von %d (%.1f %%); gierig: %d Kugeln"%(erste,n,100.0*erste/n,kugeln))
