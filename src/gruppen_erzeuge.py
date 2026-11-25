#!/usr/bin/env python3
"""Prüfcode zu Auftrag G (kein Suchcode). Erzeugt und prüft Verknüpfungstafeln kleiner Gruppen.

Aufruf:  python3 src/gruppen_erzeuge.py [AUSGABEORDNER]     (Standard: runs/gruppen/tafeln)

Je Gruppe: Tafel aufbauen, prüfen (abgeschlossen, Assoziativität, Neutrales Element = Index 0, Inverse,
Ordnung n, abelsch ja/nein, Zahl der Elemente je Ordnung gegen die bekannte Tafel), alle Automorphismen
bestimmen (n <= 25, höchstens drei Erzeuger: Bilder der Erzeuger durchprobieren, Homomorphie voll nachprüfen)
und die Datei NAME.grp schreiben:

    # NAME  Beschreibung
    n
    n Zeilen: tab[g][h] = Index von g*h
    abelsch 0|1
    aut b0 b1 ... b(n-1)        (eine Zeile je Automorphismus, b_g = Bild von Element g)

Nur ganze Zahlen. Bei einer Abweichung Abbruch mit Fehlercode.
"""
import os
import sys
from collections import Counter
from itertools import product

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


# ---------------------------------------------------------------- Konstruktionen (Elemente = Tupel)
def zyklisch(n):
    return [(a,) for a in range(n)], lambda x, y: ((x[0] + y[0]) % n,)


def produkt_zyklisch(*ns):
    el = list(product(*[range(n) for n in ns]))
    return el, lambda x, y: tuple((a + b) % n for a, b, n in zip(x, y, ns))


def dieder(m):
    """D_m mit 2m Elementen (r, s): (r1,s1)(r2,s2) = (r1 + (-1)^s1 r2, s1+s2)."""
    el = [(r, s) for s in range(2) for r in range(m)]
    return el, lambda x, y: ((x[0] + (-1) ** x[1] * y[0]) % m, (x[1] + y[1]) % 2)


def s3_mal_z3():
    el = [(r, s, c) for c in range(3) for s in range(2) for r in range(3)]
    return el, lambda x, y: ((x[0] + (-1) ** x[1] * y[0]) % 3, (x[1] + y[1]) % 2, (x[2] + y[2]) % 3)


def z3z3_z2():
    """(Z3 x Z3) : Z2, Z2 invertiert beide Faktoren (verallgemeinerte Diedergruppe)."""
    el = [(a, b, s) for s in range(2) for b in range(3) for a in range(3)]
    return el, lambda x, y: ((x[0] + (-1) ** x[2] * y[0]) % 3, (x[1] + (-1) ** x[2] * y[1]) % 3, (x[2] + y[2]) % 2)


def dizyklisch5():
    """Dic5 = <a,b | a^10 = 1, b^2 = a^5, b a b^-1 = a^-1>; Element a^i b^j = (i, j)."""
    el = [(i, j) for j in range(2) for i in range(10)]

    def mul(x, y):
        i, j = x
        k, l = y
        # a^i b^j a^k b^l = a^(i + (-1)^j k) b^(j+l);  b^2 = a^5
        e = i + (-1) ** j * k + (5 if (j == 1 and l == 1) else 0)
        return (e % 10, (j + l) % 2)
    return el, mul


def halbdirekt(p, q, w):
    """Z_p : Z_q, (a,b)(a',b') = (a + w^b a', b + b') mod (p, q); w hat Ordnung q mod p."""
    el = [(a, b) for b in range(q) for a in range(p)]
    return el, lambda x, y: ((x[0] + pow(w, x[1], p) * y[0]) % p, (x[1] + y[1]) % q)


# Name -> (Beschreibung, Konstruktion, erwartete Zahl der Elemente je Ordnung)
GRUPPEN = {
    "z7": ("Z7", lambda: zyklisch(7), {1: 1, 7: 6}),
    "z18": ("Z18", lambda: zyklisch(18), {1: 1, 2: 1, 3: 2, 6: 2, 9: 6, 18: 6}),
    "z3xz6": ("Z3 x Z6", lambda: produkt_zyklisch(3, 6), {1: 1, 2: 1, 3: 8, 6: 8}),
    "d9": ("D9, Diedergruppe mit 18 Elementen", lambda: dieder(9), {1: 1, 2: 9, 3: 2, 9: 6}),
    "s3xz3": ("S3 x Z3", s3_mal_z3, {1: 1, 2: 3, 3: 8, 6: 6}),
    "z3z3_z2": ("(Z3 x Z3) : Z2, Z2 invertiert beide Faktoren", z3z3_z2, {1: 1, 2: 9, 3: 8}),
    "z20": ("Z20", lambda: zyklisch(20), {1: 1, 2: 1, 4: 2, 5: 4, 10: 4, 20: 8}),
    "z2xz10": ("Z2 x Z10", lambda: produkt_zyklisch(2, 10), {1: 1, 2: 3, 5: 4, 10: 12}),
    "d10": ("D10, Diedergruppe mit 20 Elementen", lambda: dieder(10), {1: 1, 2: 11, 5: 4, 10: 4}),
    "dic5": ("Dic5 = <a,b | a^10=1, b^2=a^5, bab^-1=a^-1>", dizyklisch5, {1: 1, 2: 1, 4: 10, 5: 4, 10: 4}),
    "f20": ("F20 = Z5 : Z4, Erzeuger von Z4 = Multiplikation mit 2", lambda: halbdirekt(5, 4, 2), {1: 1, 2: 5, 4: 10, 5: 4}),
    "z21": ("Z21", lambda: zyklisch(21), {1: 1, 3: 2, 7: 6, 21: 12}),
    "f21": ("Z7 : Z3, Erzeuger von Z3 = Multiplikation mit 2", lambda: halbdirekt(7, 3, 2), {1: 1, 3: 14, 7: 6}),
    "z25": ("Z25", lambda: zyklisch(25), {1: 1, 5: 4, 25: 20}),
    "z5xz5": ("Z5 x Z5", lambda: produkt_zyklisch(5, 5), {1: 1, 5: 24}),
}
# bekannte Größen der Automorphismengruppen (Kreuzprobe)
AUT_ERWARTET = {"z7": 6, "z18": 6, "z3xz6": 48, "d9": 54, "s3xz3": 12, "z3z3_z2": 432, "z20": 8, "z2xz10": 24,
                "d10": 40, "f20": 20, "z21": 12, "f21": 42, "z25": 20, "z5xz5": 480}


# ---------------------------------------------------------------- Aufbau und Prüfung
def baue(el, mul):
    n = len(el)
    idx = {e: i for i, e in enumerate(el)}
    assert len(idx) == n, "doppelte Elemente"
    tab = [[idx[mul(a, b)] for b in el] for a in el]       # KeyError = nicht abgeschlossen
    return n, tab


def pruefe(n, tab):
    e = 0
    for g in range(n):
        assert tab[e][g] == g and tab[g][e] == g, "Index 0 ist nicht neutral"
    for a in range(n):
        for b in range(n):
            for c in range(n):
                assert tab[tab[a][b]][c] == tab[a][tab[b][c]], "nicht assoziativ"
    inv = []
    for g in range(n):
        w = [h for h in range(n) if tab[g][h] == 0]
        assert len(w) == 1 and tab[w[0]][g] == 0, "kein eindeutiges Inverses"
        inv.append(w[0])
    abelsch = all(tab[a][b] == tab[b][a] for a in range(n) for b in range(n))
    ordn = Counter()
    for g in range(n):
        k, x = 1, g
        while x != 0:
            x = tab[x][g]
            k += 1
        ordn[k] += 1
    return inv, abelsch, dict(ordn)


def ordnung(tab, g):
    k, x = 1, g
    while x != 0:
        x = tab[x][g]
        k += 1
    return k


def erzeugnis(tab, gens):
    n = len(tab)
    seen = {0}
    todo = [0]
    while todo:
        x = todo.pop()
        for s in gens:
            y = tab[x][s]
            if y not in seen:
                seen.add(y)
                todo.append(y)
    return seen


def erzeugerpaar(tab):
    n = len(tab)
    for g in range(1, n):
        if len(erzeugnis(tab, [g])) == n:
            return [g]
    for g in range(1, n):
        for h in range(g + 1, n):
            if len(erzeugnis(tab, [g, h])) == n:
                return [g, h]
    for g in range(1, n):
        for h in range(g + 1, n):
            for k in range(h + 1, n):
                if len(erzeugnis(tab, [g, h, k])) == n:
                    return [g, h, k]
    raise SystemExit("kein Erzeugersystem mit höchstens drei Elementen")


def automorphismen(tab):
    n = len(tab)
    gens = erzeugerpaar(tab)
    ords = [ordnung(tab, g) for g in gens]
    kandidaten = [[h for h in range(n) if ordnung(tab, h) == o] for o in ords]
    # Wege von 0 zu jedem Element (Wörter in den Erzeugern), BFS
    weg = {0: []}
    todo = [0]
    while todo:
        x = todo.pop(0)
        for i, s in enumerate(gens):
            y = tab[x][s]
            if y not in weg:
                weg[y] = weg[x] + [i]
                todo.append(y)
    auts = []
    for bilder in product(*kandidaten):
        phi = [0] * n
        for x, w in weg.items():
            v = 0
            for i in w:
                v = tab[v][bilder[i]]
            phi[x] = v
        if len(set(phi)) != n:
            continue
        if all(phi[tab[a][b]] == tab[phi[a]][phi[b]] for a in range(n) for b in range(n)):
            auts.append(phi)
    return auts


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "runs", "gruppen", "tafeln")
    os.makedirs(out, exist_ok=True)
    fehler = 0
    for name, (beschr, bau, erwartet) in GRUPPEN.items():
        n, tab = baue(*bau())
        inv, abelsch, ordn = pruefe(n, tab)
        ok = ordn == erwartet
        auts = automorphismen(tab)
        ok_aut = (name not in AUT_ERWARTET) or len(auts) == AUT_ERWARTET[name]
        if name == "dic5":
            ok_aut = True      # keine Vergleichszahl aus dem Gedächtnis, nur gerechnet
        print(f"{name:8s} n={n:2d} {'abelsch' if abelsch else 'nichtabelsch':12s} Ordnungen {ordn} "
              f"{'= erwartet' if ok else 'ABWEICHUNG, erwartet ' + str(erwartet)}; |Aut| = {len(auts)}"
              f"{'' if ok_aut else '  ABWEICHUNG, erwartet ' + str(AUT_ERWARTET[name])}")
        if not ok or not ok_aut:
            fehler += 1
            continue
        with open(os.path.join(out, name + ".grp"), "w") as f:
            f.write(f"# {name}  {beschr}\n{n}\n")
            for r in tab:
                f.write(" ".join(map(str, r)) + "\n")
            f.write(f"abelsch {1 if abelsch else 0}\n")
            for a in auts:
                f.write("aut " + " ".join(map(str, a)) + "\n")
    return 1 if fehler else 0


if __name__ == "__main__":
    sys.exit(main())
