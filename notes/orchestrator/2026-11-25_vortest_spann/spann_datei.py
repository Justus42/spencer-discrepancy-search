# Vortest Orchestrator 25.11.2026: wie spann.py, aber für beliebige Matrixdateien. Aufruf: spann_datei.py R DATEI ...
import numpy as np, sys
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
def pop(a): return POP[a&0xFFFF]+POP[a>>16]
def basis(vs):
    b=[]
    for v in vs:
        for x in b: v=min(v,v^x)
        if v: b.append(v)
    return b
R=int(sys.argv[1])
for f in sys.argv[2:]:
    rows=[[int(x) for x in z.split()] for z in open(f) if z.split() and not z.startswith("#")]
    m=len(rows[0]); keep=[j for j in range(m) if any(r[j]!=0 for r in rows)]
    w=[sum(1<<i for i,j in enumerate(keep) if r[j]==-1) for r in rows]; m=len(keep); full=(1<<m)-1; c0=w[0]
    B=basis([c^c0 for c in w]+[full]); r=len(B)
    L=np.zeros(1,dtype=np.uint32)
    for b in B: L=np.concatenate([L,L^np.uint32(b)])
    L=L^np.uint32(c0)
    red=[]
    for b in sorted(B,reverse=True):
        for x in red: b=min(b,b^x)
        red.append(b)
    piv=[b.bit_length()-1 for b in red]; rest=[j for j in range(m) if j not in piv]
    t=np.arange(1<<(m-r),dtype=np.uint32); reps=np.zeros_like(t)
    for i,j in enumerate(rest): reps|=((t>>i)&1)<<j
    mu=np.zeros(len(reps),dtype=np.int64)
    for s in range(0,len(reps),64):
        W=pop(reps[s:s+64,None]^L[None,:]); mu[s:s+64]=(W<=R).sum(axis=1)
    vals,cnt=np.unique(mu,return_counts=True); half=len(L)//2
    print(f"{f}: m={m} Zeilen={len(w)} Rang {r}, Paare in L {half}, min mu {mu.min()}, Schranke {half/mu.min():.2f}, mu: {dict(zip(vals.tolist(),cnt.tolist()))}, Basis-Kennung {hash(tuple(sorted(red)))%10**8}")
