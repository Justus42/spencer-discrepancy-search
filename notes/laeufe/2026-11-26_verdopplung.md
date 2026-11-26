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
