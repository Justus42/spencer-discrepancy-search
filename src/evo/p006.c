/* p006 -- Insel C (freie Erfindung, kein vorhandener Suchcode). Elter: keiner (Wurzel der Insel).
 * Zug: eine zufaellige Zeile entfernen und durch die global beste Zeile ersetzen; Gewinn aller 2^(m-1) Kandidaten zugleich
 *   per Walsh-Hadamard-Faltung (gewichtete offene Klassen * Kugel); Tabu auf eben entfernte Werte; Gewichte offener Klassen +1,
 *   wenn ein Zug nicht verbessert.
 * Warum: groessere Nachbarschaft als ein Bitwechsel (jede Zeile auf einmal exakt optimal), andere Suchlandschaft als Insel A.
 * Aufruf: p006 m R PAARE STARTWERT SEKUNDEN AUSGABEDATEI   (Suchcode; blind, liest keine Dateien)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef uint32_t u32;
typedef int64_t i64;

static int m, R, K;
static u32 FULL, TOP;
static long N;
static u32 *w;
static uint8_t *cnt;
static i64 *wt;
static i64 *Bh, *G;
static u32 *mBall; static long nBall;
static long nopen;

static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs >> 12; rs ^= rs << 25; rs ^= rs >> 27; return rs * 2685821657736338717ULL; }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline u32 canon(u32 x) { return (x & TOP) ? (x ^ FULL) : x; }

static void wht(i64 *a) {
    long h = 1;
    if (N >= 4) {   /* h = 1 und 2 zusammen */
        for (long s = 0; s < N; s += 4) {
            i64 x0 = a[s], x1 = a[s + 1], x2 = a[s + 2], x3 = a[s + 3];
            i64 y0 = x0 + x1, y1 = x0 - x1, y2 = x2 + x3, y3 = x2 - x3;
            a[s] = y0 + y2; a[s + 1] = y1 + y3; a[s + 2] = y0 - y2; a[s + 3] = y1 - y3;
        }
        h = 4;
    }
    for (; h < N; h <<= 1)
        for (long s = 0; s < N; s += 2 * h)
            for (long t = s; t < s + h; t++) { i64 x = a[t], y = a[t + h]; a[t] = x + y; a[t + h] = x - y; }
}
static void setze(u32 v, int d) {   /* Kugel um Paar v ein-/austragen */
    for (long t = 0; t < nBall; t++) {
        u32 c = canon(v ^ mBall[t]);
        if (d > 0) { if (cnt[c]++ == 0) nopen--; }
        else { if (--cnt[c] == 0) nopen++; }
    }
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
    if (m < 4 || m > 28 || 2 * R >= m || K < 1 || K > 255) { fprintf(stderr, "Parameter ungueltig\n"); return 2; }
    double t0 = jetzt();
    rs = 0x9E3779B97F4A7C15ULL ^ (seed * 0xD1B54A32D192ED03ULL); for (int q = 0; q < 10; q++) rnd();
    FULL = (1u << m) - 1; TOP = 1u << (m - 1); N = (long)TOP;
    cnt = calloc(N, 1); wt = malloc(N * 8); Bh = malloc(N * 8); G = malloc(N * 8);
    for (long c = 0; c < N; c++) wt[c] = 1;
    nBall = 0; for (u32 x = 0; x <= FULL; x++) if (__builtin_popcount(x) <= R) nBall++;
    mBall = malloc(nBall * 4); { long e = 0; for (u32 x = 0; x <= FULL; x++) if (__builtin_popcount(x) <= R) mBall[e++] = x; }
    for (long y = 0; y < N; y++) { int d = __builtin_popcount((u32)y); Bh[y] = (d <= R || m - d <= R); }
    wht(Bh);
    w = malloc(K * 4);
    nopen = N;
    for (int i = 0; i < K; i++) { w[i] = canon((u32)rnd() & FULL); setze(w[i], +1); }
    u32 *best = malloc(K * 4); memcpy(best, w, K * 4);
    long bestopen = nopen; double tbest = jetzt(), tschreib = 0, tlog = jetzt();
    schreibe(aus, best);
    int tabulen = K;
    u32 *tabuv = calloc(tabulen, 4); int tpos = 0; for (int q = 0; q < tabulen; q++) tabuv[q] = 0xFFFFFFFFu;
    int64_t it = 0;
    while (nopen > 0) {
        it++;
        double t = jetzt();
        if (t - t0 > sek) break;
        if (t - tlog > 10) { fprintf(stderr, "t=%.0f it=%lld offen=%ld best=%ld\n", t - t0, (long long)it, nopen, bestopen); tlog = t; }
        long vorher = nopen;
        int i = rnd() % K;
        u32 alt = w[i];
        setze(alt, -1);
        for (long c = 0; c < N; c++) G[c] = cnt[c] ? 0 : wt[c];
        wht(G);
        for (long c = 0; c < N; c++) G[c] *= Bh[c];
        wht(G);
        /* G[v] = N * gewichteter Gewinn von v; ungewichteter Gewinn fuer Aspiration nicht noetig: Tabu nur auf Werte */
        i64 bg = -1; u32 bv = alt; long ties = 0;
        for (long v = 0; v < N; v++) {
            i64 g = G[v];
            if (g < bg) continue;
            int tb = 0; for (int q = 0; q < tabulen; q++) if (tabuv[q] == (u32)v) { tb = 1; break; }
            if (tb) continue;
            if (g > bg) { bg = g; bv = (u32)v; ties = 1; }
            else if (rnd() % (++ties) == 0) bv = (u32)v;
        }
        w[i] = bv; setze(bv, +1);
        tabuv[tpos] = alt; tpos = (tpos + 1) % tabulen;
        if (nopen >= vorher) for (long c = 0; c < N; c++) if (!cnt[c]) wt[c]++;
        if (nopen < bestopen) {
            bestopen = nopen; memcpy(best, w, K * 4); tbest = jetzt();
            if (tbest - tschreib > 1.0 || nopen == 0) { schreibe(aus, best); tschreib = tbest; }
        }
    }
    schreibe(aus, best);
    fprintf(stderr, "ENDE best=%ld it=%lld t_best=%.1f t=%.1f\n", bestopen, (long long)it, tbest - t0, jetzt() - t0);
    return 0;
}
