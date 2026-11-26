#!/bin/bash
# lauf.sh -- Ideenrunde 26.11.2026: alle Pruefrechnungen des Orchestrators in einem Lauf (2 Kerne, etwa 7 Minuten; danach lin_kontrolle.py).
# Aufruf: bash lauf.sh REPO ARBEITSORDNER     schreibt *.log.txt in diesen Ordner (neben lauf.sh)
set -e
REPO=$(cd "$1" && pwd); T=$2; D=$(cd "$(dirname "$0")" && pwd); mkdir -p $T
for p in had_streich had_loecher had_frei1 had_maske had_allein had_ersatz had_ersatz2 rm15_loecher rm15_kugel zirk15; do gcc -O3 -march=native -o $T/$p $D/$p.c -lm 2>/dev/null; done
cd $T; python3 $D/had_eingaben.py $REPO $T > $D/eingaben.log.txt
python3 - <<'PY'
b=open('had24.txt').read().strip().split('\n\n')
for k in range(2): open('had24_teil%d.txt'%k,'w').write('\n\n'.join(b[k::2])+'\n\n')
r=open('had16iv.txt').read().strip().split('\n\n')[0].split('\n'); r[5]=r[9]; open('ctl_frei1.txt','w').write('\n'.join(r)+'\n\n')
PY
{ echo "# Tabelle: exakte Diskrepanz, alle Spaltenmengen (had_streich, T=0)"; date
  for spec in "had8 8" "had12 12" "had16syl 16" "had16iv 16" "had20 20"; do set -- $spec; for c in 0 1 2 3 4; do echo -n "$1 c=$c: "; ./had_streich $1.txt $2 $c 0 2>&1 >/dev/null | tail -n 1; done; done
  echo "# Ordnung 24 (60 McKay-Klassen + Transponierte), Schwellen"
  echo -n "c=2 T=8: "; ./had_streich had24.txt 24 2 8 2>&1 >/dev/null | tail -n 1
  echo -n "c=5 T=9: "; ./had_streich had24.txt 24 5 9 2>&1 >/dev/null | tail -n 1
  date; } > $D/tabelle.log.txt
( ./had_streich had24_teil0.txt 24 4 8 > c4_teil0.out 2> c4_teil0.err ) &
./had_streich had24_teil1.txt 24 4 8 > c4_teil1.out 2> c4_teil1.err; wait
{ echo "# Ordnung 24 ohne 4 Spalten, disc >= 8 (Teil 0 = Originale, Teil 1 = Transponierte)"; tail -n 1 c4_teil0.err c4_teil1.err
  echo "# Alleinanteil je Zeile fuer alle Treffer"; ( ./had_allein had24_teil0.txt 24 8 c4_teil0.out > a0.out ) & ./had_allein had24_teil1.txt 24 8 c4_teil1.out > a1.out; wait; cat a0.out a1.out; date; } > $D/ordnung24_c4.log.txt
cp c4_teil0.out $D/treffer_had24_c4_teil0.txt; cp c4_teil1.out $D/treffer_had24_c4_teil1.txt
{ echo "# Lochzahlen und Ein-Zeilen-Test (had_loecher, had_frei1)"; date
  ./had_loecher had20.txt 20 4 8 1; ./had_loecher had24.txt 24 5 9 2000; ./had_loecher had24.txt 24 3 9 100; ./had_loecher had24.txt 24 1 9 1
  echo "Kontrollen had_frei1:"; ./had_frei1 had8.txt 8 1 5 1; ./had_frei1 had16iv.txt 16 1 7 1; ./had_frei1 ctl_frei1.txt 16 1 7 1 | tail -n 1
  echo "Ziel d=9, Ordnung 24:"; ./had_frei1 had24.txt 24 1 9 1; ./had_frei1 had24.txt 24 5 9 400; ./had_frei1 had24.txt 24 3 9 40; date; } > $D/freie_zeile.log.txt
{ echo "# RM(1,5): Nebenklassen, tiefe Loecher, L-Histogramm"; date; ./rm15_loecher tief.bin; echo "# eine Kugel bei (31,10)"; ./rm15_kugel tief.bin 4000 | tail -n 1
  echo "# Struktur der Loecher bei (30,10)"; python3 $D/rm15_loch30.py tief.bin; date; } > $D/rm15.log.txt
./rm15_kugel tief.bin 4000 > $D/rm15_kugel_alle.log.txt
python3 - <<'PY'
def rd(fn): return [[[int(v) for v in l.split()] for l in b.split('\n')] for b in open(fn).read().strip().split('\n\n')]
S=rd('had16syl.txt')[0]; IV,IVT=rd('had16iv.txt')
dbl=lambda A,B:[a+b for a,b in zip(A,B)]+[a+[-v for v in b] for a,b in zip(A,B)]
with open('had32_dbl.txt','w') as f:
    for A,B in ((S,S),(IV,IV),(IVT,IVT),(IV,IVT),(S,IV),(S,IVT)):
        for r in dbl(A,B): f.write(' '.join(map(str,r))+'\n')
        f.write('\n')
q=13; qr={x*x%q for x in range(1,q)}; chi=lambda a:0 if a%q==0 else (1 if a%q in qr else -1)
n=q+1; C=[[0]*n for _ in range(n)]
for j in range(1,n): C[0][j]=1; C[j][0]=1
for i in range(q):
    for j in range(q): C[i+1][j+1]=chi(i-j)
H=[[0]*(2*n) for _ in range(2*n)]
for i in range(n):
    for j in range(n):
        blk=[[1,-1],[-1,-1]] if C[i][j]==0 else [[C[i][j],C[i][j]],[C[i][j],-C[i][j]]]
        for a in (0,1):
            for b in (0,1): H[2*i+a][2*j+b]=blk[a][b]
assert all(sum(a*b for a,b in zip(H[i],H[j]))==0 for i in range(28) for j in range(i+1,28))
open('had28_paley2.txt','w').write('\n'.join(' '.join(map(str,r)) for r in H)+'\n\n')
def konf(el,sub,issq):
    q=len(el); n=q+1; C=[[0]*n for _ in range(n)]
    for j in range(1,n): C[0][j]=1; C[j][0]=1
    for i in range(q):
        for j in range(q):
            if i!=j: C[i+1][j+1]=1 if issq(sub(el[i],el[j])) else -1
    assert all(sum(C[i][k]*C[j][k] for k in range(n))==(q if i==j else 0) for i in range(n) for j in range(n)); return C
sq={x*x%29 for x in range(1,29)}; C30=konf(list(range(29)),lambda a,b:(a-b)%29,lambda d:d in sq)
el=[(a,b) for a in range(5) for b in range(5)]; mul=lambda x,y:((x[0]*y[0]+2*x[1]*y[1])%5,(x[0]*y[1]+x[1]*y[0])%5); s25={mul(x,x) for x in el if x!=(0,0)}
C26=konf(el,lambda x,y:((x[0]-y[0])%5,(x[1]-y[1])%5),lambda d:d in s25)
for name,C in (('konf26.txt',C26),('konf30.txt',C30)): open(name,'w').write('\n'.join(' '.join(map(str,r)) for r in C)+'\n\n')
PY
{ echo "# Ordnung 32: Eichung Sylvester (Soll 13693568 / 0 / 215040), dann Doppelungen [[A,B],[A,-B]]: S,S | IV,IV | IVT,IVT | IV,IVT | S,IV | S,IVT"; date
  ./had_maske had32syl.txt 32 11 1; ./had_maske had32syl.txt 32 9 1; ./had_maske had32syl.txt 32 10 3
  echo "d=11, eine Spalte gestrichen:"; ./had_maske had32_dbl.txt 32 11 1 10000 20 100000
  echo "d=10, keine oder zwei Spalten gestrichen:"; ./had_maske had32_dbl.txt 32 10 0 3 30000 10001 20020
  echo "# Ordnung 28 (Paley II, q=13)"; ./had_maske had28_paley2.txt 28 8 0; ./had_maske had28_paley2.txt 28 10 0 3 5 30 f 33; ./had_maske had28_paley2.txt 28 9 1 2
  echo "# Paley-Konferenzmatrizen 26 und 30 (flach = max|s| < T)"; for t in 5 7 9; do ./had_maske konf26.txt 26 $t 0; done; for t in 5 7 9; do ./had_maske konf30.txt 30 $t 0; done
  echo "# zirkulant"; ./zirk15 7; ./zirk15 15; date; } > $D/ordnung28_32_konferenz.log.txt
{ echo "# eine ersetzte Spalte (had_ersatz, had_ersatz2)"; date; ./had_ersatz had8.txt 8 5 | tail -n 1; ./had_ersatz had16iv.txt 16 7 | tail -n 1; ./had_ersatz had20.txt 20 7 | tail -n 1
  ./had_ersatz2 had12.txt 12 5 | tail -n 1; ./had_ersatz2 had12.txt 12 3 | tail -n 1; ./had_ersatz2 had20.txt 20 7 | tail -n 3; date; } > $D/ersatzspalte.log.txt
{ echo "# Struktur der 704er (Auftrag E)"; python3 $D/struktur704.py $REPO; echo "# Buendelung der Loecher, Hadamard 24 (Matrix 0) ohne Spalte 0, d = 9"; python3 $D/loch_buendel.py had24.txt 0 0; date; } > $D/struktur.log.txt
echo FERTIG
