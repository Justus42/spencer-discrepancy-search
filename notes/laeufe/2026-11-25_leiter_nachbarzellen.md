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
