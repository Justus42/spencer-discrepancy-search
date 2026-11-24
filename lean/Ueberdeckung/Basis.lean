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

/-! ## Blockweise Prüfung (für den Kernel-Versuch: je Block ein eigener Satz) -/

/-- Gilt `f` auf jedem Block `{b * 2^s + y | y < 2^s}` für alle `b < 2^(n-s)`, so gilt `f` unterhalb `2^n`. -/
theorem alle_aus_bloecken (n s : Nat) (f : Nat → Bool) (hs : s ≤ n)
    (h : ∀ b, b < 2 ^ (n - s) → alleUnter s (fun y => f (b * 2 ^ s + y)) = true) :
    ∀ x, x < 2 ^ n → f x = true := by
  intro x hx
  have hp : 0 < 2 ^ s := Nat.pow_pos (by decide)
  have hn : 2 ^ n = 2 ^ (n - s) * 2 ^ s := by rw [← Nat.pow_add, Nat.sub_add_cancel hs]
  have hb : x / 2 ^ s < 2 ^ (n - s) := by
    rw [Nat.div_lt_iff_lt_mul hp, ← hn]; exact hx
  have hy : x % 2 ^ s < 2 ^ s := Nat.mod_lt _ hp
  have := (alleUnter_iff s _).1 (h _ hb) _ hy
  rwa [show x / 2 ^ s * 2 ^ s + x % 2 ^ s = x by
    rw [Nat.mul_comm]; exact Nat.div_add_mod x (2 ^ s)] at this

/-- Sammelschritt: aus `P b` für alle `b < k` und `P k` folgt `P b` für alle `b < k + 1`. -/
theorem bis_succ {P : Nat → Prop} {k : Nat} (h : ∀ b, b < k → P b) (hk : P k) :
    ∀ b, b < k + 1 → P b := by
  intro b hb
  rcases Nat.lt_succ_iff_lt_or_eq.1 hb with h' | rfl
  · exact h b h'
  · exact hk

theorem bis_null {P : Nat → Prop} : ∀ b, b < 0 → P b := fun _ h => absurd h (Nat.not_lt_zero _)

/-! ## Rechenform für den Kernel: XOR und Bitzählung -/

/-- Anzahl der Einsen in den unteren `k` Bits von `v`. -/
def einsen : Nat → Nat → Nat
  | 0, _ => 0
  | k + 1, v => v % 2 + einsen k (v / 2)

theorem einsen_xor (k x y : Nat) : einsen k (x ^^^ y) = abstandR k x y := by
  induction k generalizing x y with
  | zero => rfl
  | succ k ih =>
    simp only [einsen, abstandR, Nat.xor_div_two, ih]
    congr 1
    have h1 := @Nat.xor_mod_two_eq_one x y
    rcases Nat.mod_two_eq_zero_or_one x with hx | hx <;>
      rcases Nat.mod_two_eq_zero_or_one y with hy | hy <;>
      rcases Nat.mod_two_eq_zero_or_one (x ^^^ y) with hz | hz <;>
      simp_all

/-- Gibt es in `W` ein Wort im Abstand höchstens `r` von `x`? -/
def nahe (n r x : Nat) : List Nat → Bool
  | [] => false
  | c :: W => Nat.ble (einsen n (x ^^^ c)) r || nahe n r x W

theorem nahe_iff (n r x : Nat) (W : List Nat) :
    nahe n r x W = true ↔ ∃ c ∈ W, abstand n x c ≤ r := by
  induction W with
  | nil => simp [nahe]
  | cons c W ih =>
    simp only [nahe, Bool.or_eq_true, Nat.ble_eq, ih, einsen_xor, abstandR_eq_abstand, List.mem_cons]
    constructor
    · rintro (h | ⟨d, hd, h⟩)
      · exact ⟨c, Or.inl rfl, h⟩
      · exact ⟨d, Or.inr hd, h⟩
    · rintro ⟨d, (rfl | hd), h⟩
      · exact Or.inl h
      · exact Or.inr ⟨d, hd, h⟩

/-- Block `b` der Größe `2^s` mit der Kernel-Rechenform. -/
def blockK (n r s b : Nat) (W : List Nat) : Bool :=
  alleUnter s (fun y => nahe n r (b * 2 ^ s + y) W)

/-- Bestehen alle Blöcke, gilt die Überdeckungsaussage. -/
theorem aus_bloecken (n r s : Nat) (W : List Nat) (hs : s ≤ n)
    (h : ∀ b, b < 2 ^ (n - s) → blockK n r s b W = true) :
    ∀ x, x < 2 ^ n → ∃ c ∈ W, abstand n x c ≤ r := fun x hx =>
  (nahe_iff n r x W).1 (alle_aus_bloecken n s (fun x => nahe n r x W) hs h x hx)

/-! ## Schnelle Bitzählung (SWAR) für den Kernel

`swar` benutzt nur Kernel-beschleunigte Operationen (`Nat.land`, `Nat.shiftRight`, `Nat.add`, `Nat.mul`) mit rohen
Zahlliteralen. Ihre Richtigkeit wird nicht allgemein bewiesen, sondern durch Nachrechnen aller
Werte `v < 2^m` gegen `einsen m` im Kernel (`swar_aus_bloecken`). -/

/-- Bitzählung nach dem SWAR-Verfahren (für Zahlen unter `2^32` gedacht). -/
def swar (v : Nat) : Nat :=
  let a := Nat.add (Nat.land v (nat_lit 0x55555555)) (Nat.land (Nat.shiftRight v (nat_lit 1)) (nat_lit 0x55555555))
  let b := Nat.add (Nat.land a (nat_lit 0x33333333)) (Nat.land (Nat.shiftRight a (nat_lit 2)) (nat_lit 0x33333333))
  let c := Nat.land (Nat.add b (Nat.shiftRight b (nat_lit 4))) (nat_lit 0x0F0F0F0F)
  Nat.land (Nat.shiftRight (Nat.mul c (nat_lit 0x01010101)) (nat_lit 24)) (nat_lit 0xFF)

/-- Block `b` der Größe `2^s` für den Vergleich `swar = einsen m`. -/
def swarBlock (m s b : Nat) : Bool :=
  alleUnter s (fun y => Nat.beq (swar (b * 2 ^ s + y)) (einsen m (b * 2 ^ s + y)))

theorem swar_aus_bloecken (m s : Nat) (hs : s ≤ m) (h : ∀ b, b < 2 ^ (m - s) → swarBlock m s b = true) :
    ∀ v, v < 2 ^ m → swar v = einsen m v := fun v hv =>
  Nat.eq_of_beq_eq_true (alle_aus_bloecken m s (fun v => Nat.beq (swar v) (einsen m v)) hs h v hv)

/-- Wie `nahe`, aber mit `swar` und `Nat.xor`. -/
def naheS (r x : Nat) : List Nat → Bool
  | [] => false
  | c :: W => Nat.ble (swar (Nat.xor x c)) r || naheS r x W

theorem naheS_iff {n r x : Nat} {W : List Nat} (hswar : ∀ v, v < 2 ^ n → swar v = einsen n v)
    (hx : x < 2 ^ n) (hW : ∀ c ∈ W, c < 2 ^ n) :
    naheS r x W = true ↔ ∃ c ∈ W, abstand n x c ≤ r := by
  induction W with
  | nil => simp [naheS]
  | cons c W ih =>
    have hc := hW c (List.mem_cons_self ..)
    have he : swar (Nat.xor x c) = abstand n x c := by
      show swar (x ^^^ c) = abstand n x c
      rw [hswar _ (Nat.xor_lt_two_pow hx hc), einsen_xor, abstandR_eq_abstand]
    simp only [naheS, Bool.or_eq_true, Nat.ble_eq, he, ih (fun d hd => hW d (List.mem_cons_of_mem _ hd)),
      List.mem_cons]
    constructor
    · rintro (h | ⟨d, hd, h⟩)
      · exact ⟨c, Or.inl rfl, h⟩
      · exact ⟨d, Or.inr hd, h⟩
    · rintro ⟨d, (rfl | hd), h⟩
      · exact Or.inl h
      · exact Or.inr ⟨d, hd, h⟩

/-- Block `b` der Größe `2^s` mit `naheS`. -/
def blockS (r s b : Nat) (W : List Nat) : Bool :=
  alleUnter s (fun y => naheS r (b * 2 ^ s + y) W)

theorem aus_bloeckenS {n : Nat} (hswar : ∀ v, v < 2 ^ n → swar v = einsen n v) (r s : Nat) (W : List Nat)
    (hs : s ≤ n) (hW : ∀ c ∈ W, c < 2 ^ n)
    (h : ∀ b, b < 2 ^ (n - s) → blockS r s b W = true) :
    ∀ x, x < 2 ^ n → ∃ c ∈ W, abstand n x c ≤ r := fun x hx =>
  (naheS_iff hswar hx hW).1 (alle_aus_bloecken n s (fun x => naheS r x W) hs h x hx)

/-! ## Zusammensetzen vieler Blocksätze (Baum statt Kette, damit die Schachtelung flach bleibt) -/

theorem baum_blatt (P : Nat → Prop) (o : Nat) (h : P o) : ∀ j, j < 1 → P (o + j) := by
  intro j hj; rw [show j = 0 by omega, Nat.add_zero]; exact h

theorem baum_knoten (P : Nat → Prop) (o k : Nat) (h1 : ∀ j, j < k → P (o + j))
    (h2 : ∀ j, j < k → P (o + k + j)) : ∀ j, j < k + k → P (o + j) := by
  intro j hj
  by_cases h : j < k
  · exact h1 j h
  · have := h2 (j - k) (by omega); rwa [Nat.add_assoc, Nat.add_sub_cancel' (by omega)] at this

end Ueberdeckung
