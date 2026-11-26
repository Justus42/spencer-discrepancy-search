#!/usr/bin/env python3
# Auftrag D: Abgleich Restzahl der Suche (zerl_halb, Zeile "seed S: ... beste Restzahl R" bzw. LOESUNG) mit der Nachzaehlung
# durch zerl_offen.py (Zeile "...paar.txt: K=.., Typ (..), d=..: offene Klassen N von M") in den Blocklogs runs/leiter_d10/*_k*_t*.log.txt.
import re, glob, sys
n = ab = loes = 0
for f in sorted(glob.glob('runs/leiter_d10/*_k[0-9]*_t[0-9]*-[0-9]*.log.txt')):
    if '_seed' in f or 'abgebrochen' in f: continue
    suche = None
    for z in open(f):
        m = re.match(r'(\S+) seed (\d+): (LOESUNG|ZEITGRENZE)(.*)', z)
        if m:
            r = re.search(r'beste Restzahl (\d+)', m.group(4))
            suche = (m.group(1), m.group(2), 0 if m.group(3) == 'LOESUNG' else int(r.group(1)))
            continue
        m = re.search(r'offene Klassen (\d+) von (\d+)', z)
        if m and suche:
            n += 1; loes += suche[2] == 0
            if int(m.group(1)) != suche[2]:
                ab += 1; print('ABWEICHUNG', f, suche, m.group(1))
            suche = None
print(f'Laeufe {n}, davon gel. {loes}, Abweichungen {ab}')
