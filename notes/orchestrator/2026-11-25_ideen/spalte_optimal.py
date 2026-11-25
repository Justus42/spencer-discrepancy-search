#!/usr/bin/env python3
"""Vortest zu Astras Vorschlag B (Orchestrator, 25.11.2026): (15,4), ctrl_16 ohne eine Zeile (15 x 15, 233 offene Klassen).
Eine ganze Spalte wird verdeckt und ALLE 2^15 Ergaenzungen dieser Spalte werden exakt bewertet (alle 15 Eintraege gemeinsam frei).
Fuer einen Basispunkt y der uebrigen 14 Koordinaten und Zeile i sei s_i = <a_i ohne Spalte, y> (gerade). |s_i| >= 8: beide Fortsetzungen gedeckt.
|s_i| = 6: nur die Fortsetzung x_j mit c_i * x_j * sign(s_i) = +1 ist gedeckt. Offene Punkte je y: [alle c_i*sign(s_i) = -1 auf E(y)] + [alle = +1].
Kontrolle: alle 16 Zeilen, verdeckte Spalte: Minimum 0 und die echte Spalte unter den Optima. Nur ganze Zahlen."""
import sys, itertools, collections, numpy as np
A=np.array([list(map(int,l.split())) for l in open(sys.argv[1]) if l.strip()])[:,:15]
Y=np.array(list(itertools.product((1,-1),repeat=14)),dtype=np.int64)
C=np.arange(1<<16,dtype=np.int64)
def beste(M,j):
    n=M.shape[0]; S=Y@np.delete(M,j,axis=1).T
    innen=(np.abs(S)>=8).any(axis=1); S=S[~innen]
    mp=((S==6)*(1<<np.arange(n))).sum(axis=1); mn=((S==-6)*(1<<np.arange(n))).sum(axis=1)
    cnt=collections.Counter(zip(mp.tolist(),mn.tolist()))
    c=np.arange(1<<n,dtype=np.int64); tot=np.zeros(1<<n,dtype=np.int64)       # Bit i von c gesetzt <=> c_i = -1
    for (p,q),k in cnt.items():
        tot+= k*(((c&p)==p)&((c&q)==0)).astype(np.int64) + k*(((c&p)==0)&((c&q)==q)).astype(np.int64)
    echt=int(sum(1<<i for i in range(n) if M[i,j]==-1))
    return int(tot.min())//2, int(tot[echt])//2, int((tot==tot.min()).sum()), len(cnt)
# Kontrolle
k=[beste(A,j) for j in (0,7,14)]; print("Kontrolle (16 Zeilen, Spalte 0, 7, 14 verdeckt): (Minimum, Wert der echten Spalte, Zahl der Optima, Muster):",k)
res=collections.Counter(); schlecht=[]
for r in range(16):
    M=np.delete(A,r,axis=0)
    for j in range(15):
        mi,echt,nopt,npat=beste(M,j); res[(mi,echt,nopt)]+=1
        if mi<233: schlecht.append((r,j,mi))
print("15 Zeilen (jede der 16 Zeilen gestrichen) x 15 Spalten = 240 Faelle: (Minimum Klassen, Wert der echten Spalte, Zahl der Optima): Anzahl")
for key,v in sorted(res.items()): print("  ",key,":",v)
print("Faelle mit Minimum unter 233:",schlecht if schlecht else "keine")
