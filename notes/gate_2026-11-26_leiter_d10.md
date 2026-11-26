# Gate zu Auftrag D (Leiter bei d = 10), 26.11.2026

Orchestrator-Gate. Sitzungsbranch `claude/auftrag-d-f98pwb` (Commit `cc3128e`, enthält `main` = `f2ab8a6`), Auftrag `notes/leiter_d10_2026-11-26_auftrag.md`,
Protokoll `notes/laeufe/2026-11-26_leiter_d10.md`. Beginn 14:38 Uhr, Ende 14:45 Uhr (Containeruhr, MEZ). Eigene Programme und Logs:
`notes/orchestrator/2026-11-26_gate_leiter_d10/` (kein Code der Sitzung). Container des Orchestrators: 2 Kerne.

## Urteil: bestanden, Ergebnis „Suche erfolglos“; bei (30,10) sagt die Suche nichts

Alle Zahlen der Sitzung stimmen. Kein K = 16, kein Rekordkandidat, keine Matrix zu prüfen. Der Auftrag ist sauber ausgeführt.
Die Aussagekraft ist aber kleiner, als die Tabelle im Bericht nahelegt (Punkte 1 und 2).

## Was ich selbst geprüft habe

| Prüfung | Ergebnis |
|---|---|
| `sha256sum -c verifiers/SHA256SUMS` | 3 von 3 OK; `zerl_halb.c`, `zerl_offen.py` und die Prüfdateien unverändert; neu in `src/` nur drei Treiberskripte und `abgleich_d10.py` |
| Restzahlen aller gespeicherten Paare, eigener Zähler in C (`gate_d.c`, `gate_d.py`) | 106 von 106 gleich der Kopfzeile (100 Läufe der Blöcke und 6 aus Schritt 1); 10 gelöst, alles Kontrollen |
| Kontrollen K3 (8 von 8) und (25,8) bei d = 9 (2 von 8) | je 0 offene Klassen nachgezählt |
| Beste Restzahl je Zelle | (26,8) 115 136, (28,9) 125 632, (30,10) 1 911 391, (24,7) 81 536, (22,6) 55 648: gleich |
| Struktur der besten Zustände in den Typen (16,x) | U ist dort eine volle Hadamard-Matrix 16 x 16 (paarweise orthogonale Zeilen): beide 115 136er bei (26,8), beide 81 536er bei (24,7), der 129 052er bei (28,9), beide gelösten Kontrollen bei (25,8) |

## Proben des Orchestrators (nicht Teil des Auftrags, von keiner Sitzung gegengerechnet)

Dieselbe Bauregel wie beim 48er, eine Größe höher: M = [[H,H],[H,−H]] mit H = Sylvester-Hadamard 16, also U = W = H mit K = 16. Die 64 Wörter sind der
Reed-Muller-Code RM(1,5). Dann Spalten streichen und bei d = 10 zählen (`probe_sylvester32.py`, zusammen 3 Sekunden).

| Zelle | Bauregel: offene Klassen | Suche der Sitzung: beste Restzahl |
|---|---|---|
| (30,10), 2 Spalten gestrichen (alle Lagen gleichwertig, drei gerechnet) | **107 520** | 1 911 391 |
| (28,9), 4 Spalten gestrichen | 133 056 (allgemeine Lage), 1 704 192 (vier Punkte mit Summe 0) | 125 632 |
| (26,8), 6 Spalten gestrichen, 40 zufällige Lagen | 117 504 (24 Lagen), 121 536, 138 816 | 115 136 |

Dazu Paley-Hadamard 32 (q = 31) ohne 2 Spalten, eigener allgemeiner Zähler (`offen_allg.c`, an Sylvester geeicht: 215 040 Punkte bis auf Vorzeichen = 107 520 Klassen):
270 000 Punkte bis auf Vorzeichen, also schlechter als Sylvester. Ohne 4 Spalten: 273 924 bis 406 630 (vier Lagen).

Bei d = 11 (dort erlaubt die Rekordgrenze 76 Wörter, also 32 Paarzeilen plus bis zu 6 freie Zeilen) lassen dieselben 64 Wörter offen:
(31,10) 1,28 %, (29,9) 3,03 %, (27,8) 5,08 %, (25,7) 10,2 %, (23,6) 18,4 % der Klassen (`probe_sylvester32_d11.py`, je beste von 3 bis 24 zufälligen Lagen).

## Widerspruch und Einschränkungen

1. **(30,10) ist nicht ernsthaft geprüft.** Die Suche endet dort 18-mal schlechter als eine Bauregel, die man in einer Sekunde hinschreibt. Der Löser schafft in
   20 Minuten eine Runde. „K = 16 nicht erreicht“ beschreibt bei (30,10) das Werkzeug, nicht die Zelle. Das ist auch mein Fehler: Schritt 1 des Auftrags verlangte nur,
   dass der Löser „erkennbar arbeitet“, und keinen Vergleich mit einer Bauregel. (30,10) ist die Zelle mit der größten zulässigen Dichte der ganzen Tabelle (3,06).
2. **Die „Plateaus“ bei (26,8) und (28,9) sind das Hadamard-Becken.** Die Suche landet 2 bis 6 % unter der Bauregel Sylvester ohne Spalten und nicht weiter.
   In den Typen (16,x) ist U in den besten Zuständen eine volle Hadamard-Matrix. Das ist derselbe Befund wie im Gate H: Der Löser findet die bekannte Struktur wieder,
   nichts jenseits davon. Vier Startwerte mehr oder doppelte Laufzeit ändern daran nichts, wie Schritt 3 zeigt.
3. **Die Positivkontrolle trägt für die großen Zellen nicht.** (25,8) bei d = 9 hat 8,4 Mio. Klassen, (30,10) hat 268 Mio. Regel 2 ist nach dem Wortlaut des Auftrags erfüllt,
   in der Sache nur für (22,6) bis (26,8).
4. **Auch die Bauregeln reichen nicht.** Sylvester 32 und Paley 32 verfehlen d = 10 bei 30 Spalten um 0,04 bis 0,05 % der Punkte. Ein Treffer mit K = 16 wäre zudem nur
   Gleichstand (10/√32 = 5/√8) und ein Tabellenwert (64 statt 128); für einen Rekord braucht es 62 Wörter oder weniger.
5. **Nicht gerechnet:** Schritt 3 für (30,10), (24,7), (22,6); (20,5) gar nicht. (28,9) in Schritt 3 mit 1800 s statt 2400 s. Offen berichtet, angenommen.
6. **Regel 4:** Blöcke liefen als Hintergrundjobs mit Treiberskript, das nach jedem Block committet und pusht; ein Block wurde vom Werkzeug nach 30 Minuten abgebrochen
   und sauber neu gerechnet. „Im Vordergrund warten“ ist in den Cloud-Sitzungen über 10 Minuten nicht möglich (so auch in Auftrag M). Vorschlag: den Satz in `CLAUDE.md`
   anpassen (eigener PR, Entscheidung von Justus).
7. Die Sitzung hat drei eigene Zählfehler im Protokoll selbst gefunden und berichtigt (17 Typen, 68 und 16 Läufe). Die Endfassung stimmt.

## Einhaltung

Rechenzeit laut Protokoll 07:14 bis 13:34 UTC, 6 h 20 min von 7 h. Nichts installiert, kein neuer Suchcode. Eigener Branch, `main` am Ende geholt.
STATUS: fünf Tabellenzeilen ergänzt, eine Negativzeile, zwei Fehlversuche. LANDKARTE: eine Zeile. Stand-Zeilen nicht angefasst (im Gate nachgezogen). Nichts nach außen gemeldet.

## Bewertung

- Erwartung aus dem Auftrag („kein Rekord; K = 16 in (28,9) oder (30,10) möglich“, unkalibriert): kein Rekord trifft zu, K = 16 ist nicht eingetreten.
- Mit D sind die Leitern der Kern-Familie bei d = 8, 9 und 10 durch, überall ohne Rekordkandidat. Erreicht sind 48 und 50 Wörter gegen 40 (d = 8), 64 gegen 50 (d = 9)
  und mehr als 64 gegen 62 (d = 10). Weitere Leitern mit demselben Löser halte ich nicht für sinnvoll.
- Billiger Test, falls Justus (30,10) weiterverfolgen will: Warmstart des Lösers (`--start`) aus dem Sylvester-Zustand mit 107 520 offenen Klassen, 1 Startwert, 20 Minuten.
  Abbruch, wenn die Restzahl nach zwei Runden nicht unter 107 520 fällt. Obergrenze 2 Stunden. Ein Treffer wäre Gleichstand und Tabellenwert, kein Rekord.
- Kein Auftrag ohne Entscheidung von Justus. Es läuft nichts.
