/* Tabu-Suche: K Verschiebungen einer Menge S (Bild der Hamming-Kugel im Quotienten F2^m)
   muessen F2^m ueberdecken. Bewertung aller Ersetzungen exakt per Walsh-Transformation (ganzzahlig).
   Aufruf: coset_tabu m n R K seed sekunden spaltenfile
   spaltenfile: n ganze Zahlen (Spalten h_i der Paritaetspruefmatrix, je m Bit).
   Ausgabe: Zeilen "best <unueberdeckt>" und bei Erfolg "SOLUTION c1 c2 ...". */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

static uint64_t rs[2];
static inline uint64_t rotl(uint64_t x,int k){return (x<<k)|(x>>(64-k));}
static uint64_t rnd(void){uint64_t s0=rs[0],s1=rs[1],r=s0+s1;s1^=s0;rs[0]=rotl(s0,55)^s1^(s1<<14);rs[1]=rotl(s1,36);return r;}

static int n,m,R,K; static uint32_t H[64]; static uint32_t N;
static uint8_t *Sset; static uint32_t *Slist; static uint32_t Ssz;

static void ball(int start,int left,uint32_t syn){
  Sset[syn]=1;
  if(!left) return;
  for(int i=start;i<n;i++) ball(i+1,left-1,syn^H[i]);
}
static void wht64(int64_t *a){
  for(uint32_t h=1;h<N;h<<=1)
    for(uint32_t i=0;i<N;i+=h<<1)
      for(uint32_t j=i;j<i+h;j++){int64_t x=a[j],y=a[j+h];a[j]=x+y;a[j+h]=x-y;}
}
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}

int main(int argc,char**argv){
  if(argc<8){fprintf(stderr,"usage\n");return 2;}
  m=atoi(argv[1]);n=atoi(argv[2]);R=atoi(argv[3]);K=atoi(argv[4]);
  uint64_t seed=strtoull(argv[5],0,10);double tmax=atof(argv[6]);
  FILE*f=fopen(argv[7],"r");for(int i=0;i<n;i++)if(fscanf(f,"%u",&H[i])!=1)return 3;fclose(f);
  rs[0]=seed*0x9E3779B97F4A7C15ULL+1;rs[1]=seed^0xD1B54A32D192ED03ULL;for(int i=0;i<20;i++)rnd();
  N=1u<<m;
  Sset=calloc(N,1);ball(0,R,0);
  Slist=malloc(sizeof(uint32_t)*N);Ssz=0;for(uint32_t s=0;s<N;s++)if(Sset[s])Slist[Ssz++]=s;
  int64_t *Sh=malloc(8*(size_t)N),*A=malloc(8*(size_t)N);
  for(uint32_t s=0;s<N;s++)Sh[s]=Sset[s];wht64(Sh);
  uint8_t*cnt=calloc(N,1);uint32_t*c=malloc(4*K);int64_t*tabu=calloc(N,8);
  /* Abschnitt: |S| */
  fprintf(stderr,"m=%d n=%d R=%d K=%d |S|=%u (%.4f von 2^m), untere Schranke K>=%.2f\n",m,n,R,K,Ssz,(double)Ssz/N,(double)N/Ssz);
  double t0=now();int64_t it=0,bestall=1<<30;
  for(int restart=0;;restart++){
    memset(cnt,0,N);
    for(int j=0;j<K;j++){c[j]=rnd()&(N-1);for(uint32_t q=0;q<Ssz;q++)cnt[c[j]^Slist[q]]++;}
    memset(tabu,0,8*(size_t)N);
    int64_t best=1<<30,since=0;
    for(;;it++){
      if((it&15)==0&&now()-t0>tmax){fprintf(stderr,"Zeitlimit, best=%ld, it=%ld\n",(long)bestall,(long)it);printf("done best %ld\n",(long)bestall);return 1;}
      int j=rnd()%K;
      for(uint32_t q=0;q<Ssz;q++)cnt[c[j]^Slist[q]]--;
      int64_t holes=0;for(uint32_t s=0;s<N;s++){A[s]=(cnt[s]==0);holes+=A[s];}
      wht64(A);for(uint32_t s=0;s<N;s++)A[s]*=Sh[s];wht64(A);
      /* A[x]/N = Zahl der Loecher, die x+S deckt */
      int64_t bs=-1;uint32_t bc=0;int nt=0;
      for(uint32_t x=0;x<N;x++){
        int64_t sc=A[x]>>m;
        if(tabu[x]>it && holes-sc!=0) continue;
        if(sc>bs){bs=sc;bc=x;nt=1;}else if(sc==bs&&(rnd()%(++nt))==0)bc=x;
      }
      tabu[c[j]]=it+10+rnd()%20;
      c[j]=bc;for(uint32_t q=0;q<Ssz;q++)cnt[bc^Slist[q]]++;
      int64_t unc=holes-bs;
      if(unc<best){best=unc;since=0;}else since++;
      if(unc<bestall){bestall=unc;fprintf(stderr,"[%.0fs it=%ld r=%d] unueberdeckt %ld\n",now()-t0,(long)it,restart,(long)unc);}
      if(unc==0){printf("SOLUTION");for(int q=0;q<K;q++)printf(" %u",c[q]);printf("\n");fflush(stdout);fprintf(stderr,"geloest nach %.1fs\n",now()-t0);return 0;}
      if(since>3000)break; /* Neustart */
    }
  }
}
