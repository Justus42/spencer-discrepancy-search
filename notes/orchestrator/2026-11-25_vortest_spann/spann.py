# Vortest Orchestrator 25.11.2026: Codes innerhalb des affinen Spanns L eines bekannten guten Codes.
# mu(D) = Zahl der Wortpaare {l, l+1} aus L im Abstand <= R von einem Punkt der Nebenklasse D (für alle Punkte von D gleich).
# Zählschranke: ein Code aus k Paaren in L braucht k >= |L|/2 / min_D mu(D)   (L/<1> wirkt durch Verschiebung).
import numpy as np, sys
R_="controls/"
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
def pop(a): return POP[a&0xFFFF]+POP[a>>16]
def lade(f):
    rows=[[int(x) for x in z.split()] for z in open(R_+f) if z.split()]
    m=len(rows[0]); keep=[j for j in range(m) if any(r[j]!=0 for r in rows)]
    return [sum(1<<i for i,j in enumerate(keep) if r[j]==-1) for r in rows], len(keep)
def basis(vs):
    b=[]
    for v in vs:
        for x in b: v=min(v,v^x)
        if v: b.append(v)
    return b
for f,R,ziel in [("ctrl_8.txt",1,8),("ctrl_16.txt",4,15),("ctrl_17.txt",5,15),("L2026_17x17.txt",5,15),("astra_19.txt",6,15),("had24_1p3_paley12.txt",6,20),("had24_2p2_paley12.txt",6,20)]:
    w,m=lade(f); full=(1<<m)-1; c0=w[0]
    B=basis([c^c0 for c in w]+[full]); r=len(B)
    L=np.zeros(1,dtype=np.uint32)
    for b in B: L=np.concatenate([L,L^np.uint32(b)])
    L=L^np.uint32(c0)
    # Vertreter der Nebenklassen von span(B): Punkte mit Nullen auf den Pivot-Bits
    # reduzierte Basis -> Pivot = höchstes Bit
    red=[]
    for b in sorted(B,reverse=True):
        for x in red: b=min(b,b^x)
        red.append(b)
    piv=[b.bit_length()-1 for b in red]; assert len(set(piv))==r
    rest=[j for j in range(m) if j not in piv]
    t=np.arange(1<<(m-r),dtype=np.uint32); reps=np.zeros_like(t)
    for i,j in enumerate(rest): reps|=((t>>i)&1)<<j
    mu=np.zeros(len(reps),dtype=np.int64)
    for s in range(0,len(reps),256):
        W=pop(reps[s:s+256,None]^L[None,:]); mu[s:s+256]=(W<=R).sum(axis=1)
    # eigener Code: Paare
    k0=len(set(min(c,c^full) for c in w))
    half=len(L)//2; mn=int(mu.min())
    vals,cnt=np.unique(mu,return_counts=True)
    print(f"{f}: (m,R)=({m},{R}) Rang {r}, Paare in L {half}, eigener Code {k0} Paare, min mu={mn}, Schranke k >= {half}/{mn} = {half/mn:.2f}; Ziel {ziel} Paare -> {'AUSGESCHLOSSEN' if ziel*mn<half else 'nicht ausgeschlossen'}")
    print("    Verteilung mu:",dict(zip(vals.tolist(),cnt.tolist())))
