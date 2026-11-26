#!/usr/bin/env python3
"""PRUEFCODE Auftrag M, Schritt 1: vollstaendiges Durchprobieren fuer kleine Faelle, Vergleich mit build/misch_frei.

Erzeugt kleine Faelle (Laenge m, Radius R, zufaellige offene Punktmengen, abgeschlossen unter x -> -x),
entscheidet f = 1 und f = 2 durch Durchprobieren ALLER Zeilen bzw. ALLER Zeilenpaare (r1, r2) aus {+1,-1}^m
(eigener Code, nur ganze Zahlen) und vergleicht mit der Ausgabe von build/misch_frei (getrennter Prozess).
Aufruf: brute_frei.py ANZAHL STARTWERT AUSGABEORDNER
"""
import random
import re
import subprocess
import sys
from pathlib import Path

WURZEL = Path(__file__).resolve().parent.parent.parent


def abstand(a, b):
    return bin(a ^ b).count("1")


def deckt(r, x, m, R):
    p = abstand(r, x)
    return p <= R or p >= m - R


def vektor(x, m):
    return [1 - 2 * ((x >> j) & 1) for j in range(m)]


def fall(rng, m, R, art):
    voll = (1 << m) - 1
    if art == "ja1":   # Teilmenge einer Kugel (mit Gegenkugel)
        r1 = rng.randrange(1 << m)
        kand = [x for x in range(1 << m) if deckt(r1, x, m, R)]
        P = set(rng.sample(kand, rng.randint(2, len(kand))))
    elif art == "ja2":   # Teilmenge der Vereinigung zweier Kugeln (mit Gegenkugeln)
        r1, r2 = rng.randrange(1 << m), rng.randrange(1 << m)
        kand = [x for x in range(1 << m) if deckt(r1, x, m, R) or deckt(r2, x, m, R)]
        P = set(rng.sample(kand, min(len(kand), rng.randint(4, 3 * len(kand) // 4))))
    else:              # zufaellige Punkte
        P = set(rng.randrange(1 << m) for _ in range(rng.randint(2, 12) if art == "zufall" else rng.randint(12, 40)))
    P |= {x ^ voll for x in P}
    return sorted(P)


def brute(P, m, R):
    maske = []
    for r in range(1 << m):
        b = 0
        for i, x in enumerate(P):
            if deckt(r, x, m, R):
                b |= 1 << i
        maske.append(b)
    alle = (1 << len(P)) - 1
    n1 = sum(1 for b in maske if b == alle)
    maxcov = max(bin(b).count("1") for b in maske)
    f2 = n1 > 0
    if not f2:
        verschieden = sorted(set(maske))
        for i, a in enumerate(verschieden):
            if f2:
                break
            for b in verschieden[i:]:
                if a | b == alle:
                    f2 = True
                    break
    return n1, maxcov, f2


def main():
    anzahl, seed, ordner = int(sys.argv[1]), int(sys.argv[2]), Path(sys.argv[3])
    ordner.mkdir(parents=True, exist_ok=True)
    rng = random.Random(seed)
    abw = 0
    stat = {"f1 ja": 0, "f2 ja": 0, "f2 nein": 0}
    for k in range(anzahl):
        m = rng.choice([7, 8, 9, 10])
        R = rng.choice([1, 2, 2, 3]) if m >= 9 else rng.choice([1, 2])
        art = ("ja1", "ja2", "zufall", "gross")[k % 4]
        P = fall(rng, m, R, art)
        n1, maxcov, f2 = brute(P, m, R)
        datei = ordner / f"fall{k:02d}.txt"
        datei.write_text("".join(" ".join(f"{v:2d}" for v in vektor(x, m)) + "\n" for x in P), encoding="utf-8")
        out = subprocess.run([str(WURZEL / "build" / "misch_frei"), "punkte", str(datei), str(m), str(R), "2"],
                             capture_output=True, text=True).stdout
        c_n1 = int(re.search(r"f=1: Zeilen r .*?: (\d+)", out).group(1))
        c_max = int(re.search(r"hoechstens (\d+) offene Punkte", out).group(1))
        c_f1 = "f=1: JA" in out
        c_f2 = c_f1 or "f=2: JA" in out
        ok = (c_n1 == n1 and c_max == maxcov and c_f1 == (n1 > 0) and c_f2 == f2)
        abw += not ok
        stat["f1 ja"] += n1 > 0
        stat["f2 ja" if f2 else "f2 nein"] += 1
        print(f"Fall {k:2d}: m={m} R={R} {art:6s} |P|={len(P):3d}  brute: f1-Zeilen {n1:4d} maxcov {maxcov:3d} f2 {'ja' if f2 else 'nein':4s}"
              f" | misch_frei: {c_n1:4d} {c_max:3d} {'ja' if c_f2 else 'nein':4s}  {'OK' if ok else 'ABWEICHUNG'}")
    print(f"Faelle {anzahl}, Abweichungen {abw}; {stat}")
    sys.exit(1 if abw else 0)


if __name__ == "__main__":
    main()
