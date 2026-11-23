# STATUS

Stand: 23.11.2026. Timebox: 23.11.2026 bis 04.01.2027, danach Entscheidung über Abbruch.

## Ziel

Eine Matrix A in [-1,1]^(n x n) mit disc(A)/sqrt(n) > 5/sqrt(8) ≈ 1,7678 finden.
disc(A) = min über alle Vorzeichenvektoren x von max_i |(Ax)_i|. Eine einzelne endliche
Matrix genügt (Konstante C_10c in `teorth/optimizationproblems`, Seite `constants/10c.md`).
Rekord seit 05.09.2026: 8x8-Matrix aus dem [7,4]-Hamming-Code plus Nullspalte, Diskrepanz 5.

## Reduktion

Eine Vorzeichenmatrix mit N Zeilen auf m Spalten (Bit 1 = Eintrag -1) hat Diskrepanz
>= m - 2R genau dann, wenn die Hamming-Kugeln vom Radius R um alle Zeilen UND ihre
Komplemente den Würfel {0,1}^m überdecken. Ein selbstkomplementärer Überdeckungscode mit
2N Wörtern liefert die Schranke (m - 2R) / sqrt(max(m, N)).

## Ziele

| Länge m | Radius R | Wörter höchstens | ergibt | Dichte | Tabelle K(m,R) |
|---|---|---|---|---|---|
| 25 | 8 | 50 | 9/5 = 1,8000 | 2,69 | 25–64 |
| 23 | 7 | 50 | 9/5 = 1,8000 | 2,33 | 29–64 |
| 20 | 6 | 40 | 8/sqrt(20) = 1,7889 | 2,31 | 24–64 |
| 18 | 5 | 40 | 8/sqrt(20) = 1,7889 | 1,93 | 28–64 |
| 30 | 10 | 62 | 10/sqrt(31) = 1,7961 | 3,06 | 27–128 |

Dichte = Wörter x Kugelvolumen / 2^m. Zum Vergleich bekannte Codes: (15,4)/32 hat 1,90,
(17,5)/32 hat 2,30, (19,6)/32 hat 2,67. Gestrichen: (15,4) mit 30 Wörtern (Dichte 1,78,
Suche bewegt sich nicht). Reihenfolge der Versuche: 25/8 zuerst, dann 20/6.

## Gesichert (beide Prüfskripte stimmen exakt überein)

| Datei in `controls/` | Größe | Diskrepanz | Herkunft |
|---|---|---|---|
| `ctrl_8.txt` | 8x8 | 5 | eigene Suche, entspricht dem Rekord |
| `ctrl_9.txt` | 9x9 | 5 | eigene Suche |
| `ctrl_16.txt` | 16x16 | 7 | eigene Suche, Code (15,4) mit 32 Wörtern |
| `ctrl_17.txt` | 17x17 | 7 | eigene Suche, Code (17,5) mit 32 Wörtern |
| `L2026_17x17.txt` | 17x17 | 7 | veröffentlichtes Zertifikat aus dem Repo |
| `astra_19.txt` | 19x19 | 7 | Konstruktion `src/construct_32.py` |

`src/construct_32.py` liefert selbstkomplementäre 32-Wörter-Codes für jede ungerade Länge
n = 2R+7 >= 15. Per Brute-Force bestätigt für n = 15, 17, 19, 21, 23. Die Konstruktion
stammt aus einer Abfrage an GPT-6.2 Astra; die historische Zuordnung ist unbelegt.

## Messwerte der Suche (`src/search_lns.py`, wenige Minuten, 2 CPU-Kerne)

- Findet die Kontrollen bis Länge 17 in unter einer Sekunde.
- Länge 19, Radius 6: braucht 46 Wörter, bekannt sind 32. Die Suche ist ab hier schwächer
  als bekannte Konstruktionen.
- Bis zur vollständigen Überdeckung: (18,5) 80 Wörter, (20,6) 72, (23,7) 84.
- Mit fester Wortzahl bleiben unüberdeckt: (20,6)/40 0,76 %, (23,7)/50 0,07 %,
  (25,8)/50 0,10 %, (18,5)/40 2,2 %. Kleine Restquoten sagen wenig: die letzten Punkte
  sind die teuren.

## Fehlversuche

- Naives Simulated Annealing mit Bit-Flips: reproduziert Länge 17 nicht.
- Gieriges Verfahren plus Zerstören/Reparieren: verbessert den gierigen Start kaum.
- Zufällige lineare [n,5]-Codes mit Einsvektor: Radius zu groß.
- (15,4) mit 30 Wörtern: bleibt bei 466 unüberdeckten Punkten stehen.
- 23.11.2026, Positivkontrolle für die Astra-Suche 20/6: Der 32-Wörter-Code aus `construct_32.py`
  (n=15,17,19) hat Translationsstabilisator {0, 1}, ist also KEINE Vereinigung von Nebenklassen
  eines dreidimensionalen D. Die Struktur D=<1,a,b> kann ihn nicht mit vier Nebenklassen finden;
  Suche 20/6 nicht gestartet (Laufzeit der Kontrolle <1 s, siehe `runs/2026-11-23_positivkontrolle_19.md`).
  Alle 6 Kontrollen in `controls/` mit beiden Prüfskripten erneut identisch bestätigt.

## Vorschläge aus der Astra-Abfrage (unbelegt, noch nicht getestet)

Volltext: `notes/openrouter/astra_2026-11-23.md`. Subjektive Schätzung dort: 30 %, dass
mindestens eines der Ziele existiert.

- **20/6:** fünf Nebenklassen eines dreidimensionalen linearen Codes D = <1, a, b>.
  D bis auf Spaltenpermutation aufzählen (vier Vielfachheiten der Spaltentypen, Summe 20).
  Dann 2^17 Syndrome mit fünf Verschiebungen einer einzigen Menge überdecken, Tabu-Suche
  mit Bewertung aller Ersetzungen per Walsh-Transformation.
- **25/8:** Symmetrie vorgeben: Permutation vom Zykeltyp 5^5 plus Komplementbildung, fünf
  volle Bahnen zu je zehn Wörtern. Am Ende die Symmetrie lösen und einzelne Paare tauschen.

## Offen und unbelegt

- Ein Forenbeitrag auf „EinsteinArena“ zum selben Ansatz (Spencer-Konstante über
  antipodale Überdeckungen) wird in Geminis Bericht erwähnt, aber ohne Link. Eine eigene
  Suche fand ihn nicht.
- Von den 41 offenen Pull Requests im Zielrepo war nur die erste Seite einsehbar (25).
- Zeilenweise Nullen und gebrochene Einträge erhöhen das Budget pro Zeile nicht, außer in
  Paritätsfällen (n = 17, 19). Als Endspiel-Politur per LP bleiben sie möglich.

## Literaturstand (Gemini, 23.11.2026)

Quellen: `notes/gemini/`. Von Claude nur stichprobenhaft geprüft, sonst unbelegt übernommen.

- **Zielzellen heute:** K(18,5) 28–64, K(20,6) 24–64, K(21,6) 34–64, K(23,7) 29–64,
  K(25,8) 25–64 (untere Schranken per SDP, Gijswijt und Polak 2026). Kein Ziel ist
  ausgeschlossen. Obere Schranken seit 2011 unverändert, soweit auffindbar.
- **Die 64 wird von linearen [n,6]-Codes erreicht** (Baicheva und Bouyukliev 2010). Alles
  unter 64 muss nichtlinear sein. Die linearen Codes taugen als Startpunkt einer Suche.
- **Die 32-Wörter-Codes sind zwingend nichtlinear** (Graham und Sloane 1985). Passt zum
  eigenen Fehlversuch mit linearen Codes.
- **Dämpfer:** Maharaj (arXiv:2610.03760) gibt an, alle offenen Zellen mit q <= 4 per
  Tabu-Suche auf Bahnen abgesucht zu haben, und meldet nur quaternäre Verbesserungen.
  Im Paper nachgelesen: Binäre Zellen werden nicht eigens erwähnt, der Aufwand ist unklar.
  Dagegen steht, dass K(20,3) im Oktober 2026 von Dritten verbessert wurde.
- Die Originalkonstruktion von Östergård und Kaikkonen (1998) war nicht einsehbar.

## Entscheidung 23.11.2026: Kontrolle neu gefasst, Zwischenziel ergänzt

Die erste Positivkontrolle war falsch gestellt: Sie fragte, ob ein bestimmter Code die
gesuchte Struktur hat. Richtig ist die Frage, ob die Methode die bekannte **Größe**
erreicht, mit welchem Code auch immer.

**Leiter.** Für feste Länge und festen Radius werden Überdeckungen als Vereinigung von
Nebenklassen eines linearen Codes D mit Einsvektor gesucht, für Dimension 1, 2 und 3.
Dimension 1 ist der allgemeine selbstkomplementäre Code, Dimension 3 der Astra-Vorschlag.
Protokolliert wird je Dimension die kleinste Wortzahl mit vollständiger Überdeckung.

- **Kontrolle A, Länge 19, Radius 6:** 32 Wörter. Mit Dimension 1 muss das gelingen
  (`controls/astra_19.txt` belegt die Existenz). Gelingt es nicht, ist die Suche zu schwach.
- **Kontrolle B, Länge 20, Radius 6:** 64 Wörter. Ein solcher selbstkomplementärer Code
  existiert sicher (der 19er-Code, verlängert um eine freie Koordinate).
- **Danach** die Wortzahl bei Länge 20 senken.

**Zwischenziel.** Jede Überdeckung mit weniger als 64 Wörtern bei (18,5), (20,6), (21,6),
(23,7) oder (25,8) wäre für sich ein neuer Eintrag in den Tabellen der Überdeckungscodes
(Stand der Tabellen laut Geminis Bericht, von Claude nicht vollständig geprüft). Ab 40
Wörtern bei Länge 20 fällt zusätzlich der Spencer-Rekord. Für beides gilt Regel 3:
erst beide Prüfskripte, dann Neuheit prüfen, dann entscheiden.

**Abbruchregel für den Astra-Vorschlag.** Erreicht Dimension 3 weder 32 Wörter bei
Länge 19 noch 64 bei Länge 20, wird sie verworfen. Weiter dann mit Dimension 1 und 2.

## Ergebnis Leiter 23.11.2026 (Cloud, 4 Kerne, ca. 45 Min. Wall-Zeit)

Kern `src/coset_tabu.c` (Walsh-Bewertung, ganzzahlig), Treiber `src/coset_ladder.py`. Logs: `runs/2026-11-23_leiter_*.log.txt`.
Nachprüfung jedes Treffers per Brute-Force über alle 2^n Punkte (Radius), selbstkomplementär.

| Kontrolle | Ergebnis |
|---|---|
| A: Länge 19, R=6, 32 Wörter, d=1 | bestanden (5 s) |
| A: dito d=2 (K=8), alle 9 Typen, je 20 s | 2 Typen gelöst: (12,7), (11,8) |
| A: dito d=3 (K=4), alle 84 Typen, je 12 s | keiner gelöst (Restzahl >= 288 Syndrome) |
| B: Länge 20, R=6, 64 Wörter, d=1 (K=32, 120 s) | NICHT gelöst (Rest 78): Suche bei 2^19 Syndromen zu schwach |
| B: d=2 (K=16) | mehrere Typen gelöst |
| B: d=3 (K=8), alle 97 Typen, je 15 s | 64 Typen gelöst |

Kleinste Wortzahl bei Länge 20, R=6 (nur diese Stufen versucht):
- d=3, K=7 (56 Wörter): alle 97 Typen, je 20 s, nicht gelöst (beste Restzahl 48, Typ (5,5,5,5)).
- d=2, K=15 (60 Wörter): **gelöst** für Typ (11,9) (Seeds 2 und 3) und (12,8) (Seed 2); (13,7) nicht (Rest 12 nach 420 s).
- d=2 weiter hinunter (K=14), d=1 unter 64: nicht versucht.

Astra-Abbruchregel: d=3 erreicht 64 bei Länge 20, bleibt also im Spiel, ist bei Länge 19 aber nicht
bis 32 gekommen und bei Länge 20 schlechter als d=2.

**Treffer, Regel 1 erfüllt, Regel 3 (Neuheit) offen:** Überdeckung von (20,6) mit **60 Wörtern** (15 Nebenklassen eines 2-dim. D mit Einsvektor).
Wörter: `runs/woerter_n20_R6_d2_K15_*.txt`; Matrizen 30 x 20: `runs/matrix_n20_60w_*.txt`
(30x30 mit Nullspalten für den Verifier: `runs/matrix_n30pad_60w_*.txt`).
Beide Prüfskripte melden für alle drei Treffer dieselbe Diskrepanz **8** (`check_disc.py` auf 30x20,
`verify_spencer.py` auf 30x30 mit Nullspalten; Quotient 8/sqrt(30) = 1,46, also KEIN Spencer-Rekord;
dafür wären <= 40 Wörter nötig).
**Gate-Prüfung 23.11.2026:** Treffer von Claude unabhängig bestätigt (Brute-Force-Radius in `coset_ladder.py`,
`check_disc.py`, `verify_spencer.py`). Neuheit offen: Kéri-Tabelle K(20,6) = 24–64 (Stand 2011); Literatur danach
ungeprüft. Nichts nach außen gemeldet.

**Bauen und Starten** (aus dem Repo-Wurzelordner; Logs liegen als `runs/*.log.txt`):
`mkdir -p build && gcc -O3 -march=native -o build/coset_tabu src/coset_tabu.c`, dann
`python3 src/coset_ladder.py n R d K sekunden startwert [typ ...]` (Typ z. B. `11,9`). Eigener Pfad per `COSET_TABU_BIN`.

## Stufenlauf Dimension 2, Länge 20, Radius 6 (23.11.2026, 4 Kerne, ca. 1 h Wall-Zeit)

`src/run_stufen.py`: je Stufe Typen (10,10), (11,9), (12,8) x Startwerte 1–4 x 600 s. Logs: `runs/2026-11-23_stufen_K*.log.txt`.

- **K=14 (56 Wörter): gelöst**, 2 von 12 Läufen: Typ (11,9), Startwerte 2 und 3. Wörter:
  `runs/woerter_n20_R6_d2_K14_typ11-9_seed{2,3}.txt`, Matrizen `runs/matrix_n20_56w_*.txt` (28x20, `_pad` = 28x28).
  Beide Prüfskripte, beide Treffer: Diskrepanz **8** (`check_disc.py` auf 28x20, `verify_spencer.py` auf 28x28).
  Quotient 8/sqrt(28) = 1,512, kein Spencer-Rekord. Neuheit offen (siehe Gate-Prüfung oben).
  Restzahlen der 10 Fehlläufe: (10,10) 34–48; (11,9) Startwert 1: 4, Startwert 4: 2; (12,8) 7–11.
- **K=13 (52 Wörter): nicht gelöst**, 0 von 12 Läufen, je 600 s. Beste Restzahl unüberdeckter Syndrome
  (Startwerte 1/2/3/4): (10,10) 48/70/47/47; (11,9) 5/2/6/7; (12,8) 22/22/22/22.
- Stopp laut Regel bei der ersten Stufe ohne Lösung. K=12, 11, 10 nicht versucht.
- Typ (11,9) ist durchgehend am besten; (10,10) und (12,8) nie nahe an einer Lösung bei K=13.

## Nächste Gates

Gate 2 und die strukturierte Suche für 20/6 brauchen keine Grafikkarte (nur 2^17
Syndrome) und können in einer Cloud-Sitzung laufen. Die GPU wird erst für 25/8 wichtig.

1. **Lokal einrichten.** Python und GPU-Bibliothek installieren, Grafikkarte testen, alle
   Dateien in `controls/` mit beiden Prüfskripten nachrechnen.
2. **Suche auf Niveau bringen.** `src/search_lns.py` auf die GPU portieren und so
   verbessern, dass sie bei Länge 19 mit 32 Wörtern auskommt, ohne die Konstruktion zu
   kennen, oder mit der Konstruktion als Startpunkt deren Nachbarschaft durchsucht.
3. **Strukturierte Suche** nach den beiden Vorschlägen oben, erst 25/8, dann 20/6.
4. **Bei einem Treffer:** beide Prüfskripte, dann Neuheit prüfen, dann Entscheidung über
   einen Pull Request. Nichts vorher nach außen melden.

## Dateien

- `src/check_disc.py`: Prüfskript 1 (Claude), exakte Diskrepanz per Brute-Force.
- `verifiers/verify_spencer.py`: Prüfskript 2 (Gemini), unabhängig geschrieben.
- `src/search_lns.py`, `src/greedy_full.py`: Suche auf dem Quotientenwürfel.
- `src/construct_32.py`: Konstruktion der 32-Wörter-Codes.
