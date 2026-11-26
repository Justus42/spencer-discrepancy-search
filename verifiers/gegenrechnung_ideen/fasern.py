#!/usr/bin/env python3
"""Auftrag C (Sitzung, 26.11.2026): Faserbedingung fuer den Kern <1,a,b> (Ideennotiz Abschnitt 4, Idee 1). Pruefcode.

Eigene Umsetzung nach Sols Beschreibung (notes/openrouter/gegenpruefung_2026-11-26--openai-gpt-6.2-sol.md, Idee 1):
Typ = Blockgroessen (n_1..n_B), B = 3 oder 4, alle positiv, Summe m (Partitionen in 3 oder 4 Teile; die affine Gruppe
permutiert die vier Bloecke beliebig). Kern C0 = <1,a,b> kippt bei 4 Bloecken jede gerade Blockmenge, bei 3 Bloecken jede.
Quotientenpunkt x (Blockgewichte w_j) liegt in D (Abstand <= R vom Kern), wenn die maximale Korrelation >= d = m - 2R:
  Korrelation sum_j |n_j - 2 w_j|, bei 4 Bloecken mit ungerader Zahl negativer Terme und keinem Term 0 minus 2 * min|.|.
Fasern: Paritaetsvektor (w_j mod 2) modulo dem Bild des Kerns P0. M[t] = Zahl der Punkte von D in Klasse t.
Notwendig fuer K Nebenklassen: k_q >= 0 ganz, sum k_q = K, sum_q k_q M[p - q] >= |Faser| fuer jede Klasse p.
Ausgabe je Zelle: Zahl der Typen, ausgeschlossene Typen bei K, Spanne von |D| und 2^(m-3)/|D|.
Aufruf: python3 fasern.py [K]"""
import itertools
import sys
from math import comb


def typen(m):
    out = []
    for B in (3, 4):
        def rek(rest, k, mx, pre):
            if k == 0:
                if rest == 0:
                    out.append(tuple(pre))
                return
            for v in range(min(mx, rest - (k - 1)), 0, -1):
                rek(rest - v, k - 1, v, pre + [v])
        rek(m, B, m, [])
    return out


def flips(B):
    alle = list(itertools.product((0, 1), repeat=B))
    return alle if B == 3 else [f for f in alle if sum(f) % 2 == 0]


def maxkorr(n, w):
    c = [nj - 2 * wj for nj, wj in zip(n, w)]
    s = sum(abs(t) for t in c)
    if len(n) == 4 and sum(t < 0 for t in c) % 2 == 1 and all(t != 0 for t in c):
        s -= 2 * min(abs(t) for t in c)
    return s


def maxkorr_direkt(n, w):
    """Gegenprobe ueber alle Kippmengen."""
    best = None
    for f in flips(len(n)):
        s = sum((nj - 2 * wj) * (-1 if fj else 1) for nj, wj, fj in zip(n, w, f))
        best = s if best is None else max(best, s)
    return best


def pruefe_typ(n, R, K):
    B = len(n); m = sum(n); d = m - 2 * R
    P0 = {tuple((fj * nj) % 2 for fj, nj in zip(f, n)) for f in flips(B)}
    vecs = list(itertools.product((0, 1), repeat=B))
    kl = {}
    for v in vecs:
        if v in kl:
            continue
        idx = len(set(kl.values()))
        for p0 in P0:
            kl[tuple((a + b) % 2 for a, b in zip(v, p0))] = idx
    nk = len(set(kl.values()))
    rep = {}
    for v, i in kl.items():
        rep.setdefault(i, v)
    M = [0] * nk
    for w in itertools.product(*[range(nj + 1) for nj in n]):
        assert maxkorr(n, w) == maxkorr_direkt(n, w)
        if maxkorr(n, w) >= d:
            M[kl[tuple(t % 2 for t in w)]] += comb_prod(n, w)
    assert all(x % 8 == 0 for x in M)
    M = [x // 8 for x in M]
    D = sum(M)
    faser = 2 ** (m - 3) // nk
    assert faser * nk == 2 ** (m - 3)
    diff = [[kl[tuple((a + b) % 2 for a, b in zip(rep[p], rep[q]))] for q in range(nk)] for p in range(nk)]
    for ks in itertools.combinations_with_replacement(range(nk), K):
        cnt = [0] * nk
        for q in ks:
            cnt[q] += 1
        if all(sum(cnt[q] * M[diff[p][q]] for q in range(nk)) >= faser for p in range(nk)):
            return True, D, nk
    return False, D, nk


def comb_prod(n, w):
    r = 1
    for nj, wj in zip(n, w):
        r *= comb(nj, wj)
    return r


def main():
    K = int(sys.argv[1]) if len(sys.argv) > 1 else 5
    for m, R in ((16, 4), (18, 5), (20, 6)):
        T = typen(m)
        aus = []
        Ds = []
        for n in T:
            ok, D, nk = pruefe_typ(n, R, K)
            Ds.append(D)
            if not ok:
                aus.append(n)
        print(f"({m},{R}): {len(T)} Typen, bei K = {K} ausgeschlossen {len(aus)}: {aus}")
        print(f"   |D| zwischen {min(Ds)} und {max(Ds)}, Zaehlschranke 2^{m-3}/|D| zwischen {2**(m-3)/max(Ds):.2f} und {2**(m-3)/min(Ds):.2f}")


if __name__ == "__main__":
    main()
