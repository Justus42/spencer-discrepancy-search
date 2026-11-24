-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Ctrl16.Teil0
import Kern.Swar15

namespace Ueberdeckung.Kern.Ctrl16
open Ueberdeckung



end Ueberdeckung.Kern.Ctrl16

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/Ctrl16.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.Ctrl16
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

theorem woerter_klein_kern : ∀ c ∈ woerter, c < 2 ^ 15 := by decide +kernel

theorem bloecke_kern : ∀ b, b < 2 ^ (15 - 8) → blockS 4 8 b woerter = true := fun b hb => by
  have := Kern.Ctrl16.n_0_128 b hb; rwa [Nat.zero_add] at this

/-- Jedes x < 2^15 hat ein Wort der Liste im Hamming-Abstand höchstens 4. -/
theorem radius4_kern : ∀ x, x < 2 ^ 15 → ∃ c ∈ code matrix, abstand 15 x c ≤ 4 := by
  rw [woerter_eq_kern]
  exact aus_bloeckenS Kern.Swar15.swar_ok 4 8 woerter (by decide) woerter_klein_kern bloecke_kern

/-- Hauptsatz in Matrixform (Kernel-Fassung). -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 15 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 15 x z ≤ 4 :=
  (code_iff_matrix 15 4 matrix).1 radius4_kern

/-- Prüfschleife lehnt Radius 3 ab (Kernel). -/
theorem pruefe_nicht_3_kern : pruefe 15 3 (code matrix) = false := by decide +kernel

theorem nicht_radius3_kern : ¬ ∀ x, x < 2 ^ 15 → ∃ c ∈ code matrix, abstand 15 x c ≤ 3 := by
  rw [← pruefe_iff, pruefe_nicht_3_kern]; decide

end Ueberdeckung.Ctrl16
