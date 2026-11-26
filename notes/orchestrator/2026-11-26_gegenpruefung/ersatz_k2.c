/* ersatz_k2.c -- Gegenpruefung 26.11.2026 (Orchestrator), Vorschlag von Sol nachgerechnet; ein Programm, nicht gegengerechnet.
   Hadamard-Matrix H (Ordnung N = 0 mod 4, N <= 32), zwei Spalten a < b durch BELIEBIGE Spalten v, w ersetzt. Ziel disc >= Z (N = 20: Z = 8).
   Notwendige Bedingung: Fuer jedes x'' (uebrige N-2 Spalten) mit max|b_i| <= Z-2 und jedes (e1,e2) braucht es eine Zeile mit |b_i| = Z-2 und
   (e1 v_i, e2 w_i) = sgn(b_i)(1,1). Also muessen die Werte sgn(b_i)(v_i,w_i) auf der kritischen Menge S = {i: |b_i| = Z-2} alle vier Vorzeichenpaare annehmen.
   Die kritischen Mengen liegen je in einer der zwei Zeilengruppen g_i = H[i][a]*H[i][b] = +-1 (wird mitgezaehlt), die Gruppen entkoppeln.
   Je Gruppe: alle 4^(N/2) Belegungen (erste Zeile der Gruppe fest) gegen alle Bedingungen. Hat eine Gruppe keine Belegung, ist das Spaltenpaar ausgeschlossen.
   Aufruf: ersatz_k2 DATEI N Z [SCHRITT [LAUT [MINDESTGROESSE]]]
   Ausgabe: je Matrix Zusammenfassung; am Ende Zahl der Paare, ausgeschlossene Paare, Paare mit ueberlebenden Belegungen in beiden Gruppen. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
static int N; static int8_t M[32][32];
typedef struct { uint32_t m,s; } Bed;
static int cmpb(const void*a,const void*b){ const Bed*x=a,*y=b; int px=__builtin_popcount(x->m),py=__builtin_popcount(y->m); if(px!=py) return px-py; if(x->m!=y->m) return x->m<y->m?-1:1; return x->s<y->s?-1:x->s>y->s; }
static Bed *B; static long cap=1<<22;
static long ueberlebende(const int*rows,int nr,const Bed*C,long nc,uint32_t*bsp){ /* Belegungen val[k] in 0..3 fuer rows[k]; val[0] = 0 fest */
    /* Bedingungen auf Gruppenindizes umrechnen */
    static uint16_t cm[1<<20], cs[1<<20]; long n=0; int pos[32]; for(int i=0;i<32;i++) pos[i]=-1; for(int k=0;k<nr;k++) pos[rows[k]]=k;
    for(long c=0;c<nc;c++){ uint16_t m=0,s=0; for(int i=0;i<N;i++) if(C[c].m>>i&1){ m|=1u<<pos[i]; if(C[c].s>>i&1) s|=1u<<pos[i]; } cm[n]=m; cs[n]=s; n++; }
    long gut=0; uint64_t total=1ULL<<(2*(nr-1));
    for(uint64_t t=0;t<total;t++){ int val[16]; val[0]=0; for(int k=1;k<nr;k++) val[k]=(t>>(2*(k-1)))&3;
        long c; for(c=0;c<n;c++){ int or=0; uint16_t m=cm[c]; while(m){ int k=__builtin_ctz(m); m&=m-1; or|=1<<(val[k]^((cs[c]>>k&1)?3:0)); } if(or!=15) break; }
        if(c==n){ gut++; if(bsp) *bsp=(uint32_t)t; } }
    return gut; }
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); N=atoi(argv[2]); int Z=atoi(argv[3]); long schritt=argc>4?atol(argv[4]):1; int laut=argc>5?atoi(argv[5]):0; int mingr=argc>6?atoi(argv[6]):0; /* laut=1: je Paar beide Gruppen ausgeben; mingr: nur Bedingungen mit |S| >= mingr (nur zur Kontrolle der Aufzaehlung) */ B=malloc(sizeof(Bed)*cap);
    long paare=0, aus=0, offen=0, zaehler=0, gemischt=0; int mi=0; long minS=99, maxbed=0;
    for(;;){ int ok=1; for(int i=0;i<N&&ok;i++) for(int j=0;j<N;j++){ int v; if(fscanf(f,"%d",&v)!=1){ ok=0; break; } M[i][j]=(int8_t)v; } if(!ok) break;
        long maus=0, moffen=0, mp=0;
        for(int a=0;a<N;a++) for(int b=a+1;b<N;b++){ if(zaehler++%schritt) continue;
            int cols[32],m=0; for(int j=0;j<N;j++) if(j!=a&&j!=b) cols[m++]=j;
            int s[32]; int x[32]; for(int i=0;i<N;i++){ s[i]=0; for(int j=0;j<m;j++) s[i]+=M[i][cols[j]]; } for(int j=0;j<m;j++) x[j]=1;
            long nb=0; uint64_t total=1ULL<<(m-1);
            for(uint64_t g=0;;){ int mx=0; for(int i=0;i<N;i++){ int v=s[i]<0?-s[i]:s[i]; if(v>mx) mx=v; }
                if(mx<=Z-2){ uint32_t mk=0,sg=0; for(int i=0;i<N;i++) if(s[i]==Z-2||s[i]==-(Z-2)){ mk|=1u<<i; if(s[i]<0) sg|=1u<<i; }
                    uint32_t alt=(~sg)&mk; if(alt<sg) sg=alt; /* x'' -> -x'' gibt dieselbe Bedingung (alle vier Werte gespiegelt) */
                    if(nb<cap){ B[nb].m=mk; B[nb].s=sg; } nb++; int p=__builtin_popcount(mk); if(p<minS) minS=p; }
                if(++g==total) break; int j=__builtin_ctzll(g)+1; if(x[j]>0){ for(int i=0;i<N;i++) s[i]-=2*M[i][cols[j]]; x[j]=-1; } else { for(int i=0;i<N;i++) s[i]+=2*M[i][cols[j]]; x[j]=1; } }
            if(nb>cap){ fprintf(stderr,"zu viele Bedingungen\n"); return 3; }
            qsort(B,nb,sizeof(Bed),cmpb); long u=0; for(long k=0;k<nb;k++) if(k==0||B[k].m!=B[u-1].m||B[k].s!=B[u-1].s) B[u++]=B[k]; if(u>maxbed) maxbed=u;
            int rows[2][32],nr[2]={0,0}; uint32_t gm[2]={0,0}; for(int i=0;i<N;i++){ int g=(M[i][a]*M[i][b]>0)?0:1; rows[g][nr[g]++]=i; gm[g]|=1u<<i; }
            static Bed C[2][1<<20]; long nc[2]={0,0}; for(long k=0;k<u;k++){ if(__builtin_popcount(B[k].m)<mingr) continue; if((B[k].m&gm[0])==B[k].m) C[0][nc[0]++]=B[k]; else if((B[k].m&gm[1])==B[k].m) C[1][nc[1]++]=B[k]; else gemischt++; }
            long g0=ueberlebende(rows[0],nr[0],C[0],nc[0],NULL); long g1=(g0||laut)?ueberlebende(rows[1],nr[1],C[1],nc[1],NULL):-1;
            if(laut) printf("mat %d Spalten %d,%d: flache x'' %ld, Bedingungen %ld/%ld, Belegungen %ld/%ld\n",mi,a,b,nb,nc[0],nc[1],g0,g1);
            paare++; mp++; if(g0==0||g1==0){ aus++; maus++; } else { offen++; moffen++; printf("OFFEN mat %d Spalten %d,%d: Bedingungen %ld/%ld, Belegungen Gruppe 0: %ld, Gruppe 1: %ld\n",mi,a,b,nc[0],nc[1],g0,g1); }
        }
        printf("mat %d: %ld Paare, ausgeschlossen %ld, offen %ld\n",mi,mp,maus,moffen); fflush(stdout); mi++; }
    printf("N=%d Ziel %d: %ld Spaltenpaare; ausgeschlossen (eine Gruppe ohne Belegung) %ld; offen %ld; kleinste kritische Menge %ld; Bedingungen ueber beide Gruppen %ld; meiste Bedingungen je Paar %ld\n",N,Z,paare,aus,offen,minS,gemischt,maxbed); return 0; }
