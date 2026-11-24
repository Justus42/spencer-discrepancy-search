# Auftrag: Zweitmeinung zum Projekt Spencer-Diskrepanzkonstante (Stand 24.11.2026, 14 Uhr)

Du bist kritischer Zweitgutachter. Widerspruch ist erwünscht, Bestätigung nur, wo sie begründet ist.
Angehängt sind `STATUS.md` (vollständiges Protokoll mit allen Fehlversuchen) und `src/construct_48.py`.
Du hast keine Websuche. Literaturangaben aus dem Gedächtnis kennzeichnest du als unbelegt.

## Ziel

Untere Schranke der Spencer-Konstante verbessern (Konstante C_10c in `teorth/optimizationproblems`):
eine Matrix A in [-1,1]^(n x n) mit disc(A)/sqrt(n) > 5/sqrt(8) ≈ 1,7678, wobei
disc(A) = min über Vorzeichenvektoren x von max_i |(Ax)_i|. Rekord seit 05.09.2026: 8x8-Matrix aus
dem [7,4]-Hamming-Code plus Nullspalte, Diskrepanz 5.

## Reduktion (nicht von uns, steht auch auf der Seite des Zielrepos)

Eine Vorzeichenmatrix mit N Zeilen und m Spalten hat Diskrepanz >= m - 2R genau dann, wenn die
Hamming-Kugeln vom Radius R um alle Zeilen und ihre Komplemente den Würfel {0,1}^m überdecken.
Ein selbstkomplementärer Überdeckungscode mit 2N Wörtern liefert (m - 2R)/sqrt(max(m, N));
fehlende Spalten oder Zeilen werden mit Nullen aufgefüllt.

Rekordschwellen: (20,6) mit höchstens 40 Wörtern; (18,5) mit 40; (21,6), (23,7), (25,8) mit 50
(bei Nebenklassen zu je 4 Wörtern: 48); (30,10) mit 62.

## Vorgehen

- Rollen: Claude als Orchestrator und Prüfer an Gates. Claude Code in Cloud-Sitzungen rechnet
  (4 Kerne, Container-Neustarts etwa alle 2 Stunden, deshalb Blöcke bis 30 Minuten). Gemini als
  feindseliger Gutachter und für Literatur. Der Mensch ist Arzt, kein Mathematiker und kein
  Programmierer; er entscheidet und führt zusammen.
- Regeln: Ein Ergebnis gilt erst, wenn zwei unabhängige Prüfskripte exakt übereinstimmen. Vor jeder
  Suche eine Positivkontrolle. Fehlversuche werden protokolliert. Nichts nach außen ohne
  Neuheitsprüfung und Offenlegung des KI-Einsatzes; keine Forenposts, keine Mails an Mathematiker.
  Timebox 23.11.2026 bis 04.01.2027.
- Suchmethode: Der Code ist Vereinigung von K Nebenklassen von D = {0, a, Komplement(a), 1}
  (Dimension 2), Typ = (Gewicht(a), n - Gewicht(a)). Tabu-Suche im Syndromraum mit 2^(n-2) Syndromen;
  alle Ersetzungen einer Nebenklasse werden je Schritt per Walsh-Hadamard-Transformation bewertet.
  Warmstart durch Streichen einer Nebenklasse aus einer Lösung.

## Ergebnisse in zwei Tagen

1. **(20,6): 48 Wörter** statt 64 (Kéri-Tabelle und Post-Kéri-Datensatz: 24–64). Weg: 60, 56, 52, 48.
   Die 48er-Codes sind selbstkomplementär, normal (Norm 13 in allen 20 Koordinaten), Radius 6 per
   Brute-Force, bestätigt von drei unabhängigen Skripten. Als Spencer-Schranke: 8/sqrt(24) = 1,633,
   kein Rekord.
2. **Struktur.** Die 48er-Codes sind 20 Spalten von M = [[H,H],[H,-H]] mit H = Hadamard-Matrix der
   Ordnung 12. Vollständig nachgerechnete Regel: 1 Spalte links und 3 rechts gestrichen ergibt
   Diskrepanz 8 genau dann, wenn die linke Spalte zu den drei rechten gehört (660 Fälle), sonst 6
   (1980 Fälle). 2+2 ergibt 8 genau bei einer gemeinsamen Spalte (1320 Fälle). 0+4 nie. Bei 22 Spalten
   (1+1) ergibt sich 8 genau bei verschiedenen Spalten. Die Suche hat diese Struktur selbst gefunden;
   die Konstruktion ohne Suche liegt im Repo.
3. **Familie.** Über die amalgamierte direkte Summe mit {000,111} (Normalität) folgen 48 Wörter für
   (22,7), (24,8), (26,9), je per Brute-Force bestätigt; Tabelle: 64, 64, 56. (28,10) steht in der
   Tabelle bei 56 und ist noch nicht nachgerechnet.
4. **Starrheit.** Streicht man eine beliebige der 12 Nebenklassen, bleiben in beiden 48er-Codes exakt
   6108 Punkte offen. Ein Versuch auf 44 Wörter wurde nicht unternommen.
5. **Hadamard-Tests, alle negativ.** Ordnung 20 (3 Klassen): Diskrepanz 6. Ordnung 24 (alle 60 Klassen
   und ihre Transponierten): Diskrepanz 6, also 24 x 23 höchstens 7, Rekord bräuchte 9. Bauregel auf
   Ordnung 40 übertragen (H der Ordnung 20, 40 x 36, Rekord bei 12): für alle 30 780 Spaltenauswahlen
   gibt es einen Vorzeichenvektor mit allen Beträgen <= 8. Getestet ist nur gleiche Zeilenausrichtung
   in beiden Hälften.
6. **Rundgang durch Nachbarzellen** (Zufallsstart, Dimension 2), ebenfalls negativ: (18,5) erreicht
   64 Wörter nur mit Typ (11,7), 60 nicht (300 s je Lauf). (21,6) erreicht nicht einmal 64 Wörter
   (0 von 12 Läufen, 900 s, beste Restzahl 480 Syndrome). (23,7) erreicht 60 Wörter nicht (0 von 8,
   1800 s). Vorbehalt: Ob es in dieser Nebenklassen-Familie bei (21,6) überhaupt einen Code mit 64
   Wörtern gibt, ist nicht gesichert.
7. **(25,8) blind:** 2^23 Syndrome, 0,475 s je Schritt auf einem Cloud-Kern. Treffer bei (20,6)
   brauchten 48 000 bis 342 000 Schritte, das wären 8 bis 55 Stunden je Lauf. Nicht gestartet.
   Ein PC mit Ryzen 7 9800X3D und RTX 5080 ist vorhanden und bisher ungenutzt.
8. **Neuheit.** Vier Gemini-Recherchen, zwei davon Deep Research, fanden keine Schranke unter 64 für
   K(20,6) und keine bekannte Konstruktion für 48. Der Volltext von Östergård und Kaikkonen 1998 war
   nicht einsehbar. Die Reduktion selbst ist Vorarbeit Dritter (Zielrepo, EinsteinArena-Thread 273).

## Einschätzung des Orchestrators (ausdrücklich zur Kritik gestellt)

- Der Erfolg bei (20,6) war zellspezifisch: Dort existiert die Hadamard-Struktur der Ordnung 12.
  Außerhalb dieser Familie ist die Suche nicht konkurrenzfähig.
- Eine blinde Suche bei (25,8) ist damit nicht begründbar, auch nicht mit Grafikkarte.
- Empfehlung: Rekordjagd mit dieser Methode beenden, (28,10) nachrechnen, das Tabellenergebnis
  aufschreiben und am 27.11. über eine Meldung entscheiden. Naheliegender Ort: ein Pull Request oder
  Issue beim Repo `florath/covering-codes-lean`, das die Post-Kéri-Tabelle pflegt.

## Fragen

1. **Ist die Schlussfolgerung gerechtfertigt?** Wo ist die Argumentation schwach, was haben wir
   übersehen, welche unserer Negativbefunde tragen weniger, als wir annehmen?
2. **Konkrete, prüfbare Ideen für einen Rekord** (Diskrepanz/sqrt(max(n,N)) > 1,7678):
   a. Warum funktioniert die Hadamard-12-Struktur gerade bei n - 2R = 8? Gibt es eine Erklärung, die
      ein Gegenstück für Diskrepanz 9, 10 oder 11 nahelegt?
   b. Andere Bausteine: Konferenz- und Gewichtsmatrizen, Einträge 0 oder gebrochene Einträge in
      [-1,1], verschiedene Zeilenausrichtung [[H, P·H],[H, -P·H]], drei oder mehr Blöcke, andere
      Untergruppen D.
   c. Bessere Suchverfahren oder Symmetrievorgaben für (25,8), (23,7), (30,10). Lohnt die Grafikkarte?
   Nenne zu jeder Idee den ersten billigen Test (Minuten bis eine Stunde auf 4 Kernen), ein
   Abbruchkriterium und deine geschätzte Erfolgswahrscheinlichkeit.
3. **Tabellenergebnis.** Wie belastbar ist die Neuheit? Welche Prüfungen fehlen vor einer Meldung?
   Wo und in welcher Form sollte gemeldet werden? Ist die Aussage „K(2R+8, R) <= 48 für alle R >= 6“
   über das Normalitätsargument korrekt? Folgen aus dem 48er-Code über Standardkonstruktionen weitere
   Tabellenzellen?
4. **Vorschlag für das weitere Vorgehen bis 04.01.2027:** priorisierte Liste mit höchstens fünf
   Punkten, je mit Aufwand, Nutzen und Risiko. Sag klar, wenn du „aufhören und melden“ für richtig
   hältst.

## Form der Antwort

Deutsch. Ergebnis zuerst. Jede Tatsachenbehauptung als belegt (woher) oder unbelegt kennzeichnen.
Keine erfundenen Quellen. Eigene Rechnungen als solche kennzeichnen, damit wir sie nachrechnen können.
