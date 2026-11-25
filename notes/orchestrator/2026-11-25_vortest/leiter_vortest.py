# Vortest Ansatz 2 (exakt): Kern der Codes, Luecken beim Streichen einer Nebenklasse
import sys, numpy as np, glob, os
n=20; FULL=(1<<n)-1
X=np.arange(1<<n,dtype=np.uint32)
def lade(fn): return sorted(set(int(t) for t in open(fn).read().split()))
def kern(W):
    S=set(W); return [t for t in (w^W[0] for w in W) if all((w^t) in S for w in W)]
def analyse(name,W):
    S=set(W); assert all((w^FULL) in S for w in W)
    K=kern(W); 
    # Nebenklassen des Kerns
    seen=set(); cos=[]
    for w in W:
        if w in seen: continue
        c=[w^t for t in K]; seen.update(c); cos.append(c)
    D=np.empty((len(cos),1<<n),dtype=np.uint8)
    for i,c in enumerate(cos):
        d=np.full(1<<n,n,dtype=np.uint8)
        for w in c: np.minimum(d,np.bitwise_count(X^np.uint32(w)),out=d)
        D[i]=d
    cov=(D<=6); cnt=cov.sum(axis=0)
    rad=int(D.min(axis=0).max())
    allein=[int((cov[i]&(cnt==1)).sum()) for i in range(len(cos))]
    a=[t for t in K if t not in (0,FULL)]
    typ=sorted({bin(t).count('1') for t in a})
    print(f"{name}: {len(W)} Woerter, Radius {rad}, Kern {len(K)} Woerter (Gewichte ohne 0/1: {typ}), {len(cos)} Nebenklassen, allein ueberdeckte Punkte je Nebenklasse: {sorted(allein)}")
for fn in sys.argv[1:]:
    analyse(os.path.basename(fn), lade(fn))
