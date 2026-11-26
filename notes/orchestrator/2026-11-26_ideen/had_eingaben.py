#!/usr/bin/env python3
"""had_eingaben.py -- schreibt die Eingabedateien fuer had_streich (Ideenrunde 26.11.2026, Orchestrator).
had8/had16syl (Sylvester), had12 (Paley q=11), had16iv (aus ctrl_16 ergaenzt, mit Transponierter), had20 (3 Klassen + Transponierte),
had24 (60 McKay-Klassen + Transponierte). Jede Matrix wird auf Hadamard-Eigenschaft geprueft."""
import sys, os
repo, out = sys.argv[1], sys.argv[2]
def chk(H):
    n=len(H); assert all(len(r)==n for r in H)
    for i in range(n):
        for j in range(i+1,n): assert sum(a*b for a,b in zip(H[i],H[j]))==0, (i,j)
def T(H): return [list(r) for r in zip(*H)]
def wr(name, mats):
    for H in mats: chk(H)
    with open(os.path.join(out,name),'w') as f:
        for H in mats:
            for r in H: f.write(' '.join('%d'%v for v in r)+'\n')
            f.write('\n')
    print(name, len(mats), 'Matrizen, Ordnung', len(mats[0]))
syl=lambda n:[[1-2*(bin(i&j).count('1')&1) for j in range(n)] for i in range(n)]
wr('had8.txt',[syl(8)]); wr('had16syl.txt',[syl(16)]); wr('had32syl.txt',[syl(32)])
q=11; qr={x*x%q for x in range(1,q)}; chi=lambda a:0 if a%q==0 else (1 if a%q in qr else -1)
P=[[1]*12]+[[-1]+[(1 if i==j else chi(j-i)) for j in range(q)] for i in range(q)]
wr('had12.txt',[P])
rows=[[int(v) for v in l.split()] for l in open(os.path.join(repo,'controls/ctrl_16.txt')) if l.strip()]
A=[r[:15] for r in rows]
last=[1]+[-sum(a*b for a,b in zip(A[0],A[i])) for i in range(1,16)]
assert all(abs(v)==1 for v in last)
H=[A[i]+[last[i]] for i in range(16)]
wr('had16iv.txt',[H,T(H)])
ms=[]
for k in (1,2,3):
    H=[[int(v) for v in l.split()] for l in open(os.path.join(repo,'controls/had20_klasse%d.txt'%k)) if l.strip()]
    ms+= [H,T(H)]
wr('had20.txt',ms)
ms=[]
for l in open(os.path.join(repo,'controls/had24_mckay_hex.txt')):
    t=l.split()
    if len(t)!=24: continue
    H=[[1 if (int(h,16)>>(23-j))&1 else -1 for j in range(24)] for h in t]
    ms+=[H,T(H)]
wr('had24.txt',ms)
