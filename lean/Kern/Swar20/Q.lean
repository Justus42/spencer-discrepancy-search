-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis

namespace Ueberdeckung.Kern.Swar20
open Ueberdeckung

/-- Block b: `swar` stimmt mit `einsen 20` auf `[b·2^8, (b+1)·2^8)` überein. -/
abbrev Q (b : Nat) : Prop := swarBlock 20 8 b = true

end Ueberdeckung.Kern.Swar20
