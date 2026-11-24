# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 2; Code der Modellantwort unverändert.
def kandidaten():
    from itertools import combinations, product
    E = list(combinations(range(6), 2))
    def row(B):
        return [-1 if e in B else 1 for e in E]
    F = []
    for i in range(5):
        F.append({(i, 5)} | {tuple(sorted(((i-j) % 5, (i+j) % 5)))
                               for j in (1, 2)})
    fixed = list(map(row, F))
    choices = []
    for a, b in combinations(range(1, 6), 2):
        S = {0, a, b}
        cut = {e for e in E if len(set(e) & S) == 1}
        choices.append([row(set(E)-cut)] + [row(cut-f) for f in F if f <= cut])
    for k, ix in enumerate(product(range(3), repeat=10)):
        yield f"duad-{k}", fixed + [c[i] for c, i in zip(choices, ix)]
