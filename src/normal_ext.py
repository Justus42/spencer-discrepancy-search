"""Normalitaet und Verlaengerung durch Verdreifachen einer Koordinate (Graham-Sloane).
Ein Code C (Laenge n, Radius R) ist normal bzgl. Koordinate i, wenn fuer alle x
d(x,C0)+d(x,C1) <= 2R+1 (C0/C1: Woerter mit x_i=0/1, Abstand im ganzen Raum). Norm = Maximum dieser Summe.
Dann ist der Code mit Koordinate i dreifach (Laenge n+2) ein Code vom Radius R+1 mit gleicher Wortzahl.
Aufruf: normal_ext.py n R woerter.txt praefix_ausgabe   (schreibt Laengen n+2, n+4, n+6)
Alles per Brute-Force ueber alle 2^n Punkte; nur ganze Zahlen."""
import sys
import numpy as np

def dist_to(n, W):
    X = np.arange(1 << n, dtype=np.uint32); dm = np.full(1 << n, 255, dtype=np.uint8)
    for w in W: np.minimum(dm, np.bitwise_count(X ^ np.uint32(w)).astype(np.uint8), out=dm)
    return dm

def radius(n, W): return int(dist_to(n, W).max())

def norm(n, W, i):
    C0 = [w for w in W if not (w >> i) & 1]; C1 = [w for w in W if (w >> i) & 1]
    return int((dist_to(n, C0).astype(np.int16) + dist_to(n, C1)).max())

def triple(n, W, i):
    """Koordinate i durch drei Kopien ersetzen: Bit i wird zu Bits i, n, n+1 (neue Laenge n+2)."""
    out = []
    for w in W:
        b = (w >> i) & 1
        out.append(w | (b << n) | (b << (n + 1)))
    return out

if __name__ == "__main__":
    n, R = int(sys.argv[1]), int(sys.argv[2]); W = list(map(int, open(sys.argv[3]).read().split()))
    pre = sys.argv[4]
    assert len(set(W)) == len(W)
    r = radius(n, W); print(f"n={n} Woerter={len(W)} Radius={r}", flush=True); assert r == R
    norms = [norm(n, W, i) for i in range(n)]
    print(f"Norm je Koordinate: {norms}  normal (alle <= {2*R+1}): {all(x <= 2*R+1 for x in norms)}", flush=True)
    i = 0; cur, cn, cR = W, n, R
    for step in range(3):
        if not norm(cn, cur, i) <= 2 * cR + 1: print(f"Koordinate {i} bei n={cn} nicht normal, Stopp"); break
        cur = triple(cn, cur, i); cn += 2; cR += 1
        r = radius(cn, cur); full = (1 << cn) - 1; S = set(cur)
        print(f"n={cn} Woerter={len(S)} Radius(Brute-Force)={r} (Soll {cR}) selbstkomplementaer={all(w ^ full in S for w in S)}", flush=True)
        assert r == cR and len(S) == len(W)
        open(f"{pre}_n{cn}_R{cR}.txt", "w").write(" ".join(map(str, sorted(cur))) + "\n")
