"""Block-Treiber K=13, n=20, R=6, d=2, Typ (11,9): ein Block = 4 Startwerte parallel, je 1800 s.
Log je Startwert laeuft laufend (stderr des Kerns direkt in die Datei). Fertig-Marke je Startwert:
runs/2026-11-23_K13_typ11-9_seed{s}.fertig.txt (eine Zeile: Ergebnis). Fertige Startwerte werden uebersprungen.
Aufruf: run_block.py block_nr secs      (block 0 = Startwerte 1-4, ..., block 3 = 13-16)"""
import sys, os, subprocess, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from coset_ladder import build, words, covering_radius, BIN, ROOT
os.chdir(ROOT)
n, R, d, K, mult = 20, 6, 2, 13, (11, 9)
blk, secs = int(sys.argv[1]), int(sys.argv[2])
seeds = [s for s in range(4 * blk + 1, 4 * blk + 5)
         if not os.path.exists(f"runs/2026-11-23_K13_typ11-9_seed{s}.fertig.txt")]
G, Hc, m, hp, rr = build(n, mult, d)
with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
    f.write(" ".join(map(str, Hc))); hfile = f.name
procs = []
for s in seeds:
    log = open(f"runs/2026-11-23_K13_typ11-9_seed{s}.log.txt", "w")
    procs.append((s, log, subprocess.Popen([BIN, str(m), str(n), str(R), str(K), str(s), str(secs), hfile],
                                           stdout=subprocess.PIPE, stderr=log, text=True)))
for s, log, p in procs:
    out, _ = p.communicate(); log.close()
    sol = [l for l in out.splitlines() if l.startswith("SOLUTION")]
    last = [l for l in open(f"runs/2026-11-23_K13_typ11-9_seed{s}.log.txt") if "unueberdeckt" in l]
    best = last[-1].split("unueberdeckt")[1].strip() if last else "?"
    if sol:
        W = words(n, G, hp, list(map(int, sol[0].split()[1:])))
        full = (1 << n) - 1; S = set(W)
        ok = len(S) == len(W) and all(w ^ full in S for w in S); rad = covering_radius(n, W)
        fw = f"runs/woerter_n{n}_R{R}_d{d}_K{K}_typ11-9_seed{s}.txt"
        open(fw, "w").write(" ".join(map(str, sorted(W))) + "\n")
        res = f"GELOEST Woerter={len(S)} selbstkompl={ok} Radius(Brute-Force)={rad} Datei={fw}"
    else:
        res = f"nicht geloest, kleinste Restzahl={best}"
    open(f"runs/2026-11-23_K13_typ11-9_seed{s}.fertig.txt", "w").write(f"seed={s} {res}\n")
    print(f"seed={s} {res}", flush=True)
os.unlink(hfile)
