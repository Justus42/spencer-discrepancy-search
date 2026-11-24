/*
 * zirkulant.c -- Suchcode (kein Beweis). Vollständige Aufzählung zirkulanter n x n-Matrizen mit
 * Einträgen +-1 bis auf Verschiebung, Komplement und Multiplikatoren, je Klasse Gegenbeispielsuche.
 *
 * Erste Zeile a in {0,1}^n (Bit t = 1 heißt Eintrag -1), C_ij = a_{(j-i) mod n}.
 * Gruppe: Verschiebung a_t -> a_{t+k}, Multiplikator a_t -> a_{mt} (ggT(m,n) = 1, auch m = -1, das ist
 * die Transponierte), Komplement a -> ~a. Alle drei lassen disc(C) gleich (Zeilen-/Spaltenpermutation
 * bzw. Vorzeichen). Vertreter einer Klasse = kleinstes a der Bahn als ganze Zahl.
 * Rand (-r): zusätzlich eine Einser-Zeile und eine Nullspalte, also (n+1) x (n+1); die Nullspalte
 * ändert A x nicht und wird weggelassen. Die Gruppe lässt auch die geränderte Diskrepanz gleich.
 *
 * Je Klasse:
 *  1. Filter: Tabu-Suche nach x mit max |(A x)_i| < d. Gelingt das nicht im Budget: Überlebender.
 *  2. Abstieg: weiter nach kleinerem Maximum suchen (kleines Budget). Ergebnis u = obere Schranke
 *     für disc der Klasse. Häufigkeiten von u und die Vertreter mit größtem u werden ausgegeben.
 * Überlebende und Vertreter mit größtem u werden danach exakt geprüft (build/exakt_disc, Prüfcode).
 *
 * Aufruf: zirkulant n d [-r] [-p THREADS] [-o PRAEFIX] [-b BUDGET] [-a]   (-a: alle Klassen mit u nach PRAEFIX_alle.txt)
 * Ausgabe: Zusammenfassung auf stdout; PRAEFIX_ueberlebende.txt, PRAEFIX_top.txt (Vertreter hex, u).
 * Bauen: gcc -O3 -march=native -pthread -o build/zirkulant src/zirkulant.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define MAXN 32
#define TOPMAX 2000

static int n, d, rand_, nthreads = 4, budget = 400;
static const char *praefix = "zirk";
static uint32_t maske;
static int nmult, mults[MAXN];
static uint32_t mtab[MAXN][4][256];     /* Multiplikator-Bildtabellen je Byte */

static uint64_t naechster_block = 0, bloecke;
static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static FILE *f_ueb, *f_alle;
static long g_klassen = 0, g_ueb = 0, g_hist[2 * MAXN + 2];
static int g_umax = -1;
static uint32_t g_top[TOPMAX]; static int g_ntop = 0;
static long g_filterschritte = 0;

static inline uint32_t rot(uint32_t a, int k) { return k ? (((a >> k) | (a << (n - k))) & maske) : a; }
static inline uint32_t mul(uint32_t a, int mi) {
    const uint32_t (*t)[256] = mtab[mi];
    return t[0][a & 255] | t[1][(a >> 8) & 255] | t[2][(a >> 16) & 255] | t[3][(a >> 24) & 255];
}

/* a ist kanonisch, wenn kein Element der Bahn kleiner ist */
static int kanonisch(uint32_t a) {
    for (int k = 1; k < n; k++) if (rot(a, k) < a) return 0;
    uint32_t c = (~a) & maske;
    for (int k = 0; k < n; k++) if (rot(c, k) < a) return 0;
    for (int mi = 0; mi < nmult; mi++) {
        uint32_t b = mul(a, mi), bc = (~b) & maske;
        for (int k = 0; k < n; k++) if (rot(b, k) < a || rot(bc, k) < a) return 0;
    }
    return 1;
}

typedef struct { uint64_t s; } rng_t;
static inline uint64_t rnd(rng_t *g) { uint64_t x = g->s; x ^= x << 13; x ^= x >> 7; x ^= x << 17; return g->s = x; }
static inline int ex(int v, int T) { if (v < 0) v = -v; return v > T ? v - T : 0; }

/* Tabu-Suche: versucht max <= T zu erreichen, Start von x. Rückgabe 1 bei Erfolg (x, r aktualisiert). */
static int tabu(int R, int8_t A[][MAXN], int *x, int *r, int T, int maxschritte, rng_t *g, long *zaehler) {
    int tab[MAXN] = {0};
    int cost = 0; for (int i = 0; i < R; i++) cost += ex(r[i], T);
    if (!cost) return 1;
    int bestcost = cost;
    for (int it = 1; it <= maxschritte; it++) {
        (*zaehler)++;
        int bj = -1, bd = 1 << 30, nb = 0;
        for (int j = 0; j < n; j++) {
            int ds = 0, s2 = 2 * x[j];
            for (int i = 0; i < R; i++) ds += ex(r[i] - s2 * A[i][j], T) - ex(r[i], T);
            if (tab[j] > it && cost + ds >= bestcost) continue;
            if (ds < bd) { bd = ds; bj = j; nb = 1; }
            else if (ds == bd && (rnd(g) % (++nb)) == 0) bj = j;
        }
        if (bj < 0) continue;
        int s2 = 2 * x[bj];
        for (int i = 0; i < R; i++) r[i] -= s2 * A[i][bj];
        x[bj] = -x[bj];
        cost += bd;
        tab[bj] = it + 1 + (int)(rnd(g) % (unsigned)(n / 4 + 2));
        if (cost < bestcost) bestcost = cost;
        if (!cost) return 1;
    }
    return 0;
}

static int maxabs(int R, const int *r) { int m = 0; for (int i = 0; i < R; i++) { int a = r[i] < 0 ? -r[i] : r[i]; if (a > m) m = a; } return m; }

/* Liefert u (beste gefundene obere Schranke); *ueb = 1, wenn kein x mit max < d gefunden */
static int bearbeite(uint32_t a, int *ueb, long *zaehler) {
    int8_t A[MAXN + 1][MAXN];
    int R = n + rand_;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) A[i][j] = ((a >> ((j - i + n) % n)) & 1) ? -1 : 1;
    if (rand_) for (int j = 0; j < n; j++) A[n][j] = 1;
    rng_t g = { 0x9E3779B97F4A7C15ULL ^ ((uint64_t)a * 0xD1B54A32D192ED03ULL) ^ ((uint64_t)n << 40) };
    for (int i = 0; i < 5; i++) rnd(&g);
    int x[MAXN], r[MAXN + 1], bx[MAXN], best = 1 << 30;
    /* 1. Filter: Ziel max <= d-1, mehrere Neustarts */
    int ok = 0;
    for (int start = 0; start < 8 && !ok; start++) {
        for (int j = 0; j < n; j++) x[j] = (rnd(&g) & 1) ? 1 : -1;
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < n; j++) s += A[i][j] * x[j]; r[i] = s; }
        int m = maxabs(R, r);
        if (m < best) { best = m; memcpy(bx, x, sizeof x); }
        if (m < d) { ok = 1; break; }
        if (tabu(R, A, x, r, d - 1, budget * 4, &g, zaehler)) {
            ok = 1; best = maxabs(R, r); memcpy(bx, x, sizeof x);
        }
    }
    *ueb = !ok;
    if (!ok) return best;
    /* Nachrechnen des Gegenbeispiels, unabhängig von r */
    {
        int m = 0;
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < n; j++) s += A[i][j] * bx[j]; if (s < 0) s = -s; if (s > m) m = s; }
        if (m != best || m >= d) { fprintf(stderr, "FEHLER Nachrechnung a=%x\n", a); exit(3); }
    }
    /* 2. Abstieg ab dem Gegenbeispiel, zwei Neustarts */
    for (int start = 0; start < 3; start++) {
        if (start == 0) memcpy(x, bx, sizeof x);
        else for (int j = 0; j < n; j++) x[j] = (rnd(&g) & 1) ? 1 : -1;
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < n; j++) s += A[i][j] * x[j]; r[i] = s; }
        int m = maxabs(R, r); if (m < best) best = m;
        while (best > 0 && tabu(R, A, x, r, best - 1, budget, &g, zaehler)) { m = maxabs(R, r); if (m < best) best = m; }
    }
    return best;
}

static void *arbeiter(void *arg) {
    (void)arg;
    long kl = 0, ue = 0, hist[2 * MAXN + 2] = {0}, zaehler = 0;
    int umax = -1; uint32_t top[TOPMAX]; int ntop = 0;
    for (;;) {
        uint64_t b = __atomic_fetch_add(&naechster_block, 1, __ATOMIC_RELAXED);
        if (b >= bloecke) break;
        uint64_t lo = b << 16, hi = lo + (1ULL << 16);
        if (hi > (1ULL << n)) hi = 1ULL << n;
        for (uint64_t aa = lo; aa < hi; aa++) {
            uint32_t a = (uint32_t)aa;
            if (!kanonisch(a)) continue;
            kl++;
            int ueb;
            int u = bearbeite(a, &ueb, &zaehler);
            if (ueb) {
                ue++;
                pthread_mutex_lock(&mx);
                fprintf(f_ueb, "%x %d\n", a, u); fflush(f_ueb);
                pthread_mutex_unlock(&mx);
                continue;
            }
            if (f_alle) { pthread_mutex_lock(&mx); fprintf(f_alle, "%x %d\n", a, u); pthread_mutex_unlock(&mx); }
            hist[u]++;
            if (u > umax) { umax = u; ntop = 0; }
            if (u == umax && ntop < TOPMAX) top[ntop++] = a;
        }
    }
    pthread_mutex_lock(&mx);
    g_klassen += kl; g_ueb += ue; g_filterschritte += zaehler;
    for (int i = 0; i < 2 * MAXN + 2; i++) g_hist[i] += hist[i];
    if (umax > g_umax) { g_umax = umax; g_ntop = 0; }
    if (umax == g_umax) for (int i = 0; i < ntop && g_ntop < TOPMAX; i++) g_top[g_ntop++] = top[i];
    pthread_mutex_unlock(&mx);
    return 0;
}

static int ggt(int a, int b) { while (b) { int t = a % b; a = b; b = t; } return a; }

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "Aufruf: zirkulant n d [-r] [-p THREADS] [-o PRAEFIX] [-b BUDGET]\n"); return 2; }
    int alle = 0;
    n = atoi(argv[1]); d = atoi(argv[2]);
    for (int i = 3; i < argc; i++) {
        if (!strcmp(argv[i], "-r")) rand_ = 1;
        else if (!strcmp(argv[i], "-p") && i + 1 < argc) nthreads = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-o") && i + 1 < argc) praefix = argv[++i];
        else if (!strcmp(argv[i], "-b") && i + 1 < argc) budget = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-a")) alle = 1;
        else { fprintf(stderr, "unbekannt: %s\n", argv[i]); return 2; }
    }
    if (n < 2 || n > 31) { fprintf(stderr, "2 <= n <= 31\n"); return 2; }
    maske = (uint32_t)((1ULL << n) - 1);
    for (int m = 2; m < n; m++) if (ggt(m, n) == 1) mults[nmult++] = m;
    for (int mi = 0; mi < nmult; mi++) {
        int m = mults[mi], minv = 0;
        for (int t = 1; t < n; t++) if ((m * t) % n == 1) minv = t;
        /* b_t = a_{m t}: Bit s von a landet auf Position minv * s */
        for (int p = 0; p < 4; p++) for (int v = 0; v < 256; v++) {
            uint32_t w = 0;
            for (int i = 0; i < 8; i++) { int s = 8 * p + i; if (s < n && ((v >> i) & 1)) w |= 1u << ((minv * s) % n); }
            mtab[mi][p][v] = w;
        }
    }
    char fn[512];
    snprintf(fn, sizeof fn, "%s_ueberlebende.txt", praefix);
    f_ueb = fopen(fn, "w");
    if (alle) { snprintf(fn, sizeof fn, "%s_alle.txt", praefix); f_alle = fopen(fn, "w"); }
    bloecke = ((1ULL << n) + 65535) >> 16;
    struct timespec t0, t1; clock_gettime(CLOCK_MONOTONIC, &t0);
    pthread_t th[64];
    for (int i = 0; i < nthreads; i++) pthread_create(&th[i], 0, arbeiter, 0);
    for (int i = 0; i < nthreads; i++) pthread_join(th[i], 0);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fclose(f_ueb);
    if (f_alle) fclose(f_alle);
    double sek = (t1.tv_sec - t0.tv_sec) + 1e-9 * (t1.tv_nsec - t0.tv_nsec);
    snprintf(fn, sizeof fn, "%s_top.txt", praefix);
    FILE *ft = fopen(fn, "w");
    /* sortiert ausgeben (Reihenfolge der Threads ist zufällig) */
    for (int i = 1; i < g_ntop; i++) { uint32_t v = g_top[i]; int j = i; while (j > 0 && g_top[j - 1] > v) { g_top[j] = g_top[j - 1]; j--; } g_top[j] = v; }
    for (int i = 0; i < g_ntop; i++) fprintf(ft, "%x %d\n", g_top[i], g_umax);
    fclose(ft);
    printf("n=%d%s d=%d: klassen %ld, ueberlebende %ld, beste obere schranke u_max %d (%d vertreter%s), zeit %.1f s, tabu-schritte %ld\n",
           n, rand_ ? " mit rand" : "", d, g_klassen, g_ueb, g_umax, g_ntop, g_ntop >= TOPMAX ? ", gekappt" : "", sek, g_filterschritte);
    printf("verteilung u:");
    for (int i = 0; i < 2 * MAXN + 2; i++) if (g_hist[i]) printf(" %d:%ld", i, g_hist[i]);
    printf("\n");
    return 0;
}
