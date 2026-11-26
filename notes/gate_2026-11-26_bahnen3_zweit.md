# Gate zu Auftrag B2 (zweite Rechnung zu den 48ern unter der 3-Sylowgruppe P), 26.11.2026

Geprüfter Stand: Branch `ccr-a8e1e233-3psa2j`, Commit `52a18c3` (enthält `main` bis `fe088c7`). Prüfer: Orchestrator,
26.11.2026, 06:05 bis 06:08 Uhr (Uhr des Containers). Eigene Skripte und Log:
`notes/orchestrator/2026-11-26_gate_bahnen3_zweit/`. Rechenzeit unter 1 Minute auf 2 Kernen.

## Urteil

**Bestanden. Alle erwarteten Zahlen sind bestätigt, keine Abweichung.** Damit sind drei Aussagen über den 48er bei (20,6) zweifach gerechnet und
dürfen in die Meldung: „unter P keine Überdeckung mit höchstens 46 Wörtern“, „die 19 008 P-invarianten 48er bilden 8 Klassen“ und „es gibt einen
neunten 48er“. Die Treffer aus Auftrag H sind nichts Neues (achtmal 1+3, einmal 2+2). Kein Rekordweg, kein Tabellenergebnis.

## Was geprüft wurde

| Nr. | Punkt | Ergebnis |
|---|---|---|
| 1 | Prüfdateien, Diff | `sha256sum -c verifiers/SHA256SUMS` OK (alle drei). Gegenüber `main` geändert: nur `STATUS.md`, `LANDKARTE.md`; sonst neue Dateien (sieben Programme `src/b2_*`, `runs/bahnen3_zweit/` 37, Protokoll). Zwei Dateien um 2 MB (Liste der 19 008, Zuordnung mit Abbildungen). Die Programme des ersten Laufs (`src/bahnen3_*`) sind unberührt. Stand-Zeilen nicht angefasst. `main` war geholt |
| 2 | Protokoll gegen Auftrag | Teil 0 ohne Abweichung. Teil 1: (a), (b), (c), (d) wie erwartet; erster Test und Hochrechnung vorhanden; alle Pflichtkontrollen vorhanden (27 verkleinerte Fälle gegen Durchprobieren, 48er gezählt, Positivkontrolle mit Neunerbahn: 576 Lösungen). Teil 2: Kontrollen und (a), (b), (c) wie erwartet. Etwa 1 h 40 Min. von 6 h, kein Block über 30 Minuten |
| 3 | Vollständigkeit der Fälle | Gewicht höchstens 23 lässt null, eine oder zwei Neunerbahnen zu (drei wären 27): (a), (c) mit kleinen Bahnen bis 14, (d) mit kleinen Bahnen bis 5. Das sind alle Fälle |
| 4 | Anderes Verfahren | erstes Programm: Verzweigen am offenen Punkt, Schranke aus paarweise fremden Kandidaten. B2: Verzweigen über Bahnen (drin oder nicht), Schranke durch Kostenverteilung auf die offenen Punkte; für zwei Neunerbahnen ein eigener Weg über gebündelte Einserbahnen. Laut Protokoll; den Code habe ich nicht gelesen |
| 5 | Unabhängigkeit | In den sieben Programmen kein Verweis auf `src/bahnen3_*`, `bahnen_zaehl` oder `notes/orchestrator/`. Dass die Sitzung die Dateien nicht gelesen hat, ist ihre eigene Angabe und nicht nachprüfbar |
| 6 | P | die neun Elemente aus beiden Läufen sind als Mengen gleich (`gate_b2.py`, Teil A) |
| 7 | Beide Listen der 19 008 | selbst ausgebaut (Vertreter mit P, 24 Paarklassen je Lösung): je 19 008 verschiedene Mengen, **19 008 gemeinsam, 0 nur im ersten, 0 nur in B2**; der 48er (1+3) steht in beiden; alle vorkommenden Bahnen haben Größe 3 (Teil B) |
| 8 | Stichprobe Radius | 12 zufällige der 19 008 und 3 der 576 Lösungen mit Neunerbahn: Überdeckungsradius 6 über alle 2^20 Punkte (Teil C) |
| 9 | Alle Abbildungen der Klassenzuordnung | **19 008 von 19 008** selbst nachgerechnet: Abbildung angewandt, Wortmenge gleich der des Vertreters. Klassengrößen 432, 6912, 6912, 1728, 1728, 432, 432, 432; Größe mal Gruppenordnung überall 248 832 (Teil D) |
| 10 | Zuordnung von 1+3, 2+2, seed4 und den neun Treffern aus Auftrag H | Matrizen direkt aus den Quelldateien gelesen, alle 12 Abbildungen richtig: 1+3 und k3_seed1 bis 8 in Klasse 0, 2+2 und a_k12_t10-10_seed1 in Klasse 7, seed4 allein in Klasse 8 (Teil E) |
| 11 | Die neun Vertreter sind paarweise verschieden | eigene Invarianten: Produkte über vier Zeilen trennen alle außer {0,7} und {5,6}; die Verteilung der Punkttypen über alle 2^20 Punkte trennt diese beiden Paare. **Alle 36 Paare getrennt**, unabhängig vom Kanonisierer der Sitzung (Teil F, `gate_b2_inv2.py`) |
| 12 | `STATUS.md`, `LANDKARTE.md` | gegengelesen; nur Belegart und offene Punkte geändert, wie beauftragt. Im Gate: Dateizeile ergänzt, Stand-Zeilen nachgezogen |

## Befunde

1. **Die Klassenzahl ist jetzt ohne jeden Kanonisierer belegt.** Gleichwertigkeit: 19 008 nachgerechnete Abbildungen auf acht Vertreter. Verschiedenheit:
   Invarianten für alle 36 Paare der neun Vertreter. Daraus folgt „genau 8 Klassen unter den 19 008, dazu seed4 als neunte“, ganz ohne bliss und ohne das
   Programm der Sitzung. Die Gruppenordnungen (576, 144, 36, 24) ruhen weiter auf zwei Programmen (bliss beim Orchestrator, Kanonisierer der Sitzung).
2. **Die Zählung selbst (keine Überdeckung mit Gewicht höchstens 23) habe ich nicht ein drittes Mal gerechnet.** Sie ruht auf zwei Programmen mit
   verschiedenen Verfahren, die bei Gewicht 24 dieselben 19 008 Mengen liefern. Das ist die Belegart, die der Auftrag verlangt.
3. **2+2 steht nicht wörtlich in der Liste**, nur eine gleichwertige Kopie (Klasse 7). Das ist richtig so: P ist die 3-Sylowgruppe von 1+3 in dessen
   Koordinaten. Der Auftrag hatte „darunter 1p3 und 2p2“ ungenau formuliert.
4. **Fehlversuch der Sitzung**, sauber gemeldet: Das erste Verfahren für zwei Neunerbahnen lief 29 Minuten ins Zeitlimit und wurde ersetzt; das Ersatzverfahren
   besteht acht Kontrollen gegen Durchprobieren.

## Belegarten nach dem Gate

| Aussage | Belegart |
|---|---|
| Unter P keine selbstkomplementäre Überdeckung von (20,6) mit höchstens 46 Wörtern | voll, zwei Programme mit verschiedenen Verfahren |
| Genau 19 008 P-invariante 48er aus kleinen Bahnen | voll, zwei Programme; Listen im Gate verglichen |
| Die 19 008 bilden genau 8 Klassen | Abbildung je Lösung (19 008, im Gate nachgerechnet) und Invarianten für die Vertreter (zwei Sätze: Sitzung und Gate) |
| seed4 ist ein neunter 48er | Invarianten (Sitzung und Gate) |
| Treffer aus Auftrag H: achtmal 1+3, einmal 2+2 | Abbildung je Treffer, im Gate nachgerechnet |
| Gruppenordnungen 576 / 144 / 36 / 24 | zwei Programme (bliss, Kanonisierer der Sitzung) |

## Nicht geprüft

- Der Code der sieben Programme ist nicht gelesen; die Suchläufe (a), (c), (d) sind nicht wiederholt.
- Die 27 Kontrollen gegen Durchprobieren: nur das Protokoll gelesen.
- Unverändert offen und nicht Teil des Auftrags: Untergruppen ⟨g⟩ mit weniger als 48 Wörtern.

## Zusammenführung

Der Gate-Branch enthält den Gate-Branch zu N (`orchestrator/gate-n-2026-11-26`, PR 46) und damit `main`. Zwei Konflikte gelöst (`STATUS.md`,
Fehlversuche; `LANDKARTE.md`, eine Tabellenzeile): Zeilen von N und B2 nebeneinander. PR 46 zuerst mergen, dann diesen.
