#!/usr/bin/env python3
"""Prüfcode (Auftrag G): Lassen die benutzten Symmetrien die exakte Diskrepanz gleich? Zufällige f und zufällige Elemente der
Symmetriegruppe H (und Komplement), exakte Diskrepanz beider Matrizen über build/exakt_disc. Zusätzlich: f(x) -> f(x^-1) auf den
nichtabelschen Gruppen (die Transponierte), um zu zeigen, dass sie NICHT erlaubt ist (Zählung der Abweichungen)."""
import os, random, sys
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "..", "..", "src"))
import gruppen_pruef as gp
ROOT = os.path.join(os.path.dirname(__file__), "..", "..", "..")
random.seed(25112026)
tmp = gp.TMPDATEI
for name, anzahl in (("d9", 60), ("s3xz3", 60), ("z3z3_z2", 60), ("f20", 60), ("dic5", 60), ("f21", 60), ("z3xz6", 30)):
    n, tab, inv = gp.lies_tafel(os.path.join(ROOT, "runs", "gruppen", "tafeln", name + ".grp"))
    H = list(gp.symmetriegruppe(n, tab))
    maske = (1 << n) - 1
    bad = 0; transp_abw = 0
    for _ in range(anzahl):
        f = random.getrandbits(n)
        s = random.choice(H)
        g = 0
        for x in range(n):                       # g(x) = f(s(x))
            if (f >> s[x]) & 1:
                g |= 1 << x
        if random.random() < 0.5:
            g ^= maske
        d1 = gp.exakt_matrix(gp.matrix(f, n, tab, inv, False), tmp)
        d2 = gp.exakt_matrix(gp.matrix(g, n, tab, inv, False), tmp)
        bad += (d1 != d2)
        t = 0                                     # f(x^-1)
        for x in range(n):
            if (f >> inv[x]) & 1:
                t |= 1 << x
        transp_abw += (gp.exakt_matrix(gp.matrix(t, n, tab, inv, False), tmp) != d1)
    print(f"{name}: {anzahl} Paare (f, Symmetrie), Abweichungen der exakten Diskrepanz: {bad}; f(x^-1) weicht ab in {transp_abw} von {anzahl}")
