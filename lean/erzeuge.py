#!/usr/bin/env python3
"""Erzeuger: Kontrolldatei (±1-Matrix, eine Zeile je Textzeile) -> Lean-Datei mit der Matrix.

Prüft vor dem Schreiben:
  * SHA-256 der Kontrolldatei gleich --sha256,
  * Form: alle Zeilen gleich lang, nur Einträge -1, 0, 1; Nullen nur als ganze Nullspalten und nur mit
    --ohne-nullspalten (die werden gestrichen); danach nur noch ±1,
  * Zeilenzahl gleich --zeilen und Länge gleich --laenge (nach dem Streichen, vor --ohne-zeile).

Schreibt lean/Ueberdeckung/Daten/<Modul>.lean. Die Matrix wird unverändert übernommen (Einträge als ganze
Zahlen); Wörter, Komplemente und Abstände bildet erst Lean (`Ueberdeckung.code`). Erzeugte Dateien nie von Hand ändern.

Aufruf:  python3 lean/erzeuge.py DATEI --sha256 HEX --zeilen N --laenge M --modul NAME
                                [--ohne-nullspalten] [--ohne-zeile I]
"""
import argparse
import hashlib
import os
import sys

HIER = os.path.dirname(os.path.abspath(__file__))


def fehler(text):
    sys.exit("FEHLER: " + text)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("datei")
    p.add_argument("--sha256", required=True)
    p.add_argument("--zeilen", type=int, required=True)
    p.add_argument("--laenge", type=int, required=True)
    p.add_argument("--modul", required=True)
    p.add_argument("--ohne-nullspalten", action="store_true")
    p.add_argument("--ohne-zeile", type=int, default=None,
                   help="Zeile I (ab 0) weglassen; mit ihr fällt ihr Komplement weg (Negativkontrolle)")
    a = p.parse_args()

    if not a.modul.isidentifier() or not a.modul[0].isupper():
        fehler("Modulname muss ein Bezeichner mit Großbuchstaben vorn sein: %r" % a.modul)

    roh = open(a.datei, "rb").read()
    ist = hashlib.sha256(roh).hexdigest()
    if ist != a.sha256.lower():
        fehler("SHA-256 %s statt %s" % (ist, a.sha256))

    zeilen = []
    for nr, text in enumerate(roh.decode("ascii").splitlines(), 1):
        if not text.strip():
            continue
        try:
            zeile = [int(t) for t in text.split()]
        except ValueError:
            fehler("Zeile %d: kein ganzzahliger Eintrag" % nr)
        if any(e not in (-1, 0, 1) for e in zeile):
            fehler("Zeile %d: Eintrag außerhalb von -1, 0, 1" % nr)
        zeilen.append(zeile)
    if not zeilen:
        fehler("leere Datei")
    breite = len(zeilen[0])
    if any(len(z) != breite for z in zeilen):
        fehler("Zeilen verschieden lang")

    null = [j for j in range(breite) if all(z[j] == 0 for z in zeilen)]
    if null and not a.ohne_nullspalten:
        fehler("Nullspalten %s vorhanden, aber --ohne-nullspalten fehlt" % null)
    zeilen = [[e for j, e in enumerate(z) if j not in null] for z in zeilen]
    if any(e not in (-1, 1) for z in zeilen for e in z):
        fehler("nach dem Streichen der Nullspalten noch Einträge außer ±1 (Null außerhalb einer Nullspalte)")
    if len(zeilen) != a.zeilen:
        fehler("%d Zeilen statt %d" % (len(zeilen), a.zeilen))
    if len(zeilen[0]) != a.laenge:
        fehler("Länge %d statt %d" % (len(zeilen[0]), a.laenge))

    if a.ohne_zeile is not None:
        if not 0 <= a.ohne_zeile < len(zeilen):
            fehler("--ohne-zeile außerhalb")
        del zeilen[a.ohne_zeile]

    rel = os.path.relpath(os.path.abspath(a.datei), os.path.dirname(HIER)).replace(os.sep, "/")
    kopf = [
        "-- ERZEUGT von lean/erzeuge.py. Nicht von Hand ändern.",
        "-- Quelle: %s" % rel,
        "-- SHA-256 der Quelle: %s" % ist,
        "-- Zeilen in der Quelle: %d, Länge nach Streichen: %d" % (a.zeilen, a.laenge),
        "-- Gestrichene Nullspalten (Index ab 0): %s" % (null if null else "keine"),
        "-- Weggelassene Zeile (Index ab 0, samt Komplement): %s" % (a.ohne_zeile if a.ohne_zeile is not None else "keine"),
        "-- Bit j eines Worts = 1 genau dann, wenn Eintrag j = -1 (siehe Ueberdeckung.wort).",
        "",
        "namespace Ueberdeckung.%s" % a.modul,
        "",
        "/-- Die ±1-Matrix, Zeile für Zeile. -/",
        "def matrix : List (List Int) := [",
    ]
    rumpf = ["  [%s]%s" % (", ".join(str(e) for e in z), "," if i + 1 < len(zeilen) else "")
             for i, z in enumerate(zeilen)]
    fuss = ["]", "", "end Ueberdeckung.%s" % a.modul, ""]
    ziel = os.path.join(HIER, "Ueberdeckung", "Daten", a.modul + ".lean")
    os.makedirs(os.path.dirname(ziel), exist_ok=True)
    with open(ziel, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(kopf + rumpf + fuss))
    print("geschrieben: %s (%d Zeilen, Länge %d)" % (os.path.relpath(ziel, os.path.dirname(HIER)), len(zeilen), len(zeilen[0])))


if __name__ == "__main__":
    main()
