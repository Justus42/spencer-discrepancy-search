#!/usr/bin/env python3
"""Hilfscode Auftrag S: Wirkung der Spaltenpermutationen (aus build/spann_aut) auf G und auf die Erweiterungsrichtungen.

  spann_sym.py gruppe PROBLEM PERMS AUSGABE_GEN [anzahl_gen]
      Lineare Abbildungen von G (verschiedene), Bahnen auf G\\{0}, Erzeuger (zufaellig gezogen, Bahnen geprueft),
      schreibt Erzeuger als Bilder der Basisvektoren (je Zeile d Zahlen).
  spann_sym.py erweiterungen MATRIX R PERMS
      Bahnen der Gruppe auf den Nebenklassen y + V (Erweiterungsrichtungen), mit mu-Verteilung je Bahn.
  spann_sym.py stab_gen PROBLEM PERMS MATRIX y AUSGABE_GEN
      Erzeuger des Stabilisators von y + V, wirkend auf G' = (V + <y>)/<1> (Problem aus export ... y).
"""
import random
import sys
from collections import Counter

sys.path.insert(0, __file__.rsplit('/', 1)[0])
import spann_zaehl as sz


def lies_problem(pfad):
    f = open(pfad)
    kopf = f.readline()
    d, nc = map(int, f.readline().split())
    W = list(map(int, f.readline().split()))
    m = int(kopf.split('m=')[1].split(',')[0])
    return m, d, W


def lies_perms(pfad):
    return [list(map(int, z.split())) for z in open(pfad) if z.strip()]


def bild(p, x):
    y = 0
    while x:
        j = (x & -x).bit_length() - 1
        x &= x - 1
        y |= 1 << p[j]
    return y


def lineare_wirkung(m, d, W, perms):
    """G-Index g <-> v(g) = W[g] ^ W[0] (mod Einsvektor). Gibt je Permutation die Bilder der Basisvektoren e_i."""
    ALLE = (1 << m) - 1
    idx = {}
    for g, w in enumerate(W):
        v = w ^ W[0]
        idx[v] = g
        idx[v ^ ALLE] = g
    basis = [1 << i for i in range(d)]
    mats = set()
    for p in perms:
        mats.add(tuple(idx[bild(p, W[e] ^ W[0])] for e in basis))
    return sorted(mats)


def anwenden(M, g):
    y = 0
    i = 0
    while g:
        if g & 1:
            y ^= M[i]
        g >>= 1
        i += 1
    return y


def bahnen(N, gens):
    eltern = list(range(N))

    def finde(x):
        while eltern[x] != x:
            eltern[x] = eltern[eltern[x]]
            x = eltern[x]
        return x
    for M in gens:
        for g in range(N):
            a, b = finde(g), finde(anwenden(M, g))
            if a != b:
                eltern[a] = b
    return Counter(finde(g) for g in range(1, N))


def gruppe(problem, perms_pfad, ziel, anz=12):
    m, d, W = lies_problem(problem)
    N = 1 << d
    mats = lineare_wirkung(m, d, W, lies_perms(perms_pfad))
    voll = bahnen(N, mats)
    random.seed(1)
    gens = random.sample(mats, min(anz, len(mats)))
    teil = bahnen(N, gens)
    print(f"{problem}: {len(mats)} verschiedene lineare Abbildungen von G (d={d}); Bahnen auf G\\0 (alle): "
          f"{sorted(voll.values())}; mit {len(gens)} Erzeugern: {sorted(teil.values())}")
    with open(ziel, 'w') as f:
        for M in gens:
            f.write(" ".join(map(str, M)) + "\n")
    return mats


def erweiterungen(matrix, R, perms_pfad):
    m, W = sz.lies_woerter(matrix)
    C, c0, V = sz.spann(m, W)
    reps = [y for y in sz.nebenklassen_vertreter(m, V) if y]
    perms = lies_perms(perms_pfad)
    pos = {y: i for i, y in enumerate(reps)}
    eltern = list(range(len(reps)))

    def finde(x):
        while eltern[x] != x:
            eltern[x] = eltern[eltern[x]]
            x = eltern[x]
        return x
    random.seed(2)
    probe = random.sample(perms, min(300, len(perms)))
    for p in probe:
        for y in reps:
            z = sz.reduziere(V, bild(p, y))
            a, b = finde(pos[y]), finde(pos[z])
            if a != b:
                eltern[a] = b
    kl = {}
    for i, y in enumerate(reps):
        kl.setdefault(finde(i), []).append(y)
    print(f"{matrix}: Erweiterungsrichtungen {len(reps)}, Bahnen (300 Gruppenelemente): {len(kl)}")
    erg = []
    for r, ys in sorted(kl.items(), key=lambda t: len(t[1])):
        y = min(ys)
        V2 = sz.echelon(V + [y])
        _, mus = sz.mu_verteilung(m, R, sz.reduziere(V2, c0), V2)
        print(f"  Bahn Groesse {len(ys)}, Vertreter y={y}, mu: " + ", ".join(f"{a}: {b}" for a, b in sorted(Counter(mus).items())))
        erg.append((len(ys), y))
    return erg


def stab_gen(problem, perms_pfad, matrix, y, ziel, anz=12):
    """Erzeuger des Stabilisators von y+V (als Spaltenpermutationen), wirkend auf das erweiterte Problem."""
    m, W = sz.lies_woerter(matrix)
    C, c0, V = sz.spann(m, W)
    perms = lies_perms(perms_pfad)
    stab = [p for p in perms if sz.reduziere(V, bild(p, y) ^ y) == 0]
    m2, d2, W2 = lies_problem(problem)
    mats = lineare_wirkung(m2, d2, W2, stab)
    N = 1 << d2
    voll = bahnen(N, mats)
    random.seed(3)
    gens = random.sample(mats, min(anz, len(mats)))
    teil = bahnen(N, gens)
    print(f"{problem}: Stabilisator von y={y}: {len(stab)} Permutationen, {len(mats)} lineare Abbildungen von G (d={d2}); "
          f"Bahnen auf G\\0 alle {sorted(voll.values())}, mit {len(gens)} Erzeugern {sorted(teil.values())}")
    with open(ziel, 'w') as f:
        for M in gens:
            f.write(" ".join(map(str, M)) + "\n")


if __name__ == '__main__':
    a = sys.argv
    if a[1] == 'gruppe':
        gruppe(a[2], a[3], a[4])
    elif a[1] == 'erweiterungen':
        erweiterungen(a[2], int(a[3]), a[4])
    elif a[1] == 'stab_gen':
        stab_gen(a[2], a[3], a[4], int(a[5]), a[6])


def pruefe_erzeuger(problem, gen_pfad):
    """Prueft direkt am Problem: jeder Erzeuger M ist linear-bijektiv auf G und bildet jedes Muster A_D
    auf ein Translat eines Musters A_D' ab (D -> D' bijektiv). Dann bildet x -> M x + t Loesungen auf Loesungen ab."""
    f = open(problem); f.readline()
    d, nc = map(int, f.readline().split()); f.readline()
    N = 1 << d
    pats = [frozenset(map(int, z.split()[2:])) for z in f if z.strip()]
    def norm(A):
        return min(tuple(sorted(a ^ t for a in A)) for t in A)
    klasse = {}
    for i, A in enumerate(pats):
        klasse.setdefault(norm(A), []).append(i)
    gens = [list(map(int, z.split())) for z in open(gen_pfad) if z.strip()]
    for M in gens:
        assert len({anwenden(M, g) for g in range(N)}) == N, "nicht bijektiv"
        bilder = []
        for A in pats:
            k = norm(frozenset(anwenden(M, a) for a in A))
            assert k in klasse, "Musterbild ist kein Translat eines Musters"
            bilder.append(k)
        assert len(set(bilder)) == len(klasse)
    print(f"{gen_pfad}: {len(gens)} Erzeuger geprueft, jeder erhaelt die Musterfamilie (bis auf Translation), {len(klasse)} Musterklassen")


if __name__ == '__main__' and sys.argv[1] == 'pruefe':
    pruefe_erzeuger(sys.argv[2], sys.argv[3])
