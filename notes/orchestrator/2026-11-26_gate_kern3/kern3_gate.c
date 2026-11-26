/* kern3_gate.c -- Gate zu Auftrag K (Orchestrator, 26.11.2026). Pruefcode, nur ganze Zahlen. Kein Code der Sitzung.
   Frage: Gibt es K Nebenklassen des Kerns D = <1,a,b> (Dimension 3) in F_2^m, deren Vereinigung Ueberdeckungsradius <= R hat?
   Typ = Blockgroessen n1 >= n2 >= n3 >= n4 >= 0 (n3 >= 1): die Stellen nach (a_j, b_j); D = alle geraden Umklapp-Muster der vier Bloecke.
   Quotient Q = F_2^m / D: Stellen m-3, m-2, m-1 sind je eine feste Stelle der Bloecke 1, 2, 3; Vertreter = Vektoren mit diesen drei Bits 0,
   also Q = {0, ..., 2^(m-3) - 1} mit XOR. S = Klassen mit Abstand <= R zu D. Nebenklasse c deckt c XOR S.
   Vollstaendige Suche, absichtlich einfach gehalten:
     c_1 = 0 (Verschiebung).  c_2 = ein Vertreter je Bahn unter Vertauschungen INNERHALB der Bloecke (sie lassen D, S und 0 fest);
     Bahn = kleinster Blockgewichtsvektor ueber die 8 Elemente von D. Keine weitere Symmetrie, keine Ordnung der Paare.
     Danach: offenen Punkt p waehlen, ueber alle c mit p in c+S verzweigen; fruehere Geschwister im spaeteren Teilbaum verboten.
     Schranken (r = noch fehlende Nebenklassen, g(c) = |U n (c+S)| fuer ALLE c, per Walsh-Transformation exakt):
       Summe der r groessten g < |U| => Schnitt;  nur c mit g(c) >= |U| - (Summe der r-1 groessten g).
   Aufrufe:
     kern3_gate alle m R K ZEIT          alle Typen, je hoechstens ZEIT Sekunden
     kern3_gate kmin m R KMAX ZEIT       je Typ kleinstes K <= KMAX
     Umgebungsvariable TYPEN=Datei (Zeilen a-b-c-d): nur diese Typen (fuer alle und kmin)
     kern3_gate plant m R n1 n2 n3 n4 r c_1 ... c_K   prueft, dass die K Klassen ueberdecken, streicht die letzten r, sucht r neue (ohne c_1 = 0, ohne Bahnen)
   Bauen: gcc -O3 -march=native -fopenmp -o kern3_gate kern3_gate.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#define QMAX (1 << 15)
#define WMAX (QMAX / 64)
typedef uint64_t u64;
static int m, R, Q, W, n[4]; static uint32_t bm[4];           /* bm: Nichtpivot-Stellen je Block */
static u64 Sbit[WMAX], Slow[64][WMAX]; static int64_t Sw[QMAX]; static int *Sl, nS;
static double tlimit, t0; static volatile int abbruch;
static void fwht(int64_t *a) { for (int h = 1; h < Q; h <<= 1) for (int i = 0; i < Q; i += h << 1) for (int j = i; j < i + h; j++) { int64_t x = a[j], y = a[j + h]; a[j] = x + y; a[j + h] = x - y; } }
static int dist(uint32_t v, int *key) {
    int w[4], best = 1 << 30; int kb[4] = {99, 99, 99, 99};
    for (int j = 0; j < 4; j++) w[j] = __builtin_popcount(v & bm[j]);
    for (int f = 0; f < 16; f++) { if (__builtin_popcount(f) & 1) continue; int s = 0, k[4];
        for (int j = 0; j < 4; j++) { k[j] = (f >> j & 1) ? n[j] - w[j] : w[j]; s += k[j]; }
        if (n[3] == 0 && (f >> 3 & 1)) { /* Block 4 leer: Muster zaehlt trotzdem (Gewicht 0) */ }
        if (s < best) best = s;
        if (memcmp(k, kb, sizeof k) < 0) { int lt = 0; for (int j = 0; j < 4; j++) { if (k[j] < kb[j]) { lt = 1; break; } if (k[j] > kb[j]) break; } if (lt) memcpy(kb, k, sizeof k); } }
    if (key) *key = ((kb[0] * 32 + kb[1]) * 32 + kb[2]) * 32 + kb[3];
    return best;
}
static void setup(int a, int b, int c, int d) {
    n[0] = a; n[1] = b; n[2] = c; n[3] = d; Q = 1 << (m - 3); W = Q / 64; int pos = 0;
    for (int j = 0; j < 4; j++) { bm[j] = 0; int cnt = j < 3 ? n[j] - 1 : n[j]; for (int t = 0; t < cnt; t++) bm[j] |= 1u << pos++; }
    memset(Sbit, 0, sizeof Sbit); nS = 0; free(Sl); Sl = malloc(Q * sizeof(int));
    for (int v = 0; v < Q; v++) { Sw[v] = 0; if (dist(v, 0) <= R) { Sbit[v >> 6] |= 1ull << (v & 63); Sl[nS++] = v; Sw[v] = 1; } }
    fwht(Sw);
    for (int l = 0; l < 64; l++) for (int w = 0; w < W; w++) { u64 x = 0, s = Sbit[w]; for (int i = 0; i < 64; i++) if (s >> i & 1) x |= 1ull << (i ^ l); Slow[l][w] = x; }
}
static inline void minusT(const u64 *U, int c, u64 *V) { const u64 *T = Slow[c & 63]; int h = c >> 6; for (int w = 0; w < W; w++) V[w] = U[w] & ~T[w ^ h]; }
static inline int pc(const u64 *U) { int s = 0; for (int w = 0; w < W; w++) s += __builtin_popcountll(U[w]); return s; }
typedef struct { long long knoten; int8_t *forb; int found; int sol[16]; int K; } St;
static int cmpg; 
static int rec(St *st, const u64 *U, int nU, int r, int tiefe, const int32_t *gpar) {
    st->knoten++;
    if ((st->knoten & 1023) == 0 && omp_get_wtime() - t0 > tlimit) abbruch = 1;
    if (abbruch) return 0;
    if (nU == 0) { st->found = 1; return 1; }
    if (r == 0) return 0;
    int p = -1;
    if (r == 1) {                                   /* eine Nebenklasse muss alles decken */
        for (int w = 0; w < W; w++) if (U[w]) { p = w * 64 + __builtin_ctzll(U[w]); break; }
        for (int i = 0; i < nS; i++) { int c = p ^ Sl[i]; if (st->forb[c] || (gpar && gpar[c] < nU)) continue;
            const u64 *T = Slow[c & 63]; int h = c >> 6, ok = 1; for (int w = 0; w < W; w++) if (U[w] & ~T[w ^ h]) { ok = 0; break; }
            if (ok) { st->sol[tiefe] = c; st->found = 1; return 1; } }
        return 0;
    }
    int64_t *a = malloc(Q * sizeof(int64_t)); int32_t *g = malloc(Q * sizeof(int32_t));
    for (int v = 0; v < Q; v++) a[v] = U[v >> 6] >> (v & 63) & 1;
    fwht(a); for (int v = 0; v < Q; v++) a[v] *= Sw[v]; fwht(a);
    for (int v = 0; v < Q; v++) g[v] = (int32_t)(a[v] >> (m - 3));
    free(a);
    int top[16]; for (int i = 0; i < r; i++) top[i] = 0;          /* r groesste g */
    for (int v = 0; v < Q; v++) { int x = g[v]; if (x > top[r - 1]) { int i = r - 1; while (i > 0 && top[i - 1] < x) { top[i] = top[i - 1]; i--; } top[i] = x; } }
    int sum = 0; for (int i = 0; i < r; i++) sum += top[i];
    if (sum < nU) { free(g); return 0; }
    int thr = nU - (sum - top[r - 1]);
    /* p: unter bis zu 24 offenen Punkten der mit den wenigsten zulaessigen Deckern */
    int bestn = 1 << 30, seen = 0, step = nU / 24 + 1, idx = 0;
    for (int w = 0; w < W && seen < 24; w++) { u64 x = U[w]; while (x) { int v = w * 64 + __builtin_ctzll(x); x &= x - 1;
        if (idx++ % step) continue; seen++; int cnt = 0; for (int i = 0; i < nS && cnt < bestn; i++) { int c = v ^ Sl[i]; if (!st->forb[c] && g[c] >= thr) cnt++; }
        if (cnt < bestn) { bestn = cnt; p = v; } } }
    if (bestn == 0) { free(g); return 0; }
    int *L = malloc(nS * sizeof(int)), nL = 0;
    for (int i = 0; i < nS; i++) { int c = p ^ Sl[i]; if (!st->forb[c] && g[c] >= thr) L[nL++] = c; }
    for (int i = 1; i < nL; i++) { int c = L[i], j = i; while (j > 0 && g[L[j - 1]] < g[c]) { L[j] = L[j - 1]; j--; } L[j] = c; }   /* absteigend nach g */
    u64 V[WMAX]; int res = 0, done = 0;
    for (int i = 0; i < nL && !abbruch; i++) { int c = L[i]; minusT(U, c, V); st->sol[tiefe] = c;
        if (rec(st, V, nU - g[c], r - 1, tiefe + 1, g)) { res = 1; break; }
        st->forb[c]++; done = i + 1; }
    for (int i = 0; i < done; i++) st->forb[L[i]]--;
    free(L); free(g); (void)cmpg; return res;
}
/* Ergebnis: 1 gefunden, 0 keine (vollstaendig), -1 Zeitgrenze */
static int suche(int K, long long *knoten, int *reps, int *sol) {
    u64 U1[WMAX]; for (int w = 0; w < W; w++) U1[w] = ~Sbit[w]; int n1 = Q - nS; *knoten = 0; *reps = 0;
    if (K == 1) return n1 == 0 ? 1 : 0;
    char *seen = calloc(1 << 20, 1); int *rep = malloc(Q * sizeof(int)), nr = 0;
    for (int v = 1; v < Q; v++) { int key; dist(v, &key); if (!seen[key]) { seen[key] = 1; rep[nr++] = v; } }
    free(seen); *reps = nr; int found = 0; long long kn = 0; abbruch = 0; t0 = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic,1) reduction(+:kn)
    for (int i = 0; i < nr; i++) { if (found || abbruch) continue;
        St st; memset(&st, 0, sizeof st); st.forb = calloc(Q, 1); st.sol[0] = 0; st.sol[1] = rep[i];
        u64 U2[WMAX]; minusT(U1, rep[i], U2);
        if (rec(&st, U2, pc(U2), K - 2, 2, 0)) {
            #pragma omp critical
            { if (!found) { found = 1; memcpy(sol, st.sol, sizeof st.sol); } } }
        kn += st.knoten; free(st.forb); }
    free(rep); *knoten = kn; return found ? 1 : (abbruch ? -1 : 0);
}
static int deckt(const int *c, int K) { u64 U[WMAX], V[WMAX]; for (int w = 0; w < W; w++) U[w] = ~0ull; for (int i = 0; i < K; i++) { minusT(U, c[i], V); memcpy(U, V, W * 8); } return pc(U); }
int main(int argc, char **argv) {
    if (argc < 6) return 2;
    m = atoi(argv[2]); R = atoi(argv[3]);
    if (!strcmp(argv[1], "plant")) {
        setup(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7])); int r = atoi(argv[8]), K = argc - 9, c[32];
        for (int i = 0; i < K; i++) c[i] = atoi(argv[9 + i]);
        int offen = deckt(c, K), offen2 = deckt(c, K - r); St st; memset(&st, 0, sizeof st); st.forb = calloc(Q, 1);
        u64 U[WMAX], V[WMAX]; for (int w = 0; w < W; w++) U[w] = ~0ull; for (int i = 0; i < K - r; i++) { minusT(U, c[i], V); memcpy(U, V, W * 8); }
        tlimit = 600; t0 = omp_get_wtime(); abbruch = 0; int e = rec(&st, U, offen2, r, 0, 0);
        printf("Typ (%d,%d,%d,%d) m=%d R=%d: |S|=%d; vorgegebene %d Klassen: offen %d; ohne die letzten %d: offen %d; Suche nach %d neuen: %s, %lld Knoten, %.1f s\n",
               n[0], n[1], n[2], n[3], m, R, nS, K, offen, r, offen2, r, e ? "GEFUNDEN" : (abbruch ? "ZEIT" : "KEINE"), st.knoten, omp_get_wtime() - t0);
        if (e) { int d[32]; for (int i = 0; i < K - r; i++) d[i] = c[i]; for (int i = 0; i < r; i++) d[K - r + i] = st.sol[i]; printf("  gefundene Ergaenzung deckt: offen %d\n", deckt(d, K)); }
        return 0;
    }
    static int filt[256][4]; int nfilt = -1;
    if (getenv("TYPEN")) { FILE *f = fopen(getenv("TYPEN"), "r"); if (!f) return 2; nfilt = 0; while (fscanf(f, "%d-%d-%d-%d", &filt[nfilt][0], &filt[nfilt][1], &filt[nfilt][2], &filt[nfilt][3]) == 4) nfilt++; fclose(f); }
    int K = atoi(argv[4]); tlimit = atof(argv[5]); int kminmode = !strcmp(argv[1], "kmin"); int zahl[4] = {0, 0, 0, 0}, typen = 0; double ges = omp_get_wtime();
    for (int a = m; a >= 1; a--) for (int b = a; b >= 1; b--) for (int c = b; c >= 1; c--) { int d = m - a - b - c; if (d < 0 || d > c) continue;
        if (nfilt >= 0) { int ok = 0; for (int i = 0; i < nfilt; i++) if (filt[i][0] == a && filt[i][1] == b && filt[i][2] == c && filt[i][3] == d) ok = 1; if (!ok) continue; }
        setup(a, b, c, d); typen++; long long kn; int reps, sol[16], e = 0, k; double t = omp_get_wtime();
        if (kminmode) { for (k = 1; k <= K; k++) { e = suche(k, &kn, &reps, sol); if (e) break; }
            if (e == 1) printf("(%d,%d,%d,%d) |S|=%d Kmin=%d\n", a, b, c, d, nS, k); else printf("(%d,%d,%d,%d) |S|=%d %s\n", a, b, c, d, nS, e == 0 ? "Kmin>KMAX" : "ZEIT");
            continue; }
        e = suche(K, &kn, &reps, sol); zahl[e + 1]++;
        printf("(%d,%d,%d,%d) |S|=%d c2-Bahnen %d: %s, %lld Knoten, %.1f s", a, b, c, d, nS, reps, e == 1 ? "GEFUNDEN" : e == 0 ? "KEINE" : "ZEIT", kn, omp_get_wtime() - t);
        if (e == 1) { printf("  Klassen:"); for (int i = 0; i < K; i++) printf(" %d", sol[i]); printf("  offen %d", deckt(sol, K)); }
        printf("\n"); fflush(stdout); }
    if (!kminmode) printf("m=%d R=%d K=%d: %d Typen; KEINE (vollstaendig) %d, GEFUNDEN %d, ZEIT %d; %.0f s\n", m, R, K, typen, zahl[1], zahl[2], zahl[0], omp_get_wtime() - ges);
    return 0;
}
