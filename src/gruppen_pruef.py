#!/usr/bin/env python3
"""Prüfcode zu Auftrag G (kein Suchcode, teilt keinen Code mit gruppenmatrix.c). Nur ganze Zahlen.

  gruppen_pruef.py zaehle GRP...            Klassenzahl per Burnside (Symmetriegruppe selbst aus der Tafel gebildet,
                                            Automorphismen selbst bestimmt, nicht aus der Datei gelesen)
  gruppen_pruef.py zaehle_zyklisch          Selbsttest der Burnside-Rechnung: Z_n, n = 3..15 gegen die direkte Zählung (Teil C)
                                            und für S3, Z2xZ4 direkte Bahnenzählung
  gruppen_pruef.py matrix HEX GRP [-r] [--ohne i,j] DATEI   Matrix A[g][h] = f(g^-1 h) schreiben
  gruppen_pruef.py exakt GRP LISTE [-r] [--streich] [-k MAX]
        exakte Diskrepanz (build/exakt_disc) für Zeilen "hex u" bzw. (mit --streich) "hex i j u" der Liste (höchstens MAX)
  gruppen_pruef.py ugegenexakt GRP LISTE [-r]    für alle Zeilen "hex u": exakt <= u? (Teil 1 d)
"""
import os
import subprocess
import sys
from itertools import product

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
import tempfile
TMPDATEI = os.path.join(tempfile.gettempdir(), "_gp_tmp_%d.txt" % os.getpid())
BIN = os.environ.get("EXAKT_BIN", os.path.join(ROOT, "build", "exakt_disc"))


# ---------------------------------------------------------------- Gruppe lesen (nur die Tafel)
def lies_tafel(fn):
    zeilen = [l for l in open(fn) if l.strip() and not l.startswith("#")]
    n = int(zeilen[0])
    tab = [list(map(int, zeilen[1 + g].split())) for g in range(n)]
    for g in range(n):
        assert tab[0][g] == g == tab[g][0]
    for a in range(n):
        for b in range(n):
            for c in range(n):
                assert tab[tab[a][b]][c] == tab[a][tab[b][c]]
    inv = [next(h for h in range(n) if tab[g][h] == 0) for g in range(n)]
    return n, tab, inv


def alle_automorphismen(n, tab):
    """Alle Automorphismen: ein Erzeugersystem kleinster Größe suchen, Bilder durchprobieren, Homomorphie voll prüfen."""
    def erz(gs):
        s = {0}
        neu = [0]
        while neu:
            x = neu.pop()
            for g in gs:
                y = tab[x][g]
                if y not in s:
                    s.add(y)
                    neu.append(y)
        return s
    gens = None
    for k in (1, 2, 3):
        for gs in product(range(1, n), repeat=k):
            if list(gs) == sorted(set(gs)) and len(erz(gs)) == n:
                gens = list(gs)
                break
        if gens:
            break
    def ordn(g):
        k, x = 1, g
        while x != 0:
            x, k = tab[x][g], k + 1
        return k
    # Wörter
    weg = {0: ()}
    schlange = [0]
    while schlange:
        x = schlange.pop(0)
        for i, g in enumerate(gens):
            y = tab[x][g]
            if y not in weg:
                weg[y] = weg[x] + (i,)
                schlange.append(y)
    auts = []
    for bilder in product(*[[h for h in range(n) if ordn(h) == ordn(g)] for g in gens]):
        phi = []
        for x in range(n):
            v = 0
            for i in weg[x]:
                v = tab[v][bilder[i]]
            phi.append(v)
        if len(set(phi)) == n and all(phi[tab[a][b]] == tab[phi[a]][phi[b]] for a in range(n) for b in range(n)):
            auts.append(tuple(phi))
    return auts


def symmetriegruppe(n, tab, mit_aut=True):
    """H = von x -> k x, x -> x l und den Automorphismen erzeugte Permutationsgruppe (Abschluss, ohne Formel)."""
    erz = [tuple(tab[k][x] for x in range(n)) for k in range(n)]
    erz += [tuple(tab[x][k] for x in range(n)) for k in range(n)]
    if mit_aut:
        erz += alle_automorphismen(n, tab)
    erz = list(set(erz))
    ident = tuple(range(n))
    H = {ident}
    schlange = [ident]
    while schlange:
        p = schlange.pop()
        for g in erz:
            q = tuple(g[p[x]] for x in range(n))
            if q not in H:
                H.add(q)
                schlange.append(q)
    return H


def zyklen(p):
    n = len(p)
    seen = [False] * n
    out = []
    for s in range(n):
        if not seen[s]:
            l = 0
            x = s
            while not seen[x]:
                seen[x] = True
                x = p[x]
                l += 1
            out.append(l)
    return out


def burnside(H):
    """Bahnen auf f in {0,1}^n unter H x {id, Komplement}. (Summe über sigma: Fixpunkte 2^c, plus Fixpunkte von
    f -> ~(f o sigma): nur wenn alle Zyklen gerade, dann 2^c.)"""
    gesamt = 0
    for p in H:
        z = zyklen(p)
        gesamt += 2 ** len(z)
        if all(l % 2 == 0 for l in z):
            gesamt += 2 ** len(z)
    assert gesamt % (2 * len(H)) == 0
    return gesamt // (2 * len(H))


def direkt(n, H):
    """Bahnen direkt zählen (nur für kleine n), jede Bahn durch Anwenden aller Gruppenelemente und Komplement."""
    gesehen = bytearray(1 << n)
    klassen = 0
    maske = (1 << n) - 1
    perms = list(H)
    for f in range(1 << n):
        if gesehen[f]:
            continue
        klassen += 1
        bits = [(f >> y) & 1 for y in range(n)]
        for p in perms:
            g = 0
            for y in range(n):
                if bits[y]:
                    g |= 1 << p[y]
            gesehen[g] = 1
            gesehen[g ^ maske] = 1
    return klassen


def zaehle(dateien):
    for fn in dateien:
        n, tab, inv = lies_tafel(fn)
        H = symmetriegruppe(n, tab)
        print(f"{os.path.basename(fn)}: n={n} |H|={len(H)} klassen={burnside(H)}", flush=True)


def zaehle_zyklisch():
    soll = [2, 4, 3, 8, 5, 14, 11, 26, 15, 88, 37, 130, 184]    # n = 3..15, Teil C (direkte Zählung)
    ok = True
    for n, s in zip(range(3, 16), soll):
        tab = [[(a + b) % n for b in range(n)] for a in range(n)]
        H = symmetriegruppe(n, tab)
        b = burnside(H)
        print(f"Z{n}: Burnside {b}, Teil C {s}, {'gleich' if b == s else 'ABWEICHUNG'}")
        ok &= (b == s)
    # direkte Zählung für nichtzyklische kleine Gruppen
    s3 = [[0] * 6 for _ in range(6)]
    from itertools import permutations
    el = list(permutations(range(3)))
    idx = {e: i for i, e in enumerate(el)}
    s3 = [[idx[tuple(a[b[x]] for x in range(3))] for b in el] for a in el]
    # Index des neutralen Elements nach vorne
    e0 = idx[(0, 1, 2)]
    perm = [e0] + [i for i in range(6) if i != e0]
    pos = {old: new for new, old in enumerate(perm)}
    s3n = [[pos[s3[perm[a]][perm[b]]] for b in range(6)] for a in range(6)]
    z2z4 = [[(a % 2 + b % 2) % 2 + 2 * (((a // 2) + (b // 2)) % 4) if False else 0 for b in range(8)] for a in range(8)]
    z2z4 = [[((a & 1) ^ (b & 1)) + 2 * (((a >> 1) + (b >> 1)) % 4) for b in range(8)] for a in range(8)]
    for name, t in (("S3", s3n), ("Z2xZ4", z2z4)):
        n = len(t)
        H = symmetriegruppe(n, t)
        b, dd = burnside(H), direkt(n, H)
        print(f"{name}: Burnside {b}, direkt {dd}, {'gleich' if b == dd else 'ABWEICHUNG'}")
        ok &= (b == dd)
    return 0 if ok else 1


# ---------------------------------------------------------------- Matrizen und exakte Prüfung
def matrix(f, n, tab, inv, rand, ohne=()):
    M = [[(-1 if (f >> tab[inv[g]][h]) & 1 else 1) for h in range(n)] for g in range(n)]
    if rand:
        M.append([1] * n)
    if ohne:
        M = [[v for j, v in enumerate(r) if j not in ohne] for r in M]
    if rand:
        M = [r + [0] for r in M]
    return M


def schreibe(M, fn):
    with open(fn, "w") as fh:
        for r in M:
            fh.write(" ".join(f"{v:2d}" for v in r) + "\n")


def exakt_matrix(M, fn):
    schreibe(M, fn)
    p = subprocess.run([BIN, fn], capture_output=True, text=True, check=True)
    out = p.stdout.strip()
    d = int(out.split("disc ")[1].split(",")[0])
    z = int(out.split("zeuge ")[1], 16)
    x = [(-1 if (z >> j) & 1 else 1) for j in range(len(M[0]))]
    chk = max(abs(sum(u * v for u, v in zip(r, x))) for r in M)
    assert chk == d, (chk, d)
    return d


def lies_liste(fn, streich):
    out = []
    for l in open(fn):
        t = l.split()
        if not t:
            continue
        if streich:
            out.append((int(t[0], 16), (int(t[1]), int(t[2])), int(t[3])))
        else:
            out.append((int(t[0], 16), (), int(t[1])))
    return out


def exakt(grp, liste, rand, streich, kmax, tmp):
    n, tab, inv = lies_tafel(grp)
    erg = []
    for f, ohne, u in lies_liste(liste, streich)[:kmax]:
        dd = exakt_matrix(matrix(f, n, tab, inv, rand, ohne), tmp)
        erg.append((f, ohne, u, dd))
    return erg


def main():
    cmd = sys.argv[1]
    if cmd == "zaehle":
        zaehle(sys.argv[2:])
    elif cmd == "zaehle_zyklisch":
        return zaehle_zyklisch()
    elif cmd == "matrix":
        f, grp, fn = int(sys.argv[2], 16), sys.argv[3], sys.argv[-1]
        ohne = ()
        if "--ohne" in sys.argv:
            ohne = tuple(int(t) for t in sys.argv[sys.argv.index("--ohne") + 1].split(","))
        n, tab, inv = lies_tafel(grp)
        schreibe(matrix(f, n, tab, inv, "-r" in sys.argv, ohne), fn)
    elif cmd == "exakt":
        kmax = int(sys.argv[sys.argv.index("-k") + 1]) if "-k" in sys.argv else 10 ** 9
        for f, ohne, u, dd in exakt(sys.argv[2], sys.argv[3], "-r" in sys.argv, "--streich" in sys.argv, kmax, TMPDATEI):
            print(f"{f:x} {ohne} u={u} exakt={dd}")
    elif cmd == "ugegenexakt":
        erg = exakt(sys.argv[2], sys.argv[3], "-r" in sys.argv, False, 10 ** 9, TMPDATEI)
        zu_klein = [(f, u, dd) for f, ohne, u, dd in erg if u < dd]
        zu_gross = sum(1 for f, ohne, u, dd in erg if u > dd)
        mx = max(dd for _, _, _, dd in erg)
        print(f"klassen {len(erg)}, u < exakt (VERLETZT): {len(zu_klein)}, u > exakt: {zu_gross}, exakt max {mx}, "
              f"u max {max(u for _, _, u, _ in erg)}")
        return 1 if zu_klein else 0
    return 0


if __name__ == "__main__":
    sys.exit(main())
