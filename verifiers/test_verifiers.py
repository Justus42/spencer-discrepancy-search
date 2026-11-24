#!/usr/bin/env python3
"""Testsuite fuer die Pruefkette (check_disc.py, verify_spencer.py, exakt_disc, pruefe_matrix.py).

Aufruf aus dem Repo-Wurzelordner:  python3 verifiers/test_verifiers.py      (Laufzeit < 5 Min.)
Nur Standardbibliothek plus numpy (das die Pruefskripte selbst brauchen). Rechnet nichts Neues
ausser der unabhaengigen Bruch-Gegenprobe in c). Aendert keines der drei geschuetzten Programme.

a) Soll-Diskrepanzen aller Kontrollen ueber den Wrapper (Abweichung von STATUS.md wird gemeldet).
b) Defektdateien in verifiers/defekt/ (aus controls/ctrl_8.txt abgeleitet): jedes Programm lehnt ab.
c) Bekannte Unterschiede der Programme als erwartetes Verhalten festgehalten, nicht behoben:
   mehr Zeilen als Spalten (verify_spencer lehnt ab, check_disc rechnet),
   gebrochene Eintraege (check_disc lehnt ab, verify_spencer rechnet).
   Aendert sich eines davon, schlaegt der Test an: das ist gewollt.
d) Wrapper-Modus --paar und --alle-pads, sowie SHA256SUMS der drei geschuetzten Dateien.
"""
from __future__ import annotations

import hashlib
import itertools
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
from fractions import Fraction
from pathlib import Path

WURZEL = Path(__file__).resolve().parent.parent
CHECK = WURZEL / "src" / "check_disc.py"
VERIFY = WURZEL / "verifiers" / "verify_spencer.py"
WRAPPER = WURZEL / "verifiers" / "pruefe_matrix.py"
DEFEKT = WURZEL / "verifiers" / "defekt"
CTRL = WURZEL / "controls"
EXAKT = Path(os.environ.get("EXAKT_DISC", WURZEL / "build" / "exakt_disc"))

# a) Soll-Werte laut STATUS.md (Abschnitt "Gesichert" und Hadamard-Abschnitte)
SOLL = [
    ("ctrl_8", 5), ("ctrl_9", 5), ("ctrl_16", 7), ("ctrl_17", 7), ("L2026_17x17", 7), ("astra_19", 7),
    ("had20_klasse1", 6), ("had20_klasse2", 6), ("had20_klasse3", 6),
    ("had24_1p3_paley12", 8), ("had24_2p2_paley12", 8), ("bahn15_40w", 7),
]


def lauf(cmd, zeit=120):
    p = subprocess.run([str(c) for c in cmd], capture_output=True, text=True, timeout=zeit)
    return p.returncode, p.stdout, p.stderr


def py(skript, *args):
    return lauf([sys.executable, "-I", skript, *args])


def hat_ergebnis(out: str) -> bool:
    """Gibt es in der Ausgabe irgendeine Diskrepanz-Zeile (egal von welchem Programm)?"""
    return bool(re.search(r"exact discrepancy =|Exakte Diskrepanz:|: disc -?\d+,", out))


def exakt_bereit() -> bool:
    global EXAKT
    if EXAKT.is_file() and os.access(EXAKT, os.X_OK):
        return True
    if shutil.which("gcc") and EXAKT == WURZEL / "build" / "exakt_disc":
        EXAKT.parent.mkdir(exist_ok=True)
        r = subprocess.run(["gcc", "-O3", "-march=native", "-o", str(EXAKT), str(WURZEL / "src" / "exakt_disc.c")],
                           capture_output=True, text=True)
        return r.returncode == 0
    return False


def ctrl8_zeilen() -> list[list[str]]:
    return [z.split() for z in (CTRL / "ctrl_8.txt").read_text().splitlines() if z.strip()]


def schreibe_zeilen(zeilen: list[list[str]], pfad: Path) -> None:
    pfad.write_text("".join(" ".join(z) + "\n" for z in zeilen))


def defekt_soll() -> dict[str, str]:
    """Die vier Defektdateien, wie sie aus ctrl_8 hervorgehen (Test b prueft Byte fuer Byte)."""
    t = ctrl8_zeilen()
    kurz = [z[:] for z in t]; kurz[3] = kurz[3][:-1]                 # Zeile 4 ohne letzten Eintrag
    zwei = [z[:] for z in t]; zwei[2][2] = "2"                       # Zeile 3, Spalte 3: Eintrag 2
    buch = [z[:] for z in t]; buch[4][5] = "x"                       # Zeile 5, Spalte 6: Buchstabe x
    f = lambda m: "".join(" ".join(z) + "\n" for z in m)
    return {"zu_kurze_zeile.txt": f(kurz), "eintrag_2.txt": f(zwei), "buchstabe.txt": f(buch), "leer.txt": ""}


def brute_bruch(m: list[list[Fraction]]) -> Fraction:
    """Unabhaengige Gegenprobe (nur diese Testdatei): min_x max_i |(Ax)_i| mit Bruechen."""
    n = len(m[0])
    best = None
    for x in itertools.product((1, -1), repeat=n - 1):
        x = (1,) + x
        v = max(abs(sum(a * b for a, b in zip(z, x))) for z in m)
        best = v if best is None or v < best else best
    return best


class A_Sollwerte(unittest.TestCase):
    """a) jede Kontrolle ueber den Wrapper, mit Erwartung aus STATUS.md."""

    def test_soll(self):
        abweichungen = []
        for name, soll in SOLL:
            rc, out, err = py(WRAPPER, str(CTRL / f"{name}.txt"), "--erwartet", str(soll))
            if rc != 0:
                abweichungen.append(f"{name}: Soll {soll}, Wrapper-Code {rc}\n{out}{err}")
        self.assertFalse(abweichungen, "ABWEICHUNG VON STATUS.md:\n" + "\n".join(abweichungen))

    def test_wrapper_meldet_abweichung(self):
        # Gegenprobe: falscher Sollwert muss Fehlercode 1 geben
        rc, out, _ = py(WRAPPER, str(CTRL / "ctrl_8.txt"), "--erwartet", "6")
        self.assertEqual(rc, 1, out)


class B_Defekte(unittest.TestCase):
    """b) jedes Pruefprogramm lehnt jede Defektdatei ab (Rueckgabecode != 0, keine Diskrepanz-Zeile)."""

    def test_defektdateien_stammen_aus_ctrl_8(self):
        for name, inhalt in defekt_soll().items():
            self.assertEqual((DEFEKT / name).read_text(), inhalt, f"{name} weicht von der Ableitung aus ctrl_8 ab")

    def test_ablehnung(self):
        programme = {"check_disc": lambda f: py(CHECK, f), "verify_spencer": lambda f: py(VERIFY, f),
                     "pruefe_matrix": lambda f: py(WRAPPER, f)}
        if exakt_bereit():
            programme["exakt_disc"] = lambda f: lauf([EXAKT, f])
        fehler = []
        for name in defekt_soll():
            for prog, fn in programme.items():
                rc, out, err = fn(str(DEFEKT / name))
                if rc == 0 or hat_ergebnis(out):
                    fehler.append(f"{prog} nimmt {name} an (Code {rc}): {out.strip()[:100]}")
        self.assertFalse(fehler, "\n".join(fehler))

    def test_exakt_gebaut(self):
        if not exakt_bereit():
            self.skipTest("build/exakt_disc nicht vorhanden und nicht baubar: exakt_disc nicht mitgeprueft")


class C_BekannteUnterschiede(unittest.TestCase):
    """c) Festgehalten, nicht behoben. Ein Aufwachen dieser Tests heisst: Verhalten hat sich geaendert."""

    def test_mehr_zeilen_als_spalten(self):
        f = str(CTRL / "bahn15_40w.txt")  # 20 x 15
        rc, out, _ = py(CHECK, f)
        self.assertEqual(rc, 0)
        self.assertIn("exact discrepancy = 7", out)                       # check_disc rechnet
        rc, out, err = py(VERIFY, f)
        self.assertNotEqual(rc, 0)                                         # verify_spencer lehnt ab
        self.assertIn("Ungueltige Dimension 20x15", err)
        self.assertFalse(hat_ergebnis(out))
        # mit Nullspalten aufgefuellt rechnet es, und der Wrapper macht genau das selbst
        rc, out, _ = py(VERIFY, str(CTRL / "bahn15_40w_pad.txt"))
        self.assertEqual(rc, 0)
        self.assertIn("Exakte Diskrepanz: 7", out)
        rc, out, _ = py(WRAPPER, f, "--erwartet", "7")
        self.assertEqual(rc, 0, out)

    def test_gebrochene_eintraege(self):
        t = ctrl8_zeilen()
        erwartet = brute_bruch([[Fraction(v) for v in z] for z in [["1/2"] + t[0][1:]] + t[1:]])
        self.assertEqual(erwartet, Fraction(9, 2))                         # unabhaengige Bruch-Gegenprobe
        for schreibweise in ("1/2", "0.5"):
            with tempfile.TemporaryDirectory() as tmp:
                f = Path(tmp) / "bruch.txt"
                schreibe_zeilen([[schreibweise] + t[0][1:]] + t[1:], f)
                rc, out, err = py(CHECK, str(f))
                self.assertNotEqual(rc, 0, f"check_disc nimmt {schreibweise} an")  # check_disc lehnt ab
                self.assertFalse(hat_ergebnis(out))
                rc, out, err = py(VERIFY, str(f))
                self.assertEqual(rc, 0, err)                                # verify_spencer rechnet
                self.assertIn(f"Exakte Diskrepanz: {erwartet}", out)
                if exakt_bereit():
                    rc, out, _ = lauf([EXAKT, str(f)])
                    self.assertNotEqual(rc, 0)                              # exakt_disc lehnt ab
                    self.assertFalse(hat_ergebnis(out))
                rc, out, err = py(WRAPPER, str(f))
                self.assertEqual(rc, 2, out + err)                          # Wrapper verlangt ganze Zahlen


class D_WrapperPaareUndPruefsummen(unittest.TestCase):
    def test_paar_positiv(self):
        for name in ("bahn15_40w", "had24_1p3_paley12", "had24_2p2_paley12"):
            rc, out, _ = py(WRAPPER, "--paar", str(CTRL / f"{name}.txt"), str(CTRL / f"{name}_pad.txt"))
            self.assertEqual(rc, 0, out)

    def test_paar_negativ(self):
        t = [z[:] for z in ctrl8_zeilen()]
        orig = CTRL / "ctrl_8.txt"
        faelle = {}
        a = [z + ["0"] for z in t] + [["0"] * 9]; a[0][0] = "-1" if a[0][0] == "1" else "1"
        faelle["Original-Eintrag geaendert"] = a
        b = [z + ["0"] for z in t] + [["0"] * 9]; b[8][3] = "1"
        faelle["Eintrag in Nullzeile"] = b
        c = [z + ["0"] for z in t] + [["0"] * 9]; c[2][8] = "1"
        faelle["Eintrag in Nullspalte"] = c
        faelle["PAD kleiner"] = [z[:-1] for z in t[:-1]]
        faelle["Nullspalte vorn eingefuegt"] = [["0"] + z for z in t]
        with tempfile.TemporaryDirectory() as tmp:
            for name, m in faelle.items():
                f = Path(tmp) / "x_pad.txt"
                schreibe_zeilen(m, f)
                rc, out, _ = py(WRAPPER, "--paar", str(orig), str(f))
                self.assertEqual(rc, 1, f"{name}: {out}")

    def test_alle_pads(self):
        rc, out, _ = py(WRAPPER, "--alle-pads", str(CTRL), str(WURZEL / "runs"))
        self.assertEqual(rc, 0, out)
        self.assertRegex(out, r"\d+ _pad-Dateien geprueft: \d+ in Ordnung, 0 Abweichungen")

    def test_sha256sums(self):
        liste = WURZEL / "verifiers" / "SHA256SUMS"
        self.assertTrue(liste.is_file(), "verifiers/SHA256SUMS fehlt")
        eintraege = [z.split(None, 1) for z in liste.read_text().splitlines() if z.strip()]
        self.assertEqual(sorted(p.strip().lstrip("*") for _, p in eintraege),
                         ["src/check_disc.py", "src/exakt_disc.c", "verifiers/verify_spencer.py"])
        for summe, pfad in eintraege:
            pfad = pfad.strip().lstrip("*")
            echt = hashlib.sha256((WURZEL / pfad).read_bytes()).hexdigest()
            self.assertEqual(echt, summe, f"Pruefsumme von {pfad} stimmt nicht: geschuetzte Datei geaendert?")


if __name__ == "__main__":
    t0 = time.monotonic()
    erg = unittest.main(exit=False, verbosity=2).result
    print(f"Laufzeit: {time.monotonic() - t0:.1f} s")
    sys.exit(0 if erg.wasSuccessful() else 1)
