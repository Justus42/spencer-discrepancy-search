# Quellen der externen Dateien

Stand 24.11.2026 (Sitzung Prüfrahmen). „Selbst geprüft“ heißt: in dieser Sitzung abgerufen oder nachgerechnet.
„Angabe des Orchestrators“ heißt: übernommen, nicht selbst geprüft.

## florath/covering-codes-lean, Überdeckungscode-Tabelle

| Angabe | Wert | Prüfstand |
|---|---|---|
| Repo | `florath/covering-codes-lean` | |
| Commit | `bbed9a690e6c54f8bba423031d4d516f70d267b9` | Hash selbst geprüft (Abruf gerade dieses Commits gelang) |
| Commit-Datum | 16.09.2026 | **Angabe des Orchestrators**; der Abruf der Commit-Daten über die GitHub-Schnittstelle wurde abgelehnt (HTTP 403, Repo nicht freigegeben), nicht umgangen |
| Datei | `reference-data/post-keri/non_mixed_covering_codes.csv` | |
| SHA-256 | `5efaf8d94a3ef15e9efc052233259697fc5f0b5e51389bcc5c8c108b6c3ad1d6` | **selbst bestätigt**: Abruf von `https://raw.githubusercontent.com/florath/covering-codes-lean/bbed9a690e6c54f8bba423031d4d516f70d267b9/reference-data/post-keri/non_mixed_covering_codes.csv` am 24.11.2026, 70 828 Bytes, SHA-256 gleich dem Wert des Orchestrators |
| Abruf des Orchestrators | 24.11.2026 | Angabe des Orchestrators |

Die Datei liegt nicht im Repo. Stichprobe zur Gegenprobe der Tabelle in STATUS.md: Zeile `2,20,6,24,64,lit_bhandari_chanduka_lal_1998,rule_trivial_upper`
(K(20,6) = 24–64), gleich der Angabe in STATUS.md.

## controls/had24_mckay_hex.txt, 60 Hadamard-Klassen der Ordnung 24

| Angabe | Wert | Prüfstand |
|---|---|---|
| Quelle laut STATUS.md (Abschnitt „Gate-Prüfung … sechstes Gate“) | Brendan McKay, Sammlung `https://users.cecs.anu.edu.au/~bdm/data/hadamard.html`, Datei `had24.txt` | Angabe aus STATUS.md |
| Abruf | 24.11.2026 (Angabe des Auftrags), im Repo seit Commit `3dfee28` (24.11.2026, 11:01 +0100) | |
| Form | Webabruf mit Textwiedergabe, je Zeile eine Matrix aus 24 Hexadezimalzahlen (eine je Matrixzeile), Bit 0 = −1 | Angabe aus STATUS.md |
| SHA-256 der Repo-Datei | `4ad6d964b15cf091783e32063c278daecbc3745b38d19e7741d8c842e1519fda` | **selbst bestätigt** (`sha256sum`, gleich dem erwarteten Wert) |

Die Prüfsumme gilt für die Datei im Repo, nicht für die Originaldatei bei McKay. Ein erneuter Abruf des Originals war in dieser Sitzung
nicht möglich (Proxy lehnte den Host ab, HTTP 403); die Wiedergabe lässt sich deshalb nicht Byte für Byte mit der Quelle vergleichen.
Gegenrechnung des Inhalts: `src/pruefe_mckay.py` (60 Matrizen Hadamard, paarweise verschieden, alle Diskrepanz 6, siehe STATUS.md).
Dass es 60 verschiedene Klassen sind, ist Angabe der Quelle.

## controls/L2026_17x17.txt, 17 x 17, Diskrepanz 7

Nachtrag 24.11.2026 (Sitzung Status-Umzug, Protokoll `notes/laeufe/2026-11-24_status_umzug.md`). Quelle jetzt **selbst geprüft**.

| Angabe | Wert | Prüfstand |
|---|---|---|
| SHA-256 der Datei | `1494954b636824220856508d98d4de139135f66e14015ead0fe9e04fa85d39ed` | **selbst bestätigt** |
| Repo | `teorth/optimizationproblems` | selbst geprüft |
| Datei, Abschnitt | `constants/10c.md`, „Certificate for the $7/\sqrt{17}$ lower bound“ (Matrix im Textblock, Zeilen 288–304) | selbst geprüft |
| Referenz | [L2026] Li, Youhua. „17 by 17 sign-matrix certificate for C10c, submitted to this repository (2026).“ | selbst geprüft (Zeile 437 der Seite) |
| Commit laut Auftrag | `83e93c372e2e4889b12e5ada3a7c3422521e7baf`, 23.11.2026 15:56:20 −0800 (Autor Terence Tao) | selbst geprüft (Git-Abruf); SHA-256 von `constants/10c.md` dort `16d585513824abda8b59577956e9571f00dab805238ce879105d4a0c2bc0c36a` |
| Vergleich | 17 x 17, **0 abweichende Einträge** gegen die Repo-Datei | selbst gerechnet |
| Abruf | 24.11.2026 | |

**Abweichung zur Angabe des Orchestrators (gemeldet, nicht geglättet):** Commit `83e93c3` ändert `constants/10c.md` nicht. Der Abschnitt und die
Referenz [L2026] stehen schon seit Commit `bedad0f9db7de4c8645c5eb93a848ec0e5f38bf2` (18.07.2026, Autor Youhua Li, „an improved lower bound of 10c constant“),
mit derselben Matrix (0 Abweichungen); letzte Änderung der Seite vor `83e93c3` ist `8f39d99` (21.11.2026), ebenfalls 0 Abweichungen.
Das passt dazu, dass die Datei schon am 23.11.2026, 16:05 +0100 (15:05 UTC, also vor `83e93c3` mit 23:56 UTC) in dieses Repo kam (Commit `1d1248b`).
`83e93c3` ist also eine Fassung, in der die Matrix steht, nicht ihre Erstveröffentlichung.

Frühere Fassung dieses Abschnitts (Stand Prüfrahmen, „Quelle nicht rekonstruierbar“): siehe Git-Geschichte dieser Datei.
