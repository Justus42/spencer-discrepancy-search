# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 1; Code der Modellantwort unverändert.
def kandidaten():
    import numpy as np
    from itertools import combinations
    sq = {i*i % 11 for i in range(1, 11)}
    H = np.eye(12, dtype=int)
    H[0, 1:] = 1
    H[1:, 0] = -1
    for i in range(11):
        for j in range(11):
            if i != j:
                H[i+1, j+1] = 1 if (i-j) % 11 in sq else -1
    U, V = H[:, 1:], np.delete(H, 1, axis=1)
    core = np.block([[U, V], [U, -V]])
    for a, b in combinations(range(12), 2):
        switch = H[:, a] * H[:, b]
        for c in range(1, 12):
            s = H[:, c]
            base = np.column_stack((core, np.r_[s, s*switch]))
            for j in (1, 2, 4):
                for e in (-1, 1):
                    r = np.r_[U[0]*U[j], e*V[j]*V[(j+c) % 12], 1]
                    yield f"hinge-{a}-{b}-{c}-{j}-{e}", np.vstack((base,r)).tolist()
