# Vortest 25.11.2026: Codes im affinen Spann eines bekannten guten Codes

Rechnung des Orchestrators (Chat vom 25.11., ca. 11 bis 12 Uhr), zusammen unter 5 Minuten Rechenzeit, **nicht gegengerechnet**.
Skripte und Ausgabe: `notes/orchestrator/2026-11-25_vortest_spann/` (aus dem Repo-Wurzelordner starten).
Anlass: Kandidat 1 der Mittags-Übergabe („Codes von kleinem Rang“). `STATUS.md` und `LANDKARTE.md` sind nicht geändert.

## Idee

Ein selbstkomplementärer Code C liegt in seinem affinen Spann L (kleinster affiner Unterraum über GF(2), der C und den
Einsvektor enthält, 2^r Wörter). Statt im ganzen Würfel sucht man kleinere Codes nur unter den 2^(r−1) Wortpaaren von L.

**Zählschranke.** μ(x) = Zahl der Wortpaare von L im Radius R um x. μ ist auf Nebenklassen von L konstant. Ein Code aus
k Paaren von L überdeckt die 2^(r−1) Verschiebungen eines Punktes x zusammen genau k·μ(x)-mal. Also k ≥ 2^(r−1) / min μ.

## Befunde

| Code | Zelle | Rang r | Paare in L | min μ | Schranke | Folge |
|---|---|---|---|---|---|---|
| `ctrl_8` (Hamming) | (7,1) | 4 | 8 | 1 | 8 | Kontrolle: perfekt, wie erwartet |
| `ctrl_16` | (15,4) | 8 | 128 | 8 | 16 | **30 Wörter im Spann von `ctrl_16` unmöglich** (voll, Zählargument; CP-SAT: 16 Paare lösbar, 15 unlösbar, je 0,3 s) |
| `ctrl_17`, `L2026_17x17` | (17,5) | 8 | 128 | 8 | 16 | 30 Wörter im Spann unmöglich |
| `astra_19` | (19,6) | 8 | 128 | 8 | 16 | ebenso |
| `had24_1p3_paley12`, `had24_2p2_paley12` | (20,6) | 12 | 2048 | 212 | 9,66 | **20 Paare (40 Wörter, Rekordzelle) nicht ausgeschlossen**; μ-Werte 212 (36 Nebenklassen), 240 (219), 268 (1) |
| `bahn15_40w` | (15,4) | 11 | (nicht gerechnet) | | | |

Kern (Verschiebungen, die den Code festhalten): `ctrl_16` 2, `ctrl_17` 4, 48er 4.

**Rang-9-Erweiterungen bei (15,4):** L von `ctrl_16` plus eine der 127 Nebenklassen. Drei Sorten nach μ-Werten:
56 Erweiterungen mit min μ = 22, 70 mit 24, eine mit 28. Zählschranke 9,1 bis 11,6, also 15 Paare (30 Wörter, Quotient
7/√15 = 1,807) nicht ausgeschlossen. Exakter Versuch für die eine Erweiterung mit min μ = 28 (256 Paare, 6528 verschiedene
Bedingungen, CP-SAT, 2 Kerne): 16 Paare in 0,7 s gefunden, 15 Paare nach 120 s unentschieden.

**Kandidat 1 wörtlich (Rang 6, lineare [m,6]-Codes mit Einsvektor):** Ausdünnen verlangt min μ ≥ 2 (für 40 von 64, 50 von 64
und 62 von 64 Wörtern gleichermaßen). Stichprobe: (20,6) 6000 zufällige Codes, (25,8) 300: keiner mit min μ ≥ 2
(meist Radius zu groß, sonst min μ = 1). Belegart: Suche erfolglos, schließt nichts aus. Die bekannten guten Codes haben
Rang 8 und 12, nicht 6; Rang 6 hat also kein Vorbild.

## Was das bedeutet

- Die Zählschranke erklärt, warum `ctrl_16` in seinem eigenen Spann nicht zu schlagen ist: Dort ist er perfekt
  (16 · 8 = 128). Das ist ein neuer, voller Negativbefund für (15,4), unabhängig von den Symmetrie-Zählungen.
- Bei den 48ern ist viel Luft (Schranke 10, bekannt 24). Der Spann ist ein endlicher, kleiner Suchraum (2048 Paare statt
  2^19) mit eingebauter Positivkontrolle (der 48er selbst). 46 oder 44 Wörter wären ein Tabellenergebnis, 40 ein Rekord.
- Ob ein kleinerer Code im Spann liegt, ist offen. Es gibt keinen Grund, das zu erwarten, außer dass beide bekannten
  guten Codes kleinen Rang haben.

## Vorschlag für einen Auftrag S („Suche im Spann“), noch nicht freigegeben

1. **Gegenrechnung** dieses Vortests mit eigenem Programm: Ränge, μ-Verteilungen, Zählschranke, die drei Sorten bei Rang 9.
2. **(15,4), 30 Wörter:** exakt (SAT oder ILP mit Symmetriebrechung) in den drei Sorten von Rang-9-Erweiterungen, für
   `ctrl_16` und die zweite Klasse der 112 Lösungen. Kontrolle: 16 Paare lösbar, Rang 8 mit 15 Paaren unlösbar.
   Danach, falls billig, Rang 10.
3. **(20,6), unter 48:** im Spann beider 48er (und der übrigen bekannten Klassen, falls ihr Spann ein anderer ist).
   Kontrolle: 24 Paare aus Zufallsstart. Dann Leiter 23, 22, 21, 20 Paare; exakt, wo es geht, sonst lokale Suche.
4. **Abbruch:** Kontrolle nicht bestanden, oder exakte Fälle nach je 30 Minuten unentschieden und lokale Suche bei
   23 Paaren in 16 Läufen ohne Treffer. **Obergrenze:** 4 Stunden Rechenzeit.
5. Jeden Treffer auf Gleichwertigkeit zu bekannten Codes prüfen (bliss) und durch `verifiers/pruefe_matrix.py` schicken.

## Nachtrag 25.11., ca. 12:30 Uhr (ebenfalls Rechnung des Orchestrators, nicht gegengerechnet)

- **Zweite Klasse der 112 Lösungen bei (15,4)** (`runs/gegenrechnung15/loesung111_16x15.txt`): Rang 8, μ-Werte
  12 (14 Nebenklassen), 15 (64), 16 (49), 29 (1). Zählschranke 128/12 = 10,67. Anders als bei `ctrl_16` sind hier
  **15 Paare im eigenen Spann nicht ausgeschlossen**. CP-SAT, 2 Kerne: 16 Paare in 1,4 s, 15 Paare nach 150 s unentschieden.
  `loesung0_16x15.txt` hat dieselben μ-Werte wie `ctrl_16`.
- **Neunter 48er** (`runs/bahnen3/teil4/kontrolle_T24_seed4_matrix.txt`): Rang 12, dieselben μ-Werte wie 1+3 und 2+2.
  Ob die drei Spanne gleich oder gleichwertig sind, ist nicht geprüft.
- **Kandidat 2 der Mittags-Übergabe** (Code = wenige Nebenklassen eines Kerns ⟨1,a,b⟩ mit 8 Wörtern, (20,6), 5 Nebenklassen):
  Die Zählschranke über B(D) = Zahl der Nebenklassen mit Minimalgewicht ≤ 6 schließt keinen der 97 Typen aus
  (Schranke 2,46 bis 4,20 gegen 5). Eine vollständige Zählung braucht also echte Aufzählung über 2^17 Syndrome und ist
  entgegen der Übergabe nicht in Minuten zu haben (Schätzung, unbelegt). Zurückgestellt; kein bekannter guter Code hat
  einen Kern über 4.
- Aufträge: `notes/spann_2026-11-25_auftrag.md` (S) und `notes/gruppen_2026-11-25_auftrag.md` (G).
