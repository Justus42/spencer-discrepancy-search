#!/bin/bash
# Auftrag D: je Typ ein Block (Startwerte parallel), danach Commit und Push; Treffer werden gemeldet.
# Aufruf: [SEEDS="1 2 3 4"] [SEKX=...] bash src/run_leiter_d10_typen.sh PRAEFIX K d SEK TYP1 [TYP2 ...]   (TYP = m1-m2)
PRAEFIX=$1; K=$2; D=$3; SEK=$4; shift 4
SEEDS=${SEEDS:-"1 2 3 4"}
O=runs/leiter_d10
for TYP in "$@"; do
  M1=${TYP%-*}; M2=${TYP#*-}; NAME=${PRAEFIX}_k${K}_t${TYP}
  echo "== $(date -u +%T) Start Block $NAME"
  bash src/run_leiter_d10.sh $NAME $K $M1 $M2 $D $SEK $SEEDS -- --neustart 20 | tee $O/${NAME}.log.txt
  echo "== $(date -u +%T) Ende Block $NAME"
  git add $O/${NAME}* >/dev/null 2>&1
  git commit -q -m "Auftrag D: Block $NAME

Co-Authored-By: Claude Sonnet 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_0119JzQZ5Ynx3h6z915jAnbF" >/dev/null 2>&1; git push -q origin HEAD >/dev/null 2>&1
  if grep -q "offene Klassen 0 " $O/${NAME}.log.txt; then echo "TREFFER in $NAME"; fi
done
