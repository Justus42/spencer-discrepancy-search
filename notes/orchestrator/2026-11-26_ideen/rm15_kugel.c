/* rm15_kugel.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Wie viele tiefe Loecher von RM(1,5) deckt EINE freie Zeile w bei (31,10), d = 11?  T(w) = Summe ueber alle Loch-Nebenklassen x
   von L(w+x), L = #{u: |W(u)| >= 12}. Anteil = T(w) / (32 * 427924). T haengt nur von der AGL(5,2)-Klasse der Nebenklasse von w ab.
   Stichprobe: Zufallsfunktionen mit geradem Gewicht 0..16 (je ANZ Stueck), je Spektrum-Signatur bis zu 3 Vertreter ausgewertet.
   Aufruf: rm15_kugel tief.bin ANZ */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
static inline void fwht32(int8_t *a){ for(int h=1;h<32;h<<=1) for(int i=0;i<32;i+=h<<1) for(int j=i;j<i+h;j++){ int8_t x=a[j],y=a[j+h]; a[j]=x+y; a[j+h]=x-y; } }
static uint32_t *D; static long nD;
static void sig(uint32_t f,char*s){ int8_t a[32]; int cnt[40]; memset(cnt,0,sizeof cnt); for(int p=0;p<32;p++) a[p]=(f>>p&1)?-1:1; fwht32(a);
    for(int u=0;u<32;u++){ int v=a[u]<0?-a[u]:a[u]; cnt[v]++; } int o=0; s[0]=0; for(int v=32;v>=0;v-=4) if(cnt[v]) o+=sprintf(s+o,"%d^%d ",v,cnt[v]); }
static long T(uint32_t w,long*hist){ long t=0; for(int i=0;i<8;i++) hist[i]=0; for(long k=0;k<nD;k++){ uint32_t g=w^D[k]; int8_t a[32]; for(int p=0;p<32;p++) a[p]=(g>>p&1)?-1:1; fwht32(a);
        int L=0; for(int u=0;u<32;u++) L+=(a[u]>=12)|(a[u]<=-12); t+=L; hist[L]++; } return t; }
static uint64_t rs=88172645463325252ULL; static uint32_t rnd(void){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return (uint32_t)(rs>>16); }
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"rb"); fseek(f,0,SEEK_END); nD=ftell(f)/4; rewind(f); D=malloc(4*nD); if(fread(D,4,nD,f)!=(size_t)nD) return 1; long anz=atol(argv[2]);
    char sigs[128][96]; int cnts[128]; int ns=0; double best=0;
    for(int wt=0; wt<=16; wt+=2) for(long t=0;t<anz;t++){
        uint32_t w=0; int k=0; while(k<wt){ int p=rnd()%32; if(!(w>>p&1)){ w|=1u<<p; k++; } }
        if(wt==16 && t%2==0) w=rnd()&~1u, w^=(__builtin_popcount(w)&1); /* auch voellig zufaellige gerade Funktionen */
        char s[96]; sig(w,s); int i; for(i=0;i<ns;i++) if(!strcmp(sigs[i],s)) break; if(i==ns){ strcpy(sigs[ns],s); cnts[ns++]=0; }
        if(cnts[i]>=3) continue; cnts[i]++; long h[8]; long tt=T(w,h); double ant=(double)tt/(32.0*nD); if(ant>best) best=ant;
        printf("w=%08x Gew.%2d Spektrum [%s] T=%ld Anteil %.4f  Loch-Nebenklassen je L: 0:%ld 1:%ld 2:%ld 3:%ld 4:%ld 6:%ld\n",w,__builtin_popcount(w),s,tt,ant,h[0],h[1],h[2],h[3],h[4],h[6]); }
    printf("Signaturen: %d; groesster Anteil der tiefen Loecher in EINER Kugel (mit Gegenkugel): %.4f; noetig im Mittel bei f Zeilen: 1/f\n",ns,best); return 0; }
