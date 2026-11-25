# Auftrag P (Entwurf 25.11.2026, noch nicht freigegeben): Gegenrechnung der Vortests der Ideensammlung

Vom Orchestrator, 25.11.2026. Grundlage: `notes/ideen_2026-11-25.md`, `notes/ideen_2026-11-25_pruefvermerk.md`, Skripte und Logs in
`notes/orchestrator/2026-11-25_ideen/`. Zweck: Projektregel „eigene Rechnungen des Orchestrators werden von einer Sitzung gegengerechnet“.
Kein Rekordweg; erst danach dürfen die Befunde in `STATUS.md` und `LANDKARTE.md`. Freigabe und Start durch Justus.

```
Auftrag P (Sonnet, Effort hoch): Vortests vom 25.11.2026 mit eigenen Programmen nachrechnen, Belege speichern

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-25.md und notes/ideen_2026-11-25_pruefvermerk.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_pruef_ideen.md, Logs und Belege runs/pruef_ideen/, STATUS.md erst am Ende. Nichts installieren
(python-sat und ortools gibt es nicht): eigene Programme in C oder Python (Standardbibliothek, numpy). Die Skripte des Orchestrators dürfen
gelesen, aber nicht als Beleg wiederverwendet werden; jedes Ergebnis kommt aus einem eigenen Programm. Die Stand-Zeile nicht ändern.
Bei jeder Abweichung von einer erwarteten Zahl: diesen Teil stoppen, Abweichung ins Protokoll, mit dem nächsten Teil weitermachen.

Teil 1, drei Zeilen des 32ers durch zwei beliebige ersetzen ((15,4), controls/ctrl_16.txt, erste 15 Spalten).
  Aussage: Für jedes der 560 Zeilentripel gibt es keine zwei Zeilen mit Einträgen in [-1,1], die zusammen mit den 13 übrigen jede Klasse
  über die Schwelle 5*sqrt(15/8) = 6,847 bringen. Grund: Zwei orientierte Punkte derselben Ersatzzeile haben Hamming-Abstand <= 8. Also müssten
  sich die Lochklassen (von den 13 Zeilen nicht mit Betrag >= 7 getroffen) auf zwei Zeilen verteilen und mit Vorzeichen versehen lassen, so dass
  je Zeile gilt: Abstand <= 6 verlangt gleiches Vorzeichen, Abstand >= 9 verschiedenes, 7 und 8 sind frei.
  Erwartet: Lochzahl 967 (448 Tripel) oder 987 (112 Tripel); bei zwei gestrichenen Zeilen 550 (64 Paare) oder 562 (56).
  Verlangt: je Tripel ein Zertifikat, also eine kleine Menge von Lochklassen (im Vortest 7 bis 10 Stück), die sich schon allein nicht verteilen
  lässt, gespeichert in runs/pruef_ideen/kern_r3/. Ein getrenntes Prüfskript (nur ganze Zahlen) bestätigt jedes Zertifikat durch Aufzählen aller
  Verteilungen auf zwei Zeilen mit Zweifärbung der Vorzeichen, und prüft, dass die Klassen wirklich Löcher sind. Suche der Zertifikate frei
  (zum Beispiel gierig wachsen lassen, bis unverteilbar, dann ausdünnen).
  Kontrollen: zwei gestrichene Zeilen, zwei Ersatzzeilen: verteilbar (die Originalzeilen), das Prüfskript muss das erkennen; zwei gestrichene,
  eine Ersatzzeile: Zertifikat vorhanden.
  Danach, je höchstens 60 Minuten: dasselbe für die zweite 32er-Klasse (runs/gegenrechnung15/loesung111_16x15.txt; im Vortest ebenfalls
  alle 560 Tripel unverteilbar, Lochzahlen 967 und 987);
  und vier gestrichene Zeilen gegen drei Ersatzzeilen (1820 Fälle; erwartete Lochzahlen 1476 (784 Fälle), 1484 (896), 1508 (112), 1512 (28);
  im Vortest 8 Stichproben, alle unverteilbar). Was nicht fertig wird, mit Fortschritt als unentschieden vermerken.

Teil 2, ganze Spalte neu wählen: ctrl_16 ohne eine Zeile (16 Möglichkeiten), eine Spalte verdeckt (15), alle 2^15 Ergänzungen exakt.
  Erwartet: in allen 240 Fällen Minimum 233 offene Klassen, die echte Spalte ist optimal, 4 Optima. Kontrolle mit allen 16 Zeilen: Minimum 0.

Teil 3, kozyklische Matrizen (notes/ideen_2026-11-25.md Abschnitt 5; Gruppe E der Ordnung 2n mit zentraler Involution z, f ungerade,
  M[g][h] = f(g^-1 h) über Vertretern der Nebenklassen von <z>).
  n = 18 (Ziel 8): Z36, Dic9, Z3 x Dic3. n = 20 (Ziel 8): Z40, Z5 x Q8, Z5 x D4, Z5 x Z4 x Z2 mit z = (0,2,0), Dic10, D20.
  Für jede Funktion f mit f(e) = +1 ein Gegenbeispiel x mit allen |<M_i,x>| <= 6 speichern (komprimiert) und mit einem getrennten Skript
  nachrechnen. Erwartet: 0 Überlebende. Dazu der exakte Bestwert je n über src/exakt_disc.c (Vortest: bei n = 20 wird Diskrepanz 6 erreicht,
  Z5 x Q8: 3240 Funktionen mit Diskrepanz >= 6, darunter 480 Hadamard-Matrizen; Z40: 28 560, keine Hadamard-Matrix).
  Kontrollzahlen für das eigene Programm: negazyklisch n = 8: 64 von 128 Funktionen mit Diskrepanz >= 4; n = 10: 270 von 512; Williamson-Anordnung
  mit zirkulanten 3 x 3-Blöcken (Z3 x Q8): 336 von 2048.
  Wenn Zeit bleibt (höchstens 60 Minuten): n = 26, 28, 30 mit Ziel 10 als Sieb ohne gespeicherte Gegenbeispiele; im Vortest ist Dic15 bei n = 30
  nur zu 80 % gelaufen.

Teil 4, Zahlen der Zerlegung (notes/ideen_2026-11-25.md Abschnitt 3.2 und Prüfvermerk, Tabelle zu Sol):
  - Paley-12 voll (12 x 12): Profile (6^3,2^9) 880 Klassen mit 220 Zeilenmustern, (4^9,0^3) 220 mit 220, übrige enthalten einen Betrag >= 8.
  - Schnitt einer Radius-2-Kugel mit einer Radius-4-Kugel in Q13 bei Abstand 0 bis 6: 92, 92, 92, 47, 47, 15, 15. Bilanz bei m2 = 13:
    vorhanden 24 * 1093 = 26 232, nötig 26 144, Spielraum 88; überzählig mindestens 24 * 11 * 15 = 3960, erlaubt höchstens 320.
  - Paley-12 ohne Spalte 0 (12 x 11), Profil (5^3,3^6,1^3), d = 9: Keine Kette C in B in A von Zeilenmengen mit |A| + 3|B| + 6|C| <= 9 erfüllt
    alle 220 Muster (Muster erfüllt, wenn eine 5er-Zeile in A, eine 3er-Zeile in B oder eine 1er-Zeile in C liegt). Kleinstes |A| bei |B| = 0, 1, 2
    und C leer: 10, 7, 6. Summen: m2 = 10: 4920 gegen 5120 (ausgeschlossen); m2 = 12: 25 980 gegen 20 480; m2 = 14: 128 472 gegen 81 920.
  - Lemma: U des 48ers ohne eine Zeile, y = diese Zeile: alle übrigen |u_i| = 1.

Teil 5, nur lesen und urteilen: Astras Ausschluss reeller 10 x 10-Matrizen (Astra-Antwort, Abschnitt 1, letzter Punkt). Jeden Schritt prüfen;
  wo möglich mit einer kleinen Rechnung belegen (zum Beispiel: größte nach unten abgeschlossene Familie von Teilmengen einer 10-Menge mit
  |S vereinigt T| <= 4 für alle Paare, die eine Dreiermenge, aber keine Vierermenge enthält: höchstens 43 Mengen). Ergebnis: schlüssig oder Lücke.

Obergrenze: 5 Stunden Rechenzeit, Teil 1 höchstens 3 Stunden. Blöcke von höchstens 30 Minuten, nach jedem Block pushen.
Am Ende ohne Zuruf auswerten: main holen; die in notes/ideen_2026-11-25.md Abschnitt 7 und 10 vorbereiteten Zeilen nach STATUS.md
(Negativbefunde mit Belegart, Fehlversuche als Einzeiler) und LANDKARTE.md (neue Zeile „kozyklisch“ mit exaktem Bestwert; (15,4): „13 gemeinsame
Zeilen mit ctrl_16 ausgeschlossen, auch reell“) übernehmen, soweit bestätigt; Abweichungen ausdrücklich nennen. Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/gegenrechnung_ideen_2026-11-25_auftrag.md in main und führe Auftrag P aus. Arbeite auf einem eigenen Branch.
```
