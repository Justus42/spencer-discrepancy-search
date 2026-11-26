# Auftrag C (Entwurf des Orchestrators, 26.11.2026, 18:39 Uhr; gilt erst nach Freigabe durch Justus): Gegenrechnung der Ideenrunde vom 26.11.

Grundlage: `notes/ideen_2026-11-26.md` (berichtigte Fassung) und `notes/ideen_2026-11-26_pruefvermerk.md`. Alle Zahlen dort stammen
aus Programmen des Orchestrators, je eines, und sind nach Projektregel von einer Sitzung gegenzurechnen. Erst danach kommen die Nachträge
aus Abschnitt 7 der Notiz in `STATUS.md` und `LANDKARTE.md`. Kein Rekordweg.

```
Auftrag C (Sonnet): Gegenrechnung mit eigenen Programmen. Kein Suchcode.

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-26.md und notes/ideen_2026-11-26_pruefvermerk.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_gegenrechnung_ideen.md, Logs runs/gegenrechnung_ideen/ als *.log.txt, Prüfcode nach
verifiers/gegenrechnung_ideen/. Nichts installieren. Uhrzeiten nur mit date. Blöcke von höchstens 30 Minuten, danach Protokoll, commit, push.
Eigene Programme schreiben; die Programme in notes/orchestrator/2026-11-26_ideen/ und notes/orchestrator/2026-11-26_gegenpruefung/
erst lesen, wenn eine Zahl abweicht, um die Ursache zu finden. Eingaben: controls/ (Hadamard 20 und 24, ctrl_16), Sylvester und Paley
selbst erzeugen und auf Orthogonalität prüfen.

Schritt 0: sha256sum -c verifiers/SHA256SUMS. Eichung des eigenen Diskrepanzprogramms an controls/ (ctrl_8: 5, ctrl_16: 7,
  had20_klasse1: 6, had24_1p3_paley12: 8) gegen python3 verifiers/pruefe_matrix.py.

Sollzahlen (je Punkt: eigene Zahl, Sollzahl, gleich oder nicht):
  1. Exakte Diskrepanz für alle Mengen von c gestrichenen Spalten, c = 0 bis 4:
     Ordnung 8 (Sylvester): 4 / 5 / 4 / 3 / 14-mal 2 und 56-mal 4.   Ordnung 12 (Paley): 4 / 5 / 4 / 3 / 4.
     Ordnung 16 Sylvester: 4 / 5 / 6 / 5 / 140-mal 4 und 1680-mal 6.
     Ordnung 16 aus ctrl_16 (fehlende Spalte ergänzen) und Transponierte: 6 / 7 / 6 / 5 / 56-mal 4 und 3584-mal 6.
     Ordnung 20 (3 Klassen und Transponierte): 6 / 7 / 6 / 5 / 6, in allen Fällen gleich.
  2. Ordnung 24 (60 Klassen und Transponierte): ohne 2 Spalten disc >= 8 in 7420 von 33 120 Fällen; ohne 4 Spalten disc >= 8 in
     42 304 von 1 275 120 Fällen (je Ausrichtung 21 152: acht Klassen mit 2640, je eine mit 16, 6, 6 und 4);
     ohne 5 Spalten disc >= 9 in 0 von 5 100 480 Fällen.
  3. Ordnung 24 ohne 1 Spalte, d = 9: zwischen 3520 und 16 192 flache x (bis auf Vorzeichen) je Fall; mit einer freien Zeile lösbar: 0 von 2880.
  4. Alleinanteil je Zeile für alle 42 304 Fälle aus Punkt 2: zwischen 1264 und 1354; für die erste Matrix der Datei controls/had24_mckay_hex.txt 1297 bis 1300.
  5. Hadamard 20 ohne 1 Spalte, eine neue Spalte: je Fall 20 064 flache x' und 3021 verschiedene Bedingungen (1140 der Größe 3,
     1881 der Größe 4); gute Ersatzspalten: 0 in allen 120 Fällen.
     Hadamard 20 ohne 2 Spalten, zwei neue Spalten: je Paar 1272 flache x'', je Zeilengruppe 488 bis 600 Bedingungen, keine Belegung
     in allen 1140 Paaren (Bedingung wie in notes/ideen_2026-11-26.md, Abschnitt 3f, und im Kopf von ersatz_k2.c beschrieben).
  6. RM(1,5): Nebenklassen je Abstand 0 bis 12: 1, 32, 496, 4960, 35 960, 201 376, 906 192, 3 365 856, 10 119 795, 21 288 320,
     22 064 064, 8 693 888, 427 924. Abstand 12: 219 604 mit Spektrum [8^16 0^16], 208 320 mit [8^12 4^16 0^4]; nach Grad und
     Autokorrelation 868 / 166 656 / 52 080 / 208 320. Stellen mit |W| >= 12 je gerader Nebenklasse: 0: 427 924, 1: 5 091 689,
     2: 15 643 096, 3: 9 999 360, 4: 2 378 475, 6: 13 888. Sylvester 32 ohne 1 Spalte: 13 693 568 flache x bei d = 11, 0 bei d = 9;
     ohne 2 Spalten: 215 040 bei d = 10 (jeweils bis auf Vorzeichen).
  7. Paley-Konferenzmatrix 26: disc 5 (260 flache x bei Schwelle 7); Ordnung 30: disc 7 (45 588 flache x bei Schwelle 9).
     Zirkulant n = 15: größte Diskrepanz 5. Paley II der Ordnung 28 (q = 13): disc 6.
  8. Faserbedingung für den Kern <1,a,b> (Abschnitt 4, Idee 1): 55 / 74 / 97 Typen bei (16,4) / (18,5) / (20,6); bei K = 5
     ausgeschlossen 11 / 4 / 1.
  9. Verdopplungszellen: die zwölf Zellen aus Abschnitt 3e gegen die Tabelle (Abruf und Prüfsumme wie in src/offene_zellen.py).

Schritt 2, zwei Stichproben voll rechnen, wenn der Durchsatz reicht (erst 200 Fälle messen, dann entscheiden):
  Ordnung 24 ohne 3 Spalten und ohne 5 Spalten, je plus eine freie Zeile bei d = 9 (242 880 und 5 100 480 Fälle; Soll: nie lösbar).
  Reicht die Zeit nicht: Anteil der gerechneten Fälle angeben, Belegart „Stichprobe".

Abweichung: den Punkt nicht weiterrechnen, beide Zahlen und die vermutete Ursache ins Protokoll und an den Anfang des Berichts.
Die übrigen Punkte weiterrechnen.

Obergrenze: 4 Stunden Rechenzeit. Am Ende ohne Zuruf auswerten, main holen und mergen. STATUS.md und LANDKARTE.md: die Nachträge aus
Abschnitt 7 von notes/ideen_2026-11-26.md, aber nur für Punkte ohne Abweichung, mit Belegart und „zwei Programme (Orchestrator, Sitzung)";
dazu der Fehlversuch aus Abschnitt 6 der Notiz. Stand-Zeilen nicht ändern. Nicht nach main zusammenführen. Nichts nach außen melden.
```

## Startprompt für die Sitzung

```
Lies notes/gegenrechnung_ideen_2026-11-26_auftrag.md in main und führe Auftrag C aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
