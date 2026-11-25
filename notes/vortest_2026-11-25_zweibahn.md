# Vortest und Auftragsentwürfe: Zwei-Bahnen-Codes und Leiter unter 48 (25.11.2026, 07:20 Uhr)

Verfasser: Orchestrator-Claude. Repo-Stand beim Rechnen: `main` = `aa83b04`. Skripte und Log:
`notes/orchestrator/2026-11-25_vortest/`. **Alle Zahlen hier sind Rechnungen des Orchestrators mit einem Programm,
nicht gegengerechnet.** Beide Aufträge unten hat Justus am 25.11.2026 um 07:54 freigegeben; er startet die Sitzungen selbst.

## Ergebnis in zwei Sätzen

Kein Rekordkandidat. In allen Zellen bis Länge 20 gibt es keinen Code der Zwei-Bahnen-Bauart innerhalb der Rekordgrenze;
die Bauart reproduziert aber `ctrl_16` (112 Lösungen) und die 32-Wörter-Codes bei (17,5) (1792 Lösungen).

## Bauart

Länge m = c·q + f. Die Gruppe ⟨g⟩ verschiebt c Spaltenblöcke der Länge q gleichzeitig zyklisch und lässt f Spalten fest.
Bahnen auf Wortpaaren {w, ~w}; ein Code ist eine Vereinigung von Bahnen. `ctrl_16` hat diese Form (c = 2, q = 7, f = 1:
zwei volle Bahnen plus zwei feste Paare). Gezählt wird vollständig: Verzweigen am ersten unüberdeckten Punkt. „Hart“ heißt
ein Punkt, den keine erlaubte kurze Bahn erreicht; die vollen Bahnen müssen alle harten Punkte allein überdecken.

## Zählungen (Belegart: voll, ein Programm)

Grenze = Rekordgrenze der Zelle in Paaren (Wörter/2). „alle“ = jede Zusammensetzung aus Bahnen bis zur Grenze.

| Zelle | Typ | Bahnen nach Größe | Fall | Überdeckungen |
|---|---|---|---|---|
| (15,4) | 7+7+1 | 1: 4, 7: 2340 | alle, ≤ 16 Paare | **112** (Kontrolle, = Prüfvermerk D1) |
| (15,4) | 7+7+1 | | alle, ≤ 15 Paare | 0 (Kontrolle) |
| (17,5) | 7+7+3 | 1: 16, 7: 9360 | 2 volle + ≤ 2 feste | **1792**, alle 32 Wörter, Radius 5 per Brute-Force nachgerechnet |
| (17,5) | 7+7+3 | | 2 volle + ≤ 1 festes | 0 |
| (13,3) | 6+6+1 | 1: 4, 2: 6, 3: 20, 6: 670 | alle, ≤ 15 | 0 |
| (16,4) | 8+8 | 1: 4, 2: 6, 4: 60, 8: 4064 | alle, ≤ 20 | 0 |
| (16,4) | 7+7+2 | 1: 8, 7: 4680 | 2 volle + ≤ 6 feste | 0 |
| (18,5) | 9+9 | 1: 2, 3: 10, 9: 14 560 | alle, ≤ 20 | 0 |
| (19,5) | 9+9+1 | 1: 4, 3: 20, 9: 29 120 | 2 volle + ≤ 7 Paare aus kurzen Bahnen | 0 |
| (20,6) | 10+10 | 1: 4, 2: 2, 5: 204, 10: 52 326 | alle, ≤ 20 | 0 |
| (20,6) | 10+10 | | 2 volle + ≤ 4 Paare aus kurzen Bahnen (48 Wörter) | 0 |
| (20,6) | 9+9+2 | 1: 8, 3: 40, 9: 58 240 | 2 volle + ≤ 2 Paare | 0 |
| (20,6) | 9+9+2 | | 1 volle + ≤ 11 Paare aus kurzen Bahnen | **nicht entschieden** (keine harten Punkte) |
| (20,6) | 19+1 | 1: 2, 19: 27 594 | 1 volle + ≤ 1 festes | 0 |
| (20,6) | 17+3 | 1: 8, 17: 30 840 | 1 volle + ≤ 3 feste | 0 |

Nicht entschieden sind außerdem: (16,4) 7+7+2 und (19,5) 9+9+1 mit nur einer vollen Bahn; (20,6) 10+10 ab 25 Paaren.
Die Bahnzahlen stimmen mit der Formel in `bahnzahl.py` überein (Burnside).

Folgen:
- Der Punkt „Vollständige Aufzählung K=4 bei n=20 (ca. 280 CPU-h)“ aus STATUS ist für den Teilfall erledigt, in dem die
  vier 5er-Zyklen zu zwei 10er-Zyklen zusammenhängen.
- Unter Z10 mit zwei 10er-Zyklen gibt es auch keinen Code mit 48 Wörtern aus zwei vollen Bahnen. Mit zwei vollen Bahnen
  erreicht die Bauart bei (20,6) die 48 der Paley-Doppelung also nicht (andere Zusammensetzungen bis 48: nicht gezählt).

## Leiter unter 48 bei (20,6): exakte Vorprüfung

`leiter_vortest.py`, Brute-Force über 2^20 Punkte. Zahl der Punkte, die eine Nebenklasse von ⟨1, a⟩ allein überdeckt:

| Code | kleinster Wert | übrige Werte |
|---|---|---|
| beide 48er (aus 52er Startwert 5, 6) | 6108 | alle 12 gleich 6108 (bestätigt STATUS) |
| 52er Startwert 5, 6, Kontrolle 3 | **0** | 648 bis 5728 |
| 52er Startwert 3, 4, 10, 12, Kontrolle 2, 6 | **48** | 1044 bis 5564 |
| 56er Startwert 2 / 3 | 32, 32 / 36, 40 | 1636 bis 4644 |

Alle elf Codes haben den Kern ⟨1, a⟩ mit vier Wörtern (Gewichte 9 und 11). Drei von neun 52ern enthalten einen 48er, die
übrigen sechs sind 12 Nebenklassen plus ein Flicken für 12 Syndrome. Die Tabu-Suche hat K = 12 aus Zufallsstart nie
gefunden; ein Lauf K = 12 ist nie gestartet worden (Archiv, „Warmstart und K=12“). Fühlprobe (Suche, kein Beleg):
K = 12, Typ (11,9), Startwert 1, 900 s, ein Cloud-Kern: ungelöst, Restzahl 18.

## Auftrag Z (freigegeben): Zwei-Bahnen-Zählung in den Randzellen

```
Auftrag Z (Opus, Effort hoch): Zwei-Bahnen-Codes, vollständige Zählung

Lies zuerst STATUS.md, LANDKARTE.md, CLAUDE.md und notes/vortest_2026-11-25_zweibahn.md. Eigener Branch, Protokoll
notes/laeufe/2026-11-25_zweibahn.md, STATUS.md erst am Ende. Blöcke von höchstens 30 Minuten, nach jedem Block
committen und pushen. Zwischendateien über 1 MB nicht ins Repo. Zählcode und Suchcode getrennt. Vor jedem
Treffer-Bericht sha256sum -c verifiers/SHA256SUMS und python3 verifiers/pruefe_matrix.py.

Bauart: selbstkomplementärer Code der Länge m = 2q + f, invariant unter g = gleichzeitige zyklische Verschiebung auf
zwei Spaltenblöcken der Länge q, dazu f Fixspalten. Bahnen von <g> auf Wortpaaren {w, ~w}. Code = Vereinigung von
Bahnen. Gesucht: alle Überdeckungen mit Radius R innerhalb der Rekordgrenze der Zelle. So ist ctrl_16 gebaut.

Teil 1, Gegenrechnung mit eigenem Programm (notes/orchestrator/2026-11-25_vortest/zweibahn.c nicht benutzen). Erwartet:
 a) (15,4), q=7, f=1: 2340 volle Bahnen, 4 feste Paare. Höchstens 16 Paare: 112 Überdeckungen. Höchstens 15: 0.
 b) (17,5), q=7, f=3: 9360 volle Bahnen, 16 feste Paare. Zwei volle Bahnen plus höchstens 2 feste Paare: 1792
    Überdeckungen, alle mit 32 Wörtern. Plus höchstens 1 festes Paar: 0.
 c) (20,6), q=10, f=0: Bahnen der Größe 1/2/5/10: 4/2/204/52326. Höchstens 20 Paare, alle Zusammensetzungen: 0.
    Zwei volle Bahnen plus kurze Bahnen mit zusammen höchstens 4 Paaren: 0.
 d) (20,6), q=9, f=2: Bahnen der Größe 1/3/9: 8/40/58240. Zwei volle Bahnen plus höchstens 2 Paare: 0.
 e) Offen aus dem Vortest, bitte entscheiden: (20,6), q=9, f=2, eine volle Bahn plus kurze Bahnen mit zusammen
    höchstens 11 Paaren.
 Bei Abweichung in a) bis d): stoppen und berichten.
 a) und b) sind die Positivkontrolle in derselben Struktur. Das Programm für Teil 2 und 3 muss a) und b) mit
 derselben Methode (Symmetrieabbau, Filter) treffen, bevor es auf neue Zellen läuft.

Teil 2, d = 9, Rekord bei höchstens 25 Paaren (50 Wörter, 9/sqrt(25) = 1,800):
 a) (25,8), q=11, f=3: 1 525 200 volle Bahnen, 16 feste Paare. Alle Überdeckungen aus zwei vollen Bahnen plus
    höchstens 3 festen Paaren, und aus einer vollen Bahn plus höchstens 14 festen Paaren.
    Nebenergebnis: kleinste Zahl fester Paare, mit der zwei volle Bahnen überdecken (bis 9 wären es höchstens
    62 Wörter, Tabelle 64).
 b) (25,8), q=12, f=1: Bahnen der Größe 1/2/3/4/6/12: 4/6/20/60/670/1 397 740. Zwei volle Bahnen plus höchstens 1 Paar.
 c) (23,7), q=11, f=1: 381 300 volle Bahnen, 4 feste Paare. Zwei volle Bahnen plus höchstens 3 feste Paare.

Teil 3, d = 10, Rekord bei höchstens 31 Paaren (62 Wörter, 10/sqrt(31) = 1,796):
 a) (30,10), q=13, f=4: 41 297 760 volle Bahnen, 32 feste Paare. Zwei volle Bahnen plus höchstens 5 feste Paare.
    Nebenergebnis: kleinste Zahl fester Paare überhaupt (Tabelle 128 Wörter).
 b) (28,9), q=13, f=2: 10 324 440 volle Bahnen, 8 feste Paare. Zwei volle plus höchstens 5 feste.
 c) (26,8), q=13, f=0: 2 581 110 volle, 2 feste Paare. Und q=11, f=4: 3 050 400 volle, 32 feste (zwei volle plus
    höchstens 9 feste).

Hinweise zum Verfahren (frei, solange Teil 1 stimmt):
 - Harte Punkte: die kein erlaubtes festes Paar erreicht. Die vollen Bahnen müssen sie allein überdecken.
 - Erste Bahn nur bis auf den Normalisator von <g>: unabhängige Verschiebung der Blöcke, gemeinsamer Multiplikator,
   Blocktausch, Permutation der Fixspalten, Komplementieren ganzer Blöcke und Fixspalten. Zweite Bahn über alle,
   gefiltert an harten Punkten, die die erste offen lässt. Den Symmetrieabbau an Teil 1 a) und b) prüfen: Zählung
   mit und ohne Abbau muss gleich sein.
 - Die Bitmatrix Bahn mal Bahn des Vortests passt ab m = 22 nicht mehr in den Speicher.

Obergrenzen: Teil 1 30 Minuten, Teil 2 90 Minuten, Teil 3 2 Stunden Rechenzeit. Abbruch: Teil 1 weicht ab; oder
Zeit um, dann berichten, welche Fälle ganz abgearbeitet sind (nur die tragen „voll“, der Rest „Suche“).
Treffer innerhalb der Rekordgrenze: sofort Wörter und Matrix ablegen, pruefe_matrix.py mit Zeitgrenze 2 Stunden,
berichten. Ist das Prüfen zu langsam: berichten, nichts umgehen.
Am Ende: LANDKARTE.md (Regel 9, je Zelle und Typ die kleinste erreichte Wortzahl), STATUS.md höchstens fünf Zeilen,
Fehlversuche als Einzeiler, nichts nach außen melden.
```

## Auftrag L (freigegeben): Leiter unter 48 bei (20,6)

```
Auftrag L (Sonnet, Effort mittel): Nebenklassen-Leiter unter 48 bei (20,6)

Lies zuerst STATUS.md, LANDKARTE.md, CLAUDE.md, notes/vortest_2026-11-25_zweibahn.md (Abschnitt Leiter) und im Archiv
notes/verlauf/STATUS_2026-11-24_vollstaendig.md die Abschnitte „Lauf K=13“ und „Warmstart und K=12“. Eigener Branch,
Protokoll notes/laeufe/2026-11-25_leiter48.md, STATUS.md erst am Ende. Blöcke von höchstens 30 Minuten, committen und
pushen. Kein neuer Suchcode: src/coset_tabu.c, src/coset_ladder.py, src/run_block.py, src/run_warm.py,
src/warm_prep.py unverändert. Treffer über python3 verifiers/pruefe_matrix.py, vorher sha256sum -c verifiers/SHA256SUMS.

Teil 1, Gegenrechnung mit eigenem kurzem Skript (Brute-Force über 2^20 Punkte). Erwartet, Zahl der Punkte, die eine
Nebenklasse von <1,a> allein überdeckt:
 - beide 48er (runs/woerter_n20_R6_d2_K12_typ11-9_aus52seed{5,6}.txt): 12 mal 6108.
 - 52er Startwert 5, 6 und Kontrolle 3: kleinster Wert 0. Startwert 3, 4, 10, 12, Kontrolle 2 und 6: kleinster Wert 48.
 - 56er Startwert 2: kleinste Werte 32, 32. Startwert 3: 36, 40.
 Bei Abweichung: stoppen und berichten.

Teil 2, Positivkontrolle auf der Zielstufe: K=12 (48 Wörter), Typ (11,9), Zufallsstart, Startwerte 1 bis 8, je
 1800 s, 4 parallel. Bestanden: mindestens 2 von 8. Genau 1 von 8: Startwerte 9 bis 16 nachschieben, bestanden bei
 insgesamt mindestens 2. 0 von 8: Stopp, Fehlversuch eintragen, keine Leiter.
 Vortest des Orchestrators: Startwert 1, 900 s, ein Kern: ungelöst, Restzahl 18.
 Jede Lösung: Radius per Brute-Force, pruefe_matrix.py, Alleinanteil je Nebenklasse. Ist ein Alleinanteil 0, ist das
 K=11 ohne Suche: sofort berichten.

Teil 3, nur nach bestandener Kontrolle: K=11 (44 Wörter). Typ (11,9), Startwerte 1 bis 8, je 1800 s. Dazu Warmstart
 aus jeder K=12-Lösung aus Teil 2 und aus den beiden 48ern (Nebenklasse mit dem kleinsten Alleinanteil streichen), je
 900 s. Danach Typ (10,10) und (12,8), je 4 Startwerte mal 1800 s. K=10 nur, wenn K=11 gelöst ist.

Obergrenze: 4 Stunden Rechenzeit insgesamt. Abbruch: Kontrolle nicht bestanden oder erste Stufe ohne Lösung.
Belegart bei Misserfolg: „Suche erfolglos“. Am Ende LANDKARTE.md (Regel 9), STATUS.md höchstens fünf Zeilen,
Fehlversuche als Einzeiler, nichts nach außen melden.
```

## Nachrechnen

```
cd notes/orchestrator/2026-11-25_vortest
gcc -O3 -march=native -fopenmp -o zweibahn zweibahn.c
./zweibahn 2 7 1 4 16        # c q f R T: alle Zusammensetzungen
./zweibahn 2 7 3 5 16 2      # Modus 2: genau zwei volle Bahnen plus kurze
./zweibahn 1 19 1 6 20 1     # Modus 1: genau eine volle Bahn plus kurze
python3 leiter_vortest.py ../../../runs/woerter_n20_R6_d2_K12_typ11-9_aus52seed5.txt
```

Laufzeiten auf zwei Cloud-Kernen: jede Zeile der Tabelle unter 2,5 Minuten, Speicher bis 430 MB. Ausgaben in
`zaehlung.log.txt`. Der Modus ohne Angabe ist bei (20,6) mit q = 9 zu langsam (nach 10 Minuten abgebrochen).

## Sonst aufgefallen

`STATUS.md` (Offene Punkte) verweist auf `notes/gate_2026-11-25_bahnen3.md`. Die Datei und der Ordner
`notes/orchestrator/2026-11-25_gate_bahnen3/` fehlten in `main`; sie lagen nur auf dem Branch
`orchestrator/rueckfrage2-sol-2026-11-24` (Stand `ed9d47c`) und sind von dort unverändert nachgetragen.
