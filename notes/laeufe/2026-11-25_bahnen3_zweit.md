# Auftrag B2: zweite Rechnung zu den 48ern unter der 3-Sylowgruppe (Cloud, 4 Kerne)

Auftrag: `notes/gegenrechnung_bahnen3_2026-11-25_auftrag.md` (B2), freigegeben durch den Startprompt am 25.11.2026.
Eigene Programme, nicht gelesen: `src/bahnen3_*`, `src/bahnen_zaehl.c`, `notes/orchestrator/`; Ergebnisdateien unter
`runs/bahnen3/` erst nach eigenem Ergebnis geöffnet. Logs `runs/bahnen3_zweit/`. Branch `ccr-a8e1e233-3psa2j`.
Beginn 25.11.2026, 21:55 UTC (date).

## Programme

- `src/b2_aequiv.c` (Prüfcode): Code → Graph (Ecken r±, c±; Kanten r+–r−, c+–c−, r^s–c^t bei M·s·t = +1; Zeilen- und
  Spaltenecken verschieden gefärbt). Eigene Farbverfeinerung (Signatur = Hash der Nachbarfarben, nur farbabhängig, also
  invariant), Individualisieren der ersten kleinsten Zelle, Spurvergleich, kanonisches Blatt = Maximum von (Spur, Zertifikat).
  |Aut| = Zahl der Blätter mit bestem Zertifikat. Modi `aut` (alle Automorphismen als Spaltenabbildung) und `klassen`.
- `src/b2_teil0.py` (Prüfcode): prüft jedes Element einzeln nach, Abgeschlossenheit, Ordnungen, P, Bahnen (numpy), Radius.

## Teil 0: Kontrollzahlen (had24_1p3_paley12)

Log `runs/bahnen3_zweit/teil0.log.txt`, Elemente `G_elemente_1p3.txt`, `P_elemente.txt`. Laufzeit 0,1 s (C) + 2 s (Python).

| Angabe | erwartet | eigen |
|---|---|---|
| Ordnung von G | 576 | 576 (3 497 Suchknoten; jedes Element nachgeprüft, alle 576² Produkte in G) |
| Elementordnungen | 1:1, 2:87, 3:8, 4:216, 6:120, 8:144 | 1:1, 2:87, 3:8, 4:216, 6:120, 8:144 |
| Untergruppe der Ordnung 9 | genau eine, nicht zyklisch | genau eine (keine Elemente der Ordnung 9, {1} ∪ 8 Elemente der Ordnung 3 abgeschlossen), nicht zyklisch, Normalteiler |
| Bahnen auf 2^19 Paaren | 8 / 160 / 58 200 | 8 / 160 / 58 200, zusammen 58 368 |
| 48er | 8 Dreierbahnen, Radius 6 | 24 Zeilenpaare = 8 volle Dreierbahnen; Radius 6 über alle 2^20 Punkte |

had24_2p2_paley12: ebenfalls |Aut| = 576 (1 961 Knoten). **Teil 0: keine Abweichung.**

## Teil 1: zweite Zählung (Zwischenstand)

Programm `src/b2_zaehl.c` (Prüfcode, C/OpenMP, ganze Zahlen). Bahnen selbst berechnet (C, getrennt von Python), Überdeckung
symmetrisch (0 Abweichungen), 48er = Bahnen 8, 39, 79, 96, 109, 130, 142, 161 (je Größe 3), überdeckt alles (`teil1_info.log.txt`).
Verfahren: binäres Verzweigen über kleine Bahnen (drin/nicht drin), Zählschranke Σ m_u · min w_c/cov_m(c) (Festkomma 2^32, abgerundet),
Zustand über D = 1792 schwerste Punktbahnen, bei überdecktem D Vollprüfung über alle 58 368 und Weiterverzweigen. Einzelheiten im Kopf
der Datei und unten (Vollständigkeit).

Erster Test (a) mit Gewicht ≤ 18: keine, 155 255 Knoten, 0,3 s (D = 512). Hochrechnung (D = 512): B = 20/21/22: 9 s / 40 s / 148 s;
mit D = 1792: B = 21 6,6 s, B = 22 16,7 s → B = 23 ca. 1–2 min, B = 24 ca. 5–10 min, (c)/(d) je Minuten. Weit unter 4 h.

| Lauf | Ergebnis | Knoten | Zeit (4 Kerne) | Log |
|---|---|---|---|---|
| (a) nur Größe 1/3, Gewicht ≤ 23 | **keine** | 4,70 · 10^7 | 47 s | `teil1a_B23.log.txt` |
| (b) nur Größe 1/3, Gewicht ≤ 24 | **19 008**, alle Gewicht 24, alle aus 8 Dreierbahnen, paarweise verschieden, 48er dabei | 3,65 · 10^8 (1,15 · 10^8 Vollprüfungen) | 350 s | `teil1b_B24.log.txt`, Liste `teil1b_B24_liste.txt` |
