# Auftrag Teil A: Ideen und Methodentransfer zur Spencer-Schwelle (Stand 25.11.2026, 16:44 Uhr)

Du bist Kombinatoriker mit Schwerpunkt Überdeckungscodes, extremale Mengenlehre und Diskrepanz, mit Überblick über Suchverfahren.
Du hast keine Websuche. Kennzeichne jede Aussage: [R] eigene Rechnung oder Herleitung, [L] Literatur aus dem Gedächtnis (unbelegt),
[U] Urteil oder Schätzung. Deutsch, Ergebnis zuerst, keine erfundenen Quellen.

Angehängt: `STATUS.md` (Ziel, offene Zellen, alle Negativbefunde mit Belegart, Fehlversuche), `LANDKARTE.md` (bester Wert je Familie und Größe),
`notes/vortest_2026-11-25_spann.md` (Zählschranke im Spann).

## Worum es geht

Gesucht ist eine Matrix A in [-1,1]^(n x n) mit disc(A)/sqrt(n) > 5/sqrt(8) = 1,7678; disc(A) = min über x in {-1,1}^n von max_i |<a_i,x>|.
Rekord: n = 8, Hamming [7,4] plus Nullspalte, disc 5. Reduktion: Eine ±1-Matrix mit N Zeilen und m Spalten hat disc ≥ m − 2R genau dann, wenn die
Kugeln vom Radius R um alle Zeilen und ihre Komplemente {0,1}^m überdecken. Ein Rekord braucht einen selbstkomplementären Überdeckungscode der
Länge m, Radius R = (m − d)/2, mit höchstens 2·B(d) Wörtern, m ≤ B(d): B(7) = 15, B(8) = 20, B(9) = 25, B(10) = 31, B(11) = 38.

Oberste Zellen je d, erlaubte Wortzahl, bekannt, Dichte bei erlaubter Wortzahl:
(15,4): 30 erlaubt, bekannt 32, Dichte 1,78. (20,6): 40 erlaubt, bekannt 48 (von uns; Tabelle 64), 2,31. (25,8): 50 erlaubt, bekannt 64, 2,69.
(30,10): 62 erlaubt, bekannt 128, 3,06. (37,13): 76 erlaubt, außerhalb der Tabelle, 3,76. Übrige offene Zellen in `STATUS.md`.

Was trägt: (15,4) mit 32 Wörtern ist eine punktierte Hadamard-16 vom 2-Rang 8 (16 x 15, disc 7, Quotient 1,750). (20,6) mit 48 Wörtern ist
[[H,H],[H,−H]] mit H = Paley-12, links 1 und rechts 3 Spalten gestrichen (24 x 20, disc 8, Quotient 1,633).

Seit dem 24.11. abends sind sechs Ansätze negativ ausgegangen, dazu heute vier Vortests. In Stichworten, mit Belegart:
- vollständig durchgerechnet: Hadamard 20 und 24 (voll und mit bis zu 3 gestrichenen Spalten); (15,4) mit 30 Wörtern unter Spaltensymmetrie der
  Ordnung 5, 7, 11, 13; Zwei-Bahnen-Codes bei (20,6) bis (30,10); Bahnen unter der 3-Sylowgruppe des 48ers; 30 Wörter im affinen Spann der 32er
  (Zählschranke k ≥ 2^(r−1)/min μ = 16); n = 11 mit einer Null je Zeile.
- Gegenbeispiel je Fall: alle zirkulanten ±1-Matrizen n = 18 bis 31, Gruppenmatrizen über den nichtzyklischen Gruppen der Ordnung 18, 20, 21, 25,
  negazyklische und Williamson-artige Matrizen n = 18 bis 30 (Vortest), Doppelung der Ordnung 40.
- nur Suche erfolglos: freie Suche (15,4) mit 30 Wörtern (endet immer bei „32er ohne ein Paar“, 466 offene Punkte; auch mit Nullen als Einträgen
  verbessert keine Änderung von bis zu drei Einträgen einer Zeile diesen Stand); Nebenklassen-Suche unter 48 bei (20,6); 46 Wörter im Spann der 48er.
- Lehre aus einem früheren Rundlauf: 16 von Modellen vorgeschlagene Bauformen, 388 503 Matrizen, 0 Überlebende, bester Quotient 1,400.
  **Deshalb keine Listen von Matrizenfamilien zum Sieben.**

## Auflagen

1. Nichts vorschlagen, was in den Anhängen als ausgeschlossen oder erfolglos durchsucht steht; wenn doch, sage genau, was anders ist.
2. Jeder Vorschlag trägt eine Zahl, die wir in Minuten nachrechnen können (Volumen, Anzahl, Dichte, Schranke, erwartete Restzahl), und sagt,
   welches bekannte Objekt die Methode zuerst wiederfinden muss (Hamming 8 x 7, der 32er, der 48er).
3. Lieber drei durchdachte Vorschläge als zehn skizzierte. Wenn du keinen für tragfähig hältst, sage das und begründe es mit Zahlen.

## Fragen

**1. Welche Annahme ist am ehesten falsch oder zu eng?** Wir suchen fast nur ±1-Einträge, fast nur die oberste Zelle je d, fast nur N ≈ m und fast nur
Codes mit großer Symmetrie oder kleinem Verschiebungskern. Nenne höchstens drei Annahmen, deren Aufgabe den Suchraum wirklich ändert, je mit einem
Rechenbeleg, dass dort Platz ist (oder dass dort keiner ist).

**2. Höchstens fünf Ansätze, die bei uns fehlen.** Je Ansatz genau: (a) Vorbild, also eine bekannte Konstruktion, ein Satz oder ein Verfahren aus
einem anderen Gebiet, das hier noch nicht angewandt wurde; (b) Mechanismus in zwei Sätzen mit einer Zahl; (c) billigster erster Test und welche Zahl
bei Erfolg, welche bei Misserfolg herauskäme; (d) Abbruchkriterium; (e) Positivkontrolle. Erwünscht ist Methodentransfer, zum Beispiel aus
SAT- und ILP-Technik (Symmetriebrechung, Zerlegung, gegenbeispielgeführte Synthese), aus der Gittertheorie (nächster Gitterpunkt in der
Maximumsnorm), aus der Diskrepanztheorie (erbliche und lineare Diskrepanz, Determinantenschranke), aus der Theorie normaler und subnormaler
Codes, aus Mehrfachüberdeckungen, aus Designs und Zwei-Graphen.

**3. Bewertungsleiter für eine evolutionäre Programmsuche.** Vorbild: LLM-gesteuerte Suche, bei der nicht Objekte, sondern Suchprogramme verändert
und gegen einen exakten Prüfer bewertet werden (Ramsey-Zahlen: Bewertung über einen gültigen Graphen plus Bonus für einen größeren, fast gültigen;
lineare Codes: Bewertung (d, −A_d)). Unser Prüfer zählt offene Punkte exakt. Bekannte Stützstellen: (15,4) mit 30 Wörtern: bester Stand 466 offene
Punkte von 32 768; (20,6) mit 44 Wörtern aus dem 48er: 6108 von 1 048 576; (20,6) mit 46 Wörtern im Spann: 886 offene Punktklassen. Schlage eine
Bewertung vor, die zwischen diesen Stützstellen ein brauchbares Gefälle hat, und begründe mit Zahlen, warum die bloße Restzahl es nicht hat
(falls du das meinst). Welche Zwischenziele wären echte Stufen (zum Beispiel 31 Wörter ohne Selbstkomplementarität, 44 Wörter, andere Zelle)?

**4. Wo würdest du mit 10 CPU-Stunden suchen?** Genau ein Vorhaben, mit Zelle, Suchraum, erstem Test und der Zahl, an der du den Abbruch festmachst.

**5. Rangfolge der fünf obersten Zellen** nach einer Größe, die du selbst ausrechnest und benennst (nicht nach Gefühl). Wenn nach deinen Zahlen
keine Zelle erreichbar aussieht, sage das.

## Form der Antwort

Zuerst drei Sätze: Was würdest du an unserer Stelle als Nächstes tun, und was würdest du lassen? Dann die Fragen 1 bis 5. Am Ende je Vorschlag
eine Zeile: Chance [U] (unkalibriert), Aufwand in CPU-Stunden, erster Test.
