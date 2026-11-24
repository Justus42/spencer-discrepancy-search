# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 8; Code der Modellantwort unverändert.
from itertools import product

def kandidaten():
    I = tuple(range(8))
    P = (0, 1, 3, 4, 5, 6, 7, 2)
    Q = tuple(P.index(t) for t in I)
    F = (P, Q, (0, 2, 1, 3, 4, 5, 6, 7),
         (0, 1, 2, 3, 5, 4, 6, 7), (0, 2, 4, 6, 1, 3, 5, 7))
    cols = [(l, t) for t in I for l in range(4) if t or l < 2]
    for a, b, c, d in product(range(5), repeat=4):
        lab = ((I,)*4, (P,)*4, (I, F[a], F[b], P), (F[c], I, F[d], Q))
        for phase in (0, 1):
            A = [[1-2*(((r & lab[k][l][t]).bit_count()
                        + (k & l).bit_count()
                        + phase*(k >= 2)*(l >= 2)*(P[t] & 1)) & 1)
                  for l, t in cols] for k in range(4) for r in I]
            for omit in range(16, 32, 2):
                M = A[:omit] + A[omit+1:]
                if len({tuple(v*x[0] for v in x) for x in M}) == 31:
                    yield f"walsh4-{a}{b}{c}{d}-{phase}-{omit}", M
