-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Swar15.Teil0

namespace Ueberdeckung.Kern.Swar15
open Ueberdeckung



/-- `swar` zählt die Bits aller Zahlen unter `2^15` richtig (im Kernel nachgerechnet). -/
theorem swar_ok : ∀ v, v < 2 ^ 15 → swar v = einsen 15 v :=
  swar_aus_bloecken 15 8 (by decide) (fun b hb => by
    have := n_0_128 b hb; rwa [Nat.zero_add] at this)

end Ueberdeckung.Kern.Swar15
