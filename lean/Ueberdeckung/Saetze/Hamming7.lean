import Ueberdeckung.Basis
import Ueberdeckung.Daten.Hamming7

/-! a) Hamming [7,4] (`controls/ctrl_8.txt` ohne Nullspalte) — Beweis der Prüfschleife mit `native_decide`. -/

namespace Ueberdeckung.Hamming7

/-- Form: 8 Zeilen, jede der Länge 7, nur Einträge ±1. -/
theorem form : matrix.length = 8 ∧ ∀ z ∈ matrix, z.length = 7 ∧ ∀ e ∈ z, e = 1 ∨ e = -1 := by decide

/-- 16 Wörter (Zeilen und Komplemente), paarweise verschieden. -/
theorem woerter : (code matrix).length = 16 ∧ (code matrix).Nodup := by decide

/-- Die vom Erzeuger geschriebene Wortliste ist genau `code matrix`. -/
theorem woerter_eq : code matrix = woerter := by decide

/-- Prüfschleife, Radius 1. -/
theorem pruefe_1 : pruefe 7 1 (code matrix) = true := by native_decide

/-- Jedes x < 2^7 hat ein Wort der Liste im Hamming-Abstand höchstens 1. -/
theorem radius1 : ∀ x, x < 2 ^ 7 → ∃ c ∈ code matrix, abstand 7 x c ≤ 1 :=
  (pruefe_iff 7 1 (code matrix)).1 pruefe_1

/-- Hauptsatz in Matrixform: jedes x < 2^7 liegt im Abstand höchstens 1 von einer Zeile von M oder -M. -/
theorem hauptsatz : ∀ x, x < 2 ^ 7 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 7 x z ≤ 1 :=
  (code_iff_matrix 7 1 matrix).1 radius1

/-- Prüfschleife lehnt Radius 0 ab. -/
theorem pruefe_nicht_0 : pruefe 7 0 (code matrix) = false := by native_decide

/-- Radius 0 reicht nicht: Es gibt ein x < 2^7 ohne Wort im Abstand höchstens 0. -/
theorem nicht_radius0 : ¬ ∀ x, x < 2 ^ 7 → ∃ c ∈ code matrix, abstand 7 x c ≤ 0 := by
  rw [← pruefe_iff, pruefe_nicht_0]; decide

end Ueberdeckung.Hamming7
