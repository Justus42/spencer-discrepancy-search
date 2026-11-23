#!/usr/bin/env python3
"""
Unabhaengiger, exakter Verifier fuer die untere Schranke der Spencer-Konstante.

Mathematische Definition:
Gegeben eine Matrix A in [-1, 1]^(m x n) mit m <= n (falls m < n, implizit mit
Nullzeilen auf n x n aufgefuellt), berechne exakt:
    D(A) = min_{x in {-1, +1}^n} max_{1 <= i <= m} |(A x)_i|
und den zugehoerigen Spencer-Quotienten D(A) / sqrt(n).

Verwendung:
    python verify_spencer.py --selftest
    python verify_spencer.py matrix.json
    python verify_spencer.py matrix.txt --no-zero-skip
"""

from __future__ import annotations

import argparse
import csv
import json
import math
import time
from fractions import Fraction
from pathlib import Path
from typing import List, Tuple

import numpy as np

# Aktueller Rekord: 5 / sqrt(8)  <=>  D^2 / n = 25 / 8
RECORD_NUM = 25
RECORD_DEN = 8


def load_matrix_exact(path: Path) -> List[List[Fraction]]:
    """Laedt eine Matrix exakt als Liste von Fractions (JSON, CSV/Whitespace oder Integer-.npy)."""
    suffix = path.suffix.lower()

    if suffix == ".npy":
        arr = np.load(path, allow_pickle=False)
        if arr.ndim != 2:
            raise ValueError(f".npy muss 2D sein, erhalten: shape={arr.shape}")
        if not np.issubdtype(arr.dtype, np.integer):
            raise ValueError(
                f"Float-.npy abgelehnt (Rundungsrisiko): dtype={arr.dtype}. "
                "Bitte Integer-.npy oder JSON/CSV mit exakten Strings verwenden."
            )
        return [[Fraction(int(v)) for v in row] for row in arr]

    text = path.read_text(encoding="utf-8").strip()
    if suffix == ".json":
        raw = json.loads(text)
        if isinstance(raw, dict):
            for key in ("matrix", "A", "rows", "code"):
                if key in raw:
                    raw = raw[key]
                    break
        if not isinstance(raw, list) or not raw or not isinstance(raw[0], list):
            raise ValueError("JSON muss eine 2D-Liste (oder ein Dict mit key 'matrix'/'A') sein.")
        return [[Fraction(str(v)) for v in row] for row in raw]

    # Fallback: CSV oder Whitespace-getrennte Textdatei
    rows: List[List[Fraction]] = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        if "," in line:
            tokens = [tok.strip() for tok in next(csv.reader([line])) if tok.strip()]
        else:
            tokens = line.split()
        rows.append([Fraction(tok) for tok in tokens])
    if not rows:
        raise ValueError("Keine Matrixzeilen in der Eingabedatei gefunden.")
    return rows


def validate_and_scale(
    matrix: List[List[Fraction]],
) -> Tuple[np.ndarray, int, int, int]:
    """Prueft m <= n und A_ij in [-1, 1] und skaliert exakt auf int64."""
    m = len(matrix)
    n = len(matrix[0])
    if n == 0:
        raise ValueError("Matrix hat 0 Spalten.")
    for i, row in enumerate(matrix):
        if len(row) != n:
            raise ValueError(f"Zeile {i} hat Laenge {len(row)}, erwartet {n}.")
        for j, val in enumerate(row):
            if val < -1 or val > 1:
                raise ValueError(f"Eintrag A[{i},{j}] = {val} liegt ausserhalb [-1, 1].")
    if m > n:
        raise ValueError(
            f"Ungueltige Dimension {m}x{n}: Fuer eine n x n Spencer-Matrix muss m <= n gelten."
        )

    lcm_den = 1
    for row in matrix:
        for val in row:
            lcm_den = math.lcm(lcm_den, val.denominator)

    # Sichere obere Schranke fuer |sum_j B_ij x_j| <= n * lcm_den
    max_possible_sum = n * lcm_den
    if max_possible_sum >= (1 << 62):
        raise OverflowError(
            f"Skalierte Summe (max {max_possible_sum}) zu gross fuer sicheres int64."
        )

    scaled = np.empty((m, n), dtype=np.int64)
    for i, row in enumerate(matrix):
        for j, val in enumerate(row):
            scaled_val = val * lcm_den
            assert scaled_val.denominator == 1
            scaled[i, j] = int(scaled_val.numerator)

    return scaled, lcm_den, m, n


def verify_matrix(
    matrix: List[List[Fraction]],
    chunk_bits: int = 16,
    skip_zero_cols: bool = True,
) -> dict:
    """Berechnet die exakte Diskrepanz ueber alle 2^n Vorzeichenvektoren."""
    t0 = time.perf_counter()
    B, L, m, n = validate_and_scale(matrix)

    if skip_zero_cols:
        active_cols = [j for j in range(n) if np.any(B[:, j] != 0)]
    else:
        active_cols = list(range(n))

    k = len(active_cols)
    num_zero_cols = n - k

    if k == 0:
        return {
            "m": m,
            "n": n,
            "active_cols": 0,
            "scale_L": L,
            "discrepancy": Fraction(0),
            "ratio_float": 0.0,
            "ratio_sq": Fraction(0),
            "beats_record": False,
            "ties_record": False,
            "minimizer_count": 1 << n,
            "witness": [1] * n,
            "elapsed_s": time.perf_counter() - t0,
        }

    B_act = np.ascontiguousarray(B[:, active_cols], dtype=np.int64)
    B_act_T = np.ascontiguousarray(B_act.T, dtype=np.int64)

    # Exakte Symmetrie: ||A(-x)||_inf == ||A x||_inf => erste aktive Koordinate fest auf +1
    total_half = 1 << (k - 1)
    chunk_size = min(1 << chunk_bits, total_half)
    bit_shifts = np.arange(k - 1, dtype=np.uint64)

    global_min: int | None = None
    min_count_half = 0
    witness_half_idx = 0

    for start in range(0, total_half, chunk_size):
        end = min(start + chunk_size, total_half)
        c = end - start

        signs = np.empty((c, k), dtype=np.int64)
        signs[:, 0] = 1
        if k > 1:
            idx = np.arange(start, end, dtype=np.uint64)
            bits = ((idx[:, None] >> bit_shifts[None, :]) & np.uint64(1)).astype(np.int64)
            signs[:, 1:] = 2 * bits - 1

        # Exakte int64-Matrixmultiplikation
        prod = signs @ B_act_T
        assert prod.dtype == np.int64
        row_inf = np.abs(prod).max(axis=1)

        local_min = int(row_inf.min())
        if global_min is None or local_min < global_min:
            global_min = local_min
            min_count_half = int(np.sum(row_inf == global_min))
            witness_half_idx = start + int(np.argmin(row_inf))
        elif local_min == global_min:
            min_count_half += int(np.sum(row_inf == global_min))

    assert global_min is not None

    # Vollstaendigen Zeugenvektor x* in {-1, +1}^n rekonstruieren
    witness = [1] * n
    witness[active_cols[0]] = 1
    for b in range(k - 1):
        witness[active_cols[b + 1]] = 1 if ((witness_half_idx >> b) & 1) else -1

    # Unabhaengige Nachpruefung des Zeugen in reiner Python-Fraction-Arithmetik
    witness_disc = max(
        abs(sum(matrix[i][j] * witness[j] for j in range(n))) for i in range(m)
    )
    disc_exact = Fraction(global_min, L)
    if witness_disc != disc_exact:
        raise RuntimeError(
            f"Interner Konsistenzfehler: NumPy={disc_exact} != Fraction={witness_disc}"
        )

    ratio_sq = (disc_exact * disc_exact) / n
    record_sq = Fraction(RECORD_NUM, RECORD_DEN)
    total_minimizers = min_count_half * (1 << (1 + num_zero_cols))

    return {
        "m": m,
        "n": n,
        "active_cols": k,
        "scale_L": L,
        "discrepancy": disc_exact,
        "ratio_float": float(disc_exact) / math.sqrt(n),
        "ratio_sq": ratio_sq,
        "beats_record": ratio_sq > record_sq,
        "ties_record": ratio_sq == record_sq,
        "minimizer_count": total_minimizers,
        "witness": witness,
        "elapsed_s": time.perf_counter() - t0,
    }


def build_hamming_8x8_record() -> List[List[Fraction]]:
    """Konstruiert die 8x8-Rekordmatrix (5/sqrt(8)) aus dem [7,4,3]-Hamming-Code + Nullspalte."""
    # Paritaetspruefmatrix H (Spalten 1..7 binaer)
    H = [[(col >> bit) & 1 for col in range(1, 8)] for bit in range(3)]
    rows: List[List[Fraction]] = []
    for mask in range(1 << 7):
        bits = [(mask >> j) & 1 for j in range(7)]
        if bits[0] != 0:  # nur ein Vertreter pro antipodalem Paar
            continue
        syndrome = [sum(H[r][j] * bits[j] for j in range(7)) % 2 for r in range(3)]
        if syndrome == [0, 0, 0]:
            row = [Fraction(1 if b == 0 else -1) for b in bits] + [Fraction(0)]
            rows.append(row)
    assert len(rows) == 8
    return rows


def print_report(label: str, res: dict) -> None:
    status = (
        "NEUER REKORD (> 5/sqrt(8))"
        if res["beats_record"]
        else ("GLEICHSTAND (= 5/sqrt(8))" if res["ties_record"] else "unter Rekord (< 5/sqrt(8))")
    )
    print(f"=== {label} ===")
    print(f"Dimension        : {res['m']} x {res['n']} (aktive Spalten: {res['active_cols']}, kgV L={res['scale_L']})")
    print(f"Exakte Diskrepanz: {res['discrepancy']}")
    print(f"Quotient D/sqrt(n): {res['ratio_float']:.8f}  (exakt D^2/n = {res['ratio_sq']})")
    print(f"Vergleich 5/sqrt8: {status}  (Rekord D^2/n = {RECORD_NUM}/{RECORD_DEN})")
    print(f"Minimal-Vektoren : {res['minimizer_count']} von {1 << res['n']}")
    print(f"Zeuge x*         : {res['witness']}")
    print(f"Laufzeit         : {res['elapsed_s']:.3f} s")


def main() -> None:
    parser = argparse.ArgumentParser(description="Exakter Spencer-Diskrepanz-Verifier.")
    parser.add_argument("files", nargs="*", type=Path, help="Matrix-Dateien (.json, .csv, .txt, .npy)")
    parser.add_argument("--selftest", action="store_true", help="Testet gegen die 8x8-Hamming-Rekordmatrix")
    parser.add_argument("--no-zero-skip", action="store_true", help="Auch reine Nullspalten voll ausmultiplizieren")
    parser.add_argument("--chunk-bits", type=int, default=16, help="Chunk-Groesse als 2^k (Default: 16)")
    args = parser.parse_args()

    if args.selftest:
        mat8 = build_hamming_8x8_record()
        res8 = verify_matrix(mat8, chunk_bits=args.chunk_bits, skip_zero_cols=not args.no_zero_skip)
        print_report("Selftest: Hamming-[7,4,3] + Nullspalte (8x8)", res8)
        assert res8["discrepancy"] == Fraction(5)
        assert res8["ties_record"] is True
        print("Selftest OK.\n")

    for path in args.files:
        mat = load_matrix_exact(path)
        res = verify_matrix(mat, chunk_bits=args.chunk_bits, skip_zero_cols=not args.no_zero_skip)
        print_report(str(path), res)
        print()


if __name__ == "__main__":
    main()

