/* bahnen_schaetz.c -- Teil D (24.11.2026): nur Schaetzung des Aufwands einer vollstaendigen Aufzaehlung
 * fuer K volle Bahnen bei n=20, R=6. Startet die Aufzaehlung NICHT, sondern misst Stichproben.
 *
 * Baum wie in bahnen_zaehl.c: Ebene 1 = Bahnen, die die feste Punkt-Bahn {0, 1...1} ueberdecken,
 * bis auf den Stabilisator N0 dieses Punktes im Normalisator (Blockdrehungen, Blockvertauschungen,
 * Multiplikator i -> 2i); weitere Ebenen: Verzweigen nach dem ersten unueberdeckten Punkt in statischer
 * Ordnung (wenigste Ueberdecker zuerst), "o_i drin, fruehere Geschwister nicht"; letzte Ebene: UND-Kette.
 * Je Klasse a werden s zufaellige Kinder b gezogen und deren Teilbaum (K-2 weitere Bahnen) vollstaendig
 * durchlaufen und gestoppt. Hochrechnung: Summe ueber Klassen (Kinderzahl x mittlere Teilbaumzeit).
 *
 * Aufruf: bahnen_schaetz n R K s seed
 * Bauen: gcc -O3 -march=native -fopenmp -o build/bahnen_schaetz src/bahnen_schaetz.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

static int n, R, NB, NF, NT, W;
static int *bid; static uint32_t *rep; static uint64_t *M; static int *ord;
static long long knoten[8];

static uint32_t drehe(uint32_t x) { uint32_t y = 0; for (int b = 0; b < NB; b++) { uint32_t k = (x >> (5 * b)) & 31u; y |= (((k << 1) | (k >> 4)) & 31u) << (5 * b); } return y; }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static uint64_t rs = 88172645463325252ULL;
static uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

static long long zaehle(int t, const uint64_t *U, const uint64_t *avail, int tiefe) {
    knoten[tiefe]++;
    int leer = 1; for (int w = 0; w < W; w++) if (U[w]) { leer = 0; break; }
    if (leer) return 1;            /* Ueberdeckung schon erreicht (Erweiterungen nicht gezaehlt) */
    if (t == 0) return 0;
    if (t == 1) {
        uint64_t *X = malloc(8 * W); memcpy(X, avail, 8 * W);
        for (int w = 0; w < W; w++) { uint64_t u = U[w]; while (u) { int p = w * 64 + __builtin_ctzll(u); u &= u - 1;
            const uint64_t *r = M + (size_t)p * W; int nz = 0;
            for (int v = 0; v < W; v++) { X[v] &= r[v]; nz |= (X[v] != 0); }
            if (!nz) { free(X); return 0; } } }
        long long c = 0; for (int v = 0; v < W; v++) c += __builtin_popcountll(X[v]);
        free(X); return c;
    }
    int p = -1; for (int i = 0; i < NT; i++) { int q = ord[i]; if ((U[q >> 6] >> (q & 63)) & 1) { p = q; break; } }
    const uint64_t *r = M + (size_t)p * W;
    uint64_t *A2 = malloc(8 * W), *U2 = malloc(8 * W); memcpy(A2, avail, 8 * W);
    long long tot = 0;
    for (int v = 0; v < W; v++) { uint64_t x = r[v] & avail[v]; while (x) { int o = v * 64 + __builtin_ctzll(x); x &= x - 1;
        A2[o >> 6] &= ~(1ULL << (o & 63));
        const uint64_t *ro = M + (size_t)o * W; for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
        tot += zaehle(t - 1, U2, A2, tiefe + 1); } }
    free(A2); free(U2); return tot;
}

/* Normalisator-Erzeuger, die {0, 1..1} festhalten */
static uint32_t g_dreh0(uint32_t x) { uint32_t k = x & 31; return (x & ~31u) | (((k << 1) | (k >> 4)) & 31); }
static uint32_t g_tausch(uint32_t x) { return (x & ~1023u) | ((x >> 5) & 31) | ((x & 31) << 5); }
static uint32_t g_zyklus(uint32_t x) { uint32_t y = 0; for (int j = 0; j < NB; j++) y |= ((x >> (5 * j)) & 31) << (5 * ((j + 1) % NB)); return y; }
static uint32_t g_mult(uint32_t x) { uint32_t y = 0; for (int j = 0; j < NB; j++) for (int i = 0; i < 5; i++) if ((x >> (5 * j + i)) & 1) y |= 1u << (5 * j + (2 * i) % 5); return y; }

int main(int argc, char **argv) {
    n = atoi(argv[1]); R = atoi(argv[2]); int K = atoi(argv[3]); int s = atoi(argv[4]); rs ^= (uint64_t)atoi(argv[5]) * 0x9E3779B97F4A7C15ULL;
    NB = n / 5; uint32_t N = 1u << n, alle = N - 1;
    double t0 = jetzt();
    bid = malloc(sizeof(int) * N); rep = malloc(sizeof(uint32_t) * N);
    for (uint32_t x = 0; x < N; x++) bid[x] = -1;
    int id = 0;
    for (int pass = 0; pass < 2; pass++) for (uint32_t x = 0; x < N; x++) {
        if (bid[x] >= 0 || ((drehe(x) == x) != pass)) continue;
        rep[id] = x; uint32_t y = x; for (int j = 0; j < 5; j++) { bid[y] = id; bid[y ^ alle] = id; y = drehe(y); }
        if (!pass) NF = id + 1; id++;
    }
    NT = id; W = (NT + 63) / 64;
    int nm = 0; uint32_t *mask = malloc(sizeof(uint32_t) * N);
    for (uint32_t x = 0; x < N; x++) if (__builtin_popcount(x) <= R) mask[nm++] = x;
    M = calloc((size_t)NT * W, 8);
    #pragma omp parallel for schedule(dynamic, 64)
    for (int p = 0; p < NT; p++) { uint64_t *r = M + (size_t)p * W; for (int i = 0; i < nm; i++) { int o = bid[rep[p] ^ mask[i]]; r[o >> 6] |= 1ULL << (o & 63); } }
    int *gr = malloc(sizeof(int) * NT);
    for (int p = 0; p < NT; p++) { int c = 0; const uint64_t *r = M + (size_t)p * W;
        for (int w = 0; w < (NF >> 6); w++) c += __builtin_popcountll(r[w]);
        if (NF & 63) c += __builtin_popcountll(r[NF >> 6] & ((1ULL << (NF & 63)) - 1));
        gr[p] = c; }
    ord = malloc(sizeof(int) * NT); for (int i = 0; i < NT; i++) ord[i] = i;
    /* nach gr sortieren (Zaehlsort) */
    { int mx = 0; for (int p = 0; p < NT; p++) if (gr[p] > mx) mx = gr[p];
      int *c = calloc(mx + 2, sizeof(int)); for (int p = 0; p < NT; p++) c[gr[p] + 1]++; for (int i = 1; i <= mx + 1; i++) c[i] += c[i - 1];
      for (int p = 0; p < NT; p++) ord[c[gr[p]]++] = p; free(c); }
    int p0 = bid[0];
    printf("n=%d R=%d K=%d: %d volle Bahnen, Aufbau %.1f s; Punkt-Bahn {0,1..1} wird von %d vollen Bahnen ueberdeckt (min. Zeile %d)\n",
           n, R, K, NF, jetzt() - t0, gr[p0], gr[ord[0]]);
    /* Klassen der Ueberdecker von p0 unter N0 (Union-Find ueber Erzeuger) */
    int *cand = malloc(sizeof(int) * NF), nc = 0;
    for (int o = 0; o < NF; o++) if ((M[(size_t)p0 * W + (o >> 6)] >> (o & 63)) & 1) cand[nc++] = o;
    int *par = malloc(sizeof(int) * NF); for (int o = 0; o < NF; o++) par[o] = o;
    #define FIND(x) ({ int _x = (x); while (par[_x] != _x) { par[_x] = par[par[_x]]; _x = par[_x]; } _x; })
    uint32_t (*gen[4])(uint32_t) = { g_dreh0, g_tausch, g_zyklus, g_mult };
    for (int o = 0; o < NF; o++) for (int g = 0; g < 4; g++) { int a = FIND(o), b = FIND(bid[gen[g](rep[o])]); if (a != b) par[a < b ? b : a] = a < b ? a : b; }
    int *klgr = calloc(NF, sizeof(int)), nkl_alle = 0, nkl = 0;
    for (int o = 0; o < NF; o++) { if (FIND(o) == o) nkl_alle++; klgr[FIND(o)]++; }
    int *klv = malloc(sizeof(int) * NF);
    for (int i = 0; i < nc; i++) if (FIND(cand[i]) == cand[i]) klv[nkl++] = cand[i];
    printf("Klassen aller vollen Bahnen unter N0: %d; Klassen der %d Ueberdecker von {0,1..1}: %d\n", nkl_alle, nc, nkl);
    /* je Klasse: Kinderzahl, s Stichproben-Teilbaeume */
    uint64_t *U = malloc(8 * W), *A = malloc(8 * W), *U1 = malloc(8 * W), *A1 = malloc(8 * W);
    double summe = 0, summe_ohne_sym = 0, tmess = 0; long long kinder_ges = 0, loes = 0;
    for (int k = 0; k < nkl; k++) {
        int a = klv[k];
        for (int w = 0; w < W; w++) { U[w] = 0; A[w] = 0; }
        for (int p = 0; p < NT; p++) U[p >> 6] |= 1ULL << (p & 63);
        for (int o = 0; o < NF; o++) A[o >> 6] |= 1ULL << (o & 63);
        A[a >> 6] &= ~(1ULL << (a & 63));
        const uint64_t *ra = M + (size_t)a * W; for (int w = 0; w < W; w++) U[w] &= ~ra[w];
        int p1 = -1; for (int i = 0; i < NT; i++) { int q = ord[i]; if ((U[q >> 6] >> (q & 63)) & 1) { p1 = q; break; } }
        const uint64_t *r1 = M + (size_t)p1 * W;
        int nk = 0; int *kid = malloc(sizeof(int) * NF);
        for (int o = 0; o < NF; o++) if (((r1[o >> 6] & A[o >> 6]) >> (o & 63)) & 1) kid[nk++] = o;
        double tk = 0; int m = s < nk ? s : nk; long long kn2 = 0, kn3 = 0;
        for (int j = 0; j < m; j++) {
            int i = rnd() % nk; int b = kid[i];
            memcpy(A1, A, 8 * W); for (int jj = 0; jj <= i; jj++) A1[kid[jj] >> 6] &= ~(1ULL << (kid[jj] & 63));
            const uint64_t *rb = M + (size_t)b * W; for (int w = 0; w < W; w++) U1[w] = U[w] & ~rb[w];
            memset(knoten, 0, sizeof knoten);
            double ta = jetzt(); loes += zaehle(K - 2, U1, A1, 2); tk += jetzt() - ta;
            kn2 += knoten[2]; kn3 += knoten[3];
        }
        double mittel = tk / m;
        summe += nk * mittel; summe_ohne_sym += (double)klgr[a] * nk * mittel; tmess += tk; kinder_ges += nk;
        printf("Klasse %2d: Vertreter %7u, Klassengroesse %5d, Kinder %5d, Stichprobe %d: mittlere Teilbaumzeit %.4f s, "
               "Knoten Ebene 3/4 je Teilbaum %.0f / %.0f\n", k, rep[a], klgr[a], nk, m, mittel, (double)kn2 / m, (double)kn3 / m);
        fflush(stdout); free(kid);
    }
    printf("Hochrechnung K=%d: mit Symmetrie (Ebene 1 nur Klassenvertreter) %.0f CPU-s = %.2f CPU-h; ohne Symmetrie %.2f CPU-h; "
           "Ebene-2-Knoten %lld; Messzeit %.0f s; Loesungen in Stichproben %lld\n",
           K, summe, summe / 3600, summe_ohne_sym / 3600, kinder_ges, tmess, loes);
    return 0;
}
