"""Warmstart-Treiber, n=20, R=6, d=2, Typ (11,9). Ein Aufruf = ein Block, Startwerte parallel (hoechstens 4).
Startwert s nimmt Startdatei Nr. (s-1) % Anzahl (leere Liste = Zufallsstart). Log laeuft laufend; Fertig-Marke
runs/<tag>_seed<s>.fertig.txt; fertige Startwerte werden uebersprungen.
Aufruf: run_warm.py tag K sekunden seed_von seed_bis [startdatei ...]"""
import sys, os, subprocess, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from coset_ladder import build, words, covering_radius, BIN, ROOT
os.chdir(ROOT)
n, R, d, mult = 20, 6, 2, (11, 9)
tag, K, secs, s0, s1 = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5])
starts = sys.argv[6:]
G, Hc, m, hp, rr = build(n, mult, d)
with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
    f.write(" ".join(map(str, Hc))); hfile = f.name
procs = []
for s in range(s0, s1 + 1):
    if os.path.exists(f"runs/{tag}_seed{s}.fertig.txt"): continue
    st = starts[(s - 1) % len(starts)] if starts else None
    log = open(f"runs/{tag}_seed{s}.log.txt", "w"); log.write(f"Startdatei: {st}\n"); log.flush()
    cmd = [BIN, str(m), str(n), str(R), str(K), str(s), str(secs), hfile] + ([st] if st else [])
    procs.append((s, st, log, subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=log, text=True)))
for s, st, log, p in procs:
    out, _ = p.communicate(); log.close()
    sol = [l for l in out.splitlines() if l.startswith("SOLUTION")]
    lines = open(f"runs/{tag}_seed{s}.log.txt").read().splitlines()
    last = [l for l in lines if "unueberdeckt" in l]
    best = last[-1].split("unueberdeckt")[1].strip() if last else "?"
    tsol = [l for l in lines if l.startswith("geloest nach")]
    if sol:
        W = words(n, G, hp, list(map(int, sol[0].split()[1:])))
        full = (1 << n) - 1; S = set(W)
        ok = len(S) == len(W) and all(w ^ full in S for w in S); rad = covering_radius(n, W)
        fw = f"runs/woerter_n{n}_R{R}_d{d}_K{K}_typ11-9_{tag.split('/')[-1]}_seed{s}.txt"
        open(fw, "w").write(" ".join(map(str, sorted(W))) + "\n")
        res = f"GELOEST ({tsol[0] if tsol else ''}) Woerter={len(S)} selbstkompl={ok} Radius(Brute-Force)={rad} Datei={fw}"
    else:
        res = f"nicht geloest, kleinste Restzahl={best}"
    open(f"runs/{tag}_seed{s}.fertig.txt", "w").write(f"seed={s} start={st} {res}\n")
    print(f"seed={s} {res}", flush=True)
os.unlink(hfile)
