# Unabhaengiger Versuch, Teil 2 (keine Ueberdeckung mit <= 23 Zeilen aus Bahnen der Groesse 1 und 3) mit CP-SAT zu beweisen.
import numpy as np, sys, time
from ortools.sat.python import cp_model
C = np.load('/home/claude/orch/C_small.npy'); ssize = np.load('/home/claude/orch/ssize.npy')
cols = np.unique(C, axis=1).T                       # verschiedene Klauseln
masks = sorted({int(''.join('1' if b else '0' for b in c), 2) for c in cols}, key=lambda v: bin(v).count('1'))
kept = []
for mk in masks:                                     # nur minimale Klauseln behalten
    if not any(k & mk == k for k in kept): kept.append(mk)
print("Klauseln:", C.shape[1], "verschieden", len(masks), "minimal", len(kept), "kleinste", bin(kept[0]).count('1'), flush=True)
L = C.shape[0]
m = cp_model.CpModel(); v = [m.NewBoolVar(str(i)) for i in range(L)]
for mk in kept:
    m.AddBoolOr([v[i] for i in range(L) if (mk >> (L - 1 - i)) & 1])
m.Add(sum(int(s) * x for s, x in zip(ssize, v)) <= int(sys.argv[1]))
s = cp_model.CpSolver(); s.parameters.max_time_in_seconds = float(sys.argv[2]); s.parameters.num_workers = 1
t = time.time(); st = s.Solve(m); print("hoechstens", sys.argv[1], "Zeilen:", s.StatusName(st), f"{time.time()-t:.0f}s")
