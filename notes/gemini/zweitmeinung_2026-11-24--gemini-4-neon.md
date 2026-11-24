> **Gemini-Antwort**
>
> - Modell: gemini-4-neon
> - Datum: 24.11.2026, 15:19
> - Batch: Auftrag gemini:batches/e7g092xi2nmbfdizh3wv5rbsnj69t6p6hcmg, abgegeben 24.11.2026, 14:27, fertig 24.11.2026, 15:02
> - Tokens: Eingabe 21.729 · Antwort 7.388 · Denken 21.475 · gesamt 50.592
> - Geschätzte Kosten (Batch, 50 % des Standardpreises, Stand 22.11.2026): ~0,25 US-$ (Eingabe 21.729 × 1,5 $/M, Ausgabe inkl. Denken 28.863 × 7,5 $/M; Preisstand 21.11.2026)
> - Denkstufe: medium (thinking_level)
> - Speicherung bei Google: Projekteinstellung
> - finishReason: STOP
> - Eingelesen: notes/zweitmeinung_2026-11-24_auftrag.md (7.338 Zeichen), STATUS.md (44.899 Zeichen), src/construct_48.py (2.323 Zeichen)

---

# Zweitmeinung: Projekt Spencer-Diskrepanzkonstante (Stand 24.11.2026)

## Ergebnis zuerst

1. **Zustimmung zur Methodengrenze:** Der Orchestrator hat recht damit, die **blinde $d=2$-Nebenklassen-Suche** bei $(25,8)$ nicht zu starten – auch nicht auf der RTX 5080. Die Methode hat bei $(20,6)$ und $(22,7)$ primär deshalb gegriffen, weil dort die Hadamard-12-Familie liegt.
2. **Widerspruch zum sofortigen Abbruch der Rekordjagd:** Nach nur zwei Tagen einer sechswöchigen Timebox die Rekordjagd ganz zu beenden, ist **zu früh**. Die Negativbefunde tragen weniger weit als dargestellt:
   - Die Hadamard-Tests A und B haben eine **echte logische Lücke** ($20 \times 18$ und $24 \times 21$ wurden nicht ausgeschlossen, obwohl beide theoretisch Rekordniveau erreichen können).
   - Die bisherige Suche war fast ausschließlich auf **reine $\{\pm 1\}$-Matrizen mit additiver Nebenklassen-Symmetrie** beschränkt. Drei grundverschiedene, billig antestbare Routen sind völlig unberührt: (i) kontinuierliche/LP-Politur bei $n=16$ (wo nur $1{,}02\%$ zum Rekord fehlen), (ii) Permutations-Orbit-Suche ($5^4$/$5^5$) statt Nebenklassen, (iii) Konferenzmatrizen und der Hadamard-Lückenschluss.
   - Die Erfolgschance für einen neuen Spencer-Rekord bleibt realistisch gesehen **gering (insgesamt ca. 10–20 %)**, aber der Aufwand für diese gezielten Tests liegt bei wenigen Tagen mit klaren Abbruchkriterien.
3. **Tabellenergebnis sichern und melden:** Die explizite, suchfreie Konstruktion $K(2R+8, R) \le 48$ für alle $R \ge 6$ ist mathematisch korrekt (Induktion über Normalität), sauber und voraussichtlich meldewürdig. Vor einer Meldung (am besten als Issue/PR bei `florath/covering-codes-lean`) müssen aber noch die Normalität der **konstruktiven** Codes aus `src/construct_48.py` geprüft und die gesamte Diagonale sowie mögliche Folgecodes gegen die Post-Kéri-CSV abgeglichen werden.

---

## 1. Kritik an der Einschätzung des Orchestrators

### Wo der Orchestrator recht hat
- **Zellspezifischer Erfolg bei $(20,6)$ und $(22,7)$:** Die Vermutung des Orchestrators lässt sich sogar noch schärfer belegen als im Protokoll. `[eigene Rechnung]` Verdreifacht man im 48er-Code von $(20,6)$ mit Typ $(11,9)$ eine 0-Koordinate des Basisvektors $a$, entsteht bei $(22,7)$ exakt Typ $(11,11)$; verdreifacht man eine 1-Koordinate, entsteht Typ $(13,9)$. Dass bei $(22,7)$ genau $(11,11)$ in 8 von 8 Läufen leicht gelöst wurde `[belegt: STATUS.md]`, während $(13,9)$ 0 von 8 schaffte, zeigt direkt, dass die Suche auch bei $(22,7)$ vor allem die verdreifachte Hadamard-12-Struktur (plus eine überflüssige Nebenklasse) eingesammelt hat.
- **Keine blinde $d=2$-Suche bei $(25,8)$:** Mit $2^{23}$ Syndromen und einer Schrittzeit von ca. $0{,}5\text{ s}$ (Faktor 120 langsamer als $(20,6)$) `[belegt: STATUS.md]` ist eine blinde WHT-Suche aussichtslos. Selbst wenn eine RTX 5080 die 64-MB-WHT um den Faktor 15–30 beschleunigt, ändert das nichts daran, dass $d=2$ bei $(25,8)$ nur 48 statt 50 Wörter zulässt und kein struktureller Grund für eine Lösung in diesem Unterraum bekannt ist.

### Wo die Argumentation schwach ist und was übersehen wurde
1. **Logische Lücke in Test A (Ordnung 24) und Test B (Ordnung 20):**
   - Im Protokoll wird aus $\text{disc}(H_{24}) = 6$ (für alle 60 McKay-Klassen) geschlossen, dass Ordnung 24 abgeschlossen sei, weil jede $24 \times 23$-Untermatrix höchstens Diskrepanz 7 habe `[belegt: STATUS.md]`.
   - **Das schließt $24 \times 21$ (3 Spalten gestrichen) nicht aus!** `[eigene Rechnung]` Beim Streichen von 3 Spalten kann die Diskrepanz um bis zu 3 steigen, also theoretisch von 6 auf **9**. Eine $24 \times 21$-Matrix mit Diskrepanz 9 liefert $9/\sqrt{24} \approx 1{,}8371 > 1{,}7678$ (Rekord; entspricht $(21,6)$ mit 48 Wörtern, Überdeckungsdichte $48 \cdot 82\,160 / 2^{21} \approx 1{,}88$, knapp unter $(15,4)/32$ mit $1{,}90$).
   - **Ebenso bei Ordnung 20 (Test B):** $\text{disc}(H_{20}) = 6$ schließt nicht aus, dass eine $20 \times 18$-Untermatrix (2 Spalten gestrichen) Diskrepanz **8** hat `[eigene Rechnung]`. Das gäbe $8/\sqrt{20} \approx 1{,}7889 > 1{,}7678$ (Rekord; entspricht $(18,5)$ mit 40 Wörtern).
2. **Der „Rundgang“ beweist weniger als behauptet:**
   - Bei $(21,6)$ ist – wie im Protokoll selbst eingeräumt `[belegt: STATUS.md]` – unklar, ob überhaupt ein selbstkomplementärer 64er-Code mit 2-dim. Translationsstabilisator existiert.
   - Bei $(23,7)$ wurden in 1800 s nur ca. 30 000 Schritte gerechnet `[belegt: STATUS.md]`. Bei $(20,6)$ brauchten die $K=13$-Treffer aber 48 000 bis 342 000 Schritte `[belegt: STATUS.md]`. Ein Abbruch nach 30 000 Schritten bei $K=15$ ohne Kontrolle bei $K=16$ und ohne Warmstart zeigt nur, dass 30 Minuten CPU-Zeit zu kurz sind.
3. **Verengung auf reine $\{\pm 1\}$-Matrizen:**
   - Der aktuelle Rekord $5/\sqrt{8} \approx 1{,}7678$ nutzt eine Nullspalte (`8 x 7` effektiv) `[belegt: STATUS.md]`. Im Abschnitt „Offen und unbelegt“ wird pauschal behauptet, gebrochene Einträge und Nullen hülfen nur in Paritätsfällen wie $n=17, 19$ `[belegt: STATUS.md]`. Dabei wurde übersehen, dass **$n=16$ (`ctrl_16.txt`) mit 15 $\pm 1$-Spalten und einer Nullspalte genau ein solcher Fall ist** – und dort mit $7/\sqrt{16} = 1{,}7500$ nur **$1{,}02\%$** (Diskrepanz $+0{,}0711$) zum Rekord fehlen `[eigene Rechnung]`.

---

## 2. Konkrete, prüfbare Ideen für einen Spencer-Rekord

### 2a. Warum funktioniert die Hadamard-12-Struktur bei $n - 2R = 8$ – und warum nicht bei Ordnung 40?

`[eigene Rechnung]` Sei $M = \begin{pmatrix} H_L & H_R \\ H_L & -H_R \end{pmatrix}$ mit $H$ einer Hadamard-Matrix der Ordnung $h$, aus der links $d_L$ und rechts $d_R$ Spalten gestrichen wurden. Für jeden Vorzeichenvektor $x = (x_L, x_R)$ gilt mit $u = H_L x_L$ und $v = H_R x_R$:
$$\max_i |(Mx)_i| = \max_{1 \le i \le h} \max(|u_i + v_i|, |u_i - v_i|) = \max_{1 \le i \le h} (|u_i| + |v_i|).$$
Nach Parseval ($H^T H = h I$) gilt für den quadratischen Mittelwert der Einträge:
$$\frac{1}{h}\sum_{i=1}^h u_i^2 = h - d_L, \qquad \frac{1}{h}\sum_{i=1}^h v_i^2 = h - d_R.$$

- **Bei $h=12$, $1+3$ ($d_L=1, d_R=3$):** Alle $u_i, v_i$ sind **ungerade**, also $|u_i|+|v_i|$ **gerade**. Da $\frac{1}{12}\|u\|_2^2 = 11 > 3^2 = 9$, **muss** mindestens ein $|u_i| \ge 5$ sein. Da $|v_i| \ge 1$ ist, ist $\max(|u_i|+|v_i|) \ge 6$ bereits durch Parseval erzwungen. Für Diskrepanz $< 8$ müssten an allen Stellen mit $|u_i| = 5$ zwingend $|v_i| = 1$ sein, und es dürfte kein $|u_i| \ge 7$ auftreten. Durch die gemeinsame gestrichene Spalte $c_0 \in L \cap R$ stehen $u$ und $v$ zudem beide senkrecht auf $c_0$ (also $\sum u_i = \sum v_i = 0$ nach Zeilen-Normierung). In der kleinen Dimension $h=12$ kollidiert das mit der Kongruenz aller Einträge von $H\tilde{x} \pmod 4$ (für jede Hadamard-Matrix der Ordnung $4k$ sind alle Einträge von $Hx$ paarweise kongruent mod 4, da je zwei Zeilen an $2k$ Stellen differieren).
- **Warum Test C bei $h=20$ (Ordnung 40, $d_L=1, d_R=3$) scheitern musste:** Hier ist $\frac{1}{20}\|u\|_2^2 = 19 < 5^2 = 25$ und $\frac{1}{20}\|v\|_2^2 = 17 < 5^2 = 25$. Selbst für die viel strengere Schranke $|u_i|+|v_i| \le 8$ erlaubt das Paar $(1,7)$ ein Quadrat von $1^2+7^2 = 50 > 36 = 19+17$; für das Rekordziel $\text{disc} = 12$ (also Verhinderung von $\le 10$) liegen Paare wie $(5,5) \mapsto 50$ und $(3,7) \mapsto 58$ meilenweit über dem Parseval-Durchschnitt $36$.

**Konsequenz:** Die Diskrepanz 8 bei $h=12$ ist ein **Klein-Zahlen-Paritätseffekt**, weil $11$ knapp über $3^2$ liegt und der Schritt zur nächsten ungeraden Zahl ($+2$) relativ zu $\sqrt{12}$ groß ist. Er skaliert **nicht** auf $h=20$ (weder mit $P=I$ noch mit $P \neq I$ in $[[H, PH],[H, -PH]]$, da Parseval von $P$ unberührt bleibt). Ein direktes Gegenstück für Diskrepanz 9, 10 oder 11 ergibt sich aus diesem Mechanismus **nicht**.

---

### 2b & 2c. Vier konkrete, prüfbare Ideen (mit erstem Test, Abbruchkriterium, Chance)

#### Idee 1: Kontinuierliche LP-Politur bei $n=16$ (und Prüfung kleiner Konferenzmatrizen)
- **Mathematik:** `[eigene Rechnung]` `ctrl_16.txt` ($16 \times 15$ in $\{\pm 1\}$ plus Nullspalte) erreicht $7/\sqrt{16} = 1{,}7500$ `[belegt: STATUS.md]`. Zum Rekord fehlen nur $+0{,}0711$. Zählt man alle kritischen Vektoren $X_{\text{krit}} = \{x \in \{\pm 1\}^{15} : \|A_0 x\|_\infty = 7\}$ auf (nur $2^{15} = 32\,768$ Vektoren insgesamt!), kann man exakt prüfen, ob eine nicht-triviale 16. Spalte $c \in [-1,1]^{16}$ zusammen mit kleinen Störungen $\Delta A$ (via lokalem LP/Alternating Optimization oder MILP) $\text{disc}(A) > 7{,}0711$ erreicht. Eine notwendige Bedingung für reine Spalten-Ergänzung ist, dass kritische $x$ von mindestens zwei Zeilen mit entgegengesetztem $c_i \cdot \text{sgn}((A_0 x)_i)$ auf 7 gehalten werden; bei gleichzeitiger Variation aller Einträge von $A \in [-1,1]^{16 \times 16}$ entfällt selbst diese Einschränkung.
  Zusätzlich: Die **Konferenzmatrizen** $C_{10}$ und $C_{14}$ ($C_n C_n^T = (n-1)I$, Diagonale 0, Rest $\pm 1$) haben ungerade Zeilensummen. `[eigene Rechnung]` Bei $C_{14}$ ist $\frac{1}{14}\|C_{14}x\|_2^2 = 13 > 3^2$, also $\text{disc}(C_{14}) \ge 5$ trivial; falls $\text{disc}(C_{14}) = 7$ wäre, ergäbe das sofort $7/\sqrt{14} \approx 1{,}8708$ (Rekord!).
- **Erster billiger Test (15–30 Min., 4 Kerne):**
  1. Exakte Diskrepanz von $C_{10}, C_{14}$ (Paley-Konferenzmatrizen) und $C_{18}$ per Meet-in-the-middle berechnen (< 1 Min.).
  2. Auf `ctrl_16.txt` (und 2–3 anderen $(15,4)/32$-Codes) $X_{\text{krit}}$ aufzählen, Häufigkeit von $|I(x)| = 1$ messen, und ein einfaches lokales LP (Iterative Linearisierung / Cutting-Plane um $A_0$) laufen lassen.
- **Abbruchkriterium:** $\text{disc}(C_{14}) = 5$ (erwartbar, da $14 \cdot 5^2 = 350 \gg 182$); und die LP-Politur bei $n=16$ klebt nach 20 Iterationen bei $\le 7{,}00$ fest (oder verbessert nicht über $7{,}01$).
- **Geschätzte Erfolgswahrscheinlichkeit:** **5–15 %** (höchste Einzelchance unter allen Ideen, weil nur 1 % fehlt und das kontinuierliche Regime bisher komplett ungetestet ist).

#### Idee 2: Lückenschluss Hadamard-Spaltenstreichung ($20 \times 18$ und $24 \times 21$)
- **Mathematik:** `[eigene Rechnung]` Siehe Abschnitt 1: $20 \times 18$ aus $H_{20}$ (Diskrepanz 8 $\implies 1{,}7889$) und $24 \times 21$ aus $H_{24}$ (Diskrepanz 9 $\implies 1{,}8371$).
- **Erster billiger Test (15–45 Min., 4 Kerne):**
  1. Alle $\binom{20}{2} = 190$ Spaltenpaare der 3 Klassen von $H_{20}$ (+ Transponierte = 1140 Matrizen) mit einem schnellen C-Gegenbeispielsucher auf $T=6$ prüfen (< 2 Min.).
  2. Für 5–10 Klassen aus `controls/had24_mckay_hex.txt` (inkl. Paley 24 und $M(H_{12})$) alle $\binom{24}{3} = 2024$ Spaltentripel per Gegenbeispielsucher auf $T=7$ prüfen (~30 Min.).
- **Abbruchkriterium:** Für alle getesteten Matrizen wird ein Gegenbeispiel mit $\le 6$ (bei $20 \times 18$) bzw. $\le 7$ (bei $24 \times 21$) gefunden.
- **Geschätzte Erfolgswahrscheinlichkeit:** **1–3 %** (sehr niedrig wegen enger Dichte, aber minimaler Aufwand und schließt die Hadamard-Route logisch sauber ab).

#### Idee 3: Permutations-Orbit-Suche ($5^4$ bei $(20,6)/40$ und $5^5$ bei $(25,8)/50$) – hier lohnt die GPU
- **Mathematik:** Statt additiver Nebenklassen ($d=2$, die bei $(25,8)$ nur 48 statt 50 Wörter zulassen) wird eine **Koordinaten-Permutation** $\sigma$ vom Zykeltyp $5^4$ (bei $n=20$) bzw. $5^5$ (bei $n=25$) plus Komplementbildung vorgegeben (entspricht dem ungetesteten Astra-Vorschlag `[belegt: STATUS.md]`). Eine volle Bahn hat 10 Wörter; man sucht nur **4 Bahnen** bei $(20,6)$ bzw. **5 Bahnen** bei $(25,8)$.
  - Vorteil gegenüber WHT: Keine 64-MB-`int64`-Transformation nötig! Ein Überdeckungs-Bitset für $2^{25}$ Punkte belegt nur **4 MB** (passt in den 96-MB-L3-Cache des Ryzen 9800X3D und erst recht in die RTX 5080). Im Orbit-Raum ($\mathbb{F}_2^n / \langle \sigma, \mathbf{1} \rangle$) gibt es bei $n=20$ nur ca. $105\,000$ Orbits, bei $n=25$ ca. $3{,}35 \cdot 10^6$ Orbits.
- **Erster billiger Test (30–60 Min., 4 Kerne, noch ohne GPU):**
  1. **Positivkontrolle:** $(15,4)$ mit 32 Wörtern unter Zykeltyp $5^3$ + Komplement (3 Bahnen à 10 + $\{0, \mathbf{1}\}$).
  2. **Machbarkeitstest:** $(20,6)$ mit 40 Wörtern unter Zykeltyp $5^4$ + Komplement (4 Bahnen à 10, nur $105\,000$ Orbits; Simulated Annealing / Tabu-Suche über 4 Repräsentanten).
- **Abbruchkriterium:** Scheitert die Positivkontrolle $(15,4)/32$ oder bleibt $(20,6)/40$ nach 60 Minuten weit von einer Überdeckung entfernt (> 200 unüberdeckte Orbits), lohnt sich die GPU-Portierung für $(25,8)/50$ nicht.
- **Geschätzte Erfolgswahrscheinlichkeit:** **5–10 %** (erfordert die Annahme, dass ein Rekordcode eine 5-Zyklen-Symmetrie besitzt, aber gerade nichtlineare Überdeckungscodes weisen oft solche Gruppen auf `[unbelegt / Standardwissen aus dem Gedächtnis]`).

#### Idee 4: Verwerfen von $[[H, PH], [H, -PH]]$ bei Ordnung 40 und 3-Block-Systemen
- Wie in 2a begründet, verhindert die Parseval-Skalierung bei Ordnung 40 einen Erfolg. Bei $3 \times 3$-Blöcken fehlt die Orthogonalität. **Empfehlung: Nicht testen (Erfolgswahrscheinlichkeit < 1 %).**

---

## 3. Bewertung des Tabellenergebnisses ($K(2R+8, R) \le 48$)

### Ist die Aussage „$K(2R+8, R) \le 48$ für alle $R \ge 6$“ korrekt?
**Ja, vollständig korrekt.** `[eigene Rechnung]` Sei $C \subseteq \mathbb{F}_2^n$ normal in Koordinate 1 mit Norm $\le 2R+1$, also $d(x, C_0) + d(x, C_1) \le 2R+1$ für alle $x \in \mathbb{F}_2^n$. Verdreifacht man Koordinate 1 zu $C' = \{(c_1, c_1, c_1, c_{\ge 2}) : c \in C\} \subseteq \mathbb{F}_2^{n+2}$, so gilt für jedes $y = (y_a, y_b, y_c, z) \in \mathbb{F}_2^{n+2}$ mit $w = y_a + y_b + y_c$:
$$d(y, C'_0) + d(y, C'_1) = \bigl(w + d(z, (C_0)_{\ge 2})\bigr) + \bigl(3 - w + d(z, (C_1)_{\ge 2})\bigr) = 3 + d(z, (C_0)_{\ge 2}) + d(z, (C_1)_{\ge 2}).$$
Für $x = (0, z) \in \mathbb{F}_2^n$ gilt $d(x, C_0) + d(x, C_1) = 1 + d(z, (C_0)_{\ge 2}) + d(z, (C_1)_{\ge 2}) \le 2R+1$, also $d(z, (C_0)_{\ge 2}) + d(z, (C_1)_{\ge 2}) \le 2R$. Daraus folgt:
$$d(y, C'_0) + d(y, C'_1) \le 2R + 3 = 2(R+1) + 1.$$
Damit hat $C'$ sowohl Überdeckungsradius $\le \lfloor (2R+3)/2 \rfloor = R+1$ als auch wieder Norm $\le 2(R+1)+1$ in den neuen Koordinaten. Per Induktion folgt $K(2R+8, R) \le 48$ für alle $R \ge 6$ – ein Brute-Force-Nachrechnen von $(28,10)$ ist für den Beweis gar nicht nötig (höchstens als zusätzlicher Plausibilitätstest).

### Folgen weitere Tabellenzellen über Standardkonstruktionen?
1. **Gesamte Diagonale $n - 2R = 8$:** Neben $(20,6), (22,7), (24,8), (26,9), (28,10)$ auch $(30,11), (32,12)$ usw. (Prüfung gegen die Post-Kéri-CSV nötig, die für $q=2$ traditionell bis $n=33$ reicht `[unbelegt / Gedächtnis]`).
2. **Amalgamierte direkte Summe (ADS) mit anderen normalen Codes:** `[eigene Rechnung]` Ist $C_1 = (20,48)6$ normal und $C_2 = (n_2, M_2)R_2$ ein normaler Code, liefert die ADS einen $(19 + n_2, 24 \cdot M_2)(6 + R_2)$-Code. Konkrete Kandidaten, die gegen die CSV geprüft werden sollten:
   - Mit dem perfekten Hamming-Code $(7,16)1$ (normal `[unbelegt / Gedächtnis]`): **$K(26, 7) \le 384$** (und via Verdreifachen $K(26+2k, 7+k) \le 384$).
   - Mit $(6,12)1$ (normal `[unbelegt / Gedächtnis]`): **$K(25, 7) \le 288$** (und $K(25+2k, 7+k) \le 288$).
   - Mit $(5,8)1$: $K(24,7) \le 192$.
   - Mit $(9, 16)2$ o. ä.: $K(28,8) \le 384$.
   Ein 5-Minuten-Skript gegen `non_mixed_covering_codes.csv` klärt sofort, welche dieser Zellen unter dem aktuellen Tabellenwert liegen.

### Belastbarkeit der Neuheit & fehlende Prüfungen vor einer Meldung
- **Belastbarkeit:** **Mittel bis hoch.** Dass `florath/covering-codes-lean` (Stand Sept. 2026) und EinsteinArena (Sept. 2026) $K(20,6) \le 64$ führen `[belegt: STATUS.md]`, ist ein starkes Indiz. Das Restrisiko liegt darin, dass die Konstruktion über $M(H_{12})$ sehr elementar ist und theoretisch in einer Arbeit über den Überdeckungsradius von Hadamard-Codes (z. B. Honkala, Lobstein, Östergård, Cohen) erwähnt worden sein könnte, ohne in Kéris Tabelle übernommen zu werden.
- **Zwingend fehlende Prüfungen vor Meldung:**
  1. **Normalität der konstruktiven Codes:** `normal_ext.py` muss auf `controls/had24_1p3_paley12.txt` und `controls/had24_2p2_paley12.txt` aus `src/construct_48.py` laufen! Bisher ist Normalität nur für die Such-Codes `aus52seed{5,6}` protokolliert `[belegt: STATUS.md]`. Erst damit ist der Beweis der gesamten Familie vollständig **suchfrei**.
  2. **Vollständiger CSV-Abgleich:** Alle Zellen $(2R+8, R)$ bis $n=33$ sowie die ADS-Folgezellen (oben) gegen `non_mixed_covering_codes.csv` abgleichen.
  3. **Gezielte Literatur-Nachprüfung (Gemini):** Spezifisch nach „covering radius“, „Hadamard matrix of order 12“, „doubling“ / „Sylvester“ und „48“ suchen (nicht nur nach „$K(20,6)$“), um zu prüfen, ob die Hadamard-Konstruktion unter anderem Namen bekannt ist.
- **Ort und Form der Meldung:** Ein **Issue oder Pull Request bei `florath/covering-codes-lean`** ist der richtige, risikoarme erste Ort. Meldet dort die **suchfreie Konstruktion** (`construct_48.py`) samt einem 30-zeiligen, deterministischen Brute-Force-Verifizierer für Radius 6 und Norm 13 über $\mathbb{F}_2^{20}$ plus dem 5-zeiligen Induktionsbeweis für $R \ge 6$. Legt den KI-Einsatz nüchtern offen (Suche/Analyse mit KI-Unterstützung, Ergebnis rein konstruktiv und deterministisch verifiziert) und fragt explizit nach, ob diese Konstruktion in der Literatur bereits bekannt ist.

---

## 4. Priorisierter Plan bis 04.01.2027 (max. 5 Punkte)

Ich halte **„jetzt sofort komplett aufhören“ für verfrüht**, aber **„Tabellenergebnis zuerst absichern und melden, dann noch 2–3 Wochen gezielt die drei ungetesteten Rekord-Routen abarbeiten und bei negativem Ausgang sauber schließen“** für die beste Strategie.

| Prio | Aufgabe | Aufwand | Nutzen | Risiko |
|---|---|---|---|---|
| **1** | **Tabellenergebnis finalisieren:** Normalität von `had24_1p3_paley12` & `had24_2p2_paley12` per Brute-Force prüfen, gesamte Diagonale + ADS-Folgezellen gegen `non_mixed_covering_codes.csv` abgleichen, gezielte Hadamard-Literatursuche, dann Issue/PR bei `florath/covering-codes-lean` vorbereiten (Entscheidung 27.11.). | 0,5–1 Tag (Minuten Rechenzeit) | **Hoch** (sichert das einzige bisherige echte Resultat sauber und suchfrei ab). | Gering (schlimmstenfalls stellt sich heraus, dass es bekannt war – durch die sachliche Frageform im Issue völlig schmerzfrei). |
| **2** | **Hadamard-Lückenschluss ($20 \times 18$, $24 \times 21$) & Konferenzmatrizen ($C_{10}, C_{14}, C_{18}$):** Schnelle Prüfung der übersehenen Spaltenstreichungen und der Paley-Konferenzmatrizen. | 0,5 Tag (< 1 h Rechenzeit, 4 Kerne) | **Mittel** (schließt die algebraische Route logisch wasserdicht ab; kleine Chance auf Sofortrekord). | Sehr gering (klarer Endpunkt). |
| **3** | **LP-Politur bei $n=16$ (`ctrl_16.txt` u. a.):** Aufzählung von $X_{\text{krit}}$ ($2^{15}$ Punkte), Test auf Spaltenergänzung + kontinuierliche lokale Optimierung in $[-1,1]^{16 \times 16}$ (Ziel: $> 7{,}0711$). | 1–2 Tage (< 1 h Rechenzeit) | **Hoch** (höchste Einzelchance auf einen Spencer-Rekord, da nur $1{,}02\%$ fehlen; erschließt das nicht-binäre Regime). | Mittel (könnte bei $7{,}00$ festkleben, dann sofort Abbruch). |
| **4** | **Permutations-Orbit-Suche (Kontrolle $(15,4)/32$, dann $(20,6)/40$ mit $5^4$):** Testet den ungetesteten Symmetrie-Ansatz auf 4 Cloud-Kernen. **Nur bei positivem Verlauf** Portierung auf RTX 5080 / 9800X3D für $(25,8)/50$ ($5^5$). | 2–3 Tage (ohne GPU); +3–5 Tage nur falls Gate bestanden | **Mittel bis Hoch** (einzige plausible kombinatorische Route zu $(25,8)/50$, bei der die lokale Hardware sinnvoll eingesetzt wird). | Mittel (wenn die Positivkontrolle oder $(20,6)/40$ scheitert, wird der GPU-Aufwand komplett gespart). |
| **5** | **Hard-Stopp & Abschlussbericht (spätestens Mitte/Ende Dezember):** Wenn Punkte 2–4 ohne Rekord enden, Projekt sauber dokumentieren (inkl. der Negativbefunde) und beenden – keine blinde Verlängerung. | 1 Tag | **Hoch** (verhindert versunkene Zeit in aussichtsloser Suche). | Kein Risiko. |
