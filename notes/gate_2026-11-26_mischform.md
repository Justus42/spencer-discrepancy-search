# Gate zu Auftrag M (Nebenklassen plus freie Zeilen bei (18,5)), 26.11.2026

Orchestrator-Gate. Sitzungsbranch `ccr-5456f815-s8zi3l` (Commit `99125f4`, enthält `main` = `1f60fc4`), Auftrag `notes/mischform_18_5_2026-11-26_auftrag.md`,
Protokoll `notes/laeufe/2026-11-26_mischform_18_5.md`. Beginn 12:15 Uhr, Ende 12:20 Uhr (Containeruhr, MEZ). Eigenes Skript und Logs:
`notes/orchestrator/2026-11-26_gate_mischform/` (`gate_m.py`, kein Code der Sitzung).

## Urteil: bestanden, mit einem deutlichen Abstrich bei Schritt 3

Alle Zahlen der Sitzung stimmen mit meiner Nachrechnung überein. Kein Code unter 50 Wörtern, kein Rekordkandidat. Die 50 sind von einer Sitzung gegengerechnet.
Der Abstrich: Die gemeinsame Suche hat bei 48 Wörtern die Mischform praktisch nicht geprüft (Punkt 1 unten).

## Was ich selbst geprüft habe

| Aussage der Sitzung | eigene Rechnung | Ergebnis |
|---|---|---|
| Prüfsummen | `sha256sum -c verifiers/SHA256SUMS` | 3 von 3 OK; `zerl_halb.c`, `zerl_offen.py` und die Prüfdateien nicht berührt |
| Zwölf Zustände K = 12: je 4 offene Punkte, je 1008 Zeilen, die alle überdecken | eigener Zähler (`gate_m.py s0`) und eigene Handrechnung: nötig \|a+b\| ≥ 8 und \|a−b\| ≥ 8 mit a, b ungerade, also (9,1) oder (1,9), 2·2·126 + 2·2·126 = 1008 | 12 von 12 gleich |
| Positivkontrolle (12,1): 8 von 8 gelöst | eigener Zähler auf den 8 Matrizen; `pruefe_matrix.py --erwartet 8` | je 0 offene Punkte, 12 Paare plus 1 freie Zeile; 8 von 8 dreimal Diskrepanz 8 |
| Endstände der 44 Zielläufe (4 / 816 / 820 / 644 / 876 offene Punkte) | eigener Zähler über alle Punkte (`gate_m.py s3`) | 52 von 52 Matrizen gleich der Angabe der Sitzung |
| 160 Zustände K = 11, 9 Endstände, f ≤ 2 überall nein | offene Punkte aller 160 nachgezählt; eigene exakte Entscheidung f ≤ 2 auf je einem Vertreter der 9 Gruppen (`gate_m.py s2`) | 160 von 160 gleich; 9 von 9 nein, maxcov gleich (672 / 702 / 1770 / 2096 / 1322 / 2964 / 2816 / 2076 / 1720) |
| (20,6) ohne eine Paarzeile: f = 1 nein, f = 2 ja, 12 von 12 | eigene Rechnung (`gate_m.py c20`), zugleich Positivkontrolle meines f = 2-Codes | 12 von 12 gleich, 6108 offene Punkte, maxcov 3514 |
| Schritt 4: (16,4) 24 Zustände, (23,7) 12 Zustände, f = 1 überall nein | eigene Rechnung (`gate_m.py s4`) | 36 von 36 nein; offene Punkte gleich (112 … 336 und 2192 / 2688 / 2720) |

## Widerspruch und Einschränkungen

1. **Schritt 3 hat drei Formen gerechnet, aber nur eine gefunden.** In allen 44 Endmatrizen der Ziele (11,2), (10,4), (9,6) bilden die freien Zeilen
   selbst Paare (u,w), (u,−w): jede Matrix besteht aus 12 Paaren, keine einzige hat eine ungepaarte freie Zeile (`gate_m_s3.log.txt`). Die Vermutung der Sitzung
   ist damit geprüft und gilt in allen Läufen, auch im Typ (10,8). Die drei Formen sind also dieselbe Form K = 12, f = 0. Im Typ (9,9) ist das der Zustand mit
   Restzahl 1 aus Auftrag N, im Typ (10,8) ein neuer Wert (816 offene Punkte, 20 von 20 Läufen; N hatte K = 12 dort nicht gerechnet).
   „48 Wörter in keiner der drei Formen“ ist deshalb eine Aussage über reine Paarzustände. Die echte Mischform lief nur in der Positivkontrolle (12,1).
   Erklärung (plausibel, nicht bewiesen): Bei 48 Wörtern ist f immer gerade, und die Paare machen die offene Menge spiegelsymmetrisch (z → −z in der W-Hälfte).
   Wählt die Suche eine freie Zeile als beste Einzelzeile, ist ihr Spiegelbild die naheliegende nächste.
2. **Das liegt auch an meinem Auftrag.** Die Stufe 46 mit ungeradem f ((11,1), (10,3), (9,5)) hätte eine ungepaarte Zeile erzwungen. Sie durfte nur nach einem
   Treffer bei 48 beginnen. Logisch war die Regel richtig (ein 46er gäbe mit einer beliebigen Zeile einen 48er), praktisch hat sie den einzigen Test der Mischform verhindert.
3. **Schritt 2 ist „voll, je Zustand“, sagt aber wenig.** Bei 6 von 9 Vertretern scheitert f = 2 schon an der Zählschranke (jede erste Zeile lässt mehr offen, als eine
   zweite überdecken kann), bei den übrigen bleiben 1, 1 und 7 Kandidaten. Geprüft sind Endstände von `zerl_halb`, nicht Zustände mit K = 11 allgemein.
   Ich habe je Gruppe einen Vertreter selbst entschieden, nicht alle 160.
4. **24 statt 28 Zustände bei (16,4):** Fehler in meinem Auftrag, die Sitzung hat richtig gezählt.
5. **Regel 4:** Blöcke liefen als Hintergrundjobs mit Benachrichtigung statt im Vordergrund (Werkzeuggrenze 10 Minuten). Zwischenstände wurden committet und gepusht. Angenommen.
6. **Block 3f mit 4 statt 8 Startwerten:** wegen der 3-Stunden-Grenze, offen berichtet. Angenommen.
7. `src/misch_such.c` (verworfene erste Fassung, 273 Zeilen) liegt ohne Log im Repo; der Fehlversuch steht in `STATUS.md`. Kein Schaden.

## Nebenbefund (Rechnung des Orchestrators, von keiner Sitzung gegengerechnet)

Der 48er bei (20,6) (`runs/zerlegung/k3_seed1.paar.txt`) ist gegen den Austausch einer Paarzeile starr: Streicht man eine der 12 Paarzeilen, so sind die
beiden gestrichenen Zeilen die einzigen zwei Zeilen, die den Rest überdecken, bis auf Vorzeichen. Das gilt in 12 von 12 Fällen (`gate_m_c20_eindeutig.log.txt`).
Belegart: voll, je Fall. Folge: Von diesem 48er führt kein Tausch „ein Paar gegen zwei freie Zeilen“ zu einem anderen Code.

## Einhaltung

Rechenzeit laut Protokoll 07:13 bis 11:12 UTC, knapp 4 von 6 Stunden; Schritt 3 2 h 55 min von 3 h. Nichts installiert. Eigener Branch, `main` am Ende geholt.
STATUS: eine Befundzeile, zwei Fehlversuche als Einzeiler, eine Dateizeile. LANDKARTE: eine Zeile. Stand-Zeilen nicht angefasst (im Gate nachgezogen). Nichts nach außen gemeldet.

## Bewertung und nächster Schritt

- Erwartung aus dem Auftrag („48 oder 46 sind möglich“, unkalibriert): nicht eingetreten.
- Die Linie (18,5) steht bei 50 Wörtern, die Rekordgrenze liegt bei 40. Auch ein 48er wäre nur ein Tabellenwert. Ich empfehle, die Linie ruhen zu lassen.
- Wer sie doch weiterführt: billiger erster Test wäre (11,1) im Typ (9,9), 8 Startwerte zu 900 s (ein Block von 30 Minuten). Das wäre der erste echte Test der
  Mischform unter 50 Wörtern. Abbruch, wenn kein Lauf unter 100 offene Punkte kommt. Kein Auftrag ohne Entscheidung von Justus.
- Es läuft Auftrag D, Bericht steht aus.
