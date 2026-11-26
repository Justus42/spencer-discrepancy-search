/* zirk15.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Alle zirkulanten +-1-Matrizen n x n (n ungerade <= 17): groesste exakte Diskrepanz. Fuer n = 15 ist das die Zelle (15,4) mit 30 Woertern
   unter der zyklischen Gruppe Z15 (eine Bahn); Rekord braeuchte disc 7. Aufruf: zirk15 n */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(int argc,char**argv){ int n=atoi(argv[1]); uint32_t full=(1u<<n)-1; int best=0; long nbest=0; int hist[32]={0};
    for(uint32_t r=0;r<(1u<<(n-1));r++){ /* erste Zeile, Bit n-1 = 0 (bis auf Vorzeichen) */
        uint32_t rot[32]; for(int k=0;k<n;k++) rot[k]=((r<<k)|(r>>(n-k)))&full;
        int mn=99; for(uint32_t x=0;x<(1u<<(n-1)) && mn>best-1; x++){ int mx=0; for(int k=0;k<n;k++){ int s=n-2*__builtin_popcount(rot[k]^x); if(s<0) s=-s; if(s>mx) mx=s; } if(mx<mn) mn=mx; }
        if(mn>=best){ if(mn>best){ best=mn; nbest=0; } nbest++; } }
    printf("n=%d: groesste Diskrepanz zirkulanter Matrizen >= ... exakt %d (%ld erste Zeilen bis auf Vorzeichen), Quotient %.3f\n",n,best,nbest,best/__builtin_sqrt((double)n)); (void)hist; return 0; }
