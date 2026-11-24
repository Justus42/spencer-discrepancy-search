# Vortest-Skripte des Orchestrators, 24.11.2026 abends

Eigene Rechnungen des Orchestrator-Claude (Chat im Projekt), je wenige Minuten, Python mit numpy, igraph, sympy, ortools.
**Kein Prüfcode im Sinn von Regel 1 und nichts davon ist gegengerechnet**, außer wo unten vermerkt. Die Zahlen stehen als
Erwartung in den Aufträgen A und B (`notes/recherche_2026-11-24_pruefvermerk.md`, Abschnitt E) und werden dort von den
Sitzungen mit eigenem Code nachgerechnet. Die Skripte enthalten feste Pfade der Orchestrator-Umgebung
(`/home/claude/justus42/spencer-discrepancy-search`, `/home/claude/orch`); zum Nachlaufen anpassen.

| Skript | Inhalt | Ergebnis | Nachrechnung |
|---|---|---|---|
| `sym_enum.py`, `test15.py` | (15,4), Codes invariant unter einer Spaltenpermutation der Ordnung 7 (7+7+1), 13 (13+1+1), 11 (11+1⁴), vollständige Aufzählung | Ordnung 7: 112 Lösungen mit 32 Wörtern, 0 mit 30 und 28. Ordnung 13 und 11: 0 bei 30, 32, 34 | **offen, in keinem Auftrag** |
| `z7_kontrolle_16x15.txt` | eine der 112 Lösungen | `verifiers/pruefe_matrix.py`: 7/7/7 | erledigt (Prüfrahmen) |
| `test15b.py` | kleinste Restzahl mit 30 Wörtern unter Ordnung 7 | 466, je feste Zeile 56 Paare | **offen** |
| `chk.py` | `ctrl_16` ohne je eine Zeile; Dichten | immer 466 offene Punkte | **offen** |
| `aut.py` | Gruppen der Kontrollmatrizen, Zyklentypen der Elemente der Ordnung 3 und 7 | siehe Prüfvermerk D4 | Auftrag B, Teil 1 (nur `had24_1p3`) |
| `kl.py` | Kleitman-Dichte je n; Gram-Werte der 17x17-Matrizen | n = 1 bis 9 und 12 unter 1 | deckt sich mit `src/dichte_schranke.py` |
| `n11.py`, `n11_bb.py`, `nX_bb.py` | n = 11, d = 6, Zeilen mit einer Null: CP-SAT und eigene Verzweigungssuche; Kontrolle n = 8 | n = 11 unentschieden (277 916 Knoten in 240 s); n = 8: Lösung nach 8 Knoten (5/5/5), 7 Zeilen keine | Auftrag A |
| `zirk0.py` | Zirkulante mit Nulldiagonale, n = 8, 11, 14, 17 | Maximum 1, 4, 5, 6 | Auftrag A, Teil 1c |
| `z9.py`, `z9b.py` | (20,6), 3-Sylowgruppe des 48er-Codes: Bahnen, Teilfamilie Größe 1 und 3, CP-SAT | 8 / 160 / 58 200 Bahnen, 58 368 Punktklassen; 24 Zeilen gefunden, darunter nichts in 150 s (kein Beweis) | Auftrag B |

Aufrufe: `python3 test15.py`, `python3 test15b.py`, `python3 chk.py`, `python3 aut.py <Matrixdateien>`, `python3 kl.py`,
`python3 n11.py 10 150`, `python3 n11_bb.py 240`, `python3 nX_bb.py <Sekunden> <n> <d> <Zeilen>`, `python3 zirk0.py`,
`python3 z9.py 150`, `python3 z9b.py 130 20`.

Offen für eine kurze Sonnet-Sitzung: die drei mit „offen“ markierten Zeilen zu (15,4) mit eigenem Code nachrechnen
(Vorlage `src/bahnen_zaehl.c`, dort Ordnung 5).
