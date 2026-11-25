#!/usr/bin/env python3
"""Prüfcode (Auftrag G, Teil 3): Kontrolle der Spaltenstreichung bei gesenktem d. Für eine Gruppe und d (hier d = 6, n = 18)
werden alle Klassen mit u >= d-2 mit allen Spaltenpaaren exakt gerechnet (build/exakt_disc) und mit den Überlebenden von
`gruppenmatrix -S 2` verglichen: Jede (Klasse, Paar) mit exakt >= d MUSS Überlebender sein (der Filter findet nur echte
Gegenbeispiele). Außerdem: exakt (gestrichen) <= exakt (voll) + 2 für jede geprüfte Zeile."""
import os, subprocess, sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..", "src"))
import gruppen_pruef as gp
ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..")
name, d = sys.argv[1], int(sys.argv[2])
grp = os.path.join(ROOT, "runs", "gruppen", "tafeln", name + ".grp")
pre = os.path.join(ROOT, "runs", "gruppen", "kontrolle", f"t3_{name}_d{d}")
subprocess.run([os.path.join(ROOT, "build", "gruppenmatrix"), grp, str(d), "-S", "2", "-a", "-o", pre], check=True, capture_output=True)
n, tab, inv = gp.lies_tafel(grp)
klassen = [l.split() for l in open(pre + "_alle.txt") if l.strip()]
ueb = {(int(a, 16), int(i), int(j)) for a, i, j, u in (l.split() for l in open(pre + "_ueberlebende.txt") if l.strip())}
tmp = gp.TMPDATEI
zu_pruefen = [(int(a, 16), int(u)) for a, u in klassen if int(u) >= d - 2]
wahr_ueb = 0; falsch_neg = 0; gross = 0; paare = 0; maxd = 0
for f, u in zu_pruefen:
    dv = gp.exakt_matrix(gp.matrix(f, n, tab, inv, False), tmp)
    for i in range(n):
        for j in range(i + 1, n):
            dd = gp.exakt_matrix(gp.matrix(f, n, tab, inv, False, (i, j)), tmp)
            paare += 1
            maxd = max(maxd, dd)
            if dd > dv + 2:
                gross += 1
            if dd >= d:
                wahr_ueb += 1
                if (f, i, j) not in ueb:
                    falsch_neg += 1
print(f"{name} d={d}: Klassen mit u >= d-2: {len(zu_pruefen)}, Paare {paare}, exakt >= d: {wahr_ueb}, Überlebende des Filters: {len(ueb)}, "
      f"echte Treffer unter den Nicht-Überlebenden (VERLETZT): {falsch_neg}, exakt(gestrichen) > exakt(voll)+2 (VERLETZT): {gross}, größtes exakt {maxd}")
sys.exit(1 if falsch_neg or gross else 0)
