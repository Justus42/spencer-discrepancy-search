-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis
import Ueberdeckung.Daten.Hamming7

namespace Ueberdeckung.Kern.Hamming7
open Ueberdeckung

/-- Block b: alle `x` in `[b·2^7, (b+1)·2^7)` haben ein Wort im Abstand höchstens 1. -/
abbrev Q (b : Nat) : Prop := blockS 1 7 b Ueberdeckung.Hamming7.woerter = true

end Ueberdeckung.Kern.Hamming7
