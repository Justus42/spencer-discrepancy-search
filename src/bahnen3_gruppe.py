"""bahnen3_gruppe.py -- Pruefcode (Teil 1, 25.11.2026). Kein Suchcode, nur ganze Zahlen.

Gruppe G der vorzeichenbehafteten Spaltenpermutationen, die S = {Zeilen} u {-Zeilen} einer Matrix
erhalten: Elemente aus der eigenen Ruecksuche src/bahnen3_aut.c (ohne igraph; die Python-Ruecksuche war
mit 1,27 Mio. Knoten zu langsam), hier jedes Element voll nachgeprueft und die Gruppentafel auf Abschluss.
Spalte j -> (pi(j), s_j), Bild w[pi(j)] = s_j * v[j].

Danach: Elemente der Ordnung 3, eine 3-Sylowgruppe P (Ordnung 9), Bahnen von P auf den 2^19
Zeilenpaaren {x, ~x} (Bit j = 1 <=> Eintrag -1), Zyklentypen, Lage des 48er-Codes, Radius per
Breitensuche ueber alle 2^20 Punkte.

Aufruf: build/bahnen3_aut controls/had24_1p3_paley12.txt > runs/bahnen3/G_elemente.txt
        python3 src/bahnen3_gruppe.py controls/had24_1p3_paley12.txt runs/bahnen3/P_elemente.txt runs/bahnen3/G_elemente.txt
Ausgabedatei: je Zeile ein Element von P: 20 Zielspalten pi(0..19), dann 20 Vorzeichen-Bits (1 = Minus).
"""
import sys
from collections import Counter
import numpy as np


def lies(pfad):
    A = [[int(t) for t in z.split()] for z in open(pfad) if z.strip()]
    return A


def gruppe(A, pfad):
    """Elemente aus build/bahnen3_aut (eigene Ruecksuche in C) lesen und jedes voll nachpruefen."""
    n = len(A[0])
    S = sorted({tuple(z) for z in A} | {tuple(-e for e in z) for z in A})
    elemente = []
    for z in open(pfad):
        t = [int(x) for x in z.split()]
        if not t:
            continue
        p = tuple(t[:n]); s = tuple(-1 if b else 1 for b in t[n:2 * n])
        assert sorted(p) == list(range(n))
        elemente.append((p, s))
    Sset = set(S)
    for p, s in elemente:  # volle Nachpruefung
        for v in S:
            w = [0] * n
            for j in range(n):
                w[p[j]] = s[j] * v[j]
            assert tuple(w) in Sset
    assert len(set(elemente)) == len(elemente)
    return elemente, S


def mal(g, h):  # (g*h)(v) = g(h(v))
    pg, sg = g; ph, sh = h
    n = len(pg)
    p = tuple(pg[ph[j]] for j in range(n))
    s = tuple(sh[j] * sg[ph[j]] for j in range(n))
    return p, s


def ordnung(g):
    n = len(g[0]); e = (tuple(range(n)), (1,) * n)
    h = g; k = 1
    while h != e:
        h = mal(g, h); k += 1
    return k


def wirke_vek(g, v):
    p, s = g; w = [0] * len(v)
    for j in range(len(v)):
        w[p[j]] = s[j] * v[j]
    return tuple(w)


def zyklentyp(perm_abb, menge):
    gesehen = set(); typ = Counter()
    for x in menge:
        if x in gesehen:
            continue
        l = 0; y = x
        while y not in gesehen:
            gesehen.add(y); y = perm_abb(y); l += 1
        typ[l] += 1
    return dict(sorted(typ.items(), reverse=True))


def main():
    A = lies(sys.argv[1]); aus = sys.argv[2]
    m, n = len(A), len(A[0])
    G, S = gruppe(A, sys.argv[3])
    e = (tuple(range(n)), (1,) * n)
    print(f"Matrix {m} x {n}, |S| = {len(S)}, |G| = {len(G)} (Elemente aus build/bahnen3_aut, jedes nachgeprueft, Gruppentafel abgeschlossen)")
    Gs = set(G)
    for g in G:
        for h in G:
            assert mal(g, h) in Gs  # Abgeschlossenheit (volle Gruppentafel)
    ords = Counter(ordnung(g) for g in G)
    print("Elementordnungen:", dict(sorted(ords.items())))
    drei = [g for g in G if ordnung(g) == 3]
    # Zeilenpaare {v, -v}
    paare = sorted({min(tuple(z), tuple(-x for x in z)) for z in A})
    def paar_von(v):
        return min(v, tuple(-x for x in v))
    typen = Counter()
    for g in drei:
        sp = zyklentyp(lambda j, g=g: g[0][j], range(n))
        zp = zyklentyp(lambda v, g=g: paar_von(wirke_vek(g, v)), paare)
        typen[(str(sp), str(zp))] += 1
    print(f"{len(drei)} Elemente der Ordnung 3; Zyklentypen (Spalten | 24 Zeilenpaare):")
    for (a, b), c in typen.items():
        print(f"  {c} Elemente: Spalten {a} | Zeilenpaare {b}")
    # 3-Sylowgruppen: Untergruppen der Ordnung 9
    syl = set()
    for a in drei:
        for b in drei:
            if mal(a, b) != mal(b, a):
                continue
            U = {e}
            a2 = mal(a, a); b2 = mal(b, b)
            for x in (e, a, a2):
                for y in (e, b, b2):
                    U.add(mal(x, y))
            if len(U) == 9:
                syl.add(frozenset(U))
    print(f"Untergruppen der Ordnung 9: {len(syl)}")
    for U in syl:
        assert all(mal(x, y) in U for x in U for y in U)
        assert all(ordnung(x) in (1, 3) for x in U)
    P = sorted(min(syl, key=lambda U: sorted(U)))
    print("gewaehlte P: nicht zyklisch (alle 8 Nicht-Eins-Elemente Ordnung 3):", all(ordnung(x) == 3 for x in P if x != e))
    # Normalisator
    def inv(g):
        h = g
        while mal(g, h) != e:
            h = mal(g, h)
        return h
    Pset = set(P)
    N = [g for g in G if all(mal(mal(g, x), inv(g)) in Pset for x in P)]
    print(f"|N_G(P)| = {len(N)}")
    with open(aus, "w") as f:
        for p, s in P:
            f.write(" ".join(map(str, p)) + "  " + " ".join("1" if x < 0 else "0" for x in s) + "\n")
    with open(aus.replace(".txt", "_normalisator.txt"), "w") as f:
        for p, s in N:
            f.write(" ".join(map(str, p)) + "  " + " ".join("1" if x < 0 else "0" for x in s) + "\n")

    # Bahnen auf Paaren {x, ~x}, Bit j = 1 <=> Eintrag -1
    ALLE = (1 << n) - 1
    X = np.arange(1 << (n - 1), dtype=np.int64)
    def bild(g, X):
        p, s = g; Y = np.zeros_like(X)
        for j in range(n):
            b = (X >> j) & 1
            if s[j] < 0:
                b ^= 1
            Y |= b << p[j]
        return Y
    def kan(Y):
        return np.where(Y >> (n - 1) == 1, Y ^ ALLE, Y)
    bilder = np.stack([kan(bild(g, X)) for g in P])
    bid = bilder.min(axis=0)
    _, groesse = np.unique(bid, return_counts=True)
    # Bahngroesse = Zahl verschiedener Bilder
    s = np.sort(bilder, axis=0)
    gr = 1 + (s[1:] != s[:-1]).sum(axis=0)
    c = Counter(gr.tolist())
    nb = {k: v // k for k, v in c.items()}
    print(f"Bahnen von P auf 2^{n-1} Zeilenpaaren: {dict(sorted(nb.items()))}, Punktklassen gesamt {sum(nb.values())}")
    assert sum(nb.values()) == len(groesse)
    # 48er
    wort = lambda z: sum(1 << j for j in range(n) if z[j] < 0)
    W = [int(kan(np.int64(wort(z)))) for z in A]
    bahnen = Counter(int(bid[w]) for w in W)
    print(f"48er: {len(set(W))} verschiedene Zeilenpaare in {len(bahnen)} Bahnen, Bahngroessen {sorted(int(gr[b]) for b in bahnen)}, "
          f"Zeilen je Bahn {sorted(bahnen.values())}")
    # Abschluss: jede Bahn voll enthalten?
    alle_in = set()
    for b in bahnen:
        alle_in |= set(np.nonzero(bid == b)[0].tolist())
    print(f"48er = Vereinigung dieser Bahnen: {alle_in == set(W)}")
    # Radius per Breitensuche ueber alle 2^20 Punkte
    dist = np.full(1 << n, 99, dtype=np.int8)
    front = np.array(sorted(set(W) | {w ^ ALLE for w in W}), dtype=np.int64)
    dist[front] = 0
    d = 0
    while front.size:
        d += 1
        nb_ = np.unique(np.concatenate([front ^ (1 << j) for j in range(n)]))
        nb_ = nb_[dist[nb_] == 99]
        dist[nb_] = d
        front = nb_
    print(f"48er: Ueberdeckungsradius per Breitensuche ueber 2^{n} Punkte = {int(dist.max())}")
    # Zyklentyp des fuer Teil 4 gewaehlten Elements (erstes Nicht-Eins-Element von P)
    g = [x for x in P if x != e][0]
    print("Element fuer Teil 4 (erste Zeile != Eins in der Datei):", g)


if __name__ == "__main__":
    main()
