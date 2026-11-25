#!/usr/bin/env python3
"""Treiber Auftrag G. Je Auftrag: build/gruppenmatrix (Suchcode, Filter + Abstieg), dann exakte Prüfung (Prüfcode,
src/gruppen_pruef.py -> build/exakt_disc) aller Überlebenden und der Vertreter mit größtem u (bis einer exakt u_max erreicht,
höchstens K). Überlebender mit exakter Diskrepanz >= d: sofort Stopp (Rückgabe 10), Matrix wird abgelegt.
Log: runs/gruppen/blocklog.txt (laufend geschrieben).

Aufruf: run_gruppen.py [-c] [-k K] [-z SEKUNDEN] [-T k/m] AUFTRAG ...   (-c: Kontrollmodus, kein Stopp bei Überlebendem)
  AUFTRAG = GRUPPE:d[:r][:s]    (r = mit Rand, s = zwei Spalten gestrichen); z. B. d9:8, z21:9:r, z5xz5:9:s
"""
import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gruppen_pruef as gp

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "runs", "gruppen")
TAFELN = os.path.join(OUT, "tafeln")
LOG = os.path.join(OUT, "blocklog.txt")


def log(s):
    line = time.strftime("%Y-%m-%d %H:%M:%S UTC ", time.gmtime()) + s
    print(line, flush=True)
    with open(LOG, "a") as f:
        f.write(line + "\n")


def main():
    args = sys.argv[1:]
    K, zeit, teil, kontrolle = 20, 1700, None, False
    if "-c" in args:
        args.remove("-c"); kontrolle = True      # Kontrolle: Überlebender mit exakt >= d ist erwartet, kein Stopp
    if "-k" in args:
        i = args.index("-k"); K = int(args[i + 1]); del args[i:i + 2]
    if "-z" in args:
        i = args.index("-z"); zeit = int(args[i + 1]); del args[i:i + 2]
    if "-T" in args:
        i = args.index("-T"); teil = args[i + 1]; del args[i:i + 2]
    os.makedirs(OUT, exist_ok=True)
    for a in args:
        t = a.split(":")
        grp, d = t[0], int(t[1])
        rand, streich = "r" in t[2:], "s" in t[2:]
        name = grp + ("_rand" if rand else "") + ("_ohne2" if streich else "")
        praefix = os.path.join(OUT, name)
        gfile = os.path.join(TAFELN, grp + ".grp")
        cmd = [os.path.join(ROOT, "build", "gruppenmatrix"), gfile, str(d), "-o", praefix, "-a"]
        cmd += (["-r"] if rand else []) + (["-S", "2"] if streich else []) + (["-T", teil] if teil else [])
        log(f"START {name} d={d}: {' '.join(cmd[1:])}")
        t0 = time.time()
        try:
            p = subprocess.run(cmd, capture_output=True, text=True, timeout=zeit)
        except subprocess.TimeoutExpired:
            log(f"ABBRUCH {name}: Zeitlimit {zeit} s überschritten")
            continue
        for line in p.stdout.strip().split("\n"):
            log(f"  {line}")
        if p.returncode != 0:
            log(f"FEHLER {name}: rc={p.returncode} {p.stderr.strip()}")
            return 3
        tmp = praefix + "_tmp.txt"
        n, tab, inv = gp.lies_tafel(gfile)
        # Überlebende exakt
        ueb = [l.split() for l in open(praefix + "_ueberlebende.txt") if l.strip()]
        for z in ueb:
            h = int(z[0], 16)
            ohne = (int(z[1]), int(z[2])) if streich else ()
            dd = gp.exakt_matrix(gp.matrix(h, n, tab, inv, rand, ohne), tmp)
            log(f"  ueberlebender {z}: exakt {dd} (d={d})")
            if dd >= d:
                fn = praefix + f"_KANDIDAT_{z[0]}{'_' + z[1] + '_' + z[2] if streich else ''}.txt"
                gp.schreibe(gp.matrix(h, n, tab, inv, rand, ohne), fn)
                if kontrolle:
                    log(f"KONTROLLE: Überlebender {z} hat exakte Diskrepanz {dd} >= {d} (erwartet). Matrix {fn}")
                    continue
                log(f"STOPP: Überlebender {z} hat exakte Diskrepanz {dd} >= {d}. Matrix {fn}")
                return 10
        # beste gefundene: Vertreter mit größtem u exakt
        top = [l.split() for l in open(praefix + "_top.txt") if l.strip()]
        umax = int(top[0][-1]) if top else -1
        best_exakt, gerechnet, bester = -1, 0, None
        for z in top[:K]:
            h = int(z[0], 16)
            ohne = (int(z[1]), int(z[2])) if streich else ()
            dd = gp.exakt_matrix(gp.matrix(h, n, tab, inv, rand, ohne), tmp)
            gerechnet += 1
            if dd > best_exakt:
                best_exakt, bester = dd, z[:-1]
            if dd == umax:
                break
        if os.path.exists(tmp):
            os.remove(tmp)
        log(f"ERGEBNIS {name} d={d}: Überlebende {len(ueb)}, u_max {umax} ({len(top)} Vertreter), "
            f"exakt geprüft {gerechnet}, größte exakte Diskrepanz darunter {best_exakt} (Vertreter {bester})"
            f"{' = u_max, also beste Diskrepanz aller Klassen genau ' + str(umax) if best_exakt == umax else ''}; "
            f"Laufzeit {time.time() - t0:.0f} s")
    return 0


if __name__ == "__main__":
    sys.exit(main())
