#!/usr/bin/env python3
"""Treiber Teil C. Je Auftrag: build/zirkulant (Suchcode, Filter + Abstieg), dann exakte Prüfung (Prüfcode)
aller Überlebenden und der Vertreter mit größtem u (bis eines exakt u_max erreicht, höchstens K).
Überlebender mit exakter Diskrepanz >= d: sofort Stopp (Rückgabe 10).
Log: runs/teilC/blocklog.txt (laufend geschrieben).

Aufruf: run_zirkulant.py [-k K] [-z SEKUNDEN] n[r] ...   (r = mit Rand; d wird aus der Rekordschwelle berechnet)
"""
import os
import subprocess
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import zirkulant_pruef as zp

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "runs", "teilC")
LOG = os.path.join(OUT, "blocklog.txt")


def schwelle(n, rand):
    """kleinstes d mit d/sqrt(N) > 5/sqrt(8), N = n bzw. n+1, d mit der Parität von n (jede Zeile hat n Einträge +-1)."""
    N = n + (1 if rand else 0)
    d = 1
    while 8 * d * d <= 25 * N or d % 2 != n % 2:
        d += 1
    return d


def log(s):
    line = time.strftime("%Y-%m-%d %H:%M:%S UTC ", time.gmtime()) + s
    print(line, flush=True)
    with open(LOG, "a") as f:
        f.write(line + "\n")


def main():
    args = sys.argv[1:]
    K, zeit = 20, 1700
    if "-k" in args:
        i = args.index("-k"); K = int(args[i + 1]); del args[i:i + 2]
    if "-z" in args:
        i = args.index("-z"); zeit = int(args[i + 1]); del args[i:i + 2]
    os.makedirs(OUT, exist_ok=True)
    for a in args:
        rand = a.endswith("r")
        n = int(a.rstrip("r"))
        d = schwelle(n, rand)
        name = f"n{n}{'_rand' if rand else ''}"
        praefix = os.path.join(OUT, name)
        cmd = [os.path.join(ROOT, "build", "zirkulant"), str(n), str(d), "-o", praefix] + (["-r"] if rand else [])
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
        # Überlebende exakt
        ueb = [l.split() for l in open(praefix + "_ueberlebende.txt") if l.strip()]
        tmp = praefix + "_tmp.txt"
        for h, u in ueb:
            dd = zp.exakt_eins(int(h, 16), n, rand, tmp)
            log(f"  ueberlebender {h}: exakt {dd} (d={d})")
            if dd >= d:
                zp.schreibe(zp.matrix(int(h, 16), n, rand), praefix + f"_KANDIDAT_{h}.txt")
                log(f"STOPP: Überlebender {h} hat exakte Diskrepanz {dd} >= {d}. Matrix {praefix}_KANDIDAT_{h}.txt")
                return 10
        # beste gefundene: Vertreter mit größtem u exakt
        top = [l.split() for l in open(praefix + "_top.txt") if l.strip()]
        umax = int(top[0][1]) if top else -1
        best_exakt, gerechnet = -1, 0
        for h, u in top[:K]:
            dd = zp.exakt_eins(int(h, 16), n, rand, tmp)
            gerechnet += 1
            best_exakt = max(best_exakt, dd)
            if dd == umax:
                break
        if os.path.exists(tmp):
            os.remove(tmp)
        log(f"ERGEBNIS {name} d={d}: Überlebende {len(ueb)}, u_max {umax} ({len(top)} Vertreter), "
            f"exakt geprüft {gerechnet}, größte exakte Diskrepanz darunter {best_exakt}"
            f"{' = u_max, also beste Diskrepanz aller Klassen genau ' + str(umax) if best_exakt == umax else ''}; "
            f"Laufzeit {time.time() - t0:.0f} s")
    return 0


if __name__ == "__main__":
    sys.exit(main())
