> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-sol
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 24.11.2026, 22:38
> - Batch: Auftrag openrouter:batch-1795555378-XnB4KlJp2kB6sje9H72g, abgegeben 24.11.2026, 22:22, fertig 24.11.2026, 22:29
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 1.614 · Ausgabe 39.910 (davon Denken 32.369) · gesamt 41.524
> - Kosten laut OpenRouter: 0,20 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: xhigh
> - finish_reason: stop

---

**[R] Elf Doppelkugeln überdecken den 11-Würfel nicht, auch bei beliebig verteilten Nullen.** Der entscheidende Schritt ist eine gemeinsame Umkodierung: Jede Doppelkugel wird zu zwei gewöhnlichen Radius-2-Kugeln in einem 10-Würfel. Ich beweise die benötigte Untergrenze \(K(10,2)\ge 23\) unten, ohne Tabellenwert.

**[R] Für ternäre Zeilen bleiben nach der Reduktion auf Trägergrößen \(10,8,6\) nur drei Fälle offen:** zwei oder drei Zeilen mit Trägergröße 8, oder je eine Zeile mit Trägergröße 8 und 6; alle übrigen Zeilen haben jeweils Trägergröße 10. Für die allgemeine Familie schließe ich zusätzlich alle \(k\ge 8\) aus. Für \(k=4,5,6,7\) habe ich weder Ausschluss noch Konstruktion.

## 1. Der Ausschluss für \(n=11\)

**[R] Gemeinsame Umkodierung.** Schreibe den Würfel binär. Da 11 ungerade ist, hat jede Klasse modulo Komplement genau einen Vertreter geraden Gewichts. Sei \(H\) die Menge dieser \(1024\) Vertreter. Eine Zeile mit Null in Spalte \(j\) gibt auf den übrigen zehn Koordinaten zwei komplementäre Muster vor. Ergänze jedes in Spalte \(j\) so, dass das resultierende Zentrum in \(H\) liegt. Hat \(x\in H\) auf den zehn aktiven Koordinaten Abstand \(t\) zu einem Muster, dann ist sein voller Abstand zum ergänzten Zentrum \(t+(t\bmod 2)\). Dieser ist genau dann höchstens 2, wenn \(t\le 2\). Also ist die Treffermenge jeder Zeile die Vereinigung zweier Radius-2-Kugeln in \(H\), unabhängig von \(j\). Punktiert man anschließend **eine einzige gemeinsame Koordinate**, entspricht jede solche Kugel einer gewöhnlichen Radius-2-Kugel in \(\{0,1\}^{10}\): Der volle gerade Abstand ist der punktierte Abstand, aufgerundet auf die nächste gerade Zahl.

**[R] Zwei kleine Blockzählungen.** Um alle Kanten von \(K_7\) bzw. \(K_9\) mit vollständigen Graphen auf höchstens vier Knoten zu überdecken, braucht man mindestens fünf bzw. acht Blöcke. Zum Beweis darf man Blöcke auf Größe vier ergänzen und nötigenfalls wiederholen.

- **[R] \(K_7\):** Bei vier Blöcken muss jeder Knoten in mindestens zwei Blöcken liegen, da er sechs Nachbarn hat. Mindestens fünf Knoten liegen in genau zwei Blöcken, weil es insgesamt nur 16 Inzidenzen gibt. Ihre Zweiermengen von Blockindizes schneiden sich paarweise. Solche Zweiermengen haben entweder einen gemeinsamen Index oder liegen sämtlich in einem Dreieck: Aus \(\{a,b\},\{a,c\}\) erzwingt eine Menge ohne \(a\) die Menge \(\{b,c\}\), und jede weitere muss alle drei treffen. Im ersten Fall enthält ein Block mindestens fünf Knoten. Im zweiten liegen alle Inzidenzen der Knoten vom Grad zwei in drei Blöcken. Hat man \(h\ge 5\) solche Knoten, dann liegen dort mindestens \(2h+2(7-h)=14\) Inzidenzen, denn auch jeder andere Knoten liegt in mindestens drei der vier Blöcke. Drei Blöcke haben aber nur zwölf Plätze.
- **[R] \(K_9\):** Bei sieben Blöcken liegt jeder Knoten in mindestens drei Blöcken. Aus den 28 Inzidenzen folgen acht Knotengrade 3 und ein Knotengrad 4. Für eine Kante \(e\) sei \(\lambda_e\) ihre Blockmultiplizität. An jedem Knoten vom Grad 3 ist die Summe der Kantenexzesse \(3\cdot3-8=1\). Jede Kante hat einen solchen Endpunkt, also ist \(\lambda_e\le2\). Damit ist \(\sum_e\binom{\lambda_e}{2}=7\cdot 6-36=6\). Zählt man dasselbe nach Blockpaaren \(B,B'\), erhält man dagegen
  \[
  \sum_{B<B'}\binom{|B\cap B'|}{2}
  \ \ge\ \sum_{B<B'}(|B\cap B'|-1)
  \ =\ 8\binom32+\binom42-\binom72=9.
  \]

**[R] Lokaler Exzessbeweis.** Verbinde in \(H\) zwei Punkte bei Hamming-Abstand 2. Der Graph hat Grad 55; eine abgeschlossene Nachbarschaft ist eine Radius-2-Kugel der Größe 56. Angenommen, \(M\le22\) solche Kugeln überdecken \(H\). Sei \(m(x)\) die Überdeckungsmultiplizität, \(f(x)=m(x)-1\ge0\) und \(Af(x)\) die Summe von \(f\) über die 55 Nachbarn von \(x\). An einem Nichtzentrum seien \(u,v\) die Anzahlen der Zentren im Abstand 2 bzw. 4. Dann
\[
Af(x)=19u+6v-55.
\]
In der Tat entsprechen die Nachbarn von \(x\) den Kanten des \(K_{11}\) auf den Koordinaten. Ein Zentrum im Abstand 2 deckt die 19 Kanten, die sein Koordinatenpaar treffen; ein Zentrum im Abstand 4 deckt die sechs Kanten seines Koordinatenvierers. Weiter entfernte Zentren decken keinen Nachbarn.

**[R]** An jedem Nichtzentrum gilt \(u\ge1\) und \(Af(x)\ge12\). Bei \(u=1\) bleiben die Kanten eines \(K_9\), also ist \(v\ge8\) und \(Af(x)\ge12\). Bei \(u=2\) bleibt mindestens ein \(K_7\), also \(v\ge5\) und \(Af(x)\ge13\). Bei \(u=3\) bleibt mindestens ein \(K_5\), dessen zehn Kanten mindestens zwei Viererblöcke brauchen; das ergibt \(Af(x)\ge14\). Für \(u\ge4\) genügt \(19u-55\ge21\). Somit müsste
\[
55(56M-1024)=\sum_x Af(x)\ \ge\ 12(1024-M)
\]
gelten. Schon für \(M=22\) ist links \(11440\) und rechts \(12024\); für kleineres \(M\) wird die Ungleichung noch schlechter. Das beweist sowohl \(K(10,2)\ge23\) als auch den gesuchten Ausschluss. **Keine Annahme über die Nullverteilung geht ein.**

## 2. Ternäre und reelle Zeilen

**[R] Reduktion und Ergebnis.** Eine Zeile ungerader Trägergröße muss für einen Treffer Betrag mindestens 7 erreichen. Man darf deshalb einen Eintrag ergänzen, oder bei vollem Träger einen Eintrag streichen, ohne einen Treffer zur Schwelle 6 zu verlieren. Leere Treffermengen darf man durch beliebige nichtleere ersetzen. Es genügt daher, Trägergrößen \(10,8,6\) zu behandeln. Bezeichne die Zeilenzahlen mit \(p,q,s\). Die Treffermengen in \(H\) haben Größen \(112,72,32\); das Exzessbudget ist
\[
E=208-40q-80s.
\]
Ich schließe unten \(q+2s\ge4\) sowie \((q,s)=(1,0),(0,1)\) aus. Zusammen mit Punkt 1 bleiben genau die drei oben genannten **nicht entschiedenen** Fälle.

### [R] Ausschluss bei \(q+2s\ge4\)

**[R] Paritätsschnitt.** Für Spalte \(j\) setze \(\chi_j(x)=\prod_{\ell\ne j}x_\ell\) in Vorzeichennotation. Die Summe von \(\chi_j\) über die Treffermenge einer Zeile ist null, wenn die Zeile eine Null außerhalb von \(j\) hat: Das Flippen dieser Nullkoordinate ist eine vorzeichenwechselnde Bijektion auf den Klassen. Eine Zeile mit einziger Null in \(j\) liefert dagegen \(72\) mal das Produkt ihrer zehn aktiven Zeichen, denn es gibt \(2(1+\binom{10}{2})=92\) Treffer in der passenden Parität und \(2\cdot10=20\) in der anderen. Ist \(H_j\) die Summe dieser Zeilenprodukte, folgt aus \(f\ge0\)
\[
72|H_j|\le E.
\]
Für \(q+2s\ge4\) ist \(E\le48\); also sind die Träger-10-Zeilen in jeder Nullspalte nach ihren Produkten genau ausgeglichen. Insbesondere ist \(p\) gerade. Zusammen mit \(E\ge0\) bleiben nur \((q,s)=(5,0),(2,1),(1,2)\).

**[R] Eine Schranke für Zeilen in derselben Nullspalte.** Auf den zehn aktiven Koordinaten, modulo Komplement, betrachte \(h\le5\) Zentren derselben Parität. Sei \(U_h\) die maximale Größe der Vereinigung ihrer Kugelteile in dieser Parität. Dann
\[
U_h\le 46h-6\binom h2+\binom h3,
\qquad
(U_0,\ldots,U_5)\ \le\ (0,46,86,121,152,180)
\]
komponentenweise. Für den Beweis darf man doppelte Zentren durch unbenutzte derselben Parität ersetzen. Der Paarabstand modulo Komplement ist dann 2 oder 4, mit Schnittgröße 18 bzw. 6. Bei Abstand 2 sind es die beiden Zentren und \(2\cdot8\) Punkte mit einem Fehler auf den zwei Differenzkoordinaten und einem außerhalb; bei Abstand 4 sind es die \(\binom42\) Aufteilungen der Differenzen.

**[R]** Ein Dreifachschnitt hat je nach Anzahl \(b=0,1,2,3\) der Abstand-2-Paare höchstens \(1,3,5,11\) Punkte. Zur Kontrolle: Relativ zu einem gemeinsamen Treffer lassen sich die drei Zentren eindeutig bis auf gemeinsame Komplementierung so orientieren, dass ihre Abstände 2 oder 4 sind. Auf den gemischten Koordinaten weicht jeweils genau ein Zentrum von der Mehrheit ab. Die drei Anzahlen solcher Abweichungen sind, bis auf Reihenfolge, \((2,2,2),(1,1,3),(0,2,2),(1,1,1)\). Bei Wahl der Mehrheitszeichen ist dies der Fehlervektor; ein Flip auf einer gemischten Koordinate senkt einen Eintrag um 1 und erhöht die anderen um 1, ein Flip auf einer einstimmigen erhöht alle um 1. Zulässig sind nur Einträge 0 oder 2. Im ersten Fall ist kein Flip möglich; im zweiten muss einer der drei Fehler des dritten Zentrums geflippt werden; im dritten sind kein Flip oder je ein Flip aus den beiden Zweiergruppen möglich. Im vierten sind es ein einzelner gemischter Flip, alle drei gemischten Flips oder ein einzelner einstimmiger Flip, insgesamt \(3+1+7\). Damit ist der Dreifachschnitt höchstens \(1+\frac{10}{3}b\). In der Bonferroni-Obergrenze bis Ordnung drei kostet jedes Abstand-2-Paar zusätzlich 12 und bringt über Dreifachschnitte höchstens \((h-2)10/3\le10\) zurück. Das ergibt die behauptete Schranke.

**[R] Waldschnitt.** Für einen Wald auf Zeilenindizes ist die Summe seiner Kantengewichte \(|T_i\cap T_j|\) höchstens \(E\). Genauer: An einem überdeckten Punkt induzieren die treffenden Zeilen einen Wald; sein Beitrag zur Differenz zwischen Exzess und Kantengewichtssumme ist die Zahl seiner Komponenten minus eins. Bei Gleichheit müssen die treffenden Zeilen also an jedem Punkt zusammenhängend sein. Benutzt man mehrere Nullspalten, bilden die Paare mit verschiedenen Nullen einen zusammenhängenden Graphen auf den Träger-10-Zeilen, mit Kantengewicht mindestens 6 laut Schnitttabelle.

**[R]** In den beiden Fällen \(q+2s=5\) ist \(E=8\). Der Waldschnitt erzwingt eine einzige Nullspalte für die \(p=6\) bzw. \(p=8\) großen Zeilen. Mit der ausgeglichenen Parität ist ihre Vereinigung höchstens \(4(U_h+10h)\), \(h=p/2\): In jeder Parität tragen die Zentren der anderen Parität nur ihre zehn Nachbarn bei; die freie Koordinate verdoppelt. Für \(p=6\) ergibt sich einschließlich der fünf kleinen Zeilen höchstens \(604+360=964\), für \(p=8\) höchstens \(768+72+64=904\).

**[R]** Für \((q,s)=(2,1)\) ist \(E=48\). Eine einzige Nullspalte der acht großen Zeilen ergibt höchstens \(768+144+32=944\) Treffer. Bei mehreren Nullspalten kann keine Gruppe vier oder mehr Zeilen enthalten: Wegen der ausgeglichenen Parität gäbe es zwei gleichparitätige Paare in der Gruppe, beide mit Gewicht mindestens 12. Diese zwei Kanten lassen sich mit fünf Kanten zwischen Nullspalten zu einem Baum ergänzen, mit Gewicht mindestens \(54\). Also gibt es vier Zweiergruppen.

**[R]** Jede Träger-8-Zeile hat mindestens eine dieser vier Nullspalten im eigenen Träger und schneidet eine dazugehörige große Zeile in mindestens drei Klassen. Auf den sieben gemeinsamen aktiven Koordinaten kann man die Vorzeichen auf Abstand \(e\le3\) orientieren. Bei \(e=3\) gibt es drei Aufteilungen der Fehler als \(2+1\), bei \(e=2\) mindestens drei als \(2+0\) oder \(1+1\). Bei \(e\le1\) kann man alle Fehler der großen Zeile zuweisen und zusätzlich jeweils eine ihrer drei exklusiven aktiven Koordinaten flippen. Die große Zeile ist in allen gezählten Treffern gleich orientiert, also sind es verschiedene Klassen. Man kann somit an jeden Baum auf den acht großen Zeilen beide Träger-8-Zeilen mit je mindestens Gewicht 3 anhängen. Ein Baum aus Kanten zwischen Nullspalten liefert bereits \(7\cdot6+2\cdot3=48\). Jede Kante innerhalb einer Zweiergruppe muss daher Gewicht null haben: Sonst ersetzt man eine der sieben Baumkanten durch diese Kante mit Gewicht mindestens 12.

**[R]** Wähle nun einen Treffer zweier großer Zeilen \(i,j\) aus verschiedenen Gruppen. Für jede der beiden Zeilen \(k\) einer dritten Gruppe kann man den Pfad \(i-k-j\) zum Baum zwischen Nullspalten erweitern und die beiden kleinen Zeilen wie eben anhängen. Der Wald erreicht jedes Mal das Budget 48. Die Gleichheitsbedingung zwingt den Treffer deshalb in **beide** Zeilen der dritten Gruppe. Deren Schnitt ist aber leer. Das schließt den letzten Fall mit \(q+2s\ge4\) aus.

### [R] Ausschluss genau einer kleinen Zeile

**[R] Lokale Rechnung.** Verwende wieder den Graphen auf \(H\). Die zehn großen Zeilen liefern höchstens 20 verschiedene Kugelzentren. Für obere Schranken der Vereinigung bzw. für den Exzess dürfen mehrfach vorkommende Kugeln hier gestrichen werden; sei \(M\le20\) die Zahl der verbleibenden Zentren. Sei \(w\) die Zahl der Nachbarn von \(x\), die die kleine Zeile trifft. Für ein Nichtzentrum gilt jetzt
\[
Af(x)=19u+6v+w-55.
\]
Sei \(a\) der kleinere Abstand von \(x\) zu den beiden Mustern auf dem kleinen Träger. Die folgenden Zahlen und Verlustschranken gelten an Nichtzentren; „Verlust“ bedeutet \(\max(0,12-Af(x))\).
\[
\begin{array}{c|r|r|r|r}
\text{Träger}&a&\#x&w&\text{Verlust höchstens}\\ \hline
8&0&8&27&4\\
8&1&64&13&6\\
8&2&224&7&5\\
8&3&448&3&3\\ \hline
6&0&32&10&12\\
6&1&192&5&1\\
6&2&480&1&5
\end{array}
\]
Außerhalb der Tabelle ist \(w=0\), die kleine Zeile trifft \(x\) nicht, und Punkt 1 gibt Verlust null.

**[R] Nachweis der Tabelle.** Auf Träger 8 darf nach einem Zweierflip höchstens ein aktiver Fehler bleiben. Für \(a=0,1,2,3\) deckt die kleine Zeile daher im Kantengraphen von \(x\) jeweils: alle Kanten an den drei Nullen; einen Stern am Fehler plus das Dreieck der Nullen; die Kante zwischen den zwei Fehlern plus deren sechs Kanten zu den Nullen; bzw. das Dreieck der drei Fehler. Auf Träger 6 muss nach dem Flip kein aktiver Fehler bleiben; man erhält \(K_5\) auf den Nullen, einen Stern mit fünf Kanten bzw. eine einzelne Kante. Das gibt \(w\); die Punktzahlen sind \(2^{11-\text{Träger}}\binom{\text{Träger}}a\). Zur Untergrenze für \(v\) bleiben nach den \(u\) Paarzentren mindestens die Kanten eines \(K_{11-2u}\), abzüglich der eben beschriebenen kleinen Kantensammlung. Eine dort noch nötige Knotengradzahl \(d\) erfordert mindestens \(\lceil d/3\rceil\) Viererblock-Inzidenzen.

- **[R] Träger 8, \(a=0,1\):** Bei \(u=0,1,2\) erzwingt \(a=0\) mindestens \(v=6,3,0\), denn nach Entfernung der drei Sternzentren bleibt mindestens \(K_8,K_6,K_4\) (für \(u=2\) genügt die Schranke null). Bei \(a=1\) erhält man nach Entfernung des Sternzentrums mindestens \(K_{10},K_8,K_6\) abzüglich eines Dreiecks; die Gradschranke ergibt \(v\ge8,6,3\).
- **[R] Träger 8, \(a=2,3\):** Hier ist \(u\ge1\). Bei \(a=2,u=1\) haben im verbleibenden \(K_9\) mindestens vier Knoten noch Grad 8 und die übrigen mindestens Grad 4. Das verlangt mindestens \(4\cdot3+5\cdot2=22\) Inzidenzen, also \(v\ge6\). Bei \(u=2\) bleiben mindestens \(21-7\) Kanten, also \(v\ge3\). Bei \(a=3,u=2\) erzwingt der \(K_7\) abzüglich eines Dreiecks mindestens 14 Inzidenzen, also \(v\ge4\). Für \(a=3,u=1\) gilt \(v\ge7\): Sechs Blöcke für \(K_9\) abzüglich eines Dreiecks müssten je einen Dreiecksknoten enthalten und für jeden der drei Dreiecksknoten die sechs anderen Knoten in zwei Tripel teilen. (Die Gradschranke ist genau \(3\cdot2+6\cdot3=24\), daher muss jeder Blockplatz wie angegeben genutzt werden.) Die sechs Knoten hätten damit drei binäre Partitionsmerkmale, in jeder Position \(3{:}3\) ausgeglichen, ohne komplementäres Wortpaar, da jedes Knotenpaar gemeinsam in einem Block liegen muss. Das ist unmöglich: Schalte ein vorkommendes Wort zu \(000\). Dann fehlt \(111\); ohne Gewicht-1-Wort wäre die Gesamtzahl der Einsen gerade statt neun. Kommt etwa \(100\) vor, fehlt \(011\), sodass die letzten beiden Positionen nie gleichzeitig 1 sind. Deren Ausgleich verlangt aber zusammen sechs Einsen, im Widerspruch zum vorkommenden \(000\).
- **[R] Träger 6, \(a=1,2\):** Hier ist \(u\ge1\). Bei \(a=1,u=1\) verlangen die acht Knoten außerhalb des Sternzentrums mindestens 24 Inzidenzen, und das Sternzentrum braucht mindestens eine weitere, also \(v\ge7\). Bei \(u=2\) brauchen die sechs übrigen Knoten mindestens zwölf Inzidenzen und das Sternzentrum mindestens eine, also \(v\ge4\). Liegt das Sternzentrum nicht im verbleibenden vollständigen Graphen, gelten die Schranken erst recht. Bei \(a=2\) braucht \(K_9\) abzüglich einer Kante mindestens \(9\cdot3\) Inzidenzen, also \(v\ge7\); im \(K_7\) bleiben mindestens 20 Kanten, also \(v\ge4\).
- **[R]** Für \(u=3\) bleiben mindestens zehn Kanten vor Abzug der kleinen Kantensammlung, also \(v\ge\max(0,\lceil(10-w)/6\rceil)\). Für \(u\ge4\) genügt \(19u-55\ge21\). Einsetzen gibt alle Verlustschranken der Tabelle.

**[R] Summation.** Für Träger 6 ist damit
\[
\sum_x Af(x)\ \ge\ 12(1024-M)-32\cdot12-192-480\cdot5
\ \ge\ 9072,
\]
aber \(\sum_x Af(x)=55(56M+32-1024)\le7040\). Für Träger 8 liefert die Tabelle
\[
\sum_x Af(x)\ \ge\ 12(1024-M)-8\cdot4-64\cdot6-224\cdot5-448\cdot3
\ \ge\ 9168.
\]
Bei \(M\le19\) widerspricht das bereits \(55(56M+72-1024)\). Bei \(M=20\) fehlen noch 72 Einheiten gegenüber dessen Wert \(9240\). Die Zentren, die in der Untergrenze noch gar nicht mitgezählt sind, liefern mindestens 108: Es gibt mindestens neun sich schneidende Paare großer Zeilen. Bei verteilten Nullen sind es mindestens die neun Paare zwischen Nullgruppen. Bei einer einzigen Nullspalte gibt es mindestens \(2\binom52=20\) gleichparitätige Paare, alle mit nichtleerem Schnitt. Jedes solche Zeilenpaar liefert ein eigenes Paar von Kugelzentren im Abstand höchstens 4, da hier alle 20 Zentren verschieden sind. An jedem dieser beiden Zentren trägt das andere mindestens 6 zu \(Af\) bei; die eigene Kugel deckt bereits alle 55 Nachbarn. Also ist die Summe mindestens \(9168+108=9276>9240\).

### [R] Was bei reellen Zeilen fehlt

**[R]** Es fehlt nicht nur eine Volumenschranke, sondern ein Ersatz für die gemeinsame Kugeldarstellung oder ein Argument für die verbleibenden Treffermengen. Insbesondere ist die **zeilenweise Dominanz durch ternäre Zeilen falsch**. Betrachte \(a=(\frac12,\frac12,\frac12,\frac12,1,\ldots,1)\). In der positiven Hälfte zur Schwelle 6 sind alle einzelnen Fehler, alle Paare aus einem Halbgewicht und einem Vollgewicht sowie alle Tripel unter den vier Halbgewichten zulässig: Das Fehlergewicht darf höchstens \((9-6)/2=1{,}5\) sein.

**[R]** Wäre diese Treffermenge in der einer ternären Zeile enthalten, könnte man deren ungeraden Träger zunächst wie oben begradigen. Die positive Fehlerfamilie von \(a\) ist abwärts abgeschlossen und entlang von Einzelflips zusammenhängend. Die dominierende Zeile kann dort nicht zwischen den beiden Trefferhälften wechseln, weil ein Flip ihren Wert um höchstens 2 ändert. Orientiere sie daher positiv auf dieser Familie. Man darf dann jedes negative aktive Zeichen durch ein positives ersetzen: Bei einem Fehler auf dieser Koordinate benutzt man als Vergleich den ebenfalls zulässigen Punkt ohne diesen Fehler; ohne Fehler erhöht sich das Skalarprodukt. Aber eine so ausgerichtete Träger-10-Zeile scheitert an einem zulässigen Tripel außerhalb ihrer einzigen Null, eine Träger-8-Zeile an einem zulässigen Halb-/Vollgewichtspaar außerhalb ihrer drei Nullen, eine Träger-6-Zeile an einem einzelnen aktiven Fehler. Das Beispiel gilt unverändert für Schwellen \(5<d\le6\), da die Skalarprodukte dieser Zeile ganzzahlig sind.

## 3. Die Familie \(n=3k+2\)

**[R] \(k=2\):** Die sieben verschiedenen von null verschiedenen Spalten in \(\mathbb F_2^3\) bilden eine Prüfmatrix. Jedes nichtnullige Syndrom ist genau eine Spalte, also korrigiert man es durch genau einen Flip; die Radius-1-Kugeln um die 16 Kernwörter partitionieren den 7-Würfel. Die Spaltensumme ist null, der Kern also komplementär gepaart. Acht Zeilen mit gemeinsamer freier Koordinate genügen.

**[R] \(k=8,9\): lokaler Ausschluss trotz ausreichenden Volumens.** Setze \(S_k=V(3k+1,k-1)\) und \(I_k=V(3k+1,k-2)\). Im Quotienten modulo Vorzeichen hat eine Zeile \(2S_k\) Treffer, davon \(2I_k\) im Inneren, also bei aktivem Abstand höchstens \(k-2\). Betrachte die \(n\) Einzelflip-Nachbarn eines Punkts, der in keinem Inneren liegt. Jede Zeile trifft dort entweder null oder genau \(k\) Nachbarn: am aktiven Rand \(k-1\) sind es die \(k-1\) Fehlerflips plus der Nullflip, direkt außerhalb im Abstand \(k\) die \(k\) Fehlerflips. Die beiden komplementären Hälften sind zu weit auseinander, um beide zur Nachbarschaft beizutragen. Bei einer Überdeckung ist die Summe der Nachbarexzesse daher eine nichtnegative Zahl kongruent \(-n\bmod k\), also mindestens \(k-2\). Summieren und durch 2 teilen liefert die notwendige Ungleichung
\[
n(nS_k-2^{3k})\ \ge\ (k-2)(2^{3k}-nI_k).
\]
Für \(k=8\) sind die Seiten \(54\,707\,640<62\,364\,360\), für \(k=9\) sind sie \(137\,188\,531<597\,830\,842\).

**[R] \(k\ge10\): Volumenausschluss.** Die Dichte ist \((3k+2)S_k/2^{3k}\). Für \(k=10\) ist \(S_{10}=31\,621\,024\), also \(32 S_{10}=1\,011\,872\,768<2\cdot 536\,870\,912=2^{30}\). Mit \(m=3k+1\) gibt Ausmultiplizieren der drei neuen Koordinaten
\[
S_{k+1}=8S_k+\binom m k-4\binom m{k-1}-\binom m{k-2}
\ \le\ (7+1/k)S_k.
\]
Die Ungleichung folgt aus \(\binom m k=(2+2/k)\binom m{k-1}\) und \(S_k\le2\binom m{k-1}\), da die Rückwärtsquotienten im Binomialsummentail höchstens \(1/2\) sind. Damit sinkt die Dichte für \(k\ge10\): Ihr Quotient ist höchstens \(\frac{35}{32}\frac{71}{80}<1\).

**[R]** Das ergibt \(k=2\) möglich, \(k=3\) und alle \(k\ge8\) unmöglich. Für \(k=4,5,6,7\) liefert dies keinen Entscheid; ich habe auch keine Bauform mit verteilten Nullen für diese Fälle.

## 4. Schnitte für eine Baumsuche

**[R]** Für reine Doppelkugeln ist keine Suche mehr nötig. Die folgenden Schnitte sind dennoch unabhängig vom Endwiderspruch und lassen sich auch auf Teilmengen von Zeilen anwenden.

- **[R] Maximaler Wald statt isolierter Paargrenzen.** Für die bisher gewählten Zeilen sei \(r_t\) der Rang des Graphen aus Schnittkanten mit Gewicht mindestens \(t\), also Knotenzahl minus Komponentenzahl. Für reine Doppelkugeln ist der exakte maximale Waldwert
  \[
  6r_6+6r_{12}+14r_{26}+14r_{40}+36r_{76}.
  \]
  Absteigendes Einfügen ohne Kreis erreicht in jeder Schwellenstufe den jeweiligen Rang; Summation über die Stufen ergibt die Formel. Bei mehr als 208 abbrechen. In einer vollständigen verteilten Nullverteilung ist \(r_6=10\), somit \(r_{76}\le2,\ r_{40}\le4,\ r_{26}\le7\). Das erlaubt höchstens 3, 10 bzw. 28 Paare ab der jeweiligen Schwelle: Bei Rang \(r\) sind höchstens \(\binom{r+1}{2}\) Kanten möglich, durch Zusammenlegen der nichttrivialen Komponenten zur oberen Abschätzung. Falls \(r_{76}=2\), gilt sogar \(r_{26}=r_{40}=2\) und \(r_{12}\le3\).
- **[R] Nullspalten und Parität.** Eine Nullgruppe mit \(t\) Zeilen und \(h\le t/2\) Zeilen in der Minderheitsparität muss
  \[
  U_h-10h\ \ge\ 18t-52
  \]
  erfüllen. In der Minderheitshälfte liefern die \(t\) Zeilen höchstens \(2[U_h+10(t-h)]\) Treffer, die anderen \(11-t\) Zeilen genau 56 je Zeile vor Abzug ihrer Schnitte. Das ergibt den Schnitt. Für \(t=11\) ist er unmöglich. Für \(t=9\) erzwingt er \(h=4\), mit höchstens vier Punkten Überschuss in dieser Hälfte, noch vor Abzug der Schnitte mit den beiden fremden Zeilen. Jede fremde Zeile schneidet aber jede der vier Minderheitszeilen dort in mindestens sechs Punkten: Auf den neun gemeinsamen Koordinaten orientiere auf \(e\le4\); bei \(e=4\) gibt es die sechs \(2{:}2\)-Aufteilungen, bei \(e=3\) drei Aufteilungen mit zwei Fehlern der Minderheitszeile, jeweils mit zwei Wahlen für deren Nullbit. Für \(e=0,1,2\) gibt es bereits aus gemeinsamen Mustern mit null bzw. zwei Fehlern der Minderheitszeile mindestens sechs: bei \(e=0,1\) das Muster ohne Fehler dort plus die Muster mit einem gemeinsamen Fehler und einem Fehler auf der exklusiven aktiven Koordinate; bei \(e=2\) die zwei Muster mit einem Fehler auf den Differenzkoordinaten und einem exklusiven Fehler sowie das Muster mit beiden Fehlern auf den Differenzkoordinaten. Für jedes dieser letzten drei Muster sind beide Nullbitwerte zulässig. Also ist \(t=9\) ausgeschlossen.
- **[R] Auch \(t=10\) ist ausgeschlossen.** Der Paritätsschnitt erzwingt \(h=5\). Die zehn Zeilen treffen zusammen höchstens \(4(U_5+50)\le920\) Klassen. Die fremde Zeile darf sie also in höchstens acht Klassen schneiden. Laut Paartabelle muss sie zu jeder der zehn Zeilen Typ \(e=4\) haben. Orientiert man auf diesen Abstand, besteht jeder Schnitt aus den sechs Zweiermengen der vier Differenzkoordinaten; die beiden exklusiven Vorzeichen sind fest. Für zwei verschiedene Zeilen der Zehnergruppe sind diese Sechsermengen entweder wegen des exklusiven Vorzeichens disjunkt, oder sie haben höchstens \(\binom32=3\) gemeinsame Klassen. Ihr gemeinsamer Beitrag ist damit mindestens neun. Doppelte Zeilen kann man für einen Existenztest vorab durch unbenutzte Zeilen derselben Nullgruppe und Parität ersetzen. Der Paritätsschnitt schließt außerdem bei \(t=8\) eine Aufteilung \(5{:}3\) selbst ohne Schnittabzug aus.

[R] 1 — **bewiesen**: Elf Doppelkugeln reichen nicht.  
[R] 2 — **Teilresultat**: Drei ternäre Zeilenprofile bleiben offen; die zeilenweise Reduktion reeller Zeilen ist widerlegt.  
[R] 3 — **Teilresultat**: Ausschluss für \(k=3\) und \(k\ge8\); kein Entscheid für \(4\le k\le7\).  
[R] 4 — **bewiesen**: Wald- und Paritätsschnitte, einschließlich des Ausschlusses jeder Nullgruppe der Größe mindestens neun.
