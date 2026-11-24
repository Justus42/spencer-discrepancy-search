# Status-Umzug, 24.11.2026 (Cloud, 19:38–ca. 19:50 UTC, Obergrenze 45 Min.)

Branch `ccr-b3e1311d-svmtzr`, neu von `main` = `ef69a87` (geprüft: `git log -1 origin/main` = ef69a87).
Reiner Doku-PR bis auf Teil C (`src/offene_zellen.py`) und Teil D (`src/gegenrechnung_testc.py`, `controls/QUELLEN.md`,
`notes/rundlauf_2026-11-24_auftrag.md`). Kein Ergebnis ändert sich. `LANDKARTE.md` und die drei Prüfdateien nicht angefasst
(`git diff origin/main` auf diese vier Dateien leer).

## Teil A, Archiv

`notes/verlauf/STATUS_2026-11-24_vollstaendig.md` = `git show origin/main:STATUS.md > …`.

```
9dc45109e2ffa8b5b8e0dffe5f1b77c625ffa5399da95de92c199300a41cdf24  -        (git show origin/main:STATUS.md | sha256sum)
9dc45109e2ffa8b5b8e0dffe5f1b77c625ffa5399da95de92c199300a41cdf24  notes/verlauf/STATUS_2026-11-24_vollstaendig.md
```
Gleich. `notes/verlauf/README.md` angelegt (eingefroren, alte Verweise meinen das Archiv, ältere Dateien nicht umgeschrieben).

## Teil B, neue STATUS.md

- **Zeilenzahl: 179** (Grenze 220). Abschnitte in der verlangten Reihenfolge; „Nächste Gates“ entfällt (steht im Archiv).
- Wörtliche Abschnitte per Skript aus dem Archiv eingesetzt, nicht abgetippt. Beleg (Python: jede alte Zeile `in` neuen Zeilen, und der
  alte Abschnitt als zusammenhängender Text enthalten):

  | Abschnitt | Archivzeilen | Zeilen | fehlen | als Block enthalten |
  |---|---|---|---|---|
  | Fehlversuche (B6) | 59–95 | 37 | 0 | ja |
  | Ziel | 5–10 | 6 | 0 | ja |
  | Reduktion | 12–17 | 6 | 0 | ja |
  | Dateien | 691–707 | 17 | 0 | ja |

  Unter Fehlversuche ist eine Zeile für diese Sitzung angehängt (Einzeiler); keine alte Zeile gekürzt.
- Abschnitt „Ziele“ (alte Tabelle der fünf Zielzellen) ist durch „Offene Zellen“ ersetzt; „Messwerte der Suche“, „Vorschläge aus der Astra-Abfrage“
  und die Lauf-Abschnitte stehen nur noch im Archiv.
- **Hinweis, nicht geglättet:** Zwei verlangte Aussagen stehen nicht im Archiv, in `LANDKARTE.md` oder `notes/laeufe/`, sondern in `notes/`:
  die Formulierung „K(2R+8, R) ≤ 48 für R ≥ 6“ (`notes/zweitmeinung_2026-11-24_pruefvermerk.md`, auch im Rundlauf-Auftrag) und der
  Entscheidungstermin 27.11. (`notes/zweitmeinung_2026-11-24_auftrag.md`). In STATUS mit diesen Fundstellen genannt. Die Bausteine der Familie
  (Normalität Norm 13, Verdreifachen einer Koordinate) stehen im Archiv.
- `CLAUDE.md`: nur Regel 4 (Cloud) geändert. Zeile 3 („… und das nächste Gate“) ist damit veraltet, laut Auftrag aber nicht angefasst.

## Teil C, offene Zellen

`src/offene_zellen.py` (Prüfcode, ganze Zahlen und Brüche). Eingabe: florath-CSV am Commit `bbed9a6`, neu abgerufen,
70 828 Bytes, **SHA-256 `5efaf8d9…ad1d6` gleich `controls/QUELLEN.md`** (das Skript bricht bei Abweichung ab). Log `runs/status_umzug/offene_zellen.log.txt`.

**Ergebnis: 20 offene Zellen, alle 20 mit Schranken und Dichte genau wie erwartet** ((13,3) 28–42 1,38 … (31,10) 37–224 2,69).
Alle übrigen Zellen mit Tabelleneintrag geschlossen (untere Schranke > 2B), ebenfalls wie erwartet.

Abweichungen zur Angabe, gemeldet:
1. Zellen mit R = 0 ((d,0) für d = 7–11) stehen nicht in der CSV. Trivial geschlossen (K(m,0) = 2^m > 2B); vom Skript als „FEHLT“ ausgegeben.
2. **(33,11) bei d = 11** liegt im Bereich m ≤ min(B, 33) = 33, fehlt aber in der CSV (bei n = 33 gibt es nur r = 1–10). Die Angabe des
   Orchestrators endet bei (31,10). Ohne Tabellenwert unentschieden. Eigene Rechnung: Kugelschranke 2^33 / V(33,11) = 24,96, also K ≥ 25 ≤ 76;
   Dichte bei 76 Wörtern 3,05. Nach der Kugelschranke allein wäre die Zelle offen. Nicht in die STATUS-Tabelle übernommen, nur vermerkt.

## Teil D

**D1, Quelle `controls/L2026_17x17.txt`.** `teorth/optimizationproblems` per Git-Abruf über den Proxy (öffentliches Repo, nur lesen).
- Commit `83e93c372e2e4889b12e5ada3a7c3422521e7baf` existiert: 23.11.2026 15:56:20 −0800 (= 23:56 UTC), Autor Terence Tao.
  `constants/10c.md` dort: 16 975 Bytes, SHA-256 `16d58551…c0c36a` (Rohabruf und Git-Objekt gleich).
- Abschnitt „Certificate for the 7/sqrt(17) lower bound“ (Zeile 283), Matrix Zeilen 288–304, 17 x 17: **0 abweichende Einträge** gegen die Repo-Datei.
  Referenz Zeile 437: „[L2026] Li, Youhua. 17 by 17 sign-matrix certificate for C10c, submitted to this repository (2026).“ Bestätigt.
- **Abweichung:** `83e93c3` ändert `constants/10c.md` nicht. Der Abschnitt steht seit `bedad0f` (18.07.2026, Autor Youhua Li,
  „an improved lower bound of 10c constant“), Matrix dort ebenfalls 0 Abweichungen; letzte Änderung vor `83e93c3`: `8f39d99` (21.11.2026), 0 Abweichungen.
  Die Datei kam am 23.11.2026 15:05 UTC in dieses Repo (`1d1248b`), also vor `83e93c3`. In `controls/QUELLEN.md` so eingetragen.
- Fehlversuch: `github.com/…/commit/….patch` → Proxy 403 (Sekunden), nicht umgangen; statt dessen `git fetch --depth` über den Git-Weg.

**D2:** Auftragstext unverändert als `notes/rundlauf_2026-11-24_auftrag.md`, mit der verlangten Kopfzeile.

**D3, Test C zweites Programm.** `src/gegenrechnung_testc.py`: nur Standardbibliothek, importiert nichts aus `src/hadamard_tests.py`.
Bauregel aus dem Kopf von `src/gegenbeispiel40.c` gelesen (M = [[H,H],[H,−H]], gestrichene Spalten links/rechts, Zeichen `1` = −1);
Zuordnung Log → `controls/had20_klasse{k}.txt` aus `runs/2026-11-24_testC_block1_pruefung.log.txt`. Prüft Hadamard-Eigenschaft, Vollständigkeit
der Auswahlen (3420 bzw. 6840 je Klasse), Muster, berechnet max |Ax| selbst und vergleicht mit dem Log-Wert.
Die Vektoren stehen in den Logs (`runs/2026-11-24_testC_klasse{1,2,3}_muster{13,22}_T{10,8}.log.txt`). Laufzeit 7 s.
**Ergebnis: T=10: 30 780 Vektoren, alle Maximum genau 10; Zusatz T=8: 30 780 Vektoren, alle Maximum genau 8; 0 fehlend, 0 Fehler, 0 Abweichung
zum Log.** Log `runs/status_umzug/testc_gegenrechnung.log.txt`.

## Abschluss

- `sha256sum -c verifiers/SHA256SUMS`: `src/check_disc.py: OK`, `verifiers/verify_spencer.py: OK`, `src/exakt_disc.c: OK`.
- `python3 verifiers/test_verifiers.py`: 11 Tests OK, 9,2 s.
- Kein Treffer, kein neues Ergebnis. Nichts gemergt, nichts nach außen gemeldet. PR auf `main` geöffnet (eigenes Repo).
