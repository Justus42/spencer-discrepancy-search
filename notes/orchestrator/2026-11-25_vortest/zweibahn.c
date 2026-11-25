/* Vortest des Orchestrators (25.11.2026): selbstkomplementaere Ueberdeckungscodes, invariant unter einer
   zyklischen Gruppe <g>, g = gleichzeitige zyklische Verschiebung auf c Spaltenzyklen der Laenge q, dazu f Fixspalten.
   Laenge m = c*q + f, Radius R. Vollstaendige Zaehlung aller Vereinigungen von Bahnen (auf Wortpaaren {w, ~w})
   mit hoechstens T Paaren, die den Wuerfel ueberdecken. Verzweigen am ersten unueberdeckten Punkt.
   Gefunden wird jede Ueberdeckung, in der keine Bahn ueberfluessig ist (jede andere enthaelt eine solche).
   Aufruf: zweibahn c q f R T
   Bauen: gcc -O3 -march=native -fopenmp -o zweibahn zweibahn.c */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>

static int c, q, f, R, T, m;
static uint32_t FULL;
static int N;                 /* Zahl der Bahnen */
static uint32_t *rep;         /* Vertreter je Bahn */
static int *sz;               /* Bahngroesse (Paare) */
static int32_t *orb;          /* Bahnindex je Wort */
static uint64_t **cov;        /* cov[i]: Bitmenge der Punktbahnen, die Bahn i ueberdeckt */
static int NW; static uint64_t TAIL;
static int minsz; static int lim[65]; static uint64_t *shortcovb[65]; static int nurkurz=0;

static inline uint32_t g(uint32_t w){
  uint32_t out = w & ~((c*q==32)?0xffffffffu:((1u<<(c*q))-1));
  uint32_t qm = (1u<<q)-1;
  for(int k=0;k<c;k++){ uint32_t b=(w>>(k*q))&qm; b=((b<<1)|(b>>(q-1)))&qm; out|=b<<(k*q);}
  return out;
}
static inline uint32_t canon(uint32_t w){ uint32_t v=(~w)&FULL; return w<v?w:v; }
static inline int hit(uint32_t a, uint32_t b){ int d=__builtin_popcount(a^b); return d<=R || d>=m-R; }

#define MAXSOL 200000
static int (*sol)[24]; static int nsol=0;
static long long nodes=0;

static int cmpint(const void*a,const void*b){return *(const int*)a-*(const int*)b;}
static int cmpsol(const void*a,const void*b){return memcmp(a,b,24*sizeof(int));}

static void record(int *ch, int nch){
  int s[24]; for(int i=0;i<24;i++) s[i]=-1; memcpy(s,ch,nch*sizeof(int)); qsort(s,nch,sizeof(int),cmpint);
  #pragma omp critical
  { if(nsol<MAXSOL){ memcpy(sol[nsol],s,sizeof(s)); nsol++; } }
}

static void solve(uint64_t *mask, int budget, int *ch, int nch, long long *nd){
  (*nd)++;
  int w=0; while(w<NW && mask[w]==~0ULL) w++;
  if(w==NW){ record(ch,nch); return; }
  if(budget<minsz) return;
  int x = w*64 + __builtin_ctzll(~mask[w]);
  uint64_t *cx = cov[x];
  uint64_t *nm = NULL;
  int L = lim[budget>64?64:budget]; if(nurkurz && L>lim[q-1]) L=lim[q-1];
  for(int ww=0; ww*64<L; ww++){
    uint64_t bits = cx[ww];
    while(bits){
      int j = ww*64 + __builtin_ctzll(bits); bits &= bits-1;
      if(j>=L) break;
      uint64_t *cj = cov[j];
      if(budget - sz[j] < minsz){ if(0){} 
        /* letzte Bahn: muss alles Uebrige ueberdecken */
        int ok=1; for(int v=w; v<NW; v++) if((mask[v]|cj[v])!=~0ULL){ok=0;break;}
        (*nd)++;
        if(ok){ ch[nch]=j; record(ch,nch+1); }
      } else {
        if(budget - sz[j] < q){ /* danach nur noch kurze Bahnen: was die zusammen nicht erreichen, muss schon ueberdeckt sein */
          uint64_t *sc=shortcovb[budget-sz[j]];
          int ok=1; for(int v=w; v<NW; v++) if((mask[v]|cj[v]|sc[v])!=~0ULL){ok=0;break;}
          (*nd)++; if(!ok) continue;
        }
        if(!nm) nm = malloc(NW*sizeof(uint64_t));
        for(int v=0; v<NW; v++) nm[v]=mask[v]|cj[v];
        ch[nch]=j; solve(nm, budget-sz[j], ch, nch+1, nd);
      }
    }
  }
  if(nm) free(nm);
}

int main(int argc, char **argv){
  if(argc<6){ fprintf(stderr,"Aufruf: zweibahn c q f R T\n"); return 2; }
  c=atoi(argv[1]); q=atoi(argv[2]); f=atoi(argv[3]); R=atoi(argv[4]); T=atoi(argv[5]);
  m=c*q+f; FULL=(1u<<m)-1;
  uint32_t NP=1u<<m;
  orb=malloc(NP*sizeof(int32_t)); for(uint32_t w=0;w<NP;w++) orb[w]=-1;
  rep=malloc((NP/2/ (q>1?1:1))*sizeof(uint32_t)); sz=malloc((NP/2)*sizeof(int));
  N=0; int cnt[64]; memset(cnt,0,sizeof(cnt));
  for(uint32_t w=0;w<NP;w++){
    if(orb[w]>=0) continue;
    uint32_t list[64]; int s=0; uint32_t v=w;
    for(int k=0;k<q;k++){ uint32_t cv=canon(v); int neu=1; for(int t=0;t<s;t++) if(list[t]==cv){neu=0;break;} if(neu) list[s++]=cv; v=g(v);}
    for(int t=0;t<s;t++){ orb[list[t]]=N; orb[(~list[t])&FULL]=N; }
    rep[N]=canon(w); sz[N]=s; cnt[s]++; N++;
  }
  { /* umsortieren: kurze Bahnen zuerst */
    int *perm=malloc(N*sizeof(int)), *inv=malloc(N*sizeof(int)); int p=0;
    for(int s=1;s<=q;s++) for(int i=0;i<N;i++) if(sz[i]==s) perm[p++]=i;
    uint32_t *r2=malloc(N*sizeof(uint32_t)); int *s2=malloc(N*sizeof(int));
    for(int i=0;i<N;i++){ r2[i]=rep[perm[i]]; s2[i]=sz[perm[i]]; inv[perm[i]]=i; }
    memcpy(rep,r2,N*sizeof(uint32_t)); memcpy(sz,s2,N*sizeof(int));
    for(uint32_t w=0;w<NP;w++) orb[w]=inv[orb[w]];
    for(int b=0;b<=64;b++){ int l=0; while(l<N && sz[l]<=b) l++; lim[b]=l; }
  }
  printf("m=%d (c=%d q=%d f=%d) R=%d T=%d: %d Bahnen auf Paaren;", m,c,q,f,R,T,N);
  minsz=64; for(int s=1;s<=q;s++) if(cnt[s]){ printf(" Groesse %d: %d", s, cnt[s]); if(s<minsz) minsz=s; }
  printf("\n"); fflush(stdout);
  NW=(N+63)/64;
  cov=malloc(N*sizeof(uint64_t*));
  for(int i=0;i<N;i++){ cov[i]=calloc(NW,sizeof(uint64_t)); }
  #pragma omp parallel for schedule(dynamic,64)
  for(int i=0;i<N;i++){
    uint32_t ws[64]; uint32_t v=rep[i]; for(int k=0;k<q;k++){ ws[k]=v; v=g(v);}
    for(int j=0;j<N;j++){
      uint32_t p=rep[j]; int h=0; for(int k=0;k<q;k++) if(hit(ws[k],p)){h=1;break;}
      if(h) cov[i][j>>6] |= 1ULL<<(j&63);
    }
    for(int j=N;j<NW*64;j++) cov[i][j>>6] |= 1ULL<<(j&63);   /* Auffuellbits gelten als ueberdeckt */
  }
  /* Symmetrie der Relation pruefen (Stichprobe) */
  long long asym=0; for(int i=0;i<N;i+= (N>5000? 97:1)) for(int j=0;j<N;j+= (N>5000? 89:1)) if(((cov[i][j>>6]>>(j&63))&1)!=((cov[j][i>>6]>>(i&63))&1)) asym++;
  printf("Ueberdeckungsrelation gebaut, Asymmetrien in der Stichprobe: %lld\n", asym); fflush(stdout);
  for(int b=0;b<q;b++){ shortcovb[b]=calloc(NW,sizeof(uint64_t)); for(int i=0;i<N;i++) if(sz[i]<=b) for(int v=0;v<NW;v++) shortcovb[b][v]|=cov[i][v]; }
  sol=malloc(MAXSOL*sizeof(*sol));
  int modus = argc>6 ? atoi(argv[6]) : -1;
  if(modus==2 || modus==1){
    /* Fall A (modus 2): genau zwei volle Bahnen plus kurze Bahnen mit zusammen hoechstens T-2q Paaren.
       Fall B (modus 1): genau eine volle Bahn plus kurze Bahnen mit zusammen hoechstens T-q Paaren.
       "Harte" Punktbahnen: von keiner erlaubten kurzen Bahn erreichbar; die vollen Bahnen muessen sie alle ueberdecken. */
    nurkurz=1;
    int rest = T - modus*q; if(rest<0){ printf("Budget zu klein\n"); return 0; }
    uint64_t *sc = shortcovb[rest>q-1?q-1:rest];
    uint64_t *hard=malloc(NW*sizeof(uint64_t)); long long nh=0;
    for(int v=0;v<NW;v++){ hard[v]=~sc[v]; nh+=__builtin_popcountll(hard[v]); }
    int F0=lim[q-1];   /* volle Bahnen haben Index >= F0 */
    printf("Fall mit %d vollen Bahnen, Restbudget %d: harte Punktbahnen %lld von %d\n", modus, rest, nh, N); fflush(stdout);
    if(nh==0){ printf("keine harten Punkte: dieser Fall ist hier nicht entschieden\n"); return 0; }
    int v0=0; while(hard[v0]==0) v0++; int x0=v0*64+__builtin_ctzll(hard[v0]);
    int *top=malloc(N*sizeof(int)); int ntop=0;
    for(int j=F0;j<N;j++) if((cov[x0][j>>6]>>(j&63))&1) top[ntop++]=j;
    printf("erste volle Bahn: %d Kandidaten (ueberdecken harte Punktbahn %d, Vertreter %u)\n", ntop, x0, rep[x0]); fflush(stdout);
    long long tot=0, paare=0, ohne=0;
    #pragma omp parallel for schedule(dynamic,8) reduction(+:tot,paare,ohne)
    for(int t=0;t<ntop;t++){
      int j1=top[t]; uint64_t *c1=cov[j1]; int ch[24]; long long nd=0; ch[0]=j1;
      uint64_t *mk=malloc(NW*sizeof(uint64_t));
      int v1=0; while(v1<NW && (hard[v1]&~c1[v1])==0) v1++;
      if(modus==1){
        if(v1==NW){ paare++; memcpy(mk,c1,NW*sizeof(uint64_t)); solve(mk, rest, ch, 1, &nd); }
      } else if(v1==NW){ ohne++; }
      else {
        int x1=v1*64+__builtin_ctzll(hard[v1]&~c1[v1]); uint64_t *cx=cov[x1];
        for(int ww=F0>>6; ww<NW; ww++){ uint64_t bits=cx[ww];
          while(bits){ int j2=ww*64+__builtin_ctzll(bits); bits&=bits-1; if(j2<F0) continue; if(j2>=N) break;
            uint64_t *c2=cov[j2]; int ok=1; nd++;
            for(int v=v1; v<NW; v++) if(hard[v]&~(c1[v]|c2[v])){ok=0;break;}
            if(!ok) continue;
            paare++; for(int v=0;v<NW;v++) mk[v]=c1[v]|c2[v]; ch[1]=j2; solve(mk, rest, ch, 2, &nd);
          } }
      }
      free(mk); tot+=nd;
    }
    qsort(sol,nsol,sizeof(*sol),cmpsol);
    int u=0; for(int i=0;i<nsol;i++) if(i==0||memcmp(sol[i],sol[i-1],sizeof(*sol))) memcpy(sol[u++],sol[i],sizeof(*sol));
    printf("Knoten: %lld. Kombinationen voller Bahnen, die alle harten Punkte ueberdecken: %lld. Erste Bahn ueberdeckt allein alle harten Punkte (nicht weiterverfolgt): %lld. Ueberdeckungen (hoechstens %d Paare): %d\n", tot, paare, ohne, T, u);
    int best=1000; for(int i=0;i<u;i++){ int p=0; for(int k=0;k<24&&sol[i][k]>=0;k++) p+=sz[sol[i][k]]; if(p<best) best=p; }
    if(u) printf("kleinste Paarzahl: %d\n", best);
    FILE *fo=fopen("loesungen.txt","w"); for(int i=0;i<u;i++){ for(int k=0;k<24&&sol[i][k]>=0;k++) fprintf(fo,"%u ", rep[sol[i][k]]); fprintf(fo,"\n"); } fclose(fo);
    return 0;
  }
  /* oberste Ebene: Punktbahn 0, alle Bahnen, die sie ueberdecken */
  int *top=malloc(N*sizeof(int)); int ntop=0;
  for(int j=0;j<N;j++) if((cov[0][j>>6]>>(j&63))&1) if(sz[j]<=T) top[ntop++]=j;
  printf("oberste Ebene: %d Bahnen ueberdecken Punktbahn 0 (Vertreter %u)\n", ntop, rep[0]); fflush(stdout);
  long long tot=0;
  #pragma omp parallel for schedule(dynamic,8) reduction(+:tot)
  for(int t=0;t<ntop;t++){
    int j=top[t]; int ch[24]; long long nd=0; ch[0]=j;
    uint64_t *mk=malloc(NW*sizeof(uint64_t)); memcpy(mk,cov[j],NW*sizeof(uint64_t));
    if(T-sz[j]<minsz){ int ok=1; for(int v=0;v<NW;v++) if(mk[v]!=~0ULL){ok=0;break;} if(ok) record(ch,1); nd++; }
    else solve(mk, T-sz[j], ch, 1, &nd);
    free(mk); tot+=nd;
  }
  qsort(sol,nsol,sizeof(*sol),cmpsol);
  int u=0; for(int i=0;i<nsol;i++) if(i==0||memcmp(sol[i],sol[i-1],sizeof(*sol))) memcpy(sol[u++],sol[i],sizeof(*sol));
  printf("Knoten: %lld. Ueberdeckungen (verschiedene Bahnmengen, hoechstens %d Paare): %d%s\n", tot, T, u, nsol>=MAXSOL?" (Speichergrenze erreicht!)":"");
  /* nach Zusammensetzung und kleinster Paarzahl */
  int best=1000; for(int i=0;i<u;i++){ int p=0; for(int k=0;k<24&&sol[i][k]>=0;k++) p+=sz[sol[i][k]]; if(p<best) best=p; }
  if(u) printf("kleinste Paarzahl: %d\n", best);
  for(int i=0;i<u && i<3;i++){ printf("Beispiel %d:", i); for(int k=0;k<24&&sol[i][k]>=0;k++) printf(" %u(%d)", rep[sol[i][k]], sz[sol[i][k]]); printf("\n"); }
  FILE *fo=fopen("loesungen.txt","w"); for(int i=0;i<u;i++){ for(int k=0;k<24&&sol[i][k]>=0;k++) fprintf(fo,"%u ", rep[sol[i][k]]); fprintf(fo,"\n"); } fclose(fo);
  return 0;
}
