/* p026 -- Auftrag F. Kopie von p022 (Insel Saat) mit zwei Aenderungen:
 *   1. Startmatrix als Aufrufparameter STARTDATEI statt der festen Datei controls/had24_1p3_paley12.txt.
 *   2. Gestrichene Zeile als Aufrufparameter ZEILE (0-basiert); gestrichen wird genau diese eine Zeile, PAARE = Zeilen der Datei - 1.
 *   Sonst unveraendert (Zuege, Zugwahl, Rauschen 1 %, kein Neustart wie p022/p018).
 *   Kontrolle: p026 20 6 23 S SEK AUS controls/had24_1p3_paley12.txt (S-1)%24 entspricht p022 20 6 23 S SEK AUS.
 * Aufruf: p026 m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI STARTDATEI ZEILE   (Suchcode; liest nur die Startdatei)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define RAUSCHEN 0.01
#define GLATT 0
#define STAG 1e9
#define STOER 6

typedef uint32_t u32;
typedef int32_t v8i __attribute__((vector_size(32)));
#define NV 4                      /* bis m = 32: 4 Vektoren zu 8 Spalten je Wort */

static int m, R, K, nv;
static u32 FULL, TOP;
static u32 *w;
static uint32_t *wt;
static uint16_t *cs;              /* Bits 0-7: Zahl der Ueberdecker; Bits 8-15: XOR von (Wortnummer+1) */
static v8i *MK, *BK;              /* je Wort NV Vektoren */
static int64_t *tabu;
static u32 *openl; static int32_t *opos; static long nopen;
static u32 **mRnoj; static long nRnoj;
static u32 *mR, *mR1; static long nR, nR1;
static u32 *mBall; static long nBall;
static v8i SH[NV];

static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs >> 12; rs ^= rs << 25; rs ^= rs >> 27; return rs * 2685821657736338717ULL; }
static inline double rndu(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline u32 canon(u32 x) { return (x & TOP) ? (x ^ FULL) : x; }
static inline int32_t mk(int i, int j) { return ((int32_t *)&MK[i * NV])[j]; }
static inline int32_t bk(int i, int j) { return ((int32_t *)&BK[i * NV])[j]; }

static inline void addbits(v8i *row, u32 bits, int32_t d) {
    v8i b = (v8i){0} + (int32_t)bits;
    for (int v = 0; v < nv; v++) row[v] += ((b >> SH[v]) & 1) * d;
}
static inline void open_add(u32 c) { opos[c] = nopen; openl[nopen++] = c; }
static inline void open_del(u32 c) { long p = opos[c]; u32 l = openl[--nopen]; openl[p] = l; opos[l] = p; opos[c] = -1; }

static inline void make_andere(u32 c, int ohne, int32_t d) {
    for (int i = 0; i < K; i++) {
        if (i == ohne) continue;
        u32 x = c ^ w[i]; int dd = __builtin_popcount(x);
        if (dd == R + 1) addbits(&MK[i * NV], x, d);
        else if (m - dd == R + 1) addbits(&MK[i * NV], x ^ FULL, d);
    }
}
static inline void break_von(u32 c, int u, int32_t d) {  /* u ist einziger Ueberdecker von c */
    u32 x = c ^ w[u]; int dd = __builtin_popcount(x);
    if (dd == R) addbits(&BK[u * NV], x ^ FULL, d);
    else if (m - dd == R) addbits(&BK[u * NV], x, d);
}
static void zeile_neu(int i) {
    for (int v = 0; v < NV; v++) { MK[i * NV + v] = (v8i){0}; BK[i * NV + v] = (v8i){0}; }
    u32 wi = w[i];
    for (long t = 0; t < nR; t++) { u32 c = canon(wi ^ mR[t]); if ((cs[c] & 255) == 1) addbits(&BK[i * NV], mR[t] ^ FULL, (int32_t)wt[c]); }
    for (long t = 0; t < nR1; t++) { u32 c = canon(wi ^ mR1[t]); if ((cs[c] & 255) == 0) addbits(&MK[i * NV], mR1[t], (int32_t)wt[c]); }
}
static void kippe(int i, int j) {
    u32 wo = w[i], wn = wo ^ (1u << j);
    uint16_t tag = (uint16_t)((i + 1) << 8);
    u32 *L = mRnoj[j];
    for (long t = 0; t < nRnoj; t++) {
        u32 c = canon(wo ^ L[t]);
        uint16_t v = (uint16_t)((cs[c] - 1) ^ tag); cs[c] = v;
        int n = v & 255;
        if (n == 0) { open_add(c); make_andere(c, i, (int32_t)wt[c]); }
        else if (n == 1) break_von(c, (v >> 8) - 1, (int32_t)wt[c]);
    }
    for (long t = 0; t < nRnoj; t++) {
        u32 c = canon(wn ^ L[t]);
        uint16_t v = cs[c]; int n = v & 255;
        if (n == 0) { open_del(c); make_andere(c, i, -(int32_t)wt[c]); }
        else if (n == 1) break_von(c, (v >> 8) - 1, -(int32_t)wt[c]);
        cs[c] = (uint16_t)((v + 1) ^ tag);
    }
    w[i] = wn;
    zeile_neu(i);
}
static void alles_neu(void) {
    long N = (long)TOP;
    memset(cs, 0, N * 2);
    for (int i = 0; i < K; i++) for (long t = 0; t < nBall; t++) { u32 c = canon(w[i] ^ mBall[t]); cs[c] = (uint16_t)((cs[c] + 1) ^ ((i + 1) << 8)); }
    nopen = 0;
    for (long c = 0; c < N; c++) { opos[c] = -1; if (!(cs[c] & 255)) open_add((u32)c); }
    for (int i = 0; i < K; i++) zeile_neu(i);
}
static void schreibe(const char *pfad, const u32 *bw) {
    char tmp[4096]; snprintf(tmp, sizeof tmp, "%s.tmp", pfad);
    FILE *f = fopen(tmp, "w"); if (!f) return;
    for (int i = 0; i < K; i++) { for (int j = 0; j < m; j++) fprintf(f, "%s", (bw[i] >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    fclose(f); rename(tmp, pfad);
}

int main(int argc, char **argv) {
    if (argc != 9) { fprintf(stderr, "Aufruf: %s m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI STARTDATEI ZEILE\n", argv[0]); return 2; }
    const char *SAAT = argv[7]; int zeile = atoi(argv[8]);
    m = atoi(argv[1]); R = atoi(argv[2]); K = atoi(argv[3]);
    uint64_t seed = strtoull(argv[4], 0, 10); double sek = atof(argv[5]); const char *aus = argv[6];
    if (m < 4 || m > 30 || 2 * (R + 1) >= m || K < 1 || K > 254) { fprintf(stderr, "Parameter ungueltig\n"); return 2; }
    double t0 = jetzt();
    rs = 0x9E3779B97F4A7C15ULL ^ (seed * 0xD1B54A32D192ED03ULL); for (int q = 0; q < 10; q++) rnd();
    FULL = (1u << m) - 1; TOP = 1u << (m - 1); nv = (m + 7) / 8;
    for (int v = 0; v < NV; v++) for (int l = 0; l < 8; l++) SH[v][l] = 8 * v + l;
    long N = (long)TOP;
    cs = calloc(N, 2); wt = malloc(N * 4); for (long c = 0; c < N; c++) wt[c] = 1; opos = malloc(N * sizeof(int32_t)); openl = malloc(N * sizeof(u32));
    w = malloc(K * sizeof(u32));
    MK = aligned_alloc(32, K * NV * sizeof(v8i)); BK = aligned_alloc(32, K * NV * sizeof(v8i));
    tabu = calloc(K * m, sizeof(int64_t));
    nR = nR1 = nBall = 0;
    for (u32 x = 0; x <= FULL; x++) { int p = __builtin_popcount(x); if (p == R) nR++; if (p == R + 1) nR1++; if (p <= R) nBall++; }
    mR = malloc(nR * 4); mR1 = malloc(nR1 * 4); mBall = malloc(nBall * 4);
    long a = 0, b = 0, e = 0;
    for (u32 x = 0; x <= FULL; x++) { int p = __builtin_popcount(x); if (p == R) mR[a++] = x; if (p == R + 1) mR1[b++] = x; if (p <= R) mBall[e++] = x; }
    mRnoj = malloc(m * sizeof(u32 *));
    nRnoj = 0; for (long t = 0; t < nR; t++) if (!(mR[t] & 1u)) nRnoj++;
    for (int j = 0; j < m; j++) { mRnoj[j] = malloc(nRnoj * 4); long q = 0; for (long t = 0; t < nR; t++) if (!(mR[t] >> j & 1)) mRnoj[j][q++] = mR[t]; }

    /* Saat lesen */
    u32 saat[64]; int ns = 0;
    { FILE *f = fopen(SAAT, "r"); if (!f) { fprintf(stderr, "Saat nicht lesbar\n"); return 2; }
      char z[4096];
      while (fgets(z, sizeof z, f) && ns < 64) { char *p = z; u32 x = 0; int j = 0; long v;
        while (j < 64) { char *q; v = strtol(p, &q, 10); if (q == p) break; p = q; if (j < m && v == -1) x |= 1u << j; j++; }
        if (j >= m) saat[ns++] = x; }
      fclose(f); }
    if (K != ns - 1 || zeile < 0 || zeile >= ns) { fprintf(stderr, "Saat hat %d Zeilen, PAARE %d / ZEILE %d nicht moeglich\n", ns, K, zeile); return 2; }
    int weg[64] = {0}, nweg = 0;
    weg[zeile] = 1; nweg = 1;
    u32 start[64]; { int q = 0; for (int r = 0; r < ns; r++) if (!weg[r]) start[q++] = saat[r]; }
    memcpy(w, start, K * sizeof(u32));
    alles_neu();
    fprintf(stderr, "Saat: %d Zeilen gestrichen, Start offen=%ld\n", nweg, nopen);
    u32 *best = malloc(K * sizeof(u32)); memcpy(best, w, K * sizeof(u32));
    long bestopen = nopen; double tbest = jetzt(), tschreib = 0, tlog = jetzt();
    schreibe(aus, best);
    int64_t it = 0, nbump = 0, nneu = 0;
    long lokalbest = nopen; double tlokal = jetzt();
    int cand_i[4096], cand_j[4096];
    while (nopen > 0) {
        it++;
        if ((it & 255) == 0) {
            double t = jetzt();
            if (t - t0 > sek) break;
            if (t - tlokal > STAG) {   /* Neustart vom Bestpunkt mit Stoerung */
                memcpy(w, best, K * sizeof(u32));
                for (int q = 0; q < STOER; q++) w[rnd() % K] ^= 1u << (rnd() % m);
                for (long q = 0; q < N; q++) wt[q] = 1;
                memset(tabu, 0, K * m * sizeof(int64_t));
                alles_neu();
                lokalbest = nopen; tlokal = t; nneu++;
            }
            if (t - tlog > 10) { fprintf(stderr, "t=%.0f it=%lld offen=%ld lokal=%ld best=%ld neustarts=%lld\n", t - t0, (long long)it, nopen, lokalbest, bestopen, (long long)nneu); tlog = t; }
        }
        u32 c = openl[rnd() % nopen];
        int nc = 0, Dmin = 1 << 30;
        for (int i = 0; i < K; i++) {
            u32 x = c ^ w[i]; int d = __builtin_popcount(x); int D = d < m - d ? d : m - d;
            if (D < Dmin) { Dmin = D; nc = 0; }
            if (D == Dmin) { u32 bits = (d <= m - d) ? x : (x ^ FULL);
                while (bits) { int j = __builtin_ctz(bits); bits &= bits - 1; cand_i[nc] = i; cand_j[nc] = j; nc++; } }
        }
        int wi = -1, wj = -1;
        if (rndu() < RAUSCHEN) { int q = rnd() % nc; wi = cand_i[q]; wj = cand_j[q]; }
        else {
            int32_t bs = INT32_MAX; int ties = 0;
            for (int i = 0; i < K; i++) for (int j = 0; j < m; j++) {
                int32_t s = bk(i, j) - mk(i, j);
                if (tabu[i * m + j] > it) continue;
                if (s < bs) { bs = s; wi = i; wj = j; ties = 1; }
                else if (s == bs) { ties++; if (rnd() % ties == 0) { wi = i; wj = j; } }
            }
            if (wi < 0) { int q = rnd() % nc; wi = cand_i[q]; wj = cand_j[q]; }
            if (bs >= 0) {   /* kein gewichteter Gewinn: offene Klassen schwerer */
                for (long q = 0; q < nopen; q++) { u32 o = openl[q]; wt[o]++; make_andere(o, -1, 1); }
                ++nbump;
                if (GLATT && nbump % GLATT == 0) {
                    for (long q = 0; q < N; q++) if (wt[q] > 1) wt[q]--;
                    for (int i = 0; i < K; i++) zeile_neu(i);
                }
            }
        }
        kippe(wi, wj);
        tabu[wi * m + wj] = it + 6 + (long)(rnd() % 4);
#ifdef PRUEF
        if (it % 997 == 0) {
            long n0 = nopen; v8i *a1 = malloc(K * NV * 32), *a2 = malloc(K * NV * 32); uint16_t *c0 = malloc(N * 2);
            memcpy(a1, MK, K * NV * 32); memcpy(a2, BK, K * NV * 32); memcpy(c0, cs, N * 2);
            alles_neu();
            if (n0 != nopen || memcmp(c0, cs, N * 2) || memcmp(a1, MK, K * NV * 32) || memcmp(a2, BK, K * NV * 32)) { fprintf(stderr, "PRUEF FEHLER it=%lld\n", (long long)it); return 3; }
            free(a1); free(a2); free(c0);
        }
#endif
        if (nopen < lokalbest) { lokalbest = nopen; tlokal = jetzt(); }
        if (nopen < bestopen) {
            bestopen = nopen; memcpy(best, w, K * sizeof(u32)); tbest = jetzt();
            if (tbest - tschreib > 1.0 || nopen == 0) { schreibe(aus, best); tschreib = tbest; }
        }
    }
    schreibe(aus, best);
    fprintf(stderr, "ENDE best=%ld it=%lld t_best=%.1f t=%.1f bumps=%lld neustarts=%lld\n", bestopen, (long long)it, tbest - t0, jetzt() - t0, (long long)nbump, (long long)nneu);
    return 0;
}
