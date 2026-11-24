"""Plausibilitätsprobe (Prüfcode, ganze Zahlen): Größte Menge vom Durchmesser <= D im n-Würfel per exakter
Clique-Suche (Branch and Bound) für kleine n, Vergleich mit der Formel aus dichte_schranke.kleitman.
Belegt den Satz von Kleitman NICHT, prüft nur, dass die Formel hier richtig umgesetzt ist."""
import sys
sys.path.insert(0, __file__.rsplit("/", 1)[0])
from dichte_schranke import kleitman


def max_menge(n, D):
    N = 1 << n
    nb = [sum(1 << y for y in range(N) if bin(x ^ y).count("1") <= D) for x in range(N)]
    best = 0

    def rek(kandidaten, groesse):
        nonlocal best
        if groesse + bin(kandidaten).count("1") <= best:
            return
        if kandidaten == 0:
            best = max(best, groesse)
            return
        while kandidaten:
            if groesse + bin(kandidaten).count("1") <= best:
                return
            v = kandidaten.bit_length() - 1
            kandidaten &= ~(1 << v)
            rek(kandidaten & nb[v], groesse + 1)
        best = max(best, groesse)

    rek(nb[0] & ~1, 1)       # ohne Einschränkung enthält die Menge 0 (Translation)
    return best


for n in range(3, 7):
    for D in range(1, n - 1):   # D = n-1 (Hälfte des Würfels) weggelassen: n=6, D=5 lief 600 s ohne Ende
        m, k = max_menge(n, D), kleitman(n, D)
        print(f"n={n} D={D}: größte Menge {m}, Formel {k}, {'gleich' if m == k else 'ABWEICHUNG'}")
