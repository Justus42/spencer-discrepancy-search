# Gate zu Auftrag G (Gruppenmatrizen über nichtzyklischen Gruppen), 25.11.2026

Geprüfter Stand: Branch `claude/serene-goldberg-ofa5qc`, Commit `d16fba8`. Prüfer: Orchestrator (Chat vom 25.11., gegen 13 Uhr).
Eigenes Programm ohne Code der Sitzung und **ohne Symmetrieabbau**: `notes/orchestrator/2026-11-25_gate_gruppen/`
(Ausgabe `ausgabe.log.txt`, etwa 4 Minuten Rechenzeit auf 2 Kernen). Rechnung des Orchestrators, nicht gegengerechnet;
sie ist selbst die Gegenrechnung zur Sitzung.

## Urteil

**Bestanden. Kein Rekordkandidat.** Der Hauptbefund der Sitzung (0 Überlebende) ist mit einem zweiten, unabhängigen
Verfahren bestätigt: Für jede einzelne Funktion f gibt es ein nachgerechnetes x unter der Schwelle.

## Was geprüft wurde

| Nr. | Punkt | Ergebnis |
|---|---|---|
| 1 | Die zehn Gruppentafeln der Sitzung gegen eigene Tafeln (Assoziativität, abelsch, Zentrum, Elementordnungen) | alle gleich; die Kennzahlen legen die Gruppe bei diesen Ordnungen fest |
| 2 | Positivkontrolle des Gate-Programms: Z7 mit Rand, d = 5 | 14 von 64 Funktionen unwiderlegt, alle exakt 5 (Hamming); ohne Rand und mit d = 7: 0 |
| 3 | Volle Matrizen, alle f mit f(e) = +1: Ordnung 18 (4 Gruppen, mit und ohne Rand, je 2^17), Ordnung 20 (4 Gruppen, je 2^19), Z7:Z3 (mit und ohne Rand, 2^20), Z5xZ5 (2^24) | **kein f ohne Gegenbeispiel**, in allen 15 Fällen |
| 4 | Ordnung 20: alle f mit exakter Diskrepanz ≥ 6 (alle 2^19 x durchprobiert) | Z2xZ10: 520 Funktionen; D10, Dic5, F20: je 200 Funktionen, je eine Bahn unter Verschiebungen und Komplement. Passt zu 2 / 1 / 1 / 1 Klassen der Sitzung |
| 5 | Zwei Spalten gestrichen, ohne Symmetrieabbau: Ordnung 18 | kein f mit Diskrepanz ≥ 6, also nichts zu streichen (wie Sitzung) |
| 6 | Zwei Spalten gestrichen: Ordnung 20, Obermenge L aller f mit Diskrepanz ≥ 6, alle 190 Spaltenpaare | alle 138 700 Matrizen mit Gegenbeispiel unter 8 |
| 7 | Zwei Spalten gestrichen: Z7:Z3, Obermenge von „Diskrepanz ≥ 7“ (2582 Funktionen), alle 210 Paare | alle 542 220 Matrizen mit Gegenbeispiel unter 9. Geht über die Sitzung hinaus (dort nur die Schranke u_max + 2) |
| 8 | Zwei Spalten gestrichen: Z5xZ5 | **nur Stichprobe:** die vier Vertreter der Sitzung mit u = 7, alle 300 Paare, alle mit Gegenbeispiel unter 9. Die Auswahl der Klassen (u ≥ 7) ruht auf dem Programm der Sitzung allein |
| 9 | Teil 4 (Kern ⟨1,a,b⟩): 97 Typen, B(D) 31 180 bis 53 372 | Sitzung bestätigt die Zahlen des Vortests; damit zwei unabhängige Skripte |
| 10 | Prüfdateien | `sha256sum -c verifiers/SHA256SUMS` auf dem Branch OK; gegenüber `main` nur `STATUS.md` und `LANDKARTE.md` geändert, sonst nur neue Dateien; Zeile 3 beider Dateien unberührt |

## Berichtigungen und Vorbehalte

- Protokoll, Teil 2: „jedes Mal der erste“ stimmt bei Z5xZ5 nicht. Dort wurden zwei Vertreter exakt geprüft, der zweite
  (`8d2f`) hat 7 (`runs/gruppen/blocklog.txt`). Im Protokoll berichtigt.
- Z5xZ5: Die u-Verteilung nennt vier Klassen mit u = 7. Für zwei davon (`8459`, `108a8f`) findet das Gate ein x mit
  Maximum 5; ihre Diskrepanz ist also höchstens 5. u ist dort zu groß, was erlaubt ist (obere Schranke). Folgenlos.
- Belegart bleibt „Gegenbeispiel je Klasse, Bester exakt“. Durch das Gate gilt für die vollen Matrizen zusätzlich
  „Gegenbeispiel je Funktion, ohne Symmetrieabbau“.
- Die Zeitangaben im Protokoll der Sitzung sind nach deren eigener Aussage teils nachträglich angeglichen und nicht
  geprüft. Für das Ergebnis ohne Belang.
- Nicht gerechnet bleibt: Ordnung 24, vier gestrichene Spalten bei Ordnung 25, Rand bei Ordnung 20 und 25.

## Einordnung

Gruppenmatrizen über nichtzyklischen Gruppen erreichen nirgends mehr als die zirkulanten Matrizen gleicher Ordnung
(bester Quotient 1,492 bei Z7:Z3 mit Rand, N = 22). Kandidat 3 der Mittags-Übergabe ist damit erledigt.
