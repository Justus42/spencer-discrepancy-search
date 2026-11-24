-- ERZEUGT von lean/erzeuge.py. Nicht von Hand ändern.
-- Quelle: controls/ctrl_16.txt
-- SHA-256 der Quelle: 79a659ebf2874d69c0829031da3216275634c55723f5b1e89011ea02a0888d52
-- Zeilen in der Quelle: 16, Länge nach Streichen: 15
-- Gestrichene Nullspalten (Index ab 0): [15]
-- Weggelassene Zeile (Index ab 0, samt Komplement): keine
-- Bit j eines Worts = 1 genau dann, wenn Eintrag j = -1 (siehe Ueberdeckung.wort).

namespace Ueberdeckung.Ctrl16

/-- Die ±1-Matrix, Zeile für Zeile. -/
def matrix : List (List Int) := [
  [-1, 1, 1, 1, 1, -1, -1, -1, 1, 1, -1, -1, 1, -1, -1],
  [1, 1, 1, 1, -1, -1, -1, 1, 1, 1, 1, 1, -1, 1, -1],
  [-1, -1, -1, 1, -1, 1, -1, -1, -1, 1, 1, 1, 1, 1, -1],
  [-1, 1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, 1],
  [-1, 1, 1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, -1, 1],
  [1, -1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, 1, 1],
  [1, 1, 1, 1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1, 1],
  [-1, -1, 1, -1, -1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1],
  [-1, 1, 1, 1, 1, 1, -1, 1, -1, -1, -1, 1, -1, 1, 1],
  [-1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, -1, -1, 1, -1],
  [1, 1, -1, -1, 1, 1, -1, -1, 1, -1, -1, 1, 1, 1, -1],
  [-1, -1, 1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, -1, -1],
  [-1, -1, 1, -1, 1, -1, 1, 1, 1, -1, 1, 1, 1, 1, -1],
  [-1, 1, -1, 1, -1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1],
  [1, -1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, -1],
  [1, 1, 1, -1, -1, 1, 1, 1, -1, 1, -1, 1, 1, -1, -1]
]

/-- Wortliste: Zeilen, dann Komplemente (Lean prüft `code matrix = woerter`). -/
def woerter : List Nat := [
  27873, 20592, 16855, 8197, 9209, 1442, 2704, 3163, 5953, 22921, 18124, 30355, 16939, 20277, 27458, 25880, 4894, 12175, 15912, 24570, 23558, 31325, 30063, 29604, 26814, 9846, 14643, 2412, 15828, 12490, 5309, 6887
]

end Ueberdeckung.Ctrl16
