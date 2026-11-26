#!/usr/bin/env python3
"""sylvester_kontrolle.py -- Pruefcode Auftrag K, Schritt 0 d) (teilt keinen Code mit src/kern3_such.c).

Sylvester 32 (Zeile u, Spalte j: (-1)^popcount(u&j)) auf den 16 Spalten 0 1 3 4 6 7 10 12 15 16 17 18 22 23 25 28.
1. Matrix 32 x 16 nach runs/kern3/sylvester32_16sp.matrix.txt (Bit 1 = Eintrag -1).
2. Zeilen plus Komplemente: 64 verschiedene Woerter, linear (abgeschlossen unter xor), Ueberdeckungsradius 4 (alle 2^16 Punkte).
3. Fuer (u1,u2) in (2,9) (1,2) (2,4) (1,8) (6,8): a = Zeile u1, b = Zeile u2; Typ aus den Bloecken (a_j,b_j);
   Vertreter der 8 Nebenklassen von <1,a,b> (u modulo <u1,u2>). Pruefen: Vereinigung = die 64 Woerter.
4. Umbenennung in die Normalform des Suchcodes (Blockgroessen absteigend, Label (a_j,b_j) = (i&1, i>>1), Bloecke
   hintereinander): jede Bijektion der vier Label ist affin (AGL(2,2) = S4), also bleibt D = <1,a,b> als Menge gleich
   (wird nachgeprueft). Spalten werden umgeordnet; die Vertreter in Normalform gehen nach
   runs/kern3/sylvester_<u1>-<u2>.vertreter.txt (Zeilen 'VERTRETER ...', Eingabe fuer ueberdeckung.py treffer).
"""
import sys
from pathlib import Path
import numpy as np

WURZEL = Path(__file__).resolve().parent.parent.parent
SPALTEN = [0, 1, 3, 4, 6, 7, 10, 12, 15, 16, 17, 18, 22, 23, 25, 28]
PAARE = [((2, 9), (4, 4, 4, 4)), ((1, 2), (5, 4, 4, 3)), ((2, 4), (5, 5, 3, 3)), ((1, 8), (6, 5, 3, 2)), ((6, 8), (8, 3, 3, 2))]
m = 16


def zeile(u):
    return [bin(u & j).count("1") & 1 for j in SPALTEN]   # Bit 1 = Eintrag -1


def als_int(bits):
    return sum(bit << j for j, bit in enumerate(bits))


def radius(worte):
    pc = np.array([bin(i).count("1") for i in range(1 << 16)], dtype=np.int32)
    x = np.arange(1 << 16)
    dmin = np.full(x.shape, 99)
    for w in worte:
        dmin = np.minimum(dmin, pc[x ^ w])
    return int(dmin.max())


def main():
    fehler = 0
    out = WURZEL / "runs" / "kern3"
    out.mkdir(parents=True, exist_ok=True)
    with open(out / "sylvester32_16sp.matrix.txt", "w") as f:
        for u in range(32):
            f.write(" ".join("-1" if bit else "1" for bit in zeile(u)) + "\n")
    eins = (1 << m) - 1
    rows = [als_int(zeile(u)) for u in range(32)]
    worte = set(rows) | set(r ^ eins for r in rows)
    lin = all((x ^ y) in worte for x in worte for y in worte)
    rad = radius(sorted(worte))
    print("Woerter %d, linear %s, Radius %d" % (len(worte), lin, rad))
    if not (len(worte) == 64 and lin and rad == 4):
        fehler += 1
    for (u1, u2), soll in PAARE:
        a, b = rows[u1], rows[u2]
        label = [((a >> j) & 1) | (((b >> j) & 1) << 1) for j in range(m)]
        groesse = [label.count(l) for l in range(4)]
        typ = tuple(sorted(groesse, reverse=True))
        D = {0, a, b, a ^ b}
        D = D | {d ^ eins for d in D}
        # Vertreter: u modulo <u1,u2>
        gesehen, vert = set(), []
        for u in range(32):
            kl = frozenset({u, u ^ u1, u ^ u2, u ^ u1 ^ u2})
            if kl in gesehen:
                continue
            gesehen.add(kl)
            vert.append(rows[u])
        vereinigung = {c ^ d for c in vert for d in D}
        ok_ver = (len(vert) == 8 and vereinigung == worte)
        # Umbenennung: Label l -> neues Label lam[l], Groessen absteigend in neuer Labelreihenfolge
        alte = sorted(range(4), key=lambda l: (-groesse[l], l))
        lam = {alte[i]: i for i in range(4)}
        neu_label = [lam[l] for l in label]
        a2 = sum((nl & 1) << j for j, nl in enumerate(neu_label))
        b2 = sum(((nl >> 1) & 1) << j for j, nl in enumerate(neu_label))
        D2 = {0, a2, b2, a2 ^ b2}
        D2 = D2 | {d ^ eins for d in D2}
        ok_D = (D2 == D)
        # Spaltenreihenfolge: neues Label 0, 1, 2, 3, darin aufsteigend
        reihe = sorted(range(m), key=lambda j: (neu_label[j], j))
        def perm(x):
            return "".join(str((x >> reihe[k]) & 1) for k in range(m))
        a_neu = perm(a2)
        n_neu = tuple(neu_label.count(i) for i in range(4))
        # a2 muss in der Normalform die Bloecke 1 und 3 sein
        erwart_a = "".join("1" if (i & 1) else "0" for i in range(4) for _ in range(n_neu[i]))
        ok_norm = (a_neu == erwart_a) and n_neu == typ
        with open(out / ("sylvester_%d-%d.vertreter.txt" % (u1, u2)), "w") as f:
            for c in vert:
                f.write("VERTRETER %s\n" % perm(c))
        gut = ok_ver and ok_D and ok_norm and typ == soll
        print("(u1,u2)=(%d,%d) Typ %s (Soll %s) Vereinigung=64 Woerter: %s, D gleich nach Umbenennung: %s, Normalform: %s -> %s"
              % (u1, u2, typ, soll, ok_ver, ok_D, ok_norm, "OK" if gut else "FEHLER"))
        fehler += 0 if gut else 1
    print("FEHLER %d" % fehler)
    return 0 if fehler == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
