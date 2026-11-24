/* bahnen3_tabu.c -- SUCHCODE (Teil 4, 25.11.2026). Kein Pruefcode: jeder Treffer wird getrennt mit
 * src/bahnen3_pruef.py (Ausbau der Bahnen, Breitensuche ueber alle 2^20 Punkte) nachgerechnet.
 *
 * Laenge 20, Radius 6, ein Element g der Ordnung 3 (vorzeichenbehaftete Spaltenpermutation, Zeile L der P-Datei).
 * Bahnen von <g> auf den 2^19 Zeilenpaaren {x, ~x}: feste Zeilen (Gewicht 1) und Dreierbahnen (Gewicht 3).
 * Zustand: Bahnmenge mit Zeilenzahl <= T. Zielgroesse: Zahl unueberdeckter Zeilenpaare (Punktbahn mit Gewicht 1/3).
 * Muster wie src/bahnen_tabu.c: zufaellige offene Punktbahn p; Kandidaten b = Bahnen, die p ueberdecken (nicht tabu),
 * die TOPT besten Dreierbahnen und TOPT1 besten festen Zeilen nach Gewinn; Zuege: Tausch (a in Loesung, b), wenn die
 * Zeilenzahl <= T bleibt (Aenderung = Verlust(a) - Ueberlappung(a,b) - Gewinn(b)), oder Hinzunahme von b bei Platz
 * (Aenderung = -Gewinn(b)). Bester Zug, Zufall bei Gleichstand. Entfernte Bahn 2-9 Schritte tabu.
 * Stillstand (30 000 Schritte ohne Verbesserung) -> Neustart aus Zufall (Zeilenzahl genau T).
 *
 * Aufruf: bahnen3_tabu P.txt L T sekunden seed_von seed_bis ausgabepraefix [stichprobe]
 *   stichprobe > 0: keine Gewinnpflege; Gewinn nur fuer alle festen Zeilen und 'stichprobe' zufaellige Dreierbahnen
 *   unter den Ueberdeckern von p berechnet (schneller je Schritt, schwaechere Auswahl). 0 (Standard): exakt gepflegt.
 *   Je Startwert ein Thread. Log laufend nach stdout. Treffer: praefix_seed<s>.txt (je Bahn: Vertreterwort Gewicht).
 * Bauen: gcc -O3 -march=native -fopenmp -o build/bahnen3_tabu src/bahnen3_tabu.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define N 20
#define RAD 6
#define TOPT 24
#define TOPT1 8
#define MAXS 64

static int pi_[N], mi[N];
static int NT, W, NF;                 /* NF feste Zeilen (Ids 0..NF-1), danach Dreierbahnen */
static int *bid; static uint32_t *rep;
static uint64_t *M;
static double sek;
static int T;
static int STICH = 0;   /* 0: Gewinne exakt gepflegt (wie bahnen_tabu.c); >0: Gewinn nur fuer STICH zufaellige Kandidaten berechnet */
static const char *praefix;

static uint32_t kan(uint32_t x) { return (x >> (N - 1)) & 1 ? x ^ ((1u << N) - 1) : x; }
static uint32_t wirk(uint32_t x) { uint32_t y = 0; for (int j = 0; j < N; j++) y |= (((x >> j) & 1) ^ mi[j]) << pi_[j]; return y; }
static inline int wt(int q) { return q < NF ? 1 : 3; }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline uint64_t rnd(uint64_t *s) { uint64_t x = *s; x ^= x << 13; x ^= x >> 7; x ^= x << 17; return *s = x; }

typedef struct {
    int S[MAXS], ns, wsum; uint8_t *in; uint8_t *cnt; uint64_t *U1;
    int *gain; int *ulist, *upos, nu; long long f; int *tabu; uint64_t rs; uint64_t *U0; int *kand;
} Zust;

static void bit_set(uint64_t *b, int q) { b[q >> 6] |= 1ULL << (q & 63); }
static void bit_clr(uint64_t *b, int q) { b[q >> 6] &= ~(1ULL << (q & 63)); }
static void u_add(Zust *z, int q) { z->upos[q] = z->nu; z->ulist[z->nu++] = q; z->U0[q >> 6] |= 1ULL << (q & 63); }
static void u_del(Zust *z, int q) { int i = z->upos[q], l = z->ulist[--z->nu]; z->ulist[i] = l; z->upos[l] = i; z->upos[q] = -1; z->U0[q >> 6] &= ~(1ULL << (q & 63)); }

static void gain_row(Zust *z, int q, int d) {
    if (STICH) return;
    const uint64_t *r = M + (size_t)q * W;
    for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int b = v * 64 + __builtin_ctzll(x); x &= x - 1; z->gain[b] += d; } }
}
static void hinzu(Zust *z, int b) {
    z->in[b] = 1; z->S[z->ns++] = b; z->wsum += wt(b);
    const uint64_t *r = M + (size_t)b * W;
    for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1;
        uint8_t c = z->cnt[q]++;
        if (c == 0) { z->f -= wt(q); u_del(z, q); bit_set(z->U1, q); gain_row(z, q, -wt(q)); }
        else if (c == 1) bit_clr(z->U1, q);
    } }
}
static void weg(Zust *z, int i) {
    int a = z->S[i]; z->S[i] = z->S[--z->ns]; z->in[a] = 0; z->wsum -= wt(a);
    const uint64_t *r = M + (size_t)a * W;
    for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1;
        uint8_t c = z->cnt[q]--;
        if (c == 1) { z->f += wt(q); u_add(z, q); bit_clr(z->U1, q); gain_row(z, q, wt(q)); }
        else if (c == 2) bit_set(z->U1, q);
    } }
}
static long long wpc(const uint64_t *U1, const uint64_t *ra, const uint64_t *rb) {
    long long s = 0; int fw = NF >> 6;
    for (int v = 0; v < W; v++) {
        uint64_t x = U1[v] & ra[v]; if (rb) x &= rb[v];
        if (!x) continue;
        if (v > fw) s += 3 * __builtin_popcountll(x);
        else { while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1; s += wt(q); } }
    }
    return s;
}
static void aufbau(Zust *z) { /* cnt, U1, Liste, f, gain aus z->S (wie bahnen_tabu.c) */
    memset(z->cnt, 0, NT); memset(z->in, 0, NT); memset(z->U1, 0, sizeof(uint64_t) * W);
    z->wsum = 0;
    for (int i = 0; i < z->ns; i++) {
        int a = z->S[i]; z->in[a] = 1; z->wsum += wt(a);
        const uint64_t *r = M + (size_t)a * W;
        for (int v = 0; v < W; v++) { uint64_t x = r[v]; while (x) { int q = v * 64 + __builtin_ctzll(x); x &= x - 1; if (z->cnt[q] < 255) z->cnt[q]++; } }
    }
    z->nu = 0; z->f = 0; memset(z->gain, 0, sizeof(int) * NT); memset(z->U0, 0, sizeof(uint64_t) * W);
    for (int q = 0; q < NT; q++) {
        z->upos[q] = -1;
        if (z->cnt[q] == 0) { u_add(z, q); z->f += wt(q); gain_row(z, q, wt(q)); }
        else if (z->cnt[q] == 1) bit_set(z->U1, q);
    }
}
static void start(Zust *z) {
    memset(z->in, 0, NT); z->ns = 0; int w = 0;
    while (w < T) {
        int b;
        if (T - w >= 3 && (rnd(&z->rs) % 8)) b = NF + (int)(rnd(&z->rs) % (NT - NF));
        else b = (int)(rnd(&z->rs) % NF);
        if (z->in[b] || w + wt(b) > T) continue;
        z->in[b] = 1; z->S[z->ns++] = b; w += wt(b);
    }
    aufbau(z);
}

static void lauf(int seed) {
    Zust z; z.rs = 0x9E3779B97F4A7C15ULL ^ ((uint64_t)seed * 0x100000001B3ULL); for (int i = 0; i < 10; i++) rnd(&z.rs);
    z.in = malloc(NT); z.cnt = malloc(NT); z.U1 = malloc(sizeof(uint64_t) * W); z.gain = malloc(sizeof(int) * NT); z.U0 = calloc(W, sizeof(uint64_t)); z.kand = malloc(sizeof(int) * NT);
    z.ulist = malloc(sizeof(int) * NT); z.upos = malloc(sizeof(int) * NT); z.tabu = calloc(NT, sizeof(int));
    double t0 = jetzt(), tlog = t0;
    start(&z);
    long long best = z.f, bestrun = z.f; long step = 0, seit = 0, neust = 0;
    int bestS[MAXS], bestn = z.ns; memcpy(bestS, z.S, sizeof(int) * z.ns);
    const long STILL = 30000;
    printf("[seed %d] Start f=%lld, %d Bahnen, %d Zeilen (Start %.1f s)\n", seed, z.f, z.ns, z.wsum, jetzt() - t0); fflush(stdout);
    int geloest = 0;
    while (1) {
        if (z.f == 0) { geloest = 1; break; }
        double t = jetzt();
        if ((step & 15) == 0) {
            if (t - t0 > sek) break;
            if (t - tlog > 60) { tlog = t; printf("[seed %d] t=%.0fs Schritt %ld f=%lld bester=%lld Neustarts=%ld Bahnen=%d Zeilen=%d\n", seed, t - t0, step, z.f, best, neust, z.ns, z.wsum); fflush(stdout); }
        }
        step++;
        int p = z.ulist[rnd(&z.rs) % z.nu];
        int cb[TOPT + TOPT1], cg[TOPT + TOPT1], n3 = 0, n1 = 0;
        int *b3 = cb, *g3 = cg, *b1 = cb + TOPT, *g1 = cg + TOPT;
        const uint64_t *rp = M + (size_t)p * W;
        int nk = 0;
        for (int v = 0; v < W; v++) { uint64_t x = rp[v]; while (x) { int b = v * 64 + __builtin_ctzll(x); x &= x - 1;
            if (z.in[b] || z.tabu[b] > step) continue;
            z.kand[nk++] = b;
        } }
        if (STICH) { /* feste Zeilen (Ids < NF, vorne) immer, von den Dreierbahnen eine Zufallsstichprobe */
            int k1 = 0; while (k1 < nk && z.kand[k1] < NF) k1++;
            int m3 = nk - k1;
            if (m3 > STICH) {
                for (int i = 0; i < STICH; i++) { int j = i + (int)(rnd(&z.rs) % (m3 - i)); int t = z.kand[k1 + i]; z.kand[k1 + i] = z.kand[k1 + j]; z.kand[k1 + j] = t; }
                nk = k1 + STICH;
            }
            for (int i = 0; i < nk; i++) z.gain[z.kand[i]] = (int)wpc(z.U0, M + (size_t)z.kand[i] * W, 0);
        }
        for (int i = 0; i < nk; i++) { int b = z.kand[i]; {
            int g = z.gain[b];
            int *bb = b < NF ? b1 : b3, *gg = b < NF ? g1 : g3, *nn = b < NF ? &n1 : &n3, cap = b < NF ? TOPT1 : TOPT;
            if (*nn < cap) { int j = (*nn)++; while (j > 0 && gg[j - 1] < g) { gg[j] = gg[j - 1]; bb[j] = bb[j - 1]; j--; } gg[j] = g; bb[j] = b; }
            else if (g > gg[cap - 1] || (g == gg[cap - 1] && (rnd(&z.rs) & 1))) { int j = cap - 1; while (j > 0 && gg[j - 1] < g) { gg[j] = gg[j - 1]; bb[j] = bb[j - 1]; j--; } gg[j] = g; bb[j] = b; }
        } }
        int kb[TOPT + TOPT1], kg[TOPT + TOPT1], nc = 0;
        for (int i = 0; i < n3; i++) { kb[nc] = b3[i]; kg[nc++] = g3[i]; }
        for (int i = 0; i < n1; i++) { kb[nc] = b1[i]; kg[nc++] = g1[i]; }
        if (nc == 0) continue;
        long long loss[MAXS];
        for (int i = 0; i < z.ns; i++) loss[i] = wpc(z.U1, M + (size_t)z.S[i] * W, 0);
        long long bd = 1LL << 60; int ba = -2, bbi = -1, nt = 0;
        for (int c = 0; c < nc; c++) {
            if (z.wsum + wt(kb[c]) <= T) {  /* Hinzunahme */
                long long d = -kg[c];
                if (d < bd) { bd = d; ba = -1; bbi = c; nt = 1; }
                else if (d == bd && (rnd(&z.rs) % (++nt)) == 0) { ba = -1; bbi = c; }
            }
            for (int i = 0; i < z.ns; i++) {
                if (z.wsum - wt(z.S[i]) + wt(kb[c]) > T) continue;
                long long d = loss[i] - kg[c];
                if (d - loss[i] > bd) continue;
                d -= wpc(z.U1, M + (size_t)z.S[i] * W, M + (size_t)kb[c] * W);
                if (d < bd) { bd = d; ba = i; bbi = c; nt = 1; }
                else if (d == bd && (rnd(&z.rs) % (++nt)) == 0) { ba = i; bbi = c; }
            }
        }
        if (ba == -2) continue;
        int b = kb[bbi];
        hinzu(&z, b);
        if (ba >= 0) { int a = z.S[ba]; weg(&z, ba); z.tabu[a] = step + 2 + (int)(rnd(&z.rs) % 8); }
        if (z.f < best) { best = z.f; bestn = z.ns; memcpy(bestS, z.S, sizeof(int) * z.ns); }
        if (z.f < bestrun) { bestrun = z.f; seit = 0; } else if (++seit > STILL) {
            neust++; seit = 0; start(&z); bestrun = z.f; memset(z.tabu, 0, sizeof(int) * NT);
        }
    }
    double tt = jetzt() - t0;
    if (geloest) {
        char fn[512]; snprintf(fn, sizeof fn, "%s_seed%d.txt", praefix, seed);
        FILE *o = fopen(fn, "w"); for (int i = 0; i < z.ns; i++) fprintf(o, "%u %d\n", rep[z.S[i]], wt(z.S[i])); fclose(o);
        printf("[seed %d] GELOEST nach %.1f s, Schritt %ld, Neustarts %ld, %d Bahnen, %d Zeilen -> %s\n", seed, tt, step, neust, z.ns, z.wsum, fn);
    } else {
        printf("[seed %d] NICHT geloest nach %.1f s, Schritte %ld, Neustarts %ld, beste Restzahl %lld Zeilenpaare; beste Bahnen:", seed, tt, step, neust, best);
        for (int i = 0; i < bestn; i++) printf(" %u/%d", rep[bestS[i]], wt(bestS[i]));
        printf("\n");
    }
    fflush(stdout);
}

int main(int argc, char **argv) {
    if (argc < 8) { fprintf(stderr, "Aufruf: P.txt L T sekunden seed_von seed_bis praefix [stichprobe]\n"); return 1; }
    if (argc > 8) STICH = atoi(argv[8]);
    int L = atoi(argv[2]); T = atoi(argv[3]); sek = atof(argv[4]);
    int s0 = atoi(argv[5]), s1 = atoi(argv[6]); praefix = argv[7];
    FILE *fp = fopen(argv[1], "r");
    for (int g = 0; g <= L; g++) {
        for (int j = 0; j < N; j++) if (fscanf(fp, "%d", &pi_[j]) != 1) return 1;
        for (int j = 0; j < N; j++) if (fscanf(fp, "%d", &mi[j]) != 1) return 1;
    }
    fclose(fp);
    double t0 = jetzt();
    uint32_t NP = 1u << (N - 1);
    bid = malloc(sizeof(int) * NP); rep = malloc(sizeof(uint32_t) * NP);
    for (uint32_t x = 0; x < NP; x++) bid[x] = -1;
    int id = 0;
    for (int pass = 0; pass < 2; pass++) for (uint32_t x = 0; x < NP; x++) {
        if (bid[x] >= 0) continue;
        uint32_t y = kan(wirk(x)), z2 = kan(wirk(y));
        int fest = (y == x);
        if (fest != (pass == 0)) continue;
        if (!fest && kan(wirk(z2)) != x) { fprintf(stderr, "Element hat nicht Ordnung 3\n"); return 1; }
        rep[id] = x; bid[x] = id; bid[y] = id; bid[z2] = id; id++;
        if (pass == 0) NF = id;
    }
    NT = id; W = (NT + 63) / 64;
    int nm = 0; uint32_t *mask = malloc(sizeof(uint32_t) * (1u << N));
    for (uint32_t x = 0; x < (1u << N); x++) if (__builtin_popcount(x) <= RAD) mask[nm++] = x;
    M = calloc((size_t)NT * W, sizeof(uint64_t));
    if (!M) { fprintf(stderr, "kein Speicher\n"); return 1; }
    #pragma omp parallel for schedule(dynamic, 64)
    for (int p = 0; p < NT; p++) { uint64_t *r = M + (size_t)p * W; for (int i = 0; i < nm; i++) { int o = bid[kan(rep[p] ^ mask[i])]; r[o >> 6] |= 1ULL << (o & 63); } }
    long long ones = 0; for (size_t i = 0; i < (size_t)NT * W; i++) ones += __builtin_popcountll(M[i]);
    printf("Stichprobe %d; Element Zeile %d: %d feste Zeilen, %d Dreierbahnen, %d Bahnen; T=%d Zeilen (%d Woerter); Matrix %.2f GB, mittlere Zeile %.0f, Aufbau %.1f s\n",
           STICH, L, NF, NT - NF, NT, T, 2 * T, (double)NT * W * 8 / 1e9, (double)ones / NT, jetzt() - t0);
    fflush(stdout);
    int ns = s1 - s0 + 1;
    #pragma omp parallel for schedule(dynamic, 1) num_threads(ns < 4 ? ns : 4)
    for (int s = s0; s <= s1; s++) lauf(s);
    return 0;
}
