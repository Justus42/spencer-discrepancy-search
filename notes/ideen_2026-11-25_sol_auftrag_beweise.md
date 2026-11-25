# Auftrag: vier enge Fragen zur zerlegten Doppelungsfamilie und zu Zeilen mit Nullen (Stand 25.11.2026, 16:30 Uhr)

Du bist Kombinatoriker mit Schwerpunkt Überdeckungscodes, extremale Mengenlehre und Diskrepanz. Du beweist oder widerlegst; du rätst nicht.
Du hast keine Websuche. Kennzeichne jede Aussage: [R] eigene Rechnung oder Herleitung (lückenlos), [L] Literatur aus dem Gedächtnis
(unbelegt), [U] Urteil oder Schätzung. Deutsch, Ergebnis zuerst, keine erfundenen Quellen.

**Auflage: Jeder Schritt trägt eine Zahl, die wir nachrechnen können** (Anzahl, Volumen, Schnittgröße, Profil, Schranke). Schritte ohne solche
Zahl zählen nicht. Ein vollständiger Beweis zu einer Frage zählt mehr als Teilideen zu allen. Wenn ein Beweis eine Lücke hat, benenne sie genau.
Keine neuen Bauformen vorschlagen, außer eine Frage verlangt ausdrücklich eine Konstruktion; dann als vollständige Matrix.

Angehängt: `STATUS.md` (Ziel, offene Zellen, alle Negativbefunde mit Belegart), `LANDKARTE.md` (bester Wert je Familie und Größe),
`notes/vortest_2026-11-25_spann.md` (Zählschranke im Spann), `notes/ideen_2026-11-25.md` (Vortests von heute, nicht gegengerechnet).

## Stand

**Ziel.** Matrix A in [-1,1]^(n x n) mit disc(A)/sqrt(n) > 5/sqrt(8) = 1,7678; disc(A) = min über x in {-1,1}^n von max_i |<a_i,x>|.
Rekord: n = 8, Hamming [7,4] plus Nullspalte, disc 5. Reduktion: Eine ±1-Matrix mit N Zeilen und m Spalten hat disc ≥ m − 2R genau dann, wenn
die Kugeln vom Radius R um alle Zeilen und ihre Komplemente {0,1}^m überdecken. Gebraucht wird ein selbstkomplementärer Überdeckungscode der
Länge m, Radius R = (m − d)/2, mit höchstens 2·B(d) Wörtern und m ≤ B(d): B(7) = 15, B(8) = 20, B(9) = 25, B(10) = 31, B(11) = 38.

**Offene Zellen** (m,R), untere bis obere Schranke der Tabelle, in Klammern die erlaubte Wortzahl:
d = 7 (30): (13,3) 28–42; (15,4) 23–32. d = 8 (40): (16,4) 34–64; (18,5) 28–64; (20,6) 24–64, wir haben 48.
d = 9 (50): (19,5) 40–64; (21,6) 34–64; (23,7) 29–64; (25,8) 25–64. d = 10 (62): (20,5) bis (30,10), bekannt je 128. d = 11 (76): bekannt je 224.

**Was trägt** (exakt geprüft): (15,4) mit 32 Wörtern = punktierte Hadamard-16 vom 2-Rang 8, 16 x 15, disc 7, Quotient 1,750.
(20,6) mit 48 Wörtern = M = [[H,H],[H,−H]], H = Paley-Hadamard 12, links 1 und rechts 3 Spalten gestrichen (die linke gehört zu den drei rechten),
24 x 20, disc 8, Quotient 1,633; in Lean bewiesen.

**Ausgeschlossen, Belegart „voll“ (vollständig durchgerechnet):** Hadamard 20 und 24 voll, ohne 1 Spalte, 20 ohne 2, 24 ohne 3 Spalten (disc 6 bzw. 7);
(15,4) mit 30 Wörtern unter Spaltensymmetrie der Ordnung 5 (5+5+5), 7, 11, 13; Zwei-Bahnen-Codes bei (20,6), (23,7), (25,8), (26,8), (28,9), (30,10);
Bahnen unter der 3-Sylowgruppe des 48ers (≤ 46 Wörter); (15,4) mit 30 Wörtern im affinen Spann beider 32er-Klassen (Rang 8) und in 3 von 9 Klassen
von Rang-9-Erweiterungen; n = 11 mit einer Null je Zeile. **Belegart „Gegenbeispiel je Fall“:** alle zirkulanten ±1-Matrizen n = 18–31 (auch mit
Rand), alle Gruppenmatrizen über den 10 nichtzyklischen Gruppen der Ordnung 18, 20, 21, 25; Doppelung der Ordnung 40. **Nur „Suche erfolglos“
(schließt nichts aus):** freie Suche (15,4) mit 30 Wörtern (bleibt bei 466 offenen Punkten = 32er ohne ein Paar); 46 Wörter im Spann der 48er;
Nebenklassen-Suche unter 48 bei (20,6). Einzelheiten in den Anhängen.

**Zählschranke im Spann.** L = affiner Spann eines Codes (2^r Wörter), μ(x) = Zahl der Wortpaare von L im Radius R um x. Ein Code aus k Paaren von L
braucht k ≥ 2^(r−1) / min μ. Bei (15,4): 16 für den Spann des 32ers (scharf). Bei (20,6), Spann der 48er (r = 12): 9,66; schließt 20 Paare nicht aus.

## Die zerlegte Doppelungsfamilie (Grundlage für Frage 1 bis 3)

Ein Code mit Verschiebungskern {0, a, 1+a, 1}, a = Einsen genau auf den letzten m2 Koordinaten, und K Nebenklassen ist ein Paar (U, W):
U ist K x m1, W ist K x m2, Einträge ±1; die 4K Wörter sind (U_i, W_i), (U_i, −W_i) und ihre Komplemente. Mit u = U y, v = W z gilt
max über diese Zeilen von |<Zeile,(y,z)>| = max_i (|u_i| + |v_i|). Also:

    (*)  Überdeckung mit d  ⟺  für alle y in {±1}^m1, z in {±1}^m2:  max_i (|u_i| + |v_i|) ≥ d.

Rekorde in dieser Familie: d = 8, m1 + m2 = 20, K ≤ 10. Oder d = 9, m1 + m2 ≤ 25 ungerade, K ≤ 12 (48 Wörter ≤ 50).

Von uns exakt gerechnet (Punkte modulo Vorzeichen, H = Paley-Hadamard 12):
- Kappenvolumen (beide Vorzeichen), Länge 9: |v| ≥ 3: 260 von 512; ≥ 5: 92; ≥ 7: 20; ≥ 9: 2. Länge 11: |u| ≥ 3: 1124 von 2048; ≥ 5: 464; ≥ 7: 134; ≥ 9: 24.
  Länge 13: |v| ≥ 5: 2·1093 von 8192; ≥ 9: 2·92. Länge 14: |v| ≥ 4: 2·3473 von 16384; ≥ 6: 2·1471; ≥ 8: 2·470.
- U = H ohne eine Spalte (12 x 11), 1024 Klassen y, Profile |u| sortiert: (7,5,5,3,3,3,1⁶) 660-mal; (5³,3⁶,1³) 220-mal; (9,3⁵,1⁶) 132-mal; (11,1¹¹) 12-mal.
  Im Profil (5³,3⁶,1³) durchläuft die Menge F der drei 5er-Zeilen alle 220 Dreiermengen genau einmal; die Menge O der drei 1er-Zeilen ist durch F bestimmt.
  Im Profil (7,5,5,3,3,3,1⁶) kommt jedes Paar (7er-Zeile s, 5er-Paar {a,b} aus den übrigen 11) genau einmal vor (12·55 = 660); die drei 3er-Zeilen sind dadurch bestimmt.
- W = H ohne drei Spalten (12 x 9), 256 Klassen z: (7,5,3,3,3,1⁷) 72; (5,5,5,3,3,3,1⁶) 72; (5,5,3⁶,1⁴) 54; (7,3⁶,1⁵) 36; (9,3,3,1⁹) 12; (5⁴,1⁸) 9; (3¹²) 1.
  Mit diesem (U, W) gilt (*) für d = 8 (der 48er, K = 12).
- U = H voll (12 x 12), 2048 Klassen y: (6³,2⁹) 880-mal mit 220 Zeilenmustern (alle Dreiermengen); (8,4⁵,0⁶) 792; (4⁹,0³) 220 mit 220 Zeilenmustern
  (alle Dreiermengen als Nullzeilen); (10,2¹¹) 144; (12,0¹¹) 12.
- Lemma [R, von uns]: Hat ein y nur |u_i| ≤ 1 (m1, m2 ungerade, d = 8), so müssen die 2K Wörter von W allein Q_m2 mit |v| ≥ 7 überdecken; für m2 = 9
  heißt das 2K·10 ≥ 512, also K ≥ 26. Streicht man aus U des 48ers eine Zeile, so ist diese Zeile selbst ein solches y. Also gibt es kein K = 11 oder 10
  mit U aus Zeilen von „H ohne eine Spalte“.
- Versuch: U = 8 Hamming-Zeilen (8 x 7) mit zwei verdreifachten Spalten (8 x 11, jedes y hat eine Zeile mit |u| ≥ 5), W = 8 Hamming-Zeilen mit einer
  verdreifachten Spalte (8 x 9, jedes z hat |v| ≥ 5). Für jede der 8! Zeilenzuordnungen sind genau 24 192 von 262 144 Klassen (y,z) offen (d = 8).

## Frage 1 (Prüfung): d = 9, U = H voll (12 x 12), W beliebig 12 x 13

Unsere Herleitung: Kritisch sind nur (6³,2⁹) (6er-Zeilen brauchen |v| ≥ 3, übrige ≥ 7) und (4⁹,0³) (4er-Zeilen brauchen |v| ≥ 5, Nullzeilen ≥ 9),
jeweils mit allen 220 Dreiermengen. Daraus für jedes z: (eine Zeile mit |v| ≥ 9) oder (mindestens 4 Zeilen mit |v| ≥ 5 und dazu: eine Zeile mit |v| ≥ 7
oder höchstens zwei Zeilen mit |v| = 1). Zählung der Inzidenzen „Wort im Abstand ≤ 4“: 24·1093 = 26 232. Nötig: jeder Punkt außerhalb der 24 Kugeln
vom Radius 2 (zusammen höchstens 2208 Punkte) viermal, jeder Punkt darin mindestens einmal: 2208 + 4·5984 = 26 144. Spielraum 88. Je zwei Zeilen haben
mit passendem Vorzeichen Abstand δ ≤ 6; dann liegen mindestens 15 Punkte der Radius-2-Kugel des einen Worts auch im Radius 4 des anderen. Summe über
24 Wörter und 11 fremde Zeilen: mindestens 3960 überzählige Inzidenzen, erlaubt sind (mit Überlappung der kleinen Kugeln) höchstens 88 + 21·29 = 697.
Also kein W. **Aufgabe:** Prüfe jede Zahl und jeden Schluss; nenne den ersten Fehler oder bestätige. Gilt dasselbe für m2 = 9 und 11 (dort ist schon
der Spielraum negativ: −872, −1760)?

## Frage 2 (Entscheidung): d = 9, U = H ohne eine Spalte (12 x 11), W beliebig 12 x m2 mit m2 in {10, 12, 14}

Anforderungen an |v| je Zeile: Profil (5³,3⁶,1³): 5er-Zeilen ≥ 4, 3er-Zeilen ≥ 6, 1er-Zeilen ≥ 8. Profil (7,5,5,3,3,3,1⁶): 7er-Zeile ≥ 2, 5er-Zeilen ≥ 4,
3er-Zeilen ≥ 6, 1er-Zeilen ≥ 8. Unsere Beobachtung [U]: Für jedes z ohne Zeile mit |v| ≥ 6 müssen mindestens 10 Zeilen |v| ≥ 4 haben; im Mittel haben
nur 24·V(m2, (m2−4)/2)/2^m2 = 4,1 / 4,7 / 5,1 Zeilen |v| ≥ 4. **Aufgabe:** Beweise mit einer vollständigen Zählung, dass es kein W gibt (für welche m2),
oder gib ein W als Matrix an. Die Zeilen mit |v| ≥ 6 und ≥ 8 können ausgleichen; die Zählung muss das einrechnen.

## Frage 3 (notwendige Bedingungen): d = 8, K = 10, U und W beliebig

Typen (m1, m2) mit m1 + m2 = 20: ungerade (19,1), (17,3), (15,5), (13,7), (11,9); gerade (18,2), (16,4), (14,6), (12,8), (10,10).
**Aufgabe:** (a) Für jeden Typ die Stufenbedingungen aus (*) mit Volumen: Welche Typen fallen schon durch Zählen? Tabelle mit den Zahlen.
(b) Für Typ (11,9): die schärfsten notwendigen Bedingungen an U (10 x 11) und W (10 x 9), zum Beispiel: Für jedes y mit max |u_i| = 3 auf der
Zeilenmenge S(y) müssen die Zeilen von W in S(y) zusammen mit den übrigen (die |v| ≥ 7 brauchen) Q9 überdecken; mit K(9,2) = 16 [L] folgt |S(y)| ≥ ?.
Formuliere jede Bedingung so, dass ein Suchprogramm sie als Schnitt verwenden kann. (c) Beweise, falls möglich, dass K = 10 für Typ (11,9) unmöglich ist,
oder benenne genau, welche Profilverteilung von U nötig wäre (wie viele y dürfen max |u| = 3, 5 haben, mit welchen Zeilenmengen).
(d) Erkläre die Zahl 24 192 und ihre Unabhängigkeit von der Zuordnung; folgt daraus eine untere Schranke für die offenen Klassen bei jedem
Paar aus einem (11,3)-Code mit 16 Wörtern und einem (9,2)-Code mit 16 Wörtern?

## Frage 4 (Dominanz): Zeilen mit Nullen bei N = 15

Bei N = 15 zählt eine Zeile mit ungeradem Träger s für x, wenn |<a,x>| ≥ 7, mit geradem Träger, wenn ≥ 8. Volumen: s = 15: 3882 Punkte von 32 768;
s = 13: 3024; s = 11: 2144; s = 14: 1880. Die Kappe einer Zeile mit Träger 13 (Mitte c, zwei freie Koordinaten) liegt in keiner Kugel vom Radius 4,
aber in der Vereinigung der Kugeln um (c,00) und (c,11); die Kugel um (c,00) hat 715 Punkte, die der Kappe fehlen, die Kappe 286, die der Kugel fehlen.
Exakt gerechnet: Von `ctrl_16` ohne eine Zeile (233 offene Klassen) aus verbessert keine Änderung von bis zu drei Einträgen einer Zeile auf Werte in
{−1,0,1} die Zahl 233. **Aufgabe:** Beweise oder widerlege: Gibt es 15 Zeilen in {−1,0,1}^15, die überdecken, so gibt es auch 15 Zeilen in {±1}^15.
Wenn weder Beweis noch Gegenbeispiel gelingt: ein Zählargument mit Zahlen, das sagt, wie viele Zeilen mit Nullen eine Lösung höchstens haben kann.

## Form der Antwort

Je Frage: Ergebnis in einem Satz, dann die Schritte mit Zahlen. Am Ende eine Zeile je Frage mit dem Status „bewiesen“, „widerlegt“, „Konstruktion“,
„Teilresultat“ oder „offen“. Wenn du in unseren Zahlen oben einen Fehler findest, nenne ihn zuerst.
