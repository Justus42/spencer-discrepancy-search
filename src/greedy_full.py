"""Randomised greedy until everything is covered: how many complementary pairs does plain greedy need?"""
import sys, time, numpy as np
from search_lns import Search, to_matrix
m, R = int(sys.argv[1]), int(sys.argv[2]); seeds = int(sys.argv[3]); out = sys.argv[4] if len(sys.argv) > 4 else None
best = None
for s in range(seeds):
    t0 = time.time(); S = Search(m, R, s); cnt = np.zeros(S.N, dtype=np.int16); C = []; masks = []
    while (cnt == 0).any():
        S.greedy_fill(cnt, C, masks, 1)
    print(f"m={m} R={R} seed={s}: greedy covers everything with {len(C)} pairs = {2*len(C)} words ({time.time()-t0:.0f}s)", flush=True)
    if best is None or len(C) < len(best): best = list(C)
if out:
    with open(out, "w") as f:
        for r in [[-1 if (c >> j) & 1 else 1 for j in range(m)] for c in best]:
            f.write(" ".join(f"{v:2d}" for v in r) + "\n")
