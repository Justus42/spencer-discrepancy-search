# LANDKARTE

Stand 25.11.2026 (nach Auftrag L, Leiter unter 48 bei (20,6)). Bester Quotient disc / √N je Matrixfamilie (Zeile) und Größe N (Spalte), N = max(n, Zeilen).
Ziel: Quotient > 5/√8 = 1,7678 (Rekord: Hamming + Nullspalte, N = 8). Für d gilt die Parität nicht in der Schwellenzeile;
bei ±1-Zeilen mit gerader Länge sind nur gerade d möglich.

**Belegart:** `e` exakt (vollständige Aufzählung, Werte aus `controls/`, `runs/teilA/`, `runs/teilC/`; Rekord-nahe Einträge
mit beiden Prüfskripten), `r` exakt über die Reduktion (Überdeckungsradius per Brute-Force, `construct_32.py`),
`g` obere Schranke: für jeden Fall ein Gegenbeispiel gefunden und nachgerechnet (exakter Wert nicht bekannt),
`s` Suche erfolglos: Quotient in Klammern ist das Ziel, das nicht erreicht wurde (kein Beweis, dass es nicht existiert).
Bei Familien mit mehreren Mitgliedern gilt der beste Wert; bei Teil C ist das Maximum über alle Klassen exakt
(jede Klasse durch ein Gegenbeispiel nach oben beschränkt, der beste Vertreter exakt). Ebenso bei den „Rundlauf“-Zeilen (Ansatz 3): jede Matrix der Familie per nachgerechnetem Gegenbeispiel nach oben beschränkt, mindestens eine exakt auf dem Wert; 0 Überlebende.

## N bis 20

| Familie | 6 | 7 | 8 | 9 | 10 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **Rekordschwelle: d nötig (Quotient)** | 5 (2,041) | 5 (1,890) | 6 (2,121) | 6 (2,000) | 6 (1,897) | 7 (1,871) | 7 (1,807) | 8 (2,000) | 8 (1,940) | 8 (1,886) | 8 (1,835) | 8 (1,789) |
| Hamming [7,4] + Nullspalte (`ctrl_8`) |  |  | 1,768 e |  |  |  | |  |  |  |  |  |
| eigene Suche (`ctrl_9`) |  |  |  | 1,667 e |  |  | |  |  |  |  |  |
| 32-Wörter-Codes (`ctrl_16`, `ctrl_17`, `L2026_17x17`, `construct_32.py`) |  |  |  |  |  |  | | 1,750 e | 1,698 e |  | 1,606 e |  |
| Hadamard voll (Ordnung 20: 3 Klassen, 24: 60 Klassen, je mit Transponierten) |  |  |  |  |  |  | |  |  |  |  | 1,342 e |
| Hadamard ohne 1 Spalte (alle Spalten, alle Klassen) |  |  |  |  |  |  | |  |  |  |  | 1,565 e |
| Hadamard 20 ohne 2 / 24 ohne 3 Spalten (alle Paare/Tripel) |  |  |  |  |  |  | |  |  |  |  | 1,342 e |
| Bahnen unter 5er-Zyklen plus Komplement (n=15: 40 Wörter, alle 525 Vierermengen, `bahn15_40w`) |  |  |  |  |  |  | |  |  |  |  | 1,565 e |
| Bahnen unter der 3-Sylowgruppe P des 48ers (n=20, R=6; 40 Wörter = Rekord 1,789 ausgeschlossen, voll) | | | | | | | | | | | | – (voll) |
| (15,4) unter Spaltensymmetrie Primzahlordnung 7 (7+7+1), 11, 13: 30 Wörter ausgeschlossen, 32 Wörter nur Ordnung 7 (112 Lösungen, 2 Klassen, eine `ctrl_16`; voll) | | | | | | | | 1,750 e | | | | |
| Paley-Konferenzmatrizen | 1,225 e |  |  |  | 0,949 e | 1,336 e | |  |  | 1,179 e |  |  |
| zirkulant n x n (Teil C, alle Klassen) |  | 1,134 e |  |  |  |  | |  |  | 0,943 e |  | 1,342 e |
| zirkulant + Einser-Zeile + Nullspalte, (n+1) x (n+1) (Teil C) |  |  | 1,768 e |  |  |  | |  |  |  | 0,918 e |  |
| Rundlauf Astra 2 Duaden (15x15, 59 049 Matrizen) | | | | | | | 0,775 e | | | | | |
| Rundlauf Sol 1 Fano-Ebenentausch (15x15, 153 664) | | | | | | | 1,291 e | | | | | |
| Rundlauf Astra 4 Walsh-Charts (20x20, 1 024) | | | | | | | | | | | | 0,894 e |
| Rundlauf Astra 6 Differenzblöcke (20x18, 2 160) | | | | | | | | | | | | 1,342 e |
| Rundlauf Sol 2 Paley-12-Verkabelung (20x20, 71 280) | | | | | | | | | | | | 0,894 e |

## N ab 21

| Familie | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 | 40 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **Rekordschwelle: d nötig (Quotient)** | 9 (1,964) | 9 (1,919) | 9 (1,877) | 9 (1,837) | 9 (1,800) | 10 (1,961) | 10 (1,925) | 10 (1,890) | 10 (1,857) | 10 (1,826) | 10 (1,796) | 11 (1,945) | 12 (1,897) |
| 32-Wörter-Codes (`ctrl_16`, `ctrl_17`, `L2026_17x17`, `construct_32.py`) | 1,528 r |  | 1,460 r |  |  |  |  |  |  |  |  |  |  |
| Hadamard voll (Ordnung 20: 3 Klassen, 24: 60 Klassen, je mit Transponierten) |  |  |  | 1,225 e |  |  |  |  |  |  |  |  |  |
| Hadamard ohne 1 Spalte (alle Spalten, alle Klassen) |  |  |  | 1,429 e |  |  |  |  |  |  |  |  |  |
| Hadamard 20 ohne 2 / 24 ohne 3 Spalten (alle Paare/Tripel) |  |  |  | 1,429 e |  |  |  |  |  |  |  |  |  |
| Doppelung [[H,H],[H,-H]], H Hadamard 20, Bauregel 1+3/2+2 (40 x 36) |  |  |  |  |  |  |  |  |  |  |  |  | ≤1,265 g |
| Paley-12-Doppelung, Bauregel 1+3 und 2+2 (24 x 20) |  |  |  | 1,633 e |  |  |  |  |  |  |  |  |  |
| Überdeckungscodes aus Nebenklassen-Suche (48/52/56/60 Wörter bei (20,6), 52 bei (22,7), 64 bei (18,5)) |  |  |  | 1,633 e |  | 1,569 e |  | 1,512 e |  | 1,461 e |  | 1,414 e |  |
| Bahnen unter der 3-Sylowgruppe P des 48ers (n=20, R=6; unter P 48 Wörter optimal, ≤ 46 ausgeschlossen, voll) |  |  |  | 1,633 e |  |  |  |  |  |  |  |  |  |
| Nebenklassen-Suche, nicht erreichte Ziele |  |  |  |  |  |  |  |  |  | (1,643) s |  | (1,591) s |  |
| zirkulant n x n (Teil C, alle Klassen) | 1,091 e |  | 1,460 e | 1,225 e | 1,400 e | 1,177 e |  |  | 1,300 e | 1,095 e | 1,257 e |  |  |
| zirkulant + Einser-Zeile + Nullspalte, (n+1) x (n+1) (Teil C) | 1,309 e | 1,492 e |  | 1,429 e | 1,200 e | 1,373 e | 1,155 e |  |  |  |  |  |  |
| Rundlauf Astra 1 Paley-Scharnier (25x23, 4 356) | | | | | 1,400 e | | | | | | | | |
| Rundlauf Astra 3 Fano-Blöcke (25x21, 1 176) | | | | | 1,000 e | | | | | | | | |
| Rundlauf Astra 5 Golay-Bahnen (25x23, 2 346) | | | | | 1,400 e | | | | | | | | |
| Rundlauf Astra 7 SL(2,3) (25x25, 1 568) | | | | | 1,000 e | | | | | | | | |
| Rundlauf Astra 8 punktierte Walsh-Basen (31x30, 1 024) | | | | | | | | | | | 1,078 e | | |
| Rundlauf Sol 3 Walsh-Bänke (25x23, 8 192) | | | | | 1,000 e | | | | | | | | |
| Rundlauf Sol 4 Dreispalten-Lift (25x25, 7 920) | | | | | 1,400 e | | | | | | | | |
| Rundlauf Sol 5 Parabelbänder (25x25, 6 144) | | | | | 1,000 e | | | | | | | | |
| Rundlauf Sol 6 Paley-26 (31x26, 40 680) | | | | | | | | | | | 1,078 e | | |
| Rundlauf Sol 7 Hadamard-16 geschaltet (31x28, 17 920) | | | | | | | | | | | 0,718 e | | |
| Rundlauf Sol 8 vier Walsh-Bänke (31x30, 10 000) | | | | | | | | | | | 1,078 e | | |

## Bester Eintrag dieser Karte je N (nur `e`/`r`)

Nur die Familien oben, ohne Auffüllen. Jede Matrix der Größe N gibt mit Nullzeilen/-spalten auch d/√(N+k); so liegt z. B.
bei N = 18 schon `ctrl_17` mit 7/√18 = 1,650 über dem Tabellenwert.


| N | 6 | 7 | 8 | 9 | 10 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Quotient | 1,225 | 1,134 | 1,768 | 1,667 | 0,949 | 1,336 | 1,291 | 1,750 | 1,698 | 1,179 | 1,606 | 1,565 | 1,528 | 1,492 | 1,460 | 1,633 | 1,400 | 1,569 | 1,155 | 1,512 | 1,300 | 1,461 | 1,257 | 1,414 |

## Zeilen mit einer Null, Einträge in {-1, 0, 1} (Auftrag A, 25.11.2026)

Eigene Tabelle, weil N = 11, 13 in den Tabellen oben fehlen. Protokoll `notes/laeufe/2026-11-25_einzelnull.md`.

| Familie | N | Ergebnis | Quotient | Belegart |
|---|---|---|---|---|
| n = 8, 8 Zeilen mit je einer Null (Träger 7) | 8 | 57 600 Lösungen mit disc 5, davon 55 680 mit verteilten Nullen (`runs/einzelnull/n8_verteilt.txt`) | 1,768 | e |
| n = 11, jede Zeile genau eine Null (Träger 10) | 11 | disc 6 ausgeschlossen; disc ist gerade, Zirkulant mit Nulldiagonale erreicht 4 | 1,206 (max.) | e |
| n = 11, Träger 10, 8, 6 gemischt | 11 | disc 6: Profile mit Budget ≤ 48 ausgeschlossen (voll); (3,0), (2,0), (1,1), (1,0), (0,1) offen | (1,809) | s |
| Zirkulant mit Nulldiagonale n = 11, 14, 17 (alle Klassen) | 11/14/17 | Maximum 4, 5, 6 | 1,206 / 1,336 / 1,455 | e |
| selbstkomplementäre Codes (13,3), Tabu-Leiter (n = 14 mit einer Null je Zeile ist äquivalent, P1) | 21 | kleinste erreichte Wortzahl **42** (21 x 13, disc 7, 3 Prüfprogramme); 40 nicht erreicht | 1,528 | e (42) / (1,565) s (40) |

## Nötige Dichte, Schranke für reelle Matrizen

Für reelle n x n-Matrizen in [-1,1] (auch mit Nullen aufgefüllte) mit disc > 5√(n/8) müssen 2n Kappen den Würfel {±1}^n überdecken; jede Kappe hat
Durchmesser ≤ D (D größte ganze Zahl mit 8(n−D)² > 25n). Mit dem Satz von Kleitman (J. Combin. Theory 1 (1966), 209–214; Zitat belegt, Volltext nicht eingesehen) fasst eine Kappe höchstens Kl Punkte.
Nötige Dichte 2n·Kl/2^n ≥ 1. Unter 1: Für dieses n schlägt keine reelle Matrix den Rekord (Belegart `e`, bedingt auf Kleitman). Dichte ≥ 1 sagt nichts.
Programm `src/dichte_schranke.py`, Protokoll `notes/laeufe/2026-11-24_reell16.md`.

| n | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| D | 2 | 3 | 4 | 5 | 5 | 6 | 7 | 8 | 8 | 9 | 10 | 11 | 12 | 12 | 13 | 14 | 15 | 16 |
| nötige Dichte | **0,562** | **0,633** | 1,094 | 1,203 | **0,785** | 1,200 | 1,292 | 1,777 | 1,229 | 1,306 | 1,733 | 1,829 | 2,306 | 1,645 | 1,724 | 2,142 | 2,235 | 2,694 |

Fett: unter 1, also n = 8, 9, 12 ausgeschlossen. Lokal um `ctrl_16` / `ctrl_8` (Zeilen im L1-Abstand ≤ 2) gibt es ebenfalls nichts über 7 bzw. 5
(`notes/laeufe/2026-11-24_reell16.md`, Schritt 3).

## Einzelheiten zu den Zeilen

- **(15,4) unter Spaltensymmetrie:** Gegenrechnung 25.11.2026 (`notes/laeufe/2026-11-25_gegenrechnung15.md`, `src/gegen15_sym.py`, Logs `runs/gegenrechnung15/`):
  Ordnung 7 (7+7+1): 2 volle Bahnen + 2 feste Zeilen (32 Wörter) 112 Überdeckungen, bei 30 und 28 Wörtern 0 (kleinste Restzahl 466 bzw. 1100);
  Ordnung 13 (13+1+1) und 11 (11+1⁴): eine volle Bahn + 2–4 bzw. 4–6 feste Zeilen (30, 32, 34 Wörter) 0. Bestes Ergebnis 32 Wörter, disc 7, Quotient 1,750 (= `ctrl_16`).
- **32-Wörter-Codes:** N = 16, 17, 19 aus `controls/` (beide Prüfskripte, disc 7); N = 21, 23 nur Radius per Brute-Force (STATUS).
- **Hadamard:** volle Matrizen `controls/had20_klasse*.txt`, McKay-60 (`controls/had24_mckay_hex.txt`); ohne 1 Spalte:
  `runs/teilA/zusatz_ohne_eine_spalte.log.txt`; ohne 2/3 Spalten: `runs/teilA/a1*`, `a2*` (alle 1140 bzw. 242 880 Fälle).
- **Bahnen unter 5er-Zyklen plus Komplement:** `controls/bahn15_40w.txt` (20 x 15, beide Prüfskripte 7), Aufzählung vollständig
  (`notes/laeufe/2026-11-24_bahnen5.md`); bei n=20 nur 70 Wörter gefunden (60 Wörter: Suche erfolglos, 40: nicht versucht).
- **Bahnen unter der 3-Sylowgruppe P des 48ers:** P (Ordnung 9, nicht zyklisch, Normalteiler von Aut = 576) auf den 2^19 Zeilenpaaren,
  Bahnen 8/160/58 200. Vollständige Zählung: nur Bahnen der Größe 1/3 Minimum genau 24 Zeilen (19 008 Lösungen, 48er dabei); mit
  Neunerbahnen keine Überdeckung mit ≤ 23 Zeilen. Also kein P-invarianter Code mit 40, 42, 44 oder 46 Wörtern (Rekord bräuchte 40).
  Nur ein Element der Ordnung 3 (⟨g⟩, 174 848 Bahnen): Tabu-Kontrolle (48 Wörter) nicht bestanden (1 von 4 bzw. 0 von 4), < 48 ungeprüft.
  Protokoll `notes/laeufe/2026-11-25_bahnen3.md`.
- **Nebenklassen-Leiter unter 48 bei (20,6) (Auftrag L, 25.11.2026):** Kern ⟨1,a⟩, Typ (11,9). Kleinste erreichte Wortzahl weiter **48** (Quotient 1,633 e); neu ein dritter 48er aus K=12-Zufallsstart (`runs/woerter_n20_R6_d2_K12_typ11-9_k12_seed5.txt`, disc 8 mit drei Programmen). K=12 aus Zufallsstart: 1 von 16 Läufen à 1800 s (Kontrolle nicht bestanden), K=11 (44 Wörter, N = 22, 1,706) nicht versucht. Protokoll `notes/laeufe/2026-11-25_leiter48.md`.
- **Doppelung Ordnung 40:** Test C in STATUS, alle 30 780 Spaltenauswahlen mit Gegenbeispiel max ≤ 8, Rekord bräuchte 12.
  Nur gleiche Zeilenausrichtung in beiden Hälften geprüft.
- **Paley-12-Doppelung:** `controls/had24_1p3_paley12.txt`, `had24_2p2_paley12.txt`, beide Prüfskripte disc 8.
- **Überdeckungscodes:** (20,6) mit 48/52/56/60 Wörtern gibt N = 24/26/28/30, (22,7) mit 52 Wörtern N = 26, (18,5) mit
  64 Wörtern N = 32, alle disc 8 mit beiden Prüfskripten. Nicht erreicht: (23,7) mit 60 Wörtern (N = 30, disc 9),
  (21,6) mit 64 Wörtern (N = 32, disc 9). Nicht gestartet: (25,8) mit 50 Wörtern (N = 25, disc 9, Quotient 1,800),
  (20,6) mit 40 Wörtern (N = 20, disc 8, Quotient 1,789).
- **Zirkulant (Teil C):** Protokoll `notes/laeufe/2026-11-24_zirkulant.md`, Matrizen `runs/teilC/beste/`.
  n = 7 ohne Rand ist Kontrolle (Maximum 3); mit Rand ist n = 7 der Hamming-Rekord (N = 8, disc 5).
- **Rundlauf (Ansatz 3):** 16 Familien aus je einem Python-Generator (`src/rundlauf/astra_*.py`, `sol_*.py`, Modellcode unverändert), 388 503 Matrizen, für jede ein Gegenbeispiel mit Maximum < d
  (d = Spalten − 2R: 7, 8, 9, 10), 0 Überlebende. Beste Familienwerte 3–7, Quotienten 0,718–1,400 gegen 1,7678. Protokoll `notes/laeufe/2026-11-24_rundlauf.md`, Gegenbeispiele `runs/rundlauf/*.txt.gz`.
  Die Beweise in den Modellantworten sind ungeprüft, der Auftragstext an die Modelle liegt nicht im Repo; die Quotienten gelten nur für die gelieferten Matrizen, nicht für die Zielzellen.
