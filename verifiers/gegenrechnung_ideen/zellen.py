#!/usr/bin/env python3
"""Auftrag C (Sitzung, 26.11.2026): Verdopplungszellen (Ideennotiz 3e) gegen die Post-Kéri-Tabelle. Pruefcode.

Ein selbstkomplementaerer Code mit K Woertern in (m, R) gibt mit einer angehaengten Spalte (Zeilen (u,+1), (u,-1)) einen Code
mit 2K Woertern in (m+1, R), also d = m + 1 - 2R. Rekord, wenn 2K <= 2 B(d) (B(d) = groesste ganze Zahl mit 25 B < 8 d^2)
und m + 1 <= min(B(d), 33). K ist gerade (selbstkomplementaer), also K <= Kmax = groesste gerade Zahl <= B(d).
Zelle offen nach Tabelle: untere Schranke K(m,R) <= Kmax. Fehlende Tabellenzeilen werden gemeldet.
Abruf wie src/offene_zellen.py: florath/covering-codes-lean, Commit bbed9a6, reference-data/post-keri/non_mixed_covering_codes.csv,
SHA-256 5efaf8d9... (controls/QUELLEN.md). Aufruf: python3 zellen.py CSV"""
import csv
import hashlib
import sys

SOLL = "5efaf8d94a3ef15e9efc052233259697fc5f0b5e51389bcc5c8c108b6c3ad1d6"


def B(d):
    b = 0
    while 25 * (b + 1) < 8 * d * d:
        b += 1
    return b


def main():
    roh = open(sys.argv[1], "rb").read()
    ist = hashlib.sha256(roh).hexdigest()
    print(f"SHA-256 {ist}: {'gleich' if ist == SOLL else 'ABWEICHUNG'}")
    if ist != SOLL:
        sys.exit(1)
    tab = {}
    for z in csv.DictReader(roh.decode("utf-8-sig").splitlines()):
        if z["q"] == "2":
            tab[(int(z["n"]), int(z["r"]))] = (int(z["lower_bound"]), int(z["upper_bound"]))
    offen = []
    for d in range(8, 12):
        b = B(d); kmax = b - b % 2
        for R in range(1, 40):
            m = d - 1 + 2 * R
            if m + 1 > min(b, 33):
                break
            if (m, R) not in tab:
                print(f"d = {d}: Zelle ({m},{R}) fehlt in der Tabelle")
                continue
            lo, hi = tab[(m, R)]
            zust = "offen" if lo <= kmax else "zu"
            print(f"d = {d}, B = {b}, Woerter <= {kmax}: ({m},{R}) Tabelle {lo}-{hi}: {zust}")
            if lo <= kmax:
                offen.append((d, m, R, lo, hi))
    print(f"offene Verdopplungszellen: {len(offen)}")
    for d, m, R, lo, hi in offen:
        print(f"  d = {d}: ({m},{R}) {lo}-{hi}")


if __name__ == "__main__":
    main()
