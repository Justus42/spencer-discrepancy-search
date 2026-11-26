/* had_ersatz.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Frage: Hadamard-Matrix (N <= 32) mit EINER Spalte durch eine beliebige Spalte v in [-1,1]^N ersetzt -- kann disc >= T+1 werden (T = N-1-2R ungerade)?
   Gibt es ein x' (uebrige N-1 Spalten) mit genau EINER Zeile |b_i| = T und allen anderen |b_i| <= T-2, dann nein:
   mit passendem Vorzeichen der neuen Koordinate bleibt diese Zeile bei T - |v_i| <= T < T+1, die anderen bei <= T-2+1.
   Ausgabe je Matrix und Spalte: Zahl solcher x'. Aufruf: had_ersatz DATEI N T */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
static int N; static int8_t M[32][32];
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int T=atoi(argv[3]); int mi=0; long faelle=0, ohne=0; uint64_t gmin=~0ULL;
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        for(int weg=0;weg<N;weg++){ int cols[32],m=0; for(int j=0;j<N;j++) if(j!=weg) cols[m++]=j;
            int8_t s0[32] __attribute__((aligned(32))); int8_t c2[32][32] __attribute__((aligned(32))); int8_t x[32]; memset(s0,0,32); memset(c2,0,sizeof c2);
            for(int j=0;j<m;j++){ x[j]=1; for(int i=0;i<N;i++){ c2[j][i]=2*M[i][cols[j]]; s0[i]+=M[i][cols[j]]; } }
            __m256i s=_mm256_load_si256((const __m256i*)s0), col[32], t1=_mm256_set1_epi8((char)(T-1)), t2=_mm256_set1_epi8((char)T);
            for(int j=0;j<m;j++) col[j]=_mm256_load_si256((const __m256i*)c2[j]);
            uint64_t z=0,total=1ULL<<(m-1);
            for(uint64_t g=0;;){ __m256i a=_mm256_abs_epi8(s); uint32_t ge=(uint32_t)_mm256_movemask_epi8(_mm256_cmpgt_epi8(a,t1)), gt=(uint32_t)_mm256_movemask_epi8(_mm256_cmpgt_epi8(a,t2));
                z+=(gt==0 && ge!=0 && (ge&(ge-1))==0);
                if(++g==total) break; int j=__builtin_ctzll(g)+1;
                if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; } }
            faelle++; if(z==0){ ohne++; printf("OHNE Einzelzeile: mat %d Spalte %d\n",mi,weg); } if(z<gmin) gmin=z; }
        mi++; }
    printf("N=%d T=%d: %ld Faelle (Matrix, Spalte); ohne ein x' mit genau einer Zeile bei %d: %ld; kleinste Zahl solcher x' (bis auf Vorzeichen): %llu\n",N,T,faelle,T,ohne,(unsigned long long)gmin); return 0; }
