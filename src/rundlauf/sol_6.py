# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 6; Code der Modellantwort unverändert.
from itertools import product, permutations

def kandidaten():
    Z = list(product(range(5), repeat=2))
    def chi(x, y):
        return 1 if (x*x-2*y*y) % 5 in (1, 4) else -1
    def at(x, y, v):
        return 5*((x+v[0]) % 5) + (y+v[1]) % 5
    def proj(v, x, y):
        return x if v == 5 else (y-v*x) % 5
    neg = [z for z in Z[1:] if chi(*z) == -1]
    a, c = (1, 0), (1, 1)
    patches = []
    for skip, delta, shift in product(range(6), (1, 2), range(5)):
        rows = [[-1 if (proj(v, x, y)-v-shift) % 5 in (0, delta) else 1
                 for x, y in Z] + [1] for v in range(6) if v != skip]
        patches.append(((skip, delta, shift), rows))
    for b, d in permutations(neg, 2):
        u, v = (a[0]-b[0], a[1]-b[1]), (c[0]-d[0], c[1]-d[1])
        if (u[0]*v[1]-u[1]*v[0]) % 5 == 0:
            continue
        for stripe, kind in product(range(2), range(3)):
            A = []
            for x, y in Z:
                diag = (1, 1-2*(x in (0, 1)), 1-2*((x+y) % 5 in (0, 2)))[kind]
                r = [diag if (s, t) == (x, y) else chi(s-x, t-y)
                     for s, t in Z] + [1]
                i, j = at(x, y, a), at(x, y, b)
                r[i], r[j] = r[j], r[i]
                if not stripe or (x+2*y) % 5 in (0, 1, 2):
                    i, j = at(x, y, c), at(x, y, d)
                    r[i], r[j] = r[j], r[i]
                A.append(r)
            A.append([1]*26)
            for key, rows in patches:
                M = A + rows
                if len(set(map(tuple, M))) == 31:
                    yield f"conf26-{b}-{d}-{stripe}{kind}-{key}", M
