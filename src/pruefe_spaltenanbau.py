#!/usr/bin/env python3
"""Prüfcode (kein Suchcode): Spaltenanbau an die Paley-Doppelung 24 x 20.

Aussage: An had24_1p3_paley12 / had24_2p2_paley12 (24 x 20, disc 8) hebt keine
21. Spalte mit Einträgen in [-1,1] die Diskrepanz auf 9. Beleg je Matrix: ein
Vorzeichenvektor x mit max|Ax| = 8 in genau einer Zeile i0, alle anderen <= 6.

Schritte:
 1. Zählung aller 2^20 Vorzeichenvektoren (numpy, nur Ganzzahlen int32):
    Maximum 8 und genau eine Maximalzeile; über alle x und bis auf Vorzeichen (x_1 = +1).
 2. Zeuge speichern (runs/spaltenanbau/) und mit reinen Python-Ganzzahlen nachrechnen.
 3. Gegenprobe: 10 zufällige ±1-Spalten, beide Vorzeichen s, kleineres Maximum <= 8.

Aufruf: python3 src/pruefe_spaltenanbau.py [--seed 20261124]
"""
import argparse
import os
import random
import sys

import numpy as np

MATRIZEN = ["controls/had24_1p3_paley12.txt", "controls/had24_2p2_paley12.txt"]
ERWARTET = {"had24_1p3_paley12": 4968, "had24_2p2_paley12": 6048}
AUSGABE = "runs/spaltenanbau"


def lies(pfad):
    A = []
    with open(pfad) as f:
        for z in f:
            if z.strip():
                A.append([int(t) for t in z.split()])
    breite = {len(r) for r in A}
    assert len(breite) == 1, "nicht rechteckig"
    for r in A:
        for v in r:
            assert v in (-1, 1), "Eintrag nicht ±1"
    return A


def zaehle(A):
    """Schritt 1: alle 2^n Vektoren. Bit j von k = 1 bedeutet x_j = -1."""
    M = np.array(A, dtype=np.int32)
    m, n = M.shape
    gesamt = 0
    halb = 0  # x_1 = +1, also Bit 0 = 0
    zeuge = None
    disc = None
    block = 1 << 16
    spalten_bits = np.arange(n, dtype=np.int64)
    for start in range(0, 1 << n, block):
        k = np.arange(start, start + block, dtype=np.int64)
        X = (1 - 2 * ((k[:, None] >> spalten_bits[None, :]) & 1)).astype(np.int32)
        W = np.abs(X @ M.T)  # block x m
        mx = W.max(axis=1)
        d = int(mx.min())
        disc = d if disc is None else min(disc, d)
        anz = (W == mx[:, None]).sum(axis=1)
        treffer = (mx == 8) & (anz == 1)
        gesamt += int(treffer.sum())
        halb += int((treffer & ((k & 1) == 0)).sum())
        if zeuge is None and treffer.any():
            i = int(np.argmax(treffer))
            zeuge = [int(v) for v in X[i]]
    return disc, gesamt, halb, zeuge


def zeilenwerte(A, x):
    return [sum(a * b for a, b in zip(r, x)) for r in A]


def pruefe_zeuge(A, x):
    """Schritt 2, nur Python-Ganzzahlen, unabhängig von numpy."""
    assert len(x) == len(A[0]) and all(v in (-1, 1) for v in x)
    w = zeilenwerte(A, x)
    b = [abs(v) for v in w]
    i0 = [i for i, v in enumerate(b) if v == 8]
    ok = max(b) == 8 and len(i0) == 1 and all(v <= 6 for i, v in enumerate(b) if i != i0[0])
    return ok, w, i0


def gegenprobe(A, x, rng, anzahl=10):
    w = zeilenwerte(A, x)
    zeilen = []
    for t in range(anzahl):
        b = [rng.choice((-1, 1)) for _ in A]
        maxima = {s: max(abs(w[i] + s * b[i]) for i in range(len(A))) for s in (1, -1)}
        kleiner = min(maxima.values())
        zeilen.append((t, "".join("+" if v > 0 else "-" for v in b), maxima[1], maxima[-1], kleiner))
    return zeilen


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seed", type=int, default=20261124)
    args = ap.parse_args()
    os.makedirs(AUSGABE, exist_ok=True)
    rng = random.Random(args.seed)
    fehler = 0
    for pfad in MATRIZEN:
        name = os.path.basename(pfad)[:-4]
        A = lies(pfad)
        print(f"== {name}: {len(A)} x {len(A[0])}, Einträge nur ±1")
        disc, gesamt, halb, zeuge = zaehle(A)
        print(f"Schritt 1: disc (Brute-Force 2^{len(A[0])}) = {disc}")
        print(f"Schritt 1: Max 8, genau eine Maximalzeile: über alle x = {gesamt}, bis auf Vorzeichen = {halb}")
        soll = ERWARTET[name]
        if gesamt == soll:
            lesart = "über alle Vektoren"
        elif halb == soll:
            lesart = "bis auf globales Vorzeichen"
        else:
            lesart = "KEINE Lesart passt"
            fehler += 1
        print(f"Schritt 1: Prüfvermerk {soll} -> {lesart}")
        if zeuge is None:
            print("Schritt 2: KEIN Zeuge gefunden")
            fehler += 1
            continue
        datei = os.path.join(AUSGABE, f"zeuge_{name}.txt")
        with open(datei, "w") as f:
            f.write(" ".join(str(v) for v in zeuge) + "\n")
        # aus Datei zurücklesen und unabhängig nachrechnen
        with open(datei) as f:
            x = [int(t) for t in f.read().split()]
        ok, w, i0 = pruefe_zeuge(A, x)
        print(f"Schritt 2: Zeuge {datei}: x = {' '.join(str(v) for v in x)}")
        print(f"Schritt 2: Ax = {w}")
        print(f"Schritt 2: Zeile i0 = {i0} (0-basiert), Max 8 einmal, Rest <= 6: {'OK' if ok else 'FEHLER'}")
        if not ok:
            fehler += 1
        print("Schritt 3: Spalte b (Zeile 0..23) | max bei s=+1 | max bei s=-1 | kleineres")
        for t, bs, p, m_, k in gegenprobe(A, x, rng):
            st = "OK" if k <= 8 else "FEHLER"
            if k > 8:
                fehler += 1
            print(f"  {t}: {bs} | {p} | {m_} | {k} {st}")
    print("ERGEBNIS:", "alles OK" if fehler == 0 else f"{fehler} Fehler")
    sys.exit(1 if fehler else 0)


if __name__ == "__main__":
    main()
