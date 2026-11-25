# Auftrag E (freigegeben 25.11.2026, 18:10 Uhr): Evolutionsschleife für freie Suchprogramme

Freigabe durch Justus im Orchestrator-Chat, 25.11.2026, 18:10 Uhr: „E, P und H parallel“. Die drei Sitzungen laufen nebeneinander und neben R9:
je eigener Branch, je eigenes Protokoll, `STATUS.md` und `LANDKARTE.md` erst am Ende und erst nach `main` holen.

Vom Orchestrator, 25.11.2026. Grundlage: `notes/ideen_2026-11-25.md` (Abschnitte 4, 8, 10), `notes/ideen_2026-11-25_pruefvermerk.md`,
Astra-Antwort `notes/openrouter/ideen_2026-11-25--openai-gpt-6.2-astra.md` (Abschnitt 3, Bewertungsleiter). Der Gemini-Bericht vom 25.11. ist eingearbeitet (`notes/gemini/ideen_2026-11-25_kurz.md`): Bauhinweise für Insel A, Insel „Saat“.

```
Auftrag E (Opus, Effort hoch): Suchprogramme in Runden gegen einen eingefrorenen Bewerter verbessern

Lies zuerst STATUS.md, CLAUDE.md, notes/ideen_2026-11-25.md (Abschnitte 4, 8, 10, 11), notes/ideen_2026-11-25_pruefvermerk.md und
notes/gemini/ideen_2026-11-25_kurz.md.
Eigener Branch, Protokoll notes/laeufe/<Datum>_evo.md, Logs und Archiv runs/evo/, STATUS.md erst am Ende. Nichts installieren
(kein pip, kein apt); C mit gcc -O3 -march=native -fopenmp, Python nur Standardbibliothek und numpy.
Die Stand-Zeile (Zeile 3) von STATUS.md und LANDKARTE.md nicht ändern.

Worum es geht: Die Zellen (20,6) mit 40 Wörtern (Rekord 8/sqrt(20) = 1,789) und mit 46 oder 44 Wörtern (Tabellenwert) wurden nie frei
gesucht, nur in eingeschränkten Räumen (Nebenklassen, Bahnen, Spann). Vorbild: LLM-gesteuerte Programmsuche (lineare Codes: arXiv 2609.37056;
Ramsey-Zahlen: arXiv 2603.09172). Dort verändert ein Sprachmodell das Suchprogramm, ein exakter Prüfer bewertet. Hier bist du selbst die Schleife:
Du schreibst kein einzelnes Suchprogramm, sondern verbesserst es in Runden und führst ein Archiv.

Begriffe: Ein Code aus k Paaren ist eine k x m-Matrix mit Einträgen +1/-1 (je Zeile ein Wort, das Komplement gehört dazu). Punkte modulo
Vorzeichen heißen Klassen (2^(m-1) Stück). Eine Klasse ist offen, wenn kein Wort und kein Komplement im Hamming-Abstand <= R liegt.

Schritt 1, Bewerter (Prüfcode, danach eingefroren): src/evo_bewertung.c. Eingabe m, R, Datei mit k Zeilen; Ausgabe: Zahl offener Klassen, exakt,
nur ganze Zahlen, alle 2^(m-1) Klassen. Kontrollen, alle müssen stimmen:
  - Hamming: controls/ctrl_8.txt (erste 7 Spalten), R = 1: 0 offen; ohne eine Zeile: 8.
  - controls/ctrl_16.txt (erste 15 Spalten), R = 4: 0 offen; ohne eine beliebige der 16 Zeilen: jeweils genau 233.
  - controls/had24_1p3_paley12.txt (24 Zeilen sind 24 Paare), R = 6: 0 offen. Streicht man die zwei Zeilen einer Nebenklasse
    (Zeilen i und i+12): 3054 offene Klassen (= 6108 Punkte). Streicht man eine einzelne Zeile: 1297 offene Klassen (jede der 24).
  Danach sha256sum der Datei nach runs/evo/BEWERTER.sha256; der Bewerter wird nicht mehr geändert. Nur seine Zahl zählt, nie die Selbstauskunft
  eines Suchprogramms.

Schritt 2, Archiv und Runde.
  Archiv runs/evo/archiv.tsv, eine Zeile je Lauf: Runde, Programm, Elter, Insel, m, R, Paare, Startwert, Sekunden, offene Klassen, Kommentar.
  Eine Runde: (1) Elter aus dem Archiv wählen (bester der Insel oder bewusst ein anderer). (2) Kind schreiben, src/evo/pNNN.c, mit Kopfkommentar:
  Elter, was geändert, warum (zwei Zeilen). Aufruf einheitlich: pNNN m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI. (3) Vier Startwerte parallel,
  festes Zeitbudget. (4) Bewerter auf alle Ausgaben. (5) Archiv ergänzen, committen, pushen.
  Regeln: Ein Kind, das dieselben Zahlen liefert wie der Elter, zählt als Fehlschlag. Programme lesen keine Dateien aus controls/ oder runs/
  (blind), außer in der Insel „Saat“. Mindestens drei Inseln plus Saat:
  A Züge auf Wortebene: einen offenen Punkt wählen und ein Wort durch einen Bitwechsel zu ihm hinziehen; Zähler je Klasse fortschreiben statt
    neu zählen (ein Bitwechsel ändert 2*C(m-1,R) Zähler, bei (20,6) 54 264); Tabu über 6 bis 9 Schritte; Punktgewichte (Gewicht eines Punkts
    steigt, solange er offen bleibt) an- und abschaltbar, beide Fassungen im Archiv vergleichen. Vorbilder laut Literaturbericht (ungeprüft):
    Marosi, arXiv 2608.19872; van Staden, Preprint Oktober 2026 (dort war bei stark überlappenden Überdeckungen Tabu ohne Gewichte besser).
  Saat (erst nach K2, als eigene Insel): Streichen und Reparieren ab dem 48er. Start: controls/had24_1p3_paley12.txt ohne eine Zeile
    (1297 offene Klassen) oder ohne eine Nebenklasse (3054). Bekannt aus dem Vortest: Kein Wechsel von ein oder zwei Einträgen einer Zeile
    senkt die 1297; ein Schnellprogramm blieb 240 s bei 1297. Die Insel braucht also Züge, die ein lokales Minimum verlassen.
  B Doppelungsform (Paar (U,W), Wörter (U_i,W_i) und (U_i,-W_i), Bedingung max_i(|U y|_i + |W z|_i) >= d, siehe
  notes/ideen_2026-11-25.md Abschnitt 3); C freie Erfindung ohne Wiederverwendung vorhandenen Suchcodes. Alle fünf Runden drei Zeilen ins Protokoll:
  was trägt, was nicht; bei fünf Runden ohne Verbesserung die Insel wechseln.
  Bewertung: zuerst die kleinste Paarzahl mit 0 offenen Klassen. Bei Gleichstand der Bonus g = log2((1+b)/(1+h)) für den besten Lauf mit einem
  Paar weniger, h = offene Klassen, b = Referenz: 233 bei (15,4) mit 15 Paaren; bei (20,6) mit 23 Paaren 1297 (Einzelstreichung
  aus dem 48er), mit 22 Paaren 3054; für 21 und 20 Paare die beste eigene Restzahl der vorigen Stufe.

Schritt 3, Kontrollen (zugleich Abbruchkriterien).
  K1: (15,4) mit 16 Paaren, blind, Runden zu 4 Startwerten x 300 s. Soll: spätestens in Runde 10 erreicht ein Programm 0 offene Klassen in
      mindestens 2 von 4 Startwerten. Sonst stoppen und berichten.
  K2: (20,6) mit 24 Paaren, blind, Runden zu 4 Startwerten x 600 s. Soll: spätestens nach 15 weiteren Runden 0 offene Klassen in mindestens
      1 von 4 Startwerten. Sonst stoppen und die beste Zahl berichten. Zum Vergleich: die alte Nebenklassen-Suche fand den 48er in 1 von 16
      Läufen zu 1800 s.

Schritt 4, Ziele, erst nach K2: (20,6) mit 23 Paaren, dann 22, 21, 20. Je Ziel höchstens 10 Runden ohne Verbesserung der besten Restzahl,
dann das nächste Vorhaben nicht mehr beginnen, sondern auswerten. Nebenziel, höchstens 5 Runden: (15,4) mit 15 Paaren mit dem besten Programm
aus K1, ohne Saat aus dem 32er (laut Prüfvermerk teilt kein 30er 13 Zeilen mit einem der beiden bekannten 32er; die freie Suche endete bisher immer bei 233).

Treffer (0 offene Klassen bei (20,6) mit weniger als 24 Paaren oder bei (15,4) mit 15 Paaren): sofort anhalten. Matrix nach runs/evo/ ablegen,
sha256sum -c verifiers/SHA256SUMS, python3 verifiers/pruefe_matrix.py MATRIX (erwartet Diskrepanz 8 bzw. 7), Ergebnis ins Protokoll, pushen,
berichten. Nichts nach außen melden.

Obergrenze: 8 Stunden Rechenzeit (Summe der Rundenbudgets), davon K1 höchstens 1 Stunde, K2 höchstens 3 Stunden. Blöcke von höchstens
30 Minuten, nach jedem Block Protokoll und Archiv pushen (CLAUDE.md Regel 4).
Belegart: Nicht erreichte Ziele sind „Suche erfolglos“ und schließen nichts aus.
Am Ende ohne Zuruf auswerten: main holen; STATUS.md höchstens fünf Zeilen plus Fehlversuche als Einzeiler; LANDKARTE.md: beste erreichte
Paarzahl je Zelle und beste Restzahl darunter (Regel 9). Nicht nach main zusammenführen.
```

## Startprompt für die Sitzung

```
Lies notes/evo_2026-11-25_auftrag.md in main und führe Auftrag E aus. Arbeite auf einem eigenen Branch.
```
