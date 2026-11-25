# Auftrag R9 (freigegeben 25.11.2026): Rang 9 bei (15,4) zu Ende rechnen

Freigabe durch Justus im Orchestrator-Chat, 25.11.2026, 15:22 Uhr: eine Sonnet-Sitzung, die einige Stunden nebenher läuft,
zur Vollständigkeit. Grundlage: `notes/laeufe/2026-11-25_spann.md` (Auftrag S) und `notes/gate_2026-11-25_spann.md`.

```
Auftrag R9 (Sonnet, Effort hoch): die sechs offenen Rang-9-Klassen bei (15,4) exakt entscheiden

Lies zuerst STATUS.md, CLAUDE.md, notes/laeufe/2026-11-25_spann.md (Teil 2) und notes/gate_2026-11-25_spann.md.
Eigener Branch, Protokoll notes/laeufe/2026-11-25_rang9.md, Logs runs/spann_rang9/, STATUS.md erst am Ende.
Nichts installieren. Die Stand-Zeile (Zeile 3) von STATUS.md und LANDKARTE.md nicht ändern.

Frage: Gibt es einen selbstkomplementären Code mit 30 Wörtern (15 Paaren), Länge 15, Radius 4, dessen Wörter alle in
einer Rang-9-Erweiterung des affinen Spanns einer der beiden bekannten 32er-Klassen liegen? Von 9 Klassen solcher
Erweiterungen sind 3 ausgeschlossen. Offen sind 6, mit den Problemdateien runs/spann/probleme/<name>.txt und den
Symmetrie-Erzeugern <name>.gen (beide im Gate unabhängig geprüft):
  l111_r9_y384, l111_r9_y128, l111_r9_y129, ctrl16_r9_y7, l111_r9_y385, ctrl16_r9_y40   (in dieser Reihenfolge)
Programm: src/spann_dfs.c, Aufruf wie in src/run_spann_teil2b.sh:
  build/spann_dfs PROBLEM 15 ZEIT 3 0 ERZEUGER      (Bauen: gcc -O3 -march=native -fopenmp -o build/spann_dfs src/spann_dfs.c)
Das Programm zerlegt die Suche in Aufgaben (7 374 bis 20 817 je Fall) und arbeitet sie mit OpenMP ab.

Schritt 1, einzige Programmänderung: zwei zusätzliche, optionale Argumente VON BIS. Dann werden nur die Aufgaben mit
Index VON <= i < BIS bearbeitet; Erzeugung und Reihenfolge der Aufgaben bleiben unverändert. Ausgabe am Ende eindeutig:
Bereich, Zahl der Aufgaben insgesamt, Knoten, und ob der Bereich vollständig ohne Lösung ist, eine Lösung enthält oder
am Zeitlimit abgebrochen wurde. An Suche, Schranken und Symmetriebrechung nichts ändern. Ohne die neuen Argumente muss
sich das Programm verhalten wie bisher.

Schritt 2, Kontrollen (vor jeder neuen Rechnung, alle mit dem geänderten Programm):
 a) ctrl16_r9_y3, 15 Paare, ohne VON BIS: KEINE Lösung, 10 055 Aufgaben, 4 236 752 Knoten (wie Log
    runs/spann/teil2b_ctrl16_r9_y3_k15.log.txt).
 b) Derselbe Fall in vier Bereichen, die zusammen 0 bis 10 055 abdecken: jeder Bereich ohne Lösung, die Knotenzahlen
    addieren sich zu genau 4 236 752.
 c) Positivkontrolle: ctrl16_r9_y3 mit 16 Paaren, in zwei Bereichen (erste Hälfte, zweite Hälfte der Aufgaben): mindestens
    einer meldet LÖSBAR. Die ausgegebene Lösung mit src/spann_pruef.py nachrechnen (Radius 4).
 Bei Abweichung in a) oder b): stoppen und berichten, nichts weiterrechnen.

Schritt 3, Treiber src/run_spann_rang9.sh: arbeitet einen Fall in Bereichen ab, schreibt je Bereich eine Zeile nach
runs/spann_rang9/<name>.fortschritt (VON, BIS, Knoten, Sekunden, Ergebnis) und setzt beim nächsten Aufruf hinter dem
letzten vollständigen Bereich fort. Bereichsgröße so wählen, dass ein Bereich 5 bis 20 Minuten dauert (die frühen
Aufgaben sind deutlich teurer als die späten; mit 50 Aufgaben beginnen und anpassen). Zeitlimit je Aufruf 1500 s; ein
am Zeitlimit abgebrochener Bereich gilt als nicht gerechnet und wird mit halber Größe wiederholt. Nach jedem Bereich
Protokoll aktualisieren, committen, pushen (CLAUDE.md Regel 4: Blöcke von höchstens 30 Minuten). Nach einem Neustart des
Containers: build/ neu bauen, Treiber erneut starten, er setzt fort.

Schritt 4, Rechnen: die sechs Fälle in der Reihenfolge oben. Ein Fall ist entschieden, wenn alle Bereiche 0 bis zur
Aufgabenzahl lückenlos als vollständig ohne Lösung im Fortschritt stehen; das am Ende je Fall mit einem kleinen
Skript nachprüfen (Lücken, Überlappungen, Summe der Knoten) und die Summe ins Protokoll schreiben.
Schätzung des Orchestrators aus den Logs von Auftrag S (unbelegt, eher zu hoch): zusammen 4 bis 8 Stunden auf 4 Kernen;
einzeln etwa 0,5 / 0,7 / 1,4 / 1,7 / 1,9 / 2,2 Stunden.

Falls ein Bereich LÖSBAR meldet (das wäre ein Rekordkandidat, 7/sqrt(15) = 1,807): sofort alles anhalten. Die 15
Elemente über Zeile 3 der Problemdatei in Wörter übersetzen (Bit j = Spalte j, Bit 1 = Eintrag -1), als 15 x 15-Matrix
mit Einträgen +1/-1 nach runs/spann_rang9/ ablegen, Radius mit src/spann_pruef.py prüfen, dann
sha256sum -c verifiers/SHA256SUMS und python3 verifiers/pruefe_matrix.py MATRIX (erwartet: Diskrepanz 7). Ergebnis ins
Protokoll, pushen, berichten. Nichts nach außen melden.

Obergrenze: 8 Stunden Rechenzeit insgesamt, je Fall höchstens 3 Stunden; ein Fall, der daran scheitert, bleibt mit
seinem Fortschritt (Aufgaben fertig von gesamt) als unentschieden stehen, dann der nächste. Abbruch: Kontrolle weicht ab;
Zeit um; Lösung gefunden.
Belegart für entschiedene Fälle: voll, ein Programm, nur mit Symmetrie (wie die drei bisherigen Rang-9-Ausschlüsse).
Am Ende ohne Zuruf auswerten: main holen; in STATUS.md die Zeile „Codes im affinen Spann bekannter Codes, (15,4)“ und
den offenen Punkt „Suche im Spann“ anpassen, Fehlversuche als Einzeiler; in LANDKARTE.md die Zeile und den Absatz zu
Auftrag S anpassen (Regel 9). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/rang9_2026-11-25_auftrag.md in main und führe Auftrag R9 aus. Arbeite auf einem eigenen Branch.
```
