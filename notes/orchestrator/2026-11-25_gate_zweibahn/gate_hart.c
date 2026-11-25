/* Gate Z (Orchestrator, 25.11.2026): unabhaengige Nachzaehlung "zwei volle Bahnen ueberdecken alle harten Punkte".
   Laenge m = 2q + f (f >= 1), Radius R. Hart: kein festes Wort erreicht den Punkt.
   Eingabe: Datei mit Zeilen "Wort Gewicht" (erste Bahn A: Klassenvertreter aus klassen.py) oder "-" fuer alle vollen Bahnen (Gewicht 1).
   Ausgabe: Zahl der geordneten Paare (A, B), B != A, die zusammen alle harten Punkte ueberdecken, ungewichtet und gewichtet.
   Bauen: gcc -O3 -march=native -fopenmp -o gate_hart gate_hart.c ; Aufruf: gate_hart q f R datei|- */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
static int q, f, R, m; static uint32_t QM, BM;
static inline uint32_t rot(uint32_t w){ uint32_t a=w&QM, b=(w>>q)&QM, e=w&~BM; a=((a<<1)|(a>>(q-1)))&QM; b=((b<<1)|(b>>(q-1)))&QM; return a|(b<<q)|e; }
static inline int trifft(const uint32_t *ws, uint32_t p){ for(int k=0;k<q;k++){ int d=__builtin_popcount(ws[k]^p); if(d<=R||d>=m-R) return 1; } return 0; }
static inline int falt(int w){ return w<q-w?w:q-w; }
int main(int argc,char**argv){
  q=atoi(argv[1]); f=atoi(argv[2]); R=atoi(argv[3]); m=2*q+f; QM=(1u<<q)-1; BM=(1u<<(2*q))-1;
  uint32_t half=1u<<(m-1);                       /* oberstes Bit (eine Fixspalte) = 0 als Normierung des Komplements */
  uint32_t *orb=malloc(sizeof(uint32_t)*(half/q+16)), *hart=malloc(sizeof(uint32_t)*(half/q+16)); long no=0,nh=0,kurz=0;
  for(uint32_t w=0;w<half;w++){
    uint32_t v=w; int min=1, per=0; for(int k=1;k<q;k++){ v=rot(v); if(v<w){min=0;break;} if(v==w){per=1;} }
    if(!min) continue;
    int wa=__builtin_popcount(w&QM), wb=__builtin_popcount((w>>q)&QM);
    if(per || ((wa==0||wa==q)&&(wb==0||wb==q))){ kurz++; continue; }   /* kurze Bahn (periodisch); bei Primzahl q nur die festen Woerter */
    orb[no++]=w; if(falt(wa)+falt(wb)>R) hart[nh++]=w;
  }
  printf("m=%d q=%d f=%d R=%d: volle Bahnen %ld, kurze Vertreter %ld, harte Klassen (in vollen Punktbahnen) %ld\n",m,q,f,R,no,kurz,nh); fflush(stdout);
  uint32_t *A; long *gew; long nA=0;
  if(strcmp(argv[4],"-")==0){ A=orb; nA=no; gew=malloc(sizeof(long)*no); for(long i=0;i<no;i++) gew[i]=1; }
  else { FILE*fi=fopen(argv[4],"r"); A=malloc(sizeof(uint32_t)*100000); gew=malloc(sizeof(long)*100000); unsigned long w; long g; while(fscanf(fi,"%lu %ld",&w,&g)==2){A[nA]=(uint32_t)w; gew[nA]=g; nA++;} fclose(fi); }
  long sumg=0; for(long i=0;i<nA;i++) sumg+=gew[i];
  printf("erste Bahn: %ld Vertreter, Summe der Gewichte %ld\n",nA,sumg); fflush(stdout);
  long paare=0, paare_gew=0, allein=0; uint32_t *U=malloc(sizeof(uint32_t)*(nh+1));
  for(long i=0;i<nA;i++){
    uint32_t wa[64]; uint32_t v=A[i]; for(int k=0;k<q;k++){ wa[k]=v; v=rot(v);} 
    /* kanonischer Vertreter von A in der Bahnliste (zum Ausschluss B == A) */
    uint32_t amin=0xffffffffu; for(int k=0;k<q;k++){ uint32_t c=wa[k]; if(c&half) c=(~c)&((half<<1)-1); if(c<amin) amin=c; }
    long nu=0; for(long h=0;h<nh;h++) if(!trifft(wa,hart[h])) U[nu++]=hart[h];
    if(nu==0){ allein++; continue; }
    long c=0;
    #pragma omp parallel for schedule(static) reduction(+:c)
    for(long j=0;j<no;j++){
      if(orb[j]==amin) continue;
      uint32_t wb[64]; uint32_t x=orb[j]; for(int k=0;k<q;k++){ wb[k]=x; x=rot(x);} 
      int ok=1; for(long u=0;u<nu;u++) if(!trifft(wb,U[u])){ok=0;break;}
      if(ok) c++;
    }
    paare+=c; paare_gew+=c*gew[i];
  }
  printf("geordnete Paare (A, B), die alle harten Klassen ueberdecken: %ld, gewichtet %ld; A allein ueberdeckt alle harten: %ld\n",paare,paare_gew,allein);
  return 0;
}
