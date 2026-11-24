import Ueberdeckung.Basis
import Ueberdeckung.Daten.Had24_1p3

/-! Ziel: `controls/had24_1p3_paley12.txt` — Beweis der Prüfschleife mit `decide +kernel`. -/

namespace Ueberdeckung.Had24_1p3

/-- Prüfschleife, Radius 6. -/
theorem pruefe_6_kern : pruefe 20 6 (code matrix) = true := by decide +kernel

/-- Jedes x < 2^20 hat ein Wort der Liste im Hamming-Abstand höchstens 6. -/
theorem radius6_kern : ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 :=
  (pruefe_iff 20 6 (code matrix)).1 pruefe_6_kern

/-- Hauptsatz in Matrixform: jedes x < 2^20 liegt im Abstand höchstens 6 von einer Zeile von M oder -M. -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 20 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 20 x z ≤ 6 :=
  (code_iff_matrix 20 6 matrix).1 radius6_kern

/-- Prüfschleife lehnt Radius 5 ab. -/
theorem pruefe_nicht_5_kern : pruefe 20 5 (code matrix) = false := by decide +kernel

/-- Radius 5 reicht nicht: Es gibt ein x < 2^20 ohne Wort im Abstand höchstens 5. -/
theorem nicht_radius5_kern : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 5 := by
  rw [← pruefe_iff, pruefe_nicht_5_kern]; decide

end Ueberdeckung.Had24_1p3
