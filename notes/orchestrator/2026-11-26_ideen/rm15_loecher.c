/* rm15_loecher.c -- Ideenrunde 26.11.2026, Pruefrechnung des Orchestrators (nicht gegengerechnet).
   Alle 2^26 Nebenklassen von RM(1,5) in F_2^32 (Vertreter: f = 0 auf den Punkten 0,1,2,4,8,16). Je Nebenklasse das Walsh-Spektrum.
   Ausgabe: (1) Zahl der Nebenklassen je max|W| (Abstand zu RM(1,5) = (32-max|W|)/2); tiefe Loecher = max|W| = 8.
   (2) L(g) = Zahl der u mit |W_g(u)| >= 12, Histogramm ueber alle Nebenklassen gerader Funktionen (W = 0 mod 4).
   (3) schreibt die Vertreter der tiefen Loecher nach DATEI (uint32, binaer).
   Aufruf: rm15_loecher DATEI */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
static inline void fwht32(int8_t *a){ for(int h=1;h<32;h<<=1) for(int i=0;i<32;i+=h<<1) for(int j=i;j<i+h;j++){ int8_t x=a[j],y=a[j+h]; a[j]=x+y; a[j+h]=x-y; } }
int main(int argc,char**argv){
    int frei[26],nf=0; for(int p=0;p<32;p++) if(p&(p-1)) frei[nf++]=p; /* p weder 0 noch Zweierpotenz */
    long hmax[40]; memset(hmax,0,sizeof hmax); long hL[40]; memset(hL,0,sizeof hL); long hLodd[40]; memset(hLodd,0,sizeof hLodd);
    long hL10tief[40]; memset(hL10tief,0,sizeof hL10tief);
    FILE*out=fopen(argv[1],"wb"); long ntief=0; char spek[8][64]; long spekz[64]; int nspek=0; char sp[64][64];
    for(uint32_t r=0;r<(1u<<26);r++){
        uint32_t f=0; for(int k=0;k<26;k++) if(r>>k&1) f|=1u<<frei[k];
        int8_t a[32]; for(int p=0;p<32;p++) a[p]=(f>>p&1)?-1:1; fwht32(a);
        int mx=0,L=0,cnt[40]; memset(cnt,0,sizeof cnt); for(int u=0;u<32;u++){ int v=a[u]<0?-a[u]:a[u]; if(v>mx) mx=v; if(v>=12) L++; cnt[v]++; }
        hmax[mx]++; if((__builtin_popcount(f)&1)==0) hL[L]++; else hLodd[L]++;
        if(mx==8){ fwrite(&f,4,1,out); ntief++; char s[64]; sprintf(s,"8^%d 4^%d 0^%d",cnt[8],cnt[4],cnt[0]); int t; for(t=0;t<nspek;t++) if(!strcmp(sp[t],s)) break; if(t==nspek){ strcpy(sp[nspek],s); spekz[nspek++]=0; } spekz[t]++; }
        if((__builtin_popcount(f)&1)==0 && L>=5){ char s[64]; int o=0; for(int v=32;v>=0;v-=4) if(cnt[v]) o+=sprintf(s+o,"%d^%d ",v,cnt[v]); static int gez=0; if(gez<0) printf("  L=%d Spektrum %s f=%08x\n",L,s,f),gez++; }
    }
    fclose(out); (void)spek;
    printf("Nebenklassen je max|W| (Abstand):"); for(int v=0;v<40;v++) if(hmax[v]) printf(" %d(%d):%ld",v,(32-v)/2,hmax[v]); printf("\n");
    printf("tiefe Loecher: %ld Nebenklassen = %ld Funktionen; Spektren:",ntief,ntief*64); for(int t=0;t<nspek;t++) printf("  [%s]: %ld",sp[t],spekz[t]); printf("\n");
    printf("L = #{u: |W|>=12}, gerade Funktionen:"); for(int v=0;v<40;v++) if(hL[v]) printf(" L=%d:%ld",v,hL[v]); printf("\n");
    printf("L = #{u: |W|>=12}, ungerade Funktionen:"); for(int v=0;v<40;v++) if(hLodd[v]) printf(" L=%d:%ld",v,hLodd[v]); printf("\n");
    return 0; }
