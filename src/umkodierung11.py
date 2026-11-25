#!/usr/bin/env python3
"""Prüfcode: Umkodierung nach Sol (Abschnitt I). n = 11, d = 6, Zeilen mit genau einer Null (alle 5632 modulo Vorzeichen).

Binär: Bit 1 = Eintrag -1. Jede Klasse modulo Komplement hat genau einen Vertreter geraden Gewichts (n ungerade); H = diese 1024.
Behauptung: Treffermenge (als Teilmenge von H) = {x in H: d(x,c1) <= 2 oder d(x,c2) <= 2}, wobei c1, c2 die beiden
aktiven Muster (Zeile und Gegenzeile) sind, in der Nullspalte so ergänzt, dass das Gewicht gerade ist.
Nach Streichen der Koordinate 10 (gemeinsam für alle Zeilen): Treffermenge = zwei gewöhnliche Radius-2-Kugeln im 10-Würfel.
Nur ganze Zahlen. Aufruf: python3 src/umkodierung11.py
"""
import itertools

N, D = 11, 6
H = [x for x in range(1 << N) if bin(x).count("1") % 2 == 0]
assert len(H) == 1024
wt = lambda v: bin(v).count("1")


def skalar(a, x):
    return sum((-ai if (x >> i) & 1 else ai) for i, ai in enumerate(a))


abw_h = abw_p = 0
komplementaer = 0
zeilen = 0
for z in range(N):
    for vz in itertools.product((1, -1), repeat=N - 1):
        if vz[0] != 1:
            continue  # modulo Vorzeichen
        a = list(vz)
        a.insert(z, 0)
        zeilen += 1
        treffer = {x for x in H if abs(skalar(a, x)) >= D}
        zentren = []
        for s in (1, -1):
            c = 0
            for i, ai in enumerate(a):
                if ai * s == -1:
                    c |= 1 << i
            if wt(c) % 2:
                c |= 1 << z
            zentren.append(c)
        kugeln = {x for x in H if min(wt(x ^ c) for c in zentren) <= 2}
        if kugeln != treffer:
            abw_h += 1
        # punktiert: Koordinate 10 streichen
        m = (1 << 10) - 1
        zp = [c & m for c in zentren]
        tp = {x & m for x in treffer}
        kp = {y for y in range(1 << 10) if min(wt(y ^ c) for c in zp) <= 2}
        if len(tp) != len(treffer) or kp != tp:
            abw_p += 1
        if zp[0] ^ zp[1] == m:
            komplementaer += 1
print(f"Zeilen: {zeilen}")
print(f"Abweichungen (zwei Radius-2-Kugeln in H): {abw_h}")
print(f"Abweichungen (nach Streichen von Koordinate 10, zwei Kugeln im 10-Wuerfel): {abw_p}")
print(f"Zentren nach Streichen voll komplementaer: {komplementaer}")
