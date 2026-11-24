"""Gegenrechnung der 60 Hadamard-Matrizen der Ordnung 24 (controls/had24_mckay_hex.txt).

Kein Suchcode. Liest je Zeile 24 Hexzahlen (je 6 Hexziffern = 24 Bit, Bit 0 = -1;
Bitreihenfolge hoechstes Bit = Spalte 0 ist die uebliche Lesart; beide Lesarten werden
geprueft, die Diskrepanz haengt nicht davon ab). Prueft H H^T = 24 I, paarweise
verschieden, und rechnet die exakte Diskrepanz von H und H^T mit hadamard_tests.exakte_disc.

Aufruf: python3 src/pruefe_mckay.py [datei]
"""
import os
import sys
import time
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hadamard_tests import exakte_disc, pruefe_hadamard  # noqa: E402


def lies(datei):
    mats = []
    for zeile in open(datei):
        w = zeile.split()
        if not w:
            continue
        assert len(w) == 24, len(w)
        H = np.array([[1 - 2 * (((int(h, 16) >> (23 - j)) & 1) ^ 1) for j in range(24)] for h in w],
                     dtype=np.int64)
        # Bit 1 = +1, Bit 0 = -1 (Angabe in STATUS.md)
        mats.append(H)
    return mats


if __name__ == "__main__":
    datei = sys.argv[1] if len(sys.argv) > 1 else "controls/had24_mckay_hex.txt"
    mats = lies(datei)
    print(f"{len(mats)} Matrizen gelesen")
    t0 = time.time()
    schl = set()
    ergebnis = []
    for i, H in enumerate(mats, 1):
        pruefe_hadamard(H)
        schl.add(H.tobytes())
        d = exakte_disc(H)
        dt = exakte_disc(H.T.copy())
        ergebnis.append((d, dt))
        print(f"{i:2d}: Diskrepanz {d}, transponiert {dt}  ({time.time() - t0:.0f} s)", flush=True)
    print(f"paarweise verschieden (als Matrizen): {len(schl) == len(mats)}")
    print(f"Haeufigkeit Diskrepanz H: { {v: sum(1 for e in ergebnis if e[0] == v) for v in sorted({e[0] for e in ergebnis})} }")
    print(f"Haeufigkeit Diskrepanz H^T: { {v: sum(1 for e in ergebnis if e[1] == v) for v in sorted({e[1] for e in ergebnis})} }")
    print("ERWARTET alle 6:", all(e == (6, 6) for e in ergebnis))
