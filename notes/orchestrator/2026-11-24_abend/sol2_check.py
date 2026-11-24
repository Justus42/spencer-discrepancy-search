# Nachrechnung von Sols P1 (zweite Antwort): n = 14, d = 7, Zeile mit genau einer Null.
# Behauptung: Auf jeder Paritaetshaelfte H_p ist die Treffermenge einer Zeile genau eine antipodale Kugel vom
# Radius 3 im 13-Wuerfel (nach Ergaenzen der Null und Streichen einer beliebigen gemeinsamen Koordinate k).
# Folge: 14 Zeilen ueberdecken nur, wenn es (je Paritaet) einen selbstkomplementaeren Code (13,3) mit 28 Woertern gibt.
import numpy as np
n, r, d = 14, 3, 7
xs = np.arange(1 << n, dtype=np.int64)
par = np.bitwise_count(xs.astype(np.uint64)) % 2          # Gewichtsparitaet = Vorzeichenprodukt
S = 1 - 2 * ((xs[:, None] >> np.arange(n)) & 1)
rng = np.random.default_rng(7); bad = 0; cnt = 0; groessen = set()
for _ in range(120):
    j = int(rng.integers(n)); a = rng.choice([-1, 1], size=n); a[j] = 0
    hit = np.abs(S @ a) >= d
    w0 = sum(1 << c for c in range(n) if a[c] == -1)
    for p in (0, 1):
        H = par == p
        c = w0 if bin(w0).count("1") % 2 != p else w0 | (1 << j)     # Ergaenzung mit Paritaet 1-p
        assert bin(c).count("1") % 2 == 1 - p
        for k in range(n):                                            # jede gestrichene Koordinate
            low = (1 << k) - 1
            xp = ((xs >> (k + 1)) << k) | (xs & low); cp = ((c >> (k + 1)) << k) | (c & low)
            dd = np.bitwise_count((xp ^ cp).astype(np.uint64))
            ball = (dd <= r) | (dd >= n - 1 - r)
            bad += int((hit[H] != ball[H]).any()); cnt += 1
        groessen.add(int(hit[H].sum()) // 2)
print("geprueft:", cnt, "Faelle (Zeile x Paritaet x Koordinate), Abweichungen:", bad, "| Klassen je Zeile und Haelfte:", groessen)
