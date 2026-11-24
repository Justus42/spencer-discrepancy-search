# Auftragstext der Rückfrage an GPT 6.2 Sol (24.11.2026, 22:22)

Freigabe durch Justus am 24.11.2026, 22:21 („Ja zu Frage an Sol via Batch“). Batch-Auftrag
`openrouter:batch-1795555378-XnB4KlJp2kB6sje9H72g`, Modell `openai/gpt-6.2-sol`, Denkstufe xhigh, Ausgabegrenze 65 536
Tokens, keine Dateien angehängt. Antwort: `notes/openrouter/` auf diesem Branch.

Systemrolle: „Du bist Kombinatoriker mit Schwerpunkt Überdeckungscodes, extremale Mengenlehre und Diskrepanz. Du beweist
oder widerlegst; du rätst nicht.“

```
# Rückfrage: Würfelüberdeckung durch Doppelkugeln (Zeilen mit einer Null), Stand 24.11.2026

Du hast keine Websuche. Kennzeichne jede Aussage: [R] eigene Rechnung oder Herleitung (lückenlos), [L] Literatur aus dem Gedächtnis (unbelegt), [U] Urteil oder Schätzung. Deutsch, Ergebnis zuerst, keine erfundenen Quellen. Ein vollständiger Beweis zählt mehr als viele Ideen. Wenn ein Beweis eine Lücke hat, benenne sie genau.

## Hintergrund
Gesucht ist eine n x n-Matrix A mit Einträgen in [-1,1] und disc(A)/sqrt(n) > 5/sqrt(8) = 1,7678, wobei disc(A) = min über x in {-1,1}^n von max_i |<a_i, x>|. Rekord: n = 8, Hamming-Code [7,4] plus Nullspalte, disc 5.

Für eine Zeile a und Schwelle d sei T(a) = {x : |<a,x>| >= d}. Gebraucht werden n Zeilen, deren Mengen T(a_i) den Würfel {-1,1}^n überdecken. Die Halbmenge {x : <a,x> >= d} hat Hamming-Durchmesser <= n - d; nach Kleitman (1966) ist ihre Größe höchstens die der Kugel (gerader Durchmesser 2r: V(n,r)) bzw. der Doppelkugel (ungerader Durchmesser 2r+1: 2*V(n-1,r)). Die Doppelkugel entsteht genau durch eine Zeile mit EINER Null und n-1 Einträgen +-1: T(a) = {x : Abstand von x zu a oder -a auf den n-1 Nicht-Null-Koordinaten <= r}, die Null-Koordinate ist frei. Die Null darf in jeder Zeile an anderer Stelle stehen.

Offene Fälle mit ungeradem n - d (Schwelle d = kleinste zulässige ganze Zahl):
- n = 8, d = 5, r = 1: 8 Doppelkugeln zu je 32 Punkten, Dichte 1. Lösung existiert (Hamming, alle Nullen in einer Spalte).
- n = 11, d = 6, r = 2: 11 Doppelkugeln, je 224 Punkte von 2048, Dichte 1,203. Erfolg wäre ein Rekord: 6/sqrt(11) = 1,809.
- n = 14, d = 7, r = 3: 14 Doppelkugeln, je 1512 von 16384, Dichte 1,292. 7/sqrt(14) = 1,871.
- n = 17, d = 8, r = 4: 17 Doppelkugeln, je 10068 von 131072, Dichte 1,306. 8/sqrt(17) = 1,940.

## Was exakt gerechnet ist (n = 11, Punkte modulo Vorzeichen: 1024 Klassen, jede Zeile trifft 112 Klassen)
- Es gibt 5632 Zeilen mit genau einer Null (modulo Vorzeichen).
- Budget: 11*112 - 1024 = 208 Klassen Mehrfachüberdeckung insgesamt.
- Schnittgröße |T(a) ∩ T(b)| in Klassen, a = (0, 1, ..., 1):
  - b mit Null in derselben Spalte, Abstand delta auf den 10 übrigen Koordinaten (modulo Komplement, delta = 1..5): 40, 40, 12, 12, 0. Anzahl solcher b: 10, 45, 120, 210, 126.
  - b mit Null in anderer Spalte, Abstand e auf den 9 gemeinsamen Nicht-Null-Koordinaten (modulo Vorzeichen von b und Vorzeichen der Koordinate, in der a null ist; e = 0..4): 76, 40, 26, 12, 6. Anzahl: 20, 180, 720, 1680, 2520.
- Stehen alle Nullen in einer Spalte, ist es ein selbstkomplementärer Überdeckungscode der Länge 10, Radius 2, mit 22 Wörtern: unmöglich wegen K(10,2) >= 24 [L, Tabelle]. Analog n = 14: K(13,3) >= 28, es bräuchte genau 28; n = 17: K(16,4) >= 34, es bräuchte genau 34 [L, Tabelle].
- Zirkulante Matrizen mit Nulldiagonale, alle ersten Zeilen: größte Diskrepanz 4 (n = 11), 5 (n = 14), 6 (n = 17). Ausgeschlossen.
- Eine vollständige Baumsuche für n = 11 läuft gerade; eine Python-Fassung kam in 4 Minuten nie über 7 Zeilen hinaus, ohne Entscheidung.
- Bei n = 8 findet dieselbe Suche die Hamming-Lösung sofort; mit 7 Zeilen gibt es keine.

## Aufgabe
1. n = 11: Beweise, dass 11 Doppelkugeln (beliebige Nullpositionen) {-1,1}^11 NICHT überdecken, oder gib 11 Zeilen an, die es tun (als Liste von Zeilen mit Einträgen -1, 0, 1). Nutze, was trägt: Budget 208, die Schnitttabelle, Hunter-Worsley (Vereinigung <= Summe minus Gewicht eines Spannbaums der paarweisen Schnitte), Exzess-Argumente nach van Wee, Zählen nach Gewichtsschichten, Fallunterscheidung nach der Verteilung der Nullen auf die Spalten (wie viele Zeilen haben die Null in Spalte j?).
2. Erweitere, wenn möglich, auf alle Zeilen in {-1,0,1}^11 (Träger 8: je 72 Klassen; Träger 6: je 32 Klassen; ungerade Träger sind dominiert) und sage, was für reelle Zeilen in [-1,1]^11 fehlt.
3. Allgemein n = 3k+2 mit d = k+3, also n - d = 2k-1 (n = 8, 11, 14, 17): Gibt es einen strukturellen Grund, warum n = 8 geht und größere k nicht (oder doch)? Gesucht ist entweder ein Beweis für alle k >= 3, eine Bauform mit algebraischem Kern und verteilten Nullen (keine Zirkulante, keine feste Nullspalte), oder eine klare Aussage, dass du keines von beiden hast.
4. Falls kein Beweis gelingt: die schärfsten notwendigen Bedingungen an eine Lösung für n = 11 (welche Paartypen aus der Schnitttabelle dürfen wie oft vorkommen, welche Nullverteilungen sind ausgeschlossen), so formuliert, dass eine Baumsuche sie als Schnitt verwenden kann.

Am Ende: eine Zeile je Punkt 1 bis 4 mit dem Status "bewiesen", "Konstruktion", "Teilresultat" oder "offen".
```
