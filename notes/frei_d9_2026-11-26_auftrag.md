# Auftrag F (Entwurf des Orchestrators, 26.11.2026, 18:39 Uhr; gilt erst nach Freigabe durch Justus): freie Suche in den Zellen (25,8) und (23,7)

Grundlage: `notes/ideen_2026-11-26.md` (Idee 4, berichtigte Fassung) und Sols Einwand im Prüfvermerk (erster Test verkleinert).
In den Zellen mit d = 9 wurde nie frei gesucht. Bekannt sind je 64 Wörter (32 Paare) aus Auftrag N (Kern-Familie). Die Rekordgrenze
liegt bei 50 Wörtern (25 Paare): 25 Zeilen mit disc 9 geben 9/√25 = 1,8.

Erwartung des Orchestrators (unkalibriert): kein Rekord. Das ist Rechensuche mit einem anderen Löser als den Leitern; bei (20,6) hat
derselbe Sucher den 48er blind gefunden und ist dann stehen geblieben. Möglich sind Tabellenwerte unter 64. Dieser Auftrag hat von den
vieren die niedrigste Priorität.

```
Auftrag F (Sonnet): freie Suche mit src/evo/p018.c bei (25,8) und (23,7), dazu eine Fassung mit Startdatei nach der Vorlage p022.

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-26.md (Idee 4), notes/laeufe/2026-11-25_evo.md und notes/gate_2026-11-25_evo.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_frei_d9.md, Logs und Ausgaben runs/frei_d9/ als *.log.txt und *.txt,
STATUS.md und LANDKARTE.md erst am Ende. Nichts installieren. Stand-Zeilen nicht ändern. Vorhandene Programme in src/evo/ und
src/evo_bewertung.c nicht ändern. Uhrzeiten nur mit date. Blöcke von höchstens 30 Minuten, danach Protokoll, commit, push.
Die Grafikkarte wird nicht benutzt.

Werkzeug: wie in Auftrag E. build/evo/p018 m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI; Zählung nur mit build/evo_bewertung m R DATEI
(vorher sha256sum -c runs/evo/BEWERTER.sha256).
Bekannte Codes: runs/leiter_n/c25-8_k16.matrix.txt (32 x 25) und runs/leiter_n/c23-7_k16.matrix.txt (32 x 23), je disc 9.

Schritt 0, Kontrollen (scheitert a oder b: stoppen und berichten):
  a) sha256sum -c verifiers/SHA256SUMS und sha256sum -c runs/evo/BEWERTER.sha256.
  b) Bewerter auf beiden bekannten Matrizen: 0 offene Klassen. Dauer eines Bewerterlaufs bei m = 25 notieren.
  c) Kleine Kontrolle des Suchers: p018 20 6 24, Startwerte 1 und 2, je 120 s. Soll: mindestens 1 von 2 mit 0 offenen Klassen
     (im Gate zu Auftrag E: 2 von 2 in 8 s und 51 s).

Schritt 1, erster Test (30 Minuten), nur (25,8):
  a) Vergleichszahl der Bauregel: aus dem bekannten 32er je eine Zeile streichen (32 Fälle) und mit dem Bewerter zählen.
     Kleinste, größte und mittlere Zahl offener Klassen ins Protokoll. Das ist die Messlatte für 31 Paare.
  b) Durchsatz: p018 25 8 32, Startwert 1, 300 s. Schritte je Sekunde, Speicherbedarf, beste Zahl offener Klassen.
  c) p018 25 8 31, Startwerte 1 bis 4, je 300 s. Beste Restzahl gegen die Messlatte aus a).
  Abbruch: Schafft p018 bei m = 25 weniger als 2 Schritte je Sekunde oder braucht mehr Speicher als vorhanden: (25,8) auslassen,
  mit (23,7) bei Schritt 1 neu beginnen; scheitert auch das, stoppen und berichten.

Schritt 2, blinde Kontrolle: p018 25 8 32, Startwerte 1 bis 4, je 1500 s (ein Block). Soll: mindestens 1 von 4 mit 0 offenen Klassen.
  Erreicht kein Lauf 0: beste Restzahl notieren, Schritt 3 nur noch mit der Startdatei-Fassung.

Schritt 3, Fassung mit Startdatei: p026 = Kopie von src/evo/p022.c mit zwei Änderungen: Startmatrix als weiterer Aufrufparameter statt
  der festen Datei, und die gestrichene Zeile als Parameter. Sonst nichts ändern. Kontrolle: p026 mit der Startdatei
  controls/had24_1p3_paley12.txt bei 20 6 23 und der gestrichenen Zeile (S-1) mod 24 muss dieselbe Restzahl liefern wie p022 mit Startwert S.
  Dann (25,8), 31 Paare: Start aus dem 32er ohne die Zeile mit der kleinsten Zahl aus Schritt 1a, Startwerte 1 bis 4, je 1500 s.
  Erreicht: 30 Paare aus dem neuen Code ohne dessen schwächste Zeile, und so weiter. Zwei Stufen nacheinander nicht erreicht: Zelle beenden.

Schritt 4: dasselbe für (23,7) (Schritte 1 bis 3 mit 600 s statt 1500 s), soweit die Zeit reicht.

Treffer prüfen:
  - Bewerter 0 offene Klassen; Matrix nach runs/frei_d9/; ./build/exakt_disc auf der Matrix: Diskrepanz 9;
    python3 verifiers/pruefe_matrix.py MATRIX --erwartet 9 mit Zeitgrenze 60 Minuten. Reißt sie: vermerken „Regel 1 offen" und weiterrechnen.
  - 26 bis 31 Paare: Tabellenwert (unter 64 Wörtern). Prüfen, speichern, weiterrechnen.
  - 25 Paare oder weniger: REKORDKANDIDAT. Sofort anhalten, prüfen, Protokoll, pushen, berichten.
  Nichts nach außen melden. Ob ein Wert neu ist, prüft der Orchestrator.

Obergrenze: 6 Stunden Rechenzeit. Belegart: Treffer exakt, alles andere „Suche erfolglos". Am Ende ohne Zuruf auswerten: alle Restzahlen
noch einmal mit dem Bewerter, main holen und mergen, STATUS.md höchstens fünf Zeilen plus Fehlversuche als Einzeiler, LANDKARTE.md eine
Zeile. Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/frei_d9_2026-11-26_auftrag.md in main und führe Auftrag F aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
