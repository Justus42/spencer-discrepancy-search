import Ueberdeckung.Basis
import Ueberdeckung.Daten.C18_5_50w_ohne24

/-! Auftrag LZ, Negativkontrolle: der 50er ohne die freie Zeile 24 (und ihr Komplement) — Beweis der Prüfschleife mit `native_decide`. Quelle `controls/c18_5_50w.txt ohne Zeile 24`. -/

namespace Ueberdeckung.C18_5_50w_ohne24

/-- Form: 24 Zeilen, jede der Länge 18, nur Einträge ±1. -/
theorem form : matrix.length = 24 ∧ ∀ z ∈ matrix, z.length = 18 ∧ ∀ e ∈ z, e = 1 ∨ e = -1 := by decide

/-- 48 Wörter (Zeilen und Komplemente), paarweise verschieden. -/
theorem anzahl_woerter : (code matrix).length = 48 ∧ (code matrix).Nodup := by decide

/-- Die vom Erzeuger geschriebene Wortliste ist genau `code matrix`. -/
theorem woerter_eq : code matrix = woerter := by decide

/-- Prüfschleife lehnt Radius 5 ab. -/
theorem pruefe_nicht_5 : pruefe 18 5 (code matrix) = false := by native_decide

/-- Radius 5 reicht nicht: Es gibt ein x < 2^18 ohne Wort im Abstand höchstens 5. -/
theorem nicht_radius5 : ¬ ∀ x, x < 2 ^ 18 → ∃ c ∈ code matrix, abstand 18 x c ≤ 5 := by
  rw [← pruefe_iff, pruefe_nicht_5]; decide

end Ueberdeckung.C18_5_50w_ohne24
