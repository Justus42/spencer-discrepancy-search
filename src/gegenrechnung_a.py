#!/usr/bin/env python3
"""Teil A der Gegenrechnung vom 24.11.2026 (Prüfcode, kein Suchcode, nur ganze Zahlen).

Rechnet die Angaben aus notes/zweitmeinung_2026-11-24_pruefvermerk.md mit eigenem Code nach.
Die exakte Diskrepanz kommt aus build/exakt_disc (src/exakt_disc.c), die Zeugen werden hier
noch einmal unabhängig mit Python-Ganzzahlen nachgerechnet.

  python3 src/gegenrechnung_a.py a1      Hadamard 20 ohne zwei Spalten (3 Klassen + Transponierte)
  python3 src/gegenrechnung_a.py a2      Hadamard 24 ohne drei Spalten (60 Klassen + Transponierte)
  python3 src/gegenrechnung_a.py a3      Paley-Konferenzmatrizen Ordnung 6, 10, 14, 18
  python3 src/gegenrechnung_a.py a4      Normalität von controls/had24_1p3_paley12.txt
  python3 src/gegenrechnung_a.py zeugen  Zeugen aus runs/teilA/ nachrechnen
"""
import gzip
import itertools
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

import numpy as np

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.environ.get("EXAKT_BIN", os.path.join(ROOT, "build", "exakt_disc"))
OUT = os.path.join(ROOT, "runs", "teilA")


def lies_matrix(fn):
    rows = []
    with open(fn) as f:
        for line in f:
            if line.strip():
                rows.append([int(t) for t in line.split()])
    return rows


def schreibe_matrix(fn, M):
    with open(fn, "w") as f:
        for r in M:
            f.write(" ".join(f"{v:2d}" for v in r) + "\n")


def ist_hadamard(M):
    n = len(M)
    for i in range(n):
        for j in range(n):
            s = sum(M[i][k] * M[j][k] for k in range(n))
            if s != (n if i == j else 0):
                return False
    return True


def transponiert(M):
    return [list(c) for c in zip(*M)]


def max_betrag(M, x):
    """max_i |sum_j M_ij x_j|, reine Python-Ganzzahlen."""
    return max(abs(sum(a * b for a, b in zip(r, x))) for r in M)


def zeuge_zu_x(hexz, n, weg):
    z = int(hexz, 16)
    x = []
    for j in range(n):
        x.append(0 if j in weg else (-1 if (z >> j) & 1 else 1))
    return x


def lauf_streichen(fn, k, transp):
    cmd = [BIN, fn, "-s", str(k)] + (["-t"] if transp else [])
    p = subprocess.run(cmd, capture_output=True, text=True, check=True)
    zeilen = []
    for line in p.stdout.split("\n"):
        if line.strip():
            w, d, z = line.split()
            zeilen.append((tuple(int(t) for t in w.split(",")), int(d), z))
    return zeilen, p.stderr.strip()


def pruefe_zeilen(M, zeilen):
    """Zeugen ganzzahlig nachrechnen; Rückgabe Liste der Abweichungen."""
    n = len(M[0])
    fehler = []
    for weg, d, z in zeilen:
        x = zeuge_zu_x(z, n, set(weg))
        if any(x[j] != 0 for j in weg) or max_betrag(M, x) != d:
            fehler.append((weg, d, z))
    return fehler


def a1():
    os.makedirs(OUT, exist_ok=True)
    log = []
    gesamt = {}
    for k in (1, 2, 3):
        fn = os.path.join(ROOT, "controls", f"had20_klasse{k}.txt")
        M = lies_matrix(fn)
        assert ist_hadamard(M), fn
        for transp in (False, True):
            zeilen, info = lauf_streichen(fn, 2, transp)
            MM = transponiert(M) if transp else M
            fehler = pruefe_zeilen(MM, zeilen)
            assert len(zeilen) == 190
            verteilung = {}
            for _, d, _ in zeilen:
                verteilung[d] = verteilung.get(d, 0) + 1
            name = f"had20_klasse{k}{' transponiert' if transp else ''}"
            gesamt[name] = verteilung
            log.append(f"{name}: 190 Paare, Diskrepanz-Verteilung {dict(sorted(verteilung.items()))}, "
                       f"Zeugen nachgerechnet, Fehler {len(fehler)}")
            with open(os.path.join(OUT, f"a1_{'T' if transp else 'H'}{k}.txt"), "w") as f:
                for weg, d, z in zeilen:
                    f.write(f"{weg[0]},{weg[1]} {d} {z}\n")
    for line in log:
        print(line)


def mckay(datei):
    Ms = []
    with open(datei) as f:
        for line in f:
            t = line.split()
            if not t:
                continue
            M = []
            for h in t:
                v = int(h, 16)
                M.append([1 if (v >> (23 - j)) & 1 else -1 for j in range(24)])
            Ms.append(M)
    return Ms


def a2():
    d2 = os.path.join(OUT, "mckay")
    os.makedirs(d2, exist_ok=True)
    Ms = mckay(os.path.join(ROOT, "controls", "had24_mckay_hex.txt"))
    assert len(Ms) == 60
    for i, M in enumerate(Ms):
        assert ist_hadamard(M), i
        schreibe_matrix(os.path.join(d2, f"h24_{i + 1:02d}.txt"), M)
    aufgaben = [(i, t) for i in range(60) for t in (False, True)]

    def eins(a):
        i, t = a
        fn = os.path.join(d2, f"h24_{i + 1:02d}.txt")
        zeilen, info = lauf_streichen(fn, 3, t)
        return a, zeilen

    zeugen_fn = os.path.join(OUT, "a2_zeugen_24x21.txt.gz")
    gesamt = {}
    with ThreadPoolExecutor(max_workers=4) as ex, gzip.open(zeugen_fn, "wt") as zf:
        zf.write("# je Zeile: klasse(1-60) H|T gestrichene_spalten diskrepanz zeuge_hex "
                 "(Bit j = 1: x_j = -1, Spalten der Originalmatrix, gestrichene Bits 0)\n")
        for (i, t), zeilen in ex.map(eins, aufgaben):
            assert len(zeilen) == 2024
            M = Ms[i]
            MM = transponiert(M) if t else M
            fehler = pruefe_zeilen(MM, zeilen)
            vert = {}
            for weg, d, z in zeilen:
                vert[d] = vert.get(d, 0) + 1
                zf.write(f"{i + 1} {'T' if t else 'H'} {weg[0]},{weg[1]},{weg[2]} {d} {z}\n")
            for d, c in vert.items():
                gesamt[d] = gesamt.get(d, 0) + c
            print(f"klasse {i + 1:2d} {'T' if t else 'H'}: 2024 Tripel, Verteilung {dict(sorted(vert.items()))}, "
                  f"Zeugen-Fehler {len(fehler)}", flush=True)
    print(f"GESAMT 120 x 2024 = {120 * 2024} Fälle, Verteilung {dict(sorted(gesamt.items()))}")


def jacobsthal(q):
    """Quadratischer Charakter über GF(q), q = p oder 9; Elemente als Index 0..q-1."""
    if q == 9:
        # GF(9) = F3[i]/(i^2+1), Element a+b*i als 3*b+a
        elems = [(a, b) for b in range(3) for a in range(3)]
        def add(u, v):
            return ((u[0] + v[0]) % 3, (u[1] + v[1]) % 3)
        def neg(u):
            return ((-u[0]) % 3, (-u[1]) % 3)
        def mul(u, v):
            return ((u[0] * v[0] - u[1] * v[1]) % 3, (u[0] * v[1] + u[1] * v[0]) % 3)
    else:
        elems = list(range(q))
        def add(u, v):
            return (u + v) % q
        def neg(u):
            return (-u) % q
        def mul(u, v):
            return (u * v) % q
    null = elems[0]
    quadrate = {mul(e, e) for e in elems if e != null}
    def chi(e):
        return 0 if e == null else (1 if e in quadrate else -1)
    return [[chi(add(b, neg(a))) for b in elems] for a in elems]


def a3():
    os.makedirs(OUT, exist_ok=True)
    for q in (5, 9, 13, 17):
        Q = jacobsthal(q)
        n = q + 1
        C = [[0] + [1] * q] + [[1] + Q[i] for i in range(q)]
        # Konferenzmatrix: symmetrisch, C C^T = (n-1) I
        ok = all(sum(C[i][k] * C[j][k] for k in range(n)) == ((n - 1) if i == j else 0)
                 for i in range(n) for j in range(n))
        sym = all(C[i][j] == C[j][i] for i in range(n) for j in range(n))
        fn = os.path.join(OUT, f"a3_paley_konferenz_{n}.txt")
        schreibe_matrix(fn, C)
        p = subprocess.run([BIN, fn], capture_output=True, text=True, check=True)
        out = p.stdout.strip()
        d = int(out.split("disc ")[1].split(",")[0])
        z = out.split("zeuge ")[1]
        x = zeuge_zu_x(z, n, set())
        chk = max_betrag(C, x)
        import math
        # kleinstes d mit d/sqrt(n) > 5/sqrt(8), d^2 * 8 > 25 n exakt; jede Zeile hat n-1 Einträge
        # ungleich 0, also hat jede Zeilensumme die Parität von n-1
        noetig = 1
        while 8 * noetig * noetig <= 25 * n or noetig % 2 != (n - 1) % 2:
            noetig += 1
        print(f"Paley-Konferenzmatrix Ordnung {n} (q={q}): konferenz={ok}, symmetrisch={sym}, "
              f"Diskrepanz {d} (Zeuge nachgerechnet: {chk}), Rekord bräuchte >= {noetig}")


def a4():
    fn = os.path.join(ROOT, "controls", "had24_1p3_paley12.txt")
    M = lies_matrix(fn)
    m = len(M[0])
    woerter = []
    for r in M:
        w = sum(1 << j for j in range(m) if r[j] == -1)
        woerter.append(w)
        woerter.append(w ^ ((1 << m) - 1))
    assert len(set(woerter)) == len(woerter)
    pts = np.arange(1 << m, dtype=np.uint32)
    W = np.array(woerter, dtype=np.uint32)
    INF = 10 ** 6
    radius = 0
    # Abstände je Wort, und Minimum je Klasse (Koordinate i = 0 bzw. 1)
    min0 = np.full((m, 1 << m), INF, dtype=np.int32)
    min1 = np.full((m, 1 << m), INF, dtype=np.int32)
    gesamt = np.full(1 << m, INF, dtype=np.int32)
    for w in woerter:
        dist = np.bitwise_count(pts ^ np.uint32(w)).astype(np.int32)
        np.minimum(gesamt, dist, out=gesamt)
        for i in range(m):
            if (w >> i) & 1:
                np.minimum(min1[i], dist, out=min1[i])
            else:
                np.minimum(min0[i], dist, out=min0[i])
    radius = int(gesamt.max())
    normen = [int((min0[i] + min1[i]).max()) for i in range(m)]
    print(f"{len(woerter)} Wörter, Länge {m}, Überdeckungsradius {radius} (2R+1 = {2 * radius + 1})")
    print(f"Norm je Koordinate: {normen}")
    print(f"normal (alle Normen <= 2R+1): {all(N <= 2 * radius + 1 for N in normen)}")


def zeugen():
    """Gespeicherte Zeugen aus a2 noch einmal komplett ganzzahlig nachrechnen."""
    Ms = mckay(os.path.join(ROOT, "controls", "had24_mckay_hex.txt"))
    n = 0
    schlecht = 0
    maxd = 0
    with gzip.open(os.path.join(OUT, "a2_zeugen_24x21.txt.gz"), "rt") as f:
        for line in f:
            if line.startswith("#"):
                continue
            k, t, w, d, z = line.split()
            M = Ms[int(k) - 1]
            MM = transponiert(M) if t == "T" else M
            weg = {int(v) for v in w.split(",")}
            x = zeuge_zu_x(z, 24, weg)
            v = max_betrag(MM, x)
            n += 1
            maxd = max(maxd, v)
            if v != int(d) or v > 7 or any(x[j] != 0 for j in weg):
                schlecht += 1
    print(f"{n} Zeugen gelesen, größter Betrag {maxd}, Abweichungen/über 7: {schlecht}")


if __name__ == "__main__":
    {"a1": a1, "a2": a2, "a3": a3, "a4": a4, "zeugen": zeugen}[sys.argv[1]]()
