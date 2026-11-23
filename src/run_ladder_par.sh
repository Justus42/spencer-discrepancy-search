#!/bin/bash
# usage: run_ladder_par.sh n R d K secs logfile  -> alle Typen, 4 parallel
n=$1; R=$2; d=$3; K=$4; secs=$5; log=$6
python3 -I -c "
import sys; sys.path.insert(0,'src')
from coset_ladder import types_for
for t in types_for($n,$d): print(','.join(map(str,t)))" | xargs -P4 -I{} python3 src/coset_ladder.py $n $R $d $K $secs 1 {} >> $log
