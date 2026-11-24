> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-astra
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 24.11.2026, 18:32
> - Batch: Auftrag openrouter:batch-1795539884-0bblL4PyPRGQzb2oYzYv, abgegeben 24.11.2026, 18:04, fertig 24.11.2026, 18:09
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 24.477 · Ausgabe 17.225 (davon Denken 10.573) · gesamt 41.702
> - Kosten laut OpenRouter: 0,44 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: high
> - finish_reason: stop

---

[U] **Zuerst würde ich Nr. 1 (Paley-Scharnier, \(23/7\)) und Nr. 2 (Duaden, \(15/4\)) sieben.** Bei Nr. 1 startet man mit Diskrepanz 8 und kann genau angeben, welche Randpunkte die zusätzliche Spalte reparieren muss. Bei Nr. 2 entscheidet im mittleren Gewicht eine konkrete Bedingung an einem Graphen mit sechs Knoten; dafür braucht man nicht einmal den allgemeinen Prüfstand. Einen neuen Ausschluss für eine **ganze** offene Zelle kenne ich nicht.

[R] Ein nützlicher **Teilausschluss vorweg**: Aus `construct_32.py` bei Länge 15 kann man nicht zwei Zeilen entfernen und durch eine einzige neue Zeile ersetzen, wenn die anderen 14 unverändert bleiben. Die 16 Ausgangszeilen sind eine um eine Spalte punktierte Hadamard-Matrix: Innerhalb jeder Achtergruppe folgt die Orthogonalität vor Punktierung aus Walsh-Orthogonalität, zwischen den Gruppen aus der entgegengesetzten zweiten Hälfte. Der projektive Abstand \(\delta(u,v)=\min(d_H(u,v),15-d_H(u,v))\) zweier alter Zeilen ist also 7. Um jedes der beiden entfernten Zentren liegt eine **ganze 2-Kugel**, die keine der unveränderten Zeilen samt Komplement mit Radius 4 trifft. Um eine solche 2-Kugel allein zu decken, muss das neue Paar projektiven Abstand höchstens 2 von ihrem Zentrum haben: Die beiden komplementären 4-Kugeln liegen getrennt, und eine einzelne 4-Kugel enthält die 2-Kugel nur bei Zentrenabstand \(\le2\). Für beide entfernten Zentren zugleich widerspricht das \(7>2+2\). Nr. 2 ist deshalb kein lokaler Zweier-Merge.

[R] **Lesart der Angaben:** Wortzahl zählt die Komplemente mit. Der Quotient ist die garantierte Untergrenze **bei Erfolg**, kein gemessenes Ergebnis. Die Tabellen-Untergrenzen verwende ich als Eingabedaten aus dem Auftrag. Die acht Codeblöcke sind unabhängig; jeweils nur einen als Modul verwenden. [U] Die Erzeugungszeiten schätze ich für normales CPython unter 60 s je Familie, ohne Sieben; ich habe sie nicht gemessen. Die Chancen beziehen sich auf mindestens einen Treffer **im angegebenen Generator**, nicht auf die Existenz in der Zelle.

### 1. Paley-Scharnier: eine neue Spalte, die je Zeilenpaar den Block wechseln darf

1. [R] **\((23,7)\), 50 Wörter, \(25\times23\), \(d=9\), Quotient \(1{,}8000\).**
2. [R] \(50\cdot390656/2^{23}=2{,}3285\); \(50\ge29\).
3. [R] Ausgangspunkt ist die im Auftrag bestandene **1+1-Kontrolle mit verschiedenen Streichspalten**, also \(U,V\) mit je elf Spalten und \(\min_{x,y}\max_i(|u_i|+|v_i|)=8\). Die neue Spalte hat im Paar \(i\) entweder Einträge \((s_i,s_i)\) oder \((s_i,-s_i)\). Für ihr Vorzeichen \(t\) wird der Paarwert damit exakt
   \[
   |u_i+s_it|+|v_i|\quad\text{oder}\quad |u_i|+|v_i+s_it|.
   \]
   **Alle alten Werte ab 10 sind automatisch sicher.** Auf der 8-Schale sind \(u_i,v_i\) ungerade, also nie null: Man braucht nur ein enges Paar, bei dem das Scharnier den Betrag vergrößert, oder die 25. Zeile. Als Schalter und Vorzeichen dienen Paley-Spalten bzw. Spaltenprodukte, als Reparaturzeile Produkte von Paley-Zeilen. Das ist keine Spaltenstreichung aus Hadamard 24.
4. [R] Generator, 4 356 Ausgaben:
```python
def kandidaten():
    import numpy as np
    from itertools import combinations
    sq = {i*i % 11 for i in range(1, 11)}
    H = np.eye(12, dtype=int)
    H[0, 1:] = 1
    H[1:, 0] = -1
    for i in range(11):
        for j in range(11):
            if i != j:
                H[i+1, j+1] = 1 if (i-j) % 11 in sq else -1
    U, V = H[:, 1:], np.delete(H, 1, axis=1)
    core = np.block([[U, V], [U, -V]])
    for a, b in combinations(range(12), 2):
        switch = H[:, a] * H[:, b]
        for c in range(1, 12):
            s = H[:, c]
            base = np.column_stack((core, np.r_[s, s*switch]))
            for j in (1, 2, 4):
                for e in (-1, 1):
                    r = np.r_[U[0]*U[j], e*V[j]*V[(j+c) % 12], 1]
                    yield f"hinge-{a}-{b}-{c}-{j}-{e}", np.vstack((base,r)).tolist()
```
5. [R] **Gegentest:** Zeugen der alten 8-Schale wiederverwenden, beide \(t\) testen. Sobald alle engen Paare schrumpfen und die neue Zeile Betrag \(\le7\) hat, ist der Kandidat tot. Derselbe alte Zeuge ist für sämtliche 4 356 Kandidaten verwendbar.
6. [U] **Chance: etwa \(0{,}3\,\%\).** Der Vorteil gegenüber einer freien Verlängerung ist die exakte Beschränkung auf eine einzige alte Schale; das Risiko sind Punkte mit nur einem engen Paar.

### 2. Duaden: eine Einsfaktorisierung und zehn unabhängig wählbare Schnitte

1. [R] **\((15,4)\), 30 Wörter, \(15\times15\), \(d=7\), Quotient \(7/\sqrt{15}=1{,}8074\).**
2. [R] \(30\cdot1941/2^{15}=1{,}7770\); \(30\ge23\).
3. [R] Koordinaten sind die Kanten von \(K_6\). Fünf Zeilen sind perfekte Matchings einer Einsfaktorisierung. Für jeden der zehn unorientierten \(3|3\)-Schnitte gibt es drei weitere Zeilenoptionen: die zwei inneren Dreiecke als eine 6-Menge oder einen von zwei 6-Zyklen im Schnitt. Genau zwei Faktoren kreuzen den Schnitt dreifach: Die fünf Kreuzungszahlen sind ungerade und summieren sich zu 9. **Mechanismus im schwierig zu deckenden mittleren Gewicht:** Ein 7-Kanten-Graph wird getroffen, wenn er einen gewählten Faktor enthält, oder wenn er mit einer gewählten 6-Menge mindestens fünf oder höchstens eine Kante gemeinsam hat. Das folgt exakt aus den Abständen \(7+k-2|X\cap B|\). Die Optionen tauschen also bei gleichem Zeilenvolumen unterschiedliche Klassen von Graphen aus. Die zehn Entscheidungen müssen keine 5er-Symmetrie respektieren.
4. [R] Generator, \(3^{10}=59\,049\) Ausgaben:
```python
def kandidaten():
    from itertools import combinations, product
    E = list(combinations(range(6), 2))
    def row(B):
        return [-1 if e in B else 1 for e in E]
    F = []
    for i in range(5):
        F.append({(i, 5)} | {tuple(sorted(((i-j) % 5, (i+j) % 5)))
                               for j in (1, 2)})
    fixed = list(map(row, F))
    choices = []
    for a, b in combinations(range(1, 6), 2):
        S = {0, a, b}
        cut = {e for e in E if len(set(e) & S) == 1}
        choices.append([row(set(E)-cut)] + [row(cut-f) for f in F if f <= cut])
    for k, ix in enumerate(product(range(3), repeat=10)):
        yield f"duad-{k}", fixed + [c[i] for c, i in zip(choices, ix)]
```
5. [R] **Gegentest:** Nur die \(\binom{15}{7}=6435\) Graphen mit sieben Kanten nehmen, dabei alle mit ganzem Faktor vorab streichen. Für jede der 30 optionalen Zeilen genau einmal ihre Treffermenge als Bitmaske anlegen. Dann sind es pro Kandidat zehn bitweise ODER, um die ganze Gewichtsschicht zu prüfen. Ein fehlendes Bit ist ein exakter Gegenzeuge.
6. [U] **Chance: etwa \(0{,}1\,\%\).** Besonders günstig ist hier der Preis eines vollständigen negativen Befunds.

### 3. Drei Fano-Blöcke mit vier Summenzeilen

1. [R] **\((21,6)\), 50 Wörter, \(25\times21\), \(d=9\), Quotient \(1{,}8000\).**
2. [R] \(50\cdot82160/2^{21}=1{,}9588\); \(50\ge34\).
3. [R] Drei unabhängige 7-Bit-Blöcke. Vier feste Zeilen realisieren **exakt** \(|S_0|+|S_1|+|S_2|\) als Maximum, wobei \(S_j\) die Blocksumme ist. Die übrigen Zeilen verwenden die sieben Verschiebungen des Fano-Musters mit Minusmenge \(\{0,1,3\}\). Diese sieben Muster zusammen mit der konstanten Zeile und allen Komplementen bilden einen perfekten Radius-1-Code: Die 16 Zentren haben Mindestabstand 3 und \(16(1+7)=128\). Jeder Block liefert daher einen Betrag \(\ge5\), entweder als Summe oder in einem Fano-Kanal. Drei verschiedene Zuordnungen der Kanäle sollen zu einzelnen Spitzen Beiträge aus den Nachbarblöcken addieren. **Nicht** behauptet wird, dass drei einzeln garantierte Spitzen gleichzeitig auftreten.
4. [R] Generator, 1 176 Ausgaben:
```python
def kandidaten():
    from itertools import product
    f = [-1 if j in (0, 1, 3) else 1 for j in range(7)]
    fixed = [[1]*7 + [s]*7 + [t]*7 for s, t in product((-1,1), repeat=2)]
    for a, b, h, k in product(range(1,7), range(7), range(7), range(4)):
        rows = fixed[:]
        for r in range(3):
            shifts = (0, r*a, (0,b,h)[r])
            signs = (1, -1 if r == 1 else 1, -1 if (k >> (r % 2)) & 1 else 1)
            for i in range(7):
                rows.append([signs[c]*f[(j-i-shifts[c]) % 7]
                             for c in range(3) for j in range(7)])
        # Identische projektive Zeilen nicht als zusätzliche Wörter zählen.
        if len({tuple(v*z[0] for v in z) for z in rows}) == 25:
            yield f"fano-{a}-{b}-{h}-{k}", rows
```
[R] Die 1 176 sind hier eine **Obergrenze** vor dem Duplikatfilter.
5. [R] **Gegentest:** Jeden Block gleich einer nichtkonstanten Fano-Zeile oder ihrem Negativen setzen. Das sind nur \(14^3\) Tripel, und die vier Summenzeilen erreichen darauf höchstens 3. Die Fano-Korrelationen pro Block haben genau eine Spitze vom Betrag 7, sonst Betrag 1. Schon die Kanalindizes und Vorzeichen genügen zur Prüfung.
6. [U] **Chance: etwa \(0{,}05\,\%\).** Der Test auf drei isolierte Spitzen dürfte die Familie stark verkleinern.

### 4. Vier quadratische Walsh-Charts und ein fünfter Chart mit vier Randspalten

1. [R] **\((20,6)\), 40 Wörter, \(20\times20\), \(d=8\), Quotient \(1{,}7889\).**
2. [R] \(40\cdot60460/2^{20}=2{,}3064\); \(40\ge24\).
3. [R] Die ersten 16 Koordinaten sind \((x,y)\in\mathbb F_4^2\). Chart \(t\) hat vier Phasen \(\operatorname{Tr}(txy+a(x+ty))\), \(a\in\mathbb F_4\). Innerhalb eines Charts sind die Zeilen orthogonal. Zwischen verschiedenen endlichen Charts haben sie Skalarprodukt \(\pm4\): In der Summe über \(x,y\) verschwindet die innere Charaktersumme über \(x\) außer bei genau einem \(y\). Der fünfte Chart verwendet eine andere quadratische Phase. Die letzten vier Spalten liefern **in jedem Chart** entweder eine Korrektur vom Betrag 4 an genau einem Platz oder vier Korrekturen vom Betrag 2; das sind die beiden Spektraltypen eines Vorzeichenvektors am Walsh-System der Ordnung 4. Die freien Parameter legen fest, *wo* und mit welchem Vorzeichen korrigiert wird. Es wird nicht einfach ein vollständiger RM-Code um vier Zeilen ergänzt.
4. [R] Generator, 1 024 Ausgaben:
```python
def kandidaten():
    from itertools import product
    def mul(a,b):
        return ((a if b & 1 else 0) ^
                ((((a << 1) ^ (7 if a & 2 else 0))) if b & 2 else 0))
    def tr(a):
        return a >> 1
    def sg(i):
        return 1-2*(i & 1)
    core = []
    for t, a in product(range(5), range(4)):
        w = []
        for x,y in product(range(4), repeat=2):
            if t < 4:
                z = tr(mul(t,mul(x,y)) ^ mul(a,x ^ mul(t,y)))
            else:
                z = (x == 3) ^ (y == 3) ^ tr(mul(a,y))
            w.append(sg(z))
        core.append(w)
    for u,v,w,k in product(range(4),range(4),range(4),range(16)):
        shifts = (0,u,v,w,u ^ v ^ w)
        M = []
        for t,a in product(range(5),range(4)):
            e = ((k >> (t % 4)) & 1) * (a == 3)
            tail = [sg(tr(mul(a ^ shifts[t],j)) ^ e) for j in range(4)]
            M.append(core[4*t+a] + tail)
        yield f"charts-{u}-{v}-{w}-{k}", M
```
5. [R] **Gegentest:** Für die ersten 16 Zeichen affine und quadratische Boolesche Phasen auf vier Bits nehmen, für den Rand alle 16 Möglichkeiten. Zuerst affine Phasen testen, die in keinem der 20 Zeilenpräfixe vorkommen. Die Präfix-Skalarprodukte sind über den gesamten Generator dieselben.
6. [U] **Chance: etwa \(0{,}1\,\%\).** Interessant ist die geringe Kreuzkorrelation ohne den Zwang, 32 Wörter schon für eine einzige lineare Basis auszugeben.

### 5. Zwei 11er-Bahnen aus dem Golay-Code und drei feste Paare

1. [R] **\((23,7)\), 50 Wörter, \(25\times23\), \(d=9\), Quotient \(1{,}8000\).**
2. [R] \(50\cdot390656/2^{23}=2{,}3285\); \(50\ge29\).
3. [L] Der binäre zyklische Golay-Code der Länge 23 ist perfekt mit Radius 3; unten verwende ich das Generatorpolynom mit Exponenten \(0,1,5,6,7,9,11\). [R] Statt einer 23er-Bahn verwende ich den **Multiplikator 2** auf den Koordinaten modulo 23; seine Ordnung ist 11. Zwei bewegliche Bahnen stammen nur aus Gewichten 7 und 8. Der konstruktive Grund für diese Wahl ist die Golay-Abstandsstruktur: Auf der Schicht der Golay-Wörter kann ein *anderes* Golay-Zentrum nur bei Abstand **genau 7** mitdecken. Die Gewicht-7-Differenzen liefern dafür viele zulässige Übergänge; die 8er-Zentren ändern, welche Paritätsklasse dabei getroffen wird. Die drei festen Paare brauchen nicht im Golay-Code zu liegen. [R] Perfektheit allein beweist hier **keinen** Radius 7 nach Ausdünnung.
4. [R] Generator, weniger als 2 500 Ausgaben:
```python
def kandidaten():
    from itertools import combinations
    g = sum(1 << j for j in (0,1,5,6,7,9,11))
    G = [0]
    for i in range(12):
        G += [w ^ (g << i) for w in G]
    def twice(w):
        return sum(((w >> j) & 1) << ((2*j) % 23) for j in range(23))
    def row(w):
        return [1-2*((w >> i) & 1) for i in range(23)]
    pool = {w for w in G if w.bit_count() in (7,8)}
    orbs = []
    while pool:
        x = min(pool)
        orb = [x]
        y = twice(x)
        while y != x:
            orb.append(y)
            y = twice(y)
        pool.difference_update(orb)
        if len(orb) == 11:
            orbs.append(list(map(row,orb)))
    q = sum(1 << j for j in {i*i % 23 for i in range(1,23)})
    fixed = list(map(row,(0,q,((1 << 23)-2) ^ q)))
    for i,j in combinations(range(len(orbs)),2):
        yield f"gol11-{i}-{j}", fixed + orbs[i] + orbs[j]
```
5. [R] **Gegentest:** Nur die 4 096 Wörter des Elterncodes prüfen, zuerst die nicht gewählten Heptaden. Wegen der 11er-Symmetrie reicht je Bahn ein Vertreter. Erst bei bestandener Prüfung Fehler vom Gewicht 1, dann 2 und 3 zu diesen Wörtern addieren.
6. [U] **Chance: unter \(0{,}05\,\%\).** Die Golay-Schicht ist zugleich der Vorteil als Billigtest und das größte Hindernis für das Ausdünnen.

### 6. Zwei supplementäre Differenzblöcke auf neun Punkten, mit verdrehter unterer Kopplung

1. [R] **\((18,5)\), 40 Wörter, \(20\times18\), \(d=8\), Quotient \(1{,}7889\).**
2. [R] \(40\cdot12616/2^{18}=1{,}9250\); \(40\ge28\).
3. [R] In \(\mathbb Z_9\) liefern \(\{0,1,3\}\) und \(\{0,4\}\) gemeinsam jede von null verschiedene geordnete Differenz genau einmal. Für ihre Vorzeichenzirkulanten \(P,Q\) folgt \(PP^\top+QQ^\top=16I+2J\). Die unverdrehte Kernmatrix \([P,Q;-Q^\top,P^\top]\) hat daher innerhalb jeder Neunergruppe sehr gleichmäßige Zeilenabstände, zwischen den Gruppen orthogonale Zeilen. Eine relative Permutation und ein Vorzeichenmuster **nur unten rechts** verändern die Kopplung. Zwei Randzeilen decken unabhängig davon alle Eingaben mit \(|S_L|+|S_R|\ge8\). Das ist weder eine zirkulante 18er-Matrix noch eine Konferenzmatrix.
4. [R] Generator, höchstens 2 268 Ausgaben:
```python
def kandidaten():
    def circ(D):
        return [[-1 if (j-i) % 9 in D else 1 for j in range(9)] for i in range(9)]
    P,Q = circ({0,1,3}), circ({0,4})
    T = list(map(list,zip(*P)))
    W = list(map(list,zip(*Q)))
    fixed = [[1]*18,[1]*9+[-1]*9] + [p+q for p,q in zip(P,Q)]
    from itertools import combinations
    masks = [()] + [(0,)] + list(combinations(range(9),2)) + [(0,3,6),(0,1,3)]
    for a in (1,2,4,5,7,8):
        for b in range(9):
            for D in masks:
                bot = [[-z for z in W[i]] +
                       [(-z if i in D else z) for z in T[(a*i+b) % 9]]
                       for i in range(9)]
                yield f"sds9-{a}-{b}-{D}", fixed+bot
```
5. [R] **Gegentest:** Beide Hälften periodisch modulo 3 wählen, danach in nur einer Hälfte ein Zeichen kippen. Für periodische Hälften gibt es zunächst nur \(8^2\) Kombinationen. Alles mit \(|S_L|+|S_R|\ge8\) sofort überspringen.
6. [U] **Chance: unter \(0{,}05\,\%\).** Die kleine Zielzelle hat wenig Dichte; die beiden Randzeilen sind eine gezielte, aber teure Absicherung.

### 7. Geränderte reguläre Entwicklung über \(\mathrm{SL}(2,3)\)

1. [R] **\((25,8)\), 50 Wörter, \(25\times25\), \(d=9\), Quotient \(1{,}8000\).**
2. [R] \(50\cdot1807781/2^{25}=2{,}6938\); \(50\ge25\).
3. [R] Eine 10-Menge wird links über der Gruppe der Ordnung 24 entwickelt. Im normalen Quaternionen-Unterverband \(Q\) und seinen beiden Nebenklassen hat sie Größen **\(1,3,6\)**. Dieser Typ ist nicht nur nach Dichte gewählt: Ein Vorzeichenvektor, der genau auf einer Achterklasse negativ ist, hat auf den ersten 24 Koordinaten gegen die entwickelten Zeilen die Werte \(4-2(8-2k)\), \(k=1,3,6\), also \(-8,0,12\). Die Spitze 12 überlebt jedes Randzeichen. Die konstante Zusatzzeile deckt die konstanten Gruppenvektoren ab. Damit scheitert die Entwicklung **nicht an den Vorzeichenfunktionen auf dem Quotienten der Ordnung 3**, auch nicht nach Komplementierung. Die Singletonposition kann durch Translation auf die Identität normiert werden.
4. [R] Generator, \(56\cdot28=1568\) Ausgaben:
```python
def kandidaten():
    from itertools import product, combinations
    G = [g for g in product(range(3),repeat=4) if (g[0]*g[3]-g[1]*g[2]) % 3 == 1]
    def mul(g,h):
        a,b,c,d = g
        e,f,i,j = h
        return ((a*e+b*i)%3,(a*f+b*j)%3,(c*e+d*i)%3,(c*f+d*j)%3)
    I = (1,0,0,1)
    Q = {g for g in G if (g[0]+g[3])%3 == 0} | {I,(2,0,0,2)}
    h = next(g for g in G if g not in Q)
    A = sorted(mul(h,q) for q in Q)
    B = sorted(set(G)-Q-set(A))
    idx = {g:i for i,g in enumerate(G)}
    left = [[idx[mul(g,v)] for v in G] for g in G]
    for i,X in enumerate(combinations(A,3)):
        for j,Y in enumerate(combinations(B,6)):
            D = [idx[v] for v in (I,)+X+Y]
            M = [[1]*25]
            for p in left:
                r = [1]*25
                for t in D:
                    r[p[t]] = -1
                M.append(r)
            yield f"sl23-{i}-{j}", M
```
5. [R] **Gegentest:** Nächste Quotientenprobe: Eingaben konstant auf den zwölf Paaren \(\{g,-g\}\), beide Randzeichen. Das sind \(8192\) Proben vor Symmetriereduktion. Auch Eingaben **entgegengesetzt** auf jedem solchen Paar sind billig: Hier verschwindet jeder Paarbeitrag, wenn die Zeile auf dem Paar konstant ist.
6. [U] **Chance: etwa \(0{,}05\,\%\).** Von den Einbahn-Entwicklungen würde ich diese vor einer Entwicklung über einer Gruppe mit vielen reellen linearen Charakteren wählen.

### 8. Zwei punktierte Walsh-Basen mit quadratischer Kreuzphase und fehlendem unteren Paarpartner

1. [R] **\((30,10)\), 62 Wörter, \(31\times30\), \(d=10\), Quotient \(10/\sqrt{31}=1{,}7961\).**
2. [R] \(62\cdot53009102/2^{30}=3{,}0608\); \(62\ge27\).
3. [R] Zwei Hälften von je 15 Koordinaten. Oben steht in jeder Hälfte eine bei 0 punktierte Walsh-Basis auf vier Bits. Unten wird jede Basis mit einer nichtausgearteten quadratischen Phase multipliziert; 15 der 16 unteren Zeilen bleiben. Innerhalb einer Halbbasis sind verschiedene Korrelationen vom Betrag 1, **zwischen** den beiden Halbbasen vom Betrag 3 oder 5: Das volle quadratische Walsh-Spektrum ist \(\pm4\), die weggelassene Stelle trägt 1 bei. Zwei 5er können also gerade die verlangte 10 liefern. Wichtig ist die relative **zeilenabhängige** Vorzeichenwahl rechts: Ohne sie können die beiden punktierungsbedingten 5er-Spitzen beim Test mit den ausgelassenen unteren Zeilen systematisch gegeneinander stehen. Es handelt sich nicht um eine punktierte Hadamard-Doppelung mit identischen Halbblöcken.
4. [R] Generator, 1 024 Ausgaben:
```python
def kandidaten():
    def sg(z):
        return 1-2*(z & 1)
    def lin(i,x):
        return (i & x).bit_count() & 1
    def quad(x,k):
        a,b,c,d = [(x >> j) & 1 for j in range(4)]
        return ((a*b ^ c*d), (a*c ^ b*d), (a*d ^ b*c))[k]
    U = [[sg(lin(i,x)) for x in range(1,16)] for i in range(16)]
    for k in (1,2):
        for h in range(1,9):
            for p in range(8):
                for v in range(8):
                    M = []
                    for i in range(16):
                        s = sg(quad(i,0) ^ lin(p,i))
                        M.append(U[i]+[s*z for z in U[i]])
                    for i in range(1,16):
                        j = h if i == 1 else (1 if i == h else i)
                        t = sg(quad(i,k) ^ lin(v,i))
                        M.append([sg(quad(x,0))*U[i][x-1] for x in range(1,16)] +
                                 [t*sg(quad(x,k))*U[j][x-1] for x in range(1,16)])
                    yield f"cross30-{k}-{h}-{p}-{v}", M
```
5. [R] **Gegentest:** Zuerst in beiden Hälften die ausgelassene untere Basiszeile einsetzen, mit beiden relativen Vorzeichen. Unten entstehen nur Beträge \(\le2\); oben **muss** irgendwo \(5+5\) mit gleichem Vorzeichen auftreten. Danach alle Paare von Halbzeilen aus den oberen und unteren Basen testen. Für keinen dieser Tests braucht man den \(2^{30}\)-Lauf.
6. [U] **Chance: unter \(0{,}05\,\%\).** Die hohe Dichte hilft, aber der fehlende Paarpartner gibt einen ungewöhnlich scharfen Gegenzeugentest.

[U] **Rangfolge für den Maschinenlauf:** **1, 2**, dann 4, 3, 7, 8, 5, 6. Nr. 1 hat den engsten Anschluss an einen bestätigten Treffer; Nr. 2 liefert für einen kleinen Aufwand die sauberste vollständige Familienentscheidung. **Als Rekordwette halte ich keine der acht für aussichtsreich.** Als kurze Siebläufe halte ich insbesondere die ersten beiden für lohnend: Ein negatives Ergebnis grenzt dort einen konkreten Reparaturmechanismus ein, statt nur einen weiteren erfolglosen Zufallsstart zu protokollieren.
