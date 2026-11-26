# Prüfvermerk zur Gegenprüfung der Ideenliste vom 26.11.2026

Orchestrator, 26.11.2026, 17:03 bis 18:15 Uhr, Abschnitt 6 und die Berichtigung der Notiz bis 18:35 Uhr (Zeiten aus Zeitstempeln und `date`).
Geprüft werden zwei Antworten auf den Prompt 5b aus `notes/ideen_2026-11-26.md` und der Deep-Research-Bericht zu Prompt 5a (Abschnitt 6):

- **Gemini** (App, von Justus um 17:03 Uhr geschickt): bricht mitten in Punkt 2 ab. Teilantwort in `notes/gemini/gegenpruefung_2026-11-26_teilantwort.md`.
- **GPT-6.2 Sol** (Bridge, Batch, Freigabe 17:54 Uhr, fertig 18:04 Uhr, 0,14 US-$): vollständig, `notes/openrouter/gegenpruefung_2026-11-26--openai-gpt-6.2-sol.md`.
  Auftragstext `notes/ideen_2026-11-26_sol_auftrag_gegenpruefung.md`. Sol bekam die Notiz unkorrigiert und ohne Geminis Antwort.

Eigene Rechnungen dazu: `notes/orchestrator/2026-11-26_gegenpruefung/`, zusammen unter 5 Minuten, je ein Programm außer wo vermerkt.
Die Notiz `notes/ideen_2026-11-26.md` ist nach Freigabe durch Justus (18:20 Uhr) berichtigt; die erste Fassung ist der Stand von `main` nach PR 56 (`ff723e4`).

## 1. Was an der Notiz falsch ist

| Stelle | Einwand | von | Urteil nach Nachrechnung |
|---|---|---|---|
| 3e, amalgamierte Summen | Die Annahme d ≤ 1,768·√N je Faktor gilt nicht für Faktoren mit mehr Spalten als Zeilen | Gemini und Sol, unabhängig | **trifft zu.** Beispiel: 20 Wörter bei (19,6) gäben über die Verdopplung K(20,6) ≤ 2·K(19,6) den Rekord |
| 3e | d ≥ d_A + d_B − 1 ist eine untere Schranke aus dem Bausatz; ich habe sie als obere Schranke zum Ausschließen benutzt | Sol (Gemini als „gerade Norm“) | **trifft zu.** Sols Gegenbeispiel (zweimal F_2 × {00,11}, Amalgam hat d = 3 statt 1) nachgerechnet |
| 3e | Der Quotient 1,63 für (2,3) ist nur eine Obergrenze, real 1,22 | Gemini | trifft zu, ändert nichts |
| 3c | „107 520 Löcher“: das sind Klassen der Kern-Familie (±y, ±z). Bis auf Vorzeichen sind es 215 040, als Punkte 430 080 | Sol | **Wortlaut falsch, Zahlen stimmen** (so auch in Gate D und in 3a: 0,040 %). Die Bedingung „kein Walsh-Wert −8“ gilt erst nach der Normierung x_0 = +1 |
| 3c | „mindestens 8 freie Zeilen“ gilt nur für ±1-Zeilen und nur, wenn alle 32 Sylvester-Zeilen bleiben | Sol (Gemini kündigt es an) | **trifft zu.** Für reelle Zeilen bleibt nur das Rahmenlemma: höchstens 8 von 32 je Nebenklasse, also mindestens 4 Zeilen. Mit gestrichenen Zeilen folgt nur: mindestens 2 müssen weg |
| 3c | „höchstens 7,1 %“ und „mindestens 15 Zeilen“ folgen nicht aus einer Stichprobe | Sol | trifft zu; es bleibt ein Hinweis |
| 3c | „bestenfalls Gleichstand“ beim Warmstart folgt nicht aus dem Rahmenlemma | Sol | trifft zu; der Gleichstand folgt aus dem Ziel des Warmstarts (64 Wörter), nicht aus dem Lemma |
| 1 und 3a | „Hadamard ist ab N = 20 die falsche Richtung“ ist ein Urteil, kein Befund: Voll gerechnet sind Streichungen bei N = 20 und 24, bei 28 eine Matrix, bei 32 sieben | Sol | trifft zu; als Urteil kennzeichnen |
| Idee 1 | „K = 8 muss herauskommen“ ist keine Positivkontrolle; richtig ist: Der bekannte Code muss als Zertifikat K ≤ 8 für seinen Typ durchgehen | Sol | trifft zu |
| Idee 1 | 30 Minuten für alle Typen sind nicht belegt (naiv 1,9·10^14 Kandidaten je Typ) | Sol | trifft zu; der erste Test muss kleiner werden |
| Idee 4 | Ordnung 28: rund 387 Mio. Masken, „Minuten“ ist nicht belegt | Sol | trifft zu (397 594 Masken je Matrix nachgerechnet); mit Frühabbruch vermutlich machbar, aber ungemessen |

## 2. Was bestätigt ist (unabhängig hergeleitet, ohne meine Programme)

- Rahmenlemma, auch für reelle Zeilen (Σ = N·‖w‖² ≤ N·m): Gemini und Sol.
- Löcher bei (31,10) = tiefe Löcher von RM(1,5), Paritäten: Gemini und Sol.
- 427 924 = 219 604 + 208 320 Nebenklassen im Abstand 12: Sol von Hand; Rechenweg nachvollzogen
  ((140·39·64 + 30·28·15·1024 + 896²)/64 = 219 604; 31·2·140·8·192/64 = 208 320).
- Höchstens 6 Walsh-Stellen mit Betrag ≥ 12, genau 6 nur in 13 888 Nebenklassen, 5 nie: Sol (31·2·16·896/64) und Gemini
  (Stabilisator der Ordnung 23 040; 319 979 520 / 23 040 = 13 888). Drei Wege, eine Zahl.
- Eine ersetzte Spalte bei Ordnung 20: Der Ausschluss gilt auch für reelle Ersatzspalten (Runden auf das Vorzeichen erhält jeden Treffer). Sol gibt
  dazu einen Handbeweis: Nach Normierung der gestrichenen Spalte haben alle kritischen Zeilen dasselbe Vorzeichen, jedes der 1140 Tripel müsste
  beide Vorzeichen der Ersatzspalte enthalten, und das geht bei 20 Zeilen nicht. Die 1140 Tripel stammen aus meinem Programm.

## 3. Neue Rechnungen aus den Einwänden

1. **Verdopplung (überbreite Faktoren).** Die Tabelle (florath `bbed9a6`, Prüfsumme bestätigt) lässt offen: (17,5) und (19,6) mit 20 Wörtern;
   (20,6), (22,7), (24,8) mit 24; (23,7) bis (29,10) mit 30; (26,8) bis (30,10) mit 38 (`verdopplung_tabelle.log.txt`). Das ist der Kern-Typ (m−1, 1),
   kein neuer Bautyp, aber er wurde nie gesucht. Probe mit p018 und dem eingefrorenen Bewerter (`verdopplung_probe.log.txt`):
   (19,6) mit 32 Wörtern in 0,3 s gelöst; mit 30 Wörtern 2236 offene Klassen; mit 20 Wörtern 26 192 von 262 144.
   (17,5) mit 20 Wörtern 8952 von 65 536. Belegart: Suche erfolglos, weit entfernt.
2. **Zwei ersetzte Spalten bei Ordnung 20 (Sols Zerlegung in zwei Zehnergruppen).** Für jedes der 1140 Spaltenpaare (3 Klassen mit Transponierten):
   1272 flache x'', 488 bis 600 Bedingungen je Gruppe, **keine einzige Belegung in keiner Gruppe**. Damit erreicht keine Hadamard-20-Matrix mit zwei
   beliebig ersetzten Spalten disc 8, auch nicht mit reellen Einträgen in diesen Spalten. Belegart: voll (`ersatz_k2.c` alle Paare;
   `ersatz_k2_gegen.py` mit anderem Aufbau fünf Paare, gleiche Zahlen; Aufzählung mit abgeschwächten Bedingungen 36 = 36). Idee 2 ist bis k = 2 erledigt.
3. **Faserbedingung für den Kern der Dimension 3 (Sols billigerer Test).** 55, 74, 97 Typen bei (16,4), (18,5), (20,6); bei (20,6) kommen die
   Zahlen aus `STATUS.md` heraus (B(D) 31 180 bis 53 372, Schranke 2,46 bis 4,20). Mit K = 5 schließt die Faserbedingung nur 11 von 55, 4 von 74
   und 1 von 97 Typen aus (`kern3_fasern.log.txt`). Der Filter erledigt Idee 1 nicht; es bleiben 44, 70 und 96 Typen für eine echte Suche.

## 4. Nicht übernommen oder offen

- Geminis „gerade Norm“ als eigener Ausweg: gleichbedeutend mit Sols Punkt (ein Code mit Norm 2R ist ein um eine Spalte verlängerter Code mit
  d + 1). Kein zusätzlicher Fall; meine Herleitung dazu ist ungeprüft.
- Geminis „amalgamierte halbdirekte Summe“ und Amalgamation über mehrere Koordinaten: ohne Quelle, nicht geprüft. Blockweise Summe, Doppelung und
  Sols Kantenvariante (Zeilen (a_i, b_j), (a_i, −b_j) entlang eines Graphen) liegen in der Kern-⟨1,a⟩-Familie mit wiederholten Zeilen.
- Sols Richtung 3 (Hadamard 20 mit zwei Nullen je Zeile an zeilenabhängigen Stellen): nicht gerechnet. Kandidat für einen Filtertest.
- Sols Handbeweise habe ich in den Zahlen nachvollzogen, nicht Zeile für Zeile formal geprüft.

## 5. Folgen für die Rangliste (Vorschlag, Entscheidung bei Justus)

- **Idee 2** ist bis k = 2 negativ erledigt. k = 3 nur, wenn sich eine ähnlich billige Zerlegung findet.
- **Idee 1** bleibt der beste Kandidat, aber der erste Test wird kleiner: (16,4), die 44 verbliebenen Typen, je Typ ein Zeitlimit, Kontrolle
  „bekannter linearer Code geht als K ≤ 8 durch“.
- **Neu: Verdopplungszellen** ((19,6) mit 20 Wörtern und die elf weiteren aus Punkt 3.1). Billig zu suchen, die Probe liegt weit weg.
- **Abschnitt 3e** der Notiz wird zurückgenommen und durch die Tabelle aus Punkt 3.1 ersetzt.

## 6. Geminis Deep Research zu Prompt 5a (von Justus um 18:20 Uhr geschickt)

Bericht: `notes/gemini/ideen_2026-11-26_deep-research.pdf` (18 Seiten). Geprüft habe ich, was die Notiz trägt.

| Aussage des Berichts | Prüfung | Urteil |
|---|---|---|
| t[n,6] = ⌊(n−8)/2⌋ für n ≥ 18 ist Theorem 4.1 von Baicheva und Bouyukliev (Adv. Math. Commun. 4 (2010), 399–404); obere Schranke von Graham und Sloane 1985; Ausnahmen t[15,6] = 4, t[17,6] = 5 | Arbeit über die im Bericht genannte Adresse abgerufen (Kurzfassung durch das Abrufwerkzeug, nicht selbst Zeile für Zeile gelesen): Titel, Theorem 4.1, Zuschreibung und t[17,6] = 5 bestätigt | **belegt.** Meine Zuschreibung an Kilby und Sloane 1987 war falsch |
| t[n,7]: nur die obere Schranke ⌊(n−9)/2⌋ ist belegt, keine Gleichheit | dieselbe Arbeit nennt für Dimension 7 nur obere Schranken | **übernommen.** Mein Satz „d ≤ 10 bei 64 Zeilen“ ist gestrichen; lineare Codes sind nur bis Dimension 6 ausgeschlossen |
| Verheiden 1978: Ergänzung möglich, wenn höchstens 7 Zeilen fehlen; scharf | Quelle des Berichts ist eine MathOverflow-Diskussion; für mich nicht abrufbar | **aus zweiter Hand**, in der Notiz so gekennzeichnet |
| 427 924 = 219 604 + 208 320, aber vier Klassen statt zwei; Löcher bei zwei gestrichenen Spalten nur aus zwei der vier | nachgerechnet (`rm15_klassen.py`): Grad 2: 868; Grad 3: 166 656 und 52 080; Grad 4: 208 320; Löcher 0, 0, 53 760, 376 320 | **bestätigt** |
| Reis und Song: Anteil 2^−22 ersetzter Spalten, gilt ab n ≥ 2^50, Faktor 1 + 2^−22 | am 26.11. nur die Kurzfassung gelesen (dort steht der Anteil 2^−22 und „erste Verbesserung über √n“) | Schwelle 2^50 **nicht selbst geprüft**; Folge für uns: kein Mechanismus für n ≤ 40 |
| Ordnung 16: had16.1 und had16.2 geben 4 und 5, had16.3 und had16.4 geben 6, 7, 6 | meine Tabelle hat für Sylvester und die Klasse von `ctrl_16` dieselben Werte; had16.1 und had16.2 nicht nachgerechnet | teils bestätigt, Rest als Geminis Rechnung gekennzeichnet |
| Paley 20: 6, 7, 6, 5, 6, 5 für 0 bis 5 gestrichene Spalten; Sylvester 32: 9 und 8 | stimmt mit 3a überein (dort alle Klassen und Spaltenmengen bis c = 4) | bestätigt |
| K(16,4), K(18,5), K(20,6) ≤ 64 und K(30,10) ≤ 128 stammen aus linearen Codes der Dimension 6 und 7 | passt zu `lin_kontrolle.py` (lineare Codes mit d = 8 bei Länge 16, 18, 20 gefunden) | plausibel, Primärquelle (Kéris Fußnoten) laut Bericht nicht abrufbar |
| Seit September 2026 kein Wert über 5/√8 (Seite 10c, EinsteinArena, Preprints) | nicht erneut selbst geprüft (letzte eigene Prüfung 26.11., 10:08 und 11:30 Uhr) | ersetzt die Neuheitsprüfung vor einer Meldung nicht |
| Keine Bauart außerhalb der Hadamard-Familie mit n − 2R ≥ 8 bei Länge 20 bis 31 und höchstens 2n Wörtern gefunden | nicht prüfbar | „nichts gefunden“, kein Ausschluss |

Nicht geprüft und nicht verwendet: die neueren Arbeiten zu Hadamard-Bent-Folgen und zum Gale-Berlekamp-Spiel, die der Bericht nennt
(dort geht es um andere Größen als die Diskrepanz), und die Einzelheiten zu Dimension 8 und 9.

