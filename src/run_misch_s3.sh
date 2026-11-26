#!/bin/bash
# Auftrag M, Schritt 3: gemeinsame Suche (Programm PROG, Standard misch_alt) bei (18,5), d = 8. 4 Laeufe gleichzeitig.
# Aufruf: bash src/run_misch_s3.sh NAME K f m1 SEK SEED1 SEED2 ... [-- weitere Optionen]
# Dateien runs/mischform/s3/NAME_k<K>_f<f>_t<m1>-<m2>_seed<s>.{log.txt,paar.txt,frei.txt,matrix.txt}.
# Nach jedem Lauf: eigener Pruefcode (offene Punkte ueber alle 2^18 Punkte); bei 0 offenen Punkten zusaetzlich pruefe_matrix.py --erwartet 8.
NAME=$1; K=$2; F=$3; M1=$4; SEK=$5; shift 5
SEEDS=(); while [ $# -gt 0 ] && [ "$1" != "--" ]; do SEEDS+=($1); shift; done; [ "$1" = "--" ] && shift
M2=$((18 - M1)); O=runs/mischform/s3; mkdir -p $O
B=${NAME}_k${K}_f${F}_t${M1}-${M2}
printf '%s\n' "${SEEDS[@]}" | xargs -P 4 -I{} sh -c "./build/${PROG:-misch_alt} 18 8 $K $M1 $F {} $SEK $O/${B}_seed{} $* > $O/${B}_seed{}.log.txt 2>&1"
for s in "${SEEDS[@]}"; do
  P=$O/${B}_seed$s
  echo "$B seed $s: $(grep -E '^(LOESUNG|ZEITGRENZE)' $P.log.txt)"
  python3 verifiers/mischform/pruef_misch.py offen $P.matrix.txt 8 | sed 's/^/    /'
  if python3 verifiers/mischform/pruef_misch.py offen $P.matrix.txt 8 | grep -q 'offene Punkte (von 2^18 = 262144): 0$'; then
    python3 verifiers/pruefe_matrix.py $P.matrix.txt --erwartet 8 > $P.pruefe_matrix.log.txt 2>&1; echo "    pruefe_matrix.py Exit $?: $(tail -1 $P.pruefe_matrix.log.txt)"
  fi
done
