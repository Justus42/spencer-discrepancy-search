/* had_allein.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Fuer jede Hadamard-Matrix der Datei und jede Spaltenmenge aus der Trefferdatei (Ausgabe von had_streich, disc >= T):
   Alleinanteil je Zeile = Zahl der x (erste Koordinate +1), die NUR diese Zeile mit |s| >= T deckt. Ist er fuer eine Zeile 0,
   so ist die Zeile ueberfluessig (Code mit zwei Woertern weniger). Ausgabe: Verteilung des kleinsten Alleinanteils und der Profile.
   Aufruf: had_allein DATEI N T TREFFERDATEI */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
static int N; static int8_t M[32][32];
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int T=atoi(argv[3]); FILE*fv=fopen(argv[4],"r"); int mi=0; long faelle=0, gmin=1L<<40, gmincover=99; 
    long hist_min[8]={0}; /* Klassen des kleinsten Alleinanteils: 0, <100, <1000, <3000, <6000, <10000, >= */
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        char w[16]; int idx; long k; if(fscanf(fv,"%15s %d %ld",w,&idx,&k)!=3||idx!=mi){ fprintf(stderr,"Trefferdatei passt nicht\n"); return 3; }
        long mmin=1L<<40,mmax=0;
        for(long t=0;t<k;t++){ unsigned mask; if(fscanf(fv,"%x",&mask)!=1) return 4; int cols[32],m=0; for(int j=0;j<N;j++) if(!(mask>>j&1)) cols[m++]=j;
            int8_t s0[32] __attribute__((aligned(32))); int8_t c2[32][32] __attribute__((aligned(32))); int8_t x[32]; memset(s0,0,32); memset(c2,0,sizeof c2);
            for(int j=0;j<m;j++){ x[j]=1; for(int i=0;i<N;i++){ c2[j][i]=2*M[i][cols[j]]; s0[i]+=M[i][cols[j]]; } }
            __m256i s=_mm256_load_si256((const __m256i*)s0), col[32], thr=_mm256_set1_epi8((char)(T-1));
            for(int j=0;j<m;j++) col[j]=_mm256_load_si256((const __m256i*)c2[j]);
            long allein[32]; memset(allein,0,sizeof allein); long offen=0; uint64_t total=1ULL<<(m-1);
            for(uint64_t g=0;;){ uint32_t bm=(uint32_t)_mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_abs_epi8(s),thr));
                if(bm==0) offen++; else if((bm&(bm-1))==0) allein[__builtin_ctz(bm)]++;
                if(++g==total) break; int j=__builtin_ctzll(g)+1;
                if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; } }
            if(offen){ fprintf(stderr,"FEHLER: mat %d maske %x hat %ld offene Punkte\n",mi,mask,offen); return 5; }
            long mn=1L<<40,mx=0; for(int i=0;i<N;i++){ if(allein[i]<mn) mn=allein[i]; if(allein[i]>mx) mx=allein[i]; }
            if(mn<mmin) mmin=mn; if(mx>mmax) mmax=mx; if(mn<gmin){ gmin=mn; } faelle++;
            hist_min[mn==0?0:mn<100?1:mn<1000?2:mn<3000?3:mn<6000?4:mn<10000?5:6]++;
            if(mn==0) printf("UEBERFLUESSIGE ZEILE: mat %d maske %x\n",mi,mask); }
        if(k) printf("mat %d: %ld Faelle, kleinster Alleinanteil einer Zeile %ld, groesster %ld\n",mi,k,mmin,mmax);
        mi++; }
    printf("GESAMT: %ld Faelle; kleinster Alleinanteil ueberhaupt %ld; Faelle nach kleinstem Alleinanteil: =0: %ld, <100: %ld, <1000: %ld, <3000: %ld, <6000: %ld, <10000: %ld, >=10000: %ld\n",
        faelle,gmin,hist_min[0],hist_min[1],hist_min[2],hist_min[3],hist_min[4],hist_min[5],hist_min[6]); (void)gmincover; return 0; }
