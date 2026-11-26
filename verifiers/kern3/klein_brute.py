#!/usr/bin/env python3
"""klein_brute.py -- Pruefcode Auftrag K: Vollstaendigkeit der Suche (src/kern3_such.c dfs) an kleinen Zellen.

Fuer kleine (m,R) und alle Typen bestimmt das Skript das kleinste K ohne jede Symmetrie oder Schranke
(einzige Annahme: c_1 = Klasse 0, gilt durch Verschiebung), durch Aufzaehlen aller (K-2)-Teilmengen der uebrigen
Klassen und Test, ob die restlichen offenen Klassen von einer einzigen Nebenklasse gedeckt werden
(Schnitt der Deckermengen aller offenen Klassen). Danach muss der Suchcode (getrennter Prozess) fuer
K = Kmin GEFUNDEN und fuer K = Kmin - 1 KEINE melden. Teilt keinen Code mit dem Suchcode.
Aufruf: klein_brute.py m R KMAX [m R KMAX ...]
"""
import itertools
import subprocess
import sys
from pathlib import Path

WURZEL = Path(__file__).resolve().parent.parent.parent
SUCHER = WURZEL / "build" / "kern3_such"


def typen(m):
    aus = []
    for n0 in range(m, -1, -1):
        for n1 in range(min(n0, m - n0), -1, -1):
            for n2 in range(min(n1, m - n0 - n1), -1, -1):
                n3 = m - n0 - n1 - n2
                if n3 > n2 or n3 < 0:
                    continue
                if sum(1 for v in (n0, n1, n2, n3) if v) >= 3:
                    aus.append((n0, n1, n2, n3))
    return aus


def brute_kmin(n, R, kmax):
    m = sum(n)
    o, blk = 0, []
    for v in n:
        blk.append(((1 << v) - 1) << o)
        o += v
    eins = (1 << m) - 1
    a, b = blk[1] | blk[3], blk[2] | blk[3]
    D = [0, a, b, a ^ b, eins, eins ^ a, eins ^ b, eins ^ a ^ b]
    rep = sorted({min(x ^ d for d in D) for x in range(1 << m)})
    pos = {r: i for i, r in enumerate(rep)}
    kl = lambda x: pos[min(x ^ d for d in D)]
    Q = len(rep)
    S = [r for r in rep if min(bin(r ^ d).count("1") for d in D) <= R]
    T = [sum(1 << kl(rep[c] ^ s) for s in S) for c in range(Q)]   # Nebenklasse c deckt diese Klassen
    alle = (1 << Q) - 1
    for K in range(1, kmax + 1):
        if K == 1:
            if T[0] == alle:
                return 1
            continue
        for rest in itertools.combinations(range(1, Q), K - 2):
            u = T[0]
            for c in rest:
                u |= T[c]
            offen = alle & ~u
            if offen == 0:
                return K
            # Decker aller offenen Klassen: Klasse y wird von c gedeckt <=> y in T[c] <=> c in T[y]
            dk = alle
            y = offen
            while y and dk:
                i = (y & -y).bit_length() - 1
                dk &= T[i]
                y &= y - 1
            if dk:
                return K
    return None


def dfs(n, R, K):
    m = sum(n)
    p = subprocess.run([str(SUCHER), "dfs", str(m), str(R)] + [str(v) for v in n] + [str(K), "600"], capture_output=True, text=True)
    for z in p.stdout.splitlines():
        if z.startswith("ERGEBNIS"):
            return z.split()[1]
    return "FEHLER:" + p.stdout + p.stderr


def main():
    a = [int(v) for v in sys.argv[1:]]
    fehler = 0
    for i in range(0, len(a), 3):
        m, R, kmax = a[i], a[i + 1], a[i + 2]
        for n in typen(m):
            kmin = brute_kmin(n, R, kmax)
            if kmin is None:
                d = dfs(n, R, kmax)
                ok = (d == "KEINE")
                print("(%d,%d) Typ %s brute: kein K <= %d | dfs K=%d: %s %s" % (m, R, n, kmax, kmax, d, "OK" if ok else "ABWEICHUNG"), flush=True)
            else:
                d1 = dfs(n, R, kmin)
                d0 = dfs(n, R, kmin - 1) if kmin > 1 else "KEINE"
                ok = (d1 == "GEFUNDEN" and d0 == "KEINE")
                print("(%d,%d) Typ %s brute Kmin=%d | dfs K=%d: %s, K=%d: %s %s" % (m, R, n, kmin, kmin, d1, kmin - 1, d0, "OK" if ok else "ABWEICHUNG"), flush=True)
            fehler += 0 if ok else 1
    print("ABWEICHUNGEN %d" % fehler)
    return 0 if fehler == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
