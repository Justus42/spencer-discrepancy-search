# Prüfvermerk 25.11.2026: Antworten von Astra (Teil A, Ideen) und Sol (Teil B, Beweise)

Vom Orchestrator, 17:18 bis 17:42 Uhr (Zeiten aus `date`). Antworten: `notes/openrouter/ideen_2026-11-25--openai-gpt-6.2-astra.md`
(fertig 17:00 Uhr, 0,41 US-$) und `notes/openrouter/beweise_2026-11-25--openai-gpt-6.2-sol.md` (fertig 17:05 Uhr, 0,20 US-$).
Auftragstexte: `notes/ideen_2026-11-25_astra_auftrag_ideen.md`, `notes/ideen_2026-11-25_sol_auftrag_beweise.md`.
Meine Rechnungen: `notes/orchestrator/2026-11-25_ideen/` (`pruef_antworten.py`, `zwei_zeilen.py`, `zwei_zeilen_kern.py`, `spalte_optimal.py`, je mit Log).
**Alles hier ist einmal gerechnet, mit einem Programm, nicht gegengerechnet** (Auftrag P).

## Kurzfassung

- **Kein Kandidat.** Astras zwei Vorschläge sind beide sofort getestet und negativ: der erste vollständig, auch für reelle Zeilen; der zweite an
  seinem eigenen Erfolgskriterium (232) gescheitert.
- Dabei fällt eine neue, starke Aussage ab: **Kein Code mit 30 Wörtern bei (15,4), auch keine Matrix mit reellen Einträgen, teilt 13 Zeilen mit einem der
  beiden bekannten 32er.** Stichproben sagen dasselbe für 12 Zeilen. Die Umgebung des 32ers ist starr; ein 30er müsste weit weg liegen.
- Sol bestätigt meinen Zählbeweis (d = 9, volle Paley-Hälfte) und schließt einen weiteren Fall aus (punktierte Hälfte, m2 = 10). Für K = 10 bei (20,6)
  fällt kein Typ durch Zählen; es gibt aber scharfe Schnitte für eine Suche.
- Nebenbei von Astra: reelle 10 x 10-Matrizen sind ausgeschlossen (bisher nur n = 8, 9, 12).

## Astra, Teil A

| Aussage | Prüfung | Ergebnis |
|---|---|---|
| Zwei Paarkugeln des 32ers teilen genau 140 Klassen; jedes Paar hat 233 private Klassen | `pruef_antworten.py` | stimmt (Zeilenabstände nur 7 und 8) |
| Nach Streichen dreier Zeilen 699 bis 1119 Lochklassen, nach zweien 466 bis 606 | alle 560 Tripel, alle 120 Paare | stimmt als Schranke; tatsächlich 967 (448 Tripel) oder 987 (112), bzw. 550 (64 Paare) oder 562 (56) |
| E_offen je oberster Zelle (2,47·10³ bis 1,3·10⁹) | nachgerechnet | stimmt (bei (20,6) 4,52·10⁴ statt 4,53·10⁴) |
| **Vorschlag A:** 13 Zeilen fest, zwei freie reelle Zeilen, Zuordnung der Löcher plus lineare Programme | siehe unten | **unmöglich für alle 560 Tripel**, schon an der Abstandsbedingung |
| **Vorschlag B:** eine ganze Spalte exakt neu wählen; Erfolg ≤ 232, Misserfolg genau 233 | `spalte_optimal.py` | **genau 233 in allen 240 Fällen** (16 gestrichene Zeilen x 15 Spalten); Kontrolle mit 16 Zeilen: Minimum 0, echte Spalte unter 4 Optima |
| Reelle 10 x 10-Matrizen ausgeschlossen (Kappe liegt in einer Radius-2-Kugel oder hat höchstens 43 Punkte; Vereinigung ≤ 505 < 512; sonst K(10,2) ≥ 24) | Schritte von Hand nachvollzogen, Zahlen 1+10+24+8 = 43 und 504+43−42 = 505 nachgerechnet | schlüssig; Belegart Herleitung, bedingt auf K(10,2) ≥ 24 aus der Tabelle; nicht maschinell geprüft |
| Bewertungsleiter (gültiger Elter plus log-Bonus für ein Kind mit einem Paar weniger; Einheit Antipodenklassen; 31 Wörter ohne Selbstkomplementarität sind keine Stufe) | Urteil | übernommen in Auftrag E; der Hinweis zur Einheit der „886 Punktklassen“ ist berechtigt |

**Zu Vorschlag A im Einzelnen** (`zwei_zeilen.py`). Notwendig für jede Ersatzzeile a in [−1,1]^15 bei Schwelle > 5·√(15/8) = 6,847: Zwei orientierte
Punkte derselben Zeile haben Abstand ≤ 8 (aus a·(x+y) ≤ 2·(15 − Abstand)). Die Lochklassen müssen sich also auf die freien Zeilen verteilen und
orientieren lassen, so dass je Zeile Abstand ≤ 6 gleiches und ≥ 9 verschiedenes Vorzeichen bedeutet. Das ist ein SAT-Problem.

| gestrichen r | frei k | Fälle | Ergebnis |
|---|---|---|---|
| 2 | 2 | 2 (Kontrolle) | erfüllbar, wie es sein muss |
| 2 | 1 | 2 | unerfüllbar |
| **3** | **2** | **alle 560** | **alle unerfüllbar** (je etwa 1,4 Mio. Klauseln, CaDiCaL, je unter 1 s Lösezeit) |
| **3** | **2** | **alle 560 der zweiten 32er-Klasse** (`runs/gegenrechnung15/loesung111_16x15.txt`) | **alle unerfüllbar** (Lochzahlen ebenfalls 967 und 987) |
| 4 | 3 | 8 von 1820 (je 2 pro Lochzahl 1476, 1484, 1508, 1512) | alle unerfüllbar |

Kleine Zertifikate ohne SAT-Löser (`zwei_zeilen_kern.py`): Für vier Tripel genügen 7 bis 10 Lochklassen, die sich schon allein nicht auf zwei Zeilen
verteilen lassen; geprüft durch Aufzählen aller Verteilungen mit Zweifärbung der Vorzeichen. Damit ist die Aussage unabhängig nachprüfbar.
Belegart r = 3: vollständig durchgerechnet für beide 32er-Klassen, ein Programm (SAT-Bibliothek), vier Zertifikate mit eigenem Skript bestätigt.
Nicht gerechnet: r = 4 vollständig; r ≥ 5.

## Sol, Teil B

| Frage | Aussage von Sol | Prüfung | Ergebnis |
|---|---|---|---|
| 1 | Mein Zählbeweis stimmt; Schranke 697 lässt sich auf 320 schärfen; Schnittgrößen 92, 92, 92, 47, 47, 15, 15; gilt auch für m2 = 9, 11; alle 220 Dreiermengen kommen vor (Beweis über Produkte dreier Zeilen) | Schnittgrößen per Formel nachgerechnet; 220 Muster hatte ich schon gezählt | **d = 9 mit U = volle Paley-12 ist für jedes W unmöglich** (m2 = 9, 11, 13). Belegart: Herleitung, von zwei Seiten (Orchestrator, Sol), Zahlen nachgerechnet |
| 2 | U = Paley-12 ohne Spalte: punktweise a + 3b + 6c ≥ 10 (a, b, c = Zahl der Zeilen mit \|v\| ≥ 4, 6, 8); daraus m2 = 10 unmöglich (4920 < 5120); m2 = 12, 14 nicht entschieden | Ungleichung vollständig aufgezählt: keine Kette C ⊆ B ⊆ A mit a + 3b + 6c ≤ 9 erfüllt alle 220 Muster; kleinstes a bei b = 0, 1, 2 ist 10, 7, 6; Summen nachgerechnet | **m2 = 10 ausgeschlossen** (vollständig, Zählargument plus Aufzählung); m2 = 12 (Zelle (23,7)) und 14 (Zelle (25,8)) offen |
| 3 | K = 10 bei (20,6): kein Typ fällt durch Volumenzählung. Schnitte für Typ (11,9): jedes y braucht max \|u\| ≥ 3; bei Maximum 3 mindestens 6 Dreierzeilen; bei genau einer Fünferzeile mindestens 5 Dreierzeilen; rechts bei genau einem Fünfer mindestens 2 Dreier; nicht beide Seiten dürfen eine Klasse mit Maximum 3 haben; Maskentest. Erklärung der 24 192 = 504·4·4·3 | Stichproben: gemeinsames Volumen 58 402, Schnittgrößen 46, 18, 18, 6, 6, Inzidenzen (4620, 3300, 1650, 550, 110, 10) nachgerechnet | Zahlen stimmen, soweit geprüft; die Überlappungsargumente („mindestens 6“, „mindestens 5“) sind **nicht** nachgerechnet und gelten bis dahin als unbelegt |
| 4 | Dominanz offen. Zeilen mit geradem Träger sind dominiert (eine Null durch ±1 ersetzen); Volumenbudget: höchstens 14 Zeilen mit ≥ 3 Nullen, höchstens 12 mit genau einer | Volumina stimmen mit `zellen_heuristik.py` überein; das Ersetzungsargument ist ein Einzeiler (8 − 1 ≥ 7) | Nullen zählen bei N = 15 nur paarweise (Träger 13, 11, …); mehr folgt nicht |

## Was daraus folgt

1. (15,4) bleibt Kontrollzelle, nicht Zielzelle: freie Suche endet immer am 32er, und um den 32er herum ist bis zu drei getauschten Zeilen alles
   ausgeschlossen, auch reell. Warmstarts aus dem 32er sind sinnlos.
2. Die Paley-Hälften tragen bei d = 9 nicht (drei von fünf Fällen ausgeschlossen, zwei offen und unplausibel). Für d = 9 müsste U anders gebaut sein.
3. Für K = 10 bei (20,6) gibt es keinen billigen Ausschluss, aber brauchbare Schnitte. Das stützt Auftrag H als Suche mit exakten Filtern.
4. Astra als Ideengeber: zwei saubere, sofort prüfbare Vorschläge mit richtigen Zahlen, beide negativ. Das war die 0,41 US-$ wert, weil die Prüfung
   eine vollständige Aussage geliefert hat; ein Rekordweg ist es nicht.
