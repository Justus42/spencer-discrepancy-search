Auftrag an GPT 6.2 Astra und GPT 6.2 Sol, abgegeben 24.11.2026, 18:04, angehängt STATUS.md, LANDKARTE.md, src/construct_32.py, src/construct_48.py von main.

# Auftrag: Konstruktions-Rundlauf (Ansatz 3), Stand 24.11.2026, 18 Uhr

Du bist Kombinatoriker mit Schwerpunkt Überdeckungscodes und Designs.
Gesucht sind keine Urteile über unser Vorgehen, sondern konkrete
Bauformen für Matrizen, die wir maschinell sieben. Angehängt:
STATUS.md, LANDKARTE.md, src/construct_32.py, src/construct_48.py.
Du hast keine Websuche. Kennzeichne jede Aussage: [R] eigene Rechnung
oder Herleitung, [L] Literatur aus dem Gedächtnis (unbelegt),
[U] Urteil oder Schätzung.

## Ziel
Matrix A mit Einträgen in {-1, 0, 1}, N Zeilen, m Spalten, mit
disc(A)/sqrt(max(m,N)) > 5/sqrt(8) = 1,7678.
disc(A) = min über Vorzeichenvektoren x von max_i |(Ax)_i|.
Für ganzzahliges Ziel d heißt das max(m,N) <= B(d):
d=7: 15, d=8: 20, d=9: 25, d=10: 31, d=11: 38, d=12: 46.
Reduktion: Eine +-1-Matrix hat disc >= m - 2R genau dann, wenn die
Kugeln vom Radius R um alle Zeilen und ihre Komplemente {0,1}^m
überdecken. Gebraucht wird also ein selbstkomplementärer Code der
Länge m, Radius R = (m-d)/2, mit höchstens 2*B(d) Wörtern.
Gebrochene Einträge helfen nicht: Die größte mögliche Kappe einer
Zeile ist immer eine Hamming-Kugel (Satz von Kleitman) [L].

## Offene Zellen (untere-obere Schranke laut Post-Kéri-Tabelle,
## Dichte = Wörter * Kugelvolumen / 2^m bei voller Wortzahl)
d=7, <=30 Wörter: (13,3) 28-42, Dichte 1,38; (15,4) 23-32, 1,78.
d=8, <=40: (16,4) 34-64, 1,54; (18,5) 28-64, 1,93;
  (20,6) 24-64, 2,31 (wir haben 48).
d=9, <=50: (19,5) 40-64, 1,59; (21,6) 34-64, 1,96;
  (23,7) 29-64, 2,33; (25,8) 25-64, 2,69.
d=10, <=62: (20,5) 62-128; (22,6) 49-128; (24,7) 41-128;
  (26,8) 36-128; (28,9) 30-128; (30,10) 27-128, Dichte 3,06.
d=11, <=76: (23,6) bis (31,10), bekannt je 224.
Alle anderen Zellen sind durch die untere Schranke geschlossen.

## Was funktioniert (alles exakt geprüft)
- Hamming [7,4] plus Nullspalte: 8 x 8, disc 5, Quotient 1,768 (Rekord).
- (15,4) mit 32 Wörtern: 16 x 15, disc 7, Quotient 1,750.
  Familie (2R+7, R) mit 32 Wörtern, siehe construct_32.py.
- [[H,H],[H,-H]] mit H = Paley-Hadamard 12, links 1 und rechts 3
  Spalten gestrichen (mit Überlappung): 24 x 20, disc 8, Quotient 1,633.
  Daraus (2R+8, R) mit 48 Wörtern für alle R >= 6.
  Mechanismus: max_i |(Mx)_i| = max_i (|u_i| + |v_i|) mit u = H_I x,
  v = H_J y; bei Ordnung 12 verbietet die Parität den Wert 6 fast immer.
- Automorphismengruppen dieser Matrizen (Spaltenpermutationen mit
  Vorzeichen): Ordnungen 2688, 5376, 4608, 129024, 576. Nur die
  Primfaktoren 2, 3 und 7, nie 5.

## Was ausgeschlossen ist (nicht erneut vorschlagen)
Vollständig durchgerechnet:
- Hadamard-Matrizen der Ordnung 20 (3 Klassen) und 24 (60 Klassen),
  mit Transponierten: voll disc 6; ohne 1 Spalte 7; Ordnung 20 ohne
  2 Spalten 6; Ordnung 24 ohne 3 Spalten 7.
- Doppelung [[H,H],[H,-H]] mit H der Ordnung 20, Streichmuster 1+3
  und 2+2, gleiche Zeilenausrichtung: disc <= 8, nötig wäre 12.
- Paley-Konferenzmatrizen der Ordnung 6, 10, 14, 18: disc 3, 3, 5, 5.
- Alle zirkulanten +-1-Matrizen für n = 18, 20, 21, 23, 24, 25, 26,
  29, 30, 31, dazu mit Einser-Zeile und Nullspalte für n = 18 bis 26:
  beste disc 4 bis 7. Damit sind auch alle zyklischen Differenzmengen
  dieser Ordnungen erledigt.
- Codes, die unter einer Permutation aus 5er-Zyklen plus Komplement
  invariant sind: bei (15,4) mindestens 40 Wörter (genau 525 Lösungen).
- (16,4) mit 32 Wörtern: unmöglich, K(16,4) >= 34 [L, Tabelle].
Nur Suche erfolglos (schließt nichts aus):
- Vereinigungen von Nebenklassen von {0, a, Komplement(a), 1}:
  (18,5) nur 64 Wörter, (21,6) und (23,7) nichts bis 60.
- 5er-Zyklen bei (20,6): 70 Wörter gefunden, 60 nicht.
- (15,4) mit 30 Wörtern, freie Suche: bleibt bei 466 offenen Punkten.

## Aufgabe
Schlage 8 bis 12 Bauformen vor, jede für eine bestimmte offene Zelle.
Erwünscht sind Formen mit algebraischem Kern und wenigen freien
Entscheidungen, wie bei Hamming 7 und Hadamard 12. Mögliche
Richtungen, keine Einschränkung: nichtlineare Codes als Vereinigung
weniger Nebenklassen kleiner Codes mit großer Gruppe (Reed-Muller,
Simplex, Golay und Teilcodes); Kerdock-, Preparata-, Nordstrom-
Robinson-artige Codes; amalgamierte direkte Summen und andere
Verklebungen normaler Codes; +-1-Inzidenzmatrizen nichtzyklischer
Designs und stark regulärer Graphen; gruppenentwickelte Matrizen
über nichtzyklischen Gruppen der Ordnung 16 bis 25; Zeilen mit
Nullen (Kugel in weniger Koordinaten); Blockformen mit
verschiedenen Bausteinen links und rechts.

Je Bauform genau diese Punkte:
1. Name, Zielzelle (m, R), Wortzahl, d, Quotient bei Erfolg.
2. Dichteprüfung [R]: Wortzahl * Kugelvolumen / 2^m und Vergleich
   mit der unteren Schranke oben. Vorschläge unter Dichte 1 oder
   unter der unteren Schranke entfallen.
3. Warum der Radius erreichbar sein könnte: ein Argument, kein Gefühl.
   Nenne den Mechanismus (Parität, Gewichtsverteilung, Normalität).
4. Python-Funktion nach diesem Vertrag:
     def kandidaten():
         # liefert Paare (name, M); M = Liste von Zeilen,
         # Einträge in {-1, 0, 1}, höchstens B(d) Zeilen und Spalten
         yield ...
   Nur Standardbibliothek und numpy, deterministisch, höchstens
   60 Zeilen, höchstens 200 000 Matrizen, Erzeugung unter 60 s.
   Kleine Parameterfamilien (welche Spalten, welche Verschiebung,
   welche Differenzmenge) sind ausdrücklich erwünscht. Schreibe
   keinen Code, der Vorzeichenvektoren sucht; das Sieben macht
   unser Prüfstand (einige tausend Matrizen je Sekunde bis m = 26,
   exakte Prüfung bis m = 31).
5. Billigster Gegentest: Woran sieht man in Sekunden, dass es
   nicht geht?
6. Geschätzte Chance [U], unkalibriert.

Am Ende: Rangfolge, die zwei Bauformen, die du zuerst testen
würdest, und eine klare Aussage, falls du keine für aussichtsreich
hältst. Wenn du für eine offene Zelle einen Grund kennst, warum sie
unerreichbar ist, nenne ihn zuerst.

Form: Deutsch, Ergebnis zuerst, keine erfundenen Quellen.
