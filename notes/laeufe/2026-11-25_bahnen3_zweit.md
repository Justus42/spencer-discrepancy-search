# Auftrag B2: zweite Rechnung zu den 48ern unter der 3-Sylowgruppe (Cloud, 4 Kerne)

Auftrag: `notes/gegenrechnung_bahnen3_2026-11-25_auftrag.md` (B2), freigegeben durch den Startprompt am 25.11.2026.
Eigene Programme, nicht gelesen: `src/bahnen3_*`, `src/bahnen_zaehl.c`, `notes/orchestrator/`; Ergebnisdateien unter
`runs/bahnen3/` erst nach eigenem Ergebnis geöffnet. Logs `runs/bahnen3_zweit/`. Branch `ccr-a8e1e233-3psa2j`.
Beginn 25.11.2026, 21:55 UTC (date).

## Programme

- `src/b2_aequiv.c` (Prüfcode): Code → Graph (Ecken r±, c±; Kanten r+–r−, c+–c−, r^s–c^t bei M·s·t = +1; Zeilen- und
  Spaltenecken verschieden gefärbt). Eigene Farbverfeinerung (Signatur = Hash der Nachbarfarben, nur farbabhängig, also
  invariant), Individualisieren der ersten kleinsten Zelle, Spurvergleich, kanonisches Blatt = Maximum von (Spur, Zertifikat).
  |Aut| = Zahl der Blätter mit bestem Zertifikat. Modi `aut` (alle Automorphismen als Spaltenabbildung) und `klassen`.
- `src/b2_teil0.py` (Prüfcode): prüft jedes Element einzeln nach, Abgeschlossenheit, Ordnungen, P, Bahnen (numpy), Radius.

## Teil 0: Kontrollzahlen (had24_1p3_paley12)

Log `runs/bahnen3_zweit/teil0.log.txt`, Elemente `G_elemente_1p3.txt`, `P_elemente.txt`. Laufzeit 0,1 s (C) + 2 s (Python).

| Angabe | erwartet | eigen |
|---|---|---|
| Ordnung von G | 576 | 576 (3 497 Suchknoten; jedes Element nachgeprüft, alle 576² Produkte in G) |
| Elementordnungen | 1:1, 2:87, 3:8, 4:216, 6:120, 8:144 | 1:1, 2:87, 3:8, 4:216, 6:120, 8:144 |
| Untergruppe der Ordnung 9 | genau eine, nicht zyklisch | genau eine (keine Elemente der Ordnung 9, {1} ∪ 8 Elemente der Ordnung 3 abgeschlossen), nicht zyklisch, Normalteiler |
| Bahnen auf 2^19 Paaren | 8 / 160 / 58 200 | 8 / 160 / 58 200, zusammen 58 368 |
| 48er | 8 Dreierbahnen, Radius 6 | 24 Zeilenpaare = 8 volle Dreierbahnen; Radius 6 über alle 2^20 Punkte |

had24_2p2_paley12: ebenfalls |Aut| = 576 (1 961 Knoten). **Teil 0: keine Abweichung.**

## Teil 1: zweite Zählung (Zwischenstand)

Programm `src/b2_zaehl.c` (Prüfcode, C/OpenMP, ganze Zahlen). Bahnen selbst berechnet (C, getrennt von Python), Überdeckung
symmetrisch (0 Abweichungen), 48er = Bahnen 8, 39, 79, 96, 109, 130, 142, 161 (je Größe 3), überdeckt alles (`teil1_info.log.txt`).
Verfahren: binäres Verzweigen über kleine Bahnen (drin/nicht drin), Zählschranke Σ m_u · min w_c/cov_m(c) (Festkomma 2^32, abgerundet),
Zustand über D = 1792 schwerste Punktbahnen, bei überdecktem D Vollprüfung über alle 58 368 und Weiterverzweigen. Einzelheiten im Kopf
der Datei und unten (Vollständigkeit).

Erster Test (a) mit Gewicht ≤ 18: keine, 155 255 Knoten, 0,3 s (D = 512). Hochrechnung (D = 512): B = 20/21/22: 9 s / 40 s / 148 s;
mit D = 1792: B = 21 6,6 s, B = 22 16,7 s → B = 23 ca. 1–2 min, B = 24 ca. 5–10 min, (c)/(d) je Minuten. Weit unter 4 h.

| Lauf | Ergebnis | Knoten | Zeit (4 Kerne) | Log |
|---|---|---|---|---|
| (a) nur Größe 1/3, Gewicht ≤ 23 | **keine** | 4,70 · 10^7 | 47 s | `teil1a_B23.log.txt` |
| (b) nur Größe 1/3, Gewicht ≤ 24 | **19 008**, alle Gewicht 24, alle aus 8 Dreierbahnen, paarweise verschieden, 48er dabei | 3,65 · 10^8 (1,15 · 10^8 Vollprüfungen) | 350 s | `teil1b_B24.log.txt`, Liste `teil1b_B24_liste.txt` |
| (c) genau eine Neunerbahn, Gesamtgewicht ≤ 23 (kleine ≤ 14), D = 512 | **keine** | 4,84 · 10^8 + 5,20 · 10^8 | 667 s + 693 s (2 Blöcke) | `teil1c_neun1_B23_block{1,2}.log.txt` |
| (d) genau zwei Neunerbahnen, Gesamtgewicht ≤ 23 (Verfahren B) | **keine** | 9,37 · 10^6 Schritte (o1, T), 5,45 · 10^8 o2-Kandidaten, 0 volle Treffer | 28 s | `teil1d_neun2b_B23.log.txt` |

Verfahren (d), B: o1 über alle 58 200 Neunerbahnen, T über alle Dreier-Teilmengen mit 3|T| ≤ B − 18 (bei B = 23: ∅ und 160 einzelne),
Einserbahnen gebündelt: Q1 = U \ (cov T ∪ cov o1 ∪ cov aller 8 Einser) muss ganz in cov o2 liegen. Kandidaten o2 > o1 per UND der
Überdeckermengen von bis zu 32 über Q1 gestreuten Punkten (Überdeckung symmetrisch, `info`: 0 Abweichungen), dann volle Prüfung
Q1 ⊆ cov o2, dann exakt: alle Einsermengen O mit |O| ≤ B − 18 − 3|T|, die jeden Rest-Punkt treffen. Jede Menge {o1 < o2} ∪ T ∪ O genau einmal.

### Vollständigkeit

- Klein-Modus: Jede überdeckende Menge S mit Gewicht ≤ B ist mit der Wurzel verträglich (I ⊆ S, E ∩ S = ∅). In einem verträglichen
  Knoten ist S \ I ⊆ A (unentschieden, Gewicht passt) und überdeckt alle offenen D-Punkte; die Schranke ist eine untere Schranke für
  die Kosten jeder solchen Überdeckung (Kosten jeder Bahn auf die von ihr überdeckten offenen Punkte verteilt; jeder Punkt erhält
  mindestens das kleinste Verhältnis seiner Kandidaten; Festkomma abgerundet), also kein Abschneiden; ein offener D-Punkt hat einen
  Kandidaten in S \ I. Die Verzweigung „c drin / c nicht drin“ schickt S in genau ein Kind. Ist D überdeckt, aber nicht alles, hat
  der offene Punkt p einen Überdecker in S \ I, Verzweigung wie oben. Ist alles überdeckt, werden I ∪ T für alle unentschiedenen T
  mit passendem Gewicht gezählt, darunter S. Disjunkte Zweige → jede Menge genau einmal.
- (c): derselbe Motor je Neunerbahn auf U \ cov(o); (d): siehe oben, notwendige Bedingung Q1 ⊆ cov o2, dann exakt.
- Grenzen: Die Zählung ruht auf der Bahnberechnung und der Überdeckungsrelation in C; beide sind mit Teil 0 (Python) bei Bahngrößen
  und Lage des 48ers gegengeprüft; Lösungen mit Python über alle 2^20 Punkte nachgeprüft (Stichproben, s. u.).

### Kontrollen (alle Pflicht)

| Kontrolle | Ergebnis | Log |
|---|---|---|
| Klein-Modus gegen Durchprobieren, 11 Fälle (K = 8 … 300 zufällige Punktbahnen, B = 9 … 12, D teils kleiner als K → Vollprüfungspfad) | 11 von 11 gleich, bis 1,9 Mio. Lösungen je Fall (Obermengen eingeschlossen) | `teil1_test0.log.txt` |
| Ein-Neuner-Modus gegen Durchprobieren (alle 58 200 Neunerbahnen × alle kleinen Mengen), 8 Fälle, B = 11 … 15 | 8 von 8 gleich, bis 70 325 Lösungen | `teil1_test1.log.txt` |
| Zwei-Neuner-Modus (B) gegen Durchprobieren (alle Paare × alle kleinen Mengen), 8 Fälle, B = 18 … 22 (3 Fälle mit o1 < 200) | 8 von 8 gleich, bis 3,8 · 10^8 Lösungen, mit Dreierbahn in T | `teil1_test2.log.txt` |
| Positivkontrolle (b): 48er gezählt | ja (Bahnen 8, 39, 79, 96, 109, 130, 142, 161 in der Liste) | oben |
| Positivkontrolle mit Neunerbahn: Neunerbahn 168 (Index 0) vorgegeben, kleine Bahnen ≤ 21: keine; ≤ 22: **576 Lösungen** (Gewicht 31: 1 Neuner, 1 Einser, 7 Dreier) | alle 576 mit Python nachgeprüft (Radius 6, Gewicht stimmt) | `teil1_positiv_neun.log.txt`, Liste `teil1_positiv_neun_liste.txt` |
| Stichprobe 30 der 19 008 mit Python (Bahnen ausgebaut, Radius über 2^20) | 30 von 30 | `teil1_positiv_neun.log.txt` |

### Vergleich mit dem ersten Programm (erst nach eigenem Ergebnis geöffnet)

`runs/bahnen3/teil2_klein_B24_liste_4_56.txt` (Format: „0 24 : Vertreter/Größe …“), beide Listen als Mengen von 24 Paarklassen
(Vertreter mit P ausgebaut): **19 008 gemeinsam, 0 nur eigen, 0 nur erstes Programm** (`teil1_vergleich.log.txt`).

**Ergebnis Teil 1: (a) keine, (b) genau 19 008 (alle 8 Dreierbahnen, 48er dabei, identisch mit dem ersten Programm), (c) keine,
(d) keine.** Rechenzeit Teil 1 ca. 45 Min. (ohne Fehlversuche).
