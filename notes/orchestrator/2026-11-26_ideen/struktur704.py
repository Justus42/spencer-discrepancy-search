#!/usr/bin/env python3
"""struktur704.py -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
Struktur der Endzustaende der Evolutionsschleife (Auftrag E): Spalten-Gram (R^T R) und Zeilen-Gram (R R^T) der 23 x 20-Matrizen mit 704
offenen Klassen und der 24 x 20-Treffer, im Vergleich zu den 48ern (Hadamard 24 ohne 4 Spalten: Spalten orthogonal).
Aufruf: struktur704.py REPO"""
import sys, os, csv, collections
import numpy as np
repo=sys.argv[1]
def lade(fn):
    return np.array([[int(v) for v in l.split()] for l in open(fn) if l.strip() and l.split()[0].lstrip('-').isdigit()])
def beschreibe(R):
    n,m=R.shape; Gc=R.T@R; Gr=R@R.T
    oc=collections.Counter(abs(int(v)) for i in range(m) for v in Gc[i,i+1:]); orr=collections.Counter(abs(int(v)) for i in range(n) for v in Gr[i,i+1:])
    ev=np.linalg.eigvalsh(Gc.astype(float))
    return "%dx%d | Spalten |<.,.>| %s | Zeilen |<.,.>| %s | Eigenwerte Spalten-Gram min %.2f max %.2f"%(n,m,dict(sorted(oc.items())),dict(sorted(orr.items())),ev[0],ev[-1])
print("48er 1+3:", beschreibe(lade(os.path.join(repo,'controls/had24_1p3_paley12.txt'))))
rows=list(csv.DictReader(open(os.path.join(repo,'runs/evo/archiv.tsv')),delimiter='\t'))
gesehen=collections.Counter()
for r in rows:
    if r['m']!='20': continue
    fn=os.path.join(repo,'runs/evo/out','r%03d_%s_m20R6k%s_s%s.txt'%(int(r['Runde']),r['Programm'],r['Paare'],r['Startwert']))
    if not os.path.exists(fn): continue
    if not ((r['Paare']=='23' and r['offene_Klassen'] in ('704',)) or (r['Paare']=='24' and r['offene_Klassen']=='0')): continue
    b=beschreibe(lade(fn)); gesehen[(r['Paare'],r['offene_Klassen'],b)]+=1
for (p,o,b),k in sorted(gesehen.items()): print("Paare %s, offen %s, %d Laeufe: %s"%(p,o,k,b))
