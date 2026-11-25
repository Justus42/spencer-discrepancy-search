# Gemini Deep Research vom 25.11.2026: Kurzfassung und Prüfvermerk

Bericht: `notes/gemini/ideen_2026-11-25_deep-research.pdf` (19 Seiten, von Justus um 17:49 Uhr eingereicht). Prompt:
`notes/gemini/ideen_2026-11-25_deep-research_prompt.md`. Prüfung durch den Orchestrator, 17:49 bis 18:00 Uhr (Zeiten aus `date`).
Rechnungen: `notes/orchestrator/2026-11-25_gemini/` (einmal gerechnet, ein Programm, nicht gegengerechnet).
Regel 4: Was hier nicht als „geprüft“ steht, ist eine Aussage Geminis und unbelegt.

## Was der Bericht behauptet und was davon hält

| Aussage Geminis | Prüfung | Urteil |
|---|---|---|
| Reis und Song, arXiv 2609.34471 (28.09.2026): obere Schranke √(3·arsinh 10)·√n + 4 < 2,9992·√n + 4; erste asymptotische untere Schranke über 1 (Hadamard-Matrix, 2⁻²² der Spalten durch Zufallsspalten ersetzt, n ≥ 2⁵⁰); der Wert 5/√8 wird dort Reis' Dissertation von 2023 zugeschrieben | Volltext gelesen (alphaXiv) | **stimmt.** Der Rekord ist also älter als der Eintrag vom 05.09.2026; für uns ändert das die Schwelle nicht. Dort auch: Spielman vermutet D_n ≤ 2 |
| Die Tabellenwerte 64 bei (16,4) bis (25,8) und 128 bei (30,10) sind lineare Codes von Graham und Sloane 1985, per ADS fortgeschrieben; lineare Codes der Dimension 6 können dort nicht besser sein (Baicheva und Bouyukliev 2010); nichtlinear wurden diese Zellen nie angegriffen | nicht geprüft (Quellen nicht gelesen) | plausibel, unbelegt. Passt dazu, dass unsere 48 aus einer nichtlinearen Bauart sofort unter 64 lagen |
| Keine veröffentlichte obere Schranke unter 64 für (20,6) oder (25,8) gefunden; EinsteinArena-Thread 273 nennt weiter 64; Quantavane behauptet ein „Theorem D“ zu sechs binären Zellen ohne Angabe, welche | nicht geprüft | **für Donnerstag (Regel 3) vormerken:** Quantavane-Seiten, `openai/math`, EinsteinArena |
| Mein Satz im Prompt „k(d) = 2, 4, 7, 12, 16 für d = 1 bis 5 bekannt“ ist für d = 4, 5 falsch; gesichert nur d = 1, 2, 3; z. B. 11 ≤ K(12,4) ≤ 12, 15 ≤ K(11,3) ≤ 16 | nicht geprüft | **nehme ich an:** der Satz war meine unbelegte Behauptung. Für die Suche ohne Folge |
| „Zellen (13,3), (16,4), (18,5), (19,5), (21,6) aus der Zielliste gestrichen“ | `STATUS.md` | **stimmt nicht:** Sie stehen in der Tabelle der offenen Zellen; mein Prompt nannte nur die obersten Zellen |
| Fund 1: 48er ist normal, also K(20+2i, 6+i) ≤ 48; für 40 fehlen vier Paare; Test: Zeile streichen und mit WalkSAT plus Tabu reparieren | bekannt (STATUS); Test siehe unten | Vorschlag übernommen in Auftrag E |
| Fund 2: Bei n = 16 liegt `ctrl_16` mit Nullspalte nur 0,0711 unter der Schwelle; reelle Einträge beseitigen die Paritätslücke | siehe unten | **trägt nicht.** Die 0,0711 sind eine Stufe, kein kleiner Abstand |
| Fund 3: Hadamard-Matrizen mit ersetzten statt gestrichenen Spalten testen (nach Reis und Song); antipodal verschärfte Delsarte- oder SDP-Schranken für (13,3) und (15,4) | Stichprobe siehe unten; Schranken nicht gerechnet | Spaltenersetzung negativ in der Stichprobe; die Schranken wären Aufräumarbeit, kein Rekordweg |
| Methoden: Marosi (arXiv 2608.19872, q ≥ 5): WalkSAT-Suche mit Streichen und Reparieren, Senkungen um 15 bis 25,7 %; van Staden (Okt. 2026): Tabu schlägt Simulated Annealing klar, bei stark überlappenden Überdeckungen war Tabu ohne Gewichte besser als mit; ab (25,8) Grafikkarte und Tausch ganzer Zeilen | nicht geprüft | übernommen als Bauhinweise für Auftrag E (Gewichte an- und abschaltbar vergleichen) |
| Zur Zerlegung (U, W) nichts in der Literatur; verwandt: blockweise direkte Summe, subnormale Codes (Honkala 1991, Östergård 1992) | nicht geprüft | Auftrag H bleibt Neuland |
| Wettbewerb: EinsteinArena bestes Ergebnis 7/√17 = 1,698; dort wird an (15,4), (18,5), (20,6) gearbeitet, bei (15,4) hängt man an denselben 466 Punkten; Y. H. vom 05.09. vermutlich Yue Huang (Épi-Agent) | nicht geprüft | Zeitdruck für die Meldung K(20,6) ≤ 48, kein Grund für Eile bei der Rekordsuche |

## Die drei „Minuten-Tests“ Geminis, gerechnet

1. **Streichen und Reparieren ab dem 48er, (20,6).** Start: 48er ohne Zeile 0, 23 Zeilen, 1297 offene Klassen.
   - Exakt (`nachbarschaft20.py`): Kein Wechsel von einem oder zwei Einträgen einer Zeile senkt die 1297. Der Start ist ein lokales Minimum.
   - Mein Schnellprogramm `tern_ls` (je 240 s, mit und ohne Gewichte, etwa 24 000 Schritte): bleibt bei 1297. Das Programm hat schon die
     Kontrolle 16 x 15 nicht bestanden; **der Test ist damit nicht entschieden**, er gehört in Auftrag E (Insel „Saat“) mit einem richtigen Suchprogramm.
2. **Reelle Umgebung von `ctrl_16` bei n = 16** (`gemini_tests.py`, Teil 1). Mit Schwelle 7 fasst eine Zeilenkappe bis zu 2·V(15,4) = 3882 Punkte,
   mit Schwelle über 7,0711 nur noch V(16,4) = 2517 (Durchmesser 8 statt 9, Satz von Kleitman). 32 Kappen geben Dichte 1,229, und für Kappen, die in
   Kugeln liegen, ist das die geschlossene Zelle (16,4) mit 32 Wörtern (K(16,4) ≥ 34). Zusätzlich: Jede Zeile von `ctrl_16` hat 233 private
   Punkte, darunter 924 Paare im Abstand 8; mit der Zuordnung „dominierende Zeile“ landen daraus Punkte im Abstand 9 in derselben Zeile. Das von
   Gemini beschriebene lineare Programm mit fester Zuordnung kommt also nicht über 7,07. Dazu passt der Befund vom 24.11. (Zeilen im L1-Abstand ≤ 2:
   nichts über 7). Belegart: Herleitung plus Zählung; kein vollständiger Ausschluss reeller 16 x 16-Matrizen.
3. **Spaltenersetzung bei Hadamard 20** (Teil 2): drei Klassen, 1, 2 oder 3 Spalten durch zufällige ±1-Spalten ersetzt, je 1000 Stichproben.
   In allen 9000 Fällen gibt es unter 20 000 Zufallspunkten mindestens 85 mit allen Beträgen ≤ 6. Kein Fall auch nur in der Nähe von 8.
   Belegart: Stichprobe, schließt nichts aus. Bei Ordnung 16 erübrigt sich der Test (Zelle (16,4) mit 32 Wörtern ist geschlossen).

## Nebenbefund für Auftrag H

`ctrl_17` und `L2026_17x17` (32 Wörter, (17,5), d = 7) haben einen Verschiebungskern der Größe 4 mit Gewichten 8 und 9. Sie sind also Codes der
zerlegten Familie mit K = 8 und Typ (9,8) und taugen als zweite Positivkontrolle für den Halbproblem-Löser. `ctrl_16` und `astra_19` haben nur
den Kern {0, 1}; Geminis Bemerkung, die nicht-Sylvestersche Hadamard-16 habe dasselbe Muster wie die Paley-Doppelung, trifft auf `ctrl_16` nicht zu.

## Folgen

- Rang 1 bleibt Auftrag E und wird durch den Bericht gestützt: Die Zielzellen wurden laut Gemini nie nichtlinear angegriffen, und in den
  Nachbartabellen haben genau solche Suchen (Streichen und Reparieren, Tabu) zuletzt Rekorde geliefert. Belegt ist davon nur unser eigener 48er.
- Rang 2 bleibt Auftrag H, ohne Vorbild in der Literatur.
- Geminis zwei „Flanken“: die reelle bei n = 16 entfällt; die andere ist Auftrag E.
- Nächste Stufe, erst nach bestandenen Kontrollen von E: (25,8) mit 50 Wörtern auf dem PC mit Grafikkarte (dort ist die 64 ebenfalls linear geerbt).
