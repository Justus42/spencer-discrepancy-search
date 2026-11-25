> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-sol
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 25.11.2026, 17:18
> - Batch: Auftrag openrouter:batch-1795622153-3Q61w8zeCtesAaSgbgjp, abgegeben 25.11.2026, 16:55, fertig 25.11.2026, 17:05
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 29.133 · Ausgabe 33.697 (davon Denken 25.227) · gesamt 62.830
> - Kosten laut OpenRouter: 0,20 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: xhigh
> - finish_reason: stop

---

[R] **Vorab zu Frage 1:** Die \(3960\) sind **gewichtete** Zusatzinzidenzen: Dieselbe Zusatzinzidenz kann für mehrere kleine Kugeln gezählt werden. Als Schranke für diese gewichtete Summe sind \(697\) gültig, aber nicht scharf; es genügen \(320\). Die Spielräume \(88,-872,-1760\) stimmen.

## Frage 1 – volle Hadamard-Hälfte

[R] **Ergebnis:** Für \(m_2=9,11,13\) gibt es kein \(W\) mit \(12\) Zeilen, das mit der vollen Hadamard-Hälfte Diskrepanz \(9\) erreicht.

1. [R] **Die beiden kritischen Profile und ihre Zeilenmengen.** Alle Zeilen einer Hadamard-Matrix der Ordnung \(12\) haben Abstände \(6\) voneinander. Daher sind für ein festes \(y\) alle \(|u_i|\) entweder \(0\bmod 4\) oder \(2\bmod 4\). Parseval gibt \(\sum_i u_i^2=144\). Falls kein \(|u_i|\ge8\) vorkommt, bleiben genau \((4^9,0^3)\) und \((6^3,2^9)\): \(9\cdot16=144\) bzw. \(3\cdot36+9\cdot4=144\). Ein \(|u_i|\ge8\) reicht bereits, da \(m_2\) ungerade ist.

   [R] Für jede Dreiermenge \(F=\{a,b,c\}\) von Zeilen sei \(T=h_a h_b h_c\), koordinatenweise multipliziert. Auf \(F\) hat \(HT\) den Wert \(0\). Für \(i\notin F\) ist
   \[
   \#\{j:h_{ij}=h_{aj}=h_{bj}=h_{cj}\}
      =\frac{12+\langle h_i,T\rangle}{8}.
   \]
   Das folgt durch Ausmultiplizieren der \(3\) Gleichheitsindikatoren; die \(3\) linearen und die \(3\) quadratischen Terme verschwinden durch Orthogonalität. Somit ist \(\langle h_i,T\rangle\equiv4\bmod8\). Die \(9\) verbleibenden Quadrate summieren sich zu \(144=9\cdot16\), also sind alle Beträge \(4\). Damit realisiert \(T\) jede der \(\binom{12}{3}=220\) Nullzeilenmengen. Der Mehrheitsvektor \((h_a+h_b+h_c-T)/2\) realisiert dieselbe Dreiermenge als 6er-Zeilen: dort ist der Betrag \(12/2=6\), sonst \(4/2=2\).

2. [R] **Die Bedingung an \(z\) ist exakt.** Ohne eine Zeile mit \(|v_i|\ge9\) verlangt \((4^9,0^3)\) mindestens \(4\) Zeilen mit \(|v_i|\ge5\): Sonst passen alle solchen Zeilen in eine Null-Dreiermenge. Das Profil \((6^3,2^9)\) ist für alle \(220\) Dreiermengen gedeckt, wenn eine Zeile \(|v_i|\ge7\) hat. Gibt es keine solche Zeile, ist es genau dann gedeckt, wenn keine Dreiermenge ganz aus \(|v_i|=1\) besteht. Das bestätigt die angegebene Alternative.

3. [R] **Volumenrechnung.** Sei \(B(x)\) die Anzahl der großen Kugeln um die \(24\) signierten Zeilen, \(A(x)\) die Anzahl der kleinen. Ihre Radien sind \(r_5=(m_2-5)/2\) und \(r_9=(m_2-9)/2\). Für \(D=24V(m_2,r_9)-|\{x:A(x)>0\}|\ge0\) lautet die notwendige Inzidenzzahl
   \[
   \sum_x B(x)\ \ge\ 4\cdot 2^{m_2}-3\cdot24V(m_2,r_9)+3D.
   \]

   | [R] \(m_2\) | \(V(m_2,r_5)\) | \(V(m_2,r_9)\) | vorhandene Inzidenzen | nötig schon bei \(D=0\) | Spielraum |
   |---:|---:|---:|---:|---:|---:|
   | 9 | 46 | 1 | 1 104 | 1 976 | \(-872\) |
   | 11 | 232 | 12 | 5 568 | 7 328 | \(-1\,760\) |
   | 13 | 1 093 | 92 | 26 232 | 26 144 | \(88\) |

4. [R] **Schnitte für \(m_2=13\).** Zu jedem der \(24\) signierten Wörter und jeder der \(11\) fremden Zeilen kann man das fremde Vorzeichen mit Abstand \(\delta\le6\) wählen. Für \(\delta=0,1,2,3,4,5,6\) hat der Schnitt der Radius-2-Kugel mit der fremden Radius-4-Kugel die Größen
   \[
   92,\ 92,\ 92,\ 47,\ 47,\ 15,\ 15.
   \]
   Nachrechenbar sind sie durch \(\sum_{j+k\le2,\ \delta-j+k\le4}\binom{\delta}{j}\binom{13-\delta}{k}\), wobei \(j\) in den verschiedenen und \(k\) in den gleichen Koordinaten flippt. Folglich ist \(\sum_x A(x)(B(x)-1)\ge24\cdot11\cdot15=3960\), mit Summanden \(0\) für \(A(x)=0\).

5. [R] **Obere Schranke für genau dieselbe Summe.** Innerhalb der kleinen Kugelvereinigung ist \(\sum(B-1)\le88-3D\), also \(D\le29\). Weil pro Zeile höchstens eines ihrer beiden Vorzeichen im Radius \(4\) liegt, ist \(B\le12\). Die Mehrfachzählung durch \(A-1\) kostet somit höchstens \(11D\):
   \[
   \sum_x A(x)(B(x)-1)\le88-3D+11D\le320<3960.
   \]

## Frage 2 – punktierte Hadamard-Hälfte

[R] **Ergebnis:** Für \(m_2=10\) ist \(W\) unmöglich; für \(m_2=12,14\) liefert die folgende Zählung keinen Entscheid.

1. [R] **Die Dreiermengenbedingung.** Normiere die gestrichene Spalte von \(H\) auf lauter \(+1\). Für die \(220\) Dreiermengen \(F\) liefert der Mehrheitsvektor aus Frage 1 nach Punktierung genau \(5\) auf \(F\) und sonst Beträge \(1\) oder \(3\). Für \(i\notin F\) gilt
   \[
   i\in O(F)\quad\Longleftrightarrow\quad \langle h_i,\textstyle\prod_{r\in F}h_r\rangle=-4.
   \]
   Denn der volle Mehrheitsvektor hat dort Skalarprodukt \(+2\) im Fall \(-4\), sonst \(-2\), und die Punktierung zieht \(1\) ab. Schreibe \(A=\{i:|v_i|\ge4\}\), \(B=\{i:|v_i|\ge6\}\), \(C=\{i:|v_i|\ge8\}\), mit Größen \(a,b,c\). Wenn \(c=0\), ist jedes \(F\) mit \(F\cap A=\varnothing\) und \(B\subseteq O(F)\) ein Gegenbeispiel zu \((*)\).

2. [R] **Fall \(b=0\) oder \(b=1\), jeweils \(c=0\).** Für \(b=0\) braucht man \(a\ge10\). Für \(B=\{i\}\) betrachte für jede andere Zeile \(r\) den Block aus den verbleibenden Koordinaten, in denen \(h_r=h_i\). Diese \(11\) Blöcke haben Größe \(5\); je zwei schneiden sich in \(2\) Punkten, jeder Punkt liegt in \(5\) Blöcken, jedes Punktepaar in \(2\) Blöcken. Die Zahlen folgen aus den \(6\) Übereinstimmungen mit \(i\) und den \(3\) gemeinsamen Übereinstimmungen vor Streichen der normierten Spalte; für die Spaltenzählung gilt dieselbe Orthogonalität. Nach der Formel in Schritt 1 ist \(i\in O(F)\) genau dann, wenn die \(3\) Blöcke zu \(F\) keinen gemeinsamen Punkt haben.

   [R] Unter \(6\) solchen Blöcken gibt es immer \(3\) ohne gemeinsamen Punkt. Läge ein Punkt in mindestens \(4\) der \(6\) Blöcke, wähle \(4\) davon und einen Block außerhalb dieses Punktes, der existiert, weil die Punktmultiplizität höchstens \(5\) ist. Die \(\binom42=6\) zweiten Schnittpunkte der \(4\) Blöcke sind verschieden, da jedes Punktepaar in genau \(2\) Blöcken liegt. Der fünfte Block müsste alle \(6\) enthalten, hat aber Größe \(5\). Liegt dagegen jeder Punkt in höchstens \(3\) der \(6\) Blöcke, gibt es höchstens \(2\binom62/3=10\) Dreierinzidenzen, aber \(\binom63=20\) zu treffende Tripel. Daher kann das Komplement von \(A\) höchstens \(5\) Zeilen enthalten: \(a\ge7\).

3. [R] **Fall \(b=2,\ c=0\).** Sei \(B=\{i,j\}\). In den \(6\) Spalten mit \(h_i=h_j\) hat jede andere Zeile genau \(3\) Übereinstimmungen mit \(i\), darunter die gestrichene Spalte. Die übrigen \(2\) Übereinstimmungen sind eine Kante auf \(5\) Punkten. Jede Kante kommt genau einmal vor: Zwei dieser Spalten haben vor Zeilenausschluss \(3\) gemeinsame Übereinstimmungszeilen, davon sind \(i,j\) zwei. Die \(10\) anderen Zeilen entsprechen also den \(10\) Kanten von \(K_5\).

   [R] Bilden drei Kanten ein Dreieck \(F\), so ist das Produkt der drei Zeilen relativ zu \(i\) auf den \(5\) Punkten stets \(-1\) und in der gestrichenen Spalte \(+1\). Daher ist
   \[
   \left\langle h_i,\prod_{r\in F}h_r\right\rangle+
   \left\langle h_j,\prod_{r\in F}h_r\right\rangle=2(1-5)=-8.
   \]
   Beide Terme sind \(\pm4\), also liegt \(B\) in \(O(F)\). Somit muss \(A\setminus B\) alle \(\binom53=10\) Dreiecke treffen. Jede Kante trifft nur \(3\); nötig sind mindestens \(4\) Kanten, also \(a\ge6\).

4. [R] **Alle hohen Zeilen sind in der Bilanz enthalten.** Punktweise gilt
   \[
   a+3b+6c\ge10.
   \]
   Für \(c=0\) geben \(b=0,1,2\) die Untergrenzen \(10,10,12\); bei \(b\ge3\) gilt \(a+3b\ge4b\ge12\). Bei \(c\ge1\) gibt schon \(a\ge b\ge c\) die Schranke \(10c\). Auf den \(M=2^{m_2-1}\) Klassen modulo Vorzeichen hat eine Zeile für Schwelle \(t>0\) das Volumen \(q_t=V(m_2,(m_2-t)/2)\). Summieren ergibt:

   | [R] \(m_2\) | \((q_4,q_6,q_8)\) | \(12(q_4+3q_6+6q_8)\) | \(10M\) | Differenz |
   |---:|---:|---:|---:|---:|
   | 10 | \((176,56,11)\) | 4 920 | 5 120 | **\(-200\)** |
   | 12 | \((794,299,79)\) | 25 980 | 20 480 | \(+5\,500\) |
   | 14 | \((3473,1471,470)\) | 128 472 | 81 920 | \(+46\,552\) |

   [R] Bei \(10\) genügt also schon das Profil \((5^3,3^6,1^3)\). Bei \(12,14\) ist die Lücke konkret: Selbst unter Anrechnung aller \(\ge6\)- und \(\ge8\)-Inzidenzen erzwingt diese Ungleichung keinen Fehlpunkt; sie nutzt insbesondere die \(660\) Anforderungen des zweiten Profils nicht.

## Frage 3 – \(K=10,\ d=8\)

[R] **Ergebnis:** Keiner der \(10\) Typen scheitert an den unten tabellierten Volumentests; für \((11,9)\) ergeben sich unter anderem \(|S(y)|\ge6\) bei Maximum \(3\) und mindestens \(5\) Dreierzeilen bei genau einer Fünferzeile. Die \(24\,192\) für die angegebene Hamming-Paarung sind exakt.

### (a) Stufentabelle

1. [R] Setze \(M_l=2^{l-1}\), \(q_l(t)=\#\{[x]:|\langle h,x\rangle|\ge t\}\) für eine feste Zeile \(h\) der Länge \(l\). Für positive Schwellen passender Parität ist \(q_l(t)=V(l,(l-t)/2)\), ferner \(q_l(0)=M_l\) und \(q_l(t)=0\) für \(t>l\). Für ein festes \(y\) sind die vollständigen **Stufen-Volumenbedingungen**
   \[
   \sum_{i=1}^{10}q_{m_2}(8-|u_i|)\ge M_{m_2},
   \]
   und entsprechend vertauscht für jedes \(z\). Nichtpositive Schwellen zählen das volle \(M_l\). Sei \(T_l\) die höchste Stufe mit \(10q_l(T_l)\ge M_l\). Dann muss jede \(y\)-Klasse \(\max|u|\ge8-T_{m_2}\) und jede \(z\)-Klasse \(\max|v|\ge8-T_{m_1}\) haben.

2. [R] In den beiden Spalten „Stufentest“ steht \(T_l:(q_l(T_l),q_l(T_l+2))\), sodass man die Grenze gegen \(M_l/10\) prüfen kann. Die nächste Spalte gibt die Einzelzeilenvolumina an den erzwungenen Maxima an. Die letzte Spalte ist das Volumen **eines** Zeilenindex in \((*)\), also \(\sum_s(q_{m_2}(s)-q_{m_2}(s+2))q_{m_1}(8-s)\), summiert über die passenden Stufen \(s\). Gegenprobe: \(10\) mal dieses Volumen muss mindestens \(2^{18}=262\,144\) sein.

   | [R] Typ | Stufentest \(m_1\) | Stufentest \(m_2\) | erzwungene Maxima | Volumina dort | gemeinsames Volumen |
   |---|---|---|---|---|---:|
   | \(19,1\) | \(7:(43796,16664)\) | \(1:(1,0)\) | \(7,1\) | \(43796,1\) | 43 796 |
   | \(17,3\) | \(7:(9402,3214)\) | \(3:(1,0)\) | \(5,1\) | \(21778,4\) | 49 984 |
   | \(15,5\) | \(7:(1941,576)\) | \(3:(6,1)\) | \(5,1\) | \(4944,16\) | 54 079 |
   | \(13,7\) | \(5:(1093,378)\) | \(5:(8,1)\) | \(3,3\) | \(2380,29\) | 56 939 |
   | \(11,9\) | \(5:(232,67)\) | \(5:(46,10)\) | \(3,3\) | \(562,130\) | 58 402 |
   | \(18,2\) | \(6:(31180,12616)\) | \(2:(1,0)\) | \(6,2\) | \(31180,1\) | 43 796 |
   | \(16,4\) | \(6:(6885,2517)\) | \(4:(1,0)\) | \(4,2\) | \(14893,5\) | 49 984 |
   | \(14,6\) | \(6:(1471,470)\) | \(4:(7,1)\) | \(4,2\) | \(3473,22\) | 54 079 |
   | \(12,8\) | \(6:(299,79)\) | \(4:(37,9)\) | \(4,2\) | \(794,93\) | 56 477 |
   | \(10,10\) | \(6:(56,11)\) | \(6:(56,11)\) | \(2,2\) | \(386,386\) | 57 268 |

   [R] In jeder Zeile reichen beide Einzelvolumina für den jeweiligen Test \(10q_l\ge M_l\); auch das kleinste gemeinsame Volumen gibt \(10\cdot43\,796=437\,960>262\,144\). Das ist kein Existenztest: Die profilweisen Ungleichungen aus Schritt 1 bleiben für alle \(10\) Typen zu prüfen.

### (b) und (c) Schnitte für \((11,9)\) und erforderliche Profile

1. [R] **Direkte Profil-Schnitte.** Links sind die Volumina zu \(1,3,5,\ge7\) rechts \(10,46,130,256\), rechts entsprechend links \(67,232,562,1024\). Für ein linkes Profil ohne \(\ge7\), mit \(a\) Fünfern und \(b\) Dreiern, gilt daher
   \[
   100+120a+36b\ge256.
   \]
   Für ein rechtes Profil ohne \(\ge7\) gilt \(670+495a+165b\ge1024\). Insbesondere ist Maximum \(1\) beidseits verboten; bei Maximum \(3\) braucht rechts die Dreiermenge mindestens \(3\) Zeilen. Für einen geometrischen Suchschnitt setze \(D_i\) gleich der benötigten antipodalen Kugel auf der Gegenseite. Für jeden Index \(j\) muss
   \[
   |D_j|+\sum_{i\ne j}\bigl(|D_i|-|D_i\cap D_j|\bigr)\ge M
   \]
   gelten, mit \(M=256\) bzw. \(1024\). Dieser Schnitt benutzt die konkreten Zeilenabstände, nicht nur das Profil.

2. [R] **Linkes Maximum \(3\): mindestens \(6\) Dreierzeilen.** Für \(s=|S(y)|\) muss \(Q_9/\{\pm1\}\) durch \(s\) Kugeln vom Radius \(2\) und \(10-s\) vom Radius \(1\) gedeckt werden. \(100+36s\ge256\) gibt zunächst \(s\ge5\). Für \(s=5\) sind \(280\) Inzidenzen vorhanden, also höchstens \(24\) Verlust durch Überlappung erlaubt. Zwei Radius-2-Kugeln haben bei gefaltetem Abstand \(0,1,2,3,4\) Schnittgrößen \(46,18,18,6,6\). Für Abstand \(1\) ist das \(2(1+8)=18\); für Abstand \(2\) ist es \(2+2(1+7)=18\); bei Abstand \(3\) sind es die \(3\) Einzel- und \(3\) Zweiermengen der verschiedenen Koordinaten, bei Abstand \(4\) deren \(\binom42=6\) Zweiermengen.

   [R] Hat irgendein Paar Abstand \(\le2\), verliert man schon \(18+3\cdot6=36\), indem man dieses Paar zuerst einfügt. Andernfalls liegen alle Abstände in \(\{3,4\}\), und kein dritter Ball enthält den gesamten Sechserschnitt eines Paars. Relativ zum ersten Zentrum kann der dritte als Menge \(E\) von \(3\) oder \(4\) Koordinaten geschrieben werden. Für Punkte des Schnitts mit Gewicht \(\le2\) ist das Gegenwort zu \(E\) mindestens \(9-4-2=3\) entfernt. Bei Paarabstand \(3\) müssten alle \(3\) Einzelmengen im dritten Ball liegen; das erzwingt, dass \(E\) genau die Dreiermenge des zweiten Zentrums ist. Bei Paarabstand \(4\) müssten alle \(6\) Zweiermengen in \(E\) liegen, was ebenfalls das zweite Zentrum erzwingt. Beim Einfügen von \(5\) Kugeln verliert man daher mindestens \(6+7+6+6=25\). Das lässt höchstens \(255\) Punkte gedeckt. Die Literaturzahl \(K(9,2)=16\) wird hierfür nicht benötigt; allein auf alle kleinen Kugeln zu Radius \(2\) aufzurunden würde nur \(20\) Wörter ergeben, ohne \(s\) einzuschränken.

3. [R] **Linkes Maximum \(5\) mit nur einem Fünfer: mindestens \(5\) Dreier.** Außerhalb der Radius-3-Kugel des Fünferzentrums verbleiben genau die \(\binom94=126\) Vierermengen, relativ zu diesem Zentrum dargestellt. Eine Radius-2-Kugel mit Zentrum vom gefalteten Gewicht \(\delta=0,1,2,3,4\) trifft davon \(0,0,21,21,26\); für Radius \(1\) sind es \(0,0,0,6,6\). Beispielsweise hat ein Radius-2-Zentrum \(P\) vom Gewicht \(4\) als Treffer \(P\), \(4\cdot5=20\) Vierermengen mit Schnittgröße \(3\) und \(5\) disjunkte Vierermengen. Für Gewicht \(2\) gibt es \(\binom72=21\) Obermengen, für Gewicht \(3\) gibt es \(6\) Obermengen und \(\binom64=15\) disjunkte Treffer. Somit liefern \(b\le3\) Dreier höchstens \(26b+6(9-b)\le114<126\).

   [R] Für \(b=4\) ist das Budget \(134\), der Spielraum also \(8\). Mindestens \(3\) der \(4\) Radius-2-Zentren müssen Gewicht \(4\) haben, da jedes andere mindestens \(5\) kostet. Zwei solche Viererzentren \(P,Q\) haben unter den \(126\) Punkten gemeinsame Treffer in den Anzahlen \(10,4,4,10,26\) für \(|P\cap Q|=0,1,2,3,4\). Dies zählt die \(X\) mit \(|X\cap P|,|X\cap Q|\in\{0,3,4\}\), durch Aufteilung auf \(P\cap Q,P\setminus Q,Q\setminus P,[9]\setminus(P\cup Q)\) der Größen \(j,4-j,4-j,1+j\). Bei \(4\) Viererzentren beträgt der Überlappungsverlust mindestens \(3\cdot4=12\); bei \(3\) mindestens \(2\cdot4+5=13\) einschließlich des schlechteren Zentrums. Beides übersteigt \(8\).

4. [R] **Ein entsprechender rechter Schnitt.** Bei rechtem Maximum \(5\) mit genau einem Fünfer bleiben links außerhalb seiner Radius-4-Kugel die \(\binom{11}{5}=462\) Fünfermengen. Für Zentrumgewichte \(0,1,2,3,4,5\) treffen Radius-3-Kugeln davon \(0,0,84,84,112,112\), Radius-2-Kugeln \(0,0,0,28,28,37\). Die Zahlen zählen für eine Fünfermenge \(X\) und Zentrum \(E\) die Fälle
   \[
   \min(5+|E|-2|X\cap E|,\ 6-|E|+2|X\cap E|)\le r
   \]
   mit \(\binom{|E|}{|X\cap E|}\binom{11-|E|}{5-|X\cap E|}\) Möglichkeiten. Bei höchstens einem Dreier sind also höchstens \(112+8\cdot37=408<462\) Treffer möglich. Rechts braucht dieses Profil mindestens \(2\) Dreier.

5. [R] **Anzahlen und Zeilenmengen als Suchschnitte.** Links gibt es \(1024\) Klassen und insgesamt, über die \(10\) Zeilen, genau
   \[
   (4620,3300,1650,550,110,10)
   \]
   Einträge der Beträge \(1,3,5,7,9,11\). Definiere \(n_{3,s}\) als Anzahl der Klassen mit Maximum \(3\) und \(s\) Dreiern, \(n_{5,a,b}\) als Anzahl mit Maximum \(5\), \(a\) Fünfern und \(b\) Dreiern. Zulässig sind nur \(s\ge6\), \(a\ge1\), \(a+b\le10\), und bei \(a=1\) nur \(b\ge5\). Insbesondere
   \[
   \sum_s s n_{3,s}+\sum_{a,b}b n_{5,a,b}\le3300,\qquad
   \sum_{a,b}a n_{5,a,b}\le1650.
   \]
   Schreibt man \(n_3=\sum_s n_{3,s}\), \(n_5=\sum_{a,b}n_{5,a,b}\), folgt \(6n_3+5\sum_b n_{5,1,b}\le3300\), insbesondere \(n_3\le550\). Da es \(670\) Inzidenzen für \(\ge7\) gibt und eine einzelne Zeile davon \(67\) liefert, gilt \(354\le n_3+n_5\le957\). Rechts lauten die entsprechenden exakten Inzidenzzahlen \((1260,840,360,90,10)\) für \(1,3,5,7,9\).

   [R] Für die **Zeilenmengen** speichere für jede niedrige Klasse links \((A,B)=(\{|u|=5\},\{|u|=3\})\), rechts \((C,D)=(\{|v|=5\},\{|v|=3\})\). Für jedes Paar solcher Masken ist der exakte Schnitt
   \[
   A\cap(C\cup D)\ne\varnothing\quad\text{oder}\quad B\cap C\ne\varnothing.
   \]
   Das sind genau die \(3\) erfolgreichen Stufenkombinationen \((5,5),(5,3),(3,5)\); alle übrigen niedrigen Kombinationen summieren sich zu höchstens \(6\). Insbesondere dürfen nicht beide Seiten eine Klasse mit Maximum \(3\) besitzen. Die linken \(s\)-Mengen müssen jede rechte Fünfermaske treffen, wenn links überhaupt Maximum \(3\) vorkommt. Zusammen mit der automatischen Deckung bei einem Betrag \(\ge7\) ist der Maskentest für ein konkretes Paar vollständig. Die angegebenen Anzahlschranken allein entscheiden nicht, ob zwei realisierbare Maskenfamilien mit \(10\) Zeilen existieren.

### (d) Die \(24\,192\) offenen Klassen

1. [R] Im \([7,4]\)-Hamming-Code tragen die \(7\) Nichtnullspalten der Prüfmatrix die \(7\) Punkte der Fano-Ebene. Die \(7\) Codewörter vom Gewicht \(3\) sind ihre \(7\) Geraden; ihre Komplemente, \(0\) und \(\mathbf1\) ergeben die \(16\) Codewörter. Jede Mehrheitsbelegung der \(7\) Koordinaten hat genau einen nächsten Codepunkt, da die \(8\) möglichen Syndrome genau den Fehlern \(0,e_1,\ldots,e_7\) entsprechen. Modulo Vorzeichen gibt es für jedes Syndrom \(8\) nächste Zeilen. Bei Fehlerposition \(j\ne0\) ist das Betragsprofil \((5,3^3,1^4)\): Die \(3\) Dreierzeilen unterscheiden sich von der nächsten Zeile durch die \(3\) Geraden durch \(j\). Bei Fehler \(0\) ist es \((7,1^7)\).

2. [R] Eine verdreifachte Koordinate liefert zur Mehrheitsbelegung entweder Gewicht \(1\) mit Multiplizität \(3\) oder Gewicht \(3\) mit Multiplizität \(1\). Bei einer einstimmigen Gruppe an \(p\) und Fehler \(j\ne p\) steigt die nächste Zeile auf mindestens \(7\), einschließlich \(j=0\). Bei \(j=p\) ergibt sich \((5^3,3^5)\): Die \(3\) Geraden durch \(p\) steigen von \(3\) auf \(5\), die anderen \(4\) von Betrag \(1\) auf \(3\), die nächste Zeile sinkt von \(5\) auf \(3\). Damit ist die Gegenseite stets gedeckt, da sie irgendwo mindestens \(5\) hat. Bei zwei einstimmigen Gruppen \(p,q\) links ist die nächste Zeile für \(j\notin\{p,q\}\) mindestens \(9\). Bei \(j=p\) gibt es \(2\) Geraden durch \(p\), die \(q\) meiden, mit Betrag \(3+2+2=7\); für \(j=q\) gilt dasselbe. Also können nur die Klassen mit **allen Gruppen gespalten** und **Syndrom ungleich \(0\)** offen sein.

3. [R] Links gibt es davon \(8\cdot7\cdot3^2=504\). Fixiere eine solche Klasse. Rechts kann die nächste Zeile nur eine der \(4\) linken Einserzeilen sein; sonst deckt ihr rechter Fünfer. Für jede dieser \(4\) Möglichkeiten ist die linke Fünferzeile eine andere Zeile, die relativ zum rechten Codepunkt genau eine Fano-Gerade bezeichnet. Genau \(4\) der \(7\) Fehlerpositionen liegen nicht auf dieser Geraden, lassen also auch die linke Fünferzeile ungedeckt. Die rechte gespaltene Gruppe liefert Multiplizität \(3\). Das ergibt
   \[
   504\cdot4\cdot4\cdot3=24\,192
   \]
   offene von \(1024\cdot256=262\,144\) Klassen. Eine Zeilenzuordnung ändert weder die \(4\) möglichen rechten nächsten Zeilen noch die \(4\) Fehlerpositionen außerhalb der jeweils bezeichneten Geraden. Daher gilt die Zahl für alle \(8!\) Zuordnungen.

4. [R] **Was sich auf beliebige Hälften überträgt:** Für beliebige *antipodale* \(16\)-Wort-Codes der beiden angegebenen Radien, also \(8\) Zeilen je Hälfte, sei \(N_{5,1,b}\) die Anzahl linker Klassen mit genau einem Fünfer, \(b\) Dreiern und sonst Einsern. Die Außenvolumina \(26,6\) aus (b) geben die allgemeine Untergrenze
   \[
   \#\text{offen}\ \ge\ \sum_{b=0}^4(84-20b)\,N_{5,1,b},
   \]
   denn für eine solche Klasse sind höchstens \(130+26b+6(7-b)\) rechte Klassen gedeckt. Die Konstanz \(24\,192\) benutzt dagegen die \(504\) speziellen linken Klassen und die Geradenstruktur rechts. Sie liefert ohne einen weiteren Satz über die Profilverteilungen anderer Codes keine einheitliche positive Untergrenze für alle Paare.

## Frage 4 – Zeilen mit Nullen

[R] **Ergebnis:** Die Existenzimplikation bleibt unentschieden; die Volumenzählung erzwingt höchstens \(14\) Zeilen mit mindestens \(3\) Nullen und höchstens \(14\) Nullzeilen, falls keine Zeile genau \(2\) Nullen hat.

1. [R] **Volumina und lokaler Ersatz.** Eine Zeile mit Träger \(s\) hat auf \(Q_{15}\) das Volumen \(2^{16-s}V(s,(s-d_s)/2)\), mit \(d_s=7\) für ungerades und \(d_s=8\) für gerades \(s\). Für \(s=15,14,\ldots,7\) erhält man
   \[
   3882,\ 1880,\ 3024,\ 1264,\ 2144,\ 704,\ 1280,\ 256,\ 512;
   \]
   für \(s\le6\) ist das Volumen \(0\). Bei geradem Träger kann man **eine** Null beliebig durch \(\pm1\) ersetzen: Jeder bisher erfasste Punkt behält Betrag mindestens \(8-1=7\). Das füllt insbesondere eine einzelne Null aus, beweist aber keinen Ersatz für Träger \(13\).

2. [R] **Das Hindernis bei zwei Nullen.** Die einseitige Träger-13-Kappe ist \(B_3(c)\times Q_2\), mit \(378\cdot4=1512\) Punkten. Von einem beliebigen Zentrum kann man in den \(13\) festen Koordinaten innerhalb \(B_3(c)\) Abstand mindestens \(3\) erreichen, in den freien zusätzlich \(2\); Radius \(4\) reicht also nicht. Die zwei Zentren \((c,00),(c,11)\) genügen, da die freien Bits von einem der beiden höchstens Abstand \(1\) haben. Der Schnitt mit der Radius-4-Kugel um \((c,00)\) hat \(3\cdot378+92=1226\) Punkte. Die Differenzen sind exakt \(1941-1226=715\) und \(1512-1226=286\). Damit scheitert die zeilenweise Kappeninklusion mit einer einzigen Vollzeile; das ist **kein** Gegenbeispiel zur Existenzimplikation für \(15\) Zeilen.

3. [R] **Zählbudget.** Schreibe \(n_s\) für die Anzahl Zeilen mit Träger \(s\) und \(v_s\) für ihr Volumen. Jede Lösung muss
   \[
   \sum_s (3882-v_s)n_s\le15\cdot3882-32768=25\,462
   \]
   erfüllen. Der Verlust ist mindestens \(1738\) bei mindestens \(3\) Nullen, \(2002\) bei genau einer Null und \(2602\) bei mindestens \(5\) Nullen. Die jeweiligen Obergrenzen sind damit \(14,12,9\). Allgemeiner: Sind \(z\) Zeilen mit Nullen behaftet und haben davon \(t\) genau zwei Nullen, so gilt
   \[
   1738z-880t\le25\,462.
   \]
   Für \(z=15\) ist also \(t\ge1\) nötig. Eine unbedingte Obergrenze \(z<15\) folgt aus dem Volumenbudget nicht: Das Profil \(n_{13}=15\) hat Volumen \(45\,360>32\,768\). Ob sich dieses oder ein anderes Nullprofil tatsächlich überdeckend anordnen lässt, entscheidet die Rechnung nicht.

**Status 1:** [R] **bewiesen** – Ausschluss für \(m_2=9,11,13\).  
**Status 2:** [R] **Teilresultat** – Ausschluss für \(m_2=10\); \(12,14\) offen.  
**Status 3:** [R] **Teilresultat** – Stufentabelle, Profilschnitte und \(24\,192\) bewiesen; allgemeiner Ausschluss für \(K=10\) offen.  
**Status 4:** [R] **Teilresultat** – Trägerbudget bewiesen; Dominanzimplikation offen.
