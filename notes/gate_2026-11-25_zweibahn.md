# Gate zu Auftrag Z: Zwei-Bahnen-Codes (25.11.2026, 11:00 Uhr)

Geprüft: Branch `claude/auftrag-z-frknut`, Stand `44f105068ce5d9ffea3e7a92c55a273d8764c724` (PR 31), Protokoll
`notes/laeufe/2026-11-25_zweibahn.md`. Prüfer: Orchestrator-Claude mit eigenen Programmen
(`notes/orchestrator/2026-11-25_gate_zweibahn/`); `src/zweibahn_zaehl.c` der Sitzung dafür nicht gelesen.
**Ergebnis: bestanden, mit einer Korrektur und zwei Vermerken.** Kein Treffer, kein Rekordkandidat.

| Nr. | Prüfung | Ergebnis |
|---|---|---|
| 1 | Teil 1 a) bis d) gegen den Vortest | alle Zahlen gleich (112 / 0, 1792 / 0, Bahnzahlen, vier Nullen); zwei unabhängige Programme. e) neu entschieden: 0 |
| 2 | Zahl der harten Klassen, von Hand aus Binomialzahlen (`schranke.py`) | alle neun gleich: 1540, 2800, 32 928, 283 536, 753 984, 1 507 968, 2 199 054, 7 059 624, 18 120 960 |
| 3 | Zahl der Normalisator-Klassen voller Bahnen, eigene Rechnung über Halsketten-Klassen (`klassen.py`) | gleich: q = 7: 19, q = 9: 109, q = 11: 507, q = 13: 4394; Summe der Klassengrößen je gleich der Zahl voller Bahnen |
| 4 | Geordnete Paare voller Bahnen, die alle harten Klassen überdecken (`gate_hart.c`, mit und ohne Klassenabbau) | (15,4): 224, (17,5): 5376, wie in den Logs der Sitzung; (20,6) 9+9+2: 0 |
| 5 | **Zweite, unabhängige Zählung** der neuen Zellen mit q = 11 (`gate_hart.c`, alle 507 Klassen) | (23,7) 11+11+1: 0 (29 s). (25,8) 11+11+3: 0 (216 s). (26,8) 11+11+4: 0 (228 s). Keine einzelne volle Bahn überdeckt die harten Klassen |
| 6 | Stichprobe mit demselben Programm bei q = 13 (Zufallsauswahl, Startwert 20261125) | (28,9): 40 von 4394 Klassen, 0. (30,10): 25 von 4394 Klassen, 0 (11 Min.). Keine volle Zählung |
| 7 | Umfang des Branches | 66 Dateien, keine über 300 kB, keine Prüfdatei, `CLAUDE.md` unberührt, `sha256sum` vorher und nachher OK laut Protokoll |
| 8 | Änderungen an `STATUS.md` und `LANDKARTE.md` | nur Ergänzungen, sachlich richtig bis auf einen Satz (Korrektur unten) |

## Korrektur

`LANDKARTE.md`, Erläuterung: „Bei allen Typen mit q ungerade überdecken 2 volle Bahnen mit keiner Zahl fester Paare“ ist so
falsch. Bei q = 7 überdecken zwei volle Bahnen mit zwei festen Paaren ((15,4): 112 Codes, (17,5): 1792). Richtig: bei allen
Typen mit q = 9, 11 und 13. Wird nach dem Merge von PR 31 in einem Doku-PR berichtigt.

## Folgerung, die das Protokoll nicht zieht

Das Protokoll führt „eine volle Bahn plus feste Paare“ in Teil 3 und Teil 2 c) als nicht gezählt. Für Primzahl q ist der Fall
schon miterledigt: Würde eine volle Bahn A mit festen Paaren überdecken, überdeckte A allein alle harten Klassen, und dann
überdeckte jedes Paar (A, B) sie auch. Gezählt sind 0 solche Paare. Bei q = 11 und q = 13 gibt es außer vollen Bahnen nur
feste Paare, und drei volle Bahnen sprengen die Grenze (33 > 25, 39 > 31). Also gilt für (23,7) 11+11+1, (25,8) 11+11+3,
(26,8) 11+11+4 und 13+13, (28,9) 13+13+2, (30,10) 13+13+4: **kein Code dieser Bauart innerhalb der Rekordgrenze, alle
Zusammensetzungen.** Offen bleibt nur (25,8) 12+12+1 mit kurzen Bahnen der Größe 2 bis 6.

## Vermerke

1. **Belegart nach dem Gate.** Zwei unabhängige Programme: Teil 1 a) bis d) und die drei Zellen mit q = 11. Ein Programm
   (mit Stichprobe des Orchestrators): (26,8) 13+13, (28,9), (30,10), (25,8) 12+12+1 und Teil 1 e). „Voll“ ist dafür
   vertretbar; vor einer Aussage nach außen bräuchten diese Fälle eine zweite volle Zählung.
2. **Schätzfehler des Orchestrators.** Der Auftrag nannte für q = 13 „einige Dutzend“ Klassen, es sind 4394. Die
   Permutationen und Komplemente der Fixspalten verkleinern die Klassenzahl nicht weiter, sobald der Fixteil auf 0 normiert
   ist. Die Sitzung hat das richtig erkannt, den Lauf nach 10 Minuten abgebrochen, umgebaut und die neue Fassung an Teil 1
   erneut geprüft.

Nebenbefund: Eine reine Zählschranke entscheidet keinen der Fälle. Zwei volle Bahnen könnten rechnerisch das 1,8- bis
2,8-Fache der harten Punkte überdecken (`schranke.py`); der Ausschluss braucht die Aufzählung.

## Einordnung

Die Bauart von `ctrl_16` trägt nicht über q = 7 hinaus. In allen acht Typen von (20,6) bis (30,10) scheitern zwei volle
Bahnen schon an den Punkten, die kein festes Wort erreicht. Die Dichte an der Längenobergrenze (2,7 bei (25,8), 3,1 bei
(30,10)) hilft dieser Bauart also nicht.

## Nachrechnen

```
cd notes/orchestrator/2026-11-25_gate_zweibahn
python3 schranke.py
python3 klassen.py 11 3 > kl_11_3.txt          # q f; Ausgabe: Vertreterwort und Zahl voller Bahnen je Klasse
gcc -O3 -march=native -fopenmp -o gate_hart gate_hart.c
./gate_hart 7 1 4 -                            # q f R, alle vollen Bahnen als erste Bahn: 224
./gate_hart 11 3 8 kl_11_3.txt                 # Klassenvertreter als erste Bahn: 0
```

`gate_hart.c` braucht f ≥ 1 und zählt nur volle Bahnen plus feste Wörter. Laufzeiten auf zwei Cloud-Kernen.
