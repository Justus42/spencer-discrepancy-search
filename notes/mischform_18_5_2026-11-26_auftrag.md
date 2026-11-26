# Auftrag M (Entwurf des Orchestrators, 26.11.2026, 07:53 Uhr; gilt erst nach Freigabe durch Justus): Nebenklassen plus freie Zeilen bei (18,5)

Grundlage: Auftrag N und sein Gate (`notes/laeufe/2026-11-25_leiter_nachbarzellen.md`, `notes/gate_2026-11-26_leiter_n.md`).
Stand bei (18,5), d = 8: 52 Wörter als Kern-⟨1,a⟩-Code (K = 13, Typ (9,9)); 50 Wörter aus dem Gate (ein Zustand mit K = 12 und einer offenen Klasse,
dazu **eine freie Zeile**, Matrix 25 x 18). Tabelle: 64. Rekordgrenze: 40 Wörter (20 Zeilen).
Proben des Gates (keine Suche mit Anspruch): K = 11 im Typ (9,9) endet bei Restzahl 967 (3868 offene Punkte); eine freie Zeile überdeckt davon 2076,
gierig genügen drei (wieder 50). Die freie Suche ohne Struktur endet mit 24 Zeilen bei 288 offenen Klassen. Eine gemeinsame Suche über
Nebenklassen und freie Zeilen ist nie gelaufen.

Zweck: (1) die 50 von einer Sitzung gegenrechnen (Projektregel), (2) 48 Wörter und weniger bei (18,5) suchen.
Erwartung des Orchestrators (unkalibriert): kein Rekord; 48 oder 46 sind möglich.

```
Auftrag M (Opus, Effort hoch): Codes der Form "K Nebenklassen von <1,a> plus f freie Zeilen" bei (18,5), d = 8.

Lies zuerst STATUS.md, CLAUDE.md, notes/laeufe/2026-11-25_leiter_nachbarzellen.md und notes/gate_2026-11-26_leiter_n.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_mischform_18_5.md, Logs runs/mischform/, STATUS.md und LANDKARTE.md erst am Ende.
Nichts installieren (kein pip, kein apt). Stand-Zeilen nicht ändern. Uhrzeiten nur mit date, nicht schätzen.
src/zerl_halb.c und src/zerl_offen.py nicht ändern (benutzen ja). Neuer Suchcode nach src/, eigener Prüfcode getrennt davon.

Begriffe: Ein Zustand ist ein Paar (U | W) mit K Zeilen, U K x m1, W K x m2, m1 + m2 = 18, dazu f freie Zeilen aus {+1,-1}^18.
Matrix: die 2K Zeilen (U_i, W_i) und (U_i, -W_i) und die f freien Zeilen, zusammen 2K + f Zeilen, 4K + 2f Wörter.
Ein Punkt x ist überdeckt, wenn |<Zeile, x>| >= 8 für eine Zeile (Abstand <= 5 zu der Zeile oder ihrem Negativ).
Eigener Prüfcode zählt die offenen Punkte über alle 2^18 Punkte direkt auf der Matrix, ganze Zahlen, ohne Code aus der Suche.
Ein Treffer gilt erst mit: eigener Prüfcode 0 offene Punkte UND python3 verifiers/pruefe_matrix.py MATRIX --erwartet 8 (drei Programme).

Schritt 0, Kontrollen (Abbruch, wenn eine scheitert: stoppen und berichten):
  - sha256sum -c verifiers/SHA256SUMS; gcc -O3 -march=native -o build/zerl_halb src/zerl_halb.c; build/exakt_disc bauen.
  - Gegenrechnung der 50 (Pflicht, eigener Weg): Die zwölf Zustände runs/leiter_n/s2c18-5_k12_t9-9_seed{1..4}.paar.txt und
    runs/leiter_n/s3c18-5{a,b}_k12_t9-9_seed{5..12}.paar.txt haben je genau 4 offene Punkte. Bestimme für jeden Zustand VOLLSTÄNDIG alle
    Zeilen r aus {+1,-1}^18, die diese Punkte überdecken (alle 2^18 Kandidaten durchgehen), und zähle sie. Erwartet: mindestens eine je Zustand
    (der Orchestrator hat je eine gebaut, die Anzahl kennt er nicht). Dann: notes/orchestrator/2026-11-26_gate_leiter_n/gate_c18-5_50w.matrix.txt
    mit dem eigenen Prüfcode und mit pruefe_matrix.py prüfen: erwartet 25 Zeilen, 50 verschiedene Wörter, 0 offene Punkte, Diskrepanz 8.
  - K = 11 nachstellen: ./build/zerl_halb alt 11 9 9 8 --seed S --zeit 150 --aus PAAR --neustart 20 für S = 1, 2. Erwartet (Gate): Restzahl 967,
    also 3868 offene Punkte.

Schritt 1, exakte Entscheidung für wenige freie Zeilen bei festem Zustand (neuer Code, vollständig):
  Eingabe ein Zustand, Ausgabe für f = 1 und f = 2: gibt es f freie Zeilen, die alle offenen Punkte überdecken? Vollständig, nicht gierig.
  Weg für f = 2 zum Beispiel: ein offener Punkt p muss von Zeile 1 überdeckt sein (ohne Einschränkung), also liegt Zeile 1 im Abstand <= 5
  von p oder -p; für jede solche Zeile müssen die übrigen offenen Punkte in EINER Kugel (mit Gegenkugel) liegen. Vollständigkeit im Protokoll begründen.
  Kontrollen für diesen Code, alle Pflicht:
   - Positiv f = 1: alle zwölf Zustände mit K = 12 aus Schritt 0 -> "ja", und die Zahl der Lösungen gleich der aus Schritt 0.
   - Positiv f = 2 in fremder Zelle: der 48er bei (20,6) ohne eine Nebenklasse. Nimm runs/zerlegung/k3_seed1.paar.txt (K = 12, Typ (11,9), d = 8,
     0 offene Klassen), streiche eine Zeile des Paars (K = 11) und frage nach f = 2 mit Radius 6: Antwort muss "ja" sein (die zwei Zeilen
     (U_i, W_i), (U_i, -W_i) sind eine Lösung). Für alle 12 Wahlen der gestrichenen Zeile. Dazu f = 1: erwartet "nein" (unbelegt, bitte berichten).
   - Gegen vollständiges Durchprobieren auf mindestens 8 kleinen Fällen (kleine Länge, zufällige offene Punktmengen), 0 Abweichungen.
  Erster Test (billig): die zwei Zustände mit K = 11 aus Schritt 0: f = 1 erwartet "nein" (eine Zeile überdeckt höchstens etwa 2076 von 3868),
  f = 2: Erwartung offen. "Ja" wäre ein Code mit 48 Wörtern: prüfen wie unten, weiter mit Schritt 2.

Schritt 2, viele Zustände mit K = 11: Erzeuge mit zerl_halb Zustände für K = 11 in den Typen (9,9), (10,8), (11,7), (12,6), je mindestens
  16 Startwerte zu 150 s, und entscheide für jeden f <= 2 exakt. Nimm nicht nur den Endzustand: wenn es mit vertretbarem Aufwand geht, auch
  Zwischenzustände mit kleiner Restzahl (zerl_halb nicht ändern; zum Beispiel kürzere Laufzeiten und andere --neustart-Werte).
  Protokolliere je Zustand: Restzahl, offene Punkte, größte Zahl offener Punkte, die EINE freie Zeile überdeckt, Antwort für f = 2.

Schritt 3, gemeinsame Suche (neuer Suchcode): Zustand und freie Zeilen zusammen verändern, Ziel 0 offene Punkte bei festem (K, f).
  Verfahren frei (zum Beispiel Bitwechsel in U, W und in den freien Zeilen mit Gewichten auf den offenen Punkten wie in Auftrag E/H).
  Positivkontrolle in derselben Struktur, Pflicht vor den Zielen: (K, f) = (12, 1) aus Zufallsstart, 8 Startwerte je höchstens 600 s,
  Soll mindestens 2 von 8 gelöst (das ist der 50er). Scheitert sie nach 90 Minuten Arbeit am Suchcode: Schritt 3 abbrechen und berichten;
  die Schritte 1, 2 und 4 gelten trotzdem.
  Ziele in dieser Reihenfolge, je Typ (9,9) zuerst, dann (10,8), je 8 Startwerte zu 900 s:
   48 Wörter: (11, 2), (10, 4), (9, 6)        46 Wörter: (11, 1), (10, 3), (9, 5)        44 Wörter: (10, 2), (9, 4), (8, 6)
  Eine Stufe (46, dann 44) nur beginnen, wenn die vorige mindestens einen Treffer hatte. Nach jedem Treffer: prüfen, speichern, weiter.
  Rekordkandidat (40 Wörter oder weniger, also 2K + f <= 20): sofort anhalten, prüfen, Protokoll, pushen, berichten.

Schritt 4, billig, nur gespeicherte Zustände, exakt mit dem Code aus Schritt 1 (f = 1):
   - (16,4), d = 8, Radius 4: alle 28 Zustände runs/leiter_n/*c16-4*_k15_*.paar.txt. "Ja" wären 62 Wörter (Tabelle 64).
   - (23,7), d = 9, Radius 7: alle Zustände runs/leiter_n/*c23-7*_k15_*.paar.txt. "Ja" wären 62 Wörter (Tabelle 64).
  Erwartung: überall "nein" (unbelegt). Je Zustand offene Punkte und beste Einzelzeile protokollieren.

Treffer prüfen: eigener Prüfcode (0 offene Punkte über alle Punkte) und pruefe_matrix.py --erwartet d; Matrix nach runs/mischform/.
Jeder Code mit weniger als 50 Wörtern bei (18,5) ist ein Tabellenergebnis: prüfen, speichern, weiterrechnen. Nichts nach außen melden.
Ob ein Wert neu ist, prüft der Orchestrator.

Abbruch: Schritt 0 scheitert. Sonst kein vorzeitiger Abbruch außer bei einem Rekordkandidaten.
Obergrenze: 6 Stunden Rechenzeit, davon höchstens 3 Stunden für Schritt 3. Blöcke von höchstens 30 Minuten, nach jedem Block Protokoll
aktualisieren, committen, pushen (CLAUDE.md, Regel 4).
Belegart: Treffer exakt (drei Prüfprogramme). "f <= 2 unmöglich bei Zustand Z" ist voll, aber nur für diesen Zustand. Alles Übrige ist
"Suche erfolglos" und schließt nichts aus.
Am Ende ohne Zuruf auswerten: sha256sum -c erneut; main holen (git fetch origin '+refs/heads/main:refs/remotes/origin/main', dann mergen);
STATUS.md höchstens fünf Zeilen plus Fehlversuche als Einzeiler; LANDKARTE.md mit dem besten Ergebnis je (K, f) (Regel 9).
Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/mischform_18_5_2026-11-26_auftrag.md in main und führe Auftrag M aus. Arbeite auf einem eigenen Branch. Nichts per pip installieren. Uhrzeiten nur mit date, nicht schätzen.
```
