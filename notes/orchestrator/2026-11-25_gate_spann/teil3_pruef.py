# Gate zu Auftrag S (Orchestrator 25.11.2026), Teil 3: Spanne der drei 48er als Mengen vergleichen; eine Kontrolllösung (k = 24,
# Startwert 4, Wörter aus runs/spann/teil3_h13_k24_s4.log.txt) per Brute-Force über 2^20 Punkte nachrechnen und im Spann verorten.
import numpy as np
def lade(f):
    rows=[[int(x) for x in z.split()] for z in open(f) if z.split()]
    m0=len(rows[0]); keep=[j for j in range(m0) if any(r[j]!=0 for r in rows)]
    return [sum(1<<i for i,j in enumerate(keep) if r[j]==-1) for r in rows], len(keep)
def ech(vs):
    B=[]
    for v in vs:
        for b in B: v=min(v,v^b)
        if v: B.append(v); B.sort(reverse=True)
    for i in range(len(B)):
        for j in range(len(B)):
            if i!=j and (B[i]>>(B[j].bit_length()-1))&1: B[i]^=B[j]
    return tuple(sorted(B,reverse=True))
def red(B,v):
    for b in B: v=min(v,v^b)
    return v
S={}
for name,f in [("1p3","controls/had24_1p3_paley12.txt"),("2p2","controls/had24_2p2_paley12.txt"),("neunter","runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt")]:
    w,m=lade(f); full=(1<<m)-1; B=ech([c^w[0] for c in w]+[full]); S[name]=(B,red(B,w[0]),w)
    print(f"{name}: Rang {len(B)}, affiner Spann = (V, c0 mod V) = Kennung {hash((B,red(B,w[0])))%10**10}")
print("gleich als Mengen: 1p3/2p2",S["1p3"][:2]==S["2p2"][:2],"1p3/neunter",S["1p3"][:2]==S["neunter"][:2],"2p2/neunter",S["2p2"][:2]==S["neunter"][:2])
zeile=[l for l in open("runs/spann/teil3_h13_k24_s4.log.txt") if l.startswith("WOERTER:")][0]
W=np.array([int(x) for x in zeile.split()[1:]],dtype=np.int64); m=20; full=(1<<m)-1
B,c0,w13=S["1p3"]
print("Kontrolllösung: Wörter",len(W),"alle im Spann von 1p3:",all(red(B,int(x))==c0 for x in W),"; gleich dem 1p3-Code als Menge:",set(min(int(x),int(x)^full) for x in W)==set(min(c,c^full) for c in w13))
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
x=np.arange(1<<m,dtype=np.int64); best=np.full(1<<m,99,dtype=np.uint8)
for c in W:
    d=POP[(x^c)&0xFFFF]+POP[(x^c)>>16]; best=np.minimum(best,np.minimum(d,m-d))
print("Überdeckungsradius der Kontrolllösung (48 Wörter, alle 2^20 Punkte):",int(best.max()))
