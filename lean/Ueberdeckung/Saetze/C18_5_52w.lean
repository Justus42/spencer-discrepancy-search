import Ueberdeckung.Basis
import Ueberdeckung.Daten.C18_5_52w

/-! Auftrag LZ: K(18,5) ≤ 52, 26 Zeilen — Beweis der Prüfschleife mit `native_decide`. Quelle `controls/c18_5_52w.txt`. -/

namespace Ueberdeckung.C18_5_52w

/-- Form: 26 Zeilen, jede der Länge 18, nur Einträge ±1. -/
theorem form : matrix.length = 26 ∧ ∀ z ∈ matrix, z.length = 18 ∧ ∀ e ∈ z, e = 1 ∨ e = -1 := by decide

/-- 52 Wörter (Zeilen und Komplemente), paarweise verschieden. -/
theorem anzahl_woerter : (code matrix).length = 52 ∧ (code matrix).Nodup := by decide

/-- Die vom Erzeuger geschriebene Wortliste ist genau `code matrix`. -/
theorem woerter_eq : code matrix = woerter := by decide

/-- Prüfschleife, Radius 5. -/
theorem pruefe_5 : pruefe 18 5 (code matrix) = true := by native_decide

/-- Jedes x < 2^18 hat ein Wort der Liste im Hamming-Abstand höchstens 5. -/
theorem radius5 : ∀ x, x < 2 ^ 18 → ∃ c ∈ code matrix, abstand 18 x c ≤ 5 :=
  (pruefe_iff 18 5 (code matrix)).1 pruefe_5

/-- Hauptsatz in Matrixform: jedes x < 2^18 liegt im Abstand höchstens 5 von einer Zeile von M oder -M. -/
theorem hauptsatz : ∀ x, x < 2 ^ 18 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 18 x z ≤ 5 :=
  (code_iff_matrix 18 5 matrix).1 radius5

/-- Prüfschleife lehnt Radius 4 ab. -/
theorem pruefe_nicht_4 : pruefe 18 4 (code matrix) = false := by native_decide

/-- Radius 4 reicht nicht: Es gibt ein x < 2^18 ohne Wort im Abstand höchstens 4. -/
theorem nicht_radius4 : ¬ ∀ x, x < 2 ^ 18 → ∃ c ∈ code matrix, abstand 18 x c ≤ 4 := by
  rw [← pruefe_iff, pruefe_nicht_4]; decide

end Ueberdeckung.C18_5_52w
