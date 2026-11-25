# Gate zu Auftrag A: Zeilen mit einer Null (25.11.2026, 01:05)

Geprüft: Branch `ccr-6615c26c-zvmbps`, Stand `7a0466a`, Protokoll `notes/laeufe/2026-11-25_einzelnull.md`.
Prüfer: Orchestrator-Claude. **Ergebnis: bestanden, mit zwei Vermerken.** Gemergt als Squash (PR 24, `6d63258`).

| Nr. | Prüfung | Ergebnis |
|---|---|---|
| 1 | Teil 1 (5632 Zeilen, 112 Klassen, Schnitttabelle, Budget 208, Träger 8 und 6, Zirkulante) | deckt sich mit der Vorrechnung des Orchestrators (zwei unabhängige Programme) |
| 2 | Kontrolle n = 8: 57 600 Lösungen, 1920 mit fester Nullspalte | passt zur Theorie: 240 perfekte Codes der Länge 7, 240² = 57 600, 8 · 240 = 1920 |
| 3 | `runs/einzelnull/n8_verteilt.txt` (Nullen in zwei Spalten, je 4) | `verifiers/pruefe_matrix.py`: 5 / 5 / 5 |
| 4 | n = 11, je eine Null pro Zeile: 0 Lösungen (Baumsuche der Sitzung) | zweiter, unabhängiger Weg: Umkodierung auf 22 Kugeln bei (10,2), Tabelle K(10,2) ≥ 24. Umkodierung vom Orchestrator und von der Sitzung für alle 5632 Zeilen nachgerechnet, je 0 Abweichungen |
| 5 | `runs/sc13/w42_s4_matrix.txt` (21 x 13) | Prüfrahmen 7 / 7 / 7; eigener Code: 21 verschiedene Zeilenpaare, Radius 3 |
| 6 | P1 für n = 14 (alle 57 344 Zeilen, 0 Abweichungen) | deckt sich mit der Stichprobe des Orchestrators (3360 Fälle, 0 Abweichungen) |
| 7 | Änderungen an `STATUS.md` und `LANDKARTE.md` | nur Ergänzungen, sachlich richtig |

Vermerke:
1. **Gemischte Träger bei n = 11 (10, 8, 6) sind nicht entschieden** (Suche erfolglos, nur Typ 0 vollständig). Die Sitzung
   rechnet nach dem Gate eine Profilsuche weiter; deren Ergebnis ist hier nicht geprüft.
2. **Ablauf:** Zwischen Gate (`7a0466a`) und Merge kam ein weiterer Commit dazu (`b5f1301`: Nachrechnung der Umkodierung,
   Profilsuche im Suchcode). Er wurde mitgemergt und erst danach gelesen: 128 Zeilen, keine Änderung an STATUS oder LANDKARTE,
   unbedenklich. Künftig beim Merge den geprüften Stand festnageln.

Panne der Sitzung: In der Historie des Branches liegen etwa 86 MB Fortsetzungsdateien (ab Commit `2faa929`). Im aktuellen
Stand sind sie nicht enthalten; durch den Squash-Merge sind sie nicht nach `main` gelangt. Der Branch ist noch nicht
gelöscht, weil die Sitzung weiterarbeitet. Ein weiterer PR von diesem Branch muss wieder als Squash gemergt werden.
