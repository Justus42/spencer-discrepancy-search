#!/usr/bin/env python3
"""Zieht die 16 Generatoren unverändert aus den beiden Modellantworten (kein Suchcode, kein Prüfcode).
Aufruf (aus dem Wurzelordner): python3 src/rundlauf/extrahiere.py
Schreibt src/rundlauf/astra_1.py ... astra_8.py, sol_1.py ... sol_8.py.
Der Code der Modelle steht Zeichen für Zeichen so da, wie er im Codeblock steht; davor nur eine
Kommentarzeile mit der Herkunft. Bei sol_2.py und sol_4.py steht der Vorspann paley12 (ebenfalls unverändert) davor.
"""
import re, sys, os

ORT = os.path.join("notes", "openrouter")
DATEIEN = {
    "astra": os.path.join(ORT, "rundlauf_2026-11-24--openai-gpt-6.2-astra.md"),
    "sol": os.path.join(ORT, "rundlauf_2026-11-24--openai-gpt-6.2-sol.md"),
}
ZIEL = os.path.join("src", "rundlauf")

def codebloecke(text):
    # ```python ... ``` mit Zeilenanfang
    return [m.group(1) for m in re.finditer(r"^```python\n(.*?)^```", text, re.S | re.M)]

def main():
    for name, pfad in DATEIEN.items():
        text = open(pfad, encoding="utf-8").read()
        bl = codebloecke(text)
        vorspann = None
        if name == "sol":
            vorspann, bl = bl[0], bl[1:]
            assert "def paley12" in vorspann
        assert len(bl) == 8, (name, len(bl))
        for i, code in enumerate(bl, 1):
            assert "def kandidaten" in code, (name, i)
            kopf = f"# Quelle: {pfad}, Generator Nr. {i}; Code der Modellantwort unverändert.\n"
            if vorspann is not None and i in (2, 4):
                inhalt = kopf + "# --- Vorspann paley12 (gemeinsam für Nr. 2 und 4, unverändert) ---\n" + vorspann + "# --- Generator ---\n" + code
            else:
                inhalt = kopf + code
            with open(os.path.join(ZIEL, f"{name}_{i}.py"), "w", encoding="utf-8") as f:
                f.write(inhalt)
            print(name, i, len(code.splitlines()), "Zeilen")

if __name__ == "__main__":
    main()
