#!/usr/bin/env python3
"""Pruefung zweier 'Minuten-Tests' aus dem Gemini-Bericht vom 25.11.2026 (Orchestrator). Nur ganze Zahlen.
Teil 1 (Gemini Fund 2, n = 16): ctrl_16 mit Nullspalte hat disc 7; der Rekord braeuchte bei n = 16 mehr als 5*sqrt(2) = 7,0711.
  Kappengroesse einer Zeile (eine Vorzeichenseite): Schwelle 7 -> Durchmesser <= 9 -> hoechstens 2*V(15,4); Schwelle > 7,07 -> Durchmesser <= 8
  -> hoechstens V(16,4). Dazu: Haelt man die Zuordnung 'dominierende Zeile' von ctrl_16 fest, muessen alle Punkte einer Zeile paarweise
  Abstand <= 8 in Q16 haben. Zwei private Punkte u1, u2 einer Zeile (nur sie erreicht 7) liefern (u1,+) und (u2,-) mit Abstand d(u1,u2)+1.
  Gibt es private Paare im Abstand 8, ist das LP mit fester Zuordnung nicht ueber 7,07 zu heben.
Teil 2 (Gemini Fund 3b): Hadamard 20 (drei Klassen), 1 bis 3 Spalten durch zufaellige +-1-Spalten ersetzt, je 1000 Stichproben:
  gibt es ein x mit allen |<Zeile,x>| <= 6 (dann disc <= 6 < 8)?"""
import sys, itertools, numpy as np
from math import comb
V=lambda m,r: sum(comb(m,k) for k in range(r+1))
print("Teil 1: groesste Kappe (eine Seite) bei n = 16: Schwelle 7:",2*V(15,4),"Punkte; Schwelle > 7,0711:",V(16,4),"Punkte; 32 solche Kappen: Dichte",round(32*V(16,4)/2**16,3),"; Tabelle K(16,4) >= 34 > 32")
A=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()])[:,:15]
X=np.array(list(itertools.product((1,-1),repeat=15)),dtype=np.int64); val=X@A.T
cov=np.abs(val)>=7; allein=cov.sum(axis=1)==1
res=[]
for i in range(16):
    P=X[allein&(val[:,i]>=7)]          # private Punkte der Zeile i, auf die positive Seite orientiert
    D=(15-P@P.T)//2; res.append((len(P),int(D.max()),int((D==8).sum()//2)))
print("  je Zeile (private orientierte Punkte, groesster Abstand untereinander, Zahl der Paare im Abstand 8):",sorted(set(res)))
rng=np.random.default_rng(1); Y=rng.choice([-1,1],size=(20000,20)).astype(np.int64)
for kl in (1,2,3):
    H=np.array([list(map(int,l.split())) for l in open(f"{sys.argv[2]}/had20_klasse{kl}.txt") if l.strip()])[:20,:20]
    assert (H@H.T==20*np.eye(20,dtype=np.int64)).all()
    for k in (1,2,3):
        ohne=0; kleinst=20000
        for t in range(1000):
            M=H.copy(); cols=rng.choice(20,size=k,replace=False); M[:,cols]=rng.choice([-1,1],size=(20,k))
            flach=int((np.abs(Y@M.T).max(axis=1)<=6).sum()); kleinst=min(kleinst,flach); ohne+= flach==0
        print(f"Teil 2: Hadamard 20 Klasse {kl}, {k} Spalte(n) ersetzt, 1000 Stichproben: in {ohne} Faellen kein x mit max <= 6 unter 20000 Zufallspunkten; kleinste Trefferzahl {kleinst}")
