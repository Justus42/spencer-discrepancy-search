#!/usr/bin/env python3
"""Gate R9, Punkt 3: Positivkontrolle c) der Sitzung unabhängig nachgerechnet.
Die 16 Zeilen aus runs/spann_rang9/kontrolle_c_loesung.txt: liegen alle im Universum ctrl16_r9_y3 (Zeile 3 der
Problemdatei)? Überdeckungsradius der 32 Wörter (Zeilen und Komplemente) über alle 2^15 Punkte?"""
import numpy as np
m = 15; full = (1 << m) - 1
rows = [[int(x) for x in z.split()] for z in open("runs/spann_rang9/kontrolle_c_loesung.txt") if z.split()]
cw = [sum(1 << j for j in range(m) if r[j] == -1) for r in rows]
w = [int(x) for x in open("runs/spann/probleme/ctrl16_r9_y3.txt").read().splitlines()[2].split()]
uni = {min(v, v ^ full) for v in w}
print("Zeilen", len(rows), "Spalten", {len(r) for r in rows}, "verschiedene Paare", len({min(c, c ^ full) for c in cw}))
print("alle im Universum ctrl16_r9_y3 (256 Paare):", all(min(c, c ^ full) in uni for c in cw))
POP = np.array([bin(i).count("1") for i in range(1 << m)], dtype=np.int16)
x = np.arange(1 << m)
d = np.full(1 << m, 99, dtype=np.int16)
for c in cw:
    wt = POP[x ^ c]; d = np.minimum(d, np.minimum(wt, m - wt))
print("Überdeckungsradius:", int(d.max()), "; Punkte im Abstand 4:", int((d == 4).sum()), "; daraus disc >=", m - 2 * int(d.max()))
