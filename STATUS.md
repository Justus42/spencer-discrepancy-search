# STATUS

Stand: 23.11.2026. Timebox: 23.11.2026 bis 04.01.2027, danach Entscheidung über Abbruch.

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

## Ziele

| Länge m | Radius R | Wörter höchstens | ergibt | Dichte | Tabelle K(m,R) |
|---|---|---|---|---|---|
| 25 | 8 | 50 | 9/5 = 1,8000 | 2,69 | 25–64 |
| 23 | 7 | 50 | 9/5 = 1,8000 | 2,33 | 29–64 |
| 20 | 6 | 40 | 8/sqrt(20) = 1,7889 | 2,31 | 24–64 |
| 18 | 5 | 40 | 8/sqrt(20) = 1,7889 | 1,93 | 27–64 |
| 30 | 10 | 62 | 10/sqrt(31) = 1,7961 | 3,06 | 27–128 |

Dichte = Wörter x Kugelvolumen / 2^m. Zum Vergleich bekannte Codes: (15,4)/32 hat 1,90,
(17,5)/32 hat 2,30, (19,6)/32 hat 2,67. Gestrichen: (15,4) mit 30 Wörtern (Dichte 1,78,
Suche bewegt sich nicht). Reihenfolge der Versuche: 25/8 zuerst, dann 20/6.

## Gesichert (beide Prüfskripte stimmen exakt überein)

| Datei in `controls/` | Größe | Diskrepanz | Herkunft |
|---|---|---|---|
| `ctrl_8.txt` | 8x8 | 5 | eigene Suche, entspricht dem Rekord |
| `ctrl_9.txt` | 9x9 | 5 | eigene Suche |
| `ctrl_16.txt` | 16x16 | 7 | eigene Suche, Code (15,4) mit 32 Wörtern |
| `ctrl_17.txt` | 17x17 | 7 | eigene Suche, Code (17,5) mit 32 Wörtern |
| `L2026_17x17.txt` | 17x17 | 7 | veröffentlichtes Zertifikat aus dem Repo |
| `astra_19.txt` | 19x19 | 7 | Konstruktion `src/construct_32.py` |

`src/construct_32.py` liefert selbstkomplementäre 32-Wörter-Codes für jede ungerade Länge
n = 2R+7 >= 15. Per Brute-Force bestätigt für n = 15, 17, 19, 21, 23. Die Konstruktion
stammt aus einer Abfrage an GPT-6.2 Astra; die historische Zuordnung ist unbelegt.

## Messwerte der Suche (`src/search_lns.py`, wenige Minuten, 2 CPU-Kerne)

- Findet die Kontrollen bis Länge 17 in unter einer Sekunde.
- Länge 19, Radius 6: braucht 46 Wörter, bekannt sind 32. Die Suche ist ab hier schwächer
  als bekannte Konstruktionen.
- Bis zur vollständigen Überdeckung: (18,5) 80 Wörter, (20,6) 72, (23,7) 84.
- Mit fester Wortzahl bleiben unüberdeckt: (20,6)/40 0,76 %, (23,7)/50 0,07 %,
  (25,8)/50 0,10 %, (18,5)/40 2,2 %. Kleine Restquoten sagen wenig: die letzten Punkte
  sind die teuren.

## Fehlversuche

- Naives Simulated Annealing mit Bit-Flips: reproduziert Länge 17 nicht.
- Gieriges Verfahren plus Zerstören/Reparieren: verbessert den gierigen Start kaum.
- Zufällige lineare [n,5]-Codes mit Einsvektor: Radius zu groß.
- (15,4) mit 30 Wörtern: bleibt bei 466 unüberdeckten Punkten stehen.

## Vorschläge aus der Astra-Abfrage (unbelegt, noch nicht getestet)

Volltext bis 23.12.2026 über die Bridge abrufbar (`batch_job`, Kennung
`openrouter:batch-1795445676-GZZW2hUqsWh8LXiEFgzr`). Er liegt noch nicht im Repo, weil die
Bridge dieses Repo nicht in ihrer Freigabeliste hat. Subjektive Schätzung dort: 30 %, dass
mindestens eines der Ziele existiert.

- **20/6:** fünf Nebenklassen eines dreidimensionalen linearen Codes D = <1, a, b>.
  D bis auf Spaltenpermutation aufzählen (vier Vielfachheiten der Spaltentypen, Summe 20).
  Dann 2^17 Syndrome mit fünf Verschiebungen einer einzigen Menge überdecken, Tabu-Suche
  mit Bewertung aller Ersetzungen per Walsh-Transformation.
- **25/8:** Symmetrie vorgeben: Permutation vom Zykeltyp 5^5 plus Komplementbildung, fünf
  volle Bahnen zu je zehn Wörtern. Am Ende die Symmetrie lösen und einzelne Paare tauschen.

## Offen und unbelegt

- Die Code-Tabelle (Kéri) ist von 2009. Neuere Schranken für die Zielzellen wurden nicht
  gefunden, sind aber nicht ausgeschlossen. Literaturprompt für Gemini steht aus.
- Von den 41 offenen Pull Requests im Zielrepo war nur die erste Seite einsehbar (25).
- Zeilenweise Nullen und gebrochene Einträge erhöhen das Budget pro Zeile nicht, außer in
  Paritätsfällen (n = 17, 19). Als Endspiel-Politur per LP bleiben sie möglich.

## Nächste Gates

1. **Lokal einrichten.** Python und GPU-Bibliothek installieren, Grafikkarte testen, alle
   Dateien in `controls/` mit beiden Prüfskripten nachrechnen.
2. **Suche auf Niveau bringen.** `src/search_lns.py` auf die GPU portieren und so
   verbessern, dass sie bei Länge 19 mit 32 Wörtern auskommt, ohne die Konstruktion zu
   kennen, oder mit der Konstruktion als Startpunkt deren Nachbarschaft durchsucht.
3. **Strukturierte Suche** nach den beiden Vorschlägen oben, erst 25/8, dann 20/6.
4. **Bei einem Treffer:** beide Prüfskripte, dann Neuheit prüfen, dann Entscheidung über
   einen Pull Request. Nichts vorher nach außen melden.

## Dateien

- `src/check_disc.py`: Prüfskript 1 (Claude), exakte Diskrepanz per Brute-Force.
- `verifiers/verify_spencer.py`: Prüfskript 2 (Gemini), unabhängig geschrieben.
- `src/search_lns.py`, `src/greedy_full.py`: Suche auf dem Quotientenwürfel.
- `src/construct_32.py`: Konstruktion der 32-Wörter-Codes.
