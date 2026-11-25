#!/usr/bin/env python3
"""Vortest Ideensammlung 25.11.2026 (Orchestrator). Nur ganze Zahlen/Brueche und math.comb.
Teil 1: Kappenanteil je Traegergroesse s bei Groesse N (Schwelle: kleinstes d > 5*sqrt(N/8) mit d = s mod 2).
Teil 2: je d die oberste Zelle (m = groesstes m <= B(d) mit m = d mod 2): erlaubte Dichte und
        'Stufen ueber der Energieschranke': e(m) = kleinste ganze Zahl >= sqrt(m) mit e = m mod 2, Stufen = (d - e)/2.
Teil 3: dieselbe Stufenzahl fuer die gesicherten Matrizen (Zeilen N, Spalten m, disc d)."""
from math import comb, isqrt
def V(m,r): return sum(comb(m,k) for k in range(r+1)) if r>=0 else 0
def dmin(N,par):            # kleinstes d mit 8 d^2 > 25 N und d = par mod 2
    d=1
    while 8*d*d<=25*N or d%2!=par%2: d+=1
    return d
def B(d):
    b=0
    while 25*(b+1)<8*d*d: b+=1
    return b
def e(m):
    r=isqrt(m)
    if r*r<m: r+=1
    if r%2!=m%2: r+=1
    return r
print("Teil 1: N, Schwelle 5*sqrt(N/8); je Traeger s: d, Radius, Kappenanteil (beide Vorzeichen), Dichte bei N Zeilen")
for N in range(13,26):
    out=[]
    for s in range(N,N-5,-1):
        d=dmin(N,s)
        if (s-d)<0: continue
        r=(s-d)//2
        frac=2*V(s,r)/2**s
        out.append(f"s={s}: d={d} r={r} Anteil={frac:.4f} Dichte={N*frac:.3f}")
    print(f"N={N} Schwelle={5*(N/8)**0.5:.3f} | "+" | ".join(out))
print()
print("Teil 2: d, B(d), oberste Zelle (m,R), Woerter 2B, Dichte, sqrt(m), e(m), Stufen")
for d in range(5,17):
    b=B(d); m=b if b%2==d%2 else b-1; R=(m-d)//2
    dens=2*b*V(m,R)/2**m
    print(f"d={d} B={b} Zelle=({m},{R}) Woerter={2*b} Dichte={dens:.3f} sqrt(m)={m**0.5:.3f} e(m)={e(m)} Stufen={(d-e(m))/2}")
print()
print("Teil 3: gesicherte Matrizen (Name, Zeilen, Spalten m, disc): e(m), Stufen, Quotient")
for name,N,m,d in [("ctrl_8",8,7,5),("Griego 6x6",6,6,4),("ctrl_9",9,9,5),("ctrl_16",16,15,7),("ctrl_17",17,17,7),("astra_19",19,19,7),
                   ("bahn15_40w",20,15,7),("had20 voll",20,20,6),("had20 ohne 1",20,19,7),("had24 voll",24,24,6),("had24 ohne 1",24,23,7),
                   ("48er",24,20,8),("64 bei (18,5)",32,18,8),("42 bei (13,3)",21,13,7)]:
    print(f"{name}: N={N} m={m} d={d} e(m)={e(m)} Stufen={(d-e(m))/2} Quotient={d/max(N,m)**0.5:.3f}")
