# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-sol.md, Generator Nr. 5; Code der Modellantwort unverändert.
from itertools import product

def kandidaten():
    Z = list(product(range(5), repeat=2))
    for c in ((1,)*5, (2,)*5, (1, 2, 1, 2, 3)):
        for mode in (0, 1):
            bands = {}
            for b in range(5):
                o = mode*b*b
                for skip in range(1, 5):
                    B = {(x, (c[b]*x*x+b*x+o) % 5) for x in range(5)}
                    B |= {(0, (t+o) % 5) for t in range(1, 5) if t != skip}
                    bands[b, skip] = [
                        [-1 if ((x-s) % 5, (y-b*s) % 5) in B else 1
                         for x, y in Z] for s in range(5)]
            for skips in product(range(1, 5), repeat=5):
                M = [r for b, t in enumerate(skips) for r in bands[b, t]]
                yield f"parabel-{c}-{mode}-{skips}", M
