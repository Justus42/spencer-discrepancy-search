# Vortest Orchestrator 25.11.2026: exakter Test (CP-SAT) im affinen Spann L eines Codes, optional L plus Zusatzvektoren.
# Aufruf: spann_sat.py DATEI R k SEKUNDEN [v ...]    (v = Zusatzvektor als ganze Zahl, erweitert L)
import numpy as np, sys, time
from ortools.sat.python import cp_model
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
def pop(a): return POP[a&0xFFFF]+POP[a>>16]
def basis(vs):
    b=[]
    for v in vs:
        for x in b: v=min(v,v^x)
        if v: b.append(v)
    return b
f=sys.argv[1]; R=int(sys.argv[2]); k=int(sys.argv[3]); zeit=float(sys.argv[4]); extra=[int(x) for x in sys.argv[5:]]
rows=[[int(x) for x in z.split()] for z in open(f) if z.split()]
m=len(rows[0]); keep=[j for j in range(m) if any(r[j]!=0 for r in rows)]
w=[sum(1<<i for i,j in enumerate(keep) if r[j]==-1) for r in rows]; m=len(keep); full=(1<<m)-1; c0=w[0]
B=basis([c^c0 for c in w]+[full]+extra)
L=np.zeros(1,dtype=np.uint32)
for b in B: L=np.concatenate([L,L^np.uint32(b)])
L=L^np.uint32(c0)
P=np.array(sorted(set(min(int(x),int(x)^full) for x in L)),dtype=np.uint32)
allp=np.arange(1<<(m-1),dtype=np.uint32)
W=pop(allp[:,None]^P[None,:]); A=(W<=R)|(W>=m-R); Z=np.unique(A,axis=0)
mdl=cp_model.CpModel(); x=[mdl.NewBoolVar(f"x{i}") for i in range(len(P))]
for z in Z: mdl.AddBoolOr([x[i] for i in np.nonzero(z)[0]])
mdl.Add(sum(x)<=k); mdl.Add(x[0]==1)
s=cp_model.CpSolver(); s.parameters.max_time_in_seconds=zeit; s.parameters.num_workers=2
t=time.time(); st=s.Solve(mdl)
print(f"{f} Rang {len(B)} Paare {len(P)} Bedingungen {len(Z)} (kleinste {int(Z.sum(axis=1).min())}) k<={k}: {s.StatusName(st)} nach {time.time()-t:.1f} s",flush=True)
if st in (cp_model.OPTIMAL,cp_model.FEASIBLE): print("  Vertreter je Paar:",[int(P[i]) for i in range(len(P)) if s.Value(x[i])])
