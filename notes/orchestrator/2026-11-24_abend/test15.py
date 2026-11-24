import sys, time, numpy as np
sys.path.insert(0, '/home/claude/orch')
from sym_enum import *
n, R = 15, 4
# p = 7, Zyklentyp 7+7+1
gfull, gfix, Cf, Cx, reps, sig = run(n, R, 7, 2, "p7 7+7+1")
for b in (2, 1, 0):
    t = time.time()
    sols, cnt = pairs_with_fixed(Cf, Cx, b)
    print(f"  2 volle Bahnen + {b} feste Zeilen = {2*(14+b)} Woerter: {len(sols)} Loesungen, je Festmenge {cnt}  ({time.time()-t:.1f}s)", flush=True)
    if b == 2 and sols:
        S, i, j = sols[0]
        M = rows_of(n, 7, sig, gfull[i], True) + rows_of(n, 7, sig, gfull[j], True) + [rows_of(n, 7, sig, gfix[s], False)[0] for s in S]
        open('/home/claude/orch/z7_kontrolle_16x15.txt', 'w').write('\n'.join(' '.join(f'{v:2d}' for v in r) for r in M) + '\n')
    if b == 1 and sols:
        print("  TREFFER 30 Woerter:", sols[:5])
# p = 13, 13+1+1: 1 Bahn + 2 feste Zeilen (Kontrolle: +3, +4)
gfull, gfix, Cf, Cx, reps, sig = run(n, R, 13, 1, "p13 13+1+1")
for b in (2, 3, 4):
    print(f"  1 volle Bahn + {b} feste = {2*(13+b)} Woerter: {len(one_with_fixed(Cf, Cx, b))} Loesungen")
# p = 11, 11+4: 1 Bahn + 4 feste Zeilen (Kontrolle: +5, +6)
gfull, gfix, Cf, Cx, reps, sig = run(n, R, 11, 1, "p11 11+1^4")
for b in (4, 5, 6):
    print(f"  1 volle Bahn + {b} feste = {2*(11+b)} Woerter: {len(one_with_fixed(Cf, Cx, b))} Loesungen")
