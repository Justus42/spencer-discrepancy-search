/* bahnen_tabu.c -- SUCHCODE (Teil B/C, 24.11.2026). Kein Pruefcode: jeder Treffer wird getrennt
 * mit src/bahnen_pruef.py (Breitensuche ueber alle 2^n Punkte) nachgerechnet.
 *
 * Lokale Suche (Tabu) ueber K volle Bahnen der Gruppe <sigma, Komplement> auf {0,1}^n, n = 5k,
 * sigma dreht jeden 5er-Block (Bits 5j..5j+4) um eine Stelle. Zielgroesse: Zahl unueberdeckter
 * Punkte (volle Punkt-Bahn = 10 Punkte, festes Paar = 2 Punkte).
 *
 * Zug: zufaellige unueberdeckte Punkt-Bahn p waehlen; Kandidaten b = volle Bahnen, die p
 * ueberdecken (nicht tabu); die T besten nach Gewinn; fuer jedes Paar (a in Loesung, b) exakte
 * Aenderung = Verlust(a) - Ueberlappung(a,b) - Gewinn(b); bester Tausch (Zufall bei Gleichstand).
 * Entfernte Bahnen sind fuer tt Schritte tabu. Stillstand -> Neustart (zufaellig oder Warmstart).
 *
 * Aufruf: bahnen_tabu n R K sekunden seed_von seed_bis ausgabepraefix [startdatei [streichen]]
 *   startdatei: Bahn-Vertreter (Woerter als Zahlen); streichen=1: die Bahn streichen, deren Wegfall
 *   am wenigsten aufdeckt (K = Zahl der Vertreter - 1 wird erzwungen).
 *   Je Startwert ein Thread. Log laufend nach stdout. Treffer: praefix_seed<s>.txt (Vertreter).
 * Bauen: gcc -O3 -march=native -fopenmp -o build/bahnen_tabu src/bahnen_tabu.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define TOPT 24

static int n, R, K, NB, NF, NT, W;
static int *bid; static uint32_t *rep;
static uint64_t *M;
static double sek;
static int nstart = 0, startb[64];
static const char *praefix;

static uint32_t drehe(uint32_t x) {
    uint32_t y = 0;
    for (int b = 0; b < NB; b++) { uint32_t k = (x >> (5 * b)) & 31u; y |= (((k << 1) | (k >> 4)) & 31u) << (5 * b); }
    return y;
}
static inline int wt(int q) { return q < NF ? 10 : 2; }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline uint64_t rnd(uint64_t *s) { uint64_t x = *s; x ^= x << 13; x ^= x >> 7; x ^= x << 17; return *s = x; }

typedef struct {
    int S[64]; uint8_t *in; uint8_t *cnt; uint64_t *U1; /* Bitset cnt==1 */
    int *gain; int *ulist, *upos, nu; long long f; int *tabu; uint64_t rs;
} Zust;

static void bit_set(uint64_t *b, int q) { b[q >> 6] |= 1ULL << (q & 63); }
static void bit_clr(uint64_t *b, int q) { b[q >> 6] &= ~(1ULL << (q & 63)); }

static void u_add(Zust *z, int q) { z->upos[q] = z->nu; z->ulist[z->nu++] = q; }
static void u_del(Zust *z, int q) { int i = z->upos[q], l = z->ulist[--z->nu]; z->ulist[i] = l; z->upos[l] = i; z->upos[q] = -1; }

static void gain_row(Zust *z, int q, int d) { /* gain[b] += d fuer alle vollen b in Zeile q */
    const uint64_t *r = M + (size_t)q * W;
    for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int b = v * 64 + __builtin_ctzll(x); x &= x - 1; if (b < NF) z->gain[b] += d; } }
}

static void hinzu(Zust *z, int b) {
    z->in[b] = 1;
    const uint64_t *r = M + (size_t)b * W;
    for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1;
        uint8_t c = z->cnt[q]++;
        if (c == 0) { z->f -= wt(q); u_del(z, q); bit_set(z->U1, q); gain_row(z, q, -wt(q)); }
        else if (c == 1) bit_clr(z->U1, q);
    } }
}
static void weg(Zust *z, int a) {
    z->in[a] = 0;
    const uint64_t *r = M + (size_t)a * W;
    for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1;
        uint8_t c = z->cnt[q]--;
        if (c == 1) { z->f += wt(q); u_add(z, q); bit_clr(z->U1, q); gain_row(z, q, wt(q)); }
        else if (c == 2) bit_set(z->U1, q);
    } }
}

/* gewichtete Popcount von U1 & Zeile a (& Zeile b) */
static long long wpc(const uint64_t *U1, const uint64_t *ra, const uint64_t *rb) {
    long long s = 0; int fw = NF >> 6;
    for (int v = 0; v < W; v++) {
        uint64_t x = U1[v] & ra[v]; if (rb) x &= rb[v];
        if (!x) continue;
        if (v < fw) s += 10 * __builtin_popcountll(x);
        else { while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1; s += wt(q); } }
    }
    return s;
}

static void aufbau(Zust *z) { /* cnt, U1, Liste, f, gain aus z->S */
    memset(z->cnt, 0, NT); memset(z->in, 0, NF); memset(z->U1, 0, sizeof(uint64_t) * W);
    for (int i = 0; i < K; i++) {
        int a = z->S[i]; z->in[a] = 1;
        const uint64_t *r = M + (size_t)a * W;
        for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1; z->cnt[q]++; } }
    }
    z->nu = 0; z->f = 0; memset(z->gain, 0, sizeof(int) * NF);
    for (int q = 0; q < NT; q++) {
        z->upos[q] = -1;
        if (z->cnt[q] == 0) { u_add(z, q); z->f += wt(q); gain_row(z, q, wt(q)); }
        else if (z->cnt[q] == 1) bit_set(z->U1, q);
    }
}

static void start(Zust *z, int warm) {
    if (warm) for (int i = 0; i < K; i++) z->S[i] = startb[i];
    else for (int i = 0; i < K; i++) { int b, ok; do { b = rnd(&z->rs) % NF; ok = 1; for (int j = 0; j < i; j++) if (z->S[j] == b) ok = 0; } while (!ok); z->S[i] = b; }
    aufbau(z);
}

static void lauf(int seed) {
    Zust z; z.rs = 0x9E3779B97F4A7C15ULL ^ ((uint64_t)seed * 0x100000001B3ULL); for (int i = 0; i < 10; i++) rnd(&z.rs);
    z.in = malloc(NF); z.cnt = malloc(NT); z.U1 = malloc(sizeof(uint64_t) * W); z.gain = malloc(sizeof(int) * NF);
    z.ulist = malloc(sizeof(int) * NT); z.upos = malloc(sizeof(int) * NT); z.tabu = calloc(NF, sizeof(int));
    int warm = nstart > 0;
    double t0 = jetzt(), tlog = t0;
    start(&z, warm);
    long long best = z.f, bestrun = z.f; long step = 0, seit = 0, neust = 0;
    int bestS[64]; memcpy(bestS, z.S, sizeof(int) * K);
    long STILL = (n <= 15) ? 20000 : 30000;
    printf("[seed %d] Start f=%lld (%s)\n", seed, z.f, warm ? "Warmstart" : "Zufall"); fflush(stdout);
    int geloest = 0;
    while (1) {
        if (z.f == 0) { geloest = 1; break; }
        double t = jetzt();
        if ((step & 63) == 0) {
            if (t - t0 > sek) break;
            if (t - tlog > 30) { tlog = t; printf("[seed %d] t=%.0fs Schritt %ld f=%lld bester=%lld Neustarts=%ld\n", seed, t - t0, step, z.f, best, neust); fflush(stdout); }
        }
        step++;
        int p = z.ulist[rnd(&z.rs) % z.nu];
        /* Top-T Kandidaten nach Gewinn */
        int cb[TOPT], cg[TOPT], nc = 0;
        const uint64_t *rp = M + (size_t)p * W;
        for (int v = 0; v < W; v++) { uint64_t x = rp[v]; while (x) { int b = v * 64 + __builtin_ctzll(x); x &= x - 1;
            if (b >= NF || z.in[b] || z.tabu[b] > step) continue;
            int g = z.gain[b];
            if (nc < TOPT) { int j = nc++; while (j > 0 && cg[j - 1] < g) { cg[j] = cg[j - 1]; cb[j] = cb[j - 1]; j--; } cg[j] = g; cb[j] = b; }
            else if (g > cg[TOPT - 1] || (g == cg[TOPT - 1] && (rnd(&z.rs) & 1))) { int j = TOPT - 1; while (j > 0 && cg[j - 1] < g) { cg[j] = cg[j - 1]; cb[j] = cb[j - 1]; j--; } cg[j] = g; cb[j] = b; }
        } }
        if (nc == 0) continue;
        long long loss[64];
        for (int i = 0; i < K; i++) loss[i] = wpc(z.U1, M + (size_t)z.S[i] * W, 0);
        long long bd = 1LL << 60; int ba = -1, bb = -1, nt = 0;
        for (int c = 0; c < nc; c++) for (int i = 0; i < K; i++) {
            long long d = loss[i] - cg[c];
            if (d - loss[i] > bd) continue; /* Ueberlappung <= loss: d - loss ist untere Schranke */
            d -= wpc(z.U1, M + (size_t)z.S[i] * W, M + (size_t)cb[c] * W);
            if (d < bd) { bd = d; ba = i; bb = c; nt = 1; }
            else if (d == bd && (rnd(&z.rs) % (++nt)) == 0) { ba = i; bb = c; }
        }
        int a = z.S[ba], b = cb[bb];
        hinzu(&z, b); weg(&z, a); z.S[ba] = b;
        z.tabu[a] = step + 2 + (int)(rnd(&z.rs) % 8) + (K > 4 ? 0 : 0);
        if (z.f < best) { best = z.f; memcpy(bestS, z.S, sizeof(int) * K); }
        if (z.f < bestrun) { bestrun = z.f; seit = 0; } else if (++seit > STILL) {
            neust++; seit = 0; start(&z, warm); bestrun = z.f; memset(z.tabu, 0, sizeof(int) * NF);
        }
    }
    double tt = jetzt() - t0;
    if (geloest) {
        char fn[512]; snprintf(fn, sizeof fn, "%s_seed%d.txt", praefix, seed);
        FILE *o = fopen(fn, "w"); for (int i = 0; i < K; i++) fprintf(o, "%u ", rep[z.S[i]]); fprintf(o, "\n"); fclose(o);
        printf("[seed %d] GELOEST nach %.1f s, Schritt %ld, Neustarts %ld -> %s\n", seed, tt, step, neust, fn);
    } else {
        printf("[seed %d] NICHT geloest nach %.1f s, Schritte %ld, Neustarts %ld, beste Restzahl %lld Punkte; beste Bahnen:", seed, tt, step, neust, best);
        for (int i = 0; i < K; i++) printf(" %u", rep[bestS[i]]);
        printf("\n");
    }
    fflush(stdout);
}

int main(int argc, char **argv) {
    if (argc < 8) { fprintf(stderr, "Aufruf: n R K sekunden seed_von seed_bis praefix [startdatei [streichen]]\n"); return 1; }
    n = atoi(argv[1]); R = atoi(argv[2]); K = atoi(argv[3]); sek = atof(argv[4]);
    int s0 = atoi(argv[5]), s1 = atoi(argv[6]); praefix = argv[7];
    NB = n / 5; uint32_t N = 1u << n, alle = N - 1;
    double t0 = jetzt();
    bid = malloc(sizeof(int) * N); rep = malloc(sizeof(uint32_t) * N);
    for (uint32_t x = 0; x < N; x++) bid[x] = -1;
    int id = 0;
    for (int pass = 0; pass < 2; pass++) for (uint32_t x = 0; x < N; x++) {
        if (bid[x] >= 0 || ((drehe(x) == x) != pass)) continue;
        rep[id] = x; uint32_t y = x; for (int j = 0; j < 5; j++) { bid[y] = id; bid[y ^ alle] = id; y = drehe(y); }
        if (!pass) NF = id + 1;
        id++;
    }
    NT = id; W = (NT + 63) / 64;
    int nm = 0; uint32_t *mask = malloc(sizeof(uint32_t) * N);
    for (uint32_t x = 0; x < N; x++) if (__builtin_popcount(x) <= R) mask[nm++] = x;
    M = calloc((size_t)NT * W, sizeof(uint64_t));
    if (!M) { fprintf(stderr, "kein Speicher\n"); return 1; }
    #pragma omp parallel for schedule(dynamic, 64)
    for (int p = 0; p < NT; p++) { uint64_t *r = M + (size_t)p * W; for (int i = 0; i < nm; i++) { int o = bid[rep[p] ^ mask[i]]; r[o >> 6] |= 1ULL << (o & 63); } }
    long long ones = 0; for (size_t i = 0; i < (size_t)NT * W; i++) ones += __builtin_popcountll(M[i]);
    printf("n=%d R=%d K=%d: %d volle Bahnen, %d Paare, Kugel %d, Matrix %.2f GB, mittlere Zeile %.0f, Aufbau %.1f s\n",
           n, R, K, NF, NT - NF, nm, (double)NT * W * 8 / 1e9, (double)ones / NT, jetzt() - t0);
    if (argc > 8) {
        FILE *f = fopen(argv[8], "r"); unsigned long x;
        while (fscanf(f, "%lu", &x) == 1) { int o = bid[x]; if (o >= NF) { fprintf(stderr, "Startbahn %lu ist kein volle Bahn\n", x); return 1; } startb[nstart++] = o; }
        fclose(f);
        int streich = argc > 9 && atoi(argv[9]);
        if (streich) { /* Bahn streichen, deren Wegfall am wenigsten aufdeckt */
            uint8_t *cnt = calloc(NT, 1);
            for (int i = 0; i < nstart; i++) { const uint64_t *r = M + (size_t)startb[i] * W; for (int q = 0; q < NT; q++) if ((r[q >> 6] >> (q & 63)) & 1) cnt[q]++; }
            int bi = -1; long long bl = 1LL << 60;
            for (int i = 0; i < nstart; i++) {
                const uint64_t *r = M + (size_t)startb[i] * W; long long l = 0;
                for (int q = 0; q < NT; q++) if (cnt[q] == 1 && ((r[q >> 6] >> (q & 63)) & 1)) l += wt(q);
                printf("  Wegfall Bahn %u deckt %lld Punkte auf\n", rep[startb[i]], l);
                if (l < bl) { bl = l; bi = i; }
            }
            printf("gestrichen: Bahn %u (%lld Punkte)\n", rep[startb[bi]], bl);
            startb[bi] = startb[--nstart]; free(cnt);
        }
        if (nstart != K) { fprintf(stderr, "Startdatei hat %d Bahnen, K=%d\n", nstart, K); return 1; }
    }
    fflush(stdout);
    int ns = s1 - s0 + 1;
    #pragma omp parallel for schedule(dynamic, 1) num_threads(ns < 4 ? ns : 4)
    for (int s = s0; s <= s1; s++) lauf(s);
    return 0;
}
