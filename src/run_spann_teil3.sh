#!/bin/bash
# Auftrag S, Teil 3: lokale Suche im Spann eines 48ers, 4 Laeufe parallel.
# Aufruf: bash src/run_spann_teil3.sh PROBLEMNAME K SEK SEED... [-- STARTDATEIEN...]
# Logs runs/spann/teil3_<problem>_k<K>_s<seed>.log.txt bzw. _w<startname>.log.txt
N=$1; K=$2; SEK=$3; shift 3
P=runs/spann/probleme
jobs=()
while [ $# -gt 0 ] && [ "$1" != "--" ]; do jobs+=("s:$1"); shift; done
[ "$1" == "--" ] && shift
for f in "$@"; do jobs+=("w:$f"); done
lauf() {
  local j=$1
  if [ "${j:0:1}" == "s" ]; then
    s=${j:2}; LOG=runs/spann/teil3_${N}_k${K}_s${s}.log.txt
    build/spann_ls $P/$N.txt $K $s $SEK > $LOG 2>&1
  else
    f=${j:2}; b=$(basename $f .txt); LOG=runs/spann/teil3_${N}_k${K}_w${b}.log.txt
    build/spann_ls $P/$N.txt $K 1 $SEK $f > $LOG 2>&1
  fi
  echo "$(basename $LOG): $(grep -E 'ENDE' $LOG)"
}
export -f lauf; export N K SEK P
printf "%s\n" "${jobs[@]}" | xargs -P 4 -I{} bash -c 'lauf {}'
