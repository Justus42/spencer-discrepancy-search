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

## Schritt 3: abwechselnde Suche (`zerl_halb alt`), Treiber `src/run_zerl_alt.sh`

Verfahren: Zufallsstart (U, W); Phase „U fest“: Gewichtssuche auf W mit der exakten Restzahl als Grundkosten (Anforderungsvektoren aus U mit
Vielfachheit), bis 300 Iterationen ohne Verbesserung (`--phase`); dann „W fest“ ebenso für U; usw. Tauschzüge wie in K1.
Vorversuch (`test_alt`, Startwerte 1–4, je ≤ 180 s, 18:25 UTC): 3 von 4 in 1,1–1,5 s gelöst; Startwert 3 hing ab Runde 2 bei Restzahl 228 (218 Runden).
Ergänzt: `--neustart R` = neuer Zufallsstart nach R Runden ohne Verbesserung. Treffer `test_alt_seed1` mit `pruefe_matrix.py`: 8/8/8.

### K3: K = 12, Typ (11,9), d = 8, Startwerte 1–8, je ≤ 900 s, `--neustart 20` (18:28–18:29 UTC), Log `runs/zerlegung/k3.log.txt`

| Startwert | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|
| Zeit bis Restzahl 0 | 1,2 s | 1,3 s | 20,7 s (1 Neustart) | 1,8 s | 1,0 s | 1,7 s | 24,3 s (1 Neustart) | 1,2 s |

**8 von 8 gelöst** (Soll ≥ 4 von 8; alte Nebenklassen-Suche 1 von 16 zu 30 Min.). Alle 8 Paare: Prüfcode 0 offen; Matrix 24 x 20 mit
`pruefe_matrix.py`: check_disc, verify_spencer, exakt_disc je **Diskrepanz 8** (`runs/zerlegung/k3_pruefe.log.txt`). Fingerabdruck
(Abstandsprofile der 48 Wörter, Beträge der Zeilen-Skalarprodukte 0:108, 2:144, 4:24) gleich dem von 1+3 und 2+2; er trennt diese beiden
nicht, Gleichwertigkeit der acht Treffer also offen. **K3 bestanden.** Schritt 4 beginnt.

## Schritt 4: Ziele, `bash src/run_zerl_ziel.sh PRAEFIX K d SEK TYPEN…` (je Typ 4 Startwerte parallel, `--neustart 20`, Commit nach jedem Block)

### (a) d = 8, K = 11 (44 Wörter, Zelle (20,6)), Typen (11,9), (13,7), (10,10), (12,8); 60 Min.

K = 12-Probe je Typ (1 Startwert, ≤ 300 s, 18:30–18:35 UTC, `runs/zerlegung/a_k12.log.txt`): (10,10) **gelöst** nach 38,8 s (2 Neustarts;
`pruefe_matrix.py` 8/8/8; das ist der Typ der 2+2-Doppelung); (12,8) Restzahl 180; (13,7) Restzahl 290 (je 300 s, nicht gelöst).

| Typ | Block (UTC) | je Startwert | Ergebnis K = 11 | beste Restzahl (von 262 144) |
|---|---|---|---|---|
| (11,9) | 18:35–18:48 | 810 s, ca. 257 000 Iterationen, 27 Neustarts | nicht gelöst (0 von 4) | **1288** (alle vier Startwerte gleich) |
| (13,7) | 18:48–19:02 | 810 s, ca. 206 000 Iterationen, 28 Neustarts | nicht gelöst (0 von 4) | **2698** (alle vier gleich) |
| (10,10) | 19:03–19:16 | 810 s, ca. 304 000 Iterationen, 40 Neustarts | nicht gelöst (0 von 4) | **1063** (drei Startwerte; vierter 1116) |
| (12,8) | 19:16–19:30 | 810 s, ca. 111 000 Iterationen, 13 Neustarts | nicht gelöst (0 von 4) | **1836** (alle vier gleich) |

Ergebnis (a): **kein Code mit 44 Wörtern** in der Familie gefunden (Belegart: Suche erfolglos). Gleiche Restzahl über alle Startwerte deutet auf
ein festes bestes Minimum der Suche je Typ hin. Der 1288-Zustand bei (11,9) ist kein Beinahe-Treffer (303 verschiedene offene y, 136 offene z).
Rechenzeit (a): 5 + 4 · 13,5 = 59 Min.

### (b) d = 8, K = 10 (40 Wörter, Rekord), dieselben Typen; gekürzt auf 20 Min.

Begründung der Kürzung: Jede Lösung mit K = 10 gibt durch eine beliebige elfte Nebenklasse eine Lösung mit K = 11; die ist in (a) in keinem Typ
gefunden. Daher je Typ nur 4 Startwerte zu 300 s.

| Typ | Block (UTC) | je Startwert | Ergebnis K = 10 | beste Restzahl (von 262 144) |
|---|---|---|---|---|
| (11,9) | 19:30–19:35 | 300 s | nicht gelöst (0 von 4) | 3234 (alle vier) |
| (13,7) | 19:35–19:40 | 300 s | nicht gelöst (0 von 4) | 5926 (alle vier) |
| (10,10) | 19:40–19:45 | 300 s | nicht gelöst (0 von 4) | 1852 (zwei; sonst 1916) |
| (12,8) | 19:45–19:51 | 300 s | nicht gelöst (0 von 4) | 4280 (zwei; sonst 4349) |

Ergebnis (b): kein Code mit 40 Wörtern gefunden (Suche erfolglos). Log `runs/zerlegung/b_k10.log.txt`.

### (c) d = 9, K = 12 (48 Wörter), Typen (11,10), (12,9), (12,11), (13,10), (12,13), (11,14); U, W aus Zufallsstart; je Typ 4 x 570 s

| Typ | Zelle | Block (UTC) | Ergebnis K = 12 | beste Restzahl (von 2^(m1−1) · 2^(m2−1)) |
|---|---|---|---|---|
| (11,10) | (21,6) | 19:51–20:01 | nicht gelöst (0 von 4) | 12 957 von 524 288 (alle vier) |
| (12,9) | (21,6) | 20:01–20:10 | nicht gelöst (0 von 4) | 11 002 von 524 288 (alle vier) |
| (12,11) | (23,7) | 20:10–20:20 | nicht gelöst (0 von 4) | **1760** von 2 097 152 (alle vier) |
| (13,10) | (23,7) | 20:20–20:30 | nicht gelöst (0 von 4) | 26 360 von 2 097 152 (alle vier) |
| (12,13) | (25,8) | 20:30–20:39 | nicht gelöst (0 von 4) | 5280 von 8 388 608 (alle vier) |
| (11,14) | (25,8) | 20:39–20:49 | nicht gelöst (0 von 4) | 5280 von 8 388 608 (sonst 21 560, 55 485, 55 328) |

Ergebnis (c): **kein Code mit 48 Wörtern bei d = 9** in der Familie gefunden (Suche erfolglos). Log `runs/zerlegung/c_d9.log.txt`.
Die besten Zustände bei (12,11) bestehen aus einer vollen Hadamard-Matrix 12 x 12 (alle Zeilen-Skalarprodukte 0) und einer Hadamard-Matrix ohne
Spalte (alle ±1): dieselbe Restzahl 1760 wie die lokale Suche in Schritt 2 (Paley-U, W 12 x 12). Ebenso 5280 bei (11,14) wie in Schritt 2. Die freie
Suche läuft also von selbst in die Paley-Struktur; die liegt bei d = 9 weit von einer Überdeckung.

Abgleich: alle 56 Läufe aus Schritt 4 (Einzellogs je Typ) – beste Restzahl der Suche gleich der Nachzählung mit `zerl_offen.py`, 0 Abweichungen
(`runs/zerlegung/abgleich_restzahlen.log.txt`). `sha256sum -c verifiers/SHA256SUMS` am Ende (20:49 UTC): alle drei OK (`runs/zerlegung/sha256_ende.log.txt`).

## Zusammenfassung

- Kontrollen: Schritt 0 (alle Vortestzahlen), K1 (4/4 beide Richtungen, ≤ 6,3 s), K1b (`ctrl_17` zerlegt, 4/4 beide Richtungen), K2 (voll), K3 (8/8, ≤ 24 s) bestanden.
- Kein Treffer: K = 11 und K = 10 bei (20,6) in vier Typen, d = 9 mit K = 12 in sechs Typen nicht erreicht (Suche erfolglos).
- Schritt 2 unentschieden (m2 = 12, 14); m2 = 10 als Kontrolle mit der Zählschranke über alle 880 Anforderungen ausgeschlossen.
- Methodisch: Der Löser findet den 48er aus Zufallsstart in Sekunden (alte Nebenklassen-Suche 1 von 16 in 30 Min.); entscheidend waren dynamische
  Gewichte je offenem Paar und Tauschzüge zweier Zeilenwörter. Die Suche endet je Typ immer am selben besten Wert (z. B. 1288 bei (11,9), K = 11).
- Rechenzeit (Blöcke, Wanduhr, 4 Kerne): etwa 3 h 20 Min. von 6 h; Sitzung 17:15–20:52 UTC (Zeiten nach `date`).

## Fehlversuche (für STATUS.md)

- K1 erste Fassung, W fest / U gesucht, nur Einzelzeilen-Züge: 0 von 4 in je 600 s (Restzahl 22, falsche Zeilenzuordnung); Stellschrauben 8 x 90–180 s ohne Erfolg; behoben durch Tauschzüge.
- `dfs` auf K1 (60 s) und auf d = 9, m2 = 10 ohne Zählschranke (120 s): Zeitgrenze, steckt in den letzten Zeilen.
- Schritt 2, d = 9, U = Paley-12 ohne Spalte 0: m2 = 12 und 14 je `dfs` mit Zählschranke 1200 s und `ls` 1200 s, unentschieden; Gewichtssuche je 240 s, bestes Verhältnis 0,82 / 0,67.
- Schritt 4 (a)–(c) wie oben, je 4 Startwerte, `--neustart 20`: 810 s (K = 11), 300 s (K = 10), 570 s (d = 9) je Typ; kein Treffer.
