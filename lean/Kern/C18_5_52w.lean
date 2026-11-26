-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.C18_5_52w.Teil0
import Kern.C18_5_52w.Teil1
import Kern.C18_5_52w.Teil2
import Kern.C18_5_52w.Teil3
import Kern.C18_5_52w.Teil4
import Kern.C18_5_52w.Teil5
import Kern.C18_5_52w.Teil6
import Kern.C18_5_52w.Teil7
import Kern.C18_5_52w.Teil8
import Kern.C18_5_52w.Teil9
import Kern.C18_5_52w.Teil10
import Kern.C18_5_52w.Teil11
import Kern.C18_5_52w.Teil12
import Kern.C18_5_52w.Teil13
import Kern.C18_5_52w.Teil14
import Kern.C18_5_52w.Teil15
import Kern.Swar18

namespace Ueberdeckung.Kern.C18_5_52w
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

end Ueberdeckung.Kern.C18_5_52w

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/C18_5_52w.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.C18_5_52w
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

theorem woerter_klein_kern : ∀ c ∈ woerter, c < 2 ^ 18 := by decide +kernel

theorem bloecke_kern : ∀ b, b < 2 ^ (18 - 8) → blockS 5 8 b woerter = true := fun b hb => by
  have := Kern.C18_5_52w.n_0_1024 b hb; rwa [Nat.zero_add] at this

/-- Jedes x < 2^18 hat ein Wort der Liste im Hamming-Abstand höchstens 5. -/
theorem radius5_kern : ∀ x, x < 2 ^ 18 → ∃ c ∈ code matrix, abstand 18 x c ≤ 5 := by
  rw [woerter_eq_kern]
  exact aus_bloeckenS Kern.Swar18.swar_ok 5 8 woerter (by decide) woerter_klein_kern bloecke_kern

/-- Hauptsatz in Matrixform (Kernel-Fassung). -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 18 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 18 x z ≤ 5 :=
  (code_iff_matrix 18 5 matrix).1 radius5_kern

/-- Prüfschleife lehnt Radius 4 ab (Kernel). -/
theorem pruefe_nicht_4_kern : pruefe 18 4 (code matrix) = false := by decide +kernel

theorem nicht_radius4_kern : ¬ ∀ x, x < 2 ^ 18 → ∃ c ∈ code matrix, abstand 18 x c ≤ 4 := by
  rw [← pruefe_iff, pruefe_nicht_4_kern]; decide

end Ueberdeckung.C18_5_52w
