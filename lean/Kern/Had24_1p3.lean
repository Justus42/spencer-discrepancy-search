-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Had24_1p3.Teil0
import Kern.Had24_1p3.Teil1
import Kern.Had24_1p3.Teil2
import Kern.Had24_1p3.Teil3
import Kern.Had24_1p3.Teil4
import Kern.Had24_1p3.Teil5
import Kern.Had24_1p3.Teil6
import Kern.Had24_1p3.Teil7
import Kern.Had24_1p3.Teil8
import Kern.Had24_1p3.Teil9
import Kern.Had24_1p3.Teil10
import Kern.Had24_1p3.Teil11
import Kern.Had24_1p3.Teil12
import Kern.Had24_1p3.Teil13
import Kern.Had24_1p3.Teil14
import Kern.Had24_1p3.Teil15
import Kern.Had24_1p3.Teil16
import Kern.Had24_1p3.Teil17
import Kern.Had24_1p3.Teil18
import Kern.Had24_1p3.Teil19
import Kern.Had24_1p3.Teil20
import Kern.Had24_1p3.Teil21
import Kern.Had24_1p3.Teil22
import Kern.Had24_1p3.Teil23
import Kern.Had24_1p3.Teil24
import Kern.Had24_1p3.Teil25
import Kern.Had24_1p3.Teil26
import Kern.Had24_1p3.Teil27
import Kern.Had24_1p3.Teil28
import Kern.Had24_1p3.Teil29
import Kern.Had24_1p3.Teil30
import Kern.Had24_1p3.Teil31
import Kern.Had24_1p3.Teil32
import Kern.Had24_1p3.Teil33
import Kern.Had24_1p3.Teil34
import Kern.Had24_1p3.Teil35
import Kern.Had24_1p3.Teil36
import Kern.Had24_1p3.Teil37
import Kern.Had24_1p3.Teil38
import Kern.Had24_1p3.Teil39
import Kern.Had24_1p3.Teil40
import Kern.Had24_1p3.Teil41
import Kern.Had24_1p3.Teil42
import Kern.Had24_1p3.Teil43
import Kern.Had24_1p3.Teil44
import Kern.Had24_1p3.Teil45
import Kern.Had24_1p3.Teil46
import Kern.Had24_1p3.Teil47
import Kern.Had24_1p3.Teil48
import Kern.Had24_1p3.Teil49
import Kern.Had24_1p3.Teil50
import Kern.Had24_1p3.Teil51
import Kern.Had24_1p3.Teil52
import Kern.Had24_1p3.Teil53
import Kern.Had24_1p3.Teil54
import Kern.Had24_1p3.Teil55
import Kern.Had24_1p3.Teil56
import Kern.Had24_1p3.Teil57
import Kern.Had24_1p3.Teil58
import Kern.Had24_1p3.Teil59
import Kern.Had24_1p3.Teil60
import Kern.Had24_1p3.Teil61
import Kern.Had24_1p3.Teil62
import Kern.Had24_1p3.Teil63
import Kern.Swar20

namespace Ueberdeckung.Kern.Had24_1p3
open Ueberdeckung

theorem n_0_128 : ∀ j, j < 128 → Q (0 + j) := baum_knoten Q 0 64 n_0_64 n_64_64
theorem n_128_128 : ∀ j, j < 128 → Q (128 + j) := baum_knoten Q 128 64 n_128_64 n_192_64
theorem n_0_256 : ∀ j, j < 256 → Q (0 + j) := baum_knoten Q 0 128 n_0_128 n_128_128
theorem n_256_128 : ∀ j, j < 128 → Q (256 + j) := baum_knoten Q 256 64 n_256_64 n_320_64
theorem n_384_128 : ∀ j, j < 128 → Q (384 + j) := baum_knoten Q 384 64 n_384_64 n_448_64
theorem n_256_256 : ∀ j, j < 256 → Q (256 + j) := baum_knoten Q 256 128 n_256_128 n_384_128
theorem n_0_512 : ∀ j, j < 512 → Q (0 + j) := baum_knoten Q 0 256 n_0_256 n_256_256
theorem n_512_128 : ∀ j, j < 128 → Q (512 + j) := baum_knoten Q 512 64 n_512_64 n_576_64
theorem n_640_128 : ∀ j, j < 128 → Q (640 + j) := baum_knoten Q 640 64 n_640_64 n_704_64
theorem n_512_256 : ∀ j, j < 256 → Q (512 + j) := baum_knoten Q 512 128 n_512_128 n_640_128
theorem n_768_128 : ∀ j, j < 128 → Q (768 + j) := baum_knoten Q 768 64 n_768_64 n_832_64
theorem n_896_128 : ∀ j, j < 128 → Q (896 + j) := baum_knoten Q 896 64 n_896_64 n_960_64
theorem n_768_256 : ∀ j, j < 256 → Q (768 + j) := baum_knoten Q 768 128 n_768_128 n_896_128
theorem n_512_512 : ∀ j, j < 512 → Q (512 + j) := baum_knoten Q 512 256 n_512_256 n_768_256
theorem n_0_1024 : ∀ j, j < 1024 → Q (0 + j) := baum_knoten Q 0 512 n_0_512 n_512_512
theorem n_1024_128 : ∀ j, j < 128 → Q (1024 + j) := baum_knoten Q 1024 64 n_1024_64 n_1088_64
theorem n_1152_128 : ∀ j, j < 128 → Q (1152 + j) := baum_knoten Q 1152 64 n_1152_64 n_1216_64
theorem n_1024_256 : ∀ j, j < 256 → Q (1024 + j) := baum_knoten Q 1024 128 n_1024_128 n_1152_128
theorem n_1280_128 : ∀ j, j < 128 → Q (1280 + j) := baum_knoten Q 1280 64 n_1280_64 n_1344_64
theorem n_1408_128 : ∀ j, j < 128 → Q (1408 + j) := baum_knoten Q 1408 64 n_1408_64 n_1472_64
theorem n_1280_256 : ∀ j, j < 256 → Q (1280 + j) := baum_knoten Q 1280 128 n_1280_128 n_1408_128
theorem n_1024_512 : ∀ j, j < 512 → Q (1024 + j) := baum_knoten Q 1024 256 n_1024_256 n_1280_256
theorem n_1536_128 : ∀ j, j < 128 → Q (1536 + j) := baum_knoten Q 1536 64 n_1536_64 n_1600_64
theorem n_1664_128 : ∀ j, j < 128 → Q (1664 + j) := baum_knoten Q 1664 64 n_1664_64 n_1728_64
theorem n_1536_256 : ∀ j, j < 256 → Q (1536 + j) := baum_knoten Q 1536 128 n_1536_128 n_1664_128
theorem n_1792_128 : ∀ j, j < 128 → Q (1792 + j) := baum_knoten Q 1792 64 n_1792_64 n_1856_64
theorem n_1920_128 : ∀ j, j < 128 → Q (1920 + j) := baum_knoten Q 1920 64 n_1920_64 n_1984_64
theorem n_1792_256 : ∀ j, j < 256 → Q (1792 + j) := baum_knoten Q 1792 128 n_1792_128 n_1920_128
theorem n_1536_512 : ∀ j, j < 512 → Q (1536 + j) := baum_knoten Q 1536 256 n_1536_256 n_1792_256
theorem n_1024_1024 : ∀ j, j < 1024 → Q (1024 + j) := baum_knoten Q 1024 512 n_1024_512 n_1536_512
theorem n_0_2048 : ∀ j, j < 2048 → Q (0 + j) := baum_knoten Q 0 1024 n_0_1024 n_1024_1024
theorem n_2048_128 : ∀ j, j < 128 → Q (2048 + j) := baum_knoten Q 2048 64 n_2048_64 n_2112_64
theorem n_2176_128 : ∀ j, j < 128 → Q (2176 + j) := baum_knoten Q 2176 64 n_2176_64 n_2240_64
theorem n_2048_256 : ∀ j, j < 256 → Q (2048 + j) := baum_knoten Q 2048 128 n_2048_128 n_2176_128
theorem n_2304_128 : ∀ j, j < 128 → Q (2304 + j) := baum_knoten Q 2304 64 n_2304_64 n_2368_64
theorem n_2432_128 : ∀ j, j < 128 → Q (2432 + j) := baum_knoten Q 2432 64 n_2432_64 n_2496_64
theorem n_2304_256 : ∀ j, j < 256 → Q (2304 + j) := baum_knoten Q 2304 128 n_2304_128 n_2432_128
theorem n_2048_512 : ∀ j, j < 512 → Q (2048 + j) := baum_knoten Q 2048 256 n_2048_256 n_2304_256
theorem n_2560_128 : ∀ j, j < 128 → Q (2560 + j) := baum_knoten Q 2560 64 n_2560_64 n_2624_64
theorem n_2688_128 : ∀ j, j < 128 → Q (2688 + j) := baum_knoten Q 2688 64 n_2688_64 n_2752_64
theorem n_2560_256 : ∀ j, j < 256 → Q (2560 + j) := baum_knoten Q 2560 128 n_2560_128 n_2688_128
theorem n_2816_128 : ∀ j, j < 128 → Q (2816 + j) := baum_knoten Q 2816 64 n_2816_64 n_2880_64
theorem n_2944_128 : ∀ j, j < 128 → Q (2944 + j) := baum_knoten Q 2944 64 n_2944_64 n_3008_64
theorem n_2816_256 : ∀ j, j < 256 → Q (2816 + j) := baum_knoten Q 2816 128 n_2816_128 n_2944_128
theorem n_2560_512 : ∀ j, j < 512 → Q (2560 + j) := baum_knoten Q 2560 256 n_2560_256 n_2816_256
theorem n_2048_1024 : ∀ j, j < 1024 → Q (2048 + j) := baum_knoten Q 2048 512 n_2048_512 n_2560_512
theorem n_3072_128 : ∀ j, j < 128 → Q (3072 + j) := baum_knoten Q 3072 64 n_3072_64 n_3136_64
theorem n_3200_128 : ∀ j, j < 128 → Q (3200 + j) := baum_knoten Q 3200 64 n_3200_64 n_3264_64
theorem n_3072_256 : ∀ j, j < 256 → Q (3072 + j) := baum_knoten Q 3072 128 n_3072_128 n_3200_128
theorem n_3328_128 : ∀ j, j < 128 → Q (3328 + j) := baum_knoten Q 3328 64 n_3328_64 n_3392_64
theorem n_3456_128 : ∀ j, j < 128 → Q (3456 + j) := baum_knoten Q 3456 64 n_3456_64 n_3520_64
theorem n_3328_256 : ∀ j, j < 256 → Q (3328 + j) := baum_knoten Q 3328 128 n_3328_128 n_3456_128
theorem n_3072_512 : ∀ j, j < 512 → Q (3072 + j) := baum_knoten Q 3072 256 n_3072_256 n_3328_256
theorem n_3584_128 : ∀ j, j < 128 → Q (3584 + j) := baum_knoten Q 3584 64 n_3584_64 n_3648_64
theorem n_3712_128 : ∀ j, j < 128 → Q (3712 + j) := baum_knoten Q 3712 64 n_3712_64 n_3776_64
theorem n_3584_256 : ∀ j, j < 256 → Q (3584 + j) := baum_knoten Q 3584 128 n_3584_128 n_3712_128
theorem n_3840_128 : ∀ j, j < 128 → Q (3840 + j) := baum_knoten Q 3840 64 n_3840_64 n_3904_64
theorem n_3968_128 : ∀ j, j < 128 → Q (3968 + j) := baum_knoten Q 3968 64 n_3968_64 n_4032_64
theorem n_3840_256 : ∀ j, j < 256 → Q (3840 + j) := baum_knoten Q 3840 128 n_3840_128 n_3968_128
theorem n_3584_512 : ∀ j, j < 512 → Q (3584 + j) := baum_knoten Q 3584 256 n_3584_256 n_3840_256
theorem n_3072_1024 : ∀ j, j < 1024 → Q (3072 + j) := baum_knoten Q 3072 512 n_3072_512 n_3584_512
theorem n_2048_2048 : ∀ j, j < 2048 → Q (2048 + j) := baum_knoten Q 2048 1024 n_2048_1024 n_3072_1024
theorem n_0_4096 : ∀ j, j < 4096 → Q (0 + j) := baum_knoten Q 0 2048 n_0_2048 n_2048_2048

end Ueberdeckung.Kern.Had24_1p3

/-! Teil 5: dieselben Aussagen wie in `Ueberdeckung/Saetze/Had24_1p3.lean`, nur mit `decide +kernel`. -/

namespace Ueberdeckung.Had24_1p3
open Ueberdeckung

theorem woerter_eq_kern : code matrix = woerter := by decide +kernel

theorem woerter_klein_kern : ∀ c ∈ woerter, c < 2 ^ 20 := by decide +kernel

theorem bloecke_kern : ∀ b, b < 2 ^ (20 - 8) → blockS 6 8 b woerter = true := fun b hb => by
  have := Kern.Had24_1p3.n_0_4096 b hb; rwa [Nat.zero_add] at this

/-- Jedes x < 2^20 hat ein Wort der Liste im Hamming-Abstand höchstens 6. -/
theorem radius6_kern : ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6 := by
  rw [woerter_eq_kern]
  exact aus_bloeckenS Kern.Swar20.swar_ok 6 8 woerter (by decide) woerter_klein_kern bloecke_kern

/-- Hauptsatz in Matrixform (Kernel-Fassung). -/
theorem hauptsatz_kern : ∀ x, x < 2 ^ 20 →
    ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 20 x z ≤ 6 :=
  (code_iff_matrix 20 6 matrix).1 radius6_kern

/-- Prüfschleife lehnt Radius 5 ab (Kernel). -/
theorem pruefe_nicht_5_kern : pruefe 20 5 (code matrix) = false := by decide +kernel

theorem nicht_radius5_kern : ¬ ∀ x, x < 2 ^ 20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 5 := by
  rw [← pruefe_iff, pruefe_nicht_5_kern]; decide

end Ueberdeckung.Had24_1p3
