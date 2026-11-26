-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis

namespace Ueberdeckung.Kern.Swar18
open Ueberdeckung

/-- Block b: `swar` stimmt mit `einsen 18` auf `[b·2^8, (b+1)·2^8)` überein. -/
abbrev Q (b : Nat) : Prop := swarBlock 18 8 b = true

end Ueberdeckung.Kern.Swar18
