#!/bin/sh
# Erzeugt alle Lean-Datendateien aus controls/. Aufruf aus dem Repo-Wurzelordner: sh lean/erzeuge_alle.sh
set -e
E="python3 -I lean/erzeuge.py"
# a) Hamming [7,4]: ctrl_8 (8 x 8, letzte Spalte Null) ohne Nullspalte, 8 Zeilen + Komplemente = 16 Wörter
$E controls/ctrl_8.txt  --sha256 c1df5cefdf934c07af5a8ee601bc047419e58db70dbcc9450f85f45593a0a396 --zeilen 8  --laenge 7  --modul Hamming7 --ohne-nullspalten
# b) ctrl_16 ohne Nullspalte: 16 Zeilen + Komplemente = 32 Wörter, Länge 15
$E controls/ctrl_16.txt --sha256 79a659ebf2874d69c0829031da3216275634c55723f5b1e89011ea02a0888d52 --zeilen 16 --laenge 15 --modul Ctrl16 --ohne-nullspalten
# c) Negativkontrolle: had24_1p3_paley12 ohne Zeile 0 und ihr Komplement = 46 Wörter
$E controls/had24_1p3_paley12.txt --sha256 44fe5d63e95ebafcf5f7b76c415960c6dfa02d18f22061ef963031692eb987bf --zeilen 24 --laenge 20 --modul Had24_1p3_ohne0 --ohne-zeile 0
# Ziel: je 24 Zeilen + Komplemente = 48 Wörter, Länge 20
$E controls/had24_1p3_paley12.txt --sha256 44fe5d63e95ebafcf5f7b76c415960c6dfa02d18f22061ef963031692eb987bf --zeilen 24 --laenge 20 --modul Had24_1p3
$E controls/had24_2p2_paley12.txt --sha256 2529a95ce7b6ac27c8899cbc148989cd83564603526a5a13f28af9b4c5334b7f --zeilen 24 --laenge 20 --modul Had24_2p2
# Auftrag LZ (26.11.2026): K(18,5) <= 50 und <= 52, Länge 18
# 50 Wörter (25 Zeilen + Komplemente); Zeile 24 ist die freie Zeile
$E controls/c18_5_50w.txt --sha256 602863c85dfbff1b5e9f405021c6121ebb553223780fef3e6046f58fe8598d2d --zeilen 25 --laenge 18 --modul C18_5_50w
# Negativkontrolle: ohne Zeile 24 und ihr Komplement = 48 Wörter
$E controls/c18_5_50w.txt --sha256 602863c85dfbff1b5e9f405021c6121ebb553223780fef3e6046f58fe8598d2d --zeilen 25 --laenge 18 --modul C18_5_50w_ohne24 --ohne-zeile 24
# 52 Wörter (26 Zeilen + Komplemente)
$E controls/c18_5_52w.txt --sha256 5c816e6954c099012f193e722db2385b26b5d4cc29b26a0e7ebf20856ecbb782 --zeilen 26 --laenge 18 --modul C18_5_52w
