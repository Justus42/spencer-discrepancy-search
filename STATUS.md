# STATUS

Stand: 24.11.2026. Timebox: 23.11.2026 bis 04.01.2027, danach Entscheidung über Abbruch.

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
- 24.11.2026, Rundgang (18,5), d=2, Zufallsstart, je 300 s, Startwerte 1–4: K=15 (60 Wörter) in allen drei Typen ungelöst
  ((11,7) Restzahl 36–38, (10,8) 48–57, (9,9) 76–90), 12 Läufe à 300 s = 15 Min. K=16: (9,9) und (10,8) ungelöst (je 4 x 300 s), (11,7) gelöst.

- 23.11.2026, Lauf n=20 R=6 d=2 K=13 Typ (11,9) Startwerte 1–16, je 1800 s: durch Container-Neustart verloren, keine Restzahlen
  (Details im Abschnitt „Lauf K=13“).
- 24.11.2026, Kontrolle n=22 R=7 d=2 K=13, Zufallsstart, je 1800 s, Cloud 4 Kerne: Typ (13,9) Startwerte 1–8 alle ungelöst
  (Restzahlen 7, 2, 6, 7, 2, 2, 3, 1); Typ (12,10) Startwerte 3 und 7 ungelöst (Restzahl 8, 8). Details im Abschnitt „Kontrolle Skalierung“.
- 24.11.2026, Hadamard-Tests (Abschnitt „Hadamard-Bauregel und Tests“):
  - Download der Sloane-Bibliothek (Ordnung 24, 60 Klassen) nicht möglich: `neilsloane.com` nicht auflösbar, `https://` über den Proxy 403.
    Test A läuft deshalb nur auf konstruierbaren Klassen (53 von 60).
  - Erster Klassen-Fingerabdruck (Histogramm der 4-Zeilen-Summen) trennt bei Ordnung 20 nichts: 72 Matrizen, 1 Schlüssel statt 3 (Laufzeit 1 s).
    Ersetzt durch Typen aller 6-Zeilen-Mengen (trennt bei Ordnung 20 genau 3 Klassen).
  - Installation von `pynauty` für kanonische Graphformen wurde von der Sitzung nicht freigegeben und nicht umgangen; eigene Invarianten statt dessen.

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

- Der Forenbeitrag auf „EinsteinArena“ ist **belegt** (Thread 273, FableCover, 21.09.2026), Inhalt laut Prüfvermerk in
  `notes/gemini/neuheit_2026-11-24_kurz.md`: gleiche Reduktion, nur n <= 20, K(20,6) als „known 24..64“, kein Code unter 64 gemeldet.
  (Angabe des Orchestrator-Claude, in dieser Sitzung nicht erneut abgerufen.) `Mapika/coldcase` hat laut Prüfvermerk nur Ergebnisse für q >= 5, keine binären.
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
- **Maharaj (arXiv:2610.03760) ist kein Dämpfer.** Korrektur 23.11.2026 (Angabe des Nutzers): Das Paper behandelt
  quaternäre Preparata-Codes und sagt nichts über binäre Zellen. Die frühere Lesart (systematische Tabu-Suche über alle
  offenen Zellen mit q <= 4) ist zurückgenommen. Offen bleibt nur, dass K(20,3) im Oktober 2026 von Dritten verbessert wurde.
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
**Gate-Prüfung 23.11.2026 (erstes Gate, 60 Wörter):** Treffer von Claude mit eigenem drittem Skript bestätigt
(nicht im Repo), zusätzlich Brute-Force-Radius in `coset_ladder.py`, `check_disc.py`, `verify_spencer.py`. Neuheit offen: Kéri-Tabelle K(20,6) = 24–64 (Stand 2011); Literatur danach
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

**Gate-Prüfung 23.11.2026 (zweites Gate, 56 Wörter, K=14, Typ (11,9), Startwerte 2 und 3):** Die 56 Wörter wurden von Claude
mit eigenem drittem Skript bestätigt (nicht im Repo). Die Neuheitsprüfung läuft über Gemini; bis dahin gilt Neuheit als offen.

## Neuheit K(20,6) <= 56 (Stand 23.11.2026, Neuheit weiter offen)

- Gemini fand nichts unter 64.
- Angabe des Nutzers, in dieser Sitzung von Claude nicht selbst nachgeprüft: `florath/covering-codes-lean` (nach Kéri,
  Commit 16.09.2026) nennt für K(20,6) weiter 24–64. Kéri-Nachbarzellen schließen Standardkonstruktionen aus.
- `fohn.app`: unbelegt.
- Geminis ausführlicher Neuheitsbericht: Stand 24.11.2026 nicht im Repo angekommen (auf keinem Branch); Platzhalter
  `notes/gemini/neuheit_2026-11-23.md` bleibt, bis die Datei vorliegt.

## Normalität und Folgecodes aus den 56 Wörtern (23.11.2026)

`src/normal_ext.py`, Brute-Force über alle 2^n Punkte. Beide 56er-Codes (K=14, Typ (11,9), Startwerte 2 und 3) sind
**normal: Norm 13 = 2R+1 bei allen 20 Koordinaten**. Verdreifachen einer Koordinate (je Schritt Länge +2, Radius +1,
gleiche 56 Wörter, selbstkomplementär; jedes Mal vorher Normalität nachgeprüft, immer dieselbe Koordinate):
Länge 22 (R=7), 24 (R=8), 26 (R=9), jeweils Radius per Brute-Force bestätigt.
Wörter `runs/woerter_ext56_typ11-9_seed{2,3}_n{22,24,26}_R{7,8,9}.txt`, Matrizen `runs/matrix_ext56_n*_*.txt`
(28 x n, `_pad` = 28 x 28 für `verify_spencer.py`).
**Alle 6 Matrizen: `check_disc.py` und `verify_spencer.py` melden exakt Diskrepanz 8** (Quotient 8/sqrt(28) = 1,512, kein
Spencer-Rekord).
**Korrektur 24.11.2026:** K(26,9) <= 56 und K(28,10) <= 56 stehen schon in der Tabelle (Östergård–Kaikkonen 1998); die
56er-Folgecodes bei Länge 26 sind also nicht neu. Selbst nachgeprüft am 24.11.2026 in `florath/covering-codes-lean`,
`reference-data/post-keri/non_mixed_covering_codes.csv` (Stand `main` beim Abruf), Zeilen q=2:

| Zelle | Tabelle | eigene Codes |
|---|---|---|
| K(20,6) | 24–64 (obere: trivial) | 56, 52 |
| K(22,7) | 21–64 (obere: trivial) | 56, 52 |
| K(24,8) | 19–64 (obere: trivial) | 56, 52 |
| K(26,9) | 17–56 (Östergård–Kaikkonen 1998) | 56 (nicht neu), 52 |
| K(28,10) | 15–56 (Östergård–Kaikkonen 1998) | nicht erzeugt |

Die 52er-Codes lägen in allen vier erzeugten Zellen unter dem Tabellenwert, auch bei K(26,9) (52 < 56). Das ist nur der
Abgleich mit dieser einen Tabelle; Neuheit gilt erst nach Regel 3 als geprüft. Nichts nach außen gemeldet.

## Lauf K=13, nur Typ (11,9), Startwerte 1–16 (gestartet 23.11.2026, 19:03 UTC)

Länge 20, Radius 6, Dimension 2, K=13 (52 Wörter), je 1800 s, 4 parallel, harte Obergrenze 2,5 h (`src/run_k13_typ119.sh`).
Logs: `runs/2026-11-23_K13_typ11-9_seed*.log.txt`. Kein K=12 ohne Freigabe. Auswertung auf Zuruf (Regel 4).

**Ergebnis: Lauf abgebrochen, keine Auswertung möglich.** Beim Nachsehen um 21:28 UTC war der Container neu gestartet
(Betriebszeit 0 Min., kein Prozess, keine Fertig-Marke). Es existieren nur die Logs der Startwerte 1–4, alle leer
(0 Bytes, zuletzt geschrieben 19:03; der Treiber schreibt erst am Laufende). Startwerte 5–16 wurden nie gestartet.
Es gibt also weder Treffer noch Restzahlen, kein Läufer hat seine 1800 s beendet; der Abbruchzeitpunkt ist unbekannt.
Nicht neu gestartet (Anweisung: nichts Neues starten). Eintrag unter Fehlversuche: Lauf 23.11.2026, n=20 R=6 d=2 K=13
Typ (11,9) Startwerte 1–16, je 1800 s: durch Container-Neustart verloren, Laufzeit unbekannt (< 2,4 h), kein Ergebnis.

### Neustart in Blöcken (ab 21:34 UTC, `src/run_block.py`, Log läuft laufend, Fertig-Marke je Startwert)

Block 1 (Startwerte 1–4, je 1800 s, 21:34–22:05 UTC): **2 von 4 gelöst, 52 Wörter (K=13) bei (20,6).**
- Startwert 1: nicht gelöst, beste Restzahl 2. Startwert 2: nicht gelöst, beste Restzahl 2.
- **Startwert 3 und 4: GELÖST**, 52 Wörter, selbstkomplementär, Radius per Brute-Force 6. Wörter
  `runs/woerter_n20_R6_d2_K13_typ11-9_seed{3,4}.txt`, Matrizen `runs/matrix_n20_52w_typ11-9_seed{3,4}.txt` (26 x 20, `_pad` 26 x 26).
- **Beide Prüfskripte, beide Treffer: Diskrepanz 8** (`check_disc.py` auf 26 x 20, `verify_spencer.py` auf 26 x 26;
  Quotient 8/sqrt(26) = 1,569, kein Spencer-Rekord, dafür wären <= 40 Wörter nötig). Regel 1 erfüllt. Neuheit offen (Regel 3).
- Normalität: beide 52er-Codes normal, Norm 13 bei allen 20 Koordinaten. Folgecodes (Koordinate verdreifacht): Länge 22 (R=7),
  24 (R=8), 26 (R=9), je 52 Wörter, Radius per Brute-Force bestätigt (`runs/woerter_ext52_*`, `runs/matrix_ext52_*`).
  Alle 6 Matrizen: beide Prüfskripte melden Diskrepanz 8.
Block 2 (Startwerte 5–8, je 1800 s, 22:07–22:37 UTC): **2 von 4 gelöst, 52 Wörter.**
- Startwert 5 und 6: **GELÖST**, 52 Wörter, Radius per Brute-Force 6; Wörter `runs/woerter_n20_R6_d2_K13_typ11-9_seed{5,6}.txt`,
  Matrizen `runs/matrix_n20_52w_typ11-9_seed{5,6}.txt`. Beide Prüfskripte: Diskrepanz 8 (26 x 20 bzw. 26 x 26).
  Beide Codes normal (Norm 13, alle 20 Koordinaten); Folgecodes Länge 22/24/26 (R=7/8/9), je 52 Wörter, Radius per Brute-Force
  bestätigt, alle 6 Matrizen von beiden Prüfskripten mit Diskrepanz 8 bestätigt.
- Startwert 7: nicht gelöst, beste Restzahl 7. Startwert 8: nicht gelöst, beste Restzahl 2.

Block 3 (Startwerte 9–12, je 1800 s, 22:39–23:09 UTC): **2 von 4 gelöst, 52 Wörter.**
- Startwert 10 und 12: **GELÖST**, 52 Wörter, Radius per Brute-Force 6; Wörter `runs/woerter_n20_R6_d2_K13_typ11-9_seed{10,12}.txt`,
  Matrizen `runs/matrix_n20_52w_typ11-9_seed{10,12}.txt`. Beide Prüfskripte: Diskrepanz 8. Beide Codes normal (Norm 13, alle
  Koordinaten); Folgecodes Länge 22/24/26 je 52 Wörter, Radius bestätigt, alle 6 Matrizen von beiden Prüfskripten Diskrepanz 8.
- Startwert 9: nicht gelöst, beste Restzahl 2. Startwert 11: nicht gelöst, beste Restzahl 7.

Block 4, erster Versuch (Startwerte 13–16, ab 23:11 UTC): durch Container-Neustart um ca. 23:36 UTC abgebrochen, ohne Ergebnis
(nach 1303/1250/1286/936 s Laufzeit, beste Restzahl bis dahin 2/2/6/2; Teillogs `runs/*seed1[3-6]_abgebrochen.log.txt`).
Zweiter Versuch von Block 4 gestartet 23:37 UTC.
Block 4, zweiter Versuch (Startwerte 13–16, je 1800 s, 23:37–00:08 UTC): **0 von 4 gelöst.** Beste Restzahl je Startwert:
13: 2, 14: 2, 15: 6, 16: 2.

**Gesamtergebnis K=13, Typ (11,9), Startwerte 1–16, je 1800 s (Länge 20, R=6, d=2): 6 von 16 gelöst, 52 Wörter.**
- Gelöst: Startwerte 3, 4, 5, 6, 10, 12. Alle sechs Treffer: Radius per Brute-Force 6, selbstkomplementär, beide Prüfskripte
  Diskrepanz 8 (Quotient 8/sqrt(26) = 1,569, kein Spencer-Rekord), alle sechs normal (Norm 13, alle Koordinaten), Folgecodes für
  Länge 22/24/26 (R=7/8/9) mit je 52 Wörtern von beiden Prüfskripten bestätigt (Diskrepanz 8).
- Nicht gelöst, beste Restzahl: Startwert 1: 2, 2: 2, 7: 7, 8: 2, 9: 2, 11: 7, 13: 2, 14: 2, 15: 6, 16: 2.
- Kein K=12 gestartet (nicht freigegeben). Neuheit der 52 Wörter bei (20,6) offen (Regel 3): Gemini-Bericht steht aus.

**Gate-Prüfung 24.11.2026 (drittes Gate, 52 Wörter):** Die sechs 52er-Codes (Startwerte 3, 4, 5, 6, 10, 12) wurden vom
Orchestrator-Claude mit eigenem Skript bestätigt (nicht im Repo): Radius 6, selbstkomplementär, normal, Länge 22 mit Radius 7,
alle verschieden. Zusätzlich in dieser Sitzung nachgerechnet: die sechs Wortmengen sind paarweise disjunkt (kein gemeinsames Wort).

- Der frühere Lauf mit leeren Logs ist damit überschrieben (Logs der Startwerte 1–4 stammen jetzt aus Block 1).

## Warmstart und K=12 (24.11.2026)

`src/coset_tabu.c` nimmt jetzt optional eine Datei mit Startmitten (Warmstart; jeder Neustart beginnt dort).
`src/warm_prep.py` rechnet Wörter auf Nebenklassen-Syndrome zurück und streicht die Nebenklasse, deren Wegfall am wenigsten
Syndrome aufdeckt. Treiber `src/run_warm.py` (laufendes Log, Fertig-Marken). Logs und Startdateien in `runs/warm/`.

**Positivkontrolle K=13 aus den 56er-Codes** (je 1 Nebenklasse gestrichen: 8 bzw. 9 Syndrome offen), 8 Läufe je 900 s:
**3 von 8 gelöst** (Startwert 2 nach 8 s, 3 nach 173 s, 6 nach 77 s). Ungelöst, beste Restzahl: Startwert 1: 3, 4: 5, 5: 6,
7: 5, 8: 2. Vergleich Zufallsstart: 6 von 16 in 1800 s, davon 1 unter 900 s. Schwelle (>= 2 von 8) erreicht.
Die drei Kontroll-Treffer (52 Wörter): beide Prüfskripte Diskrepanz 8.

**K=12 (48 Wörter) ohne Suchlauf gefunden.** Beim Vorbereiten der Warmstarts zeigte sich: In den 52er-Codes der Startwerte
5 und 6 ist je eine Nebenklasse überflüssig (ihr Wegfall deckt 0 Syndrome auf). Ohne sie bleiben 48 Wörter:
- `runs/woerter_n20_R6_d2_K12_typ11-9_aus52seed{5,6}.txt`: je 48 Wörter, selbstkomplementär, Radius per Brute-Force 6,
  die beiden Codes sind verschieden.
- Matrizen `runs/matrix_n20_48w_typ11-9_aus52seed{5,6}.txt` (24 x 20, `_pad` 24 x 24). **Beide Prüfskripte, beide Codes:
  Diskrepanz 8**, Quotient 8/sqrt(24) = 1,633. Kein Spencer-Rekord (dafür <= 40 Wörter nötig). Regel 1 erfüllt.
- **Normal:** Norm 13 bei allen 20 Koordinaten. Folgecodes (Koordinate verdreifacht) Länge 22/24/26 mit R=7/8/9 und je
  48 Wörtern, Radius per Brute-Force bestätigt (`runs/woerter_ext48_*`); für diese keine Prüfskripte gerechnet.
- Tabellenabgleich (florath, siehe oben): K(20,6) 24–64, K(26,9) 17–56. 48 läge darunter. Neuheit nach Regel 3 offen.
- Laut Anweisung nach dem Treffer gestoppt: Der geplante K=12-Suchlauf (16 x 1800 s) wurde **nicht** gestartet. Kein K=11.
  Nicht geprüft: ob die übrigen 52er-Codes (auch die drei aus der Kontrolle) ebenfalls überflüssige Nebenklassen haben.

**Gate-Prüfung 24.11.2026 (viertes Gate, 48 Wörter):** Die beiden 48er-Codes (aus 52er Startwert 5 und 6) wurden vom
Orchestrator-Claude mit eigenem Skript bestätigt (nicht im Repo).
- **Befund:** Streichen jeder einzelnen der 12 Nebenklassen lässt in beiden Codes **exakt 6108 Punkte** von F2^20 offen
  (= 1527 Syndrome, je 4 Punkte). In dieser Sitzung nachgerechnet (Brute-Force über alle 2^20 Punkte, alle 12 Streichungen,
  beide Codes: 12 x 6108). Keine Nebenklasse ist überflüssig; der billige Weg zu K=11 ist damit zu.
- **Kein Versuch auf K=11** (Anweisung).
- **Folgecodes Länge 22/24/26 (R=7/8/9), je 48 Wörter, beide Prüfskripte:** Matrizen `runs/matrix_ext48_n{22,24,26}_typ11-9_aus52seed{5,6}.txt`
  (24 x n; `_pad` = 24 x 24 bei n=22). `check_disc.py` und `verify_spencer.py` melden für **alle 6 Matrizen exakt Diskrepanz 8**
  (Ausgaben `runs/ext48/`, Skript `runs/ext48/pruefe.sh`, 67 s auf 4 Kernen). Regel 1 erfüllt. Quotient höchstens 8/sqrt(24) = 1,633,
  kein Spencer-Rekord. Tabellenabgleich (florath): K(22,7) 21–64, K(24,8) 19–64, K(26,9) 17–56; 48 läge jeweils darunter.
  Neuheit nach Regel 3 offen.

## Machbarkeit (25,8), Dimension 2, K=12 (48 Wörter), keine Suche (24.11.2026)

Quotient F2^23 (m = 25 - 2), also 2^23 = 8 388 608 Syndrome; Bild der Kugel |S| = 1 730 099 (20,6 % von 2^23, beide Typen),
untere Schranke K >= 4,85. Messmodus im Kern per Compile-Schalter (normaler Lauf unverändert): `-DMESS=200` bricht nach
200 Schritten ab und meldet die Zeit je Schritt, `-DUEBERLAUF` prüft zusätzlich jede Addition, Subtraktion und
Multiplikation der Walsh-Bewertung mit `__builtin_*_overflow` und merkt sich den größten Betrag. Treiber `src/mess_schritt.py`,
Ausgaben `runs/mess258/`.

**64-Bit-Überlauf: ausgeschlossen (Beweis plus Messung).**
1. Erste Walsh-Transformation auf der Loch-Indikatorfunktion a in {0,1}: jeder Zwischenwert ist eine ±-Summe von Einträgen
   von a, also Betrag <= Zahl der Löcher <= 2^23.
2. Ŝ (Walsh-Transformierte der Kugel-Indikatorfunktion, einmal vorab): Betrag <= |S| < 2^21, Zwischenwerte ebenso.
3. Produkt B(s) = â(s)·Ŝ(s): Betrag < 2^23 · 2^21 = 2^44.
4. Zweite Transformation: jeder Zwischenwert ist eine ±-Summe von Werten B(s), also Betrag <= Σ|B(s)|. Nach Cauchy-Schwarz
   und Parseval (Σ â² = 2^23 · Löcher, Σ Ŝ² = 2^23 · |S|) gilt Σ|B| <= 2^23 · sqrt(Löcher · |S|) < 2^23 · sqrt(2^23 · 2^21) = 2^45.
   Endwert = 2^23 · (Löcher in x+S) <= 2^23 · |S| < 2^44.
5. Alle Werte < 2^45, Grenze von int64 ist 2^63: Reserve 2^18. Zähler `cnt` (8 Bit) hält höchstens K = 12.
Messung (je 200 Schritte, beide Typen): größter Betrag 1 820 873 195 520 (2^40,73) bei (13,12), 1 845 862 858 752 (2^40,75)
bei (14,11); kein Überlauf-Flag gesetzt.

**Zeit je Schritt (200 Schritte, Startwert 1, ohne Vorbereitung; Vorbereitung je 0,07–0,10 s):**

| Typ | allein (1 Lauf) | 4 Läufe gleichzeitig |
|---|---|---|
| (13,12) | 0,475 s | 0,581 / 0,582 s (Startwerte 1, 2) |
| (14,11) | 0,504 s | 0,570 / 0,588 s (Startwerte 3, 4) |

**Einordnung (Rechnung, keine Messung auf dem PC):** Bei (20,6), m = 18, 4 parallel: ca. 4,7 ms je Schritt
(z. B. 386 528 Schritte in 1800 s); die sechs K=13-Treffer brauchten 48 408 bis 342 182 Schritte. Bei (25,8) sind es
ca. 120-mal mehr Zeit je Schritt (32-mal mehr Syndrome, Rest Speicher: 2 x 64 MB Arbeitsfelder passen nicht in den Cache).
1800 s ergeben nur ca. 3 100 Schritte; der Neustart nach 3 000 Schritten ohne Verbesserung käme also höchstens einmal.
Bräuchte (25,8) ähnlich viele Schritte wie (20,6), wären das 8–55 h je Lauf auf einem Cloud-Kern. Nach 200 Schritten
standen noch 31 500 (Typ 13,12) bzw. 16 070 (Typ 14,11) Syndrome offen. Der Ryzen 9800X3D (96 MB L3-Cache) und die
GPU sind nicht gemessen. Keine Suche gestartet.

## Kontrolle Skalierung (22,7), Dimension 2, K=13 (52 Wörter), Zufallsstart (ab 24.11.2026, 06:35 UTC)

Quotient F2^20, |S| = 266 080 (25,4 %), untere Schranke K >= 3,94. Existenz von 52 und 48 Wörtern bei (22,7) ist durch die
Folgecodes belegt (nicht notwendig als Nebenklassen eines solchen D). Typen (13,9), (12,10), (11,11), je Startwerte 1–8,
je 1800 s, Blöcke zu 4. Treiber `src/run_gen.py`, Aufruf `src/run_skal22.sh typ von bis`; Logs `runs/skal22/`.
Gegentest vorab: Der normale Kern reproduziert die Warmstart-Kontrolle K=13 Startwert 2 (Startdatei aus 56er Startwert 3)
mit derselben Lösung nach Schritt 2011, der Messmodus ändert also nichts.

- Block 1, Typ (13,9), Startwerte 1–4 (06:35–07:05 UTC): **0 von 4 gelöst.** Beste Restzahl: 7 / 2 / 6 / 7.
  Je ca. 62 000 Schritte in 1800 s (ca. 29 ms je Schritt bei 4 parallel; zum Vergleich (20,6): ca. 386 000).
- Block 2, Typ (13,9), Startwerte 5–8 (07:05–07:35 UTC): **0 von 4 gelöst.** Beste Restzahl: 2 / 2 / 3 / **1**.
- Block 3, Typ (12,10), Startwerte 1–4 (07:35–08:05 UTC): **3 von 4 gelöst**, 52 Wörter bei (22,7): Startwert 1 nach 799 s
  (28 330 Schritte), 2 nach 84 s (3 012), 4 nach 377 s (13 429). Startwert 3: beste Restzahl 8. Alle drei selbstkomplementär,
  Radius per Brute-Force 7. Wörter `runs/skal22/woerter_n22_R7_d2_K13_typ12-10_seed{1,2,4}.txt`, Matrizen
  `runs/skal22/matrix_n22_52w_typ12-10_seed{1,2,4}.txt` (26 x 22, `_pad` 26 x 26). **Beide Prüfskripte, alle drei: Diskrepanz 8.**
- Block 4, Typ (12,10), Startwerte 5–8 (08:06–08:36 UTC): **3 von 4 gelöst**, 52 Wörter: Startwert 5 nach 604 s (20 365 Schritte),
  6 nach 1686 s (57 794), 8 nach 50 s (1 720). Startwert 7: beste Restzahl 8. Selbstkomplementär, Radius per Brute-Force 7.
  Wörter/Matrizen wie oben mit `seed{5,6,8}`. **Beide Prüfskripte, alle drei: Diskrepanz 8.** Die sechs (12,10)-Codes sind paarweise verschieden.
- Block 5, Typ (11,11), Startwerte 1–4 (08:36–08:43 UTC): **4 von 4 gelöst**, 52 Wörter: nach 361 s (11 459 Schritte), 40 s (1 230),
  198 s (6 524), 389 s (12 362). Selbstkomplementär, Radius per Brute-Force 7. Wörter `runs/skal22/woerter_n22_R7_d2_K13_typ11-11_seed{1..4}.txt`,
  Matrizen `runs/skal22/matrix_n22_52w_typ11-11_seed{1..4}.txt` (26 x 22, `_pad` 26 x 26). **Beide Prüfskripte, alle vier: Diskrepanz 8.**
- Block 6, Typ (11,11), Startwerte 5–8 (08:43–08:53 UTC): **4 von 4 gelöst**, 52 Wörter: nach 255 s (8 049 Schritte), 592 s (19 355),
  87 s (2 738), 45 s (1 380). Selbstkomplementär, Radius per Brute-Force 7. **Beide Prüfskripte, alle vier: Diskrepanz 8.**

**Gesamtergebnis Skalierung (22,7), K=13, Zufallsstart, je 1800 s: 14 von 24 gelöst.**

| Typ | gelöst | Zeit bis Lösung | beste Restzahl der Fehlläufe |
|---|---|---|---|
| (13,9) | 0 von 8 | – | 7, 2, 6, 7, 2, 2, 3, 1 |
| (12,10) | 6 von 8 | 50–1686 s | 8, 8 (Startwerte 3, 7) |
| (11,11) | 8 von 8 | 40–592 s | – |

- Alle 14 Treffer: 52 Wörter, selbstkomplementär, Radius 7 per Brute-Force, paarweise verschieden; `check_disc.py` und
  `verify_spencer.py` melden für alle 14 exakt Diskrepanz 8 (Quotient 8/sqrt(26) = 1,569, kein Spencer-Rekord). Regel 1 erfüllt.
- Die Methode skaliert damit von 2^18 auf 2^20 Syndrome: Bei (20,6) gelangen mit dem besten Typ (11,9) 6 von 16 in 1800 s, bei
  (22,7) mit dem besten Typ (11,11) 8 von 8, trotz ca. 6-mal weniger Schritten je Lauf. Der ausgewogene Typ ist hier am besten,
  der unausgewogenste (13,9) löst nie (bei (20,6) war es umgekehrt: (10,10) schlecht, (11,9) gut).
- Neuheit: 52 lag schon über die Folgecodes vor (K(22,7) Tabelle 21–64). Nach Regel 3 offen. Kein K=12 bei (22,7) gestartet.


## Nebenfrage: Typ (12,8) bei K=13 immer exakt 22

Kein Startwert-Fehler im Code erkennbar: `coset_tabu.c` setzt das Zufallsgerät pro Startwert verschieden und würfelt die
Startmitten neu. Kurzläufe (60 s, bei Last durch den Hauptlauf) mit Startwert 1 und 11 liefen verschieden (28 bzw. 39 offen,
verschiedene Verläufe), erreichen die 22 in dieser Zeit also nicht. Echtes Plateau damit wahrscheinlich, aber nicht bewiesen:
4 unabhängige Läufe von 600 s enden alle bei 22, kein Lauf darunter. Entscheidung offen (siehe Ergebnis unten, falls ergänzt).

## Gate-Prüfung 24.11.2026 (fünftes Gate) und Angaben des Orchestrator-Claude

Von dieser Sitzung **nicht** nachgeprüft, nur übernommen (kein Skript möglich oder außerhalb des Repos):
- Kontrolle (22,7) bestätigt, 3 von 14 Treffern mit eigenem Skript des Orchestrators geprüft; (25,8) blind zurückgestellt (Entscheidung, kein Lauf).
  Im Repo selbst liegen für alle 14 Treffer beide Prüfskripte vor (Abschnitt „Kontrolle Skalierung“).
- Zielrepo `teorth/optimizationproblems`, Seite `constants/10c.md`, Commit vom 23.11.2026: Rekord unverändert 5/sqrt(8), die Seite beschreibt dieselbe Reduktion.
- Literatur: Reis, Song, arXiv:2609.34471 existiert (Orchestrator geprüft, Prüfvermerk `hadamard_2026-11-24_deep-research.md`); Solé u. a. 2021, hal-03240109 **ungeprüft**
  (Abruf blockiert). Mit Hadamard-Matrizen der Ordnung 20/24 sind die Zahlen dort nicht übernommen.

Selbst nachgerechnet in dieser Sitzung (siehe nächster Abschnitt): der Strukturbefund mit den Zählungen 660/1980 und 1320/2970/66.

## Hadamard-Bauregel und Tests (24.11.2026, Cloud, Python/numpy ganzzahlig, C für die Gegenbeispielsuche)

**Bauregel (Teil B, ohne Suche).** `src/construct_48.py`: H = Paley-Matrix der Ordnung 12 (q=11), M = [[H,H],[H,-H]] (24 x 24).
Links Spalte 0, rechts Spalten 0,1,2 streichen (1+3, Überlappung) gibt 24 x 20; ebenso 2+2 mit links {0,1}, rechts {0,2} (Überlappung 1).
Dateien `controls/had24_1p3_paley12.txt` und `controls/had24_2p2_paley12.txt` (24 x 20), `_pad` (24 x 24, Nullspalten, für `verify_spencer.py`).
**`check_disc.py` und `verify_spencer.py` melden für beide Matrizen exakt Diskrepanz 8** (Regel 1 erfüllt). Quotient 8/sqrt(24) = 1,633, kein Spencer-Rekord.
Brute-Force über alle 2^20 Punkte (`hadamard_tests.py radius`): beide ergeben 48 Wörter (24 Zeilen + Komplemente) mit Überdeckungsradius 6, also (20,6) mit 48 Wörtern
**ohne Suche konstruierbar** (`runs/2026-11-24_construct48_radius.log.txt`). Neuheit offen (Regel 3), Stand wie oben: Tabelle 24–64.

**Kontrollen (`src/hadamard_tests.py kontrolle`, 244 s, `runs/2026-11-24_hadamard_kontrolle.log.txt`), alle bestanden:**
- Sylvester 8 ohne je eine Spalte: Diskrepanz 5 in allen 8 Fällen (volle Matrix 4). Paley 12 voll 4, M voll 6.
- 1+3 aus M: 660 Fälle mit Überlappung = Diskrepanz 8, 1980 ohne = 6. 2+2: Überlappung 1 = 8 (1320), Überlappung 0 = 6 (2970), Überlappung 2 = 6 (66).
  0+4: 495 Fälle immer 6. 1+1: verschiedene Spalten 8 (132), gleiche Spalte 6 (12). Das bestätigt den Strukturbefund des Orchestrators vollständig.
- Gegenprobe gegen `check_disc.py` an 6 Stichproben: gleich.

**Test B (Ordnung 20, volle Matrix; `runs/2026-11-24_hadamard_testB.log.txt`, 14 s):** 72 gebaute Matrizen (Paley I q=19, Paley II q=9, Goethals–Seidel n=5,
zufällige Ergänzungen aus 4–7 Zeilen) fallen in genau **3 Klassen** (Schlüssel = Typenverteilung der 6-Zeilen-Mengen; bekannt sind 3). Alle drei: Diskrepanz **6**, auch
die Transponierten (erwartet 6). Beide Prüfskripte bestätigen 6 (`runs/2026-11-24_hadamard_testB_pruefskripte.log.txt`). Vertreter `controls/had20_klasse{1,2,3}.txt`
(Klasse 1 = Goethals–Seidel-Vertreter, 2 = Paley II, 3 = Paley I). Kein Rekordkandidat (8 nötig).

**Test A (Ordnung 24; `runs/2026-11-24_hadamard_testA.log.txt`, `..._lauf2.log.txt`):** Sloane-Bibliothek nicht erreichbar (siehe Fehlversuche), deshalb nur konstruierbare Klassen:
Paley I (q=23), Doppelung [[A, P·A],[A, -P·A]] mit A = Paley 12 und 60 zufälligen Zeilenpermutationen P (plus P = id), 60 Goethals–Seidel-Matrizen n=6 und zufällige
Ergänzungen von 4–8 vorgegebenen Zeilen (215 + 196 Stück; gierig mit Neustart). Ergebnis: **53 verschiedene Klassen** (Lauf 1: 52 in 413 s, Lauf 2 mit anderem Startwert: 1 neue in 347 s;
verschiedene Schlüssel bedeuten sicher verschiedene Klassen). **Alle 53: volle Diskrepanz 6, ihre Transponierten ebenfalls 6; keine mit 8.** Damit entfällt die Spaltenstreichung,
Rekordschwelle (24 x 23 mit Diskrepanz 9, nur aus 8 möglich, denn disc ändert sich beim Streichen einer Spalte um höchstens 1) wird nirgends erreicht.
**Lücke:** 7 der 60 Klassen sind nicht erfasst; dass eine davon Diskrepanz 8 hat, ist nicht ausgeschlossen. Dafür wäre die Bibliothek nötig. Je Schlüssel stimmten bis zu 3 Stichproben überein.
Matrizen: `runs/hadamard/h24_klasse01..53.txt`.

**Test C (Ordnung 40, M = [[H,H],[H,-H]], H aus den 3 Klassen der Ordnung 20, 40 x 36, Rekord bei Diskrepanz 12):** Suchcode `src/gegenbeispiel40.c` (Tabu-Suche), Prüfung getrennt
und ganzzahlig in `hadamard_tests.py pruefe_c` (zählt die Auswahlen selbst und rechnet jedes x nach). „Muster“ gelesen als: Klasse x Bauregel-Typ, über **alle** Spaltenauswahlen
(1+3 mit Überlappung: 3420 je Klasse; 2+2 mit Überlappung 1: 6840 je Klasse; zusammen 30 780). Das genügt für alle Äquivalenzen, weil sich jede Auswahl in M(H^T) oder nach
Zeilen-/Spaltenoperationen auf eine in M(H') mit H' aus den 3 Klassen zurückführen lässt: Ist H = P·H'·Q mit vorzeichenbehafteten Permutationsmatrizen, so ist M(H) = (I⊗P)·M(H')·(I⊗Q),
die Spaltenauswahl wandert mit Q in beiden Hälften gleich mit (Vorzeichen ändern die Diskrepanz nicht). Die Transponierte von H liegt wieder in einer der 3 Klassen. (Argument, nicht per Skript geprüft.)
- **T = 10 (Gegenbeispiel = Vorzeichenvektor mit allen |(Ax)_i| <= 10, also Diskrepanz <= 10 < 12):** für **alle 30 780 Auswahlen gefunden und ganzzahlig nachgerechnet**, 0 offen, 0 fehlend,
  0 falsch (je Klasse und Muster 0,0–0,1 s; `runs/2026-11-24_testC_*_T10.log.txt`, Prüfung `runs/2026-11-24_testC_block1_pruefung.log.txt`).
- Zusatz T = 8: ebenfalls für alle 30 780 Auswahlen gefunden und nachgerechnet (je Muster 18–35 s; `runs/2026-11-24_testC_zusatz_T8.log.txt`). Diskrepanz also schon <= 8, weit unter 12.
- **Ergebnis: Die Bauregel überträgt sich nicht auf Ordnung 40.** Alle Muster erledigt; Punkt 10 (kein Gegenbeispiel) trat nicht ein, ein Plan für 2^36 Vektoren ist nicht nötig.
  Gesamtzeit Test C: wenige Minuten statt 60 Minuten Budget, kein Block über 30 Minuten.
- **Grenze der Aussage:** getestet ist nur H' = H (gleiche Zeilenausrichtung in beiden Hälften). Die 48er-Codes aus der Suche haben laut Prüfvermerk die Form [[U,W],[U,-W]] mit
  U, W aus möglicherweise verschiedenen Hadamard-Matrizen der Ordnung 12. Zeilenausrichtungen P in [[H, P·H],[H, -P·H]] bei Ordnung 40 sind nicht getestet (20! Möglichkeiten).

## Gate-Prüfung 24.11.2026 (sechstes Gate): Hadamard-Tests bestätigt, Test A vollständig

Vom Orchestrator-Claude mit eigenen Skripten gerechnet (nicht im Repo, kein Code aus `src/`):

- `controls/had24_1p3_paley12.txt` und `controls/had24_2p2_paley12.txt`: Diskrepanz 8, Spalten orthogonal.
- Test B: die drei Dateien `controls/had20_klasse*.txt` sind Hadamard-Matrizen, Diskrepanz 6, Transponierte 6.
- Test A, die 53 Dateien in `runs/hadamard/`: alle Hadamard-Matrizen, alle Diskrepanz 6.
- Test C: alle 30 780 Gegenbeispiele aus den Logs nachgerechnet, größter Betrag 10, jede Auswahl mit Überlappung 1.
- **Test A ist damit vollständig.** Die Lücke von 7 Klassen ist geschlossen: `controls/had24_mckay_hex.txt` enthält alle
  60 Klassen der Ordnung 24 aus Brendan McKays Sammlung (users.cecs.anu.edu.au/~bdm/data/hadamard.html, Datei
  `had24.txt`; je Zeile eine Matrix, 24 Hexadezimalzahlen, eine je Matrixzeile, Bit 0 bedeutet -1). Die Datei kam
  über einen Webabruf mit Textwiedergabe; alle 60 Zeilen ergeben gültige, paarweise verschiedene Hadamard-Matrizen
  (H·H^T = 24·I geprüft). Dass es 60 verschiedene Klassen sind, ist Angabe der Quelle und nicht selbst geprüft.
  **Alle 60 haben Diskrepanz 6, die Transponierten ebenfalls.** Jede 24 x 23-Untermatrix hat also höchstens 7,
  die Rekordschwelle 9 wird bei Ordnung 24 nicht erreicht. `pynauty` wird nicht gebraucht.
- Offen für die nächste Sitzung: die 60 Matrizen mit `src/hadamard_tests.py` gegenrechnen.

**Stand der Hadamard-Route:** abgeschlossen ohne Rekordkandidat (Ordnung 20 voll, Ordnung 24 ohne eine Spalte,
Bauregel bei Ordnung 40 mit gleicher Zeilenausrichtung). Die Konstruktion der 48 Wörter bei (20,6) ohne Suche steht.

## Gegenrechnung McKay-60 (24.11.2026)

`src/pruefe_mckay.py` (nutzt `exakte_disc` und `pruefe_hadamard` aus `src/hadamard_tests.py`, kein Suchcode) liest
`controls/had24_mckay_hex.txt`: 60 Matrizen, alle Hadamard (H·H^T = 24·I), paarweise verschieden, **alle 60 Diskrepanz 6, alle
60 Transponierten ebenfalls 6** (59 s, `runs/2026-11-24_mckay60_gegenrechnung.log.txt`). Erwartung erfüllt. Ordnung 24 bleibt ohne Rekordkandidat.

## Rundgang Nebenklassen-Suche, Dimension 2, Zufallsstart (ab 24.11.2026, 10:08 UTC, Cloud 4 Kerne, Obergrenze 3 h = bis 13:08 UTC)

Treiber `src/run_gen.py` (Aufruf wie bei „Kontrolle Skalierung“), Logs und Wörter in `runs/rundgang/`, Blockprotokoll `runs/rundgang/blocklog.txt`.
Je Block: 4 Startwerte parallel, nach jedem Block STATUS, Commit, Push. Je Stufe wird mit dem bisher besten Typ begonnen;
bleibt die Stufe dort ungelöst, folgen die übrigen Typen (Abweichung von der Reihenfolge der Anweisung, spart Zeit; „Stufe ohne Lösung“ heißt weiter: alle Typen ungelöst).

**(18,5), je 300 s, 4 Startwerte je Typ:**
- K=16 (64 Wörter, Kontrolle): (9,9) 0 von 4 (Restzahl 20/22/21/18), (10,8) 0 von 4 (10/13/20/28), **(11,7) 4 von 4 gelöst**
  (32 s, 30 s, 125 s, 191 s). Wörter 64, selbstkomplementär, Radius per Brute-Force 5. Kontrolle bestanden.

- K=15 (60 Wörter): **0 von 12**. (11,7): Restzahl 36/37/37/38; (10,8): 48/57/52/55; (9,9): 81/90/76/79. Stopp laut Anweisung:
  kleinste Stufe mit Lösung K=16 (64 Wörter, keine Verbesserung gegenüber der Tabelle). K=14 bis K=10 nicht versucht, Rekord (K=10) nicht erreicht.
  Der bekannte untere Rand der Suche liegt hier also bei 64 (die Tabelle K(18,5) nennt 28–64).

<!-- RUNDGANG-ENDE -->

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
- `src/construct_48.py`: Paley-12-Doppelung, Bauregel 1+3 und 2+2 (Teil B), schreibt `controls/had24_*`.
- `src/hadamard_tests.py`: exakte Diskrepanz (Meet-in-the-middle, int8), Kontrollen, Test A/B, Überdeckungsradius, Nachprüfung der Gegenbeispiele für Test C. Kein Suchcode.
- `src/gegenbeispiel40.c`: Suchcode für Test C (Tabu-Suche auf Vorzeichenvektoren). Bauen: `gcc -O3 -march=native -o build/gegenbeispiel40 src/gegenbeispiel40.c`.
