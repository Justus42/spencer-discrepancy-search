#!/usr/bin/env python3
"""
pruef_kozyk.py -- Pruefcode (Auftrag P, Teil 3). Nur ganze Zahlen. Keine Gemeinsamkeit mit src/pruef_ideen/kozyk.c.

Baut die Gruppen selbst, und zwar als Gruppen monomialer 2x2-Matrizen ueber Einheitswurzeln (Exponenten ganzzahlig; Matrixprodukt),
also auf anderem Weg als die Index-Formeln des Suchprogramms. Die Elementnummerierung ist dokumentiert (kozyk.c, Kopf):
  Direktprodukt F0 x F1 x F2, Index = f0 + |F0|*(f1 + |F1|*f2);
  CYC(m): a; DIC(m): i + 2m*j fuer a^i b^j (a = diag(z, z^-1), z primitiv 2m-te Einheitswurzel, b = [[0,1],[-1,0]]);
  DIH(m): i + m*j fuer r^i s^j (r = diag(w, w^-1), w primitiv m-te Einheitswurzel, s = [[0,1],[1,0]]).
Zentrale Involution z: je Gruppe vorgegeben, wird auf "zentral, Ordnung 2" geprueft; Gruppengesetz (Assoziativitaet, Inverse) wird geprueft.
Vertreter der Nebenklassen von <z>: der kleinere Index von {g, zg}. Funktionsnummer F (n-1 Bit): Bit k-1 = 1 heisst f(Vertreter k) = -1, f(e) = +1.
M[g][h] = f(g^-1 h), f(zg) = -f(g).

Aufruf:
  pruef_kozyk.py GRUPPE LCEIL PREFIX NSHARD [--exakt UEBERLEBENDEN-PREFIX]
     liest PREFIX.shard{0..NSHARD-1}.bin (3 Bytes je Funktion), rechnet jede Funktion nach: max_i |<M_i,x>| <= LCEIL.
  pruef_kozyk.py GRUPPE --struktur            Gruppeninvarianten ausgeben
"""
import sys, os, subprocess, gzip
import numpy as np

# ---------- Gruppen als monomiale Matrizen ----------
# Element einer Komponentengruppe: (N, ((j0, e0), (j1, e1), ...)): Zeile i hat den einzigen Eintrag zeta_N^{e_i} in Spalte j_i.

def mmul(A, B):
    N, ra = A
    N2, rb = B
    assert N == N2
    out = []
    for (j, e) in ra:
        k, f = rb[j]
        out.append((k, (e + f) % N))
    return (N, tuple(out))

def komponente(typ, m):
    """Liefert Liste der Elemente in der dokumentierten Nummerierung (als Matrizen)."""
    if typ == "CYC":
        N = m
        return [(N, ((0, a % N),)) for a in range(m)]
    if typ == "DIC":
        N = 2 * m
        def a_pow(i): return (N, ((0, i % N), (1, (-i) % N)))
        b = (N, ((1, 0), (0, m)))           # [[0,1],[-1,0]], -1 = zeta^m
        e = (N, ((0, 0), (1, 0)))
        els = []
        for j in range(2):
            for i in range(2 * m):
                els.append(mmul(a_pow(i), b if j else e))
        return els
    if typ == "DIH":
        N = m
        def r_pow(i): return (N, ((0, i % N), (1, (-i) % N)))
        s = (N, ((1, 0), (0, 0)))
        e = (N, ((0, 0), (1, 0)))
        els = []
        for j in range(2):
            for i in range(m):
                els.append(mmul(r_pow(i), s if j else e))
        return els
    raise ValueError(typ)

SPEC = {
    "Z16": ([("CYC", 16)], (8,)),
    "Z20": ([("CYC", 20)], (10,)),
    "Z3xQ8": ([("CYC", 3), ("DIC", 2)], (0, 2)),
    "Z36": ([("CYC", 36)], (18,)),
    "Dic9": ([("DIC", 9)], (9,)),
    "Z3xDic3": ([("CYC", 3), ("DIC", 3)], (0, 3)),
    "Z40": ([("CYC", 40)], (20,)),
    "Z5xQ8": ([("CYC", 5), ("DIC", 2)], (0, 2)),
    "Z5xD4": ([("CYC", 5), ("DIH", 4)], (0, 2)),
    "Z5xZ4xZ2": ([("CYC", 5), ("CYC", 4), ("CYC", 2)], (0, 2, 0)),
    "Dic10": ([("DIC", 10)], (10,)),
    "D20": ([("DIH", 20)], (10,)),
    "Z52": ([("CYC", 52)], (26,)),
    "Dic13": ([("DIC", 13)], (13,)),
    "Z56": ([("CYC", 56)], (28,)),
    "Z7xQ8": ([("CYC", 7), ("DIC", 2)], (0, 2)),
    "Z7xD4": ([("CYC", 7), ("DIH", 4)], (0, 2)),
    "Z7xZ4xZ2": ([("CYC", 7), ("CYC", 4), ("CYC", 2)], (0, 2, 0)),
    "Dic14": ([("DIC", 14)], (14,)),
    "D28": ([("DIH", 28)], (14,)),
    "Z60": ([("CYC", 60)], (30,)),
    "Dic15": ([("DIC", 15)], (15,)),
}


def gruppe(name):
    faktoren, zc = SPEC[name]
    komps = [komponente(t, m) for (t, m) in faktoren]
    groessen = [len(k) for k in komps]
    ordnung = 1
    for g in groessen:
        ordnung *= g
    lookup = [{el: i for i, el in enumerate(k)} for k in komps]
    # Tabelle der Komponenten
    ktab = []
    for ki, k in enumerate(komps):
        tab = [[None] * len(k) for _ in k]
        for a, A in enumerate(k):
            for b, B in enumerate(k):
                tab[a][b] = lookup[ki][mmul(A, B)]
        ktab.append(tab)

    def zerlege(x):
        c = []
        for g in groessen:
            c.append(x % g)
            x //= g
        return c

    def baue(c):
        x, f = 0, 1
        for ci, g in zip(c, groessen):
            x += ci * f
            f *= g
        return x

    tab = [[0] * ordnung for _ in range(ordnung)]
    for a in range(ordnung):
        ca = zerlege(a)
        for b in range(ordnung):
            cb = zerlege(b)
            tab[a][b] = baue([ktab[i][ca[i]][cb[i]] for i in range(len(groessen))])
    z = baue(list(zc))
    return ordnung, tab, z


def pruefe_gruppe(ordnung, tab, z):
    # neutrales Element 0
    assert all(tab[0][a] == a and tab[a][0] == a for a in range(ordnung)), "Index 0 nicht neutral"
    inv = [None] * ordnung
    for a in range(ordnung):
        for b in range(ordnung):
            if tab[a][b] == 0:
                inv[a] = b
        assert inv[a] is not None and tab[inv[a]][a] == 0
    for a in range(ordnung):
        for b in range(ordnung):
            ab = tab[a][b]
            for c in range(ordnung):
                assert tab[ab][c] == tab[a][tab[b][c]], "nicht assoziativ"
    assert z != 0 and tab[z][z] == 0, "z keine Involution"
    assert all(tab[z][a] == tab[a][z] for a in range(ordnung)), "z nicht zentral"
    return inv


def struktur(name):
    ordnung, tab, z = gruppe(name)
    inv = pruefe_gruppe(ordnung, tab, z)
    ordn = {}
    for a in range(ordnung):
        k, x = 1, a
        while x != 0:
            x = tab[x][a]
            k += 1
        ordn[k] = ordn.get(k, 0) + 1
    zentrum = sum(1 for a in range(ordnung) if all(tab[a][b] == tab[b][a] for b in range(ordnung)))
    abelsch = zentrum == ordnung
    return ordnung, z, inv, ordn, zentrum, abelsch


def tabellen(name):
    ordnung, tab, z = gruppe(name)
    inv = pruefe_gruppe(ordnung, tab, z)
    n = ordnung // 2
    rep = [g for g in range(ordnung) if g < tab[z][g]]
    rang = {}
    for k, g in enumerate(rep):
        rang[g] = k
    assert len(rep) == n and rep[0] == 0
    IDX = np.zeros((n, n), dtype=np.int64)
    SGN = np.zeros((n, n), dtype=np.int8)
    for a in range(n):
        for b in range(n):
            e = tab[inv[rep[a]]][rep[b]]
            if e in rang:
                IDX[a, b], SGN[a, b] = rang[e], 1
            else:
                IDX[a, b], SGN[a, b] = rang[tab[z][e]], -1
    return n, IDX, SGN


def matrix_von(F, n, IDX, SGN):
    f = np.ones(n, dtype=np.int64)
    for k in range(1, n):
        if (F >> (k - 1)) & 1:
            f[k] = -1
    return (SGN.astype(np.int64) * f[IDX])


def pruefe_gegenbeispiele(name, L, prefix, nshard):
    n, IDX, SGN = tabellen(name)
    N = 1 << (n - 1)
    daten = b""
    for s in range(nshard):
        fn = "%s.shard%d.bin" % (prefix, s)
        if not os.path.exists(fn):
            fn += ".gz"
        op = gzip.open if fn.endswith(".gz") else open
        with op(fn, "rb") as f:
            daten += f.read()
    if len(daten) != 3 * N:
        print("FEHLER: %d Bytes statt %d" % (len(daten), 3 * N))
        return False
    xs = np.frombuffer(daten, dtype=np.uint8).reshape(N, 3).astype(np.int64)
    xs = xs[:, 0] | (xs[:, 1] << 8) | (xs[:, 2] << 16)
    fehlend = int(np.sum(xs >= (1 << n)))
    schlecht = 0
    maxima = np.zeros(2 * n + 2, dtype=np.int64)
    CH = 1 << 14
    bitsF = np.arange(n - 1)
    hbit = np.arange(n)
    for lo in range(0, N, CH):
        hi = min(N, lo + CH)
        Fs = np.arange(lo, hi, dtype=np.int64)
        fv = np.ones((hi - lo, n), dtype=np.int8)
        fv[:, 1:] = 1 - 2 * ((Fs[:, None] >> bitsF[None, :]) & 1)
        E = SGN[None, :, :] * fv[:, IDX]                     # (chunk, g, h)
        x = 1 - 2 * ((xs[lo:hi, None] >> hbit[None, :]) & 1)  # (chunk, h), Bit 1 = -1
        r = np.einsum("fgh,fh->fg", E.astype(np.int32), x.astype(np.int32))
        mx = np.abs(r).max(axis=1)
        ok = (mx <= L) & (xs[lo:hi] < (1 << n))
        schlecht += int(np.sum(~ok))
        for v in mx:
            maxima[v] += 1
    print("%s: n = %d, Funktionen %d, Gegenbeispiele nachgerechnet: Fehlstellen %d, nicht bestanden %d (max <= %d verlangt)" % (name, n, N, fehlend, schlecht, L))
    print("  Verteilung des erreichten Maximums der gespeicherten x:", {i: int(c) for i, c in enumerate(maxima) if c})
    return schlecht == 0 and fehlend == 0


def exakt_nachrechnen(name, ueb_prefix, nshard, tmpdir, stichprobe_rest=300, ziel=None):
    """Alle Ueberlebenden mit eigener Matrix durch build/exakt_disc schicken (exakte Diskrepanz), dazu eine Stichprobe der
    Nicht-Ueberlebenden mit -g (muss Maximum < ziel liefern)."""
    n, IDX, SGN = tabellen(name)
    N = 1 << (n - 1)
    uebs = []
    for s in range(nshard):
        for line in open("%s.shard%d.ueberlebende.txt" % (ueb_prefix, s)):
            if line.startswith("O"):
                continue
            uebs.append(int(line))
    uebs = sorted(set(uebs))
    tmp = os.path.join(tmpdir, "pruef_kozyk_%s.mat" % name)
    hist = {}
    had = 0
    for F in uebs:
        M = matrix_von(F, n, IDX, SGN)
        with open(tmp, "w") as f:
            for row in M:
                f.write(" ".join(str(int(v)) for v in row) + "\n")
        out = subprocess.run(["build/exakt_disc", tmp], capture_output=True, text=True).stdout
        d = int(out.split("disc")[1].split(",")[0])
        hist[d] = hist.get(d, 0) + 1
        G = M @ M.T
        if np.array_equal(G, n * np.eye(n, dtype=np.int64)):
            had += 1
    print("%s: %d Ueberlebende (eigene Matrizen), exakte Diskrepanz laut build/exakt_disc: %s; Hadamard-Matrizen (M M^T = nI): %d" % (name, len(uebs), hist, had))
    # Stichprobe Nicht-Ueberlebende
    rng = np.random.default_rng(12345)
    ueb_set = set(uebs)
    n_ok = n_bad = 0
    gesehen = 0
    while gesehen < stichprobe_rest:
        F = int(rng.integers(0, N))
        if F in ueb_set:
            continue
        gesehen += 1
        M = matrix_von(F, n, IDX, SGN)
        with open(tmp, "w") as f:
            for row in M:
                f.write(" ".join(str(int(v)) for v in row) + "\n")
        out = subprocess.run(["build/exakt_disc", tmp, "-g", str(ziel)], capture_output=True, text=True).stdout
        if "gefunden" in out:
            n_ok += 1
        else:
            n_bad += 1
    print("  Stichprobe %d Nicht-Ueberlebende: mit x unter dem Ziel %d gefunden (disc < %d): %d, nicht: %d" % (stichprobe_rest, ziel, ziel, n_ok, n_bad))
    return hist, had, n_bad == 0


def vergleiche_zeilen(name, anzahl):
    """Zeilen, die das Suchprogramm fuer zufaellige Funktionsnummern F baut (allgemeiner Weg und Maskenweg des Siebs),
    mit der eigenen Matrix aus der eigenen Gruppe vergleichen."""
    n, IDX, SGN = tabellen(name)
    N = 1 << (n - 1)
    rng = np.random.default_rng(2026)
    Fs = [0, 1, N - 1, N // 2] + [int(v) for v in rng.integers(0, N, size=anzahl)]
    eingabe = "\n".join(str(F) for F in Fs) + "\n"
    out = subprocess.run(["build/kozyk", name, "ZEILEN", "0", "0", "0", "0", "0"], input=eingabe, capture_output=True, text=True).stdout
    zeilen = out.strip().split("\n")
    gut = schlecht = maskab = 0
    for F, line in zip(Fs, zeilen):
        kopf, rest = line.split("|")
        t = kopf.split()
        assert int(t[0]) == F
        rows_c = [int(v) for v in t[1:]]
        M = matrix_von(F, n, IDX, SGN)
        rows_p = [sum(1 << h for h in range(n) if M[g, h] < 0) for g in range(n)]
        if rows_c == rows_p:
            gut += 1
        else:
            schlecht += 1
        if "mask=ok" not in rest:
            maskab += 1
    print("%s: Zeilenvergleich Suchprogramm gegen eigene Gruppe: %d von %d Funktionen gleich, abweichend %d, Maskenweg abweichend %d" % (name, gut, len(Fs), schlecht, maskab))
    return schlecht == 0 and maskab == 0 and gut == len(Fs)


def main():
    name = sys.argv[1]
    if len(sys.argv) > 2 and sys.argv[2] == "--zeilen":
        ok = vergleiche_zeilen(name, int(sys.argv[3]))
        sys.exit(0 if ok else 1)
    if len(sys.argv) > 2 and sys.argv[2] == "--struktur":
        ordnung, z, inv, ordn, zentrum, ab = struktur(name)
        print("%s: Ordnung %d, z = %d, Elementordnungen %s, Zentrum %d, abelsch %s" % (name, ordnung, z, dict(sorted(ordn.items())), zentrum, ab))
        return
    L = int(sys.argv[2])
    prefix = sys.argv[3]
    nshard = int(sys.argv[4])
    ok = pruefe_gegenbeispiele(name, L, prefix, nshard)
    if "--exakt" in sys.argv:
        i = sys.argv.index("--exakt")
        tmpdir = sys.argv[i + 2]
        ziel = int(sys.argv[i + 3])
        exakt_nachrechnen(name, sys.argv[i + 1], nshard, tmpdir, ziel=ziel)
    print("ERGEBNIS", name, "bestaetigt" if ok else "ABWEICHUNG")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
