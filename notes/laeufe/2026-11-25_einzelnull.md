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
