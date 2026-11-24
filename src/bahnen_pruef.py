"""Pruefcode (24.11.2026), getrennt vom Suchcode, kein Code aus src/bahnen_*.c.

Bahnen unter sigma (jeder 5er-Block, Bits 5j..5j+4, um eine Stelle gedreht) plus Komplement.
Unterbefehle:
  radius n R datei            je Zeile Bahn-Vertreter -> Woerter bilden, Ueberdeckungsradius per
                              Breitensuche ueber alle 2^n Punkte (numpy, ganzzahlig), Selbstkomplement.
  liste n R datei             wie radius fuer jede Zeile einer Loesungsliste, dazu Abschluss der Liste
                              unter Normalisator-Erzeugern (Blockdrehung, Blocktausch, Multiplikator 2,
                              Verschiebung um blockkonstantes Wort).
  matrix n datei ausgabe      Woerter -> Matrix (je Komplementpaar eine Zeile, Bit 1 = -1) und _pad
                              (quadratisch mit Nullspalten).
  produkt datei15 ausgabe     Produkt der Woerter mit {00000, 11111} (neuer Block oben), Vertreter je Bahn.
"""
import sys
import numpy as np


def sigma(x, n):
    y = 0
    for b in range(n // 5):
        blk = (x >> (5 * b)) & 31
        y |= (((blk << 1) | (blk >> 4)) & 31) << (5 * b)
    return y


def bahn(x, n):
    alle = (1 << n) - 1
    o, y = set(), x
    for _ in range(5):
        o.add(y); o.add(y ^ alle); y = sigma(y, n)
    return o


def woerter(reps, n):
    w = set()
    for r in reps:
        w |= bahn(r, n)
    return sorted(w)


def radius(ws, n):
    """Ueberdeckungsradius per Breitensuche auf dem Wuerfel."""
    N = 1 << n
    d = np.full(N, -1, dtype=np.int32)
    front = np.array(sorted(set(ws)), dtype=np.int64)
    d[front] = 0
    k = 0
    while True:
        nb = (front[:, None] ^ (1 << np.arange(n, dtype=np.int64))[None, :]).ravel()
        nb = np.unique(nb)
        nb = nb[d[nb] < 0]
        if nb.size == 0:
            break
        k += 1
        d[nb] = k
        front = nb
    assert (d >= 0).all()
    return int(d.max())


def selbstkomplementaer(ws, n):
    s = set(ws)
    return all((w ^ ((1 << n) - 1)) in s for w in s)


def lies(datei):
    zeilen = []
    for z in open(datei):
        z = z.split("#")[0].split()
        if z:
            zeilen.append([int(t) for t in z])
    return zeilen


def kanon(x, n):
    return min(bahn(x, n))


def normalisator_erzeuger(n):
    k = n // 5

    def blockdreh(x):  # nur Block 0 drehen
        blk = x & 31
        return (x & ~31) | (((blk << 1) | (blk >> 4)) & 31)

    def blocktausch(x):  # Block 0 <-> Block 1
        a, b = x & 31, (x >> 5) & 31
        return (x & ~1023) | (b) | (a << 5)

    def blockzyklus(x):  # Block j -> Block j+1 mod k
        y = 0
        for j in range(k):
            y |= ((x >> (5 * j)) & 31) << (5 * ((j + 1) % k))
        return y

    def mult2(x):  # Koordinate i -> 2i mod 5 in allen Bloecken
        y = 0
        for j in range(k):
            for i in range(5):
                if (x >> (5 * j + i)) & 1:
                    y |= 1 << (5 * j + (2 * i) % 5)
        return y

    def versch(x):  # Verschiebung um 11111 in Block 0
        return x ^ 31

    return [("Blockdrehung", blockdreh), ("Blocktausch", blocktausch), ("Blockzyklus", blockzyklus),
            ("Multiplikator2", mult2), ("Verschiebung", versch)]


def main():
    cmd = sys.argv[1]
    if cmd == "radius":
        n, R = int(sys.argv[2]), int(sys.argv[3])
        reps = [r for z in lies(sys.argv[4]) for r in z]
        ws = woerter(reps, n)
        rad = radius(ws, n)
        sk = selbstkomplementaer(ws, n)
        print(f"{sys.argv[4]}: {len(reps)} Vertreter, {len(ws)} Woerter, selbstkomplementaer={sk}, "
              f"Ueberdeckungsradius={rad} -> {'OK' if rad <= R and sk else 'FEHLER'} (Ziel R={R})")
    elif cmd == "liste":
        n, R = int(sys.argv[2]), int(sys.argv[3])
        zeilen = lies(sys.argv[4])
        schl = set()
        schlecht = 0
        for z in zeilen:
            ws = woerter(z, n)
            if len(ws) != 10 * len(z) or radius(ws, n) > R:
                schlecht += 1
            schl.add(frozenset(kanon(r, n) for r in z))
        print(f"{len(zeilen)} Zeilen, {len(schl)} verschiedene Bahnmengen, davon nicht ueberdeckend/fehlerhaft: {schlecht}")
        for name, f in normalisator_erzeuger(n):
            fehlt = 0
            for S in schl:
                bild = frozenset(kanon(f(r), n) for r in S)
                if bild not in schl:
                    fehlt += 1
            print(f"  Abschluss unter {name}: Bilder ausserhalb der Liste: {fehlt}")
    elif cmd == "matrix":
        n = int(sys.argv[2])
        reps = [r for z in lies(sys.argv[3]) for r in z]
        ws = woerter(reps, n)
        alle = (1 << n) - 1
        zeilen = sorted({min(w, w ^ alle) for w in ws})
        A = [[-1 if (w >> i) & 1 else 1 for i in range(n)] for w in zeilen]
        aus = sys.argv[4]
        with open(aus, "w") as f:
            for z in A:
                f.write(" ".join(f"{v:2d}" for v in z) + "\n")
        m = max(len(A), n)
        with open(aus.replace(".txt", "_pad.txt"), "w") as f:
            for z in A:
                f.write(" ".join(f"{v:2d}" for v in z + [0] * (m - n)) + "\n")
            for _ in range(m - len(A)):
                f.write(" ".join(" 0" for _ in range(m)) + "\n")
        print(f"{aus}: {len(A)} x {n}, _pad {m} x {m}")
    elif cmd == "produkt":
        reps15 = [r for z in lies(sys.argv[2]) for r in z]
        ws15 = woerter(reps15, 15)
        ws20 = sorted({w | (t << 15) for w in ws15 for t in (0, 31)})
        reps20 = sorted({kanon(w, 20) for w in ws20})
        with open(sys.argv[3], "w") as f:
            f.write(" ".join(map(str, reps20)) + "\n")
        print(f"Produkt: {len(ws20)} Woerter, {len(reps20)} Bahnen, Vertreter {reps20}")
    else:
        raise SystemExit(__doc__)


if __name__ == "__main__":
    main()
