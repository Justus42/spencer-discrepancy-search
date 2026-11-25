# Gate zu Auftrag G, Teil C und D (Orchestrator 25.11.2026), eigenes Programm ohne Symmetrieabbau. Benutzt gate_g.py (Tafeln).
# C: Kontrolle (Z7 mit Rand, d = 5 muss Unwiderlegte lassen) und Z5xZ5 voll (2^24 Funktionen).
# D: zwei Spalten gestrichen. L = alle f, für die kein x mit max < d-2 gefunden wurde (Obermenge von {disc >= d-2}).
#    Für jedes f in L und jedes Spaltenpaar ein x mit max |A' x| < d.
import numpy as np, sys, time, itertools
ARGS=sys.argv; sys.argv=[sys.argv[0]]
import gate_g as gg
sys.argv=ARGS
def widerlege(T,d,rand,rng,maxit):
    n=len(T); tot=1<<(n-1); CH=1<<18; rest=[]; xs=0
    for lo in range(0,tot,CH):
        F=gg.alle_f(n,lo,min(tot,lo+CH)); it=0
        while len(F)>0 and it<maxit:
            x=rng.choice([-1.0,1.0],size=n).astype(np.float32)
            if rand and abs(x.sum())>=d: continue
            F=F[np.abs(F@x[T].T).max(axis=1)>=d]; it+=1
        xs+=it; rest.append(F)
    return np.concatenate(rest),xs
rng=np.random.default_rng(2026112502)
for was in sys.argv[1:]:
    t0=time.time()
    if was=="z7":
        T=gg.tafel(list(range(7)),lambda a,b:(a+b)%7)
        for rand,d in ((True,5),(False,5),(True,7)):
            L,xs=widerlege(T,d,rand,rng,4000)
            # exakt: alle x
            X=np.array(list(itertools.product([-1.0,1.0],repeat=7)),dtype=np.float32); echt=0
            for f in L:
                m=np.zeros(len(X),dtype=np.float32)
                for g in range(7): m=np.maximum(m,np.abs(X[:,T[g]]@f))
                if rand: m=np.maximum(m,np.abs(X.sum(axis=1)))
                echt+=int(m.min()>=d)
            print(f"Kontrolle Z7 Rand={rand} d={d}: 64 Funktionen, unwiderlegt {len(L)}, davon exakt disc >= d: {echt}",flush=True)
    elif was=="z5xz5":
        T,d,_=gg.G["z5xz5"]; L,xs=widerlege(T,d,False,rng,6000)
        print(f"z5xz5 voll d={d}: {1<<24} Funktionen, {xs} Zufalls-x (Summe über 64 Blöcke), OHNE Gegenbeispiel: {len(L)} ({time.time()-t0:.0f} s)",flush=True)
    else:
        T,d,_=gg.G[was]; n=len(T)
        L,xs=widerlege(T,d-2,False,rng,8000)
        print(f"{was}: n={n}, Ziel max < {d-2}: unwiderlegt |L| = {len(L)} (Obermenge von disc >= {d-2}) ({time.time()-t0:.0f} s)",flush=True)
        if len(L)==0: continue
        inv=np.argsort(T,axis=1)                      # inv[g][c] = k mit g k = c, also A[g][c] = f(g^-1 c) = f(inv[g][c])
        A=L[:,inv]                                    # (|L|, n, n)
        paare=list(itertools.combinations(range(n),2)); offen=np.ones((len(L),len(paare)),dtype=bool); it=0
        p0=np.array([p[0] for p in paare]); p1=np.array([p[1] for p in paare])
        while offen.any() and it<20000:
            x=rng.choice([-1.0,1.0],size=n).astype(np.float32); it+=1
            R=A@x                                      # (|L|, n)
            fi=np.nonzero(offen.any(axis=1))[0]
            for i in fi:
                pj=np.nonzero(offen[i])[0]
                Rp=R[i][None,:]-x[p0[pj]][:,None]*A[i][:,p0[pj]].T-x[p1[pj]][:,None]*A[i][:,p1[pj]].T
                offen[i,pj[np.abs(Rp).max(axis=1)<d]]=False
        print(f"   {len(L)} x {len(paare)} = {len(L)*len(paare)} gestrichene Matrizen, {it} Zufalls-x, OHNE Gegenbeispiel (max < {d}): {int(offen.sum())} ({time.time()-t0:.0f} s)",flush=True)
