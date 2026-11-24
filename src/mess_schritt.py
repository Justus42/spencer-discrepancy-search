"""Machbarkeit (keine Suche): baut H wie coset_ladder.build und startet den Kern im Messmodus
(build/coset_tabu_mess: -DMESS=200; build/coset_tabu_ueberlauf: zusaetzlich -DUEBERLAUF).
Aufruf: mess_schritt.py binary n R d K seed typ   (typ z. B. 13,12)"""
import sys, os, subprocess, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from coset_ladder import build
binary, n, R, d, K, seed, typ = sys.argv[1], *map(int, sys.argv[2:7]), tuple(map(int, sys.argv[7].split(",")))
G, Hc, m, hp, rr = build(n, typ, d)
with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
    f.write(" ".join(map(str, Hc))); fn = f.name
p = subprocess.run([binary, str(m), str(n), str(R), str(K), str(seed), "100000", fn], capture_output=True, text=True)
os.unlink(fn)
print(f"typ={typ} seed={seed} binary={os.path.basename(binary)}")
print(p.stderr.strip()); print(p.stdout.strip(), flush=True)
