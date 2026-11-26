# Auftrag V (Entwurf des Orchestrators, 26.11.2026, 18:38 Uhr; gilt erst nach Freigabe durch Justus): Verdopplungszellen mit dem vorhandenen Sucher

Grundlage: `notes/ideen_2026-11-26.md` (Abschnitt 3e und Idee 3) und `notes/ideen_2026-11-26_pruefvermerk.md` (Punkt 3.1).
Hängt man an einen Code mit K Zeilen (2K Wörtern) in der Zelle (m,R) eine Spalte an (Zeilen (u,+1) und (u,−1)), entsteht ein Code mit
2K Zeilen in (m+1,R), dessen Diskrepanz um 1 größer ist. Ein Code mit 10 Zeilen bei (19,6) oder (17,5) gäbe so 20 Zeilen mit disc 8,
ein Code mit 12 Zeilen bei (22,7) oder (24,8) gäbe 24 Zeilen mit disc 9. Die Tabelle lässt das offen. Gesucht wurde dort nie.

Probe des Orchestrators (p018, je 20 bis 60 s, ein Startwert): (19,6) mit 16 Paaren in 0,3 s gelöst, mit 15 Paaren 2236 offene Klassen,
mit 10 Paaren 26 192 von 262 144; (17,5) mit 10 Paaren 8952 von 65 536.

Erwartung des Orchestrators (unkalibriert): kein Rekord. Möglich sind Tabellenwerte knapp unter den bekannten 32 Wörtern bei (19,6) und (17,5).

```
Auftrag V (Sonnet): Leitern mit dem vorhandenen Sucher src/evo/p018.c in vier Zellen. Kein neuer Suchcode.

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-26.md (Abschnitt 3e, Idee 3) und notes/laeufe/2026-11-25_evo.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_verdopplung.md, Logs und Ausgaben runs/verdopplung/ als *.log.txt und *.txt,
STATUS.md und LANDKARTE.md erst am Ende. Nichts installieren. Stand-Zeilen nicht ändern. src/evo/p018.c und src/evo_bewertung.c nicht
ändern; eigene Treiberskripte (bash) sind erlaubt. Uhrzeiten nur mit date. Blöcke von höchstens 30 Minuten, danach Protokoll, commit, push.

Werkzeug:
  gcc -O3 -march=native -fopenmp -o build/evo/p018 src/evo/p018.c -lm     Aufruf: build/evo/p018 m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI
  sha256sum -c runs/evo/BEWERTER.sha256 ; gcc -O3 -march=native -fopenmp -o build/evo_bewertung src/evo_bewertung.c
  Zählung: build/evo_bewertung m R DATEI   (nur diese Zahl gilt, nie die Selbstauskunft des Suchers)

Zellen (m,R), Paarzahl der Kontrolle, Ziel:
  (19,6): Kontrolle 16 Paare, Ziel 10 Paare (Rekord über die Verdopplung, 20 x 20 mit disc 8)
  (17,5): Kontrolle 16 Paare, Ziel 10 Paare
  (22,7): Kontrolle 24 Paare, Ziel 12 Paare (24 Zeilen, 23 Spalten, disc 9)
  (24,8): Kontrolle 24 Paare, Ziel 12 Paare (24 Zeilen, 25 Spalten, disc 9)

Schritt 0, Kontrollen (scheitert a oder b: stoppen und berichten):
  a) sha256sum -c verifiers/SHA256SUMS und sha256sum -c runs/evo/BEWERTER.sha256.
  b) (19,6) und (17,5): 16 Paare, Startwerte 1 bis 4, je 60 s. Soll: 4 von 4 mit 0 offenen Klassen (Bewerter).
  c) (22,7) und (24,8): 24 Paare, Startwerte 1 bis 4, je 300 s. Soll: mindestens 1 von 4. Vergleich mit der Bauregel: aus
     controls/had24_1p3_paley12.txt eine Spalte verdreifachen (zwei Kopien der Spalte anhängen) gibt 24 x 22, zwei Spalten verdreifachen
     gibt 24 x 24; der Bewerter muss für beide 0 melden. Scheitert die blinde Kontrolle in einer Zelle, dort nur die Abstandsprobe (Schritt 1).

Schritt 1, erster Test (30 Minuten): Abstandsprobe am Ziel. Je Zelle die Zielpaarzahl, Startwerte 1 bis 4, je 120 s (m <= 19) bzw.
  300 s (m >= 22). Ins Protokoll: offene Klassen laut Bewerter, Anteil an 2^(m-1), Schritte je Sekunde, Speicherbedarf.

Schritt 2, Leiter abwärts von der Kontrolle: Paarzahl je Stufe um 1 senken, Startwerte 1 bis 4, je 120 s bzw. 300 s.
  Stufe erreicht (0 offen): prüfen (unten), weiter. Zwei Stufen nacheinander nicht erreicht: in der letzten nicht erreichten Stufe
  einmal 8 Startwerte mit doppelter Zeit, dann Zelle beenden. Beste Restzahl je Stufe notieren.

Treffer prüfen:
  - Ausgabedatei (PAARE Zeilen, m Spalten) mit python3 verifiers/pruefe_matrix.py DATEI --erwartet (m - 2R), Zeitgrenze 60 Minuten.
    Reißt sie bei 22 oder 24 Spalten: vermerken „Regel 1 offen, nur Bewerter und exakt_disc“ und weiterrechnen.
  - Weniger Paare als die Kontrolle: Tabellenwert. Prüfen, nach runs/verdopplung/ speichern, weiterrechnen.
  - Zielpaarzahl erreicht: REKORDKANDIDAT. Die verdoppelte Matrix schreiben (jede Zeile u einmal als (u,+1), einmal als (u,-1)),
    python3 verifiers/pruefe_matrix.py MATRIX --erwartet (m - 2R + 1). Sofort anhalten, Protokoll, pushen, berichten.
  Nichts nach außen melden. Ob ein Wert neu ist, prüft der Orchestrator.

Obergrenze: 3 Stunden Rechenzeit. Belegart: Treffer exakt, alles andere „Suche erfolglos". Am Ende ohne Zuruf auswerten:
alle gemeldeten Restzahlen noch einmal mit dem Bewerter zählen, main holen und mergen, STATUS.md höchstens fünf Zeilen plus Fehlversuche
als Einzeiler, LANDKARTE.md eine Zeile (kleinste erreichte Paarzahl je Zelle, beste Restzahl darunter). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/verdopplung_2026-11-26_auftrag.md in main und führe Auftrag V aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
