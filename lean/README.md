# Lean-Zertifikat: Überdeckungscodes, Stufe A (nur Kernbibliothek)

Lean bestätigt: Die 48 Wörter aus `controls/had24_1p3_paley12.txt` (24 Zeilen und ihre Komplemente, Bit 1 = Eintrag −1)
überdecken alle 2^20 Punkte mit Radius 6, also K(20,6) ≤ 48. Ebenso `had24_2p2_paley12.txt`. Kein Mathlib, kein `sorry`,
keine eigenen Axiome.

## Was bewiesen wird

`Ueberdeckung/Basis.lean` (von Hand, kurz, lesen!):

- `abstand n x y` = Anzahl der `i < n` mit `x.testBit i ≠ y.testBit i` (Spezifikation, keine Rechenabkürzung).
- `wort z`: Bit `j` = 1 genau dann, wenn Eintrag `j` der Zeile `z` gleich −1 ist. `code M` = Wörter der Zeilen von `M` und von `−M`.
- `pruefe n r C`: schnelle Prüfschleife. **`pruefe_iff`** beweist: `pruefe n r C = true ↔ ∀ x < 2^n, ∃ c ∈ C, abstand n x c ≤ r`.
- `abstandZeile n x z` = Anzahl der `j < n`, an denen Bit `j` von `x` nicht zu „Eintrag `j` von `z` ist −1“ passt;
  `code_iff_matrix` übersetzt die Aussage über Wörter in eine Aussage direkt über die ±1-Matrix.

Je Datensatz `Ueberdeckung/Saetze/<Name>.lean` (Beispiel `Had24_1p3`):

| Satz | Aussage |
|---|---|
| `form` | 24 Zeilen, jede Länge 20, nur ±1 (`decide`) |
| `woerter` | `code matrix` hat 48 Einträge, paarweise verschieden (`decide`) |
| `radius6` | `∀ x, x < 2^20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6` |
| `hauptsatz` | `∀ x, x < 2^20 → ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 20 x z ≤ 6` |
| `nicht_radius5` | `¬ ∀ x, x < 2^20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 5` |

Die Matrix steht wörtlich in `Ueberdeckung/Daten/<Name>.lean`, erzeugt von `erzeuge.py` (prüft SHA-256 und Form der Quelle).
Erzeugte Dateien nie von Hand ändern.

Die Sätze in `Ueberdeckung/Saetze/` nutzen `native_decide` (vertraut dem Lean-Compiler, Achse `Lean.ofReduceBool`).
`Kern/<Name>.lean` beweist dieselben Sätze mit `decide +kernel` (nur der Kernel rechnet, keine Zusatzachse); Ergebnisse siehe unten.

## Reproduktion (Linux, aus dem Repo-Wurzelordner)

```sh
# 1. Lean holen (Version wie lean/lean-toolchain), ca. 530 MB
curl -LO https://github.com/leanprover/lean4/releases/download/v4.30.0-rc2/lean-4.30.0-rc2-linux.tar.zst
# SHA-256 (in der Sitzung vom 24.11.2026 berechnet): 5b51625f154f0a13b37bd892f1d95f79e9fd5b9f0d095b4126215ee2bc8dbe86
mkdir -p ~/lean && tar --zstd -xf lean-4.30.0-rc2-linux.tar.zst -C ~/lean     # ohne zstd: siehe unten
export PATH=~/lean/lean-4.30.0-rc2-linux/bin:$PATH
#    (alternativ elan: elan toolchain install leanprover/lean4:v4.30.0-rc2)

# 2. Erzeugen (überschreibt lean/Ueberdeckung/Daten/*.lean, Ergebnis muss gleich bleiben)
sh lean/erzeuge_alle.sh
cd lean && sha256sum -c SHA256SUMS && cd ..

# 3. Prüfen
cd lean
lake build                      # alle Sätze mit native_decide, ca. 4 Min. auf 1 Kern
lake env lean Achsen.lean       # #print axioms für jeden Hauptsatz
lake build Kern.Hamming7        # Teil 5, Kernel-Variante, je Stufe einzeln
```

Ohne `zstd`: `pip install zstandard`, dann

```sh
python3 -c "import zstandard,tarfile; r=zstandard.ZstdDecompressor().stream_reader(open('lean-4.30.0-rc2-linux.tar.zst','rb')); tarfile.open(fileobj=r,mode='r|').extractall('$HOME/lean')"
```

Windows: dieselbe Version als `lean-4.30.0-rc2-windows.zip` von derselben Release-Seite, sonst gleich (PowerShell statt `sh`).
