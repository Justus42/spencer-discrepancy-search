# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 6; Code der Modellantwort unverändert.
def kandidaten():
    def circ(D):
        return [[-1 if (j-i) % 9 in D else 1 for j in range(9)] for i in range(9)]
    P,Q = circ({0,1,3}), circ({0,4})
    T = list(map(list,zip(*P)))
    W = list(map(list,zip(*Q)))
    fixed = [[1]*18,[1]*9+[-1]*9] + [p+q for p,q in zip(P,Q)]
    from itertools import combinations
    masks = [()] + [(0,)] + list(combinations(range(9),2)) + [(0,3,6),(0,1,3)]
    for a in (1,2,4,5,7,8):
        for b in range(9):
            for D in masks:
                bot = [[-z for z in W[i]] +
                       [(-z if i in D else z) for z in T[(a*i+b) % 9]]
                       for i in range(9)]
                yield f"sds9-{a}-{b}-{D}", fixed+bot
