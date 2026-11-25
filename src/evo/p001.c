/* p001 -- Insel A (Zuege auf Wortebene). Elter: keiner (Wurzel der Insel).
 * Geaendert: erste Fassung. Offene Klasse zufaellig waehlen, ein Wort mit Paarabstand R+1 per Bitwechsel heranziehen
 *   (sonst das naechste Wort einen Schritt naeher); Zaehler je Klasse fortgeschrieben; Make/Break je Zug exakt; Tabu 6-9; ohne Gewichte.
 * Warum: Grundprogramm laut Auftrag; Vergleichsbasis fuer die Fassung mit Punktgewichten.
 * Aufruf: p001 m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI   (Suchcode; blind, liest keine Dateien)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef GEWICHTE
#define GEWICHTE 0
#endif
#define RAUSCHEN 0.05      /* Anteil zufaelliger Zuege unter den Kandidaten */

typedef uint32_t u32;
typedef int64_t i64;

static int m, R, K;
static u32 FULL, TOP;
static u32 *w;
static uint8_t *cnt;
static u32 *wt;
static i64 *mkw, *bkw;       /* gewichtet */
static int32_t *mk1, *bk1;   /* ungewichtet (wahre Aenderung der Zahl offener Klassen) */
static int64_t *tabu;
static u32 *openl; static int32_t *opos; static long nopen;
static i64 Wopen;            /* Summe der Gewichte offener Klassen */
static u32 **mRnoj; static long nRnoj;
static u32 *mR, *mR1; static long nR, nR1;
static u32 *mBall; static long nBall;

static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs >> 12; rs ^= rs << 25; rs ^= rs >> 27; return rs * 2685821657736338717ULL; }
static inline double rndu(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }

static inline u32 canon(u32 x) { return (x & TOP) ? (x ^ FULL) : x; }

static inline void open_add(u32 c) { opos[c] = nopen; openl[nopen++] = c; Wopen += wt[c]; }
static inline void open_del(u32 c) { long p = opos[c]; u32 l = openl[--nopen]; openl[p] = l; opos[l] = p; opos[c] = -1; Wopen -= wt[c]; }

/* Make-Beitrag der Klasse c fuer alle Woerter ausser 'ohne' (Paarabstand R+1). */
static inline void make_andere(u32 c, int ohne, i64 dw, int d1) {
    for (int i = 0; i < K; i++) {
        if (i == ohne) continue;
        u32 x = c ^ w[i];
        int d = __builtin_popcount(x);
        u32 bits;
        if (d == R + 1) bits = x; else if (m - d == R + 1) bits = x ^ FULL; else continue;
        while (bits) { int j = __builtin_ctz(bits); bits &= bits - 1; mkw[i * m + j] += dw; mk1[i * m + j] += d1; }
    }
}
/* Break-Beitrag fuer den einzigen Ueberdecker (ausser 'ohne'), falls dessen Paarabstand genau R ist. */
static inline void break_einziger(u32 c, int ohne, i64 dw, int d1) {
    for (int i = 0; i < K; i++) {
        if (i == ohne) continue;
        u32 x = c ^ w[i];
        int d = __builtin_popcount(x);
        int D = d < m - d ? d : m - d;
        if (D <= R) {
            if (D == R) {
                u32 bits = (d == R) ? (x ^ FULL) : x;   /* uebereinstimmende Bits zur naeheren Ausrichtung */
                while (bits) { int j = __builtin_ctz(bits); bits &= bits - 1; bkw[i * m + j] += dw; bk1[i * m + j] += d1; }
            }
            return;
        }
    }
}
static void zeile_neu(int i) {
    for (int j = 0; j < m; j++) { mkw[i * m + j] = 0; bkw[i * m + j] = 0; mk1[i * m + j] = 0; bk1[i * m + j] = 0; }
    u32 wi = w[i];
    for (long t = 0; t < nR; t++) {
        u32 c = canon(wi ^ mR[t]);
        if (cnt[c] == 1) { u32 bits = mR[t] ^ FULL; i64 g = wt[c];
            while (bits) { int j = __builtin_ctz(bits); bits &= bits - 1; bkw[i * m + j] += g; bk1[i * m + j]++; } }
    }
    for (long t = 0; t < nR1; t++) {
        u32 c = canon(wi ^ mR1[t]);
        if (cnt[c] == 0) { u32 bits = mR1[t]; i64 g = wt[c];
            while (bits) { int j = __builtin_ctz(bits); bits &= bits - 1; mkw[i * m + j] += g; mk1[i * m + j]++; } }
    }
}
static void kippe(int i, int j) {
    u32 wo = w[i], wn = wo ^ (1u << j);
    u32 *L = mRnoj[j];
    for (long t = 0; t < nRnoj; t++) {          /* verlassende Klassen */
        u32 c = canon(wo ^ L[t]);
        uint8_t v = --cnt[c];
        if (v == 0) { open_add(c); make_andere(c, i, wt[c], 1); }
        else if (v == 1) break_einziger(c, i, wt[c], 1);
    }
    for (long t = 0; t < nRnoj; t++) {          /* hinzukommende Klassen */
        u32 c = canon(wn ^ L[t]);
        uint8_t v = cnt[c]++;
        if (v == 0) { open_del(c); make_andere(c, i, -(i64)wt[c], -1); }
        else if (v == 1) break_einziger(c, i, -(i64)wt[c], -1);
    }
    w[i] = wn;
    zeile_neu(i);
}
static void alles_neu(void) {
    long N = (long)TOP;
    memset(cnt, 0, N);
    for (int i = 0; i < K; i++) for (long t = 0; t < nBall; t++) cnt[canon(w[i] ^ mBall[t])]++;
    nopen = 0; Wopen = 0;
    for (long c = 0; c < N; c++) { opos[c] = -1; if (!cnt[c]) open_add((u32)c); }
    for (int i = 0; i < K; i++) zeile_neu(i);
}
static void schreibe(const char *pfad, const u32 *bw) {
    char tmp[4096]; snprintf(tmp, sizeof tmp, "%s.tmp", pfad);
    FILE *f = fopen(tmp, "w"); if (!f) return;
    for (int i = 0; i < K; i++) { for (int j = 0; j < m; j++) fprintf(f, "%s", (bw[i] >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    fclose(f); rename(tmp, pfad);
}

int main(int argc, char **argv) {
    if (argc != 7) { fprintf(stderr, "Aufruf: %s m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI\n", argv[0]); return 2; }
    m = atoi(argv[1]); R = atoi(argv[2]); K = atoi(argv[3]);
    uint64_t seed = strtoull(argv[4], 0, 10); double sek = atof(argv[5]); const char *aus = argv[6];
    if (m < 4 || m > 30 || 2 * (R + 1) >= m || K < 1 || K > 255) { fprintf(stderr, "Parameter ungueltig\n"); return 2; }
    double t0 = jetzt();
    rs = 0x9E3779B97F4A7C15ULL ^ (seed * 0xD1B54A32D192ED03ULL); for (int q = 0; q < 10; q++) rnd();
    FULL = (m == 32) ? 0xFFFFFFFFu : ((1u << m) - 1); TOP = 1u << (m - 1);
    long N = (long)TOP;
    cnt = calloc(N, 1); wt = malloc(N * sizeof(u32)); opos = malloc(N * sizeof(int32_t)); openl = malloc(N * sizeof(u32));
    for (long c = 0; c < N; c++) wt[c] = 1;
    w = malloc(K * sizeof(u32));
    mkw = calloc(K * m, sizeof(i64)); bkw = calloc(K * m, sizeof(i64)); mk1 = calloc(K * m, 4); bk1 = calloc(K * m, 4);
    tabu = calloc(K * m, sizeof(int64_t));
    /* Masken */
    nR = nR1 = nBall = 0;
    for (u32 x = 0; x <= FULL; x++) { int p = __builtin_popcount(x); if (p == R) nR++; if (p == R + 1) nR1++; if (p <= R) nBall++; if (x == FULL) break; }
    mR = malloc(nR * 4); mR1 = malloc(nR1 * 4); mBall = malloc(nBall * 4);
    long a = 0, b = 0, e = 0;
    for (u32 x = 0; x <= FULL; x++) { int p = __builtin_popcount(x); if (p == R) mR[a++] = x; if (p == R + 1) mR1[b++] = x; if (p <= R) mBall[e++] = x; if (x == FULL) break; }
    mRnoj = malloc(m * sizeof(u32 *));
    nRnoj = 0; for (long t = 0; t < nR; t++) if (!(mR[t] & 1u)) nRnoj++;
    for (int j = 0; j < m; j++) { mRnoj[j] = malloc(nRnoj * 4); long q = 0; for (long t = 0; t < nR; t++) if (!(mR[t] >> j & 1)) mRnoj[j][q++] = mR[t]; }

    for (int i = 0; i < K; i++) w[i] = (u32)rnd() & FULL;
    alles_neu();
    u32 *best = malloc(K * sizeof(u32)); memcpy(best, w, K * sizeof(u32));
    long bestopen = nopen; double tbest = jetzt(), tschreib = 0, tlog = jetzt();
    schreibe(aus, best);
    int64_t it = 0;
    int cand_i[4096], cand_j[4096];
    while (nopen > 0) {
        it++;
        if ((it & 255) == 0) {
            double t = jetzt();
            if (t - t0 > sek) break;
            if (t - tlog > 10) { fprintf(stderr, "t=%.0f it=%lld offen=%ld best=%ld W=%lld\n", t - t0, (long long)it, nopen, bestopen, (long long)Wopen); tlog = t; }
        }
        u32 c = openl[rnd() % nopen];
        /* Kandidaten: Woerter mit Paarabstand R+1 (alle abweichenden Bits), sonst minimaler Abstand */
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
            i64 bs = INT64_MAX; int ties = 0;
            for (int q = 0; q < nc; q++) {
                int i = cand_i[q], j = cand_j[q], ix = i * m + j;
                i64 s = bkw[ix] - mkw[ix];
                int erlaubt = tabu[ix] <= it || (nopen + bk1[ix] - mk1[ix] < bestopen);
                if (!erlaubt) continue;
                if (s < bs) { bs = s; wi = i; wj = j; ties = 1; }
                else if (s == bs) { ties++; if (rnd() % ties == 0) { wi = i; wj = j; } }
            }
            if (wi < 0) { int q = rnd() % nc; wi = cand_i[q]; wj = cand_j[q]; }
#if GEWICHTE
            if (bs >= 0) {  /* kein gewichteter Gewinn: Gewichte offener Klassen erhoehen */
                for (long q = 0; q < nopen; q++) { u32 o = openl[q]; wt[o]++; Wopen++; make_andere(o, -1, 1, 0); }
            }
#endif
        }
        kippe(wi, wj);
        tabu[wi * m + wj] = it + 6 + (long)(rnd() % 4);
#ifdef PRUEF
        if (it % 997 == 0) {   /* Selbsttest: fortgeschriebene Groessen gegen Neuberechnung */
            long n0 = nopen; i64 W0 = Wopen; i64 *a1 = malloc(K * m * 8), *a2 = malloc(K * m * 8); int32_t *a3 = malloc(K * m * 4), *a4 = malloc(K * m * 4);
            memcpy(a1, mkw, K * m * 8); memcpy(a2, bkw, K * m * 8); memcpy(a3, mk1, K * m * 4); memcpy(a4, bk1, K * m * 4);
            uint8_t *c0 = malloc(N); memcpy(c0, cnt, N);
            alles_neu();
            if (n0 != nopen || W0 != Wopen || memcmp(c0, cnt, N) || memcmp(a1, mkw, K * m * 8) || memcmp(a2, bkw, K * m * 8) || memcmp(a3, mk1, K * m * 4) || memcmp(a4, bk1, K * m * 4)) {
                fprintf(stderr, "PRUEF FEHLER it=%lld\n", (long long)it); return 3; }
            free(a1); free(a2); free(a3); free(a4); free(c0);
        }
#endif
        if (nopen < bestopen) {
            bestopen = nopen; memcpy(best, w, K * sizeof(u32)); tbest = jetzt();
            if (tbest - tschreib > 1.0 || nopen == 0) { schreibe(aus, best); tschreib = tbest; }
        }
    }
    schreibe(aus, best);
    fprintf(stderr, "ENDE best=%ld it=%lld t_best=%.1f t=%.1f\n", bestopen, (long long)it, tbest - t0, jetzt() - t0);
    return 0;
}
