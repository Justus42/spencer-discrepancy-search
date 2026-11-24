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

| Angabe | Wert | Prüfstand |
|---|---|---|
| SHA-256 der Datei | `1494954b636824220856508d98d4de139135f66e14015ead0fe9e04fa85d39ed` | **selbst bestätigt** |
| URL | **Quelle nicht rekonstruierbar** | |
| Abrufdatum | **Quelle nicht rekonstruierbar** | |

Was sich belegen lässt, und nur das:
- Die Datei kam am 23.11.2026 mit dem ersten Übergabe-Commit `1d1248b` (16:05 +0100) ins Repo. Keine frühere Fassung, kein anderer Branch enthält sie.
- STATUS.md nennt als Herkunft nur „veröffentlichtes Zertifikat aus dem Repo“ (seit demselben Commit unverändert). Welches Repo gemeint ist, steht nirgends:
  README.md nennt `teorth/optimizationproblems` als Zielrepo der Konstante, verbindet die Datei aber nicht damit.
- Weder in STATUS.md, in `notes/`, in `runs/` noch in der Git-Geschichte (alle Branches, Suche nach „L2026“ in allen Änderungen) steht eine URL, ein Pfad,
  ein Commit oder ein Abrufdatum. Der Dateiname „L2026“ ist ebenfalls nirgends erklärt.
- Inhalt belegt: beide Prüfskripte und `exakt_disc` melden Diskrepanz 7 (`verifiers/test_verifiers.py`).

Nichts geraten. Wer die Quelle kennt (vermutlich der Auftraggeber der Übergabe-Sitzung), trägt sie hier ein.
