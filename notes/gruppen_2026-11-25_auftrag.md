# Auftrag G (freigegeben 25.11.2026): Gruppenmatrizen über nichtzyklischen Gruppen

Freigabe durch Justus im Orchestrator-Chat, 25.11.2026. Läuft parallel zu Auftrag S (`notes/spann_2026-11-25_auftrag.md`).
Kandidat 3 der Übergabe vom 25.11. mittags. Kandidat 2 (wenige Nebenklassen eines großen Kerns) ist nur als kurze
Gegenrechnung enthalten (Teil 4); Grund im Nachtrag von `notes/vortest_2026-11-25_spann.md`.

```
Auftrag G (Sonnet, Effort hoch): Gruppenmatrizen über nichtzyklischen Gruppen, vollständig

Lies zuerst STATUS.md, LANDKARTE.md, CLAUDE.md, notes/laeufe/2026-11-24_zirkulant.md und src/zirkulant.c,
src/run_zirkulant.py, src/zirkulant_pruef.py. Eigener Branch, Protokoll notes/laeufe/2026-11-25_gruppen.md, Logs
runs/gruppen/, STATUS.md erst am Ende. Blöcke von höchstens 30 Minuten, nach jedem Block committen und pushen.
Vor jedem Treffer-Bericht sha256sum -c verifiers/SHA256SUMS und python3 verifiers/pruefe_matrix.py.
Parallel läuft Auftrag S auf einem anderen Branch: die Stand-Zeile (Zeile 3) von STATUS.md und LANDKARTE.md nicht ändern.

Bauart: G Gruppe der Ordnung n, f: G -> {+1, -1}. Matrix A mit A[g][h] = f(g^-1 h), n x n. Zyklisches G gibt die
zirkulanten Matrizen (erledigt, Teil C vom 24.11.). Neu: alle nichtzyklischen Gruppen der Ordnungen
 18: Z3 x Z6, D9 (Diedergruppe, 18 Elemente), S3 x Z3, (Z3 x Z3) : Z2 (Z2 invertiert beide Faktoren)
 20: Z2 x Z10, D10 (20 Elemente), Dic5 (dizyklisch, <a,b | a^10 = 1, b^2 = a^5, b a b^-1 = a^-1>), F20 = Z5 : Z4 (Z4 wirkt treu, Erzeuger = Multiplikation mit 2)
 21: Z7 : Z3 (nichtabelsch)
 25: Z5 x Z5
Rekordschwelle d (kleinstes d mit d/sqrt(N) > 5/sqrt(8) und Parität der Zeilenlänge): n = 18: 8, n = 20: 8,
n = 21: 9, n = 25: 9. Mit Rand (zusätzliche Einser-Zeile und Nullspalte, wie in zirkulant.c): n = 18: N = 19, d = 8;
n = 21: N = 22, d = 9. Rand bei n = 20 und 25 nicht (Schwelle 10 bzw. 11, aussichtslos).

Programm: neues src/gruppenmatrix.c nach dem Muster von src/zirkulant.c (Suchcode: Aufzählung bis auf Symmetrie,
je Klasse Filter per Tabu-Suche nach x mit max |(Ax)_i| < d, dann Abstieg auf obere Schranke u). Die Gruppe kommt als
Verknüpfungstafel aus einer Datei, die ein kleines Python-Skript erzeugt und prüft (Assoziativität, Ordnung, abelsch
ja/nein, Zahl der Elemente je Ordnung). Erlaubte Symmetrien (lassen die Diskrepanz gleich): f(x) -> f(kx),
f(x) -> f(xk), f -> f nach einem Automorphismus von G, f -> -f. NICHT erlaubt bei nichtabelschem G: f(x) -> f(x^-1)
(das ist die Transponierte; sie kommt in der Aufzählung ohnehin als eigenes f vor). Wer die Automorphismen nicht
sicher hat, lässt sie weg; das kostet nur Zeit. Exakte Prüfung der Überlebenden und der Vertreter mit größtem u wie
in run_zirkulant.py über build/exakt_disc (Prüfcode, unverändert).

Teil 1, Kontrollen (Positivkontrolle in derselben Struktur: dasselbe Programm mit zyklischer Tafel). Erwartet:
 a) Z20, d = 8: 3760 Klassen, 0 Überlebende, größte Diskrepanz exakt 6.
    Z25, d = 9: 33 781 Klassen, 0 Überlebende, exakt 7. Z18, d = 8: 1346 Klassen, exakt 4. Z21, d = 9: 4434 Klassen,
    größtes u 7, exakt 5.
 b) Z7 mit Rand, d = 5: 5 Klassen, genau 1 Überlebender, exakt 5 (Hamming-Rekord).
 c) Klassenzahl jeder der zehn Gruppen gegen eine unabhängige Bahnen-Zählung in Python (Burnside oder direkt).
 d) Für eine der Gruppen der Ordnung 18: u gegen die exakte Diskrepanz aller Klassen; u darf nie kleiner sein.
 Bei Abweichung in a) oder b): stoppen und berichten.

Teil 2, volle Matrizen: alle zehn Gruppen, dazu Rand bei Ordnung 18 und 21. Je Gruppe: Klassenzahl, Überlebende,
 u-Verteilung, größte Diskrepanz exakt. Überlebender mit exakter Diskrepanz >= d: sofort Stopp, Matrix ablegen,
 pruefe_matrix.py, berichten.

Teil 3, zwei Spalten gestrichen (n x (n-2), Schwelle d bleibt: 18 -> 16 Spalten d = 8; 20 -> 18, d = 8;
 21 -> 19, d = 9; 25 -> 23, d = 9). Streichen von zwei Spalten ändert die Diskrepanz um höchstens 2. Also nur
 Klassen mit u >= d - 2 aus Teil 2 (u ist obere Schranke, das Aussieben ist damit streng). Für diese alle
 Spaltenpaare, Filter und exakte Prüfung wie oben.

Teil 4, kurze Gegenrechnung zu Kandidat 2 (eigenes Skript, notes/orchestrator/2026-11-25_vortest_spann/kern3.py nicht
 benutzen). Kern D = <1,a,b> der Länge 20 (8 Wörter); Typ = Größen der vier Spaltenklassen nach (a_i, b_i), absteigend
 sortiert, mindestens drei Klassen nicht leer. B(D) = Zahl der Nebenklassen von D mit Minimalgewicht <= 6.
 Erwartet: 97 Typen; größtes B(D) = 53 372 bei Typ (6,5,5,4); (5,5,5,5): 51 447; kleinstes 31 180 bei (18,1,1,0).
 Schranke 2^17 / B(D) liegt überall unter 5, schließt also keinen Typ aus. Nur bestätigen oder Abweichung melden.

Obergrenzen: Teil 1 30 Minuten, Teil 2 45 Minuten, Teil 3 30 Minuten, Teil 4 10 Minuten Rechenzeit; zusammen höchstens
2 Stunden. Abbruch: Kontrolle weicht ab; Zeit um, dann berichten, welche Gruppen ganz abgearbeitet sind.
Belegart: wie in Teil C (Gegenbeispiel je Klasse, Bester exakt). Nicht gerechnet: Ordnung 24 (14 nichtzyklische
Gruppen, Schwelle 10) und vier gestrichene Spalten bei Ordnung 25; im Protokoll als nicht gerechnet vermerken.
Am Ende ohne Zuruf auswerten: LANDKARTE.md (Regel 9, neue Zeile „Gruppenmatrizen nichtzyklisch“ mit dem besten exakten
Quotienten je N), STATUS.md höchstens fünf Zeilen plus Verweis aufs Protokoll, Fehlversuche als Einzeiler, nichts nach
außen melden.
```

## Startprompt für die Sitzung

```
Lies notes/gruppen_2026-11-25_auftrag.md in main und führe Auftrag G aus. Arbeite auf einem eigenen Branch.
```
