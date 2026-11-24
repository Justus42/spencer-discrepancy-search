-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Had24_1p3.Teil0
import Kern.Had24_1p3.Teil1
import Kern.Had24_1p3.Teil2
import Kern.Had24_1p3.Teil3
import Kern.Had24_1p3.Teil4
import Kern.Had24_1p3.Teil5
import Kern.Had24_1p3.Teil6
import Kern.Had24_1p3.Teil7
import Kern.Swar20

namespace Ueberdeckung.Kern.Had24_1p3
open Ueberdeckung

theorem n_0_1024 : ∀ j, j < 1024 → Q (0 + j) := baum_knoten Q 0 512 n_0_512 n_512_512
theorem n_1024_1024 : ∀ j, j < 1024 → Q (1024 + j) := baum_knoten Q 1024 512 n_1024_512 n_1536_512
theorem n_0_2048 : ∀ j, j < 2048 → Q (0 + j) := baum_knoten Q 0 1024 n_0_1024 n_1024_1024
theorem n_2048_1024 : ∀ j, j < 1024 → Q (2048 + j) := baum_knoten Q 2048 512 n_2048_512 n_2560_512
theorem n_3072_1024 : ∀ j, j < 1024 → Q (3072 + j) := baum_knoten Q 3072 512 n_3072_512 n_3584_512
theorem n_2048_2048 : ∀ j, j < 2048 → Q (2048 + j) := baum_knoten Q 2048 1024 n_2048_1024 n_3072_1024
theorem n_0_4096 : ∀ j, j < 4096 → Q (0 + j) := baum_knoten Q 0 2048 n_0_2048 n_2048_2048

end Ueberdeckung.Kern.Had24_1p3

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/Had24_1p3.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.Had24_1p3
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

theorem woerter_klein_kern : ∀ c ∈ woerter, c < 2 ^ 20 := by decide +kernel

theorem bloecke_kern : ∀ b, b < 2 ^ (20 - 8) → blockS 6 8 b woerter = true := fun b hb => by
  have := Kern.Had24_1p3.n_0_4096 b hb; rwa [Nat.zero_add] at this

/-- Jedes x < 2^20 hat ein Wort der Liste im Hamming-Abstand höchstens 6. -/
theorem radius6_kern : ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 := by
  rw [woerter_eq_kern]
  exact aus_bloeckenS Kern.Swar20.swar_ok 6 8 woerter (by decide) woerter_klein_kern bloecke_kern

/-- Hauptsatz in Matrixform (Kernel-Fassung). -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 20 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 20 x z ≤ 6 :=
  (code_iff_matrix 20 6 matrix).1 radius6_kern

/-- Prüfschleife lehnt Radius 5 ab (Kernel). -/
theorem pruefe_nicht_5_kern : pruefe 20 5 (code matrix) = false := by decide +kernel

theorem nicht_radius5_kern : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 5 := by
  rw [← pruefe_iff, pruefe_nicht_5_kern]; decide

end Ueberdeckung.Had24_1p3
