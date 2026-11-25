"""Prüfcode (kein Suchcode), Auftrag L Teil 1: Alleinanteil je Nebenklasse, Brute-Force über alle 2^20 Punkte.
Eigenes Skript, unabhängig von notes/orchestrator/2026-11-25_vortest/leiter_vortest.py.
Zu einem Wortlisten-Code W (Länge 20, selbstkomplementär): Kern T = {t : W + t = W} (enthält 0 und 1...1),
Nebenklassen von T in W, für jeden Punkt x die Zahl der Nebenklassen mit einem Wort im Abstand <= 6.
Alleinanteil einer Nebenklasse = Zahl der Punkte, die nur sie überdeckt.
Aufruf: leiter_allein.py datei.txt [datei.txt ...]   (Wörter als Zahlen, durch Leerzeichen getrennt)"""
import sys
import numpy as np

N, R = 20, 6
X = np.arange(1 << N, dtype=np.uint32)


def lade(fn):
    return sorted(set(int(t) for t in open(fn).read().split()))


def kern(W):
    S = set(W)
    return sorted(t for t in {w ^ W[0] for w in W} if all((w ^ t) in S for w in W))


def analyse(W):
    S = set(W)
    full = (1 << N) - 1
    assert all((w ^ full) in S for w in S), "nicht selbstkomplementär"
    T = kern(W)
    klassen, gesehen = [], set()
    for w in W:
        if w in gesehen:
            continue
        k = sorted(w ^ t for t in T)
        gesehen.update(k)
        klassen.append(k)
    assert sum(len(k) for k in klassen) == len(W)
    cnt = np.zeros(1 << N, dtype=np.uint8)
    cov = []
    for k in klassen:
        d = np.full(1 << N, 255, dtype=np.uint8)
        for w in k:
            np.minimum(d, np.bitwise_count(X ^ np.uint32(w)).astype(np.uint8), out=d)
        c = d <= R
        cov.append(c)
        cnt += c
    radius_ok = int((cnt == 0).sum()) == 0
    allein = [int((c & (cnt == 1)).sum()) for c in cov]
    return dict(woerter=len(W), kern=len(T), klassen=len(klassen), radius6=radius_ok,
                allein=allein, kerngewichte=sorted({bin(t).count("1") for t in T if t not in (0, full)}))


if __name__ == "__main__":
    for fn in sys.argv[1:]:
        r = analyse(lade(fn))
        print(f"{fn}: W={r['woerter']} Kern={r['kern']} (Gew. {r['kerngewichte']}) Klassen={r['klassen']} "
              f"Radius<=6={r['radius6']} min_allein={min(r['allein'])} allein={sorted(r['allein'])}", flush=True)
