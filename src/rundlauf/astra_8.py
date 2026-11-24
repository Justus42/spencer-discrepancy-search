# Quelle: notes/openrouter/rundlauf_2026-11-24--openai-gpt-6.2-astra.md, Generator Nr. 8; Code der Modellantwort unverändert.
def kandidaten():
    def sg(z):
        return 1-2*(z & 1)
    def lin(i,x):
        return (i & x).bit_count() & 1
    def quad(x,k):
        a,b,c,d = [(x >> j) & 1 for j in range(4)]
        return ((a*b ^ c*d), (a*c ^ b*d), (a*d ^ b*c))[k]
    U = [[sg(lin(i,x)) for x in range(1,16)] for i in range(16)]
    for k in (1,2):
        for h in range(1,9):
            for p in range(8):
                for v in range(8):
                    M = []
                    for i in range(16):
                        s = sg(quad(i,0) ^ lin(p,i))
                        M.append(U[i]+[s*z for z in U[i]])
                    for i in range(1,16):
                        j = h if i == 1 else (1 if i == h else i)
                        t = sg(quad(i,k) ^ lin(v,i))
                        M.append([sg(quad(x,0))*U[i][x-1] for x in range(1,16)] +
                                 [t*sg(quad(x,k))*U[j][x-1] for x in range(1,16)])
                    yield f"cross30-{k}-{h}-{p}-{v}", M
