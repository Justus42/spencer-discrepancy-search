# Vortest Orchestrator 25.11.2026: GF(2)-Rang und Kern der bekannten guten Codes
import sys, itertools
R="controls/"
def lade(f, cols=None):
    rows=[]
    for z in open(R+f):
        t=z.split()
        if not t: continue
        v=[int(x) for x in t]
        rows.append(v)
    # Nullspalten weg
    m=len(rows[0]); keep=[j for j in range(m) if any(r[j]!=0 for r in rows)]
    rows=[[r[j] for j in keep] for r in rows if any(x!=0 for x in r)]
    if any(0 in r for r in rows): return None,len(keep)
    return [sum(1<<j for j,x in enumerate(r) if x==-1) for r in rows], len(keep)
def rang(vs):
    b=[]
    for v in vs:
        for x in b: v=min(v,v^x)
        if v: b.append(v)
    return len(b)
for f in ["ctrl_8.txt","ctrl_9.txt","ctrl_16.txt","ctrl_17.txt","L2026_17x17.txt","astra_19.txt","bahn15_40w.txt","had24_1p3_paley12.txt","had24_2p2_paley12.txt"]:
    w,m=lade(f)
    if w is None: print(f,"Nullen in Zeilen, übersprungen"); continue
    full=(1<<m)-1
    C=set(w)|{x^full for x in w}
    kern=[t for t in C if all((c^t) in C for c in C)]
    # Kern bzgl. Translation: t mit C+t=C (t in C+C); teste alle t=c^c0
    c0=next(iter(C)); kern=[c^c0 for c in C if all((d^c^c0) in C for d in C)]
    print(f"{f}: m={m} Wörter={len(C)} affiner Rang={rang([c^c0 for c in C])} |Kern|={len(kern)}")
