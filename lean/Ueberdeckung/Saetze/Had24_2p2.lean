import Ueberdeckung.Basis
import Ueberdeckung.Daten.Had24_2p2

/-! Ziel: `controls/had24_2p2_paley12.txt` — Beweis der Prüfschleife mit `native_decide`. -/

namespace Ueberdeckung.Had24_2p2

/-- Form: 24 Zeilen, jede der Länge 20, nur Einträge ±1. -/
theorem form : matrix.length = 24 ∧ ∀ z ∈ matrix, z.length = 20 ∧ ∀ e ∈ z, e = 1 ∨ e = -1 := by decide

/-- 48 Wörter (Zeilen und Komplemente), paarweise verschieden. -/
theorem anzahl_woerter : (code matrix).length = 48 ∧ (code matrix).Nodup := by decide

/-- Die vom Erzeuger geschriebene Wortliste ist genau `code matrix`. -/
theorem woerter_eq : code matrix = woerter := by decide

/-- Prüfschleife, Radius 6. -/
theorem pruefe_6 : pruefe 20 6 (code matrix) = true := by native_decide

/-- Jedes x < 2^20 hat ein Wort der Liste im Hamming-Abstand höchstens 6. -/
theorem radius6 : ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 :=
  (pruefe_iff 20 6 (code matrix)).1 pruefe_6

/-- Hauptsatz in Matrixform: jedes x < 2^20 liegt im Abstand höchstens 6 von einer Zeile von M oder -M. -/
theorem hauptsatz : ∀ x, x < 2 ^ 20 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 20 x z ≤ 6 :=
  (code_iff_matrix 20 6 matrix).1 radius6

/-- Prüfschleife lehnt Radius 5 ab. -/
theorem pruefe_nicht_5 : pruefe 20 5 (code matrix) = false := by native_decide

/-- Radius 5 reicht nicht: Es gibt ein x < 2^20 ohne Wort im Abstand höchstens 5. -/
theorem nicht_radius5 : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 5 := by
  rw [← pruefe_iff, pruefe_nicht_5]; decide

end Ueberdeckung.Had24_2p2
