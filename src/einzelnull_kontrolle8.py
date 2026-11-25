#!/usr/bin/env python3
"""Auftrag A, Teil 2: unabhängige Gegenrechnung der Positivkontrolle n = 8, d = 5 (Prüfcode, nur ganze Zahlen).

1. Zählt alle exakten Überdeckungen der 128 Klassen durch Zeilen mit genau einer Null (Algorithmus X, eigener Code).
2. Liest die Lösungsliste des Suchcodes (runs/einzelnull/teil2_loes8.txt): jede Lösung hat disc >= 5
   (min über x max |Ax| ganzzahlig), Mengen paarweise verschieden, Anzahl gleich.
3. Zählt Lösungen, die a = (0,1,...,1) und den Typvertreter enthalten und deren Paartypen alle <= t sind.
Aufruf: python3 src/einzelnull_kontrolle8.py
"""
import itertools
import sys

N, D = 8, 5
PUNKTE = [tuple([1] + [(-1 if (b >> k) & 1 else 1) for k in range(N - 1)]) for b in range(1 << (N - 1))]


def norm(a):
    s = next(v for v in a if v != 0)
    return tuple(v * s for v in a)


def zeilen():
    out = set()
    for z in range(N):
        for vz in itertools.product((1, -1), repeat=N - 1):
            a = list(vz)
            a.insert(z, 0)
            out.add(norm(a))
    return sorted(out)


def hits(a):
    return frozenset(i for i, x in enumerate(PUNKTE) if abs(sum(u * v for u, v in zip(a, x))) >= D)


def algo_x(Z, H):
    spalten = {p: set() for p in range(len(PUNKTE))}
    for i, h in enumerate(H):
        for p in h:
            spalten[p].add(i)
    loes = []

    def rek(offen, gewaehlt):
        if not offen:
            loes.append(frozenset(gewaehlt))
            return
        p = min(offen, key=lambda q: len(spalten_aktiv[q]))
        for i in list(spalten_aktiv[p]):
            h = H[i]
            # entferne alle Zeilen, die h schneiden
            entfernt = []
            for q in h:
                for j in list(spalten_aktiv[q]):
                    for q2 in H[j]:
                        if q2 in spalten_aktiv and j in spalten_aktiv[q2]:
                            spalten_aktiv[q2].discard(j)
                            entfernt.append((q2, j))
            gesp = {q: spalten_aktiv.pop(q) for q in h}
            rek(offen - h, gewaehlt + [i])
            for q, s in gesp.items():
                spalten_aktiv[q] = s
            for q2, j in entfernt:
                spalten_aktiv[q2].add(j)

    spalten_aktiv = {p: set(s) for p, s in spalten.items()}
    rek(frozenset(range(len(PUNKTE))), [])
    return loes


def disc(M):
    return min(max(abs(sum(u * v for u, v in zip(r, x))) for r in M) for x in PUNKTE)


def typ(a, b):
    za, zb = a.index(0), b.index(0)
    gem = [j for j in range(N) if a[j] and b[j]]
    dist = sum(1 for j in gem if a[j] != b[j])
    return (za == zb, min(dist, len(gem) - dist))


def main():
    Z = zeilen()
    H = [hits(a) for a in Z]
    print(f"Zeilen mit einer Null modulo Vorzeichen: {len(Z)}, Treffer je Zeile: {set(len(h) for h in H)}")
    L = algo_x(Z, H)
    verteilt = sum(1 for s in L if len(set(Z[i].index(0) for i in s)) > 1)
    print(f"Algorithmus X: {len(L)} exakte Ueberdeckungen, davon Nullen verteilt: {verteilt}")
    # Suchcode-Liste lesen
    such = []
    cur = []
    for line in open("runs/einzelnull/teil2_loes8.txt"):
        if line.startswith("#"):
            if cur:
                such.append(cur)
            cur = []
        elif line.strip():
            cur.append(tuple(int(v) for v in line.split()))
    if cur:
        such.append(cur)
    S_such = set(frozenset(norm(r) for r in M) for M in such)
    S_x = set(frozenset(Z[i] for i in s) for s in L)
    print(f"Suchcode-Liste: {len(such)} Loesungen, verschieden: {len(S_such)}, gleich der Algo-X-Menge: {S_such == S_x}")
    # disc >= 5 fuer alle (Stichprobe aller verteilten + aller einspaltigen waere langsam: alle 57600 mit Ganzzahlen)
    schlecht = 0
    for idx, M in enumerate(such):
        if idx % 50 == 0 or len(set(r.index(0) for r in M)) == 1:
            if disc(M) < D:
                schlecht += 1
    print(f"disc >= 5 geprueft (jede 50. und alle einspaltigen): Verstoesse {schlecht}")
    # Symmetriepfad
    a = tuple([0] + [1] * (N - 1))
    typen = sorted(set(typ(a, b) for b in Z if b != a))
    for t in typen:
        # Vertreter wie im Suchcode
        if t[0]:
            rep = [0] + [1] * (N - 1)
        else:
            rep = [1, 0] + [1] * (N - 2)
        for j in range(N - t[1], N):
            rep[j] = -1
        rep = norm(rep)
        assert typ(a, rep) == t
        Ht = None
        cnt = 0
        for s in S_x:
            if a in s and rep in s:
                def rang(u, v):  # Ordnung wie im Suchcode: (Schnitt, gleiche Spalte, Abstand)
                    tt = typ(u, v)
                    return (len(H[Z.index(u)] & H[Z.index(v)]), tt[0], tt[1])
                rt = rang(a, rep)
                ok = all(rang(u, v) <= rt for u, v in itertools.combinations(s, 2))
                if ok:
                    cnt += 1
        print(f"  Typ {t}: Loesungen mit a, Vertreter, alle Paarraenge <= Rang(a,Vertreter): {cnt}")


if __name__ == "__main__":
    sys.exit(main())
