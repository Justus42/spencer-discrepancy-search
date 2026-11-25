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

## Teil 2: ganze Spalte neu wählen (`src/pruef_ideen/spalte.c`, Belege `runs/pruef_ideen/spalte/`)

Verfahren: 16 × 15 (`ctrl_16` ohne Nullspalte) ohne Zeile r, Spalte c verdeckt. Klassen {x,−x} mit x_c = +1 (2^14 Stück); Zeile i liefert s_i + a_i, offen für die Ergänzung a
genau dann, wenn alle |s_i + a_i| <= 5 (bei |s_i| <= 4 frei, s_i = ±6 erzwingt a_i, |s_i| >= 8 nie offen). Alle 2^15 Ergänzungen exakt bewertet (Zählung über Klassenmasken).
Unabhängige Gegenprobe im selben Programm: direkte Zählung über die ganze Matrix (alle Klassen, alle Zeilen) für die echte Spalte und 40 Zufallsspalten je Fall, alle stimmen.

| Fall | Ergebnis |
|---|---|
| Kontrolle, alle 16 Zeilen, je 15 verdeckte Spalten (2^16 Ergänzungen) | Minimum 0, 4 Optima, echte Spalte darunter: 15 von 15 |
| **16 gestrichene Zeilen × 15 Spalten = 240 Fälle** | **Minimum 233 in allen 240 Fällen, 4 Optima, echte Spalte (233) optimal, kein Wert unter 233; 240 von 240 direkte Gegenproben OK** |
| Zusatz (nicht verlangt), zweite 32er-Klasse (`loesung111`) | Kontrolle Minimum 0 (4 Optima, echte Spalte darunter, 15 von 15); 240 Fälle alle Minimum 233, 4 Optima, echte Spalte optimal |

Rechenzeit 13 s (240 Fälle) bzw. 1 s (Kontrolle). Erwartung erfüllt, keine Abweichung.

## Teil 3: kozyklische Matrizen (`src/pruef_ideen/kozyk.c`, Prüfer `verifiers/pruef_ideen/pruef_kozyk.py`, Belege `runs/pruef_ideen/kozyklisch/`)

Aufbau: Gruppe E der Ordnung 2n mit zentraler Involution z, f ungerade, M[g][h] = f(g⁻¹h) über den Nebenklassenvertretern (kleinerer Index von {g, zg}), f(e) = +1:
2^(n−1) Funktionen je Gruppe. Gruppen im Suchprogramm über Indexformeln (CYC, DIC = dizyklisch, DIH = dihedral, Direktprodukte), im Prüfer unabhängig als
Matrizengruppen (monomiale 2×2-Matrizen über Einheitswurzeln, Matrixprodukt), mit Gruppengesetz-, Zentrums- und Elementordnungsprüfung (`gruppen_struktur_n8_bis_20.log.txt`).
Gegenprobe der Zeilen: für je 3004 Funktionsnummern je Gruppe (alle 22 Gruppen) liefert das Suchprogramm genau dieselben Zeilen wie die Matrix des Prüfers
(`zeilenvergleich.log.txt`; dabei auch der Maskenweg des Siebs gleich; Gray-Code-Fortschreibung gegen frischen Aufbau 5 Mio. Schritte ohne Abweichung).

**Kontrollzahlen des eigenen Programms** (Sieb Stufe 2, Überlebende exakt mit `build/exakt_disc`; `kontrollzahlen.log.txt`): negazyklisch n = 8: **64 von 128** mit disc >= 4;
n = 10: **270 von 512**; Z3 × Q8 (Williamson-Anordnung, n = 12): **336 von 2048** (darunter 96 Hadamard-Matrizen). Alle drei wie erwartet.

**n = 18 und 20 (Ziel 8): je Funktion ein gespeichertes Gegenbeispiel x mit max_i |<M_i,x>| <= 6** (Dateien `GRUPPE_nN.shardS.bin.gz`, 3 Bytes je Funktion, Funktionsnummer = Position;
die Prüfung liest sie und rechnet jede Funktion mit eigener Matrix nach).

| Gruppe | n | Funktionen (f(e)=+1) | ohne Gegenbeispiel (<= 6) | ohne x mit max <= 4 (disc >= 6) | exakte disc dieser | Hadamard | Prüfung der Gegenbeispiele |
|---|---|---|---|---|---|---|---|
| Z36 | 18 | 131 072 | **0** | 4 338 | alle 6 | 0 | bestätigt |
| Dic9 | 18 | 131 072 | **0** | 23 922 | alle 6 | 0 | bestätigt |
| Z3 × Dic3 | 18 | 131 072 | **0** | 4 752 | alle 6 | 0 | bestätigt |
| Z40 | 20 | 524 288 | **0** | 28 560 | alle 6 | 0 | bestätigt |
| Z5 × Q8 | 20 | 524 288 | **0** | **3 240** | alle 6 | **480** | bestätigt |
| Z5 × D4 | 20 | 524 288 | **0** | 0 | – | 0 | bestätigt |
| Z5 × Z4 × Z2 (z = (0,2,0)) | 20 | 524 288 | **0** | 900 | alle 6 | 0 | bestätigt |
| Dic10 | 20 | 524 288 | **0** | 55 400 | alle 6 | 5 600 | bestätigt |
| D20 | 20 | 524 288 | **0** | 0 | – | 0 | bestätigt |

Erwartungen erfüllt: 0 Überlebende bei Ziel 8, Z5 × Q8: 3240 Funktionen mit disc >= 6 darunter 480 Hadamard-Matrizen, Z40: 28 560 ohne Hadamard-Matrix.
**Exakter Bestwert (`src/exakt_disc.c`): n = 18: 6 (Quotient 1,414), n = 20: 6 (1,342).** Beide Seiten gerechnet: die Überlebenden (Matrizen des Suchprogramms)
und noch einmal alle Überlebenden mit den Matrizen des Prüfers, dazu je Gruppe 300 zufällige Nicht-Überlebende (alle mit disc <= 4, `-g 6`).
Neue Zahlen (nicht im Vortest): n = 18: 4338 / 23 922 / 4752; n = 20: Dic10 55 400 (5 600 Hadamard), Z5 × Z4 × Z2 900, Z5 × D4 und D20 keine mit disc >= 6.
Rechenzeit: n = 18, 20 zusammen etwa 4 Minuten Suche plus 4,5 Minuten Prüfung.

**n = 26, 28, 30 (Ziel 10), Sieb ohne gespeicherte Gegenbeispiele** (Stufe 8, höchstens 20 000 Zufallsproben je Funktion; `ZUSAMMENFASSUNG.txt`):

| n | Gruppen | Funktionen je Gruppe | Ergebnis |
|---|---|---|---|
| 26 | Z52, Dic13 | 33 554 432 | je Funktion ein x mit max <= 8 gefunden, **0 Überlebende** |
| 28 | Z56, Z7×Q8, Z7×D4, Z7×Z4×Z2, Dic14, D28 | 134 217 728 | **0 Überlebende** in allen sechs |
| 30 | Z60, **Dic15 vollständig** (536 870 912, im Vortest 80 %) | 536 870 912 | **0 Überlebende** in beiden |

Rechenzeit insgesamt etwa 7 Minuten. Kontrolle des Siebs (Siebweg auf n = 20 erzwungen, Stufe 4 mit 20 000 Proben): Z5×Q8 4074 Überlebende (exakt 3240), Z40 63 109 (28 560), Dic10 108 401 (55 400),
also Obermenge der exakten Überlebenden, wie es sein muss; Stufe 6: 0 Überlebende (`kontrolle_siebpfad.log.txt`). Für n = 26 bis 30 ist nur „disc <= 8" belegt (Quotient <= 1,569 / 1,512 / 1,461), kein exakter Bestwert.

