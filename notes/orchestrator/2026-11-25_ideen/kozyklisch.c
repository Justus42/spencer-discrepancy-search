// Vortest Ideensammlung 25.11.2026 (Orchestrator). SUCHCODE (Sieb), kein Pruefcode.
// Kozyklische Matrizen: Gruppe E der Ordnung 2n mit zentraler Involution z, f: E -> {+-1} ungerade (f(zg) = -f(g)),
// M[g][h] = f(g^-1 h) ueber Vertretern der n Nebenklassen von <z>. Die 2n Woerter (Zeilen und Komplemente) sind EINE Bahn von E;
// das Komplement steckt in der Gruppe. Fuer jedes f (f(e)=+1) wird ein x mit max_i |<M_i,x>| <= d-2 gesucht
// (Zufallsproben, dann Abstieg, dann vollstaendig). Ueberlebt ein f alle drei Stufen, gilt disc >= d (noch ungeprueft!).
// Gruppen: Elemente (c,i,e), c in Z_k, i in Z_2t, e in {0,1}; Typ: 0 zyklisch Z_2n (negazyklisch); 1 Dieder-artig; 2 dizyklisch-artig; 3 abelsch Z_k x Z_2t x Z_2.
// z = (0,t,0). n = 2kt (Typ 0: n beliebig).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
static int n,typ,k,t,d,lo,hi; static uint32_t T[32][4][256], SG[32]; static uint32_t mask; static unsigned long long rs=88172645463325252ULL;
static inline unsigned long long rnd(void){ rs^=rs<<13; rs^=rs>>7; rs^=rs<<17; return rs; }
typedef struct{int c,i,e;}El;
static El mul(El a,El b){ El r; if(typ==0){ r.c=0;r.e=0;r.i=(a.i+b.i)%(2*n); return r; }
  r.c=(a.c+b.c)%k; r.e=a.e^b.e; int j=(typ==3||!a.e)?b.i:(2*t-b.i)%(2*t); r.i=(a.i+j)%(2*t); if(typ==2&&a.e&&b.e) r.i=(r.i+t)%(2*t); return r; }
static int eq(El a,El b){return a.c==b.c&&a.i==b.i&&a.e==b.e;}
static El inv(El a){ // durch Suche (kleine Gruppe)
  El r; if(typ==0){r.c=0;r.e=0;r.i=(2*n-a.i)%(2*n);return r;}
  for(r.c=0;r.c<k;r.c++)for(r.i=0;r.i<2*t;r.i++)for(r.e=0;r.e<2;r.e++){ El p=mul(a,r); if(p.c==0&&p.i==0&&p.e==0) return r; } fprintf(stderr,"kein Inverses\n"); exit(3); }
static int repidx(El a,int*sgn){ if(typ==0){ *sgn=a.i>=n; return a.i%n; } int i=a.i; *sgn=0; if(i>=t){i-=t;*sgn=1;} return (a.c*t+i)*2+a.e; }
static El rep(int idx){ El a; if(typ==0){a.c=0;a.e=0;a.i=idx;return a;} a.e=idx&1; idx>>=1; a.i=idx%t; a.c=idx/t; return a; }
static uint32_t rows[32]; static int src[32][32];
static inline void mkrows(uint32_t f){ for(int g=0;g<n;g++) rows[g]=(T[g][0][f&255]^T[g][1][(f>>8)&255]^T[g][2][(f>>16)&255]^T[g][3][f>>24]^SG[g]); }
static inline int unc(uint32_t x){ for(int g=0;g<n;g++){ int pc=__builtin_popcount(rows[g]^x); if(pc<lo||pc>hi) return 0; } return 1; }
static int abstieg(uint32_t*xout,int restarts){ int pc[32];
  for(int r=0;r<restarts;r++){ uint32_t x=rnd()&mask; int pen=0; for(int g=0;g<n;g++){ pc[g]=__builtin_popcount(rows[g]^x); pen+= pc[g]<lo?lo-pc[g]:(pc[g]>hi?pc[g]-hi:0); }
    for(int it=0;it<6*n&&pen>0;it++){ int bj=-1,bp=1<<30,cntb=0; for(int j=0;j<n;j++){ int p=0; for(int g=0;g<n;g++){ int q=pc[g]+(((rows[g]^x)>>j)&1?-1:1); p+= q<lo?lo-q:(q>hi?q-hi:0); }
        if(p<bp){bp=p;bj=j;cntb=1;} else if(p==bp){ cntb++; if(rnd()%cntb==0) bj=j; } }
      if(bp>pen && (rnd()&3)) break; for(int g=0;g<n;g++) pc[g]+=(((rows[g]^x)>>bj)&1?-1:1); x^=1u<<bj; pen=bp; }
    if(pen==0){ *xout=x; return 1; } }
  return 0; }
int main(int argc,char**argv){ if(argc<6){fprintf(stderr,"Aufruf: typ n k t d [stufe2-restarts] [nur-zaehlen]\n");return 1;}
  typ=atoi(argv[1]); n=atoi(argv[2]); k=atoi(argv[3]); t=atoi(argv[4]); d=atoi(argv[5]); int R2=argc>6?atoi(argv[6]):60; double maxsec=argc>7?atof(argv[7]):600;
  if(typ!=0 && n!=2*k*t){fprintf(stderr,"n != 2kt\n");return 1;} lo=(n-d+2)/2; hi=(n+d-2)/2; mask=(n==32)?0xffffffffu:((1u<<n)-1);
  // Selbsttest: Assoziativitaet auf Stichprobe, z zentral
  if(typ!=0){ El z={0,t,0}; for(int a=0;a<2*n;a++)for(int b=0;b<2*n;b++){ El A={ (a/(4*t))%k, (a/2)%(2*t), a&1 }, Bq={ (b/(4*t))%k,(b/2)%(2*t),b&1 }; El C={ ((a+b)*7/(4*t))%k, ((a*3+b)/2)%(2*t),(a+b)&1 };
      if(!eq(mul(mul(A,Bq),C),mul(A,mul(Bq,C)))){fprintf(stderr,"nicht assoziativ\n");return 3;} if(!eq(mul(A,z),mul(z,A))){fprintf(stderr,"z nicht zentral\n");return 3;} } }
  memset(T,0,sizeof T); for(int g=0;g<n;g++){ SG[g]=0; El gi=inv(rep(g)); uint32_t seen=0; for(int h=0;h<n;h++){ int s; int p=repidx(mul(gi,rep(h)),&s); src[g][h]=p; if(seen>>p&1){fprintf(stderr,"keine Bijektion\n");return 3;} seen|=1u<<p; if(s) SG[g]|=1u<<h; } 
    for(int b=0;b<4;b++)for(int v=0;v<256;v++){ uint32_t r=0; for(int h=0;h<n;h++){ int p=src[g][h]; if(p/8==b && (v>>(p%8)&1)) r|=1u<<h; } T[g][b][v]=r; } }
  uint32_t probes[256]; for(int i=0;i<256;i++) probes[i]=rnd()&mask;
  unsigned long long total=1ULL<<(n-1), s1=0,s2=0,s3=0,surv=0,nhad=0;
  if(getenv("DRUCK")){ uint32_t f=(uint32_t)strtoul(getenv("DRUCK"),0,16); mkrows(f); for(int g=0;g<n;g++){ for(int h=0;h<n;h++) printf("%2d ",(rows[g]>>h&1)?-1:1); printf("\n"); } return 0; } clock_t t0=clock(); int abbruch=0; unsigned long long done=0;
  for(unsigned long long ff=0;ff<total;ff++){ uint32_t f=(uint32_t)(ff<<1); // Bit 0 = 0, also f(e) = +1
    if((ff&0xfffff)==0 && (double)(clock()-t0)/CLOCKS_PER_SEC>maxsec){abbruch=1;break;} done++;
    mkrows(f); int found=0; for(int i=0;i<256;i++) if(unc(probes[i])){found=1;break;} if(found) continue; s1++;
    uint32_t x; if(abstieg(&x,R2)) continue; s2++;
    s3++; unsigned long long half=1ULL<<(n-1); for(unsigned long long xx=0;xx<half;xx++) if(unc((uint32_t)xx)){found=1;break;}
    if(found) continue; surv++; { int had=1; for(int a=0;a<n&&had;a++)for(int b=a+1;b<n;b++) if(2*__builtin_popcount(rows[a]^rows[b])!=n){had=0;break;} nhad+=had; if(had&&nhad<=3){ printf("HADAMARD f=%08x\n",f); } } if(surv<=20){ printf("UEBERLEBT f=%08x  Zeile 0:",f); for(int h=0;h<n;h++) printf(" %d",(rows[0]>>h&1)?-1:1); printf("\n"); fflush(stdout); } }
  printf("typ=%d n=%d k=%d t=%d Ziel d=%d: %llu von %llu Funktionen%s; nach Proben uebrig %llu, nach Abstieg %llu, vollstaendig geprueft %llu, UEBERLEBENDE %llu (davon Hadamard %llu); %.1f s\n",
    typ,n,k,t,d,done,total,abbruch?" (ZEITGRENZE, unvollstaendig)":"",s1,s2,s3,surv,nhad,(double)(clock()-t0)/CLOCKS_PER_SEC);
  return 0; }
