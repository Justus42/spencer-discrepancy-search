import Ueberdeckung.Basis
import Ueberdeckung.Daten.Had24_1p3_ohne0

/-! c) Negativkontrolle: `had24_1p3_paley12` ohne Zeile 0 und ihr Komplement — Beweis der Prüfschleife mit `decide +kernel`. -/

namespace Ueberdeckung.Had24_1p3_ohne0

/-- Prüfschleife lehnt Radius 6 ab. -/
theorem pruefe_nicht_6_kern : pruefe 20 6 (code matrix) = false := by decide +kernel

/-- Radius 6 reicht nicht: Es gibt ein x < 2^20 ohne Wort im Abstand höchstens 6. -/
theorem nicht_radius6_kern : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 := by
  rw [← pruefe_iff, pruefe_nicht_6_kern]; decide

end Ueberdeckung.Had24_1p3_ohne0
