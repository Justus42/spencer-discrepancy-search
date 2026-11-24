# Geminis Neuheitsbericht zu K(20,6) <= 48, Kurzfassung (24.11.2026)

- Quelle: Gemini über Justus' App, normaler Chat, ohne Deep Research. Beim Einfügen gingen die Links teils verloren.
- Status: Aussagen eines anderen Modells. Nichts davon gilt als belegt, außer es steht unten unter „Vom Orchestrator-Claude nachgeprüft“.

## Prompt

```
Du bist feindseliger Gutachter. Behauptung: ein selbstkomplementärer,
normaler binärer Code der Länge 20 mit 48 Wörtern und Radius 6,
Vereinigung von 12 Nebenklassen von {0, a, Komplement(a), 1} mit
Gewicht(a) = 11. Daraus K(2R+8, R) <= 48 für R >= 6. Tabellenstand:
K(20,6), K(22,7), K(24,8) <= 64; K(26,9), K(28,10) <= 56
(Östergård–Kaikkonen 1998).
1. Mit welcher Konstruktion erhielten Östergård und Kaikkonen die 56
   bei (26,9) und (28,10)? Liefert sie auch etwas für (20,6)?
2. Gibt es eine bekannte Konstruktion (blockweise direkte Summe,
   subnormale Codes, stückweise konstante Codes, Matrixmethode),
   die 48 oder weniger für (20,6) oder diese Familie ergibt?
3. Gibt es 2025–2026 Veröffentlichungen (arXiv, Zenodo, GitHub,
   fohn.app) zu K(20,6), K(22,7), K(24,8) oder K(26,9)?
Kennzeichne jede Aussage als belegt (mit Link) oder unbelegt.
Erfinde keine Quellen. "Nichts gefunden" ist eine gültige Antwort.
```

## Vom Orchestrator-Claude nachgeprüft

- 24.11.2026: EinsteinArena-Thread 273 gelesen (einsteinarena.com/problems/spencer-discrepancy/threads/273, FableCover, 21.09.2026): gleiche Reduktion auf selbstkomplementäre Überdeckungscodes; nur n <= 20 betrachtet; Latte dort 7/sqrt(17); K(20,6) als „known 24..64“, Ziel <= 40 nicht erreicht; kein Code unter 64 gemeldet; (25,8), (23,7), (30,10) kommen nicht vor; Plateau 466 bei (15,4) mit 30 Wörtern wie in unseren Fehlversuchen. Die Seite zeigt nur relative Daten und blendet drei Antworten aus.
- 24.11.2026: `Mapika/coldcase` geklont (letzter Commit 24.08.2026): Ergebnisse nur für q >= 5, keine binären. Die binäre Kéri-Tabelle liegt dort als Zieldaten vor, die Methode umfasst Nebenklassen-Vereinigungen.
- Standardkonstruktionen für 48, überschlagen: direkte Summe bräuchte K(19,6) <= 24 (Tabelle 32); K(18,6) <= 12, K(17,6) <= 6 und die ADS-Bausteine (16,5), (14,4), (12,3) <= 13 scheitern an den unteren Schranken.
- Die Auflagen 1 und 2 des Berichts (Code vorlegen, Normalität erschöpfend prüfen) sind im Repo erfüllt, siehe STATUS.
- Volltext Östergård–Kaikkonen 1998: nicht frei zugänglich, Konstruktion der 56 bei (26,9) und (28,10) bleibt unbelegt.

## Bericht (unverändert)

Gutachter-Vorprüfung der Behauptung: In dieser Form nicht annahmefähig.
Ohne explizite Angabe von a, den 12 Nebenklassenvertretern und einem unabhängigen erschöpfenden Verifikationsprotokoll über alle 2^{20} = 1{,}048{,}576 Wörter ist die Existenz des behaupteten (20, 48)6-Codes unbelegt. Ebenso ist die behauptete Normalität (N(C, i) \le 13 für mindestens eine Koordinate i) unbelegt: Da \mathbf{1} \in H = \{0, a, \bar{a}, 1\} gilt, ist zwar jede Vereinigung von 12 disjunkten Nebenklassen des linearen [20,2]-Untercodes H (mit Gewichten 0, 9, 11, 20) automatisch selbstkomplementär und hat in jeder Koordinate ausgeglichene Teilcodes \vert{}C_0^{(i)}\vert{} = \vert{}C_1^{(i)}\vert{} = 24 (unbelegt durch externen Link; elementare Rechnung), daraus folgt aber nicht automatisch d(x, C_0^{(i)}) + d(x, C_1^{(i)}) \le 13 für alle x \in \mathbb{F}_2^{20}. Erst wenn Normalität für mindestens eine Koordinate erschöpfend geprüft ist, greift die amalgamierte direkte Summe (ADS) mit dem normalen Wiederholungscode \{000, 111\} (belegt für das Konzept der Normalität und ADS: Graham & Sloane 1985, Kéri – Normality), die rekursiv normale (20+2k, 48)(6+k)-Codes und damit K(2R+8, R) \le 48 für alle R \ge 6 erzwingt (unbelegt durch externen Link; elementare Induktion über d_0 + d_1 + 3 \le 2R + 3). Der zitierte Tabellenstand K(20,6) \le 64 deckt sich immerhin mit der Sekundärquelle EinsteinArena Thread 273 (Sept. 2026) (known 24..64) (belegt als Sekundärzitat), ist aber gegenüber Kéris Primärtabelle hier nicht direkt eingesehen (unbelegt).
1. Östergård & Kaikkonen (1998): Herkunft der 56 bei (26,9) und (28,10) sowie Relevanz für (20,6)
 * Existenz und Gesamtprofil der Arbeit: Das Paper ist P. R. J. Östergård & M. K. Kaikkonen, New upper bounds for binary covering codes, Discrete Mathematics 178 (1998), 165–179, DOI 10.1016/S0012-365X(97)81825-0 (belegt: Östergård Publikationsliste, DBLP, Aalto Research Portal, SciSpace DOI-Verweis).
 * Allgemeine Methoden der Arbeit: Laut Metadaten und Abstract nutzt die Arbeit sowohl neue als auch ältere Konstruktionen, darunter nichtlineare Codes, Tabu-Suche und Simulated Annealing, ergänzt um einzelne lineare Codes und eine aktualisierte Tabelle für n \le 64, r \le 12 (belegt: Aalto Research Portal, Semantic Scholar Abstract, Bohrium/Kaikkonen).
 * Konkrete Konstruktion für K(26,9) \le 56 und K(28,10) \le 56: Nichts gefunden (unbelegt). Der Volltext von Discrete Mathematics 178 (1998), 165–179, ist nicht frei indexiert abrufbar, und die binären Tabellendateien unter Kéris Tabellenindex bzw. Kéris Verzeichnis Binary/ zeigten in den abrufbaren Suchergebnissen weder die Zellen (26,9)/(28,10) noch deren Konstruktionsschlüssel.
 * Liefert diese Konstruktion etwas für (20,6)? Nichts gefunden (unbelegt), da die exakte Konstruktion aus dem Volltext von 1998 nicht eingesehen werden konnte. Als Indiz (kein Primärbeweis) spricht EinsteinArena Thread 273 dafür, dass der bekannte Stand für K(20,6) weiterhin bei 64 liegt (belegt als Sekundärquelle), die 1998er Konstruktion also für (20,6) offenbar nichts unter 64 lieferte (unbelegt).
 * Gutachter-Auflage: Vor jeder Einreichung müssen Sie den Volltext von Östergård & Kaikkonen (1998) sowie den exakten Source-Key der Zellen (20,6), (22,7), (24,8), (26,9) und (28,10) in Kéris Binärtabelle direkt einsehen und zitieren.
2. Bekannte Konstruktionen mit \le 48 für (20,6) oder die Familie K(2R+8, R)
 * Gesamtergebnis zu \le 48: Nichts gefunden (unbelegt hinsichtlich absoluter Vollständigkeit, da weder Cohen–Honkala–Litsyn–Lobstein (1997) noch Kéris vollständige Binärtabelle im Volltext vorlagen). Keine der durchsuchten Literatur-, Preprint- oder Diskussionsquellen nennt eine Konstruktion mit K(20,6) \le 48 oder K(2R+8, R) \le 48 für R \ge 6.
 * Einordnung der vier abgefragten Methodengruppen:
   * Blockweise direkte Summe (BDS): Als Standardkonstruktion für Überdeckungscodes etabliert (belegt: Etzion & Greenberg / ResearchGate, GAP/GUAVA-Dokumentation). Kein Beleg für ein Resultat \le 48 bei (20,6) oder K(2R+8, R) (Nichts gefunden).
   * Subnormale / (k,t)-subnormale Codes: Als Methode von Honkala und Östergård etabliert (belegt: Östergård 1992 auf Publikationsliste, Honkala / Colab.ws). Kein Beleg für \le 48 bei (20,6) oder K(2R+8, R) (Nichts gefunden).
   * Stückweise konstante Codes (Piecewise Constant Codes): Als eigenes Kapitel im Standardwerk Cohen, Honkala, Litsyn, Lobstein (Covering Codes, North-Holland 1997) behandelt (belegt: Elsevier, Hugendubel-Inhaltsangabe). Kein Beleg für \le 48 bei (20,6) oder K(2R+8, R) (Nichts gefunden).
   * Matrixmethode: Als Standardansatz zur Suche nach Nebenklassenvereinigungen linearer Untercodes etabliert (belegt: Östergård – AJC Vol. 12, aktuell auch bei K(20,3) auf fohn.app Task #10). Kein Beleg für \le 48 bei (20,6) oder K(2R+8, R) (Nichts gefunden).
 * Gutachter-Hinweis zur eigenen Behauptung: Ihr eigener Ansatz (12 Nebenklassen des [20,2]-Codes H = \{0, a, \bar{a}, 1\}) ist selbst ein Spezialfall der Matrixmethode mit einer 18 \times 20-Kontrollmatrix und \vert{}S\vert{} = 12 Syndromen (unbelegt durch externen Link; direkte lineare Algebra). Ferner schließt die Kugelüberdeckungsschranke 48 nicht aus (\lceil 2^{20} / \sum_{k=0}^6 \binom{20}{k} \rceil = \lceil 1{,}048{,}576 / 60{,}460 \rceil = 18; unbelegt durch externen Link; elementare Rechnung). Ob triviale Produkt-/BDS-Zerlegungen (48 = 2\cdot 24 = 3\cdot 16 = 4\cdot 12 = 6\cdot 8) über bekannte kleinere Tabellenwerte ausgeschlossen sind, muss am Primärbestand von Kéris Tabelle nachgewiesen werden (unbelegt).
3. Veröffentlichungen 2025–2026 (arXiv, Zenodo, GitHub, fohn.app) zu K(20,6), K(22,7), K(24,8) oder K(26,9)
Kurzbefund: Zu neuen oberen Schranken für genau K(20,6), K(22,7), K(24,8) oder K(26,9) wurde auf allen vier Plattformen nichts gefunden. Mehrere 2025–2026-Arbeiten bearbeiten jedoch benachbarte Zellen oder untere Schranken und müssen als Abgrenzung geprüft werden:
| Plattform | Gefundene 2025–2026-Quellen im Themenfeld (alle belegt) | Bezug zu K(20,6), K(22,7), K(24,8), K(26,9) |
|---|---|---|
| arXiv | Gijswijt & Polak (Apr. 2025 / v2 Juni 2026), Semidefinite lower bounds for covering codes: arXiv:2504.01932, PDF | Nur untere Schranken (belegt). Enthält „Table 5: Bounds for q = 2 and R \le 6“ (belegt). Ob (20,6) darin eine neue untere Schranke erhält, war nicht einsehbar (unbelegt – zwingend prüfen!). |
| arXiv | Davydov, Marcugini & Pambianco (Nov. 2025), New upper bounds for binary linear covering codes: arXiv:2511.02542 | Nur binär linear für R \in \{2,3,4\} (belegt). Kein Bezug zu R \ge 6. |
| arXiv | Florath (Juni 2026), Formal Foundations and Proof-Carrying Certificates for q-ary Covering Codes in Lean 4: arXiv:2606.09600 | Lean-4-Formalisierung/Datenbank (belegt). Zu (20,6)–(26,9): Nichts gefunden. |
| arXiv | Marosi (Aug./Sept. 2026), New upper and lower bounds on covering codes K_q(n,R) for alphabets of size 5 \le q \le 21: arXiv:2608.19872, v3 HTML | Nur 5 \le q \le 21, nicht binär (belegt). Bestätigt zudem, dass Kéris Tabellen zuletzt im Nov. 2011 aktualisiert wurden (belegt). |
| arXiv | Wu (Aug. 2026), Covering 1024 syndromes with 50 columns: arXiv:2608.27494; Davydov et al. (Sept. 2026), Further results on binary codes of covering radius 2: arXiv:2609.16078 | Nur binär linear mit R = 2 (belegt). |
| arXiv | Himelfarb et al. (Aug. 2026), On Nearly-Perfect Covering Codes Beyond Radius One: arXiv:2608.12595; Etzion (Juli 2026): arXiv:2607.14840; Xie et al. (Juni 2026): arXiv:2606.15379 | Andere Fragestellungen (Van-Wee-Gleichheit, Covering Sequences, Insertion/Deletion) (belegt). |
| Zenodo | Certified Branch Reductions for the Binary Covering Number K_2(11,3) (Sept. 2026): Zenodo 22647766 | Betrifft ausschließlich K_2(11,3) (belegt). Zu (20,6)–(26,9): Nichts gefunden. |
| GitHub | Mapika/coldcase (GPU-Rekordsuche, Aug. 2026); Floraths Repo zitiert in THIRD_PARTY_NOTICES.md; SeverinVisionary (K(8,1,2)); Noah-Parsons (gemischte Codes) | Alle vier Repos sind belegt. Ob Mapika/coldcase auch binäre Zellen wie (20,6) anfasst, war aus dem Snippet nicht ablesbar (unbelegt – Repo-Inhalt direkt prüfen!). Konkrete Treffer zu (20,6)–(26,9): Nichts gefunden. |
| fohn.app | Task #10 / Discovery (29.–30. Okt. 2026): Binärer Überdeckungscode für K(20,3) \le 1536 (Matrixmethode, nicht peer-reviewed): fohn.app/t/10, fohn.app/discoveries/covering-code-k20-3 | Betrifft K(20,3), nicht K(20,6) (belegt). Zu (20,6), (22,7), (24,8), (26,9) auf fohn.app: Nichts gefunden. |
| Sonstiges (EinsteinArena) | Thread Covering-code reformulation (21. Sept. 2026): EinsteinArena #273 | Nennt K(20,6) explizit im Spencer-Diskrepanz-Kontext mit Stand known 24..64, also kein Wert unter 64 bekannt (belegt als Forumseintrag). |
Gutachter-Checkliste vor Wiedervorlage (alle offenen Punkte aktuell unbelegt)
 * Explizites Artefakt & Verifikation: Geben Sie a \in \mathbb{F}_2^{20} (\text{wt}(a) = 11) und die 12 Nebenklassenvertreter maschinenlesbar an; verifizieren Sie unabhängig (z. B. per vollständiger 2^{20}-Bitmaske und separatem Distanzscan) sowohl \vert{}C\vert{} = 48 als auch \max_{x \in \mathbb{F}_2^{20}} d(x, C) = 6.
 * Normalitätszertifikat: Benennen Sie explizit die akzeptable Koordinate i \in \{1,\dots,20\} und belegen Sie erschöpfend über alle 2^{19} = 524{,}288 Vektoren y \in \mathbb{F}_2^{19}, dass d(y, A_0) + d(y, A_1) \le 12 gilt. Ohne diesen Nachweis bricht die gesamte Folgebehauptung K(2R+8, R) \le 48 für R \ge 6 zusammen.
 * Primärquellen-Audit: Prüfen Sie im Volltext (a) Östergård & Kaikkonen (1998), Discrete Math. 178, 165–179, für die exakte Herleitung der 56 bei (26,9) und (28,10), (b) Kéris Binärtabelle unter old.sztaki.hu/~keri/codes/ für alle Zellen (20,6) bis (28,10) und die kleineren Bausteinzellen, (c) Table 5 von arXiv:2504.01932 für aktuelle untere Schranken bei q=2, R \le 6 sowie (d) das Verzeichnis cov/ in Mapika/coldcase.
