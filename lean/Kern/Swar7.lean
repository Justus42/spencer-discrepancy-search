-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Swar7.Teil0

namespace Ueberdeckung.Kern.Swar7
open Ueberdeckung



/-- `swar` zählt die Bits aller Zahlen unter `2^7` richtig (im Kernel nachgerechnet). -/
theorem swar_ok : ∀ v, v < 2 ^ 7 → swar v = einsen 7 v :=
  swar_aus_bloecken 7 7 (by decide) (fun b hb => by
    have := n_0_1 b hb; rwa [Nat.zero_add] at this)

end Ueberdeckung.Kern.Swar7
