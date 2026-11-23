"""Construction proposed by GPT-6.2 Astra (via bridge, 2026-11-23): self-complementary 32-word
covering codes of odd length n = 2R+7 >= 15. Checked here independently by brute force."""
import sys
import numpy as np

def code(n):
    assert n >= 15 and n % 2
    p = [0, 1, 3, 4, 5, 6, 7, 2]  # cubing in GF(8), modulus X^3+X+1
    cols = list(range(1, 16)) + [0]*(n-15)
    C = []
    for k in range(16):
        w = 0
        for i, j in enumerate(cols):
            x = j % 8 if k < 8 else p[j % 8]
            b = ((k & x).bit_count() + (k//8)*(j//8)) % 2
            w |= b << i
        C.extend([w, w ^ ((1 << n)-1)])
    return C

if __name__ == "__main__":
    for n in map(int, sys.argv[1:]):
        C = code(n); R = (n - 7)//2
        X = np.arange(1 << n, dtype=np.uint32)
        dmin = np.full(1 << n, n, dtype=np.uint8)
        for w in C:
            np.minimum(dmin, np.bitwise_count(X ^ np.uint32(w)), out=dmin)
        selfc = all((w ^ ((1 << n) - 1)) in set(C) for w in C)
        print(f"n={n}: words={len(set(C))}, self-complementary={selfc}, covering radius={int(dmin.max())} (claimed {R})")
        rows = [C[i] for i in range(0, 32, 2)]
        with open(f"astra_{n}.txt", "w", encoding="utf-8") as f:
            for c in rows:
                f.write(" ".join(f"{-1 if (c >> j) & 1 else 1:2d}" for j in range(n)) + "\n")
            for _ in range(n - 16):
                f.write(" ".join(f"{-1 if (rows[0] >> j) & 1 else 1:2d}" for j in range(n)) + "\n")
