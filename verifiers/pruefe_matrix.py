#!/usr/bin/env python3
"""Wrapper: Regel 1 (zwei unabhaengige Pruefskripte, gleiche Diskrepanz) in einem Aufruf.

Prueft nur; er rechnet die Diskrepanz nie selbst. Er teilt keinen Code mit den
Pruefprogrammen, sondern startet sie als getrennte Prozesse und vergleicht ihre Ausgaben.

Modus 1 (Standard):  pruefe_matrix.py MATRIX [--erwartet D] [--zeitlimit S]
    MATRIX: Leerraum-getrennte ganze Zahlen, beliebig rechteckig, eine Zeile je Textzeile.
    1. Der Wrapper schreibt die mit Nullen quadratisch aufgefuellte Fassung in einen
       temporaeren Ordner (Nullspalten bei mehr Zeilen als Spalten, sonst Nullzeilen).
    2. Getrennte Prozesse, gleichzeitig: src/check_disc.py auf dem Original,
       verifiers/verify_spencer.py auf der aufgefuellten Fassung, build/exakt_disc
       auf dem Original (nur wenn gebaut; Pfad ueberschreibbar mit EXAKT_DISC).
    3. Die Werte werden verglichen. Bei Abweichung Fehlercode.

Modus 2:  pruefe_matrix.py --paar ORIGINAL PAD
    Prueft, dass PAD genau das Original plus angehaengte Nullspalten und/oder Nullzeilen ist.
    Rechnet keine Diskrepanz. Nichts wird repariert.

Modus 3:  pruefe_matrix.py --alle-pads VERZEICHNIS...
    Sucht rekursiv alle *_pad.txt, ordnet je das Original (gleicher Name ohne _pad) zu und
    wendet Modus 2 an. Nur Bericht, nichts wird veraendert.

Fehlercodes: 0 alles gleich / alles in Ordnung;  1 Abweichung;
             2 Eingabe- oder Aufruffehler;  3 kein Wert (z. B. Zeitlimit, alle Programme ohne Ergebnis).
"""
from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
import tempfile
import time
from fractions import Fraction
from pathlib import Path

WURZEL = Path(__file__).resolve().parent.parent
CHECK = WURZEL / "src" / "check_disc.py"
VERIFY = WURZEL / "verifiers" / "verify_spencer.py"
EXAKT_STANDARD = WURZEL / "build" / "exakt_disc"
EXAKT_MAX_ZEILEN, EXAKT_MAX_SPALTEN = 64, 40  # Grenzen von src/exakt_disc.c


class Eingabefehler(Exception):
    pass


def lies_matrix(pfad: Path) -> list[list[int]]:
    """Strenge Eingabe: nur ganze Zahlen, alle Zeilen gleich lang, nicht leer."""
    try:
        text = pfad.read_text(encoding="utf-8")
    except OSError as e:
        raise Eingabefehler(f"{pfad}: {e}")
    zeilen: list[list[int]] = []
    for nr, roh in enumerate(text.splitlines(), 1):
        roh = roh.strip()
        if not roh:
            continue
        try:
            zeilen.append([int(tok) for tok in roh.split()])
        except ValueError:
            raise Eingabefehler(f"{pfad}: Zeile {nr} enthaelt keine ganzen Zahlen: {roh[:60]!r}")
        if any(abs(v) > 1 for v in zeilen[-1]):
            raise Eingabefehler(f"{pfad}: Zeile {nr} hat einen Eintrag ausserhalb von {{-1,0,1}}")
    if not zeilen:
        raise Eingabefehler(f"{pfad}: keine Matrixzeilen")
    breite = len(zeilen[0])
    for i, z in enumerate(zeilen):
        if len(z) != breite:
            raise Eingabefehler(f"{pfad}: Zeile {i + 1} hat {len(z)} statt {breite} Eintraege")
    return zeilen


def auffuellen(m: list[list[int]]) -> list[list[int]]:
    """Quadratisch mit Nullen: Nullspalten rechts bei mehr Zeilen, Nullzeilen unten bei mehr Spalten."""
    r, c = len(m), len(m[0])
    n = max(r, c)
    return [z + [0] * (n - c) for z in m] + [[0] * n for _ in range(n - r)]


def schreibe(m: list[list[int]], pfad: Path) -> None:
    pfad.write_text("".join(" ".join(map(str, z)) + "\n" for z in m), encoding="utf-8")


# ---------------------------------------------------------------- Modus 2

def pruefe_paar(original: Path, pad: Path) -> tuple[bool, str]:
    """(in Ordnung, Text). PAD muss Original plus angehaengte Nullspalten/-zeilen sein."""
    try:
        o, p = lies_matrix(original), lies_matrix(pad)
    except Eingabefehler as e:
        return False, f"Eingabefehler: {e}"
    ro, co, rp, cp = len(o), len(o[0]), len(p), len(p[0])
    if rp < ro or cp < co:
        return False, f"PAD ({rp} x {cp}) ist kleiner als das Original ({ro} x {co})"
    for i in range(ro):
        if p[i][:co] != o[i]:
            j = next(j for j in range(co) if p[i][j] != o[i][j])
            return False, (f"Original-Block stimmt nicht: Zeile {i}, Spalte {j}: "
                           f"Original {o[i][j]}, PAD {p[i][j]}")
    for i in range(rp):
        for j in range(cp):
            if (i >= ro or j >= co) and p[i][j] != 0:
                return False, f"Eintrag ausserhalb des Original-Blocks ist nicht 0: Zeile {i}, Spalte {j}: {p[i][j]}"
    zusatz = f"; PAD {rp} x {cp}, Original {ro} x {co}, +{cp - co} Nullspalten, +{rp - ro} Nullzeilen"
    if rp != cp:
        zusatz += " (PAD nicht quadratisch)"
    return True, "PAD = Original + Nullen" + zusatz


# ---------------------------------------------------------------- Modus 1

def starte(cmd: list[str], zeitlimit: float) -> dict:
    t0 = time.monotonic()
    try:
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    except OSError as e:
        return {"p": None, "t0": t0, "fehler": str(e)}
    return {"p": p, "t0": t0, "limit": t0 + zeitlimit}


def warte(job: dict) -> dict:
    if job.get("p") is None:
        return {"status": "fehler", "text": job["fehler"], "out": "", "err": "", "sek": 0.0}
    p = job["p"]
    try:
        out, err = p.communicate(timeout=max(0.1, job["limit"] - time.monotonic()))
    except subprocess.TimeoutExpired:
        p.kill()
        out, err = p.communicate()
        return {"status": "zeit", "text": "Zeitlimit", "out": out, "err": err, "sek": time.monotonic() - job["t0"]}
    return {"status": "ende", "rc": p.returncode, "out": out, "err": err, "sek": time.monotonic() - job["t0"]}


def werte_aus(name: str, res: dict, erwartete_form: tuple[int, int]) -> dict:
    """Macht aus dem Prozessergebnis {'wert': Fraction} oder {'kein_wert': Grund}."""
    if res["status"] != "ende":
        return {"kein_wert": res["text"], "sek": res["sek"], "art": "zeit" if res["status"] == "zeit" else "fehler"}
    out, rc = res["out"], res["rc"]
    if name == "check_disc":
        w = re.search(r"exact discrepancy = (-?\d+)\s*$", out, re.M)
        f = re.search(r"matrix (\d+) x (\d+),", out)
    elif name == "verify_spencer":
        w = re.search(r"Exakte Diskrepanz:\s*(-?\d+(?:/\d+)?)\s*$", out, re.M)
        f = re.search(r"Dimension\s*:\s*(\d+) x (\d+)", out)
    else:  # exakt_disc
        w = re.search(r": disc (-?\d+),", out)
        f = re.search(r"matrix (\d+) x (\d+)", out)
    if rc != 0 or not w:
        letzte = (res["err"].strip().splitlines() or ["(keine Meldung)"])[-1]
        return {"kein_wert": f"abgelehnt/fehlgeschlagen (Rueckgabecode {rc}): {letzte[:150]}",
                "sek": res["sek"], "art": "abgelehnt"}
    form = (int(f.group(1)), int(f.group(2))) if f else None
    return {"wert": Fraction(w.group(1)), "form": form, "sek": res["sek"],
            "soll_form": erwartete_form}


def modus_matrix(pfad: Path, erwartet: Fraction | None, zeitlimit: float, ruhig: bool) -> int:
    m = lies_matrix(pfad)
    r, c = len(m), len(m[0])
    n = max(r, c)
    exakt = Path(os.environ.get("EXAKT_DISC", EXAKT_STANDARD))
    exakt_ok = exakt.is_file() and os.access(exakt, os.X_OK)
    exakt_grund = None
    if not exakt_ok:
        exakt_grund = f"nicht gebaut ({exakt}); bauen: gcc -O3 -march=native -o build/exakt_disc src/exakt_disc.c"
    elif r > EXAKT_MAX_ZEILEN or c > EXAKT_MAX_SPALTEN:
        exakt_ok, exakt_grund = False, f"uebersprungen, {r} x {c} liegt ueber den Grenzen {EXAKT_MAX_ZEILEN} x {EXAKT_MAX_SPALTEN}"

    with tempfile.TemporaryDirectory(prefix="pruefe_matrix_") as tmp:
        pad = Path(tmp) / "aufgefuellt.txt"
        schreibe(auffuellen(m), pad)
        ok, text = pruefe_paar(pfad, pad)  # Selbstkontrolle des eigenen Auffuellens
        if not ok:
            raise Eingabefehler(f"interner Fehler beim Auffuellen: {text}")
        py = sys.executable
        jobs = {
            "check_disc": (starte([py, "-I", str(CHECK), str(pfad)], zeitlimit), (r, c)),
            "verify_spencer": (starte([py, "-I", str(VERIFY), str(pad)], zeitlimit), (n, n)),
        }
        if exakt_ok:
            jobs["exakt_disc"] = (starte([str(exakt), str(pfad)], zeitlimit), (r, c))
        erg = {name: werte_aus(name, warte(job), form) for name, (job, form) in jobs.items()}

    if not ruhig:
        print(f"Matrix {pfad}: {r} x {c}, aufgefuellt {n} x {n}")
    gleich_form = True
    for name, e in erg.items():
        if "wert" in e:
            if e["form"] is not None and e["form"] != e["soll_form"]:
                gleich_form = False
                zusatz = f"  FORM ABWEICHEND: gemeldet {e['form']}, erwartet {e['soll_form']}"
            else:
                zusatz = ""
            print(f"  {name:15s} Diskrepanz {e['wert']}  ({e['sek']:.1f} s){zusatz}")
        else:
            print(f"  {name:15s} KEIN WERT: {e['kein_wert']}  ({e['sek']:.1f} s)")
    if exakt_grund:
        print(f"  {'exakt_disc':15s} nicht gerechnet: {exakt_grund}")

    werte = {n_: e["wert"] for n_, e in erg.items() if "wert" in e}
    ohne = {n_: e for n_, e in erg.items() if "wert" not in e}
    if not werte:
        print("ERGEBNIS: kein Programm hat einen Wert geliefert (Fehlercode 3)")
        return 3
    if ohne:
        print("ABWEICHUNG: " + ", ".join(ohne) + " ohne Wert, andere rechnen (Fehlercode 1)")
        return 1
    if len(set(werte.values())) != 1:
        print("ABWEICHUNG: Werte verschieden: " + ", ".join(f"{k}={v}" for k, v in werte.items()) + " (Fehlercode 1)")
        return 1
    if not gleich_form:
        print("ABWEICHUNG: ein Programm meldet eine andere Matrixform (Fehlercode 1)")
        return 1
    d = next(iter(werte.values()))
    if len(werte) < 2:
        print("ABWEICHUNG: nur ein Wert, kein Vergleich moeglich (Fehlercode 1)")
        return 1
    if erwartet is not None and d != erwartet:
        print(f"ABWEICHUNG vom erwarteten Wert: gerechnet {d}, erwartet {erwartet} (Fehlercode 1)")
        return 1
    print(f"ERGEBNIS: alle {len(werte)} Programme melden exakt dieselbe Diskrepanz {d}"
          + (f" (= erwartet)" if erwartet is not None else ""))
    return 0


# ---------------------------------------------------------------- Modus 3

def modus_alle_pads(verzeichnisse: list[Path]) -> int:
    pads = sorted(p for v in verzeichnisse for p in v.rglob("*_pad.txt"))
    if not pads:
        print("keine *_pad.txt gefunden")
        return 3
    n_ok, abw = 0, []
    for pad in pads:
        orig = pad.with_name(pad.name[: -len("_pad.txt")] + ".txt")
        if not orig.is_file():
            abw.append((pad, "Original fehlt: " + orig.name))
            continue
        ok, text = pruefe_paar(orig, pad)
        if ok:
            n_ok += 1
        else:
            abw.append((pad, text))
    print(f"{len(pads)} _pad-Dateien geprueft: {n_ok} in Ordnung, {len(abw)} Abweichungen")
    for pad, text in abw:
        print(f"ABWEICHUNG {pad}: {text}")
    return 1 if abw else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("dateien", nargs="*", type=Path)
    ap.add_argument("--paar", action="store_true", help="Modus 2: ORIGINAL PAD")
    ap.add_argument("--alle-pads", action="store_true", help="Modus 3: alle *_pad.txt unter den Verzeichnissen")
    ap.add_argument("--erwartet", type=str, default=None, help="erwartete Diskrepanz (Modus 1)")
    ap.add_argument("--zeitlimit", type=float, default=240.0, help="Sekunden je Programm (Standard 240)")
    a = ap.parse_args()
    try:
        if a.alle_pads:
            if not a.dateien:
                raise Eingabefehler("--alle-pads braucht mindestens ein Verzeichnis")
            return modus_alle_pads(a.dateien)
        if a.paar:
            if len(a.dateien) != 2:
                raise Eingabefehler("--paar braucht genau ORIGINAL und PAD")
            ok, text = pruefe_paar(*a.dateien)
            print(("OK: " if ok else "ABWEICHUNG: ") + text)
            return 0 if ok else 1
        if len(a.dateien) != 1:
            raise Eingabefehler("genau eine Matrixdatei angeben")
        erwartet = Fraction(a.erwartet) if a.erwartet is not None else None
        return modus_matrix(a.dateien[0], erwartet, a.zeitlimit, ruhig=False)
    except Eingabefehler as e:
        print(f"EINGABEFEHLER: {e}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
