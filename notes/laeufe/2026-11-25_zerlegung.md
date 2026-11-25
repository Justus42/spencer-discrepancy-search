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
