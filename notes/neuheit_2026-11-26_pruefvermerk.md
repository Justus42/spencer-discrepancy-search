# Prüfvermerk: Neuheit von K(20,6) ≤ 48 und K(18,5) ≤ 50 (Regel 3), 26.11.2026

Prüfer: Orchestrator, Chat vom 25.11. nachmittags, ausgelöst von der geplanten Erinnerung. 26.11.2026, 10:01 bis 10:08 Uhr
(Uhr des Containers). Ohne Justus am Gerät gerechnet. Nichts nach außen geschickt. Roh-Abfragen:
`notes/orchestrator/2026-11-26_neuheit/abfragen.log.txt`. Ergänzt `notes/gemini/neuheit_18_5_2026-11-26_kurz.md` (07:54 Uhr).

## Ergebnis

**Kein Fund.** In allen heute gelesenen Quellen steht für beide Zellen weiter 64. Nach diesem Stand sind K(20,6) ≤ 48 und
K(18,5) ≤ 50 neu. Der Rekord auf der Seite 10c ist unverändert 5/√8.

## Was geprüft wurde

| Nr. | Quelle | Befund | Weg und Grenze |
|---|---|---|---|
| 1 | Kéris Tabelle, die Primärquelle (`old.sztaki.hu/~keri/codes/`) | Letzte Änderung laut Indexseite 21.11.2011. Binäre Tabelle `2_tables.pdf`: K(18,5) 27 bis 64, K(20,6) 24 bis 64. Kontrollzellen passen: (17,5) und (15,4) ≤ 32, (26,9) und (28,10) ≤ 56, jeweils Östergård und Kaikkonen 1998 | **Zum ersten Mal gelesen.** Maschinell aus einem PDF mit beschädigter Textschicht; die Zuordnung der Quellenkürzel ist unsicher, die Werte passen zu den Kontrollzellen |
| 2 | Post-Kéri-Tabelle, `florath/covering-codes-lean` | `main` = `bbed9a6`, unverändert seit 07:50 Uhr. CSV mit SHA-256 `5efaf8d9…`, unverändert. (18,5): 28 bis 64, (20,6): 24 bis 64, obere Schranke jeweils `rule_trivial_upper`. Vier Zweige, **kein Pull Request** | `git ls-remote` und `curl`. Die Einschränkung von heute früh entfällt: Der Git-Zugang zeigt PR-Referenzen (bei teorth 379 Stück), bei florath gibt es also keine |
| 3 | `florath/covering-codes` (zweites Repo) | ein Zweig `master` = `46ba806`, kein Pull Request | `git ls-remote`; Inhalt nicht gelesen |
| 4 | Seite `constants/10c.md`, `teorth/optimizationproblems` | `main` = `83e93c3` (23.11.2026). Letzte Änderung an 10c am 21.11. (PR 296). Beste untere Schranke 5/√8 [H2026], davor 7/√17 [L2026] | vollständiger Klon |
| 5 | Pull Requests bei teorth | Alle 379 PR-Köpfe geholt (höchste Nummer 407). Nur PR 173 und 199 ändern 10c, beide vom September und beide zur oberen Schranke; PR 186 fügt ein Skript hinzu. 39 PR-Köpfe seit dem 22.11., keiner berührt 10c, keiner enthält „Spencer“ oder „covering code“ | Diff jedes Kopfes gegen seine Basis. Ob ein einzelner PR offen oder geschlossen ist, sagt Git nicht |
| 6 | Issues bei teorth | 3 offen: Nr. 67 (SDP-Erkundung zu 10c, April 2026), Nr. 211, Nr. 218 (betrifft 46a). Keine neue untere Schranke für 10c | Listenseite über WebFetch, nur Titel |
| 7 | EinsteinArena, Aufgabe Spencer | Zwei Threads (263, 273). Bestenliste oben 1,697749 (7/√17). Thread 273: 21 Antworten, die jüngste „vor 64 Tagen“; (18,5) „known 28..64“, (20,6) „known 24..64“; kein Code unter 64, kein 24 × 20 oder 25 × 18 | WebFetch, von einem Hilfsmodell zusammengefasst; 3 Antworten zeigt die Seite nicht an |
| 8 | arXiv seit 01.09.2026 | Kein Treffer zu binären K(n,R) nach oben und keiner zu einer unteren Schranke über 5/√8. Am nächsten: 2610.03760 (K_4(6,2) ≤ 48, quaternär), 2608.19872 (q ≥ 5), 2609.16078 (linear, R = 2), 2609.34471 (Reis und Song, asymptotisch) | zwei Suchen über alphaXiv, zwei Websuchen; keine Volltextsuche |
| 9 | Quantavane (`arhancanli/quantavane-benchmark`, Stand `deeaac3` vom 21.11.2026) | 33 Problemfamilien, keine zu K_q(n,R). Die Zeichenfolge „Theorem D“ kommt im Repo nicht vor. `prime-cover` ist ein anderes Problem (Überdeckung unter Primzahlen) | flacher Klon. Geminis Hinweis auf ein „Theorem D zu sechs binären Zellen“ ist damit **nicht bestätigt**; eine Webseite außerhalb des Repos habe ich nicht gesucht |
| 10 | `openai/math` (Stand `3344da5` vom 24.11.2026) | 761 Preprint-Ordner. Kein Titel zu binären Überdeckungscodes oder zur Spencer-Konstante | nur Ordnernamen nach Stichwörtern, keine Inhalte |

Nebenfund ohne Folgen für die Meldung: arXiv:2610.24944 („Epi“, ein Agentensystem) nennt im Anhang ebenfalls die untere Schranke
5/√8 für die Spencer-Konstante. Das ist der Wert des Rekords auf Seite 10c, nicht mehr. Nur der Suchtreffer ist gelesen.

## Nicht geprüft (Stand 10:08 Uhr; Issues, Thread 273 und Kéris PDF sind im Nachtrag unten erledigt)

- **Issues bei florath:** Die Seite ließ sich zweimal nicht abrufen (Abruflimit). Einmal von Hand ansehen:
  `github.com/florath/covering-codes-lean/issues`.
- Volltexte hinter Bezahlschranken (Östergård und Kaikkonen 1998, Cohen und andere 1997) und Lobsteins Bibliographie.
- Die drei nicht angezeigten Antworten in Thread 273.
- Inhalte von `openai/math`; Kéris PDF mit eigenen Augen (Zeile n = 18, Spalte R = 5 und Zeile n = 20, Spalte R = 6).

## Nachtrag 26.11.2026, 11:21 bis 11:30 Uhr: drei Lücken geschlossen (Justus am PC, Orchestrator-Chat vom 25.11. abends)

| Lücke | Befund | Weg |
|---|---|---|
| Issues bei `florath/covering-codes-lean` | **keine offenen Issues** | Justus im Browser, Meldung 11:29 Uhr. Geschlossene Issues nicht erfragt |
| Kéris PDF mit eigenen Augen | Tabelle „Bounds on K(n, R), R = 4, 5, 6“: Zeile n = 18, Spalte R = 5: **t 27–64**; Zeile n = 20, Spalte R = 6: **d 24–64**. Hinter der 64 steht in beiden Zellen kein Quellenkürzel | Justus, Bildschirmfoto der Tabelle; vom Orchestrator im Bild nachgelesen. Bestätigt Nr. 1 oben; die Unsicherheit aus der beschädigten Textschicht entfällt für diese zwei Zellen |
| EinsteinArena, Thread 273 im Volltext | Startbeitrag (FableCover, 21.09.) und **alle 21 Antworten** angezeigt (zwei Autoren, „vor 64 bis 66 Tagen“). (18,5) und (20,6) kommen nur im Startbeitrag vor: „known 28..64“, „known 24..64“, der Eintrag 64 bei (20,6) heißt dort triviale Verdopplung, nie optimiert. Keine Antwort nennt Länge 18 oder 20, einen Code mit 48, 50 oder 52 Wörtern, Paley oder eine Verdopplung | Seitentext aus dem Browser der Claude-App auf Justus' PC (Justus hatte die Seite geöffnet), 59 994 Zeichen, vom Orchestrator mit Stichwortsuche und Lesen der Fundstellen ausgewertet. Die Antworten drehen sich um n = 12 (SAT) und n = 15; nicht jede Antwort Wort für Wort gelesen. Text nicht ins Repo übernommen |

Damit weiter ungelesen: Bezahlschranken-Texte (Östergård und Kaikkonen 1998, Cohen und andere 1997), Lobsteins Bibliographie,
Inhalte von `openai/math`, geschlossene Issues bei florath. Ergebnis unverändert: **kein Fund**.

## Für die Entscheidung am Freitag

- Wortlaut wie in `claude/Beitrag-2026-11-25-methodentransfer.md`: „kleiner als die Tabelle am Commit `bbed9a6`“, nicht „neu“
  ohne Zusatz. Für Kéri: „Stand 21.11.2011“.
- K(18,5) ≤ 50 ist erst am 26.11. entstanden; Lean-Zertifikat liegt vor (Auftrag LZ, Gate bestanden).
- Ungelesene Quellen in der Meldung nennen (Liste oben). KI-Einsatz offenlegen.
- Unmittelbar vor dem Absenden noch einmal `git ls-remote https://github.com/florath/covering-codes-lean` (eine Zeile, `main`
  muss `bbed9a6` sein, sonst Tabelle neu lesen). Justus schickt selbst.
