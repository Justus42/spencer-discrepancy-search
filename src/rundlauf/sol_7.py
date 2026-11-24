# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 7; Code der Modellantwort unverändert.
from itertools import combinations
from random import Random
import numpy as np

def kandidaten():
    H = np.array([[1-2*((i & j).bit_count() & 1)
                   for j in range(16)] for i in range(16)], dtype=np.int64)
    K = H.copy()
    K[np.ix_(range(4), range(0, 16, 4))] *= -1
    rng = Random(20261124)
    perms = [list(range(16))]
    for _ in range(7):
        p = list(range(16))
        rng.shuffle(p)
        perms.append(p)
    for l in (0, 1, 4, 5):
        U = np.delete(H, l, axis=1)
        for J in combinations(range(16), 3):
            V = np.delete(K, J, axis=1)
            for k, p in enumerate(perms):
                W = V[p]
                A = np.block([[U, W], [U, -W]]).tolist()
                drop = (l + sum(J) + 5*k) % 32
                yield f"schalt16-{l}-{J}-{k}-{drop}", A[:drop] + A[drop+1:]
