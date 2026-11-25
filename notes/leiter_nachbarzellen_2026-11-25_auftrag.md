# Auftrag N (Entwurf des Orchestrators, 25.11.2026, 22:52 Uhr; gilt erst nach Freigabe durch Justus): Leiter mit dem Löser aus Auftrag H in den Nachbarzellen

Grundlage: Auftrag H (`notes/laeufe/2026-11-25_zerlegung.md`, Gate `notes/gate_2026-11-25_zerlegung.md`). Der Löser `src/zerl_halb.c`
(abwechselnde Suche, Kern ⟨1,a⟩, K Nebenklassen = 4K Wörter) findet den 48er bei (20,6) aus Zufallsstart in Sekunden (8 von 8). Die alte
Nebenklassen-Suche brauchte dafür 30 Minuten (1 von 16) und kam in den Nachbarzellen nie unter den Tabellenwert 64 (`STATUS.md`, Negativbefunde:
(18,5) K = 15 0 von 12; (21,6) K = 16 0 von 12; (23,7) K = 15 0 von 8). Mit dem neuen Löser ist das nie versucht worden.

Zweck: je Zelle die kleinste erreichbare Nebenklassenzahl. Erwartung des Orchestrators (unkalibriert): kein Rekord; neue Tabellenwerte unter 64
sind möglich. Außerdem zeigt die Leiter, wie weit K = 12 bei d = 9 entfernt ist; das ist die Grundlage für die Entscheidung über (25,8).

```
Auftrag N (Sonnet): Leiter K = 16, 15, 14, ... mit dem vorhandenen Löser in sechs offenen Zellen. Kein neuer Suchcode.

Lies zuerst STATUS.md, CLAUDE.md, notes/laeufe/2026-11-25_zerlegung.md und notes/gate_2026-11-25_zerlegung.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_leiter_nachbarzellen.md, Logs runs/leiter_n/, STATUS.md und LANDKARTE.md erst am Ende.
Nichts installieren (kein pip, kein apt). Die Stand-Zeilen von STATUS.md und LANDKARTE.md nicht ändern. src/zerl_halb.c und
src/zerl_offen.py nicht ändern; eigene Treiberskripte (bash) sind erlaubt. Uhrzeiten nur mit date, nicht schätzen.

Werkzeug: gcc -O3 -march=native -o build/zerl_halb src/zerl_halb.c
  Suche:   ./build/zerl_halb alt K m1 m2 d --seed S --zeit SEK --aus PAAR --neustart 20      (Grenzen: K <= 16, m1 und m2 <= 16)
  Prüfung: python3 src/zerl_offen.py offen PAAR d --direkt       (0 offene Klassen = Überdeckung)
           python3 src/zerl_offen.py matrix PAAR MATRIX ; python3 verifiers/pruefe_matrix.py MATRIX --erwartet d
  Vorlage für einen Block mit 4 Startwerten: src/run_zerl_alt.sh.
Lehnt das Programm einen Typ ab oder stürzt ab: Typ auslassen, im Protokoll vermerken, nichts am Suchcode ändern.

Schritt 0, Kontrollen (Abbruch, wenn eine scheitert: stoppen und berichten):
  - sha256sum -c verifiers/SHA256SUMS (alle drei OK), Ergebnis ins Protokoll.
  - K3 wiederholen: K = 12, Typ (11,9), d = 8, Startwerte 1 bis 8, je höchstens 900 s. Erwartet 8 von 8 (Sitzung H: 1,0 bis 24,3 s), Soll
    mindestens 4 von 8. Jeden Treffer mit zerl_offen.py (0 offen) und einen mit pruefe_matrix.py (Diskrepanz 8) bestätigen.
  - Struktur von ctrl_17: K = 8, Typ (9,8), d = 7, Startwerte 1 bis 4, je höchstens 60 s. Erwartet 4 von 4 in unter 1 s (Gate H).

Zellen (Tabelle Post-Kéri laut STATUS.md, obere Schranke überall 64 Wörter = K 16) und Typen (m1,m2):
  d = 8:  (16,4) [34 bis 64]: (8,8) (9,7) (10,6) (11,5)        (18,5) [28 bis 64]: (9,9) (10,8) (11,7) (12,6)
  d = 9:  (19,5) [40 bis 64]: (10,9) (11,8) (12,7) (13,6)      (21,6) [34 bis 64]: (11,10) (12,9) (13,8) (14,7)
          (23,7) [29 bis 64]: (12,11) (13,10) (14,9) (15,8)    (25,8) [25 bis 64]: (13,12) (14,11) (15,10) (16,9)
Reihenfolge der Zellen: (18,5), (16,4), (21,6), (19,5), (23,7), (25,8).

Schritt 1, erster Test (billig): Zelle (18,5), K = 16, alle vier Typen, je 4 Startwerte zu höchstens 300 s. Erwartet: mindestens Typ (11,7)
  gelöst (das hat schon die alte Suche geschafft). Ist kein Typ gelöst: stoppen und berichten.

Schritt 2, Leiter je Zelle und Typ: beginne bei K = 16, 4 Startwerte parallel, je höchstens 300 s. Ist mindestens ein Startwert gelöst:
  Treffer prüfen (siehe unten), dann K - 1 im selben Typ. Ist keiner gelöst: Leiter dieses Typs endet, beste Restzahl notieren.
  (Begründung: aus einer Lösung mit K - 1 folgt durch eine beliebige weitere Nebenklasse eine mit K. Das ist eine Faustregel für die Suche,
  kein Ausschluss.)

Schritt 3, Nachfassen: je Zelle im besten Typ (kleinstes erreichtes K, bei Gleichstand kleinste Restzahl darunter) die nächste Stufe K - 1
  mit 8 Startwerten zu je 900 s (zwei Blöcke zu 4). Bei einem Treffer weiter mit K - 1, wieder 8 Startwerte.

Schritt 4, nur wenn danach noch Zeit ist: Zelle (20,6), d = 8, K = 11 in den Typen (14,6), (15,5), (16,4), je 4 Startwerte zu 600 s
  (Auftrag H hat nur (11,9), (13,7), (10,10), (12,8) gerechnet).

Treffer prüfen (jeder gelöste Lauf mit 4K < 64 Wörtern, also K <= 15): zerl_offen.py offen PAAR d --direkt muss 0 melden; Matrix schreiben
  nach runs/leiter_n/; pruefe_matrix.py MATRIX --erwartet d muss von allen drei Programmen d melden (bei 23 bis 25 Spalten kann das Minuten
  dauern; Zeitgrenze 30 Minuten je Matrix, bei Überschreitung vermerken und nur diese eine Matrix zurückstellen). Je Zelle und K genügt
  eine geprüfte Matrix; weitere Treffer nur mit zerl_offen.py zählen.
  - Tabellenergebnis (K <= 15, aber mehr Wörter als die Rekordgrenze): prüfen, speichern, weiterrechnen. Nicht anhalten.
  - Rekordkandidat (d = 8 mit K <= 10, d = 9 mit K <= 12): sofort anhalten, prüfen wie oben, Protokoll, pushen, berichten.
  Nichts nach außen melden. Ob ein Wert neu ist, prüft der Orchestrator.

Obergrenze: 7 Stunden Rechenzeit. Blöcke von höchstens 30 Minuten, nach jedem Block Protokoll aktualisieren, committen, pushen
(CLAUDE.md, Regel 4). Reicht die Zeit nicht für alle Zellen: Reihenfolge einhalten, den Rest als „nicht gerechnet“ melden.
Belegart: Treffer sind exakt (drei Prüfprogramme). Alles andere ist „Suche erfolglos“ und schließt nichts aus.
Am Ende ohne Zuruf auswerten: Abgleich aller gemeldeten Restzahlen mit zerl_offen.py; sha256sum -c erneut; main holen
(git fetch origin '+refs/heads/main:refs/remotes/origin/main', dann mergen); STATUS.md höchstens fünf Zeilen plus Fehlversuche als
Einzeiler; LANDKARTE.md: je Zelle und Typ kleinstes erreichtes K und beste Restzahl darunter (Regel 9). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/leiter_nachbarzellen_2026-11-25_auftrag.md in main und führe Auftrag N aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
