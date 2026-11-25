#!/bin/bash
# Auftrag A, Teil 3: Treiber fuer einen Block (hoechstens BLOCK Sekunden) der vollstaendigen Suche n=11, d=6, K=11.
# Aufruf: bash src/run_einzelnull.sh TRAEGER BLOCK_SEKUNDEN [Q S]  (TRAEGER z. B. "10" oder "10,8,6"; Q, S: Profil
#         genau Q Zeilen mit Traeger 8 und S mit Traeger 6, minimale Ueberdeckungen)
# Typen 0..9 der Reihe nach; erledigte Aufgaben je Typ in runs/einzelnull/erl_<tag>_T<t>.txt (Fortsetzung),
# fertige Typen als Zeile "fertig T" in runs/einzelnull/fertig_<tag>.txt. Log: runs/einzelnull/block_<tag>.log.txt
set -u
cd "$(dirname "$0")/.."
SUP="$1"; BLOCK="$2"; Q="${3:-}"; S="${4:-}"
TAG=$(echo "$SUP" | tr ',' '_')
EXTRA=""
if [ -n "$Q" ]; then TAG="${TAG}_q${Q}s${S}"; EXTRA="-q $Q -S $S"; fi
D=runs/einzelnull
LOG=$D/block_$TAG.log.txt
FERTIG=$D/fertig_$TAG.txt
touch "$FERTIG"
START=$(date +%s)
echo "### Block Start $(date -u '+%F %T') UTC, Traeger $SUP, Grenze $BLOCK s" >> "$LOG"
for T in 0 1 2 3 4 5 6 7 8 9; do
  if grep -qx "fertig $T" "$FERTIG"; then continue; fi
  REST=$(( BLOCK - ( $(date +%s) - START ) ))
  if [ "$REST" -lt 30 ]; then echo "### Blockzeit um vor Typ $T" >> "$LOG"; break; fi
  OUT=$D/lauf_${TAG}_T$T.out
  ./build/einzelnull_such -n 11 -d 6 -K 11 -s "$SUP" -y -3 -T "$T" -t 5 -b 4 -z "$REST" $EXTRA \
     -r "$D/erl_${TAG}_T$T.txt" -o "$D/loesungen_$TAG.txt" > "$OUT" 2>&1
  grep -vE "^  [0-9]:|Bahn [0-9]+/" "$OUT" >> "$LOG"
  if grep -q "Typ $T: fertig" "$OUT"; then echo "fertig $T" >> "$FERTIG"; fi
  if grep -q "Loesung gefunden" "$OUT"; then echo "### LOESUNG gefunden, Typ $T" >> "$LOG"; break; fi
  if grep -q "ABBRUCH" "$OUT"; then echo "### Blockzeit um in Typ $T" >> "$LOG"; break; fi
done
echo "### Block Ende $(date -u '+%F %T') UTC, fertig: $(tr '\n' ' ' < "$FERTIG")" >> "$LOG"
tail -3 "$LOG"
