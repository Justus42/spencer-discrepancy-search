"""Stufenlauf Dimension 2, Laenge 20, Radius 6: K absteigend, je Stufe Typen x Startwerte,
4 parallel, Stopp bei der ersten Stufe ohne Loesung oder nach K<=10-Treffer oder bei Zeitgrenze.
Aufruf: python3 src/run_stufen.py K_start K_ende sekunden_je_lauf gesamt_sekunden"""
import sys, time, subprocess, os
from concurrent.futures import ThreadPoolExecutor
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
kstart, kend, secs, total = map(int, sys.argv[1:5])
types = ["10,10", "11,9", "12,8"]; seeds = [1, 2, 3, 4]
t0 = time.time()
def job(a):
    K, typ, seed = a
    if time.time() - t0 + secs > total: return f"uebersprungen (Zeitgrenze) K={K} typ={typ} seed={seed}\n"
    p = subprocess.run([sys.executable, os.path.join(ROOT, "src/coset_ladder.py"), "20", "6", "2", str(K), str(secs), str(seed), typ],
                       capture_output=True, text=True, cwd=ROOT)
    return p.stdout + p.stderr
for K in range(kstart, kend - 1, -1):
    log = os.path.join(ROOT, f"runs/2026-11-23_stufen_K{K}.log.txt")
    jobs = [(K, t, s) for s in seeds for t in types]
    with ThreadPoolExecutor(4) as ex: res = list(ex.map(job, jobs))
    with open(log, "w") as f: f.write("".join(res))
    ok = sum("GELOEST" in r for r in res)
    print(f"Stufe K={K} ({4*K} Woerter): {ok} von {len(jobs)} Laeufen geloest, {time.time()-t0:.0f}s", flush=True)
    if ok == 0 or K <= 10 or any("uebersprungen" in r for r in res): break
