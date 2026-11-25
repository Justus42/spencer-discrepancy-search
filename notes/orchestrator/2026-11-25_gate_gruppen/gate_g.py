# Gate zu Auftrag G, Orchestrator 25.11.2026. Eigenes Programm, ohne Code der Sitzung, OHNE Symmetrieabbau.
# Teil A: eigene Gruppentafeln, Vergleich der Kennzahlen mit den Tafeln der Sitzung (runs/gruppen/tafeln/*.grp).
# Teil B: für JEDES f: G -> {+1,-1} mit f(e) = +1 (f und -f geben dieselbe Diskrepanz) ein x mit max |A_f x| < d,
#         A_f[g][h] = f(g^-1 h), also (A_f x)[g] = sum_k f(k) x(g k). Mit Rand zusätzlich |sum x| < d.
#         Zufällige x im Stapel; Rest: alle x durchprobiert (n <= 21) bzw. 2^22 Zufalls-x (n = 25).
# Aufruf (aus dem Repo-Wurzelordner des Sitzungsbranches): python3 gate_g.py [A] [B name ...]
import numpy as np, sys, time, itertools
from math import gcd
def tafel(elems, mul):
    idx={e:i for i,e in enumerate(elems)}
    return np.array([[idx[mul(a,b)] for b in elems] for a in elems],dtype=np.int64)
def prod(*r): return list(itertools.product(*[range(k) for k in r]))
G={}
G["z3xz6"]=(tafel(prod(3,6),lambda a,b:((a[0]+b[0])%3,(a[1]+b[1])%6)),8,True)
dih=lambda m:(lambda a,b:((a[0]+(b[0] if a[1]==0 else -b[0]))%m,(a[1]+b[1])%2))
G["d9"]=(tafel(prod(9,2),dih(9)),8,True)
G["s3xz3"]=(tafel(prod(3,2,3),lambda a,b:((a[0]+(b[0] if a[1]==0 else -b[0]))%3,(a[1]+b[1])%2,(a[2]+b[2])%3)),8,True)
G["z3z3_z2"]=(tafel(prod(3,3,2),lambda a,b:((a[0]+(b[0] if a[2]==0 else -b[0]))%3,(a[1]+(b[1] if a[2]==0 else -b[1]))%3,(a[2]+b[2])%2)),8,True)
G["z2xz10"]=(tafel(prod(2,10),lambda a,b:((a[0]+b[0])%2,(a[1]+b[1])%10)),8,False)
G["d10"]=(tafel(prod(10,2),dih(10)),8,False)
G["dic5"]=(tafel(prod(10,2),lambda a,b:((a[0]+(b[0] if a[1]==0 else -b[0])+5*a[1]*b[1])%10,(a[1]+b[1])%2)),8,False)
G["f20"]=(tafel(prod(5,4),lambda a,b:((a[0]+pow(2,a[1],5)*b[0])%5,(a[1]+b[1])%4)),8,False)
G["f21"]=(tafel(prod(7,3),lambda a,b:((a[0]+pow(2,a[1],7)*b[0])%7,(a[1]+b[1])%3)),9,True)
G["z5xz5"]=(tafel(prod(5,5),lambda a,b:((a[0]+b[0])%5,(a[1]+b[1])%5)),9,False)
def kenn(T):
    n=len(T); assert all(sorted(T[i])==list(range(n)) for i in range(n)) and all(sorted(T[:,j])==list(range(n)) for j in range(n))
    ass=all((T[T[a]][:,:]==T[a][T]).all() for a in range(n))   # (a b) c == a (b c)
    e=[i for i in range(n) if (T[i]==np.arange(n)).all() and (T[:,i]==np.arange(n)).all()]; assert len(e)==1
    ords=[]
    for g in range(n):
        k=1; h=g
        while h!=e[0]: h=T[h][g]; k+=1
        ords.append(k)
    zen=sum(1 for g in range(n) if (T[g]==T[:,g]).all())
    return ass, e[0], bool((T==T.T).all()), zen, tuple(sorted((o,ords.count(o)) for o in set(ords)))
def lies(fn):
    z=[l for l in open(fn) if not l.startswith("#")]; n=int(z[0]); return np.array([[int(x) for x in z[1+i].split()] for i in range(n)])
if "A" in sys.argv:
    for name,(T,d,rand) in G.items():
        k1=kenn(T); k2=kenn(lies(f"runs/gruppen/tafeln/{name}.grp"))
        print(f"{name}: n={len(T)} assoziativ {k1[0]}/{k2[0]} abelsch {k1[2]}/{k2[2]} Zentrum {k1[3]}/{k2[3]} Ordnungen {k1[4]} {'GLEICH' if k1[2:]==k2[2:] else 'ABWEICHUNG '+str(k2[4])}",flush=True)
def alle_f(n,lo,hi):
    c=np.arange(lo,hi,dtype=np.int64); F=np.ones((hi-lo,n),dtype=np.float32)
    for k in range(1,n): F[:,k]=1-2*((c>>(k-1))&1)
    return F
def lauf(name,T,d,rand,rng):
    n=len(T); assert kenn(T)[1]==0
    tot=1<<(n-1); CH=1<<18; rest_ges=0; xs=0; t0=time.time(); offen=[]
    for lo in range(0,tot,CH):
        F=alle_f(n,lo,min(tot,lo+CH)); it=0
        while len(F)>0 and it<3000:
            x=rng.choice([-1.0,1.0],size=n).astype(np.float32)
            if rand and abs(x.sum())>=d: continue
            R=F@x[T].T; F=F[np.abs(R).max(axis=1)>=d]; it+=1
        xs+=it
        for f in F:                       # Rest einzeln
            # (A_f x)[g] = sum_k f(k) x(g k): für viele x auf einmal X[:,T[g,k]]
            ok=False
            if n<=21:
                for s in range(0,1<<(n-1),1<<18):
                    c=np.arange(s,min(1<<(n-1),s+(1<<18)),dtype=np.int64); X=np.ones((len(c),n),dtype=np.float32)
                    for k in range(1,n): X[:,k]=1-2*((c>>(k-1))&1)
                    m=np.zeros(len(c),dtype=np.float32)
                    for g in range(n): m=np.maximum(m,np.abs(X[:,T[g]]@f))
                    if rand: m=np.maximum(m,np.abs(X.sum(axis=1)))
                    if (m<d).any(): ok=True; break
            else:
                for _ in range(16):
                    X=rng.choice([-1.0,1.0],size=(1<<18,n)).astype(np.float32); m=np.zeros(len(X),dtype=np.float32)
                    for g in range(n): m=np.maximum(m,np.abs(X[:,T[g]]@f))
                    if (m<d).any(): ok=True; break
            rest_ges+=1
            if not ok: offen.append(f.astype(int).tolist())
    print(f"{name}{' mit Rand' if rand else ''}: n={n} d={d}: {tot} Funktionen, {xs} Zufalls-x, {rest_ges} einzeln nachbehandelt, OHNE Gegenbeispiel: {len(offen)}  ({time.time()-t0:.0f} s)",flush=True)
    for f in offen[:5]: print("   offen:",f)
if "B" in sys.argv:
    namen=sys.argv[sys.argv.index("B")+1:]; rng=np.random.default_rng(20261125)
    for name in namen:
        T,d,rand=G[name]; lauf(name,T,d,False,rng)
        if rand: lauf(name,T,d,True,rng)
