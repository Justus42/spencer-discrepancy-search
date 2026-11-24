# Prüfrahmen (24.11.2026, Cloud-Sitzung, Beginn 18:44 UTC, Obergrenze 1 h, tatsächlich ca. 10 Min.)

Auftrag: Prüfsummen, Wrapper, Testsuite, Quellen, zwei Eingriffe in STATUS. Kein Ergebnis ändert sich. Nichts gemergt, nichts nach außen.
Keine Suche, keine langen Läufe; kein Block über 30 Minuten (der längste Befehl dauerte 8 s).

## Ausgangslage

- Branch: `ccr-821fbcb6-qhbg5t` (von der Sitzung vorgegeben), Stand `abaddfe` = aktuelles `origin/main` (Merge PR #16). Der im Auftrag genannte Stand
  `98f1ed9` ist darin enthalten (`git merge-base --is-ancestor`). Die drei Dateien haben bei `98f1ed9` und bei `abaddfe` dieselben Prüfsummen.
- **`sha256sum -c verifiers/SHA256SUMS` zu Beginn, nach den Änderungen an CLAUDE.md/STATUS.md und am Ende: alle drei `OK`.**
  `git diff` auf den drei Dateien: leer.

```
a23faab48bf9718632db38fb26e29a1d65357059971bbae32d43b5035884c8d4  src/check_disc.py
edc28cc128b43142263bee8f62c2ef79191e0c4ccd4ed679c9d82a2b1098d2c7  verifiers/verify_spencer.py
3f5ffb0de846a021db14c23ba05e6a8dd24f84bd8a83712983895ef600ab35eb  src/exakt_disc.c
```

## Teil 1: Prüfsummen

`verifiers/SHA256SUMS` mit den drei Werten oben (die Erwartung des Orchestrators trifft zu). `CLAUDE.md`: Regel 8 (nur eigener PR, der nichts anderes enthält und
`SHA256SUMS` mitzieht; vor jedem Treffer-Bericht `sha256sum -c`, Ergebnis ins Protokoll). `test_sha256sums` in der Testsuite hält das fest: ändert jemand eine der drei Dateien
ohne `SHA256SUMS`, schlägt der Test an.

## Teil 2: Wrapper `verifiers/pruefe_matrix.py`

- Modus 1: `pruefe_matrix.py MATRIX [--erwartet D]`. Liest streng (nur ganze Zahlen in {-1,0,1}, gleich lange Zeilen), schreibt die quadratisch mit Nullen aufgefüllte Fassung in einen
  temporären Ordner (Nullspalten bei mehr Zeilen als Spalten, sonst Nullzeilen), startet gleichzeitig als getrennte Prozesse `check_disc.py` (Original), `verify_spencer.py`
  (aufgefüllt) und `build/exakt_disc` (Original, falls gebaut und innerhalb 64 x 40). Vergleicht Wert und gemeldete Matrixform. Fehlercodes: 0 gleich, 1 Abweichung
  (auch: ein Programm ohne Wert, während ein anderes rechnet), 2 Eingabefehler, 3 kein Wert. Der Wrapper rechnet nie selbst und teilt keinen Code mit den Programmen.
- Modus 2: `--paar ORIGINAL PAD`: PAD muss das Original als linken oberen Block enthalten, alles außerhalb 0, nichts eingefügt, nur angehängt. Rechnet keine Diskrepanz.
- Modus 3: `--alle-pads VERZ...`: sucht rekursiv `*_pad.txt`, ordnet das Original (Name ohne `_pad`) zu, wendet Modus 2 an. Nur Bericht.

**Lauf über alle vorhandenen `_pad`-Dateien in `controls/` und `runs/`: 54 Dateien, 54 in Ordnung, 0 Abweichungen, nichts repariert.**
Zusätzlich die drei `runs/matrix_n30pad_60w_*.txt` (Name ohne `_pad`-Suffix, deshalb vom Muster nicht erfasst) einzeln mit `--paar` gegen `runs/matrix_n20_60w_*.txt`: alle drei in Ordnung (30 x 20 plus 10 Nullspalten).

## Teil 3: Testsuite `verifiers/test_verifiers.py` (11 Tests, 8 s auf 4 Kernen)

**a) Soll-Diskrepanzen über den Wrapper, alle drei Programme gleich, alle gleich dem Soll aus STATUS.md. Keine Abweichung von STATUS.**

| Datei | Form | Soll | gerechnet |
|---|---|---|---|
| ctrl_8 | 8 x 8 | 5 | 5 |
| ctrl_9 | 9 x 9 | 5 | 5 |
| ctrl_16 | 16 x 16 | 7 | 7 |
| ctrl_17 | 17 x 17 | 7 | 7 |
| L2026_17x17 | 17 x 17 | 7 | 7 |
| astra_19 | 19 x 19 | 7 | 7 |
| had20_klasse1 / 2 / 3 | 20 x 20 | 6 | 6 / 6 / 6 |
| had24_1p3_paley12 | 24 x 20 | 8 | 8 |
| had24_2p2_paley12 | 24 x 20 | 8 | 8 |
| bahn15_40w | 20 x 15 | 7 | 7 |

Gegenprobe: falscher Soll (ctrl_8 mit 6) ergibt Fehlercode 1.

**b) Defektdateien `verifiers/defekt/`, abgeleitet aus `controls/ctrl_8.txt`** (Test prüft die Ableitung Byte für Byte): `zu_kurze_zeile.txt` (Zeile 4 mit 7 Einträgen),
`eintrag_2.txt` (Zeile 3, Spalte 3 gleich 2), `buchstabe.txt` (Zeile 5, Spalte 6 gleich `x`), `leer.txt` (0 Bytes).
Ergebnis: **jedes Programm lehnt jede Datei ab** (Rückgabecode ungleich 0, keine Diskrepanz-Zeile): `check_disc.py` (1), `verify_spencer.py` (1), `exakt_disc` (2), Wrapper (2).
Der Vortest des anderen Chats stimmt für die ersten drei; auch bei der leeren Datei lehnen beide Python-Skripte ab (`check_disc.py` mit `zero-size array`, `verify_spencer.py` mit
„Keine Matrixzeilen“).

**c) Bekannte Unterschiede, festgehalten, nicht behoben:**
- Mehr Zeilen als Spalten (`bahn15_40w.txt`, 20 x 15): `check_disc.py` rechnet 7, `exakt_disc` rechnet 7, `verify_spencer.py` lehnt ab („Ungueltige Dimension 20x15“).
  Mit Nullspalten aufgefüllt rechnet `verify_spencer.py` 7; der Wrapper tut genau das selbst.
- Gebrochene Einträge (ctrl_8 mit `1/2` oder `0.5` an Stelle (1,1)): `check_disc.py` lehnt ab, `exakt_disc` lehnt ab, `verify_spencer.py` rechnet **9/2**
  (durch eine unabhängige Bruch-Brute-Force in der Testsuite bestätigt). Der Wrapper verlangt ganze Zahlen (Fehlercode 2).
Ändert sich eines dieser Verhalten, schlägt der Test an.

**d) Zusätzlich:** Wrapper `--paar` negativ (5 Fälle: geänderter Original-Eintrag, Eintrag in Nullzeile, Eintrag in Nullspalte, kleineres PAD, vorn eingefügte Nullspalte, alle Fehlercode 1),
`--alle-pads` über `controls/` und `runs/` (0 Abweichungen), SHA256SUMS.

## Teil 4: Quellen

Siehe `controls/QUELLEN.md`.
- florath-Datei (Commit `bbed9a69…`): **selbst nachgeprüft**. Abruf über `raw.githubusercontent.com` mit dem festen Commit, 70 828 Bytes, SHA-256 `5efaf8d9…ad1d6` gleich dem Wert des Orchestrators.
  Die Commit-Daten (16.09.2026) ließen sich nicht abrufen (GitHub-Schnittstelle für dieses Repo nicht freigegeben, HTTP 403; nicht umgangen, kein `add_repo`), also Angabe des Orchestrators.
  Hinweis: Der Rohabruf einer öffentlichen Datei fremder Repos ging durch den Proxy; er schickt nichts von diesem Projekt.
- `had24_mckay_hex.txt`: Prüfsumme der Repo-Datei bestätigt (`4ad6d964…9fda`). Neuabruf der McKay-Seite scheiterte (Proxy 403), die Datei ist daher nur gegen sich selbst und gegen die Gegenrechnung belegt.
- `L2026_17x17.txt`: Prüfsumme bestätigt (`1494954b…39ed`). **Quelle nicht rekonstruierbar.** In STATUS, `notes/`, `runs/` und der gesamten Git-Geschichte (alle Branches) steht nur
  „veröffentlichtes Zertifikat aus dem Repo“ seit dem ersten Commit `1d1248b` (23.11.2026). Keine URL, kein Abrufdatum, nichts geraten.

## Teil 5: STATUS.md, zwei Eingriffe plus Abschnitt

1. Halbsatz an sechs Stellen „nicht im Repo“ (nur ergänzt, kein Wort entfernt): erstes Gate (60 Wörter), zweites Gate (56), Geminis Neuheitsbericht, drittes Gate (52), viertes Gate (48),
   sechstes Gate (Hadamard). Das sind die sechs Treffer der Wendung in `98f1ed9`.
   **Auf `abaddfe` gibt es einen siebten Treffer** (Abschnitt „Ansatz 3“: „der Auftragstext an die Modelle liegt nicht im Repo“). Er kam mit PR #16 nach der Zählung des Auftrags dazu und wurde
   **nicht** angefasst (kein gegengerechnetes Skript möglich, es geht um einen fehlenden Text). Ebenso nicht angefasst: der Eintrag zum fünften Gate („kein Skript möglich oder außerhalb des Repos“),
   der die Wendung nicht wörtlich enthält und im Repo bereits den Satz zu den 14 Treffern trägt. Bitte entscheiden, ob dort auch ein Halbsatz gewünscht ist.
2. Zeile „Überholt (Stand 23.11.), es gilt der Beschluss vom 24.11.“ unmittelbar über der Überschrift „Nächste Gates“ (wörtlich gelesen: über dem Abschnitt, nicht darunter).
3. Neuer Abschnitt „Prüfrahmen“ vor „Dateien“ mit dem Erledigten und dem Verweis auf `controls/QUELLEN.md`. Die zwei blockierten Abrufe stehen dort als Fehlversuche (Regel 5), nicht unter „Fehlversuche“, damit STATUS nur die erlaubten Eingriffe bekommt.

Die Halbsätze stützen sich auf vorhandene Einträge im Repo (STATUS, `src/`-Skripte, Logs), nicht auf neue Rechnungen. Offen bleibt, die 53 Dateien in
`runs/hadamard/` und die 30 780 Gegenbeispiele mit einem zweiten Programm gegenzurechnen (der Wrapper wäre dafür geeignet).

## Abschluss

`sha256sum -c verifiers/SHA256SUMS`: alle drei OK. `python3 verifiers/test_verifiers.py`: 11 Tests, OK, 8 s. Der Wrapper wurde hier nur zum Prüfen benutzt, an keinem Ergebnis hat sich etwas geändert.
