# Gate zu Auftrag G, Teil E (Orchestrator 25.11.2026): exakte Diskrepanz aller f der Menge L (Ordnung 20, Ziel max < 6),
# alle 2^19 x mit x(e) = +1. Dazu Bahnen von L unter Links-/Rechtsmultiplikation und Komplement (ohne Automorphismen).
import numpy as np, sys, time
ARGS=sys.argv; sys.argv=[sys.argv[0]]
import gate_g as gg, gate_g2
sys.argv=ARGS
n=20; c=np.arange(1<<19,dtype=np.int64); X=np.ones((1<<19,n),dtype=np.float32)
for k in range(1,n): X[:,k]=1-2*((c>>(k-1))&1)
for name in sys.argv[1:]:
    t0=time.time(); T,d,_=gg.G[name]; rng=np.random.default_rng(2026112502)
    L,xs=gate_g2.widerlege(T,6,False,rng,8000); inv=np.argsort(T,axis=1)
    ex=[]
    for f in L:
        A=f[inv]; ex.append(int(np.abs(X@A.T).max(axis=1).min()))
    ex=np.array(ex); echt=L[ex>=6]
    # Bahnen unter f(x)->f(kx), f(x)->f(xl), Komplement
    S=set(tuple(int(v) for v in f) for f in echt); S|=set(tuple(-v for v in f) for f in S); bahnen=[]
    while S:
        f0=np.array(next(iter(S))); orb=set()
        for k in range(n):
            for l in range(n):
                g=f0[T[T[k]][:,l]]   # x -> f0(k x l)
                orb.add(tuple(int(v) for v in g)); orb.add(tuple(int(-v) for v in g))
        assert orb<=S or not (orb-S), "Bahn verlässt die Menge"
        bahnen.append(len(orb)); S-=orb
    vals,cnt=np.unique(ex,return_counts=True)
    print(f"{name}: |L| = {len(L)}, exakte Diskrepanz: {dict(zip(vals.tolist(),cnt.tolist()))}; f mit disc >= 6 (mit Komplement): {2*len(echt)}; Bahnen unter Verschiebungen und Komplement: {sorted(bahnen)} ({time.time()-t0:.0f} s)",flush=True)
