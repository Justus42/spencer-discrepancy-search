# Lauf 24.11.2026: Spaltenanbau an die Paley-Doppelung 24 x 20 (Gegenrechnung Prüfvermerk, Punkt 3)

Cloud-Sitzung, Branch `ccr-2b82b0fd-k51lze` (neu von `origin/main` 39743f4). Nur Prüfcode mit ganzen Zahlen, kein Suchcode.
Obergrenze 20 Min., verbraucht 1,3 s Rechenzeit. `lean/` nicht angefasst.
Quelle der Angabe: `notes/zweitmeinung_2026-11-24_pruefvermerk.md`, Punkt 3 (Orchestrator, Skripte nicht im Repo). Alles neu gerechnet.

Vorab `sha256sum -c verifiers/SHA256SUMS` (20:28 UTC):

```
src/check_disc.py: OK
verifiers/verify_spencer.py: OK
src/exakt_disc.c: OK
```

Programm `src/pruefe_spaltenanbau.py`, Log `runs/spaltenanbau/lauf.log.txt`, Zeugen `runs/spaltenanbau/zeuge_*.txt`. Aufruf:

```bash
python3 src/pruefe_spaltenanbau.py        # Startwert der Gegenprobe 20261124
```

## Aussage

An `controls/had24_1p3_paley12.txt` und `controls/had24_2p2_paley12.txt` (24 x 20, Diskrepanz 8) lässt sich keine 21. Spalte
mit Einträgen in [-1,1] anbauen, die die Diskrepanz auf 9 hebt.

## Begründung (ausgeschrieben)

1. Beide Matrizen haben nur Einträge ±1 (vom Skript beim Einlesen geprüft). Bei 20 Spalten ist jede Zeilensumme (Ax)_i
   eine Summe von 20 Zahlen ±1, also gerade.
2. Sei x ein Vorzeichenvektor mit max_i |(Ax)_i| = 8, der den Wert 8 in genau einer Zeile i0 annimmt. In jeder anderen
   Zeile ist |(Ax)_i| < 8 und gerade, also höchstens 6.
3. Neue Spalte b mit b_i in [-1,1], 21. Koordinate s in {+1,-1}. Neuer Zeilenwert: (Ax)_i + s·b_i.
4. Zeile i0: Man wählt s so, dass s·b_i0 das entgegengesetzte Vorzeichen von (Ax)_i0 hat (bei b_i0 = 0 beliebig).
   Dann ist |(Ax)_i0 + s·b_i0| = 8 − |b_i0| ≤ 8.
5. Jede andere Zeile: |(Ax)_i + s·b_i| ≤ 6 + |b_i| ≤ 7, unabhängig von s.
6. Also hat (x, s) Maximum ≤ 8, und disc([A | b]) ≤ 8.
7. Schwelle: 24 x 21 wird mit Nullen zu 24 x 24 aufgefüllt (ändert die Diskrepanz nicht), n = 24. Rekord verlangt
   disc > 5·sqrt(24)/sqrt(8) = 5·sqrt(3) ≈ 8,66, also mindestens 9.

**Prüfung auf Fehler: keiner gefunden.** Anmerkungen:

- Schritt 2 braucht die Eindeutigkeit von i0 wirklich: Bei zwei Zeilen mit Betrag 8 kann ein s nicht beide senken.
- Schritt 2 braucht ±1-Einträge (Parität). Mit Nullen in A würde „höchstens 6“ nicht folgen; hier erfüllt.
- Das Argument braucht nur einen einzigen Zeugen x je Matrix, nicht die Diskrepanz 8 der Ausgangsmatrix und keine Zählung.
- Schärfer für ±1-Spalten: Zeile i0 wird dann genau 7, alle Zeilensummen sind ungerade, also disc ≤ 7. Die Gegenprobe zeigt genau das
  (kleineres Maximum stets 7).
- Die Aussage gilt nur für das Anbauen an genau diese beiden Matrizen, nicht für andere 24 x 21-Matrizen
  (die hat Teil A „Hadamard 24 ohne 3 Spalten“ abgedeckt).

## Schritt 1: Zählung (prüft nur die Angabe des Prüfvermerks)

Alle 2^20 Vorzeichenvektoren, numpy `int32`. Nebenbei disc = 8 für beide Matrizen bestätigt.

| Matrix | Max 8, genau eine Maximalzeile: alle x | bis auf Vorzeichen (x_1 = +1) | Prüfvermerk | Lesart |
|---|---|---|---|---|
| `had24_1p3_paley12` | 9936 | **4968** | 4968 | bis auf globales Vorzeichen |
| `had24_2p2_paley12` | 12 096 | **6048** | 6048 | bis auf globales Vorzeichen |

Die Zahlen des Prüfvermerks stimmen in der Lesart „bis auf globales Vorzeichen“; über alle Vektoren sind es doppelt so viele.

## Schritt 2: Zeugen (das eigentliche Ergebnis)

Aus der Datei zurückgelesen und mit reinen Python-Ganzzahlen (ohne numpy) nachgerechnet.

| Matrix | Zeuge x (Datei) | i0 (0-basiert) | Ax |
|---|---|---|---|
| `had24_1p3_paley12` | `runs/spaltenanbau/zeuge_had24_1p3_paley12.txt`: `1 1 -1 -1 1 1 -1 -1 -1 1 1 -1 1 1 1 1 1 1 1 1` | 0 | 8, −6, 0, 2, 2, 4, 0, −6, −4, 4, 6, 6, −6, −4, −2, 4, 4, 2, 6, −4, −6, −6, 0, 0 |
| `had24_2p2_paley12` | `runs/spaltenanbau/zeuge_had24_2p2_paley12.txt`: `-1 1 -1 -1 -1 -1 1 1 1 1 -1 -1 1 1 1 1 1 1 1 1` | 13 | 6, 4, 2, 4, 0, −6, −6, −4, 2, 6, 0, 4, −6, 8, 2, 4, 4, −6, 2, −4, −2, −6, −4, −4 |

Beide: Maximum 8, genau eine Zeile mit Betrag 8, alle anderen ≤ 6. **OK.**

## Schritt 3: Gegenprobe

Je Zeuge 10 zufällige ±1-Spalten (Startwert 20261124), beide Vorzeichen der 21. Koordinate. Ergebnis in allen 20 Fällen:
ein Vorzeichen gibt Maximum 9, das andere 7; kleineres Maximum stets **7 ≤ 8**. Einzelwerte im Log.

## Ergebnis

Negativbefund, Belegart Gegenbeispiel je Fall (ein Zeuge je Matrix deckt alle Spalten b ∈ [-1,1]^24 ab):
Paley-Doppelung 24 x 20 plus eine Spalte hat Diskrepanz ≤ 8 (für ±1-Spalten ≤ 7), nötig wären 9.
Kein Treffer, deshalb keine Prüfung über `verifiers/pruefe_matrix.py` nötig.
