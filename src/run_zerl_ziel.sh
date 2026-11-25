#!/bin/bash
# Schritt 4 (Auftrag H): je Typ ein Block (4 Startwerte parallel, je SEK s), danach Commit und Push; bei einem Treffer sofort Stopp.
# Aufruf: bash src/run_zerl_ziel.sh PRAEFIX K d SEK TYP1 [TYP2 ...]   (TYP = m1-m2, z. B. 11-9)
PRAEFIX=$1; K=$2; D=$3; SEK=$4; shift 4
O=runs/zerlegung
for TYP in "$@"; do
  M1=${TYP%-*}; M2=${TYP#*-}; NAME=${PRAEFIX}_k${K}_t${TYP}
  echo "== $(date -u +%T) Block $NAME"
  bash src/run_zerl_alt.sh $NAME $K $M1 $M2 $D $SEK 1 2 3 4 -- --neustart 20 | tee $O/${NAME}.log.txt
  git add $O/${NAME}* >/dev/null 2>&1
  git commit -q -m "Auftrag H, Schritt 4: Block $NAME

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_0119JzQaCqNeHyv6Z7cFjeug" >/dev/null 2>&1; git push -q origin HEAD >/dev/null 2>&1
  if grep -q "offene Klassen 0 " $O/${NAME}.log.txt; then echo "TREFFER in $NAME: Stopp"; exit 5; fi
done
