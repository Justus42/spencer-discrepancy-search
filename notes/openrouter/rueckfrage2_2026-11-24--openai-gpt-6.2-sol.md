> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-sol
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 24.11.2026, 23:01
> - Batch: Auftrag openrouter:batch-1795556852-ov8lEKq1XIgR9lKn4gYo, abgegeben 24.11.2026, 22:47, fertig 24.11.2026, 22:56
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 1.539 · Ausgabe 27.438 (davon Denken 22.566) · gesamt 28.977
> - Kosten laut OpenRouter: 0,14 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: xhigh
> - finish_reason: stop

---

- **Frage 1 — [U] Teilresultat:** Ich habe weder einen Ausschluss noch eine Konstruktion für \(n=14\). [R] Notwendige Schnitte sind unter anderem: Der gemischte Paartyp \(e=0\) ist verboten; pro Parität gibt es höchstens **10** Paare vom gefalteten Abstand \(2\); und für jede Zeile, jede Parität und jede gestrichene Koordinate gelten drei Schalenungleichungen.
- **Frage 2 — [R] Teilresultat:** Das unten bewiesene Exzessargument schließt **30** Kugeln aus, aber nicht 34. Es ergibt \(K(16,4)\ge31\), unter der Paarbedingung also **mindestens 32 Kugeln**. Das ist die stärkste Schranke, die ich hier beweise.
- **Frage 3 — [R] Bewiesen:** Die Umkodierung gilt für jede Zeilenzahl. Bei 19 Spalten und \(d=8\) ist der Radius \(5\).

## 1. Vierzehn Zeilen mit je einer Null

**[R] P1 — Zwei gekoppelte Überdeckungen.** Sei \(H_p=\{x:\prod_jx_j=(-1)^p\}\), \(p=0,1\). Ergänze die Null jeder Zeile \(a_i\) zu einem Vorzeichen, sodass die Ergänzung \(c_i^p\) das Produkt \(-(-1)^p\) hat. Beide Zentren \(c_i^p,-c_i^p\) haben dann ungeraden Abstand zu jedem \(x\in H_p\). Der volle Abstand unterscheidet sich vom Abstand auf den aktiven Koordinaten um \(0\) oder \(1\); wegen seiner Ungeradheit liegt er genau dann bei höchstens \(3\), wenn der aktive Abstand bei höchstens \(3\) liegt. Dasselbe Argument erlaubt das Streichen **jeder** gemeinsamen Koordinate \(k\). Auf \(H_p/\{\pm1\}\) erhält man so den gefalteten 13-Würfel, mit je einer Kugel um das projizierte antipodale Zentrum. Es gibt \(4096\) Klassen; jede Zeile trifft \(378=1+13+78+286\) davon. Bei einer Überdeckung beträgt der Exzess auf **jeder** Hälfte \(14\cdot378-4096=1196\).

**[R] P2 — Ein Matrixschnitt pro Parität.** Sei \(C_p\) die \(14\times14\)-Matrix mit Zeilen \(c_i^p\), und sei \(f_p([x])\) die Trefferzahl. Die \(378=\binom{14}{1}+\binom{14}{3}\) Trefferklassen einer Zeile haben je einen Vertreter im Abstand \(1\) oder \(3\) von \(c_i^p\). Für zwei verschiedene Koordinaten enthalten genau \(2(1+\binom{12}{2})=134\) dieser Flipmengen genau eine der beiden Koordinaten. Der entsprechende Nebendiagonaleintrag der Momentensumme ist daher \((378-2\cdot134)c_{ij}^pc_{i\ell}^p=110c_{ij}^pc_{i\ell}^p\). Über alle \(4096\) Klassen ist die Momentensumme \(4096I\): Ein Flip in genau einer der beiden Koordinaten und einer dritten paart die Nebendiagonalbeiträge mit ihrem Negativen. Folglich muss gelten
\[
 W_p:=110C_p^{\mathsf T}C_p-344I
 =\sum_{[x]\in H_p/\{\pm1\}}(f_p([x])-1)xx^{\mathsf T}\succeq0.
\]
Hier ist \(344=4096-14(378-110)\). Insbesondere sind beide \(C_p\) invertierbar; auch \(C_p C_p^{\mathsf T}\succeq(172/55)I\), da die beiden Gram-Matrizen dieselben Eigenwerte haben. Als stärkerer Schnitt gilt für jeden Testvektor \(v\):
\[
 v^{\mathsf T}W_pv\ \ge\ 1196\min_{x\in H_p}(v^{\mathsf T}x)^2.
\]

**[R] P3 — Paartypen und Kandidatenanzahlen.** Setze \(h_p=\min(d_H(c_i^p,c_j^p),14-d_H(c_i^p,c_j^p))\). Die möglichen Werte sind \(0,2,4,6\); \(0\) ist nach P2 verboten. Die Schnittgrößen auf einer Hälfte sind
\[
 I(0),I(2),I(4),I(6)=378,158,68,20.
\]
Zum Nachrechnen: Für \(h=0,2,4,6\) zähle die Flipmengen der Größen \(s=1,3\) mit \(s+h-2|S\cap D|\le3\), wobei \(D\) eine \(h\)-Menge ist. Das Gegen­zentrum im Abstand \(14-h\ge8\) trägt nichts bei.

| Lage der Null | Typen | \((h_0,h_1)\), bis auf Vertauschung | Kandidaten je fester Nullspalte der zweiten Zeile |
|---|---|---|---|
| dieselbe | \(\delta=1,2,3,4,5,6\) | \((2,2),(2,2),(4,4),(4,4),(6,6),(6,6)\) | \(13,78,286,715,1287,1716\) |
| verschiedene | \(e=0,1,2,3,4,5,6\) | \((0,2),(2,2),(2,4),(4,4),(4,6),(6,6),(6,6)\) | **\(0\)** statt \(2\), dann \(24,132,440,990,1584,924\) |

**[R]** Bei gleicher Null ergänzt man den aktiven Abstand \(\delta\) zur nächsten geraden Zahl. Bei verschiedenen Nullen kommen auf den zwei Sonderkoordinaten bei ungeradem \(e\) genau ein Unterschied und bei geradem \(e\) entweder null oder zwei Unterschiede hinzu; beim Paritätswechsel tauschen null und zwei. Die Anzahlen sind \(\binom{13}{\delta}\) beziehungsweise \(2\binom{12}{e}\) für \(e<6\) und \(\binom{12}{6}\) für \(e=6\). Damit bleiben pro fester Zeile \(4095+13\cdot4094=57317\) Partnerkandidaten.

**[R] P4 — Schnitt mit der Nullverteilung.** Schreibe \(A\) für die ursprüngliche Matrix, \(n_j\) für die Zahl ihrer Nullen in Spalte \(j\), und \(C_0=A+Z,\ C_1=A-Z\), wobei \(Z\) nur die Ergänzungsvorzeichen an den Nullstellen enthält. Für jedes der \(91\) Spaltenpaare \(j<\ell\) setze
\[
 D=(A^{\mathsf T}A)_{j\ell},\qquad B=(A^{\mathsf T}Z+Z^{\mathsf T}A)_{j\ell}.
\]
Das sind Summen von \(14-n_j-n_\ell\) beziehungsweise \(n_j+n_\ell\) Vorzeichen. Die \(2\times2\)-Hauptminoren von \(W_0,W_1\) geben
\[
 |D|+|B|=\max(|D+B|,|D-B|)\le\lfloor1196/110\rfloor=10.
\]
Mit \(S_\delta,D_e\) als Anzahlen **gewählter** Zeilenpaare der beiden Tabellenzeilen gilt außerdem \(\sum_\delta S_\delta=\sum_j\binom{n_j}{2}\), \(\sum_e D_e=91-\sum_j\binom{n_j}{2}\) und \(D_0=0\). [U] Einen Ausschluss allein anhand des Vektors \((n_j)\), ohne Vorzeichen- oder Paarinformation, habe ich nicht nachgewiesen.

**[R] P5 — Exzessbudget je Zeile und für Dreifachschnitte.** Auf jeder Hälfte ist die Summe der Schnitte einer festen Zeile mit den anderen genau der Exzess auf ihrer Treffermenge. Hat sie \(N_2\) Nachbarn mit \(h=2\) und \(N_4\) mit \(h=4\), folgt
\[
 138N_2+48N_4\le1196-13\cdot20=936.
\]
Für die Summen \(J_2,J_3\) der Paar- bzw. Dreifachschnittgrößen auf einer Hälfte gilt zusätzlich \(J_3\ge J_2-1196\ge624\). Punktweise ist dafür nur \(\binom f3\ge\binom f2-(f-1)\) für \(f=1,\ldots,14\) zu prüfen.

**[R] P6 — Der Graph der \(h=2\)-Paare besteht aus Cliquen.** Orientiere zwei \(h=2\)-Nachbarn eines Zentrums so, dass beide tatsächlich Abstand \(2\) zu ihm haben. Wären ihre beiden Flipmengen disjunkt, hätte der zugehörige \(3\times3\)-Hauptminor von \(C_pC_p^{\mathsf T}-(172/55)I\) Diagonale \(598/55\) und Nebendiagonalen \(10,10,6\). Seine Determinante ist \(-13415008/166375<0\). Also schneiden sich die Flipmengen, und die beiden Nachbarn haben ebenfalls \(h=2\). Damit ist jede Zusammenhangskomponente eine Clique.

**[R] P7 — Cliquenbudget und Anzahlen gewählter Paartypen.** In einer solchen Clique sind, relativ zu einem Anker, die Flipmengen der anderen Zentren paarweise schneidende Zweiermengen. Sie haben entweder einen gemeinsamen Punkt oder bestehen aus den drei Kanten eines Dreiecks: Nach Wahl von \(\{a,b\},\{a,c\}\) ist \(\{b,c\}\) die einzige Möglichkeit ohne \(a\), und dann gibt es keine vierte Kante. Im Fall eines gemeinsamen Punkts kann man die \(s\) Zentren durch eine gemeinsame Verschiebung zu \(s\) verschiedenen Einheitsvektoren machen. Streicht man eine unbenutzte Koordinate, zählt ihre Kugelvereinigung im gefalteten Würfel
\[
 1093-\binom{13-s}{3}-\binom{13-s}{4}
\]
Klassen: alle Gewichte \(0,1,2\), und die Gewichte \(3,4\), deren Träger mindestens eine der \(s\) Positionen enthält. Im Dreiecksfall liefert das Streichen einer Dreieckskoordinate alle vier Muster auf den zwei übrigen Dreieckspositionen; die Vereinigung hat \(4\sum_{j=0}^3\binom{11}{j}=928\) Klassen. Somit beträgt der gruppeninterne Exzess für \(s=1,\ldots,6\) mindestens \(0,158,371,584,923,1245\). Bereits eine Sechserclique überschreitet also \(1196\).

**[R]** Sei \(k_s\) die Zahl der \(h=2\)-Komponenten der Größe \(s\), \(g=\sum_s k_s\) und \(g_4\) die Zahl der Komponenten des Graphen mit Kanten \(h\le4\). Zusätzlich zum internen Exzess zählt der Exzess zwischen den Gruppen mindestens die Schnittsumme eines Baums auf den Gruppen: An einem Punkt mit \(q\) getroffenen Gruppen können höchstens \(q-1\) Baumkanten beitragen. Man kann \(g-g_4\) Baumkanten mit Schnitt mindestens \(68\) wählen; die übrigen haben Schnitt mindestens \(20\). Nach Abzug von \(20(g-1)\) ergibt das
\[
 138k_2+331k_3+524k_4+843k_5+48(g-g_4)\le936,
 \qquad \sum_{s=1}^5 s k_s=14.
\]
Schon ohne den letzten Term ist das Maximum von \(\sum_s\binom{s}{2}k_s\) unter diesen ganzzahligen Bedingungen **10**; das ist durch Aufzählung von \(0\le k_s\le\lfloor14/s\rfloor\) prüfbar. Über beide Paritäten folgt damit der Suchschnitt
\[
 \boxed{\,2(S_1+S_2+D_1)+D_2\le20\,}.
\]

**[R] P8 — Schalen­schnitte für jede gestrichene Koordinate.** Für jede der \(2\cdot14\cdot14=392\) Wahlen aus Parität, gestrichener Koordinate und Referenzzeile bestimme die gefalteten Abstände \(\delta_j\in\{0,\ldots,6\}\) zu den anderen 13 projizierten Zentren. Für \(s=4,5,6\) ist \(\sum_{j=1}^{13}R_{s,\delta_j}\) mindestens die rechte Spalte:
\[
\begin{array}{c|rrrrrrr|r}
s\backslash\delta&0&1&2&3&4&5&6&\binom{13}{s}\\\hline
4&0&220&55&145&37&85&50&715\\
5&0&0&165&45&153&97&132&1287\\
6&0&0&0&120&120&176&176&1716
\end{array}
\]
Die Einträge zählen \(s\)-Mengen mit Abstand höchstens \(3\) zu einem Zentrum vom Gewicht \(t=\delta\) oder zum Gegen­zentrum vom Gewicht \(t=13-\delta\): Summiere \(\binom ti\binom{13-t}{s-i}\) über \(s+t-2i\le3\) für beide \(t\). Die Referenzzeile selbst deckt keinen Punkt dieser drei Schalen. Sind etwa alle 13 Abstände in \(\{5,6\}\), erzwingen die ersten beiden Schalen **mindestens 2 und höchstens 12** Abstände vom Typ \(5\).

## 2. Exzess im 16-Würfel

**[R] P9 — Zählung an eindeutig bedeckten Randpunkten.** Betrachte eine beliebige Überdeckung durch \(M\) verschiedene Zentren, Radius \(4\). Sei \(b(y)\) die Zahl der Zentren im Abstand \(4\), \(t(y)\) die Zahl im Abstand höchstens \(3\), und \(e(y)=b(y)+t(y)-1\). Setze \(U=\{y:b(y)=1,\ t(y)=0\}\) und \(E=\sum_y e(y)=2517M-65536\). Für \(x\in U\) liefert sein einziges deckendes Zentrum genau \(4\) Treffer unter seinen \(16\) Nachbarn. Jedes Zentrum im Abstand \(5\) liefert \(5\) weitere, alle anderen liefern null. Also braucht \(x\) mindestens drei Zentren im Abstand \(5\), und die Exzesssumme seiner Nachbarn ist mindestens \(4+3\cdot5-16=3\). Mit \(a(y)=|U\cap N(y)|\) und \(1820M=|U|+\sum_{e(y)>0}b(y)\) ergibt sich
\[
 3\cdot1820M\le\sum_{e(y)>0}\bigl(3b(y)+e(y)a(y)\bigr).
\]

**[R] P10 — Lokale Obergrenze mit genau einem Ausnahmetyp.** Die vier Fliprichtungen, die zum Inneren einer Randkugel führen, können nicht zu \(U\) führen. Verschiedene Randzentren haben verschiedene Vierermengen solcher Richtungen. Für \(e>0\) ergeben sich damit folgende Grenzen:
\[
\begin{array}{c|c|c}
\text{Fall}&a\text{ höchstens}&3b+ea\text{ höchstens}\\\hline
t\ge2&0&3e\\
t=1&12&15e\\
t=0,\ b\ge3&11&14e+3\le16e\\
t=0,\ b=2,\ \text{Vereinigung der Richtungen }\ge6&10&16\\
t=0,\ b=2,\ \text{Vereinigung der Richtungen }=5&11&17
\end{array}
\]
In der ersten Zeile bleiben beide inneren Zentren bei jedem Nachbarn deckend. In der letzten Zeile ist \(e=1\); genau drei gemeinsame Fliprichtungen führen zu Punkten mit \(t\ge2\).

**[R] P11 — Gegenrechnung und Untergrenze.** Sei \(A_*\) die Menge der Punkte im letzten Tabellenfall. Jeder davon hat die drei genannten Nachbarn; jeder Punkt mit \(t\ge2\) hat höchstens \(16\) Nachbarn und \(e\ge1\). Also \(3|A_*|\le16\sum_{t\ge2}e\). Der Spielraum von mindestens \(13e\) in der ersten Tabellenzeile bezahlt damit den Zuschlag von \(1\) je Ausnahmepunkt. Insgesamt:
\[
 \boxed{5460M\le16(2517M-65536)}
 \quad\Longleftrightarrow\quad 34812M\ge1048576.
\]
Für \(M=30\) ist \(34812M=1044360<1048576\). Wiederholte Zentren kann man vor der Anwendung entfernen; daher braucht auch eine Überdeckung mit Wiederholungen mindestens 31 verschiedene Zentren. Unter der Paarbedingung ist die Zahl der Kugeln gerade, also mindestens 32. Für **34** lautet die bewiesene Ungleichung dagegen \(185640\le320672\); sie schließt 34 **nicht** aus.

## 3. Beliebige Zeilenzahl bei ungeradem \(n\)

**[R] P12.** Ja: Für jede Zeile separat wählt man bei \(r=\lfloor(n-1-d)/2\rfloor\) die beiden Ergänzungen der entgegengesetzten aktiven Muster mit Gewichtsparität \(r\bmod2\); auf den geradgewichtigen Vertretern sind die vollen Abstände dann von Parität \(r\), sodass weder das Ignorieren der Nullkoordinate noch das Streichen einer gemeinsamen Koordinate die Schwellenentscheidung bei \(r\) ändert. Nach dem Streichen haben die beiden Zentren Abstand \(n-1\) oder \(n-2\); umgekehrt ergibt das Ergänzen beider Zentren zur Parität \(r\bmod2\) jeweils genau eine gemeinsame Koordinate, die man zur Null macht. Der Beweis zählt keine Zeilen: Bei \(n=19,d=8\) entstehen aus \(m\) Zeilen \(2m\) Kugeln in Dimension \(18\) mit \(r=5\).

**Status**  
Frage 1: **Teilresultat**  
Frage 2: **Teilresultat**  
Frage 3: **bewiesen**

**Prüfpunkte**  
P1 [R] \(4096\) Klassen pro Parität, \(378\) Treffer pro Zeile, Exzess \(1196\); Projektion für alle 14 Löschkoordinaten.  
P2 [R] \(134\) negative Beiträge pro Nebendiagonale und Zeile; \(W_p=110C_p^{\mathsf T}C_p-344I\succeq0\).  
P3 [R] Vier Schnittgrößen \(378,158,68,20\); Paartypentabelle, \(e=0\) verboten, \(57317\) Partnerkandidaten.  
P4 [R] Für alle \(91\) Spaltenpaare gilt \(|D|+|B|\le10\); die beiden Paartypensummen ergeben zusammen \(91\).  
P5 [R] Je Referenzzeile \(138N_2+48N_4\le936\); je Parität \(J_3\ge J_2-1196\ge624\).  
P6 [R] Der verbotene \(3\times3\)-Minor ist \(-13415008/166375\); jede \(h=2\)-Komponente ist eine Clique.  
P7 [R] Sechs Cliquenkosten, Baumschnitt und ganzzahliges Maximum \(10\) für die Zahl der \(h=2\)-Paare je Parität.  
P8 [R] 21 Schalenkoeffizienten; je drei Untergrenzen \(715,1287,1716\) für alle \(392\) Referenzwahlen.  
P9 [R] Jeder Punkt in \(U\) hat Nachbarexzess mindestens \(3\); die Gesamtzahl der Randinzidenzen ist \(1820M\).  
P10 [R] Fünf lokale Fälle; nur der letzte überschreitet \(16e\), um genau \(1\), und hat drei Nachbarn mit \(t\ge2\).  
P11 [R] \(3|A_*|\le16\sum_{t\ge2}e\); \(1044360<1048576\) schließt \(M=30\) aus, \(185640\le320672\) lässt \(M=34\) zu.  
P12 [R] Pro Zeile zwei Zentren im Abstand \(n-1\) oder \(n-2\), mit zeilenweiser Umkehrung; \(r=5\) für \(n=19,d=8\).
