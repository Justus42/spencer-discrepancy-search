-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis
import Ueberdeckung.Daten.Ctrl16

namespace Ueberdeckung.Kern.Ctrl16
open Ueberdeckung

/-- Block b: alle `x` in `[b·2^8, (b+1)·2^8)` haben ein Wort im Abstand höchstens 4. -/
abbrev Q (b : Nat) : Prop := blockS 4 8 b Ueberdeckung.Ctrl16.woerter = true

end Ueberdeckung.Kern.Ctrl16
