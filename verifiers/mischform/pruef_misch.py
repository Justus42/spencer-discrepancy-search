#!/usr/bin/env python3
"""PRUEFCODE Auftrag M (26.11.2026): Mischform "K Nebenklassen plus f freie Zeilen". Teilt keinen Code mit src/.

Nur ganze Zahlen (numpy int64). Ein Punkt x in {+1,-1}^m ist ueberdeckt, wenn |<Zeile, x>| >= d fuer eine Zeile.
Gezaehlt wird immer ueber ALLE 2^m Punkte (nicht nur Klassen).

Aufrufe:
  pruef_misch.py offen MATRIX D               offene Punkte ueber alle 2^m Punkte, Zahl verschiedener Woerter (Zeilen und Negative)
  pruef_misch.py zeilen MATRIX D              offene Punkte wie oben; dann ALLE r in {+1,-1}^m, die alle offenen Punkte ueberdecken
                                               (vollstaendig, alle 2^m Kandidaten), Anzahl und Liste (bis auf Vorzeichen)
  pruef_misch.py paar PAAR [FREI] AUSGABE     baut die Matrix: Zeilen (U_i, W_i), (U_i, -W_i), dann die freien Zeilen (Datei FREI)
MATRIX/PAAR/FREI: Eintraege +1/-1, Leerraum; PAAR-Zeilen "U_i | W_i"; '#' beginnt einen Kommentar.
"""
import sys

import numpy as np


def lies_zeilen(pfad):
    zeilen = []
    for roh in open(pfad, encoding="utf-8"):
        roh = roh.split("#", 1)[0].strip()
        if roh:
            zeilen.append(roh)
    return zeilen


def lies_matrix(pfad):
    A = [[int(t) for t in z.split()] for z in lies_zeilen(pfad)]
    if not A or len({len(z) for z in A}) != 1:
        raise SystemExit(f"{pfad}: leere Datei oder ungleiche Zeilenlaengen")
    A = np.array(A, dtype=np.int64)
    if not set(np.unique(A).tolist()) <= {-1, 1}:
        raise SystemExit(f"{pfad}: Eintraege nicht +-1")
    return A


def alle_punkte(m, start, anzahl):
    """Punkte mit Nummer start..start+anzahl-1; Bit j der Nummer = 1 heisst Eintrag j = -1."""
    idx = np.arange(start, start + anzahl, dtype=np.int64)
    X = np.empty((anzahl, m), dtype=np.int64)
    for j in range(m):
        X[:, j] = 1 - 2 * ((idx >> j) & 1)
    return X


def offene_punkte(A, d):
    m = A.shape[1]
    N = 1 << m
    blk = 1 << 16
    offen = []
    for s in range(0, N, blk):
        n = min(blk, N - s)
        X = alle_punkte(m, s, n)
        S = np.abs(X @ A.T)                      # n x Zeilen, ganze Zahlen
        ok = (S >= d).any(axis=1)
        offen.extend((s + np.nonzero(~ok)[0]).tolist())
    return offen


def worte(A):
    w = set()
    for z in A.tolist():
        w.add(tuple(z))
        w.add(tuple(-v for v in z))
    return len(w)


def als_vektor(nummer, m):
    return [1 - 2 * ((nummer >> j) & 1) for j in range(m)]


def main():
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    modus = sys.argv[1]
    if modus in ("offen", "zeilen"):
        A = lies_matrix(sys.argv[2])
        d = int(sys.argv[3])
        m = A.shape[1]
        off = offene_punkte(A, d)
        print(f"# {sys.argv[2]}: {A.shape[0]} Zeilen x {m} Spalten, {worte(A)} verschiedene Woerter, d = {d}")
        print(f"offene Punkte (von 2^{m} = {1 << m}): {len(off)}")
        if modus == "offen":
            return
        if not off:
            print("keine offenen Punkte; jede Zeile genuegt")
            return
        P = np.array([als_vektor(p, m) for p in off], dtype=np.int64)   # alle offenen Punkte (nicht nur Klassen)
        for p in off:
            print("  offen:", " ".join(f"{v:2d}" for v in als_vektor(p, m)))
        N = 1 << m
        blk = 1 << 16
        treffer = []
        for s in range(0, N, blk):
            n = min(blk, N - s)
            Rm = alle_punkte(m, s, n)
            S = np.abs(Rm @ P.T)
            ok = (S >= d).all(axis=1)
            treffer.extend((s + np.nonzero(ok)[0]).tolist())
        print(f"Zeilen r (alle 2^{m} Kandidaten), die alle {len(off)} offenen Punkte ueberdecken: {len(treffer)}"
              f" (bis auf Vorzeichen {len(treffer) // 2})")
        for t in treffer:
            if t & 1 == 0:   # Vertreter mit erstem Eintrag +1
                print("  r:", " ".join(f"{v:2d}" for v in als_vektor(t, m)))
        return
    if modus == "paar":
        U, W = [], []
        for z in lies_zeilen(sys.argv[2]):
            l, r = z.split("|")
            U.append([int(t) for t in l.split()])
            W.append([int(t) for t in r.split()])
        zeilen = []
        for u, w in zip(U, W):
            zeilen.append(u + w)
            zeilen.append(u + [-v for v in w])
        aus = sys.argv[-1]
        if len(sys.argv) == 5:
            zeilen.extend(lies_matrix(sys.argv[3]).tolist())
        with open(aus, "w", encoding="utf-8") as f:
            for z in zeilen:
                f.write(" ".join(f"{v:2d}" for v in z) + "\n")
        print(f"{aus}: {len(zeilen)} Zeilen")
        return
    raise SystemExit("unbekannter Modus")


if __name__ == "__main__":
    main()
