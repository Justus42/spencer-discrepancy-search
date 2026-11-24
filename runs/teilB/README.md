# Teil B, Prüfstand (24.11.2026)

`src/pruefstand.c` (Suchcode, Tabu-Suche), bauen: `gcc -O3 -march=native -o build/pruefstand src/pruefstand.c`.

Kontrollen laut Auftrag (je 5 s): `ctrl_8` bestes Maximum 5, `had24_1p3_paley12` 8, Hadamard 24 (McKay Klasse 1) 6;
mit `-d` gleich der exakten Diskrepanz jeweils "kein Gegenbeispiel gefunden" (richtig, es gibt keins).
Zusatz `kontrollen.log.txt`: 12 Matrizen (alle aus `controls/`, McKay 1 und 60, Konferenz 18) x 3 Startwerte x 2 s,
Modus `-m`: in allen 36 Läufen exakt die Diskrepanz aus `build/exakt_disc` erreicht, jeweils nach unter 0,01 s.
