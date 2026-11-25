# STATUS

Stand: 25.11.2026 (nach Bahnen unter der 3-Sylowgruppe des 48ers). Timebox: 23.11.2026 bis 04.01.2027, danach Entscheidung über Abbruch.
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
| 8 | 40 | (16,4) | 34–64 | 1,54 | | C |
| 8 | 40 | (18,5) | 28–64 | 1,93 | 64 (Suche, bekannt) | C; A: Rundgang Nebenklassen-Suche |
| 8 | 40 | (20,6) | 24–64 | 2,31 | **48** (besser als Tabelle) | C; A: Hadamard-Bauregel und Tests |
| 9 | 50 | (19,5) | 40–64 | 1,59 | | C |
| 9 | 50 | (21,6) | 34–64 | 1,96 | | C |
| 9 | 50 | (23,7) | 29–64 | 2,33 | | C |
| 9 | 50 | (25,8) | 25–64 | 2,69 | | C |
| 10 | 62 | (20,5) | 62–128 | 1,28 | | C |
| 10 | 62 | (22,6) | 49–128 | 1,63 | | C |
| 10 | 62 | (24,7) | 41–128 | 1,98 | | C |
| 10 | 62 | (26,8) | 36–128 | 2,34 | | C |
| 10 | 62 | (28,9) | 30–128 | 2,70 | | C |
| 10 | 62 | (30,10) | 27–128 | 3,06 | | C |
| 11 | 76 | (23,6) | 73–224 | 1,32 | | C |
| 11 | 76 | (25,7) | 60–224 | 1,64 | | C |
| 11 | 76 | (27,8) | 50–224 | 1,99 | | C |
| 11 | 76 | (29,9) | 43–224 | 2,33 | | C |
| 11 | 76 | (31,10) | 37–224 | 2,69 | | C |

Eigene bessere Werte außerhalb dieser Tabelle (m > B(8) = 20, also kein Rekordweg): 48 Wörter bei (22,7) [Tabelle 21–64], (24,8) [19–64],
(26,9) [17–56], (28,10) [15–56] (A: Normalität und Folgecodes aus den 56 Wörtern; A: Gate-Prüfung … viertes Gate).

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
- **K(20,6) ≤ 48 in Lean bewiesen (24.11.2026):** Lean 4.30.0-rc2, nur Kernbibliothek, `lean/`. Satz: jedes x < 2^20 liegt im Abstand ≤ 6
  von einer Zeile von M oder −M (`had24_1p3_paley12` und `had24_2p2_paley12`); Radius 5 abgelehnt; Prüfschleife als korrekt bewiesen, ohne `sorry`.
  Zwei Wege: `native_decide` (je 2 Min.) und `decide +kernel` (10,4 bzw. 5,8 Min., Achsen nur `propext`, `Quot.sound`). Kontrollen Hamming [7,4], `ctrl_16`,
  Negativkontrolle (48er ohne ein Paar) wie erwartet. Grenzen: Lean liest die Kontrolldatei nicht selbst (Erzeuger + Prüfsumme), Reduktion auf die
  Diskrepanz nicht formalisiert, Neuheit offen. L: 2026-11-24_lean

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
| Bahnen aus 5er-Zyklen plus Komplement, n=15, R=4 | mindestens 40 Wörter (525 Vierermengen) | voll | L: 2026-11-24_bahnen5 |
| Bahnen aus 5er-Zyklen plus Komplement, n=20, R=6 | 70 Wörter gefunden, 60 nicht | Suche | L: 2026-11-24_bahnen5 |
| Bahnen unter der 3-Sylowgruppe P (Ordnung 9) des 48ers, n=20, R=6 | Minimum 48 Wörter (≤ 46 ausgeschlossen); nur Größe 1/3: 19 008 Lösungen | voll | L: 2026-11-25_bahnen3 |
| Bahnen unter einem Element der Ordnung 3 des 48ers, n=20, R=6 | Tabu-Kontrolle 48 Wörter nicht bestanden (1/4, 0/4), < 48 ungeprüft | Suche | L: 2026-11-25_bahnen3 |
| Zeilen mit genau einer Null, n = 11, 11 Zeilen (Einträge {-1,0,1}) | disc 6 unmöglich (Rekord 1,809 ausgeschlossen); nur {-1,0,1}, nicht reell | voll | L: 2026-11-25_einzelnull |
| Zeilen mit genau einer Null, n = 11: Umkodierung auf zwei Radius-2-Kugeln im 10-Würfel nachgerechnet (5632 Zeilen, 0 Abw.) | Ausschluss auch über K(10,2) ≥ 24 | voll (Tabelle) | L: 2026-11-25_einzelnull |
| Träger 10/8/6 gemischt, n = 11, Profile (q,s) mit Budget ≤ 48: (5,0) (4,0) (3,1) (2,1) (1,2) (0,2) | keine Lösung | voll | L: 2026-11-25_einzelnull |
| Träger 10/8/6 gemischt, Profile (3,0) (2,0) (1,1) (1,0) (0,1) | nichts gefunden, (3,0) Typen 0–3 voll, Rest offen | Suche | L: 2026-11-25_einzelnull |
| Selbstkomplementäre Codes (13,3) (= n = 14 mit einer Null je Zeile, P1 bestätigt) | 42 Wörter gefunden, 40 nicht (4 x 10 Min.) | Suche | L: 2026-11-25_einzelnull |
| Rundlauf Ansatz 3 (16 Familien, 388 503 Matrizen) | 0 Überlebende, bester Quotient 1,400 | Gegenbeispiel | L: 2026-11-24_rundlauf; A: Ansatz 3 |
| Reelle Einträge um `ctrl_16` (Zeilen im L1-Abstand ≤ 2) | nichts über 7 | voll | L: 2026-11-24_reell16 |
| Reelle Matrizen n = 8, 9, 12 (Dichte < 1) | nichts über dem Rekord | voll, bedingt auf Kleitman (belegt: J. Combin. Theory 1 (1966), 209–214) | L: 2026-11-24_reell16; K |
| Nebenklassen-Suche d=2, (18,5) | nur 64 Wörter (K=15: 0 von 12) | Suche | A: Rundgang Nebenklassen-Suche |
| Nebenklassen-Suche d=2, (21,6) | Kontrolle 64 Wörter nicht erreicht (0 von 12) | Suche | A: Rundgang Nebenklassen-Suche |
| Nebenklassen-Suche d=2, (23,7) | 60 Wörter nicht erreicht (0 von 8) | Suche | A: Rundgang Nebenklassen-Suche |
| Freie Suche (15,4) mit 30 Wörtern | bleibt bei 466 offenen Punkten | Suche | A: Ziele; A: Fehlversuche |

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

## Offene Punkte

- Meldung von K(20,6) ≤ 48 (und der Familie): Entscheidung am 27.11.2026 (`notes/zweitmeinung_2026-11-24_auftrag.md`).
- Satz von Kleitman: Zitat belegt (D. J. Kleitman, „On a combinatorial conjecture of Erdős“, J. Combin. Theory 1 (1966), 209–214;
  zwei unabhängige Fundstellen, 24.11.2026), Volltext nicht eingesehen (`notes/recherche_2026-11-24_pruefvermerk.md`, A1).
- Zählungen des Orchestrators zu (15,4) nicht gegengerechnet: keine 30 Wörter unter Symmetrie der Ordnung 7 (Typ 7+7+1), 11, 13;
  `ctrl_16` ohne eine Zeile lässt immer 466 Punkte offen (`notes/orchestrator/2026-11-24_abend/README.md`).
- Verteilte Nullen sind auf Zellen zurückgeführt (Umkodierung nach Sol; n = 11 und n = 14 für alle Zeilen nachgerechnet, n = 17
  Stichprobe): ungerades n gibt die Zelle (n−1, r) mit Paaren, die bis auf eine Koordinate komplementär sind; gerades n die
  selbstkomplementäre Zelle. Die gelockerte Paarbedingung bei (16,4) und (18,5) ist nicht gesucht (Prüfvermerk, Abschnitte I, J).
- Beweise in den Rundlauf-Antworten ungeprüft (L: 2026-11-24_rundlauf). Auftragstext jetzt in `notes/rundlauf_2026-11-24_auftrag.md`.
- 3-Sylowgruppe: Die 19 008 Lösungen bilden 8 Äquivalenzklassen (4 mit Gruppenordnung 576, darunter 1+3 und 2+2; 2 mit 144; 2 mit 36);
  der Tabu-Treffer aus Teil 4 ist ein neunter 48er (Gruppenordnung 24). Rechnung des Orchestrators, nicht gegengerechnet
  (`notes/gate_2026-11-25_bahnen3.md`). „≤ 46 ausgeschlossen“ ruht auf einem Zählprogramm; ⟨g⟩ mit < 48 Wörtern ungeprüft (L: 2026-11-25_bahnen3).
- n=25 unter 5er-Symmetrie nicht gestartet (L: 2026-11-24_bahnen5).
- Vollständige Aufzählung K=4 bei n=20 (ca. 280 CPU-h) nicht gestartet (L: 2026-11-24_bahnen5).
- Variante mit 3 vollen Bahnen plus 5 festen Paaren ungeprüft (A: Fehlversuche).
- n = 11 gemischt: Profile (3,0) ab Typ 4, (2,0), (1,1), (1,0), (0,1) offen; Fortsetzung `bash src/run_profile.sh SEK` (L: 2026-11-25_einzelnull).
- Nächster Schritt: Ideensammlung (Literatur, Methodentransfer).

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
- `notes/verlauf/`: Archiv der vollständigen STATUS.md vom 24.11.2026 (eingefroren). `notes/rundlauf_2026-11-24_auftrag.md`: Auftragstext Ansatz 3.
