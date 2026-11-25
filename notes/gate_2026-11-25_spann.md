# Gate zu Auftrag S (Suche im affinen Spann), 25.11.2026

Geprüfter Stand: Branch `claude/cool-cannon-5umcqa`, Commit `cfdbaa1` (enthält `main` mit Auftrag G). Prüfer: Orchestrator
(Chat vom 25.11., gegen 15:10 Uhr). Eigene Programme ohne Code der Sitzung: `notes/orchestrator/2026-11-25_gate_spann/`
(Ausgabe `ausgabe.log.txt`). Rechenzeit etwa 6 Minuten auf 2 Kernen, davon 4 Minuten für Punkt 5. Das ist mehr als die
üblichen „wenigen Minuten“; der Punkt ist der einzige neue volle Ausschluss in der Rekordzelle und war so in einem Lauf zu haben.

## Urteil

**Bestanden. Kein kleinerer Code, kein Rekordkandidat, kein Tabellenergebnis.**

## Was geprüft wurde

| Nr. | Punkt | Ergebnis |
|---|---|---|
| 1 | Prüfdateien, Diff | `sha256sum -c` OK; gegenüber `main` nur `STATUS.md` und `LANDKARTE.md` geändert, sonst neue Dateien; keine Datei über 300 KB |
| 2 | Teil 1 (Ränge, Kerne, μ-Verteilungen, Rang-9-Sorten) | Sitzung bestätigt alle Zahlen des Vortests mit eigenem Programm; damit zwei unabhängige Programme |
| 3 | Die 11 Problemdateien der Sitzung (`runs/spann/probleme/*.txt`) | alle treu: Die Familie der Überdecker-Mengen aller Punkte des Würfels, direkt gerechnet, ist gleich der Familie der verschobenen Muster; Ausgangscode und Erweiterungsvektor liegen im Universum |
| 4 | Die Symmetrie-Erzeuger der Sitzung (11 Dateien, je 12 Erzeuger) | alle gültig: bijektiv, jedes Muster geht auf ein verschobenes Muster |
| 5 | **Teil 2a, L(loesung111), 15 Paare:** eigene exakte Suche ohne Symmetrie (nur Verschiebung), eigener Export, elementare Schranken | **keine Lösung, vollständig**, 29 847 905 Knoten. Kontrolle davor: 16 Paare lösbar in L(`ctrl_16`) und L(loesung111), 15 Paare in L(`ctrl_16`) unlösbar |
| 6 | Automorphismengruppen und Klassen der Rang-9-Erweiterungen, unabhängig über bliss | Ordnungen 322 560 und 225 792 wie Sitzung; Bahnen 1, 14, 42, 70 und 7, 7, 8, 49, 56 mit denselben Vertretern |
| 7 | Rang 9, die drei vollen Ausschlüsse (`ctrl_16` y=3 und y=1, loesung111 y=1; 63 Erweiterungen) | **nicht nachgerechnet.** Geprüft sind Eingaben (Punkt 3), Symmetrien (Punkt 4) und Klassen (Punkt 6); die Suche selbst lief nur in einem Programm und nur mit Symmetriebrechung |
| 8 | Teil 3a: Spanne der drei 48er | als Mengen verschieden, alle Rang 12 (bestätigt). Die Zahlen 576 und 81 nicht nachgerechnet |
| 9 | Teil 3b: eine Kontrolllösung (24 Paare, Startwert 4) | Radius 6 über alle 2^20 Punkte, alle Wörter im Spann von 1+3, als Menge ein anderer Code als 1+3. Gleichwertigkeit zu den bekannten 48ern nicht geprüft |
| 10 | Teil 3c: 23 Paare | nur Logs gelesen; Belegart „Suche erfolglos“ ist richtig gesetzt |

## Belegarten nach dem Gate

| Aussage | Belegart |
|---|---|
| Kein Code mit 30 Wörtern im Spann von `ctrl_16` | voll, Zählschranke, zwei Programme |
| Kein Code mit 30 Wörtern im Spann der zweiten Klasse (loesung111) | **voll, zwei Programme** (Sitzung mit und ohne Symmetrie, Gate ohne Symmetrie) |
| Kein Code mit 30 Wörtern in 63 der 254 Rang-9-Erweiterungen | voll, ein Programm, nur mit Symmetrie |
| Übrige 191 Rang-9-Erweiterungen (6 Klassen) | unentschieden; lokale Suche bleibt bei 233 offenen Punktklassen |
| 46 Wörter im Spann der 48er (drei Universen) | Suche erfolglos (28 Läufe zu 15 Minuten, beste Restzahl 886 Punktklassen) |

## Berichtigungen

- `STATUS.md` und `LANDKARTE.md` nannten „4 Klassen (63 Erweiterungen)“ bzw. „4 von 9 Rang-9-Klassen“. Es sind **3** Rang-9-Klassen
  (42 + 14 + 7 = 63); der vierte unlösbare Fall ist L(loesung111) im Rang 8. Berichtigt. Der Chat-Bericht der Sitzung nannte
  62 statt 63 Erweiterungen.
- Stand-Zeilen von `STATUS.md` und `LANDKARTE.md` vom Orchestrator nachgezogen; offene Punkte und nächster Schritt in `STATUS.md` ergänzt.

## Berichtigung der Zeitangaben des Orchestrators (25.11., 15:25 Uhr)

Die Uhrzeiten in den Vermerken und Logs des Orchestrators von heute (Vortest Spann, Gate G, Gate S) waren geschätzt und lagen bis zu
anderthalb Stunden daneben. Sie sind in diesem Commit nach den Zeitstempeln der Commits und Nachrichten berichtigt. Inhaltlich ändert sich nichts.

## Beobachtungen

- Die lokale Suche endet in allen acht Rang-9-Fällen bei genau 233 offenen Punktklassen. Das ist die Zahl von „`ctrl_16` ohne
  eine Zeile“ (466 Punkte). Naheliegend, aber unbelegt: Sie findet jedes Mal einen 32er und lässt ein Paar weg.
- Die Sitzung durfte `ortools`, `python-sat` und `igraph` nicht installieren (Berechtigungsfilter), obwohl der Auftrag es erlaubte.
  Im Orchestrator-Container geht es. Für SAT-Versuche an den sechs offenen Klassen braucht es also entweder eine Sitzung, in der
  Justus die Installation von Hand bestätigt, oder den PC.
- Die Zählschranke war in keinem der offenen Fälle scharf (9 bis 12 gegen 16). Wo sie scharf ist (`ctrl_16`), ist der Fall sofort
  erledigt; überall sonst entscheidet erst die Aufzählung.

## Einordnung

Die Idee „kleinerer Code im Spann eines guten Codes“ hat in der Rekordzelle (15,4) im Rang 8 nichts gebracht und ist dort
vollständig erledigt. Im Rang 9 ist ein Viertel der Fälle ausgeschlossen, der Rest offen, mit negativer Tendenz. Bei (20,6)
gibt es nur eine erfolglose Suche; die Positivkontrolle (24 Paare aus Zufallsstart in Sekunden) zeigt, dass die Suche das
Universum gut beherrscht, was den Fehlschlag bei 23 Paaren gewichtiger macht als bei der Tabu-Leiter vom Vormittag.
