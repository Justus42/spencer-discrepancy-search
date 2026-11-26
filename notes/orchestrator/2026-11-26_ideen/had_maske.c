/* had_maske.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Wie had_loecher, aber fuer vorgegebene Spaltenmengen: zaehlt je Matrix der Datei und je Maske (hex, Bit j = Spalte j gestrichen)
   die flachen x (max|s| < T, erste verbleibende Koordinate = +1, also Punkte bis auf Vorzeichen). N <= 32.
   Aufruf: had_maske DATEI N T MASKE1 [MASKE2 ...] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
static int N; static int8_t M[32][32];
static uint64_t flach(const int*cols,int m,int T){
    int8_t s0[32] __attribute__((aligned(32))); int8_t c2[32][32] __attribute__((aligned(32))); int8_t x[32]; memset(s0,0,32); memset(c2,0,sizeof c2);
    for(int j=0;j<m;j++){ x[j]=1; for(int i=0;i<N;i++){ c2[j][i]=2*M[i][cols[j]]; s0[i]+=M[i][cols[j]]; } }
    __m256i s=_mm256_load_si256((const __m256i*)s0), col[32], thr=_mm256_set1_epi8((char)(T-1)), eins=_mm256_set1_epi8(-1);
    for(int j=0;j<m;j++) col[j]=_mm256_load_si256((const __m256i*)c2[j]);
    uint64_t z=0,total=1ULL<<(m-1);
    for(uint64_t g=0;;){ z+=_mm256_testz_si256(_mm256_cmpgt_epi8(_mm256_abs_epi8(s),thr),eins);
        if(++g==total) break; int j=__builtin_ctzll(g)+1;
        if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; } }
    return z; }
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int T=atoi(argv[3]); int mi=0;
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        for(int a=4;a<argc;a++){ uint32_t mask=(uint32_t)strtoul(argv[a],NULL,16); int cols[32],m=0; for(int j=0;j<N;j++) if(!(mask>>j&1)) cols[m++]=j;
            uint64_t z=flach(cols,m,T); printf("mat %d maske %08x m=%d T=%d: flach %llu von %llu (%.5f %%)\n",mi,mask,m,T,(unsigned long long)z,1ULL<<(m-1),100.0*z/(double)(1ULL<<(m-1))); fflush(stdout); }
        mi++; }
    return 0; }
