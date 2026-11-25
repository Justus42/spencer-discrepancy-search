# Auftrag S (freigegeben 25.11.2026): Suche im Spann

Freigabe durch Justus im Orchestrator-Chat, 25.11.2026. Läuft parallel zu Auftrag G (`notes/gruppen_2026-11-25_auftrag.md`).
Grundlage: `notes/vortest_2026-11-25_spann.md` samt Nachtrag.

```
Auftrag S (Opus, Effort hoch): Kleinere Codes im affinen Spann bekannter guter Codes

Lies zuerst STATUS.md, LANDKARTE.md, CLAUDE.md und notes/vortest_2026-11-25_spann.md. Eigener Branch, Protokoll
notes/laeufe/2026-11-25_spann.md, STATUS.md erst am Ende. Blöcke von höchstens 30 Minuten, nach jedem Block committen
und pushen. Zwischendateien über 1 MB nicht ins Repo. Zählcode, Suchcode und Prüfcode getrennt. Vor jedem
Treffer-Bericht sha256sum -c verifiers/SHA256SUMS und python3 verifiers/pruefe_matrix.py.
Im Container erlaubt: pip install --break-system-packages ortools python-sat igraph; apt-Pakete für SAT-Löser.
Parallel läuft Auftrag G auf einem anderen Branch: die Stand-Zeile (Zeile 3) von STATUS.md und LANDKARTE.md nicht ändern.

Begriffe: C selbstkomplementärer Code der Länge m, Radius R. L = affiner Spann = kleinster affiner Unterraum über GF(2),
der C und alle Komplemente enthält, 2^r Wörter, 2^(r-1) Wortpaare {w, ~w}. mu(x) = Zahl der Paare von L im Radius R um x
(Paar zählt, wenn w oder ~w im Radius liegt); mu ist auf Nebenklassen von L konstant.
Zählschranke: Ein Code aus k Paaren von L braucht k >= 2^(r-1) / min mu.
Gesucht: Codes aus weniger Paaren, alle Paare aus L (oder aus einer Erweiterung von L um eine Dimension).

Teil 1, Gegenrechnung mit eigenem Programm (notes/orchestrator/2026-11-25_vortest_spann/ nicht benutzen). Erwartet:
 a) Rang r und Kern (Zahl der Verschiebungen t mit C + t = C):
    ctrl_8 4 / 16; ctrl_16 8 / 2; ctrl_17 8 / 4; L2026_17x17 8 / 4; astra_19 8 / 2; bahn15_40w 11 / 2;
    had24_1p3_paley12 12 / 4; had24_2p2_paley12 12 / 4.
 b) mu-Werte (Wert: Zahl der Nebenklassen von L), Radius der Zelle:
    ctrl_8 (7,1): 1: 8.
    ctrl_16 (15,4): 8: 1, 14: 56, 16: 70, 29: 1.
    ctrl_17 und L2026_17x17 (17,5): 8: 1, 12: 42, 15: 192, 16: 147, 22: 8, 25: 7, 28: 49, 29: 59, 32: 7.
    astra_19 (19,6): 8: 15, 14: 560, 16: 700, 22: 1, 23: 70, 28: 350, 29: 15, 30: 210, 43: 1, 48: 42, 52: 70, 55: 14.
    had24_1p3, had24_2p2 und runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt (20,6): 212: 36, 240: 219, 268: 1.
    runs/gegenrechnung15/loesung0_16x15.txt: wie ctrl_16. runs/gegenrechnung15/loesung111_16x15.txt (zweite Klasse):
    Rang 8, 12: 14, 15: 64, 16: 49, 29: 1.
 c) Erweiterungen von L(ctrl_16) um eine der 127 Nebenklassen (Rang 9, 256 Paare), sortiert nach kleinstem mu:
    56 mit 22, 70 mit 24, eine mit 28.
 Bei Abweichung in a) bis c): stoppen und berichten.

Teil 2, Zelle (15,4), Ziel 15 Paare (30 Wörter, 7/sqrt(15) = 1,807, Rekord). Exakt entscheiden, nicht nur suchen.
 Positivkontrolle in derselben Struktur, vor a) und b), mit demselben Programm: im Spann von ctrl_16 sind 16 Paare
 lösbar und 15 unlösbar (folgt schon aus der Zählschranke 128/8 = 16).
 a) Spann der zweiten Klasse (loesung111, Rang 8, 128 Paare, 16 384 Punktklassen): Gibt es 15 Paare?
    Vortest: CP-SAT auf 2 Kernen findet 16 Paare in 1,4 s; 15 Paare nach 150 s unentschieden.
 b) Alle Rang-9-Erweiterungen von L(ctrl_16) und von L(loesung111), je 127. Entweder alle einzeln, oder nachweisen,
    welche unter der Symmetriegruppe von L gleichwertig sind, und je Klasse einen Vertreter. Vortest: die Erweiterung
    von L(ctrl_16) mit kleinstem mu 28: 16 Paare in 0,7 s, 15 Paare nach 120 s unentschieden.
 c) Nur wenn a) und b) entschieden sind und Zeit bleibt: Rang 10, lokale Suche, Belegart „Suche“.
 Hinweise (frei, solange die Kontrolle stimmt): Verschiebung um L erlaubt, ein Paar festzuhalten. Symmetrien von L
 (Spaltenpermutationen und Komplementieren von Spalten, die L festhalten) brechen. Je Nebenklasse D gilt die
 Bilanz: k Paare treffen die 2^(r-1) Verschiebungen zusammen genau k * mu(D)-mal, jede muss mindestens einmal
 getroffen sein; das gibt harte Überschuss-Grenzen für eine eigene Tiefensuche. SAT (kissat, cadical), CP-SAT oder ILP.
 Ein Fall, der nach 30 Minuten nicht entschieden ist, heißt „unentschieden“ und wird so berichtet; dann dort eine
 lokale Suche von 10 Minuten nach einer Lösung.

Teil 3, Zelle (20,6), unter 48 Wörtern. Universum: Spann L der 48er (Rang 12, 2048 Paare). 46 oder 44 Wörter wären
 ein Tabellenergebnis, 40 Wörter (20 Paare, 8/sqrt(20) = 1,789) ein Rekord. Zählschranke nur 2048/212 = 9,66.
 a) Sind die Spanne von had24_1p3, had24_2p2 und dem neunten 48er (Datei oben) gleich oder gleichwertig? Wie viele
    der 19 008 Lösungen unter P (Listen in runs/bahnen3/) liegen in L(had24_1p3)? Wie viele verschiedene Universen?
 b) Positivkontrolle: neue lokale Suche (Auswahl von k der 2048 Paare, Kosten = Zahl offener Punkte), k = 24,
    Zufallsstart, Startwerte 1 bis 8, je höchstens 10 Minuten. Bestanden: mindestens 2 von 8. Sonst Stopp.
 c) Leiter: k = 23, Startwerte 1 bis 8 je 15 Minuten, dazu Warmstart aus jedem bekannten 48er im Universum ohne ein
    Paar. Nur nach einem Treffer weiter zu k = 22, 21, 20. Je weiteres Universum aus a) dasselbe mit 4 Startwerten.
 d) Falls billig: exakter Versuch für k = 23 (SAT, höchstens 30 Minuten); Erwartung: unentschieden.
 Jeder Treffer: Radius per Brute-Force über 2^20 Punkte, Matrix ablegen, pruefe_matrix.py, Gleichwertigkeit zu den
 bekannten 48ern und untereinander prüfen (Graph-Kanonisierung, z. B. igraph/bliss).

Obergrenzen: Teil 1 30 Minuten, Teil 2 90 Minuten, Teil 3 2 Stunden Rechenzeit; zusammen höchstens 4 Stunden.
Abbruch: Teil 1 weicht ab; eine Positivkontrolle wird nicht bestanden (dann nur diesen Teil abbrechen); Zeit um.
Belegart: „voll“ nur für exakt entschiedene Fälle, sonst „Suche erfolglos“ oder „unentschieden“.
Am Ende ohne Zuruf auswerten: LANDKARTE.md (Regel 9, je Zelle und Universum die kleinste erreichte Paarzahl),
STATUS.md höchstens fünf Zeilen plus Verweis aufs Protokoll, Fehlversuche als Einzeiler, nichts nach außen melden.
```

## Startprompt für die Sitzung

```
Lies notes/spann_2026-11-25_auftrag.md in main und führe Auftrag S aus. Arbeite auf einem eigenen Branch.
```
