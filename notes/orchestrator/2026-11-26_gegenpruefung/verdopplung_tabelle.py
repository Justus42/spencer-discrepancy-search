#!/usr/bin/env python3
"""verdopplung_tabelle.py -- Gegenpruefung 26.11.2026 (Orchestrator). Geminis und Sols Einwand zu 3e: ueberbreite Faktoren.
K(m+1,R) <= 2 K(m,R): Ein selbstkomplementaerer Code mit hoechstens B(d) Woertern in der Zelle (m,R) mit m-2R = d-1 gibt durch Anhaengen
einer Spalte (Zeilen (u,+1),(u,-1)) einen Code mit hoechstens 2B(d) Woertern und Diskrepanz d. Welche dieser Zellen laesst die Tabelle offen?
Eingabe: non_mixed_covering_codes.csv (florath, Commit bbed9a6, SHA-256 5efaf8d9...). Aufruf: verdopplung_tabelle.py CSV"""
import csv, sys
T={}
for z in csv.DictReader(open(sys.argv[1],encoding='utf-8-sig')):
    if z['q']=='2': T[(int(z['n']),int(z['r']))]=(int(z['lower_bound']),int(z['upper_bound']))
for d,b in ((8,20),(9,25),(10,31),(11,38)):
    w=b-(b%2)
    for m in range(d-1,b):
        if (m-(d-1))%2: continue
        R=(m-(d-1))//2
        if R<1 or (m,R) not in T: continue
        lo,hi=T[(m,R)]
        if lo<=w: print("d=%d: Zelle (%d,%d) mit hoechstens %d Woertern, Tabelle %d bis %d: OFFEN"%(d,m,R,w,lo,hi))
