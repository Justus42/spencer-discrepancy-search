"""Prüfcode, Teil B: Dichteschranke je n für reelle n x n-Matrizen. Nur ganze Zahlen und Brüche.

Annahme: A in [-1,1]^(n x n) mit disc(A) > t = 5*sqrt(n/8). Jede Färbung x liegt in einer der 2n Kappen
{x : +-a_i . x > t}. Zwei Färbungen x, y in derselben Kappe haben Hamming-Abstand d < n - t
(a.x + a.y <= 2(n-d) und beide > t). D = größte ganze Zahl mit 8 (n-D)^2 > 25 n  (also D < n - t).
Satz von Kleitman (Literatur, hier NICHT belegt): Eine Menge vom Durchmesser D im n-Würfel hat höchstens
 Kl = Summe_{i <= D/2} C(n,i)                   für gerades D,
 Kl = 2 * Summe_{i <= (D-1)/2} C(n-1,i)         für ungerades D  Elemente.
Nötig: 2 n Kl >= 2^n. Dichte = 2 n Kl / 2^n; unter 1: keine reelle n x n-Matrix hat disc > t.
Aufruf: python3 src/dichte_schranke.py [nmin nmax]
"""
import sys
from fractions import Fraction
from math import comb


def grosses_D(n):
    D = n - 1
    while D >= 0 and not 8 * (n - D) ** 2 > 25 * n:
        D -= 1
    # D ist die größte ganze Zahl mit der Eigenschaft (n-D wächst mit sinkendem D)
    assert D >= 0 and 8 * (n - D) ** 2 > 25 * n and not 8 * (n - D - 1) ** 2 > 25 * n if D < n - 1 else True
    return D


def kleitman(n, D):
    if D % 2 == 0:
        return sum(comb(n, i) for i in range(D // 2 + 1))
    return 2 * sum(comb(n - 1, i) for i in range((D - 1) // 2 + 1))


def dichte(n):
    D = grosses_D(n)
    Kl = kleitman(n, D)
    return D, Kl, Fraction(2 * n * Kl, 2 ** n)


def runde3(q):
    """Fraction -> Dezimaldarstellung mit 3 Stellen, bei exaktem Gleichstand zur geraden Ziffer
    (round() auf Fraction, rein ganzzahlig; n = 8 hat genau 9/16 = 0,5625 -> 0,562)."""
    z = round(q * 1000)
    return f"{z // 1000},{z % 1000:03d}"


ERWARTET = "0,562 0,633 1,094 1,203 0,785 1,200 1,292 1,777 1,229 1,306 1,733 1,829 2,306 1,645 1,724 2,142 2,235 2,694".split()

if __name__ == "__main__":
    a, b = (int(sys.argv[1]), int(sys.argv[2])) if len(sys.argv) == 3 else (8, 25)
    print("  n |  D |            Kl |  2n*Kl / 2^n | exakt (Bruch)            | < 1?")
    abw = 0
    for n in range(a, b + 1):
        D, Kl, q = dichte(n)
        # t^2 = 25 n / 8: D = größte ganze Zahl < n - t
        assert 8 * (n - D) ** 2 > 25 * n >= 8 * (n - D - 1) ** 2
        s = runde3(q)
        if len(sys.argv) != 3:
            ok = s == ERWARTET[n - 8]
            abw += not ok
        else:
            ok = True
        print(f"{n:3d} | {D:2d} | {Kl:13d} | {s:>11s} | {str(q):24s} | {'JA (kein Rekord möglich)' if q < 1 else ''}"
              f"{'' if ok else '   <-- ABWEICHUNG, erwartet ' + ERWARTET[n - 8]}")
    if len(sys.argv) != 3:
        print("Abweichungen von der erwarteten Liste:", abw)
