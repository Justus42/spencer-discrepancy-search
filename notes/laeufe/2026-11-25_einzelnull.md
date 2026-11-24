# Lauf 25.11.2026: Zeilen mit einer Null, n = 11, d = 6 (Auftrag A)

Cloud-Sitzung, 4 Kerne, Branch `ccr-6615c26c-zvmbps`. Rohlogs: `runs/einzelnull/`.
Frage: Gibt es eine 11 x 11-Matrix mit Einträgen in {-1, 0, 1} und Diskrepanz >= 6 (Quotient 1,809 > 1,7678)?

## Start

- 24.11.2026 21:26 UTC: `sha256sum -c verifiers/SHA256SUMS` alle drei OK. `build/exakt_disc` gebaut.

## Teil 1: Gegenrechnung (Prüfcode `src/einzelnull_zaehl.py`, `src/einzelnull_zirk.py`), alle Angaben bestätigt

| Angabe | eigenes Ergebnis | Log |
|---|---|---|
| 1024 Klassen; Träger 10: 5632 Zeilen je 112 | 1024; 5632 je 112 | `runs/einzelnull/teil1a.log.txt` |
| Schnitte gleiche Spalte, Abstand 1–5: 40, 40, 12, 12, 0 (10, 45, 120, 210, 126) | gleich | ebd. |
| Schnitte andere Spalte, Abstand 0–4: 76, 40, 26, 12, 6 (20, 180, 720, 1680, 2520) | gleich | ebd. |
| Budget 11·112 − 1024 = 208 | 208 | ebd. |
| Träger 8: 21120 je 72; Träger 6: 14784 je 32 | gleich | ebd. |
| ungerade Träger dominiert | Träger 11 (67 Kl.), 9 (40), 7 (16): Treffermenge liegt in der jeder Zeile mit einem Eintrag weniger, alle Fälle geprüft. Grund: Summe ungerade, also \|s\| ≥ 7, ein Eintrag weg ändert um 1. Träger 5: 0 Klassen. | ebd. |
| Zirkulante Nulldiagonale n = 11, 14, 17: 56/700/2068 Klassen, Maximum 4/5/6 | gleich (Klassen: Multiplikatoren und Negation); Bestwerte mit `exakt_disc` bestätigt | `runs/einzelnull/teil1c.log.txt` |

Folgerung: Es genügen Zeilen mit Träger 10, 8, 6. Mindestens 6 Zeilen mit Träger 10 (40·k10 ≥ 232), höchstens 5 mit Träger 8, 2 mit Träger 6.

## Teil 2: Positivkontrolle n = 8, d = 5 (Suchcode `src/einzelnull_such.c`, Gegenrechnung `src/einzelnull_kontrolle8.py`)

- 512 Zeilen mit einer Null, je 16 Klassen von 128, Budget 0 (exakte Überdeckung).
- 8 Zeilen: **57 600** Lösungen (Suchcode, 0,4 s), davon **55 680 mit verteilten Nullen**, 1920 mit allen Nullen in einer
  Spalte (= 8 Spalten × 240 perfekte Codes der Länge 7). Unabhängiger Algorithmus X in Python: dieselbe Menge (57 600, gleich).
- 7 Zeilen: keine (Zählargument 7·16 < 128, Suchcode meldet Budget negativ).
- Symmetriepfad (`-y`, erste Zeile fest, zweite Bahnvertreter, Paartyp-Maximum): Typ (andere Spalte, 3) 12, Typ (gleiche Spalte, 3) 36,
  sonst 0. Python aus der vollen Liste mit derselben Rangordnung: 12 und 36. Logs `teil2*.log.txt`.
- Beispiel mit verteilten Nullen `runs/einzelnull/n8_verteilt.txt`: alle 3 Prüfprogramme disc 5 (Quotient 1,768 = Rekord, keine
  Verbesserung, aber andere Struktur als Hamming plus Nullspalte). Regel 2: `ctrl_8`, `ctrl_9` reproduziert (je 5).

## Teil 3: Verfahren (Suchcode `src/einzelnull_such.c`, Treiber `src/run_einzelnull.sh`)

- Verzweigung am unüberdeckten Punkt mit den wenigsten lebenden Zeilen, Geschwister-Ausschluss.
- Verlust einer Zeile = (112 − |H|) + Überlappung mit dem Überdeckten; Summe ≤ 208. Schranke 1: Summe der r kleinsten
  aktuellen Verluste. Schranke 2 (`-b 4`): Punkte ohne gemeinsame lebende Zeile brauchen verschiedene Zeilen, ihre
  Mindestverluste addieren sich (Faktor 3,5 schneller bei Typ 5, gleiche Ergebnisse in allen Kontrollen).
- Symmetrie (`-y`): Paartyp = (gleiche Nullspalte?, Abstand modulo Komplement), 10 Typen nach Schnitt geordnet. Für jeden
  Typ t: a = (0,1,…,1), zweite Zeile = Vertreter, alle Paare von Träger-10-Zeilen haben Typ ≤ t (t ist das Maximum der Lösung).
- Dritte Zeile (`-3`): Bahnvertreter unter dem Stabilisator von {a, Vertreter} (Elemente per Aufzählung, jedes geprüft;
  Teilmenge der Gruppe gibt feinere Bahnen, bleibt vollständig). Kanonisch: dritte Zeile = Zeile mit größtem Profil
  (sortiertes Paar ihrer Typen zu a und Vertreter), alle weiteren Träger-10-Zeilen haben Profil ≤ diesem.
- Zulässig, weil jede Lösung mindestens 6 Zeilen mit Träger 10 hat (Programm prüft: 112 + 10·72 < 1024).
- Kontrollen derselben Programmfassung: n = 8, d = 5, K = 8 findet Lösungen (Typ 0: 2, Typ 1: 6); K = 7 keine.
  Mit Überlappung (Budget > 0), mit und ohne Symmetrie (`runs/einzelnull/kontrolle_klein.log.txt`, `kontrolle10.log.txt`):

  | n, d, K | Budget | ohne Symmetrie | mit `-y -3` |
  |---|---|---|---|
  | 8, 3, 3 | 46 | 0 (vollständig) | 0 |
  | 8, 3, 4 | 104 | 487 424 | 11 Vertreter |
  | 9, 4, 4 | 40 | 0 | 0 |
  | 9, 4, 5 | 114 | 0 | 0 |
  | 10, 5, 6 | 40 | 0 | 0 |
  | 10, 5, 8 | 224 | 1072 gefunden (Zeitgrenze) | 48 Vertreter |

  Beispiel n = 10, K = 8 (`kontrolle10_K8_beispiel.txt`): alle 3 Prüfprogramme disc 5.
- Aufwand ohne `-3` (nur Typ 5: 231 410 Aufgaben, 400 in 21 s) wäre ~3–4 h je Typ gewesen; mit `-3` Typ 5 Teilmessung: 30 von 40 Bahnen in ~2 Min.

## Teil 3: Blöcke Träger 10

- Block 1 (22:02–22:27 UTC, 25 Min.): Typen 0–7 vollständig, 0 Lösungen (Typ 5: 3,42 Mio. Aufgaben, 219 s; Typ 6: 156 s;
  Typ 7: 204 s). Typ 8: 1 081 537 von 1 179 160 Aufgaben, Abbruch Zeit. Max. Tiefe 7. Log `runs/einzelnull/block_10.log.txt`.
- Block 2 (22:28–22:38 UTC): Typ 8 fertig (2 624 711 Aufgaben, davon 1 543 174 in Block 2), Typ 9 fertig (889 615 Aufgaben, 208 s).
- **Ergebnis Träger 10: alle 10 Typen vollständig, 0 Lösungen.** Belegart: vollständig durchgerechnet (Suchcode mit
  Symmetriereduktion, Kontrollen oben). Rechenzeit ca. 36 Min. auf 4 Kernen. Erledigte Aufgaben je Typ: `erl_10_zaehlung.txt`.
  (Hinweis: In Block 1 zeigte Typ 8 „1 179 160 gesamt“, weil nach dem Abbruch nicht alle Bahnen gezählt wurden.)
- Fortsetzungsdateien `erl_*.txt` ab jetzt nicht mehr im Repo (77 MB in Commit 2faa929 schon enthalten).

## Teil 3: Blöcke gemischt (Träger 10, 8, 6)

- Kontrolle gemischt n = 10, d = 5 (Träger 9, 8, 6): K = 7 ohne Symmetrie nichts gefunden (Zeitgrenze 150 s), mit `-y -3`
  vollständig 0; K = 8 beide finden Lösungen (beide nur mit Träger 9). Eine Kontrolle, die Träger 8/6 erzwingt, fehlt.
  `runs/einzelnull/kontrolle_gemischt.log.txt`.
- Block 3 (22:43–23:03 UTC): Typ 0 fertig (0 Aufgaben). Typen 1–9 abgebrochen: mehr als 2^20 Aufgaben je Bahn (Programm
  beendet sich absichtlich, Nummernraum). Nichts als fertig gewertet; Teilfortschritt verworfen. Nummernraum auf 2^24 je
  Bahn erweitert, n = 8-Kontrolle danach unverändert (8 Lösungen).
- Block 4 (23:04–23:29 UTC): Typ 1 nicht fertig: 13 585 003 von 13 598 179 gezählten Aufgaben (gezählt nur bis zum Abbruch,
  5 bis 6 von 7 Bahnen), 0 Lösungen. Knoten: Tiefe 5 13,6 Mio., Tiefe 6 2,6 Mio., max. Tiefe 6.
- **Abbruch der gemischten Fassung** nach ca. 45 Min. Rechenzeit: Die 21 120 Zeilen mit Träger 8 stehen in jeder Knotenliste;
  schon Typ 1 (bei Träger 10 in 0,1 s fertig) braucht > 25 Min., Typen 5–9 wären um Größenordnungen teurer.
  **Ergebnis gemischt: Suche erfolglos (nicht vollständig).** Vollständig ist nur Typ 0.
- Idee für später: zuerst die höchstens 5 Zeilen mit Träger 8/6 als Bahnvertreter festlegen, dann nur Träger 10 mit Restbudget.

## Teil 4 (neu nach Nachtrag 24.11., 23:10): P1 und Leiter bei (13,3)

- **P1 nachgerechnet** (Prüfcode `src/p1_pruef14.c`, 58 s): alle 57 344 Zeilen mit einer Null (n = 14, d = 7), beide
  Paritätshälften, alle 14 Streichkoordinaten = 1 605 632 Fälle: je Zeile und Hälfte 378 Klassen, Treffermenge immer genau
  B(c,3) ∪ B(~c,3) im 13-Würfel (c = Zeile ohne Streichkoordinate, Null durch ±1 ersetzt), **0 Abweichungen**.
  Log `runs/einzelnull/p1_pruef14.log.txt`.
- Leiter (Suchcode `src/sc13_tabu.c`, Treiber `src/run_sc13.sh`, Prüfcode `src/sc_pruef.py`), Zufallsstart, je Stufe
  4 Startwerte parallel, je höchstens 600 s; 23:30–23:54 UTC. Log `runs/sc13/leiter.log.txt`.

  | Wörter | gelöst | Zeit bis Lösung bzw. Restzahl |
  |---|---|---|
  | 48 (Kontrolle) | 4 von 4 | 0,0–0,7 s |
  | 44 (Kontrolle) | 4 von 4 | 2–244 s |
  | 42 | 1 von 4 | Startwert 4 nach 126 s; sonst Rest 76–144 (bester 8) |
  | 40 | 0 von 4 | Rest 160–196 (bester 16); **Stopp** |

- Alle 9 gefundenen Codes mit `sc_pruef.py`: komplement-abgeschlossen, Radius 3. `sha256sum -c verifiers/SHA256SUMS`: alle OK
  (`runs/sc13/sha256.log.txt`). 42er als 21 x 13-Matrix (`runs/sc13/w42_s4_matrix.txt`): alle 3 Prüfprogramme disc 7
  (Quotient 7/√21 = 1,528, kein Rekord, gleich der Tabellenschranke K(13,3) ≤ 42). 30/28 Wörter nicht versucht (Stopp bei 40).

## Ergebnis

- Teil 1: alle Angaben bestätigt. Teil 2: Kontrolle bestanden; bei n = 8 haben 55 680 von 57 600 Lösungen verteilte Nullen.
- Teil 3, Träger 10: **Es gibt keine 11 x 11-Matrix, deren Zeilen je genau eine Null und sonst ±1 haben, mit Diskrepanz ≥ 6.**
  Belegart: vollständig durchgerechnet (Suchcode mit Symmetriereduktion; kein Zertifikat). Gilt nur für Einträge in {-1,0,1}.
- Teil 3, gemischt (Träger 10/8/6): Suche erfolglos, nicht fertig (Typ 0 vollständig, Typ 1 fast, Typen 2–9 nicht).
- Teil 4: P1 bestätigt; (13,3) selbstkomplementär: 42 Wörter erreicht, 40 nicht. Kein Rekord.
