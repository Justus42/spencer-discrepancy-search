#!/bin/bash
# Auftrag R9: einen Rang-9-Fall bei (15,4) in Aufgabenbereichen abarbeiten, setzt hinter dem letzten
# vollstaendigen Bereich fort.
# Aufruf: bash src/run_spann_rang9.sh NAME [BLOCKSEK=1500] [START_GROESSE=50]
#   NAME      z. B. l111_r9_y384 (Problem runs/spann/probleme/NAME.txt/.gen)
#   BLOCKSEK  ein neuer Bereich wird nur begonnen, wenn die geschaetzte Dauer noch in den Block passt
#   Zeitlimit je Bereich: 1500 s; ein am Limit abgebrochener Bereich gilt als nicht gerechnet, wird mit halber Groesse wiederholt
# Fortschrittsdatei runs/spann_rang9/NAME.fortschritt: Zeile "# AUFGABEN n", dann je Bereich "VON BIS KNOTEN SEKUNDEN ERGEBNIS"
#   ERGEBNIS: OHNE_LOESUNG | ZEITLIMIT | LOESUNG
# Beenden: 0 Block fertig (oder Fall fertig), 3 Loesung gefunden (alles anhalten), 2 Fehler
NAME=$1; BLOCK=${2:-1500}; GR=${3:-50}
[ -z "$NAME" ] && { echo "Aufruf: $0 NAME [BLOCKSEK] [START_GROESSE]"; exit 2; }
P=runs/spann/probleme; O=runs/spann_rang9
F=$O/$NAME.fortschritt; LOG=$O/$NAME.log.txt
BIN=build/spann_dfs; K=15; LIMIT=1500
mkdir -p $O
[ -x $BIN ] || gcc -O3 -march=native -fopenmp -o $BIN src/spann_dfs.c || exit 2
touch $F
if ! grep -q '^# AUFGABEN' $F; then
  n=$($BIN $P/$NAME.txt $K 600 3 0 $P/$NAME.gen 0 0 | tee -a $LOG | grep '^Aufgaben:' | awk '{print $2}')
  [ -z "$n" ] && { echo "Aufgabenzahl nicht ermittelt"; exit 2; }
  echo "# AUFGABEN $n" >> $F
fi
N=$(grep '^# AUFGABEN' $F | awk '{print $3}')
# Fortsetzungspunkt: lueckenlos von 0 an vollstaendige Bereiche
VON=$(grep OHNE_LOESUNG $F | sort -n -k1,1 | awk 'BEGIN{e=0} $1==e{e=$2} END{print e}')
LETZT=$(grep -v '^#' $F | tail -n 1)
if [ -n "$LETZT" ]; then
  set -- $LETZT; ER=$5; G0=$(( $2 - $1 )); DUR=$4
  if [ "$ER" = ZEITLIMIT ]; then GR=$(( G0 / 2 )); [ $GR -lt 1 ] && GR=1
  elif [ "$ER" = OHNE_LOESUNG ]; then
    GR=$(awk -v g=$G0 -v d=$DUR 'BEGIN{f=600/(d>1?d:1); if(f>4)f=4; if(f<0.5)f=0.5; x=int(g*f+0.5); if(x<10)x=10; print x}')
  fi
fi
T0=$(date +%s); EST=0
while [ $VON -lt $N ]; do
  EL=$(( $(date +%s) - T0 ))
  [ $EL -gt 0 ] && [ $(( EL + EST )) -gt $BLOCK ] && { echo "Blockende (verstrichen ${EL}s)"; break; }
  BIS=$(( VON + GR )); [ $BIS -gt $N ] && BIS=$N
  S0=$(date +%s.%N)
  OUT=$($BIN $P/$NAME.txt $K $LIMIT 3 0 $P/$NAME.gen $VON $BIS | grep -v Fortschritt)
  echo "$OUT" >> $LOG
  S1=$(date +%s.%N); SEK=$(awk -v a=$S0 -v b=$S1 'BEGIN{printf "%.0f", b-a}')
  Z=$(echo "$OUT" | grep '^BEREICH')
  if [ -z "$Z" ]; then echo "$NAME $VON $BIS: kein BEREICH-Ergebnis (Fehler)"; exit 2; fi
  KN=$(echo "$Z" | sed 's/.*Knoten \([0-9]*\),.*/\1/'); ER=$(echo "$Z" | awk '{print $NF}')
  echo "$VON $BIS $KN $SEK $ER" >> $F
  echo "$NAME: $VON-$BIS von $N: $ER, Knoten $KN, ${SEK}s"
  case $ER in
    LOESUNG) echo "LOESUNG GEFUNDEN in $NAME, Bereich $VON-$BIS: alles anhalten"; exit 3;;
    ZEITLIMIT) GR=$(( (BIS - VON) / 2 )); [ $GR -lt 1 ] && GR=1; EST=$LIMIT;;
    OHNE_LOESUNG) VON=$BIS; EST=$SEK
      GR=$(awk -v g=$(( BIS - (BIS-GR) )) -v d=$SEK 'BEGIN{f=600/(d>1?d:1); if(f>4)f=4; if(f<0.5)f=0.5; x=int(g*f+0.5); if(x<10)x=10; print x}');;
  esac
done
[ $VON -ge $N ] && echo "$NAME: alle $N Aufgaben gerechnet"
exit 0
