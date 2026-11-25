# Gate zu Auftrag H (Zerlegung der Kern-⟨1,a⟩-Familie, Halbproblem-Löser), 25.11.2026

Geprüfter Stand: Branch `ccr-fe07bd17-kedjsz`, Commit `b4f480c` (enthält `main` bis `1e91abd`). Prüfer: Orchestrator,
25.11.2026, 22:12 bis 22:17 Uhr (Uhr des Containers). Eigene Skripte ohne Code der Sitzung und Logs:
`notes/orchestrator/2026-11-25_gate_zerlegung/`. Rechenzeit unter 1 Minute auf 2 Kernen.

## Urteil

**Bestanden. Kein Treffer, kein Rekordkandidat, kein Tabellenergebnis.** Alle Kontrollen des Auftrags sind bestanden, alle gemeldeten
Restzahlen stimmen. Die Negativbefunde zu K = 11, K = 10 und d = 9 sind „Suche erfolglos“ und schließen nichts aus.
Neu aus dem Gate: Die besten Zustände bei d = 9 in den Typen (12,11) und (12,13) liegen in einem Fall, der schon bewiesen ausgeschlossen ist
(Befund 1). Die Restzahl 1760 ist deshalb kein Beinahe-Treffer.

## Was geprüft wurde

| Nr. | Punkt | Ergebnis |
|---|---|---|
| 1 | Prüfdateien, Diff | `sha256sum -c verifiers/SHA256SUMS` OK (alle drei). Gegenüber `main` geändert: nur `STATUS.md`, `LANDKARTE.md`; sonst neue Dateien (`runs/zerlegung/` 260, neun unter `src/`, Protokoll), keine über 300 KB. Stand-Zeilen von der Sitzung nicht angefasst. `main` war geholt. Nichts installiert |
| 2 | Protokoll gegen Auftrag | Schritt 0 ohne Abweichung; K1 4 von 4 in beiden Richtungen (Soll 3 von 4), nach 40 Minuten Sitzungszeit (Abbruchgrenze 2 Stunden); K1b; K2; K3 8 von 8 (Soll 4 von 8); Schritt 4 in der verlangten Reihenfolge. Etwa 3 h 20 Min. von 6 h. Commit-Zeiten der Blöcke passen zum Protokoll, kein Block über 30 Minuten |
| 3 | Alle gespeicherten Paare, eigener Zähler `gate_zerl.py` | 89 Paare (56 aus Schritt 4, 2 aus Schritt 2, 3 Proben mit K = 12, 28 aus den Kontrollen): offene Klassen in 89 von 89 gleich der gemeldeten Restzahl. Genau 28 Paare haben 0 offene Klassen: 27 aus den Kontrollen und die Probe mit K = 12 im Typ (10,10); der Vorversuch mit Startwert 3 hat 228, wie im Protokoll. Kein Zielpaar (K = 11, K = 10, d = 9) ist gelöst, es wurde also kein Treffer übersehen |
| 4 | K3, die acht Treffer | Matrix 24 × 20 selbst aus dem Paar gebaut (Zeilenmenge gleich der Datei der Sitzung), `pruefe_matrix.py`: Diskrepanz 8 von allen drei Programmen, 8 von 8 |
| 5 | Wiederholbarkeit | K3 mit Startwert 1 neu gerechnet: Lösung nach 1,2 s, Paar byte-gleich zum Lauf der Sitzung |
| 6 | Zusatzkontrolle des Gates (nicht im Auftrag): abwechselnde Suche aus Zufallsstart in der Struktur von `ctrl_17`, K = 8, Typ (9,8), d = 7 | 4 von 4 gelöst in je 0,1 s, mit eigenem Zähler bestätigt. Der Auftrag hatte `ctrl_17` nur für das Halbproblem verlangt |
| 7 | K2 | Log gelesen: „keine Lösung“ an der Wurzel durch die Volumenschranke (256 > 110), für drei gestrichene Zeilen. Das Lemma dahinter ist in Auftrag P gegengerechnet. Die tiefere Logik der Tiefensuche prüft K2 nicht; dafür steht der Vergleich gegen vollständiges Aufzählen (1200 kleine Fälle, 0 Abweichungen, Logs gelesen, nicht neu gerechnet) |
| 8 | Schritt 2, W mit 10 Spalten | Zählschranke an der Wurzel, 5120 gegen 4920, N0 = 10, B = 410. Dieselben Zahlen wie bei Sol und in Auftrag P (dort 17 031 Ketten aufgezählt); drittes Programm |
| 9 | Schritt 2, W mit 12 und 14 Spalten | „unentschieden“ ist richtig bezeichnet: Zeitgrenze der Tiefensuche, lokale Suche ohne Lösung. Kein Ausschluss |
| 10 | Filter von Sol | Die ungeprüften Überlappungsfilter (mindestens 6 bzw. 5 Dreierzeilen) sind im Suchcode nicht verwendet. Verwendet ist nur die verallgemeinerte Zählschranke; ihr Kern N0 wird exakt berechnet, die Knotenschranke ist gegen vollständiges Aufzählen getestet (900 Fälle) |
| 11 | `STATUS.md`, `LANDKARTE.md` | gegengelesen; Quotienten und Spalten stimmen (8/√22 = 1,706; 8/√24 = 1,633; 8/√20 = 1,789; 9/√24 = 1,837; 9/√25 = 1,800). Belegart „Suche“ überall, wo nur gesucht wurde |

## Befunde

1. **Die besten Zustände bei d = 9 liegen in einem ausgeschlossenen Fall** (`gate_hadamard12.log.txt`). In allen vier Läufen zu (12,11) und allen vier zu
   (12,13) ist die Hälfte mit 12 Spalten eine volle Hadamard-Matrix (alle 66 Zeilenprodukte 0) mit denselben Profilen wie Paley-12 (880 / 792 / 220 / 144 / 12).
   Für genau diese Hälfte schließt der Zählbeweis jede zweite Hälfte mit 9, 11 oder 13 Spalten aus (Auftrag P, Belegart Herleitung plus Zahlen).
   Die Suche läuft also in ein Becken, in dem es keine Lösung gibt. Dasselbe gilt für den Zustand mit 1760 aus Schritt 2 (W dort eine volle Hadamard-Matrix).
   Die Sitzung hat die Paley-Struktur bemerkt, den Schluss aber nicht gezogen. In `STATUS.md` nachgetragen.
2. **Alle Positivkontrollen sind Hadamard-artig.** Die acht Treffer von K3 haben dieselben Zeilenprodukte wie der bekannte 48er (U: 66-mal 1; W: 54-mal 1,
   12-mal 3), `ctrl_17` hat als eine Hälfte eine Hadamard-Matrix 8 × 8. Die Kontrollen zeigen, dass die Suche solche Lösungen findet. Sie zeigen nicht,
   dass sie eine Lösung anderer Bauart fände. Bei K = 11 und K = 10 gibt es keine vollen Hadamard-Hälften; dort sagt „immer dieselbe Restzahl“ vor allem
   etwas über die Suche.
3. **Kürzung von (b) auf 20 Minuten:** zulässig („höchstens 60“). Die Begründung trägt nur als Faustregel: Aus jeder Lösung mit K = 10 folgt zwar eine mit
   K = 11, aber dass die Suche keine mit K = 11 fand, schließt keine aus.
4. **Kleinigkeiten:** „je höchstens 24 s“ bei K3 war 24,3 s, in `STATUS.md` und `LANDKARTE.md` auf „≤ 25 s“ gesetzt. Die ersten Block-Commits der Sitzung
   tragen keine Mitautor-Zeilen (von der Sitzung selbst gemeldet), ohne Folgen für die Ergebnisse.

## Belegarten nach dem Gate

| Aussage | Belegart |
|---|---|
| K = 11 (44 Wörter) und K = 10 (40 Wörter) bei (20,6) in den Typen (11,9), (13,7), (10,10), (12,8) nicht erreicht | Suche erfolglos; Restzahlen zweifach gezählt |
| d = 9, K = 12 in sechs Typen nicht erreicht | Suche erfolglos; Restzahlen zweifach gezählt |
| d = 9, U = Paley-12 ohne Spalte, W mit 10 Spalten ausgeschlossen | Zählschranke, drei Programme (Vortest, P, H), Herleitung nicht formal geprüft |
| dasselbe mit 12 und 14 Spalten | unentschieden |
| Der Löser findet den 48er aus Zufallsstart (8 von 8, höchstens 25 s) und im Typ (10,10) (1 Lauf, 39 s) | Treffer mit drei Prüfprogrammen bestätigt; im Gate für die acht Treffer von K3 wiederholt |

## Nicht geprüft

- Die Suche selbst (`zerl_halb.c` nicht gelesen bis auf die Stelle zu Sol); geprüft sind ihre Ergebnisse.
- Die Tests der Tiefensuche gegen vollständiges Aufzählen und die Gewichtssuche (Verhältnis 0,82 und 0,67): nur Logs gelesen.
- Welcher Klasse (1+3, 2+2 oder eine andere) die Treffer angehören: offen, wie von der Sitzung gemeldet.
- Der Treffer im Typ (10,10) mit K = 12: nur mit eigenem Zähler nachgezählt (0 offen), nicht erneut durch `pruefe_matrix.py`.

## Zusammenführung

Der Gate-Branch enthält den Gate-Branch zu P (`orchestrator/gate-p-2026-11-25`, PR 42) und damit `main`. Zwei Konflikte in `STATUS.md`
(Negativbefunde, Offene Punkte) gelöst: Zeilen von P und von H nebeneinander. PR 42 zuerst mergen, dann diesen.
