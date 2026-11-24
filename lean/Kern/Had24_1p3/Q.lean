-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis
import Ueberdeckung.Daten.Had24_1p3

namespace Ueberdeckung.Kern.Had24_1p3
open Ueberdeckung

/-- Block b: alle `x` in `[b·2^8, (b+1)·2^8)` haben ein Wort im Abstand höchstens 6. -/
abbrev Q (b : Nat) : Prop := blockS 6 8 b Ueberdeckung.Had24_1p3.woerter = true

end Ueberdeckung.Kern.Had24_1p3
