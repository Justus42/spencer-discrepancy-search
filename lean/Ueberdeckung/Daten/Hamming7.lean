-- ERZEUGT von lean/erzeuge.py. Nicht von Hand ändern.
-- Quelle: controls/ctrl_8.txt
-- SHA-256 der Quelle: c1df5cefdf934c07af5a8ee601bc047419e58db70dbcc9450f85f45593a0a396
-- Zeilen in der Quelle: 8, Länge nach Streichen: 7
-- Gestrichene Nullspalten (Index ab 0): [7]
-- Weggelassene Zeile (Index ab 0, samt Komplement): keine
-- Bit j eines Worts = 1 genau dann, wenn Eintrag j = -1 (siehe Ueberdeckung.wort).

namespace Ueberdeckung.Hamming7

/-- Die ±1-Matrix, Zeile für Zeile. -/
def matrix : List (List Int) := [
  [1, 1, -1, -1, 1, -1, -1],
  [-1, -1, -1, -1, 1, 1, -1],
  [1, 1, 1, 1, 1, 1, -1],
  [1, 1, -1, -1, -1, 1, 1],
  [-1, 1, -1, 1, 1, -1, 1],
  [1, -1, -1, 1, 1, 1, 1],
  [-1, 1, 1, -1, 1, 1, 1],
  [1, -1, 1, -1, 1, -1, 1]
]

/-- Wortliste: Zeilen, dann Komplemente (Lean prüft `code matrix = woerter`). -/
def woerter : List Nat := [
  108, 79, 64, 28, 37, 6, 9, 42, 19, 48, 63, 99, 90, 121, 118, 85
]

end Ueberdeckung.Hamming7
