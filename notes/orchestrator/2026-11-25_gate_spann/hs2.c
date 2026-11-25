// Gate zu Auftrag S (Orchestrator 25.11.2026): eigene exakte Suche, ohne Symmetrie außer Verschiebung (0 in S).
// Gesucht S in G = F_2^q, |S| <= k, mit S + A_D = G für alle Nebenklassen D. Übersetzen mit -DW=2 (q <= 7) oder -DW=4 (q = 8).
// Schranken (alle elementar): je Nebenklasse offene Punkte <= Rest * mu(D); ein offener Punkt ohne erlaubten Kandidaten;
// global: offene Punkte <= Summe der Rest größten Gewinne erlaubter Kandidaten.
// Aufruf: hs2 problem.txt k sekunden [teil anzahl]   (teilt die Zweige der Wurzel auf mehrere Prozesse auf)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#ifndef W
#define W 4
#endif
typedef struct { uint64_t w[W]; } M;
static int q, n, nD, mu[4096]; static M *T; static M ALL;
static unsigned long long knoten=0; static int gefunden=0, teil=0, anzahl=1, abbruch=0; static double limit; static time_t t0;
static inline int pc(const M*a){int c=0;for(int i=0;i<W;i++)c+=__builtin_popcountll(a->w[i]);return c;}
static int S[64], ns=0;
static int cmp(const void*a,const void*b){ return *(const int*)b-*(const int*)a; }
static void suche(int r, M *off, M X, int tiefe){
  if(gefunden||abbruch) return;
  if((++knoten&0xFFFF)==0 && difftime(time(NULL),t0)>limit){ abbruch=1; return; }
  int U=0;
  for(int D=0;D<nD;D++){ int c=pc(&off[D]); if(c>r*mu[D]) return; U+=c; }
  if(U==0){ gefunden=1; printf("LOESUNG:"); for(int i=0;i<ns;i++) printf(" %d",S[i]); printf("\n"); return; }
  if(r==0) return;
  int gew[64*W], liste[64*W], nl=0;
  for(int s=0;s<n;s++){ if((X.w[s>>6]>>(s&63))&1){ gew[s]=-1; continue; }
    int g=0; for(int D=0;D<nD;D++) for(int j=0;j<W;j++) g+=__builtin_popcountll(off[D].w[j]&T[D*n+s].w[j]);
    gew[s]=g; liste[nl++]=g; }
  qsort(liste,nl,sizeof(int),cmp); { long sum=0; for(int i=0;i<r&&i<nl;i++) sum+=liste[i]; if(sum<U) return; }
  int bd=-1,bg=-1,bc=1<<30;
  for(int D=0;D<nD;D++) for(int i=0;i<W;i++){ uint64_t b=off[D].w[i];
      while(b){ int g=i*64+__builtin_ctzll(b); b&=b-1;
        int cnt=0; for(int j=0;j<W;j++) cnt+=__builtin_popcountll(T[D*n+g].w[j]&~X.w[j]);
        if(cnt==0) return;
        if(cnt<bc){ bc=cnt; bd=D; bg=g; } } }
  int cand[64*W], nc=0;
  for(int s=0;s<n;s++) if(((T[bd*n+bg].w[s>>6]>>(s&63))&1) && gew[s]>=0) cand[nc++]=s;
  for(int i=0;i<nc;i++) for(int j=i+1;j<nc;j++) if(gew[cand[j]]>gew[cand[i]]){ int t=cand[i]; cand[i]=cand[j]; cand[j]=t; }
  M *neu=malloc(sizeof(M)*nD);
  for(int i=0;i<nc;i++){ int s=cand[i];
    if(tiefe==0 && (i % anzahl)!=teil){ X.w[s>>6]|=1ULL<<(s&63); continue; }
    for(int D=0;D<nD;D++) for(int j=0;j<W;j++) neu[D].w[j]=off[D].w[j]&~T[D*n+s].w[j];
    S[ns++]=s; suche(r-1,neu,X,tiefe+1); ns--;
    X.w[s>>6]|=1ULL<<(s&63);
    if(gefunden||abbruch) break; }
  free(neu);
}
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"r"); int k=atoi(argv[2]); limit=atof(argv[3]); if(argc>5){teil=atoi(argv[4]);anzahl=atoi(argv[5]);}
  int m,R; if(fscanf(f,"%d %d %d %d",&q,&nD,&m,&R)!=4) return 1; n=1<<q; if(n>64*W) return 2;
  T=calloc((size_t)nD*n,sizeof(M)); memset(&ALL,0,sizeof ALL); for(int g=0;g<n;g++) ALL.w[g>>6]|=1ULL<<(g&63);
  for(int D=0;D<nD;D++){ int c; if(fscanf(f,"%d",&c)!=1) return 3; mu[D]=c;
    for(int i=0;i<c;i++){ int t; if(fscanf(f,"%d",&t)!=1) return 3; for(int s=0;s<n;s++){ int x=s^t; T[D*n+s].w[x>>6]|=1ULL<<(x&63); } } }
  M *off=malloc(sizeof(M)*nD); M X; memset(&X,0,sizeof X);
  for(int D=0;D<nD;D++) for(int j=0;j<W;j++) off[D].w[j]=ALL.w[j]&~T[D*n+0].w[j];
  S[ns++]=0; X.w[0]|=1; t0=time(NULL);
  suche(k-1,off,X,0);
  printf("q=%d Nebenklassen=%d k=%d Teil %d/%d: %s, %llu Knoten, %.0f s\n",q,nD,k,teil,anzahl,
    gefunden?"LOESBAR":(abbruch?"ABGEBROCHEN (Zeitlimit), nichts entschieden":"KEINE LOESUNG (vollstaendig)"),knoten,difftime(time(NULL),t0));
  return 0; }
