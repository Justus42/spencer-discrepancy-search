#!/usr/bin/env python3
"""Treiber (Auftrag H, Schritt 2): sucht Stufengewichte lam fuer die Zaehlschranke von build/zerl_halb zaehl.
f(lam) = N0*NZ - K*B ist konkav und homogen; gesucht max f > 0 (dann kein G). Koordinatensuche ueber ganze Zahlen.
Aufruf: python3 src/zerl_gewichte.py FEST d mg SEKUNDEN"""
import random, re, subprocess, sys, time
fest, d, mg, sek = sys.argv[1], sys.argv[2], int(sys.argv[3]), float(sys.argv[4])
L = mg // 2 + 1 if mg % 2 == 0 else (mg - 1) // 2 + 1
def f(lam):
    p = subprocess.run(["build/zerl_halb", "zaehl", fest, d, str(mg)] + [str(x) for x in lam], capture_output=True, text=True)
    m = re.search(r"N0\*NZ = (\d+)\s+gegen\s+K\*B = (\d+)", p.stdout); a, b = int(m.group(1)), int(m.group(2))
    return a / b, a, b, p.stdout
lam = [0, 1, 3, 6] + [0] * (L - 1 - 4)
best = f(lam); t0 = time.time(); rnd = random.Random(1); n = 1
print("Start", lam, f"Verhaeltnis {best[0]:.4f}", flush=True)
while time.time() - t0 < sek:
    neu = lam[:]
    for _ in range(rnd.choice([1, 1, 2])):
        j = rnd.randrange(len(neu)); neu[j] = max(0, neu[j] + rnd.choice([-3, -2, -1, 1, 2, 3]))
    if sum(neu) == 0: continue
    r = f(neu); n += 1
    if r[0] > best[0]:
        lam, best = neu, r
        print(f"{time.time()-t0:6.1f} s  lam {lam}  Verhaeltnis {best[0]:.4f}  ({best[1]} gegen {best[2]})", flush=True)
print(f"Ende nach {n} Auswertungen: lam {lam}, Verhaeltnis {best[0]:.4f}; {'KEINE LOESUNG (Zaehlschranke)' if best[0] > 1 else 'nicht entschieden'}")
print(best[3])
