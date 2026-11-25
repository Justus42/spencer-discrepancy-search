# Gate zu Auftrag G, Teil F (Orchestrator 25.11.2026): Z5xZ5, zwei Spalten gestrichen, Stichprobe auf den vier Vertretern
# der Sitzung mit u = 7 (Tafel und Bitkonvention der Sitzung: Bit x = 1 heißt f(x) = -1), alle 300 Spaltenpaare, eigenes Suchen.
import numpy as np, itertools, sys
ARGS=sys.argv; sys.argv=[sys.argv[0]]
import gate_g as gg
sys.argv=ARGS
T=gg.lies("runs/gruppen/tafeln/z5xz5.grp"); n=25; d=9; inv=np.argsort(T,axis=1); rng=np.random.default_rng(7)
for hx in ["8459","8d2f","8f4f","108a8f"]:
    a=int(hx,16); f=np.array([1-2*((a>>x)&1) for x in range(n)],dtype=np.float32); A=f[inv]
    voll=min(int(np.abs(A@x).max()) for x in rng.choice([-1.0,1.0],size=(200000,n)).astype(np.float32)[:,:,None][:,:,0])
    paare=list(itertools.combinations(range(n),2)); offen=set(range(len(paare))); best={}; it=0
    while offen and it<200000:
        x=rng.choice([-1.0,1.0],size=n).astype(np.float32); R=A@x; it+=1
        for j in list(offen):
            c1,c2=paare[j]; m=np.abs(R-x[c1]*A[:,c1]-x[c2]*A[:,c2]).max()
            if m<d: offen.discard(j)
    print(f"z5xz5 f={hx}: beste gefundene Schranke voll {voll}; 300 Spaltenpaare, ohne Gegenbeispiel (max < 9): {len(offen)} nach {it} Zufalls-x",flush=True)
