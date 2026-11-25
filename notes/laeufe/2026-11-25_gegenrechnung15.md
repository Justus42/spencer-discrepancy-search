# Gegenrechnung der Orchestrator-Zählungen zu (15,4) (25.11.2026, Cloud, 4 Kerne, Auftrag C)

Gegenstand: selbstkomplementäre Codes der Länge 15, Radius 4 (Zeile a trifft x, wenn |<a,x>| ≥ 7), invariant unter einer
Spaltenpermutation sigma von Primzahlordnung p. Reine Gegenrechnung, keine Suche. Branch `ccr-017ff5f2-28uk0d`.
Die Skripte des Orchestrators (`notes/orchestrator/2026-11-24_abend/`) wurden **erst nach** der eigenen Rechnung angesehen
(nur `sym_enum.py` und `test15.py`, zum Vergleich der Methode); nichts davon übernommen.

## Start und Ende

- 25.11.2026 00:19 UTC: `sha256sum -c verifiers/SHA256SUMS` alle drei OK (`runs/gegenrechnung15/sha256_vorher.log.txt`).
- 25.11.2026 00:41 UTC (nach allen Läufen): wieder alle drei OK (`sha256_nachher.log.txt`). Keine Prüfdatei verändert.
- `build/exakt_disc` gebaut (`gcc -O3 -march=native -o build/exakt_disc src/exakt_disc.c`), damit läuft der Wrapper mit drei Programmen.

## Verfahren (eigener Code `src/gegen15_sym.py`, Prüfcode, nur ganze Zahlen)

- Wörter als Zahlen 0..2^15−1, Bit i = Spalte i, Bit 1 = Eintrag −1. sigma = Zyklen auf aufeinanderfolgenden Spalten
  (7+7+1, 13+1+1, 11+1+1+1+1). Gruppe G = ⟨sigma, Komplement⟩ der Ordnung 2p.
- Punktklassen = G-Bahnen auf den 2^15 Punkten (kleinstes Wort als Vertreter). Eine Zeilenbahn überdeckt eine Punktklasse genau dann,
  wenn die Kugel vom Radius 4 (1941 Punkte) um ihr Vertreterwort sie trifft (die Bahn ist G-invariant, G wirkt isometrisch).
- Aufzählung: Überdeckungsmatrix Bahn × Klasse; Paare voller Bahnen mit allen Teilmengen fester Zeilen über ein Matrixprodukt
  „unüberdeckt × Klassengewicht“ (float32, exakt, alle Summen < 2^24); Gewicht 0 = Überdeckung, sonst Zahl offener Punkte.
- **Unabhängige direkte Nachrechnung** ohne Klassen und Kugeln: Matrix der Zeilen (je Komplementpaar eine Zeile) bauen und
  max_i |<a_i,x>| < 7 auf allen 2^15 Punkten zählen (`offen_matrix`).
- Logs: `runs/gegenrechnung15/teil1..4.log.txt`, `kontrolle_*.log.txt`, `aequiv*.log.txt`. Rechenzeit gesamt ca. 22 Min. (davon 10 Min.
  für einen ersten, zu langsamen Aequivalenztest, abgebrochen; Teile 1–4 zusammen unter 15 s).

## Teil 1: p = 7, Zyklen 7+7+1 (`teil1.log.txt`)

| Angabe Orchestrator | eigenes Ergebnis | Belegart |
|---|---|---|
| 2344 Punktklassen | 2344 (Burnside (2^15 + 6·8)/14 = 2344 stimmt mit der Aufzählung) | voll |
| 2340 volle Zeilenbahnen, 4 feste Zeilen | 2340, 4 (Klassengrößen nur 14 und 2) | voll |
| 2 volle + 2 feste (32 Wörter): genau 112 | **112**; 4 Paare fester Zeilen je 28, 2 Paare keine | voll |
| 2 volle + 1 feste (30 Wörter): 0 | **0** (alle 4 festen Zeilen, 2 736 630 Paare) | voll |
| 2 volle ohne feste (28 Wörter): 0 | **0** | voll |
| kleinste Restzahl bei 30 Wörtern: 466, je feste Zeile 56 Paare | **466**, für jede der 4 festen Zeilen **56** Paare | voll |

Einzelheiten:
- Feste Zeilen (Bits 0–6 | 7–13 | 14): 0 = alles +1; 1 = erster Zyklus −; 2 = zweiter Zyklus −; 3 = beide Zyklen −; Spalte 14 jeweils +.
  Paare mit Überdeckung: (0,1), (0,2), (1,3), (2,3), je 28; ohne: (0,3) und (1,2) (kleinste Restzahl dort 438, je 56 Paare).
  Also liefern genau die Paare fester Zeilen, die sich in einem Zyklus unterscheiden, Lösungen.
- Zusatz (nicht verlangt): 28 Wörter: kleinste Restzahl 1100 (56 Paare). 34 Wörter (2 volle + 3 feste): 56 je Tripel, zusammen 224;
  36 Wörter (2 volle + 4 feste): 112.
- Alle 112 Lösungen direkt nachgerechnet (Skalarprodukte, alle 2^15 Punkte): jeweils 0 offene Punkte. Die Minima 466 und 1100 direkt
  bestätigt (je ein Paar am Minimum: 466 bzw. 1100 offene Punkte).
- **Positivkontrolle in derselben Struktur:** `verifiers/pruefe_matrix.py` auf Lösung Nr. 0 und Nr. 111 (16 × 15, `loesung0_16x15.txt`,
  `loesung111_16x15.txt`): `check_disc` 7, `verify_spencer` 7 (aufgefüllt 16 × 16), `exakt_disc` 7, Rückgabecode 0. Zusätzlich alle 112 Lösungen mit
  `pruefe_matrix.py --erwartet 7`: 112 in Ordnung, 0 abweichend (`pruefe_matrix_alle112.log.txt`). Quotient 7/√16 = 1,75 = `ctrl_16`, kein Rekord.

### Zusatz: Gleichwertigkeit zu `ctrl_16`

`ctrl_16` ohne Nullspalte hat alle Zeilenskalarprodukte ±1 (Gram-Eigenwerte 0 und 16): nach Zeilenvorzeichen sind es die Zeilen einer
Hadamard-Matrix der Ordnung 16 ohne eine Spalte. Das gilt auch für alle 112 Lösungen. Gleichwertigkeit (Zeilen- und Spaltenvertauschung,
Zeilen- und Spaltennegation) mit eigenem Zurückverfolgen: Zeilenzuordnung unter Erhaltung aller Vierzeilen-Summen
Q(i,k,l,m) = Σ_j a_ij a_kj a_lj a_mj (invariant unter Spaltenvorzeichen), am Ende Spalten als Mehrmengen bis aufs Vorzeichen vergleichen.
Kontrollen: verwürfelte `ctrl_16` gleichwertig (ja); Sylvester-16 ohne Spalte 0 gegen ohne Spalte 5 gleichwertig (ja); Sylvester-16 ohne Spalte gegen
`ctrl_16` (nein); `ctrl_16` mit einem umgedrehten Eintrag verliert den Hadamard-Typ (erkannt).

**Ergebnis: 56 der 112 Lösungen sind zu `ctrl_16` gleichwertig, die anderen 56 nicht.** Die 112 zerfallen in genau 2 Klassen zu je 56
(`aequiv.log.txt`, 289 s für den Vergleich mit `ctrl_16`, 9 Min. 40 s mit dem Klassenvergleich). Die zweite Klasse ist also ein von
`ctrl_16` verschiedener 16 × 15-Code mit 32 Wörtern, Radius 4, Diskrepanz 7. Die Verteilung der Vierzeilen-Summen ist in beiden Klassen
gleich ((−9: 112), (−1: 1344), (7: 336), (15: 28)), trennt sie also nicht; getrennt hat sie die Zuordnungssuche (Nachweis „nicht
gleichwertig“ ruht auf der erschöpfenden Suche, die bei den Kontrollen die erwarteten Antworten gab). Ob die zweite Klasse einen anderen Spaltenhinweis
auf dieselbe Hadamard-Klasse bedeutet oder eine andere Hadamard-Klasse, ist nicht untersucht.

## Teil 2: p = 13, Zyklen 13+1+1 (`teil2.log.txt`)

Punktklassen 1264 (Burnside 1264), volle Bahnen 1260, feste Zeilen 4.

| Konfiguration | Wörter | Überdeckungen | kleinste Restzahl offener Punkte |
|---|---|---|---|
| 1 volle Bahn + 2 feste | 30 | **0** | 1690 |
| 1 volle Bahn + 3 feste | 32 | **0** | 1326 |
| 1 volle Bahn + 4 feste | 34 | **0** | 1326 |

Minima direkt bestätigt. Stichprobe 60 zufällige (Bahn, 2 feste): Klassenmethode = direkte Rechnung in 60 von 60.
Positivkontrolle der Maschinerie bei p = 13 (`kontrolle_2voll*.log.txt`): 2 volle Bahnen (52 Wörter) überdecken nicht (kleinste Restzahl 2), mit einer
festen Zeile (54 Wörter) je 1328 Überdeckungen, die ersten 50 direkt nachgerechnet (0 offene Punkte). Die Maschinerie findet also Überdeckungen, wo es sie gibt.

## Teil 3: p = 11, Zyklen 11+1+1+1+1 (`teil3.log.txt`)

Punktklassen 1504 (Burnside 1504), volle Bahnen 1488, feste Zeilen 16.

| Konfiguration | Wörter | Teilmengen | Überdeckungen | kleinste Restzahl offener Punkte |
|---|---|---|---|---|
| 1 volle Bahn + 4 feste | 30 | 1820 | **0** | 2420 |
| 1 volle Bahn + 5 feste | 32 | 4368 | **0** | 2200 |
| 1 volle Bahn + 6 feste | 34 | 8008 | **0** | 1980 |

Minima direkt bestätigt; Stichprobe 60 zufällige (Bahn, 5 feste): 60 von 60 gleich. Zwei volle Bahnen (44 Wörter) ohne feste Zeilen: keine
Überdeckung, kleinste Restzahl 1306 (nicht verlangt, nur Kontrolle der Maschinerie).

## Teil 4: `ctrl_16` ohne Nullspalte, eine Zeile gestrichen (`teil4.log.txt`)

16 × 15, alle Einträge ±1. Mit allen 16 Zeilen: 0 offene Punkte. Bei jeder der 16 Zeilen gestrichen: **genau 466** offene Punkte
(jede Zeile trifft genau 466 Punkte als einzige). Verteilung der Zahl treffender Zeilen je Punkt: 0:0, 1:7456, 2:21504, 3:3584, 4:224.
Direkte Rechnung mit ganzen Skalarprodukten auf allen 2^15 Punkten, ohne Klassen.

## Gesamtergebnis

Alle Zahlen des Orchestrators stimmen: 2344 / 2340 / 4; 112 (4 × 28 + 2 × 0); 0 bei 30 und 28 Wörtern; 466 mit je 56 Paaren; 0 für p = 13 und p = 11
bei 30, 32, 34 Wörtern; 466 bei allen 16 Streichungen von `ctrl_16`. Belegart voll, zwei unabhängige Programme (Orchestrator-Skripte
und `src/gegen15_sym.py`), dazu direkte Nachrechnung ohne Klassen. Neu aus der Gegenrechnung: die 112 Lösungen zerfallen in 2
Gleichwertigkeitsklassen zu je 56, nur eine davon ist `ctrl_16`.

Grenzen: Aussage nur für Symmetrien von Primzahlordnung (p = 7 mit Typ 7+7+1, 11 mit 11+1⁴, 13 mit 13+1+1) und die Zählung „volle Bahnen + feste Zeilen“
wie im Auftrag; andere Zyklentypen (z. B. 7+1⁸, 5er- und 3er-Typen) und Nicht-Primzahlordnungen sind nicht Teil dieser Rechnung.
Ein 30-Wörter-Code muss auch gar keine Symmetrie haben; das hier ist kein Beweis, dass K(15,4) > 30.
Nichts nach außen gemeldet.
