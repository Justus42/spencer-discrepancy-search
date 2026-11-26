> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-sol
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 26.11.2026, 18:06
> - Batch: Auftrag openrouter:batch-1795712113-Oi6Q8xNU4qFqNY5JXQal, abgegeben 26.11.2026, 17:55, fertig 26.11.2026, 18:04
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 10.432 · Ausgabe 25.430 (davon Denken 18.062) · gesamt 35.862
> - Kosten laut OpenRouter: 0,14 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: xhigh
> - finish_reason: stop

---

**Belegstatus:** „Nachgerechnet“ heißt im Folgenden algebraisch oder kombinatorisch hergeleitet. „Gelesen“ heißt: Die Zahl steht in der Notiz; ich habe weder die Eingabedateien noch den Programmlauf geprüft. Laufzeitangaben sind ungemessene Aufwandsschätzungen. Keine Websuche; die Literaturbehauptungen verwende ich nicht als Belege.

## 1. Sylvester 32: Der Faktor zwei ist falsch; die Schranke von acht freien Zeilen hält

**Nachgerechnet.** Für zwei gestrichene Spalten gibt es bei \(d=10\) **215 040 antipodale Lochklassen**, nicht 107 520. Die in 3c genannten Teilzahlen 53 760 und 376 320 zählen bereits Fortsetzungen mit festem Vorzeichen an der ersten gestrichenen Stelle. Man darf ihre Summe nur noch durch **zwei** teilen. Die \(0{,}040\,\%\) aus 3a stimmen mit \(215\,040/2^{29}\) überein, nicht mit \(107\,520/2^{29}\).

### Rahmenlemma und Paritäten

**Nachgerechnet.** Für eine punktierte Hadamard-Matrix \(H\) mit \(m\) Spalten gilt \(H^\top H=NI_m\). Deshalb ist die richtige Gleichung
\[
 \sum_{i=1}^N\langle w,x\circ h_i\rangle^2
 =N\|w\|_2^2\qquad(x\in\{\pm1\}^m).
\]
Das ist \(Nm\) für Vorzeichenzeilen und höchstens \(Nm\) für \(w\in[-1,1]^m\). Die Schranke \(\lfloor Nm/d^2\rfloor\) ist also auch für reelle freie Zeilen richtig. Sie zählt zunächst **indizierte** Punkte. Bei den beiden Sylvester-Punktierungen sind die 32 Punkte auch modulo Antipoden verschieden: Zwei verschiedene affine Wörter unterscheiden sich vor dem Streichen an mindestens 16 Stellen. Die Multiplikation mit einer Sylvester-Zeile permutiert die Zeilen bis auf Vorzeichen und erhält daher die Löcher. Das gilt nicht automatisch für andere Hadamard-Klassen.

**Nachgerechnet.** Schreibe \(F=(-1)^f\) und \(W_u=\sum_t F(t)(-1)^{u\cdot t}\). Für \(k=|\operatorname{supp} f|\) und \(v=\bigoplus_{t:f(t)=1}t\) gilt
\[
 W_u\equiv -2k+4u\cdot v\pmod 8,\qquad \sum_u W_u^2=1024.
\]
Bei geradem \(k\) ist also \(4\mid W_u\). Bei ungeradem \(k\) ist \(\max|W_u|\le8\) unmöglich: Alle Koeffizienten wären dann \(\pm2,\pm6\). Die Differenz \(W_u-4\,\operatorname{sgn}W_u\) wäre nach der Kongruenz \(2\) mal ein vorzeichenbehafteter Charakter. Beim Invertieren würde man also nur einen Wert von \(F\) um \(\pm2\) ändern. Das resultierende Quadratnormquadrat wäre mindestens 32, während das Spektrum \(4\,\operatorname{sgn}W_u\) Quadratnormquadrat \(32\cdot16/32=16\) liefert. Bei geradem \(k\) schließt Parseval \(\max|W_u|<8\) aus. Die unten gezählten Spektren mit Maximum 8 zeigen damit: Abstand 12 ist tatsächlich der tiefe Abstand, und jedes tiefe Loch ist gerade.

- **Eine Streichung, nachgerechnet:** Jedes punktierte Wort hat genau eine gerade Fortsetzung. Für diese gilt \(W_u=B_u+s\), wobei \(B_u\) die punktierte Korrelation und \(s=F(0)\) ist. Weil \(B_u\) ungerade ist,
  \[
  \max|B_u|<11\ \Longleftrightarrow\ \max|B_u|\le9
  \ \Longleftrightarrow\ \max|W_u|\le8.
  \]
  Die Zuordnung in 3c stimmt hier **bijektiv auf Wörtern**, nicht auf antipodalen Klassen ohne den Faktor zwei.
- **Zwei Streichungen, nachgerechnet:** Für \(0,b\) gilt \(W_u=B_u+s+t(-1)^{u\cdot b}\), mit \(s=F(0)\), \(t=F(b)\). Ein Loch hat \(\max|B_u|\le8\); folglich sind **beide** geraden Fortsetzungen tief, da deren Walsh-Werte durch 4 teilbar und höchstens 10 im Betrag sind. Umgekehrt ist \(B_u\) der Mittelwert der beiden Fortsetzungsspektren. Für eine einzelne tiefe Fortsetzung lautet die Zusatzbedingung: kein \(W_u=-8s\) auf \((-1)^{u\cdot b}=st\). „Kein \(-8\)“ stimmt also erst nach der Normierung \(s=+1\).

### Die Zählwerte, unabhängig von den Programmen

**Nachgerechnet.** Zur Zählung zerlege entlang einer von null verschiedenen Richtung \(v\). Auf den zwei Frequenzhälften ist \(W=2\widehat g_\pm\), wobei
\[
 g_\pm(z)=\tfrac12(F(z,0)\pm F(z,1)),\qquad z\in\mathbb F_2^4.
\]
Die beiden ternären Funktionen haben komplementäre Träger. Ist \(v\) der obige XOR-Vektor und \(v\ne0\), so hat bei einem tiefen Loch die eine Hälfte \(|\widehat g|=2\) überall, die andere \(|\widehat g|\in\{0,4\}\).

- **Spektrum \([8^{12}4^{16}0^4]\):** Die erste ternäre Funktion hat nach Parseval Trägergröße vier. Ihr Träger ist eine affine Ebene, und das Produkt der vier Vorzeichen ist \(-1\). Denn bei vier Punkten ohne affine Abhängigkeit könnte ein Charakter alle vier Summanden gleichrichten; bei einer Ebene verhindert genau das negative Produkt diese Gleichrichtung. Es gibt \(35\cdot4=140\) Ebenen und je acht Vorzeichenbelegungen. Auf den drei anderen Ebenennebenklassen muss die zweite Funktion aus drei affinen Vorzeichenfunktionen mit **verschiedenen** lokalen Frequenzen bestehen. Das gibt \(4\cdot3\cdot2\cdot2^3=192\) Möglichkeiten: Zwei gleiche affine Frequenzen können sich zu Betrag 8 addieren; eine oder drei nichtaffine Belegungen geben falsche Reste modulo 4; bei zwei nichtaffinen Belegungen können sich an der Frequenz der verbleibenden affinen Belegung \(4,2,2\) gleichrichten. Damit ergeben sich \(31\cdot2\cdot140\cdot8\cdot192/64=\mathbf{208\,320}\) RM-Nebenklassen.
- **Spektrum \([8^{16}0^{16}]\):** Zerlege stattdessen \(F\) in zwei vierdimensionale boolesche Schnitte mit Spektren \(U,V\). Weil \(U+V,U-V\in\{0,\pm8\}\), haben \(U,V\) dieselbe Betrags-4-Maske und disjunkte Betrags-8-Masken. Die gemeinsame Maske hat Größe 0, 8 oder 16. Bei Größe 0 ist jede Betrags-8-Maske eine affine Ebene mit acht zulässigen Belegungen. Zu einer Ebene gibt es \(3+18\cdot2=39\) disjunkte Ebenen: drei parallele, je zwei für die 18 Richtungen mit eindimensionalem Schnitt, keine für komplementäre Richtungen. Bei Größe 8 ist die gemeinsame Maske eine der 30 affinen Hyperebenen. Auf ihrer Ergänzung wählt man je zwei Betrags-8-Stellen, disjunkt; pro Wahl gibt es \(4\cdot8=32\) Belegungen, durch Invertieren auf den beiden Frequenzhälften: Die zwei Koeffizienten bestimmen eine vorzeichenbelegte affine Ebene im dreidimensionalen Raum, der komplementäre Viererträger hat acht Belegungen mit negativem Produkt. Bei Größe 16 sind beide Schnitte bent. Es gibt 896 bent Funktionen auf vier Variablen: Jede hat auf jeder affinen Hyperebene gerades Gewicht, wie aus \(W_0,W_u=\pm4\) folgt; damit verschwinden die kubischen und quartischen ANF-Koeffizienten. Die quadratische Form muss nichtausgeartet sein. Es gibt \(20160/(15\cdot8\cdot3\cdot2)=28\) nichtausgeartete alternierende Formen, durch Zählen symplektischer Basen, und je 32 affine Zusätze. Insgesamt also
  \[
  (140\cdot39\cdot8^2+30\binom82\binom62 32^2+896^2)/64
  =\mathbf{219\,604}.
  \]
  Die Summe \(427\,924\) und die daraus folgenden \(27\,387\,136\) Wörter stimmen.

**Nachgerechnet, auch für die zwei Streichungen.** Man kann den Normalisierungsfehler direkt an den beiden Spektraltypen festmachen:

| Spektrum der geraden Fortsetzung | Beide Fortsetzungen tief, ohne Fixierung von \(F(0)\) | Davon \(F(0)=+1\) |
|---|---:|---:|
| \([8^{16}0^{16}]\) | 107 520 | 53 760 |
| \([8^{12}4^{16}0^4]\) | 752 640 | 376 320 |
| Summe | 860 160 | 430 080 |

Für die erste Zeile zerlege entlang \(b\). Das Umdrehen beider gelöschter Zeichen kippt genau einen nichtnull ternären Eintrag. Die betroffene Frequenzhälfte muss danach überall \(|\widehat g|=2\) haben; ihr Träger ist also eine Viererebene durch 0. Vor dem Kippen sind auf ihr acht affine Vorzeichenbelegungen möglich. Das gibt \(2\cdot35\cdot8\cdot192=107\,520\). Kippen paart diese Fälle bijektiv mit den Fällen des zweiten Spektrums mit XOR-Vektor \(v=b\).

Für das zweite Spektrum mit \(v\ne b\), zerlege entlang \(v\). Im vierdimensionalen Quotienten seien \((p,q)\) die Koordinaten parallel und transversal zur Viererebene des ersten Trägers. In der anderen Frequenzhälfte gehört jede der drei aktiven lokalen Frequenzen zu genau einer Transversalklasse \(q\). Die Zwei-Punkte-Korrektur ist in dieser Hälfte auf den Frequenzen nichtnull, auf denen die beiden Beiträge gleiches Vorzeichen haben. Hat \(b\) eine von null verschiedene Transversalkoordinate \(q_b\), ist das bei jeder lokalen Frequenz eine affine Gerade in der transversalen Frequenzebene. Das Vorzeichen des alten Betrags-4-Koeffizienten muss darauf konstant sein, sonst entsteht irgendwo Betrag 6. Dafür müssten alle drei aktiven Transversalklassen in \(\{0,q_b\}\) liegen: unmöglich. Ist \(q_b=0\), müssen von den zwei lokalen Frequenzen mit nichtnull Korrektur genau eine fehlen und die andere zur Klasse \(q=0\) gehören. Dort passt das Vorzeichen automatisch, weil beide gekippten Punkte in dieser Klasse liegen. Also: sieben Ebenenrichtungen durch die Quotientenrichtung von \(b\), drei affine Ebenen davon nicht durch 0, acht Vorzeichenbelegungen auf der Ebene, \(4\cdot2\cdot8=64\) auf dem Komplement und zwei Frequenzhälften. Das gibt \(30\cdot7\cdot3\cdot8\cdot64\cdot2=645\,120\) zusätzliche Fortsetzungen.

Ohne Normierung hat jede antipodale Lochklasse vier Fortsetzungen, **mit** \(F(0)=+1\) zwei. Beide Spalten der Tabelle geben somit 215 040, nicht 107 520.

### Die 13 888 und die freien Zeilen

**Nachgerechnet.** Für eine gerade Funktion setze \(a_u=W_u/4\). Es gilt \(\sum a_u^2=64\), und die Anzahl ungerader \(a_u\) ist \(0,16\) oder \(32\). Gezählt werden Stellen mit \(\boldsymbol{|W_u|\ge12}\), nicht bloß \(W_u\ge12\).

- Sind alle \(a_u\) gerade oder alle ungerade, gibt es höchstens vier Treffer. Im ersten Fall kostet jeder mindestens 16 Quadratsummeneinheiten, im zweiten mindestens acht zusätzlich zur Grundsumme 32.
- Bei 16 ungeraden Stellen bleiben nach der Grundsumme 16 noch 48 Einheiten. Ein ungerader Treffer kostet mindestens acht zusätzlich, ein gerader mindestens 16. Sechs Treffer sind nur mit sechs ungeraden Beträgen 3 und einer identisch nullen geraden Frequenzhälfte möglich. Für genau fünf Treffer blieben nur zwei Muster auf der geraden Hälfte von \(W\): einmal Betrag 16, sonst null; oder zweimal Betrag 8, sonst null. Beide sind unmöglich: Die inverse vierdimensionale Transformation von \(W/2\) auf dieser Hälfte hätte mindestens einen nichtganzzahligen Wert, statt in \(\{0,\pm1\}\) zu liegen.
- Bei sechs Treffern ist \(F\) daher entlang \(v\ne0\) eigen mit Vorzeichen \(\pm1\). Auf dem Quotienten steht eine ungerade vierdimensionale Funktion mit Walsh-Beträgen 2 und 6. Solche Funktionen sind genau die Einpunktnachbarn der bent Funktionen: Wie oben ist \(W-4\operatorname{sgn}W\) zweimal ein vorzeichenbehafteter Charakter. Die Inversion ändert einen Punkt; jetzt erzwingt Parseval, dass er gekippt wird und kein Betrag 3 entsteht. Umgekehrt erzeugt jeder Einpunktwechsel das verlangte Spektrum, und der bent Nachbar ist durch \(4\operatorname{sgn}W\) eindeutig. Somit gibt es \(16\cdot896\) Quotientenfunktionen und
  \[
  31\cdot2\cdot16\cdot896/64=\mathbf{13\,888}
  \]
  außergewöhnliche RM-Nebenklassen.

**Nachgerechnet.** Für eine **binäre** freie Zeile \(w\) und eine Lochklasse \(xC\) ist die gerade Fortsetzung des Produkts das Produkt der geraden Fortsetzungen. Die 32 zu prüfenden Korrelationen sind \(W_u-s\); bei \(4\mid W_u\) trifft \(|W_u-s|\ge11\) genau dann zu, wenn \(|W_u|\ge12\). Multiplikation mit \(w\) permutiert die RM-Nebenklassen. Also kann dieselbe freie Zeile auf höchstens 13 888 Lochnebenklassen sechs Punkte treffen, auf allen anderen höchstens vier. Bei sieben Zeilen braucht jede Lochnebenklasse mindestens zwei Sechsertreffer, da \(7\cdot4+2=30<32\). Aber \(7\cdot13\,888=97\,216<2\cdot427\,924\). **Mindestens acht binäre freie Zeilen** ist richtig.

Drei Grenzen des Schlusses:

1. **Nachgerechnet:** Acht zusätzliche Zeilen verletzen das Budget 38 nur, wenn alle 32 Ausgangszeilen stehen bleiben. Mit Streichungen bleiben die alten Löcher zwar offen, aber acht freie Zeilen bedeuten dann zunächst nur: mindestens zwei alte Zeilen müssen weg. Der Schluss schließt solche Mischbauten nicht aus.
2. **Gelesen:** Die 7,1 % stammen laut Notiz aus einer Stichprobe. Daraus folgen weder „höchstens 7,1 %“ noch „mindestens 15 freie Zeilen“. Für reelle freie Zeilen gilt das Rahmenlemma, aber die Sechs-oder-vier-Zählung ist kein Beweis: Sie benutzt die binäre gerade Fortsetzung und deren Kongruenzen.
3. **Nachgerechnet:** Bei zwei Streichungen liefert das Rahmenlemma weiterhin \(\lfloor32\cdot30/10^2\rfloor=9\) Punkte je freie Zeile und damit mindestens vier freie und mindestens fünf entfernte Zeilen beim Budget 31. „Bestenfalls Gleichstand mit 64 Wörtern“ folgt daraus nicht.

## 2. Amalgamierte Summen: 3e schließt nicht einmal alle normalen binären Faktoren aus

**Nachgerechnet.** Der richtige quadratische Nenner für einen Faktor mit \(m_A\) Spalten und \(N_A\) Zeilen ist nach Auffüllen \(\sqrt{\max(m_A,N_A)}\). „Der Faktor ist kein Rekord“ impliziert bei \(m_A>N_A\) also **nicht** \(d_A\le(5/\sqrt8)\sqrt{N_A}\). Beim Produkt ist entsprechend \(\max(N_AN_B,m_A+m_B-1)\) die relevante Größe. Eine universelle Schranke allein in \(N_A\) wäre ein zusätzlicher zu beweisender Satz.

**Nachgerechnet.** Der zweite Fehler ist das Gleichheitszeichen. Beim üblichen Amalgamieren werden Wörter mit gleichem Bit an der gemeinsamen Koordinate gepaart. Normalität liefert \(R\le R_A+R_B\): Die beiden zum Prüfwort möglichen Summen der bedingten Abstände addieren sich, nach einmaligem Abzug für die doppelt gezählte gemeinsame Stelle, zu höchstens \(2(R_A+R_B)+1\). Somit folgt
\[
d\ \boldsymbol{\ge}\ d_A+d_B-1,
\]
nicht Gleichheit. Eine untere Konstruktionsschranke darf man nicht als obere Ausschlussschranke einsetzen. Gegenbeispiel: Beide Faktoren seien \(\mathbb F_2\times\{00,11\}\), mit einer Wiederholungskoordinate als gemeinsamer Stelle. Sie sind dort normal, haben Länge 3, Radius 1 und \(d_A=d_B=1\). Das Amalgam ist \(\mathbb F_2^2\times\{000,111\}\), Länge 5, Radius 1, also \(d=3\), nicht 1. Mit einem einzeiligen Faktor gerader Wiederholungslänge kann sich \(d\) ebenfalls ändern. Die ungerade Wiederholung hält \(d\) fest: Normalität liefert die untere Schranke, und für die obere kann man den wiederholten Vorzeichenblock auf Summen \(\pm1\) setzen.

**Nachgerechnet.** *Unter* den in 3e eingesetzten Zusatzannahmen und *für den Wert* \(d_A+d_B-1\) stimmt die Rechnung nach Ersetzen der gerundeten Konstante: Die Grenze ist \(1-\sqrt8/5=0{,}434314\ldots\). Für \(2\le N_A\le N_B\) bleiben \((2,2),(2,3),(2,4)\), mit den angegebenen Quotienten als **Obergrenzen für diesen Wert**, nicht als tatsächlich erreichte Quotienten der Amalgamen.

**Nachgerechnet.** Zeilenzahlen müssen nicht multipliziert werden. Blockdiagonal erhält man \(N_A+N_B\) Zeilen, allerdings nur \(\max(d_A,d_B)\). Eine addierende Variante unter einer stärkeren Hypothese: Wähle Kanten \(ij\) eines bipartiten Graphen und setze je Kante die beiden Zeilen \((a_i,b_j),(a_i,-b_j)\). Wenn für jedes Paar von Färbungen eine Kante zwei bei ihren jeweiligen Schwellen treffende Zeilen verbindet, ist die Diskrepanz mindestens \(d_A+d_B\). Treffen bei jeder Färbung Anteile mindestens \(p_A,p_B>0\), genügen per Vereinigungsabschätzung schon mehr als \((m_A+m_B)\log 2/(p_Ap_B)\) zufällig gewählte Kanten für einen Existenzbeweis; die Zeilenzahl ist höchstens doppelt so groß. Ohne Mehrfachdeckung trägt das Argument nicht: Haben beide Faktoren für jede Zeile eine Färbung, die nur diese Zeile trifft, braucht dieses Zertifikat das volle Produkt.

## 3. Eine Ersatzspalte: Der Boolesche Ausschluss überträgt sich hier auf reelle Ersatzspalten

**Nachgerechnet.** Bei Ordnung 20 ist die Rekordschwelle \(\tau=5\sqrt{20/8}=7{,}90569\ldots\). Für festes \(x'\) sind die 19-spaltigen Korrelationen \(b_i\) ungerade. Gibt es ein \(|b_i|\ge9\), hält diese Zeile für beide Vorzeichen der Ersatzkoordinate mindestens 8. Sonst können nur Zeilen mit \(|b_i|=7\) die Schwelle schlagen, und zwar genau dann, wenn
\[
 \operatorname{sgn}(b_i)\,x_{20}v_i>\tau-7.
\]
Für das Ziel \(\operatorname{disc}\ge8\) muss das Produkt sogar \(1\) sein. In beiden Fällen bewahrt Runden von \(v_i\in[-1,1]\) auf sein Vorzeichen alle nötigen Treffer. Das Boolesche Verbot ist also **für die Existenzfrage exakt**, auch für den strikten reellen Rekord. Es gilt nicht als allgemeine Regel für reelle Einträge in den *übrigen* Spalten; dort fehlt die Paritätslücke.

**Nachgerechnet unter einer gelesenen Prämisse.** Wenn tatsächlich alle 1140 Tripel als kritische Zeilenmengen auftreten, ist die Suche über \(2^{19}\) Ersatzspalten überflüssig. Normiere die entfernte Spalte durch Zeilenvorzeichen auf \(+\). Die ursprünglichen Zeilen unterscheiden sich paarweise an zehn Stellen; die übrigen 19 Einträge haben daher dasselbe Zeilenprodukt. Für festes \(x'\) sind alle \(b_i\) gleich modulo 4, und alle kritischen Beträge 7 haben somit dasselbe Vorzeichen. Jedes der 1140 Tripel muss in der Ersatzspalte beide Zeichen enthalten. Das ist unmöglich. Ich habe das Auftreten aller Tripel **nur gelesen**, nicht über die 120 Eingaben nachgeprüft.

## 4. Aus 3a folgt keine Grenze für Hadamard-nahe Matrizen

**Nachgerechnet als Reichweitenprüfung; Tabellenwerte gelesen.** Die erschöpfenden Rechnungen an bestimmten Ordnungen betreffen bestimmte **Punktierungen**, nicht die Umgebung einer Hadamard-Matrix. Es fehlt vor allem die Familie **gemischter Spaltenstörungen**: mehrere Ersatzspalten, auch reell, kombiniert mit Punktierung, Rand oder ausgetauschten Zeilen. Eine neue Spalte kann alte flache Färbungen gerade dort anheben, wo bloßes Streichen keine neue Deckung erzeugt; zugleich muss man die neu entstehenden flachen Färbungen prüfen. Insbesondere erfasst eine vollständige Liste von Punktierungen keine solche Nachbarschaft. Schon innerhalb reiner Punktierungen sind Ordnung 28 und die sieben Fälle bei 32 keine Grundlage für „ab \(N=20\)“. Auch die behaupteten Abschlüsse über „geschlossene Zellen“ bei größeren Streichzahlen lassen sich aus der abgedruckten, überwiegend strukturellen Karte allein nicht rekonstruieren. Das Literaturargument zur Ergänzbarkeit partieller Hadamard-Matrizen bleibt unbelegt.

## 5. Die vier ersten Tests

### Idee 1 — Kern \(\langle1,a,b\rangle\)

**Nachgerechnet.** „\(K=8\) muss herauskommen“ ist eine falsche Positivkontrolle. Ein konkreter linearer Code der Dimension 6, der die Eins und den gewählten Kern enthält, zertifiziert für **seinen** Typ \(K\le8\), nicht \(K_{\min}=8\), und nichts für die anderen Typen. Prüfe das konkrete Acht-Nebenklassen-Zertifikat. Ein Solver, der ein echtes \(K=7\) findet, hätte die vorgeschlagene Kontrolle sonst „nicht bestanden“.

**Nachgerechnet.** Es gibt für \(m=16,18,20\) jeweils 55, 74 und 97 Blocktypen: Partitionen in drei oder vier positive Blockgrößen, da die affine Gruppe auf den vier Typen alle Permutationen realisiert. Schon für \(m=16\) bleiben nach Festlegen einer Nebenklasse als Null im naiven Test \(\binom{8191}{4}\approx1{,}87\cdot10^{14}\) Kandidaten je Typ. **Aufwandsschätzung:** „In 30 Minuten das Minimum für alle Typen“ ist ohne starke weitere Reduktion keine belastbare Zusage.

**Nachgerechnet: billigerer notwendiger Test.** Teile die Prüfpunktmenge nach den Paritäten auf den drei oder vier Blöcken. Für jedes Paar aus Punktparität \(p\) und Repräsentantenparität \(q\) berechne die bedeckte Anzahl \(M_{pq}\) per Binomialzählung der Blockabstände. Die maximale Korrelation ist die Summe der Blockbeträge bei drei Blöcken; bei vier Blöcken und falschem Vorzeichenprodukt verliert man zweimal den kleinsten Betrag. Suche dann nur nichtnegative ganze Belegungszahlen \(k_q\) mit Summe 5 und \(\sum_q M_{pq}k_q\ge|\text{Faser }p|\) für jede Faser. Eine nicht erfüllbare Relaxation erledigt den Typ, eine erfüllbare deckt noch nichts. Ob sie alle Typen erledigt, habe ich nicht berechnet. **Gelesen:** Die Zählschranke in der Karte ist kein Ausschluss; der Zwei-Bahnen-Eintrag deckt diese Fünf-Nebenklassen-Familie nicht als solche ab.

### Idee 2 — Zwei Ersatzspalten

**Nachgerechnet.** Hier gibt es einen billigeren exakten Ansatz als den generischen Verzweigungslöser. Normiere eine gestrichene Spalte auf \(+\). Die andere zerlegt die Zeilen in zwei Zehnergruppen. Wegen der Zeilenprodukte sind für jedes \(x''\) die Korrelationen in einer Gruppe \(0\bmod4\), in der anderen \(2\bmod4\). Bei einem flachen \(x''\) mit \(\max|b_i|\le6\) liegen also sämtliche kritischen Zeilen \(|b_i|=6\) in **einer** Gruppe. Es sind mindestens fünf: Sind \(q\) Beträge 6 und \(j\) Beträge 4, liefert Parseval \(360=40+32q+16j\), \(j\le10\). Der Schnelltest „weniger als vier kritische Zeilen“ kann daher nie zuschlagen.

Gib jeder Zeile den Wert \((v_i,w_i)\in\{\pm1\}^2\). In jeder kritischen Menge müssen die **signierten** Werte \(\operatorname{sgn}(b_i)(v_i,w_i)\) alle vier Möglichkeiten enthalten. Zuerst sind daher nur die Produkte \(v_iw_i\) zu prüfen: mindestens zwei kritische Zeilen jedes Produkts. Das sind höchstens \(2^9\) Produktbelegungen pro Zehnergruppe modulo globalem Wechsel; für Überlebende bleiben getrennte Vorzeichenbedingungen. Die beiden Zehnergruppen entkoppeln. Dieselbe Rundung wie bei einer Ersatzspalte gilt hier: Bei \(|b_i|=6\) müssen beide Ersatzbeiträge positiv sein und jeder muss größer als \(\tau-7\) sein; bei \(|b_i|\ge10\) ist der Treffer ohnehin sicher.

**Gelesen / Aufwandsschätzung.** Die \(k=1\)-Kontrolle ist negativ und testet nicht den neuen Vierfarbenfall. Ein einziger vollständig gegengeprüfter kleiner \(k=2\)-Fall mit erfüllbaren und unerfüllbaren Schwellen ist sinnvoller. Erst einen Ordnung-20-Fall mit dem Produktfilter messen, nicht alle Klassen in 30 Minuten versprechen. Kein Eintrag der Karte schließt \(k=2\) allgemein aus. Für \(k\ge3\) ist die Übertragung von binären Negativbefunden auf reelle Ersatzspalten nicht durch dieselbe Rundung bewiesen.

### Idee 3 — Freie Suche in \((25,8)\)

**Nachgerechnet.** Die Dichte stimmt: \(V(25,8)=1\,807\,781\), also \(50V/2^{25}=2{,}6938\ldots\). Die Volumenschranke erledigt die Idee nicht. „Keine Hadamard-Matrix der Ordnung 25“ erledigt auch kein Suchbecken: Ein Code auf 25 Koordinaten kann von einer anderen Ordnung durch Rand oder Punktierung abstammen. Der Zwei-Bahnen-Ausschluss ist kein Ausschluss freier Suche.

**Gelesen / Aufwandsschätzung.** Zwei blinde 64er aus vier Starts sind eine Heuristikkontrolle, kein billiger Funktionstest und kein Test der eigentlichen Hürde von 64 auf 50. Ein bekannter 64er gehört zwar zur freien Suchstruktur, liefert aber keinen positiven Kontrollfund nahe dem Zielbudget. Billiger: den vorhandenen 64er exakt prüfen, ein antipodales Paar entfernen, die neu offenen Klassen zählen und eine lokale Reparatur mit festem Auswertungsbudget versuchen. Allein die Vollprüfung läuft über \(2^{24}\) antipodale Klassen; vier Blindläufe haben aus der Notiz keine belastbare 30-Minuten-Grenze. Einen sofortigen mathematischen Ausschluss der freien Familie habe ich nicht.

### Idee 4 — Abschlussrechnungen

**Nachgerechnet.** Bei Ordnung 28 sind allein für gerade \(c\le6\) pro Matrix \(1+378+20\,475+376\,740=397\,594\) Masken möglich, bei 487 Klassen samt Transponierten vor Symmetriereduktion rund **387 Millionen**. „Minuten, sobald die Dateien da sind“ folgt weder aus der Fallzahl noch aus einer Paley-Messung. Bei Ordnung 24 stehen für \(c=3,5\) samt Transponierten \(242\,880+5\,100\,480\) Masken an. Die Stichproben mit null Treffern sind kein Ausschluss. Und \(c\le6\) allein liefert keine geschlossene Tabelle für Ordnung 28.

**Gelesen / Aufwandsschätzung.** Die angegebenen Positivkontrollen prüfen Teilverfahren; sie prüfen weder die Klassenvollständigkeit noch Laufzeit oder Korrektheit der noch offenen Maskenfälle. Bei der Kontrolle für eine freie Zeile ist die Aufgabe dieselbe Art Überdeckungstest, aber nicht dieselbe Hadamard-Ordnung und Punktierung. Der billigste Gegentest ist bereits erfolgt: Die Faktoren zwei in 3c widersprechen der Notiz. Für die verbleibenden Rechnungen zuerst einige feste Masken doppelt prüfen und den Durchsatz messen. Die Karte schließt die offenen Teile nicht aus; die dort schon als „voll“ gelisteten Teile erneut zu rechnen ist Replikation, keine neue Richtung.

## 6. Drei engere Richtungen mit kurzem Ersttest

Die Zeiten sind **ungemessene Zeitbudgets für den Ersttest**, keine Zusagen für einen vollständigen Ausschluss.

1. **Zwei Ersatzspalten bei Ordnung 20, mit Zehnergruppen statt generischem Solver.** Für ein Spaltenpaar alle flachen \(x''\) sammeln, kritische Mengen deduplizieren und die \(2^9\) Produktbelegungen je Gruppe testen. **Unter 30 Minuten als Pilot**: null überlebende Produkte ist bereits ein exakter Negativbefund für das Paar, auch reell. Andernfalls erst die verbleibenden Vorzeichenbedingungen bearbeiten.
2. **Kern der Dimension 3: Paritätsfasern vor exakter Suche.** Bei \(m=16\) für die 55 Typen nur die kleinen ganzzahligen Faserbedingungen mit Budget fünf prüfen, dazu einen konkreten bekannten linearen Zeugen im passenden Typ validieren. **Unter 30 Minuten als Filtertest**; nur überlebende Typen rechtfertigen eine echte Mengenüberdeckungssuche.
3. **Zeilenabhängige Nullstellen bei Ordnung 20 statt weiterer globaler Punktierungen.** Der Ausgangswert \(\operatorname{disc}(H)=6\) ist **gelesen**. **Nachgerechnet:** Eine Null pro Zeile kann einen vorhandenen 6-Zeugen nur auf 7 anheben; zwei Nullen sind der erste nicht sofort ausgeschlossene Fall. Für einen 6-Zeugen kann eine Zeile die Rekordschwelle nur schlagen, wenn \(|b_i|=6\) und **beide** genullten Summanden gegen das Vorzeichen von \(b_i\) standen. **Unter 30 Minuten als Filtertest:** Für eine feste \(H\) einige hundert 6-Zeugen sammeln, je Zeile die 190 Nullpaare als Domäne nehmen und nur diese notwendigen Zeugenbedingungen prüfen. Unerfüllbarkeit erledigt diese \(H\) in der ganzen Zwei-Nullen-Nachbarschaft; Erfüllbarkeit ist noch kein Kandidat. Der Nullstellen-Ausschluss der Karte betrifft nicht diese Ordnung und Familie.
