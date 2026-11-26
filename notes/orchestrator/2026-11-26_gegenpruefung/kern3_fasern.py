#!/usr/bin/env python3
"""kern3_fasern.py -- Gegenpruefung 26.11.2026 (Orchestrator), Vorschlag von Sol nachgerechnet; ein Programm, nicht gegengerechnet.
Kern D = <1,a,b> (8 Woerter), Laenge m, Radius R = (m-d)/2. Die Koordinaten zerfallen nach (a_j,b_j) in 4 Bloecke der Groessen n0..n3.
Ein Punkt z (relativ zum Vertreter einer Nebenklasse) hat Blockgewichte w0..w3; er ist gedeckt, wenn eines der 8 Woerter von D Abstand <= R hat.
M[t] = Zahl der gedeckten Punkte mit Paritaetsvektor t (Paritaet der Blockgewichte). Ein Punkt der Faser p wird von der Nebenklasse mit
Vertreterparitaet q nur ueber M[p+q] gedeckt. NOTWENDIG fuer K Nebenklassen: es gibt q_1..q_K mit  Summe_j M[p+q_j] >= |Faser p|  fuer jede Faser p.
Ist das fuer keine Wahl erfuellbar, ist der Typ mit K Nebenklassen ausgeschlossen (voll). Erfuellbar heisst noch nichts.
Kontrolle: Summe_t M[t] / 8 = B(D) muss fuer (20,6) zwischen 31180 und 53372 liegen (STATUS.md, 97 Typen).
Aufruf: kern3_fasern.py"""
from math import comb
from itertools import combinations_with_replacement, product
def typen(m):
    T=set()
    for n0 in range(0,m+1):
        for n1 in range(0,m+1-n0):
            for n2 in range(0,m+1-n0-n1):
                n=(n0,n1,n2,m-n0-n1-n2)
                if sum(1 for v in n if v>0)<3: continue      # a, b, 1 unabhaengig: mindestens 3 Bloecke
                T.add(tuple(sorted(n,reverse=True)))          # die 4 Blocktypen sind unter GL(2,2) und Verschiebung um 1 vertauschbar
    return sorted(T)
def M_tabelle(n,R):
    m=sum(n); M=[0]*16
    for w in product(*[range(v+1) for v in n]):
        w0,w1,w2,w3=w
        ds=[w0+w1+w2+w3, w0+w1+(n[2]-w2)+(n[3]-w3), w0+(n[1]-w1)+w2+(n[3]-w3), w0+(n[1]-w1)+(n[2]-w2)+w3]
        if min(ds)<=R or max(ds)>=m-R:
            t=(w0&1)|((w1&1)<<1)|((w2&1)<<2)|((w3&1)<<3)
            M[t]+=comb(n[0],w0)*comb(n[1],w1)*comb(n[2],w2)*comb(n[3],w3)
    return M
def fasern(n):
    # Groesse der Faser p: 2^(n_i-1) je nichtleerem Block; leerer Block hat nur Paritaet 0
    F=[0]*16
    for p in range(16):
        g=1
        for i in range(4):
            bit=(p>>i)&1
            if n[i]==0: g*= (1 if bit==0 else 0)
            else: g*=2**(n[i]-1)
        F[p]=g
    return F
def kleinstes_K(n,R,Kmax=8):
    M=M_tabelle(n,R); F=fasern(n); ps=[p for p in range(16) if F[p]>0]
    for K in range(1,Kmax+1):
        for qs in combinations_with_replacement(range(16),K-1):   # q_1 = 0 o. B. d. A.
            q=(0,)+qs
            if all(sum(M[p^x] for x in q)>=F[p] for p in ps): return K,M
    return Kmax+1,M
for (m,R) in ((16,4),(18,5),(20,6)):
    T=typen(m); erg={}; bd=[]
    for n in T:
        K,M=kleinstes_K(n,R); erg.setdefault(K,[]).append(n); bd.append(sum(M)//8)
    print("Zelle (%d,%d): %d Typen; B(D) = Summe M / 8 zwischen %d und %d; Zaehlschranke 2^(m-3)/B(D) zwischen %.2f und %.2f"%(m,R,len(T),min(bd),max(bd),2**(m-3)/max(bd),2**(m-3)/min(bd)))
    for K in sorted(erg):
        print("   kleinstes K der Faserbedingung = %s: %d Typen%s"%(K if K<=8 else '>8',len(erg[K]),'' if len(erg[K])>12 else '  '+str(erg[K])))
    print("   mit K = 5 (40 Woerter, Rekord) nicht ausgeschlossen: %d Typen: %s"%(sum(len(v) for k,v in erg.items() if k<=5), [n for k,v in erg.items() if k<=5 for n in v][:40]))
