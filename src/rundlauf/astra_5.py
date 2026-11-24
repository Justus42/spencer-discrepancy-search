# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 5; Code der Modellantwort unverändert.
def kandidaten():
    from itertools import combinations
    g = sum(1 << j for j in (0,1,5,6,7,9,11))
    G = [0]
    for i in range(12):
        G += [w ^ (g << i) for w in G]
    def twice(w):
        return sum(((w >> j) & 1) << ((2*j) % 23) for j in range(23))
    def row(w):
        return [1-2*((w >> i) & 1) for i in range(23)]
    pool = {w for w in G if w.bit_count() in (7,8)}
    orbs = []
    while pool:
        x = min(pool)
        orb = [x]
        y = twice(x)
        while y != x:
            orb.append(y)
            y = twice(y)
        pool.difference_update(orb)
        if len(orb) == 11:
            orbs.append(list(map(row,orb)))
    q = sum(1 << j for j in {i*i % 23 for i in range(1,23)})
    fixed = list(map(row,(0,q,((1 << 23)-2) ^ q)))
    for i,j in combinations(range(len(orbs)),2):
        yield f"gol11-{i}-{j}", fixed + orbs[i] + orbs[j]
