# Vortest Orchestrator 25.11.2026: (15,4), Codes in Rang-9-Erweiterungen L' = L u (L+v) des affinen Spanns L von ctrl_16.
# Zuerst nur Zählschranke und Zahl der verschiedenen Erweiterungen; dann optional exakter Test einer Erweiterung (CP-SAT).
import numpy as np, sys, time
R_="controls/"
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
def pop(a): return POP[a&0xFFFF]+POP[a>>16]
rows=[[int(x) for x in z.split()] for z in open(R_+"ctrl_16.txt") if z.split()]
m=15; R=4; full=(1<<m)-1
w=[sum(1<<j for j in range(m) if r[j]==-1) for r in rows]
def basis(vs):
    b=[]
    for v in vs:
        for x in b: v=min(v,v^x)
        if v: b.append(v)
    return b
c0=w[0]; B=basis([c^c0 for c in w]+[full]); assert len(B)==8
red=[]
for b in sorted(B,reverse=True):
    for x in red: b=min(b,b^x)
    red.append(b)
piv=[b.bit_length()-1 for b in red]; rest=[j for j in range(m) if j not in piv]
def rep(t): return sum(((t>>i)&1)<<j for i,j in enumerate(rest))
vs=[rep(t) for t in range(1,128)]
def universum(extra):
    BB=B+extra; L=np.zeros(1,dtype=np.uint32)
    for b in BB: L=np.concatenate([L,L^np.uint32(b)])
    L=L^np.uint32(c0)
    return np.array(sorted(set(min(int(x),int(x)^full) for x in L)),dtype=np.uint32)  # ein Vertreter je Paar
allp=np.arange(1<<(m-1),dtype=np.uint32)   # Punkte bis auf Komplement
def inzidenz(P):
    W=pop(allp[:,None]^P[None,:]); return (W<=R)|(W>=m-R)
if sys.argv[1]=="zaehl":
    res={}
    for v in vs:
        P=universum([v]); A=inzidenz(P); mu=A.sum(axis=1); res.setdefault((int(mu.min()),tuple(np.unique(mu).tolist())),[]).append(v)
    for k,l in sorted(res.items()): print("min mu",k[0],"Werte",k[1],"Erweiterungen",len(l),"Beispiel v =",l[0],"Schranke k >=",round(256/k[0],2))
else:
    from ortools.sat.python import cp_model
    v=int(sys.argv[2]); k=int(sys.argv[3]); zeit=float(sys.argv[4])
    extra=[v] if v else []
    P=universum(extra); A=inzidenz(P)
    Z=np.unique(A,axis=0)                      # verschiedene Bedingungen
    mdl=cp_model.CpModel(); x=[mdl.NewBoolVar(f"x{i}") for i in range(len(P))]
    for z in Z: mdl.AddBoolOr([x[i] for i in np.nonzero(z)[0]])
    mdl.Add(sum(x)<=k); mdl.Add(x[0]==1)      # Verschiebung: Paar 0 ohne Einschränkung im Code
    s=cp_model.CpSolver(); s.parameters.max_time_in_seconds=zeit; s.parameters.num_workers=4
    t=time.time(); st=s.Solve(mdl)
    print(f"v={v} Paare im Universum={len(P)} Bedingungen={len(Z)} k<={k}: {s.StatusName(st)} nach {time.time()-t:.1f} s")
    if st in (cp_model.OPTIMAL,cp_model.FEASIBLE):
        K=[int(P[i]) for i in range(len(P)) if s.Value(x[i])]; print("  Wörter (ein Vertreter je Paar):",K)
