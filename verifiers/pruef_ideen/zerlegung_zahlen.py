#!/usr/bin/env python3
"""
zerlegung_zahlen.py -- Pruefrechnung (Auftrag P, Teil 4). Nur ganze Zahlen (numpy-Ganzzahlfelder).

Prueft die Zahlen der Zerlegung (notes/ideen_2026-11-25.md Abschnitt 3.2, Pruefvermerk, Tabelle zu Sol) mit eigenem Code:
  A  Paley-12 voll: Profile von u = H y.
  B  Schnittgroessen Radius-2-Kugel / Radius-4-Kugel in Q13, Bilanz bei m2 = 13, Zaehlschranken (m2 = 9, 11, 13);
     Stichproben: die Schranke "Summe A(x)(B(x)-1) >= 3960" bei zufaelligen W; Aequivalenz "Bedingung an W" gegen die
     tatsaechliche Ueberdeckung mit der vollen Paley-Haelfte.
  C  Paley-12 ohne Spalte 0 (12 x 11): Profile, Ketten C <= B <= A mit a + 3b + 6c <= 9, kleinstes |A|, Summen fuer m2 = 10, 12, 14.
  D  Lemma: U des 48ers ohne eine Zeile, y = diese Zeile: alle uebrigen |u_i| = 1 (Paley-12 und Datei controls/had24_1p3_paley12.txt).
Aufruf: python3 -I verifiers/pruef_ideen/zerlegung_zahlen.py [Teil ...]
"""
import sys
import itertools
from math import comb
import numpy as np


def paley12():
    q = 11
    res = {(x * x) % q for x in range(1, q)}
    chi = lambda a: 0 if a % q == 0 else (1 if a % q in res else -1)
    S = np.zeros((12, 12), dtype=np.int64)
    S[0, 1:] = 1
    S[1:, 0] = -1
    for i in range(q):
        for j in range(q):
            S[i + 1, j + 1] = chi(i - j)
    H = S + np.eye(12, dtype=np.int64)
    assert (H @ H.T == 12 * np.eye(12, dtype=np.int64)).all()
    return H


def klassen(m):
    """Alle y in {+-1}^m mit y_0 = +1, Zeilen des Feldes (2^(m-1) x m)."""
    k = np.arange(1 << (m - 1), dtype=np.int64)
    bits = (k[:, None] >> np.arange(m - 1)[None, :]) & 1
    y = np.ones((1 << (m - 1), m), dtype=np.int64)
    y[:, 1:] = 1 - 2 * bits
    return y


def V(m, r):
    return sum(comb(m, j) for j in range(r + 1))


# ---------------------------------------------------------------- Teil A
def teil_a():
    print("== Teil A: Paley-12 voll (12 x 12)")
    H = paley12()
    ys = klassen(12)
    u = np.abs(ys @ H.T)           # (2048, 12)
    prof = {}
    muster = {}
    for row in u:
        t = tuple(sorted(row.tolist(), reverse=True))
        prof[t] = prof.get(t, 0) + 1
    for row in u:
        t = tuple(sorted(row.tolist(), reverse=True))
        if t == (6, 6, 6) + (2,) * 9:
            muster.setdefault("6", set()).add(tuple(np.nonzero(row == 6)[0].tolist()))
        if t == (4,) * 9 + (0, 0, 0):
            muster.setdefault("0", set()).add(tuple(np.nonzero(row == 0)[0].tolist()))
    for t in sorted(prof, key=lambda t: -prof[t]):
        print("  Profil", t, ":", prof[t], "Klassen")
    n6 = prof.get((6, 6, 6) + (2,) * 9, 0)
    n0 = prof.get((4,) * 9 + (0, 0, 0), 0)
    rest = sum(c for t, c in prof.items() if t not in ((6, 6, 6) + (2,) * 9, (4,) * 9 + (0, 0, 0)))
    rest_ok = all(max(t) >= 8 for t in prof if t not in ((6, 6, 6) + (2,) * 9, (4,) * 9 + (0, 0, 0)))
    print("  (6^3,2^9): %d Klassen, %d Zeilenmuster (Dreiermengen der 6er-Zeilen)" % (n6, len(muster.get("6", ()))))
    print("  (4^9,0^3): %d Klassen, %d Zeilenmuster (Dreiermengen der Nullzeilen)" % (n0, len(muster.get("0", ()))))
    print("  uebrige: %d Klassen, jede mit einem Betrag >= 8: %s" % (rest, rest_ok))
    ok = (n6, len(muster.get("6", ())), n0, len(muster.get("0", ())), rest_ok) == (880, 220, 220, 220, True)
    print("  Erwartung 880/220, 220/220, uebrige >= 8:", "erfuellt" if ok else "ABWEICHUNG")
    return ok


# ---------------------------------------------------------------- Teil B
def kugel_schnitte():
    m = 13
    pts = np.arange(1 << m, dtype=np.int64)
    pop = np.array([bin(i).count("1") for i in range(1 << m)], dtype=np.int64)
    out = []
    for delta in range(7):
        x = 0
        y = (1 << delta) - 1
        a = pop[pts ^ x] <= 2
        b = pop[pts ^ y] <= 4
        out.append(int(np.sum(a & b)))
    return out


def teil_b():
    print("== Teil B: Schnitte und Bilanz")
    ok = True
    sch = kugel_schnitte()
    print("  |B(x,2) ∩ B(y,4)| in Q13 bei Abstand 0..6:", sch, "(erwartet [92, 92, 92, 47, 47, 15, 15])")
    ok &= sch == [92, 92, 92, 47, 47, 15, 15]
    for m2 in (9, 11, 13):
        r5, r9 = (m2 - 5) // 2, (m2 - 9) // 2
        vorh = 24 * V(m2, r5)
        noetig = 4 * (1 << m2) - 3 * 24 * V(m2, r9)
        print("  m2 = %d: V(m2,%d) = %d, V(m2,%d) = %d; vorhanden 24*%d = %d, noetig %d, Spielraum %d" %
              (m2, r5, V(m2, r5), r9, V(m2, r9), V(m2, r5), vorh, noetig, vorh - noetig))
        if m2 == 13:
            ok &= (vorh, noetig, vorh - noetig) == (26232, 26144, 88)
        if m2 == 9:
            ok &= vorh - noetig == -872
        if m2 == 11:
            ok &= vorh - noetig == -1760
    ueberz = 24 * 11 * min(sch[d] for d in range(7))
    print("  ueberzaehlig mindestens 24*11*%d = %d; erlaubt hoechstens 88 - 3D + 11D mit D <= 29: %d" % (min(sch), ueberz, 88 + 8 * 29))
    ok &= ueberz == 3960 and 88 + 8 * 29 == 320
    # Stichprobe der Schranke bei zufaelligen W (12 x 13): Summe_x A(x)(B(x)-1) >= 3960
    rng = np.random.default_rng(1)
    pts = np.arange(1 << 13, dtype=np.int64)
    pop = np.array([bin(i).count("1") for i in range(1 << 13)], dtype=np.int64)
    kleinste = None
    for versuch in range(300):
        W = rng.integers(0, 1 << 13, size=12)
        if versuch < 100:   # auch eng beieinanderliegende Zeilen (kleine Abstaende), die viele Schnitte haben
            W = np.array([int(W[0]) ^ int(rng.integers(0, 1 << 4)) for _ in range(12)])
        words = np.concatenate([W, W ^ ((1 << 13) - 1)])
        d = pop[pts[:, None] ^ words[None, :]]       # (8192, 24)
        A = (d <= 2).sum(axis=1)
        B = (d <= 4).sum(axis=1)
        s = int((A * (B - 1)).sum())
        if kleinste is None or s < kleinste:
            kleinste = s
    print("  Stichprobe 300 zufaellige W: kleinste Summe A(x)(B(x)-1) = %d (verlangt >= 3960): %s" % (kleinste, "ok" if kleinste >= 3960 else "VERLETZT"))
    ok &= kleinste >= 3960
    # Aequivalenz: Bedingung an W gegen tatsaechliche Ueberdeckung mit U = Paley-12 voll, d = 9
    H = paley12()
    ys = klassen(12)
    absU = np.abs(ys @ H.T)                          # (2048, 12)
    gleich = nein = 0
    wahr = falsch = 0
    for versuch in range(12):
        m2 = 13
        W = rng.choice([-1, 1], size=(12, m2)).astype(np.int64)
        if versuch >= 6:  # Zeilen von W als verrauschte Kopien einer Zeile (damit viele |v| gross sind)
            W = np.tile(W[0], (12, 1)) * rng.choice([1, 1, 1, 1, -1], size=(12, m2))
        zs = klassen(m2)
        absV = np.abs(zs @ W.T)                      # (4096, 12)
        # tatsaechlich: fuer jedes z gilt max_i (|u_i(y)| + |v_i(z)|) >= 9 fuer alle y
        tats = np.zeros(len(zs), dtype=bool)
        for zi in range(len(zs)):
            tats[zi] = bool(((absU + absV[zi][None, :]).max(axis=1) >= 9).all())
        # Bedingung der Herleitung
        bed = np.zeros(len(zs), dtype=bool)
        for zi in range(len(zs)):
            v = absV[zi]
            bed[zi] = bool(v.max() >= 9 or ((v >= 5).sum() >= 4 and (v.max() >= 7 or (v == 1).sum() <= 2)))
        if (tats == bed).all():
            gleich += 1
        else:
            nein += 1
        wahr += int(tats.sum())
        falsch += int((~tats).sum())
    print("  Aequivalenz Bedingung <-> Ueberdeckung (12 Matrizen W, je 4096 Klassen z, davon %d erfuellt, %d nicht): gleich in %d von 12 Matrizen" % (wahr, falsch, gleich))
    ok &= nein == 0 and wahr > 0 and falsch > 0
    return ok


# ---------------------------------------------------------------- Teil C
def teil_c():
    print("== Teil C: Paley-12 ohne Spalte 0 (12 x 11), d = 9")
    H = paley12()[:, 1:]
    ys = klassen(11)
    u = np.abs(ys @ H.T)
    prof = {}
    for row in u:
        t = tuple(sorted(row.tolist(), reverse=True))
        prof[t] = prof.get(t, 0) + 1
    for t in sorted(prof, key=lambda t: -prof[t]):
        print("  Profil", t, ":", prof[t], "Klassen")
    ok = True
    ziel = (5, 5, 5) + (3,) * 6 + (1,) * 3
    print("  Profil (5^3,3^6,1^3):", prof.get(ziel, 0), "Klassen (erwartet 220)")
    ok &= prof.get(ziel, 0) == 220
    muster = []
    for row in u:
        if tuple(sorted(row.tolist(), reverse=True)) == ziel:
            F = sum(1 << i for i in range(12) if row[i] == 5)
            R3 = sum(1 << i for i in range(12) if row[i] == 3)
            R1 = sum(1 << i for i in range(12) if row[i] == 1)
            muster.append((F, R3, R1))
    print("  verschiedene Dreiermengen der 5er-Zeilen:", len({m[0] for m in muster}), "(erwartet 220)")
    ok &= len({m[0] for m in muster}) == 220

    def erfuellt(A, B, C):
        return all((F & A) or (R3 & B) or (R1 & C) for (F, R3, R1) in muster)

    pc = [bin(i).count("1") for i in range(1 << 12)]
    alle = range(1 << 12)
    # Ketten C <= B <= A mit a + 3b + 6c <= 9
    bester = None
    anz_ketten = 0
    anz_erfuellend = 0
    kleinstes_A = {}
    for A in alle:
        a = pc[A]
        if a > 9:
            continue
        # B Teilmenge von A
        B = A
        while True:
            b = pc[B]
            if a + 3 * b > 9:
                if B == 0:
                    break
                B = (B - 1) & A
                continue
            # C Teilmenge von B
            C = B
            while True:
                c = pc[C]
                kosten = a + 3 * b + 6 * c
                if kosten <= 9:
                    anz_ketten += 1
                    if erfuellt(A, B, C):
                        anz_erfuellend += 1
                if C == 0:
                    break
                C = (C - 1) & B
            if B == 0:
                break
            B = (B - 1) & A
    print("  Ketten C <= B <= A mit a + 3b + 6c <= 9: %d, davon erfuellen alle 220 Muster: %d (erwartet 0)" % (anz_ketten, anz_erfuellend))
    ok &= anz_erfuellend == 0
    # kleinstes |A| bei |B| = 0, 1, 2 und C leer, und bei beliebigem a + 3b + 6c das Minimum der Kosten
    for b_ziel in (0, 1, 2):
        best = None
        for A in alle:
            if best is not None and pc[A] >= best:
                continue
            ok_A = False
            for B in range(1 << 12):
                if B & ~A or pc[B] != b_ziel:
                    continue
                if erfuellt(A, B, 0):
                    ok_A = True
                    break
            if ok_A:
                best = pc[A]
        kleinstes_A[b_ziel] = best
    print("  kleinstes |A| bei |B| = 0, 1, 2 und C leer:", [kleinstes_A[k] for k in (0, 1, 2)], "(erwartet [10, 7, 6])")
    ok &= [kleinstes_A[k] for k in (0, 1, 2)] == [10, 7, 6]
    # Summen
    for m2, soll in ((10, (4920, 5120)), (12, (25980, 20480)), (14, (128472, 81920))):
        zs = klassen(m2)
        v = np.abs(zs.sum(axis=1))               # eine feste Zeile (alles +1): Betrag der Zeilensumme je Klasse
        q4, q6, q8 = int((v >= 4).sum()), int((v >= 6).sum()), int((v >= 8).sum())
        lhs = 12 * (q4 + 3 * q6 + 6 * q8)
        rhs = 10 * (1 << (m2 - 1))
        print("  m2 = %d: (q4,q6,q8) = (%d,%d,%d); 12(q4+3q6+6q8) = %d gegen 10*2^%d = %d; Differenz %d" % (m2, q4, q6, q8, lhs, m2 - 1, rhs, lhs - rhs))
        ok &= (lhs, rhs) == soll
    return ok


# ---------------------------------------------------------------- Teil D
def teil_d():
    print("== Teil D: Lemma (U des 48ers ohne eine Zeile, y = diese Zeile: alle uebrigen |u_i| = 1)")
    H = paley12()
    ok = True
    # alle Spaltenstreichungen
    schlecht = 0
    for c in range(12):
        U = np.delete(H, c, axis=1)                  # 12 x 11
        for j in range(12):
            y = U[j]
            u = U @ y
            u = np.delete(u, j)
            if not (np.abs(u) == 1).all():
                schlecht += 1
    print("  Paley-12 ohne eine Spalte (12 Spalten) x 12 Zeilen: Abweichungen", schlecht)
    ok &= schlecht == 0
    # Datei des 48ers: linke 11 Spalten der Zeilen 0..11
    M = np.array([[int(t) for t in l.split()] for l in open("controls/had24_1p3_paley12.txt") if l.strip()], dtype=np.int64)
    U = M[:12, :11]
    schlecht2 = 0
    for j in range(12):
        u = U @ U[j]
        u = np.delete(u, j)
        if not (np.abs(u) == 1).all():
            schlecht2 += 1
    # auch die Zeilen 12..23 haben dieselbe linke Haelfte
    gleich = bool((M[12:, :11] == U).all())
    print("  controls/had24_1p3_paley12.txt, linker Block (12 x 11): Abweichungen %d von 12; untere Haelfte hat dieselbe linke Haelfte: %s" % (schlecht2, gleich))
    ok &= schlecht2 == 0
    return ok


def main():
    teile = sys.argv[1:] or ["A", "B", "C", "D"]
    ergebnis = {}
    for t in teile:
        ergebnis[t] = {"A": teil_a, "B": teil_b, "C": teil_c, "D": teil_d}[t]()
    print("ERGEBNIS:", ", ".join("%s %s" % (t, "bestaetigt" if ok else "ABWEICHUNG") for t, ok in ergebnis.items()))
    sys.exit(0 if all(ergebnis.values()) else 1)


if __name__ == "__main__":
    main()
