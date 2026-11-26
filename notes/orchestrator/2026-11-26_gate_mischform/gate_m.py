#!/usr/bin/env python3
"""gate_m.py -- Gate zu Auftrag M (Orchestrator). Eigener Code, nichts aus src/misch_*.c oder verifiers/mischform/.
Punkte und Zeilen als ganze Zahlen (Bit j = 1 genau dann, wenn Eintrag j = -1). Eine Zeile r ueberdeckt x, wenn |<r,x>| >= d,
also Abstand(r,x) <= R oder >= m - R mit R = (m - d) / 2. Offene Punkte sind unter x -> -x abgeschlossen: gerechnet wird mit Klassen (hoechstes Bit 0).
Aufruf aus dem Repo-Wurzelordner: gate_m.py TEIL   (TEIL = s0 | s3 | s2 | s4 | c20)"""
import sys, glob, os, re, numpy as np
from collections import Counter
_pc = {}
def PC(m):
    if m not in _pc:
        t = np.zeros(1 << m, dtype=np.int8)
        for j in range(m): t[1 << j: 1 << (j + 1)] = t[:1 << j] + 1
        _pc[m] = t
    return _pc[m]
def zahl(z): return sum(1 << j for j, e in enumerate(z) if e == -1)
def lies_matrix(fn): return [[int(t) for t in l.split()] for l in open(fn) if l.strip() and not l.startswith('#')]
def lies_paar(fn):
    M = []
    for l in open(fn):
        if '|' in l and not l.startswith('#'):
            u, w = ([int(t) for t in s.split()] for s in l.split('|')); M.append(u + w); M.append(u + [-e for e in w])
    return M
def offen(M, d):
    """offene Klassen (hoechstes Bit 0) ueber alle 2^(m-1) Klassen, direkt auf der Matrix"""
    m = len(M[0]); R = (m - d) // 2; pc = PC(m); X = np.arange(1 << (m - 1), dtype=np.int64); o = np.ones(len(X), dtype=bool)
    for z in M:
        a = pc[X ^ zahl(z)]; o &= (a > R) & (a < m - R)
    return X[o], m, R
def deckt(C, p, m, R):
    a = PC(m)[C ^ p]; return (a <= R) | (a >= m - R)
def kugel(c, m, R): return np.nonzero(PC(m) <= R)[0].astype(np.int64) ^ int(c)
def f1(S, m, R):
    """alle Zeilen bis auf Vorzeichen, die jede Klasse aus S ueberdecken (vollstaendig: eine solche Zeile oder ihr Negativ liegt in der Kugel um S[0])"""
    if len(S) == 0: return None
    C = kugel(S[0], m, R)
    for p in S:
        C = C[deckt(C, int(p), m, R)]
        if len(C) == 0: break
    return C
def maxcov(S, m, R):
    X = np.arange(1 << (m - 1), dtype=np.int64); c = np.zeros(len(X), dtype=np.int32)
    for p in S: c += deckt(X, int(p), m, R)
    return int(c.max())
def f2(S, m, R):
    """gibt es zwei Zeilen, die S ueberdecken? Zeile 1 ueberdeckt S[0], bis auf Vorzeichen in der Kugel um S[0]; Rest muss in eine Zeile passen."""
    mc = maxcov(S, m, R); C = kugel(S[0], m, R); cnt = np.zeros(len(C), dtype=np.int32)
    for p in S: cnt += deckt(C, int(p), m, R)
    kand = C[len(S) - cnt <= mc]; ja = 0
    for r in kand:
        rest = S[~deckt(S, int(r), m, R)]
        if len(rest) == 0 or len(f1(rest, m, R)) > 0: ja += 1
    return mc, int(cnt.max()), len(kand), ja
teil = sys.argv[1]
if teil == 's0':
    for fn in sorted(glob.glob('runs/leiter_n/s[23]c18-5*_k12_t9-9_seed*.paar.txt')):
        S, m, R = offen(lies_paar(fn), 8); C = f1(S, m, R)
        print("%-34s offene Punkte %d, Zeilen, die alle ueberdecken: %d (bis auf Vorzeichen %d)" % (os.path.basename(fn)[:-9], 2 * len(S), 2 * len(C), len(C)))
elif teil == 's3':
    soll = {}
    for fn in glob.glob('runs/mischform/s3_*.out.txt'):
        name = None
        for l in open(fn):
            a = re.search(r'# (runs/mischform/s3/\S+\.matrix\.txt)', l); b = re.search(r'offene Punkte \(von[^)]*\): (\d+)', l)
            if a: name = a.group(1)
            if b and name: soll[name] = int(b.group(1)); name = None
    ab = 0; stat = Counter()
    for fn in sorted(glob.glob('runs/mischform/s3/*.matrix.txt')):
        M = lies_matrix(fn); S, m, R = offen(M, 8); typ = re.search(r'_t(\d+)-(\d+)_', fn); m1 = int(typ.group(1))
        norm = [tuple(z) if z[0] == 1 else tuple(-e for e in z) for z in M]; menge = set(norm); paare = 0
        for z in norm:
            g = z[:m1] + tuple(-e for e in z[m1:]); g = g if g[0] == 1 else tuple(-e for e in g)
            paare += g in menge
        w = len(set(zahl(z) for z in M) | set(zahl([-e for e in z]) for z in M))
        s = soll.get(fn); ab += (s is not None and s != 2 * len(S)); 
        key = (os.path.basename(fn).split('_seed')[0], 2 * len(S), paare // 2, len(M) - paare); stat[key] += 1
        if s is None: print("ohne Sollwert im Blocklog:", fn)
    for k in sorted(stat): print("%-28s offene Punkte %4d, Zeilenpaare (u,w),(u,-w): %2d, uebrige Zeilen: %d   -> %d Laeufe" % (k + (stat[k],)))
    print("Matrizen: %d, mit Sollwert verglichen: %d, Abweichungen: %d" % (sum(stat.values()), len(soll), ab))
elif teil == 's2':
    gr = {}
    for fn in sorted(glob.glob('runs/mischform/s2/*.paar.txt')):
        S, m, R = offen(lies_paar(fn), 8); t = open(fn[:-9] + '.frei.txt').read()
        so = int(re.search(r'offene Punkte (\d+)', t).group(1)); typ = re.search(r'_t(\d+-\d+)_', fn).group(1)
        gr.setdefault((typ, 2 * len(S)), []).append((fn, S, so == 2 * len(S), 'f=2: NEIN' in t))
    print("Zustaende: %d, Gruppen (Typ, offene Punkte): %d, offene Punkte = Angabe der Sitzung: %s, Sitzung f=2 NEIN bei allen: %s"
          % (sum(len(v) for v in gr.values()), len(gr), all(x[2] for v in gr.values() for x in v), all(x[3] for v in gr.values() for x in v)))
    for (typ, n), v in sorted(gr.items()):
        fn, S = v[0][0], v[0][1]; mc, c1, kand, ja = f2(S, 18, 5)
        print("Typ %-5s offene Punkte %4d (%3d Zustaende); eigener Vertreter %s: maxcov %d, beste erste Zeile in der Kugel %d, Kandidaten nach Schnitt %d, f=2 %s"
              % (typ, n, len(v), os.path.basename(fn)[:-9], 2 * mc, 2 * c1, kand, "JA" if ja else "nein (vollstaendig)"))
elif teil == 's4':
    for pat, d in (('runs/leiter_n/*c16-4*_k15_*.paar.txt', 8), ('runs/leiter_n/*c23-7*_k15_*.paar.txt', 9)):
        st = Counter(); n = 0
        for fn in sorted(glob.glob(pat)):
            S, m, R = offen(lies_paar(fn), d); C = f1(S, m, R); st[(2 * len(S), len(C))] += 1; n += 1
        print("%s: %d Zustaende, d = %d, R = %d; (offene Punkte, Zeilen bis auf Vorzeichen, die alle ueberdecken) -> Anzahl: %s" % (pat, n, d, R, dict(sorted(st.items()))))
elif teil == 'c20':
    P = [l for l in open('runs/zerlegung/k3_seed1.paar.txt') if '|' in l and not l.startswith('#')]
    for i in range(len(P)):
        M = []
        for k, l in enumerate(P):
            if k == i: continue
            u, w = ([int(t) for t in s.split()] for s in l.split('|')); M.append(u + w); M.append(u + [-e for e in w])
        S, m, R = offen(M, 8); C = f1(S, m, R); extra = ""
        if True: mc, c1, kand, ja = f2(S, m, R); extra = "; eigenes f=2: maxcov %d, Kandidaten %d, Loesungen fuer Zeile 1: %d -> %s" % (2 * mc, kand, ja, "JA" if ja else "nein")
        print("(20,6) ohne Paarzeile %2d: offene Punkte %d, f=1 Zeilen bis auf Vorzeichen: %d%s" % (i, 2 * len(S), len(C), extra))
