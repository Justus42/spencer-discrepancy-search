# Gate zu Auftrag C (Gegenrechnung der Ideenrunde vom 26.11.2026)

Orchestrator-Gate. Sitzungsbranch `claude/auftrag-c-gegenrechnung-2xqmh5` (Commit `d6cc407`, enthält `main` = `e323707`), Auftrag
`notes/gegenrechnung_ideen_2026-11-26_auftrag.md`, Protokoll `notes/laeufe/2026-11-26_gegenrechnung_ideen.md`.
Beginn 18:40 UTC, Ende 18:50 UTC (Containeruhr; 19:40 bis 19:50 Uhr MEZ). Eigene Skripte und Logs:
`notes/orchestrator/2026-11-26_gate_gegenrechnung/` (`gate_treffer.py`, `ersatz2_direkt.c`, drei Logs).

## Urteil: bestanden

Keine Zahl der Ideenrunde weicht ab. Die Sitzung hat ihre Lücken selbst benannt. Kein Rekordkandidat.
Das Gate hat zwei Ungenauigkeiten gefunden, beide in meiner Notiz, nicht in der Arbeit der Sitzung (Punkte 1 und 3 unten).
Kein Ergebnis ändert sich dadurch.

## Was ich selbst geprüft habe

| Aussage der Sitzung | eigene Prüfung | Ergebnis |
|---|---|---|
| Prüfdateien unberührt | `git diff` gegen `main` für `src/` und die Prüfdateien; `sha256sum -c verifiers/SHA256SUMS` | kein Unterschied; 3 von 3 OK |
| Regel 4 (Blöcke, Commits, STATUS am Ende) | Zeitstempel der zehn Commits | Rechenblöcke von höchstens 7 Minuten, nach jedem Block Commit; `STATUS.md` nur im letzten Commit |
| Ordnung 24 ohne 4 Spalten: 42 304 Fälle mit disc ≥ 8 | Trefferliste der Sitzung gegen meine eigene, Fall für Fall, Matrizen über den Inhalt zugeordnet (`gate_treffer.py`) | 42 304 gemeinsam, 0 nur bei mir, 0 nur bei der Sitzung |
| disc 8 bei diesen Treffern (Regel 1) | `pruefe_matrix.py` an 14 Treffern (alle kleinen Klassen in beiden Ausrichtungen, drei Fälle ohne 2 Spalten) und 3 Nichttreffern | 14 von 14 dreimal Diskrepanz 8; 3 von 3 dreimal Diskrepanz 6 (`regel1.log.txt`) |
| Hadamard 20, zwei ersetzte Spalten: nie disc 8 | direkte Zählung ohne die Zerlegung in Gruppen und Bedingungen (`ersatz2_direkt.c`), siehe Punkt 2 | Paley 12, Ziel 6: 0 gute unter 66 · 2^22, wie das Programm der Sitzung und meines; Hadamard 20: Zufallsprobe 9 Mio., 0 gute |
| Schritt 2 (ohne 3 / 5 Spalten plus freie Zeile, voll): 0 lösbar, Löcher 24 344 bis 26 748 bzw. 11 856 bis 17 724 | Vergleich mit meiner Stichprobe von heute Nachmittag (`freie_zeile.log.txt`: 24 344 bis 26 730 bzw. 11 936 bis 17 712, 0 von 6072 bzw. 12 752 lösbar) | Stichprobe liegt im Rahmen der vollen Rechnung; nicht voll nachgerechnet |
| Einträge in `STATUS.md` und `LANDKARTE.md` | gegen Abschnitt 7 der Notiz und die Logs gelesen; Quotienten nachgerechnet (5/√8, 7/4, 7/√20, 8/√24, 6/√28, 9/√32, 5/√26, 7/√30, 5/√15, 5/√12) | stimmen; drei Ergänzungen von mir (Punkte 1 bis 3) |

## Widerspruch und Einschränkungen

1. **Die Transponierten bei Ordnung 24 sind keine neuen Klassen, meine Fallzahlen zählen doppelt.** Unter den 120 Matrizen
   „60 Klassen plus Transponierte“ sind nur 72 verschiedene: 48 der 60 Transponierten stehen wörtlich schon in der McKay-Liste
   (`gate_treffer.log.txt`). Die übrigen 12 (Originale 0, 1, 12, 14, 17, 19, 26, 39, 40, 42, 54, 56) sind nach McKays Seite zu
   Listenmatrizen äquivalent, weil die 60 Klassen dort ohne Transposition als Äquivalenz gezählt sind
   (https://users.cecs.anu.edu.au/~bdm/data/hadamard.html; nicht nachgerechnet, die Trefferzahlen 2640, 16, 6 passen dazu).
   Richtig gezählt: disc 8 ohne 2 Spalten in 3710 von 16 560, ohne 4 Spalten in 21 152 von 637 560 Fällen, in 12 der 60 Klassen.
   Dasselbe gilt für Ordnung 20 (3 Klassen nach derselben Seite; eine der drei Listenmatrizen ist symmetrisch). Kein Befund ändert sich, aber jede
   Rechnung über Ordnung 24 „mit Transponierten“ war doppelt so lang wie nötig, auch Schritt 2 der Sitzung (mein Auftrag).
2. **Zwei ersetzte Spalten: beide Programme folgen derselben Herleitung.** Die Sitzung hat die Bedingung aus dem Kopfkommentar von
   `ersatz_k2.c` genommen und offen gesagt, dass ihre Positivkontrolle leer war. „Zwei Programme“ sichert also die Ausführung, nicht
   die Herleitung. Mein Gegenstück: `ersatz2_direkt.c` entscheidet für jedes Spaltenpaar (v,w) direkt, ob die ersetzte Matrix das
   Ziel erreicht. An Paley 12 mit Ziel 6 (20 flache Punkte, 10 Bedingungen je Gruppe) sind es 0 gute unter allen 276 824 064
   Fällen; beide Programme schließen dort ebenfalls alle 66 Paare aus. An Hadamard 20 ist die volle direkte Zählung unmöglich
   (4^20 je Paar), die Zufallsprobe findet nichts und beweist wenig.
   Zur Einordnung: Das Verfahren kann nur ausschließen. An Sylvester 8 mit Ziel 4 meldet es keine Bedingung, direkt gezählt sind
   aber nur 4310 von 16 384 Spaltenpaaren gut. „ÜBERLEBT“ im Log der Sitzung heißt also „nicht ausgeschlossen“, nicht „erreicht das Ziel“.
3. **Verdopplungszellen: die Wortgrenze war bei d = 9 und d = 10 um eins zu streng.** Der Code muss nicht selbstkomplementär sein:
   Aus W Wörtern c in (m,R) werden die W Zeilen (c,+1); jeder Punkt (y,+1) liegt im Abstand ≤ R von einer Zeile, jeder Punkt (y,−1)
   vom Negativ einer Zeile. Erlaubt sind also 25 Wörter bei d = 9 und 31 bei d = 10 (selbstkomplementär 24 und 30, wie in der Notiz).
   Die Tabelle lässt die Zellen in beiden Lesarten offen. Auftrag V sucht Paare und ist davon nicht betroffen.
4. **Schritt 2 ist nur von der Sitzung voll gerechnet.** Bei mir Stichprobe. Eine volle zweite Rechnung dauert eine halbe Stunde und
   liegt über dem Rahmen eines Gates. Die Positivkontrollen der Sitzung sind nicht leer (16 von 16, 24 von 24).
5. **Nicht gegengerechnet** (so auch im Bericht der Sitzung): die Suchprobe zu den Verdopplungszellen (26 192 und 8952 offene Klassen),
   die Herleitung „mindestens 8 freie ±1-Zeilen“ bei Sylvester 32 (nur ihre Zahlen), Sols Handbeweise, die Literaturangaben.
6. **Unabhängigkeit:** Die Sitzung hat zwei Definitionen nachgelesen (Kopfkommentar `ersatz_k2.c`, Faserbedingung aus Sols Antwort),
   sonst keinen Code von mir. `gr.c` ist erkennbar eigenständig (Tiefensuche mit Schranke, Gray-Code als zweiter Weg).

## Eingetragen

`STATUS.md`: Stand-Zeile; bei „Hadamard 24 erreicht disc 8“ die richtige Zählung und der Hinweis, künftig nur die 60 Originale zu
rechnen; Wortgrenzen der Verdopplungszellen; Belegart bei den Ersatzspalten und bei Schritt 2; Verweis auf dieses Gate.
`LANDKARTE.md`: Stand-Zeile und ein Satz zur Zählung. Die Einträge der Sitzung sind sonst unverändert.
