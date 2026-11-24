"""Hadamard-Tests (24.11.2026): exakte Diskrepanz, nur ganze Zahlen (int8/int64).

Kein Suchcode. disc(A) = min ueber x in {-1,1}^m von max_i |(Ax)_i|, exakt per
Meet-in-the-middle ueber alle Vorzeichenvektoren (x_0 = +1 wegen Symmetrie).

Aufrufe (aus dem Wurzelordner):
  python3 src/hadamard_tests.py kontrolle   Sylvester 8; Zaehlung 660/1980 und 1320 fuer Paley 12
  python3 src/hadamard_tests.py klassen20   Ordnung 20: 3 Klassen bauen, Test B (volle Matrix)
  python3 src/hadamard_tests.py klassen24   Ordnung 24: konstruierbare Klassen, Test A
"""
import hashlib
import itertools
import os
import sys
import time
import random
import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from construct_48 import paley1, doubling, streiche, schreibe  # noqa: E402


# ---------------------------------------------------------------- exakte Diskrepanz
def _summen(A, cols):
    k = len(cols)
    idx = np.arange(1 << k, dtype=np.int64)
    X = 1 - 2 * ((idx[:, None] >> np.arange(k)) & 1)
    return (X @ A[:, cols].T.astype(np.int64)).astype(np.int8)


def exakte_disc(A):
    """Exakte Diskrepanz der ganzzahligen Matrix A (Eintraege -1,0,1), alle Spalten aktiv."""
    A = np.asarray(A, dtype=np.int64)
    r, m = A.shape
    aktiv = [j for j in range(m) if np.any(A[:, j] != 0)]
    A = A[:, aktiv]
    m = len(aktiv)
    assert 2 <= m <= 40 and np.abs(A).max() <= 1
    mL = m // 2
    SL = _summen(A, list(range(mL)))[0::2]          # x_0 = +1
    SR = _summen(A, list(range(mL, m)))
    best = 127
    chunk = max(1, (1 << 26) // (SR.shape[0] * r))
    for s in range(0, len(SL), chunk):
        V = SL[s:s + chunk, None, :] + SR[None, :, :]
        best = min(best, int(np.abs(V).max(axis=2).min()))
    return best


# ---------------------------------------------------------------- Hadamard-Bausteine
def pruefe_hadamard(H):
    n = H.shape[0]
    assert H.shape == (n, n) and set(np.unique(H)) <= {-1, 1}
    assert (H @ H.T == n * np.eye(n, dtype=np.int64)).all()


def sylvester(k):
    H = np.array([[1]], dtype=np.int64)
    for _ in range(k):
        H = np.block([[H, H], [H, -H]])
    return H


_COMBO_CACHE = {}


def typen6(H):
    """Invariante unter Zeilen-/Spaltenpermutation und -negation: fuer jede 6-elementige Zeilenmenge
    der sortierte Vektor der 15 Betraege |sum_c Produkt der 4 Zeilen| ihrer Vierermengen; gezaehlt wird,
    wie oft jeder Vektor vorkommt. (Bei Ordnung 20 trennt das die drei bekannten Klassen.)"""
    n = H.shape[0]
    if n not in _COMBO_CACHE:
        _COMBO_CACHE[n] = (np.array(list(itertools.combinations(range(n), 6)), dtype=np.int16),
                           list(itertools.combinations(range(6), 4)),
                           np.array(list(itertools.combinations(range(n), 4))))
    combos, sub, quads = _COMBO_CACHE[n]
    P = H[quads[:, 0]] * H[quads[:, 1]] * H[quads[:, 2]] * H[quads[:, 3]]
    S4 = np.zeros((n, n, n, n), dtype=np.uint8)
    S4[quads[:, 0], quads[:, 1], quads[:, 2], quads[:, 3]] = np.abs(P.sum(axis=1))
    T = np.sort(np.stack([S4[combos[:, a], combos[:, b], combos[:, c], combos[:, d]] for a, b, c, d in sub], axis=1), axis=1)
    u, c = np.unique(T, axis=0, return_counts=True)
    return hashlib.sha1(u.tobytes() + c.astype(np.int64).tobytes()).hexdigest()


def schluessel(H):
    return (typen6(H), typen6(H.T.copy()))


def gf9_paley2():
    """Paley II, Ordnung 20 (q = 9)."""
    els = [(a, b) for a in range(3) for b in range(3)]          # a + b*i, i^2 = -1

    def sub(x, y):
        return ((x[0] - y[0]) % 3, (x[1] - y[1]) % 3)

    def mul(x, y):
        return ((x[0] * y[0] - x[1] * y[1]) % 3, (x[0] * y[1] + x[1] * y[0]) % 3)

    quad = {mul(x, x) for x in els if x != (0, 0)}
    chi = lambda x: 0 if x == (0, 0) else (1 if x in quad else -1)
    q = 9
    C = np.zeros((q + 1, q + 1), dtype=np.int64)
    C[0, 1:] = 1
    C[1:, 0] = 1
    for i, x in enumerate(els):
        for j, y in enumerate(els):
            C[i + 1, j + 1] = chi(sub(x, y))
    assert (C @ C.T == q * np.eye(q + 1, dtype=np.int64)).all() and (C == C.T).all()
    A = np.array([[1, 1], [1, -1]], dtype=np.int64)
    B = np.array([[1, -1], [-1, -1]], dtype=np.int64)
    H = np.kron(C, A) + np.kron(np.eye(q + 1, dtype=np.int64), B)
    pruefe_hadamard(H)
    return H


def zirkulante(c):
    n = len(c)
    return np.array([[c[(j - i) % n] for j in range(n)] for i in range(n)], dtype=np.int64)


def goethals_seidel(a, b, c, d):
    n = len(a)
    A, B, C, D = (zirkulante(v) for v in (a, b, c, d))
    R = np.fliplr(np.eye(n, dtype=np.int64))
    return np.block([
        [A, B @ R, C @ R, D @ R],
        [-B @ R, A, D.T @ R, -C.T @ R],
        [-C @ R, -D.T @ R, A, B.T @ R],
        [-D @ R, C.T @ R, -B.T @ R, A]])


def gs_matrizen(n, anzahl, rng):
    """Goethals-Seidel-Matrizen der Ordnung 4n aus vier +-1-Zirkulanten der Laenge n:
    gesucht werden Quadruple, deren Leistungsspektren sich zu 4n je Frequenz addieren."""
    vek = list(itertools.product((1, -1), repeat=n))
    spek = np.array([np.round(np.abs(np.fft.fft(v)) ** 2).astype(np.int64) for v in vek])   # |.|^2 ganzzahlig
    N = len(vek)
    paare = {}
    for i in range(N):
        for j in range(i, N):
            paare.setdefault(tuple((spek[i] + spek[j]).tolist()), []).append((i, j))
    ziel = 4 * n
    treffer = []
    for k1, l1 in paare.items():
        k2 = tuple(ziel - x for x in k1)
        if k2 in paare and k1 <= k2:
            treffer.append((l1, paare[k2]))
    out = []
    for _ in range(anzahl * 4):
        l1, l2 = rng.choice(treffer)
        (i, j), (k, l) = rng.choice(l1), rng.choice(l2)
        H = goethals_seidel(*[list(vek[t]) for t in (i, j, k, l)])
        if (H @ H.T == 4 * n * np.eye(4 * n, dtype=np.int64)).all():
            out.append(H)
            if len(out) >= anzahl:
                break
    return out


def doppelung_perm(H, rng):
    """[[A, P A],[A, -P A]] mit zufaelliger Zeilenpermutation P: stets Hadamard der doppelten Ordnung."""
    n = H.shape[0]
    p = list(range(n))
    rng.shuffle(p)
    B = H[p]
    return np.block([[H, B], [H, -B]])


def normiere(H):
    """Spalten negieren, so dass Zeile 0 nur +1 hat; Zeilen so negieren, dass Spalte 0 nur +1 hat."""
    H = H * H[0][None, :]
    return H * H[:, 0][:, None]


def ergaenze(zeilen, n, rng, versuche=200):
    """Zufaellige Ergaenzung vorgegebener normierter Zeilen (Zeile 0 = Einsvektor, Spalte 0 = +1) zu
    einer Hadamard-Matrix der Ordnung n; Zeilen als Bitmasken (Bit 1 = -1). Gierig mit Neustart."""
    allv = np.arange(1 << (n - 1), dtype=np.uint64) << np.uint64(1)    # Bit 0 = 0 (Eintrag +1)
    allv = allv[np.bitwise_count(allv) == n // 2]
    for _ in range(versuche):
        cand = allv
        for z in zeilen:
            cand = cand[np.bitwise_count(cand ^ np.uint64(z)) == n // 2]
        reihen = list(zeilen)
        while len(reihen) < n and len(cand):
            c = cand[rng.randrange(len(cand))]
            reihen.append(int(c))
            cand = cand[np.bitwise_count(cand ^ c) == n // 2]
        if len(reihen) == n:
            return np.array([[1 - 2 * ((r >> j) & 1) for j in range(n)] for r in reihen], dtype=np.int64)
    return None


def bitmasken(H):
    return [sum(1 << j for j in range(H.shape[1]) if H[i, j] == -1) for i in range(H.shape[0])]


# ---------------------------------------------------------------- Kontrollen
def kontrolle():
    t0 = time.time()
    S8 = sylvester(3)
    pruefe_hadamard(S8)
    d8 = exakte_disc(S8)
    d7 = [exakte_disc(np.delete(S8, j, axis=1)) for j in range(8)]
    print(f"Sylvester 8: volle Matrix disc = {d8}; ohne je eine Spalte (8 Faelle): {d7}")
    assert set(d7) == {5}, "Kontrolle Sylvester 8 ohne Spalte muss 5 sein"
    S8T = S8.T
    print("Sylvester 8 ist symmetrisch:", bool((S8 == S8T).all()))

    H = paley1(11)
    M = doubling(H)
    pruefe_hadamard(M)
    print(f"Paley 12: volle Matrix {exakte_disc(H)}; M = [[H,H],[H,-H]] voll: {exakte_disc(M)}")
    zaehl = {}
    for l in range(12):
        for r in itertools.combinations(range(12), 3):
            d = exakte_disc(streiche(M, [l], list(r)))
            ueber = int(l in r)
            zaehl[(ueber, d)] = zaehl.get((ueber, d), 0) + 1
    print("1+3 (Ueberlappung, Diskrepanz) -> Anzahl:", dict(sorted(zaehl.items())))
    assert zaehl == {(0, 6): 1980, (1, 8): 660}, zaehl
    z2 = {}
    for l in itertools.combinations(range(12), 2):
        for r in itertools.combinations(range(12), 2):
            ueber = len(set(l) & set(r))
            d = exakte_disc(streiche(M, list(l), list(r)))
            z2[(ueber, d)] = z2.get((ueber, d), 0) + 1
    print("2+2 (Ueberlappung, Diskrepanz) -> Anzahl:", dict(sorted(z2.items())))
    assert z2 == {(0, 6): 2970, (1, 8): 1320, (2, 6): 66}, z2
    # 0+4 und 1+1 laut Pruefvermerk
    z04 = {}
    for r in itertools.combinations(range(12), 4):
        d = exakte_disc(streiche(M, [], list(r)))
        z04[d] = z04.get(d, 0) + 1
    z11 = {}
    for l in range(12):
        for r in range(12):
            d = exakte_disc(streiche(M, [l], [r]))
            z11[(int(l == r), d)] = z11.get((int(l == r), d), 0) + 1
    print("0+4 Diskrepanz -> Anzahl:", z04, "; 1+1 (gleiche Spalte, Diskrepanz) -> Anzahl:", dict(sorted(z11.items())))
    print(f"Kontrolle bestanden ({time.time() - t0:.0f} s)")

    # Gegenprobe gegen das unabhaengige Pruefskript an einer Stichprobe
    import check_disc
    rng = random.Random(1)
    for _ in range(6):
        l = rng.sample(range(12), 1)
        r = rng.sample(range(12), 3)
        A = streiche(M, l, r)
        a, b = exakte_disc(A), check_disc.disc(A)
        print(f"  Stichprobe links {l} rechts {sorted(r)}: hadamard_tests {a}, check_disc {b}")
        assert a == b


# ---------------------------------------------------------------- Ordnung 20 (Test B)
def klassen20(rng):
    kandidaten = [("Paley I (q=19)", paley1(19)), ("Paley II (q=9)", gf9_paley2())]
    for n_gs in gs_matrizen(5, 40, rng):
        kandidaten.append(("Goethals-Seidel", n_gs))
    for i in range(30):
        base = normiere(paley1(19) if i % 2 else gf9_paley2())
        k = rng.choice([4, 5, 6, 7])
        Hn = ergaenze(bitmasken(base)[:k], 20, rng)
        if Hn is not None:
            kandidaten.append((f"Ergaenzung aus {k} Zeilen", Hn))
    klassen = {}
    for name, H in kandidaten:
        pruefe_hadamard(H)
        klassen.setdefault(schluessel(H), []).append((name, H))
    return klassen, len(kandidaten)


def test_b(rng):
    t0 = time.time()
    klassen, k = klassen20(rng)
    print(f"Ordnung 20: {k} Matrizen gebaut, {len(klassen)} verschiedene Invarianten-Schluessel (bekannt: 3 Klassen)")
    assert len(klassen) == 3
    os.makedirs("controls", exist_ok=True)
    ergebnis = []
    for nr, (key, liste) in enumerate(sorted(klassen.items(), key=lambda kv: kv[0]), 1):
        name, H = liste[0]
        H = normiere(H)
        pruefe_hadamard(H)
        schreibe(f"controls/had20_klasse{nr}.txt", H)
        d, dT = exakte_disc(H), exakte_disc(H.T.copy())
        # Gegenprobe: alle Matrizen mit gleichem Schluessel (hoechstens 5) haben dieselbe Diskrepanz
        gleich = {exakte_disc(h) for _, h in liste[:5]}
        print(f"  Klasse {nr}: Vertreter '{name}', {len(liste)} Matrizen mit diesem Schluessel; "
              f"disc = {d}, disc der Transponierten = {dT}, Stichprobe gleicher Schluessel: {sorted(gleich)}")
        ergebnis.append((d, dT))
    rekord = [e for e in ergebnis if max(e) >= 8]
    print("Test B Rekordkandidaten (Diskrepanz >= 8):", rekord if rekord else "keine (erwartet 6)")
    print(f"Test B fertig ({time.time() - t0:.0f} s)")
    return len(klassen)


# ---------------------------------------------------------------- Ordnung 24 (Test A)
def sammle24(rng, zeit_s):
    t0 = time.time()
    klassen = {}

    def aufnehmen(name, H):
        pruefe_hadamard(H)
        key = schluessel(H)
        if key not in klassen:
            klassen[key] = []
        klassen[key].append((name, H))

    P23 = paley1(23)
    aufnehmen("Paley I (q=23)", P23)
    P12 = paley1(11)
    aufnehmen("Doppelung Paley 12 (P = id)", doubling(P12))
    for _ in range(60):
        aufnehmen("Doppelung Paley 12 mit Zeilenpermutation", doppelung_perm(P12, rng))
    for H in gs_matrizen(6, 60, rng):
        aufnehmen("Goethals-Seidel n=6", H)
    print(f"  nach Konstruktionen: {len(klassen)} Schluessel ({time.time() - t0:.0f} s)")
    basen = [h for liste in klassen.values() for _, h in liste[:2]]
    n_erg = 0
    while time.time() - t0 < zeit_s:
        base = normiere(rng.choice(basen))
        k = rng.choice([4, 5, 6, 7, 8])
        Hn = ergaenze(bitmasken(base)[:k], 24, rng, versuche=20)
        if Hn is None:
            continue
        n_erg += 1
        aufnehmen(f"Ergaenzung aus {k} Zeilen", Hn)
    print(f"  Ergaenzungen: {n_erg}; Schluessel gesamt {len(klassen)} ({time.time() - t0:.0f} s)")
    return klassen


def test_a(rng, zeit_s):
    t0 = time.time()
    klassen = sammle24(rng, zeit_s)
    os.makedirs("runs/hadamard", exist_ok=True)
    print(f"Ordnung 24: {len(klassen)} verschiedene Invarianten-Schluessel gefunden (bekannt: 60 Klassen)")
    tabelle = []
    for nr, (key, liste) in enumerate(sorted(klassen.items(), key=lambda kv: kv[0]), 1):
        name, H = liste[0]
        H = normiere(H)
        schreibe(f"runs/hadamard/h24_klasse{nr:02d}.txt", H)
        d, dT = exakte_disc(H), exakte_disc(H.T.copy())
        gleich = {exakte_disc(h) for _, h in liste[:3]}
        zeile = {"nr": nr, "name": name, "n": len(liste), "disc": d, "discT": dT, "gleich": sorted(gleich), "spalten": None}
        for tag, M in (("H", H), ("HT", H.T.copy())):
            dd = d if tag == "H" else dT
            if dd == 8:
                loes = [exakte_disc(np.delete(M, j, axis=1)) for j in range(24)]
                zeile["spalten_" + tag] = loes
        tabelle.append(zeile)
        extra = ""
        for tag in ("H", "HT"):
            if "spalten_" + tag in zeile:
                extra += f"; ohne eine Spalte ({tag}): {sorted(set(zeile['spalten_' + tag]))} max {max(zeile['spalten_' + tag])}"
        print(f"  Klasse {nr:2d} ({name}, {len(liste)} Matrizen): disc {d}, Transponierte {dT}, Stichprobe {zeile['gleich']}{extra}")
    maxd = max(max(z["disc"], z["discT"]) for z in tabelle)
    rekord = [z for z in tabelle for tag in ("H", "HT") if "spalten_" + tag in z and max(z["spalten_" + tag]) >= 9]
    from collections import Counter
    print("Verteilung der vollen Diskrepanz (Matrix):", dict(Counter(z["disc"] for z in tabelle)),
          "(Transponierte):", dict(Counter(z["discT"] for z in tabelle)))
    print("Test A: groesste volle Diskrepanz", maxd, "; Rekordkandidaten (24x23 mit Diskrepanz >= 9):",
          len(rekord) if rekord else "keine")
    print(f"Test A fertig ({time.time() - t0:.0f} s)")


if __name__ == "__main__":
    cmd = sys.argv[1] if len(sys.argv) > 1 else ""
    rng = random.Random(int(sys.argv[2]) if len(sys.argv) > 2 else 1)
    if cmd == "kontrolle":
        kontrolle()
    elif cmd == "klassen20":
        test_b(rng)
    elif cmd == "klassen24":
        test_a(rng, float(sys.argv[3]) if len(sys.argv) > 3 else 120.0)
    else:
        print(__doc__)
