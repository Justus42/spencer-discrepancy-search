#!/usr/bin/env python3
"""Zaehlcode Auftrag S (Suche im Spann), eigenes Programm, nur ganze Zahlen.

Wort = Zahl, Bit j = Spalte j, Bit 1 = Eintrag -1. Nullspalten werden gestrichen.
C = Zeilen und Komplemente. L = affiner Spann von C = c0 + V (V linear, enthaelt den Einsvektor).
mu(x) = Zahl der Wortpaare {w, ~w} von L mit d(x, w) <= R oder d(x, ~w) <= R (konstant auf x + V).

Aufrufe:
  spann_zaehl.py info MATRIX R            Rang, Kern, mu-Verteilung (Wert: Zahl der Nebenklassen)
  spann_zaehl.py erweiterungen MATRIX R   alle Erweiterungen von V um eine Nebenklasse, nach kleinstem mu sortiert
  spann_zaehl.py export MATRIX R DATEI [y] Problem fuer die Suche schreiben (Muster A_D je Nebenklasse D),
                                          optional um Nebenklasse y (Zahl) erweitert
  spann_zaehl.py universen LISTE ORBITS MATRIX  (Teil 3a) Loesungen aus runs/bahnen3 im Spann von MATRIX
"""
import sys
from collections import Counter


def lies_woerter(pfad):
    A = [[int(t) for t in z.split()] for z in open(pfad) if z.strip() and not z.startswith('#')]
    n = len(A[0])
    spalten = [j for j in range(n) if any(z[j] != 0 for z in A)]
    for z in A:
        for j in spalten:
            assert z[j] in (1, -1), (pfad, z)
    m = len(spalten)
    W = [sum(1 << i for i, j in enumerate(spalten) if z[j] < 0) for z in A]
    return m, W


def reduziere(basis, v):
    for b in basis:
        if v ^ b < v:  # fuehrendes Bit von b in v gesetzt
            v ^= b
    return v


def echelon(vektoren):
    """Reduzierte Stufenform: Liste mit verschiedenen fuehrenden Bits, absteigend sortiert, voll reduziert."""
    basis = []
    for v in vektoren:
        v = reduziere(basis, v)
        if v:
            basis.append(v)
            basis.sort(reverse=True)
    # voll reduzieren
    for i in range(len(basis)):
        for k in range(len(basis)):
            if k != i and basis[k] ^ basis[i] < basis[k] and (basis[k] >> (basis[i].bit_length() - 1)) & 1:
                basis[k] ^= basis[i]
    basis.sort(reverse=True)
    return basis


def spann(m, W):
    ALLE = (1 << m) - 1
    C = set(W) | {w ^ ALLE for w in W}
    c0 = min(C)
    V = echelon([c ^ c0 for c in C] + [ALLE])
    c0 = reduziere(V, c0)  # kanonischer Vertreter der Nebenklasse
    return C, c0, V


def elemente(V):
    E = [0]
    for b in V:
        E += [e ^ b for e in E]
    return E


def nebenklassen_vertreter(m, V):
    piv = {b.bit_length() - 1 for b in V}
    frei = [j for j in range(m) if j not in piv]
    R = [0]
    for j in frei:
        R += [r | (1 << j) for r in R]
    return sorted(R)


def mu_verteilung(m, R, c0, V):
    L = [c0 ^ e for e in elemente(V)]
    reps = nebenklassen_vertreter(m, V)
    mus = []
    for x in reps:
        mus.append(sum(1 for w in L if bin(w ^ x).count('1') <= R))
    return reps, mus


def kern(C, V):
    return sum(1 for t in elemente(V) if all((c ^ t) in C for c in C))


def info(pfad, R):
    m, W = lies_woerter(pfad)
    C, c0, V = spann(m, W)
    r = len(V)
    reps, mus = mu_verteilung(m, R, c0, V)
    vert = sorted(Counter(mus).items())
    k = kern(C, V)
    paare = 1 << (r - 1)
    print(f"{pfad}: m={m} R={R} Woerter={len(C)} Rang={r} Paare={paare} Kern={k} Nebenklassen={len(reps)}")
    print("  mu: " + ", ".join(f"{a}: {b}" for a, b in vert))
    print(f"  Zaehlschranke k >= {paare}/{min(mus)} = {paare / min(mus):.3f}")
    return m, C, c0, V


def erweiterungen(pfad, R):
    m, W = lies_woerter(pfad)
    C, c0, V = spann(m, W)
    reps = nebenklassen_vertreter(m, V)
    erg = []
    for y in reps:
        if y == 0:
            continue
        V2 = echelon(V + [y])
        c02 = reduziere(V2, c0)
        _, mus = mu_verteilung(m, R, c02, V2)
        erg.append((min(mus), y, tuple(sorted(Counter(mus).items()))))
    print(f"{pfad}: {len(erg)} Erweiterungen auf Rang {len(V) + 1}")
    print("  nach kleinstem mu: " + ", ".join(f"{a}: {b}" for a, b in sorted(Counter(e[0] for e in erg).items())))
    print("  verschiedene mu-Verteilungen:")
    for vt, anz in sorted(Counter(e[2] for e in erg).items()):
        print(f"    {anz} x " + ", ".join(f"{a}: {b}" for a, b in vt))
    return erg


def export(pfad, R, ziel, y=None):
    """Schreibt das abstrakte Ueberdeckungsproblem: Gruppe G = V/<1> (Dimension r-1),
    Paar g <-> Wortpaar {c0 + v(g)}. Je Nebenklasse D von V eine Musterliste A_D (Elemente von G).
    Bedingung: S + A_D = G fuer alle D."""
    m, W = lies_woerter(pfad)
    C, c0, V = spann(m, W)
    ALLE = (1 << m) - 1
    if y is not None:
        V = echelon(V + [y])
        c0 = reduziere(V, c0)
    r = len(V)
    # Basis von V mit ALLE zuerst, dann Rest; G-Koordinaten = Koeffizienten der Rest-Basis
    basis = [ALLE]
    for b in V:
        if reduziere(echelon(basis), b):
            basis.append(b)
    assert len(basis) == r
    gb = basis[1:]  # r-1 Vektoren
    vec = []
    for g in range(1 << (r - 1)):
        v = 0
        for i in range(r - 1):
            if g >> i & 1:
                v ^= gb[i]
        vec.append(v)
    reps = nebenklassen_vertreter(m, V)
    with open(ziel, 'w') as f:
        f.write(f"# Spann von {pfad}, m={m}, R={R}, Rang {r}, Erweiterung y={y}\n")
        f.write(f"{r - 1} {len(reps)}\n")
        # Paarwoerter (Vertreter c0 ^ vec[g])
        f.write(" ".join(str(c0 ^ v) for v in vec) + "\n")
        for x in reps:
            A = [g for g in range(1 << (r - 1))
                 if min(bin(c0 ^ vec[g] ^ x).count('1'), bin(c0 ^ vec[g] ^ x ^ ALLE).count('1')) <= R]
            f.write(f"{x} {len(A)} " + " ".join(map(str, A)) + "\n")
    # Lage des Ausgangscodes in G
    pos = {}
    for g, v in enumerate(vec):
        pos[c0 ^ v] = g
        pos[c0 ^ v ^ ALLE] = g
    S = sorted({pos[w] for w in C if w in pos})
    print(f"export {ziel}: G-Dimension {r - 1}, Nebenklassen {len(reps)}, Ausgangscode = {len(S)} Paare: {S}")


def universen(liste, orbits_datei, pfad):
    """Teil 3a: Loesungen aus runs/bahnen3 (Bahnvertreter/Gewicht) zu Zeilenmengen aufblaehen,
    pruefen, ob sie im Spann von MATRIX liegen, und verschiedene Spanne zaehlen."""
    m, W = lies_woerter(pfad)
    ALLE = (1 << m) - 1
    C, c0, V = spann(m, W)
    P = []
    for z in open(orbits_datei):
        t = [int(x) for x in z.split()]
        if t:
            P.append((t[:m], t[m:2 * m]))

    def bild(g, x):
        p, s = g
        y = 0
        for j in range(m):
            if ((x >> j) & 1) ^ s[j]:
                y |= 1 << p[j]
        return y

    def kan(x):
        return x ^ ALLE if x >> (m - 1) & 1 else x

    bahn_cache = {}

    def bahn(v):
        if v not in bahn_cache:
            bahn_cache[v] = sorted({kan(bild(g, v)) for g in P})
        return bahn_cache[v]

    im_spann = 0
    spanne = Counter()
    n = 0
    for z in open(liste):
        if ':' not in z:
            continue
        teile = z.split(':')[1].split()
        zeilen = []
        for t in teile:
            v, gew = map(int, t.split('/'))
            b = bahn(v)
            assert len(b) == gew, (v, gew, b)
            zeilen += b
        n += 1
        Wz = set(zeilen)
        if all(reduziere(V, w ^ c0) == 0 for w in Wz):
            im_spann += 1
        _, c0z, Vz = spann(m, list(Wz))
        spanne[(c0z, tuple(Vz))] += 1
    print(f"{liste}: {n} Loesungen, im Spann von {pfad}: {im_spann}; verschiedene Spanne: {len(spanne)}")
    print("  Raenge: " + ", ".join(f"{a}: {b}" for a, b in sorted(Counter(len(k[1]) for k in spanne).items())))
    print("  Loesungen je Spann (Haeufigkeiten): " + ", ".join(f"{a}: {b}" for a, b in sorted(Counter(spanne.values()).items())))
    eigen = (c0, tuple(V))
    print(f"  Spann von {pfad} darunter: {eigen in spanne}, mit {spanne.get(eigen, 0)} Loesungen")
    return spanne


def startdatei(problem, matrix, ziel, ohne=None):
    """Zeilen einer Matrix als Elemente von G des Problems (Paarwort w oder ~w); optional ohne die Zeile Nummer `ohne`."""
    f = open(problem); kopf = f.readline(); d, nc = map(int, f.readline().split())
    W = list(map(int, f.readline().split()))
    m = int(kopf.split('m=')[1].split(',')[0]); ALLE = (1 << m) - 1
    pos = {}
    for g, w in enumerate(W):
        pos[w] = g; pos[w ^ ALLE] = g
    m2, Z = lies_woerter(matrix)
    assert m2 == m
    el = []
    for i, w in enumerate(Z):
        if i == ohne:
            continue
        if w not in pos:
            print(f"{matrix}: Zeile {i} nicht im Spann"); return None
        el.append(pos[w])
    open(ziel, 'w').write(" ".join(map(str, el)) + "\n")
    return el


if __name__ == '__main__' and sys.argv[1] == 'startdatei':
    print(startdatei(sys.argv[2], sys.argv[3], sys.argv[4], int(sys.argv[5]) if len(sys.argv) > 5 else None))
elif __name__ == '__main__':
    a = sys.argv
    if a[1] == 'info':
        info(a[2], int(a[3]))
    elif a[1] == 'erweiterungen':
        erweiterungen(a[2], int(a[3]))
    elif a[1] == 'export':
        export(a[2], int(a[3]), a[4], int(a[5]) if len(a) > 5 else None)
    elif a[1] == 'universen':
        universen(a[2], a[3], a[4])
