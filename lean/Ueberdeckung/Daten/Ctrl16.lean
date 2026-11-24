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

end Ueberdeckung.Ctrl16
