# Nachrechnung von Sols Umkodierung (n = 11 und n = 17): Treffermenge einer Zeile mit einer Null
# = zwei gewoehnliche Kugeln im (n-1)-Wuerfel, nach Wahl des geraden Vertreters und Streichen von Koordinate 0.
import numpy as np, sys
def check(n, r, stichprobe=None):
    d = n - 1 - 2 * r
    N = 1 << n
    xs = np.arange(N, dtype=np.int64)
    even = xs[np.bitwise_count(xs.astype(np.uint64)) % 2 == 0]          # gerade Vertreter der Klassen
    S = 1 - 2 * ((even[:, None] >> np.arange(n)) & 1)                    # Vorzeichen
    pun = even >> 1                                                      # Koordinate 0 gestrichen
    assert len(set(pun.tolist())) == 1 << (n - 1)
    rng = np.random.default_rng(1); bad = 0; cnt = 0; komp = 0
    rows = [(j, b) for j in range(n) for b in range(1 << (n - 2))]
    if stichprobe: rows = [rows[i] for i in rng.choice(len(rows), stichprobe, replace=False)]
    for j, bits in rows:
        oth = [c for c in range(n) if c != j]
        a = np.zeros(n, dtype=np.int64); a[oth[0]] = 1
        for t, c in enumerate(oth[1:]): a[c] = -1 if (bits >> t) & 1 else 1
        hit = np.abs(S @ a) >= d
        zent = []
        for sgn in (1, -1):
            w = sum(1 << c for c in oth if sgn * a[c] == -1)
            if bin(w).count("1") % 2: w |= 1 << j                        # gerade ergaenzen
            zent.append(w >> 1)
        ball = np.zeros(len(even), dtype=bool)
        for z in zent: ball |= np.bitwise_count((pun ^ z).astype(np.uint64)) <= r
        bad += int((hit != ball).any()); cnt += 1
        komp += int(zent[0] ^ zent[1] == (1 << (n - 1)) - 1)
    print(f"n={n}, r={r}, d={d}: {cnt} Zeilen geprueft, Abweichungen {bad}, davon Zentrenpaar voll komplementaer {komp}")
check(11, 2)
check(17, 4, stichprobe=300)
