#!/usr/bin/env python3
"""b2_teil0.py (Auftrag B2, Pruefcode, nur ganze Zahlen)

Teil 0: Kontrollzahlen zu had24_1p3_paley12 selbst nachrechnen.
Eingabe: Matrix (24 x 20, +-1) und Elementliste aus `b2_aequiv aut` (Spalte j -> pi_j, Faktor t_j).
Jedes Element wird hier unabhaengig nachgeprueft (bildet die 48 Woerter auf sich ab), Gruppe abgeschlossen,
Elementordnungen, Untergruppen der Ordnung 9, Bahnen von P auf allen 2^19 Paaren {w,-w},
Lage des 48ers, Ueberdeckungsradius per Abstand zu allen 48 Woertern ueber alle 2^20 Punkte.
Wortkonvention: Bit j = Spalte j, Bit 1 = Eintrag -1.
Aufruf: b2_teil0.py MATRIX ELEMENTE AUSGABE_P
"""
import sys
from collections import Counter
import numpy as np

N = 20
MASK = (1 << N) - 1


def lies_matrix(fn):
    rows = []
    for line in open(fn):
        t = line.split()
        if t:
            rows.append([int(x) for x in t])
    return rows


def wort(row):
    w = 0
    for j, x in enumerate(row):
        if x == -1:
            w |= 1 << j
        elif x != 1:
            raise ValueError("Eintrag nicht +-1")
    return w


def lies_elemente(fn):
    el = []
    for line in open(fn):
        if line.startswith('#') or not line.strip():
            continue
        a, b = line.split('|')
        pi = tuple(int(x) for x in a.split())
        t = tuple(int(x) for x in b.split())
        el.append((pi, t))
    return el


def wende_an(g, w):
    pi, t = g
    r = 0
    for j in range(N):
        b = (w >> j) & 1
        if t[j] == -1:
            b ^= 1
        r |= b << pi[j]
    return r


def verkette(g, h):
    """(g o h)(w) = g(h(w))."""
    pg, tg = g
    ph, th = h
    pi = [0] * N
    t = [0] * N
    for j in range(N):
        k = ph[j]
        pi[j] = pg[k]
        t[j] = th[j] * tg[k]
    return (tuple(pi), tuple(t))


EINS = (tuple(range(N)), (1,) * N)


def ordnung(g):
    h = g
    k = 1
    while h != EINS:
        h = verkette(g, h)
        k += 1
    return k


def bild_np(g, w):
    pi, t = g
    r = np.zeros_like(w)
    for j in range(N):
        b = (w >> j) & 1
        if t[j] == -1:
            b ^= 1
        r |= b << pi[j]
    return r


def main():
    mfile, efile, pout = sys.argv[1:4]
    rows = lies_matrix(mfile)
    W = set()
    for r in rows:
        w = wort(r)
        W.add(w)
        W.add(w ^ MASK)
    print("Woerter:", len(W))
    assert len(W) == 48
    G = lies_elemente(efile)
    print("Elemente gelesen:", len(G))
    Gs = set(G)
    assert len(Gs) == len(G), "doppelte Elemente"
    for g in G:
        assert sorted(g[0]) == list(range(N))
        assert {wende_an(g, w) for w in W} == W, "Element erhaelt die Wortmenge nicht"
    print("alle Elemente erhalten die 48 Woerter: ja")
    assert EINS in Gs
    for g in G:
        for h in G:
            assert verkette(g, h) in Gs
    print("abgeschlossen unter Verkettung (alle %d^2 Produkte): ja" % len(G))
    ords = Counter(ordnung(g) for g in G)
    print("Gruppenordnung:", len(G))
    print("Elementordnungen:", " ".join("%d:%d" % (k, ords[k]) for k in sorted(ords)))
    # Untergruppen der Ordnung 9: Elemente der Ordnung 1, 3, 9
    drei = [g for g in G if ordnung(g) in (3, 9)]
    print("Elemente der Ordnung 3 oder 9:", len(drei), "(davon Ordnung 9: %d)" % sum(1 for g in drei if ordnung(g) == 9))
    P = [EINS] + drei
    Ps = set(P)
    geschlossen = all(verkette(g, h) in Ps for g in P for h in P)
    print("{1} + diese Elemente: %d Elemente, abgeschlossen: %s" % (len(P), geschlossen))
    # Eindeutigkeit: jede Untergruppe der Ordnung 9 besteht aus Elementen der Ordnung 1, 3, 9,
    # liegt also in dieser Menge; hat die Menge genau 9 Elemente, ist sie die einzige.
    if len(P) == 9 and geschlossen:
        zykl = any(ordnung(g) == 9 for g in P)
        print("genau eine Untergruppe der Ordnung 9; zyklisch:", zykl)
    else:
        print("ABWEICHUNG: keine eindeutige Untergruppe der Ordnung 9")
        sys.exit(1)
    # P-Normalteiler?
    invd = {g: inv(g, G) for g in G}
    normal = all(verkette(verkette(g, p), invd[g]) in Ps for g in G for p in P)
    print("P Normalteiler in G:", normal)
    with open(pout, "w") as f:
        f.write("# P (Ordnung 9) zu %s; Spalte j -> pi_j, Faktor t_j\n" % mfile)
        for g in sorted(P):
            f.write(" ".join(map(str, g[0])) + " | " + " ".join(map(str, g[1])) + "\n")
    # Bahnen auf den 2^19 Paaren {w,-w}
    w = np.arange(1 << N, dtype=np.int64)
    kan = np.minimum(w, w ^ MASK)
    omin = kan.copy()
    for g in P:
        b = bild_np(g, w)
        omin = np.minimum(omin, np.minimum(b, b ^ MASK))
    klassen = w[w == kan]                  # Vertreter der Paare (Bit 19 = 0)
    print("Paare:", len(klassen))
    om = omin[klassen]
    werte, groesse = np.unique(om, return_counts=True)
    vert = Counter(groesse.tolist())
    print("Bahnen nach Groesse:", " ".join("%d:%d" % (k, vert[k]) for k in sorted(vert)), " zusammen", len(werte))
    # Lage des 48ers
    zeilen = sorted({min(x, x ^ MASK) for x in W})
    bahnen = Counter(int(omin[x]) for x in zeilen)
    gr = dict(zip(werte.tolist(), groesse.tolist()))
    voll = all(bahnen[b] == gr[b] for b in bahnen)
    print("48er: %d Zeilenpaare in %d Bahnen, Groessen %s, Bahnen ganz enthalten: %s"
          % (len(zeilen), len(bahnen), sorted(gr[b] for b in bahnen), voll))
    # Radius ueber alle 2^20 Punkte
    dmin = np.full(1 << N, 99, dtype=np.int64)
    for x in W:
        d = np.bitwise_count(w ^ x).astype(np.int64)
        dmin = np.minimum(dmin, d)
    print("Ueberdeckungsradius (max min Abstand ueber alle 2^20 Punkte):", int(dmin.max()))


def inv(g, G):
    for h in G:
        if verkette(g, h) == EINS:
            return h
    raise ValueError


if __name__ == "__main__":
    main()
