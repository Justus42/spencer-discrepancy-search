-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis

namespace Ueberdeckung.Kern.Swar7
open Ueberdeckung

/-- Block b: `swar` stimmt mit `einsen 7` auf `[b·2^7, (b+1)·2^7)` überein. -/
abbrev Q (b : Nat) : Prop := swarBlock 7 7 b = true

end Ueberdeckung.Kern.Swar7
