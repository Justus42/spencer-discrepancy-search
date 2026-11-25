# Vortest Orchestrator 25.11.2026, Kandidat 2: Code = k Nebenklassen eines linearen Kerns D = <1,a,b> (8 Wörter), (20,6), k = 5.
# Typ = Größen (n1..n4) der vier Spaltenklassen nach (a_i,b_i). D wirkt durch Umklappen einer geraden Zahl von Klassen.
# B(D) = Zahl der Nebenklassen von D mit Minimalgewicht <= R. Zählschranke: k >= 2^(m-3) / B(D).
from itertools import product, combinations
from math import comb
import sys
m,R,k=int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3])
flips=[s for r in (0,2,4) for s in combinations(range(4),r)]
def parts(n,kmax,mx=None):
    if mx is None: mx=n
    if kmax==0:
        if n==0: yield ()
        return
    for a in range(min(n,mx),-1,-1):
        for r in parts(n-a,kmax-1,a): yield (a,)+r
aus=0; offen=[]; alle=0
for t in parts(m,4):
    if sum(1 for x in t if x>0)<3: continue
    alle+=1; tot=0
    for p in product(*[range(x+1) for x in t]):
        mn=min(sum((t[c]-p[c]) if c in f else p[c] for c in range(4)) for f in flips)
        if mn<=R:
            w=1
            for c in range(4): w*=comb(t[c],p[c])
            tot+=w
    assert tot%8==0
    B=tot//8; schr=2**(m-3)/B
    if schr>k: aus+=1
    else: offen.append((schr,t,B))
offen.sort()
print(f"(m,R)=({m},{R}), k={k}: {alle} Typen, durch Zählschranke ausgeschlossen {aus}, offen {len(offen)}")
for s,t,B in offen[:6]: print(f"  Typ {t}: B(D)={B}, Schranke {s:.3f}")
if offen: s,t,B=offen[-1]; print(f"  ... schlechtester offener Typ {t}: B(D)={B}, Schranke {s:.3f}")
