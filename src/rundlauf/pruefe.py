#!/usr/bin/env python3
"""Prüfcode (kein Suchcode, teilt keinen Code mit sieb.py): rechnet die Gegenbeispiele des Siebs mit ganzen Zahlen nach.
Aufruf (aus dem Wurzelordner):
  python3 src/rundlauf/pruefe.py gegenbeispiele [FAMILIE ...]   Gegenbeispiele nachrechnen, .teil.txt -> .txt.gz, Zusammenfassung
  python3 src/rundlauf/pruefe.py exakt FAMILIE [--ab T]          exakte Diskrepanz (build/exakt_disc) für alle Matrizen mit Maximum >= T
                                                                (Standard: nur die Zeilen mit Maximum NONE)
Je Zeile der Datei wird die Matrix aus dem Generator neu gebaut (Name muss übereinstimmen), A x mit Python-ganzen Zahlen gerechnet
und geprüft: |(A x)_i| <= Maximum für alle i, mit Gleichheit bei mindestens einer Zeile, und Maximum < d (Schwelle der Familie).
"""
import sys, os, gzip, subprocess, collections, time, tempfile
from multiprocessing import Pool
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from familien import FAMILIEN, lade, stufe1

WURZEL = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
DIR = os.path.join(WURZEL, "runs", "rundlauf")
EXAKT = os.path.join(WURZEL, "build", "exakt_disc")

def lies_zeilen(pfad_teil):
    """liest FAMILIE[.zielZ].teil.txt, ersatzweise die komprimierte Fassung FAMILIE[.zielZ].txt.gz"""
    if os.path.exists(pfad_teil):
        return [l.rstrip("\n").split("\t") for l in open(pfad_teil)]
    gz = pfad_teil.replace(".teil.txt", ".txt.gz")
    with gzip.open(gz, "rt") as f:
        return [l.rstrip("\n").split("\t") for l in f]

def maximum(M, hexs):
    h = int(hexs, 16)
    n = len(M[0])
    x = [-1 if (h >> j) & 1 else 1 for j in range(n)]
    return max(abs(sum(a * b for a, b in zip(r, x))) for r in M)

def teile(arg):
    """'astra_4' -> ('astra_4', None); 'astra_4:4' -> ('astra_4', 4)  (Gegenbeispiele mit Maximum < 4 verlangt)"""
    k, _, z = arg.partition(":")
    return k, (int(z) if z else None)

def gegenbeispiele(arg):
    k, ziel = teile(arg)
    F = FAMILIEN[k]
    schwelle = ziel if ziel else F["d"]
    suffix = f".ziel{ziel}" if ziel else ""
    teil = os.path.join(DIR, k + suffix + ".teil.txt")
    zeilen = lies_zeilen(teil)
    hist = collections.Counter(); none = []; abw = []; n = 0
    t0 = time.time()
    for i, (name, M) in enumerate(lade(k)()):
        if i >= len(zeilen): break
        nm, mx, hx = zeilen[i]
        if nm != name: abw.append((i, "Name", nm, name)); continue
        n += 1
        if mx == "NONE":
            none.append(name); hist["NONE"] += 1; continue
        m = maximum(M, hx)
        if m != int(mx): abw.append((i, "Maximum", mx, m)); continue
        if m >= schwelle: abw.append((i, "nicht < Schwelle", mx, schwelle)); continue
        hist[m] += 1
    ok = (n == len(zeilen) == F["anzahl"] and not abw)
    if ok and os.path.exists(teil):
        with gzip.open(os.path.join(DIR, k + suffix + ".txt.gz"), "wt") as g:
            for z in zeilen:
                g.write("\t".join(z) + "\n")
    return arg, n, len(zeilen), dict(sorted((h, c) for h, c in hist.items() if h != "NONE")), hist.get("NONE", 0), none, abw[:5], time.time() - t0

def zeigeMatrix(M):
    return "\n".join(" ".join(str(v) for v in r) for r in M) + "\n"

def exakt_eine(arg):
    k, i, name, M = arg
    tmp = tempfile.NamedTemporaryFile("w", suffix=".txt", dir="/dev/shm", delete=False)
    tmp.write(zeigeMatrix(M)); tmp.close()
    out = subprocess.run([EXAKT, tmp.name], capture_output=True, text=True).stdout
    os.unlink(tmp.name)
    d = int(out.split("disc")[1].split(",")[0])
    return name, d

def exakt(arg, ab=None, erste=0):
    k, ziel = teile(arg)
    teil = os.path.join(DIR, k + (f".ziel{ziel}" if ziel else "") + ".teil.txt")
    zeilen = lies_zeilen(teil)
    sel = [i for i, z in enumerate(zeilen) if (z[1] == "NONE" if ab is None else (z[1] == "NONE" or int(z[1]) >= ab))]
    if erste: sel = sel[:erste]
    sel = set(sel)
    auf = []
    for i, (name, M) in enumerate(lade(k)()):
        if i > max(sel, default=-1): break
        if i in sel:
            assert zeilen[i][0] == name
            auf.append((k, i, name, M))
    t0 = time.time()
    with Pool(4) as p:
        res = p.map(exakt_eine, auf, chunksize=8)
    h = collections.Counter(d for _, d in res)
    return arg, len(auf), dict(sorted(h.items())), [r for r in res if r[1] >= (ziel or FAMILIEN[k]["d"])], time.time() - t0

if __name__ == "__main__":
    modus = sys.argv[1]
    if modus == "gegenbeispiele":
        ks = sys.argv[2:] or list(FAMILIEN)
        with Pool(4) as p:
            for k, n, nz, hist, nn, none, abw, t in p.imap(gegenbeispiele, ks):
                F = FAMILIEN[teile(k)[0]]
                print(f"{k}: {n}/{nz} Zeilen nachgerechnet (erwartet {F['anzahl']}), Schwelle {teile(k)[1] or F['d']}, Verteilung Maximum {hist}, ohne Gegenbeispiel (NONE) {nn}, Abweichungen {abw}, {t:.1f} s", flush=True)
    elif modus == "exakt":
        k = sys.argv[2]
        ab = int(sys.argv[sys.argv.index("--ab") + 1]) if "--ab" in sys.argv else None
        erste = int(sys.argv[sys.argv.index("--erste") + 1]) if "--erste" in sys.argv else 0
        k, n, h, ueber, t = exakt(k, ab, erste)
        print(f"{k}: exakt geprüft {n} Matrizen (ab Maximum {ab if ab is not None else 'NONE'}{', nur die ersten ' + str(erste) if erste else ''}), Verteilung exakte Diskrepanz {h}, davon mit Diskrepanz >= Schwelle {teile(k)[1] or FAMILIEN[teile(k)[0]]['d']}: {len(ueber)} {ueber[:5]}, {t:.1f} s", flush=True)
