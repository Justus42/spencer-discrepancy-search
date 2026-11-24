"""Allgemeiner Block-Treiber (Zufallsstart): ein Aufruf = ein Block, Startwerte parallel (hoechstens 4).
Log je Startwert laeuft laufend (stderr des Kerns). Fertig-Marke <ordner>/<tag>_seed<s>.fertig.txt; fertige
Startwerte werden uebersprungen. Treffer: Woerter nach <ordner>/woerter_..., Radius per Brute-Force ueber alle 2^n Punkte.
Aufruf: run_gen.py ordner tag n R d K typ sekunden seed_von seed_bis    (typ z. B. 13,9)"""
import sys, os, subprocess, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from coset_ladder import build, words, covering_radius, BIN, ROOT
os.chdir(ROOT)
od, tag = sys.argv[1], sys.argv[2]
n, R, d, K = map(int, sys.argv[3:7]); mult = tuple(map(int, sys.argv[7].split(",")))
secs, s0, s1 = int(sys.argv[8]), int(sys.argv[9]), int(sys.argv[10])
os.makedirs(od, exist_ok=True)
G, Hc, m, hp, rr = build(n, mult, d)
with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
    f.write(" ".join(map(str, Hc))); hfile = f.name
pre = lambda s: f"{od}/{tag}_seed{s}"
procs = []
for s in range(s0, s1 + 1):
    if os.path.exists(pre(s) + ".fertig.txt"): continue
    log = open(pre(s) + ".log.txt", "w"); log.write(f"n={n} R={R} d={d} K={K} typ={mult} seed={s} secs={secs}\n"); log.flush()
    procs.append((s, log, subprocess.Popen([BIN, str(m), str(n), str(R), str(K), str(s), str(secs), hfile],
                                           stdout=subprocess.PIPE, stderr=log, text=True)))
for s, log, p in procs:
    out, _ = p.communicate(); log.close()
    lines = open(pre(s) + ".log.txt").read().splitlines()
    last = [l for l in lines if "unueberdeckt" in l]
    best = last[-1].split("unueberdeckt")[1].strip() if last else "?"
    zl = [l for l in lines if l.startswith("Zeitlimit")]
    sol = [l for l in out.splitlines() if l.startswith("SOLUTION")]
    if sol:
        W = words(n, G, hp, list(map(int, sol[0].split()[1:])))
        full = (1 << n) - 1; S = set(W)
        ok = len(S) == len(W) and all(w ^ full in S for w in S); rad = covering_radius(n, W)
        fw = f"{od}/woerter_n{n}_R{R}_d{d}_K{K}_typ{'-'.join(map(str, mult))}_seed{s}.txt"
        open(fw, "w").write(" ".join(map(str, sorted(W))) + "\n")
        res = f"GELOEST ({last[-1].split(']')[0][1:]}) Woerter={len(S)} selbstkompl={ok} Radius(Brute-Force)={rad} Datei={fw}"
    else:
        res = f"nicht geloest, kleinste Restzahl={best} ({zl[0] if zl else ''})"
    open(pre(s) + ".fertig.txt", "w").write(f"seed={s} {res}\n")
    print(f"typ={mult} seed={s} {res}", flush=True)
os.unlink(hfile)
