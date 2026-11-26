# Auftrag K2 (Entwurf des Orchestrators, 26.11.2026, 20:51 Uhr; von Justus am 26.11.2026 um 20:48 Uhr dem Grunde nach freigegeben): Kern der Dimension 3 bei (18,5) und (20,6)

Grundlage: Auftrag K (`notes/laeufe/2026-11-26_kern3.md`) und sein Gate (`notes/gate_2026-11-26_kern3.md`). Bei (16,4) sind 5 Nebenklassen
(40 Wörter) in allen 55 Typen ausgeschlossen (zwei Programme). Offen: (18,5) mit K = 5 in 43 Typen; (20,6) nicht begonnen.

Was ein Treffer wäre: K = 5 in einer der beiden Zellen gibt 20 Zeilen mit Diskrepanz 8, Quotient 1,789 (Rekordkandidat).
K = 6 bei (18,5) gibt 48 Wörter und unterböte den eigenen Wert 50 (Tabelle 64). K = 6 und K = 7 bei (20,6) bringen nichts (48 Wörter sind dort bekannt).

Erwartung des Orchestrators (unkalibriert): kein Rekord. Die bekannten 48er bei (20,6) haben einen Kern mit nur 4 Elementen, liegen also nicht in
dieser Familie; die Familie müsste von 64 auf 40 Wörter kommen. Bei (18,5) halte ich es für möglich, dass die vollständige Suche weitere Typen ausschließt.

Neu als Kontrollobjekt: Sylvester 32 auf 20 Spalten ist eine Lösung mit K = 8 bei (20,6) in 12 Typen
(`notes/orchestrator/2026-11-26_auftrag_k2/sylvester32_20sp.matrix.txt`, 32 x 20, dreimal Diskrepanz 8; Typen in `sylvester20.log.txt`).

Lehren aus den Aufträgen von heute, hier eingebaut: Jede Kontrolle verlangt nur, was die Methode können muss. Der Vergleich mit der Bauregel
hat eine Folge. Positivkontrollen in der Zielzelle vor jeder Aussage.

```
Auftrag K2 (Sonnet): Kern <1,a,b> bei (18,5) und (20,6) mit den vorhandenen Programmen. Kein neuer Suchcode.

Lies zuerst STATUS.md, CLAUDE.md, notes/laeufe/2026-11-26_kern3.md und notes/gate_2026-11-26_kern3.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_kern3_folge.md, Logs runs/kern3_folge/ als *.log.txt, STATUS.md und LANDKARTE.md erst am Ende
(vorher main holen). Nichts installieren. Stand-Zeilen nicht ändern. Uhrzeiten nur mit date. Blöcke von höchstens 30 Minuten
(Hintergrundjob mit Treiberskript ist erlaubt), nach jedem Block Protokoll, commit, push. Am Ende ohne Zuruf auswerten.
src/kern3_such.c, verifiers/kern3/ und notes/orchestrator/2026-11-26_gate_kern3/kern3_gate.c nicht ändern. Erlaubt: Treiberskripte
(Kopie von src/run_kern3.sh mit Ausgabeordner runs/kern3_folge/) und Hilfsskripte, die nur Logs lesen und zusammenfassen.

Programme:
  gcc -O3 -march=native -o build/kern3_such src/kern3_such.c            Modi info, eval, dfs, ls, lsw (siehe Kopf der Datei)
  gcc -O3 -march=native -fopenmp -o build/kern3_gate notes/orchestrator/2026-11-26_gate_kern3/kern3_gate.c
      Aufruf: TYPEN=datei build/kern3_gate alle m R K SEKUNDEN   (datei: Zeilen a-b-c-d). Mit OMP_NUM_THREADS=1 läuft ein Kern je Prozess.
      kern3_gate kann m <= 18. Für (20,6) wird es nicht gebraucht.
  Typ immer als Blockgrößen n0 >= n1 >= n2 >= n3. Eine Rechnung mit "KEINE" heißt: vollständig durchsucht, keine Überdeckung.

Schritt 0, Kontrollen (scheitert a, b, c oder e: stoppen und berichten; d entscheidet nur über Teil B):
  a) sha256sum -c verifiers/SHA256SUMS (alle drei OK), am Ende noch einmal.
  b) Eichung bei (16,4), K = 5: kern3_such dfs für Typ (5,4,4,3): KEINE; kern3_gate alle 16 4 5 120: 55 von 55 KEINE.
  c) Regel 2 für (18,5): python3 verifiers/pruefe_matrix.py controls/c18_5_50w.txt --erwartet 8 (dreimal 8).
     lsw 18 5, K = 8, Startwert 1, je 120 s, Typen (5,5,4,4), (6,5,4,3), (6,6,3,3), (7,4,4,3). Soll: mindestens 3 von 4 GEFUNDEN.
     Jeden Treffer mit verifiers/kern3/ueberdeckung.py treffer (drei Wege, 0 offen) und pruefe_matrix.py --erwartet 8 prüfen.
  d) Für (20,6): kern3_such info 20 6 5 5 5 5. Laufzeit und Speicher (VmHWM) ins Protokoll. Über 3 GB je Prozess: höchstens 2 Prozesse
     gleichzeitig. Über 7 GB oder Abbruch: Teil B auslassen und berichten.
  e) Regel 2 für (20,6): python3 verifiers/pruefe_matrix.py notes/orchestrator/2026-11-26_auftrag_k2/sylvester32_20sp.matrix.txt --erwartet 8.

Teil A, (18,5):
  A1 Messlatte. Für jeden Treffer aus 0 c: alle Fälle "ohne eine Nebenklasse" (8) und "ohne zwei" (28) mit eval zählen.
     Kleinste Zahl offener Klassen je Typ ins Protokoll. Das ist die Bauregel für K = 7 und K = 6.
  A2 Erster Test, lsw mit K = 6 auf den vier Typen aus 0 c, Startwerte 1 und 2, je 120 s.
     Abbruchregel: Liegt die beste Restzahl in keinem der vier Typen unter der Messlatte "ohne zwei", ist die Suche nicht besser als
     Streichen. Dann A3 auslassen und mit A4 weitermachen.
  A3 Nur wenn A2 besteht: lsw mit K = 6, Startwert 1, je 120 s, alle Typen, die die Faserbedingung bei K = 6 zulässt
     (verifiers/kern3/typen_fasern.py). Nur wenn dabei mindestens ein Typ GEFUNDEN meldet: lsw mit K = 5, je 120 s, auf den 43 offenen Typen.
  A4 Vollständige Suche, K = 5, die 43 Typen mit ZEITGRENZE aus runs/kern3/s4_K5_zusammen.log.txt.
     Zwei Programme, je Typ 300 s, je 4 Prozesse gleichzeitig: kern3_such dfs und kern3_gate (OMP_NUM_THREADS=1, je Typ eine TYPEN-Datei).
     In Blöcken von 20 Typen (ein Block etwa 25 Minuten), erst dfs, dann kern3_gate auf denselben Typen.
     Abbruchregel: Entscheidet im ersten Block von 20 Typen keines der beiden Programme auch nur einen Typ, A4 beenden und die
     Knotenzahlen berichten. Nicht verlängern.
     Belegart je Typ: "voll, zwei Programme", wenn beide KEINE melden; "voll, ein Programm", wenn nur eines; sonst offen.
     Meldet ein Programm GEFUNDEN: sofort stoppen, Treffer prüfen (siehe unten).

Teil B, (20,6) (nur wenn 0 d es zulässt):
  B1 Positivkontrolle in der Zielzelle: lsw 20 6, K = 8, Startwert 1, je 300 s, Typen (5,5,5,5), (6,5,5,4), (6,6,4,4), (7,5,4,4).
     In allen vier Typen existiert eine Lösung (Sylvester 32 auf 20 Spalten). Soll: mindestens 3 von 4 GEFUNDEN; Treffer prüfen wie in 0 c.
     Scheitert das: Teil B beenden und berichten (die Suche findet das bekannte Objekt nicht).
  B2 Messlatte wie A1 aus den Treffern von B1: ohne eine, ohne zwei, ohne drei Nebenklassen (8, 28, 56 Fälle).
  B3 Abstandsprobe: lsw mit K = 7, 6 und 5 auf den vier Typen, Startwert 1, je 300 s. Beste Restzahl je Stufe gegen die Messlatte.
  B4 Nur wenn in B3 mindestens ein Typ K = 6 erreicht: lsw mit K = 5, Startwert 1, je 180 s, alle Typen, die die Faserbedingung bei K = 5
     zulässt (96 von 97). Sonst Teil B beenden: Belegart "Suche erfolglos", mit den Restzahlen aus B3.

Treffer:
  - K = 5 in einer der beiden Zellen: alles stoppen. ueberdeckung.py treffer (drei Wege), Matrix 20 x m schreiben,
    sha256sum -c verifiers/SHA256SUMS, pruefe_matrix.py --erwartet 8, Ergebnis ins Protokoll, berichten. Nichts nach außen melden.
  - K = 6 bei (18,5): neuer eigener Wert (48 Wörter). Prüfen wie oben, Matrix 24 x 18 nach runs/kern3_folge/ speichern, weiterrechnen.

Obergrenze: 5 Stunden Rechenzeit. Was dann nicht gerechnet ist, steht als "nicht gerechnet" im Protokoll.

Am Ende: STATUS.md (Negativbefunde mit Belegart je Typgruppe, Fehlversuche als Einzeiler, Dateizeile) und LANDKARTE.md (eine Zeile).
Im Bericht: je Zelle und K die Zahl der Typen nach Belegart, die Messlatten, die besten Restzahlen, und was nicht gerechnet wurde.
```

Startprompt für die Sitzung:

```
Lies notes/kern3_folge_2026-11-26_auftrag.md in main und führe Auftrag K2 aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
