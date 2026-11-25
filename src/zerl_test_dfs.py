#!/usr/bin/env python3
"""Test der Tiefensuche von build/zerl_halb (Auftrag H, 25.11.2026): kleine Zufallsfaelle, Vergleich
'dfs' (mit Symmetriebrechung und Schranke) gegen 'brute' (alle Belegungen, keine Symmetrie).
Jede gemeldete Loesung wird zusaetzlich mit src/zerl_offen.py (Pruefcode) nachgezaehlt.
Aufruf: python3 src/zerl_test_dfs.py ANZAHL [SEED] [ZUSATZ...]   (ZUSATZ wird an 'dfs' angehaengt, z. B. --lam 1,2,3)"""
import os
import random
import subprocess
import sys
import tempfile

ZH = "build/zerl_halb"


def lauf(args):
    p = subprocess.run([ZH] + args, capture_output=True, text=True, timeout=600)
    zeile = [l for l in p.stdout.splitlines() if l and not l.startswith("#")]
    return p.returncode, (zeile[-1] if zeile else "")


def main():
    n = int(sys.argv[1])
    rnd = random.Random(int(sys.argv[2]) if len(sys.argv) > 2 else 1)
    tmp = tempfile.mkdtemp(prefix="zerl_test_", dir=os.environ.get("TMPDIR_TEST"))
    stat = {"beide_ja": 0, "beide_nein": 0, "abweichung": 0}
    for t in range(n):
        K = rnd.choice([2, 3, 4])
        mf = rnd.choice([3, 4, 5, 6, 7])
        mg = rnd.choice([3, 4, 5]) if K == 4 else rnd.choice([3, 4, 5, 6])
        d = rnd.randint(2, mf + mg - 1)
        F = [[rnd.choice((1, -1)) for _ in range(mf)] for _ in range(K)]
        f = os.path.join(tmp, f"F{t}.txt")
        with open(f, "w") as h:
            for z in F:
                h.write(" ".join(str(v) for v in z) + "\n")
        aus = os.path.join(tmp, f"P{t}.txt")
        rd, ld = lauf(["dfs", f, str(d), str(mg), "--zeit", "60", "--aus", aus] + sys.argv[3:])
        rb, lb = lauf(["brute", f, str(d), str(mg)])
        if rd not in (0, 3) or rb not in (0, 3):
            print(f"Fall {t}: Fehler rd={rd} rb={rb} ({ld} / {lb})")
            stat["abweichung"] += 1
            continue
        if (rd == 0) != (rb == 0):
            print(f"Fall {t}: ABWEICHUNG K={K} mf={mf} mg={mg} d={d}: dfs {ld} / brute {lb}  ({f})")
            stat["abweichung"] += 1
            continue
        if rd == 0:
            q = subprocess.run([sys.executable, "src/zerl_offen.py", "offen", aus, str(d)], capture_output=True, text=True)
            if "offene Klassen 0 " not in q.stdout:
                print(f"Fall {t}: dfs-Loesung nicht bestaetigt: {q.stdout.strip()}")
                stat["abweichung"] += 1
                continue
            stat["beide_ja"] += 1
        else:
            stat["beide_nein"] += 1
    print(f"{n} Faelle: {stat}")
    sys.exit(1 if stat["abweichung"] else 0)


if __name__ == "__main__":
    main()
