# Gate zu Auftrag S (Orchestrator 25.11.2026): eigenes Problem-Export, unabhängig von src/spann_zaehl.py.
# Aufruf: muster.py MATRIX R [v ...] > problem.txt
# G = V/<1> mit Basis b_1..b_q (q = r-1). Je Nebenklasse D von V: Muster A_D = { t in G : Gewicht(rep_D + c0 + t.b) <= R oder >= m-R }.
# Paar s überdeckt Punkt (D,g) genau dann, wenn g + s in A_D. Ausgabe: q, Zahl der Nebenklassen, je Nebenklasse das Muster als Liste.
import sys
f=sys.argv[1]; R=int(sys.argv[2]); extra=[int(x) for x in sys.argv[3:]]
rows=[[int(x) for x in z.split()] for z in open(f) if z.split()]
m0=len(rows[0]); keep=[j for j in range(m0) if any(r[j]!=0 for r in rows)]; m=len(keep); full=(1<<m)-1
w=[sum(1<<i for i,j in enumerate(keep) if r[j]==-1) for r in rows]; c0=w[0]
def red(v,B):
    for b in B:
        v=min(v,v^b)
    return v
B=[full]                                   # Basis von V, Einsvektor zuerst
for v in [c^c0 for c in w]+extra:
    v=red(v,sorted(B,reverse=True))
    if v: B.append(v)
    B=sorted(B,reverse=True)
# saubere reduzierte Basis (Pivot = höchstes Bit, gegenseitig reduziert)
RB=[]
for b in sorted(B,reverse=True):
    for x in RB: b=min(b,b^x)
    if b: RB.append(b)
for i in range(len(RB)):
    for j in range(len(RB)):
        if i!=j and (RB[i]>>(RB[j].bit_length()-1))&1: RB[i]^=RB[j]
r=len(RB); piv=[b.bit_length()-1 for b in RB]; assert len(set(piv))==r and red(full,sorted(RB,reverse=True))==0
# Basis von G: V-Basis ohne ein Element, so dass zusammen mit dem Einsvektor ganz V aufgespannt wird
gb=None
for weg in range(r):
    cand=[RB[i] for i in range(r) if i!=weg]
    T=[]
    for v in cand+[full]:
        v=red(v,sorted(T,reverse=True))
        if v: T.append(v)
    if len(T)==r: gb=cand; break
q=r-1; rest=[j for j in range(m) if j not in piv]
span=[0]
for b in gb: span=span+[x^b for x in span]          # Index t -> t.b
print(q,1<<(m-r),m,R)
mus=[]
for u in range(1<<(m-r)):
    rep=sum(((u>>i)&1)<<j for i,j in enumerate(rest))
    A=[t for t in range(1<<q) if (lambda g: g<=R or g>=m-R)(bin(rep^span[t]).count("1"))]   # rep + c0 + c0-Verschiebung hebt sich: Punkt x = rep + c0 + g.b
    mus.append(len(A)); print(len(A),*A)
import collections
print("# mu:",dict(sorted(collections.Counter(mus).items())),file=sys.stderr)
