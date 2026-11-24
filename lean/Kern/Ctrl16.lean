import Ueberdeckung.Basis
import Ueberdeckung.Daten.Ctrl16

/-! b) `controls/ctrl_16.txt` ohne Nullspalte — Beweis der Prüfschleife mit `decide +kernel`. -/

namespace Ueberdeckung.Ctrl16

/-- Prüfschleife, Radius 4. -/
theorem pruefe_4_kern : pruefe 15 4 (code matrix) = true := by decide +kernel

/-- Jedes x < 2^15 hat ein Wort der Liste im Hamming-Abstand höchstens 4. -/
theorem radius4_kern : ∀ x, x < 2 ^ 15 → ∃ c ∈ code matrix, abstand 15 x c ≤ 4 :=
  (pruefe_iff 15 4 (code matrix)).1 pruefe_4_kern

/-- Hauptsatz in Matrixform: jedes x < 2^15 liegt im Abstand höchstens 4 von einer Zeile von M oder -M. -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 15 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 15 x z ≤ 4 :=
  (code_iff_matrix 15 4 matrix).1 radius4_kern

/-- Prüfschleife lehnt Radius 3 ab. -/
theorem pruefe_nicht_3_kern : pruefe 15 3 (code matrix) = false := by decide +kernel

/-- Radius 3 reicht nicht: Es gibt ein x < 2^15 ohne Wort im Abstand höchstens 3. -/
theorem nicht_radius3_kern : ¬ ∀ x, x < 2 ^ 15 → ∃ c ∈ code matrix, abstand 15 x c ≤ 3 := by
  rw [← pruefe_iff, pruefe_nicht_3_kern]; decide

end Ueberdeckung.Ctrl16
