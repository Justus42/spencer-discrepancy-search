#!/usr/bin/env bash
# Auftrag F: Startwerte eines Suchprogramms parallel, danach Zaehlung nur mit dem Bewerter.
# Aufruf aus dem Repo-Wurzelordner:
#   bash src/frei_d9/lauf.sh TAG PROGRAMM m R PAARE SEKUNDEN "STARTWERTE" [ZUSATZARGUMENTE...]
# Ausgaben runs/frei_d9/TAG_sS.txt, Logs runs/frei_d9/TAG_sS.log.txt, Zusammenfassung runs/frei_d9/TAG.log.txt
set -u
TAG=$1; PROG=$2; M=$3; R=$4; K=$5; SEK=$6; SEEDS=$7; shift 7
D=runs/frei_d9; SUM=$D/$TAG.log.txt
sha256sum -c runs/evo/BEWERTER.sha256 >/dev/null || { echo "BEWERTER GEAENDERT - Abbruch" >> "$SUM"; exit 1; }
echo "$(date -u '+%F %T') UTC  Start $TAG: $PROG m=$M R=$R Paare=$K je $SEK s, Startwerte $SEEDS, Zusatz: $*" >> "$SUM"
for s in $SEEDS; do
  ( st=$(date +%s.%N)
    timeout $((SEK + 120)) "build/evo/$PROG" "$M" "$R" "$K" "$s" "$SEK" "$D/${TAG}_s$s.txt" "$@" > "$D/${TAG}_s$s.log.txt" 2>&1 &
    pid=$!; hwm=""
    while kill -0 $pid 2>/dev/null; do
      c=$(pgrep -P $pid | head -1); [ -n "$c" ] && h=$(sed -n 's/^VmHWM:\s*//p' /proc/$c/status 2>/dev/null) && [ -n "$h" ] && hwm=$h
      sleep 2; done
    wait $pid; echo "$hwm" > "$D/${TAG}_s$s.rss"
    en=$(date +%s.%N); echo "$en - $st" | bc > "$D/${TAG}_s$s.sek" ) &
done
wait
for s in $SEEDS; do
  OUT=$D/${TAG}_s$s.txt
  ERG=$(build/evo_bewertung "$M" "$R" "$OUT" 2>&1)
  RSS=$(cat "$D/${TAG}_s$s.rss"); rm -f "$D/${TAG}_s$s.rss"
  echo "  Startwert $s: Bewerter: $ERG | $(printf '%.0f' "$(cat "$D/${TAG}_s$s.sek")") s | Speicher (VmHWM) ${RSS} | $(grep -a 'ENDE' "$D/${TAG}_s$s.log.txt" | tail -1)" >> "$SUM"
  rm -f "$D/${TAG}_s$s.sek"
done
echo "$(date -u '+%F %T') UTC  Ende $TAG" >> "$SUM"
