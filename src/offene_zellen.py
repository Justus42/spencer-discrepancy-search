#!/usr/bin/env python3
"""Offene Zellen der Reduktion gegen die Post-Keri-Tabelle (florath/covering-codes-lean).

Prueffcode, nur ganze Zahlen und Brueche, nur Standardbibliothek.
Eingabe: reference-data/post-keri/non_mixed_covering_codes.csv am Commit
bbed9a690e6c54f8bba423031d4d516f70d267b9 (siehe controls/QUELLEN.md). Die Prueffsumme wird geprueft,
bei Abweichung bricht das Skript ab. Die Datei liegt nicht im Repo; Abruf z. B. mit
  curl -o non_mixed_covering_codes.csv https://raw.githubusercontent.com/florath/covering-codes-lean/\
bbed9a690e6c54f8bba423031d4d516f70d267b9/reference-data/post-keri/non_mixed_covering_codes.csv

Regeln (Angabe des Auftrags vom 24.11.2026):
  B(d) = groesste ganze Zahl mit 25*B < 8*d*d.
  Fuer d = 7..11 und Laenge m <= min(B, 33), m gleicher Paritaet wie d, m >= d: R = (m - d)/2.
  Zelle offen, wenn untere Schranke K(m,R) <= 2*B. Dichte = 2*B * V(m,R) / 2^m.

Aufruf: python3 src/offene_zellen.py PFAD_ZUR_CSV
"""
import csv
import hashlib
import sys
from fractions import Fraction
from math import comb

SHA256 = "5efaf8d94a3ef15e9efc052233259697fc5f0b5e51389bcc5c8c108b6c3ad1d6"


def B(d):
    b = 0
    while 25 * (b + 1) < 8 * d * d:
        b += 1
    return b


def V(m, R):
    return sum(comb(m, i) for i in range(R + 1))


def zwei_stellen(fr):
    """Bruch kaufmaennisch auf 2 Nachkommastellen, deutsches Komma."""
    h = (fr * 100 * 2 + 1) // 2  # floor(100x + 1/2)
    return f"{h // 100},{h % 100:02d}"


def main():
    pfad = sys.argv[1]
    roh = open(pfad, "rb").read()
    ist = hashlib.sha256(roh).hexdigest()
    if ist != SHA256:
        sys.exit(f"Prueffsumme falsch: {ist}")
    print(f"CSV {pfad}: {len(roh)} Bytes, SHA-256 {ist} (gleich controls/QUELLEN.md)")
    tab = {}
    for z in csv.DictReader(roh.decode("utf-8-sig").splitlines()):
        if z["q"] == "2":
            tab[(int(z["n"]), int(z["r"]))] = (int(z["lower_bound"]), int(z["upper_bound"]))
    offen = 0
    for d in range(7, 12):
        b = B(d)
        print(f"\nd={d}: B={b}, Woerter hoechstens 2B={2 * b}")
        for m in range(d, min(b, 33) + 1, 2):
            R = (m - d) // 2
            if (m, R) not in tab:
                print(f"  ({m},{R}): FEHLT in der Tabelle")
                continue
            lo, hi = tab[(m, R)]
            dichte = Fraction(2 * b * V(m, R), 2 ** m)
            zu = "offen" if lo <= 2 * b else "geschlossen"
            offen += zu == "offen"
            print(f"  ({m},{R}): {lo}-{hi}, Dichte {zwei_stellen(dichte)}, {zu}")
    print(f"\nOffene Zellen gesamt: {offen}")


if __name__ == "__main__":
    main()
