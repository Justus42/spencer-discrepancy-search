/* had_ersatz2.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Exakt: Hadamard-Matrix H (N <= 32, N = 0 mod 4) ohne Spalte j, dazu eine BELIEBIGE neue Spalte v in {+-1}^N. Gibt es v mit disc([H'|v]) >= T+1 (T ungerade, = disc(H'))?
   x = (x', e) ist flach  <=>  alle |b_i(x')| <= T und fuer alle i mit |b_i| = T gilt e*v_i = -sgn(b_i).
   Also: v ist gut  <=>  fuer jedes x' mit max|b| <= T ist v auf T7(x') = {i: |b_i| = T} weder gleich sgn(b) noch gleich -sgn(b).
   Das Programm sammelt alle Bedingungen (Maske, Vorzeichen) und prueft alle 2^(N-1) Spalten v (v_0 = +1).
   Aufruf: had_ersatz2 DATEI N T   Ausgabe: je Fall Zahl der Bedingungen und Zahl der guten v; gute v werden ausgegeben. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
static int N; static int8_t M[32][32];
typedef struct { uint32_t m,s; } Bed;
static int cmpb(const void*a,const void*b){ const Bed*x=a,*y=b; int px=__builtin_popcount(x->m),py=__builtin_popcount(y->m); if(px!=py) return px-py; if(x->m!=y->m) return x->m<y->m?-1:1; return x->s<y->s?-1:x->s>y->s; }
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int T=atoi(argv[3]); int mi=0; long faelle=0, gut_ges=0; long cap=1<<24; Bed*B=malloc(sizeof(Bed)*cap);
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        for(int weg=0;weg<N;weg++){ int cols[32],m=0; for(int j=0;j<N;j++) if(j!=weg) cols[m++]=j;
            int8_t s0[32] __attribute__((aligned(32))); int8_t c2[32][32] __attribute__((aligned(32))); int8_t x[32]; memset(s0,0,32); memset(c2,0,sizeof c2);
            for(int j=0;j<m;j++){ x[j]=1; for(int i=0;i<N;i++){ c2[j][i]=2*M[i][cols[j]]; s0[i]+=M[i][cols[j]]; } }
            __m256i s=_mm256_load_si256((const __m256i*)s0), col[32], t1=_mm256_set1_epi8((char)(T-1)), t2=_mm256_set1_epi8((char)T), null=_mm256_setzero_si256();
            for(int j=0;j<m;j++) col[j]=_mm256_load_si256((const __m256i*)c2[j]);
            long nb=0; uint64_t total=1ULL<<(m-1); uint32_t nmask=(N==32)?0xffffffffu:((1u<<N)-1);
            for(uint64_t g=0;;){ __m256i a=_mm256_abs_epi8(s); uint32_t gt=(uint32_t)_mm256_movemask_epi8(_mm256_cmpgt_epi8(a,t2));
                if(gt==0){ uint32_t ge=(uint32_t)_mm256_movemask_epi8(_mm256_cmpgt_epi8(a,t1))&nmask; uint32_t neg=(uint32_t)_mm256_movemask_epi8(_mm256_cmpgt_epi8(null,s))&ge; /* Bit = 1: b_i < 0 */
                    uint32_t sg=neg; uint32_t alt=(~neg)&ge; if(alt<sg) sg=alt; /* {s, -s} als kleinere der beiden */
                    if(nb<cap){ B[nb].m=ge; B[nb].s=sg; } nb++; }
                if(++g==total) break; int j=__builtin_ctzll(g)+1;
                if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; } }
            if(nb>cap){ fprintf(stderr,"zu viele Bedingungen\n"); return 3; }
            qsort(B,nb,sizeof(Bed),cmpb); long u=0; for(long k=0;k<nb;k++) if(k==0||B[k].m!=B[u-1].m||B[k].s!=B[u-1].s) B[u++]=B[k];
            long klein[8]={0}; for(long k=0;k<u;k++){ int p=__builtin_popcount(B[k].m); klein[p<7?p:7]++; }
            long gut=0; uint32_t bsp=0;
            for(uint32_t v=0; v<(1u<<(N-1)); v++){ /* Bit i = 1: v_i = -1; v_{N-1} = +1 fest */
                long k; for(k=0;k<u;k++){ uint32_t w=v&B[k].m; if(w==B[k].s || w==(B[k].m&~B[k].s)) break; }
                if(k==u){ gut++; bsp=v; } }
            printf("mat %d Spalte %2d: flache x' %ld, verschiedene Bedingungen %ld (Groesse 1: %ld, 2: %ld, 3: %ld, 4: %ld, 5: %ld, 6: %ld, >=7: %ld); gute Ersatzspalten: %ld",mi,weg,nb,u,klein[1],klein[2],klein[3],klein[4],klein[5],klein[6],klein[7],gut);
            if(gut) printf("  BEISPIEL v=%08x",bsp); printf("\n"); fflush(stdout); faelle++; gut_ges+=gut; }
        mi++; }
    printf("N=%d T=%d: %ld Faelle; gute Ersatzspalten insgesamt: %ld\n",N,T,faelle,gut_ges); return 0; }
