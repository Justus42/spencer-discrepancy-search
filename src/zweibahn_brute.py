#!/usr/bin/env python3
"""Pruefcode zu Auftrag Z: unabhaengige Brute-Force-Zaehlung fuer kleine Laengen (Test der Zaehlmaschine
in src/zweibahn_zaehl.c). Zaehlt alle Mengen aus k vollen Bahnen und kurzen Bahnen mit Gesamtgroesse <= B,
die {0,1}^m mit Radius R ueberdecken. Keine Klassen, kein Symmetrieabbau: Ueberdeckung als Bitmenge
ueber alle 2^m Punkte, Teilmengen kurzer Bahnen vollstaendig aufgezaehlt.
Aufruf: python3 src/zweibahn_brute.py q f R k B"""
import sys
from itertools import combinations
q, f, R, k, B = map(int, sys.argv[1:6]); m = 2*q+f; FULL = (1 << m)-1; M1 = (1 << q)-1
def rot(w):
    b1 = w & M1; b2 = (w >> q) & M1
    b1 = ((b1 << 1) | (b1 >> (q-1))) & M1; b2 = ((b2 << 1) | (b2 >> (q-1))) & M1
    return b1 | (b2 << q) | (w & (FULL ^ ((1 << 2*q)-1)))
seen = set(); orbs = []
for w in range(1 << m):
    if w in seen: continue
    o = set(); x = w
    for _ in range(q):
        o.add(x); o.add(x ^ FULL); x = rot(x)
    seen |= o; orbs.append(sorted(o))
def cov(words):
    c = 0
    for x in range(1 << m):
        if any(bin(x ^ w).count("1") <= R for w in words): c |= 1 << x
    return c
size = [len(o)//2 for o in orbs]
full = [cov(o) for o, s in zip(orbs, size) if s == q]
short = [(cov(o), s) for o, s in zip(orbs, size) if s < q and s <= B]
print(f"m={m} Bahnen {len(orbs)} voll {len(full)} kurz(erlaubt) {len(short)}")
ALL = (1 << (1 << m))-1
hist = [0]*(B+1)
for A in combinations(range(len(full)), k):
    base = 0
    for a in A: base |= full[a]
    # alle Teilmengen kurzer Bahnen mit Gewicht <= B (rekursiv)
    def rec(i, c, wsum):
        if i == len(short):
            if c == ALL: hist[wsum] += 1
            return
        rec(i+1, c, wsum)
        cv, s = short[i]
        if wsum+s <= B: rec(i+1, c | cv, wsum+s)
    rec(0, base, 0)
for t in range(B+1):
    if hist[t]: print(f"  kurz {t} Paare: {hist[t]}")
print("SUMME", sum(hist))
