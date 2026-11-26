#!/usr/bin/env python3
"""rm15_klassen.py -- Gegenpruefung 26.11.2026 (Orchestrator): Behauptung aus Geminis Deep Research nachgerechnet; ein Programm.
Behauptung: Die 427 924 tiefen Loch-Nebenklassen von RM(1,5) zerfallen in vier Klassen (Grad 2: 868; zwei Klassen vom Grad 3 mit
verschiedenen Autokorrelationsspektren; Grad 4: 208 320), und beim Streichen von zwei Spalten liefern nur die zweite Grad-3-Klasse und die
Grad-4-Klasse Loecher bei d = 10. Hier: Nebenklassen nach (Walsh-Spektrum, Grad, Autokorrelationsspektrum) zaehlen und je Gruppe die
Loecher bei (30,10) zaehlen (Punkte 0 und b = 1 gestrichen, Normierung x_0 = +1; Summe muss 430 080 sein).
Aufruf: rm15_klassen.py tief.bin   (tief.bin aus rm15_loecher.c)"""
import sys, collections, numpy as np
D=np.fromfile(sys.argv[1],dtype=np.uint32); n=len(D)
bits=((D[:,None]>>np.arange(32,dtype=np.uint32))&1).astype(np.int16)
def walsh(A):
    A=A.copy(); h=1
    while h<32:
        A=A.reshape(n,32//(2*h),2,h); A=np.concatenate([A[:,:,0,:]+A[:,:,1,:],A[:,:,0,:]-A[:,:,1,:]],axis=2).reshape(n,32); h*=2
    return A
anf=bits.copy(); h=1
while h<32:                       # Moebius-Transformation (ANF)
    anf=anf.reshape(n,32//(2*h),2,h); anf=np.concatenate([anf[:,:,0,:],anf[:,:,0,:]^anf[:,:,1,:]],axis=2).reshape(n,32); h*=2
gew=np.array([bin(u).count('1') for u in range(32)]); grad=(anf*gew).max(1)
W=walsh(1-2*bits); AK=walsh((W.astype(np.int32)**2))//32           # Autokorrelation r(a)
typ16=(np.abs(W)==8).sum(1)==16
def sig(row): return tuple(sorted(collections.Counter(int(abs(v)) for v in row).items()))
aks=[sig(r) for r in AK]
u=np.arange(32); b=1; senk=np.array([bin(x&b).count('1')%2==0 for x in u]); chi=np.where(senk,1,-1)
loch=np.zeros(n,np.int64)
for v in range(32):
    Wv=W[:,u^v]
    for s in (1,-1):
        X=s*Wv; x0=X.sum(1)//32; xb=(X*chi).sum(1)//32
        ok=((x0==1)&(xb==1)&~((X==-8)&senk).any(1))|((x0==1)&(xb==-1)&~((X==-8)&~senk).any(1)); loch+=ok
grp=collections.defaultdict(lambda:[0,0])
for i in range(n):
    k=('8^16 0^16' if typ16[i] else '8^12 4^16 0^4', int(grad[i]), aks[i]); grp[k][0]+=1; grp[k][1]+=int(loch[i])
ges=0
for k in sorted(grp):
    print("Spektrum [%s], Grad %d, |Autokorrelation| %s: %d Nebenklassen, %d Loecher bei (30,10)"%(k[0],k[1],dict(k[2]),grp[k][0],grp[k][1])); ges+=grp[k][1]
print("Summe: %d Nebenklassen, %d Loecher (Soll 427924 und 430080)"%(n,ges))
