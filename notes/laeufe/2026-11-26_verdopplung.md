# Laufprotokoll Auftrag V: Verdopplungszellen mit dem vorhandenen Sucher (26.11.2026)

Auftrag: `notes/verdopplung_2026-11-26_auftrag.md` (Entwurf des Orchestrators, 26.11.2026). Branch `claude/auftrag-v-q1vany`, Basis `main` = `e323707`.
Zeiten aus `date` (UTC; MEZ = UTC + 1). Maschine: Cloud-Container, 4 Kerne, 15 GB. Vier Startwerte laufen parallel, je einer auf einem Kern.
Sucher: `src/evo/p018.c` unverändert, Bewerter `src/evo_bewertung.c` unverändert. Treiber (bash): `src/run_verdopplung.sh`.
Archiv aller Läufe: `runs/verdopplung/archiv.tsv` (Block, Uhrzeit, m, R, Paare, Startwert, Sekunden, offen, Anteil, Schritte/s, Speicher MB, Kommentar).
Ausgaben `runs/verdopplung/out/`, Logs `runs/verdopplung/logs/` und `runs/verdopplung/block_*.log.txt`. Belegart: Bewerter (exakt, alle 2^(m-1) Klassen);
nicht erreichte Ziele: „Suche erfolglos“, schließt nichts aus. Nichts installiert.

## Schritt 0: Kontrollen (Block 1, 17:45–17:54 UTC)

a) `sha256sum -c verifiers/SHA256SUMS`: alle drei OK. `sha256sum -c runs/evo/BEWERTER.sha256`: OK (17:45 UTC, `runs/verdopplung/sha256_start.log.txt`).
b) 16 Paare, Startwerte 1–4: (19,6) 4 von 4 mit 0 offen (je 1 s); (17,5) 4 von 4 mit 0 offen (je 1 s). **Bestanden.**
c) 24 Paare, Startwerte 1–4, je 300 s: (22,7) 4 von 4 mit 0 offen (nach 178, 81, 34, 81 s); (24,8) 1 von 4 mit 0 offen (181 s), die anderen 559 / 524 / 568 offen. **Bestanden** (Soll mindestens 1 von 4).
   Bauregel: `controls/had24_1p3_paley12.txt`, Spalte 1 verdreifacht (zwei Kopien angehängt) = 24 x 22, Spalten 1 und 2 verdreifacht = 24 x 24; Bewerter (22,7): offen 0 von 2 097 152, (24,8): offen 0 von 8 388 608. **Bestanden.**
   (Dateien `runs/verdopplung/bauregel_24x22.txt`, `bauregel_24x24.txt`, Logs `bauregel_*.log.txt`.)

## Schritt 1: Abstandsprobe am Ziel (17:54 bis 18:08 UTC)

Je Zelle Zielpaarzahl, Startwerte 1–4. „Anteil“ = offene Klassen / 2^(m−1). Zahlen laut Bewerter.

| Zelle | Paare | Zeit je Lauf | offene Klassen (Startwerte 1–4) | Anteil | Schritte/s | Speicher |
|---|---|---|---|---|---|---|
| (19,6) | 10 | 120 s | 26 192 / 26 192 / 26 192 / 26 192 | 0,0999 | ca. 615 | 6 MB |
| (17,5) | 10 | 120 s | 8952 / 8952 / 8952 / 8952 | 0,1366 | ca. 2070 | 2 MB |
| (22,7) | 12 | 300 s | 234 512 / 234 634 / 234 668 / 234 589 | 0,1119 | ca. 82 | 36 MB |
| (24,8) | 12 | 300 s | 732 766 / 732 821 / 733 487 / 733 510 | 0,0874 | ca. 20 | 144 MB |

Die ersten beiden Zahlen sind genau die Probe des Orchestrators (26 192 und 8952): fester Endpunkt, nicht streuend. Bei (22,7) und (24,8) streuen die Zahlen um 0,1 %.
Der Abstand zum Ziel ist groß: etwa 9 bis 14 % aller Klassen bleiben offen; ein Treffer ist bei diesen Paarzahlen mit dieser Suche nicht in Sicht.

## Schritt 2: Leiter abwärts, (19,6) und (17,5) (Block 2, 18:09–18:33 UTC)

Stufe je 4 Startwerte à 120 s; nach zwei nicht erreichten Stufen in der letzten einmal 8 Startwerte à 240 s (Startwerte 1–4 und 5–8, je 4 parallel).
Alle Zahlen laut Bewerter, alle Läufe einer Stufe enden beim selben Wert (fester Endpunkt, kein Streuen).

| Zelle | Paare | Ergebnis | offene Klassen | Anteil |
|---|---|---|---|---|
| (19,6) | 16 | erreicht (Kontrolle, 4 von 4) | 0 | 0 |
| (19,6) | 15 | nicht erreicht (4 von 4 bei 2236) | 2236 | 0,0085 |
| (19,6) | 14 | nicht erreicht (4 x 120 s, dann 8 x 240 s: alle 12 Läufe 5312) | 5312 | 0,0203 |
| (19,6) | 10 (Ziel) | nicht erreicht | 26 192 | 0,0999 |
| (17,5) | 16 | erreicht (Kontrolle, 4 von 4) | 0 | 0 |
| (17,5) | 15 | nicht erreicht (4 von 4 bei 722) | 722 | 0,0110 |
| (17,5) | 14 | nicht erreicht (4 x 120 s, dann 8 x 240 s: alle 12 Läufe 1710) | 1710 | 0,0261 |
| (17,5) | 10 (Ziel) | nicht erreicht | 8952 | 0,1366 |

Zelle (19,6) und (17,5) beendet (Regel: zwei Stufen nacheinander nicht erreicht, dann 8 Startwerte mit doppelter Zeit in der letzten, ohne Treffer).
Kleinste erreichte Paarzahl in beiden Zellen: 16 (32 Wörter), das ist der bekannte Tabellenwert; kein Tabellenwert darunter.

## Schritt 2, (22,7) (Block 2, 18:33–19:03 UTC)

Stufe je 4 Startwerte à 300 s; nach zwei nicht erreichten Stufen 8 Startwerte à 600 s bei 22 Paaren (zwei Durchgänge à 4 Läufe, Hintergrundjob wegen des 10-Minuten-Limits der Werkzeugaufrufe; keine Abfrage zwischendurch).
Hinweis: Die Läufe mit gleichem Namen überschreiben Ausgabe und Log der kürzeren Läufe (Startwerte 1–4 bei 22 Paaren und bei 14 Paaren in (19,6)/(17,5)); die Zahlen aller Läufe stehen unverändert in `archiv.tsv`.

| Paare | Ergebnis | offene Klassen (Startwerte) | Anteil |
|---|---|---|---|
| 24 | erreicht (Kontrolle, 4 von 4) | 0 | 0 |
| 23 | nicht erreicht | 616 / 759 / 666 / 657 | 0,0003 |
| 22 | nicht erreicht (4 x 300 s) | 714 / 742 / 860 / 648 | 0,0003 |
| 22 | 8 Startwerte à 600 s | 714 / 722 / 694 / 648 / 714 / 736 / 648 / 668 | 0,0003 |
| 12 (Ziel) | nicht erreicht | 234 512 bis 234 668 | 0,1119 |

(22,7) beendet. Kleinste erreichte Paarzahl: 24 (48 Wörter, Kontrolle, entspricht der Bauregel), beste Restzahl darunter: 616 bei 23 Paaren (mit nur einer Messung à 300 s; 22 Paare: 648).

## Schritt 2, (24,8) (Block 2, 19:04–19:34 UTC)

Stufe je 4 Startwerte à 300 s; nach zwei nicht erreichten Stufen 8 Startwerte à 600 s bei 22 Paaren (zwei Durchgänge, Hintergrundjob, nicht abgefragt).
Bei (24,8) ca. 100 Schritte/s, 140 MB; auch die Kontrolle (24 Paare) gelang nur in 1 von 4 Läufen.

| Paare | Ergebnis | offene Klassen (Startwerte) | Anteil |
|---|---|---|---|
| 24 | erreicht (Kontrolle, 1 von 4; Bauregel 0 offen) | 0 / 559 / 524 / 568 | 0 bis 0,0001 |
| 23 | nicht erreicht | 2407 / 2400 / 2471 / 2444 | 0,0003 |
| 22 | nicht erreicht (4 x 300 s) | 2448 / 2577 / 2748 / 2493 | 0,0003 |
| 22 | 8 Startwerte à 600 s | 2448 / 2577 / 2634 / 2493 / 2289 / 2793 / 2064 / 3123 | 0,0002 bis 0,0004 |
| 12 (Ziel) | nicht erreicht | 732 766 bis 733 510 | 0,0874 |

(24,8) beendet. Kleinste erreichte Paarzahl: 24 (48 Wörter, Kontrolle); beste Restzahl darunter: 2064 bei 22 Paaren (23 Paare: 2400).

## Auswertung (19:34–19:36 UTC laut `date`, ohne Zuruf)

**Prüfungen.** `sha256sum -c verifiers/SHA256SUMS` (drei Prüfdateien) und `sha256sum -c runs/evo/BEWERTER.sha256`: alle vier OK, am Anfang (17:45 UTC) und am Ende (19:34 UTC)
(`runs/verdopplung/sha256_start.log.txt`, `sha256_ende.log.txt`). Kein Treffer, daher kein Treffer-Bericht und kein Aufruf von `pruefe_matrix.py`.
Alle 80 am Ende noch gespeicherten Ausgaben (letzter Lauf je Zelle, Stufe und Startwert) mit dem Bewerter noch einmal gezählt: 0 Abweichungen vom Archiv
(`runs/verdopplung/nachzaehlung.log.txt`). Die kürzeren Läufe, die von gleichnamigen längeren überschrieben wurden (Startwerte 1–4 bei (22,7) und (24,8) mit 22 Paaren,
bei 14 Paaren in (19,6) und (17,5)), sind nur über `archiv.tsv` belegt, nicht nachgezählt.
`main` geholt (Stand `fe524e4`) und in den Branch gemergt; STATUS.md (ein Negativbefund, vier Fehlversuche, eine Dateizeile; Stand-Zeilen unverändert) und
LANDKARTE.md (eine Zeile) ergänzt. Nicht nach `main` zusammengeführt. Nichts installiert, nichts nach außen gemeldet. `src/evo/p018.c`, `src/evo_bewertung.c` und die drei Prüfdateien unverändert.

**Ergebnis je Zelle (Bewerter, Belegart: Suche erfolglos, schließt nichts aus):**

| Zelle | Ziel (Paare) | kleinste erreichte Paarzahl | beste Restzahl darunter | am Ziel offen |
|---|---|---|---|---|
| (19,6) | 10 (20 x 20, disc 8) | 16 (Tabellenwert 32 Wörter, kein neuer Wert) | 15 Paare: 2236 | 26 192 von 262 144 (0,100) |
| (17,5) | 10 (20 x 18, disc 8) | 16 (Tabellenwert) | 15 Paare: 722 | 8952 von 65 536 (0,137) |
| (22,7) | 12 (24 x 23, disc 9) | 24 (48 Wörter, Bauregel) | 23 Paare: 616 | ca. 234 600 von 2 097 152 (0,112) |
| (24,8) | 12 (24 x 25, disc 9) | 24 (nur 1 von 4 blind, Bauregel gelingt) | 22 Paare: 2064 | ca. 733 000 von 8 388 608 (0,087) |

**Deutung (unbelegt).** (19,6) und (17,5): schon eine Paarzahl unter der Kontrolle bleibt bei einem festen Endpunkt hängen (alle Startwerte gleich), der Abstand zum Ziel
wächst von 15 zu 14 zu 10 Paaren stetig; die Probe des Orchestrators (26 192, 8952) ist bestätigt. Bei (22,7) und (24,8) streuen die Werte, ändern sich aber von 23 zu 22 Paaren kaum
(616/648, 2400/2064): wie bei Auftrag E (704 bei 23 Paaren in (20,6)) ein Plateau knapp unter der Kontrolle. Die Erwartung des Orchestrators (kein Rekord) trifft zu;
„Tabellenwerte knapp unter 32 Wörtern“ bei (19,6) und (17,5) wurden nicht gefunden (15 Paare nicht erreicht).

**Rechenzeit.** 17:45 bis 19:34 UTC (1 h 49 min Wanduhr, `date`) von 3 h Obergrenze. Blöcke: 17:45–18:08 (Schritte 0, 1), 18:09–18:33 ((19,6), (17,5)), 18:33–19:03 ((22,7)), 19:04–19:34 ((24,8)); nach jedem Block commit und push.
