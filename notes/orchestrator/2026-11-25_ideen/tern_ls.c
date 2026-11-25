// Vortest Ideensammlung 25.11.2026 (Orchestrator). SUCHCODE, kein Pruefcode.
// Lokale Suche direkt auf den Matrixeintraegen (N Zeilen, m Spalten), wahlweise mit Nullen (-z),
// mit Punktgewichten (-w, nach Art der Klauselgewichte bei SAT / Zeilengewichte bei Mengenueberdeckung).
// Zeile mit Traeger s zaehlt fuer x, wenn |<a,x>| >= d_s; d_s = kleinstes d mit 8 d^2 > 25*G und d = s mod 2 (G = Groesse, -G).
// Punkte modulo Vorzeichen: x mit Bit m-1 = 0. Ausgabe: beste Restzahl (Klassen), bei 0 die Matrix.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#define MAXN 40
static int N=15,m=15,G=15,zeros=0,useW=0,K=12; static long maxsteps=1000000000L; static double maxsec=60; static unsigned long long rs=1;
static int8_t *val[MAXN]; static uint8_t *cnt; static uint32_t *w; static int8_t A[MAXN][32]; static int sup[MAXN]; static int dthr[64];
static inline unsigned long long rnd(void){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return rs; }
static long P;
static long long delta(int i,int j,int b){ // Kostenaenderung, wenn A[i][j] := b
  int a=A[i][j]; int ns=sup[i]+(b!=0)-(a!=0); int dold=dthr[sup[i]], dnew=dthr[ns]; int df=b-a; long long D=0;
  const int8_t *v=val[i]; const uint8_t *c=cnt; const uint32_t *ww=w;
  for(long x=0;x<P;x++){ int xs=((x>>j)&1)?-1:1; int ov=v[x]; int nv=ov+df*xs; int oc=(ov>=dold)|(ov<=-dold); int nc=(nv>=dnew)|(nv<=-dnew);
    if(oc!=nc){ if(oc){ if(c[x]==1) D+=ww[x]; } else { if(c[x]==0) D-=ww[x]; } } }
  return D; }
static long raw;
static void apply(int i,int j,int b){ int a=A[i][j]; int ns=sup[i]+(b!=0)-(a!=0); int dold=dthr[sup[i]], dnew=dthr[ns]; int df=b-a; int8_t *v=val[i];
  for(long x=0;x<P;x++){ int xs=((x>>j)&1)?-1:1; int ov=v[x]; int nv=ov+df*xs; int oc=(ov>=dold)|(ov<=-dold); int nc=(nv>=dnew)|(nv<=-dnew); v[x]=nv;
    if(oc!=nc){ if(oc){ if(--cnt[x]==0) raw++; } else { if(cnt[x]++==0) raw--; } } }
  A[i][j]=b; sup[i]=ns; }
int main(int argc,char**argv){ const char*start=NULL; int drop=-1;
  for(int k=1;k<argc;k++){ if(!strcmp(argv[k],"-N"))N=atoi(argv[++k]); else if(!strcmp(argv[k],"-m"))m=atoi(argv[++k]); else if(!strcmp(argv[k],"-G"))G=atoi(argv[++k]);
    else if(!strcmp(argv[k],"-z"))zeros=1; else if(!strcmp(argv[k],"-w"))useW=1; else if(!strcmp(argv[k],"-s"))rs=strtoull(argv[++k],0,10)*2654435761ULL+88172645463325252ULL;
    else if(!strcmp(argv[k],"-t"))maxsec=atof(argv[++k]); else if(!strcmp(argv[k],"-K"))K=atoi(argv[++k]); else if(!strcmp(argv[k],"-f"))start=argv[++k]; else if(!strcmp(argv[k],"-drop"))drop=atoi(argv[++k]); }
  P=1L<<(m-1);
  for(int s=0;s<64;s++){ int d=1; while(8*d*d<=25*G || (d&1)!=(s&1)) d++; dthr[s]=d; }
  cnt=calloc(P,1); w=malloc(P*sizeof(uint32_t)); for(long x=0;x<P;x++)w[x]=1;
  if(start){ FILE*f=fopen(start,"r"); int r=0; int rows=N+(drop>=0); for(int i=0;i<rows;i++){ int tmp[64]; for(int j=0;j<m;j++) if(fscanf(f,"%d",&tmp[j])!=1){fprintf(stderr,"Lesefehler\n");return 2;} char buf[4096]; if(!fgets(buf,sizeof buf,f)){} if(i==drop)continue; for(int j=0;j<m;j++)A[r][j]=tmp[j]; r++; } fclose(f); }
  else for(int i=0;i<N;i++)for(int j=0;j<m;j++)A[i][j]=(rnd()&1)?1:-1;
  for(int i=0;i<N;i++){ val[i]=malloc(P); sup[i]=0; for(int j=0;j<m;j++)sup[i]+=A[i][j]!=0;
    for(long x=0;x<P;x++){ int v=0; for(int j=0;j<m;j++) v+=((x>>j)&1)?-A[i][j]:A[i][j]; val[i][x]=v; if(v>=dthr[sup[i]]||v<=-dthr[sup[i]])cnt[x]++; } }
  raw=0; for(long x=0;x<P;x++) raw+=cnt[x]==0;
  long best=raw, steps=0, bumps=0; clock_t t0=clock(); printf("Start: N=%d m=%d G=%d Nullen=%d Gewichte=%d Restzahl=%ld (Klassen)\n",N,m,G,zeros,useW,raw);
  while(raw>0 && steps<maxsteps){ if((steps&255)==0 && (double)(clock()-t0)/CLOCKS_PER_SEC>maxsec) break; steps++;
    long x=rnd()%P; while(cnt[x]) { x++; if(x==P)x=0; }
    int bi=-1,bj=-1,bb=0; long long bD=0; int have=0; int ci[64],cj[64],cb[64]; int nc=0;
    for(int t=0;t<4*K && nc<K;t++){ int i=rnd()%N; int v=val[i][x]; int gap=dthr[sup[i]]-abs(v); if(gap>6 && (rnd()&3)) continue; int sg=v>0?1:(v<0?-1:((rnd()&1)?1:-1));
      int j=rnd()%m; int xs=((x>>j)&1)?-1:1; int want=sg*xs; int a=A[i][j]; int b;
      if(a==want){ if(!zeros) continue; if(rnd()%6) continue; b=0; } // seltener Zug: Eintrag auf 0 (aendert die Paritaet)
      else if(a==0) b=want; else { b=want; if(zeros && (rnd()%4==0)) b=0; }
      ci[nc]=i;cj[nc]=j;cb[nc]=b;nc++; }
    if(nc==0) continue;
    for(int t=0;t<nc;t++){ long long D=delta(ci[t],cj[t],cb[t]); if(!have||D<bD){have=1;bD=D;bi=ci[t];bj=cj[t];bb=cb[t];} }
    if(bD<0) apply(bi,bj,bb);
    else { if(useW){ for(long y=0;y<P;y++) if(!cnt[y]) w[y]++; bumps++; if((rnd()%10)<3) apply(bi,bj,bb); }
           else { if((rnd()%100)<(bD==0?50:8)) { int t=rnd()%nc; apply(ci[t],cj[t],cb[t]); } } }
    if(raw<best){ best=raw; printf("Schritt %ld  %.1f s  Restzahl %ld\n",steps,(double)(clock()-t0)/CLOCKS_PER_SEC,best); fflush(stdout); } }
  printf("Ende: Schritte=%ld Gewichtsrunden=%ld Zeit=%.1f s beste Restzahl=%ld aktuelle=%ld\n",steps,bumps,(double)(clock()-t0)/CLOCKS_PER_SEC,best,raw);
  if(raw==0){ printf("LOESUNG (noch ungeprueft):\n"); for(int i=0;i<N;i++){ for(int j=0;j<m;j++)printf("%2d ",A[i][j]); printf("\n"); } }
  int hist[64]={0}; for(int i=0;i<N;i++)hist[sup[i]]++; printf("Traeger:"); for(int s=0;s<=m;s++) if(hist[s]) printf(" %d x s=%d",hist[s],s); printf("\n");
  return 0; }
