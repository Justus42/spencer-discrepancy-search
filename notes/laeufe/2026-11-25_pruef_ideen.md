# Auftrag P: Gegenrechnung der Vortests der Ideensammlung vom 25.11.2026 (Cloud, 4 Kerne)

Auftrag: `notes/gegenrechnung_ideen_2026-11-25_auftrag.md` (freigegeben von Justus 25.11.2026, 18:10 Uhr). Branch `claude/auftrag-p-gegenrechnung-s6wy5c`.
Eigene Programme in `src/pruef_ideen/` (Suche/Rechnung) und `verifiers/pruef_ideen/` (getrennte Prüfskripte, nur ganze Zahlen).
Skripte des Orchestrators (`notes/orchestrator/2026-11-25_ideen/`) wurden nicht gelesen und nicht verwendet. Belege: `runs/pruef_ideen/`.

## Start

- 25.11.2026 17:16 UTC: `sha256sum -c verifiers/SHA256SUMS`: `src/check_disc.py` OK, `verifiers/verify_spencer.py` OK, `src/exakt_disc.c` OK.
- Hinweis zur Uhrzeit: Die Freigabe nennt 18:10 Uhr (Ortszeit des Orchestrators); die Containeruhr zeigt UTC.

## Teil 1: (15,4), Zeilen des 32ers durch weniger Ersatzzeilen in [-1,1]^15 ersetzen

Programme: `src/pruef_ideen/kern_r3.c` (Suche, Zertifikate; Starter `lauf_kern.sh`), `verifiers/pruef_ideen/pruef_kern.py` (getrennter Prüfer, nur ganze Zahlen).
Belege: `runs/pruef_ideen/kern_r3/`. Matrizen: `controls/ctrl_16.txt` (erste 15 Spalten) = Klasse 1; `runs/gegenrechnung15/loesung111_16x15.txt` = Klasse 2.

**Herleitung (selbst nachvollzogen):** Schwelle T = 5·sqrt(15/8) = 6,847. Aus a·x > T und a·y > T folgt a·(x+y) > 13,69, und a·(x+y) <= 2·(15 − d(x,y));
also 15 − d >= 7, d <= 8. Lochklassen: Klassen {x,−x}, die von den behaltenen Zeilen nicht mit Betrag >= 7 getroffen werden (Abstand zu jeder Zeile in 5..10).
Zwei Vertreter x, y (Abstand D) mit Orientierungen: gleiches Vorzeichen gibt Abstand D, verschiedenes 15 − D. Also D <= 6 erzwingt gleiches, D >= 9 verschiedenes
Vorzeichen, D = 7, 8 frei. Zertifikat = Lochklassen, die sich schon allein nicht auf k Zeilen mit Vorzeichen verteilen lassen. Das ist eine notwendige Bedingung;
ihre Verletzung schließt alle k Ersatzzeilen in [−1,1]^15 aus (auch ternäre und binäre).

**Prüfer (getrennt, Python):** (1) jede Kernklasse ist Loch (ganze Skalarprodukte |<r,x>| <= 5 gegen alle behaltenen Zeilen), (2) vollständige Aufzählung aller Verteilungen
auf k Zeilen mit Vorzeichen (Klasse für Klasse, Zweige mit verletzter Paarbedingung abgeschnitten; die erste Klasse wird auf (Zeile 0, Vorzeichen 0) gesetzt:
Zeilenvertauschung und Vorzeichenwechsel einer ganzen Zeile sind Symmetrien), keine darf übrig bleiben, (3) Lochzahl unabhängig neu gezählt.
Negativkontrollen des Prüfers: Streichen eines Kernelements macht jedes Zertifikat verteilbar (560 von 560), eine eingesetzte Nicht-Lochklasse wird immer erkannt.

### Ergebnisse (Rechenzeit gesamt wenige Minuten; kein Block über 30 Minuten)

| Fall | Ergebnis | Beleg |
|---|---|---|
| Klasse 1, 2 gestrichen, 2 Ersatzzeilen (120 Paare) | Lochzahl 550 (64 Paare), 562 (56): wie erwartet. Mit den Originalzeilen als Ersatz: für alle 120 Paare jede Lochklasse abgedeckt, keine Paarbedingung verletzt; Suche findet Verteilungen | `klasse1_r2_k2_kontrolle.txt`, `pruefskript_kontrollen.log.txt` |
| Klasse 1, 2 gestrichen, 1 Ersatzzeile | alle 120 Paare Zertifikat mit 3 Klassen, vom Prüfer bestätigt | `klasse1_r2_k1_kontrolle.txt`, `klasse1_r2_k1_pruefung.log.txt` |
| **Klasse 1, 3 gestrichen, 2 Ersatzzeilen (560 Tripel)** | Lochzahl 967 (448), 987 (112): wie erwartet. **560 von 560 Zertifikate, vom Prüfer bestätigt**; Kerngröße 6 (475 Tripel) oder 7 (85) | `klasse1_r3_k2_zertifikate.txt`, `klasse1_r3_k2_pruefung.log.txt` |
| **Klasse 2, 3 gestrichen, 2 Ersatzzeilen (560 Tripel)** | Lochzahl 967 (448), 987 (112): wie erwartet. **560 von 560 bestätigt**; Kerngröße 6 (477) oder 7 (83) | `klasse2_r3_k2_zertifikate.txt`, `klasse2_r3_k2_pruefung.log.txt` |
| Klasse 2, 2 gestrichen, 2 Ersatzzeilen / 1 Ersatzzeile | Lochzahl 554 (112 Paare), 578 (8): beide verteilbar mit zwei Zeilen (Kontrolle); mit einer Zeile 120 von 120 Zertifikate | `klasse2_*` |
| **Klasse 1, 4 gestrichen, 3 Ersatzzeilen (1820 Fälle)** | Lochzahlen 1476 (784), 1484 (896), 1508 (112), 1512 (28): wie erwartet. **1820 von 1820 Zertifikate bestätigt**; Kerngröße 12 bis 21 | `klasse1_r4_k3_zertifikate_final.txt`, `klasse1_r4_k3_pruefung.log.txt` |

Beobachtungen:
- Die Zertifikate sind kleiner als im Vortest (6 bis 7 Klassen statt 7 bis 10), weil je Tripel 60 Neustarts mit Ausdünnen laufen.
- Beim 4-gegen-3-Fall fand der erste Lauf für Index 1223 (gestrichen 3, 6, 9, 13) keinen Kern, weil ein Neustart die Teilmengengrenze von 95 Löchern erreichte und der
  Code irrtümlich abbrach. Fehler im eigenen Suchcode behoben (jetzt weiter mit dem nächsten Neustart), Index 1223 mit 200 Neustarts nachgerechnet (Kern 15), die Zeile in
  `klasse1_r4_k3_zertifikate_final.txt` ersetzt. `klasse1_r4_k3_zertifikate.txt` ist der Rohstand mit der Zeile „VERTEILBAR“. Keine Abweichung der Aussage.
- Sicht der Aussage: ausgeschlossen ist, dass 13 Zeilen (bzw. 12 bei 4/3) mit einer der beiden 32er-Klassen gemeinsam sind. Nicht gerechnet: 4/3 für Klasse 2; 5 gestrichene.

