# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 4; Code der Modellantwort unverändert.
def kandidaten():
    from itertools import product
    def mul(a,b):
        return ((a if b & 1 else 0) ^
                ((((a << 1) ^ (7 if a & 2 else 0))) if b & 2 else 0))
    def tr(a):
        return a >> 1
    def sg(i):
        return 1-2*(i & 1)
    core = []
    for t, a in product(range(5), range(4)):
        w = []
        for x,y in product(range(4), repeat=2):
            if t < 4:
                z = tr(mul(t,mul(x,y)) ^ mul(a,x ^ mul(t,y)))
            else:
                z = (x == 3) ^ (y == 3) ^ tr(mul(a,y))
            w.append(sg(z))
        core.append(w)
    for u,v,w,k in product(range(4),range(4),range(4),range(16)):
        shifts = (0,u,v,w,u ^ v ^ w)
        M = []
        for t,a in product(range(5),range(4)):
            e = ((k >> (t % 4)) & 1) * (a == 3)
            tail = [sg(tr(mul(a ^ shifts[t],j)) ^ e) for j in range(4)]
            M.append(core[4*t+a] + tail)
        yield f"charts-{u}-{v}-{w}-{k}", M
