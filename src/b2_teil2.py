#!/usr/bin/env python3
"""b2_teil2.py (Auftrag B2, Pruefcode, nur ganze Zahlen)

Hilfsschritte fuer Teil 2 (Klassen):
  kontrolle SEED AUS              1p3, 20 zufaellige Kopien (Spaltenperm. mit Vorzeichen, Zeilenperm., Zeilenvorzeichen), 2p2
  liste P_ELEMENTE LISTE AUS      Loesungsliste (b2_zaehl) -> Matrixbloecke (24 x 20, +-1)
  dateien AUS DATEI...            Matrixdateien hintereinander als Bloecke
  pruefe BLOECKE KLASSEN          jede Zeile "i Klasse |Aut| pi | t" aus `b2_aequiv klassen`: Abbildung auf Matrix i anwenden,
                                  Wortmenge muss gleich der des Klassenvertreters sein
"""
import sys, random
N = 20; M = (1 << N) - 1

def lies_bloecke(fn):
    B = []; cur = []
    for l in open(fn):
        t = l.split()
        if not t or l.startswith('#'):
            if cur: B.append(cur); cur = []
            continue
        cur.append([int(x) for x in t])
    if cur: B.append(cur)
    return B

def schreibe(f, mat, kommentar=None):
    if kommentar: f.write('# %s\n' % kommentar)
    for r in mat: f.write(' '.join('%2d' % x for x in r) + '\n')
    f.write('\n')

def woerter(mat):
    S = set()
    for r in mat:
        w = sum(1 << j for j, x in enumerate(r) if x == -1)
        S.add(w); S.add(w ^ M)
    return S

def abbilden(mat, pi, t):
    out = []
    for r in mat:
        n = [0] * len(r)
        for j, x in enumerate(r): n[pi[j]] = t[j] * x
        out.append(n)
    return out

def main():
    m = sys.argv[1]
    if m == 'kontrolle':
        random.seed(int(sys.argv[2]))
        a = lies_bloecke('controls/had24_1p3_paley12.txt')[0]
        b = lies_bloecke('controls/had24_2p2_paley12.txt')[0]
        with open(sys.argv[3], 'w') as f:
            schreibe(f, a, '1p3')
            for k in range(20):
                pi = list(range(N)); random.shuffle(pi)
                t = [random.choice((1, -1)) for _ in range(N)]
                c = abbilden(a, pi, t)
                random.shuffle(c)
                c = [[x * s for x in r] for r, s in zip(c, [random.choice((1, -1)) for _ in c])]
                schreibe(f, c, 'Kopie %d' % (k + 1))
            schreibe(f, b, '2p2')
    elif m == 'liste':
        P = []
        for l in open(sys.argv[2]):
            if l.startswith('#'): continue
            x, y = l.split('|'); P.append(([int(v) for v in x.split()], [int(v) for v in y.split()]))
        def bild(g, w):
            pi, t = g; r = 0
            for j in range(N):
                bb = (w >> j) & 1
                if t[j] == -1: bb ^= 1
                r |= bb << pi[j]
            return r
        with open(sys.argv[4], 'w') as f:
            for k, l in enumerate(open(sys.argv[3])):
                if not l.startswith('L '): continue
                reps = [int(x) for x in l.split('|')[2].split()[1:]]
                paare = sorted({min(bild(g, v), bild(g, v) ^ M) for v in reps for g in P})
                mat = [[-1 if (p >> j) & 1 else 1 for j in range(N)] for p in paare]
                schreibe(f, mat, 'Loesung %d' % k)
    elif m == 'dateien':
        with open(sys.argv[2], 'w') as f:
            for fn in sys.argv[3:]:
                schreibe(f, lies_bloecke(fn)[0], fn)
    elif m == 'pruefe':
        B = lies_bloecke(sys.argv[2])
        rep = {}; zeilen = []
        for l in open(sys.argv[3]):
            if l.startswith('#'):
                if 'Vertreter' in l:
                    t = l.split(); rep[int(t[2].rstrip(':'))] = int(t[4].rstrip(','))
                continue
            a, b = l.split('|')
            t = a.split(); i, k = int(t[0]), int(t[1]); pi = [int(x) for x in t[3:]]
            tt = [int(x) for x in b.split()]
            zeilen.append((i, k, pi, tt))
        ok = 0
        for i, k, pi, tt in zeilen:
            assert sorted(pi) == list(range(N))
            if woerter(abbilden(B[i], pi, tt)) == woerter(B[rep[k]]): ok += 1
            else: print('FEHLER: Matrix', i, 'nicht auf Vertreter', rep[k], 'abgebildet')
        print('Abbildungen geprueft: %d, richtig: %d (Matrizen %d, Klassen %d)' % (len(zeilen), ok, len(B), len(rep)))

main()
