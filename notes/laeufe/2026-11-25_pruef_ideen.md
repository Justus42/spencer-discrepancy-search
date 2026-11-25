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
| Klasse 2 (`loesung111`), 4 gestrichen, 3 Ersatzzeilen (1820 Fälle; Zusatz, nicht verlangt) | Lochzahlen 1476 (1120), 1492 (672), 1512 (28); **1820 von 1820 Zertifikate bestätigt**; Kerngröße 13 bis 20; kein Fall verteilbar | `klasse2_r4_k3_zertifikate.txt`, `klasse2_r4_k3_pruefung.log.txt` |

Beobachtungen:
- Die Zertifikate sind kleiner als im Vortest (6 bis 7 Klassen statt 7 bis 10), weil je Tripel 60 Neustarts mit Ausdünnen laufen.
- Beim 4-gegen-3-Fall fand der erste Lauf für Index 1223 (gestrichen 3, 6, 9, 13) keinen Kern, weil ein Neustart die Teilmengengrenze von 95 Löchern erreichte und der
  Code irrtümlich abbrach. Fehler im eigenen Suchcode behoben (jetzt weiter mit dem nächsten Neustart), Index 1223 mit 200 Neustarts nachgerechnet (Kern 15), die Zeile in
  `klasse1_r4_k3_zertifikate_final.txt` ersetzt. `klasse1_r4_k3_zertifikate.txt` ist der Rohstand mit der Zeile „VERTEILBAR“. Keine Abweichung der Aussage.
- Sicht der Aussage: ausgeschlossen ist, dass 13 Zeilen (bzw. 12 bei 4/3) mit einer der beiden 32er-Klassen gemeinsam sind. Nicht gerechnet: 5 gestrichene Zeilen gegen 4 Ersatzzeilen.

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

## Teil 4: Zahlen der Zerlegung (`verifiers/pruef_ideen/zerlegung_zahlen.py`, Log `runs/pruef_ideen/zerlegung/zerlegung_zahlen.log.txt`)

Eigenes Programm (nur Ganzzahlen; Paley-12 selbst aus GF(11) gebaut, H·Hᵀ = 12·I geprüft). Rechenzeit insgesamt unter 10 Sekunden. Alle Zahlen stimmen:

| Aussage | Eigene Rechnung | Ergebnis |
|---|---|---|
| Paley-12 voll: (6³,2⁹) | 880 Klassen, 220 verschiedene Dreiermengen | bestätigt |
| (4⁹,0³) | 220 Klassen, 220 Dreiermengen | bestätigt |
| übrige Profile | 948 Klassen (792 mit (8,4⁵,0⁶), 144 mit (10,2¹¹), 12 mit (12,0¹¹)), alle mit einem Betrag >= 8 | bestätigt |
| Schnitt B(x,2) ∩ B(y,4) in Q13, Abstand 0..6 | 92, 92, 92, 47, 47, 15, 15 (Durchzählen aller 8192 Punkte) | bestätigt |
| Bilanz m2 = 13 | vorhanden 24·1093 = 26 232, nötig 26 144, Spielraum 88 (m2 = 9: −872; m2 = 11: −1760) | bestätigt |
| überzählig / erlaubt | 24·11·15 = 3960 gegen 88 + 8·29 = 320 (D <= 29) | bestätigt (Rechnung) |
| Stichprobe zur Schranke Σ A(x)(B(x)−1) >= 3960 | 300 zufällige W (davon 100 mit eng liegenden Zeilen): kleinster Wert 4984 | kein Widerspruch |
| „Bedingung an W" gleichwertig zur tatsächlichen Überdeckung mit U = Paley-12 voll, d = 9 (m2 = 13) | 12 zufällige W, je 4096 Klassen z (19 973 erfüllt, 29 179 nicht): Bedingung = Überdeckung in 12 von 12 | bestätigt (Stichprobe) |
| Paley-12 ohne Spalte 0, Profile | (7,5²,3³,1⁶) 660, (5³,3⁶,1³) 220, (9,3⁵,1⁶) 132, (11,1¹¹) 12; 220 verschiedene 5er-Dreiermengen | bestätigt |
| Ketten C ⊆ B ⊆ A mit a + 3b + 6c <= 9 | 17 031 Ketten aufgezählt, **0** erfüllen alle 220 Muster | bestätigt |
| Kleinstes \|A\| bei \|B\| = 0, 1, 2, C leer | 10, 7, 6 | bestätigt |
| Summen m2 = 10 / 12 / 14 | 4920 gegen 5120 (ausgeschlossen); 25 980 gegen 20 480; 128 472 gegen 81 920 | bestätigt |
| Lemma: U des 48ers ohne eine Zeile, y = diese Zeile | alle übrigen \|u_i\| = 1: Paley-12 ohne Spalte (12 Spalten × 12 Zeilen) 0 Abweichungen; `controls/had24_1p3_paley12.txt`, linker 12 × 11-Block: 0 Abweichungen von 12 | bestätigt |

Nachvollzogen (Herleitung, nicht nur Zahlen): Bilanz m2 = 13: Σ_x B(x) = 24·1093 muss >= 4·(2^13 − |U|) + Σ_U B ≥ 26 144 + 3D sein (U = Vereinigung der kleinen Kugeln, |U| = 2208 − D); der
Überschuss Σ_U(B−1) ist höchstens 88 − 3D, die Mehrfachzählung kostet höchstens 11·D, zusammen höchstens 88 + 8D <= 320 (D <= 29), während jede der 24·11 Paarungen (kleine Kugel, fremde Zeile mit passendem
Vorzeichen, Abstand <= 6) mindestens 15 Punkte beisteuert. Schritte stimmen; die Zahlen sind jetzt von einem zweiten Programm bestätigt.

## Teil 5: Astras Ausschluss reeller 10 × 10-Matrizen (nur lesen und urteilen, mit kleinen Rechnungen)

Prüfskript `verifiers/pruef_ideen/astra10.py` (nur ganze Zahlen; reelle a als p/q), C-Gegenstück `src/pruef_ideen/ueberdeckung10.c` für S6; Logs `runs/pruef_ideen/astra10/`.
Schwelle t0 = 5·sqrt(10/8), t0² = 31,25; Kappe von Zeile a: P = {x : a·x > t0}, überdeckte Klassen {x, −x} genau die Klassen der Punkte von P.

| Schritt Astras | Prüfung | Ergebnis |
|---|---|---|
| S1 Durchmesser der Kappe <= 4 (a·(x+y) <= 2(10 − d), > 11,18) | Herleitung nachvollzogen; 20 000 Zufallskappen (reelle a, auch dünn besetzt, fast ±1, mit wenigen kleinen Einträgen): Durchmesser nie > 4 | stimmt |
| S2 Fehlermengen nach unten abgeschlossen, \|S ∪ T\| <= 4 für alle Paare (Durchmesserschranke auf S und T∖S) | Herleitung nachvollzogen (T∖S ⊆ T liegt wieder in der Familie, Abstand \|S\| + \|T∖S\| = \|S ∪ T\|); dieselben Kappen: 6307 mit Dreiermenge, alle Eigenschaften erfüllt | stimmt |
| S3 „enthält die Kappe eine Dreiermenge (keine Vierermenge), so höchstens 1 + 10 + 24 + 8 = 43 Punkte" | exakt aufgezählt: alle 410 Dreiermengen-Familien mit {0,1,2}, jeweils alle zulässigen Zweiermengen; Zweiermengen, die {0,1,2} treffen: 24 (= 45 − 21); **größte Familie 36 Mengen** (eine Dreiermenge plus alle 24 Zweiermengen, die sie treffen, plus 10 Einer und ∅); alle 6307 Zufallskappen mit Dreiermenge hatten <= 36 Punkte | **gilt (43 ist eine gültige, aber nicht scharfe Schranke; exakt 36)** |
| Enthält die Kappe eine Vierermenge, liegen alle Mengen darin (<= 16 Punkte) | Aufzählung \|S ∪ Q\| <= 4 ⇒ S ⊆ Q | stimmt |
| ohne Dreiermenge liegt die Kappe in der Radius-2-Kugel um das Vorzeichenmuster von a (56 Klassen) | trivial (alle Fehlermengen <= 2); Kugelgröße in Klassen: 56 | stimmt |
| Kugeln gleicher Gewichtsparität: gefalteter Abstand <= 4, Schnitt >= 6 | Durchzählen aller 512 × 511/2 Paare: gleiche Parität gefalteter Abstand 2 → 20, 4 → 6; (verschiedene Parität: 1 → 20, 3 → 6, 5 → 0) | stimmt, kleinster Schnitt 6 |
| Vereinigung <= 56(10−e) + 43e − 6·max(8−e,0) <= 505 < 512 für e >= 1 | e = 1..10: 505, 498, 491, 484, 477, 470, 463, 456, 443, 430, alle < 512; Folgerung aus der sequenziellen Zählung (höchstens zwei Kugeln, die erste je Parität, verlieren nichts) nachvollzogen | stimmt |
| e = 0: „Radius-2-Code mit höchstens 20 Wörtern, Widerspruch zu K(10,2) >= 24" | Die Schranke aus der Tabelle ist **nicht nötig**: exakte Tiefensuche (zwei Programme, C und Python, beide 63 505 Knoten, 0,1 s bzw. 5 s): **zehn Radius-2-Kugeln überdecken die 512 Klassen nicht**; Positivkontrolle: mit 16 Kugeln findet dasselbe C-Programm eine Überdeckung (3 s); K = 11 nach 100 s nicht entschieden (nicht benötigt) | stimmt, jetzt ohne Tabelle |

**Urteil: schlüssig.** Jeder Schritt ist nachvollzogen und mit einer Rechnung belegt; keine Lücke gefunden. Die Abhängigkeit von „K(10,2) >= 24 aus der Tabelle" entfällt, weil der Fall e = 0
direkt durch Aufzählung erledigt ist (zehn Kugeln reichen nicht); die Schranke 43 gilt, exakt sind 36. Vorbehalt: Die Vollständigkeit der Tiefensuche beruht auf drei Argumenten (Translationssymmetrie:
erste Kugel um Klasse 0; Verzweigung über die kleinste unüberdeckte Klasse; Zuwachsschranke), die ich im Code geprüft, aber nicht formal bewiesen habe; das Python- und das C-Programm
haben denselben Suchbaum (gleiche Knotenzahl). Die Aussage gilt für reelle Einträge in [−1,1], genau 10 Zeilen, Schwelle disc/√10 > 5/√8.
Für die Landkarte: n = 10 reell wie n = 8, 9, 12 als ausgeschlossen kennzeichnen (Belegart: Herleitung + Aufzählung, ein Programm je Sprache).

## Ende: Zusammenfassung, Abweichungen, Prüfdateien

- 25.11.2026 18:36 UTC (nach allen Läufen und nach den Nachträgen in `STATUS.md` und `LANDKARTE.md`): `sha256sum -c verifiers/SHA256SUMS`: `src/check_disc.py` OK, `verifiers/verify_spencer.py` OK, `src/exakt_disc.c` OK.
  Keine der drei Prüfdateien verändert. `python3 verifiers/test_verifiers.py`: 11 Tests OK (8 s). `main` unverändert gegenüber dem Start (kein Merge nötig); nichts nach `main` zusammengeführt, nichts nach außen gemeldet.
- Regel 1 für die besten kozyklischen Matrizen (disc 6, n = 18: Z36, Dic9, Z3×Dic3; n = 20: Z40, Z5×Q8 mit und ohne Hadamard, Dic10 Hadamard): `python3 verifiers/pruefe_matrix.py ... --erwartet 6`,
  alle drei Programme (`check_disc`, `verify_spencer`, `exakt_disc`) disc 6, Rückgabecode 0 (`runs/pruef_ideen/kozyklisch/beste/`).
- Rechenzeit (4 Kerne, alle Blöcke weit unter 30 Minuten): Teil 1 etwa 17 Min. Suche und 12 Min. Prüfung (davon 4/3 Klasse 1: 8 + 5 Min., Klasse 2 als Zusatz: 8 + 5 Min.), Teil 2 15 s,
  Teil 3 etwa 4 + 7 Min. Suche und 5 Min. Prüfung, Teil 4 unter 10 s, Teil 5 unter 10 s (Python) bzw. 0,1 s (C) für die entscheidende Suche. Gesamt etwa 1 h 20 Min. Wanduhr, Obergrenze (5 h, Teil 1 höchstens 3 h) bei weitem nicht erreicht.

### Abweichungen von den erwarteten Zahlen und sonstige Befunde

Alle Erwartungen des Auftrags wurden bestätigt. Ausdrücklich zu nennen:
1. **Zertifikatsgröße:** Im Vortest 7 bis 10 Klassen je Tripel, hier 6 oder 7 (60 Neustarts mit Ausdünnen). Aussage unverändert.
2. **Schranke 43 (Astra):** gültig, aber nicht scharf; exakt 36 (Teil 5). Der Schluss bleibt, und er ruht nicht mehr auf `K(10,2) >= 24` (Fall e = 0 direkt durch Aufzählung erledigt).
3. **Dic15, n = 30:** im Vortest nur zu 80 % gelaufen, hier vollständig (536 870 912 Funktionen, 0 Überlebende).
4. **Neue Zahlen ohne Vorgabe** (nicht Abweichung, nur Zusatz): n = 18: 4338 / 23 922 / 4752 Funktionen mit disc 6; n = 20: Dic10 55 400 (5600 Hadamard), Z5 × Z4 × Z2 900, Z5 × D4 und D20 keine; exakter Bestwert n = 18 und n = 20: 6.
   Zweite 32er-Klasse: 2 gestrichene Zeilen geben Lochzahl 554 / 578 (Klasse 1: 550 / 562); bei 4 gestrichenen 1476 / 1492 / 1512 (Klasse 1: 1476 / 1484 / 1508 / 1512). Nicht im Auftrag erwartet, kein Widerspruch.
5. **Fehler im eigenen Suchcode (behoben):** `kern_r3.c` meldete bei 4/3, Index 1223 „VERTEILBAR", weil ein Neustart die Teilmengengrenze erreichte und abbrach; mit 200 Neustarts Kern aus 15 Klassen, Zeile ersetzt, Rohdatei bleibt zum Vergleich liegen.
6. **Nicht gegengerechnet, weil nicht Gegenstand von Auftrag P** und deshalb weder bestätigt noch widerlegt: `tern_ls.c`-Kontrolle, CP-SAT/Glucose-Halbproblem, verschachtelte Hamming-Hälften (24 192 offene Klassen), Sols Schnitte zu K = 10 (Typ (11,9): „mindestens 6 Dreierzeilen" usw.),
   Zerlegung d = 9 mit m2 = 12, 14. Diese stehen in `STATUS.md` nur als „Rechnung des Orchestrators, nicht gegengerechnet" bzw. als offen.
7. **Grenzen der Aussagen:** (15,4): Die Zertifikate gelten für die Ersetzung von 3 Zeilen durch 2 und von 4 durch 3; mehr gestrichene Zeilen sind nicht gerechnet. Die Aussage „13 gemeinsame Zeilen" folgt aus 3/2, „12 gemeinsame" aus 4/3 (nur im Rahmen
   von genau 15 Zeilen, die Ersatzzeilen beliebig in [−1,1]^15). Kozyklisch n = 26–30: nur „disc <= 8" mit einem Programm, keine Gegenbeispiele gespeichert. Teil 4 (Zählschranken): Herleitungen nachvollzogen und Zahlen bestätigt, aber nicht formal bewiesen.

### Wo liegt was

- `src/pruef_ideen/`: `kern_r3.c` (+ `lauf_kern.sh`), `spalte.c`, `kozyk.c` (+ `lauf_kozyk.sh`), `ueberdeckung10.c`. `verifiers/pruef_ideen/`: `pruef_kern.py`, `pruef_kozyk.py` (+ `lauf_pruef_kozyk.sh`), `zerlegung_zahlen.py`, `astra10.py`.
- `runs/pruef_ideen/kern_r3/` (Zertifikate, Prüfprotokolle, Kontrollen), `spalte/`, `kozyklisch/` (Gegenbeispiele `*.bin.gz`, Zusammenfassung `ZUSAMMENFASSUNG.txt`, `beste/`), `zerlegung/`, `astra10/`.
