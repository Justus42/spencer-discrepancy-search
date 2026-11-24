import Ueberdeckung.Basis
import Ueberdeckung.Daten.Had24_1p3_ohne0

/-! c) Negativkontrolle: `had24_1p3_paley12` ohne Zeile 0 und ihr Komplement — Beweis der Prüfschleife mit `native_decide`. -/

namespace Ueberdeckung.Had24_1p3_ohne0

/-- Form: 23 Zeilen, jede der Länge 20, nur Einträge ±1. -/
theorem form : matrix.length = 23 ∧ ∀ z ∈ matrix, z.length = 20 ∧ ∀ e ∈ z, e = 1 ∨ e = -1 := by decide

/-- 46 Wörter (Zeilen und Komplemente), paarweise verschieden. -/
theorem woerter : (code matrix).length = 46 ∧ (code matrix).Nodup := by decide

/-- Die vom Erzeuger geschriebene Wortliste ist genau `code matrix`. -/
theorem woerter_eq : code matrix = woerter := by decide

/-- Prüfschleife lehnt Radius 6 ab. -/
theorem pruefe_nicht_6 : pruefe 20 6 (code matrix) = false := by native_decide

/-- Radius 6 reicht nicht: Es gibt ein x < 2^20 ohne Wort im Abstand höchstens 6. -/
theorem nicht_radius6 : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 := by
  rw [← pruefe_iff, pruefe_nicht_6]; decide

end Ueberdeckung.Had24_1p3_ohne0
