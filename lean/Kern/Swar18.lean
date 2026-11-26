-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Swar18.Teil0
import Kern.Swar18.Teil1
import Kern.Swar18.Teil2
import Kern.Swar18.Teil3
import Kern.Swar18.Teil4
import Kern.Swar18.Teil5
import Kern.Swar18.Teil6
import Kern.Swar18.Teil7
import Kern.Swar18.Teil8
import Kern.Swar18.Teil9
import Kern.Swar18.Teil10
import Kern.Swar18.Teil11
import Kern.Swar18.Teil12
import Kern.Swar18.Teil13
import Kern.Swar18.Teil14
import Kern.Swar18.Teil15

namespace Ueberdeckung.Kern.Swar18
open Ueberdeckung

theorem n_0_128 : ∀ j, j < 128 → Q (0 + j) := baum_knoten Q 0 64 n_0_64 n_64_64
theorem n_128_128 : ∀ j, j < 128 → Q (128 + j) := baum_knoten Q 128 64 n_128_64 n_192_64
theorem n_0_256 : ∀ j, j < 256 → Q (0 + j) := baum_knoten Q 0 128 n_0_128 n_128_128
theorem n_256_128 : ∀ j, j < 128 → Q (256 + j) := baum_knoten Q 256 64 n_256_64 n_320_64
theorem n_384_128 : ∀ j, j < 128 → Q (384 + j) := baum_knoten Q 384 64 n_384_64 n_448_64
theorem n_256_256 : ∀ j, j < 256 → Q (256 + j) := baum_knoten Q 256 128 n_256_128 n_384_128
theorem n_0_512 : ∀ j, j < 512 → Q (0 + j) := baum_knoten Q 0 256 n_0_256 n_256_256
theorem n_512_128 : ∀ j, j < 128 → Q (512 + j) := baum_knoten Q 512 64 n_512_64 n_576_64
theorem n_640_128 : ∀ j, j < 128 → Q (640 + j) := baum_knoten Q 640 64 n_640_64 n_704_64
theorem n_512_256 : ∀ j, j < 256 → Q (512 + j) := baum_knoten Q 512 128 n_512_128 n_640_128
theorem n_768_128 : ∀ j, j < 128 → Q (768 + j) := baum_knoten Q 768 64 n_768_64 n_832_64
theorem n_896_128 : ∀ j, j < 128 → Q (896 + j) := baum_knoten Q 896 64 n_896_64 n_960_64
theorem n_768_256 : ∀ j, j < 256 → Q (768 + j) := baum_knoten Q 768 128 n_768_128 n_896_128
theorem n_512_512 : ∀ j, j < 512 → Q (512 + j) := baum_knoten Q 512 256 n_512_256 n_768_256
theorem n_0_1024 : ∀ j, j < 1024 → Q (0 + j) := baum_knoten Q 0 512 n_0_512 n_512_512

/-- `swar` zählt die Bits aller Zahlen unter `2^18` richtig (im Kernel nachgerechnet). -/
theorem swar_ok : ∀ v, v < 2 ^ 18 → swar v = einsen 18 v :=
  swar_aus_bloecken 18 8 (by decide) (fun b hb => by
    have := n_0_1024 b hb; rwa [Nat.zero_add] at this)

end Ueberdeckung.Kern.Swar18
