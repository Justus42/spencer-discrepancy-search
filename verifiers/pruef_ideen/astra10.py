#!/usr/bin/env python3
"""
astra10.py -- Pruefrechnung (Auftrag P, Teil 5): Astras Ausschluss reeller 10 x 10-Matrizen. Nur ganze Zahlen.

Behauptung (Astra): Fuer A in [-1,1]^(10x10) gibt es ein x in {+-1}^10 mit max_i |(Ax)_i| <= t0 = 5*sqrt(10/8) = 5,5902.
Schritte, die hier mit eigenen Rechnungen belegt werden:
  S1  Durchmesser einer Kappe {x : a.x > t0} ist <= 4 (Zahlen: 2*(10 - d) > 11,18 gibt d <= 4).
  S2  Nach Ausrichtung (a >= 0) ist die Menge der Fehlermengen nach unten abgeschlossen und |S u T| <= 4 fuer alle Paare
      (Zufallsproben mit echten reellen a, ganzzahlig gerechnet: a = p/q).
  S3  Hoechstens 43 Punkte, wenn die Kappe eine Dreiermenge enthaelt (exakte Aufzaehlung aller solchen Familien: Maximum).
  S4  Ohne Dreiermenge liegt die Kappe in der Radius-2-Kugel um das Vorzeichenmuster von a (56 Klassen).
  S5  Zwei Radius-2-Kugeln (Klassen) gleicher Gewichtsparitaet schneiden sich in >= 6 Klassen; Vereinigungsschranke
      56(10-e) + 43e - 6 max(8-e,0) < 512 fuer e = 1..10.
  S6  Fall e = 0: 10 Radius-2-Kugeln ueberdecken die 512 Klassen nicht (exakte Suche, unabhaengig von K(10,2) >= 24).
Aufruf: python3 -I verifiers/pruef_ideen/astra10.py [Schritt ...]   (S1 S2 S3 S4 S5 S6)
"""
import sys
import itertools
from fractions import Fraction
import numpy as np


# t0^2 = 25*10/8 = 31,25 ; Test a.x > t0  <=>  (a.x)^2 > 31,25 mit a.x > 0, ganzzahlig: mit a = p/q: (p.x)^2 * 8 > 250 * q^2
def kappe(p, q, X):
    """Indizes der Punkte x (Zeilen von X) mit a.x > t0, a = p/q (p ganzzahlig). Ganzzahlig: s = p.x > 0 und 8 s^2 > 250 q^2."""
    s = X @ p
    return np.nonzero((s > 0) & (8 * s * s > 250 * q * q))[0]


def alle_punkte(n):
    k = np.arange(1 << n, dtype=np.int64)
    return 1 - 2 * ((k[:, None] >> np.arange(n)[None, :]) & 1)


def s1_s2(proben=20000):
    print("== S1/S2: Durchmesser, Abgeschlossenheit, |S u T| <= 4 an Zufalls-Kappen (reelle a als p/q)")
    X = alle_punkte(10)
    rng = np.random.default_rng(7)
    ok = True
    grosse = 0
    maxgroesse = 0
    nicht_leer = 0
    for t in range(proben):
        q = int(rng.choice([8, 16, 64, 1000]))
        art = t % 6
        if art == 0:
            p = rng.integers(-q, q + 1, size=10)
        elif art == 1:
            p = rng.integers(-q, q + 1, size=10) * rng.integers(0, 2, size=10)  # duenn besetzt
        elif art == 2:
            p = np.where(rng.random(10) < 0.5, -q, q) + rng.integers(-1, 2, size=10)  # fast +-1
            p = np.clip(p, -q, q)
        elif art == 3:
            p = np.sort(rng.integers(0, q + 1, size=10))[::-1] * rng.choice([-1, 1], size=10)
        else:   # wenige kleine Eintraege: erzeugt Kappen mit Dreiermengen
            k_klein = 2 if art == 4 else 3
            p = rng.choice([-1, 1], size=10) * q
            klein = rng.permutation(10)[:k_klein]
            p[klein] = rng.integers(-(q // 4), q // 4 + 1, size=k_klein)
        K = kappe(p, q, X)
        if len(K) == 0:
            continue
        nicht_leer += 1
        maxgroesse = max(maxgroesse, len(K))
        P = X[K]
        # Durchmesser
        D = (10 - P @ P.T) // 2
        if D.max() > 4:
            ok = False
            print("  Durchmesser > 4 bei", p, q)
        # Ausrichtung: Koordinaten mit p_i < 0 umdrehen; dann Fehlermengen = Koordinaten mit x_i = -1
        sgn = np.where(p < 0, -1, 1)
        Fm = [frozenset(np.nonzero(P[j] * sgn == -1)[0].tolist()) for j in range(len(K))]
        Fs = set(Fm)
        # nach unten abgeschlossen
        for S in Fs:
            for i in S:
                if (S - {i}) not in Fs:
                    ok = False
                    print("  nicht abgeschlossen", p, q)
                    break
        # Paarvereinigungen
        for S in Fm:
            for T in Fm:
                if len(S | T) > 4:
                    ok = False
                    print("  |S u T| > 4", p, q)
        if max(len(S) for S in Fs) >= 3:
            grosse += 1
            if len(Fs) > 43:
                ok = False
                print("  Kappe mit Dreiermenge hat mehr als 43 Punkte", len(Fs))
            if len(Fs) > 36:
                ok = False
                print("  Kappe mit Dreiermenge hat mehr als 36 Punkte (exaktes Maximum aus S3)", len(Fs))
    print("  %d Kappen (nicht leer %d), davon %d mit Dreiermenge; groesste Kappe %d Punkte: %s" % (proben, nicht_leer, grosse, maxgroesse, "alle Pruefungen bestanden" if ok else "VERLETZT"))
    return ok


def s3():
    print("== S3: groesste nach unten abgeschlossene Familie auf 10 Punkten mit |S u T| <= 4 (alle Paare), mit einer Dreiermenge, ohne Vierermenge")
    n = 10
    dreier = [sum(1 << i for i in c) for c in itertools.combinations(range(n), 3)]
    zweier = [sum(1 << i for i in c) for c in itertools.combinations(range(n), 2)]
    pc = lambda x: bin(x).count("1")
    # Familie = {leere Menge} + 10 Einermengen (vertraeglich mit allem) + Zweiermengen + Dreiermengen.
    # Dreiermengen T, T' vertraeglich gdw |T u T'| <= 4. Zweiermenge P mit Dreiermenge T gdw |P u T| <= 4 (P trifft T).
    # Zweiermengen untereinander und Einermengen immer vertraeglich. Abgeschlossenheit: Zweiermengen in gewaehlten Dreiermengen sind dabei.
    # Wir zaehlen exakt (nicht nur Strukturargument): alle Mengen T-Familien (Cliquen) aufzaehlen, dazu alle Zweiermengen, die alle Dreiermengen treffen.
    verb = {a: [b for b in dreier if b != a and pc(a | b) <= 4] for a in dreier}
    best = 0
    arg = None
    anzahl = 0
    hist = {}

    def rec(clique, kand):
        nonlocal best, arg, anzahl
        if clique:
            anzahl += 1
            # alle Zweiermengen, die jede Dreiermenge treffen (Vereinigung <= 4)
            zw = sum(1 for P in zweier if all(pc(P | T) <= 4 for T in clique))
            gr = 1 + n + zw + len(clique)
            hist[len(clique)] = max(hist.get(len(clique), 0), gr)
            if gr > best:
                best, arg = gr, list(clique)
        for i, c in enumerate(kand):
            neu = [d for d in kand[i + 1:] if d in verb[c]]
            rec(clique + [c], neu)

    # Symmetrie: die Dreiermenge {0,1,2} ist enthalten (Umnummerierung); Aufzaehlung aller Cliquen, die sie enthalten
    start = dreier[0]
    rec([start], [d for d in verb[start]])
    print("  Cliquen von Dreiermengen mit {0,1,2}: %d; groesste Familie: %d Mengen (ohne Vierermengen)" % (anzahl, best))
    print("  Beispiel einer groessten Dreiermengen-Familie:", [sorted(i for i in range(n) if t >> i & 1) for t in arg])
    print("  groesste Familie je Zahl der Dreiermengen:", dict(sorted(hist.items())))
    print("  Schranke Astra: 1 + 10 + 24 + 8 = 43; exaktes Maximum %d <= 43: %s" % (best, "ja" if best <= 43 else "NEIN"))
    # Teilzahlen der Schranke
    z24 = sum(1 for P in zweier if pc(P | start) <= 4)
    print("  Zweiermengen, die {0,1,2} treffen: %d (Astra: 45 - 21 = 24)" % z24)
    # Familien mit einer Vierermenge: alle Mengen darin
    vier = sum(1 << i for i in range(4))
    ok_vier = all((S & ~vier) == 0 for S in range(1 << n) if pc(S) <= 4 and pc(S | vier) <= 4)
    print("  S u {0,1,2,3} <= 4 Punkte  =>  S in {0,1,2,3}: %s (Familie mit Vierermenge hat hoechstens 16 Mengen)" % ok_vier)
    return best <= 43 and z24 == 24 and ok_vier


def klassen_ball():
    """Klassen der 512 Antipodenklassen von {0,1}^10: Vertreter = Zahl mit Bit 9 = 0. Radius-2-Kugel um Klasse c als Bitmenge ueber Klassen."""
    full = (1 << 10) - 1
    kid = [min(x, x ^ full) for x in range(1 << 10)]
    reps = sorted(set(kid))
    idx = {r: i for i, r in enumerate(reps)}
    pc = [bin(i).count("1") for i in range(1 << 10)]
    kugel = []
    for c in reps:
        m = 0
        for x in range(1 << 10):
            if pc[x ^ c] <= 2 or pc[x ^ c ^ full] <= 2:
                m |= 1 << idx[kid[x]]
        kugel.append(m)
    return reps, idx, kugel, pc


def s4_s5():
    print("== S4/S5: Kugeln im Klassenraum, Ueberschneidungen, Vereinigungsschranke")
    reps, idx, kugel, pc = klassen_ball()
    full = (1 << 10) - 1
    groesse = {bin(k).count("1") for k in kugel}
    print("  Klassen:", len(reps), "; Groesse einer Radius-2-Kugel (Klassen):", groesse, "(Astra: 56)")
    par = [pc[r] % 2 for r in reps]
    kleinst_gl = 99
    kleinst_alle = 99
    fa = {}
    for i in range(len(reps)):
        for j in range(i + 1, len(reps)):
            s = bin(kugel[i] & kugel[j]).count("1")
            fd = min(pc[reps[i] ^ reps[j]], 10 - pc[reps[i] ^ reps[j]])
            if par[i] == par[j]:
                kleinst_gl = min(kleinst_gl, s)
                assert fd <= 4 and fd % 2 == 0
            kleinst_alle = min(kleinst_alle, s)
            fa.setdefault((par[i] == par[j], fd), set()).add(s)
    print("  kleinster Schnitt zweier Kugeln gleicher Paritaet: %d (Astra: >= 6); gefalteter Abstand dabei <= 4 und gerade: ja" % kleinst_gl)
    for k in sorted(fa):
        print("    gleiche Paritaet" if k[0] else "    verschiedene Paritaet", "gefalteter Abstand", k[1], ": Schnittgroessen", sorted(fa[k]))
    ok = groesse == {56} and kleinst_gl >= 6
    print("  Vereinigungsschranke 56(10-e) + 43e - 6 max(8-e,0):")
    for e in range(0, 11):
        v = 56 * (10 - e) + 43 * e - 6 * max(8 - e, 0)
        print("    e = %2d: %d %s" % (e, v, "(< 512)" if v < 512 else "(= 512: kein Widerspruch, Sonderfall e = 0)" if v == 512 else "(>= 512)"))
        if e >= 1:
            ok &= v < 512
        else:
            ok &= v == 512
    return ok


def s6(anzahl=10, zeitmax=300):
    print("== S6: Fall e = 0: lassen sich die 512 Klassen mit %d Radius-2-Kugeln ueberdecken? (exakte Suche, zweites Programm: Python)" % anzahl)
    import time
    reps, idx, kugel, pc = klassen_ball()
    N = len(reps)
    alle = (1 << N) - 1
    enth = [[c for c in range(N) if kugel[c] >> i & 1] for i in range(N)]   # Kugeln, die Klasse i enthalten
    t0 = time.time()
    knoten = 0
    gefunden = None

    def rec(abgedeckt, rest, wahl):
        nonlocal knoten, gefunden
        knoten += 1
        unab = N - abgedeckt.bit_count()
        if unab == 0:
            gefunden = list(wahl)
            return True
        if rest == 0:
            return False
        if time.time() - t0 > zeitmax:
            raise TimeoutError
        nicht = ~abgedeckt
        gewinn = [(kugel[c] & nicht).bit_count() for c in range(N)]
        # jede Kugel traegt hoechstens ihren momentanen Zuwachs bei: unab <= Summe der `rest` groessten Zuwaechse
        if sum(sorted(gewinn, reverse=True)[:rest]) < unab:
            return False
        i = (nicht & alle & -(nicht & alle)).bit_length() - 1      # kleinste unueberdeckte Klasse
        for c in sorted(enth[i], key=lambda c: -gewinn[c]):
            if rec(abgedeckt | kugel[c], rest - 1, wahl + [c]):
                return True
        return False

    try:
        # Erste Kugel um Klasse 0 (alle Kugeln sind unter den Translationen aequivalent)
        ok = rec(kugel[0], anzahl - 1, [0])
    except TimeoutError:
        print("  ZEITGRENZE nach %d s, %d Knoten: unentschieden" % (zeitmax, knoten))
        return None
    print("  Knoten %d, Zeit %.1f s: %s" % (knoten, time.time() - t0, "Ueberdeckung GEFUNDEN" if ok else "keine Ueberdeckung mit %d Kugeln" % anzahl))
    return not ok


def main():
    teile = sys.argv[1:] or ["S1", "S3", "S5", "S6"]
    erg = {}
    for t in teile:
        if t in ("S1", "S2"):
            erg[t] = s1_s2()
        elif t == "S3":
            erg[t] = s3()
        elif t in ("S4", "S5"):
            erg[t] = s4_s5()
        elif t == "S6":
            erg[t] = s6()
        elif t == "S6K":     # Positivkontrolle: mit 16 Kugeln gibt es eine Ueberdeckung (C-Programm findet sie), Python soll sie auch finden
            r = s6(16, 300)
            erg[t] = (r is False)
    print("ERGEBNIS:", ", ".join("%s %s" % (t, {True: "bestaetigt", False: "ABWEICHUNG", None: "unentschieden"}[v]) for t, v in erg.items()))


if __name__ == "__main__":
    main()
