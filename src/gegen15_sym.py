#!/usr/bin/env python3
"""Pruefcode (kein Suchcode): Gegenrechnung der Orchestrator-Zaehlungen zu (15,4), 25.11.2026.

Gegenstand: selbstkomplementaere Codes der Laenge 15, Radius 4 (Zeile a trifft x, wenn |<a,x>| >= 7,
gleichbedeutend: Hamming-Abstand von x zu a hoechstens 4 oder zu -a hoechstens 4), invariant unter einer
Spaltenpermutation sigma von Primzahlordnung p. Bit 1 = Eintrag -1, Bit i = Spalte i.
Gruppe G = <sigma, Komplement>, Ordnung 2p. Eigener Code, nur ganze Zahlen (numpy-Matrixprodukt in float32
rechnet exakt, alle Summen < 2^24; die Ergebnisse werden unten ausserdem unabhaengig mit ganzzahligen Skalarprodukten
auf allen 2^15 Punkten nachgerechnet).

Aufruf:  python3 src/gegen15_sym.py {teil1|teil2|teil3|teil4|aequiv|kontrolle} [AUSGABEORDNER]
"""
from __future__ import annotations

import itertools
import sys
import time
from pathlib import Path

import numpy as np

N = 15
R = 4
VOLL = (1 << N) - 1
WURZEL = Path(__file__).resolve().parent.parent

PC = np.array([bin(x).count("1") for x in range(1 << N)], dtype=np.uint8)
KUGEL = np.array([x for x in range(1 << N) if PC[x] <= R], dtype=np.int64)
assert len(KUGEL) == 1941  # 1 + 15 + 105 + 455 + 1365

ALLE = np.arange(1 << N, dtype=np.int64)
# Vorzeichenmatrix aller Punkte: X[j, x] = +1 (Bit 0) oder -1 (Bit 1), Spalte j
X = (1 - 2 * ((ALLE[None, :] >> np.arange(N)[:, None]) & 1)).astype(np.int16)


def zeit():
    return time.strftime("%H:%M:%S")


# ----------------------------------------------------------------------------------------- Struktur
class Struktur:
    """Punkt- und Wortklassen unter G = <sigma, Komplement> fuer Zyklentyp typ (Summe 15), Ordnung p."""

    def __init__(self, typ: list[int], p: int):
        assert sum(typ) == N
        self.typ, self.p = typ, p
        ziel = np.empty(N, dtype=np.int64)
        pos = 0
        for L in typ:
            for k in range(L):
                ziel[pos + k] = pos + (k + 1) % L
            pos += L
        neu = np.zeros_like(ALLE)
        for i in range(N):
            neu |= ((ALLE >> i) & 1) << int(ziel[i])
        self.tab = neu  # Wort -> sigma(Wort)
        cur = ALLE.copy()
        bilder = []
        for k in range(p):
            if k > 0:
                assert not (cur == ALLE).all(), "sigma hat kleinere Ordnung als p"
            bilder.append(cur)
            bilder.append(cur ^ VOLL)
            cur = self.tab[cur]
        assert (cur == ALLE).all(), "sigma^p ist nicht die Identitaet"
        kanon = np.minimum.reduce(bilder)
        self.rep, self.kl = np.unique(kanon, return_inverse=True)  # rep = kleinstes Wort der Klasse
        self.K = len(self.rep)
        self.gr = np.bincount(self.kl, minlength=self.K)
        self.voll = np.where(self.gr == 2 * p)[0]
        self.fest = np.where(self.gr == 2)[0]
        assert len(self.voll) + len(self.fest) == self.K, "Klassengroessen nicht 2p oder 2"
        self._deck = {}

    def deckt(self, kl_nr: int) -> np.ndarray:
        """Boolesche Zeile (Laenge K): von Klasse kl_nr als Zeilenbahn ueberdeckte Punktklassen."""
        w = int(self.rep[kl_nr])
        z = np.zeros(self.K, dtype=bool)
        z[self.kl[w ^ KUGEL]] = True
        return z

    def deckmatrix(self, idx: np.ndarray) -> np.ndarray:
        return np.array([self.deckt(int(c)) for c in idx])

    def zeilen(self, kl_nr: int) -> list[int]:
        """Zeilenvertreter (je Komplementpaar ein Wort): voll p Zeilen sigma^k(w), fest 1 Zeile."""
        w = int(self.rep[kl_nr])
        if self.gr[kl_nr] == 2:
            return [w]
        out, cur = [], w
        for _ in range(self.p):
            out.append(cur)
            cur = int(self.tab[cur])
        return out


def wort_zu_zeile(w: int) -> list[int]:
    return [-1 if (w >> j) & 1 else 1 for j in range(N)]


def matrix_aus_klassen(S: Struktur, klassen: list[int]) -> list[list[int]]:
    zeilen = []
    for c in klassen:
        zeilen += S.zeilen(c)
    return [wort_zu_zeile(w) for w in zeilen]


def offen_matrix(M: list[list[int]]) -> int:
    """Zahl der Punkte x in {-1,1}^15 mit max_i |<a_i,x>| < 7 (direkt, ganzzahlig, ohne Klassen und Kugeln)."""
    A = np.array(M, dtype=np.int16)
    skal = np.abs(A @ X)  # Zeilen x Punkte
    return int((skal.max(axis=0) < 7).sum())


# ----------------------------------------------------------------------------------------- Zaehlung
def bericht_struktur(S: Struktur):
    print(f"Zyklentyp {S.typ}, p = {S.p}: Punktklassen {S.K}; volle Zeilenbahnen {len(S.voll)}; "
          f"feste Zeilen (mod Vorzeichen) {len(S.fest)}; Klassengroessen {sorted(set(S.gr.tolist()))}", flush=True)
    # Burnside-Gegenprobe
    fix = 2 ** (len(S.typ))
    soll = (2 ** N + (S.p - 1) * fix) // (2 * S.p)
    print(f"  Burnside: (2^15 + {S.p - 1}*{fix}) / {2 * S.p} = {soll}", flush=True)
    assert soll == S.K


def teilmengen(n: int, kmax: int):
    out = []
    for k in range(kmax + 1):
        out += [c for c in itertools.combinations(range(n), k)]
    return out


def gewichtsmatrix(S: Struktur, Cx: np.ndarray, subs: list[tuple]) -> np.ndarray:
    """K x len(subs), float32: Gewicht (Punktzahl) der Klasse, wenn sie von den festen Zeilen der Teilmenge
    NICHT ueberdeckt ist, sonst 0."""
    W = np.zeros((S.K, len(subs)), dtype=np.float32)
    for c, sub in enumerate(subs):
        cov = np.zeros(S.K, dtype=bool)
        for f in sub:
            cov |= Cx[f]
        W[:, c] = np.where(cov, 0, S.gr).astype(np.float32)
    return W


def zwei_volle(S: Struktur, kmax_fest: int, log=print):
    """Alle Paare voller Bahnen i<j, dazu jede Teilmenge (bis Groesse kmax_fest) der festen Zeilen.
    Rueckgabe: subs, minimum je Teilmenge, Anzahl der Paare am Minimum, Liste der Loesungen (i,j,subnr)."""
    Cf = S.deckmatrix(S.voll).astype(np.float32)
    Cx = S.deckmatrix(S.fest)
    subs = teilmengen(len(S.fest), kmax_fest)
    W = gewichtsmatrix(S, Cx, subs)
    n = len(S.voll)
    mini = np.full(len(subs), 10 ** 9, dtype=np.int64)
    anz = np.zeros(len(subs), dtype=np.int64)
    arg = [None] * len(subs)
    loes = []
    t0 = time.time()
    for i in range(n - 1):
        unc = 1.0 - np.maximum(Cf[i], Cf[i + 1:])
        res = unc @ W  # (n-i-1) x Teilmengen, exakte ganze Zahlen in float32
        res_i = res.astype(np.int64)
        for c in range(len(subs)):
            col = res_i[:, c]
            m = int(col.min())
            if m < mini[c]:
                mini[c], anz[c] = m, 0
                arg[c] = (i, i + 1 + int(col.argmin()))
            if m == mini[c]:
                anz[c] += int((col == m).sum())
            if m == 0:
                for j in np.where(col == 0)[0]:
                    loes.append((i, i + 1 + int(j), c))
        if i % 500 == 0:
            log(f"  i = {i}/{n}  ({time.time() - t0:.0f} s)", flush=True) if callable(log) else None
    return subs, mini, anz, arg, loes


def ein_voll(S: Struktur, ks: list[int]):
    """Eine volle Bahn plus Teilmenge der festen Zeilen der Groesse k, fuer k in ks."""
    Cf = S.deckmatrix(S.voll).astype(np.float32)
    Cx = S.deckmatrix(S.fest)
    out = {}
    U = 1.0 - Cf
    for k in ks:
        subs = [c for c in itertools.combinations(range(len(S.fest)), k)]
        W = gewichtsmatrix(S, Cx, subs)
        res = (U @ W).astype(np.int64)  # voll x Teilmengen
        m = int(res.min())
        nm = int((res == m).sum())
        nul = np.argwhere(res == 0)
        out[k] = dict(subs=subs, min=m, anz_min=nm, nullen=[(int(a), subs[int(b)]) for a, b in nul],
                      arg=[(int(a), subs[int(b)]) for a, b in np.argwhere(res == m)[:3]])
    return out


# ----------------------------------------------------------------------------------------- Teile
def teil1(ordner: Path):
    S = Struktur([7, 7, 1], 7)
    bericht_struktur(S)
    print(f"[{zeit()}] Zeilenbahnen: voll {len(S.voll)}, fest {len(S.fest)} (Erwartung 2340, 4); Punktklassen {S.K} (2344)")
    print("Feste Zeilen (Wort, Bits 0-6 | 7-13 | 14):",
          [format(int(S.rep[c]), "015b")[::-1] for c in S.fest])
    t0 = time.time()
    subs, mini, anz, arg, loes = zwei_volle(S, 4)
    print(f"[{zeit()}] Paare voller Bahnen: {len(S.voll) * (len(S.voll) - 1) // 2}, Rechenzeit {time.time() - t0:.0f} s")
    print("\nTeilmenge fester Zeilen (Indizes 0..3) | Woerter | Ueberdeckungen (2 volle + Teilmenge) | kleinste Restzahl offener Punkte | Paare am Minimum")
    for c, sub in enumerate(subs):
        n_l = sum(1 for (_, _, cc) in loes if cc == c)
        print(f"  {str(sub):18s} {28 + 2 * len(sub):3d} Woerter  {n_l:5d} Ueberdeckungen   Minimum {mini[c]:6d}   Paare {anz[c]}")
    # Zusammenfassung nach Groesse
    for k in range(5):
        tot = sum(1 for (_, _, cc) in loes if len(subs[cc]) == k)
        print(f"Zusammen {k} feste Zeilen ({28 + 2 * k} Woerter): {tot} Ueberdeckungen")
    # Loesungen schreiben (nur Groesse 2)
    ordner.mkdir(parents=True, exist_ok=True)
    pfad = ordner / "loesungen_2voll_2fest.txt"
    with pfad.open("w") as f:
        f.write("# i j (Indizes der Klassenvertreter in S.voll) a b (Indizes der festen Zeilen); Vertreter = kleinste Woerter der Klasse\n")
        for (i, j, c) in loes:
            if len(subs[c]) == 2:
                f.write(f"{int(S.rep[S.voll[i]])} {int(S.rep[S.voll[j]])} {int(S.rep[S.fest[subs[c][0]]])} {int(S.rep[S.fest[subs[c][1]]])}\n")
    print(f"Loesungen nach {pfad.relative_to(WURZEL)}")
    # direkte Nachrechnung: jede Loesung, ohne Klassen
    print(f"[{zeit()}] Direkte Nachrechnung (Skalarprodukte auf allen 2^15 Punkten) aller Loesungen mit 2 festen Zeilen ...")
    bad = 0
    nl = 0
    for (i, j, c) in loes:
        if len(subs[c]) != 2:
            continue
        nl += 1
        M = matrix_aus_klassen(S, [int(S.voll[i]), int(S.voll[j])] + [int(S.fest[f]) for f in subs[c]])
        if offen_matrix(M) != 0:
            bad += 1
    print(f"  {nl} Loesungen direkt geprueft, offene Punkte != 0 in {bad} Faellen")
    # direkte Nachrechnung der Minima (30 Woerter, 1 feste Zeile)
    print("Direkte Nachrechnung eines Paares am Minimum je Teilmenge der Groesse 0 und 1:")
    for c, sub in enumerate(subs):
        if len(sub) <= 1:
            i, j = arg[c]
            M = matrix_aus_klassen(S, [int(S.voll[i]), int(S.voll[j])] + [int(S.fest[f]) for f in sub])
            print(f"  {str(sub):8s} Klassenmethode {mini[c]}, direkt {offen_matrix(M)}")
    return S, subs, loes


def teil23(p: int, typ: list[int], ks: list[int]):
    S = Struktur(typ, p)
    bericht_struktur(S)
    t0 = time.time()
    erg = ein_voll(S, ks)
    print(f"[{zeit()}] Rechenzeit {time.time() - t0:.0f} s")
    for k in ks:
        e = erg[k]
        wort = 2 * p + 2 * k
        print(f"  eine volle Bahn + {k} feste Zeilen ({wort} Woerter): Teilmengen {len(e['subs'])}, "
              f"Ueberdeckungen {len(e['nullen'])}; kleinste Restzahl offener Punkte {e['min']} "
              f"({e['anz_min']} Kombinationen)")
        # direkte Nachrechnung eines Minimums
        a, sub = e["arg"][0]
        M = matrix_aus_klassen(S, [int(S.voll[a])] + [int(S.fest[f]) for f in sub])
        print(f"    direkte Nachrechnung eines Minimums: {offen_matrix(M)} offene Punkte (Klassenmethode {e['min']})")
    return S, erg


def kontrolle_stichprobe(p: int, typ: list[int], k: int, anzahl: int = 60):
    """Klassenmethode gegen direkte Rechnung, zufaellige (volle Bahn, Teilmenge) bei p."""
    S = Struktur(typ, p)
    rng = np.random.default_rng(12345 + p)
    Cf = S.deckmatrix(S.voll)
    Cx = S.deckmatrix(S.fest)
    bad = 0
    for _ in range(anzahl):
        a = int(rng.integers(len(S.voll)))
        sub = tuple(sorted(rng.choice(len(S.fest), size=k, replace=False).tolist()))
        cov = Cf[a].copy()
        for f in sub:
            cov |= Cx[f]
        klasse = int(S.gr[~cov].sum())
        M = matrix_aus_klassen(S, [int(S.voll[a])] + [int(S.fest[f]) for f in sub])
        direkt = offen_matrix(M)
        if klasse != direkt:
            bad += 1
    print(f"  Stichprobe p = {p}, {anzahl} zufaellige (1 volle Bahn + {k} feste): Klassenmethode = direkte Rechnung in {anzahl - bad}, Abweichungen {bad}")
    return bad


def kontrolle_zwei_volle():
    """Positivkontrolle der Klassenmaschinerie bei p = 13 und 11: zwei volle Bahnen (52 bzw. 44 Woerter) ohne feste Zeilen;
    gefundene Ueberdeckungen werden direkt (Skalarprodukte) nachgerechnet."""
    for p, typ in ((13, [13, 1, 1]), (11, [11, 1, 1, 1, 1])):
        S = Struktur(typ, p)
        subs, mini, anz, arg, loes = zwei_volle(S, 0, log=lambda *a, **k: None)
        print(f"p = {p}: zwei volle Bahnen ({4 * p} Woerter): Ueberdeckungen {len(loes)}; kleinste Restzahl {mini[0]} ({anz[0]} Paare)")
        for (i, j, c) in loes[:20]:
            M = matrix_aus_klassen(S, [int(S.voll[i]), int(S.voll[j])])
            assert offen_matrix(M) == 0, "direkte Nachrechnung widerspricht"
        print(f"  die ersten {min(20, len(loes))} direkt nachgerechnet: 0 offene Punkte")
        if not loes:
            i, j = arg[0]
            M = matrix_aus_klassen(S, [int(S.voll[i]), int(S.voll[j])])
            print(f"  Minimum direkt: {offen_matrix(M)}")


def teil4():
    ctrl = [list(map(int, z.split())) for z in (WURZEL / "controls" / "ctrl_16.txt").read_text().splitlines() if z.strip()]
    assert len(ctrl) == 16 and all(len(z) == 16 for z in ctrl)
    assert all(z[15] == 0 for z in ctrl), "letzte Spalte keine Nullspalte"
    M = [z[:15] for z in ctrl]
    assert all(abs(v) == 1 for z in M for v in z)
    A = np.array(M, dtype=np.int16)
    skal = np.abs(A @ X)
    treffer = skal >= 7  # 16 x 32768
    print(f"ctrl_16 ohne Nullspalte: {len(M)} x {len(M[0])}; offene Punkte mit allen 16 Zeilen: {int((~treffer.any(axis=0)).sum())} (Erwartung 0)")
    erg = []
    for i in range(16):
        rest = np.delete(treffer, i, axis=0)
        offen = int((~rest.any(axis=0)).sum())
        nur = int((treffer[i] & (treffer.sum(axis=0) == 1)).sum())
        erg.append((i, offen, nur))
        print(f"  Zeile {i:2d} gestrichen: offene Punkte {offen}   (Punkte, die nur diese Zeile trifft: {nur})")
    print("alle 466:", all(o == 466 for _, o, _ in erg))
    # Zusatz: Mehrfachueberdeckung
    zaehl = treffer.sum(axis=0)
    print("Verteilung der Zahl treffender Zeilen je Punkt (0..16):", np.bincount(zaehl, minlength=17).tolist())
    return erg


def lies_loesungen(S: Struktur, pfad: Path):
    out = []
    for z in pfad.read_text().splitlines():
        if z.startswith("#") or not z.strip():
            continue
        a, b, c, d = map(int, z.split())
        kn = {int(w): k for k, w in enumerate(S.rep)}
        out.append([kn[a], kn[b], kn[c], kn[d]])
    return out


# ----------------------------------------------------------------------------------------- Aequivalenz
def normiere(M: list[list[int]]):
    """Zeilenvorzeichen s_i (s_0 = +1) so, dass alle Skalarprodukte zweier Zeilen -1 sind; None, wenn unmoeglich."""
    A = np.array(M, dtype=np.int64)
    G = A @ A.T
    s = np.ones(len(A), dtype=np.int64)
    s[1:] = -G[0, 1:]
    B = A * s[:, None]
    GB = B @ B.T
    if not ((GB + np.eye(len(A), dtype=np.int64) * (A.shape[1] + 1)) == -1 + np.eye(len(A), dtype=np.int64) * (A.shape[1] + 1) + 0).all():
        pass
    off = GB[~np.eye(len(A), dtype=bool)]
    return B if (off == -1).all() else None


def aequivalent(M: list[list[int]], Nn: list[list[int]]) -> bool:
    """Gleichwertigkeit zweier 16 x 15-Matrizen mit +-1 unter Zeilen-/Spaltenpermutation und Zeilen-/Spaltennegation.
    Beide muessen nach Zeilenvorzeichen paarweise Skalarprodukt -1 haben (Hadamard-16 ohne eine Spalte); sonst ValueError.
    Zeilenzuordnung durch Zurueckverfolgen mit der Erhaltung aller Vierzeilen-Summen Q(i,k,l,m) = sum_j a_ij a_kj a_lj a_mj
    (invariant unter Spaltenvorzeichen); am Ende Spalten als Mehrmengen bis auf Vorzeichen vergleichen."""
    A, B = normiere(M), normiere(Nn)
    if A is None or B is None:
        raise ValueError("Matrix nicht vom Typ Hadamard-minus-Spalte")
    r, c = A.shape
    QA = np.einsum("ia,ka,la,ma->iklm", A, A, A, A)
    QB = np.einsum("ia,ka,la,ma->iklm", B, B, B, B)
    if sorted(QA.ravel().tolist()) != sorted(QB.ravel().tolist()):
        return False

    def norm(v):
        v = tuple(int(t) for t in v)
        return v if v[0] > 0 else tuple(-t for t in v)

    zuordnung: list[int] = []
    belegt = [False] * r

    def spalten_ok() -> bool:
        P = B[zuordnung]
        return sorted(norm(A[:, j]) for j in range(c)) == sorted(norm(P[:, j]) for j in range(c))

    def rek(i: int) -> bool:
        if i == r:
            return spalten_ok()
        for z in range(r):
            if belegt[z]:
                continue
            if i >= 3:
                phi = zuordnung
                if not np.array_equal(QA[:i, :i, :i, i], QB[np.ix_(phi, phi, phi, [z])][..., 0]):
                    continue
            zuordnung.append(z)
            belegt[z] = True
            if rek(i + 1):
                return True
            zuordnung.pop()
            belegt[z] = False
        return False

    return rek(0)


def teil_aequiv(ordner: Path):
    S = Struktur([7, 7, 1], 7)
    loes = lies_loesungen(S, ordner / "loesungen_2voll_2fest.txt")
    ctrl = [list(map(int, z.split()))[:15] for z in (WURZEL / "controls" / "ctrl_16.txt").read_text().splitlines() if z.strip()]
    print(f"{len(loes)} Loesungen; Gleichwertigkeit (Zeilen/Spalten vertauschen und negieren) zu ctrl_16 ohne Nullspalte:")
    gl = []
    t0 = time.time()
    for n, kl in enumerate(loes):
        M = matrix_aus_klassen(S, kl)
        g = aequivalent(M, ctrl)
        gl.append(g)
    print(f"  gleichwertig zu ctrl_16: {sum(gl)} von {len(gl)}  ({time.time() - t0:.0f} s)")
    # Probe: Negativkontrolle, ctrl_16 gegen eine veraenderte Matrix und gegen sich selbst, mit zufaelliger Verwuerfelung
    rng = np.random.default_rng(7)
    A = np.array(ctrl)
    P = A[rng.permutation(16)][:, rng.permutation(15)] * rng.choice([-1, 1], size=16)[:, None] * rng.choice([-1, 1], size=15)[None, :]
    print("  Kontrolle verwuerfelte ctrl_16 gleichwertig:", aequivalent(P.tolist(), ctrl))
    Q = A.copy()
    Q[0, 0] = -Q[0, 0]
    print("  Kontrolle ctrl_16 mit einem umgedrehten Eintrag: Hadamard-Typ nach Normierung =", normiere(Q.tolist()) is not None, "(soll False)")
    # Sylvester-Hadamard 16 ohne eine Spalte (andere Klasse oder gleiche, je nach Ausgang) und ohne eine andere Spalte
    H = np.array([[(-1) ** bin(a & b).count("1") for b in range(16)] for a in range(16)])
    print("  Sylvester-16 ohne Spalte 0 gegen ohne Spalte 5 (gleichwertig, Automorphismus):", aequivalent(H[:, 1:].tolist(), np.delete(H, 5, axis=1).tolist()))
    print("  Sylvester-16 ohne Spalte 0 gegen ctrl_16:", aequivalent(H[:, 1:].tolist(), ctrl))
    # Mit sich selbst: Zahl der Klassen unter den Loesungen
    reps = []
    klassen = []
    for n, kl in enumerate(loes):
        M = matrix_aus_klassen(S, kl)
        for ri, Rm in enumerate(reps):
            if aequivalent(M, Rm):
                klassen[ri].append(n)
                break
        else:
            reps.append(M)
            klassen.append([n])
    print(f"  Aequivalenzklassen unter den {len(loes)} Loesungen: {len(reps)} (Groessen {[len(k) for k in klassen]})")
    return gl


def main():
    was = sys.argv[1] if len(sys.argv) > 1 else "teil1"
    ordner = Path(sys.argv[2]) if len(sys.argv) > 2 else WURZEL / "runs" / "gegenrechnung15"
    if was == "teil1":
        teil1(ordner)
    elif was == "teil2":
        teil23(13, [13, 1, 1], [2, 3, 4])
        kontrolle_stichprobe(13, [13, 1, 1], 2)
    elif was == "teil3":
        teil23(11, [11, 1, 1, 1, 1], [4, 5, 6])
        kontrolle_stichprobe(11, [11, 1, 1, 1, 1], 5)
    elif was == "teil4":
        teil4()
    elif was == "kontrolle":
        kontrolle_zwei_volle()
    elif was == "aequiv":
        teil_aequiv(ordner)
    else:
        sys.exit("unbekannter Teil")


if __name__ == "__main__":
    main()
