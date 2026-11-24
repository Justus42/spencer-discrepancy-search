# Auftragstext der zweiten Rückfrage an GPT 6.2 Sol (24.11.2026, 22:47)

Freigabe durch Justus am 24.11.2026, 22:46. Batch-Auftrag `openrouter:batch-1795556852-ov8lEKq1XIgR9lKn4gYo`, Modell
`openai/gpt-6.2-sol`, Denkstufe xhigh, Ausgabegrenze 65 536 Tokens, keine Dateien angehängt. Antwort:
`notes/openrouter/rueckfrage2_2026-11-24--openai-gpt-6.2-sol.md` auf diesem Branch. Systemrolle wie bei der ersten
Rückfrage (`notes/rueckfrage_2026-11-24_auftrag.md`). Die Zahlen zu n = 14 stammen aus einer eigenen Rechnung des
Orchestrators (nicht gegengerechnet; Auftrag A, Teil 4 nennt dieselben Grundzahlen 57344 / 756 / 2392).

```
# Zweite Rückfrage: n = 14 (und n = 17 an der Schranke), Stand 24.11.2026, 22:50

Du hast keine Websuche. Kennzeichne jede Aussage: [R] eigene Rechnung oder Herleitung (lückenlos), [L] Literatur aus dem Gedächtnis (unbelegt), [U] Urteil oder Schätzung. Deutsch, Ergebnis zuerst, keine erfundenen Quellen.

WICHTIG zur Form: Deine Antwort wird maschinell nachgerechnet. Gib zu JEDEM Beweisschritt eine prüfbare Zahl oder eine endliche Aussage an, die ein kurzes Programm bestätigen oder widerlegen kann (z. B. "für jeden der 57344 Zeilen gilt ...", "die Summe über alle 8192 Klassen ist ...", "höchstens x Klassen haben Eigenschaft y"). Nummeriere diese Prüfpunkte P1, P2, ... Ein kurzer Beweis mit fünf prüfbaren Punkten ist mehr wert als ein langer ohne. Wenn ein Schritt eine Lücke hat, benenne sie genau.

## Stand
Gesucht: n x n-Matrix A mit Einträgen in [-1,1] und disc(A)/sqrt(n) > 5/sqrt(8), disc(A) = min über x in {-1,1}^n von max_i |<a_i,x>|. Eine Zeile mit genau einer Null und n-1 Einträgen +-1 trifft x, wenn |<a,x>| >= d.

Deine erste Antwort (heute, 22:29) ist geprüft: Für ungerades n ist die Treffermenge einer solchen Zeile, in den Vertretern geraden Gewichts und nach Streichen einer gemeinsamen Koordinate, die Vereinigung zweier gewöhnlicher Kugeln vom Radius r im (n-1)-Würfel. Nachgerechnet für alle 5632 Zeilen bei n = 11 (0 Abweichungen) und eine Stichprobe bei n = 17. Damit ist n = 11 erledigt (K(10,2) >= 24 laut Tabelle; dein Beweis für >= 23 kommt dazu). Die zwei Zentren einer Zeile sind komplementär bis auf höchstens eine Koordinate.

## Frage 1 (Hauptfrage): n = 14, d = 7
14 Zeilen, jede mit genau einer Null und 13 Einträgen +-1. Zeile a trifft x, wenn der Abstand von x zu a auf den 13 aktiven Koordinaten <= 3 oder >= 10 ist. n ist gerade, deine Umkodierung greift nicht (Komplement erhält die Parität).

Exakt gerechnet (Punkte modulo Vorzeichen: 8192 Klassen):
- 57344 Zeilen (modulo Vorzeichen), jede trifft 756 Klassen. 14 * 756 = 10584, Budget für Mehrfachüberdeckung 2392, Dichte 1,292.
- Schnittgröße in Klassen, a = (0, 1, ..., 1):
  - b mit Null in derselben Spalte, Abstand delta auf den 13 übrigen Koordinaten (modulo Komplement, delta = 1..6): 316, 316, 136, 136, 40, 40. Anzahl solcher b: 13, 78, 286, 715, 1287, 1716.
  - b mit Null in anderer Spalte, Abstand e auf den 12 gemeinsamen aktiven Koordinaten (modulo Vorzeichen von b und Vorzeichen der Koordinate, in der a null ist; e = 0..6): 536, 316, 226, 136, 88, 40, 40. Anzahl: 26, 312, 1716, 5720, 12870, 20592, 12012.
  - Je zwei Zeilen schneiden sich also in mindestens 40 Klassen.
- Alle Nullen in einer Spalte: selbstkomplementärer Code bei (13,3) mit 28 Wörtern; Tabelle: 28 <= K(13,3) <= 42 [L]. Nicht ausgeschlossen, aber genau an der unteren Schranke.
- Zirkulante mit Nulldiagonale: größte Diskrepanz 5. Ausgeschlossen.
- Deine lokale Ungleichung n(n*S_k - 2^(3k)) >= (k-2)(2^(3k) - n*I_k) ist für k = 4 erfüllt (16744 >= 5616), schließt also nichts aus.

Aufgabe 1: Beweise, dass 14 solche Zeilen {-1,1}^14 nicht überdecken, oder gib 14 Zeilen an. Mögliche Wege: eine Umkodierung für gerades n (z. B. Vertreter mit x_0 = +1 und Fallunterscheidung, ob die Null in Spalte 0 steht; oder Einbettung in n = 15); ein Exzessargument mit Zweierflips wie bei K(10,2) >= 23; ein Paritäts- oder Charakterschnitt wie dein chi_j; die Tatsache, dass sich je zwei Zeilen in >= 40 Klassen schneiden (Bonferroni dritter Ordnung, Waldschnitt mit Gleichheitsbedingung).
Falls kein Beweis: die schärfsten notwendigen Bedingungen (Nullverteilung, erlaubte Paartypen mit Anzahlen), als Schnitte für eine Suche, jeweils mit Prüfpunkt.

## Frage 2: n = 17, d = 8, genau an der Schranke
Nach deiner Umkodierung: 17 Zeilen mit je einer Null überdecken genau dann, wenn 34 Kugeln vom Radius 4 den 16-Würfel überdecken, wobei die Zentren 17 Paare bilden, jedes Paar komplementär bis auf höchstens eine Koordinate. Tabelle: 34 <= K(16,4) <= 64 [L]. V(16,4) = 2517, 34 * 2517 / 65536 = 1,306.
Aufgabe 2: Schließt ein Exzessargument (wie bei K(10,2) >= 23) die 34 aus, zumindest unter der Paarbedingung? Gib die Ungleichung mit Zahlen an. Wenn nicht: Was ist die beste untere Schranke, die du für K(16,4) unter der Paarbedingung beweisen kannst?

## Frage 3 (kurz)
Gilt deine Umkodierung sinngemäß auch für Zeilen mit einer Null bei ungeradem n, wenn mehr als n Zeilen erlaubt sind (z. B. n = 19 Spalten, 19 Zeilen, d = 8: 38 Kugeln bei (18,5), Tabelle 28 <= K(18,5) <= 64)? Ein Satz mit Begründung genügt.

Am Ende: eine Zeile je Frage mit Status "bewiesen", "Konstruktion", "Teilresultat" oder "offen", und die Liste der Prüfpunkte P1, P2, ... mit je einer Zeile.
```
