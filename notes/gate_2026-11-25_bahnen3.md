# Gate zu Auftrag B: Bahnen unter der 3-Sylowgruppe des 48er-Codes (25.11.2026, 00:50)

Geprüft: Branch `claude/bahnen-3-sylowgruppe-48er-iuy314`, Stand `cfa513b`, Protokoll `notes/laeufe/2026-11-25_bahnen3.md`.
Prüfer: Orchestrator-Claude mit eigenen Programmen (`notes/orchestrator/2026-11-25_gate_bahnen3/`, Python mit igraph/bliss,
sympy, numpy, ortools; feste Pfade der Orchestrator-Umgebung). **Ergebnis: bestanden, mit zwei Vermerken.**

| Nr. | Prüfung | Ergebnis |
|---|---|---|
| 1 | Teil 1 (Gruppe 576, P Ordnung 9, Bahnen 8 / 160 / 58 200, 58 368 Punktklassen, 48er = 8 Dreierbahnen) | stimmt mit der Vorrechnung des Orchestrators vom 24.11. überein (zwei unabhängige Programme) |
| 2 | Liste der 19 008 Lösungen mit 24 Zeilen (`runs/bahnen3/teil2_klein_B24_liste_4_56.txt`) gegen eigene Überdeckungsmatrix | 19 008 verschieden, alle überdecken, alle aus 8 Dreierbahnen, 48er enthalten (`gate_b.py`) |
| 3 | Abgeschlossenheit der Liste unter G (6 Erzeuger aus bliss) und unter Vorzeichenwechsel jeder Spaltenbahn von P | 0 Lösungen fallen heraus; 654 Bahnen unter G |
| 4 | Äquivalenzklassen der 19 008 Lösungen (vorzeichenbehaftete Spaltenpermutationen; `gate_b4.py`) | **8 Klassen**: 4 mit Gruppenordnung 576 (je 432 Lösungen; darunter 1+3 und 2+2), 2 mit 144 (je 1728), 2 mit 36 (je 6912). Lösungen × Gruppenordnung ist in allen Klassen 248 832, wie es für volle Bahnen eines Normalisators sein muss |
| 5 | Treffer aus Teil 4 (`runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt`) | 48 Wörter, Gruppenordnung 24, zu keiner der 8 Klassen äquivalent (auch nicht zu 1+3, 2+2): ein neunter 48er |
| 6 | Unabhängiger Beweisversuch „keine Überdeckung mit ≤ 23 Zeilen aus kleinen Bahnen“ mit CP-SAT (`gate_b3.py`, 170 s; am 24.11. schon 150 s) | nicht entschieden. Kein Widerspruch, kein Beweis |
| 7 | Änderungen an `STATUS.md` und `LANDKARTE.md` | knapp, sachlich richtig, kein Text verloren |

Vermerke:
1. **Vollständigkeit ruht auf einem Programm.** „Keine Überdeckung mit ≤ 23 Zeilen“ (Teil 2 und Teil 3) belegt nur
   `src/bahnen3_zaehl.c` mit seinen 8 Selbsttests. Die Prüfungen 3 und 4 stützen die Zählung von Teil 2 stark
   (ein Zählfehler würde die Symmetrie der Liste fast sicher brechen), ersetzen aber keine zweite Zählung. Für Teil 3
   (Neunerbahnen) gibt es keine Positivkontrolle mit echter Lösung und keine unabhängige Stütze. Belegart „voll“ ist
   vertretbar; eine zweite Zählung mit anderem Verfahren wäre nötig, bevor die Aussage nach außen ginge.
2. **Teil 4** hat die Kontrolle nicht bestanden (1 von 4, 0 von 4); die Sitzung hat auftragsgemäß keine Leiter gestartet.
   Für ein einzelnes Element der Ordnung 3 ist unter 48 Wörtern nichts geprüft.

Neu gegenüber dem Protokoll: Es gibt mindestens **neun paarweise nicht äquivalente** selbstkomplementäre Codes mit 48
Wörtern bei (20,6): die acht P-invarianten Klassen und der Treffer aus Teil 4. Der offene Punkt „19 008 Lösungen nicht nach
Klassen sortiert“ in `STATUS.md` ist damit beantwortet (Eintrag mit dem nächsten Doku-PR).

Nebenbefund: „Regel 9“ (LANDKARTE ergänzen) steht nur in den Projektanweisungen, nicht in `CLAUDE.md` des Repos.
