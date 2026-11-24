# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 1; Code der Modellantwort unverändert.
from itertools import product

def kandidaten():
    I = tuple(range(8))
    P = (0, 1, 3, 4, 5, 6, 7, 2)
    def row(k, f, s):
        w = [1-2*((k & f[t]).bit_count() & 1) for t in I]
        return w[1:] + [s*v for v in w]
    A = [row(k, I, 1) for k in I] + [row(k, P, -1) for k in I]
    for a in range(1, 8):
        F = [k for k in I if (a & k).bit_count() % 2 == 0]
        for drop in F:
            piv = [k for k in F if k != drop]
            frozen = [A[k] for k in range(16) if k not in F]
            for typ, f in enumerate((I, P)):
                masks = [[7+t for t in range(1, 8)
                          if ((b & f[t]).bit_count() & 1) == e]
                         for b in range(1, 8) for e in (0, 1)]
                for js in product(range(14), repeat=3):
                    rows = []
                    for k, j in zip(piv, js):
                        r = A[k][:]
                        for c in masks[j]:
                            r[c] *= -1
                        rows.append(r)
                    yield f"fano-{a}-{drop}-{typ}-{js}", frozen + rows
