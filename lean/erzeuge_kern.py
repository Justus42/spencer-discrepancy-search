#!/usr/bin/env python3
"""Erzeugt die Lean-Dateien für Teil 5 (Kernel-Variante, `decide +kernel`) unter lean/Kern/.

Kein Rechnen, nur Textbausteine: je Block ein Satz `Q b` mit `decide +kernel`, zusammengesetzt als Binärbaum
(`baum_blatt`, `baum_knoten` aus Ueberdeckung/Basis.lean). Die Blöcke liegen auf Dateien zu höchstens 64 Blöcken verteilt:
jeder Blocksatz hält im Lean-Prozess etwa 25 MB fest (gemessen 24.11.2026), 64 Blöcke also etwa 2,4 GB.
Geprüft werden die Dateien mit `pruefe_kern.sh` (höchstens 4 Lean-Prozesse gleichzeitig), nicht mit `lake build`. Erzeugte Dateien nie von Hand ändern.

Aufruf aus lean/:  python3 -I erzeuge_kern.py
"""
import os

HIER = os.path.dirname(os.path.abspath(__file__))
KOPF = "-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.\n"

# Bitzahl m -> (Blockexponent s, Anzahl Teildateien)
SWAR = {7: (7, 1), 15: (8, 2), 20: (8, 64)}
# Modul, Länge n, Radius ja (oder None), Radius nein, Blockexponent s, Teildateien
DATEN = [
    ("Hamming7", 7, 1, 0, 7, 1),
    ("Ctrl16", 15, 4, 3, 8, 2),
    ("Had24_1p3_ohne0", 20, None, 6, None, 0),
    ("Had24_1p3", 20, 6, 5, 8, 64),
    ("Had24_2p2", 20, 6, 5, 8, 64),
]


def schreibe(rel, text):
    pfad = os.path.join(HIER, rel)
    os.makedirs(os.path.dirname(pfad), exist_ok=True)
    with open(pfad, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)


def baum(o, laenge, unten):
    """Sätze n_o_len für alle Knoten des Teilbaums [o, o+laenge); unten = Länge, ab der Blätter/Kinder schon existieren."""
    z = []
    if laenge == 1:
        z.append("theorem n_%d_1 : ∀ j, j < 1 → Q (%d + j) := baum_blatt Q %d (by decide +kernel)" % (o, o, o))
        return z
    h = laenge // 2
    if laenge > unten:
        z += baum(o, h, unten) + baum(o + h, h, unten)
        z.append("theorem n_%d_%d : ∀ j, j < %d → Q (%d + j) := baum_knoten Q %d %d n_%d_%d n_%d_%d"
                 % (o, laenge, laenge, o, o, h, o, h, o + h, h))
    return z


def teile(ns, q_def, importe, anzahl_bloecke, teildateien, prefix_mod):
    """Schreibt Q-Datei und Teildateien; gibt Länge je Teil zurück."""
    schreibe(prefix_mod.replace(".", "/") + "/Q.lean",
             KOPF + "".join("import %s\n" % i for i in importe) +
             "\nnamespace %s\nopen Ueberdeckung\n\n%s\n\nend %s\n" % (ns, q_def, ns))
    L = anzahl_bloecke // teildateien
    for t in range(teildateien):
        zeilen = baum(t * L, L, 0)
        # baum(o, L, 0) erzeugt den ganzen Teilbaum bis zu den Blättern
        schreibe(prefix_mod.replace(".", "/") + "/Teil%d.lean" % t,
                 KOPF + "import %s.Q\n\nnamespace %s\nopen Ueberdeckung\n\n" % (prefix_mod, ns) +
                 "\n".join(zeilen) + "\n\nend %s\n" % ns)
    return L


def oberbaum(anzahl, L):
    """Knoten oberhalb der Teildateien (Länge > L)."""
    z = []
    def rek(o, laenge):
        if laenge <= L:
            return
        h = laenge // 2
        rek(o, h); rek(o + h, h)
        z.append("theorem n_%d_%d : ∀ j, j < %d → Q (%d + j) := baum_knoten Q %d %d n_%d_%d n_%d_%d"
                 % (o, laenge, laenge, o, o, h, o, h, o + h, h))
    rek(0, anzahl)
    return z


def main():
    for m, (s, td) in SWAR.items():
        ns = "Ueberdeckung.Kern.Swar%d" % m
        mod = "Kern.Swar%d" % m
        anzahl = 2 ** (m - s)
        L = teile(ns, "/-- Block b: `swar` stimmt mit `einsen %d` auf `[b·2^%d, (b+1)·2^%d)` überein. -/\n"
                      "abbrev Q (b : Nat) : Prop := swarBlock %d %d b = true" % (m, s, s, m, s),
                  ["Ueberdeckung.Basis"], anzahl, td, mod)
        text = KOPF + "".join("import %s.Teil%d\n" % (mod, t) for t in range(td))
        text += "\nnamespace %s\nopen Ueberdeckung\n\n" % ns
        text += "\n".join(oberbaum(anzahl, L))
        text += ("\n\n/-- `swar` zählt die Bits aller Zahlen unter `2^%d` richtig (im Kernel nachgerechnet). -/\n"
                 "theorem swar_ok : ∀ v, v < 2 ^ %d → swar v = einsen %d v :=\n"
                 "  swar_aus_bloecken %d %d (by decide) (fun b hb => by\n"
                 "    have := n_0_%d b hb; rwa [Nat.zero_add] at this)\n\nend %s\n" % (m, m, m, m, s, anzahl, ns))
        schreibe(mod.replace(".", "/") + ".lean", text)

    for name, n, ja, nein, s, td in DATEN:
        ns = "Ueberdeckung.Kern.%s" % name
        mod = "Kern.%s" % name
        importe = ["Ueberdeckung.Basis", "Ueberdeckung.Daten.%s" % name]
        text = KOPF
        if ja is not None:
            anzahl = 2 ** (n - s)
            L = teile(ns, "/-- Block b: alle `x` in `[b·2^%d, (b+1)·2^%d)` haben ein Wort im Abstand höchstens %d. -/\n"
                          "abbrev Q (b : Nat) : Prop := blockS %d %d b Ueberdeckung.%s.woerter = true" % (s, s, ja, ja, s, name),
                      importe, anzahl, td, mod)
            text += "".join("import %s.Teil%d\n" % (mod, t) for t in range(td))
            text += "import Kern.Swar%d\n" % n
            text += "\nnamespace %s\nopen Ueberdeckung\n\n" % ns
            text += "\n".join(oberbaum(anzahl, L)) + "\n\nend %s\n" % ns
        else:
            text += "".join("import %s\n" % i for i in importe)
        text += "\n/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/%s.lean`, nur mit `decide +kernel`. -/\n" % name
        text += "\nnamespace Ueberdeckung.%s\nopen Ueberdeckung\n\n" % name
        text += "theorem woerter_eq_kern : code matrix = woerter := by decide +kernel\n\n"
        if ja is not None:
            text += "theorem woerter_klein_kern : ∀ c ∈ woerter, c < 2 ^ %d := by decide +kernel\n\n" % n
            text += ("theorem bloecke_kern : ∀ b, b < 2 ^ (%d - %d) → blockS %d %d b woerter = true := fun b hb => by\n"
                     "  have := Kern.%s.n_0_%d b hb; rwa [Nat.zero_add] at this\n\n" % (n, s, ja, s, name, anzahl))
            text += "/-- Jedes x < 2^%d hat ein Wort der Liste im Hamming-Abstand höchstens %d. -/\n" % (n, ja)
            text += ("theorem radius%d_kern : ∀ x, x < 2 ^ %d → ∃ c ∈ code matrix, abstand %d x c ≤ %d := by\n"
                     "  rw [woerter_eq_kern]\n"
                     "  exact aus_bloeckenS Kern.Swar%d.swar_ok %d %d woerter (by decide) woerter_klein_kern bloecke_kern\n\n"
                     % (ja, n, n, ja, n, ja, s))
            text += "/-- Hauptsatz in Matrixform (Kernel-Fassung). -/\n"
            text += ("theorem hauptsatz_kern : ∀ x, x < 2 ^ %d →\n"
                     "    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile %d x z ≤ %d :=\n"
                     "  (code_iff_matrix %d %d matrix).1 radius%d_kern\n\n" % (n, n, ja, n, ja, ja))
        text += "/-- Prüfschleife lehnt Radius %d ab (Kernel). -/\n" % nein
        text += "theorem pruefe_nicht_%d_kern : pruefe %d %d (code matrix) = false := by decide +kernel\n\n" % (nein, n, nein)
        text += ("theorem nicht_radius%d_kern : ¬ ∀ x, x < 2 ^ %d → ∃ c ∈ code matrix, abstand %d x c ≤ %d := by\n"
                 "  rw [← pruefe_iff, pruefe_nicht_%d_kern]; decide\n\nend Ueberdeckung.%s\n" % (nein, n, n, nein, nein, name))
        schreibe(mod.replace(".", "/") + ".lean", text)
    print("Kern-Dateien geschrieben")


if __name__ == "__main__":
    main()
