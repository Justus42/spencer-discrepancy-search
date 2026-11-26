-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis
import Ueberdeckung.Daten.C18_5_52w

namespace Ueberdeckung.Kern.C18_5_52w
open Ueberdeckung

/-- Block b: alle `x` in `[b·2^8, (b+1)·2^8)` haben ein Wort im Abstand höchstens 5. -/
abbrev Q (b : Nat) : Prop := blockS 5 8 b Ueberdeckung.C18_5_52w.woerter = true

end Ueberdeckung.Kern.C18_5_52w
