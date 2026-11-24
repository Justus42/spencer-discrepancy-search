"""Prüfcode (kein Suchcode), Teil A: lokales Polieren um eine +-1-Matrix, nur ganze Zahlen und Brüche.

Je Matrix A (Zeilen x Spalten, Einträge +-1):
  1. Alle Vorzeichenvektoren x bis auf globales Vorzeichen (x_0 = +1): kritisch heißt max_i |(Ax)_i| = disc.
     Gezählt: kritisch, von genau 1 / 2 / 3 Zeilen getragen (Zeile trägt, wenn |(Ax)_i| = disc).
  2. Je Zeile i und je allein getragenem x: D = Spalten j, in denen a_ij * x_j dem Vorzeichen von (Ax)_i widerspricht.
     Gezählt: verschiedene D je Zeile, |D|, h_j = Anzahl D mit j in D, größtes h_j.
  3. Stichprobenprüfung der Folgerung mit exakten Brüchen: zufällige reelle Matrizen B, deren Zeilen höchstens
     L1-Abstand 2 von (A | 0) haben, Spalten + 1; die Diskrepanz von B wird mit ganzzahliger Brute-Force
     (B mit Hauptnenner q skaliert) über alle 2^(Spalten) Vorzeichenvektoren bestimmt und muss <= disc sein.
Aufruf: python3 src/pruefe_reell16.py datei.txt [--ohne-nullspalte] [--stichproben N] | --construct32 n
"""
import sys
import random
from fractions import Fraction
from math import lcm
import numpy as np


def lade(pfad, ohne_nullspalte):
    A = np.loadtxt(pfad, dtype=np.int64, ndmin=2)
    if ohne_nullspalte:
        assert (A[:, -1] == 0).all(), "letzte Spalte ist keine Nullspalte"
        A = A[:, :-1]
    assert set(np.unique(A).tolist()) <= {-1, 1}
    return A


def alle_x(n):
    """Alle x in {+-1}^n mit x_0 = +1 (2^(n-1) Stück), als int64-Matrix."""
    idx = np.arange(1 << (n - 1), dtype=np.int64)
    bits = (idx[:, None] >> np.arange(n - 1)) & 1
    X = np.ones((1 << (n - 1), n), dtype=np.int64)
    X[:, 1:] = 1 - 2 * bits
    return X


def teil1_2(A, name):
    m, n = A.shape
    X = alle_x(n)
    V = X @ A.T                       # (2^(n-1), m)
    M = np.abs(V).max(axis=1)
    d = int(M.min())
    krit = M == d
    traeger = (np.abs(V) == d) & krit[:, None]
    anz = traeger.sum(axis=1)
    print(f"[{name}] {m} x {n}: disc = {d}, x bis auf Vorzeichen: {len(X)}")
    print(f"  kritisch: {int(krit.sum())}; von 1 Zeile: {int((anz == 1).sum())}, 2 Zeilen: {int((anz == 2).sum())}, "
          f"3 Zeilen: {int((anz == 3).sum())}, mehr: {int((anz > 3).sum())}")
    allein = np.where(anz == 1)[0]
    zeile_von = traeger[allein].argmax(axis=1)
    ok = True
    ergebnis = []
    for i in range(m):
        xs = allein[zeile_von == i]
        Ds = set()
        h = [0] * n
        groessen = set()
        for k in xs:
            s = 1 if V[k, i] > 0 else -1
            D = frozenset(j for j in range(n) if A[i, j] * X[k, j] * s < 0)
            groessen.add(len(D))
            Ds.add(D)
        for D in Ds:
            for j in D:
                h[j] += 1
        # D bestimmt x (bis auf Vorzeichen) eindeutig: Anzahl x = Anzahl D
        ok &= len(Ds) == len(xs)
        ergebnis.append((i, len(xs), len(Ds), sorted(groessen), max(h) if h else 0))
    print("  Zeile: allein getragene x | verschiedene D | |D| | größtes h_j")
    for i, nx, nd, gr, mh in ergebnis:
        print(f"   {i:2d}: {nx:4d} | {nd:4d} | {gr} | {mh}")
    print(f"  D <-> x eindeutig (Anzahl x = Anzahl D, je Zeile): {ok}")
    return d, ergebnis, X, V, allein, zeile_von


def formel_summe(A, i, X, V, allein, zeile_von, e, c):
    """Exakt (Brüche): Summe über alle D der Zeile i des neuen Zeilenwerts 7 + 2 e(D) - sum(e) - |c|
    gegen direkte Berechnung. Rückgabe: (kleinster Zeilenwert, Mittelwert, Formelmittelwert)."""
    m, n = A.shape
    d = int(np.abs(V).max(axis=1).min())
    seen = {}
    for k in allein[zeile_von == i]:
        s = 1 if V[k, i] > 0 else -1
        D = frozenset(j for j in range(n) if A[i, j] * X[k, j] * s < 0)
        seen[D] = (k, s)
    werte = []
    for D, (k, s) in seen.items():
        direkt = sum(Fraction(int(A[i, j]) * int(X[k, j]) * s) * (1 - e[j]) for j in range(n)) - abs(c)
        formel = d + 2 * sum(e[j] for j in D) - sum(e) - abs(c)
        assert direkt == formel, (direkt, formel)
        werte.append(direkt)
    return min(werte), sum(werte) / len(werte), len(werte)


def zufalls_B(A, rng, q):
    """Reelle Matrix B (Einträge Vielfache von 1/q, in [-1,1]), Zeilen L1-Abstand <= 2 von (A|0).
    Rückgabe B als Liste von Listen von Fractions, plus e und c je Zeile."""
    m, n = A.shape
    B = []
    es = []
    cs = []
    for i in range(m):
        modus = rng.choice(["streuen", "ein_eintrag", "wenige", "spalte_c"])
        budget = rng.choice([2 * q, 2 * q, rng.randrange(1, 2 * q + 1)])   # in Einheiten 1/q, höchstens 2
        w = [0] * (n + 1)       # w[j] für j<n: e_j * q, w[n]: |c| * q
        if modus == "streuen":
            for _ in range(budget):
                w[rng.randrange(n + 1)] += 1
        elif modus == "ein_eintrag":
            j = rng.randrange(n + 1)
            w[j] = budget
        elif modus == "wenige":
            for _ in range(budget):
                w[rng.choice(rng.sample(range(n + 1), 3))] += 1
        else:
            w[n] = rng.randrange(budget + 1)
            for _ in range(budget - w[n]):
                w[rng.randrange(n)] += 1
        # |c| <= 1 (Eintrag muss in [-1,1] liegen): Überschuss auf eine Spalte verlegen
        if w[n] > q:
            w[rng.randrange(n)] += w[n] - q
            w[n] = q
        # e_j <= 2 folgt aus Summe <= 2
        e = [Fraction(w[j], q) for j in range(n)]
        c = Fraction(w[n], q) * rng.choice([-1, 1])
        B.append([int(A[i, j]) * (1 - e[j]) for j in range(n)] + [c])
        es.append(e)
        cs.append(c)
    return B, es, cs


def disc_ganzzahlig(Bq, n1):
    """Exakte Diskrepanz einer ganzzahligen Matrix (Zeilen x n1) über alle 2^(n1-1) Vektoren."""
    X = alle_x(n1)
    return int(np.abs(X @ Bq.T).max(axis=1).min())


def teil3(A, d, X, V, allein, zeile_von, stichproben, seed):
    m, n = A.shape
    rng = random.Random(seed)
    fehler = 0
    max_disc = None
    min_mittel_delta = None
    for t in range(stichproben):
        q = rng.choice([1, 2, 3, 4, 5, 6, 8, 12])
        B, es, cs = zufalls_B(A, rng, q)
        # L1-Abstand je Zeile prüfen
        for i in range(m):
            dist = sum(abs(B[i][j] - int(A[i, j])) for j in range(n)) + abs(B[i][n])
            assert dist <= 2, dist
            assert all(abs(b) <= 1 for b in B[i])
        Bq = np.array([[int(b * q) for b in zeile] for zeile in B], dtype=np.int64)
        assert all(Fraction(int(Bq[i, j]), q) == B[i][j] for i in range(m) for j in range(n + 1))
        dq = disc_ganzzahlig(Bq, n + 1)          # = q * disc(B)
        disc_B = Fraction(dq, q)
        max_disc = disc_B if max_disc is None else max(max_disc, disc_B)
        if disc_B > d:
            fehler += 1
            print(f"  GEGENBEISPIEL zu Schritt 3: Stichprobe {t}, q={q}, disc(B) = {disc_B} > {d}")
        # Formelprüfung für eine zufällige Zeile (Schritt 3 selbst)
        i = rng.randrange(m)
        mn, mittel, anzD = formel_summe(A, i, X, V, allein, zeile_von, es[i], cs[i])
        assert mittel <= d - abs(cs[i]), (mittel, d)       # Mittelwert über D <= 7 - |c|
        assert mn <= d                                      # also ein D mit Wert <= 7
    print(f"  Stichproben: {stichproben}, Gegenbeispiele (disc(B) > {d}): {fehler}, größte disc(B) = {max_disc}")
    return fehler


if __name__ == "__main__":
    args = sys.argv[1:]
    stich = 300
    if "--stichproben" in args:
        k = args.index("--stichproben")
        stich = int(args[k + 1])
        del args[k:k + 2]
    if args and args[0] == "--construct32":
        sys.path.insert(0, __file__.rsplit("/", 1)[0])
        from construct_32 import code
        n = int(args[1])
        C = code(n)
        rows = [C[i] for i in range(0, 32, 2)]
        A = np.array([[-1 if (c >> j) & 1 else 1 for j in range(n)] for c in rows], dtype=np.int64)
        name = f"construct_32 n={n}"
    else:
        A = lade(args[0], "--ohne-nullspalte" in args)
        name = args[0] + (" ohne Nullspalte" if "--ohne-nullspalte" in args else "")
    d, erg, X, V, allein, zeile_von = teil1_2(A, name)
    print("  Bedingung von Schritt 3, 2*h_j <= Anzahl D (je Zeile, alle j):", all(2 * mh <= nd for _, _, nd, _, mh in erg))
    print("  Behauptung h_j <= 28:", all(mh <= 28 for _, _, _, _, mh in erg))
    f = teil3(A, d, X, V, allein, zeile_von, stich, seed=20261124)
    sys.exit(1 if f else 0)
