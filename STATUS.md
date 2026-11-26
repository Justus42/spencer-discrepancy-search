# STATUS

Stand: 26.11.2026 nachmittags (nach Auftrag D, Leiter bei d = 10: K = 16 (64 Wörter) in keiner der fünf Zellen erreicht, kein Rekordkandidat, mit Gate; davor Auftrag M, Mischform Nebenklassen plus freie Zeilen bei (18,5): kein Code mit 48 Wörtern, die 50 von einer Sitzung gegengerechnet, mit Gate; davor Auftrag LZ, Lean-Zertifikat für K(18,5) ≤ 50 und ≤ 52: Überdeckungsradius 5 in Lean bewiesen, im Gate mit eigener Lean-Installation nachgebaut, mit Gate; davor Auftrag B2, zweite Rechnung zu den 48ern unter der 3-Sylowgruppe: alle Zahlen bestätigt, mit Gate; davor Auftrag N, Leiter in den Nachbarzellen: K(18,5) ≤ 52 aus der Suche, im Gate auf ≤ 50 verbessert (Tabelle 64), kein Rekordkandidat, mit Gate; davor Auftrag E, Evolutionsschleife für freie Suche: kein Treffer bei (20,6) mit 23 Paaren, beste Restzahl 704, mit Gate; davor Auftrag H, Zerlegung der Kern-⟨1,a⟩-Familie: kein Treffer bei K = 11, K = 10 und d = 9, mit Gate; davor Auftrag P, Gegenrechnung der Vortests der Ideensammlung: alle Zahlen bestätigt, kein Kandidat, mit Gate; davor Auftrag R9, Rang 9 bei (15,4): alle neun Klassen ausgeschlossen, mit Gate; davor Auftrag G und Auftrag S, beide ohne Kandidaten, beide mit Gate. E, H und P aus `notes/ideen_2026-11-25.md` sind damit abgeschlossen. Kein Auftrag läuft). Timebox: 23.11.2026 bis 04.01.2027, danach Entscheidung über Abbruch.
Vollständige frühere Fassung: `notes/verlauf/STATUS_2026-11-24_vollstaendig.md` (Archiv, eingefroren). Übersicht Familien x N:
`LANDKARTE.md`. Arbeitsregeln: `CLAUDE.md`. Herkunft externer Dateien: `controls/QUELLEN.md`. Laufprotokolle: `notes/laeufe/`.
Fundstellen in den Tabellen: „A: …“ = Überschrift im Archiv, „L: …“ = `notes/laeufe/…`, „K“ = `LANDKARTE.md`,
„C“ = `runs/status_umzug/offene_zellen.log.txt` (Teil C dieser Sitzung).

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

## Offene Zellen

`src/offene_zellen.py` auf der Post-Kéri-Tabelle (florath, Commit `bbed9a6`, SHA-256 geprüft, `controls/QUELLEN.md`).
B(d) = größte ganze Zahl mit 25·B < 8·d². Zelle (m, R) mit R = (m−d)/2, m ≤ min(B, 33); offen, wenn untere Schranke ≤ 2B.
Dichte = 2B · V(m,R) / 2^m. Alle anderen Zellen mit m ≤ min(B, 33) sind geschlossen (R = 0 und (33,11) fehlen in der CSV, siehe Protokoll).

| d | Wörter ≤ 2B | Zelle (m,R) | Tabelle K(m,R) | Dichte | eigener Wert | Fundstelle |
|---|---|---|---|---|---|---|
| 7 | 30 | (13,3) | 28–42 | 1,38 | | C |
| 7 | 30 | (15,4) | 23–32 | 1,78 | 32 (`ctrl_16`, bekannt) | C; A: Gesichert |
| 8 | 40 | (16,4) | 34–64 | 1,54 | 64 (Auftrag N, Suche; 60 nicht) | C; L: 2026-11-25_leiter_nachbarzellen |
| 8 | 40 | (18,5) | 28–64 | 1,93 | **50** (Gate N: Zustand mit K = 12 und Restzahl 1 plus eine freie Zeile, 25 Zeilen; Auftrag N: 52 mit K = 13; Tabelle 64; 48 nicht) | C; L: 2026-11-25_leiter_nachbarzellen; A: Rundgang Nebenklassen-Suche |
| 8 | 40 | (20,6) | 24–64 | 2,31 | **48** (besser als Tabelle) | C; A: Hadamard-Bauregel und Tests |
| 9 | 50 | (19,5) | 40–64 | 1,59 | | C |
| 9 | 50 | (21,6) | 34–64 | 1,96 | 64 (Auftrag N, Suche; 60 nicht) | C; L: 2026-11-25_leiter_nachbarzellen |
| 9 | 50 | (23,7) | 29–64 | 2,33 | 64 (Auftrag N, Suche; 60 nicht, Restzahl 548) | C; L: 2026-11-25_leiter_nachbarzellen |
| 9 | 50 | (25,8) | 25–64 | 2,69 | 64 (Auftrag N, Suche; 60 nicht) | C; L: 2026-11-25_leiter_nachbarzellen |
| 10 | 62 | (20,5) | 62–128 | 1,28 | | C |
| 10 | 62 | (22,6) | 49–128 | 1,63 | K = 16 (64) nicht erreicht, beste Restzahl 55 648 (Auftrag D, Suche) | C; L: 2026-11-26_leiter_d10 |
| 10 | 62 | (24,7) | 41–128 | 1,98 | K = 16 (64) nicht erreicht, beste Restzahl 81 536 (Auftrag D, Suche) | C; L: 2026-11-26_leiter_d10 |
| 10 | 62 | (26,8) | 36–128 | 2,34 | K = 16 (64) nicht erreicht, beste Restzahl 115 136 (Auftrag D, Suche) | C; L: 2026-11-26_leiter_d10 |
| 10 | 62 | (28,9) | 30–128 | 2,70 | K = 16 (64) nicht erreicht, beste Restzahl 125 632 (Auftrag D, Suche) | C; L: 2026-11-26_leiter_d10 |
| 10 | 62 | (30,10) | 27–128 | 3,06 | K = 16 (64) nicht erreicht, beste Restzahl 1 911 391 (Auftrag D, Suche, nur eine Runde je Lauf); Bauregel Sylvester 32 ohne 2 Spalten: 107 520 Klassen (215 040 Punkte bis auf Vorzeichen), Paley 32 ohne 2 Spalten: 270 000 Punkte bis auf Vorzeichen (Proben im Gate, nicht gegengerechnet) | C; L: 2026-11-26_leiter_d10; `notes/gate_2026-11-26_leiter_d10.md` |
| 11 | 76 | (23,6) | 73–224 | 1,32 | | C |
| 11 | 76 | (25,7) | 60–224 | 1,64 | | C |
| 11 | 76 | (27,8) | 50–224 | 1,99 | | C |
| 11 | 76 | (29,9) | 43–224 | 2,33 | | C |
| 11 | 76 | (31,10) | 37–224 | 2,69 | | C |

Eigene bessere Werte außerhalb dieser Tabelle (m > B(8) = 20, also kein Rekordweg): 48 Wörter bei (22,7) [Tabelle 21–64], (24,8) [19–64],
(26,9) [17–56], (28,10) [15–56] (A: Normalität und Folgecodes aus den 56 Wörtern; A: Gate-Prüfung … viertes Gate).

Verdopplungszellen (Ideennotiz 26.11., 3e; Tabelle bestätigt von Auftrag C): ein Code mit höchstens B(d) Wörtern in (m, R) mit m − 2R = d − 1 gibt mit einer
angehängten Spalte einen Rekord. Nach der Tabelle offen: d = 8 (17,5) 20–32, (19,6) 17–32; d = 9 (20,6), (22,7), (24,8) mit ≤ 24 Wörtern;
d = 10 (23,7), (25,8), (27,9), (29,10) mit ≤ 30; d = 11 (26,8), (28,9), (30,10) mit ≤ 38; (32,11) fehlt in der CSV. `notes/ideen_2026-11-26.md`; L: 2026-11-26_gegenrechnung_ideen

## Gesichert

| Datei in `controls/` | Größe | Diskrepanz | Quotient | Fundstelle |
|---|---|---|---|---|
| `ctrl_8.txt` | 8x8 | 5 | 1,768 (Rekord) | A: Gesichert; K |
| `ctrl_9.txt` | 9x9 | 5 | 1,667 | A: Gesichert; K |
| `ctrl_16.txt` | 16x16 | 7 | 1,750 | A: Gesichert; K |
| `ctrl_17.txt` | 17x17 | 7 | 1,698 | A: Gesichert; K |
| `L2026_17x17.txt` | 17x17 | 7 | 1,698 | A: Gesichert; K; Quelle `controls/QUELLEN.md` |
| `astra_19.txt` | 19x19 | 7 | 1,606 | A: Gesichert; K |
| `bahn15_40w.txt` (+ `_pad` 20x20) | 20x15 | 7 | 1,565 | A: Gesichert; L: 2026-11-24_bahnen5 |
| `had20_klasse{1,2,3}.txt` | 20x20 | 6 | 1,342 | A: Hadamard-Bauregel und Tests (Test B); K |
| `had24_1p3_paley12.txt`, `had24_2p2_paley12.txt` (+ `_pad` 24x24) | 24x20 | 8 | 1,633 | A: Hadamard-Bauregel und Tests; K |
| `had24_mckay_hex.txt` (60 Matrizen) | 24x24 | 6 | 1,225 | A: Gate-Prüfung … sechstes Gate; A: Gegenrechnung McKay-60; K |

- **K(20,6) ≤ 48 ohne Suche:** `src/construct_48.py`, M = [[H,H],[H,−H]] mit H = Paley-Hadamard 12, links 1 und rechts 3 Spalten
  (mit Überlappung) gestrichen; 24 Zeilen plus Komplemente = 48 Wörter, Radius 6 per Brute-Force (A: Hadamard-Bauregel und Tests).
- **Familie K(2R+8, R) ≤ 48 für R ≥ 6:** `had24_1p3_paley12` ist normal, Norm 13 in allen 20 Koordinaten (A: Gegenrechnung des Prüfvermerks,
  Teil A, Punkt 4); Verdreifachen einer normalen Koordinate gibt Länge +2, Radius +1 bei gleicher Wortzahl (A: Normalität und Folgecodes aus
  den 56 Wörtern). Aussage so in `notes/zweitmeinung_2026-11-24_pruefvermerk.md`. Folgecodes 22/24/26 per Brute-Force bestätigt (A: Warmstart und K=12).
- **Neuheitsprüfung (Regel 3 offen):** Tabellenabgleich florath siehe oben. Gemini fand nichts unter 64; EinsteinArena-Thread 273 nennt
  K(20,6) „known 24..64“, keinen Code unter 64; `Mapika/coldcase` nur q ≥ 5 (A: Neuheit K(20,6) <= 56; A: Offen und unbelegt).
  Originalkonstruktion Östergård–Kaikkonen 1998 nicht einsehbar (A: Literaturstand). Nichts nach außen gemeldet.
  Thread 273 (abgerufen 24.11.2026) enthält auch die Umformulierung als selbstkomplementärer Überdeckungscode und die 466 bei
  n = 15; der Zellenrahmen ist also nicht neu, K(20,6) ≤ 48 nach diesem Stand schon (Prüfvermerk, A4).
- **Neuheit erneut geprüft (26.11.2026, Regel 3):** kein Fund für K(20,6) ≤ 48 und K(18,5) ≤ 50. Kéris Primärtabelle (Stand 21.11.2011, erstmals gelesen) und die Post-Kéri-Tabelle
  (florath `bbed9a6`, kein PR) nennen 64 in beiden Zellen; Seite 10c unverändert 5/√8; EinsteinArena, arXiv seit 01.09., Quantavane, `openai/math` ohne Treffer. Nachtrag 11:30 Uhr (Justus am PC): keine offenen Issues bei florath, Kéris PDF zeigt t 27–64 und d 24–64, Thread 273 mit allen 21 Antworten ohne Fund. Nicht gelesen:
  Bezahlschranken-Texte. `notes/neuheit_2026-11-26_pruefvermerk.md`
- **K(20,6) ≤ 48 in Lean bewiesen (24.11.2026):** Lean 4.30.0-rc2, nur Kernbibliothek, `lean/`. Satz: jedes x < 2^20 liegt im Abstand ≤ 6
  von einer Zeile von M oder −M (`had24_1p3_paley12` und `had24_2p2_paley12`); Radius 5 abgelehnt; Prüfschleife als korrekt bewiesen, ohne `sorry`.
  Zwei Wege: `native_decide` (je 2 Min.) und `decide +kernel` (10,4 bzw. 5,8 Min., Achsen nur `propext`, `Quot.sound`). Kontrollen Hamming [7,4], `ctrl_16`,
  Negativkontrolle (48er ohne ein Paar) wie erwartet. Grenzen: Lean liest die Kontrolldatei nicht selbst (Erzeuger + Prüfsumme), Reduktion auf die
  Diskrepanz nicht formalisiert, Neuheit offen. L: 2026-11-24_lean
- **K(18,5) ≤ 52, d = 8 (Auftrag N, 26.11.2026):** Löser aus Auftrag H, Kern ⟨1,a⟩, Typ (9,9), K = 13 (26 Zeilen, 52 Wörter), 4 von 4 Startwerten in ≤ 1,6 s; K = 14 und 15 ebenso. Matrix 26 x 18
  (`runs/leiter_n/c18-5_k13_t9-9.matrix.txt`): `zerl_offen.py` und direkte Zählung 0 offen, check_disc, verify_spencer, exakt_disc je Diskrepanz 8. Tabelle K(18,5) 28–64, eigener Wert bisher 64; keine Rekordgrenze (40).
  K = 12 (48 Wörter): 12 Startwerte, immer Restzahl 1. Neuheit prüft der Orchestrator. K = 16 (64 Wörter) auch in (21,6), (23,7), (25,8): 32 x 21 / 23 / 25, je Diskrepanz 9 mit drei Programmen. L: 2026-11-25_leiter_nachbarzellen
- **K(18,5) ≤ 50, d = 8 (Gate N, 26.11.2026, Rechnung des Orchestrators; in Lean bewiesen und damit von einer Sitzung gegengerechnet, Auftrag LZ, siehe letzte Zeile des Eintrags):** Jeder der 12 Zustände mit K = 12 und Restzahl 1 lässt genau die vier Punkte (±y, ±z) offen.
  Das Wort (y, z') mit z' = z an vier Stellen gewechselt hat Abstand 4 zu (y, z) und 5 zu (y, −z); sein Negativ erledigt die beiden anderen. 24 Zeilen plus diese eine: 25 Zeilen, 50 Wörter (kein Kern-⟨1,a⟩-Code mehr).
  Matrix 25 x 18 (`notes/orchestrator/2026-11-26_gate_leiter_n/gate_c18-5_50w.matrix.txt`): Radius 5 über alle 2^18 Punkte mit eigenem Zähler, check_disc, verify_spencer, exakt_disc je Diskrepanz 8; in allen 12 Zuständen gelingt es.
  Neuheit: Post-Kéri-Tabelle (florath, `main` am 26.11.2026, SHA-256 unverändert `5efaf8d9…`) nennt 28–64, obere Schranke aus der trivialen Regel. Eine Websuche ohne Fund. Gemini Deep Research (26.11.2026): kein Fund unter 64; vom Orchestrator nachgeprüft: alle Zweige bei florath nennen 28–64, keine PR-Referenzen sichtbar (`notes/gemini/neuheit_18_5_2026-11-26_kurz.md`). Am 26.11.2026 um 11:30 Uhr nachgeholt und ohne Fund: Issues bei florath (keine offenen), EinsteinArena-Thread im Volltext, Kéris PDF (t 27–64); weiter offen: Bezahlschranken-Texte (`notes/neuheit_2026-11-26_pruefvermerk.md`, Nachtrag). Keine Rekordgrenze (40). `notes/gate_2026-11-26_leiter_n.md`
  **Lean (Auftrag LZ, 26.11.2026, Gate bestanden, `notes/gate_2026-11-26_lean_18_5.md`):** 25 x 18-Matrix: jedes x < 2^18 im Abstand ≤ 5 von einer Zeile von M oder −M, Radius 4 abgelehnt; ebenso der 52er (26 x 18) und die Negativkontrolle (50er ohne freie Zeile: 48 Wörter, Radius 5 abgelehnt, 4 offene Punkte). native_decide 31 s, decide +kernel 132 s (nur `propext`, `Quot.sound`). Grenzen wie bei (20,6): Erzeuger + Prüfsumme statt Dateilesen, Überdeckung → Diskrepanz nicht formalisiert, Neuheit offen. L: 2026-11-26_lean_18_5; `lean/README.md`
- **Hadamard 24 ohne Spalten erreicht disc 8 (Ideenrunde 26.11., Gegenrechnung Auftrag C):** ohne 2 Spalten in 7420 von 33 120 Fällen, ohne 4 Spalten in 42 304 von
  1 275 120 (je Ausrichtung 21 152: 12 der 60 Klassen, acht mit 2640, je eine mit 16, 6, 6, 4). Alle diese 48er sind starr (Alleinanteil je Zeile 1264 bis 1354,
  kein 46er darin). Belegart: voll, zwei Programme (Orchestrator, Sitzung). `notes/ideen_2026-11-26.md` (3a, 3f); L: 2026-11-26_gegenrechnung_ideen

## Negativbefunde

Belegart: **voll** = vollständig durchgerechnet, **Gegenbeispiel** = je Fall ein nachgerechneter Vorzeichenvektor, **Suche** = Suche erfolglos (schließt nichts aus).

| Familie | Ergebnis | Belegart | Fundstelle |
|---|---|---|---|
| Hadamard 20 voll (3 Klassen, mit Transponierten) | disc 6 | voll | A: Hadamard-Bauregel und Tests (Test B); K |
| Hadamard 24 voll (60 McKay-Klassen, mit Transponierten) | disc 6 | voll | A: Gate-Prüfung … sechstes Gate; A: Gegenrechnung McKay-60 |
| Hadamard 20 und 24 ohne 1 Spalte (alle Spalten, Klassen, Transponierten) | disc 7 | voll | A: Teil B, C, D (Teil D); K |
| Hadamard 20 ohne 2 Spalten (1140 Fälle) | alle exakt 6 | voll | A: Gegenrechnung des Prüfvermerks, Teil A |
| Hadamard 24 ohne 3 Spalten (242 880 Fälle) | alle exakt 7 | voll | A: Gegenrechnung des Prüfvermerks, Teil A |
| Doppelung Ordnung 40, H aus 3 Klassen, 1+3 und 2+2, gleiche Zeilenausrichtung (30 780 Auswahlen) | disc ≤ 8, nötig 12 | Gegenbeispiel (zweimal nachgerechnet) | A: Hadamard-Bauregel und Tests (Test C); L: 2026-11-24_status_umzug |
| Paley-Doppelung 24 x 20 plus eine Spalte | disc bleibt ≤ 8, nötig 9 | Gegenbeispiel je Fall | L: 2026-11-24_spaltenanbau |
| Paley-Konferenzmatrizen Ordnung 6, 10, 14, 18 | disc 3, 3, 5, 5 | voll | A: Gegenrechnung des Prüfvermerks, Teil A |
| Zirkulante ±1-Matrizen n = 18–31 (10 Ordnungen), mit Rand n = 18–26 | beste disc 4–7, 0 Überlebende | Gegenbeispiel je Klasse, Bester exakt | L: 2026-11-24_zirkulant; A: Teil B, C, D |
| Gruppenmatrizen A[g][h] = f(g^-1 h), 10 nichtzyklische Gruppen der Ordnung 18, 20, 21, 25, alle Klassen; mit Rand bei 18 und 21; zwei Spalten gestrichen | 0 Überlebende; beste disc 4 (18, mit Rand 4), 6 (20), 5 (Z7:Z3; mit Rand 7), 7 (Z5xZ5); ohne 2 Spalten 6 (20), 7 (25) | Gegenbeispiel je Klasse, Bester exakt (zwei unabhängige Zählungen der Klassen; Kontrollen Z7 mit Rand, Z18/20/21/25 bestanden) | L: 2026-11-25_gruppen; K |
| Codes im affinen Spann bekannter Codes, (15,4), 15 Paare (30 Wörter): L(`ctrl_16`), L(loesung111), Rang-9-Erweiterungen (254 = 9 Klassen unter Aut(L)) | unmöglich in Rang 8 und in allen 9 Rang-9-Klassen (254 Erweiterungen; die letzten 6 Klassen in Auftrag R9, 25.11.2026, 317 Mio. Knoten, 2 h 16 Min. auf 4 Kernen); Rang 10 nicht begonnen | voll (L(loesung111): zwei Programme, Sitzung mit und ohne Symmetrie, Gate ohne Symmetrie; Rang 9: ein Programm, nur mit Symmetrie) | L: 2026-11-25_spann, 2026-11-25_rang9; K |
| Codes im affinen Spann der 48er, (20,6), 23 Paare (46 Wörter): L(1p3), L(2p2), L(neunter); unter den 19 008 P-Lösungen 81 verschiedene Spanne | Kontrolle 24 Paare 12/12, 23 Paare 0 von 28 (beste Restzahl 886) | Suche | L: 2026-11-25_spann; K |
| Kern ⟨1,a,b⟩ (8 Wörter) bei (20,6): Zählschranke 2^17/B(D) | 97 Typen, B(D) 31 180 bis 53 372, Schranke 2,46 bis 4,20 < 5: schließt keinen Typ aus | voll (eigenes Skript `src/kern_nebenklassen.py`) | L: 2026-11-25_gruppen, Teil 4 |
| Bahnen aus 5er-Zyklen plus Komplement, n=15, R=4 | mindestens 40 Wörter (525 Vierermengen) | voll | L: 2026-11-24_bahnen5 |
| Bahnen aus 5er-Zyklen plus Komplement, n=20, R=6 | 70 Wörter gefunden, 60 nicht | Suche | L: 2026-11-24_bahnen5 |
| Bahnen unter der 3-Sylowgruppe P (Ordnung 9) des 48ers, n=20, R=6 | Minimum 48 Wörter (≤ 46 ausgeschlossen); nur Größe 1/3: 19 008 Lösungen in 8 Klassen | voll, zwei Programme mit verschiedenen Verfahren (B2); im Gate: beide Listen verglichen, alle 19 008 Abbildungen und die Trennung der neun Vertreter mit eigenen Skripten bestätigt; die Zählung selbst nicht ein drittes Mal gerechnet | L: 2026-11-25_bahnen3; L: 2026-11-25_bahnen3_zweit; `notes/gate_2026-11-26_bahnen3_zweit.md` |
| Bahnen unter einem Element der Ordnung 3 des 48ers, n=20, R=6 | Tabu-Kontrolle 48 Wörter nicht bestanden (1/4, 0/4), < 48 ungeprüft | Suche | L: 2026-11-25_bahnen3 |
| Zeilen mit genau einer Null, n = 11, 11 Zeilen (Einträge {-1,0,1}) | disc 6 unmöglich (Rekord 1,809 ausgeschlossen); nur {-1,0,1}, nicht reell | voll | L: 2026-11-25_einzelnull |
| Zeilen mit genau einer Null, n = 11: Umkodierung auf zwei Radius-2-Kugeln im 10-Würfel nachgerechnet (5632 Zeilen, 0 Abw.) | Ausschluss auch über K(10,2) ≥ 24 | voll (Tabelle) | L: 2026-11-25_einzelnull |
| Träger 10/8/6 gemischt, n = 11, Profile (q,s) mit Budget ≤ 48: (5,0) (4,0) (3,1) (2,1) (1,2) (0,2) | keine Lösung | voll | L: 2026-11-25_einzelnull |
| Träger 10/8/6 gemischt, Profile (3,0) (2,0) (1,1) (1,0) (0,1) | nichts gefunden, (3,0) Typen 0–3 voll, Rest offen | Suche | L: 2026-11-25_einzelnull |
| Selbstkomplementäre Codes (13,3) (= n = 14 mit einer Null je Zeile, P1 bestätigt) | 42 Wörter gefunden, 40 nicht (4 x 10 Min.) | Suche | L: 2026-11-25_einzelnull |
| Zwei-Bahnen-Codes (⟨g⟩ auf zwei Blöcken der Länge q + f Fixspalten), Kontrollen (15,4) 112 und (17,5) 1792 bestätigt; (20,6) q=10/9 alle Teilfälle des Vortests plus 1 volle + kurze ≤ 11 | nichts mit ≤ 40 Wörtern | voll (zwei Programme: Vortest des Orchestrators und Sitzung; „1 volle + kurze ≤ 11“ ein Programm) | L: 2026-11-25_zweibahn; `notes/gate_2026-11-25_zweibahn.md` |
| Zwei-Bahnen-Codes, 2 volle Bahnen + feste Paare: (25,8) q=11/12, (23,7) bis 50 Wörter; (30,10), (28,9), (26,8) q=13/11 bis 62 Wörter | keine Überdeckung; bei q = 11 und 13 auch mit beliebig vielen festen Paaren und mit nur einer vollen Bahn nicht | voll (q = 11: zwei Programme; q = 13 und q = 12: ein Programm, im Gate nur Stichprobe) | L: 2026-11-25_zweibahn; K; `notes/gate_2026-11-25_zweibahn.md` |
| Rundlauf Ansatz 3 (16 Familien, 388 503 Matrizen) | 0 Überlebende, bester Quotient 1,400 | Gegenbeispiel | L: 2026-11-24_rundlauf; A: Ansatz 3 |
| Reelle Einträge um `ctrl_16` (Zeilen im L1-Abstand ≤ 2) | nichts über 7 | voll | L: 2026-11-24_reell16 |
| Reelle Matrizen n = 8, 9, 12 (Dichte < 1) | nichts über dem Rekord | voll, bedingt auf Kleitman (belegt: J. Combin. Theory 1 (1966), 209–214) | L: 2026-11-24_reell16; K |
| Nebenklassen-Suche d=2, (18,5) | nur 64 Wörter (K=15: 0 von 12) | Suche | A: Rundgang Nebenklassen-Suche |
| Nebenklassen-Suche d=2, (21,6) | Kontrolle 64 Wörter nicht erreicht (0 von 12) | Suche | A: Rundgang Nebenklassen-Suche |
| Nebenklassen-Suche d=2, (23,7) | 60 Wörter nicht erreicht (0 von 8) | Suche | A: Rundgang Nebenklassen-Suche |
| Freie Suche (15,4) mit 30 Wörtern | bleibt bei 466 offenen Punkten | Suche | A: Ziele; A: Fehlversuche |
| (15,4) mit 30 Wörtern: Symmetrie der Ordnung 7 (Typ 7+7+1), 11 (Typ 11+1⁴), 13 (Typ 13+1+1) | keine; Ordnung 7 mit 32 Wörtern: 112 Lösungen (2 Klassen); `ctrl_16` ohne eine Zeile: immer 466 offen | voll (zwei unabhängige Programme, direkt nachgerechnet) | L: 2026-11-25_gegenrechnung15 |
| (15,4), 30 Wörter: 13 Zeilen eines 32ers (`ctrl_16` und `loesung111`) plus 2 beliebige Zeilen in [−1,1]^15 (3 gestrichen, 2 ersetzt: je 560 Tripel); Zusatz 4 gestrichen, 3 ersetzt (je 1820 Fälle) | unmöglich in allen 2 × 560 Tripeln (Lochzahlen 967/987) und allen 2 × 1820 Vierermengen; je Fall ein Zertifikat aus 6–7 (3/2) bzw. 12–21 (4/3) Lochklassen, die sich nicht auf die Ersatzzeilen verteilen lassen; gilt auch für reelle und ternäre Zeilen | voll (je Fall Zertifikat; zwei Prüfer mit verschiedenen Verfahren, Sitzung und Gate, je alle 4760 Zertifikate; Negativkontrollen bestanden; die Paarbedingung selbst ist eine Herleitung) | L: 2026-11-25_pruef_ideen, Teil 1 |
| `ctrl_16` und `loesung111` ohne eine Zeile, eine ganze Spalte neu gewählt (240 Fälle je Klasse, alle 2^15 Ergänzungen) | Minimum 233 offene Klassen in allen 480 Fällen, echte Spalte optimal (4 Optima); Kontrolle mit 16 Zeilen: Minimum 0 | voll (zwei Programme, Sitzung und Gate, je alle 480 Fälle) | L: 2026-11-25_pruef_ideen, Teil 2 |
| Kozyklische Matrizen M[g][h] = f(g⁻¹h), f ungerade auf E (Ordnung 2n, zentrale Involution z), alle f mit f(e) = +1: n = 18 (Z36, Dic9, Z3×Dic3), n = 20 (Z40, Z5×Q8, Z5×D4, Z5×Z4×Z2, Dic10, D20) | 0 Überlebende bei Ziel 8; **exakter Bestwert disc 6** (1,414 bei n = 18, 1,342 bei n = 20); disc ≥ 6: n = 18 4338 / 23 922 / 4752, n = 20 28 560 / 3240 (480 Hadamard) / 0 / 900 / 55 400 (5600 Hadamard) / 0 | Gegenbeispiel je Funktion (gespeichert, getrennter Prüfer; im Gate alle 3 538 944 mit eigenen Gruppentafeln nachgerechnet), Bestwert exakt (`exakt_disc`; beste Matrizen mit allen drei Prüfprogrammen); die Anzahlen mit disc ≥ 6 stammen aus einem Programm, nur Z40 und Z5×Q8 stimmen mit dem Vortest überein | L: 2026-11-25_pruef_ideen, Teil 3; K |
| Kozyklische Matrizen n = 26 (Z52, Dic13), 28 (Z56, Z7×Q8, Z7×D4, Z7×Z4×Z2, Dic14, D28), 30 (Z60, Dic15 vollständig) | 0 Überlebende bei Ziel 10 (disc ≤ 8, Quotient ≤ 1,569 / 1,512 / 1,461); kein exakter Bestwert | Gegenbeispiel je Funktion gefunden, nicht gespeichert (Sitzung und Vortest des Orchestrators, zwei Programme, beide ohne gespeicherte Gegenbeispiele; Dic15 im Vortest nur zu 80 %; Sieb an n = 20 gegen exakte Zahlen kontrolliert) | L: 2026-11-25_pruef_ideen, Teil 3 |
| Zerlegung, d = 9, Kern ⟨1,a⟩, K = 12: U = Paley-12 voll (m2 = 9, 11, 13, jedes W) und U = Paley-12 ohne Spalte mit m2 = 10 | ausgeschlossen (Spielraum −872, −1760, 88 gegen mindestens 3960 Doppelzählungen, erlaubt höchstens 320; 4920 gegen 5120); m2 = 12, 14 (Zellen (23,7), (25,8)) offen | Herleitung (Zählbeweis; von Orchestrator, Sol und P nachvollzogen, nicht formal geprüft); Zahlen von zwei Programmen; bei m2 = 10 dazu Aufzählung aller 17 031 Ketten | L: 2026-11-25_pruef_ideen, Teil 4 |
| Kern ⟨1,a⟩ bei (20,6), Typ (11,9), K = 10 oder 11, U aus Zeilen von Paley-12 ohne Spalte (linke Hälfte des 48ers) | unmöglich: die gestrichene Zeile ist ein flaches y (alle \|u_i\| = 1, bestätigt), W müsste allein Q9 mit Radius 1 überdecken, 2K·10 ≥ 512 gibt K ≥ 26 | voll (Lemma aufgezählt, Zählung einzeilig; nur dieser Typ und dieses U) | L: 2026-11-25_pruef_ideen, Teil 4 |
| Reelle 10 × 10-Matrizen (Astra) | ausgeschlossen: jede Kappe hat Durchmesser ≤ 4 und liegt in einer Radius-2-Kugel oder hat ≤ 36 Punkte (Astra: ≤ 43); hat mindestens eine Kappe mehr als die Radius-2-Kugel, ist die Vereinigung aller zehn ≤ 505 < 512; sonst überdecken 10 Radius-2-Kugeln die 512 Klassen nicht (Aufzählung; Tabelle K(10,2) ≥ 24 nicht nötig) | Herleitung (Astra; von Orchestrator und P nachvollzogen, nicht formal geprüft) plus Aufzählung; Fall ohne Sonderkappe: drei Programme, zwei verschiedene Suchbäume (Sitzung 63 505 Knoten; Gate ohne Symmetriebruch 3 569 441 Knoten) | L: 2026-11-25_pruef_ideen, Teil 5; K |
| Kern-⟨1,a⟩-Paare (U,W) bei (20,6), abwechselnde Suche mit Halbproblem-Löser (Auftrag H): K = 11 (44 Wörter) und K = 10 (40) in den Typen (11,9), (13,7), (10,10), (12,8), Zufallsstart; Kontrolle K = 12 (11,9) 8 von 8 in ≤ 25 s | nicht erreicht; beste Restzahl K = 11: 1288 / 2698 / 1063 / 1836, K = 10: 3234 / 5926 / 1852 / 4280 | Suche | L: 2026-11-25_zerlegung; K |
| Kern-⟨1,a⟩-Paare bei d = 9, K = 12 (48 Wörter), Zellen (21,6), (23,7), (25,8), sechs Typen, Zufallsstart; dazu U = Paley-12 ohne Spalte, W 12 x 10/12/14 | nicht erreicht (beste Restzahl 1760 bei (12,11)); die besten Zustände bei (12,11) und (12,13) haben als eine Hälfte eine volle Hadamard-Matrix 12 x 12 und liegen damit in dem Fall, den der Zählbeweis (Zeile zu Auftrag P oben) für jede zweite Hälfte ausschließt; W 12 x 10 ausgeschlossen (Zählschranke über alle 880 Anforderungen, bestätigt Sol), 12 x 12 und 12 x 14 unentschieden | Suche (alle Restzahlen im Gate mit eigenem Zähler bestätigt); 12 x 10: Zählschranke, drittes Programm nach Vortest und Auftrag P | L: 2026-11-25_zerlegung |
| Freie Suche als Evolutionsschleife (Auftrag E: 25 Programme, Inseln A, B, C, Saat; eingefrorener Bewerter) bei (20,6) mit 23 Paaren (46 Wörter), blind und ab dem 48er; Kontrollen (15,4) 16 Paare blind 4 von 4; (20,6) 24 Paare blind 1 von 4 nur in der Doppelungsform (Insel B, Teilung 11 + 9 vorgegeben; disc 8 dreifach), freie Züge (Insel A) in der Sitzung bei 24 Paaren nicht unter 115; im Gate nachgeholt: das Zielprogramm p018 (freie Züge, volle Nachbarschaft) findet bei 24 Paaren blind einen 48er in 2 von 2 Läufen (8 s und 51 s, disc 8 dreifach) | nicht erreicht; beste Restzahl **704** offene Klassen (fester Endpunkt: 19 von 44 Läufen genau 704; 48er ohne Zeile 1297, Spann 886); (15,4) mit 15 Paaren 233 | Suche (alle 116 Restzahlen des Archivs im Gate mit eigenem Zähler bestätigt) | L: 2026-11-25_evo; `notes/gate_2026-11-25_evo.md`; K |
| Leiter K = 16, 15, … mit dem Löser aus Auftrag H, je Zelle vier Typen (Auftrag N): (16,4) K = 16 (alle Typen), K = 15 nicht (beste Restzahl 28); (18,5) K = 13 (Typ (9,9)), K = 12 Restzahl 1, übrige Typen K = 16; (19,5) K = 16 nicht (768); (21,6) K = 16 nur Typ (12,9) (1 von 8 bei 900 s), K = 15 3343; (23,7) K = 16 nur (12,11), K = 15 fest 548; (25,8) K = 16 nur (16,9), K = 15 ≈ 10 600; (20,6) K = 11 in (14,6), (15,5), (16,4): 4152 / 5996 / 9415 | bester Typ je Zelle in Spalte „eigener Wert“ oben; sonst nicht erreicht | Suche (K = 13 bei (18,5) exakt, drei Prüfprogramme; alle 228 Restzahlen mit `zerl_offen.py` nachgezählt, 0 Abweichungen; im Gate alle 240 gespeicherten Paare mit eigenem Zähler bestätigt) | L: 2026-11-25_leiter_nachbarzellen; `notes/gate_2026-11-26_leiter_n.md`; K |
| Leiter K = 16 bei d = 10 mit dem Löser aus Auftrag H (Auftrag D, Kern ⟨1,a⟩, 64 Wörter = Tabellenwert, Quotient genau 5/√8): (26,8), (28,9), (30,10), (24,7), (22,6), 17 Typen, je 4 Startwerte 600 s ((28,9), (30,10): 1200 s), dazu 8 Startwerte (26,8) 1200 s und (28,9) 1800 s im besten Typ; Kontrollen K3 8 von 8, (25,8) d = 9 2 von 8 | 0 Treffer in 84 Läufen (68 + 16; die 16 Kontrollläufe nicht gezählt); beste Restzahl (26,8) 115 136, (28,9) 125 632, (30,10) 1 911 391 (nur 1 Runde je Lauf), (24,7) 81 536, (22,6) 55 648; feste Plateaus, K = 15 nicht begonnen; (30,10), (24,7), (22,6) in Schritt 3 nicht gerechnet | Suche (alle 100 Restzahlen einschließlich Kontrollen mit `zerl_offen.py` nachgezählt, 0 Abweichungen) | L: 2026-11-26_leiter_d10; K |
| Mischform K Nebenklassen plus f freie Zeilen bei (18,5), d = 8 (Auftrag M): 50 gegengerechnet (je Zustand K = 12 genau 1008 passende Zeilen; Matrix des Gates dreifach disc 8); f ≤ 2 exakt für 160 Zustände K = 11 aus zerl_halb (vier Typen, 9 Endstände 3312–6128 offene Punkte) und 16 Endstände der gemeinsamen Suche; f = 1 exakt für 24 Zustände (16,4) und 12 Zustände (23,7) mit K = 15; gemeinsame Suche `misch_alt` (Positivkontrolle (12,1) 8 von 8) bei 48 Wörtern: (11,2), (10,4), (9,6) in (9,9) fest 4 offene Punkte, in (10,8) fest 816 | alle f-Fragen „nein“ (voll, je Zustand); 48 Wörter nicht erreicht. Gate: alle 44 Endmatrizen der Ziele bestehen aus 12 Paaren (u,w), (u,−w), keine einzige ungepaarte freie Zeile; die drei Formen sind also dieselbe Form K = 12, f = 0 (Typ (9,9): Restzahl 1 wie in N; Typ (10,8): 816 offene Punkte, in N nicht gerechnet) | voll (je Zustand) / Suche | L: 2026-11-26_mischform_18_5; K; `notes/gate_2026-11-26_mischform.md` |
| Hadamard ohne Spalten, Abschluss (Ideenrunde 26.11.): Ordnung 20 ohne 3 / 4 Spalten (6840 / 29 070 Fälle); 24 ohne 5 Spalten (5 100 480); Ordnung 8, 12, 16 (Sylvester, Klasse von `ctrl_16` mit Transponierter) c ≤ 4; Paley II 28 voll; Sylvester 32 ohne 1 / 2 Spalten; Paley-Konferenz 26 und 30; zirkulant n = 15 (alle ersten Zeilen) | 20: disc 5 und 6; 24 ohne 5: ≤ 7 in allen Fällen; bestes d je N: 5 (8), 5 (12), 7 (16), 7 (20), 8 (24), 6 (28), 9 (32); Konferenz 5 und 7; zirkulant 15: höchstens 5 | voll, zwei Programme (Orchestrator, Sitzung) | `notes/ideen_2026-11-26.md` 3a, 3d, 3f; L: 2026-11-26_gegenrechnung_ideen; K |
| Hadamard 24 ohne 1 / 3 / 5 Spalten plus eine freie ±1-Zeile, d = 9 | lösbar in 0 von 2880 / 0 von 242 880 / 0 von 5 100 480 Fällen | voll; c = 1 zwei Programme (Orchestrator, Sitzung); c = 3, 5 voll nur Sitzung (Orchestrator Stichprobe) | L: 2026-11-26_gegenrechnung_ideen |
| Hadamard 20 mit einer (120 Fälle) bzw. zwei (1140 Spaltenpaare) beliebig ersetzten Spalten, auch reelle Einträge in diesen Spalten | disc 8 nie (eine Spalte: 3021 Bedingungen je Fall; zwei: 1272 flache x'' je Paar, keine Belegung) | voll, zwei Programme (Orchestrator, Sitzung); reell über Rundung (Sol, nachvollzogen) | `notes/ideen_2026-11-26.md` 3f; L: 2026-11-26_gegenrechnung_ideen; K |
| Sylvester 32 ohne eine Spalte, alle 32 Zeilen bleiben, plus höchstens 7 freie ±1-Zeilen, d = 11 | unmöglich (mindestens 8 nötig, 80 Wörter gegen 76); gilt nicht für reelle Zeilen und nicht mit gestrichenen Zeilen | Herleitung plus Aufzählung aller 2^26 Nebenklassen; Zahlen zwei Programme (Orchestrator, Sitzung), Herleitung von Sol und Gemini nachvollzogen, nicht formal | `notes/ideen_2026-11-26.md` 3c; L: 2026-11-26_gegenrechnung_ideen |
| Verdopplungszellen (19,6) und (17,5) mit 20 Wörtern (p018, je 20 bis 60 s); amalgamierte direkte Summen: kein Negativbefund (erste Fassung zurückgenommen) | 26 192 bzw. 8952 offene Klassen | Suche, ein Programm (Orchestrator), nicht gegengerechnet | `notes/ideen_2026-11-26.md` 3e |

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
- 24.11.2026, Rundgang (21,6), d=2, K=16 (64 Wörter, Kontrolle), Zufallsstart, je 900 s, Startwerte 1–4: (11,10) Restzahl 1416–1739, (12,9) 1402–1464,
  (13,8) 480–960; alle 12 Läufe ungelöst (3 x 15 Min.).
- 24.11.2026, Rundgang (23,7), d=2, K=15 (60 Wörter), Zufallsstart, je 1800 s, Startwerte 1–4: (12,11) Restzahl 640–680, (13,10) 4777–5174; alle 8 Läufe ungelöst (2 x 30 Min.).

- 23.11.2026, Lauf n=20 R=6 d=2 K=13 Typ (11,9) Startwerte 1–16, je 1800 s: durch Container-Neustart verloren, keine Restzahlen
  (Details im Abschnitt „Lauf K=13“).
- 24.11.2026, Kontrolle n=22 R=7 d=2 K=13, Zufallsstart, je 1800 s, Cloud 4 Kerne: Typ (13,9) Startwerte 1–8 alle ungelöst
  (Restzahlen 7, 2, 6, 7, 2, 2, 3, 1); Typ (12,10) Startwerte 3 und 7 ungelöst (Restzahl 8, 8). Details im Abschnitt „Kontrolle Skalierung“.
- 24.11.2026, zirkulante Matrizen n = 18–31 (Teil C): alle Klassen, 0 Überlebende, beste Diskrepanz 4–7 (Schwellen 8–11), ca. 40 Min.
- 24.11.2026, Bahnen unter 5er-Zyklen plus Komplement, n=20 R=6, nur volle Bahnen, Tabu `src/bahnen_tabu.c`: K=6 (60 Wörter)
  ungelöst: Zufallsstart Startwerte 1–4 je 600 s (Restzahl 122/96/104/94 Punkte) und Warmstart aus den vier K=7-Lösungen je 600 s
  (94/116/116/118). Stopp; K=5, K=4 nicht versucht, Variante 3 volle Bahnen + 5 feste Paare ungeprüft. Protokoll `notes/laeufe/2026-11-24_bahnen5.md`.
- 25.11.2026, Bahnen ⟨g⟩ (Ordnung 3) des 48ers, Tabu `src/bahnen3_tabu.c`, T=24 (48 Wörter), Startwerte 1–4 je 900 s: Kontrolle nicht bestanden
  (Stichprobe 1000: 1 von 4; exakt: 0 von 4, Restzahl 326–493), keine Leiter. Python-Gruppensuche zu langsam (abgebrochen, C statt dessen). L: 2026-11-25_bahnen3.
- 25.11.2026, n = 11 gemischt (Träger 10/8/6), `src/einzelnull_such.c -y -3`: Block 3 (20 Min.) am Nummernraum gescheitert (> 2^20 Aufgaben je Bahn), Block 4 (25 Min.) Typ 1 nicht fertig; abgebrochen. L: 2026-11-25_einzelnull.
- 25.11.2026, (13,3) selbstkomplementär, `src/sc13_tabu.c`, 40 Wörter, Startwerte 1–4 je 600 s: ungelöst (Restzahl 160–196, bester 16). L: 2026-11-25_einzelnull.
- 25.11.2026, Auftrag Z, (30,10) mit Kugel um h1 je Klasse: 4394 Klassen statt geschätzter Dutzende, nach 10 Min. abgebrochen; ersetzt durch Vorfilter über alle vollen Bahnen (L: 2026-11-25_zweibahn).
- 25.11.2026, Auftrag G, Gruppenmatrizen über nichtzyklischen Gruppen (10 Gruppen, Ordnung 18/20/21/25, Rand, 2 Spalten gestrichen): 0 Überlebende, kein Kandidat; reine Rechenzeit ca. 40 s, mit Kontrollen und exakten Gegenrechnungen ca. 5 Min. L: 2026-11-25_gruppen.
- 25.11.2026, Auftrag S: `pip install ortools python-sat igraph` vom Berechtigungsfilter abgelehnt (eigene C-Suche statt SAT/bliss); Gleichwertigkeitssuche der 48er-Spanne (`spann_aut`, m=20) nach 10 Min. abgebrochen. L: 2026-11-25_spann.
- 25.11.2026, Auftrag S, (15,4) 15 Paare, Rang-9-Vertreter ctrl16 y=40/7, loesung111 y=384/128/385/129: exakt je 180 s unentschieden, lokale Suche je 600 s Restzahl 233 (in Auftrag R9 später alle exakt entschieden). L: 2026-11-25_spann.
- 25.11.2026, Auftrag R9: Kontrolle c) erste Hälfte (ctrl16_r9_y3, 16 Paare, Aufgaben 0–5160) am Zeitlimit 1200 s, nur Kontrolle, kein Einfluss; zweite Hälfte LÖSBAR wie verlangt. L: 2026-11-25_rang9.
- 25.11.2026, Auftrag S, (20,6) 23 Paare im Spann der 48er: Zufallsstart 8 + Warmstart 8 (L(1p3)), je 4 in L(2p2) und L(neunter), je 900 s, 0 von 28, Restzahl 886–989. L: 2026-11-25_spann.
- 24.11.2026, Kleitman-Probe (`src/kleitman_probe.py`, exakte Clique-Suche im Würfel): n=6, D=5 nach 600 s (Zeitgrenze) nicht fertig, nicht
  ausgewertet; Fall D = n−1 aus dem Skript genommen. n = 3–6 mit D ≤ n−2 (13 Fälle) lieferten die Formel. Kein Einfluss auf Teil B.
- 24.11.2026, Hadamard-Tests (Abschnitt „Hadamard-Bauregel und Tests“):
  - Download der Sloane-Bibliothek (Ordnung 24, 60 Klassen) nicht möglich: `neilsloane.com` nicht auflösbar, `https://` über den Proxy 403.
    Test A läuft deshalb nur auf konstruierbaren Klassen (53 von 60).
  - Erster Klassen-Fingerabdruck (Histogramm der 4-Zeilen-Summen) trennt bei Ordnung 20 nichts: 72 Matrizen, 1 Schlüssel statt 3 (Laufzeit 1 s).
    Ersetzt durch Typen aller 6-Zeilen-Mengen (trennt bei Ordnung 20 genau 3 Klassen).
  - Installation von `pynauty` für kanonische Graphformen wurde von der Sitzung nicht freigegeben und nicht umgangen; eigene Invarianten statt dessen.
- 24.11.2026, Rundlauf Ansatz 3 (Abschnitt „Ansatz 3“): 16 Familien aus den Modellantworten, 388 503 Matrizen, 0 Überlebende (kein Rekordkandidat), ca. 20 Min. Rechenzeit.
  - Kreuzprobe `check_disc.py` / `verify_spencer.py` auf einer 31 x 30-Matrix (Astra 8, exakt 6): beide Skripte nach je 200 s Zeitgrenze abgebrochen, nicht gerechnet (443 s verloren).
  - Sol 7, tieferes Ziel 4 (`sieb.py --ziel 4 --zeit 0.05 --max-sek 120`): an der Zeitgrenze bei 9 137 von 17 920 Matrizen abgebrochen, alle bis dahin ohne Treffer; Datei verworfen, Familienwert deshalb mit Stichprobe exakt belegt.
  - Sieb mit 0,05 s je Matrix bleibt bei Sol 4 in 7 von 2 174 Fällen über dem exakten Wert (7 statt 5): zu kurze Zeit, nur Suchcode; die exakte Rechnung entscheidet.
- 24.11.2026, Status-Umzug: Commit-Daten über github.com (`.patch`) Proxy 403, statt dessen Git-Abruf; erster Lauf `offene_zellen.py` am CSV-Kopf gescheitert (Spalte `q`), je Sekunden.
- 24.11.2026, Lean-Zertifikat: Kernel-Prüfung zweimal Speichermangel (einfache Schleife am Stück 117 s; 8 x 512 Blocksätze über `lake build` ca. 10 Min.), Namenskonflikt beim Neubau 2 Min.; `lakecache.blob.core.windows.net` Proxy 403 (L: 2026-11-24_lean).
- 25.11.2026, Leiter unter 48 bei (20,6), Auftrag L: Teil 1 (Alleinanteile, eigenes Skript) bestätigt; Teil 2 Kontrolle K=12, Typ (11,9), Zufallsstart, Startwerte 1–16, je 1800 s, 4 parallel: nur 1 von 16 gelöst (Startwert 5, 48er, disc 8, Allein 12 x 6108, laut Gate gleichwertig zur Paley-Doppelung 1+3; Soll mindestens 2), Restzahlen sonst 8–22. Abbruch nach Auftrag, kein K=11, keine Warmstarts. L: 2026-11-25_leiter48.
- 25.11.2026, Auftrag H, K1 erste Fassung (W fest, U gesucht, nur Einzelzeilen-Züge): 0 von 4 je 600 s (Restzahl 22, falsche Zeilenzuordnung), 8 Stellschrauben-Läufe je 90–180 s ohne Erfolg; behoben durch Tauschzüge. L: 2026-11-25_zerlegung.
- 25.11.2026, Auftrag H: `dfs` ohne Zählschranke auf K1 (60 s) und auf d = 9, W 12 x 10 (120 s) an der Zeitgrenze (steckt in den letzten Zeilen). L: 2026-11-25_zerlegung.
- 25.11.2026, Auftrag H, Schritt 2 (d = 9, U = Paley-12 ohne Spalte 0): W 12 x 12 und 12 x 14 je `dfs` mit Zählschranke 1200 s und `ls` 1200 s unentschieden (Restzahl 1760 / 5280); Gewichtssuche je 240 s, Verhältnis höchstens 0,82 / 0,67. L: 2026-11-25_zerlegung.
- 25.11.2026, Auftrag H, Schritt 4, Zufallsstart mit Neustarts, je 4 Startwerte: K = 11 (4 Typen je 810 s), K = 10 (4 Typen je 300 s, gekürzt, da jede K=10-Lösung eine K=11-Lösung gäbe), d = 9 K = 12 (6 Typen je 570 s): kein Treffer. L: 2026-11-25_zerlegung.

- 25.11.2026, Ideensammlung, Vortests des Orchestrators (von P nicht gegengerechnet): Eintrags-Suche `tern_ls.c`, Kontrolle 16 × 15 nicht bestanden (Restzahl 62–181, je 80–110 s); Halbproblem „W zu U des 48ers" mit CP-SAT und Glucose je Zeitgrenze (120–170 s); verschachtelte Hamming-Hälften 24 192 von 262 144 Klassen offen. Die kozyklischen Vortests n = 18–30 hat P bestätigt (Negativbefunde). L: 2026-11-25_pruef_ideen.
- 25.11.2026, Astra-Vorschläge A (zwei freie Zeilen, 560 + 560 Tripel, unerfüllbar, 1,7 s Suche) und B (ganze Spalte, 240 Fälle, kein Wert unter 233, 13 s): beide negativ, von P bestätigt (Negativbefunde). L: 2026-11-25_pruef_ideen.
- 25.11.2026, Auftrag P: Suchcode `kern_r3.c` brach bei 4/3, Index 1223 irrtümlich mit „VERTEILBAR" ab (Neustart an der Teilmengengrenze 95), behoben und nachgerechnet; erste Fassung des Zertifikatsprüfers ohne Symmetriebruch und Ordnung nach 9 CPU-Min. abgebrochen (danach 4 Prozesse, 5 Min.); Python-Tiefensuche (10 Kugeln) ohne Zuwachsschranke nach 115 s abgebrochen (mit Schranke 5 s). L: 2026-11-25_pruef_ideen.
- 25.11.2026, Auftrag E, Punktgewichte mit Glättung (p004), (20,6) 24 Paare, 4 x 600 s: 891–953 offene Klassen, schlechter als ohne Glättung (p002: 115–673). L: 2026-11-25_evo.
- 25.11.2026, Auftrag E, Insel C (Zeile entfernen, global beste per Walsh-Hadamard, p006), (20,6) 23 Paare, 4 x 600 s: 1569–1683 (nur 120 Züge/s). L: 2026-11-25_evo.
- 25.11.2026, Auftrag E, Doppelungsform bei 23 Paaren, je 4 x 600 s: p007 1276–1286, p016 (11+9) starr 1241, p023 (12+8) starr 1310. L: 2026-11-25_evo.
- 25.11.2026, Auftrag E, 23 Paare: zehn Runden ohne Verbesserung der 704 (Runden 18–29 ohne Nebenziel, je 4 x 600 s; längeres Tabu, Mindestabstand 7, Zeilenersatz, Doppelzüge, Saat, ohne Gewichte), Abbruch laut Auftrag. L: 2026-11-25_evo.
- 25.11.2026, Auftrag E, Nebenziel (15,4) 15 Paare, 4 Runden je 4 x 300 s (p001, p013, p018, p016): immer 233 (Doppelungsform 707). L: 2026-11-25_evo.
- 25.11.2026, Auftrag B2, (d) zwei Neunerbahnen mit Probenpunkt-Tabellenfilter (4 × 24 Punkte): 29 min ohne Ergebnis abgebrochen (Filter ließ 7 % der Paare durch), ersetzt durch Bündelung der Einserbahnen (28 s). L: 2026-11-25_bahnen3_zweit.
- 26.11.2026, Auftrag N, (18,5) K = 12 Typ (9,9): 4 x 300 s und 8 x 900 s (Startwerte 1–12), immer Restzahl 1, ungelöst. L: 2026-11-25_leiter_nachbarzellen.
- 26.11.2026, Auftrag N, K − 1 nach dem besten K: (16,4) K = 15 Typ (9,7) 4 x 300 + 8 x 900 s, Restzahl 28; (21,6) K = 15 Typ (12,9) 8 x 900 s, 3343; (23,7) K = 15 Typ (12,11) 4 x 300 + 8 x 900 s, immer 548; (25,8) K = 15 Typ (16,9) 4 x 300 + 8 x 900 s, 10 598–13 007; (19,5) K = 16 Typ (10,9) 4 x 300 + 8 x 900 s, 768–1280. L: 2026-11-25_leiter_nachbarzellen.
- 26.11.2026, Auftrag N, Schritt 4, (20,6) K = 11 (44 Wörter) Typen (14,6), (15,5), (16,4), je 4 x 600 s: nicht gelöst, Restzahl 4152 / 5996 / 9415 (schlechter als (11,9) 1288 und (10,10) 1063). L: 2026-11-25_leiter_nachbarzellen.
- 26.11.2026, Auftrag LZ: `tar --zstd`/`zstd` fehlten (`pip install zstandard` zum Entpacken, erlaubt); erster `AchsenKern.lean`-Lauf scheiterte an noch nicht gebauten Had24-Stufen (14 Min. Nachbau). L: 2026-11-26_lean_18_5.
- 26.11.2026, Auftrag M, gemeinsame Suche (18,5) 48 Wörter, `misch_alt`, je 8 x 900 s: (11,2), (10,4), (9,6) Typ (9,9) bester Stand 4 offene Punkte (fester Endpunkt); Typ (10,8) 816 ((9,6) nur 4 Startwerte, Zeitgrenze). L: 2026-11-26_mischform_18_5.
- 26.11.2026, Auftrag M, erste gemeinsame Suche `misch_such.c` (Einzelbit-Züge wie p018 auf Paaren): (12,1) nach 30 s noch 342 offene Klassen, verworfen. L: 2026-11-26_mischform_18_5.
- 26.11.2026, Auftrag D, K = 16 bei d = 10: (26,8) 4 Typen 4 x 600 s + Typ (16,10) 8 x 1200 s, (28,9) 3 Typen 4 x 1200 s + Typ (15,13) 8 x 1800 s, (30,10) 2 Typen 4 x 1200 s, (24,7) 4 Typen und (22,6) 4 Typen je 4 x 600 s: kein Treffer, beste Restzahl 115 136 / 125 632 / 1 911 391 / 81 536 / 55 648. L: 2026-11-26_leiter_d10.
- 26.11.2026, Auftrag D: Schritt 3 für (30,10), (24,7), (22,6) wegen der 7-Stunden-Grenze nicht gerechnet; (20,5) nicht begonnen; Block (26,8) Startwerte 9–12 durch das Werkzeug-Zeitlimit (30 Min. Hintergrundjob) nach 10 Min. abgebrochen und neu gerechnet (Vermerk im Protokoll).
- 26.11.2026, Ideenrunde (Orchestrator): Ordnung 24 ohne 5 Spalten mit Schwelle 7 (`had_streich`, alle 5,1 Mio. Fälle ohne Abbruch je Fall) nach 10 Min. am Zeitlimit abgebrochen, nicht nötig (Schwelle 9 genügt). `notes/ideen_2026-11-26.md`, Abschnitt 6.
## Offene Punkte

- Meldung von K(20,6) ≤ 48 (und der Familie): Entscheidung am 27.11.2026 (`notes/zweitmeinung_2026-11-24_auftrag.md`).
- Satz von Kleitman: Zitat belegt (D. J. Kleitman, „On a combinatorial conjecture of Erdős“, J. Combin. Theory 1 (1966), 209–214;
  zwei unabhängige Fundstellen, 24.11.2026), Volltext nicht eingesehen (`notes/recherche_2026-11-24_pruefvermerk.md`, A1).
- Unter den 112 Lösungen zu (15,4), Ordnung 7, 32 Wörter (alle disc 7, 16 x 15) sind 56 zu `ctrl_16` gleichwertig und 56 nicht (zweite Klasse;
  L: 2026-11-25_gegenrechnung15, vom Orchestrator unabhängig bestätigt). Beide Klassen sind punktierte Hadamard-Matrizen der Ordnung 16,
  die zweite ist die Transponierte der ersten; keine ist zu ihrer Transponierten oder zu Sylvester äquivalent. Rechnung des Orchestrators,
  nicht gegengerechnet; Zuordnung zu `had.16.3`/`had.16.4` nicht geprüft (`notes/gate_2026-11-25_gegenrechnung15.md`).
- Verteilte Nullen sind auf Zellen zurückgeführt (Umkodierung nach Sol; n = 11 und n = 14 für alle Zeilen nachgerechnet, n = 17
  Stichprobe): ungerades n gibt die Zelle (n−1, r) mit Paaren, die bis auf eine Koordinate komplementär sind; gerades n die
  selbstkomplementäre Zelle. Die gelockerte Paarbedingung bei (16,4) und (18,5) ist nicht gesucht (Prüfvermerk, Abschnitte I, J).
- Beweise in den Rundlauf-Antworten ungeprüft (L: 2026-11-24_rundlauf). Auftragstext jetzt in `notes/rundlauf_2026-11-24_auftrag.md`.
- 3-Sylowgruppe: Auftrag B2 (eigene Programme, anderes Verfahren) bestätigt „≤ 46 ausgeschlossen“, die 19 008 Lösungen (Listen identisch),
  ihre 8 Klassen (4 × |Aut| 576, darunter 1+3 und 2+2; 2 × 144; 2 × 36) und seed4 als neunten 48er (|Aut| 24); alle neun durch Invarianten
  getrennt. Die H-Treffer sind 1+3 (k3_seed1–8) und 2+2 (a_k12). Offen: ⟨g⟩ mit < 48 Wörtern ungeprüft (L: 2026-11-25_bahnen3_zweit; Gate bestanden, `notes/gate_2026-11-26_bahnen3_zweit.md`).
- n=25 unter 5er-Symmetrie nicht gestartet (L: 2026-11-24_bahnen5).
- Vollständige Aufzählung K=4 bei n=20 (ca. 280 CPU-h) nicht gestartet (L: 2026-11-24_bahnen5); Teilfall zwei 10er-Zyklen erledigt (L: 2026-11-25_zweibahn).
- Variante mit 3 vollen Bahnen plus 5 festen Paaren ungeprüft (A: Fehlversuche).
- n = 11 gemischt: Profile (3,0) ab Typ 4, (2,0), (1,1), (1,0), (0,1) offen; Fortsetzung `bash src/run_profile.sh SEK` (L: 2026-11-25_einzelnull).
- Leiter unter 48 bei (20,6): Kontrolle K=12 aus Zufallsstart 1 von 16 (nicht bestanden); K=11 nie versucht. Nur mit Warmstart aus den bekannten 48ern oder weit mehr Startwerten weiterzuverfolgen (L: 2026-11-25_leiter48). Alle drei bisherigen
  Treffer der Nebenklassen-Suche bei Typ (11,9) sind derselbe Code (Paley-Doppelung 1+3; `notes/gate_2026-11-25_leiter48.md`).
- Gruppenmatrizen: Ordnung 24 (14 nichtzyklische Gruppen, Schwelle 10) und vier gestrichene Spalten bei Ordnung 25 nicht gerechnet (L: 2026-11-25_gruppen).
- Suche im Spann: bei (15,4) Rang 8 und Rang 9 vollständig ausgeschlossen (alle 9 Klassen, Auftrag R9), Rang 10 nicht begonnen; die neun Rang-9-Ausschlüsse ruhen auf einem Programm
  mit Symmetrie (Gate zu R9 bestanden, `notes/gate_2026-11-25_rang9.md`; Gegenprobe ohne Symmetrie nicht gerechnet und nicht geplant: nach einer 90-s-Probe mehrere Stunden je Klasse). Bei (20,6) 78 weitere Spanne nicht durchsucht, Gleichwertigkeit der drei 48er-Spanne offen (L: 2026-11-25_spann; `notes/gate_2026-11-25_spann.md`).
- Auftrag P, offen: d = 9 mit punktierter Paley-Hälfte bei m2 = 12, 14 (Zählung entscheidet nicht); (15,4) mit 5 gestrichenen Zeilen nicht gerechnet; kozyklisch n = 26–30 nur disc ≤ 8 belegt (weitere Gruppen der Ordnung 40 wie Z5⋊Z8, Z4×D5, Ordnung 60 außer Z60 und Dic15 nicht gerechnet); elf Radius-2-Kugeln bei n = 10 nicht selbst entschieden (nach der Tabelle, K(10,2) ≥ 24, unmöglich) (L: 2026-11-25_pruef_ideen).
- Auftrag H: Halbproblem-Löser findet den 48er bei (20,6) aus Zufallsstart in Sekunden (8 von 8; Typ (10,10) ebenso); Treffer sind 1+3 bzw. 2+2 (B2; alle Positivkontrollen des Lösers sind Hadamard-artig); d = 9 mit Paley-U und W 12 x 12 / 12 x 14 offen (L: 2026-11-25_zerlegung).
- Nächster Schritt: Entscheidung zur Meldung K(20,6) ≤ 48 am 27.11.2026 (Neuheitsprüfung am 26.11.). Die Ansatzwahl ist erfolgt (`notes/ideen_2026-11-25.md`): Aufträge E (Evolutionsschleife),
  H (Zerlegung der Kern-Familie) und P (Gegenrechnung der Vortests), freigegeben am 25.11.2026, 18:10 Uhr; danach je ein Gate. P, H und E sind abgeschlossen (Gates bestanden, `notes/gate_2026-11-25_pruef_ideen.md`, `notes/gate_2026-11-25_zerlegung.md`, `notes/gate_2026-11-25_evo.md`). Auftrag N (`notes/leiter_nachbarzellen_2026-11-25_auftrag.md`) ist abgeschlossen (Gate bestanden, `notes/gate_2026-11-26_leiter_n.md`). Auftrag B2 (`notes/gegenrechnung_bahnen3_2026-11-25_auftrag.md`) ist abgeschlossen (Gate bestanden, `notes/gate_2026-11-26_bahnen3_zweit.md`). Auftrag LZ (`notes/lean_18_5_2026-11-26_auftrag.md`) ist abgeschlossen (Gate bestanden, `notes/gate_2026-11-26_lean_18_5.md`): die Gegenrechnung der 50 durch eine Sitzung liegt damit für die Überdeckung vor (Lean, zwei Wege), die Diskrepanz 8 stützt sich weiter auf die drei Prüfprogramme. Auftrag M (`notes/mischform_18_5_2026-11-26_auftrag.md`) ist abgeschlossen (Gate bestanden, `notes/gate_2026-11-26_mischform.md`): kein Code unter 50 Wörtern; die gemeinsame Suche hat bei 48 Wörtern nur reine Paarzustände (K = 12) erzeugt, die echte Mischform ist damit kaum geprüft. Die Linie (18,5) liegt 10 Wörter über der Rekordgrenze (40) und ruht. Auftrag D (`notes/leiter_d10_2026-11-26_auftrag.md`) ist abgeschlossen (Gate bestanden, `notes/gate_2026-11-26_leiter_d10.md`): bei d = 10 kein K = 16; bei (30,10) war die Suche 18-mal schlechter als die Bauregel Sylvester 32 ohne 2 Spalten (107 520 offene Klassen, Probe des Gates), die Zelle ist also nicht ernsthaft geprüft. Damit sind die Leitern bei d = 8, 9 und 10 ohne Rekordkandidat. Offen: Entscheidung von Justus am 27.11.2026 über Meldung und nächsten Ansatz; möglicher billiger Test: Warmstart (`--start`) aus dem Sylvester-Zustand bei (30,10); Entscheidung über (25,8): nach N liegt K = 12 bei d = 9 weit entfernt (K = 15 schon nicht erreicht).
- Auftrag N (abgeschlossen 26.11.2026, Gate bestanden, `notes/gate_2026-11-26_leiter_n.md`; L: 2026-11-25_leiter_nachbarzellen): K(18,5) ≤ 52, im Gate ≤ 50 (gegenüber der Tabelle neu, Literatur noch nicht gründlich geprüft; Überdeckung in Lean bewiesen, Auftrag LZ, Gate `notes/gate_2026-11-26_lean_18_5.md`), K = 12 bei Restzahl 1 (Beinahe-Treffer, fester Wert); Typen mit kleiner Restzahl bei K = 16 nicht nachgefasst, weil nicht bester Typ nach Auftragsregel: (23,7) (14,9) 128, (25,8) (14,11) 188, (21,6) (13,8)/(14,7) 384; die Entscheidung über (25,8): K = 16 erreicht, K = 15 weit entfernt; Hadamard-Becken wie bei Auftrag H wurden nicht geprüft.
- Auftrag E (abgeschlossen, Gate bestanden, `notes/gate_2026-11-25_evo.md`): kein Treffer; Ziele 22–20 Paare nicht begonnen; Gleichwertigkeit und Struktur der 704er sowie des blinden 48ers zu Paley offen (L: 2026-11-25_evo).
- Ideenrunde 26.11. (gegengerechnet von Auftrag C, L: 2026-11-26_gegenrechnung_ideen): amalgamierte direkte Summen sind kein Negativbefund (erste Fassung zurückgenommen); Struktur der 704er beantwortet (3f); der blinde 48er hat dieselben Kennzahlen wie die Paley-Doppelung, Gleichwertigkeit nicht geprüft.

## Prüfrahmen

- `verifiers/SHA256SUMS` sichert `src/check_disc.py`, `verifiers/verify_spencer.py`, `src/exakt_disc.c` (Regel 8; `sha256sum -c`).
- Regel 1 über den Wrapper `verifiers/pruefe_matrix.py` (rechteckig, Nullauffüllung, drei getrennte Prozesse, Fehlercode bei Abweichung).
- Testsuite `verifiers/test_verifiers.py` (7 s): 12 Soll-Diskrepanzen, Defektdateien in `verifiers/defekt/` werden abgelehnt.
- Quellen externer Dateien in `controls/QUELLEN.md`; alle 54 `_pad`-Dateien sind Original plus Nullen.
- Einzelheiten: A: Prüfrahmen; L: 2026-11-24_pruefrahmen.

## Dateien

- `src/check_disc.py`: Prüfskript 1 (Claude), exakte Diskrepanz per Brute-Force.
- `verifiers/verify_spencer.py`: Prüfskript 2 (Gemini), unabhängig geschrieben.
- `src/search_lns.py`, `src/greedy_full.py`: Suche auf dem Quotientenwürfel.
- `src/construct_32.py`: Konstruktion der 32-Wörter-Codes.
- `src/construct_48.py`: Paley-12-Doppelung, Bauregel 1+3 und 2+2 (Teil B), schreibt `controls/had24_*`.
- `src/pruefe_mckay.py`: Gegenrechnung der 60 McKay-Matrizen. `src/streichtest.py`: Prüfcode, überflüssige Nebenklassen.
- `src/hadamard_tests.py`: exakte Diskrepanz (Meet-in-the-middle, int8), Kontrollen, Test A/B, Überdeckungsradius, Nachprüfung der Gegenbeispiele für Test C. Kein Suchcode.
- `src/pruefstand.c`: Gegenbeispielsuche (Teil B). `src/zirkulant.c`, `src/zirkulant_pruef.py`, `src/zirkulant_qualitaet.py`, `src/run_zirkulant.py`: Teil C.
- `src/gruppenmatrix.c` (Suchcode), `src/gruppen_erzeuge.py`, `src/gruppen_pruef.py`, `src/run_gruppen.py`, `src/kern_nebenklassen.py` (Prüfcode/Treiber): Auftrag G. Tafeln, Logs, beste Matrizen `runs/gruppen/`.
- `src/spann_zaehl.py` (Zählcode), `src/spann_dfs.c`, `src/spann_ls.c` (Suchcode), `src/spann_aut.c`, `src/spann_sym.py` (Symmetrie), `src/spann_pruef.py` (Prüfcode), `src/run_spann_teil{2b,3}.sh`: Auftrag S. Logs `runs/spann/`.
- `src/leiter_allein.py`: Prüfcode, Alleinanteil je Nebenklasse (Auftrag L). Logs `runs/leiter48/`.
- `src/gegen15_sym.py`: Prüfcode, Gegenrechnung (15,4) unter Spaltensymmetrie Ordnung 7, 11, 13 (Teil 1–4, Gleichwertigkeit zu `ctrl_16`). Logs `runs/gegenrechnung15/`.
- `src/pruefe_reell16.py`, `src/dichte_schranke.py`, `src/kleitman_probe.py`: Prüfcode zu Ansatz 4 (Polieren um ctrl_16, Dichteschranke, Kleitman-Probe).
- `LANDKARTE.md`: Übersicht Familien x N.
- `src/rundlauf/`: Ansatz 3. `extrahiere.py` (Generatoren aus den Modellantworten), `astra_1..8.py`, `sol_1..8.py` (Modellcode unverändert), `familien.py` (Tabelle, Lader), `zaehle.py` (Zahl/Form), `sieb.py` (Treiber für `pruefstand`, prüft nichts), `pruefe.py` (Nachrechnen mit ganzen Zahlen, exakte Prüfung über `build/exakt_disc`).
- `src/exakt_disc.c`: Prüfcode, exakte Diskrepanz per Gray-Code (auch mit gestrichenen Spalten, `-s k`). `src/gegenrechnung_a.py`: Teil A.
- `src/bahnen3_aut.c`, `src/bahnen3_gruppe.py`, `src/bahnen3_zaehl.c`, `src/bahnen3_pruef.py`: Prüfcode 3-Sylowgruppe des 48ers. `src/bahnen3_tabu.c`: Suchcode dazu.
- `src/bahnen_zaehl.c`, `src/bahnen_gegen10.py`, `src/bahnen_pruef.py`, `src/bahnen_aut.py`: Prüfcode Bahnen unter 5er-Zyklen. `src/bahnen_tabu.c`: Suchcode dazu.
  `src/bahnen_schaetz.c`, `src/bahnen_schaetz_filter.py`: Aufwandsschätzung. Bauen: `gcc -O3 -march=native -fopenmp -o build/<name> src/<name>.c`.
- `src/gegenbeispiel40.c`: Suchcode für Test C (Tabu-Suche auf Vorzeichenvektoren). Bauen: `gcc -O3 -march=native -o build/gegenbeispiel40 src/gegenbeispiel40.c`.
- `src/offene_zellen.py`: Prüfcode, offene Zellen aus der Post-Kéri-Tabelle (Teil C, 24.11.2026). `src/gegenrechnung_testc.py`: Prüfcode, zweite
  Nachrechnung der 30 780 Gegenbeispiele zu Test C ohne `hadamard_tests.py`. Logs `runs/status_umzug/`.
- `src/zweibahn_zaehl.c` (Zählcode, Zwei-Bahnen-Codes), `src/zweibahn_brute.py` (Prüfcode, Brute-Force-Test), `src/run_zweibahn_teil{1,2,3}.sh`. Logs `runs/zweibahn/`.
- `src/pruef_ideen/` (Such- und Rechencode: `kern_r3.c`, `spalte.c`, `kozyk.c`, `ueberdeckung10.c`), `verifiers/pruef_ideen/` (getrennte Prüfer: `pruef_kern.py`, `pruef_kozyk.py`, `zerlegung_zahlen.py`, `astra10.py`): Auftrag P. Belege `runs/pruef_ideen/`.
- `src/b2_zaehl.c`, `src/b2_aequiv.c`, `src/b2_teil0.py`, `src/b2_teil2.py`, `src/b2_pruef_loesung.py`, `src/b2_invarianten.py`, `src/b2_invarianten2.py`: Auftrag B2 (zweite Zählung unter P, eigener Kanonisierer, Invarianten). Belege und die neun Vertreter `runs/bahnen3_zweit/`.
- `notes/verlauf/`: Archiv der vollständigen STATUS.md vom 24.11.2026 (eingefroren). `notes/rundlauf_2026-11-24_auftrag.md`: Auftragstext Ansatz 3.
- `src/zerl_offen.py` (Prüfcode), `src/zerl_halb.c` (Suchcode: `ls`, `dfs`, `alt`, `zaehl`), `src/zerl_eingaben.py`, `src/zerl_test_dfs.py`, `src/zerl_gewichte.py`, `src/run_zerl_{k1,s2,alt,ziel}.sh`: Auftrag H. Logs `runs/zerlegung/`.
- `src/evo_bewertung.c` (Prüfcode, Bewerter, eingefroren in `runs/evo/BEWERTER.sha256`), `src/evo_bewertung_kontrolle.sh`, `src/evo/p001.c`–`p025.c` (Suchcode), `src/evo/runde.sh`: Auftrag E. Archiv `runs/evo/archiv.tsv`, Ausgaben und Logs `runs/evo/`.
- `src/run_leiter_n.sh`, `src/run_leiter_typen.sh`, `src/run_leiter_stufen.sh` (Treiber für `zerl_halb alt`), `src/leiter_zusammen.sh` (Zusammenfassung): Auftrag N, kein neuer Suchcode. Logs, Paare, Matrizen `runs/leiter_n/`.
- `src/misch_frei.c` (exakt: reichen f freie Zeilen?), `src/misch_alt.c` und `src/misch_such.c` (gemeinsame Suche), `src/run_misch_s{2,3}.sh`; Prüfcode `verifiers/mischform/` (`pruef_misch.py`, `brute_frei.py`): Auftrag M. Logs `runs/mischform/`.
- `verifiers/gegenrechnung_ideen/` (Prüfcode: `eingaben.py`, `gr.c`, `rm15.c`, `fasern.py`, `zellen.py`, `ersatz1_kontrolle.py`): Auftrag C, Gegenrechnung der Ideenrunde vom 26.11. Logs `runs/gegenrechnung_ideen/`.
