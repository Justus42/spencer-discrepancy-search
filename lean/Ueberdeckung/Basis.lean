/-!
# Überdeckungscodes: Definitionen, Prüffunktion, Korrektheit

Nur Lean-Kernbibliothek, kein Mathlib, kein `sorry`, keine eigenen Axiome.

* `abstand n x y`: Spezifikation des Hamming-Abstands der Bits `0 … n-1` (Anzahl der Stellen mit `testBit` verschieden).
* `wort z`: Bit `j` ist 1 genau dann, wenn Eintrag `j` der Zeile `z` gleich `-1` ist.
* `code M`: Wörter der Zeilen von `M` und ihrer Komplemente (Zeilen von `-M`).
* `pruefe n r C`: schnelle Prüfschleife über alle `x < 2^n`; `pruefe_iff` beweist, dass sie genau die Überdeckung entscheidet.
-/

namespace Ueberdeckung

/-- Spezifikation: Anzahl der Stellen `i < n`, an denen sich `x` und `y` unterscheiden. -/
def abstand (n x y : Nat) : Nat :=
  ((List.range n).filter (fun i => x.testBit i != y.testBit i)).length

/-- Rechenform des Abstands (Bit für Bit über `% 2` und `/ 2`). -/
def abstandR : Nat → Nat → Nat → Nat
  | 0, _, _ => 0
  | n + 1, x, y => (if x % 2 = y % 2 then 0 else 1) + abstandR n (x / 2) (y / 2)

/-- Wort einer ±1-Zeile: Bit `j` gleich 1 genau dann, wenn Eintrag `j` gleich `-1` ist. -/
def wort : List Int → Nat
  | [] => 0
  | e :: z => (if e = -1 then 1 else 0) + 2 * wort z

/-- Zeilen und Komplemente (Zeilen von `-M`) als Wörter. -/
def code (M : List (List Int)) : List Nat :=
  M.map wort ++ M.map (fun z => wort (z.map (fun e => -e)))

/-- `alleUnter k f = true` genau dann, wenn `f y = true` für alle `y < 2^k` (Halbierung, Rekursionstiefe `k`). -/
def alleUnter : Nat → (Nat → Bool) → Bool
  | 0, f => f 0
  | k + 1, f => alleUnter k (fun y => f (2 * y)) && alleUnter k (fun y => f (2 * y + 1))

/-- Prüfschleife: jedes `x < 2^n` hat ein Wort in `C` im Abstand höchstens `r`. -/
def pruefe (n r : Nat) (C : List Nat) : Bool :=
  alleUnter n (fun x => C.any (fun c => decide (abstandR n x c ≤ r)))

/-! ## Korrektheit -/

theorem abstandR_eq_abstand (n x y : Nat) : abstandR n x y = abstand n x y := by
  induction n generalizing x y with
  | zero => simp [abstandR, abstand]
  | succ n ih =>
    simp only [abstandR, ih, abstand, List.range_succ_eq_map, List.filter_cons, List.filter_map,
      Function.comp_def, Nat.succ_eq_add_one, ← Nat.testBit_div_two, Nat.testBit_zero]
    have hx := Nat.mod_two_eq_zero_or_one x
    have hy := Nat.mod_two_eq_zero_or_one y
    rcases hx with hx | hx <;> rcases hy with hy | hy <;> simp [hx, hy] <;> omega

theorem alleUnter_iff (k : Nat) (f : Nat → Bool) :
    alleUnter k f = true ↔ ∀ y, y < 2 ^ k → f y = true := by
  induction k generalizing f with
  | zero => simp only [alleUnter, Nat.pow_zero]; constructor
            · intro h y hy; have : y = 0 := by omega
              subst this; exact h
            · intro h; exact h 0 (by omega)
  | succ k ih =>
    simp only [alleUnter, Bool.and_eq_true, ih]
    constructor
    · rintro ⟨h0, h1⟩ y hy
      have hy2 : y / 2 < 2 ^ k := by rw [Nat.pow_succ] at hy; omega
      rcases Nat.mod_two_eq_zero_or_one y with h | h
      · have := h0 (y / 2) hy2; rwa [show 2 * (y / 2) = y by omega] at this
      · have := h1 (y / 2) hy2; rwa [show 2 * (y / 2) + 1 = y by omega] at this
    · intro h
      refine ⟨fun y hy => h _ ?_, fun y hy => h _ ?_⟩ <;> rw [Nat.pow_succ] <;> omega

/-- Die Prüfschleife entscheidet genau die Überdeckungsaussage. -/
theorem pruefe_iff (n r : Nat) (C : List Nat) :
    pruefe n r C = true ↔ ∀ x, x < 2 ^ n → ∃ c ∈ C, abstand n x c ≤ r := by
  simp only [pruefe, alleUnter_iff, List.any_eq_true, decide_eq_true_eq, abstandR_eq_abstand]

/-- Bitform von `wort`: Bit `j` ist gesetzt genau dann, wenn Eintrag `j` existiert und gleich `-1` ist. -/
theorem testBit_wort (z : List Int) (j : Nat) : (wort z).testBit j = decide (z[j]? = some (-1)) := by
  induction z generalizing j with
  | nil => simp [wort]
  | cons e z ih =>
    cases j with
    | zero =>
      simp only [wort, Nat.testBit_zero, List.getElem?_cons_zero, Option.some.injEq]
      by_cases he : e = -1 <;> simp [he] <;> omega
    | succ j =>
      rw [Nat.testBit_succ, List.getElem?_cons_succ, ← ih j]
      congr 1; simp only [wort]; split <;> omega

/-- Abstand zu einem Zeilenwort, ausgedrückt nur durch die ±1-Einträge der Zeile. -/
theorem abstand_wort (n x : Nat) (z : List Int) :
    abstand n x (wort z) =
      ((List.range n).filter (fun j => x.testBit j != decide (z[j]? = some (-1)))).length := by
  simp only [abstand, testBit_wort]

/-- Abstand einer Zahl `x` zu einer ±1-Zeile `z` auf den Stellen `0 … n-1`: Anzahl der `j < n`, an denen
Bit `j` von `x` nicht mit „Eintrag `j` von `z` ist `-1`“ übereinstimmt. -/
def abstandZeile (n x : Nat) (z : List Int) : Nat :=
  ((List.range n).filter (fun j => x.testBit j != decide (z[j]? = some (-1)))).length

/-- Überdeckung durch `code M` ist dasselbe wie Überdeckung durch die Zeilen von `M` und `-M`. -/
theorem code_iff_matrix (n r : Nat) (M : List (List Int)) :
    (∀ x, x < 2 ^ n → ∃ c ∈ code M, abstand n x c ≤ r) ↔
    (∀ x, x < 2 ^ n → ∃ z ∈ M ++ M.map (List.map (fun e => -e)), abstandZeile n x z ≤ r) := by
  have h : code M = (M ++ M.map (List.map (fun e => -e))).map wort := by
    simp [code, List.map_append, List.map_map, Function.comp_def]
  simp only [h, List.mem_map]
  constructor
  · intro H x hx
    obtain ⟨c, ⟨z, hz, rfl⟩, hd⟩ := H x hx
    exact ⟨z, hz, by rw [abstandZeile, ← abstand_wort]; exact hd⟩
  · intro H x hx
    obtain ⟨z, hz, hd⟩ := H x hx
    exact ⟨wort z, ⟨z, hz, rfl⟩, by rw [abstand_wort]; exact hd⟩

end Ueberdeckung
