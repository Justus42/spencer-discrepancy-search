# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 3; Code der Modellantwort unverändert.
from itertools import product

def kandidaten():
    I = tuple(range(8))
    P = (0, 1, 3, 4, 5, 6, 7, 2)
    Q = tuple(P.index(t) for t in I)
    F = (I, P, Q, (0, 2, 1, 3, 4, 5, 6, 7))
    cols = [(l, t) for l in range(3) for t in I if (l, t) != (2, 0)]
    def par(v):
        return v.bit_count() & 1
    for i, j, k, h in product(range(4), repeat=4):
        lab = ((I, I, I), (F[i], F[j], I), (F[k], I, F[h]))
        for a, b in product((1, 2, 4, 7), repeat=2):
            phase = ((0, 0, 0), (a, 0, 0), (0, b, 0))
            A = [[1-2*(par(u & lab[g][l][t]) ^
                       par(phase[g][l] & P[t]) ^ (((0, 2, 4)[g] >> l) & 1))
                  for l, t in cols] for g in range(3) for u in I]
            for c in (1, 3):
                r = [1-2*(par(c & t) ^ par((a ^ b) & P[t]) ^ ((6 >> l) & 1))
                     for l, t in cols]
                M = A + [r]
                if len({tuple(v*x[0] for v in x) for x in M}) == 25:
                    yield f"walsh3-{i}{j}{k}{h}-{a}{b}-{c}", M
