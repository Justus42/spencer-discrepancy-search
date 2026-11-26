#!/bin/bash
# Auftrag N: fasst die Stufenlogs (runs/leiter_n/<PRAEFIX>_k<K>_t<TYP>.log.txt, ohne _seed) zusammen: gelöst, größte Lösungszeit, beste Restzahl.
# Aufruf: bash src/leiter_zusammen.sh PRAEFIX
for f in $(ls runs/leiter_n/$1_k*_t*.log.txt | grep -v _seed | sort -t k -k2 -n -r); do
  n=$(basename $f .log.txt)
  l=$(grep -c "LOESUNG" $f); z=$(grep -c "seed" $f | head -1)
  s=$(grep -E "seed [0-9]+:" $f | wc -l)
  b=$(grep -oE "beste Restzahl [0-9]+" $f | awk '{print $3}' | sort -n | head -1)
  t=$(grep -oE "LOESUNG nach [0-9.]+" $f | awk '{print $3}' | sort -n | tail -1)
  echo "$n: gelöst $l/$s, längste Lösungszeit ${t:--} s, beste Restzahl ${b:--}"
done
