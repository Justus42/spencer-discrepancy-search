# Gate Z: Normalisator-Klassen voller Bahnen, unabhaengig gerechnet (ueber Halsketten-Klassen der beiden Bloecke).
# Symmetrien: unabhaengige Verschiebung und Komplement je Block, gemeinsamer Multiplikator, Blocktausch; Fixteil durch Komplementieren
# der Fixspalten auf 0 normiert. Ausgabe: je Klasse ein Vertreterwort (a | b<<q, Fixteil 0) und die Zahl voller Bahnen der Klasse.
import sys
from math import gcd
q, f = int(sys.argv[1]), int(sys.argv[2]); M = (1 << q) - 1
rot = lambda w: ((w << 1) | (w >> (q - 1))) & M
def kanon(w):
    best = w; v = w
    for _ in range(q):
        v = rot(v); best = min(best, v, v ^ M)
    return best
def mult(w, k): return sum(((w >> i) & 1) << ((i * k) % q) for i in range(q))
K = [kanon(w) for w in range(1 << q)]
groesse = {}
for w in range(1 << q): groesse[K[w]] = groesse.get(K[w], 0) + 1
ks = [k for k in range(1, q) if gcd(k, q) == 1]
MK = {c: [K[mult(c, k)] for k in ks] for c in groesse}
def periode(w):          # kleinstes s > 0 mit rot^s(w) in {w, ~w} bzw. nur w
    v = w; ps = pa = None
    for s in range(1, q + 1):
        v = rot(v)
        if ps is None and v == w: ps = s
        if pa is None and v == (w ^ M): pa = s
    return ps, pa
def bahngroesse(a, b):   # Groesse der Bahn des Paares {w, ~w}, w = (a, b, Fixteil)
    for s in range(1, q + 1):
        ra, rb = a, b
        for _ in range(s): ra, rb = rot(ra), rot(rb)
        if (ra, rb) == (a, b): return s
        if f == 0 and (ra, rb) == (a ^ M, b ^ M): return s
    return q
gesehen = set(); klassen = []
cl = sorted(groesse)
for ca in cl:
    for cb in cl:
        if (ca, cb) in gesehen: continue
        orb = set()
        for i in range(len(ks)):
            orb.add((MK[ca][i], MK[cb][i])); orb.add((MK[cb][i], MK[ca][i]))
        gesehen |= orb
        klassen.append((ca, cb, sum(groesse[x] * groesse[y] for x, y in orb)))
# volle Bahnen je Klasse: Wortpaare (a, b) mit voller Bahn, mal 2^f Fixteile, durch 2q (q Drehungen, 2 fuer Komplement)
out = []; tot = 0
for ca, cb, n in klassen:
    # Bahngroesse ist auf der Klasse nicht konstant (zusammengesetztes q): direkt zaehlen
    if all(x in (0, M) for x in (ca, cb)): continue
    voll = n
    if any(q % p == 0 for p in range(2, q)):     # zusammengesetztes q: Wortpaare der Klasse einzeln pruefen
        voll = 0
        orb = set()
        for i in range(len(ks)):
            orb.add((MK[ca][i], MK[cb][i])); orb.add((MK[cb][i], MK[ca][i]))
        mem = {}
        for w in range(1 << q): mem.setdefault(K[w], []).append(w)
        for x, y in orb:
            for a in mem[x]:
                for b in mem[y]:
                    if bahngroesse(a, b) == q: voll += 1
    if voll == 0: continue
    # Vertreter mit voller Bahn waehlen
    rep = None
    for a in [w for w in range(1 << q) if K[w] == ca]:
        for b in [w for w in range(1 << q) if K[w] == cb]:
            if bahngroesse(a, b) == q: rep = (a, b); break
        if rep: break
    bahnen = voll * (1 << f) // (2 * q); tot += bahnen
    out.append((rep[0] | (rep[1] << q), bahnen))
print(f"# q={q} f={f}: {len(out)} Klassen, Summe voller Bahnen {tot}", file=sys.stderr)
for w, n in out: print(w, n)
