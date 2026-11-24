-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Swar20.Teil0
import Kern.Swar20.Teil1
import Kern.Swar20.Teil2
import Kern.Swar20.Teil3
import Kern.Swar20.Teil4
import Kern.Swar20.Teil5
import Kern.Swar20.Teil6
import Kern.Swar20.Teil7

namespace Ueberdeckung.Kern.Swar20
open Ueberdeckung

theorem n_0_1024 : ∀ j, j < 1024 → Q (0 + j) := baum_knoten Q 0 512 n_0_512 n_512_512
theorem n_1024_1024 : ∀ j, j < 1024 → Q (1024 + j) := baum_knoten Q 1024 512 n_1024_512 n_1536_512
theorem n_0_2048 : ∀ j, j < 2048 → Q (0 + j) := baum_knoten Q 0 1024 n_0_1024 n_1024_1024
theorem n_2048_1024 : ∀ j, j < 1024 → Q (2048 + j) := baum_knoten Q 2048 512 n_2048_512 n_2560_512
theorem n_3072_1024 : ∀ j, j < 1024 → Q (3072 + j) := baum_knoten Q 3072 512 n_3072_512 n_3584_512
theorem n_2048_2048 : ∀ j, j < 2048 → Q (2048 + j) := baum_knoten Q 2048 1024 n_2048_1024 n_3072_1024
theorem n_0_4096 : ∀ j, j < 4096 → Q (0 + j) := baum_knoten Q 0 2048 n_0_2048 n_2048_2048

/-- `swar` zählt die Bits aller Zahlen unter `2^20` richtig (im Kernel nachgerechnet). -/
theorem swar_ok : ∀ v, v < 2 ^ 20 → swar v = einsen 20 v :=
  swar_aus_bloecken 20 8 (by decide) (fun b hb => by
    have := n_0_4096 b hb; rwa [Nat.zero_add] at this)

end Ueberdeckung.Kern.Swar20
