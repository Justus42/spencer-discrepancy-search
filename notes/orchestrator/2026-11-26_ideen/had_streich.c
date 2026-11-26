/* had_streich.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (von keiner Sitzung gegengerechnet).
   Liest +-1-Matrizen (N x N, Bloecke durch Leerzeile getrennt) und prueft fuer jede Menge S von c gestrichenen Spalten,
   ob disc(M ohne S) >= T ist (Gray-Code ueber alle x mit x_0 = +1, Abbruch beim ersten flachen x).
   T = 0: exakter Wert (kein Abbruch). Mit -h: Hierarchie, eine c-Menge wird nur geprueft, wenn alle (c-1)-Teilmengen
   in der Datei VORHER (eine Zeile je Matrix: Index, dann Mengen als Bitmasken) stehen.
   Aufruf: had_streich DATEI N c T [VORHERDATEI]   Ausgabe: je Matrix Zeile "mat i treffer k von n" und die Treffer-Masken. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
#define MAXN 32 /* N <= 32 (ein AVX2-Register je Summenvektor) */
static int N; static int8_t M[MAXN][MAXN];
static int disc_ge(const int *cols, int m, int T, int *exakt){ /* 1, wenn kein x mit max|s|<T; exakt: min max */
    int8_t s0[32] __attribute__((aligned(32))); int8_t c2[MAXN][32] __attribute__((aligned(32))); int8_t x[MAXN];
    memset(s0,0,sizeof s0); memset(c2,0,sizeof c2);
    for(int j=0;j<m;j++){ x[j]=1; for(int i=0;i<N;i++){ c2[j][i]=2*M[i][cols[j]]; s0[i]+=M[i][cols[j]]; } }
    __m256i s=_mm256_load_si256((const __m256i*)s0); __m256i col[MAXN]; for(int j=0;j<m;j++) col[j]=_mm256_load_si256((const __m256i*)c2[j]);
    int best=127; uint64_t total=1ULL<<(m-1);
    if(T){ __m256i thr=_mm256_set1_epi8((char)(T-1));
        for(uint64_t g=0;;){
            __m256i a=_mm256_abs_epi8(s);
            if(_mm256_testz_si256(_mm256_cmpgt_epi8(a,thr),_mm256_set1_epi8(-1))){ if(exakt)*exakt=T-1; return 0; }
            if(++g==total) break; int j=__builtin_ctzll(g)+1;
            if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; }
        }
        if(exakt)*exakt=T; return 1; }
    for(uint64_t g=0;;){
        _mm256_store_si256((__m256i*)s0,_mm256_abs_epi8(s)); int mx=0; for(int i=0;i<N;i++) if(s0[i]>mx) mx=s0[i];
        if(mx<best) best=mx;
        if(++g==total) break; int j=__builtin_ctzll(g)+1;
        if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; }
    }
    if(exakt)*exakt=best; return best;
}
static uint32_t *vor=NULL; static long nvor=0;
static int cmpu(const void*a,const void*b){ uint32_t x=*(const uint32_t*)a,y=*(const uint32_t*)b; return x<y?-1:x>y; }
static int in_vor(uint32_t m){ return bsearch(&m,vor,nvor,sizeof(uint32_t),cmpu)!=NULL; }
int main(int argc,char**argv){
    if(argc<5){ fprintf(stderr,"Aufruf\n"); return 2; }
    FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int c=atoi(argv[3]), T=atoi(argv[4]); FILE*fv=argc>5?fopen(argv[5],"r"):NULL;
    int mi=0; long ges_tr=0, ges_n=0; int hist[64]; memset(hist,0,sizeof hist); long ghist[64]; memset(ghist,0,sizeof ghist);
    for(;;){
        int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; }
        if(!ok) break;
        if(fv){ /* Zeile: "mat i k m1 m2 ..." */
            char w[16]; int idx; long k; if(fscanf(fv,"%15s %d %ld",w,&idx,&k)!=3||idx!=mi){ fprintf(stderr,"Vorherdatei passt nicht\n"); return 3; }
            free(vor); vor=malloc(sizeof(uint32_t)*(k+1)); nvor=k; for(long t=0;t<k;t++){ unsigned u; fscanf(fv,"%x",&u); vor[t]=u; } qsort(vor,nvor,sizeof(uint32_t),cmpu);
        }
        long tr=0,n=0; printf("mat %d ",mi); char *buf=malloc(1<<24); size_t bl=0; buf[0]=0; memset(hist,0,sizeof hist);
        for(uint32_t mask=(1u<<c)-1; mask<(1u<<N); ){
            int skip=0;
            if(fv){ for(int b=0;b<N;b++) if(mask>>b&1){ if(!in_vor(mask&~(1u<<b))){ skip=1; break; } } }
            if(!skip){ int cols[MAXN],m=0; for(int j=0;j<N;j++) if(!(mask>>j&1)) cols[m++]=j; int ex; int r=disc_ge(cols,m,T,&ex); n++;
                if(T==0){ hist[ex]++; ghist[ex]++; } else if(r){ tr++; if(bl<(1<<24)-16) bl+=sprintf(buf+bl," %x",mask); } }
            if(c==0) break; uint32_t t=mask|(mask-1); mask=(t+1)|(((~t&-~t)-1)>>(__builtin_ctz(mask)+1));
        }
        if(T==0){ printf("exakt:"); for(int v=0;v<64;v++) if(hist[v]) printf(" d=%d:%d",v,hist[v]); printf("\n"); }
        else printf("%ld%s\n",tr,buf);
        fprintf(stderr,"mat %d: %ld von %ld geprueften Mengen mit disc >= %d\n",mi,tr,n,T);
        free(buf); ges_tr+=tr; ges_n+=n; mi++;
    }
    if(T==0){ fprintf(stderr,"GESAMT exakt:"); for(int v=0;v<64;v++) if(ghist[v]) fprintf(stderr," d=%d:%ld",v,ghist[v]); fprintf(stderr,"\n"); }
    else fprintf(stderr,"GESAMT: %d Matrizen, %ld Treffer von %ld geprueften Mengen (c=%d, T=%d)\n",mi,ges_tr,ges_n,c,T);
    return 0;
}
