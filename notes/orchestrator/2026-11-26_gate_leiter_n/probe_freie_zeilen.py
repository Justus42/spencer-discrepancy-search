#!/usr/bin/env python3
"""Probe des Gates (Orchestrator, keine Suche mit Anspruch): Zu einem ungeloesten Paar (U | W) gierig freie Zeilen ergaenzen, bis alle Punkte
ueberdeckt sind (Abstand <= R zu einem Wort oder Komplement). Kandidaten fuer eine freie Zeile: offene Punkte und Zufallsnachbarn.
Gierig ist nur eine obere Schranke fuer die noetige Zahl freier Zeilen. Aufruf: probe_freie_zeilen.py R PAAR..."""
import sys, numpy as np
R = int(sys.argv[1])
def pc(x):
    x = x - ((x >> 1) & 0x5555555555555555); x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333)
    return (((x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101 >> 56) & 0xFF
for fn in sys.argv[2:]:
    rows = [l for l in open(fn) if '|' in l and not l.startswith('#')]
    U = np.array([[int(t) for t in l.split('|')[0].split()] for l in rows]); W = np.array([[int(t) for t in l.split('|')[1].split()] for l in rows])
    Mx = np.vstack([np.hstack([U, W]), np.hstack([U, -W])]); m = Mx.shape[1]; full = (1 << m) - 1
    w = [int(sum(1 << j for j in range(m) if r[j] == -1)) for r in Mx]
    pts = np.arange(1 << m, dtype=np.int64); offen_m = np.ones(1 << m, dtype=bool)
    for c in w:
        d = pc(pts ^ c); offen_m &= (d > R) & (d < m - R)
    offen = pts[offen_m]; n0 = len(offen); extra = 0; rng = np.random.default_rng(1)
    while len(offen):
        cand = set()
        for p in offen[rng.choice(len(offen), size=min(48, len(offen)), replace=False)]:
            cand.add(int(p))
            for _ in range(150):
                mk = 0
                for j in rng.choice(m, size=rng.integers(1, R + 1), replace=False): mk |= 1 << int(j)
                cand.add(int(p) ^ mk)
        bc, bg = None, -1
        for c in cand:
            d = pc(offen ^ c); g = int(((d <= R) | (d >= m - R)).sum())
            if g > bg: bc, bg = c, g
        d = pc(offen ^ bc); offen = offen[~((d <= R) | (d >= m - R))]; extra += 1
        if extra == 1: erster = bg
    print("%s: %d Zeilen, offene Punkte %d; erste freie Zeile deckt %d davon; gierig %d freie Zeilen -> %d Woerter" % (fn.split('/')[-1], len(Mx), n0, erster if n0 else 0, extra, 2 * (len(Mx) + extra)))
