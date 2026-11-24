#!/usr/bin/env python3
"""Zweite, unabhaengige Nachrechnung der Gegenbeispiele zu Test C (Doppelung Ordnung 40).

Prueffcode, nur ganze Zahlen, nur Standardbibliothek. Importiert nichts aus src/hadamard_tests.py.
Bauregel (aus dem Kopf von src/gegenbeispiel40.c gelesen): M = [[H, H], [H, -H]] (40 x 40), H aus
controls/had20_klasse{k}.txt; links (Spalten 0..19 von M) und rechts (Spalten 20..39) werden die
genannten Spalten von H gestrichen, Reihenfolge bleibt, Ergebnis 40 x 36. Vektor: Zeichen '1' = -1.

Prueft je Log: jede Auswahl des Musters kommt genau einmal vor, Muster passt (13: links 1, rechts 3,
links in rechts; 22: je 2, Ueberlappung genau 1), max_i |(A x)_i| selbst berechnet, gleich dem im Log
genannten Wert und <= Schranke T.

Aufruf: python3 src/gegenrechnung_testc.py   (alle 12 Logs in runs/, T = 10 und T = 8)
"""
import itertools
import sys


def lade_h(pfad):
    H = [[int(t) for t in z.split()] for z in open(pfad) if z.strip()]
    assert len(H) == 20 and all(len(z) == 20 for z in H), pfad
    assert all(v in (1, -1) for z in H for v in z), pfad
    for a in range(20):  # Hadamard: H H^T = 20 I
        for b in range(20):
            s = sum(H[a][j] * H[b][j] for j in range(20))
            assert s == (20 if a == b else 0), (pfad, a, b)
    return H


def alle_auswahlen(muster):
    if muster == 13:
        return {((l,), r) for l in range(20) for r in itertools.combinations(range(20), 3) if l in r}
    return {(l, r) for l in itertools.combinations(range(20), 2) for r in itertools.combinations(range(20), 2)
            if len(set(l) & set(r)) == 1}


def matrix(H, links, rechts):
    kl = [j for j in range(20) if j not in links]
    kr = [j for j in range(20) if j not in rechts]
    oben = [[H[i][j] for j in kl] + [H[i][j] for j in kr] for i in range(20)]
    unten = [[H[i][j] for j in kl] + [-H[i][j] for j in kr] for i in range(20)]
    return oben + unten


def pruefe_log(H, muster, logpfad, T):
    soll = alle_auswahlen(muster)
    gesehen = set()
    fehler = []
    groesst = 0
    kleinst = None
    n = 0
    for zeile in open(logpfad):
        t = zeile.split()
        if not t or t[0] != "SEL":
            continue
        if int(t[1]) != muster:
            fehler.append(("muster", zeile[:60]))
            continue
        links = tuple(int(v) for v in t[2].split(","))
        rechts = tuple(int(v) for v in t[3].split(","))
        key = (links, rechts)
        if key in gesehen:
            fehler.append(("doppelt", key))
        gesehen.add(key)
        if key not in soll:
            fehler.append(("nicht im Muster", key))
            continue
        if t[4] != "gefunden":
            fehler.append(("kein Vektor", key))
            continue
        bits = t[5]
        if len(bits) != 36 or set(bits) - {"0", "1"}:
            fehler.append(("Vektor ungueltig", key))
            continue
        x = [-1 if b == "1" else 1 for b in bits]
        A = matrix(H, links, rechts)
        assert len(A) == 40 and all(len(z) == 36 for z in A)
        m = max(abs(sum(a * b for a, b in zip(z, x))) for z in A)
        n += 1
        groesst = max(groesst, m)
        kleinst = m if kleinst is None else min(kleinst, m)
        if m != int(t[6]):
            fehler.append((f"Log sagt {t[6]}, gerechnet {m}", key))
        if m > T:
            fehler.append((f"max {m} > {T}", key))
    fehlt = len(soll - gesehen)
    return n, fehlt, fehler, groesst, kleinst


def main():
    gesamt = {10: 0, 8: 0}
    alles_ok = True
    for k in (1, 2, 3):
        H = lade_h(f"controls/had20_klasse{k}.txt")
        for muster in (13, 22):
            for T in (10, 8):
                log = f"runs/2026-11-24_testC_klasse{k}_muster{muster}_T{T}.log.txt"
                n, fehlt, fehler, g, kl = pruefe_log(H, muster, log, T)
                ok = not fehler and fehlt == 0
                alles_ok &= ok
                gesamt[T] += n
                print(f"{log}: nachgerechnet {n}, fehlend {fehlt}, Fehler {len(fehler)}, max {g}, min {kl}, "
                      f"{'OK' if ok else 'ABWEICHUNG'}")
                for f in fehler[:5]:
                    print("   ", f)
    print(f"GESAMT T=10: {gesamt[10]} Vektoren, T=8: {gesamt[8]} Vektoren, {'ALLE OK' if alles_ok else 'ABWEICHUNG'}")
    sys.exit(0 if alles_ok else 1)


if __name__ == "__main__":
    main()
