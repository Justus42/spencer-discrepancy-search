#!/usr/bin/env python3
"""ueberdeckung.py -- Pruefcode Auftrag K, Schritt 0 c) und Trefferpruefung (teilt keinen Code mit src/kern3_such.c).

Fuer einen Typ (n0,n1,n2,n3) (Bloecke hintereinander, Label (a_j,b_j) = (i&1, i>>1)) und K Vertreter c_i (0/1-Strings,
Zeichen j = Spalte j) zaehlt das Skript die offenen Punkte auf drei Wegen:
  (A) direkt: alle 2^m Punkte x, gedeckt wenn ein Wort c_i + d (d in D, 8K Woerter) Abstand <= R hat (numpy);
  (B) Nebenklassenrechnung in Python: Klasse von x = min(x xor d) ueber d in D; S = Klassen mit Abstand <= R zu D;
      Klasse y offen, wenn keine Klasse von (y xor c_i) in S liegt; offene Punkte = 8 * offene Klassen;
  (C) Suchcode: build/kern3_such eval (getrennter Prozess), Ausgabe OFFEN_PUNKTE.
Abweichung zwischen A, B, C -> Fehlercode 1.

Aufrufe:
  ueberdeckung.py pruefe m R n0 n1 n2 n3 VERTRETER...      eine Menge pruefen
  ueberdeckung.py zufall ANZAHL SEED                         Schritt 0 c): zufaellige Mengen in (16,4), verschiedene Typen
  ueberdeckung.py treffer m R n0 n1 n2 n3 AUSGABEDATEI      Vertreter aus einer Suchausgabe (Zeilen 'VERTRETER ...') pruefen
"""
import random
import subprocess
import sys
from pathlib import Path

import numpy as np

WURZEL = Path(__file__).resolve().parent.parent.parent
SUCHER = WURZEL / "build" / "kern3_such"
PC16 = np.array([bin(i).count("1") for i in range(1 << 16)], dtype=np.int32)


def popc(x):
    return PC16[x & 0xFFFF] + PC16[(x >> 16) & 0xFFFF]


def kern(n):
    m = sum(n)
    blk = []
    o = 0
    for v in n:
        blk.append(((1 << v) - 1) << o)
        o += v
    eins = (1 << m) - 1
    a = blk[1] | blk[3]
    b = blk[2] | blk[3]
    D = [0, a, b, a ^ b, eins, eins ^ a, eins ^ b, eins ^ a ^ b]
    return m, D


def str2int(s):
    return sum(1 << j for j, ch in enumerate(s) if ch == "1")


def weg_A(n, R, vert):
    m, D = kern(n)
    x = np.arange(1 << m, dtype=np.int64)
    gedeckt = np.zeros(x.shape, dtype=bool)
    for c in vert:
        for d in D:
            gedeckt |= popc(x ^ (c ^ d)) <= R
    return int((~gedeckt).sum())


def weg_B(n, R, vert):
    m, D = kern(n)
    klasse = {}
    for x in range(1 << m):
        klasse[x] = min(x ^ d for d in D)
    S = set()
    for x in range(1 << m):
        if min(bin(x ^ d).count("1") for d in D) <= R:
            S.add(klasse[x])
    reps = sorted(set(klasse.values()))
    assert len(reps) == (1 << (m - 3))
    offen = 0
    for y in reps:
        if not any(klasse[y ^ c] in S for c in vert):
            offen += 1
    return 8 * offen


def weg_C(n, R, vert_str):
    m = sum(n)
    p = subprocess.run([str(SUCHER), "eval", str(m), str(R)] + [str(v) for v in n], input="\n".join(vert_str) + "\n",
                       capture_output=True, text=True, timeout=600)
    for zeile in p.stdout.splitlines():
        if zeile.startswith("OFFEN_KLASSEN"):
            t = zeile.split()
            return int(t[3])
    raise RuntimeError("keine Ausgabe vom Suchcode: " + p.stdout + p.stderr)


def pruefe(m, R, n, vert_str, name=""):
    assert sum(n) == m
    vert = [str2int(s) for s in vert_str]
    a = weg_A(n, R, vert)
    b = weg_B(n, R, vert)
    c = weg_C(n, R, vert_str)
    ok = (a == b == c)
    print("%s Typ=%s K=%d offen: direkt=%d nebenklassen=%d suchcode=%d %s" % (name, n, len(vert), a, b, c, "OK" if ok else "ABWEICHUNG"), flush=True)
    return ok, a


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    mod = sys.argv[1]
    if mod == "pruefe":
        m, R = int(sys.argv[2]), int(sys.argv[3])
        n = tuple(int(v) for v in sys.argv[4:8])
        ok, _ = pruefe(m, R, n, sys.argv[8:])
        return 0 if ok else 1
    if mod == "treffer":
        m, R = int(sys.argv[2]), int(sys.argv[3])
        n = tuple(int(v) for v in sys.argv[4:8])
        vert = [z.split()[1] for z in open(sys.argv[8]) if z.startswith("VERTRETER")]
        ok, a = pruefe(m, R, n, vert, "TREFFER")
        return 0 if (ok and a == 0) else 1
    if mod == "zufall":
        anz, seed = int(sys.argv[2]), int(sys.argv[3])
        rnd = random.Random(seed)
        typen44 = [(4, 4, 4, 4), (5, 4, 4, 3), (5, 5, 3, 3), (6, 5, 3, 2), (8, 3, 3, 2), (7, 7, 1, 1), (9, 6, 1, 0),
                   (10, 3, 3, 0), (6, 6, 4, 0), (8, 4, 2, 2), (12, 2, 2, 0), (14, 1, 1, 0)]
        fehler = 0
        for i in range(anz):
            n = typen44[i % len(typen44)]
            K = rnd.randint(1, 8)
            vert = ["".join(rnd.choice("01") for _ in range(16)) for _ in range(K)]
            ok, _ = pruefe(16, 4, n, vert, "Z%02d" % i)
            fehler += 0 if ok else 1
        print("ABWEICHUNGEN %d von %d" % (fehler, anz))
        return 0 if fehler == 0 else 1
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main())
