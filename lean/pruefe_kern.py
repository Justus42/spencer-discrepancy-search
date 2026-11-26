#!/usr/bin/env python3
"""Teil 5: prüft eine Kern-Stufe (decide +kernel), höchstens P Lean-Prozesse gleichzeitig (Speicher, siehe erzeuge_kern.py).

Aufruf aus lean/:  python3 -I pruefe_kern.py NAME [P]
NAME = Hamming7 | Ctrl16 | Had24_1p3_ohne0 | Had24_1p3 | Had24_2p2 | C18_5_50w | C18_5_50w_ohne24 | C18_5_52w.  Voraussetzung: `lean`, `lake` im PATH.
Reihenfolge: Basis und Daten über lake, dann Kern/Swar<n>/Q, Kern/<NAME>/Q, alle Teile parallel, dann Swar<n>, dann Kern/<NAME>.
Ein schon gebautes Modul (olean neuer als seine Quelle und als Basis.olean) wird übersprungen.
Ausgabe je Modul mit Zeit; Ende mit Fehlercode 1 beim ersten Fehler.
"""
import concurrent.futures as cf
import glob
import os
import subprocess
import sys
import time

OUT = ".lake/build/lib/lean"


def baue(mod):
    src = mod + ".lean"
    olean = os.path.join(OUT, mod + ".olean")
    basis = os.path.join(OUT, "Ueberdeckung/Basis.olean")
    if os.path.exists(olean) and os.path.getmtime(olean) > os.path.getmtime(src) \
            and os.path.getmtime(olean) > os.path.getmtime(basis):
        return mod, True, 0.0, "übersprungen (schon gebaut)"
    os.makedirs(os.path.dirname(olean), exist_ok=True)
    t = time.time()
    r = subprocess.run(["lake", "env", "lean", src, "-o", olean], capture_output=True, text=True)
    dt = time.time() - t
    if r.returncode != 0:
        if os.path.exists(olean):
            os.remove(olean)
        return mod, False, dt, "Exit %d\n%s%s" % (r.returncode, r.stdout[-3000:], r.stderr[-3000:])
    return mod, True, dt, (r.stdout + r.stderr).strip()[-500:]


def schicht(mods, P):
    with cf.ThreadPoolExecutor(P) as ex:
        for mod, ok, dt, text in ex.map(baue, mods):
            print("  %s %-28s %7.1f s %s" % ("ok    " if ok else "FEHLER", mod, dt, text if (text or not ok) else ""), flush=True)
            if not ok:
                ex.shutdown(cancel_futures=True)
                sys.exit(1)


def main():
    name = sys.argv[1]
    P = int(sys.argv[2]) if len(sys.argv) > 2 else 4
    n = {"Hamming7": 7, "Ctrl16": 15}.get(name, 18 if name.startswith("C18_5_") else 20)
    t0 = time.time()
    print("Stufe %s, n = %d, P = %d, Start %s UTC" % (name, n, P, time.strftime("%H:%M:%S", time.gmtime())), flush=True)
    r = subprocess.run(["lake", "build", "Ueberdeckung.Basis", "Ueberdeckung.Daten." + name], capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stdout, r.stderr); sys.exit(1)
    hat_bloecke = os.path.isdir("Kern/" + name)
    qs = ["Kern/Swar%d/Q" % n] + (["Kern/%s/Q" % name] if hat_bloecke else [])
    teile = sorted(glob.glob("Kern/Swar%d/Teil*.lean" % n)) + (sorted(glob.glob("Kern/%s/Teil*.lean" % name)) if hat_bloecke else [])
    teile = sorted((t[:-5] for t in teile), key=lambda m: (m.split("/")[1], int(m.split("Teil")[1])))
    schicht(qs if hat_bloecke else [], P)
    if hat_bloecke:
        schicht(teile, P)
        schicht(["Kern/Swar%d" % n], P)
    schicht(["Kern/" + name], P)
    print("Stufe %s fertig, %.1f s Wand, Ende %s UTC" % (name, time.time() - t0, time.strftime("%H:%M:%S", time.gmtime())), flush=True)


if __name__ == "__main__":
    main()
