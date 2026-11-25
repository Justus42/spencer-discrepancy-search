# Auftrag H (Entwurf 25.11.2026, noch nicht freigegeben): Zerlegung der Kern-⟨1,a⟩-Familie, Halbproblem-Löser

Vom Orchestrator, 25.11.2026. Grundlage: `notes/ideen_2026-11-25.md` (Abschnitt 3), `notes/ideen_2026-11-25_pruefvermerk.md`, Sol-Antwort
`notes/openrouter/beweise_2026-11-25--openai-gpt-6.2-sol.md` (Fragen 2 und 3), Vortest-Skripte `notes/orchestrator/2026-11-25_ideen/zerlegung.py`,
`profile_d9.py`, `schachtel.py`. Freigabe und Start durch Justus. Der Gemini-Bericht vom 25.11. ist eingearbeitet (`notes/gemini/ideen_2026-11-25_kurz.md`): keine Vorarbeit in der
Literatur gefunden; zweite Positivkontrolle `ctrl_17`.

```
Auftrag H (Opus, Effort hoch): Codes mit Kern <1,a> als Paar (U,W); eigener Löser für das Halbproblem; abwechselnde Suche

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-25.md (Abschnitt 3) und notes/ideen_2026-11-25_pruefvermerk.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_zerlegung.md, Logs runs/zerlegung/, STATUS.md erst am Ende. Nichts installieren
(python-sat und ortools gibt es in der Sitzung nicht). Die Stand-Zeile von STATUS.md und LANDKARTE.md nicht ändern.

Grundlage: Ein selbstkomplementärer Code mit Verschiebungskern {0, a, 1+a, 1}, a = Einsen auf den letzten m2 Koordinaten, und K Nebenklassen
ist ein Paar (U,W): U ist K x m1, W ist K x m2, Einträge +1/-1; Wörter sind (U_i,W_i), (U_i,-W_i) und Komplemente (4K Wörter). Mit u = U y,
v = W z gilt:  Überdeckung mit d  <=>  für alle y, z:  max_i (|u_i| + |v_i|) >= d.
Der 48er ist K = 12, Typ (11,9), d = 8: U = Paley-12 ohne Spalte 0, W = Paley-12 ohne Spalten 0, 1, 2 (src/construct_48.py).
Ziele: d = 8, m1 + m2 = 20, K = 11 (44 Wörter, Tabellenwert) und K = 10 (40 Wörter, Rekord 1,789). d = 9, m1 + m2 = 21, 23 oder 25,
K = 12 (48 Wörter, Rekord 9/sqrt(24) = 1,837 bzw. 9/sqrt(25) = 1,800).

Schritt 0, Zahlen des Vortests nachrechnen (eigener Prüfcode src/zerl_offen.c oder .py, nur ganze Zahlen; zählt offene Klassen (y,z) mit
y_0 = z_0 = +1 und wandelt (U,W) in die 2K x (m1+m2)-Matrix für verifiers/pruefe_matrix.py um). Erwartet:
  - 48er: 0 offene Klassen bei d = 8; pruefe_matrix.py meldet Diskrepanz 8.
  - Profile |U y| sortiert, 1024 Klassen: (7,5,5,3,3,3,1^6) 660; (5^3,3^6,1^3) 220; (9,3^5,1^6) 132; (11,1^11) 12. Bei d = 8 sind nur die 220
    kritisch (220 verschiedene Anforderungsvektoren 3,3,3,5,5,5,5,5,5,7,7,7).
  - Profile |W z|, 256 Klassen: (7,5,3,3,3,1^7) 72; (5,5,5,3,3,3,1^6) 72; (5,5,3^6,1^4) 54; (7,3^6,1^5) 36; (9,3,3,1^9) 12; (5^4,1^8) 9; (3^12) 1.
  - U = 8 Hamming-Zeilen mit zwei verdreifachten Spalten, W = 8 Hamming-Zeilen mit einer verdreifachten Spalte: 24 192 offene Klassen von
    262 144 bei d = 8, für jede Zeilenzuordnung.
  Bei Abweichung stoppen und berichten.

Schritt 1, Halbproblem-Löser (Suchcode) src/zerl_halb.c: Eingabe U (K x m1), d, m2. Ausgabe: ein W (K x m2) mit 0 offenen Klassen, oder
„keine Lösung“ nach vollständiger Suche, oder Zeitgrenze. Dasselbe gespiegelt (W fest, U gesucht). Verfahren frei. Hinweise: Jede Zeile von W
ist eines von 2^(m2-1) Wörtern; je z genügt eine Bitmenge der noch unerfüllten Anforderungsvektoren (nur die maximalen zählen);
Symmetriebrechung über Spaltenvorzeichen (erste Zeile lauter +1) und Spaltenreihenfolge. Allgemeine Löser sind im Vortest gescheitert
(CP-SAT 120 s und 170 s, Glucose 150 s, je Zeitgrenze, 2 Kerne): deshalb eigener Code.
  K1 (Positivkontrolle): U des 48ers, d = 8, m2 = 9: ein W finden, in unter 10 Minuten, in mindestens 3 von 4 Startwerten. Ebenso gespiegelt
     (W des 48ers fest, U mit 11 Spalten gesucht).
  K1b (zweite Positivkontrolle): controls/ctrl_17.txt (32 Wörter, (17,5), d = 7) hat einen Verschiebungskern der Größe 4 mit Gewichten 8 und 9,
     ist also ein Paar (U,W) mit K = 8, Typ (9,8). Zerlegung selbst bestimmen (Kernelement a suchen, Spalten nach a teilen, je Nebenklasse
     eine Zeile), mit dem Prüfcode bestätigen (0 offene Klassen bei d = 7), dann zu U ein W finden und umgekehrt.
  K2 (Negativkontrolle, vollständig): U des 48ers ohne eine Zeile (K = 11): „keine Lösung“. Grund: die gestrichene Zeile ist ein y mit allen
     |u_i| = 1, also müsste W allein Q9 mit |v| >= 7 überdecken, 22 * 10 < 512.
  Abbruch: K1 nach 2 Stunden Sitzungszeit nicht bestanden: stoppen und berichten.

Schritt 2, zwei exakte Entscheidungen mit dem Löser (billig, erwartet „keine Lösung“): d = 9, U = Paley-12 ohne Spalte 0 (12 x 11),
W 12 x 12 und 12 x 14. Bekannt: m2 = 10 ist ausgeschlossen (Zählargument, Prüfvermerk); U = volle Paley-12 ist für jedes W ausgeschlossen.
Je Fall höchstens 30 Minuten; unentschieden bleibt unentschieden.

Schritt 3, abwechselnde Suche: aus Zufallsstart (U, W), dann im Wechsel U festhalten und W verbessern oder lösen, W festhalten und U.
  K3: K = 12, Typ (11,9), d = 8: Soll mindestens 4 von 8 Läufen zu 15 Minuten erreichen 0 offene Klassen. Abbruch bei weniger als 2 von 8.
      (Die alte Nebenklassen-Suche: 1 von 16 Läufen zu 30 Minuten.) Jeden Treffer mit pruefe_matrix.py bestätigen.

Schritt 4, Ziele, erst nach K3, in dieser Reihenfolge, je Ziel höchstens 60 Minuten Rechenzeit:
  (a) d = 8, K = 11, Typen (11,9), (13,7), (10,10), (12,8).
  (b) d = 8, K = 10, dieselben Typen.
  (c) d = 9, K = 12, Typen (bis auf Vertauschen der Hälften) {11,10}, {12,9}, {12,11}, {13,10}, {12,13}, {11,14}; U und W nicht aus Paley-12.
  Als frühe Filter für Typ (11,9), d = 8 (aus der Sol-Antwort; die ersten beiden sind sicher, die übrigen vor Gebrauch selbst nachrechnen):
  jedes y braucht max |u_i| >= 3 und jedes z max |v_i| >= 3; nicht beide Seiten dürfen eine Klasse mit Maximum 3 haben; bei Maximum 3 links
  mindestens 6 Zeilen mit |u_i| = 3; bei genau einer Zeile mit |u_i| = 5 (sonst kleiner) mindestens 5 Zeilen mit 3. Vollständiger Test für ein
  Paar: zu jeder Klasse die Mengen A = {|u| = 5}, B = {|u| = 3}, C = {|v| = 5}, D = {|v| = 3}; gedeckt, wenn eine Seite einen Betrag >= 7 hat
  oder A mit C oder D oder B mit C eine Zeile teilt.

Treffer (0 offene Klassen bei einem Ziel aus Schritt 4): sofort anhalten, Matrix nach runs/zerlegung/, sha256sum -c verifiers/SHA256SUMS,
python3 verifiers/pruefe_matrix.py MATRIX (erwartet Diskrepanz 8 bzw. 9), Protokoll, pushen, berichten. Nichts nach außen melden.

Obergrenze: 6 Stunden Rechenzeit. Blöcke von höchstens 30 Minuten, nach jedem Block pushen.
Belegart: „keine Lösung“ des Lösers nur dann „voll“, wenn die Suche vollständig war und die Negativkontrolle K2 bestanden ist; ein Programm.
Sonst „Suche erfolglos“.
Am Ende ohne Zuruf auswerten: main holen; STATUS.md höchstens fünf Zeilen plus Fehlversuche; LANDKARTE.md: kleinste erreichte Nebenklassenzahl
je Typ und beste Restzahl darunter (Regel 9). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/zerlegung_2026-11-25_auftrag.md in main und führe Auftrag H aus. Arbeite auf einem eigenen Branch.
```
