> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-astra
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 24.11.2026, 14:27
> - Batch: Auftrag openrouter:batch-1795525403-QdZO1ZBYQhr088JQZEke, abgegeben 24.11.2026, 14:03, fertig 24.11.2026, 14:11
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 19.067 · Ausgabe 12.669 (davon Denken 6.649) · gesamt 31.736
> - Kosten laut OpenRouter: 0,33 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: high
> - finish_reason: stop

---

**Urteil: Die blinde Rekordjagd würde ich stoppen; das 48er-Ergebnis würde ich zur Meldung vorbereiten. Die Hadamard-Route würde ich aber erst nach einem weiteren, eng begrenzten Test schließen.** Ihr habt bei Ordnung 20 die *volle* Matrix und bei Ordnung 24 die Matrix *minus eine* Spalte ausgeschlossen. Die unmittelbar interessanten Fälle sind auch **Ordnung 20 minus zwei Spalten** und **Ordnung 24 minus drei Spalten**. Beide können die Rekordschwelle erreichen, ohne einem eurer Negativbefunde zu widersprechen. [Urteil; Rechnung unten]

**Das stärkste positive Ergebnis ist nicht der Suchlauf, sondern ein kurzer, reproduzierbarer Satz:** Aus einer expliziten \(24\times20\)-Matrix und **einer einzigen** nachgewiesenen normalen Koordinate folgt
\[
                         K(2R+8,R)\le 48\qquad(R\ge6).
\]
Dazu braucht man weder die Klassifikation der Hadamard-Matrizen noch eine Rechnung der Größe \(2^{28}\), geschweige denn eine Aussage über alle größeren Würfel. Ob der Satz *neu* ist, ist eine andere, derzeit offene Frage. [Eigene Herleitung unten; Basiszertifikat laut `STATUS.md`]

**Kennzeichnung:** **[P]** = im vorgelegten Protokoll berichtet, nicht von mir ausgeführt; **[Q]** = am mitgelieferten Quelltext ablesbar; **[R]** = eigene nachprüfbare Rechnung oder Folgerung; **[L]** = Literaturwissen aus dem Gedächtnis, hier extern unbelegt; **[U]** = Urteil, Vorschlag oder subjektive Schätzung, kein Befund. Ich hatte weder Webzugang noch die Wortlisten, Logs oder Verifier zur Ausführung.

## 1. Was die negativen Ergebnisse tatsächlich ausschließen

**[R] Die exakte Budgetformel ist nützlicher als eine Liste ausgewählter Zellen.** Für ganzzahlige Zieldiskrepanz \(d\) muss
\[
        \max(m,N)<8d^2/25 .
\]
Das maximale Budget an Spalten **und** Zeilen ist für \(d=7,8,9,10,11,12\) der Reihe nach \(15,20,25,31,38,46\). Insbesondere ist bei \(d=10\) die Ordnung 32 nur Gleichstand. Daraus folgen drei verschiedene Befunde:

| Matrix | Bekannt laut [P] | Offene oder geschlossene Rekordfrage [R] |
|---|---|---|
| \(20\times18\), zwei Spalten einer Hadamard-20 gestrichen | volle Diskrepanz 6 | **Offen:** Diskrepanz 8 genügt; \(6+2=8\). |
| \(24\times23\), eine Spalte einer Hadamard-24 gestrichen | volle Diskrepanz 6 bei allen 60 gelieferten Vertretern | **Geschlossen** unter der Klassenvollständigkeit der Quelle: höchstens 7, nötig 9. |
| \(24\times21\), drei Spalten einer Hadamard-24 gestrichen | derselbe Vollmatrixbefund | **Offen:** Diskrepanz 9 genügt; \(6+3=9\). |

**[R]** Die verwendete Ein-Spalten-Abschätzung braucht nur die Dreiecksungleichung: Eine gute Färbung der vollen Matrix bleibt nach Streichen einer Spalte bis auf \(+1\) gut. Das ist eine obere Schranke, kein Grund, den Grenzfall der Tabelle auszuschließen. Gerade eure \(24\times20\)-Konstruktion zeigt, dass Streichen die Diskrepanz tatsächlich erhöhen kann. **[U]** Ich würde diese beiden Lücken höher priorisieren als jede neue lange Tabu-Suche.

Weitere Korrekturen zur Deutung:

- **[P,R]** Bei \((22,7)\) wurden **14 von 24** Zufallsläufen mit 52 Wörtern gelöst, beim Typ \((11,11)\) sogar 8 von 8. Das widerlegt „die Methode funktioniert nur bei Länge 20“. Es belegt **nicht**, dass sie bei Zieldiskrepanz 9 funktioniert: Alle diese Treffer gehören zur Linie \(m-2R=8\).
- **[R]** Der Vorbehalt, ein durch Verdreifachung erzeugter Folgecode liege „nicht notwendig“ wieder in der \(d=2\)-Familie, ist für eure konkreten Ausgangscodes unnötig. Die Abbildung, die eine Koordinate dreifach schreibt, ist linear, injektiv und erhält den Einsvektor. Sie bildet Nebenklassen von \(D\) auf Nebenklassen des zweidimensionalen Bildes von \(D\) ab. Aus \((11,9)\) entstehen je nach Koordinate \((13,9)\) oder \((11,11)\). **Ihr habt also sogar für den erfolglosen Typ \((13,9)\) bei \((22,7)\) einen konstruktiven Existenznachweis.** [P,R] Das ist euer bestes internes Beispiel dafür, wie wenig 0 von 8 über Nichtexistenz sagt.
- **[P,R]** Bei \((21,6)\) fehlt eine Kontrolle mit nachgewiesener Zugehörigkeit zur getesteten Familie; bei \((23,7)\) wurde die 64er-Kontrolle übersprungen. „Drei Typen ungelöst“ heißt nicht „alle Typen von \(D\) ungelöst“. Ein Tabellenwert für beliebige Codes ist keine Positivkontrolle für einen bestimmten Translationstyp.
- **[P,R]** Die Zahl 6108 beweist, dass man aus diesen Darstellungen nicht einfach eine der zwölf Nebenklassen entfernen kann. Sie ist weder eine untere Schranke für 44 Wörter noch ein Test für das gleichzeitige Ersetzen mehrerer Klassen. Auch „verschiedene Wortlisten“ oder „disjunkte Wortlisten“ bedeutet nicht „inäquivalente Codes“: Schon eine Translation kann die Liste ändern, ohne das Überdeckungsproblem zu ändern.
- **[P,R]** Die 30 780 Gegenfärbungen bei Ordnung 40 sind dagegen echte Ausschlusszertifikate für die angegebenen Auswahlen mit gleicher Ausrichtung. Das ist qualitativ stärker als 30 780 erfolglose Suchen nach einem Rekord. Die Quantoren nicht vertauschen: Nicht getestet sind beliebige relative Zeilenpermutationen, beliebige Streichmuster oder beliebige \(40\times36\)-Matrizen.
- **[R]** Kleine Korrektur zur Laufzeit: \(48\,408\) bis \(342\,182\) Schritte mal \(0{,}475\) Sekunden sind **6,4–45,1 Stunden**. Die **8–55 Stunden** passen zur gemessenen Schrittzeit unter Vierfachlast. Wichtiger: Übertragbar ist die gemessene *Schrittzeit*, nicht die *Schrittzahl bis zum ersten Treffer*. Sie könnte bei \((25,8)\) kleiner, größer oder innerhalb dieser Familie effektiv unendlich sein.

## 2. Warum die Doppelung funktioniert – und wie ich nach einem Gegenstück suchen würde

### Eine Erklärung in halber Dimension

**[R]** Seien \(I,J\) die behaltenen Spaltenmengen der beiden Hälften. Für eine Färbung setze \(u=H_I x,\ v=H_J y\). Dann gilt exakt
\[
 \max\{\|u+v\|_\infty,\|u-v\|_\infty\}
                      =\max_i (|u_i|+|v_i|).
\]
Die Frage ist also nicht primär, wie groß die Norm *eines* Hadamard-Bildes ist. Entscheidend ist, ob sich zwei **Betragsprofile** so paaren lassen, dass die Spitzen des einen in die Täler des anderen fallen. Für die relative Zeilenausrichtung \(P\) wird rechts einfach \(|v_i|\) durch \(|v_{P(i)}|\) ersetzt, bis auf die Konvention für \(P\).

**[R]** Für die erfolgreiche \(11+9\)-Aufteilung lässt sich das konkretisieren. Angenommen, die Summe der Profile sei überall höchstens 6. Da beide Profile ungerade sind, ist jedes höchstens 5. Normalisiere die eine fehlende Spalte auf lauter \(+1\) durch Zeilenvorzeichen. Die zwölf Werte \(u_i\) haben dann Summe null, Quadratsumme \(12\cdot11=132\) und alle denselben Rest modulo 4: Zwei normalisierte Zeilen unterscheiden sich auf genau sechs der behaltenen Stellen, also unterscheiden sich ihre Skalarprodukte um ein Vielfaches von 4. Nach globalem Vorzeichenwechsel sind die möglichen Werte \(-3,1,5\). Aus Anzahl, Summe und Quadratsumme folgt zwingend
\[
                 \#(-3)=6,\qquad \#1=3,\qquad \#5=3 .
\]
Das rechte Profil **muss** auf drei vorgeschriebenen Stellen Betrag 1 und auf sechs weiteren Betrag höchstens 3 haben. Für \(v\) gilt zusätzlich \(\sum v_i^2=108\).

**[P,R]** Damit hat die beobachtete 8 eine präzise Erklärung: **Parität lässt zwischen 6 und 8 nichts zu; 6 erfordert eine sehr spezielle Paarung.** Laut eurer Exhaustion fehlt diese Paarung beim guten \(1+3\)-Muster und existiert beim schlechten. Die Momentrechnung allein beweist den Unterschied zwischen den Überlappungsmustern nicht. Wer einen kurzen, von der bisherigen Exhaustion unabhängigen Beweis will, kann für das feste gute Muster nur \(2^{10}\) linke und \(2^8\) rechte Profile bis auf Vorzeichen bilden und obige Verträglichkeit testen. Ein zweites, schlechtes Muster ist die notwendige Gegenkontrolle. **[U]** Das wäre für die Ausarbeitung ergiebiger als eine bloße Wiederholung von „alle \(2^{20}\) Färbungen geprüft“.

**[R,U]** Das Gegenstück für 9, 10 oder 11 ist also nicht „nimm die nächstgrößere Hadamard-Matrix“. Es ist: **Wähle Spaltenmengen, Paritäten und eine relative Ausrichtung so, dass kein Profilpaar unter der letzten verbotenen Schwelle liegt.** Bei einem ungeraden \(m\) und Ziel 9 ist diese Schwelle 7, nicht 8. Das erlaubt eine gezieltere und wesentlich billigere *Widerlegung* von Kandidaten als eine allgemeine Suche nach Codes.

### Konkrete, begrenzte Versuche

**[U]** Die Chancen in der letzten Spalte sind meine groben, **unkalibrierten** Schätzungen für einen **Spencer-Rekord im angegebenen Pilotversuch**, nicht für die Existenz eines Codes. Die Zeitangaben sind Wall-Zeit-Budgets auf vier Kernen, keine versprochenen Laufzeiten. Vor jeder Serie: ein bekannter positiver Fall und ein bekannter Gegenzeuge durch denselben Rechenpfad.

| Priorität / Ansatz | Erster billiger Test | Abbruchkriterium | Chance [U] |
|---|---|---|---|
| **1. Hadamard-Streichungen für 8 und 9** | **[R,U]** Ordnung 20: alle \(3\binom{20}{2}=570\) Fälle für \(20\times18\). Ordnung 24: zuerst alle \(\binom{24}{3}=2024\) Fälle der expliziten Doppelung, danach weitere Klassen bis zum Zeitlimit. Für 20 genügen Gegenzeugen mit Maximum 6, für 24 mit Maximum 7. Zuerst bereits bekannte gute Vollmatrixfärbungen projizieren; nur Überlebende exakt prüfen. Kontrolle: bekannte gute und schlechte \(24\times20\)-Streichung. | 2 Blöcke à 30 min; erledigte Auswahl und Zeugen speichern. Nur ein *exakt* überlebender Fall rechtfertigt Fortsetzung. Bei Zeitablauf unbekannt, nicht negativ, buchen. | Größenordnung **1–5 %** für beide Tests zusammen; mein bester Rekordversuch hier. |
| **2. Eine Spalte an den 48er-Code anbauen** | **[R,U]** Gesucht \(24\times21\) mit Diskrepanz 9. Für jede alte Färbung \(x\) mit \(\|Ax\|_\infty=8\) seien \(T_x=\{i:|(Ax)_i|=8\}\). Eine neue Vorzeichenspalte \(b\) funktioniert genau dann, wenn unter \(\operatorname{sgn}((Ax)_i)b_i,\ i\in T_x\), **beide Vorzeichen** vorkommen, für jedes solche \(x\). Denn beide Färbungen der neuen Spalte müssen abgefangen werden; alte Maxima \(\ge10\) sind automatisch sicher. Das ist ein kleines Boolesches Erfüllbarkeitsproblem mit 24 Variablen. Zuerst nur nach \(|T_x|=1\) suchen. Kontrolle: von einer bekannten guten Matrix eine Spalte entfernen und Wiederanbau prüfen, mit entsprechend angepasster Schwelle. | Ein einziges Singleton erledigt die betreffende Matrix. Sonst exakte Klauselprüfung; maximal 30 min für mehrere 48er-Realisierungen. Keine Folgerung für gleichzeitige Zeilenänderung. | **<1 %**, aber sehr billig und bei negativem Ausgang beweiskräftig. |
| **3. Ungleiche Ausrichtung, weitere Blöcke, Konferenz-/Gewichtsmatrizen** | **[R,U]** Bei der Doppelung Profile vorrechnen und für z. B. 100 feste \(P\) nach einer verträglichen Paarung suchen, zuerst bei kleinen Dimensionen. Für eine tatsächlich vorhandene Konferenz-, Gewichts- oder Dreiblockmatrix genau dieselbe *Schwelle nach tatsächlicher Ordnung* festlegen, dann ganzzahlige Gegenfärbung suchen. Nicht erst eine Bibliothek neuer Matrizen bauen. | 30 min; für jeden Kandidaten einen einzigen nachgerechneten Gegenzeugen archivieren. Sind die Kandidaten mühelos färbbar, keine größere Serie. Nur Überlebende bekommen eine exakte Prüfung. | **<1 %**. Insbesondere würde ich nicht \(20!\) Ausrichtungen durchmustern. |
| **4. Andere Symmetrie bei \((25,8),(23,7),(30,10)\)** | **[R,U]** Zyklische Gruppen der Ordnung 5 mit Komplement: etwa \(5^5,\ 5^4 1^3,\ 5^6\), respektive. Fünf volle Wortbahnen geben 50 Wörter, sechs geben 60. Zuerst nur die unter der Permutation **fixen Würfelpunkte** überdecken: Alle Wörter einer Permutationsbahn haben zu einem solchen Punkt denselben Abstand. Dort zählen nur die Blockgewichte. Erst zulässige Gewichtsmuster durch reale Bahnvertreter verfeinern; weitere unüberdeckte Punkte als Bedingungen hinzufügen. Kontrolle: größere, explizit verifizierte Überdeckung in derselben Symmetrie, eine Bahn entfernen und reparieren. | 1 h **für eine** Zelle, bevorzugt \((25,8)\). Kein Lauf ohne Kontrolle; keine Fortsetzung allein wegen kleiner Restquote. Unlösbarkeit der Fixpunktbedingungen schließt nur das jeweilige Bahnmuster aus. | **<1 %** im Pilotversuch. Vorteil gegenüber \(d=2\): Bei Ziel 9 darf man die vollen **50** statt nur 48 Wörter nutzen. |
| **5. Gebrochene Einträge / Nullen** | **[R,U]** Statt allgemeinem nichtlinearem Optimierer: eine vorhandene fast ausreichende *quadratische* Matrix nehmen, exakte schlechteste Färbungen sammeln und für jede wenigstens eine tragende Zeile mit Vorzeichen wählen. Bei fixierter Wahl ist „alle diese Skalarprodukte \(\ge t\), Einträge in \([-1,1]\)“ ein LP. Weitere Gegenfärbungen nachliefern. Ein positiver Kandidat muss rational rekonstruiert und vollständig geprüft werden. | 30–60 min, mit künstlich verschlechterter rückholbarer Kontrolle. Fehlgeschlagenes LP bei **fixierter** Zeilenwahl ist kein Ausschluss anderer Zuordnungen. | **<1 %**. Nicht an der gepaddeten \(8\times8\)-Kontrolle beginnen, siehe unten. |

**[R,L] Eine nützliche Vorprüfung für gebrochene Einträge:** In der positiven Kappe einer Zeile \(a\), also \(a\cdot x>t\), haben zwei Färbungen \(x,y\) Abstand \(d_H(x,y)<m-t\): In \(a\cdot(x+y)/2\) tragen nur die übereinstimmenden Koordinaten bei. Bei \(m=8,\ t=5\) hat jede Kappe daher Durchmesser höchstens 2. Eine solche Teilmenge des 8-Würfels hat höchstens 9 Elemente (Spezialfall des diametrischen Satzes für den Würfel [L]; auch elementar: nach Translation auf \(0\) sind enthaltene Zweiermengen paarweise schneidend, und jede enthaltene Einermenge muss jede Zweiermenge treffen). Acht Zeilen und ihre negativen Kappen erfassen also höchstens \(16\cdot9<256\) Punkte. **Keine reelle \(8\times8\)-Matrix kann den angegebenen Rekord strikt schlagen.** Das ist ein echter Ausschluss, nicht nur eine Warnung vor LP-Politur. [R]

**[R,U] Falls ihr doch die \(d=2\)-Suche verbessern wollt:** Der Quotient zu eurem Typ \((p,q)\) ist das Produkt zweier Würfel, in denen jeweils ein Wort mit seinem Komplement identifiziert wird. Der Abstand ist die Summe der beiden gefalteten Abstände \(\min(h,p-h)\) bzw. \(\min(h,q-h)\). Für Mitten \((a_i,b_i)\) und *jeden* festen linken Punkt \(u\) ist deshalb notwendig
\[
       \sum_i V_q\bigl(R-d(u,a_i)\bigr)\ \ge\ 2^{q-1},
\]
wobei \(V_q\) das Kugelvolumen im gefalteten rechten Würfel ist, bei negativem Radius null. Das gibt eine billige Siebung einer **halben** Kandidatenliste, bevor die \(b_i\) überhaupt gewählt werden; für kritische \(u\) kann man danach die tatsächliche Vereinigung rechter Kugeln als Bitmenge testen. Erste Kontrolle: Die explizite 48er-Lösung muss jeden Schnitt bestehen. Nach 30 min stoppen, wenn die Siebung auch schlechte Listen kaum verwirft. Chance auf unmittelbaren Rekord **<1 %** [U]; der Zweck wäre eine bessere Auswahl von Kandidaten, nicht ein weiterer Lauf mit derselben Nachbarschaft.

**[U] GPU-Entscheidung: vorerst nein.** Gegen einen halbstündigen lokalen *Benchmark* mit vorhandener Software spricht nichts. Einen Port würde ich erst finanzieren, wenn ein Suchverfahren eine strukturidentische Kontrolle besteht, die exakte Bewertung CPU/GPU übereinstimmt und die **Ende-zu-Ende-Schrittzeit** gemessen ist. Eine schnelle Walsh-Transformation löst weder das falsche Typenbudget noch das Problem unbekannter Erreichbarkeit. Die drei ersten Versuche oben brauchen sie nicht.

## 3. Der Tabellensatz und seine Neuheit

### Die unendliche Familie: ja, mit kurzem Beweis

**[R]** Schreibe die Wörter nach Wahl einer normalen Koordinate als \((b,c)\). Für ein Wort \(y\) auf den übrigen 19 Stellen sei \(d_b(y)\) sein Minimalabstand zu den Restwörtern mit erstem Bit \(b\). Die Normbedingung \(N\le13\) besagt in dieser Notation exakt
\[
                        d_0(y)+d_1(y)+1\le13
                        \quad\text{für jedes }y .
\]
Die \(+1\) ist wichtig: Bei den beiden Abständen zu den zwei Codehälften auf dem vollen Würfel zahlt genau eine Hälfte für das falsche Bit.

Ersetze nun die gewählte Stelle in jedem Codewort durch \(2s+1\) Kopien. Hat ein Testwort dort Gewicht \(k\), so ist sein Abstand zum neuen Code
\[
             \min\{d_0(y)+k,\ d_1(y)+2s+1-k\}.
\]
Die beiden ganzen Zahlen haben Summe höchstens \(13+2s\), also ist ihr Minimum höchstens \(6+s\). Die Länge ist \(20+2s\), die Wortzahl bleibt 48, und Komplementabschluss bleibt erhalten. Mit \(R=6+s\) ist der Satz bewiesen. **Keine Induktionsannahme über erneut zu testende Normalität ist nötig.** [R]

**[Q,P,U] Wichtige letzte Lücke in der Beweiskette:** `construct_48.py` liefert direkt die expliziten Matrizen und prüft die Orthogonalität von \(H\). Es prüft selbst **nicht** die Normalität. Im Protokoll ist die Normprüfung ausdrücklich für die Suchtreffer beschrieben; für die expliziten Paley-Dateien ist ausdrücklich der Radius dokumentiert. Für die Meldung bitte **die Norm an einer benannten Koordinate der genau eingereichten Paley-Datei** mit zwei unabhängigen Wegen prüfen. Dann entfällt jede Abhängigkeit von einer behaupteten Äquivalenz der Suchtreffer mit der Konstruktion. Ein Verifier kann direkt \(\max_y(d_0(y)+d_1(y))\) auf \(2^{19}\) Wörtern berechnen; der andere die Norm nach ihrer ursprünglichen Definition auf \(2^{20}\) Wörtern. Erwartungswert für die erste Ausgabe: **12**, nicht 13, sofern die Norm exakt 13 ist. [R]

### Weitere Zellen und Meldung

- **[P,R]** Gegen die *im Protokoll zitierte Tabelle* verbessert 48 die fünf Zellen \((20,6),(22,7),(24,8),(26,9),(28,10)\). Die fünfte folgt bereits aus dem Beweis. Die Rechnung bei \(n=28\) ist als Integrationskontrolle sinnvoll, nicht als logische Voraussetzung. Auch \((30,11),(32,12),\ldots\) folgen; ob dort eine **Tabellenverbesserung** vorliegt, muss für jede Zeile verglichen werden. \((30,10)\) folgt ausdrücklich **nicht**.
- **[R]** Weitere sichere Regeln sind Punktieren \((n-1,R,M)\), freie Koordinate \((n+1,R,2M)\) und direktes Produkt \((n+n',R+R',MM')\), jeweils als obere Schranken. Bei zwei selbstkomplementären Codes mit je einer normalen Koordinate kann man diese Stellen identifizieren und nur Wörter mit gleichem Bit kombinieren: Länge \(n+n'-1\), höchstens \(MM'/2\) Wörter, Radius höchstens \(R+R'\). Der Radius folgt durch dieselbe Summenabschätzung wie oben für die zwei Bitklassen. **[U]** Diese Regeln einmal maschinell über die vorhandenen Tabellenlängen laufen lassen und nur strikte Verbesserungen mit Herkunft ausgeben. Ohne Vergleich keine Liste vermeintlich neuer Folgeergebnisse melden.
- **[P,U] Neuheit:** Stärker als „Gemini fand nichts“ ist der konkrete CSV-Vergleich; schwächer als ein Prioritätsnachweis sind **beide**. Vier Recherchen sind keine vier unabhängigen Literaturbeweise. Besonders kritisch sind der nicht eingesehene Volltext von Östergård–Kaikkonen, die dortigen Konstruktionen und Parameterfamilien, sowie Arbeiten zu **punktierten Hadamard-Codes**, die nicht zwingend nach „\(K(20,6)\)“ indexiert sind. Auch prüfen, ob eine bekannte Familie mit *kleinerer* Wortzahl oder *kleinerem* Radius eure Ungleichung impliziert. Dafür braucht es nicht dieselbe Matrix.
- **[U] Meldeform:** Nach euren Gates zuerst ein kleiner PR beim genannten Tabellenrepo, **falls** dessen Beitragsregeln Primärzertifikate zulassen; sonst ein technisches Issue dort nach euren Freigaberegeln. Inhalt: eingefrorener Tabellencommit und betroffene Zeilen, Formel für **eine** Matrix, feste Bitkonvention und Indizes, Wortliste mit Prüfsumme, zwei schlanke unabhängige Verifier, ihre exakten Ausgaben, obiger Familienbeweis, KI-Offenlegung. Formulierung: **„Explizites Zertifikat für eine obere Schranke 48, unter dem Wert im Tabellenstand `<commit>`; historische Priorität nicht beansprucht.“** Kein Spencer-PR ohne Rekord, keine Optimalitätsbehauptung \(K=48\), keine Forenposts oder Mails. Wenn eure interne Neuheitsregel die Meldung bei offener Literaturprüfung verbietet, bleibt das Paket zunächst intern.

## 4. Entscheidungsliste bis 04.01.2027

| Rang | Was ich freigeben würde [U] | Aufwand | Nutzen / Risiko |
|---|---|---|---|
| **1** | Genau eine Paley-Konstruktion einfrieren; Größe, Radius, eine normale Koordinate unabhängig verifizieren; Familienbeweis dazulegen. | ½–1 Arbeitstag | Höchster Nutzen. Beseitigt die Verwechslungsmöglichkeit zwischen gesuchtem und explizit erzeugtem Code. |
| **2** | Tabellencommit fixieren, alle abgeleiteten Zellen vergleichen, fehlende Primärliteratur und Beitragsregeln prüfen; am 27.11. über das **Tabellenpaket** entscheiden. | 1–2 Tage, bei fehlendem Volltext offen kennzeichnen | Reales Ergebnis nutzbar machen; Hauptrisiko ist ältere Bekanntheit, nicht die Spencer-Normierung. |
| **3** | Genau die Streichungstests \(20-2,\ 24-3\), anschließend nur den billigen Singleton-/Spaltenanbautest. | 1–2 h Rechenzeit plus Prüfaufwand | Schließt die sachlich auffälligsten Lücken. Kein stiller Übergang zur Vollsuche über größere Ordnungen. |
| **4** | Nur bei Interesse an einer neuen Methode **einen** der beiden Piloten wählen: Bahnen mit 50 Wörtern **oder** Schnittsiebung im gefalteten Produkt. | ½ Tag Entwicklung, höchstens 1 h Pilot | Erkenntnis über den Ansatz; hohe Wahrscheinlichkeit keines Rekords. Ohne bestandene Kontrolle stoppen. |
| **5** | Danach Suchstand archivieren. Wiedereröffnung bis 04.01. nur bei neuer Konstruktion, reproduzierbar besserer Kontrolle oder exakt überlebendem Kandidaten. | <½ Tag | Begrenzt Aufwand; akzeptiertes Risiko: Ein existierender Rekord bleibt unentdeckt. |

**[U] Kurz: „Aufhören und melden“ ist für die blinde Suche richtig. „Die Methode ist außerhalb dieser Zelle erledigt“ ist nicht belegt.** Ich würde die zwei übersehenen Streichparameter testen, aber die Meldung des mathematisch saubereren Resultats nicht vom Ausgang weiterer Rekordversuche abhängig machen.
