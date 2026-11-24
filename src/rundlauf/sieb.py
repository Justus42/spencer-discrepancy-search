#!/usr/bin/env python3
"""Treiber (kein eigener Suchcode): ruft für jede Matrix einer Familie build/pruefstand (src/pruefstand.c) auf.
Es wird hier NICHT geprüft; jede Zeile ist nur eine Behauptung des Siebs. Geprüft wird in pruefe.py (getrennt, ganzzahlig).

Aufruf (aus dem Wurzelordner):
  python3 src/rundlauf/sieb.py FAMILIE [--probe N] [--zeit SEK] [--max-sek GESAMT]
Stufen je Matrix (erste mit Treffer gilt): Ziel T1 (unter dem Wert aus Auftrag Punkt 4), dann, falls nötig, B+1 bzw. d.
Ausgabe: runs/rundlauf/FAMILIE.teil.txt, eine Zeile je Matrix in Generatorreihenfolge: Name <TAB> Maximum <TAB> Vektor hex
  Vektor: Bit j = 1 heißt x_j = -1 (wie bei exakt_disc.c). Maximum "NONE": kein Gegenbeispiel mit Maximum < d gefunden (Kandidat Überlebender).
Fortsetzbar: vorhandene Zeilen der .teil.txt werden übersprungen (Reihenfolge ist deterministisch).
Bei --max-sek: Abbruch nach dieser Gesamtzeit (Blockgrenze), Rest beim nächsten Aufruf.
"""
import sys, os, subprocess, time, argparse, re
from multiprocessing import Pool
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from familien import FAMILIEN, lade, stufe1

WURZEL = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
BIN = os.path.join(WURZEL, "build", "pruefstand")
AUSGABE = os.path.join(WURZEL, "runs", "rundlauf")

def stufen(k):
    F = FAMILIEN[k]
    b = F["besser"]
    ziele = [stufe1(k)]
    if b[0] == "best": ziele.append(b[1] + 1)
    ziele.append(F["d"])
    ziele = sorted(set(z for z in ziele if z <= F["d"]))
    return ziele

def laufe(arg):
    idx, name, M, ziele, zeiten = arg
    tmp = f"/dev/shm/rl_{os.getpid()}.txt"
    with open(tmp, "w") as f:
        f.write("\n".join(" ".join(str(v) for v in r) for r in M) + "\n")
    best = None
    for T, z in zip(ziele, zeiten):
        out = subprocess.run([BIN, tmp, "-d", str(T), "-z", str(z)], capture_output=True, text=True).stdout
        m = re.search(r"^x = ([+-]+)$", out, re.M)
        mm = re.search(r"bestes max (\d+)", out)
        x = m.group(1); mx = int(mm.group(1))
        hexs = "%x" % sum(1 << j for j, c in enumerate(x) if c == "-")
        if mx < T:
            return idx, f"{name}\t{mx}\t{hexs}\n"
        best = (mx, hexs)
    return idx, f"{name}\tNONE\t{best[1]}\n"

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("familie")
    ap.add_argument("--probe", type=int, default=0)
    ap.add_argument("--zeit", type=float, default=0.3, help="Sekunden je Stufe 1; spätere Stufen das Zehnfache")
    ap.add_argument("--max-sek", type=float, default=0)
    ap.add_argument("--prozesse", type=int, default=4)
    ap.add_argument("--ziel", type=int, default=0, help="nur diese eine Stufe (Maximum < ZIEL), Ausgabe in FAMILIE.zielZIEL.teil.txt; Zeit = --zeit")
    a = ap.parse_args()
    k = a.familie
    ziele = [a.ziel] if a.ziel else stufen(k)
    zeiten = [a.zeit * (1 if i == 0 else 10) for i in range(len(ziele))]
    os.makedirs(AUSGABE, exist_ok=True)
    pfad = os.path.join(AUSGABE, "probe_" + k + ".txt") if a.probe else os.path.join(AUSGABE, k + (f".ziel{a.ziel}" if a.ziel else "") + ".teil.txt")
    fertig = 0
    if os.path.exists(pfad) and not a.probe:
        with open(pfad, "rb") as f:
            daten = f.read()
        if daten and not daten.endswith(b"\n"):          # abgebrochene letzte Zeile verwerfen
            daten = daten[:daten.rfind(b"\n") + 1]
            with open(pfad, "wb") as f: f.write(daten)
        fertig = daten.count(b"\n")
    t0 = time.time()
    gen = lade(k)()
    def arbeit():
        for i, (name, M) in enumerate(gen):
            if a.probe and i >= a.probe: return
            if i < fertig: continue
            yield (i, name, M, ziele, zeiten)
    n = 0
    mode = "w" if a.probe else "a"
    with open(pfad, mode, buffering=1) as out, Pool(a.prozesse) as p:
        for idx, zeile in p.imap(laufe, arbeit(), chunksize=16):
            out.write(zeile); n += 1
            if a.max_sek and time.time() - t0 > a.max_sek:
                p.terminate(); break
    ges = fertig + n
    print(f"{k}: Ziele {ziele}, Zeiten {zeiten}; neu {n}, gesamt {ges} von {FAMILIEN[k]['anzahl']}, {time.time()-t0:.1f} s", flush=True)

if __name__ == "__main__":
    main()
