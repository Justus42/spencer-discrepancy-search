# Auftrag D (Entwurf des Orchestrators, 26.11.2026, 07:54 Uhr; gilt erst nach Freigabe durch Justus): Leiter bei d = 10 mit dem Löser aus Auftrag H

Grundlage: Auftrag N (`notes/laeufe/2026-11-25_leiter_nachbarzellen.md`, Gate `notes/gate_2026-11-26_leiter_n.md`). N hat die Zellen mit d = 8 und
d = 9 gerechnet. Die Zellen mit d = 10 hat in diesem Projekt noch keine Suche angefasst (Ideennotiz vom 25.11.: „keine Bauidee, kein Prüfweg in Python“;
beides gilt mit dem Löser `src/zerl_halb.c` und mit `exakt_disc` nicht mehr).

Lage bei d = 10 (Tabelle Post-Kéri, Stand 26.11.2026): (20,5) 62 bis 128, (22,6) 49 bis 128, (24,7) 41 bis 128, (26,8) 36 bis 128,
(28,9) 30 bis 128, (30,10) 27 bis 128; die obere Schranke ist überall die triviale Regel. Rekordgrenze bei d = 10: höchstens 62 Wörter.
Für Kern-⟨1,a⟩-Codes heißt das: **K = 16 (64 Wörter) wäre ein Tabellenergebnis (64 statt 128) und träfe den Rekordquotienten genau
(10/√32 = 5/√8), ohne ihn zu übertreffen. K = 15 (60 Wörter) wäre ein Rekord (10/√30 = 1,826).**
Der Abstand zur unteren Schranke ist bei (28,9) und (30,10) am größten (64 gegen 30 bzw. 27, Faktor über 2, wie bei den bekannten 48 gegen 24
in (20,6)); bei (20,5) und (22,6) ist er klein (64 gegen 62 bzw. 49).

Erwartung des Orchestrators (unkalibriert): kein Rekord. K = 16 in (28,9) oder (30,10) halte ich für möglich; ob der Löser dort in vertretbarer
Zeit läuft, ist unbekannt und Teil des ersten Tests.

```
Auftrag D (Sonnet): Leiter K = 16, dann 15 mit dem vorhandenen Löser in den Zellen mit d = 10. Kein neuer Suchcode.

Lies zuerst STATUS.md, CLAUDE.md, notes/laeufe/2026-11-25_leiter_nachbarzellen.md und notes/gate_2026-11-26_leiter_n.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_leiter_d10.md, Logs runs/leiter_d10/, STATUS.md und LANDKARTE.md erst am Ende.
Nichts installieren (kein pip, kein apt). Stand-Zeilen nicht ändern. src/zerl_halb.c und src/zerl_offen.py nicht ändern; eigene Treiberskripte
(bash) sind erlaubt, die aus Auftrag N (src/run_leiter_*.sh) dürfen als Vorlage dienen. Uhrzeiten nur mit date, nicht schätzen.

Werkzeug wie in Auftrag N: gcc -O3 -march=native -o build/zerl_halb src/zerl_halb.c
  Suche:   ./build/zerl_halb alt K m1 m2 d --seed S --zeit SEK --aus PAAR --neustart 20      (Grenzen: K <= 16, m1 und m2 <= 16)
  Prüfung: python3 src/zerl_offen.py offen PAAR d --direkt ; python3 src/zerl_offen.py matrix PAAR MATRIX
Lehnt das Programm einen Typ ab, stürzt ab oder kommt in der Zeit nicht über den Anfang hinaus: Typ auslassen, im Protokoll vermerken,
nichts am Suchcode ändern.

Schritt 0, Kontrollen (Abbruch, wenn eine scheitert: stoppen und berichten):
  - sha256sum -c verifiers/SHA256SUMS (alle drei OK); build/exakt_disc bauen (gcc -O3 -march=native -o build/exakt_disc src/exakt_disc.c).
  - K3 wiederholen: K = 12, Typ (11,9), d = 8, Startwerte 1 bis 8, je höchstens 900 s. Erwartet 8 von 8, Soll mindestens 4 von 8.
  - Kontrolle in der Nähe des Ziels: (25,8), d = 9, K = 16, Typ (16,9), Startwerte 1 bis 8, je höchstens 600 s. In Auftrag N löste 1 von 4
    Startwerten (Startwert 4). Soll: mindestens 1 von 8. Den Treffer mit zerl_offen.py bestätigen (0 offene Klassen).

Zellen, Typen (m1,m2) und Reihenfolge:
  (26,8):  (16,10) (15,11) (14,12) (13,13)
  (28,9):  (16,12) (15,13) (14,14)
  (30,10): (16,14) (15,15)
  (24,7):  (16,8) (14,10) (13,11) (12,12)
  (22,6):  (16,6) (13,9) (12,10) (11,11)
  (20,5):  nur wenn am Ende Zeit bleibt: (11,9) (10,10)

Schritt 1, erster Test (billig, zugleich Zeitmessung): (26,8), K = 16, Typ (16,10), 4 Startwerte parallel, je 300 s. Ins Protokoll: Restzahl,
  Durchläufe oder Schritte laut Log, Speicherbedarf, Dauer von zerl_offen.py für das Ergebnis. Dasselbe einmal für (28,9) Typ (16,12) und
  (30,10) Typ (16,14), je 1 Startwert, 300 s. Daraus die Laufzeit je Zelle festlegen: Regel 600 s je Startwert bei (26,8), (24,7), (22,6);
  bei (28,9) und (30,10) 1200 s, wenn der Löser in 300 s erkennbar arbeitet, sonst die Zelle auslassen und das berichten.

Schritt 2, K = 16 je Zelle und Typ: 4 Startwerte parallel mit der Laufzeit aus Schritt 1. Beste Restzahl je Typ notieren.
  Treffer (0 offene Klassen) sofort prüfen (unten), dann im selben Typ K = 15 mit 8 Startwerten (zwei Blöcke zu 4), gleiche Laufzeit.

Schritt 3, Nachfassen: je Zelle im Typ mit der kleinsten Restzahl bei K = 16 noch einmal 8 Startwerte mit doppelter Laufzeit
  (in Blöcken von höchstens 30 Minuten).

Treffer prüfen:
  - python3 src/zerl_offen.py offen PAAR 10 --direkt muss 0 melden; Matrix schreiben nach runs/leiter_d10/.
  - ./build/exakt_disc auf der Matrix (Aufruf wie in verifiers/pruefe_matrix.py): Diskrepanz 10.
  - python3 verifiers/pruefe_matrix.py MATRIX --erwartet 10 mit Zeitgrenze 60 Minuten. Bei 28 oder 30 Spalten kann das die Grenze reißen:
    dann vermerken „Regel 1 offen, nur zerl_offen.py und exakt_disc“ und weiterrechnen. Regel 1 holt dann der Orchestrator nach.
  - K = 16 (64 Wörter): Tabellenergebnis. Prüfen, speichern, weiterrechnen.
  - K = 15 (60 Wörter): REKORDKANDIDAT. Sofort anhalten, prüfen wie oben, Protokoll, pushen, berichten.
  Nichts nach außen melden. Ob ein Wert neu ist, prüft der Orchestrator.

Obergrenze: 7 Stunden Rechenzeit. Blöcke von höchstens 30 Minuten, nach jedem Block Protokoll aktualisieren, committen, pushen
(CLAUDE.md, Regel 4). Reicht die Zeit nicht für alle Zellen: Reihenfolge einhalten, den Rest als „nicht gerechnet“ melden.
Belegart: Treffer exakt. Alles andere ist „Suche erfolglos“ und schließt nichts aus.
Am Ende ohne Zuruf auswerten: Abgleich aller gemeldeten Restzahlen mit zerl_offen.py; sha256sum -c erneut; main holen
(git fetch origin '+refs/heads/main:refs/remotes/origin/main', dann mergen); STATUS.md höchstens fünf Zeilen plus Fehlversuche als
Einzeiler; LANDKARTE.md: je Zelle und Typ beste Restzahl bei K = 16 (Regel 9; Spalten N = 30 und N = 32). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/leiter_d10_2026-11-26_auftrag.md in main und führe Auftrag D aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
