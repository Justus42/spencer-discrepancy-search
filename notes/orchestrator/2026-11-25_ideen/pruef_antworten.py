#!/usr/bin/env python3
"""Pruefung der Antworten vom 25.11.2026 (Astra Teil A, Sol Teil B), Orchestrator. Nur ganze Zahlen.
Teil 1: Zahlen aus der Sol-Antwort (Kugelschnitte, Ungleichung a+3b+6c >= 10, Volumentabelle, Inzidenzzahlen).
Teil 2: Zahlen aus der Astra-Antwort (140 gemeinsame Klassen zweier Paarkugeln, Lochzahl nach Streichen dreier Paare, E_offen)."""
import itertools, sys, numpy as np
from math import comb
V=lambda m,r: sum(comb(m,k) for k in range(r+1)) if r>=0 else 0
def schnitt(m,d,r1,r2):      # |B(c,r1) ∩ B(c',r2)| bei Abstand d
    return sum(comb(d,j)*comb(m-d,k) for j in range(d+1) for k in range(m-d+1) if j+k<=r1 and d-j+k<=r2)
print("Sol F1: Schnitt Radius-2-Kugel mit Radius-4-Kugel in Q13, Abstand 0..6:",[schnitt(13,d,2,4) for d in range(7)],"(Sol: 92,92,92,47,47,15,15)")
def gefaltet(m,d,r1,r2):     # Klassen modulo Vorzeichen: Paarkugel um c (Radius r1) geschnitten mit Paarkugel um c' (r2), d(c,c') = d
    return (schnitt(m,d,r1,r2)+schnitt(m,m-d,r1,r2))
print("Sol F3: Paarkugeln Radius 2 in Q9, gefalteter Abstand 0..4, gemeinsame Klassen:",[gefaltet(9,d,2,2) if d>0 else V(9,2) for d in range(5)],"(Sol: 46,18,18,6,6)")
print("Sol F3: Tabelle Typ (11,9): gemeinsames Volumen =",sum((V(9,(9-s)//2)-(V(9,(9-s-2)//2) if s+2<=9 else 0))*(V(11,(11-(8-s))//2) if 8-s>0 else 1024) for s in (1,3,5,7,9)),"(Sol: 58 402)")
print("Sol F3: Inzidenzen links je Betrag 1..11 ueber 10 Zeilen:",[10*comb(11,(11-t)//2) for t in (1,3,5,7,9,11)],"; rechts:",[10*comb(9,(9-t)//2) for t in (1,3,5,7,9)])
print("Sol F2: m2=10: 12*(q4+3q6+6q8) =",12*(V(10,3)+3*V(10,2)+6*V(10,1)),"gegen 10*M =",10*2**9,"; m2=12:",12*(V(12,4)+3*V(12,3)+6*V(12,2)),"gegen",10*2**11,"; m2=14:",12*(V(14,5)+3*V(14,4)+6*V(14,3)),"gegen",10*2**13)
# Ungleichung a+3b+6c >= 10 fuer U = Paley-12 ohne Spalte 0, Profil (5^3,3^6,1^3), d = 9: Muster (F,M,O)
def paley1(q):
    sq={(x*x)%q for x in range(1,q)}; chi=lambda a: 0 if a%q==0 else (1 if a%q in sq else -1)
    n=q+1; S=np.zeros((n,n),dtype=np.int64); S[0,1:]=1; S[1:,0]=-1
    for i in range(q):
        for j in range(q): S[i+1,j+1]=chi(i-j)
    return S+np.eye(n,dtype=np.int64)
def punkte(m):
    X=np.array(list(itertools.product((1,-1),repeat=m-1)),dtype=np.int64); return np.hstack([np.ones((len(X),1),dtype=np.int64),X])
U=paley1(11)[:,1:]; au=np.abs(punkte(11)@U.T)
pat=[]
for a in au:
    if sorted(a.tolist())==[1,1,1,3,3,3,3,3,3,5,5,5]:
        F=sum(1<<i for i in range(12) if a[i]==5); M=sum(1<<i for i in range(12) if a[i]==3); O=sum(1<<i for i in range(12) if a[i]==1); pat.append((F,M,O))
assert len(pat)==220
def ok(A,B,C): return all((F&A) or (M&B) or (O&C) for F,M,O in pat)
pc=lambda x: bin(x).count("1")
best={}
for A in range(4096):
    a=pc(A)
    if a>=10 and ok(A,0,0): best[0]=min(best.get(0,99),a)
    for i in range(12):
        if A>>i&1 and ok(A,1<<i,0): best[1]=min(best.get(1,99),a)
        for j in range(i+1,12):
            if A>>i&1 and A>>j&1 and ok(A,(1<<i)|(1<<j),0): best[2]=min(best.get(2,99),a)
    if a<10 and ok(A,0,0): best[0]=min(best.get(0,99),a)
print("Sol F2: kleinstes a = |A| bei c = 0 und b = 0, 1, 2 (alle Lagen von B):",best,"(Sol: 10, 7, 6); also a+3b:",{b:best[b]+3*b for b in best})
# kleinstes a + 3b + 6c ueber ALLE Ketten C in B in A (vollstaendig, mit Abbruch): nur Ketten mit a+3b+6c <= 9 suchen
gegen=0
for A in range(4096):
    a=pc(A)
    if a>9: continue
    subs=[B for B in range(4096) if B&~A==0 and a+3*pc(B)<=9]
    for B in subs:
        b=pc(B)
        for C in [C for C in range(4096) if C&~B==0 and a+3*b+6*pc(C)<=9]:
            if ok(A,B,C): gegen+=1
print("Sol F2: Ketten C in B in A mit a+3b+6c <= 9, die alle 220 Muster erfuellen:",gegen,"(Soll 0)")
# Astra
A16=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()])[:,:15]
print("Astra: gemeinsame Klassen zweier Paarkugeln (Radius 4, Q15) bei Abstand 7/8:",gefaltet(15,7,4,4),"(Astra: 140); Zeilenabstaende in ctrl_16:",sorted(set(((15-A16@A16.T)//2)[np.triu_indices(16,1)].tolist())))
X=punkte(15); cov=(np.abs(X@A16.T)>=7)
priv=[int((cov[:,i]&(cov.sum(axis=1)==1)).sum()) for i in range(16)]; print("Astra: private Klassen je Zeile:",sorted(set(priv)))
gr=[]
for T in itertools.combinations(range(16),3):
    rest=np.delete(cov,T,axis=1).any(axis=1); gr.append(int((~rest).sum()))
print("Astra: Lochklassen nach Streichen dreier Zeilen, ueber alle 560 Tripel: min",min(gr),"max",max(gr),"Werte",sorted(set(gr)),"(Astra: 699 bis 1119)")
g2=[int((~np.delete(cov,T,axis=1).any(axis=1)).sum()) for T in itertools.combinations(range(16),2)]
print("Astra: nach Streichen zweier Zeilen: Werte",sorted(set(g2)),"(Astra: 466 bis 606)")
for (m,R,k) in ((15,4,15),(20,6,20),(25,8,25),(30,10,31),(37,13,38)):
    Q=2**(m-1); print(f"Astra: E_offen ({m},{R}), k={k}: {Q*(1-V(m,R)/Q)**k:.3g}",end="; ")
print()
