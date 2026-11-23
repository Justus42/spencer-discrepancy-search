# Positivkontrolle Länge 19 (23.11.2026)

Frage: Ist der Code aus `src/construct_32.py` (n=19, 32 Wörter) eine Vereinigung von
Nebenklassen eines dreidimensionalen linearen Codes D mit Einsvektor?

Methode: Translationsstabilisator S = {d : C + d = C} per Brute-Force (32x32 Kandidaten).
C ist genau dann Vereinigung von Nebenklassen von D, wenn D in S liegt.

Ergebnis (n = 15, 17, 19): |S| = 2, S = {0, 1}. Maximal ein eindimensionales D = <1>.
Also: NEIN. Die Konstruktion hat keine Struktur D = <1,a,b>. Laufzeit < 1 s.

Folge: Die Positivkontrolle der Astra-Suche ist nicht bestehbar (sie kann den Code
nicht mit 4 Nebenklassen finden). Halt vor Schritt 4 laut Arbeitsauftrag.
Kontrollen in controls/: beide Prüfskripte stimmen für alle 6 Dateien überein
(8x8:5, 9x9:5, 16x16:7, 17x17:7, L2026_17x17:7, astra_19:7).
