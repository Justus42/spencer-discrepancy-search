#!/usr/bin/env python3
"""Prüfcode zu Teil C (kein Suchcode). Nur ganze Zahlen.

  python3 src/zirkulant_pruef.py zaehle n_max          Klassenzahl per Bahnen-Brute-Force (unabhängig von zirkulant.c)
  python3 src/zirkulant_pruef.py matrix HEX n [-r] DATEI  Matrix aus erster Zeile schreiben
  python3 src/zirkulant_pruef.py exakt PRAEFIX n [-r] [-k MAX]
        exakte Diskrepanz (build/exakt_disc) für Überlebende und bis zu MAX Vertreter mit größtem u
"""
import os
import subprocess
import sys
from math import gcd

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.environ.get("EXAKT_BIN", os.path.join(ROOT, "build", "exakt_disc"))


def bahn(a, n):
    maske = (1 << n) - 1
    bits = [(a >> t) & 1 for t in range(n)]
    out = set()
    for m in range(1, n):
        if gcd(m, n) != 1:
            continue
        b = [bits[(m * t) % n] for t in range(n)]
        for k in range(n):
            v = sum(b[(t + k) % n] << t for t in range(n))
            out.add(v)
            out.add(v ^ maske)
    return out


def zaehle(nmax):
    for n in range(2, nmax + 1):
        gesehen = set()
        kl = 0
        for a in range(1 << n):
            if a in gesehen:
                continue
            kl += 1
            gesehen |= bahn(a, n)
        print(f"n={n}: {kl} Klassen")


def matrix(a, n, rand):
    M = [[(-1 if (a >> ((j - i) % n)) & 1 else 1) for j in range(n)] for i in range(n)]
    if rand:
        M = [r + [0] for r in M] + [[1] * n + [0]]
    return M


def schreibe(M, fn):
    with open(fn, "w") as f:
        for r in M:
            f.write(" ".join(f"{v:2d}" for v in r) + "\n")


def exakt_eins(a, n, rand, fn):
    M = matrix(a, n, rand)
    schreibe(M, fn)
    p = subprocess.run([BIN, fn], capture_output=True, text=True, check=True)
    out = p.stdout.strip()
    d = int(out.split("disc ")[1].split(",")[0])
    z = int(out.split("zeuge ")[1], 16)
    x = [(-1 if (z >> j) & 1 else 1) for j in range(len(M[0]))]
    chk = max(abs(sum(u * v for u, v in zip(r, x))) for r in M)
    assert chk == d, (a, chk, d)
    return d


def exakt(praefix, n, rand, kmax):
    tmp = praefix + "_tmp_matrix.txt"
    ergebnisse = []
    for art in ("ueberlebende", "top"):
        fn = f"{praefix}_{art}.txt"
        if not os.path.exists(fn):
            continue
        zeilen = [l.split() for l in open(fn) if l.strip()]
        if art == "top":
            zeilen = zeilen[:kmax]
        for h, u in zeilen:
            dd = exakt_eins(int(h, 16), n, rand, tmp)
            ergebnisse.append((art, h, int(u), dd))
            print(f"{art} {h} u={u} exakt={dd}", flush=True)
    if os.path.exists(tmp):
        os.remove(tmp)
    return ergebnisse


if __name__ == "__main__":
    cmd = sys.argv[1]
    if cmd == "zaehle":
        zaehle(int(sys.argv[2]))
    elif cmd == "matrix":
        schreibe(matrix(int(sys.argv[2], 16), int(sys.argv[3]), "-r" in sys.argv), sys.argv[-1])
    elif cmd == "exakt":
        kmax = int(sys.argv[sys.argv.index("-k") + 1]) if "-k" in sys.argv else 20
        exakt(sys.argv[2], int(sys.argv[3]), "-r" in sys.argv, kmax)
