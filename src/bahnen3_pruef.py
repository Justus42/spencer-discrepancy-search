"""bahnen3_pruef.py -- Pruefcode (Teil 4, 25.11.2026). Kein Suchcode, nur ganze Zahlen; teilt keinen Code mit
src/bahnen3_tabu.c.

Liest ein Element g (Zeile L der P-Datei: pi(0..19), dann 20 Minus-Bits) und eine Trefferdatei (je Zeile:
Vertreterwort und Gewicht), baut die <g>-Bahnen der Zeilenpaare {x, ~x} aus (Bit j = Spalte j, Bit 1 = Eintrag -1;
Bild: Bit pi(j) = Bit j XOR Minus_j), prueft die Gewichte, und bestimmt den Ueberdeckungsradius des
selbstkomplementaeren Codes per Breitensuche ueber alle 2^20 Punkte.
Wahlweise: Matrix (je Zeilenpaar eine +-1-Zeile) in eine Datei fuer verifiers/pruefe_matrix.py.

Aufruf: python3 src/bahnen3_pruef.py P.txt L treffer.txt [matrix_aus.txt]
"""
import sys
import numpy as np

n = 20
ALLE = (1 << n) - 1


def main():
    zeilen = [z.split() for z in open(sys.argv[1]) if z.strip()]
    L = int(sys.argv[2])
    t = [int(x) for x in zeilen[L]]
    pi, mi = t[:n], t[n:2 * n]

    def g(x):
        y = 0
        for j in range(n):
            y |= (((x >> j) & 1) ^ mi[j]) << pi[j]
        return y

    def kan(x):
        return x ^ ALLE if (x >> (n - 1)) & 1 else x

    paare = set()
    gesamt_gewicht = 0
    for z in open(sys.argv[3]):
        if not z.strip():
            continue
        w, gw = (int(a) for a in z.split())
        bahn = {kan(w)}
        y = kan(w)
        for _ in range(3):
            y = kan(g(y))
            bahn.add(y)
        assert len(bahn) == gw, f"Bahn von {w} hat Groesse {len(bahn)}, Datei sagt {gw}"
        assert not (bahn & paare), "Bahnen ueberlappen"
        paare |= bahn
        gesamt_gewicht += gw
    woerter = sorted(paare | {p ^ ALLE for p in paare})
    dist = np.full(1 << n, 99, dtype=np.int8)
    front = np.array(woerter, dtype=np.int64)
    dist[front] = 0
    d = 0
    while front.size:
        d += 1
        nb = np.unique(np.concatenate([front ^ (1 << j) for j in range(n)]))
        nb = nb[dist[nb] == 99]
        dist[nb] = d
        front = nb
    r = int(dist.max())
    print(f"{sys.argv[3]}: {len(paare)} Zeilenpaare ({gesamt_gewicht} laut Gewichten), {len(woerter)} Woerter, "
          f"selbstkomplementaer, Ueberdeckungsradius {r} (Breitensuche ueber 2^{n} Punkte)")
    if len(sys.argv) > 4:
        with open(sys.argv[4], "w") as f:
            for p in sorted(paare):
                f.write(" ".join("-1" if (p >> j) & 1 else "1" for j in range(n)) + "\n")
        print(f"Matrix {len(paare)} x {n} geschrieben: {sys.argv[4]}")


if __name__ == "__main__":
    main()
