# Laufprotokoll Auftrag C: Gegenrechnung der Ideenrunde vom 26.11.2026

Sitzung (Cloud, 4 Kerne), Branch `claude/auftrag-c-gegenrechnung-2xqmh5`. Auftrag: `notes/gegenrechnung_ideen_2026-11-26_auftrag.md`.
Alle Uhrzeiten aus `date` (UTC). Beginn 17:45 UTC. Eigener Prüfcode in `verifiers/gegenrechnung_ideen/`, Logs in `runs/gegenrechnung_ideen/`.
Programme des Orchestrators nicht gelesen, außer dem Kopfkommentar von `ersatz_k2.c` (vom Auftrag für die Definition der Bedingung genannt).

## Programme

| Datei | Zweck |
|---|---|
| `eingaben.py` | Sylvester 8/16/32, Paley I 12, Paley II 28, Konferenz 26 (GF(25)) und 30 selbst erzeugt; `ctrl_16` mit ergänzter Spalte, Hadamard 20, McKay-60 gelesen; alle auf H Hᵀ = N I geprüft |
| `gr.c` | ein C-Programm, Unterbefehle `tab` (exakte disc je Spaltenmenge, Gray-Code), `schwelle` (Tiefensuche: gibt es x mit max ≤ T?), `flach` (Histogramm max\|Ax\|), `allein`, `frei`, `freizeile`, `ersatz1`, `ersatz2`, `zirk` |

## Schritt 0 (17:52 UTC)

`sha256sum -c verifiers/SHA256SUMS`: alle drei OK. Eichung `gr flach` (Gray-Code) gegen `pruefe_matrix.py`: ctrl_8 5, ctrl_16 7, had20_klasse1 6,
had24_1p3_paley12 8, je gleich. Tiefensuche (`gr schwelle`) bei T = disc − 1 „kein x“, bei T = disc „x gefunden“, je richtig. Log `schritt0.log.txt`.

## Ergebnisse (eigene Zahl / Soll / gleich?)

| Punkt | eigene Zahl | Soll | gleich | Log |
|---|---|---|---|---|
| 1, Ordnung 8 Sylvester, c = 0..4 | 4 / 5 / 4 / 3 / 14× 2, 56× 4 | 4 / 5 / 4 / 3 / 14× 2, 56× 4 | ja | `p1_tabelle.log.txt` |
| 1, Ordnung 12 Paley | 4 / 5 / 4 / 3 / 4 | 4 / 5 / 4 / 3 / 4 | ja | ebd. |
| 1, Ordnung 16 Sylvester | 4 / 5 / 6 / 5 / 140× 4, 1680× 6 | ebenso | ja | ebd. |
| 1, Ordnung 16 aus `ctrl_16` + Transponierte | 6 / 7 / 6 / 5 / 56× 4, 3584× 6 | ebenso | ja | ebd. |
| 1, Ordnung 20 (3 Klassen + Transponierte) | 6 / 7 / 6 / 5 / 6, alle Fälle gleich | ebenso | ja | ebd. |
| 2, Ordnung 24 ohne 2 Spalten, disc ≥ 8 | 7420 von 33 120 (Tiefensuche); Gegenprobe Gray-Code: 25 700× 6, 7420× 8 | 7420 von 33 120 | ja | `p2_c2.log.txt` |
| 2, ohne 4 Spalten, disc ≥ 8 | 42 304 von 1 275 120; je Ausrichtung 21 152 = 8× 2640 + 16 + 6 + 6 + 4 | ebenso | ja | `p2_c4.log.txt` |
| 2, ohne 5 Spalten, disc ≥ 9 | 0 von 5 100 480 | 0 von 5 100 480 | ja | `p2_c5.log.txt` |
| 3, ohne 1 Spalte, d = 9 | Löcher 3520 bis 16 192 je Fall; freie ±1-Zeile: 0 von 2880 (Positivkontrollen `ctrl_16` ohne Zeile 233 Löcher, 16 von 16; 48er ohne Zeile 24 von 24) | 3520 bis 16 192; 0 von 2880 | ja | `p3_frei_c1.log.txt` |
| 4, Alleinanteil, 42 304 Fälle | 1264 bis 1354; erste McKay-Matrix 1297 bis 1300; nebenbei 0 unbedeckte x (Gray-Code bestätigt disc ≥ 8 aller 42 304) | 1264 bis 1354; 1297 bis 1300 | ja | `p4_allein.log.txt` |
| 5, Hadamard 20, eine Ersatzspalte | je Fall 20 064 flache x', 3021 Bedingungen (1140 Größe 3, 1881 Größe 4), 0 gute Spalten in 120 Fällen; Kontrolle an 3 Zufallsmatrizen 10 x 10 gegen direkte Zählung, Ziele 4 und 6: gleich | ebenso | ja | `p5_ersatz1.log.txt`, `p5_ersatz1_kontrolle.log.txt` |
| 5, zwei Ersatzspalten | 1272 flache x'' je Paar, 488 bis 600 Bedingungen je Gruppe, kritische Menge immer in einer Gruppe, alle 1140 Paare ausgeschlossen (Belegungen je Gruppe brute force über 4^10) | ebenso | ja | `p5_ersatz2.log.txt` |
| 6, Sylvester 32 ohne 1 Spalte | 13 693 568 flache x bei d = 11, 0 bei d = 9 | ebenso | ja | `p6_sylv32.log.txt` |
| 6, ohne 2 Spalten | 215 040 bei d = 10 | ebenso | ja | ebd. |
| 7, Konferenz 26 / 30 | disc 5 (260 x mit max ≤ 5) / disc 7 (45 588 x mit max ≤ 7) | 5 (260) / 7 (45 588) | ja | `p7_konf_zirk.log.txt` |
| 7, Zirkulant 15; Paley II 28 | größte disc 5 (975 von 16 384 ersten Zeilen; Kontrolle n = 7: 3); disc 6 | 5; 6 | ja | ebd. |

Block 1 (17:45 bis 17:59 UTC): Punkte 1 bis 5 und 7, Punkt 6 teilweise. Rechenzeit zusammen etwa 6 Minuten Wandzeit.
Hinweis zur zweiten Ersatzspalte: Die Positivkontrolle an Paley 12 ist leer (keine flachen x''), sie prüft nichts; geblieben ist die Struktur-
prüfung (keine kritische Menge über beide Gruppen) und die Kontrolle der Logik mit einer Spalte.
| 6, RM(1,5), Nebenklassen je Abstand 0..12 | 1, 32, 496, 4960, 35 960, 201 376, 906 192, 3 365 856, 10 119 795, 21 288 320, 22 064 064, 8 693 888, 427 924 (Summe 2^26) | ebenso | ja | `p6_rm15.log.txt` |
| 6, Abstand 12 | Spektren 219 604 / 208 320; nach Grad und Autokorrelation: Grad 2 868, Grad 3 166 656 (\|r\| 0^15 8^16) und 52 080 (\|r\| 0^27 16^4), Grad 4 208 320 | ebenso | ja | ebd. |
| 6, Stellen mit \|W\| ≥ 12 je gerader Nebenklasse | 0: 427 924, 1: 5 091 689, 2: 15 643 096, 3: 9 999 360, 4: 2 378 475, 6: 13 888 | ebenso | ja | ebd. |
| 8, Faserbedingung Kern ⟨1,a,b⟩ | 55 / 74 / 97 Typen; bei K = 5 ausgeschlossen 11 / 4 / 1; bei (20,6) \|D\| 31 180 bis 53 372 (Schranke 2,46 bis 4,20) | 55 / 74 / 97; 11 / 4 / 1 | ja | `p8_fasern.log.txt` |
| 9, Verdopplungszellen | dieselben zwölf Zellen mit denselben Tabellenwerten; CSV am 26.11. 18:00 UTC neu abgerufen, SHA-256 gleich `5efaf8d9…` | zwölf Zellen aus 3e | ja | `p9_zellen.log.txt` |

Randbemerkung zu Punkt 9: Nach der Regel m + 1 ≤ min(B, 33) käme bei d = 11 auch (32,11) in Frage; die Zeile fehlt in der CSV, also nicht entscheidbar (keine Abweichung, die Notiz nennt sie nicht).
Zur Faserbedingung: Definition aus Sols Beschreibung (Paritätsklassen modulo Bild des Kerns, ganzzahlige Belegung mit Summe K), nicht aus `kern3_fasern.py`.
