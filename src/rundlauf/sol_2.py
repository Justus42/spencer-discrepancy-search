# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 2; Code der Modellantwort unverändert.
# --- Vorspann paley12 (gemeinsam für Nr. 2 und 4, unverändert) ---
def paley12():
    import numpy as np
    H = np.ones((12, 12), dtype=np.int64)
    H[1:, 0] = -1
    for i in range(11):
        for j in range(11):
            H[i+1, j+1] = 1 if (i-j) % 11 in (0, 1, 3, 4, 5, 9) else -1
    assert (H @ H.T == 12 * np.eye(12, dtype=np.int64)).all()
    return H
# --- Generator ---
from itertools import combinations, permutations, product

def kandidaten():
    H = paley12()
    U, V = H[:, 1:].tolist(), H[:, 3:].tolist()
    W = [[-v for v in r] for r in V]
    for S in combinations(range(12), 4):
        keep = [i for i in range(12) if i not in S]
        fixed = [U[i] + R[i] for i in keep for R in (V, W)]
        for P in permutations(S):
            if any(i == j for i, j in zip(S, P)):
                continue
            edges = [(U[i] + V[j], U[i] + W[j]) for i, j in zip(S, P)]
            for bits in product((0, 1), repeat=4):
                rows = [e[b] for e, b in zip(edges, bits)]
                yield f"paley-wire-{S}-{P}-{bits}", fixed + rows
