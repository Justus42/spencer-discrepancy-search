-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis

namespace Ueberdeckung.Kern.Swar15
open Ueberdeckung

/-- Block b: `swar` stimmt mit `einsen 15` auf `[b·2^8, (b+1)·2^8)` überein. -/
abbrev Q (b : Nat) : Prop := swarBlock 15 8 b = true

end Ueberdeckung.Kern.Swar15
