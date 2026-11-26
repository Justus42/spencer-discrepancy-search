# Lauf 25.11.2026: Leiter in den Nachbarzellen mit dem Löser aus Auftrag H (Auftrag N)

Auftrag: `notes/leiter_nachbarzellen_2026-11-25_auftrag.md` (Entwurf des Orchestrators, durch den Startprompt von Justus freigegeben).
Branch `ccr-569e5bcc-xnj4i6`, `main` = `33712d6`. Cloud-Sitzung, 4 Kerne. Beginn 21:55 UTC (nach `date`). Logs `runs/leiter_n/`. Nichts installiert.
Treiber: `src/run_leiter_n.sh` (ein Block), `src/run_leiter_typen.sh` (mehrere Typen, Commit nach jedem Block). Suchcode `src/zerl_halb.c` unverändert.

## Schritt 0: Kontrollen

- `sha256sum -c verifiers/SHA256SUMS` zu Beginn (21:55 UTC): `src/check_disc.py` OK, `verifiers/verify_spencer.py` OK, `src/exakt_disc.c` OK (`runs/leiter_n/sha256_anfang.log.txt`).
- K3 (K = 12, Typ (11,9), d = 8, Startwerte 1–8, `--neustart 20`): **8 von 8 gelöst** (1,2 / 1,6 / 24,9 / 1,8 / 1,2 / 2,2 / 28,9 / 1,3 s; Soll ≥ 4 von 8). Alle 8 mit
  `zerl_offen.py offen` 0 offene Klassen von 262 144. Startwert 1 als Matrix 24 x 20: `pruefe_matrix.py --erwartet 8`: check_disc, verify_spencer, exakt_disc je Diskrepanz 8.
- `ctrl_17`-Struktur (K = 8, Typ (9,8), d = 7, Startwerte 1–4, ≤ 60 s): **4 von 4** je 0,1 s, 0 offene Klassen von 32 768.
- Kontrollen bestanden um 21:56 UTC.

## Schritt 1: (18,5), d = 8, K = 16, vier Typen, je 4 Startwerte ≤ 300 s (21:57–21:59 UTC), Log `runs/leiter_n/s1_block.out.txt`

**16 von 16 gelöst** (Soll: mindestens Typ (11,7)): (9,9) 1,8–4,4 s; (10,8) 15,6–47,0 s; (11,7) 7,9–25,9 s; (12,6) 7,0–64,5 s. Alle mit `zerl_offen.py` 0 offene Klassen.
Alte Nebenklassen-Suche: K = 15 in keinem Typ erreicht (0 von 12), K = 16 nur im Typ (11,7). Schritt 1 bestanden.

## Schritt 2: Leiter (18,5), 4 Startwerte je Stufe, ≤ 300 s, `src/run_leiter_stufen.sh` (21:59–22:23 UTC), Logs `runs/leiter_n/s2c18-5_k<K>_t<m1>-<m2>*`

| Typ | kleinstes gelöstes K (Wörter) | Stufe darunter (4 x 300 s) | Befund |
|---|---|---|---|
| (9,9) | **13 (52)**, 4 von 4 in ≤ 1,6 s | K = 12: 0 von 4, **Restzahl 1** in allen vier | s. Schritt 3 |
| (10,8) | 16 (64) | K = 15: 0 von 4, Restzahl 28 (alle vier) | |
| (11,7) | 16 (64) | K = 15: 0 von 4, Restzahl 9 / 9 / 9 / 29 | |
| (12,6) | 16 (64) | K = 15: 0 von 4, Restzahl 43 / 43 / 48 / 48 | |

Treffer (18,5), K = 13, Typ (9,9), Startwert 1 (`runs/leiter_n/s2c18-5_k13_t9-9_seed1.paar.txt`, Matrix `runs/leiter_n/c18-5_k13_t9-9.matrix.txt`, 26 x 18):
`zerl_offen.py offen … 8 --direkt`: 0 offene Klassen von 65 536, direkte Zählung auf der Matrix 0; `pruefe_matrix.py --erwartet 8`: check_disc 8, verify_spencer 8,
exakt_disc 8, **alle drei Diskrepanz 8** (22:11 UTC). 26 Zeilen = 52 Wörter mit Komplementen bei Radius 5 in Q18. Tabellenergebnis (Tabelle 28–64, eigener Wert bisher 64),
keine Rekordgrenze (Rekordweg bei d = 8 verlangt K ≤ 10, 40 Wörter). Nicht gemeldet, ob neu: das prüft der Orchestrator.

## Schritt 3: (18,5), Typ (9,9), K = 12 (48 Wörter), 8 neue Startwerte (5–8 und 9–12), je 900 s, zwei Blöcke (22:23–22:53 UTC)

0 von 8 gelöst, **beste Restzahl in allen acht Läufen 1** (von 65 536; `zerl_offen.py` zählt je Paar ebenfalls 1 offene Klasse). Zusammen mit den vier 300-s-Läufen der Stufe (alle Restzahl 1) steht
K = 12 bei 12 Startwerten fest bei Restzahl 1: auffällig fester Wert, Beinahe-Treffer; kein Ausschluss (Belegart: Suche erfolglos).
Logs `runs/leiter_n/s3c18-5a_*`, `s3c18-5b_*`. Zelle (18,5) abgeschlossen: kleinstes erreichtes K = 13 (52 Wörter) im Typ (9,9).

## Schritt 2: Leiter (16,4), d = 8, 4 Startwerte je Stufe, ≤ 300 s (22:53–23:17 UTC), Logs `runs/leiter_n/s2c16-4_k<K>_t<m1>-<m2>*`

| Typ | kleinstes gelöstes K (Wörter) | Stufe darunter (4 x 300 s) |
|---|---|---|
| (8,8) | 16 (64) | K = 15: 0 von 4, Restzahl 56 (alle vier) |
| (9,7) | 16 (64) | K = 15: 0 von 4, Restzahl 36 / 28 / 28 / 28 |
| (10,6) | 16 (64) | K = 15: 0 von 4, Restzahl 82 (alle vier) |
| (11,5) | 16 (64) | K = 15: 0 von 4, Restzahl 78 / 78 / 84 / 78 |

K = 16 in allen vier Typen gelöst; das ist der Tabellenwert 64, kein Fortschritt. Treffer mit K = 16 nur mit `zerl_offen.py` gezählt (Auftrag: Prüfung ab 4K < 64).

## Schritt 3: (16,4), Typ (9,7), K = 15 (60 Wörter), 8 neue Startwerte (5–12), je 900 s, zwei Blöcke (23:17–23:47 UTC)

0 von 8 gelöst, beste Restzahl 28 (sieben Läufe), 36 (ein Lauf); zusammen mit der Stufe (28 / 28 / 28 / 36) kein Treffer in 12 Startwerten (Suche erfolglos).
Zelle (16,4) abgeschlossen: kleinstes erreichtes K = 16 (64 Wörter) in allen vier Typen, kein Fortschritt gegenüber der Tabelle. Logs `s3c16-4a_*`, `s3c16-4b_*`.

## Schritt 2: Leiter (21,6), d = 9, 4 Startwerte je Stufe, ≤ 300 s (23:47–00:08 UTC, 26.11.), Logs `runs/leiter_n/s2c21-6_k16_t<m1>-<m2>*`

| Typ | K = 16 (64 Wörter) | beste Restzahl der vier Startwerte (von 2^(m1+m2−2) = 524 288) |
|---|---|---|
| (11,10) | 0 von 4 | 1056 / 1152 / 1256 / 1256 |
| (12,9) | 0 von 4 | 384 / 384 / 576 / 576 |
| (13,8) | 0 von 4 | 384 / 672 / 1401 / 2077 |
| (14,7) | 0 von 4 | 384 / 576 / 704 / 1280 |

Auch der Tabellenwert 64 wird in keinem Typ erreicht (wie die alte Nebenklassen-Suche: 0 von 12). Die Leiter endet in allen vier Typen an der ersten Stufe; es gibt kein „kleinstes erreichtes K“.

## Schritt 3: (21,6), Typ (12,9) (beste Restzahl 384; Gleichstand mit (13,8), (14,7), (12,9) hatte zwei Läufe bei 384)

- K = 16 (64 Wörter), Startwerte 5–8, je 900 s (00:08–00:23 UTC): **Startwert 6 gelöst nach 182,7 s** (`runs/leiter_n/s3c21-6a_k16_t12-9_seed6.paar.txt`, `zerl_offen.py` 0 offene Klassen
  von 524 288); Startwerte 5, 7, 8: Restzahl 576 / 576 / 320. Gleich dem Tabellenwert 64 (kein Fortschritt; Auftrag: Prüfung mit Matrix erst ab 4K < 64).
- K = 15 (60 Wörter), Startwerte 1–4, je 900 s (00:23–00:38 UTC): 0 von 4, Restzahl 3633 / 3600 / 3343 / 3343.
- K = 15, Startwerte 5–8, je 900 s (00:38–00:53 UTC): 0 von 4, Restzahl 3642 / 3600 / 3600 / 3600. Zelle (21,6) abgeschlossen: kleinstes erreichtes K = 16 (64 Wörter, nur im Typ (12,9), 1 von 8 Läufen bei 900 s), K = 15 in 8 Startwerten nicht.

## Schritt 2: Leiter (19,5), d = 9, 4 Startwerte je Stufe, ≤ 300 s (00:54–01:14 UTC), Logs `runs/leiter_n/s2c19-5_k16_t<m1>-<m2>*`

| Typ | K = 16 (64 Wörter) | beste Restzahl der vier Startwerte (von 2^(m1+m2−2) = 131 072) |
|---|---|---|
| (10,9) | 0 von 4 | 1280 (alle vier) |
| (11,8) | 0 von 4 | 1344 / 1648 / 1760 / 1792 |
| (12,7) | 0 von 4 | 1344 / 1472 / 1648 / 1664 |
| (13,6) | 0 von 4 | 1856 / 1856 / 1856 / 1912 |

Tabellenwert 64 in keinem Typ erreicht; es gibt kein „kleinstes erreichtes K“.

## Schritt 3: (19,5), Typ (10,9), K = 16 (64 Wörter), 8 neue Startwerte (5–12), je 900 s, zwei Blöcke (01:14–01:45 UTC)

0 von 8 gelöst; beste Restzahl 1024 / 768 / 1280 / 1280 (Startwerte 5–8) und 1280 viermal (9–12). Mit den vier Läufen der Stufe (1280) kein Treffer in 12 Startwerten (Suche erfolglos).
Zelle (19,5) abgeschlossen: der Tabellenwert 64 wird nicht erreicht; beste Restzahl 768 bei K = 16 im Typ (10,9).

## Schritt 2: Leiter (23,7), d = 9, 4 Startwerte je Stufe, ≤ 300 s (01:45–02:10 UTC), Logs `runs/leiter_n/s2c23-7_k<K>_t<m1>-<m2>*`

| Typ | K = 16 (64 Wörter) | Stufe darunter |
|---|---|---|
| (12,11) | **Startwert 3 gelöst nach 78,9 s**; Startwerte 1, 2, 4: Restzahl 288 / 112 / 384 (von 2 097 152) | K = 15: 0 von 4, Restzahl 680 / 548 / 672 / 548 |
| (13,10) | 0 von 4, Restzahl 1315 / 432 / 608 / 448 | – |
| (14,9) | 0 von 4, Restzahl 128 / 672 / 736 / 480 | – |
| (15,8) | 0 von 4, Restzahl 2304 / 984 / 1920 / 1152 | – |

Der Treffer bei K = 16 (`s2c23-7_k16_t12-11_seed3.paar.txt`, `zerl_offen.py` 0 offene Klassen) ist der Tabellenwert 64, kein Fortschritt. (Die alte Nebenklassen-Suche erreichte hier K = 15 nicht, 0 von 8, und K = 16 nicht versucht.)

## Schritt 3: (23,7), Typ (12,11), K = 15 (60 Wörter), 8 neue Startwerte (5–12), je 900 s, zwei Blöcke (02:10–02:41 UTC)

0 von 8 gelöst, **beste Restzahl in allen acht Läufen genau 548** (von 2 097 152); mit den vier Läufen der Stufe (680 / 548 / 672 / 548) in 12 Startwerten kein Treffer (Suche erfolglos).
Wie bei (18,5) K = 12 (Restzahl 1) ein fester bester Wert der Suche. Zelle (23,7) abgeschlossen: kleinstes erreichtes K = 16 (64 Wörter, Tabellenwert) nur im Typ (12,11).

## Schritt 2: Leiter (25,8), d = 9, 4 Startwerte je Stufe, ≤ 300 s (02:41–03:06 UTC), Logs `runs/leiter_n/s2c25-8_k16_t<m1>-<m2>*`

| Typ | K = 16 (64 Wörter) | Stufe darunter |
|---|---|---|
| (13,12) | 0 von 4, Restzahl 554 / 763 / 847 / 985 (von 8 388 608) | – |
| (14,11) | 0 von 4, Restzahl 188 / 417 / 538 / 601 | – |
| (15,10) | 0 von 4, Restzahl 1168 / 1459 / 1636 / 2004 | – |
| (16,9) | **Startwert 4 gelöst nach 176,3 s** (Restzahl der anderen 416 / 1940 / 2074) | K = 15: 0 von 4, Restzahl 10 600 / 10 600 / 11 562 / 12 690 |

Der Treffer bei K = 16 (`s2c25-8_k16_t16-9_seed4.paar.txt`, `zerl_offen.py` 0 offene Klassen von 8 388 608) ist der Tabellenwert 64, kein Fortschritt.

## Schritt 3: (25,8), Typ (16,9), K = 15 (60 Wörter), 8 neue Startwerte (5–12), je 900 s, zwei Blöcke (03:07–03:37 UTC)

0 von 8 gelöst; beste Restzahl 10 958 / 10 598 / 11 120 / 10 656 und 10 598 / 10 872 / 13 007 / 10 754 (von 8 388 608); mit der Stufe (10 600 / 10 600 / 11 562 / 12 690) kein Treffer in 12 Startwerten.
Zelle (25,8) abgeschlossen: kleinstes erreichtes K = 16 (64 Wörter, Tabellenwert) nur im Typ (16,9); K = 15 weit entfernt (Restzahl ≈ 10 600).
Hinweis: Typ (16,9) liegt an der Grenze des Programms (m1 = 16, Auftrag: m1 und m2 ≤ 16); der Typ (14,11) kam bei K = 16 auf Restzahl 188.

## Schritt 4: (20,6), d = 8, K = 11 (44 Wörter), Typen (14,6), (15,5), (16,4), je 4 Startwerte x 600 s (03:37–04:08 UTC), Logs `runs/leiter_n/s4c20-6*`

Alle 12 Läufe ungelöst. Beste Restzahl (von 2^(m1+m2−2) = 262 144): (14,6) 4152 (alle vier), (15,5) 5996 (alle vier), (16,4) 9415 (alle vier). Zum Vergleich Auftrag H: (11,9) 1288, (10,10) 1063, (12,8) 1836, (13,7) 2698.
Je weiter der Typ von (10,10) abweicht, desto größer die Restzahl. Suche erfolglos, nichts ausgeschlossen.

## Auswertung (ohne Zuruf, 04:08–04:11 UTC)

- **Abgleich Restzahlen:** alle 228 Läufe der Stufenlogs (`runs/leiter_n/*.log.txt` ohne `_seed`): die Restzahl der Suche (Zeile „beste Restzahl“) gleich der Nachzählung des gespeicherten Paares mit `zerl_offen.py`; 63 gelöste Läufe mit 0 offenen Klassen; **0 Abweichungen**
  (`runs/leiter_n/abgleich_restzahlen.log.txt`). Hinweis: das gleiche Programm `zerl_offen.py` zählt in der Treiberschleife nach; das Zählen der Suche selbst ist ein zweites, getrenntes Programm (`zerl_halb.c`).
- **Treffer unter 64 Wörtern (Auftrag: alle K ≤ 15):** (18,5), Typ (9,9), K = 15, 14, 13, je alle 4 Startwerte: `zerl_offen.py offen … 8 --direkt` 0 offene Klassen und direkte Zählung auf der 30 / 28 / 26 x 18-Matrix 0 (12 von 12, `runs/leiter_n/treffer_direkt.log.txt`).
  Je K eine Matrix (Startwert 1) mit `pruefe_matrix.py --erwartet 8`: check_disc, verify_spencer, exakt_disc je Diskrepanz 8 (`runs/leiter_n/treffer_pruefe_matrix.log.txt`, K = 13 im Protokoll oben). Unter 4K < 64 Wörtern gab es sonst keine Treffer.
- **Zusatz (nicht verlangt):** die drei K = 16-Treffer in (21,6), (23,7), (25,8) als Matrix 32 x 21 / 23 / 25: drei Prüfprogramme je Diskrepanz 9 (`runs/leiter_n/pruefe_k16_*.log.txt`; 2,1 s, 7,8 s, 30,8 s). Damit ist N = 32 mit d = 9 (Quotient 1,591) erreicht, vorher in LANDKARTE „nicht erreicht“.
- **Prüfsummen (Regel 8):** `sha256sum -c verifiers/SHA256SUMS` am Anfang (21:55 UTC) und am Ende (04:08 UTC): alle drei OK (`runs/leiter_n/sha256_anfang.log.txt`, `sha256_ende.log.txt`).
- `main` geholt (`fe088c7`, enthält Auftrag E und Gate E) und in den Branch gemerged; nicht nach `main` zusammengeführt.

## Zusammenfassung

- Kontrollen (K3 8/8, `ctrl_17`-Struktur 4/4, Schritt 1 16/16) bestanden; Löser aus H wie im Gate beschrieben.
- **Ergebnis:** (18,5): **K = 13 (52 Wörter, Tabelle 64)**, drei Prüfprogramme Diskrepanz 8; K = 12 bleibt bei Restzahl 1 in 12 Startwerten. Die vier Zellen (16,4), (21,6), (23,7), (25,8) erreichen 64 Wörter (= Tabelle), 60 nicht; (19,5) erreicht 64 nicht.
- **Kein Rekordkandidat** (Rekordgrenze d = 8: K ≤ 10, d = 9: K ≤ 12). Für (25,8) ist K = 12 weit entfernt: K = 15 liegt bei Restzahl ≈ 10 600 (der Typ (14,11) kam bei K = 16 auf 188 und wurde nach Auftragsregel nicht nachgefasst).
- Auffälliger fester Endpunkt wie bei Auftrag H: (18,5) K = 12 immer Restzahl 1; (23,7) K = 15 immer 548; (16,4) K = 15 meist 28. Struktur dieser Zustände nicht untersucht (Hadamard-Becken wie im Gate H nicht geprüft).
- Belegart: Treffer exakt; alles andere Suche erfolglos. Ob K(18,5) ≤ 52 neu ist, prüft der Orchestrator (nichts nach außen gemeldet).
- Rechenzeit (Wanduhr, 4 Kerne): 21:55–04:08 UTC = ca. 6 h 13 Min. von 7 h. Alle Zellen und Schritt 4 gerechnet; Reihenfolge der Zellen (18,5), (16,4), (21,6), (19,5), (23,7), (25,8) eingehalten.

## Abweichungen und Fehlversuche (für STATUS.md)

- Zellen ohne erreichtes K (alle vier Typen K = 16 ungelöst): Schritt 3 auf der ersten ungelösten Stufe K = 16 im Typ mit kleinster Restzahl ((21,6): (12,9) bei Gleichstand der besten Restzahl 384 mit (13,8), (14,7); (19,5): (10,9)); Auftragsformulierung „K − 1“ hier auf die Stufe K = 16 selbst angewendet.
- In (21,6) (12,9) und (25,8) (16,9) gelang K = 16 erst nach 183 s bzw. 176 s; bei (21,6) lösten 4 x 300 s der Stufe nichts (nur Startwert 6 der 900-s-Wiederholung).
- Nach einem Treffer in Schritt 3 (21,6) K = 16 wurde nach Auftrag mit K = 15 und 8 neuen Startwerten weitergerechnet (kein Treffer).
- Alle Wartebefehle liefen als Hintergrundjobs (Werkzeuggrenze 10 Min. im Vordergrund); gerechnet wurde in Blöcken von höchstens 31 Min., nach jedem Block Commit und Push.
