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
