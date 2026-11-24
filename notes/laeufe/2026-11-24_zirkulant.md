# Lauf 24.11.2026: zirkulante ±1-Matrizen, vollständig (Teil C)

Cloud-Sitzung, 4 Kerne (Xeon 2,1 GHz), Branch `claude/cool-cannon-td98xf`. Rohprotokoll: `runs/teilC/blocklog.txt`.

## Verfahren

- `src/zirkulant.c` (Suchcode): zählt alle ersten Zeilen a ∈ {±1}^n bis auf Verschiebung, Komplement und
  Multiplikatoren t → mt (ggT(m,n) = 1; m = −1 ist die Transponierte) auf. Vertreter = kleinstes a der Bahn.
- Je Klasse erst **Filter**: Tabu-Suche nach x mit max |(Ax)_i| < d (d = Rekordschwelle). Jedes gefundene x wird im
  Programm neu nachgerechnet. Nur Klassen ohne solches x (Überlebende) gehen an die exakte Prüfung.
- Danach **Abstieg** (kleines Budget): kleinstes gefundenes Maximum u = obere Schranke der Diskrepanz dieser Klasse.
- **Exakt** (Prüfcode `build/exakt_disc`, Gray-Code über alle 2^(n−1) Vektoren, über `src/zirkulant_pruef.py`):
  alle Überlebenden und die Vertreter mit größtem u, bis einer exakt u_max erreicht. Dann ist u_max die größte
  Diskrepanz aller Klassen genau (jede Klasse hat disc ≤ u ≤ u_max, und einer erreicht u_max).
- Rand: zusätzliche Einser-Zeile und Nullspalte, also (n+1) x (n+1). Schwelle d: kleinstes d mit d/√N > 5/√8 und
  Parität von n (N = n bzw. n+1).

## Kontrollen (vorab)

| Kontrolle | Ergebnis |
|---|---|
| Klassenzahl n = 3–15 gegen unabhängige Bahnen-Zählung in Python | alle gleich (2, 4, 3, 8, 5, 14, 11, 26, 15, 88, 37, 130, 184) |
| Hamming-Rekord: n = 7 mit Rand, d = 5 | 5 Klassen, **genau 1 Überlebender** (erste Zeile Bits {0,1,3}), exakt **5** = Rekord wiedergefunden |
| Gegenprobe n = 7 ohne Rand, d = 5 | 0 Überlebende, Maximum 3 |
| u gegen exakte Werte **aller** Klassen, n = 8, 10, …, 18, mit und ohne Rand | u nie kleiner als exakt; u_max = exaktes Maximum in allen 12 Fällen; u zu groß nur in 1 bis 26 Klassen (`runs/teilC/qualitaet.log.txt`) |

## Ergebnis je n

| n | Rand | d | Klassen | Überlebende | u_max (Vertreter) | beste Diskrepanz exakt | Quotient | Laufzeit |
|---|---|---|---|---|---|---|---|---|
| 18 | nein | 8 | 1 346 | 0 | 4 (124) | 4 | 0,943 | 1 s |
| 20 | nein | 8 | 3 760 | 0 | 6 (3) | 6 | 1,342 | 3 s |
| 21 | nein | 9 | 4 434 | 0 | 7 (3) | **5** (alle 3 Vertreter mit u = 7 sind exakt 5) | 1,091 | 6 s |
| 23 | nein | 9 | 8 385 | 0 | 7 (8) | 7 | 1,460 | 11 s |
| 24 | nein | 10 | 47 462 | 0 | 6 (76) | 6 | 1,225 | 16 s |
| 25 | nein | 9 | 33 781 | 0 | 7 (86) | 7 | 1,400 | 21 s |
| 26 | nein | 10 | 108 330 | 0 | 6 (354) | 6 | 1,177 | 41 s |
| 18 | ja | 8 | 1 346 | 0 | 4 (301) | 4 | 0,918 | 1 s |
| 20 | ja | 10 | 3 760 | 0 | 6 (5) | 6 | 1,309 | 4 s |
| 21 | ja | 9 | 4 434 | 0 | 7 (23) | 7 | 1,492 | 6 s |
| 23 | ja | 9 | 8 385 | 0 | 7 (68) | 7 | 1,429 | 11 s |
| 24 | ja | 10 | 47 462 | 0 | 6 (205) | 6 | 1,200 | 18 s |
| 25 | ja | 11 | 33 781 | 0 | 7 (402) | 7 | 1,373 | 21 s |
| 26 | ja | 10 | 108 330 | 0 | 6 (886) | 6 | 1,155 | 50 s |
| 29 | nein | 11 | 331 185 | 0 | 7 (2 832) | 7 | 1,300 | 3,5 Min. |
| 30 | nein | 10 | 2 251 816 | 0 | 6 (12 872) | 6 | 1,095 | 18,7 Min. (2 Teile) |
| 31 | nein | 11 | 1 155 735 | 0 | 7 (16 621) | 7 | 1,257 | 13,7 Min. |

Quotient = beste Diskrepanz / √N. Beste Vertreter als Matrizen: `runs/teilC/beste/` (Dateiname = erste Zeile in hex,
Bit t = 1 heißt Eintrag −1). Für n = 18, 20, 21 (mit und ohne Rand) zusätzlich mit `check_disc.py` bestätigt.

**Befund: kein Überlebender, kein Kandidat.** Zirkulante Matrizen liegen weit unter der Schwelle: bester Quotient
1,492 (n = 21 mit Rand), Rekord 1,768. Die Diskrepanz wächst mit n kaum (Maximum 4 bis 7 für n = 18–31).

## Blöcke

| Block | Zeit (UTC) | Inhalt |
|---|---|---|
| Kontrollen | 14:52–14:56 | Zählung, n = 7, Qualitätstest |
| 1 | 14:57 | n = 18, 20, 21, 23, 24 |
| 2 | 14:58–15:01 | n = 25, 26; n = 18–26 mit Rand |
| 3 | 15:01–15:18 | n = 29, 31 |
| 4a | 15:20–15:28 | n = 30, Teil 1/2 (Blöcke abwechselnd, `-T 1/2`) |
| 4b | 15:29–15:39 | n = 30, Teil 2/2 |

Abweichung: n = 30 lief in zwei Teilen, weil der Lauf am Stück knapp an 30 Minuten gekommen wäre. Die Teilung ist
an n = 24 geprüft (20 414 + 27 048 = 47 462 Klassen, gleiche Verteilung). Kein Fehlversuch.
