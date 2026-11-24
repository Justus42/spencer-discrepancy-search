import Ueberdeckung.Basis
import Ueberdeckung.Daten.Ctrl16

/-! b) `controls/ctrl_16.txt` ohne Nullspalte — Beweis der Prüfschleife mit `native_decide`. -/

namespace Ueberdeckung.Ctrl16

/-- Form: 16 Zeilen, jede der Länge 15, nur Einträge ±1. -/
theorem form : matrix.length = 16 ∧ ∀ z ∈ matrix, z.length = 15 ∧ ∀ e ∈ z, e = 1 ∨ e = -1 := by decide

/-- 32 Wörter (Zeilen und Komplemente), paarweise verschieden. -/
theorem woerter : (code matrix).length = 32 ∧ (code matrix).Nodup := by decide

/-- Die vom Erzeuger geschriebene Wortliste ist genau `code matrix`. -/
theorem woerter_eq : code matrix = woerter := by decide

/-- Prüfschleife, Radius 4. -/
theorem pruefe_4 : pruefe 15 4 (code matrix) = true := by native_decide

/-- Jedes x < 2^15 hat ein Wort der Liste im Hamming-Abstand höchstens 4. -/
theorem radius4 : ∀ x, x < 2 ^ 15 → ∃ c ∈ code matrix, abstand 15 x c ≤ 4 :=
  (pruefe_iff 15 4 (code matrix)).1 pruefe_4

/-- Hauptsatz in Matrixform: jedes x < 2^15 liegt im Abstand höchstens 4 von einer Zeile von M oder -M. -/
theorem hauptsatz : ∀ x, x < 2 ^ 15 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 15 x z ≤ 4 :=
  (code_iff_matrix 15 4 matrix).1 radius4

/-- Prüfschleife lehnt Radius 3 ab. -/
theorem pruefe_nicht_3 : pruefe 15 3 (code matrix) = false := by native_decide

/-- Radius 3 reicht nicht: Es gibt ein x < 2^15 ohne Wort im Abstand höchstens 3. -/
theorem nicht_radius3 : ¬ ∀ x, x < 2 ^ 15 → ∃ c ∈ code matrix, abstand 15 x c ≤ 3 := by
  rw [← pruefe_iff, pruefe_nicht_3]; decide

end Ueberdeckung.Ctrl16
