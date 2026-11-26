# Gate zu Auftrag N (Leiter in den Nachbarzellen mit dem Löser aus Auftrag H), 26.11.2026

Geprüfter Stand: Branch `ccr-569e5bcc-xnj4i6`, Commit `fcd340f` (enthält `main` bis `fe088c7`). Prüfer: Orchestrator,
26.11.2026, 05:50 bis 05:59 Uhr (Uhr des Containers). Eigene Skripte ohne Code der Sitzung und Logs:
`notes/orchestrator/2026-11-26_gate_leiter_n/`. Rechenzeit etwa 6 Minuten auf 2 Kernen, davon 5 Minuten für zwei Proben.

## Urteil

**Bestanden. Kein Rekordkandidat. Ein Tabellenergebnis: K(18,5) ≤ 52 ist bestätigt, und das Gate verbessert es auf K(18,5) ≤ 50.**
Die Post-Kéri-Tabelle nennt 64. Ob 50 gegenüber der Literatur neu ist, ist noch nicht gründlich geprüft (unten). Nichts nach außen gemeldet.
Alle übrigen Befunde der Sitzung sind „Suche erfolglos“.

## Was geprüft wurde

| Nr. | Punkt | Ergebnis |
|---|---|---|
| 1 | Prüfdateien, Diff | `sha256sum -c verifiers/SHA256SUMS` OK (alle drei). `src/zerl_halb.c` und `src/zerl_offen.py` unverändert. Gegenüber `main` geändert: nur `STATUS.md`, `LANDKARTE.md`; sonst neue Dateien (`runs/leiter_n/` 579, vier Treiberskripte, Protokoll). Einige Logs über 300 KB. Stand-Zeilen von der Sitzung nicht angefasst. `main` war geholt. Nichts installiert |
| 2 | Protokoll gegen Auftrag | Schritt 0: K3 8 von 8, Struktur von `ctrl_17` 4 von 4. Schritt 1: 16 von 16. Reihenfolge der Zellen eingehalten, alle sechs Zellen und Schritt 4 gerechnet. 6 h 13 Min. von 7 h. Blöcke bis 31 Minuten (Auftrag: 30) |
| 3 | Alle gespeicherten Paare, eigener Zähler (`gate_zerl.py` aus dem Gate H) | 240 Paare: offene Klassen in 240 von 240 gleich der gemeldeten Restzahl. 75 gelöst: die Kontrollen, K = 16 bei (16,4) und (18,5), K = 15, 14, 13 bei (18,5) im Typ (9,9), je ein Treffer K = 16 bei (21,6), (23,7), (25,8). Kein Paar unter der Rekordgrenze |
| 4 | Der 52er, eigenes Skript `gate_18_5.py` Teil A | Matrix 26 x 18 selbst aus dem Paar gebaut, Zeilenmenge gleich der Datei der Sitzung. 52 verschiedene Wörter; kleinster Abstand zu einem Wort für **alle 2^18 Punkte** gerechnet: Überdeckungsradius 5, kein Punkt weiter weg. Ebenso für die Startwerte 2, 3, 4. `pruefe_matrix.py`: Diskrepanz 8 von allen drei Programmen |
| 5 | Tabelle | `non_mixed_covering_codes.csv` von `florath/covering-codes-lean`, Zweig `main`, abgerufen am 26.11.2026 um 05:52 Uhr: SHA-256 `5efaf8d9…`, gleich dem festgehaltenen Stand `bbed9a6`. (18,5): 28 bis 64, obere Schranke `rule_trivial_upper`. Auszug: `tabelle_auszug.log.txt` |
| 6 | `STATUS.md`, `LANDKARTE.md` | gegengelesen; 9/√32 = 1,591 stimmt, Spalten stimmen. Ergänzt um den 50er (N = 25, 8/√25 = 1,600) |

## Neu aus dem Gate: K(18,5) ≤ 50

Die Sitzung meldet für K = 12 (48 Wörter) in allen 12 Startwerten genau eine offene Klasse. Eine offene Klasse (y, z) steht für die vier Punkte (±y, ±z).
Das Wort w = (y, z') mit z' = z an vier Stellen gewechselt hat Abstand 4 zu (y, z) und Abstand 5 zu (y, −z), denn z und −z unterscheiden sich an allen
neun Stellen. Das Negativ −w erledigt (−y, −z) und (−y, z). Die 24 Zeilen des Zustands und die eine Zeile w geben also 25 Zeilen, 50 Wörter.

Geprüft (`gate_18_5.py` Teil B, `gate_18_5.log.txt`): in allen 12 Zuständen 50 verschiedene Wörter, Überdeckungsradius 5 über alle 2^18 Punkte.
Eine Matrix gespeichert (`gate_c18-5_50w.matrix.txt`, 25 x 18, aus Startwert 1): `check_disc`, `verify_spencer`, `exakt_disc` je Diskrepanz 8.
Der Code ist kein Kern-⟨1,a⟩-Code mehr (die Zusatzzeile hat keinen Partner). Für die Spencer-Konstante: N = 25, 8/√25 = 1,600, kein Rekord
(Rekordgrenze bei d = 8: 40 Wörter).

Das ist eine Rechnung des Orchestrators. Nach Projektregel muss eine Sitzung sie gegenrechnen, bevor sie als zweifach gerechnet gilt.

## Neuheit von K(18,5) ≤ 50 (Stand der Prüfung)

- Gegenüber der Post-Kéri-Tabelle (aktueller Stand, Punkt 5): neu, 50 statt 64.
- Aus der Tabelle folgt über die üblichen Regeln nichts Besseres als 64: 2 · K(17,5) = 64, K(15,4) · K(3,1) = 64, K(16,4) = 64.
- Eine Websuche am 26.11.2026 („binary covering codes new upper bounds K(18,5)“): kein Fund zu dieser Zelle. Marosi (arXiv 2608.19872) betrifft
  nur Alphabete ab 5 Zeichen.
- **Nicht geprüft:** offene PRs und Issues bei `florath/covering-codes-lean` (der Abruf brauchte eine Freigabe, die niemand erteilt hat), Kéris
  eigene Tabelle, arXiv seit September 2026 gründlich, EinsteinArena. Dafür Gemini Deep Research (Prompt in der Antwort an Justus).
- Folgewerte: K(20,6) ≤ 50 folgt, ist aber schwächer als die eigenen 48. K(19,6) ≤ 50 ist schwächer als die Tabelle (32). Es ändert sich also nur die Zelle (18,5).

## Befunde

1. **Hadamard-Becken, von der Sitzung offen gelassen:** Die zwölf Zustände mit Restzahl 1 haben in beiden Hälften die Zeilenprodukte 54-mal 1 und 12-mal 3, also
   beide Hälften von der Bauart „Hadamard-12 ohne drei Spalten“. Der 52er selbst ist nicht mehr von dieser Bauart (Produkte bis 7). Bei d = 9 haben die
   Treffer mit K = 16 eine Hälfte mit 96 bis 120 Produkten 0, bei (25,8) eine volle 16 x 16-Hadamard-Matrix.
2. **Probe: die freie Suche ist bei (18,5) schlechter als die Kern-Familie.** p018 aus Auftrag E bei (18,5) mit 24 Paaren, zwei Startwerte je 150 s:
   beide enden bei 288 offenen Klassen (`probe_p018_18_5.log.txt`). Bei (20,6) hatte dasselbe Programm den 48er in Sekunden gefunden.
3. **Probe: Mischform aus Nebenklassen und freien Zeilen.** K = 11 im Typ (9,9), zwei Startwerte je 150 s: beide Restzahl 967; die erste freie Zeile
   überdeckt 2076 der 3868 offenen Punkte, gierig genügen drei freie Zeilen, wieder 50 Wörter (`probe_k11.log.txt`, `probe_freie_zeilen.log.txt`).
   Bei (20,6) hilft das nicht: Die Zustände mit K = 11 aus Auftrag H brauchen gierig 16 freie Zeilen. Proben, keine Suche mit Anspruch.
4. **Auslegung der Sitzung, in Ordnung:** In Zellen, in denen K = 16 in keinem Typ gelöst war, wurde Schritt 3 auf K = 16 angewandt.
5. **Für (25,8):** K = 16 nur in einem Typ und einem von vier Läufen erreicht, K = 15 nicht (Restzahl etwa 10 600). K = 12, die Rekordgrenze bei d = 9,
   ist mit diesem Löser nicht in Sicht.

## Belegarten nach dem Gate

| Aussage | Belegart |
|---|---|
| K(18,5) ≤ 52 (26 x 18, Kern ⟨1,a⟩, K = 13) | exakt: drei Prüfprogramme (Sitzung und Gate), dazu eigener Zähler über alle Punkte |
| K(18,5) ≤ 50 (25 x 18) | exakt: drei Prüfprogramme und eigener Zähler über alle Punkte; Rechnung des Orchestrators, Gegenrechnung durch eine Sitzung offen |
| 64 Wörter bei (21,6), (23,7), (25,8) mit Diskrepanz 9 | exakt: drei Prüfprogramme (Sitzung); im Gate nur nachgezählt (0 offene Klassen) |
| alle nicht erreichten Stufen | Suche erfolglos; Restzahlen zweifach gezählt |

## Nicht geprüft

- Die drei Matrizen 32 x 21 / 23 / 25 nicht erneut durch `pruefe_matrix.py` (nur eigener Zähler auf den Paaren).
- Die Treiberskripte der Sitzung nicht gelesen.
- Neuheit gegenüber der Literatur, siehe oben.
