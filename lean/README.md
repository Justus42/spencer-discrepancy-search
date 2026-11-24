# Lean-Zertifikat: Überdeckungscodes, Stufe A (nur Kernbibliothek)

Lean bestätigt: Die 48 Wörter aus `controls/had24_1p3_paley12.txt` (24 Zeilen und ihre Komplemente, Bit 1 = Eintrag −1)
überdecken alle 2^20 Punkte mit Radius 6, also K(20,6) ≤ 48. Ebenso `had24_2p2_paley12.txt`. Kein Mathlib, kein `sorry`,
keine eigenen Axiome. Zwei unabhängige Wege: `native_decide` (Compiler) und `decide +kernel` (nur Kernel, keine Zusatzachse).

## Ergebnis (24.11.2026, Cloud, 4 Kerne, Protokoll `notes/laeufe/2026-11-24_lean.md`)

| Datensatz | Wörter | angenommen | abgelehnt | native_decide | decide +kernel |
|---|---|---|---|---|---|
| a) Hamming7 (`ctrl_8` ohne Nullspalte) | 16 | 1 | 0 | 0,6 s | 1,6 s |
| b) Ctrl16 (`ctrl_16` ohne Nullspalte) | 32 | 4 | 3 | 3,5 s | 21 s |
| c) Had24_1p3_ohne0 (Paar 0 weg) | 46 | — | 6 | 1,1 s | 1,1 s |
| Ziel Had24_1p3 | 48 | 6 | 5 | 120 s | 624 s (4 Prozesse) |
| Ziel Had24_2p2 | 48 | 6 | 5 | 121 s | 349 s (SWAR-Nachweis aus Had24_1p3 übernommen) |

`#print axioms`: Kernel-Fassung nur `propext`, `Quot.sound`; native Fassung zusätzlich das Hilfsaxiom von `native_decide`.

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
| `anzahl_woerter` | `code matrix` hat 48 Einträge, paarweise verschieden (`decide`) |
| `woerter_eq` | `code matrix = woerter` (die vom Erzeuger geschriebene Liste) |
| `radius6` | `∀ x, x < 2^20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 6` |
| `hauptsatz` | `∀ x, x < 2^20 → ∃ z ∈ matrix ++ matrix.map (List.map (fun e => -e)), abstandZeile 20 x z ≤ 6` |
| `nicht_radius5` | `¬ ∀ x, x < 2^20 → ∃ c ∈ code matrix, abstand 20 x c ≤ 5` |

Die Matrix steht wörtlich in `Ueberdeckung/Daten/<Name>.lean`, erzeugt von `erzeuge.py` (prüft SHA-256 und Form der Quelle).
Erzeugte Dateien nie von Hand ändern.

Die Sätze in `Ueberdeckung/Saetze/` nutzen `native_decide`: Lean vertraut dem Compiler. `#print axioms` zeigt dafür ein von
`native_decide` selbst angelegtes Hilfsaxiom (`…_native.native_decide.ax_1_1`, früher hieß das `Lean.ofReduceBool`).

`Kern/<Name>.lean` beweist dieselben Aussagen (`hauptsatz_kern`, `radius…_kern`, `nicht_radius…_kern`) mit `decide +kernel`:
nur der Lean-Kernel rechnet, kein Compiler, kein Hilfsaxiom. Damit das in Minuten statt Stunden geht:
- Bitzählung `swar` aus rohen, im Kernel beschleunigten Zahloperationen. Sie ist nicht allgemein bewiesen, sondern für alle
  `v < 2^m` im Kernel gegen die einfache Zählung `einsen m` nachgerechnet (`Kern.Swar<m>.swar_ok`).
- Die `2^n` Punkte in Blöcke zu `2^8` zerlegt, je Block ein Satz; die Blocksätze werden als Binärbaum zusammengesetzt
  (`baum_blatt`, `baum_knoten`) und auf Dateien zu höchstens 64 Blöcken verteilt, die `pruefe_kern.py` parallel prüft.
- Alle Hilfssätze (`alle_aus_bloecken`, `einsen_xor`, `naheS_iff`, `aus_bloeckenS`) sind in `Basis.lean` bewiesen.
Die Kern-Dateien schreibt `erzeuge_kern.py` (nur Textbausteine). Erzeugte Dateien nie von Hand ändern.

## Reproduktion (Linux, aus dem Repo-Wurzelordner)

```sh
# 1. Lean holen (Version wie lean/lean-toolchain), ca. 530 MB
curl -LO https://github.com/leanprover/lean4/releases/download/v4.30.0-rc2/lean-4.30.0-rc2-linux.tar.zst
# SHA-256 (in der Sitzung vom 24.11.2026 berechnet): 5b51625f154f0a13b37bd892f1d95f79e9fd5b9f0d095b4126215ee2bc8dbe86
mkdir -p ~/lean && tar --zstd -xf lean-4.30.0-rc2-linux.tar.zst -C ~/lean     # ohne zstd: siehe unten
export PATH=~/lean/lean-4.30.0-rc2-linux/bin:$PATH
#    (alternativ elan: elan toolchain install leanprover/lean4:v4.30.0-rc2)

# 2. Erzeugen (überschreibt lean/Ueberdeckung/Daten/*.lean und lean/Kern/, Ergebnis muss gleich bleiben)
sh lean/erzeuge_alle.sh
cd lean && python3 -I erzeuge_kern.py && sha256sum -c SHA256SUMS SHA256SUMS_KERN | grep -v ': OK$'; cd ..

# 3. Prüfen
cd lean
lake build                      # alle Sätze mit native_decide, ca. 2 Min. (2 Ziele parallel)
lake env lean Achsen.lean       # #print axioms für jeden Hauptsatz

# 4. Kernel-Fassung (Teil 5), je Stufe; 4 = höchstens 4 Lean-Prozesse (je bis ca. 2,5 GB Speicher)
python3 -I pruefe_kern.py Hamming7 4          # ca. 2 s
python3 -I pruefe_kern.py Ctrl16 4            # ca. 20 s
python3 -I pruefe_kern.py Had24_1p3 4         # ca. 10 Min. (enthält den SWAR-Nachweis für 2^20)
python3 -I pruefe_kern.py Had24_2p2 4         # ca. 5 Min. (SWAR-Nachweis schon gebaut)
python3 -I pruefe_kern.py Had24_1p3_ohne0 4
lake env lean AchsenKern.lean   # #print axioms der Kernel-Sätze
```

Nicht `lake build Kern.…` für die großen Stufen: `lake` startet zu viele Prozesse gleichzeitig, und jeder Blocksatz hält
etwa 25 MB fest (am 24.11.2026 so mit 16 GB ausgelaufen).

Ohne `zstd`: `pip install zstandard`, dann

```sh
python3 -c "import zstandard,tarfile; r=zstandard.ZstdDecompressor().stream_reader(open('lean-4.30.0-rc2-linux.tar.zst','rb')); tarfile.open(fileobj=r,mode='r|').extractall('$HOME/lean')"
```

Windows (nicht getestet): das Windows-Paket derselben Version von derselben Release-Seite holen (Dateiname dort nachsehen, in dieser Sitzung nicht geprüft) oder `elan` benutzen; die Befehle ab Schritt 2 gelten sinngemäß (Python statt `sh` für `erzeuge_alle.sh` nötigenfalls Zeile für Zeile).
