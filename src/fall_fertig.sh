#!/bin/bash
# Auftrag R9: prueft einen Fall und traegt ihn ins Protokoll ein. Aufruf: bash src/fall_fertig.sh NAME
N=$1; R=$(python3 src/pruefe_rang9.py runs/spann_rang9/$N.fortschritt | tee -a runs/spann_rang9/pruefung.log.txt); echo "$R"
echo "$R" | tail -1 | grep -q "^ENTSCHIEDEN" || { echo "NICHT entschieden"; exit 1; }
L=$(echo "$R" | head -1)
f() { echo "$L" | sed "s/.*$1 \([0-9]*\),.*/\1/"; }
echo "| $N | $(echo "$L" | sed 's/.*Aufgaben \([0-9]*\),.*/\1/') | $(f Bereiche) | $(f Knoten) | $(f Sekunden) | ENTSCHIEDEN: keine Lösung (lückenlos, geprüft) |" >> notes/laeufe/2026-11-25_rang9.md
git add -A && git commit -q -m "R9: $N entschieden (keine Loesung, $(f Knoten) Knoten)" -m "Co-Authored-By: Claude Sonnet 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_0119JzQa8FJWaFLQSobDYPtA" && git push -q origin claude/auftrag-r9-lsrlrp 2>&1 | tail -1
