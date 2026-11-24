> **OpenRouter-Antwort**
>
> - Modell: openai/gpt-6.2-sol
> - Betreiber: OpenAI (laut OpenRouter)
> - Datum: 24.11.2026, 18:32
> - Batch: Auftrag openrouter:batch-1795539886-vZ7OaIqKv7RxFJFyWGlp, abgegeben 24.11.2026, 18:04, fertig 24.11.2026, 18:15
> - Eingelesene Dateien: speichert OpenRouter nicht (siehe Abgabe-Meldung)
> - Tokens: Eingabe 24.477 · Ausgabe 42.855 (davon Denken 34.739) · gesamt 67.332
> - Kosten laut OpenRouter: 0,24 US-$
> - Datenregel: Batch: laut OpenRouter kein Training (Konto-Regel und Betreiber aus provider.only); kein ZDR, 30 Tage gespeichert, USA
> - Denkstufe: xhigh
> - finish_reason: stop

---

[U] **Zuerst testen würde ich Nr. 2, die Paley-Neuverkabelung für \((20,6)\), und Nr. 4, den Dreispalten-Lift für \((25,8)\).** Beide behalten einen geprüften Kern und erlauben es, viele Kandidaten mit denselben Randzeugen zu verwerfen.

[R] **Kein Ausschluss für eine ganze offene Zelle.** Zwei Ausschlüsse für Unterfamilien: Eine einzelne reguläre \(\mathbb F_5^2\)-Translationsbahn kann \((25,8)\) nicht lösen. Und bei der Paley-Konferenzmatrix der Ordnung 26 genügen weder eine beliebige \(\pm1\)-Diagonale noch fünf beliebige zusätzliche \(\pm1\)-Zeilen, um \(d=10\) zu erreichen. Die Beweise stehen bei Nr. 5 und 6; die dortigen Generatoren brechen jeweils die betreffende Struktur auf.

[R] Alle acht Generatoren liefern reine Vorzeichenmatrizen. Die Quotienten sind **Untergrenzen bei Erfolg**, die Tabellenuntergrenzen sind wie im Auftrag angesetzt. Die Generatoren sind als getrennte Module gedacht; Nr. 2 und 4 verwenden den folgenden gemeinsamen Vorspann. Keiner der Generatoren prüft Vorzeichenvektoren.

[U] Die Erzeugung allein, ohne Dateiausgabe und Prüfstand, schätze ich für jeden Generator auf unter 60 s auf einem üblichen Einzelkern. Das ist nicht gemessen. Die Prozentangaben beziehen sich jeweils auf mindestens einen Treffer in der **angegebenen endlichen Familie**, nicht auf die Existenz eines Codes in der Zelle.

[R] **Gemeinsamer Vorspann für Nr. 2 und 4:**
```python
def paley12():
    import numpy as np
    H = np.ones((12, 12), dtype=np.int64)
    H[1:, 0] = -1
    for i in range(11):
        for j in range(11):
            H[i+1, j+1] = 1 if (i-j) % 11 in (0, 1, 3, 4, 5, 9) else -1
    assert (H @ H.T == 12 * np.eye(12, dtype=np.int64)).all()
    return H
```

## 1. Fano-Ebenentausch: vier Zeilen heraus, drei hinein

1. **[R] Ziel:** \((15,4)\), 30 Wörter, \(d=7\), Quotient \(7/\sqrt{15}=1{,}8074\).
2. **[R] Dichte:** \(30\cdot1941/2^{15}=1{,}7770\); \(30\ge23\).
3. **[R] Mechanismus – lokaler Tausch im geprüften kubischen Kern.** Die 16 Ausgangszeilen sind die beiden Achterbänke aus `construct_32.py`. Der Tausch verändert nur eine affine Viererebene der ersten Bank: Eine Zeile entfällt, drei erhalten neue rechte Masken. Abgedeckt bleiben alle Punkte, die eine der zwölf unveränderten Zeilen trifft. Für den Tausch gibt es eine explizite Pflichtmenge: Sei \(S\) das Urbild einer Fano-Geraden unter \(t\mapsto t^3\); dann ist \(S\) linear unabhängig, denn \(x^3+y^3+(x+y)^3=xy(x+y)\ne0\). Für eine alte Zeile \(r\) setze auf \(S\) beide Vorzeichen gleich ihrem Walsh-Zeichen und auf den vier übrigen Paaren entgegengesetzt. Wähle dort die vier linken Vorzeichen mit Produkt \(-1\), und setze das einzelne rechte Vorzeichen bei \(t=0\) auf \(+1\). Dann trifft in der ersten Bank nur \(r\) mit Betrag 7. In der zweiten Bank bilden die vier aktiven Labels eine affine Ebene; das negative Produkt verhindert Betrag 8 im Paaranteil, also bleibt der Gesamtbetrag höchstens 5. Damit sind pro alter Zeile 56 private Punkte bis auf Gesamtvorzeichen explizit bekannt. Die Masken verändern gezielt, welche Paare auf diesen Punkten addieren statt sich aufzuheben.
4. **[R] Generator:** \(7\cdot4\cdot2\cdot14^3=153\,664\) Matrizen.
```python
from itertools import product

def kandidaten():
    I = tuple(range(8))
    P = (0, 1, 3, 4, 5, 6, 7, 2)
    def row(k, f, s):
        w = [1-2*((k & f[t]).bit_count() & 1) for t in I]
        return w[1:] + [s*v for v in w]
    A = [row(k, I, 1) for k in I] + [row(k, P, -1) for k in I]
    for a in range(1, 8):
        F = [k for k in I if (a & k).bit_count() % 2 == 0]
        for drop in F:
            piv = [k for k in F if k != drop]
            frozen = [A[k] for k in range(16) if k not in F]
            for typ, f in enumerate((I, P)):
                masks = [[7+t for t in range(1, 8)
                          if ((b & f[t]).bit_count() & 1) == e]
                         for b in range(1, 8) for e in (0, 1)]
                for js in product(range(14), repeat=3):
                    rows = []
                    for k, j in zip(piv, js):
                        r = A[k][:]
                        for c in masks[j]:
                            r[c] *= -1
                        rows.append(r)
                    yield f"fano-{a}-{drop}-{typ}-{js}", frozen + rows
```
5. **[R] Billigster Gegentest:** Die \(4\cdot56\) privaten Punkte der vier herausgenommenen Originalzeilen müssen sämtlich von den **drei** Ersatzzeilen getroffen werden. Die zwölf festen Zeilen braucht man hierfür nicht auszuwerten.
6. **[U] Chance:** etwa **0,05 %**. Wenn schon dieser Pflichtmengentest keinen Tausch durchlässt, würde ich die Familie nicht um weitere Masken erweitern.

## 2. Paley-12: vier Paare neu verkabeln

1. **[R] Ziel:** \((20,6)\), 40 Wörter, \(d=8\), Quotient \(8/\sqrt{20}=1{,}7889\).
2. **[R] Dichte:** \(40\cdot60460/2^{20}=2{,}3064\); \(40\ge24\).
3. **[R] Mechanismus – acht garantierende Paare bleiben intakt.** Ausgangspunkt ist die geprüfte \(1+3\)-Streichung. Acht Indizes behalten beide Zeilen \((U_i,\pm V_i)\); auf den anderen vier Indizes bleiben alle linken und rechten Projektionen erhalten, werden aber durch vier einzelne vorzeichenbehaftete Kanten neu gekoppelt. Für einen Eingabepunkt setze \(T=\{i: |u_i|+|v_i|\ge8\}\). Gefährdet sind **genau die Punkte mit \(T\subseteq S\)**, wobei \(S\) die vier neu verkabelten Indizes bezeichnet. Anders als beim bloßen Streichen kann eine neue Kante auch zwei Beiträge zusammenbringen, deren ursprüngliche Paare beide unter der Schwelle lagen.
4. **[R] Generator:** \(\binom{12}{4}\cdot9\cdot16=71\,280\) Matrizen. Mit `paley12` von oben.
```python
from itertools import combinations, permutations, product

def kandidaten():
    H = paley12()
    U, V = H[:, 1:].tolist(), H[:, 3:].tolist()
    W = [[-v for v in r] for r in V]
    for S in combinations(range(12), 4):
        keep = [i for i in range(12) if i not in S]
        fixed = [U[i] + R[i] for i in keep for R in (V, W)]
        for P in permutations(S):
            if any(i == j for i, j in zip(S, P)):
                continue
            edges = [(U[i] + V[j], U[i] + W[j]) for i, j in zip(S, P)]
            for bits in product((0, 1), repeat=4):
                rows = [e[b] for e, b in zip(edges, bits)]
                yield f"paley-wire-{S}-{P}-{bits}", fixed + rows
```
5. **[R] Billigster Gegentest:** Randzeugen der 48er-Kontrolle nach \(T\) speichern. Falls für einen solchen Zeugen mit \(T\subseteq S\) auf allen vier neuen Kanten schon \(|u_i|+|v_{P(i)}|\le6\) gilt, sind **alle 16** Vorzeichenbelegungen erledigt. Sonst denselben Zeugen auch mit negiertem rechten Block einsetzen; das vertauscht auf jeder Kante die beiden Vorzeichenoptionen.
6. **[U] Chance:** etwa **0,4 %**. Das ist mein erster Test, vor allem weil ein Negativbefund für viele Kandidaten gemeinsam anfällt.

## 3. Drei geschert beschriftete Walsh-Bänke

1. **[R] Ziel:** \((23,7)\), 50 Wörter, \(d=9\), Quotient \(9/5=1{,}8000\).
2. **[R] Dichte:** \(50\cdot390656/2^{23}=2{,}3285\); \(50\ge29\).
3. **[R] Mechanismus – vier einstimmige Tripel erzwingen einen Treffer.** Jede Achterbank ordnet die 23 Spalten sieben Tripeln sowie einem Paar bei Label 0 zu. Die Eingabe ergibt Tripelsummen \(s_t\in\{\pm1,\pm3\}\) und \(s_0\in\{0,\pm2\}\). Alle acht Walsh-Werte sind untereinander kongruent modulo 4: Jede nichttriviale Walsh-Differenz erfasst vier ungerade \(s_t\). Sind alle Beträge höchstens 7, liegen die Werte daher entweder in \([-7,5]\) oder in \([-5,7]\). Die Sekante von \(z^2\) auf dem jeweiligen Intervall gibt mit Parseval
   \[
   \sum_t s_t^2\le35+2|s_0|.
   \]
   Bei vier einstimmigen Tripeln wäre die linke Seite mindestens \(39+s_0^2\), ein Widerspruch. Die drei Bänke verwenden unterschiedliche Tripelbildungen **und** unterschiedliche Vorzeichenphasen; der Gegner muss die Bedingung „höchstens drei“ gleichzeitig erfüllen und zusätzlich in jeder Bank ein zulässiges Walsh-Profil erzeugen.
4. **[R] Generator:** höchstens \(4^4\cdot4^2\cdot2=8\,192\) Matrizen.
```python
from itertools import product

def kandidaten():
    I = tuple(range(8))
    P = (0, 1, 3, 4, 5, 6, 7, 2)
    Q = tuple(P.index(t) for t in I)
    F = (I, P, Q, (0, 2, 1, 3, 4, 5, 6, 7))
    cols = [(l, t) for l in range(3) for t in I if (l, t) != (2, 0)]
    def par(v):
        return v.bit_count() & 1
    for i, j, k, h in product(range(4), repeat=4):
        lab = ((I, I, I), (F[i], F[j], I), (F[k], I, F[h]))
        for a, b in product((1, 2, 4, 7), repeat=2):
            phase = ((0, 0, 0), (a, 0, 0), (0, b, 0))
            A = [[1-2*(par(u & lab[g][l][t]) ^
                       par(phase[g][l] & P[t]) ^ (((0, 2, 4)[g] >> l) & 1))
                  for l, t in cols] for g in range(3) for u in I]
            for c in (1, 3):
                r = [1-2*(par(c & t) ^ par((a ^ b) & P[t]) ^ ((6 >> l) & 1))
                     for l, t in cols]
                M = A + [r]
                if len({tuple(v*x[0] for v in x) for x in M}) == 25:
                    yield f"walsh3-{i}{j}{k}{h}-{a}{b}-{c}", M
```
5. **[R] Billigster Gegentest:** Zuerst zeilenweise affine Eingaben **pro Spaltenschicht** testen: höchstens \(16^3\) Fälle vor Gesamtvorzeichen-Reduktion. Sie prüfen gerade den Einwand, dass die Scherungen nur eine gemeinsame niedrigspektrale Eingabe umetikettieren.
6. **[U] Chance:** etwa **0,03 %**. Die Vier-Tripel-Schranke ist brauchbar, aber sie allein schließt die dünnen Profile nicht annähernd aus.

## 4. Dreispalten-Lift des 22er-Paley-Kerns

1. **[R] Ziel:** \((25,8)\), 50 Wörter, \(d=9\), Quotient \(9/5=1{,}8000\).
2. **[R] Dichte:** \(50\cdot1807781/2^{25}=2{,}6938\); \(50\ge25\).
3. **[R] Mechanismus – exakter Resttest auf einem Dreierwürfel.** Der Altblock ist die laut Status geprüfte \(1+1\)-Streichung mit verschiedenen Löschspalten und \(d=8\). Für einen festen Altpunkt sei \(t\) der Betrag einer alten Zeilenantwort. Richte das neue Dreiermuster nach dem Vorzeichen dieser Antwort aus. Bei \(t=6,8,10,\ge12\) trifft die Zeile auf dem neuen Dreierwürfel dann genau **1, 4, 7 beziehungsweise 8** Ecken. Bei \(t\le4\) trifft sie keine. Damit müssen nicht alle kritischen Altzeilen auf \(t=8\) angehoben werden: Die \(t=6\)-Zeilen können einzelne Lücken schließen, und eine \(t=10\)-Zeile lässt nur eine Ecke frei. Die drei neuen Spalten kommen aus komponentenweisen Paley-Spaltenprodukten. Die 25. Zeile koppelt zwei verschiedene alte Halbzeilen und bekommt ein eigenes Dreiermuster.
4. **[R] Generator:** \(2\binom{11}{3}\cdot6\cdot4=7\,920\) Matrizen. Mit `paley12` von oben.
```python
from itertools import combinations
import numpy as np

def kandidaten():
    H = paley12()
    U = H[:, 1:]
    V = H[:, [j for j in range(12) if j != 1]]
    A = np.block([[U, V], [U, -V]])
    for a in (0, 1):
        Q = H[:, a:a+1] * H
        for cs in combinations([j for j in range(12) if j != a], 3):
            P = Q[:, list(cs)]
            for mode in range(1, 7):
                S = P * [1-2*((mode >> h) & 1) for h in range(3)]
                M = np.column_stack((A, np.vstack((P, S)))).tolist()
                for i, j in ((0, 1), (1, 0), (1, 2), (2, 5)):
                    r = U[i].tolist() + (-V[j]).tolist()
                    r += [int(P[i, 0]), int(S[j, 1]), int(P[(i+j) % 12, 2])]
                    yield f"lift3-{a}-{cs}-{mode}-{i}{j}", M + [r]
```
5. **[R] Billigster Gegentest:** Alte Zeugen mit Maximum 8 zuerst. Für jeden Zeugen die acht neuen Ecken als Bitmaske führen und die Zeilenmasken vereinigen. Schon vor dem Mustertest ist ein Zeuge tödlich, wenn \(n_6+4n_8\) im Altblock plus der Beitrag der Zusatzzeile kleiner als 8 ist.
6. **[U] Chance:** etwa **0,3 %**. Der informative Befund ist hier nicht die mittlere Restquote, sondern ob dieselben wenigen Altprofile **alle** Liftmuster töten.

## 5. Fünf Parabelbänder über \(\mathbb F_5^2\)

1. **[R] Ziel:** \((25,8)\), 50 Wörter, \(d=9\), Quotient \(9/5=1{,}8000\).
2. **[R] Dichte:** \(50\cdot1807781/2^{25}=2{,}6938\); \(50\ge25\).
3. **[R] Mechanismus – verschiedene Richtungen statt einer regulären Bahn.** Hat eine Zeile Negativmenge \(B\) der Größe \(b\), und hat die Eingabe Negativmenge \(Y\), dann ist die Antwort \(25-2|Y|-2b+4|B\cap Y|\). Für \(b=8\) und \(Y\) aus zwei parallelen Geraden muss also ein Schnitt der Größe mindestens 5 oder genau 0 vorkommen. Ein Band läuft über fünf Translationen **in seiner eigenen Richtung**. In jeder der sechs Projektionen auf parallele Geraden verschieben vier oder fünf Bänder ihre Schnitthistogramme über alle fünf Lagen. Die Krümmung und die drei Zusatzpunkte variieren dabei das Histogramm, statt nur dasselbe Histogramm neu zu verschieben.

   **[R] Warum keine einzelne reguläre Bahn:** Die Einser-Eingabe verlangt nach Komplementierung \(b\le8\). Für \(b\le3\) genügt als Gegenzeuge jedes \(Y\) der Größe 12; für \(b=4\) gibt es ein solches \(Y\), das kein Translat ganz enthält, denn \(25\binom{12}{4}/\binom{25}{4}<1\). Für \(b=5,6\) braucht ein Zehner-\(Y\) aus zwei Geraden einen Schnitt \(\ge4\), für \(b=7,8\) einen Schnitt \(\ge5\) oder \(0\). In jeder Projektion müssen **beide** Abstandstypen der zwei Geraden funktionieren. Haben die fünf Projektionsfasern Größen \(b_i\), verlangt das \(\sum_i\binom{b_i}{2}\ge3\) für \(b=5,6\): Bei höchstens zwei Paaren gibt es höchstens ein Faserpaar mit Summe \(\ge4\). Für \(b=7,8\) sind mindestens fünf Paare nötig: Bei höchstens vier Paaren gibt es weder zwei leere Fasern noch mehr als ein Faserpaar mit Summe \(\ge5\). Über sechs Richtungen summieren sich die Paarzahlen aber zu \(\binom b2\), jeweils zu wenig.
4. **[R] Generator:** \(3\cdot2\cdot4^5=6\,144\) Matrizen. Der dicke Vertikalfaser-Index identifiziert die Translation innerhalb des Bandes; auf den anderen vier Fasern unterscheidet der lineare Term verschiedene Bänder.
```python
from itertools import product

def kandidaten():
    Z = list(product(range(5), repeat=2))
    for c in ((1,)*5, (2,)*5, (1, 2, 1, 2, 3)):
        for mode in (0, 1):
            bands = {}
            for b in range(5):
                o = mode*b*b
                for skip in range(1, 5):
                    B = {(x, (c[b]*x*x+b*x+o) % 5) for x in range(5)}
                    B |= {(0, (t+o) % 5) for t in range(1, 5) if t != skip}
                    bands[b, skip] = [
                        [-1 if ((x-s) % 5, (y-b*s) % 5) in B else 1
                         for x, y in Z] for s in range(5)]
            for skips in product(range(1, 5), repeat=5):
                M = [r for b, t in enumerate(skips) for r in bands[b, t]]
                yield f"parabel-{c}-{mode}-{skips}", M
```
5. **[R] Billigster Gegentest:** Die 30 einzelnen Geraden und die 60 Vereinigungen von zwei parallelen Geraden. Dafür genügen die sechs Projektionshistogramme jeder Negativmenge; kein allgemeiner Diskrepanzlauf ist nötig.
6. **[U] Chance:** etwa **0,02 %**. Ich würde zuerst feststellen, ob überhaupt eine Parameterklasse alle 90 Geradentests besteht.

## 6. Paley-26 mit zwei Richtungstauschen und fünf Geradenzeilen

1. **[R] Ziel:** \((26,8)\), 62 Wörter, \(d=10\), Quotient \(10/\sqrt{31}=1{,}7961\).
2. **[R] Dichte:** \(62\cdot2533987/2^{26}=2{,}3411\); \(62\ge36\).
3. **[R] Mechanismus – zwei Tausche brechen ein nachweislich zu großes Loch.** Schreibe die 25 Feldpunkte als \(\mathbb F_5^2\) und verwende das Normzeichen von \(a^2-2b^2\). Es ist auf jeder der sechs Richtungen konstant, mit je drei positiven und negativen Richtungen. Auf einer Eingabe, die auf fünf parallelen Geraden die Werte \(f_1,\ldots,f_5\) mit \(\sum f_i=1\) hat, ist der Feldanteil der Konferenzantwort daher \(\eta(5f_i-1)\), mit \(\eta\in\{\pm1\}\). Mit Randkoordinate und beliebiger Diagonale bleibt der Betrag höchstens 8. Ein Tausch zweier entgegengesetzter Einträge kann die Antwort um 4 ändern. Zwei nichtparallele Verschiebungsdifferenzen sind so gewählt, dass **keine** der sechs Projektionen beide Tausche unsichtbar macht.

   **[R] Warum fünf bloße Zusatzzeilen nicht genügen:** Es gibt \(6\cdot10\cdot2=120\) solcher Pflichtpunkte einschließlich beider Randvorzeichen. Für eine Zusatzzeile sei \(T\) ihre Summe auf den Feldpunkten und seien \(L_i\) ihre fünf Geradensummen in einer Projektion. Vor der Randkoordinate lautet die Antwort \(t_{ij}=T-2(L_i+L_j)\). Über alle 60 Geradenpaare gilt \(\sum t_{ij}^2=1500\): Pro Richtung beträgt die Summe \(12\sum L_i^2-2T^2\), und über alle Richtungen ist \(\sum L_i^2=125+T^2\), weil jedes Paar verschiedener Feldpunkte genau eine Gerade bestimmt. Ein \(|t_{ij}|=9\) deckt einen Pflichtpunkt, \(|t_{ij}|\ge11\) zwei. Aus \(1500-60=1440\) folgt, dass 24 Treffer nur bei zwölf Beträgen 11 und sonst lauter Beträgen 1 möglich wären. Nach Zeilennegation sei \(T\equiv1\bmod4\); dann sind die Antworten \(1,-11\). Weil ihre Summe pro Richtung \(2T\) ist, müssten genau zwei der zehn Paare pro Richtung den Wert \(-11\) haben. Zwei mögliche Paarsummen im Abstand 6 erlauben aber höchstens zwei verschiedene \(L_i\); wenn beide vorkommen, muss einer ein Einzelwert sein, sonst entstehen drei Paarsummen. Der Einzelwert teilt die Paare in 4 und 6, nicht 2 und 8. Also trifft jede Zusatzzeile höchstens 23 Pflichtpunkte, fünf zusammen höchstens 115.
4. **[R] Generator:** höchstens \(12\cdot11\cdot2\cdot3\cdot60=47\,520\) Matrizen; der Richtungsfilter reduziert diese Zahl.
```python
from itertools import product, permutations

def kandidaten():
    Z = list(product(range(5), repeat=2))
    def chi(x, y):
        return 1 if (x*x-2*y*y) % 5 in (1, 4) else -1
    def at(x, y, v):
        return 5*((x+v[0]) % 5) + (y+v[1]) % 5
    def proj(v, x, y):
        return x if v == 5 else (y-v*x) % 5
    neg = [z for z in Z[1:] if chi(*z) == -1]
    a, c = (1, 0), (1, 1)
    patches = []
    for skip, delta, shift in product(range(6), (1, 2), range(5)):
        rows = [[-1 if (proj(v, x, y)-v-shift) % 5 in (0, delta) else 1
                 for x, y in Z] + [1] for v in range(6) if v != skip]
        patches.append(((skip, delta, shift), rows))
    for b, d in permutations(neg, 2):
        u, v = (a[0]-b[0], a[1]-b[1]), (c[0]-d[0], c[1]-d[1])
        if (u[0]*v[1]-u[1]*v[0]) % 5 == 0:
            continue
        for stripe, kind in product(range(2), range(3)):
            A = []
            for x, y in Z:
                diag = (1, 1-2*(x in (0, 1)), 1-2*((x+y) % 5 in (0, 2)))[kind]
                r = [diag if (s, t) == (x, y) else chi(s-x, t-y)
                     for s, t in Z] + [1]
                i, j = at(x, y, a), at(x, y, b)
                r[i], r[j] = r[j], r[i]
                if not stripe or (x+2*y) % 5 in (0, 1, 2):
                    i, j = at(x, y, c), at(x, y, d)
                    r[i], r[j] = r[j], r[i]
                A.append(r)
            A.append([1]*26)
            for key, rows in patches:
                M = A + rows
                if len(set(map(tuple, M))) == 31:
                    yield f"conf26-{b}-{d}-{stripe}{kind}-{key}", M
```
5. **[R] Billigster Gegentest:** Genau die 120 Pflichtpunkte. Insbesondere zuerst zählen, welche davon die **26 getauschten Kernzeilen** noch verfehlen. Dann sind die fünf Zusatzzeilen nur gegen diese Restliste zu testen.
6. **[U] Chance:** etwa **0,1 %**. Der Ansatz ist aussagekräftig, weil er nicht bloß eine neue Konferenzordnung probiert, sondern einen konkreten Ausschluss angreift.

## 7. \(15+13\) aus zwei Hadamard-16-Bausteinen, einer geschaltet

1. **[R] Ziel:** \((28,9)\), 62 Wörter, \(d=10\), Quotient \(10/\sqrt{31}=1{,}7961\).
2. **[R] Dichte:** \(62\cdot11698223/2^{28}=2{,}7019\); \(62\ge30\).
3. **[R] Mechanismus – spektrale Überlappung nach einem orthogonalitätserhaltenden Schaltzug.** Rechts wird in der Walsh-16-Matrix ein \(4\times4\)-Rechteck negiert. Das erhält die Orthogonalität: Bei einem Zeilenpaar, von dem genau eine Zeile geschaltet wird, ist das Skalarprodukt auf den vier Schaltspalten null. Nach Streichung \(1+3\) gilt getrennt \(\sum u_i^2=16\cdot15\) und \(\sum v_i^2=16\cdot13\). Wenn alle Beträge links höchstens 5 sind, müssen mindestens sechs den Betrag 5 haben: Für die Anzahlen gilt \(3n_5+n_3=28\). Rechts sind es analog mindestens vier. Solche 5er dürfen in einem Gegenzeugen auf keinem intakten Paar zusammentreffen; eine 7 darf nicht einmal auf eine 3 der anderen Seite treffen. Die Schaltung ändert die möglichen Lagen dieser Mengen, die Zeilenzuordnung koppelt sie, und nur **ein** Paar verliert eine seiner beiden Vorzeichenoptionen. Das ist keine erneute gleich ausgerichtete Doppelung der Ordnung 20.
4. **[R] Generator:** \(4\binom{16}{3}\cdot8=17\,920\) Matrizen.
```python
from itertools import combinations
from random import Random
import numpy as np

def kandidaten():
    H = np.array([[1-2*((i & j).bit_count() & 1)
                   for j in range(16)] for i in range(16)], dtype=np.int64)
    K = H.copy()
    K[np.ix_(range(4), range(0, 16, 4))] *= -1
    rng = Random(20261124)
    perms = [list(range(16))]
    for _ in range(7):
        p = list(range(16))
        rng.shuffle(p)
        perms.append(p)
    for l in (0, 1, 4, 5):
        U = np.delete(H, l, axis=1)
        for J in combinations(range(16), 3):
            V = np.delete(K, J, axis=1)
            for k, p in enumerate(perms):
                W = V[p]
                A = np.block([[U, W], [U, -W]]).tolist()
                drop = (l + sum(J) + 5*k) % 32
                yield f"schalt16-{l}-{J}-{k}-{drop}", A[:drop] + A[drop+1:]
```
5. **[R] Billigster Gegentest:** Quadratische Phasen \(t_0t_1+t_2t_3\) plus affine Terme auf beiden 16er-Blöcken, auf die behaltenen Spalten eingeschränkt. Die Halbprofile einmal berechnen und für alle acht Zuordnungen wiederverwenden. Ein Paar von Halbprofilen mit \(|u_i|+|v_{p(i)}|\le8\) auf allen 16 Indizes erledigt den Kandidaten unabhängig von der Zeilenstreichung.
6. **[U] Chance:** etwa **0,05 %**. Die Energie erzwingt hohe Einzelwerte, aber nicht von sich aus deren Überlappung.

## 8. Vier Walsh-Bänke mit zwei gescherten Faserzerlegungen

1. **[R] Ziel:** \((30,10)\), 62 Wörter, \(d=10\), Quotient \(10/\sqrt{31}=1{,}7961\).
2. **[R] Dichte:** \(62\cdot53009102/2^{30}=3{,}0609\); \(62\ge27\).
3. **[R] Mechanismus – Hamming-Schutz für den flachen Fall, Scherung für die Mischfälle.** Vor der Streichung gibt es acht Viererfasern. Zwei Bänke verwenden dieselben Fasern und zwei verschiedene Zeilen des \(H_4\)-Musters; die anderen zwei Bänke lassen die Schichten verschieden über die Faserlabels laufen. In jeder vollen Faser hat die \(H_4\)-Transformation der Eingabe entweder vier Beträge 2 oder genau einen Betrag 4. Sind alle sieben vollen Fasern vom ersten Typ, sehen beide festen Bänke sieben Beträge 2. Bei Label 0 bleiben zwei Spalten; ihre Summe ist in genau einer der festen Bänke null. Diese Bank enthält dann den doppelten punktierten Walsh-7-Test und liefert mindestens \(2\cdot5=10\): Die acht Walsh-Zeilen und ihre Komplemente auf sieben Koordinaten haben Mindestabstand 3, und ihre 16 Radius-1-Kugeln füllen den Würfel. Die Scherung soll gerade die Mischfälle angreifen, ohne diesen Schutz zu verlieren. Gestrichen wird ausschließlich in einer gescherten Bank.
4. **[R] Generator:** höchstens \(5^4\cdot2\cdot8=10\,000\) Matrizen.
```python
from itertools import product

def kandidaten():
    I = tuple(range(8))
    P = (0, 1, 3, 4, 5, 6, 7, 2)
    Q = tuple(P.index(t) for t in I)
    F = (P, Q, (0, 2, 1, 3, 4, 5, 6, 7),
         (0, 1, 2, 3, 5, 4, 6, 7), (0, 2, 4, 6, 1, 3, 5, 7))
    cols = [(l, t) for t in I for l in range(4) if t or l < 2]
    for a, b, c, d in product(range(5), repeat=4):
        lab = ((I,)*4, (P,)*4, (I, F[a], F[b], P), (F[c], I, F[d], Q))
        for phase in (0, 1):
            A = [[1-2*(((r & lab[k][l][t]).bit_count()
                        + (k & l).bit_count()
                        + phase*(k >= 2)*(l >= 2)*(P[t] & 1)) & 1)
                  for l, t in cols] for k in range(4) for r in I]
            for omit in range(16, 32, 2):
                M = A[:omit] + A[omit+1:]
                if len({tuple(v*x[0] for v in x) for x in M}) == 31:
                    yield f"walsh4-{a}{b}{c}{d}-{phase}-{omit}", M
```
5. **[R] Billigster Gegentest:** Nicht mit dem bewiesenermaßen geschützten flachen Fall anfangen. Zuerst Eingaben mit **zwei konzentrierten Viererfasern** testen, bei den übrigen Fasern mit flachem Profil. Die ersten beiden Bänke sind in der ganzen Familie identisch; einmal gefundene niedrige Halbprofile lassen sich gegen alle Scherungen wiederverwenden.
6. **[U] Chance:** etwa **0,1 %**. Am ehesten würde ich diese Familie von den Vorschlägen mit \(d=10\) bis zur exakten Prüfung verfolgen.

### Rangfolge

[U] **2 → 4 → 8 → 6 → 1 → 7 → 3 → 5.** Bei **2** würde ich erst die vorzeichenunabhängigen Kanten-Gegentests fahren, bei **4** erst die wiederverwendbaren Acht-Ecken-Tests. Das sind die zwei Läufe, die ich starten würde.

[U] **Keine der acht Familien halte ich derzeit für einen wahrscheinlichen Rekordtreffer.** Der Grund, Nr. 2 und 4 trotzdem vorzuziehen, ist konkret: Bei einem Fehlschlag liefern sie eine kleine Liste von Altprofilen, an der sich die nächste Bauregel messen lässt, statt nur eine weitere Restquote zu produzieren.
