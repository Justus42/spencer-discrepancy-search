# Gate zu Auftrag S (Orchestrator 25.11.2026): Automorphismengruppe von V (Spaltenpermutationen mit pi(V) = V) über bliss (igraph)
# und ihre Bahnen auf den 2^(m-r) - 1 Erweiterungsrichtungen (Nebenklassen von V außer V selbst). Unabhängig von src/spann_aut.c.
import sys, igraph
f=sys.argv[1]
rows=[[int(x) for x in z.split()] for z in open(f) if z.split()]
m0=len(rows[0]); keep=[j for j in range(m0) if any(r[j]!=0 for r in rows)]; m=len(keep)   # Nullspalten streichen
full=(1<<m)-1; w=[sum(1<<i for i,j in enumerate(keep) if r[j]==-1) for r in rows]; c0=w[0]
def ech(vs):
    B=[]
    for v in vs:
        for b in B: v=min(v,v^b)
        if v: B.append(v); B.sort(reverse=True)
    # voll reduzieren
    for i in range(len(B)):
        for j in range(len(B)):
            if i!=j and (B[i]>>(B[j].bit_length()-1))&1: B[i]^=B[j]
    return sorted(B,reverse=True)
B=ech([c^c0 for c in w]+[full]); r=len(B)
def red(v):
    for b in B: v=min(v,v^b)
    return v
V=[0]
for b in B: V=V+[x^b for x in V]
woerter=[v for v in V if v]
g=igraph.Graph(); g.add_vertices(m+len(woerter)); g.add_edges([(j,m+i) for i,v in enumerate(woerter) for j in range(m) if (v>>j)&1])
farbe=[0]*m+[1]*len(woerter)
ordnung=g.count_automorphisms(color=farbe); gens=g.automorphism_group(color=farbe)
perms=[p[:m] for p in gens]
def wende(p,v): return sum(1<<p[j] for j in range(m) if (v>>j)&1)
assert all(red(wende(p,b))==0 for p in perms for b in B)
reps=sorted(set(red(y) for y in range(1<<m))-{0})
eltern={y:y for y in reps}
def find(a):
    while eltern[a]!=a: eltern[a]=eltern[eltern[a]]; a=eltern[a]
    return a
for p in perms:
    for y in reps:
        a,b=find(y),find(red(wende(p,y)))
        if a!=b: eltern[a]=b
bahnen={}
for y in reps: bahnen.setdefault(find(y),[]).append(y)
print(f"{f}: Rang {r}, |Aut(V)| = {ordnung}, {len(perms)} Erzeuger; Erweiterungsrichtungen {len(reps)}, Bahnen (Größe: kleinster Vertreter): "+", ".join(f"{len(v)}: y={min(v)}" for v in sorted(bahnen.values(),key=len)))
for ys in sys.argv[2:]:
    y=int(ys); print(f"   Vertreter der Sitzung y={y}: Bahn der Größe {len(bahnen[find(red(y))])}")
