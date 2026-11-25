# Prompt für Gemini Deep Research (25.11.2026), von Justus abzuschicken

```
Rolle: Du bist feindseliger Gegenprüfer und Literaturrechercheur. Auftrag: Literatur und Methodentransfer für eine
Rekordsuche. Belege jede Aussage mit Quelle und Link; trenne "im Volltext gelesen", "nur Abstract/Zitat" und
"eigene Vermutung". Erfinde nichts. Wenn du zu einem Punkt nichts findest, schreibe "nichts gefunden". Antworte auf Deutsch.

HINTERGRUND
Gesucht ist eine n x n-Matrix A mit Einträgen in [-1,1] und disc(A)/sqrt(n) > 5/sqrt(8) = 1,7678, wobei
disc(A) = min über x in {-1,1}^n von max_i |<a_i,x>| (Konstante C_10c in github.com/teorth/optimizationproblems,
constants/10c.md; Rekord: 8 x 8 aus dem Hamming-Code [7,4] plus Nullspalte, Diskrepanz 5).
Reduktion: Eine +-1-Matrix mit N Zeilen und m Spalten hat disc >= m - 2R genau dann, wenn die Hamming-Kugeln vom
Radius R um alle Zeilen und ihre Komplemente den Würfel überdecken. Ein Rekord braucht einen selbstkomplementären
Überdeckungscode der Länge m und vom Radius R = (m-d)/2 mit höchstens 2*B Wörtern, m <= B, B = B(d): 15, 20, 25, 31, 38
für d = 7, 8, 9, 10, 11. Zielzellen (m,R) mit erlaubter Wortzahl und bekannter Schranke K(m,R):
(15,4): 30 erlaubt, bekannt 23-32. (20,6): 40 erlaubt, Tabelle 24-64 (wir haben 48 aus einer verdoppelten
Paley-Hadamard-Matrix der Ordnung 12 mit gestrichenen Spalten). (25,8): 50 erlaubt, 25-64. (30,10): 62 erlaubt, 27-128.
(37,13): 76 erlaubt, außerhalb der Tabellen.
Schon ausgeschlossen oder erfolglos durchsucht: Hadamard-Matrizen der Ordnung 20 und 24 (auch mit gestrichenen Spalten),
zirkulante und gruppenentwickelte Matrizen bis Ordnung 31, negazyklische und Williamson-artige Matrizen der Ordnung 18-30
(nur Vortest), Codes mit zyklischer Spaltensymmetrie der Ordnung 5, 7, 11, 13, Codes im affinen Spann der bekannten guten Codes.

FRAGEN
1. Herkunft der Tabellenwerte. Woher stammen K(15,4) <= 32, K(13,3) <= 42, K(16,4), K(18,5), K(20,6) <= 64,
   K(19,5) bis K(25,8) <= 64, K(30,10) <= 128 (Autor, Jahr, Bauart: linear, amalgamierte direkte Summe, Matrixmethode,
   Suche)? Gibt es irgendwo, auch in Preprints, Dissertationen, Tabellen (Kéri, Östergård, "post-Kéri"), Foren oder
   Code-Repos, bessere Werte für diese Zellen, insbesondere K(20,6) < 64 oder K(25,8) < 64? Wurden diese Zellen mit
   großem Radius je gezielt mit nichtlinearen Codes angegriffen?
2. Die Folge k(d) = min über R von K(2R+d, R) (Länge minus doppelter Radius fest). Bekannt sind 2, 4, 7, 12, 16 für
   d = 1..5. Gibt es Literatur zu k(6), k(7), k(8), ..., zu Schranken oder zum Wachstum in d? Stichworte: Überdeckungsradius
   von Codes mit wenigen Wörtern, "covering radius of codes of size 2n", K(n,R) für R nahe n/2, t(n,k) für kleine k.
3. Codes als Vereinigung weniger Nebenklassen eines kleinen Kerns {0, a, 1+a, 1} ("u|u+v"-artige Doppelung): Gibt es
   Arbeiten, die Überdeckungscodes als Paar zweier Matrizen (U, W) mit der Bedingung max_i(|U y|_i + |W z|_i) >= d
   behandeln, oder die blockweise direkte Summe / "blockwise direct sum", normale und subnormale Codes für genau diese
   Zerlegung nutzen? Was ist über mehrfache Überdeckungen (multiple coverings, MCF) mit sehr wenigen Wörtern bekannt,
   insbesondere fast perfekte vierfache Überdeckungen von Q13 mit Radius 4?
4. Diskrepanz kleiner Matrizen. Gibt es Tabellen oder Rechnungen zum größten disc(A) für n x n-Matrizen mit Einträgen
   +-1 oder in {-1,0,1} für n <= 25, zur Diskrepanz von Hadamard-Matrizen (auch mit gestrichenen Spalten), zu
   erblicher und linearer Diskrepanz kleiner Beispiele? Gibt es irgendeine Familie mit asymptotischem Quotienten über 1?
   Was sagen Arbeiten zur unteren Schranke in Spencers Satz ("six standard deviations") über explizite Beispiele?
5. Überdeckung durch Zylinder. Eine Zeile mit Nullen überdeckt eine Kugel in weniger Koordinaten mal einen Teilwürfel.
   Bei n = 8 gibt es 55 680 Lösungsmatrizen, die Q8 perfekt in 16 solche "Doppelkugeln" mit verteilten Nullpositionen zerlegen.
   Unter welchem Namen ist das bekannt (diameter perfect codes nach Ahlswede-Aydinian-Khachatrian, Kacheln mit
   Antikodes, gemischte perfekte Codes)? Gibt es Ergebnisse für größere Parameter, die auf N = 15, 19 oder 20 passen?
6. Methodentransfer. Welche Suchverfahren haben seit etwa 2015 Rekorde bei Überdeckungscodes, Überdeckungsdesigns
   (La Jolla Covering Repository), Lotto-Designs, Fußballtoto-Problem oder covering arrays geliefert (Tabu mit
   Gewichten, große Nachbarschaften, SAT/ILP mit Symmetriebrechung, GPU)? Was davon ist auf "wenige Wörter, großer
   Radius, 2^20 bis 2^25 Punkte" übertragbar? Gibt es veröffentlichten Code?
7. Wettbewerb. Wer arbeitet erkennbar an C_10c oder an denselben Zellen (Pull Requests und Issues im Repo
   teorth/optimizationproblems, EinsteinArena, arXiv seit September 2026, AlphaEvolve- oder FunSearch-Berichte zu
   Diskrepanz oder Überdeckungscodes)? Nenne Datum und Link.

FORM DER ANTWORT
Zuerst die drei Funde, die unsere Suche am stärksten ändern würden, je mit einem Test, den man in Minuten rechnen kann.
Dann die Fragen 1 bis 7 der Reihe nach. Am Ende eine Liste "behauptet, aber nicht im Volltext geprüft".
Widersprich uns ausdrücklich, wo unsere Annahmen oben nicht zur Literatur passen.
```
