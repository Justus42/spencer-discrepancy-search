#!/usr/bin/env bash
# Auftrag V (Verdopplungszellen): Treiber fuer den vorhandenen Sucher src/evo/p018.c. Kein neuer Suchcode.
# Aufruf aus dem Repo-Wurzelordner:
#   bash src/run_verdopplung.sh BLOCK m R PAARE SEKUNDEN "KOMMENTAR" [STARTWERTE...]
# Standard-Startwerte 1 2 3 4, vier Laeufe parallel (4 Kerne). Nur die Zahl des Bewerters zaehlt, nie die Selbstauskunft.
# Archiv: runs/verdopplung/archiv.tsv  (Block, Zeit UTC, m, R, Paare, Startwert, Sekunden, offen, Anteil, Schritte/s, Speicher MB, Kommentar)
# Ausgaben: runs/verdopplung/out/, Logs: runs/verdopplung/logs/ (*.log.txt)
set -u
BLOCK=$1; M=$2; R=$3; K=$4; SEK=$5; KOMM=$6
shift 6
SEEDS=${*:-1 2 3 4}
mkdir -p build/evo runs/verdopplung/out runs/verdopplung/logs

sha256sum -c runs/evo/BEWERTER.sha256 >/dev/null || { echo "BEWERTER GEAENDERT - Abbruch"; exit 1; }
gcc -O3 -march=native -fopenmp -o build/evo_bewertung src/evo_bewertung.c || exit 1
gcc -O3 -march=native -fopenmp -o build/evo/p018 src/evo/p018.c -lm || exit 1

echo "$(date -u '+%F %T') UTC  Block $BLOCK  m=$M R=$R Paare=$K  Startwerte: $SEEDS  je $SEK s  ($KOMM)"
TAG="${BLOCK}_m${M}R${R}k${K}"
for s in $SEEDS; do
    OUT="runs/verdopplung/out/${TAG}_s${s}.txt"
    LOG="runs/verdopplung/logs/${TAG}_s${s}.log.txt"
    rm -f "$OUT"
    ( start=$(date +%s.%N)
      timeout $((SEK + 60)) build/evo/p018 "$M" "$R" "$K" "$s" "$SEK" "$OUT" > "$LOG" 2>&1 &
      pid=$!
      hwm=0
      while kill -0 $pid 2>/dev/null; do
          for q in $pid $(pgrep -P $pid); do
              v=$(awk '/VmHWM/{print $2}' /proc/$q/status 2>/dev/null); [ -n "${v:-}" ] && [ "$v" -gt "$hwm" ] && hwm=$v
          done
          sleep 0.5
      done
      wait $pid 2>/dev/null
      ende=$(date +%s.%N)
      echo "$ende - $start" | bc > "runs/verdopplung/logs/${TAG}_s${s}.sek"
      echo "$hwm" > "runs/verdopplung/logs/${TAG}_s${s}.hwm" ) &
done
wait

for s in $SEEDS; do
    OUT="runs/verdopplung/out/${TAG}_s${s}.txt"
    LOG="runs/verdopplung/logs/${TAG}_s${s}.log.txt"
    SEKIST=$(printf '%.0f' "$(cat "runs/verdopplung/logs/${TAG}_s${s}.sek")")
    MB=$(( $(cat "runs/verdopplung/logs/${TAG}_s${s}.hwm") / 1024 ))
    ENDE=$(grep -a 'ENDE' "$LOG" | tail -1)
    IT=$(echo "$ENDE" | sed -n 's/.* it=\([0-9]*\) .*/\1/p'); TT=$(echo "$ENDE" | sed -n 's/.* t=\([0-9.]*\) .*/\1/p')
    RATE=$(awk -v i="${IT:-0}" -v t="${TT:-1}" 'BEGIN{printf "%.0f", (t>0)? i/t : 0}')
    ZUSATZ=""
    if [ -s "$OUT" ]; then
        ERG=$(build/evo_bewertung "$M" "$R" "$OUT" 2>&1)
        OFFEN=$(echo "$ERG" | sed -n 's/^offen=\([0-9]*\) .*/\1/p')
        KIST=$(echo "$ERG" | sed -n 's/.* k=\([0-9]*\) .*/\1/p')
        [ -z "$OFFEN" ] && OFFEN="FEHLER"
        [ "$KIST" != "$K" ] && { ZUSATZ=" [FEHLER: $KIST Zeilen statt $K]"; OFFEN="FEHLER"; }
    else
        OFFEN="FEHLER"; ZUSATZ=" [keine Ausgabe]"
    fi
    ANTEIL=$(awk -v o="$OFFEN" -v m="$M" 'BEGIN{ if (o ~ /^[0-9]+$/) printf "%.5f", o / (2^(m-1)); else print "-" }')
    printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$BLOCK" "$(date -u '+%H:%M:%S')" "$M" "$R" "$K" "$s" "$SEKIST" "$OFFEN" "$ANTEIL" "$RATE" "$MB" "$KOMM$ZUSATZ" >> runs/verdopplung/archiv.tsv
    echo "  Startwert $s: offen=$OFFEN  Anteil=$ANTEIL  ($SEKIST s, ${RATE} Schritte/s, ${MB} MB)$ZUSATZ"
done
echo "$(date -u '+%F %T') UTC  Block $BLOCK fertig"
