#!/usr/bin/env bash
# Kontrollen fuer den Bewerter (Auftrag E, Schritt 1). Prueft nur Soll-Zahlen, aendert nichts.
# Aufruf aus dem Repo-Wurzelordner:  bash src/evo_bewertung_kontrolle.sh
set -u
B=build/evo_bewertung
T=$(mktemp -d)
fehler=0
pruef() {  # pruef BESCHREIBUNG SOLL m R DATEI
    local ist
    ist=$($B "$3" "$4" "$5" 2>/dev/null | sed -n 's/^offen=\([0-9]*\) .*/\1/p')
    if [ "$ist" = "$2" ]; then echo "OK    $1: offen=$ist"; else echo "FEHLER $1: offen=$ist, Soll $2"; fehler=1; fi
}
ohne() {  # ohne DATEI ZEILEN... -> schreibt Datei ohne die genannten Zeilen (1-basiert) nach $T/ohne.txt
    local d=$1; shift
    local ausdruck=""
    for z in "$@"; do ausdruck="$ausdruck${z}d;"; done
    sed "$ausdruck" "$d" > "$T/ohne.txt"
}

echo "== Hamming: ctrl_8, erste 7 Spalten, R = 1"
pruef "ctrl_8 voll" 0 7 1 controls/ctrl_8.txt
for z in $(seq 1 8); do ohne controls/ctrl_8.txt $z; pruef "ctrl_8 ohne Zeile $z" 8 7 1 "$T/ohne.txt"; done

echo "== ctrl_16, erste 15 Spalten, R = 4"
pruef "ctrl_16 voll" 0 15 4 controls/ctrl_16.txt
for z in $(seq 1 16); do ohne controls/ctrl_16.txt $z; pruef "ctrl_16 ohne Zeile $z" 233 15 4 "$T/ohne.txt"; done

echo "== had24_1p3_paley12, m = 20, R = 6"
pruef "48er voll" 0 20 6 controls/had24_1p3_paley12.txt
for i in $(seq 1 12); do ohne controls/had24_1p3_paley12.txt $i $((i+12)); pruef "48er ohne Nebenklasse (Zeilen $i, $((i+12)))" 3054 20 6 "$T/ohne.txt"; done
for z in $(seq 1 24); do ohne controls/had24_1p3_paley12.txt $z; pruef "48er ohne Zeile $z" 1297 20 6 "$T/ohne.txt"; done

echo "== Eingabefehler muessen abgelehnt werden"
printf ' 1 -1  1\n 1  0  1\n' > "$T/null.txt"
if $B 3 1 "$T/null.txt" >/dev/null 2>&1; then echo "FEHLER Eintrag 0 nicht abgelehnt"; fehler=1; else echo "OK    Eintrag 0 abgelehnt"; fi
printf ' 1 -1\n' > "$T/kurz.txt"
if $B 3 1 "$T/kurz.txt" >/dev/null 2>&1; then echo "FEHLER kurze Zeile nicht abgelehnt"; fehler=1; else echo "OK    kurze Zeile abgelehnt"; fi
printf ' 1 x 1\n' > "$T/text.txt"
if $B 3 1 "$T/text.txt" >/dev/null 2>&1; then echo "FEHLER Text nicht abgelehnt"; fehler=1; else echo "OK    Text abgelehnt"; fi

rm -rf "$T"
if [ $fehler = 0 ]; then echo "ALLE KONTROLLEN BESTANDEN"; else echo "KONTROLLE NICHT BESTANDEN"; exit 1; fi
