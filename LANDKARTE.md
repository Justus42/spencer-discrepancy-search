# LANDKARTE

Stand 24.11.2026. Bester Quotient disc / √N je Matrixfamilie (Zeile) und Größe N (Spalte), N = max(n, Zeilen).
Ziel: Quotient > 5/√8 = 1,7678 (Rekord: Hamming + Nullspalte, N = 8). Für d gilt die Parität nicht in der Schwellenzeile;
bei ±1-Zeilen mit gerader Länge sind nur gerade d möglich.

**Belegart:** `e` exakt (vollständige Aufzählung, Werte aus `controls/`, `runs/teilA/`, `runs/teilC/`; Rekord-nahe Einträge
mit beiden Prüfskripten), `r` exakt über die Reduktion (Überdeckungsradius per Brute-Force, `construct_32.py`),
`g` obere Schranke: für jeden Fall ein Gegenbeispiel gefunden und nachgerechnet (exakter Wert nicht bekannt),
`s` Suche erfolglos: Quotient in Klammern ist das Ziel, das nicht erreicht wurde (kein Beweis, dass es nicht existiert).
Bei Familien mit mehreren Mitgliedern gilt der beste Wert; bei Teil C ist das Maximum über alle Klassen exakt
(jede Klasse durch ein Gegenbeispiel nach oben beschränkt, der beste Vertreter exakt).

## N bis 20

| Familie | 6 | 7 | 8 | 9 | 10 | 14 | 16 | 17 | 18 | 19 | 20 |
|---|---|---|---|---|---|---|---|---|---|---|---|
| **Rekordschwelle: d nötig (Quotient)** | 5 (2,041) | 5 (1,890) | 6 (2,121) | 6 (2,000) | 6 (1,897) | 7 (1,871) | 8 (2,000) | 8 (1,940) | 8 (1,886) | 8 (1,835) | 8 (1,789) |
| Hamming [7,4] + Nullspalte (`ctrl_8`) |  |  | 1,768 e |  |  |  |  |  |  |  |  |
| eigene Suche (`ctrl_9`) |  |  |  | 1,667 e |  |  |  |  |  |  |  |
| 32-Wörter-Codes (`ctrl_16`, `ctrl_17`, `L2026_17x17`, `construct_32.py`) |  |  |  |  |  |  | 1,750 e | 1,698 e |  | 1,606 e |  |
| Hadamard voll (Ordnung 20: 3 Klassen, 24: 60 Klassen, je mit Transponierten) |  |  |  |  |  |  |  |  |  |  | 1,342 e |
| Hadamard ohne 1 Spalte (alle Spalten, alle Klassen) |  |  |  |  |  |  |  |  |  |  | 1,565 e |
| Hadamard 20 ohne 2 / 24 ohne 3 Spalten (alle Paare/Tripel) |  |  |  |  |  |  |  |  |  |  | 1,342 e |
| Bahnen unter 5er-Zyklen plus Komplement (n=15: 40 Wörter, alle 525 Vierermengen, `bahn15_40w`) |  |  |  |  |  |  |  |  |  |  | 1,565 e |
| Paley-Konferenzmatrizen | 1,225 e |  |  |  | 0,949 e | 1,336 e |  |  | 1,179 e |  |  |
| zirkulant n x n (Teil C, alle Klassen) |  | 1,134 e |  |  |  |  |  |  | 0,943 e |  | 1,342 e |
| zirkulant + Einser-Zeile + Nullspalte, (n+1) x (n+1) (Teil C) |  |  | 1,768 e |  |  |  |  |  |  | 0,918 e |  |

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
| Nebenklassen-Suche, nicht erreichte Ziele |  |  |  |  |  |  |  |  |  | (1,643) s |  | (1,591) s |  |
| zirkulant n x n (Teil C, alle Klassen) | 1,091 e |  | 1,460 e | 1,225 e | 1,400 e | 1,177 e |  |  | 1,300 e | 1,095 e | 1,257 e |  |  |
| zirkulant + Einser-Zeile + Nullspalte, (n+1) x (n+1) (Teil C) | 1,309 e | 1,492 e |  | 1,429 e | 1,200 e | 1,373 e | 1,155 e |  |  |  |  |  |  |

## Bester Eintrag dieser Karte je N (nur `e`/`r`)

Nur die Familien oben, ohne Auffüllen. Jede Matrix der Größe N gibt mit Nullzeilen/-spalten auch d/√(N+k); so liegt z. B.
bei N = 18 schon `ctrl_17` mit 7/√18 = 1,650 über dem Tabellenwert.


| N | 6 | 7 | 8 | 9 | 10 | 14 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Quotient | 1,225 | 1,134 | 1,768 | 1,667 | 0,949 | 1,336 | 1,750 | 1,698 | 1,179 | 1,606 | 1,565 | 1,528 | 1,492 | 1,460 | 1,633 | 1,400 | 1,569 | 1,155 | 1,512 | 1,300 | 1,461 | 1,257 | 1,414 |

## Einzelheiten zu den Zeilen

- **32-Wörter-Codes:** N = 16, 17, 19 aus `controls/` (beide Prüfskripte, disc 7); N = 21, 23 nur Radius per Brute-Force (STATUS).
- **Hadamard:** volle Matrizen `controls/had20_klasse*.txt`, McKay-60 (`controls/had24_mckay_hex.txt`); ohne 1 Spalte:
  `runs/teilA/zusatz_ohne_eine_spalte.log.txt`; ohne 2/3 Spalten: `runs/teilA/a1*`, `a2*` (alle 1140 bzw. 242 880 Fälle).
- **Bahnen unter 5er-Zyklen plus Komplement:** `controls/bahn15_40w.txt` (20 x 15, beide Prüfskripte 7), Aufzählung vollständig
  (`notes/laeufe/2026-11-24_bahnen5.md`); bei n=20 nur 70 Wörter gefunden (60 Wörter: Suche erfolglos, 40: nicht versucht).
- **Doppelung Ordnung 40:** Test C in STATUS, alle 30 780 Spaltenauswahlen mit Gegenbeispiel max ≤ 8, Rekord bräuchte 12.
  Nur gleiche Zeilenausrichtung in beiden Hälften geprüft.
- **Paley-12-Doppelung:** `controls/had24_1p3_paley12.txt`, `had24_2p2_paley12.txt`, beide Prüfskripte disc 8.
- **Überdeckungscodes:** (20,6) mit 48/52/56/60 Wörtern gibt N = 24/26/28/30, (22,7) mit 52 Wörtern N = 26, (18,5) mit
  64 Wörtern N = 32, alle disc 8 mit beiden Prüfskripten. Nicht erreicht: (23,7) mit 60 Wörtern (N = 30, disc 9),
  (21,6) mit 64 Wörtern (N = 32, disc 9). Nicht gestartet: (25,8) mit 50 Wörtern (N = 25, disc 9, Quotient 1,800),
  (20,6) mit 40 Wörtern (N = 20, disc 8, Quotient 1,789).
- **Zirkulant (Teil C):** Protokoll `notes/laeufe/2026-11-24_zirkulant.md`, Matrizen `runs/teilC/beste/`.
  n = 7 ohne Rand ist Kontrolle (Maximum 3); mit Rand ist n = 7 der Hamming-Rekord (N = 8, disc 5).
