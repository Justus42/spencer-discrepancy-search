# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 7; Code der Modellantwort unverändert.
def kandidaten():
    from itertools import product, combinations
    G = [g for g in product(range(3),repeat=4) if (g[0]*g[3]-g[1]*g[2]) % 3 == 1]
    def mul(g,h):
        a,b,c,d = g
        e,f,i,j = h
        return ((a*e+b*i)%3,(a*f+b*j)%3,(c*e+d*i)%3,(c*f+d*j)%3)
    I = (1,0,0,1)
    Q = {g for g in G if (g[0]+g[3])%3 == 0} | {I,(2,0,0,2)}
    h = next(g for g in G if g not in Q)
    A = sorted(mul(h,q) for q in Q)
    B = sorted(set(G)-Q-set(A))
    idx = {g:i for i,g in enumerate(G)}
    left = [[idx[mul(g,v)] for v in G] for g in G]
    for i,X in enumerate(combinations(A,3)):
        for j,Y in enumerate(combinations(B,6)):
            D = [idx[v] for v in (I,)+X+Y]
            M = [[1]*25]
            for p in left:
                r = [1]*25
                for t in D:
                    r[p[t]] = -1
                M.append(r)
            yield f"sl23-{i}-{j}", M
