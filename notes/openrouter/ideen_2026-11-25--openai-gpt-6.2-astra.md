> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-astra
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 25.11.2026, 17:18
> - Batch: Auftrag openrouter:batch-1795622150-gMrbDNRfkbAJeev1G0hr, abgegeben 25.11.2026, 16:55, fertig 25.11.2026, 17:00
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 21.356 · Ausgabe 16.138 (davon Denken 11.305) · gesamt 37.494
> - Kosten laut OpenRouter: 0,41 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: xhigh
> - finish_reason: stop

---

[U] Ich würde die zehn CPU-Stunden auf **\((15,4)\), drei Zeilenpaare des 32ers entfernen und durch zwei völlig freie reelle Zeilen ersetzen**, setzen. [R] Bei festgehaltenen 13 Zeilen hat diese Aufgabe nur **699 bis 1 119 offene Antipodenklassen**; für die beiden neuen Zeilen gibt es einen exakten Test durch Vorzeichenzuordnung und lineare Programme. [U] Weitere Matrizenfamilien, weitere Zufallsstarts im bekannten Spann und eine reelle Suche bei \(n=10\) würde ich lassen; für \(n=10\) ergibt sich unten sogar ein Ausschluss.

## 1. Welche Annahme ist zu eng?

**[U] Erstens: „Eine Ersatzzeile muss wieder eine Hamming-Kugel liefern.“** Das würde ich bei \(n=15\) aufgeben, nicht pauschal bei allen \(n\).

[R] Beispiel für den Spielraum: Eine Zeile mit 13 Einträgen \(1\) und zwei Einträgen \(1/16\) trifft bei der Rekordschwelle \(t_0=5\sqrt{15/8}\) auf der positiven Seite genau die Punkte mit höchstens drei Fehlern auf den starken Koordinaten. Denn \(7-2/16>t_0>5+2/16\). Das sind \(4(1+13+78+286)=1\,512\) Punkte. Sie passen **in keine** Radius-4-Kugel: Gegen jeden Mittelpunkt lassen sich beide schwachen Stellen und mindestens drei starke Stellen verfehlen. Die Volumendichte von 15 solchen Zeilenpaar-Kappen wäre \(15\cdot1\,512/2^{14}=1{,}384\). Das ist kein Bauvorschlag mit 15 gleichartigen Zeilen, sondern ein Beleg, dass eine einzelne Ersatzzeile eine andere Lochform schließen kann, ohne am Volumen zu scheitern.

**[U] Zweitens: „Man muss den gesamten Code strukturieren, damit die Suche klein wird.“** Ich würde stattdessen einen Teil des Codes festhalten und den *Ersatz* strukturfrei lassen.

[R] Im Spann von `ctrl_16` sind nur \(128/16\,384=1/128\) aller möglichen binären Zeilenpaare zugänglich; die mitgelieferte Schranke \(128/8=16\) schließt gerade dort die gewünschte Kompression aus. Fixiert man dagegen 13 **Zeilen**, so beschränkt das weder den Spann noch die Symmetrie der zwei Ersatzzeilen. Der Preis dieser anderen Einschränkung ist genau benennbar: Man erfasst nur Lösungen mit einem gemeinsamen 13-Zeilen-Teil mit der Kontrolle.

**[U] „Oberste Zelle“ und „\(N\approx m\)“ würde ich vorerst nicht als Hauptfehler behandeln.** [R] Etwa bei \((20,5)\) müsste man laut eurer Tabelle mit 62 Wörtern die allgemeine Untergrenze **genau** erreichen, zusätzlich selbstkomplementär. Zwei freie Zeilen bei \((15,4)\) geben einen näheren und billiger prüfbaren Ansatzpunkt.

**[R] Nebenresultat: Für reelle \(10\times10\)-Matrizen reicht auch die Dichte \(1{,}094\) nicht.** Der folgende Ausschluss braucht für den rein binären Schluss nur die in `STATUS.md` angegebene Schranke \(K(10,2)\ge24\).

* [R] Eine positive Zeilenkappe \(a\cdot x>5\sqrt{10/8}\) hat Durchmesser höchstens 4, denn \(a\cdot(x+y)\le 2(10-d_H(x,y))\). Nach Ausrichtung an den Vorzeichen von \(a\) bilden ihre Fehlermengen eine nach unten abgeschlossene Familie \(\mathcal F\). Für \(S,T\in\mathcal F\) gilt sogar \(|S\cup T|\le4\): Man wende die Durchmesserschranke auf \(S,T\setminus S\) an.
* [R] Entweder liegt die Kappe in der gewöhnlichen Radius-2-Kugel, oder sie hat **höchstens 43 Punkte**. Zum Nachrechnen: Enthält sie eine Vierermenge, so liegen alle Mengen darin. Andernfalls fixiere eine enthaltene Dreiermenge. Jede Zweiermenge muss sie treffen, also gibt es höchstens \(\binom{10}{2}-\binom72=24\) Zweiermengen. Die Dreiermengen schneiden einander in mindestens zwei Stellen; entweder enthalten alle dasselbe Paar, oder sie liegen in derselben Vierermenge. Letzteres sieht man an \(abc,abd\): Ein drittes Tripel ohne \(ab\) muss \(acd\) oder \(bcd\) sein. Also gibt es höchstens acht Tripel und insgesamt höchstens \(1+10+24+8=43\) Punkte.
* [R] Rechne auf den 512 Antipodenklassen und vergrößere jede gewöhnliche Kappe zur vollen Kugel mit 56 Klassen. Zwei Mittelpunkte gleicher Gewichtsparität haben gefalteten Abstand höchstens 4; ihre Kugeln schneiden sich in mindestens sechs Klassen (bei Abstand 4 die \(\binom42\) Mittelpunkte). Jede weitere Kugel nach der ersten ihrer Parität kostet also mindestens sechs Doppelzählungen. Bei \(e\ge1\) außergewöhnlichen Kappen ist die Vereinigungsgröße höchstens
  \[
  56(10-e)+43e-6\max(8-e,0)\ \le\ 505 <512.
  \]
  Bei \(e=0\) ergäbe die Vergrößerung einen binären Radius-2-Code mit höchstens 20 Wörtern, im Widerspruch zur angegebenen Untergrenze 24. Damit ist auch der reelle Fall erledigt.

## 2. Zwei Ansätze

### A. Zwei freie reelle Zeilen: Vorzeichenverzweigung mit LP-Zertifikaten

**(a) Vorbild.** [L] Logikbasierte Benders-Zerlegung: Eine diskrete Instanz verteilt Aufgaben, ein stetiges Teilproblem realisiert die Verteilung oder liefert eine verbotene Teilzuordnung. [R] Hier verteilt man nur die *offenen Punkte*, nicht die Einträge einer ganzen Matrix.

**(b) Mechanismus.** [R] Streiche drei der 16 Zeilenpaare; jeder offenen Klasse mit Vertreter \(x\) gib eines der vier Etiketten \((i,s)\in\{1,2\}\times\{\pm1\}\), und verlange dann \(s\,a_i\cdot x\ge t\), \(\|a_i\|_\infty\le1\). [R] Für eine gegebene Zuordnung maximiert ein LP \(t\); ein rationales Optimum \(t\ge0\) besteht den Rekordtest genau bei **\(8t^2>375\)**, sodass auch Treffer strikt zwischen \(6{,}847\) und 7 nicht durch eine Ganzzahlschwelle verloren gehen.

**(c) Billigster Zieltest.** [R] Für ein einziges gelöschtes Tripel die Löcher bilden und zuerst *ohne LP* inkompatible Etiketten streichen: Sollen \(x,y\) dieselbe Zeile benutzen, müssen die orientierten Vertreter \(sx,s'y\) Abstand höchstens 8 haben. Auf Abstand höchstens 6 der ursprünglichen Vertreter ist also gleiches Vorzeichen erzwungen, auf Abstand mindestens 9 entgegengesetztes. Erst danach LPs einsetzen; eine bereits zu schwache Teilzuordnung braucht nie erweitert zu werden. [R] Äquivalent ist der beste Spielraum für eine nichtleere Menge orientierter Vertreter \(v_j\) einer Zeile
\[
\max_{\|a\|_\infty\le1}\min_j a\cdot v_j
=\min_{\lambda\ge0,\ \sum_j\lambda_j=1}\left\|\sum_j\lambda_j v_j\right\|_1 .
\]
Damit kann ein negatives LP-Ergebnis durch Gewichte auf den tatsächlich störenden Punkten belegt werden.

[R] **Größe des Tests:** Jedes gelöschte Paar hat nach eurer Alleinanteilzählung \(466/2=233\) private Klassen. Zwei verschiedene Paarkugeln des punktierten Hadamard-Codes schneiden sich in genau \(2\binom73+\binom84=140\) Klassen: Die beiden relevanten Abstände sind 7 und 8. Jedes weitere Loch nach Streichung dreier Paare gehört zu mindestens einem ihrer drei paarweisen Schnitte. Also
\[
699=3\cdot233\ \le |H|\le\ 3\cdot233+3\cdot140=1\,119.
\]
[R] Erfolg: **0 Löcher** mit zwei neuen Zeilen und streng bestandenem Schwellentest. Vollständiger Misserfolg für dieses Tripel: alle Zuordnungen widerlegt. Ein Zeitlimit liefert **unentschieden**, nicht „unmöglich“.

**(d) Abbruch.** [U] Die konkreten Kontroll-, Pilot- und Zeitgrenzen stehen in Abschnitt 4. [R] Selbst 560 vollständig negative Entscheidungen würden nur die \(\binom{16}{3}=560\) Nachbarschaften dieses einen 32ers ausschließen. Anders als beim früheren reellen Test um `ctrl_16` gilt hier keine \(L^1\)-Schranke um eine alte Zeile; anders als bei den Drei-Eintrags-Änderungen werden zwei ganze Zeilen gemeinsam synthetisiert.

**(e) Positivkontrolle.** [U] Zwei statt drei Paare des 32ers verdecken und bei 14 festen Zeilen zwei Zeilen **blind** zurückgewinnen; Soll: 16 Zeilen, **0 Löcher bei \(t=7\)**. [R] Die Kontrollinstanz hat zwischen 466 und 606 Lochklassen. [U] Für die binäre Wiederfindung des Kontrollobjekts darf der Löser zuerst Würfelecken probieren, aber die verdeckten Zeilen nicht als Kandidaten einlesen.

### B. Einen ganzen Spaltenvektor exakt optimieren: Projektion plus signierte NAE-Bedingungen

**(a) Vorbild.** [L] Zerlegung eines Erweiterungsproblems in eine Projektion und ein Färbungsproblem; NAE („not all equal“) verlangt, dass nicht alle Literale dieselbe Farbe haben. [R] Anders als bei einem Spaltenanbau an den 48er soll hier die **Projektion selbst verändert** werden; die letzte Spalte wird aus der Programmsuche eliminiert.

**(b) Mechanismus.** [R] Projiziere \(k\) Zeilenpaare von Länge 15 auf Länge 14: Für eine Basisklasse \(y\) deckt ein projiziertes Wort im Abstand \(\le3\) beide Lifts, während bei ausschließlich Abstand 4 die getroffenen orientierten Wörter beide letzten Bits liefern müssen. [R] Sind \(b_y\) Wörter genau auf Abstand 4 und gibt es keinen inneren Treffer, so sind bei \(b_y=0,1\) genau \(2,1\) Löcher erzwungen; bei \(b_y\ge2\) ergibt sich eine signierte NAE-Bedingung mit **genau einem** Loch bei Verletzung.

**(c) Billigster Zieltest.** [R] Nimm `ctrl_16` minus ein Paar und verdecke eine ganze Spalte. Bei \(k=15\) gibt es nach globalem Spaltenvorzeichen nur \(2^{14}=16\,384\) bitweise verschiedene Ergänzungen zu vergleichen, und nur \(2^{13}=8\,192\) Basisklassen. Gleiche NAE-Bedingungen dürfen mit Multiplizität zusammengefasst werden. [R] Die alte Spalte garantiert ein Minimum \(\le233\) Lochklassen. **Erfolg des ersten Tests: \(\le232\); Misserfolg: genau 233.** [U] Das ist ein aussagekräftigerer Test als ein weiterer Lauf mit einzelnen Bit-Flips: Er entscheidet den besten gemeinsamen Wechsel aller 15 Einträge dieser Spalte.

**(d) Abbruch.** [U] Zunächst nur die höchstens \(16\cdot15=240\) Löschpaar-/Spaltenwahlen, äquivalente Wahlen nur einmal. Kein Wert unter 233 und keine Verbesserung nach höchstens einer weiteren CPU-Stunde mit veränderten 14-Spalten-Projektionen: abbrechen, nicht dieselbe freie Suche umbenennen. [R] Für veränderte Projektionen ist
\[
D=\sum_{y\ {\rm ohne\ inneren\ Treffer}}(2-b_y)_+
\]
eine billige Untergrenze vor jeder NAE-Optimierung. Die exakte Restzahl ist \(D\) **plus** die minimale gewichtete Zahl verletzter NAE-Bedingungen.

**(e) Positivkontrolle.** [U] Beim vollständigen 32er eine Spalte verdecken und eine Ergänzung mit **0 Löchern** finden; bei Aufzählung aller optimalen Ergänzungen muss die Kontrollspalte bis auf Vorzeichen darunter sein. [R] Dafür sind \(2^{15}\) statt \(2^{14}\) Ergänzungen ausreichend.

## 3. Bewertungsleiter für Suchprogramme

**[U] Ich würde keinen einzigen zellübergreifenden Restzahl-Score verwenden.** Stattdessen: ein Archiv für gültige Eltern, eines für fast gültige Kinder des jeweils nächsten Budgets und ein kleines Nebenarchiv für vielversprechende Projektionen. Programme bekommen dasselbe CPU-Budget und werden auch an verdeckten, umnummerierten Kontrollen geprüft.

1. **[R] Zuerst die Einheit vereinheitlichen.** Bei antipodalen Paaren sei \(h\) die Zahl offener Antipodenklassen. Dann sind die beiden Referenzen \(233/16\,384=1{,}422\%\) und \(3\,054/524\,288=0{,}583\%\). Bei den **886 Punktklassen** ist vor dem Vergleich die Klassenmultiplizität aus dem Prüfer auszulesen: Nur wenn es gewöhnliche Antipodenklassen sind, ist \(h=886\); sonst die Lochgewichte summieren und auf den Antipodenquotienten umrechnen. [U] Insbesondere würde ich 886 nicht als Rohpunktzahl in die Bewertung übernehmen.
2. **[U] Gültiger Elterncode plus Kompressionsbonus.** Primär zählt der kleinste *gültig* erreichte Umfang je Zelle. Bei gleichem Umfang zählt die beste vom Programm gelieferte und exakt geprüfte Lösung mit **einem Paar weniger**. Für eine Stufe mit Referenz \(b\) verwende ich für dieses Kind \(g=\log_2((1+b)/(1+h))\). [R] Bei \(b=233\) ergibt \(h=116\) genau einen Bonuspunkt; bei \(b=3\,054\) ergibt \(h=1\,526\) nahezu einen. Ein gültiger Code bleibt eine eigene Stufe und wird nicht gegen beliebig viele Bonuspunkte verrechnet.
3. **[U] Gleichstand nach *Reparierbarkeit* auflösen.** Für binäre Kandidaten in \((15,4)\) zähle ich zusätzlich die beste Restzahl nach exakter Einspalten-Ergänzung aus B. Im Nebenarchiv behalte ich Projektionen nach dem Wertepaar **\((D,\ h_{\rm ergänzt}-D)\)**, mit Minimierung beider Komponenten und ohne Dominanzvergleich zwischen verschiedenen Paarbudgets. [R] Die erste Zahl misst Löcher, die *kein* Spaltenprogramm mehr retten kann; die zweite misst widersprüchliche Bitforderungen. Die bloße Restzahl unterscheidet diese Ursachen nicht. Außerdem haben nach euren Daten **alle 16** Einzelstreichungen des 32ers exakt denselben Wert 233. [U] Das belegt einen konkreten Gleichstand, nicht das Fehlen jedes Gefälles in der gesamten Zelle. Ein Nebenarchiv ist wichtig, damit ein Programm eine vorübergehend höhere Restzahl bei besserer Projektion überhaupt weiterverfolgen darf.

**[R] Echte Stufen:** Bei \((20,6)\) liefern gültige selbstkomplementäre **46, 44, 42, 40** Wörter nacheinander \(8/\sqrt{23},8/\sqrt{22},8/\sqrt{21},8/\sqrt{20}\); erst 40 schlagen den Rekord. Die 6 108 offenen Punkte sind die Referenz für die angegebene **44er-Streichung**, keine Untergrenze für andere 44er. Bei \((15,4)\) ist schon ein geprüftes Kind mit **höchstens 464 Rohpunktlöchern** ein messbarer Fortschritt gegenüber 466; erst null ist eine gültige Kompression.

**[R] 31 Wörter ohne Selbstkomplementarität sind eine Nebenleistung, keine solche Hauptstufe.** Bei \(s\) ungepaarten Wörtern hat der antipodale Abschluss \(31+s\) Wörter, \(s\) ungerade und mindestens 1. Selbst im besten Fall ist man wieder bei 32. [U] Ich würde deshalb einen solchen Treffer mit \(s\) protokollieren, aber nicht höher als einen gültigen 32er mit besserem 30er-Kind bewerten.

## 4. Genau ein Vorhaben für zehn CPU-Stunden

**[U] Ansatz A, nur `ctrl_16`, Zielgröße \(15\times15\), 13 feste binäre und zwei freie reelle Zeilen.** Keine zweite Hadamard-Klasse und kein Wechsel zur Spaltensuche innerhalb dieses Budgets.

| Schritt | Grenze und Entscheidung |
|---|---|
| [U] Kontrolle | Zwei verdeckte Zweierlöschungen, je höchstens 300 CPU-s. **2/2** mit 0 Löchern bei \(t=7\) erforderlich, sonst Stopp. |
| [U] Pilot | 24 verschiedene Dreierlöschungen, je 55 CPU-s. Nur fortfahren, wenn mindestens eine Instanz vollständig entschieden wurde **oder** ein verifizierter Zwischenkandidat höchstens **232** Lochklassen hat. Bei 24 Zeitabläufen ohne diesen Fortschritt: Stopp. |
| [U] Hauptlauf | Erste Runde höchstens 55 CPU-s je Tripel, einschließlich Pilot. Bei Erfüllung des Pilotkriteriums verbleibende Zeit auf unentschiedene Tripel mit den kleinsten gefundenen Restzahlen verteilen. Bei Rekordkandidat sofort unabhängig prüfen. |
| [R] Budget | \(2\cdot300+560\cdot55=31\,400\) CPU-s; bis 10 CPU-h bleiben 4 600 CPU-s für Gegenprüfung und Vertiefung. |

[R] Einen reellen Treffer kann man ohne Gleitkomma zertifizieren: rationale Zeilen auf einen gemeinsamen Nenner \(q\) bringen, alle \(2^{14}\) Antipodenklassen mit ganzen Zahlen prüfen und für das kleinste erreichte ganzzahlige Zeilenmaximum \(M\) die Ungleichung \(8M^2>375q^2\) testen. [U] Ich würde dieses Zertifikat verlangen, nicht die Toleranzmeldung eines LP-Lösers.

## 5. Rangfolge der fünf obersten Zellen

**[R] Maß: erwartete absolute Zahl offener Antipodenklassen bei unabhängigen gleichverteilten Zeilenpaaren im Zielbudget.** Für \(Q=2^{m-1}\), \(V=\sum_{j=0}^R\binom mj\), \(k=B(d)\) ist das exakt
\[
E_{\rm offen}=Q(1-V/Q)^k .
\]
[R] Dazu ist keine Unabhängigkeit der *Punkte* nötig; für jeden festen Punkt sind die \(k\) Zeilenziehungen unabhängig. Klein ist gut. Das Maß bewertet die absolute Reparaturarbeit eines Zufallsstarts, nicht nur den offenen Anteil.

| Rang [R] | Zelle | \(k\) | \(E_{\rm offen}\), gerundet |
|---:|---:|---:|---:|
| 1 | \((15,4)\) | 15 | \(2{,}47\cdot10^3\) |
| 2 | \((20,6)\) | 20 | \(4{,}53\cdot10^4\) |
| 3 | \((25,8)\) | 25 | \(9{,}7\cdot10^5\) |
| 4 | \((30,10)\) | 31 | \(2{,}14\cdot10^7\) |
| 5 | \((37,13)\) | 38 | \(1{,}3\cdot10^9\) |

[R] Beispiel für Rang 1: \(V=1\,941,\ Q=16\,384\). Die bekannte Einzelstreichung liegt mit 233 Löchern schon um einen Faktor von rund **10,6** unter dem Zufallsmittel. [U] Aus keinem dieser Erwartungswerte folgt Erreichbarkeit. Für Rang 1 spricht zusätzlich zum Rechenmaß, dass genau **ein** Paar gegenüber einer gültigen Kontrolle eingespart werden muss und jeder Versuch im kleinen Quotienten vollständig prüfbar ist.

**[U] A — Chance unkalibriert:** Rekord gering; auch ein zertifizierter Ausschluss für feste Tripel wäre verwertbar. **Aufwand:** höchstens 10 CPU-h. **Erster Test:** zwei verdeckte Zweierlöschungen des 32ers, Soll 2/2 bei \(t=7\).

**[U] B — Chance unkalibriert:** Verbesserung von 233 Lochklassen eher als gültiger 30er; Rekord gering. **Aufwand:** separater Pilot höchstens 2 CPU-h, nicht aus den zehn Stunden für A. **Erster Test:** verdeckte Kontrollspalte auf 0 ergänzen, danach beim 32er minus einem Paar die Schwelle 232 testen.
