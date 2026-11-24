import Ueberdeckung.Basis
import Ueberdeckung.Daten.Hamming7

/-! a) Hamming [7,4] (`controls/ctrl_8.txt` ohne Nullspalte) — Beweis der Prüfschleife mit `decide +kernel`. -/

namespace Ueberdeckung.Hamming7

/-- Prüfschleife, Radius 1. -/
theorem pruefe_1_kern : pruefe 7 1 (code matrix) = true := by decide +kernel

/-- Jedes x < 2^7 hat ein Wort der Liste im Hamming-Abstand höchstens 1. -/
theorem radius1_kern : ∀ x, x < 2 ^ 7 → ∃ c ∈ code matrix, abstand 7 x c ≤ 1 :=
  (pruefe_iff 7 1 (code matrix)).1 pruefe_1_kern

/-- Hauptsatz in Matrixform: jedes x < 2^7 liegt im Abstand höchstens 1 von einer Zeile von M oder -M. -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 7 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 7 x z ≤ 1 :=
  (code_iff_matrix 7 1 matrix).1 radius1_kern

/-- Prüfschleife lehnt Radius 0 ab. -/
theorem pruefe_nicht_0_kern : pruefe 7 0 (code matrix) = false := by decide +kernel

/-- Radius 0 reicht nicht: Es gibt ein x < 2^7 ohne Wort im Abstand höchstens 0. -/
theorem nicht_radius0_kern : ¬ ∀ x, x < 2 ^ 7 → ∃ c ∈ code matrix, abstand 7 x c ≤ 0 := by
  rw [← pruefe_iff, pruefe_nicht_0_kern]; decide

end Ueberdeckung.Hamming7
