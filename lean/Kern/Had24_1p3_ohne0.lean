-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis
import Ueberdeckung.Daten.Had24_1p3_ohne0

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/Had24_1p3_ohne0.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.Had24_1p3_ohne0
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

/-- Prüfschleife lehnt Radius 6 ab (Kernel). -/
theorem pruefe_nicht_6_kern : pruefe 20 6 (code matrix) = false := by decide +kernel

theorem nicht_radius6_kern : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 := by
  rw [← pruefe_iff, pruefe_nicht_6_kern]; decide

end Ueberdeckung.Had24_1p3_ohne0
