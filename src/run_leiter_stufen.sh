#!/bin/bash
# Auftrag N, Schritt 2: Leiter in einem Typ. Beginnt bei K = KSTART, 4 Startwerte parallel je SEK s; ist mindestens einer gelöst,
# wird K um 1 gesenkt, sonst endet die Leiter. Nach jeder Stufe Commit und Push. Ausgabe: je Stufe die Ergebniszeilen.
# Aufruf: [SEEDS="1 2 3 4"] bash src/run_leiter_stufen.sh PRAEFIX d SEK KSTART TYP [KMIN]   (TYP = m1-m2)
PRAEFIX=$1; D=$2; SEK=$3; K=$4; TYP=$5; KMIN=${6:-1}
SEEDS=${SEEDS:-"1 2 3 4"}
O=runs/leiter_n
M1=${TYP%-*}; M2=${TYP#*-}
while [ $K -ge $KMIN ]; do
  NAME=${PRAEFIX}_k${K}_t${TYP}
  echo "== $(date -u +%T) Start $NAME (d=$D, ${SEK}s, Startwerte $SEEDS)"
  bash src/run_leiter_n.sh $NAME $K $M1 $M2 $D $SEK $SEEDS -- --neustart 20 > $O/${NAME}.log.txt 2>&1
  cat $O/${NAME}.log.txt
  echo "== $(date -u +%T) Ende $NAME"
  git add $O/${NAME}* >/dev/null 2>&1
  git commit -q -m "Auftrag N: Stufe $NAME

Co-Authored-By: Claude Sonnet 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_0119JzQb9Fqy8mv2wbs75dbR" >/dev/null 2>&1; git push -q origin HEAD >/dev/null 2>&1
  if ! grep -q "offene Klassen 0 " $O/${NAME}.log.txt; then echo "LEITER ENDET bei K=$K ($TYP)"; break; fi
  K=$((K-1))
done
