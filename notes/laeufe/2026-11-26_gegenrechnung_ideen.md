# Laufprotokoll Auftrag C: Gegenrechnung der Ideenrunde vom 26.11.2026

Sitzung (Cloud, 4 Kerne), Branch `claude/auftrag-c-gegenrechnung-2xqmh5`. Auftrag: `notes/gegenrechnung_ideen_2026-11-26_auftrag.md`.
Alle Uhrzeiten aus `date` (UTC). Beginn 17:45 UTC. Eigener Prüfcode in `verifiers/gegenrechnung_ideen/`, Logs in `runs/gegenrechnung_ideen/`.
Programme des Orchestrators nicht gelesen, außer dem Kopfkommentar von `ersatz_k2.c` (vom Auftrag für die Definition der Bedingung genannt).

## Programme

| Datei | Zweck |
|---|---|
| `eingaben.py` | Sylvester 8/16/32, Paley I 12, Paley II 28, Konferenz 26 (GF(25)) und 30 selbst erzeugt; `ctrl_16` mit ergänzter Spalte, Hadamard 20, McKay-60 gelesen; alle auf H Hᵀ = N I geprüft |
| `gr.c` | ein C-Programm, Unterbefehle `tab` (exakte disc je Spaltenmenge, Gray-Code), `schwelle` (Tiefensuche: gibt es x mit max ≤ T?), `flach` (Histogramm max\|Ax\|), `allein`, `frei`, `freizeile`, `ersatz1`, `ersatz2`, `zirk` |

## Schritt 0 (17:52 UTC)

`sha256sum -c verifiers/SHA256SUMS`: alle drei OK. Eichung `gr flach` (Gray-Code) gegen `pruefe_matrix.py`: ctrl_8 5, ctrl_16 7, had20_klasse1 6,
had24_1p3_paley12 8, je gleich. Tiefensuche (`gr schwelle`) bei T = disc − 1 „kein x“, bei T = disc „x gefunden“, je richtig. Log `schritt0.log.txt`.

## Ergebnisse (eigene Zahl / Soll / gleich?)

| Punkt | eigene Zahl | Soll | gleich | Log |
|---|---|---|---|---|
| 1, Ordnung 8 Sylvester, c = 0..4 | 4 / 5 / 4 / 3 / 14× 2, 56× 4 | 4 / 5 / 4 / 3 / 14× 2, 56× 4 | ja | `p1_tabelle.log.txt` |
| 1, Ordnung 12 Paley | 4 / 5 / 4 / 3 / 4 | 4 / 5 / 4 / 3 / 4 | ja | ebd. |
| 1, Ordnung 16 Sylvester | 4 / 5 / 6 / 5 / 140× 4, 1680× 6 | ebenso | ja | ebd. |
| 1, Ordnung 16 aus `ctrl_16` + Transponierte | 6 / 7 / 6 / 5 / 56× 4, 3584× 6 | ebenso | ja | ebd. |
| 1, Ordnung 20 (3 Klassen + Transponierte) | 6 / 7 / 6 / 5 / 6, alle Fälle gleich | ebenso | ja | ebd. |
