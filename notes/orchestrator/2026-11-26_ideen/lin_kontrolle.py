#!/usr/bin/env python3
"""lin_kontrolle.py -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
Lineare Codes der Dimension 6 mit Einsvektor = Sylvester 32 auf m ausgewaehlten Spalten (Punktmenge in AG(5,2)).
Stichprobe von 300 zufaelligen Punktmengen je m = 16, 18, 20 (Startwert 3): Wie oft ist disc >= 8 (64 Woerter, Radius (m-8)/2)?
Fuer die Treffer: Alleinanteil je Zeile (ist er > 0, sind alle 32 Zeilen noetig, kein Teilcode ueberdeckt).
Zweck: Positivkontrolle fuer Idee 1 (jeder Treffer ist eine Vereinigung von 8 Nebenklassen eines Kerns der Dimension 3).
Aufruf: lin_kontrolle.py ARBEITSORDNER   (dort liegen had_maske, had_allein, had32syl.txt aus lauf.sh)"""
import random, subprocess, sys, os
os.chdir(sys.argv[1]); random.seed(3)
for m in (16,18,20):
    masks=[]
    for _ in range(300):
        keep=random.sample(range(32),m); masks.append('%x'%sum(1<<j for j in range(32) if j not in keep))
    out=subprocess.run(['./had_maske','had32syl.txt','32','8']+masks,capture_output=True,text=True).stdout.strip().split('\n')
    tre=[masks[i] for i,l in enumerate(out) if int(l.split('flach ')[1].split()[0])==0]
    open('lin_m%d.out'%m,'w').write('mat 0 %d %s\n'%(len(tre),' '.join(tre)))
    r=subprocess.run(['./had_allein','had32syl.txt','32','8','lin_m%d.out'%m],capture_output=True,text=True).stdout.strip().split('\n')
    print('m=%d: disc >= 8 in %d von 300 Punktmengen; Beispiel (Maske der gestrichenen Spalten) %s'%(m,len(tre),tre[0]))
    print('      '+r[-1])
