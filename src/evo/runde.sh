#!/usr/bin/env bash
# Eine Runde der Evolutionsschleife (Auftrag E): Kind bauen, Startwerte parallel, Bewerter, Archiv.
# Aufruf aus dem Repo-Wurzelordner:
#   bash src/evo/runde.sh RUNDE PROGRAMM ELTER INSEL m R PAARE SEKUNDEN "KOMMENTAR" [STARTWERTE...]
# Standard-Startwerte: 1 2 3 4. Nur die Zahl des Bewerters zaehlt, nie die Selbstauskunft des Programms.
set -u
RUNDE=$1; PROG=$2; ELTER=$3; INSEL=$4; M=$5; R=$6; K=$7; SEK=$8; KOMM=$9
shift 9
SEEDS=${*:-1 2 3 4}
RN=$(printf 'r%03d' "$RUNDE")
mkdir -p build/evo runs/evo/out runs/evo/logs

# Bewerter: Quelltext gegen die eingefrorene Pruefsumme pruefen, dann frisch bauen.
sha256sum -c runs/evo/BEWERTER.sha256 >/dev/null || { echo "BEWERTER GEAENDERT - Abbruch"; exit 1; }
gcc -O3 -march=native -fopenmp -o build/evo_bewertung src/evo_bewertung.c || exit 1

# Kind bauen
gcc -O3 -march=native -fopenmp -o "build/evo/$PROG" "src/evo/$PROG.c" -lm 2> "runs/evo/logs/${RN}_${PROG}_gcc.txt" || { cat "runs/evo/logs/${RN}_${PROG}_gcc.txt"; exit 1; }

echo "$(date -u '+%F %T') UTC  Runde $RUNDE  $PROG (Elter $ELTER, Insel $INSEL)  m=$M R=$R Paare=$K  Startwerte: $SEEDS  je $SEK s"
declare -A T0
for s in $SEEDS; do
    OUT="runs/evo/out/${RN}_${PROG}_m${M}R${R}k${K}_s${s}.txt"
    rm -f "$OUT"
    ( start=$(date +%s.%N)
      timeout $((SEK + 60)) "build/evo/$PROG" "$M" "$R" "$K" "$s" "$SEK" "$OUT" > "runs/evo/logs/${RN}_${PROG}_s${s}.log.txt" 2>&1
      ende=$(date +%s.%N)
      echo "$ende - $start" | bc > "runs/evo/logs/${RN}_${PROG}_s${s}.sek" ) &
done
wait

for s in $SEEDS; do
    OUT="runs/evo/out/${RN}_${PROG}_m${M}R${R}k${K}_s${s}.txt"
    SEKIST=$(printf '%.0f' "$(cat "runs/evo/logs/${RN}_${PROG}_s${s}.sek")")
    if [ -s "$OUT" ]; then
        ERG=$(build/evo_bewertung "$M" "$R" "$OUT" 2>&1)
        OFFEN=$(echo "$ERG" | sed -n 's/^offen=\([0-9]*\) .*/\1/p')
        KIST=$(echo "$ERG" | sed -n 's/.* k=\([0-9]*\) .*/\1/p')
        [ -z "$OFFEN" ] && OFFEN="FEHLER"
        ZUSATZ=""
        [ "$KIST" != "$K" ] && { ZUSATZ=" [FEHLER: $KIST Zeilen statt $K]"; OFFEN="FEHLER"; }
    else
        OFFEN="FEHLER"; ZUSATZ=" [keine Ausgabe]"
    fi
    printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$RUNDE" "$PROG" "$ELTER" "$INSEL" "$M" "$R" "$K" "$s" "$SEKIST" "$OFFEN" "$KOMM$ZUSATZ" >> runs/evo/archiv.tsv
    echo "  Startwert $s: offen=$OFFEN  ($SEKIST s)$ZUSATZ   [Selbstauskunft: $(grep -a 'ENDE' "runs/evo/logs/${RN}_${PROG}_s${s}.log.txt" | tail -1)]"
done
echo "$(date -u '+%F %T') UTC  Runde $RUNDE fertig"
