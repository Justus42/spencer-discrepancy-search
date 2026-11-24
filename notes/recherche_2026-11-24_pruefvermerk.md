# Prüfvermerk zur Gemini-Recherche und zwei Ansätze (24.11.2026, 22:20 Uhr; ergänzt 22:35 Uhr)

Gegenstand: Gemini Deep Research „Untere Schranken der Spencer-Konstante C_10c, binäre Überdeckungscodes und
Methodentransfer“ (22 Seiten, von Justus am 24.11. um 21:57 gebracht). Prüfer: Orchestrator-Claude.
Ergänzt `claude/Uebergabe-2026-11-24-abend.md` (Projekt). Repo-Stand beim Prüfen: `main` = `51328f0` (PR 17 bis 20 gemergt).
Im Repo: Bericht `notes/gemini/recherche_2026-11-24_deep-research.pdf`, dieser Vermerk
`notes/recherche_2026-11-24_pruefvermerk.md`, Skripte `notes/orchestrator/2026-11-24_abend/`, Rückfrage an Sol
`notes/rueckfrage_2026-11-24_auftrag.md` (Branch `orchestrator/rueckfrage-sol-2026-11-24`). Dieselbe Fassung liegt im
Projekt unter `claude/Pruefvermerk-2026-11-24-recherche.md`.

## Urteil

Brauchbar. Der Bericht trennt Quelle, eigene Folgerung und „nicht gefunden“ sauber. Er enthält **einen neuen Hebel**
(Zeilen mit je einer Null an verschiedenen Stellen), bestätigt den Kleitman-Beleg und die Lücke bei selbstkomplementären
unteren Schranken. Mehrere seiner Testvorschläge sind bei uns schon erledigt. Kein Rekordkandidat.

## A. Von mir nachgeprüft

| Nr. | Aussage Gemini | Prüfung | Ergebnis |
|---|---|---|---|
| 1 | Kleitman, „On a combinatorial conjecture of Erdős“, J. Combin. Theory 1 (1966), 209–214; Schranke für beide Paritäten | Websuche, zwei unabhängige Zitate (Cambridge CPC-Artikel von Frankl; arXiv:2411.08325) | bestätigt. Der offene Punkt „Literaturbeleg Kleitman“ ist damit erledigt |
| 2 | Reelle Matrizen sind für n = 1 bis 7, 9, 12 ausgeschlossen, 5/√8 ist bei n = 8 optimal | eigene Rechnung (`kl.py`) | bestätigt, deckt sich mit `LANDKARTE.md` (8: 0,562; 9: 0,633; 12: 0,785). Bedingt auf Kleitman, der jetzt belegt ist |
| 3 | Reis und Song, arXiv:2609.34471 (28.09.2026): obere Schranke 2,9992·√n + 4, untere Schranke 1,0000002·√n für Zweierpotenzen n ≥ 2^50 | arXiv-Seite abgerufen | bestätigt. Dass dort die 8x8-Matrix abgedruckt ist, steht nicht in der Zusammenfassung: unbestätigt |
| 4 | EinsteinArena, Thread 273 (FableCover): Umformulierung als selbstkomplementärer Überdeckungscode, vier Zellen durch untere Schranken zu, K(20,6) „24 bis 64“ ohne Code unter 64, 17x17-Zeuge = Östergård–Kaikkonen | Seite abgerufen | bestätigt. Zusätzlich steht dort: lokale Suche bei n = 15 bleibt bei **466** stehen, „der bekannte Code ohne ein Paar“; Plateaus 5560 (n = 20) und etwa 7300 (n = 18); Suchen unter „vielen Symmetriegruppen“ und Nebenklassen ohne Treffer. Nullspalten kommen dort nicht vor |
| 5 | Schranken (15,4) 23–32, (16,4) 34–64, (18,5) 28–64, (21,6) 34–64, (26,9) 17–56 | Abgleich mit `STATUS.md` (florath-CSV) | stimmen überein |
| 6 | Kéri und Östergård behandeln nur sehr kleine Wortzahlen (M ≤ 12) | nur Abgleich mit Geminis Quellenlage | plausibel, Volltexte weiter nicht eingesehen |

Folge aus Nr. 4: Unser Zellenrahmen und die Erklärung der 466 sind öffentlich bekannt. Neu bleibt nach diesem Stand
K(20,6) ≤ 48. Regel 3 (Neuheit vor Meldung erneut prüfen) gilt unverändert.

## B. Unbelegt oder nur aus zweiter Hand

- „Y. H.“ = Yue Huang, Agent Épi (arXiv:2610.24944): Folgerung Geminis; von mir nicht abgerufen.
- K(20,3) ≤ 1536 durch KI-Agenten (fohn, Oktober 2026): Seite ließ sich nicht auslesen. Unbestätigt. Die Matrixmethode ist
  laut Gemini nur für die Stufe 1792 belegt.
- Originalkonstruktion Östergård–Kaikkonen 1998 und Herkunft aus `had.16.3`: weiter nur über Thread 273.
- Tabelle Gijswijt–Polak (arXiv:2504.01932): nicht selbst eingesehen, nur indirekt konsistent.
- Aufwand der SDP („816 Variablen, 15 Minuten“), alle „unter einer Stunde“-Kriterien, Zahlen zu ShinkaEvolve, Aussagen
  zu Bandeira, Buhai, Schmidt, Davydov, Marosi: ungeprüft, für unsere Entscheidungen nicht tragend.

## C. Widerspruch oder Überschneidung mit unseren Befunden

1. **Untere Schranken schwächer als unsere.** Gemini nennt (23,7) ≥ 22, (25,8) ≥ 19, (30,10) ≥ 21 (Kugelschranke). Unsere
   Tabelle hat 29, 25, 27. Unsere Werte gelten; Gemini hatte keinen Tabellenzugriff.
2. **„Nullspalten öffnen (16,4), (18,5), (21,6) wieder“** ist nicht neu. Mehr Zeilen als Spalten stehen seit jeher in der
   Zellenliste von `STATUS.md`.
3. **Neu: verteilte Einzelnullen.** Ist n − d ungerade, dann ist die größte Kappe nicht die Kugel einer ±1-Zeile, sondern
   die „Doppelkugel“ einer Zeile mit genau einer Null (Größe 2·V(n−1, r); Eindeutigkeit laut Frankl 2017, unbelegt).
   Die Null darf in jeder Zeile woanders stehen. Diese Familie erfasst unsere Zellentabelle nicht; steht die Null immer
   in derselben Spalte, ist man wieder in einer Zelle. Betroffen: **n = 11** (d = 6, Dichte 1,203), **n = 14** (d = 7,
   1,292), **n = 17** (d = 8, 1,306). Die Dichtezeile der LANDKARTE rechnet für diese n schon mit der Doppelkugel, gesucht
   wurde dort nie. Befund 3 der Abend-Übergabe („gebrochene Einträge helfen nicht“) ist damit in seiner starken Form
   nicht gedeckt. Korrektur an Gemini: „n = 20, d = 9“ ist kein Ziel, dort genügt d = 8.
4. **„Ein Z_25-Bahnpaar bei (25,8)“** ist erledigt: Zirkulante n = 25, Maximum 7, vollständig (PR 13).
5. **Freie GPU-Tabu-Suche bei (15,4) mit 30 Wörtern**, Erfolgsschwelle „unter 50 bis 100 offene Punkte“: unrealistisch.
   466 ist die Wand (unsere Suche, Thread 273, Zählung D1 unten). Der PC kommt ohnehin erst nach bestandener Kontrolle.
6. **17x17-Zeuge.** Gemini: „`had.16.3` plus 17. Koordinate“. Unsere Dateien haben Skalarprodukte ±1 und ±3 zwischen
   Zeilen (`ctrl_17`) bzw. 1, 3 und einmal 17 (`L2026_17x17`, doppelte Zeile). Das passt nicht zu „orthogonale 16x16 plus
   eine Spalte“ (nur ±1), wohl aber zu einer punktierten Hadamard-Matrix mit verdreifachter Koordinate. Deutung von mir,
   unbelegt.
7. **Matrixmethode.** Gemini nennt selbst 40 = 5·8 bei (20,6) und 48 = 3·16 bei d = 9. Das stützt den Einwand: Bei 30, 50
   und 62 Wörtern erzwingt ein Kern größer als {0, 1} die härteren Ziele 28, 48, 60. Nicht ausgeschlossen.
8. **Programm-Evolution.** Gemini nennt etwa 150 Auswertungen (ShinkaEvolve) und Claude-Code-Läufe von 16 bis 40 Stunden.
   Das schwächt meinen Einwand „Tausende Aufrufe“. Es bleibt das Problem der Zielgröße (Plateau 466).
9. **Selbstkomplementäre untere Schranken:** laut Gemini nichts in der Literatur. Eine SDP mit Komplementbedingung könnte
   realistisch nur (13,3) schließen (28 → 31), kaum (15,4) (23 → 31). Mein Urteil, unkalibriert.

## D. Eigene Rechnungen des Orchestrators (Skripte im Repo, nicht gegengerechnet)

Skripte: `notes/orchestrator/2026-11-24_abend/` (mit README). D4 bis D8 rechnen die Sitzungen A und B nach.
**D1 bis D3 stehen in keinem Auftrag** und brauchen eine eigene kurze Gegenrechnung.

| Nr. | Rechnung | Ergebnis | Belegart |
|---|---|---|---|
| D1 | (15,4), Symmetrie der Ordnung 7, Spaltenzyklen 7+7+1 (wie `ctrl_16`): 2340 volle Bahnen, 4 feste Zeilen, 2344 Punktklassen | 32 Wörter (2 Bahnen + 2 feste): **112 Lösungen**, je 28 für die Festmengen (0,1), (0,2), (1,3), (2,3), 0 für (0,3), (1,2); eine Lösung über `pruefe_matrix.py`: 7/7/7. **30 und 28 Wörter: 0 Lösungen.** Kleinste Restzahl bei 30: 466 (je 56 Paare) | voll |
| D2 | (15,4), Ordnung 13 (13+1+1) und Ordnung 11 (11+1⁴), eine Bahn plus feste Zeilen | 0 Lösungen bei 30, 32, 34 Wörtern | voll |
| D3 | `ctrl_16` ohne eine Zeile | bei jeder der 16 Zeilen genau 466 offene Punkte | voll |
| D4 | Elemente der Ordnung 3 und 7 in den Gruppen | `ctrl_16`: Ordnung 7 mit Spalten 7+7+1, Zeilen 7+7+1+1; Ordnung 3 mit Spalten 3⁴1³. `had24_1p3`/`2p2`: 3-Sylowgruppe Ordnung 9, nicht zyklisch; jedes Element Ordnung 3 mit Spalten 3⁶1², Zeilenpaare 3⁶1⁶ | exakt (bliss, sympy) |
| D5 | (20,6), 3-Sylowgruppe P des 48er-Codes: Bahnen auf Zeilenpaaren | 8 der Größe 1, 160 der Größe 3, 58 200 der Größe 9; 58 368 Punktklassen. Der 48er besteht aus genau 8 Bahnen der Größe 3. In der Teilfamilie „nur Größe 1 und 3“ findet CP-SAT 24 Zeilen, nichts darunter in 150 s; „höchstens 20 Zeilen“ in 130 s unentschieden | Suche (kein Beweis) |
| D6 | n = 11, d = 6, Zeilen mit genau einer Null: 5632 Zeilen, 1024 Punktklassen, je Zeile 112, Schnittgrößen einer Zeile mit den anderen {0, 6, 12, 26, 40, 76, 112}, Budget für Mehrfachüberdeckung 208. Aufgeschlüsselt für a = (0,1,…,1): Null in derselben Spalte, Abstand 1 bis 5: 40, 40, 12, 12, 0 (Anzahl 10, 45, 120, 210, 126); Null in anderer Spalte, Abstand 0 bis 4 auf 9 gemeinsamen Koordinaten: 76, 40, 26, 12, 6 (Anzahl 20, 180, 720, 1680, 2520) | eigene Verzweigungssuche: 277 916 Knoten in 240 s, nicht fertig, nie mehr als 7 Zeilen tief. CP-SAT 150 s unentschieden | Suche (kein Beweis) |
| D7 | Kontrolle zu D6 bei n = 8, d = 5 | 8 Zeilen: Lösung nach 8 Knoten, `pruefe_matrix.py` 5/5/5 (Hamming). 7 Zeilen: keine | voll |
| D8 | Zirkulante mit Nulldiagonale (Null je Zeile woanders), alle ersten Zeilen bis auf Multiplikator und Vorzeichen | n = 11: 56 Klassen, Maximum **4** (nötig 6). n = 14: 700 Klassen, Maximum **5** (nötig 7). n = 17: 2068 Klassen, Maximum **6** (nötig 8) | voll |
| D9 | Dichten | (15,4)/30: 1,777; /32: 1,896. (20,6)/40: 2,306; /48: 2,768. (25,8)/50: 2,694. (30,10)/62: 3,061 | exakt |

Folgerung aus D1 bis D3: Ein Code mit 30 Wörtern bei (15,4) hat keine Symmetrie der Ordnung 7 (Typ 7+7+1), 11, 13 und
keine aus drei 5er-Zyklen (PR 14). Offen: Ordnung 2 und 3, die Typen 7+1⁸, 5+5+1⁵, 5+1¹⁰, oder gar keine Symmetrie.
Empfehlung: Zelle (15,4) zurückstellen.

## E. Zwei Ansätze (von Justus am 24.11. um 22:21 freigegeben, Sitzungen laufen seit 22:26)

Beide: Opus, Effort hoch (neuer Suchcode), eigener Branch, eigenes Protokoll, `STATUS.md` erst am Ende, höchstens zwei
Sitzungen gleichzeitig. Chancen unkalibriert und klein; der Wert liegt auch im vollständigen Negativbefund.
Die Texte unten sind die gestarteten Fassungen.

### Ansatz A: Zeilen mit einer Null, n = 11 exakt, dann n = 14

```
Auftrag A (Opus, Effort hoch): Zeilen mit einer Null

Lies zuerst STATUS.md, LANDKARTE.md, CLAUDE.md. Eigener Branch, Protokoll
notes/laeufe/2026-11-25_einzelnull.md, STATUS.md erst am Ende. Bloecke von hoechstens 30 Minuten,
nach jedem Block committen und pushen. Suchcode und Pruefcode getrennt. Vor jedem Treffer-Bericht
sha256sum -c verifiers/SHA256SUMS und python3 verifiers/pruefe_matrix.py.

Ziel: exakt entscheiden, ob es eine 11 x 11-Matrix mit Eintraegen in {-1, 0, 1} und Diskrepanz >= 6 gibt.
6/sqrt(11) = 1,809 waere ein Rekord (Schwelle 5/sqrt(8) = 1,7678).
Hintergrund: Zeile a trifft x, wenn |<a,x>| >= 6. Bei einer Null und zehn Eintraegen +-1 ist die Treffermenge
eine Doppelkugel (Kleitman, ungerader Durchmesser). Die Null darf in jeder Zeile woanders stehen. Steht sie immer
in derselben Spalte, ist es die Zelle (10,2) mit 22 Woertern, ausgeschlossen durch K(10,2) >= 24.

Teil 1, Gegenrechnung der Angaben des Orchestrators mit eigenem Programm. Punkte modulo Vorzeichen: 1024 Klassen.
 a) Zeilen mit genau einer Null (modulo Vorzeichen): 5632, jede trifft 112 Klassen.
    Schnittgroessen einer Zeile mit allen anderen: {0, 6, 12, 26, 40, 76, 112}.
    Aufgeschluesselt fuer a = (0,1,...,1): Null in derselben Spalte, Abstand 1 bis 5 (modulo Komplement):
    40, 40, 12, 12, 0 (Anzahl 10, 45, 120, 210, 126). Null in anderer Spalte, Abstand 0 bis 4 auf den
    9 gemeinsamen Koordinaten: 76, 40, 26, 12, 6 (Anzahl 20, 180, 720, 1680, 2520).
    Budget fuer Mehrfachueberdeckung bei 11 Zeilen: 11*112 - 1024 = 208.
 b) Von Hand, bitte nachrechnen: Traeger 8: 21120 Zeilen, je 72 Klassen. Traeger 6: 14784 Zeilen, je 32 Klassen.
    Zeilen mit ungeradem Traeger (11, 9, 7) sind von einer Zeile mit einem Eintrag mehr oder weniger dominiert:
    pruefen und begruenden.
 c) Zirkulante mit Nulldiagonale: n = 11: 56 Klassen, Maximum 4. n = 14: 700 Klassen, Maximum 5.
    n = 17: 2068 Klassen, Maximum 6.
Bei Abweichung: stoppen und berichten.

Teil 2, Positivkontrolle in derselben Struktur: n = 8, d = 5, Zeilen mit einer Null und sieben Eintraegen.
 Erwartet: mit 8 Zeilen gibt es Loesungen (Hamming plus Nullspalte, controls/ctrl_8.txt), mit 7 Zeilen keine.
 Zusaetzlich zaehlen: Gibt es bei n = 8 Loesungen, in denen die Nullen NICHT alle in einer Spalte stehen?
 Die Suche fuer n = 11 startet erst, wenn die Kontrolle besteht.

Teil 3, vollstaendige Suche n = 11, 11 Zeilen, zuerst nur Traeger 10, dann Traeger 10, 8 und 6 gemischt.
 Verzweigen an einem unueberdeckten Punkt, Budget 208 als Schnitt, Symmetrie der vorzeichenbehafteten
 Permutationen nutzen (erste Zeile fest, zweite Zeile nur Bahnvertreter). Die Python-Suche des Orchestrators
 kam in 240 s auf 277 916 Knoten und nie tiefer als 7 Zeilen; C mit Symmetrie sollte fertig werden.
 Obergrenze: 2 Stunden Rechenzeit fuer Traeger 10, weitere 2 Stunden fuer die gemischte Fassung.
 Abbruch: Zeit um. Dann Baumstatistik berichten, Belegart "Suche erfolglos".
 Ergebnis mit Belegart: "vollstaendig durchgerechnet" nur, wenn der Baum ganz abgearbeitet ist.
 Das Ergebnis gilt fuer Eintraege in {-1, 0, 1}, nicht fuer beliebige reelle Eintraege; so vermerken.

Teil 4, nur wenn Teil 3 fertig ist und Zeit bleibt: n = 14, d = 7, 14 Zeilen mit je einer Null.
 Erwartet: 57344 Zeilen, 8192 Klassen, je Zeile 756 Klassen, Budget 14*756 - 8192 = 2392.
 Tabu-Suche (Suchcode), Leiter abwaerts ab 21 Zeilen (bekannt: K(13,3) <= 42 mit fester Nullspalte),
 je Stufe 4 Startwerte mal 15 Minuten, Stopp bei der ersten Stufe ohne Loesung. Rekord erst bei 14 oder
 15 Zeilen. Jede Stufe unter 21 zeigt, dass verteilte Nullen mehr koennen als eine feste Nullspalte.

Am Ende: LANDKARTE.md ergaenzen (Regel 9), Fehlversuche als Einzeiler, nichts nach aussen melden.
```

Erster Test (D6 bis D8): gemacht. Kontrolle besteht, n = 11 ist in Minuten weder gefunden noch ausgeschlossen, die
naheliegendste algebraische Form (Zirkulante) scheidet aus.

### Ansatz B: (20,6) unter der 3er-Gruppe des 48er-Codes

```
Auftrag B (Opus, Effort hoch): Bahnen unter der 3-Sylowgruppe des 48er-Codes

Lies zuerst STATUS.md, LANDKARTE.md, CLAUDE.md, notes/laeufe/2026-11-24_bahnen5.md (gleiche Methode mit
Ordnung 5). Eigener Branch, Protokoll notes/laeufe/2026-11-25_bahnen3.md, STATUS.md erst am Ende. Bloecke von
hoechstens 30 Minuten, committen und pushen. Suchcode und Pruefcode getrennt, Treffer ueber
python3 verifiers/pruefe_matrix.py, vorher sha256sum -c verifiers/SHA256SUMS.

Ziel: selbstkomplementaere Codes der Laenge 20 mit Radius 6 und weniger als 48 Woertern, invariant unter der
3-Sylowgruppe P (Ordnung 9, nicht zyklisch) der Automorphismengruppe von controls/had24_1p3_paley12.txt
(Gruppe der vorzeichenbehafteten Spaltenpermutationen, Ordnung 576, wie in src/bahnen_aut.py).
40 Woerter (20 Zeilen) waeren ein Spencer-Rekord (8/sqrt(20) = 1,789). 46, 44 oder 42 waeren Tabellenergebnisse.

Teil 1, Gegenrechnung mit eigenem Programm. Erwartet:
 - Bahnen von P auf Zeilenpaaren {v, -v}: 8 der Groesse 1, 160 der Groesse 3, 58 200 der Groesse 9.
 - Punktklassen: 58 368.
 - Der 48er-Code besteht aus genau 8 Bahnen der Groesse 3 und ueberdeckt alles.
 - Jedes Element der Ordnung 3: Spaltenzyklen 3^6 1^2, auf den 24 Zeilenpaaren 3^6 1^6.
Bei Abweichung: stoppen und berichten.

Teil 2, Positivkontrolle und vollstaendige Zaehlung in der Teilfamilie "nur Bahnen der Groesse 1 und 3"
(168 Bahnen): kleinste Zeilenzahl einer Ueberdeckung, exakt. Erwartet: 24 wird erreicht (der 48er muss unter den
Loesungen sein). Der Orchestrator fand mit CP-SAT in 150 s nichts unter 24, ohne Beweis. Obergrenze 1 Stunde.

Teil 3, vollstaendige Zaehlung mit Bahnen der Groesse 9, alle Ueberdeckungen mit hoechstens 23 Zeilen:
 2 Neunerbahnen plus hoechstens 5 weitere Zeilen, 1 Neunerbahn plus hoechstens 14 weitere Zeilen.
 Reihenfolge: zuerst hoechstens 20 Zeilen (Rekord), dann 21 bis 23. Verfahren wie src/bahnen_zaehl.c
 (Verzweigen am unueberdeckten Punkt, jede Menge genau einmal). Obergrenze 3 Stunden Rechenzeit.
 Abbruch: Zeit um. Dann berichten, welche Faelle vollstaendig sind und welche nicht.

Teil 4, nur wenn Teil 2 und 3 ohne Treffer fertig sind und Zeit bleibt: nur ein Element der Ordnung 3
(Bahnen der Groesse 3 und feste Zeilen, etwa 175 000 Bahnen). Tabu-Suche nach dem Muster von src/bahnen_tabu.c.
 Kontrolle: 48 Woerter aus Zufallsstart finden, mindestens 2 von 4 Startwerten in je 15 Minuten. Ohne
 bestandene Kontrolle keine Leiter. Dann Leiter 46, 44, 42, 40, je 4 Startwerte mal 15 Minuten, Stopp bei
 der ersten Stufe ohne Loesung.

Am Ende: LANDKARTE.md ergaenzen (Regel 9), Fehlversuche als Einzeiler, nichts nach aussen melden.
```

Erster Test (D4, D5): gemacht. Die Struktur stimmt, der 48er liegt in der kleinsten Teilfamilie, darunter ist in
Minuten nichts zu finden und nichts auszuschließen.

## F. Nicht verfolgt, mit Grund

- Freie Suche bei (15,4) mit 30 Wörtern, auch auf der GPU: Wand bei 466, zweimal unabhängig erklärt.
- Kontinuierliche Minimax-Optimierung reeller Matrizen: erst sinnvoll, wenn Ansatz A bei n = 11 etwas zeigt; die
  Extremform (Doppelkugel) prüft A exakt.
- Matrixmethode mit großem Kern: nur bei (20,6) mit 40 = 5·8 ohne härteres Ziel; zurückgestellt hinter B.
- Selbstkomplementäre SDP: kann keinen Rekord liefern, höchstens (13,3) schließen. Kandidat für später.
- Programm-Evolution: zurückgestellt, bis eine Zielgröße ohne Plateau vorliegt.

## G. Rückfrage an GPT 6.2 Sol

Von Justus am 24.11. um 22:21 freigegeben, als Batch abgegeben um 22:22 (Denkstufe xhigh). Frage: Beweis oder
Gegenbeispiel für n = 11 mit 11 Doppelkugeln, Erweiterung auf alle Träger, Familie n = 8, 11, 14, 17, notwendige
Bedingungen als Schnitte für die Baumsuche. Auftragstext: `notes/rueckfrage_2026-11-24_auftrag.md`. Die Antwort kommt
nach `notes/openrouter/`; Beweise und Zeilenlisten daraus gelten erst nach eigener Nachrechnung (Regel 4).

## H. Offen nach diesem Vermerk

- Erst wenn A und B fertig sind (sie ändern `STATUS.md` und `LANDKARTE.md` am Ende selbst): Kleitman-Beleg in
  `STATUS.md` und `LANDKARTE.md` eintragen („unbelegt“ streichen), Thread 273 als Fundstelle für „466“ und für den
  Zellenrahmen nennen, den Bericht in `controls/QUELLEN.md` oder STATUS verlinken.
- D1 bis D3 mit eigenem Code gegenrechnen (kurze Sonnet-Sitzung nach Vorlage `src/bahnen_zaehl.c`), dann als
  Negativbefund mit Belegart „voll“ in STATUS und LANDKARTE.
- Sols Antwort prüfen.
