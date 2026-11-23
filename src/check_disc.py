"""Independent checker: exact discrepancy of an integer matrix by brute force.

disc(A) = min over all sign vectors x in {-1,+1}^n of max_i |(A x)_i|.
Reads a whitespace-separated integer matrix from a text file. Integer arithmetic only.
Shares no code with the search scripts.
"""
import sys
import numpy as np


def disc(A):
    A = np.asarray(A, dtype=np.int64)
    rows, n = A.shape
    best = None
    chunk = 1 << 16
    for start in range(0, 1 << n, chunk):
        idx = np.arange(start, min(start + chunk, 1 << n), dtype=np.int64)
        bits = (idx[:, None] >> np.arange(n)) & 1          # 0/1
        X = 1 - 2 * bits                                   # +1/-1
        val = np.abs(X @ A.T).max(axis=1)                  # max_i |(Ax)_i| per sign vector
        m = int(val.min())
        best = m if best is None else min(best, m)
    return best


if __name__ == "__main__":
    A = np.loadtxt(sys.argv[1], dtype=np.int64, ndmin=2)
    rows, n = A.shape
    assert np.abs(A).max() <= 1, "entries must lie in [-1, 1]"
    d = disc(A)
    N = max(rows, n)
    print(f"matrix {rows} x {n}, entries in {{-1,0,1}}: exact discrepancy = {d}")
    print(f"as a {N} x {N} matrix this gives C_10c >= {d}/sqrt({N}) = {d / N ** 0.5:.6f}"
          f"   (current record 5/sqrt(8) = {5 / 8 ** 0.5:.6f})")
