# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 3; Code der Modellantwort unverändert.
def kandidaten():
    from itertools import product
    f = [-1 if j in (0, 1, 3) else 1 for j in range(7)]
    fixed = [[1]*7 + [s]*7 + [t]*7 for s, t in product((-1,1), repeat=2)]
    for a, b, h, k in product(range(1,7), range(7), range(7), range(4)):
        rows = fixed[:]
        for r in range(3):
            shifts = (0, r*a, (0,b,h)[r])
            signs = (1, -1 if r == 1 else 1, -1 if (k >> (r % 2)) & 1 else 1)
            for i in range(7):
                rows.append([signs[c]*f[(j-i-shifts[c]) % 7]
                             for c in range(3) for j in range(7)])
        # Identische projektive Zeilen nicht als zusätzliche Wörter zählen.
        if len({tuple(v*z[0] for v in z) for z in rows}) == 25:
            yield f"fano-{a}-{b}-{h}-{k}", rows
