# Gate zu Auftrag S (Orchestrator 25.11.2026): unabhängige Prüfung der Problemdateien und Symmetrie-Erzeuger der Sitzung.
# 1. Die Wörter w_g bilden mit ihren Komplementen einen affinen Raum der Dimension q+1, der den Ausgangscode enthält (und y).
# 2. Treue: Die Familie { Überdecker-Menge C(x) : x im Würfel } (direkt gerechnet) ist gleich { A_D + t : D, t }.
# 3. Jeder Erzeuger phi (linear auf G) ist bijektiv und bildet jedes Muster A_D auf ein Element der Familie ab.
# Aufruf: pruef_problem.py PROBLEM QUELLMATRIX [ERZEUGERDATEI]
import sys, numpy as np
prob,quelle=sys.argv[1],sys.argv[2]; gen=sys.argv[3] if len(sys.argv)>3 else None
Z=[l for l in open(prob)]; kopf=Z[0]; q,nD=map(int,Z[1].split()); w=np.array([int(x) for x in Z[2].split()],dtype=np.int64); n=1<<q
m=int(kopf.split("m=")[1].split(",")[0]); R=int(kopf.split("R=")[1].split(",")[0]); full=(1<<m)-1
assert len(w)==n and nD==1<<(m-q-1)
# 1
norm=lambda a: np.minimum(a,a^full)
idx={int(v):g for g,v in enumerate(norm(w))}; assert len(idx)==n
ok_lin=all(int(norm(np.array([int(w[g])^int(w[h])^int(w[0])]))[0]) in idx and idx[int(norm(np.array([int(w[g])^int(w[h])^int(w[0])]))[0])]==(g^h) for g in (1,2,5,n-1,n//2+3) for h in range(n))
rows=[[int(x) for x in z.split()] for z in open(quelle) if z.split()]
cw=[sum(1<<j for j in range(m) if r[j]==-1) for r in rows]
drin=all(min(c,c^full) in idx for c in cw)
ypart=kopf.split("Erweiterung y=")[1].strip() if "Erweiterung y=" in kopf else "None"
yok=True if ypart=="None" else (min(int(w[0])^int(ypart),int(w[0])^int(ypart)^full) in idx)
# 2
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
x=np.arange(1<<(m-1),dtype=np.int64)
Wt=POP[(x[:,None]^w[None,:])&0xFFFF]                      # m <= 16
cov=(Wt<=R)|(Wt>=m-R)                                      # (2^(m-1), n)
def packe(B): return [int.from_bytes(np.packbits(r,bitorder="little").tobytes(),"little") for r in B]
links=set(packe(cov))
muster=[]
for l in Z[3:3+nD]:
    t=l.split(); A=[int(v) for v in t[2:]]; assert len(A)==int(t[1]); muster.append(A)
def maske(A): 
    v=0
    for g in A: v|=1<<g
    return v
rechts=set()
for A in muster:
    for t in range(n): rechts.add(maske([g^t for g in A]))
treu=(links==rechts)
print(f"{prob.split('/')[-1]}: q={q} Nebenklassen={nD}; affin {ok_lin}; Ausgangscode enthalten {drin}; y enthalten {yok}; Familie direkt {len(links)} / aus Mustern {len(rechts)}: {'TREU' if treu else 'ABWEICHUNG'}",end="")
# 3
if gen:
    alle=True; ng=0
    for l in open(gen):
        im=[int(v) for v in l.split()]
        if len(im)!=q: continue
        ng+=1
        phi=[0]*n
        for g in range(n):
            v=0
            for i in range(q):
                if (g>>i)&1: v^=im[i]
            phi[g]=v
        bij=len(set(phi))==n
        erhalten=all(maske([phi[g] for g in A]) in rechts for A in muster)
        alle&=bij and erhalten
    print(f"; {ng} Erzeuger: {'alle gültig' if alle else 'UNGÜLTIG'}",end="")
print()
