# Gate zu Auftrag V (Verdopplungszellen), 26.11.2026

Orchestrator-Gate. Sitzungsbranch `claude/auftrag-v-q1vany` (Commit `74644f4`, enthielt `main` = `fe524e4`; im Gate `main` = `357e021` eingespielt,
Konflikt in `STATUS.md` durch Behalten beider Seiten gelöst). Auftrag `notes/verdopplung_2026-11-26_auftrag.md`, Protokoll
`notes/laeufe/2026-11-26_verdopplung.md`. Beginn 19:44 UTC, Ende 19:47 UTC (Containeruhr; 20:44 bis 20:47 Uhr MEZ). Eigene Skripte und Logs:
`notes/orchestrator/2026-11-26_gate_verdopplung/` (`gate_v.py`, `gate_v_bauregel.py`; eigener Zähler, weder Code der Sitzung noch der Bewerter).

## Urteil: bestanden

Alle Zahlen der Sitzung stimmen. Kein Treffer, kein Rekordkandidat, kein neuer Wert. Belegart „Suche erfolglos“, nichts ist ausgeschlossen.
Anders als bei Auftrag F hat die Suche hier ihre Positivkontrollen bestanden. Die Aussage ist trotzdem schwach, aus einem anderen Grund (Punkt 1).

## Was ich selbst geprüft habe

| Aussage der Sitzung | eigene Prüfung | Ergebnis |
|---|---|---|
| Prüfdateien, Bewerter, `p018.c` unberührt | `git diff` gegen `main`; `sha256sum -c` | kein Unterschied; 4 von 4 OK |
| Restzahlen der 80 gespeicherten Ausgaben | eigener Zähler über alle 2^(m−1) Klassen (`gate_v.py`) | 80 von 80 gleich dem Archiv und der Nachzählung der Sitzung |
| Kontrollen: 16 Paare bei (19,6), (17,5) je 4 von 4; 24 Paare bei (22,7) 4 von 4, bei (24,8) 1 von 4 | eigener Zähler; `pruefe_matrix.py` an je einer gelösten Matrix (`regel1.log.txt`) | Zahlen gleich; dreimal Diskrepanz 7, 7, 8, 8 wie erwartet |
| Bauregel-Matrizen 24 × 22 und 24 × 24: 0 offen | eigener Zähler | 0 und 0 |
| (19,6), (17,5): alle Startwerte enden beim selben Wert | Vergleich der Ausgaben | gleiche Zahl, aber verschiedene Matrizen (4 bzw. 8 verschiedene Zeilenmengen je Stufe) |
| Regel 4 | Zeitstempel der Commits | Abstände höchstens 25 Minuten mit Zwischenständen; 600-s-Läufe als Hintergrundjob (Werkzeuggrenze), wie bei Auftrag M angenommen |

## Widerspruch und Einschränkungen

1. **Bei (19,6) und (17,5) findet die Suche nur die Bauregel.** Vollständig gerechnet (`gate_v_bauregel.py`): Streicht man aus den gelösten 32ern
   der Kontrolle eine bzw. zwei Zeilen, bleiben bestenfalls genau 2236 bzw. 5312 offene Klassen bei (19,6) und 722 bzw. 1710 bei (17,5). Das sind
   exakt die Endwerte der Leiter bei 15 und 14 Paaren. Die Suche ist dort also nicht besser als Streichen. Der erste Test nach Projektregel 8
   (besser als die Bauregel) ist in diesen beiden Zellen nicht bestanden; die Sitzung hat den Vergleich nicht gezogen, mein Auftrag hat ihn für
   diese Zellen nicht verlangt.
2. **Bei (22,7) und (24,8) ist die Suche besser als Streichen, bleibt aber auf einem Plateau.** Ohne eine Zeile hat der beste gelöste 48er 2073 bzw.
   5478 offene Klassen, die Suche erreicht mit 23 Paaren 616 bzw. 2400. Mit 22 Paaren wird es kaum schlechter (648, 2064). Die Sitzung vergleicht das
   mit dem 704er-Plateau aus Auftrag E; das ist plausibel, aber nicht geprüft.
3. **Die Ziele liegen weit weg.** Bei 10 bzw. 12 Paaren bleiben 9 bis 14 % aller Klassen offen. Für die Verdopplung sagt der Lauf nur: Dieser Sucher
   kommt nicht in die Nähe.
4. **Meine Erwartung aus dem Gate zu F war falsch.** Ich hatte vermutet, p018 scheitere bei (22,7) und (24,8) schon an der Kontrolle. Es findet die
   bekannten 48er in 4 von 4 und 1 von 4 Läufen. Der Satz in `STATUS.md` ist berichtigt: Unbrauchbar ist p018 bei (25,8) und (23,7) mit 32 Paaren,
   nicht allgemein ab m = 23.
5. **„Tabellenwerte“ war ungenau.** 32 Wörter bei (19,6) und (17,5) ist der Tabellenwert. 48 Wörter bei (22,7) und (24,8) sind eigene Werte; die Tabelle nennt dort 64.
6. **Nicht nachgezählt** (so auch im Bericht): kürzere Läufe, deren Ausgaben von gleichnamigen längeren überschrieben wurden. Sie stehen nur im Archiv.

## Eingetragen

`STATUS.md`: Stand-Zeile; beim Negativbefund der Vergleich mit der Bauregel und die Berichtigung zu „Tabellenwerte“; Auftrag V als abgeschlossen;
der berichtigte Satz zu p018 aus dem Gate zu F. `LANDKARTE.md`: Stand-Zeile und dieselbe Berichtigung. Die Einträge der Sitzung sind sonst unverändert.
