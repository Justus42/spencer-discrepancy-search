"""Pruefcode Teil A1 (24.11.2026): unabhaengige Gegenprobe fuer n=10, ohne Code aus bahnen_zaehl.c.

Bahnen werden hier durch explizites Anwenden aller 10 Gruppenelemente gebildet, Ueberdeckung
direkt ueber Hamming-Abstaende aller 1024 Punkte; dann alle t-Mengen voller Bahnen durchprobiert.
Nur ganze Zahlen.
"""
import functools
import itertools
import sys
import numpy as np

n = 10
N = 1 << n


def sigma(x):
    y = 0
    for b in range(n // 5):
        blk = (x >> (5 * b)) & 31
        blk = ((blk << 1) | (blk >> 4)) & 31
        y |= blk << (5 * b)
    return y


bahnen, gesehen = [], set()
for x in range(N):
    if x in gesehen:
        continue
    o, y = set(), x
    for _ in range(5):
        o.add(y); o.add(y ^ (N - 1)); y = sigma(y)
    gesehen |= o
    bahnen.append(sorted(o))
voll = [b for b in bahnen if len(b) == 10]
paare = [b for b in bahnen if len(b) == 2]
print(f"n=10: {len(voll)} volle Bahnen, {len(paare)} feste Paare")

pts = np.arange(N, dtype=np.int64)
popc = np.array([bin(i).count("1") for i in range(N)], dtype=np.int64)


def deckt(bahn, R):
    """Indikator (Bitmaske als Python-int) der ueberdeckten Punkte."""
    d = np.full(N, 99, dtype=np.int64)
    for w in bahn:
        d = np.minimum(d, popc[pts ^ w])
    m = 0
    for i in np.nonzero(d <= R)[0]:
        m |= 1 << int(i)
    return m


ALLE = (1 << N) - 1
for R, t_ohne, t_mit in [(2, 4, 3), (3, 2, 1)]:
    cv = [deckt(b, R) for b in voll]
    cp = 0
    for b in paare:
        cp |= deckt(b, R)
    # mit beliebigen Paaren (alle Paare = groesste Wahl): t_mit volle Bahnen
    kl = sum(1 for k in range(0, t_mit + 1) for S in itertools.combinations(range(len(voll)), k)
             if (cp | (0 if not S else functools.reduce(lambda a, b: a | b, (cv[i] for i in S)))) == ALLE)
    print(f"R={R}: Mengen mit hoechstens {t_mit} vollen Bahnen + allen Paaren, die ueberdecken: {kl}")
    # ohne Paare: genau t_ohne volle Bahnen
    if t_ohne == 2:
        c = sum(1 for a, b in itertools.combinations(range(len(voll)), 2) if cv[a] | cv[b] == ALLE)
    else:
        c = 0
        L = len(voll)
        for a in range(L):
            for b in range(a + 1, L):
                ab = cv[a] | cv[b]
                for cc in range(b + 1, L):
                    abc = ab | cv[cc]
                    rest = ALLE & ~abc
                    for d in range(cc + 1, L):
                        if cv[d] & rest == rest:
                            c += 1
    print(f"R={R}: {t_ohne}-Mengen voller Bahnen (ohne Paare), die ueberdecken: {c}")
    sys.stdout.flush()
