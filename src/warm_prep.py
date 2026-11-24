"""Warmstart vorbereiten: Woerter (Laenge 20, Typ (11,9), d=2) -> Syndrome der Nebenklassen (gleiche Matrix H wie
coset_ladder.build). Fuer jede Nebenklasse: wie viele Syndrome waeren ohne sie unueberdeckt? Gestrichen wird die
mit der kleinsten Zahl; die uebrigen Syndrome gehen in die Startdatei.
Aufruf: warm_prep.py woerter.txt startdatei.txt"""
import sys, os
import numpy as np
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from coset_ladder import build
n, R, mult = 20, 6, (11, 9)
G, Hc, m, hp, rr = build(n, mult, 2)
W = list(map(int, open(sys.argv[1]).read().split()))
def syn(w):
    s = 0
    for i in range(n):
        if (w >> i) & 1: s ^= Hc[i]
    return s
from collections import Counter
cnt = Counter(syn(w) for w in W); C = sorted(cnt)
assert all(v == 4 for v in cnt.values()), "nicht 4 Woerter je Nebenklasse"
# Bild der Hamming-Kugel im Quotienten
S = {0}
for _ in range(R): S |= {x ^ h for x in S for h in Hc}
S = np.array(sorted(S), dtype=np.uint32); N = 1 << m
cov = np.zeros(N, dtype=np.int16)
for c in C: cov[np.uint32(c) ^ S] += 1
assert (cov > 0).all(), "keine volle Ueberdeckung"
loss = [int(((cov[np.uint32(c) ^ S]) == 1).sum()) for c in C]
j = int(np.argmin(loss))
rest = [c for k, c in enumerate(C) if k != j]
open(sys.argv[2], "w").write(" ".join(map(str, rest)) + "\n")
print(f"{sys.argv[1]}: {len(C)} Nebenklassen, Verlust je Streichung min/max {min(loss)}/{max(loss)}; gestrichen Syndrom {C[j]} -> "
      f"{loss[j]} unueberdeckt; Startdatei {sys.argv[2]} mit {len(rest)} Mitten")
