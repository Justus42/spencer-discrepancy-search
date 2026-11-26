/* had_frei1.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Frage: Hadamard-Matrix (N <= 32) ohne c Spalten plus EINE freie Zeile -- erreicht das Diskrepanz >= T?
   Je Fall: alle Loecher (x mit max|s| < T, x_0 = +1) aufzaehlen; dann exakt pruefen, ob es ein w gibt mit
   Abstand <= R oder >= m-R zu jedem Loch (R = (m-T)/2). w liegt o. B. d. A. in der Kugel um das erste Loch: alle V(m,R) Kandidaten.
   Ausgabe: Zahl der Faelle, Faelle ohne Loch, Faelle mit Loesung, groesste Zahl gemeinsam gedeckter Loecher (nur grob: beste Kandidaten-Tiefe).
   Aufruf: had_frei1 DATEI N c T SCHRITT */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <immintrin.h>
static int N; static int8_t M[32][32]; static uint32_t *L; static long nL, capL=1<<22;
static void loecher(const int*cols,int m,int T){
    int8_t s0[32] __attribute__((aligned(32))); int8_t c2[32][32] __attribute__((aligned(32))); int8_t x[32]; memset(s0,0,32); memset(c2,0,sizeof c2);
    for(int j=0;j<m;j++){ x[j]=1; for(int i=0;i<N;i++){ c2[j][i]=2*M[i][cols[j]]; s0[i]+=M[i][cols[j]]; } }
    __m256i s=_mm256_load_si256((const __m256i*)s0), col[32], thr=_mm256_set1_epi8((char)(T-1)), eins=_mm256_set1_epi8(-1);
    for(int j=0;j<m;j++) col[j]=_mm256_load_si256((const __m256i*)c2[j]);
    uint64_t total=1ULL<<(m-1); uint32_t bits=0; nL=0;
    for(uint64_t g=0;;){ if(_mm256_testz_si256(_mm256_cmpgt_epi8(_mm256_abs_epi8(s),thr),eins)){ if(nL<capL) L[nL]=bits; nL++; }
        if(++g==total) break; int j=__builtin_ctzll(g)+1; bits^=1u<<j;
        if(x[j]>0){ s=_mm256_sub_epi8(s,col[j]); x[j]=-1; } else { s=_mm256_add_epi8(s,col[j]); x[j]=1; } } }
static long tiefste;
static int eine_zeile(int m,int R){ /* 1, wenn ein w alle Loecher deckt */
    tiefste=0; if(nL==0) return 1; uint32_t h1=L[0];
    for(int wt=0; wt<=R; wt++){
        uint32_t e=(wt==0)?0:((1u<<wt)-1); 
        for(;;){ uint32_t w=h1^e; long k; 
            for(k=1;k<nL;k++){ int d=__builtin_popcount(w^L[k]); if(d>R && d<m-R) break; }
            if(k==nL) return 1; if(k>tiefste) tiefste=k;
            if(k>8){ uint32_t t=L[k]; L[k]=L[k/2]; L[k/2]=t; } /* haeufige Verhinderer nach vorn */
            if(wt==0) break; uint32_t t=e|(e-1); uint32_t nx=(t+1)|(((~t&-~t)-1)>>(__builtin_ctz(e)+1)); if(nx>=(1u<<m)) break; e=nx; } }
    return 0; }
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int c=atoi(argv[3]),T=atoi(argv[4]); long schritt=atol(argv[5]);
    int m=N-c,R=(m-T)/2; double V=0,b=1; for(int i=0;i<=R;i++){ V+=b; b=b*(m-i)/(i+1); } L=malloc(sizeof(uint32_t)*capL);
    long n=0,ohne=0,loes=0,zuviel=0,zaehler=0,minL=1L<<40; int mi=0;
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        for(uint32_t mask=(1u<<c)-1; mask<(1u<<N); ){
            if(zaehler++%schritt==0){ int cols[32],k=0; for(int j=0;j<N;j++) if(!(mask>>j&1)) cols[k++]=j; loecher(cols,m,T); n++; if(nL<minL) minL=nL;
                if(nL==0) ohne++; else if(nL>V || nL>capL) zuviel++; else if(eine_zeile(m,R)){ loes++; printf("LOESUNG mat %d maske %x loecher %ld\n",mi,mask,nL); } }
            if(c==0) break; uint32_t t=mask|(mask-1); mask=(t+1)|(((~t&-~t)-1)>>(__builtin_ctz(mask)+1)); }
        mi++; }
    printf("N=%d c=%d m=%d T=%d R=%d: %ld Faelle aus %d Matrizen (jede %ld-te); ohne Loch %ld; mehr Loecher als V=%.0f: %ld; mit EINER Zusatzzeile loesbar: %ld; kleinste Lochzahl %ld\n",N,c,m,T,R,n,mi,schritt,ohne,V,zuviel,loes,minL);
    return 0; }
