-- ERZEUGT von lean/erzeuge_kern.py. Nicht von Hand ändern.
import Kern.Swar7.Q

namespace Ueberdeckung.Kern.Swar7
open Ueberdeckung

theorem n_0_1 : ∀ j, j < 1 → Q (0 + j) := baum_blatt Q 0 (by decide +kernel)

end Ueberdeckung.Kern.Swar7
