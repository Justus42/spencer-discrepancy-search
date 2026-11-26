# Auftrag K (Entwurf des Orchestrators, 26.11.2026, 18:38 Uhr; gilt erst nach Freigabe durch Justus): Kern der Dimension 3 bei d = 8

Grundlage: `notes/ideen_2026-11-26.md` (Idee 1, berichtigte Fassung) und `notes/ideen_2026-11-26_pruefvermerk.md` (Punkt 3.3 und die
Einwände von Sol zu Idee 1). Die Familie wurde nie gesucht. `STATUS.md` kennt nur die Zählschranke bei (20,6), die keinen Typ ausschließt.

Worum es geht: Codes als Vereinigung von K Nebenklassen eines Kerns D = ⟨1,a,b⟩ (8 Wörter). K = 5 gibt 40 Wörter, also 20 Zeilen, und
damit die Rekordgrenze bei d = 8 in den Zellen (16,4), (18,5), (20,6). Bekannt ist K ≤ 8 (lineare Codes der Dimension 6, 64 Wörter).

Erwartung des Orchestrators (unkalibriert): kein Rekord; K = 6 oder 7 bei (16,4) halte ich für möglich (Tabellenwert 48 oder 56 statt 64).
Ob die vollständige Entscheidung je Typ in vertretbarer Zeit läuft, ist unbekannt und Teil des ersten Tests.

```
Auftrag K (Opus): kleinste Zahl K von Nebenklassen eines Kerns <1,a,b>, zuerst in der Zelle (16,4). Neuer Suchcode ist erlaubt.

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-26.md (Abschnitte 3b, 4 Idee 1) und notes/ideen_2026-11-26_pruefvermerk.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_kern3.md, Logs runs/kern3/ als *.log.txt (nicht *.log), STATUS.md und LANDKARTE.md erst am Ende.
Nichts installieren (kein pip, kein apt). Stand-Zeilen nicht ändern. Uhrzeiten nur mit date. Suchcode nach src/, Prüfcode getrennt nach
verifiers/kern3/. Blöcke von höchstens 30 Minuten (Hintergrundjob mit Treiberskript ist erlaubt), nach jedem Block Protokoll, commit, push.

Festlegungen:
  - Länge m, Radius R = (m-8)/2. D = {0, a, b, a+b} und die vier Komplemente; a, b, 1 linear unabhängig. Die Koordinaten zerfallen
    nach (a_j, b_j) in vier Blöcke der Größen n0 >= n1 >= n2 >= n3 (Typ), mindestens drei davon nichtleer. Bei m = 16 sind das 55 Typen.
  - Eine Nebenklasse c + D deckt x, wenn eines ihrer 8 Wörter Abstand <= R zu x hat. Gesucht: K Nebenklassen, die alle x decken.
  - Als Matrix: je Nebenklasse 4 Zeilen (c, c+a, c+b, c+a+b als +-1-Zeilen), also 4K Zeilen und m Spalten, Diskrepanz 8.

Schritt 0, Kontrollen (scheitert eine: stoppen und berichten):
  a) sha256sum -c verifiers/SHA256SUMS (alle drei OK).
  b) Typenliste und Faserbedingung nachrechnen (eigener Code; Vergleich mit notes/orchestrator/2026-11-26_gegenpruefung/kern3_fasern.log.txt):
     (16,4): 55 Typen, B(D) zwischen 1471 und 2378, bei K = 5 ausgeschlossen sind genau die 11 Typen
     (8,6,2,0) (10,2,2,2) (10,4,2,0) (11,2,2,1) (11,3,1,1) (11,3,2,0) (12,2,1,1) (13,1,1,1) (13,2,1,0) (14,1,1,0) (12,2,2,0).
  c) Überdeckungstest eichen: für 20 zufällige Mengen von Nebenklassen (verschiedene Typen) die Zahl offener Punkte über die
     Nebenklassenrechnung und über alle 2^16 Punkte direkt zählen; 0 Abweichungen.
  d) Positivkontrolle in derselben Struktur: Sylvester 32 (Zeile u, Spalte j: (-1)^popcount(u&j)) auf den 16 Spalten
     0 1 3 4 6 7 10 12 15 16 17 18 22 23 25 28 ist ein linearer Code mit 64 Wörtern und Radius 4. Mit a = Zeile u1, b = Zeile u2 ist er
     eine Vereinigung von 8 Nebenklassen von <1,a,b>. Beispiele: (u1,u2) = (2,9) Typ (4,4,4,4); (1,2) Typ (5,4,4,3); (2,4) Typ (5,5,3,3);
     (1,8) Typ (6,5,3,2); (6,8) Typ (8,3,3,2). Für diese fünf: der Überdeckungstest meldet 0 offene Punkte, die 32 x 16-Matrix besteht
     python3 verifiers/pruefe_matrix.py MATRIX --erwartet 8, und der Sucher findet mit K = 8 in höchstens 10 Minuten eine Überdeckung.
     Das ist ein Zertifikat K <= 8 für den Typ, keine Aussage über das Minimum.

Schritt 1, erster Test (30 Minuten): Zelle (16,4), K = 5, die 44 nicht ausgeschlossenen Typen, je Typ höchstens 30 Sekunden.
  Vollständige Suche: erste Nebenklasse = D (Verschiebung), Symmetrie der Blöcke nutzen, verzweigen über einen offenen Punkt mit den
  wenigsten Deckern, abschneiden, wenn die offenen Punkte nicht mehr in (5 - k) Nebenklassen passen. Je Typ ins Protokoll:
  „keine Überdeckung (Suche vollständig)", „Überdeckung gefunden" oder „Zeitgrenze", dazu Knotenzahl und Zeit.
  Vergleich mit der Bauregel: für die fünf Typen aus 0 d) zusätzlich das kleinste K, das der Sucher in 60 Sekunden findet.
  Abbruch: Ist nach Schritt 1 weniger als die Hälfte der 44 Typen entschieden, nicht verlängern: stoppen, Knotenzahlen berichten.

Schritt 2: die in Schritt 1 offenen Typen mit höchstens 10 Minuten je Typ.
Schritt 3: K = 6 bei (16,4) (48 Wörter): je Typ erst eine lokale Suche (2 Minuten), dann für kleine Typen vollständig (höchstens 5 Minuten).
Schritt 4, nur wenn Zeit bleibt: (18,5), 74 Typen (Faserbedingung schließt bei K = 5 vier aus), K = 5, je Typ höchstens 60 Sekunden.
  (20,6) nicht in diesem Auftrag.

Treffer prüfen:
  - Matrix (4K Zeilen, m Spalten) nach runs/kern3/ schreiben; python3 verifiers/pruefe_matrix.py MATRIX --erwartet 8.
  - K = 5: REKORDKANDIDAT (8/√20). Sofort anhalten, prüfen, sha256sum -c erneut, Protokoll, pushen, berichten.
  - K = 6 oder 7 bei (16,4), K = 6 bei (18,5): Tabellenwert. Prüfen, speichern, weiterrechnen.
  Nichts nach außen melden. Ob ein Wert neu ist, prüft der Orchestrator.

Obergrenze: 4 Stunden Rechenzeit. Belegart: „keine Überdeckung" nur bei vollständiger Suche als „voll (ein Programm)", mit Knotenzahl;
alles andere „Suche erfolglos" oder „Zeitgrenze". Am Ende ohne Zuruf auswerten, main holen
(git fetch origin '+refs/heads/main:refs/remotes/origin/main', dann mergen), STATUS.md höchstens fünf Zeilen plus Fehlversuche als Einzeiler,
LANDKARTE.md eine Zeile (bestes K je Zelle). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/kern3_2026-11-26_auftrag.md in main und führe Auftrag K aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
