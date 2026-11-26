/* had_loecher.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Zaehlt fuer Hadamard-Matrizen (N <= 32) ohne c Spalten die flachen x (max|s| < T, x_0 = +1, also Punkte bis auf Vorzeichen)
   fuer jede SCHRITT-te Spaltenmenge. Ausgabe: Minimum, Maximum, Mittel der Lochzahl und der Vergleichswert V(m,R) mit R=(m-T)/2
   (so viele Punkte bis auf Vorzeichen deckt EINE zusaetzliche Zeile hoechstens).
   Aufruf: had_loecher DATEI N c T SCHRITT */
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
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int c=atoi(argv[3]),T=atoi(argv[4]); long schritt=atol(argv[5]);
    int m=N-c,R=(m-T)/2; double V=0,b=1; for(int i=0;i<=R;i++){ V+=b; b=b*(m-i)/(i+1); }
    uint64_t mn=~0ULL,mx=0; double sum=0; long n=0,zaehler=0; int mi=0;
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        for(uint32_t mask=(1u<<c)-1; mask<(1u<<N); ){
            if(zaehler++%schritt==0){ int cols[32],k=0; for(int j=0;j<N;j++) if(!(mask>>j&1)) cols[k++]=j; uint64_t z=flach(cols,m,T); if(z<mn) mn=z; if(z>mx) mx=z; sum+=z; n++; }
            if(c==0) break; uint32_t t=mask|(mask-1); mask=(t+1)|(((~t&-~t)-1)>>(__builtin_ctz(mask)+1)); }
        mi++; }
    printf("N=%d c=%d m=%d T=%d: %ld Mengen aus %d Matrizen (jede %ld-te); flache Punkte bis auf Vorzeichen: min %llu, max %llu, Mittel %.0f von %llu; eine Zusatzzeile deckt hoechstens V(%d,%d) = %.0f\n",
        N,c,m,T,n,mi,schritt,(unsigned long long)mn,(unsigned long long)mx,sum/n,1ULL<<(m-1),m,R,V);
    return 0; }
