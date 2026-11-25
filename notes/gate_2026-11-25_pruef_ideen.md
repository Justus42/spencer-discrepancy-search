# Gate zu Auftrag P (Gegenrechnung der Vortests der Ideensammlung), 25.11.2026

Geprüfter Stand: Branch `claude/auftrag-p-gegenrechnung-s6wy5c`, Commit `8662187` (zweigt bei `5f43ac7` von `main` ab). Prüfer: Orchestrator,
25.11.2026, 19:43 bis 19:55 Uhr (Uhr des Containers). Eigene Skripte ohne Code der Sitzung und Logs:
`notes/orchestrator/2026-11-25_gate_pruef_ideen/`. Rechenzeit gut 3 Minuten auf 2 Kernen.

## Urteil

**Bestanden. Kein Kandidat, kein Tabellenergebnis.** Alle Zahlen der Vortests sind von der Sitzung mit eigenen Programmen bestätigt.
Das Gate hat die Belege der Teile 1, 2, 3 (n = 18, 20) und den Rechenkern von Teil 5 vollständig mit dritten Programmen nachgerechnet,
ohne Abweichung. Damit gelten „Starrheit um die 32er“, „kozyklisch“ und die Zerlegungszahlen als zweifach gerechnet.
Drei Belegarten habe ich in `STATUS.md` enger gefasst (unten); an den Aussagen ändert das nichts.

## Was geprüft wurde

| Nr. | Punkt | Ergebnis |
|---|---|---|
| 1 | Prüfdateien, Diff | `sha256sum -c verifiers/SHA256SUMS` OK (alle drei), `test_verifiers.py` 11 Tests OK. Gegenüber `main` geändert: nur `STATUS.md`, `LANDKARTE.md`; sonst 269 neue Dateien (`runs/pruef_ideen/` 257, `src/pruef_ideen/` 6, `verifiers/pruef_ideen/` 5, Protokoll). 24 Dateien über 300 KB, alle gespeicherte Gegenbeispiele (`*.bin.gz`, je etwa 366 KB). Stand-Zeilen von der Sitzung nicht angefasst. Nichts installiert |
| 2 | Protokoll gegen Auftrag | Teile 1 bis 5 vollständig, jede erwartete Zahl genannt und getroffen. Suchcode und Prüfcode getrennt. Etwa 1 h 20 Min. von 5 h. Die Skripte des Orchestrators wurden laut Protokoll nicht gelesen |
| 3 | Teil 1, eigenes Programm `gate_kern.c` (anderes Verfahren: nur Zeilenzuordnung aufgezählt, Vorzeichen je Zeile über Zweifärbbarkeit) | alle vier Dateien: 560 + 560 Zertifikate (3 gegen 2) und 1820 + 1820 (4 gegen 3) bestätigt: jede Kernklasse ist Loch, Lochzahl neu gezählt und gleich, nicht verteilbar. Jede Streichmenge kommt genau einmal vor. Lochzahlen 967 (448) / 987 (112) in beiden Klassen; 4 gegen 3: 1476 / 1484 / 1508 / 1512 (784 / 896 / 112 / 28) in Klasse 1, 1476 / 1492 / 1512 (1120 / 672 / 28) in Klasse 2 |
| 4 | Teil 1, Kontrollen des eigenen Programms | zwei gestrichene Zeilen, zwei Ersatzzeilen, 24 Löcher je Paar: verteilbar in 120 von 120 (beide Klassen). Eine Klasse aus einem 3-gegen-2-Zertifikat weggelassen: verteilbar in 3445 von 3445 und 3443 von 3443, die Zertifikate sind also minimal |
| 5 | Teil 1, Fehler der Sitzung (Index 1223) | Rohdatei und Endfassung unterscheiden sich nur in dieser Zeile; das neue Zertifikat (15 Klassen) ist bestätigt |
| 6 | Teil 2, eigenes Skript `gate_spalte.py` | beide Klassen, je 240 Fälle, alle 2^15 Ergänzungen: Minimum 233, vier Optima, echte Spalte optimal, in 480 von 480. Kontrolle mit 16 Zeilen: Minimum 0, vier Optima |
| 7 | Teil 3, n = 18 und 20, eigenes Skript `gate_kozyk.py` (Gruppentafeln aus Präsentationen, Gruppengesetz für alle Tripel, z zentral, Elementordnungen) | alle 3 538 944 gespeicherten Gegenbeispiele der neun Gruppen nachgerechnet: kein Maximum über 6, keine Fehlstelle. Die sieben Dateien in `beste/` sind gleich der eigenen Matrix; drei davon mit `pruefe_matrix.py` neu gerechnet: Diskrepanz 6 von allen drei Programmen |
| 8 | Teil 3, Anzahlen mit disc ≥ 6 | nicht nachgerechnet. Z40 (28 560) und Z5×Q8 (3240, davon 480 Hadamard) stimmen mit dem Vortest überein, die übrigen stammen aus einem Programm. Für den Bestwert 6 sind sie nicht nötig |
| 9 | Teil 3, n = 26, 28, 30 | nicht nachgerechnet. Sieb ohne gespeicherte Gegenbeispiele, wie im Auftrag vorgesehen. Sitzung und Vortest (zwei Programme) finden beide keine Überlebenden; Dic15 ist nur in der Sitzung vollständig |
| 10 | Teil 4 | Roh-Log gelesen: alle Zahlen gleich den Vortests (Profile, Schnitte 92/92/92/47/47/15/15, Bilanz, 17 031 Ketten ohne Treffer, kleinstes \|A\| 10/7/6, Summen, Lemma). Im Gate nicht neu gerechnet, weil Vortest und Sitzung schon zwei unabhängige Programme sind |
| 11 | Teil 5, Fall ohne Sonderkappe, eigenes Programm `gate_kugeln.c` (Verzweigung über die größte offene Klasse, kein Symmetriebruch an der Wurzel) | zehn Radius-2-Kugeln überdecken die 512 Klassen nicht: 3 569 441 Knoten, vollständig. Die Sitzung hatte 63 505 Knoten mit Symmetriebruch, in C und Python mit demselben Suchbaum. Kontrollen des eigenen Programms: (7,1) mit 8 Kugeln gefunden, (5,1) mit 3 Kugeln keine, mit 4 gefunden, (8,1) mit 16 gefunden |
| 12 | Teil 5, Schnittgrößen und Schranke, `gate_schnitte.log.txt` | Kugelgröße 56; Schnitt 20, 20, 6, 6, 0 bei gefaltetem Abstand 1 bis 5; gleiche Parität gibt Abstand 2 oder 4, also mindestens 6. Schranke 505 bis 430 für eine bis zehn Sonderkappen |
| 13 | Teil 5, Schranke 36 statt 43 | nicht nachgerechnet; für den Schluss genügt 43, und 43 hatte ich im Prüfvermerk von Hand nachvollzogen |
| 14 | `STATUS.md`, `LANDKARTE.md` | Zeile für Zeile gegen Protokoll und Abschnitte 7 und 10 der Ideennotiz gelesen: übertragen ist nur Bestätigtes; die nicht gegengerechneten Vortests stehen als solche unter „Fehlversuche“. Quotienten nachgerechnet (6/√18 = 1,414; 6/√20 = 1,342; 8/√26, 8/√28, 8/√30 = 1,569, 1,512, 1,461) |

## Befunde

1. **Aussage im Protokoll falsch, ohne Folgen:** „`main` unverändert gegenüber dem Start (kein Merge nötig)“, vermerkt um 19:36 Uhr. `main` hatte seit 19:30 Uhr den
   Merge von PR 41 (Auftrag R9). Vermutlich hat die Sitzung nur den eigenen Branch geholt. Die zwei Konflikte (`STATUS.md`, Offene Punkte; `LANDKARTE.md`,
   Zeile zur Suche im Spann) habe ich im Gate gelöst: Zeilen aus `main`, dazu die neuen Zeilen von P.
2. **Belegart enger gefasst** (`STATUS.md`, Negativbefunde). „voll“ stand dort auch bei Aussagen, die auf einem Beweis von Hand ruhen:
   - Zerlegung, d = 9 (volle Paley-Hälfte für jedes W; punktierte mit m2 = 10): jetzt „Herleitung, von drei Seiten nachvollzogen, nicht formal geprüft; Zahlen von
     zwei Programmen; bei m2 = 10 Aufzählung aller Ketten“.
   - Reelle 10 × 10-Matrizen: jetzt „Herleitung plus Aufzählung“, mit dem Hinweis auf drei Programme und zwei Suchbäume für den Zehn-Kugel-Fall.
   - Teil 1: Zusatz, dass die Paarbedingung (Abstand höchstens 8 in einer Zeile) eine Herleitung ist; die Zertifikate selbst sind voll geprüft.
3. **Mehr als verlangt, in Ordnung:** zweite 32er-Klasse bei 4 gegen 3 und bei Teil 2; neue Belegart `g*` in der Landkarte für das Sieb ohne gespeicherte Gegenbeispiele.
4. **Offener Punkt umformuliert:** „K(10,2) mit 11 Kugeln nicht entschieden“ heißt jetzt „nicht selbst entschieden (nach der Tabelle unmöglich)“.

## Belegarten nach dem Gate

| Aussage | Belegart |
|---|---|
| (15,4): kein 30er und keine reelle 15 × 15-Matrix über der Schwelle teilt 13 Zeilen mit `ctrl_16` oder `loesung111` (2 × 560 Tripel) | voll, Zertifikat je Fall, zwei Prüfer mit verschiedenen Verfahren (vorher zusätzlich SAT im Vortest); Paarbedingung als Herleitung |
| dasselbe für 12 gemeinsame Zeilen (2 × 1820 Fälle) | voll, Zertifikat je Fall, zwei Prüfer; im Vortest nur 8 Stichproben |
| ganze Spalte neu wählen: immer 233 (2 × 240 Fälle) | voll, drei Programme (Vortest nur Klasse 1, Sitzung, Gate) |
| kozyklisch n = 18, 20: 0 Überlebende bei Ziel 8, Bestwert genau 6 | Gegenbeispiel je Funktion, gespeichert, zwei Prüfer; Bestwert exakt mit drei Prüfprogrammen |
| kozyklisch n = 26, 28, 30: disc ≤ 8 | Gegenbeispiel je Funktion gefunden, nicht gespeichert (`g*`), zwei Programme |
| d = 9, U = Paley-12 voll, jedes W; U punktiert mit m2 = 10 | Herleitung plus Zahlen von zwei Programmen; m2 = 12, 14 offen |
| K = 10, 11 bei Typ (11,9) mit U aus Zeilen der linken Hälfte des 48ers | Herleitung (einzeilig) plus Aufzählung des Lemmas, zwei Programme |
| reelle 10 × 10-Matrizen ausgeschlossen | Herleitung (Astra, zweimal nachvollzogen) plus Aufzählung; Zehn-Kugel-Fall drei Programme, dazu Tabelle K(10,2) ≥ 24 |

## Nicht geprüft

- Die Suche nach den Zertifikaten (`kern_r3.c`) und das Sieb (`kozyk.c`): nicht nötig, geprüft sind die Belege.
- Die Anzahlen mit disc ≥ 6 außer Z40 und Z5×Q8; das Sieb für n = 26 bis 30; die exakte Schranke 36.
- Die Herleitungen in Teil 4 und 5 sind gelesen, nicht formalisiert.
- Weiter nicht gegengerechnet, weil nicht Gegenstand von P: `tern_ls`, CP-SAT und Glucose, verschachtelte Hamming-Hälften, Sols Schnitte zu K = 10.

## Hinweis zur Umgebung

`pip install python-sat ortools` wurde im Orchestrator-Container vom Berechtigungsfilter abgelehnt (anders als in der Übergabe vermerkt). Das Gate
kam ohne aus; statt eines SAT-Lösers eigene Tiefensuchen in C.
