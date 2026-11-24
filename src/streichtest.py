"""Streichtest (Pruefcode, kein Suchcode): Ist eine Nebenklasse ueberfluessig?
Eingabe: n R d typ woerter.txt. Zerlegt die Woerter in Nebenklassen von D (D aus coset_ladder.build, wie bei der Suche),
streicht je eine Nebenklasse und zaehlt per Brute-Force ueber alle 2^n Punkte, wie viele Punkte danach Abstand > R haben.
Ueberfluessig = 0 Punkte offen. Aufruf: streichtest.py n R d typ woerter.txt   (typ z. B. 11,7)"""
import sys, os
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from coset_ladder import build
n, R, d = map(int, sys.argv[1:4]); mult = tuple(map(int, sys.argv[4].split(",")))
W = sorted(map(int, open(sys.argv[5]).read().split()))
G, Hc, m, hp, rr = build(n, mult, d)
D = [0]
for g in G: D += [x ^ g for x in D]
assert len(set(D)) == 1 << d
seen, cos = set(), []
for w in W:
    if w in seen: continue
    c = sorted(w ^ x for x in D); assert all(x in set(W) for x in c), "Woerter sind keine Vereinigung von Nebenklassen von D"
    seen |= set(c); cos.append(c)
X = np.arange(1 << n, dtype=np.uint32)
dist = [np.bitwise_count(X ^ np.uint32(w)).astype(np.uint8) for w in W]
idx = {w: i for i, w in enumerate(W)}
def offen(skip):
    dm = np.full(1 << n, 255, dtype=np.uint8)
    for w in W:
        if w not in skip: np.minimum(dm, dist[idx[w]], out=dm)
    return int((dm > R).sum())
print(f"{len(W)} Woerter, {len(cos)} Nebenklassen, ohne Streichung offen: {offen(set())}")
res = [offen(set(c)) for c in cos]
print("offene Punkte je gestrichener Nebenklasse:", res)
print("ueberfluessige Nebenklassen:", sum(1 for r in res if r == 0))
