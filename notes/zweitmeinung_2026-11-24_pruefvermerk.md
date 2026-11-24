# Prüfvermerk zur Zweitmeinung vom 24.11.2026

Vom Orchestrator-Claude. Die Antworten der Modelle liegen in `notes/openrouter/` und `notes/gemini/`.
Sie sind Aussagen anderer Modelle und gelten nur, soweit sie hier als nachgerechnet stehen.
Die Rechnungen unten liefen mit eigenen Skripten außerhalb des Repos und sind von einer
Cloud-Sitzung noch nicht gegengerechnet.

## Ablauf

- Erster Versuch an beide Modelle gleichzeitig: Zeitüberschreitung der Bridge nach 230 s, keine Antwort gespeichert.
- GPT 6.2 Astra als Batch: fertig nach 8 Minuten, 0,33 US-$.
- Gemini 4 Neon als Batch mit Denkstufe high: alle 65 536 Tokens für das Denken verbraucht, kein Text,
  etwa 0,52 US-$ berechnet. Neu abgegeben mit Denkstufe medium.

## Astras Vorschläge, nachgerechnet am 24.11.2026

Astra nennt zwei Lücken in den Hadamard-Tests und einen billigen Zusatztest. Alle drei sind negativ.

1. **Ordnung 20 ohne zwei Spalten (20 x 18, Rekord bei Diskrepanz 8).** Exakt über alle 2^17 Färbungen:
   alle 190 Spaltenpaare, alle 3 Klassen und ihre Transponierten ergeben Diskrepanz 6.
2. **Ordnung 24 ohne drei Spalten (24 x 21, Rekord bei Diskrepanz 9).** Alle 60 Klassen aus
   `controls/had24_mckay_hex.txt` und ihre Transponierten, je alle 2024 Tripel: für jedes Tripel gibt es
   einen Vorzeichenvektor mit allen Beträgen <= 7. Die Zeugen stammen aus den Färbungen der vollen Matrix
   mit Maximum <= 8; die Zeugen selbst sind nicht gespeichert.
3. **Eine Spalte an den 48er-Code anbauen (24 x 21 mit Diskrepanz 9).** Für `controls/had24_1p3_paley12.txt`
   gibt es 4968 Färbungen mit Maximum 8 und genau einer Maximalzeile, für `had24_2p2_paley12.txt` 6048.
   Nach Astras Kriterium kann dann keine angebaute Vorzeichenspalte die Diskrepanz auf 9 heben.

## Astras Hinweis zur Beweiskette, geschlossen

`src/construct_48.py` prüft die Normalität nicht. Nachgerechnet für `controls/had24_1p3_paley12.txt`:
48 Wörter, Radius 6, Norm 13 in jeder der 20 Koordinaten. Damit hängt die Familie
K(2R+8, R) <= 48 für R >= 6 nicht mehr an den Suchtreffern.

## Was aus Astras Antwort übernommen werden sollte (noch nicht umgesetzt)

- Der Familienbeweis ohne Induktion: eine normale Koordinate durch 2s+1 Kopien ersetzen.
- Die Formulierung der Meldung als „explizites Zertifikat für eine obere Schranke 48 unter dem Wert im
  Tabellenstand <Commit>; historische Priorität nicht beansprucht“.
- Die Korrektur zur Deutung der Negativbefunde: Für Typ (13,9) bei (22,7) existiert ein Code mit 52 Wörtern
  (aus der Verdreifachung), die Suche fand ihn in 8 Läufen nicht. Erfolglose Läufe belegen also keine
  Nichtexistenz.
