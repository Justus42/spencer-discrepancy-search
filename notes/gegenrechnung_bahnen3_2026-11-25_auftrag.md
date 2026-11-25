# Auftrag B2 (Entwurf des Orchestrators, 25.11.2026, 22:52 Uhr; gilt erst nach Freigabe durch Justus): zweite Rechnung zu den 48ern unter der 3-Sylowgruppe

Grundlage: Auftrag B (`notes/laeufe/2026-11-25_bahnen3.md`, Gate `notes/gate_2026-11-25_bahnen3.md`). Drei Aussagen über den 48er bei (20,6)
ruhen bisher auf je einem Programm und dürfen deshalb nicht in die Meldung vom 27.11.:
  (1) Unter der 3-Sylowgruppe P gibt es keine selbstkomplementäre Überdeckung mit höchstens 46 Wörtern (ein Zählprogramm, `src/bahnen3_zaehl.c`).
  (2) Die 19 008 P-invarianten 48er bilden 8 Äquivalenzklassen (Rechnung des Orchestrators mit bliss, nicht gegengerechnet).
  (3) Der Treffer aus Teil 4 ist ein neunter, zu keinem davon äquivalenter 48er (ebenso).
Offen ist außerdem, zu welcher Klasse die Treffer aus Auftrag H gehören (`STATUS.md`, Offene Punkte).

Zweck: zweite, unabhängige Rechnung (Projektregel: ein Ergebnis gilt erst mit zwei unabhängigen Programmen). Kein Rekordweg.

```
Auftrag B2 (Opus, Effort hoch): zweite Rechnung zu Auftrag B mit eigenen Programmen und anderem Verfahren.

Lies zuerst STATUS.md, CLAUDE.md, notes/laeufe/2026-11-25_bahnen3.md und notes/gate_2026-11-25_bahnen3.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_bahnen3_zweit.md, Logs runs/bahnen3_zweit/, STATUS.md und LANDKARTE.md erst am Ende.
Nichts installieren (kein pip, kein apt; igraph, bliss, nauty, ortools, python-sat gibt es nicht). Stand-Zeilen nicht ändern.
Uhrzeiten nur mit date, nicht schätzen.
Unabhängigkeit: Schreibe eigene Programme. NICHT lesen: src/bahnen3_*.c, src/bahnen3_*.py, src/bahnen_zaehl.c und alles unter
notes/orchestrator/. Die Ergebnisdateien unter runs/bahnen3/ darfst du erst zum Vergleich öffnen, wenn dein eigenes Ergebnis feststeht.
Eingaben: controls/had24_1p3_paley12.txt, controls/had24_2p2_paley12.txt (je 24 Zeilen, 20 Spalten; Code = Zeilen und ihre Negative,
48 Wörter; Überdeckung heißt: jeder Punkt von {+1,-1}^20 hat zu einer Zeile oder ihrem Negativ Abstand <= 6).
Gleichwertigkeit zweier Codes: eine Spaltenpermutation mit Vorzeichenwechsel einzelner Spalten bildet die Wortmenge auf die Wortmenge ab.

Teil 0, Kontrollzahlen selbst nachrechnen (Abbruch bei Abweichung: stoppen und berichten). Für had24_1p3_paley12:
  - Gruppe G aller vorzeichenbehafteten Spaltenpermutationen, die die 48 Wörter erhalten: Ordnung 576;
    Elementordnungen 1:1, 2:87, 3:8, 4:216, 6:120, 8:144.
  - genau eine Untergruppe P der Ordnung 9, nicht zyklisch.
  - Bahnen von P auf den Paaren {w, -w} aller 2^19 Paare: 8 der Größe 1, 160 der Größe 3, 58 200 der Größe 9; 58 368 Bahnen von P auf
    den Punktklassen {x, -x}.
  - der 48er ist die Vereinigung von genau 8 Bahnen der Größe 3 und überdeckt alles (Radius 6 über alle 2^20 Punkte).

Teil 1, zweite Zählung. Gesucht sind alle Mengen von P-Bahnen (Gewicht einer Bahn = ihre Größe), die alle Punktklassen überdecken:
  (a) nur Bahnen der Größe 1 und 3, Gewicht <= 23: erwartet keine.
  (b) nur Bahnen der Größe 1 und 3, Gewicht genau 24: erwartet genau 19 008, alle aus 8 Dreierbahnen, der 48er ist dabei.
  (c) genau eine Neunerbahn, Gesamtgewicht <= 23: erwartet keine.   (d) genau zwei Neunerbahnen, Gesamtgewicht <= 23: erwartet keine.
  Anderes Verfahren als im ersten Programm (dort: Verzweigen am offenen Punkt, Bitsets, Schranke aus paarweise fremden Kandidaten).
  Wähle selbst, zum Beispiel Verzweigen über Bahnen (drin oder nicht) mit einer Zählschranke, oder erst die 8 festen Punktklassen
  abarbeiten. Vollständigkeit im Protokoll begründen.
  Kontrollen für dein Programm, alle Pflicht:
   - gegen vollständiges Durchprobieren auf verkleinerten Fällen (mindestens 8 Fälle mit zufälligen Teilmengen der Punktklassen), 0 Abweichungen;
   - Positivkontrolle für (b): der 48er selbst wird als Lösung gezählt;
   - Positivkontrolle mit Neunerbahn, die dem ersten Programm fehlte: mindestens ein Fall mit echter Lösung, die eine Neunerbahn enthält
     und vom Programm gefunden wird (zum Beispiel verkleinerte Punktmenge, oder eine vorgegebene Neunerbahn mit erhöhtem Budget).
  Erster Test (billig): (a) mit Gewicht <= 18. Hochrechnung für (a) bis (d) ins Protokoll. Liegt die Hochrechnung über 4 Stunden auf den
  vorhandenen Kernen: Teil 1 abbrechen, berichten, mit Teil 2 weitermachen. (Das erste Programm brauchte zusammen etwa 40 Minuten auf 4 Kernen.)
  Erst danach vergleichen: deine 19 008 Lösungen als Mengen gegen runs/bahnen3/teil2_klein_B24_liste_4_56.txt (Format dort selbst klären).

Teil 2, Klassen. Eigenes Programm für Gleichwertigkeit (kanonische Form oder paarweise Rücksuche mit Invarianten), ohne Fremdbibliothek.
  Kontrollen: 20 zufällige vorzeichenbehaftete Spaltenpermutationen von had24_1p3 werden als gleichwertig erkannt; die Gruppenordnung von
  had24_1p3 aus diesem Programm ist 576; had24_1p3 und had24_2p2 sind nicht gleichwertig (erwartet nach dem Gate).
  (a) Klassen der 19 008 Lösungen. Erwartet 8 Klassen: vier mit Gruppenordnung 576 (je 432 Lösungen, darunter 1p3 und 2p2), zwei mit 144
      (je 1728), zwei mit 36 (je 6912).
  (b) runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt: erwartet Gruppenordnung 24, zu keiner der 8 Klassen gleichwertig.
  (c) Zuordnung der Treffer aus Auftrag H: runs/zerlegung/k3_seed1.matrix.txt bis k3_seed8.matrix.txt und
      runs/zerlegung/a_k12_t10-10_seed1.matrix.txt. Erwartung offen: je Treffer die Klasse (eine der neun) oder „neu“ mit Gruppenordnung.
  Je Klasse einen Vertreter als Matrix nach runs/bahnen3_zweit/ und mit python3 verifiers/pruefe_matrix.py MATRIX --erwartet 8 prüfen.

Abbruch: Abweichung in Teil 0; Abweichung von einer erwarteten Zahl in Teil 1 oder 2: nicht anpassen, sondern die Abweichung mit beiden
Zahlen berichten und an dieser Stelle anhalten (dann gilt keine der beiden Rechnungen, bis die Ursache geklärt ist).
Obergrenze: 6 Stunden Rechenzeit. Blöcke von höchstens 30 Minuten, nach jedem Block Protokoll aktualisieren, committen, pushen
(CLAUDE.md, Regel 4). Suchcode und Prüfcode getrennt halten. Vor einem Bericht sha256sum -c verifiers/SHA256SUMS.
Belegart nach Erfolg: „voll, zwei Programme mit verschiedenen Verfahren“. Nichts nach außen melden.
Am Ende ohne Zuruf auswerten: main holen (git fetch origin '+refs/heads/main:refs/remotes/origin/main', dann mergen); STATUS.md höchstens
fünf Zeilen (Belegart der betroffenen Zeilen und der offene Punkt zur 3-Sylowgruppe) plus Fehlversuche als Einzeiler; LANDKARTE.md nur,
wenn sich ein Eintrag ändert. Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/gegenrechnung_bahnen3_2026-11-25_auftrag.md in main und führe Auftrag B2 aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
