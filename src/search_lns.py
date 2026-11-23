"""Self-complementary covering codes, version 2: works on the quotient cube (points up to complement).

A point is a vector y in {0,1}^(m-1) standing for the pair {(0,y), complement}. A centre c covers y
iff weight(y xor c) <= R or >= m - R. Gains for all candidate centres at once via an exact
integer Walsh-Hadamard transform. Search = randomised greedy + destroy/repair.
"""
import sys, time
import numpy as np


def fwht_inplace(a):
    n = a.size
    h = 1
    while h < n:
        v = a.reshape(-1, 2, h)
        x = v[:, 0, :].copy()
        v[:, 0, :] += v[:, 1, :]
        v[:, 1, :] = x - v[:, 1, :]
        h *= 2
    return a


class Search:
    def __init__(self, m, R, seed=0):
        self.m, self.R = m, R
        self.rng = np.random.default_rng(seed)
        self.N = 1 << (m - 1)
        self.X = np.arange(self.N, dtype=np.uint32)
        w = np.bitwise_count(self.X)
        self.Bh = fwht_inplace(((w <= R) | (w >= m - R)).astype(np.int64))

    def ball(self, c):
        d = np.bitwise_count(self.X ^ np.uint32(c))
        return (d <= self.R) | (d >= self.m - self.R)

    def gains(self, U):
        g = fwht_inplace(U.astype(np.int64))
        g *= self.Bh
        fwht_inplace(g)
        g //= self.N
        return g                                           # uncovered quotient points each candidate would cover

    def choose(self, g, slack=0.0):
        mx = int(g.max())
        cand = np.flatnonzero(g >= mx - int(slack * mx))
        return int(cand[self.rng.integers(len(cand))])

    def greedy_fill(self, cnt, C, masks, k, slack=0.0):
        for _ in range(k):
            c = self.choose(self.gains(cnt == 0), slack)
            mk = self.ball(c)
            C.append(c); masks.append(mk); cnt += mk

    def run(self, pairs, time_limit=120.0, destroy=3, slack=0.02, verbose=True):
        t0 = time.time()
        cnt = np.zeros(self.N, dtype=np.int16)
        C, masks = [], []
        self.greedy_fill(cnt, C, masks, pairs)
        cost = int((cnt == 0).sum()); greedy_cost = cost
        it = 0
        while cost > 0 and time.time() - t0 < time_limit:
            it += 1
            k = int(self.rng.integers(1, destroy + 1))
            idx = sorted(self.rng.choice(pairs, size=k, replace=False), reverse=True)
            saved = [(i, C[i], masks[i]) for i in idx]
            for i in idx:
                cnt -= masks[i]; del C[i]; del masks[i]
            self.greedy_fill(cnt, C, masks, k, slack)
            new = int((cnt == 0).sum())
            if new <= cost:
                cost = new
            else:                                           # undo
                for _ in range(k):
                    cnt -= masks[-1]; C.pop(); masks.pop()
                for i, c, mk in reversed(saved):
                    C.insert(i, c); masks.insert(i, mk); cnt += mk
        if verbose:
            tot = 2 * self.N
            print(f"m={self.m} R={self.R} words={2 * pairs}: uncovered after greedy {2 * greedy_cost} "
                  f"({100 * greedy_cost / self.N:.2f}%), after {it} repair rounds {2 * cost} "
                  f"({100 * cost / self.N:.3f}%) of {tot}; {time.time() - t0:.0f}s", flush=True)
        return cost, C


def to_matrix(m, C):
    rows = [[-1 if (c >> j) & 1 else 1 for j in range(m)] for c in C]
    n = max(m, len(rows))
    rows = [r + [0] * (n - m) for r in rows]
    while len(rows) < n:
        rows.append(rows[0])
    return rows


if __name__ == "__main__":
    m, R, pairs = map(int, sys.argv[1:4])
    tl = float(sys.argv[4]) if len(sys.argv) > 4 else 60
    seed = int(sys.argv[5]) if len(sys.argv) > 5 else 0
    out = sys.argv[6] if len(sys.argv) > 6 else None
    cost, C = Search(m, R, seed).run(pairs, time_limit=tl)
    if cost == 0:
        print("  FOUND centres:", C)
        if out:
            with open(out, "w") as f:
                for r in to_matrix(m, C):
                    f.write(" ".join(f"{v:2d}" for v in r) + "\n")
            print("  matrix written to", out)
