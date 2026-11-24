# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 4; Code der Modellantwort unverändert.
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
from itertools import combinations
import numpy as np

def kandidaten():
    H = paley12()
    U = H[:, 1:]
    V = H[:, [j for j in range(12) if j != 1]]
    A = np.block([[U, V], [U, -V]])
    for a in (0, 1):
        Q = H[:, a:a+1] * H
        for cs in combinations([j for j in range(12) if j != a], 3):
            P = Q[:, list(cs)]
            for mode in range(1, 7):
                S = P * [1-2*((mode >> h) & 1) for h in range(3)]
                M = np.column_stack((A, np.vstack((P, S)))).tolist()
                for i, j in ((0, 1), (1, 0), (1, 2), (2, 5)):
                    r = U[i].tolist() + (-V[j]).tolist()
                    r += [int(P[i, 0]), int(S[j, 1]), int(P[(i+j) % 12, 2])]
                    yield f"lift3-{a}-{cs}-{mode}-{i}{j}", M + [r]
