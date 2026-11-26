-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Ueberdeckung.Basis
import Ueberdeckung.Daten.C18_5_50w_ohne24

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/C18_5_50w_ohne24.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.C18_5_50w_ohne24
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

/-- Zeuge: `x0 = 55992` hat kein Wort der Liste im Abstand höchstens 5 (Kernel). -/
theorem zeuge_kern : nahe 18 5 55992 woerter = false := by decide +kernel

theorem nicht_radius5_kern : ¬ ∀ x, x < 2 ^ 18 → ∃ c ∈ code matrix, abstand 18 x c ≤ 5 := by
  intro h
  obtain ⟨c, hc, hle⟩ := h 55992 (by decide)
  have h1 := (nahe_iff 18 5 55992 woerter).2 ⟨c, by rw [← woerter_eq_kern]; exact hc, hle⟩
  rw [zeuge_kern] at h1
  exact absurd h1 (by decide)

end Ueberdeckung.C18_5_50w_ohne24
