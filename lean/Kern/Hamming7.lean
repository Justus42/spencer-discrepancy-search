-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Hamming7.Teil0
import Kern.Swar7

namespace Ueberdeckung.Kern.Hamming7
open Ueberdeckung



end Ueberdeckung.Kern.Hamming7

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/Hamming7.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.Hamming7
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

theorem woerter_klein_kern : ∀ c ∈ woerter, c < 2 ^ 7 := by decide +kernel

theorem bloecke_kern : ∀ b, b < 2 ^ (7 - 7) → blockS 1 7 b woerter = true := fun b hb => by
  have := Kern.Hamming7.n_0_1 b hb; rwa [Nat.zero_add] at this

/-- Jedes x < 2^7 hat ein Wort der Liste im Hamming-Abstand höchstens 1. -/
theorem radius1_kern : ∀ x, x < 2 ^ 7 → ∃ c ∈ code matrix, abstand 7 x c ≤ 1 := by
  rw [woerter_eq_kern]
  exact aus_bloeckenS Kern.Swar7.swar_ok 1 7 woerter (by decide) woerter_klein_kern bloecke_kern

/-- Hauptsatz in Matrixform (Kernel-Fassung). -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 7 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 7 x z ≤ 1 :=
  (code_iff_matrix 7 1 matrix).1 radius1_kern

/-- Prüfschleife lehnt Radius 0 ab (Kernel). -/
theorem pruefe_nicht_0_kern : pruefe 7 0 (code matrix) = false := by decide +kernel

theorem nicht_radius0_kern : ¬ ∀ x, x < 2 ^ 7 → ∃ c ∈ code matrix, abstand 7 x c ≤ 0 := by
  rw [← pruefe_iff, pruefe_nicht_0_kern]; decide

end Ueberdeckung.Hamming7
