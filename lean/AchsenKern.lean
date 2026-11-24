import Kern.Hamming7
import Kern.Ctrl16
import Kern.Had24_1p3_ohne0
import Kern.Had24_1p3
import Kern.Had24_2p2

/-! `#print axioms` für die Kernel-Fassung (Teil 5). Aufruf nach `python3 pruefe_kern.py` für alle Stufen:
`lake env lean AchsenKern.lean` -/

#print axioms Ueberdeckung.Kern.Swar20.swar_ok
#print axioms Ueberdeckung.Hamming7.hauptsatz_kern
#print axioms Ueberdeckung.Hamming7.nicht_radius0_kern
#print axioms Ueberdeckung.Ctrl16.hauptsatz_kern
#print axioms Ueberdeckung.Ctrl16.nicht_radius3_kern
#print axioms Ueberdeckung.Had24_1p3_ohne0.nicht_radius6_kern
#print axioms Ueberdeckung.Had24_1p3.radius6_kern
#print axioms Ueberdeckung.Had24_1p3.hauptsatz_kern
#print axioms Ueberdeckung.Had24_1p3.nicht_radius5_kern
#print axioms Ueberdeckung.Had24_2p2.radius6_kern
#print axioms Ueberdeckung.Had24_2p2.hauptsatz_kern
#print axioms Ueberdeckung.Had24_2p2.nicht_radius5_kern
