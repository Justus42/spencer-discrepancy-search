#!/usr/bin/env python3
"""rm15_loch30.py -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
Struktur der Loecher von Sylvester 32 ohne 2 Spalten (Punkte 0 und b) bei d = 10.
Behauptung: y in {+-1}^30 ist Loch  <=>  die gerade Fortsetzung x (x_0 = +1) ist ein tiefes Loch von RM(1,5) (alle |W| <= 8) und
  x_b = +1: kein W(u) = -8 auf der Hyperebene u.b = 0;   x_b = -1: kein W(u) = -8 ausserhalb davon.
Zaehlt das ueber alle 27 387 136 tiefen Loecher (Datei tief.bin aus rm15_loecher) und vergleicht mit 430 080 Punkten = 4 x 107 520 Klassen (Gate D).
Aufruf: rm15_loch30.py tief.bin"""
import sys, numpy as np
D=np.fromfile(sys.argv[1],dtype=np.uint32); n=len(D)
A=np.where((D[:,None]>>np.arange(32,dtype=np.uint32))&1,-1,1).astype(np.int16)
h=1
while h<32:
    A=A.reshape(n,32//(2*h),2,h); A=np.concatenate([A[:,:,0,:]+A[:,:,1,:],A[:,:,0,:]-A[:,:,1,:]],axis=2).reshape(n,32); h*=2
# Achtung: obige Umordnung liefert die Walsh-Transformation mit Index u in natuerlicher Reihenfolge
u=np.arange(32); typ16=(np.abs(A)==8).sum(1)==16
for b in (1,7,31):
    senk=np.array([bin(x&b).count('1')%2==0 for x in u]); chi_b=np.where(senk,1,-1)
    ges=0; je={True:0,False:0}
    for v in range(32):
        Wv=A[:,u^v]
        for s in (1,-1):
            W=s*Wv; x0=W.sum(1)//32; xb=(W*chi_b).sum(1)//32
            ok1=(x0==1)&(xb==1)&~((W==-8)&senk).any(1); ok2=(x0==1)&(xb==-1)&~((W==-8)&~senk).any(1)
            ok=ok1|ok2; ges+=int(ok.sum()); je[True]+=int((ok&typ16).sum()); je[False]+=int((ok&~typ16).sum())
    print("b=%2d: Loecher mit x_0=+1: %d (Soll 430080 = 4 x 107520 Klassen aus Gate D); davon Spektrum [8^16 0^16]: %d, Spektrum [8^12 4^16 0^4]: %d"%(b,ges,je[True],je[False]))
