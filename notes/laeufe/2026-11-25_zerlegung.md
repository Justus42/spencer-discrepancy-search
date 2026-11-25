# Lauf 25.11.2026: Zerlegung der Kern-⟨1,a⟩-Familie, Halbproblem-Löser (Auftrag H)

Auftrag: `notes/zerlegung_2026-11-25_auftrag.md` (freigegeben 25.11.2026, 18:10 Uhr). Branch `ccr-fe07bd17-kedjsz`, `main` = `5f43ac7`.
Cloud-Sitzung, 4 Kerne, Beginn 17:15 UTC. Logs `runs/zerlegung/`. Nichts installiert.

Neu:
- `src/zerl_offen.py` (Prüfcode, nur ganze Zahlen): offene Klassen (y,z) mit y_0 = z_0 = +1; zweite Zählung direkt auf der 2K x (m1+m2)-Matrix
  (`--direkt`); Profile; Umwandlung (U,W) → Matrix für `verifiers/pruefe_matrix.py`; Zeilenzuordnungen (Schachtel); Zerlegung einer Matrix nach Kernelement a.
- `src/zerl_eingaben.py`: Eingaben (Paley-12, Hamming aus `controls/ctrl_8.txt`) nach `runs/zerlegung/eingaben/`.
- `src/zerl_halb.c` (Suchcode): Halbproblem-Löser. `ls` = lokale Suche über ganze Zeilenwörter mit dynamischen Gewichten je offenem Paar (z, Anforderung)
  (Breakout; Alternative `--verfahren tabu`); `dfs` = vollständige Tiefensuche (nur maximale Anforderungsvektoren, Spaltenvorzeichen: erste belegte Zeile
  lauter +1, Spaltenreihenfolge: Intervallklassen gleicher Spalten, Schranke je Anforderungsvektor: offene z ≤ Summe der Kugelvolumina der freien Zeilen);
  `brute` = alle Belegungen (Test); `alt` = abwechselnde Suche. Bauen: `gcc -O3 -march=native -o build/zerl_halb src/zerl_halb.c`.
- `src/zerl_test_dfs.py`: Test `dfs` gegen `brute` an kleinen Zufallsfällen, Lösungen mit `zerl_offen.py` nachgezählt.

## Prüfsummen (Regel 8)

`sha256sum -c verifiers/SHA256SUMS` zu Beginn (17:16 UTC): `src/check_disc.py` OK, `verifiers/verify_spencer.py` OK, `src/exakt_disc.c` OK.

## Schritt 0: Vortest nachgerechnet (Belegart voll, eigenes Skript), Log `runs/zerlegung/schritt0.log.txt`

| Größe | Soll | Ist |
|---|---|---|
| 48er (U = Paley-12 ohne Spalte 0, W = ohne Spalten 0,1,2), d = 8 | 0 offen | 0 von 262 144; direkte Zählung auf der 24 x 20-Matrix ebenfalls 0 |
| `pruefe_matrix.py` auf der Matrix aus dem Paar | 8 | check_disc 8, verify_spencer 8, exakt_disc 8 (Zeilenfolge anders als `controls/had24_1p3_paley12.txt`, gleiche Zeilenmenge) |
| Profile \|U y\|, 1024 Klassen | 660 / 220 / 132 / 12 | (7,5²,3³,1⁶) 660; (5³,3⁶,1³) 220; (9,3⁵,1⁶) 132; (11,1¹¹) 12 |
| kritisch bei d = 8 | nur die 220, 220 Anforderungsvektoren (3³,5⁶,7³) | 220 kritisch, 220 verschiedene Vektoren, alle (3,3,3,5,5,5,5,5,5,7,7,7) |
| Profile \|W z\|, 256 Klassen | 72/72/54/36/12/9/1 | (5³,3³,1⁶) 72; (7,5,3³,1⁷) 72; (5²,3⁶,1⁴) 54; (7,3⁶,1⁵) 36; (9,3²,1⁹) 12; (5⁴,1⁸) 9; (3¹²) 1 |
| Hamming-Schachtel (U 8 x 11, W 8 x 9), d = 8 | 24 192 für jede Zuordnung | 24 192 für alle 40 320 Zuordnungen |

Nebenbei: bei d = 9 hat der 48er 56 992 offene Klassen. Keine Abweichung, Schritt 1 beginnt.

## Schritt 1: Halbproblem-Löser

Vorversuche (ohne Wertung, je 100–120 s, Ausgaben in der Sitzung): Tabu-Suche (`--verfahren tabu`) auf K1: beste Restzahl 8 in 100 s, nicht gelöst.
Tiefensuche auf K1: 60 s Zeitgrenze, steckt in den letzten drei Zeilen (16,9 Mio. Knoten in Tiefe 12). Lokale Suche mit dynamischen Gewichten:
4 von 4 Startwerten gelöst in 16–47 s. Diese wird für K1 eingesetzt.

Test `dfs` gegen `brute` (`src/zerl_test_dfs.py 300 7`, K = 2–4, mf = 3–7, mg = 3–6, d zufällig): 300 Fälle, 77 lösbar, 223 nicht lösbar,
**0 Abweichungen**; alle dfs-Lösungen mit `zerl_offen.py` bestätigt. Log `runs/zerlegung/test_dfs_seed7.log.txt`.

### K1 (Positivkontrolle 48er), Treiber `bash src/run_zerl_k1.sh W|U`, je höchstens 600 s, Startwerte 1–4, 4 parallel

Erste Fassung (nur Einzelzeilen-Züge, Gewichte; Logs `runs/zerlegung/k1_vorversion/`, 17:30–17:41 UTC): Richtung W gesucht 4 von 4 (15–47 s);
**Richtung U gesucht 0 von 4** (je 600 s, Restzahl 22 in allen vier). Analyse der besten Zustände: U ist dort schon Hadamard-artig (alle 66
Zeilen-Skalarprodukte ±1, Profil (5³,3⁶,1³) der offenen y), nur die Zuordnung der Zeilen zu W ist falsch; ein Einzelzeilen-Zug repariert keine
Vertauschung. Ergänzt: **Tauschzüge** (Wörter zweier Zeilen tauschen), geprüft an jedem lokalen Minimum vor der Gewichtserhöhung; Bewertung
nur über Punkte, an denen die Zeile etwas ändert; optionale Glättung (`--glatt`, Standard aus).

Endfassung (17:52 UTC), Logs `runs/zerlegung/k1_W.log.txt`, `k1_U.log.txt`:

| Richtung | gelöst | Zeit je Startwert | Soll |
|---|---|---|---|
| U fest (Paley-12 ohne Spalte 0), W 12 x 9 gesucht | **4 von 4** | 0,0 / 0,0 / 0,0 / 0,0 s (15–33 Iterationen) | ≥ 3 von 4 unter 10 Min. |
| W fest (Paley-12 ohne Spalten 0,1,2), U 12 x 11 gesucht | **4 von 4** | 0,1 / 0,1 / 1,2 / 6,3 s | ≥ 3 von 4 unter 10 Min. |

Alle 8 Lösungen mit `zerl_offen.py` nachgezählt (0 offen). Je eine (Startwert 4) zusätzlich direkt auf der Matrix gezählt und mit
`pruefe_matrix.py` geprüft: check_disc, verify_spencer, exakt_disc je **Diskrepanz 8** (`runs/zerlegung/k1_pruefe.log.txt`). **K1 bestanden.**

### K1b (zweite Positivkontrolle `ctrl_17`), Log `runs/zerlegung/k1b.log.txt`

`zerl_offen.py zerlege controls/ctrl_17.txt`: genau zwei nichttriviale Kernelemente, Gewicht 8 (Spalten 7,8,10–14,16) und 9 (0–6,9,15); K = 8,
Typ (9,8) (`runs/zerlegung/k1b_zerlegung.txt`, Paar `eingaben/paar_ctrl17.txt`). Prüfcode: 0 offene Klassen bei d = 7 (auch direkt auf der
16 x 17-Matrix), 10 976 bei d = 8. Matrix aus dem Paar: `pruefe_matrix.py` 7/7/7; Zeilenmenge nach Rückordnung der Spalten gleich `ctrl_17`.
Suche (je ≤ 600 s, Startwerte 1–4): U fest, W gesucht **4 von 4**; W fest, U gesucht **4 von 4**; je unter 0,1 s, alle 8 mit Prüfcode bestätigt. **Bestanden.**

### K2 (Negativkontrolle, vollständig), Log `runs/zerlegung/k2.log.txt`

U des 48ers ohne Zeile 0, 5 bzw. 11 (K = 11), d = 8, m2 = 9: `dfs` meldet je **KEINE LOESUNG (vollständig)** an der Wurzel (1 Knoten): der einzige
maximale Anforderungsvektor ist (7,…,7) (die gestrichene Zeile als flaches y), offene z 256 > 11 · 10 = 110. Gegenprobe lokale Suche je 30 s:
Restzahl 1448 / 1451 / 1464, keine Lösung. **Bestanden.** Grenze: K2 prüft nur die Wurzelschranke; die tiefere Logik der `dfs` ist durch den
Vergleich mit `brute` (300 Fälle) geprüft.

K1, K1b, K2 bestanden um 17:55 UTC (40 Min. Sitzungszeit). Schritt 2 beginnt.

## Schritt 2: d = 9, U = Paley-12 ohne Spalte 0 (12 x 11), W 12 x 12 und 12 x 14

Vorbereitung (17:56–18:06 UTC):
- `dfs` mit der Volumenschranke je Anforderungsvektor schafft nicht einmal den bekannt unmöglichen Fall m2 = 10 (120 s Zeitgrenze, 2 Mio. Knoten;
  880 maximale Anforderungsvektoren). Die Schranke erfasst Sols Zählargument nicht.
- Neu im Suchcode (`zerl_halb zaehl`, `dfs --lam`): **Zählschranke**, verallgemeinert nach Sol (Frage 2). Gewichte je Stufe, Kosten Λ(|v|) kumuliert;
  N0 = kleinste Summe Λ über alle Stufenvektoren (|v_1|,…,|v_12|), die alle 880 Anforderungen erfüllen (exakt per Branch-and-Bound);
  B = Σ_z Λ(|c·z|) ist für jedes Wort c gleich. Wurzel: N0 · 2^(m2−1) > 12 · B ⇒ kein W. Knoten: Σ_z max(0, N0 − Kosten der belegten
  Zeilen bei z) ≤ (freie Zeilen) · B.
- Kontrolle mit Sols Gewichten Λ(4,6,8) = (1,4,10): m2 = 10: N0 = 10, 10 · 512 = 5120 > 4920 = 12 · 410 → **ausgeschlossen** (Sols Zahlen
  exakt reproduziert, jetzt mit allen 880 Anforderungen). m2 = 12: 20 480 gegen 25 980; m2 = 14: 81 920 gegen 128 472 (wie Sol, Tabelle Frage 2).
- Gewichtssuche (`src/zerl_gewichte.py`, je 240 s, 2865 bzw. 2311 Auswertungen): bestes Verhältnis N0 · NZ / (12 · B) = **0,823** (m2 = 12) bzw.
  **0,665** (m2 = 14), beide mit Zuwächsen (0,1,2,6,0,…) für |v| ≥ 2,4,6,8,…; es müsste über 1 liegen. **Reine Zählung entscheidet nicht.**
  Logs `runs/zerlegung/s2_gewichte_m12.log.txt`, `…_m14.log.txt`.
- Test der Knotenschranke gegen `brute` (`zerl_test_dfs.py` mit `--lam 1,2,3,4`, `0,1,3,6`, `2,0,5,1`; Startwerte 11–13): 900 Fälle, 0 Abweichungen
  (`runs/zerlegung/test_dfs_lam.log.txt`).

Block Schritt 2 (18:04–18:24 UTC, `bash src/run_zerl_s2.sh 1200`, 4 parallel, je 1200 s), Log `runs/zerlegung/s2.log.txt`, Einzellogs `s2_*.log.txt`:

| Fall | Verfahren | Ergebnis |
|---|---|---|
| m2 = 10 (Kontrolle, bekannt unmöglich) | `dfs --lam 0,1,3,6,0` | **KEINE LOESUNG** an der Wurzel (Zählschranke 5120 > 4920) |
| m2 = 12, Zelle (23,7) | `dfs --lam 0,1,2,6,0,0` (N0 = 9, 18 432 gegen 22 392) | **ZEITGRENZE** 1200 s, 8,1 Mio. Knoten, 406 919 Schnitte durch die Knoten-Zählschranke; unentschieden |
| m2 = 14, Zelle (25,8) | `dfs --lam 0,1,2,6,0,0,0` (73 728 gegen 110 820) | **ZEITGRENZE** 1200 s, 2,9 Mio. Knoten, 0 Schnitte; unentschieden |
| m2 = 12 | `ls`, Startwert 1 | Restzahl 1760 (von 2 097 152), Suche erfolglos |
| m2 = 14 | `ls`, Startwert 1 | Restzahl 5280 (von 8 388 608), Suche erfolglos |

Ergebnis Schritt 2: **beide Fälle unentschieden** (Belegart: Suche erfolglos). Die Tiefensuche hängt in den letzten zwei, drei Zeilen; reine Zählung
reicht nicht (Verhältnis höchstens 0,82 bzw. 0,67). Nicht weiter verfolgt (Auftrag: höchstens 30 Min. je Fall).
