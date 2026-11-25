# Vortest Orchestrator 25.11.2026, Kandidat 1: lineare [m,6]-Codes mit Einsvektor, Radius R.
# Je Nebenklasse: Zahl der Wortpaare {c, c+1} mit Abstand <= R. Minimum 0: Radius > R. Minimum 1: kein Wort streichbar.
import numpy as np, sys, random
m=int(sys.argv[1]); R=int(sys.argv[2]); N=int(sys.argv[3]); seed=int(sys.argv[4]); modus=sys.argv[5]
rng=random.Random(seed)
POP=np.array([bin(i).count("1") for i in range(1<<16)],dtype=np.uint8)
def pop(a): return POP[a&0xFFFF]+POP[a>>16]
def rang(vs):
    b=[]
    for v in vs:
        for x in b: v=min(v,v^x)
        if v: b.append(v)
    return len(b)
hist={}; best=[]
for it in range(N):
    # Spalten: Punkte in AG(5,2) = Vektoren in Z_2^6 mit Bit 5 gesetzt
    if modus=="verschieden": cols=[32|p for p in rng.sample(range(32),m)]
    else: cols=[32|rng.randrange(32) for _ in range(m)]
    if rang(cols)<6: continue
    rows=[sum(((cols[j]>>i)&1)<<j for j in range(m)) for i in range(6)]
    words=np.zeros(64,dtype=np.uint32)
    for k in range(64):
        w=0
        for i in range(6):
            if (k>>i)&1: w^=rows[i]
        words[k]=w
    # Informationsmenge
    info=[]; b=[]
    for j in range(m):
        v=cols[j]
        for x in b: v=min(v,v^x)
        if v: b.append(v); info.append(j)
        if len(info)==6: break
    rest=[j for j in range(m) if j not in info]
    r=np.arange(1<<(m-6),dtype=np.uint32); reps=np.zeros_like(r)
    for i,j in enumerate(rest): reps|=((r>>i)&1)<<j
    W=pop(reps[:,None]^words[None,:])           # (2^(m-6),64)
    leicht=(W<=R).sum(axis=1)                   # = Zahl der Paare
    mn=int(leicht.min())
    hist[mn]=hist.get(mn,0)+1
    if mn>=2: best.append((mn,sorted(c&31 for c in cols)))
print(f"m={m} R={R} Proben={N} Modus={modus}: Verteilung des Minimums (Paare je Nebenklasse) {dict(sorted(hist.items()))}")
for b in best[:5]: print("  Kandidat:",b)
