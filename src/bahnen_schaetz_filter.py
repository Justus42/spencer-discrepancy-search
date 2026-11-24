# Teil D (24.11.2026), nur Schaetzung: Wirkung eines Filters ueber die 8 festen Paare (n=20, R=6, K=4).
# Teil D Zusatz: Filter ueber feste Paare. Jede volle Bahn bekommt eine 8-Bit-Maske der festen Paare, die sie (Radius 6) ueberdeckt.
import numpy as np, itertools
n=20; N=1<<n; R=6
x=np.arange(N,dtype=np.int64)
pc=np.zeros(N,dtype=np.int8)
for i in range(n): pc+=((x>>i)&1).astype(np.int8)
fest=[sum(31<<(5*j) for j in range(4) if (s>>j)&1) for s in range(8)]  # Paar s: {fest[s], ~fest[s]}, s<8 mit Bit3=0 als Vertreter
paare=[f for f in fest if f < (N-1)^f]
mask=np.zeros(N,dtype=np.int64)
for k,f in enumerate(paare):
    d=pc[x^f]; mask|=(((d<=R)|(d>=n-R)).astype(np.int64))<<k
# Bahn-Kanon: min ueber sigma^j und Komplement
def rot(v):
    y=np.zeros_like(v)
    for b in range(4):
        blk=(v>>(5*b))&31; y|=(((blk<<1)|(blk>>4))&31)<<(5*b)
    return y
c=x.copy(); v=x.copy()
for j in range(5):
    c=np.minimum(c,np.minimum(v,v^(N-1))); v=rot(v)
fix=(rot(x)==x)
reps=np.unique(c[~fix])
m=mask[reps]  # Maske ist bahninvariant (Paare fest unter G)
assert len(reps)==104856
cnt=np.bincount(m,minlength=256)
print("volle Bahnen:",len(reps),"; Bahnen je Paar:",[int((m>>k&1).sum()) for k in range(8)])
print("Maskengewichte (Zahl ueberdeckter Paare) -> Bahnen:",{w:int(sum(cnt[t] for t in range(256) if bin(t).count('1')==w)) for w in range(9)})
# Ebene 1: a deckt Paar 0; Ebene 2: b deckt erstes von a nicht gedecktes Paar. Anteil der (a,b), fuer die es c,d gibt mit Gesamtmaske 255
T=[t for t in range(256) if cnt[t]]
paar2=set(s|t for s in T for t in T)
geht=lambda u: any((u|v)==255 for v in paar2)
tot=ok=0; blatt=0; blatt_ok=0
for ta in T:
    if not ta&1: continue
    p1=next(k for k in range(8) if not (ta>>k)&1) if ta!=255 else None
    for tb in T:
        if p1 is not None and not (tb>>p1)&1: continue
        w=cnt[ta]*cnt[tb]; tot+=w
        u=ta|tb
        if geht(u): ok+=w
print(f"(a,b)-Paare (Ebene 2, ohne Symmetrie): {tot:.3e}, davon mit Paar-Filter zulaessig: {ok:.3e} ({ok/tot:.4%})")
# Ebene 3: c deckt erstes von a|b nicht gedecktes Paar; Blatt nur noetig, wenn ein d die restlichen Paare deckt
einz=set(T)
tot3=ok3=0
for ta in T:
    if not ta&1: continue
    for tb in T:
        u=ta|tb
        p1=next((k for k in range(8) if not (ta>>k)&1),None)
        if p1 is not None and not (tb>>p1)&1: continue
        p2=next((k for k in range(8) if not (u>>k)&1),None)
        for tc in T:
            if p2 is not None and not (tc>>p2)&1: continue
            w=cnt[ta]*cnt[tb]*cnt[tc]; tot3+=w
            if any((u|tc|v)==255 for v in einz): ok3+=w
print(f"(a,b,c)-Blaetter ohne Symmetrie: {tot3:.3e}, davon mit Paar-Filter noetig: {ok3:.3e} ({ok3/tot3:.4%})")
