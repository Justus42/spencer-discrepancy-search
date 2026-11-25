/* Suchcode Auftrag S: lokale Suche (Tabu) "waehle k Elemente S von G mit S + A_D = G fuer alle D".
 * Kosten = Zahl offener Punkte (D, g). Problemdatei aus src/spann_zaehl.py export.
 * Schritt: zufaelliger offener Punkt p; unter seinen Kandidaten t (nicht in S, nicht tabu) die mit groesstem Gewinn
 * (bis zu 6 Stueck) exakt bewerten: fuer jedes s in S Verlust beim Tausch s -> t; bester Tausch wird ausgefuehrt
 * (auch wenn schlechter). Tabu: entferntes s darf tenure Schritte nicht zurueck, eingefuegtes t nicht wieder raus.
 * Abbruch bei Kosten 0 oder Zeitlimit.
 *
 * Aufruf: spann_ls PROBLEM k seed zeit_s [STARTDATEI]   (Startdatei: Elemente von G, Leerraum-getrennt;
 *         bei mehr als k Elementen werden die ersten k genommen; bei weniger zufaellig aufgefuellt)
 * Ausgabe: Verlauf, beste Kosten, bei Erfolg LOESUNG (Elemente) und WOERTER.
 * Bauen: gcc -O3 -march=native -o build/spann_ls src/spann_ls.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MAXN 4096
#define MAXC 512
#define MAXK 64

static int d, N, NC, K;
static int mu[MAXC];
static int *A[MAXC];
static long long woerter[MAXN];
static uint8_t *cov;      /* NC*N */
static uint16_t *wer;     /* NC*N: XOR der ueberdeckenden Elemente (eindeutig, wenn cov == 1) */
static int32_t gain[MAXN];  /* offene Punkte, die t neu ueberdecken wuerde */
static int32_t loss[MAXN];  /* Punkte, die nur s ueberdeckt */
static int inS[MAXN];
static int S[MAXK];
static long long kosten;
static long long tabu_bis[MAXN];
static int32_t shared[MAXN];
static int32_t *offen; static int32_t *offpos; static long long noffen;  /* Liste offener Punkte */

static uint64_t rng;
static inline uint64_t zuf(void) { rng ^= rng << 13; rng ^= rng >> 7; rng ^= rng << 17; return rng; }
static double jetzt(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + 1e-9 * ts.tv_nsec; }

static inline void offen_dazu(int32_t p) { offpos[p] = (int32_t)noffen; offen[noffen++] = p; }
static inline void offen_weg(int32_t p) { int32_t i = offpos[p]; int32_t q = offen[--noffen]; offen[i] = q; offpos[q] = i; offpos[p] = -1; }

static void hinzu(int t) {
    inS[t] = 1;
    for (int D = 0; D < NC; D++) {
        const int *a = A[D]; int m = mu[D]; size_t base = (size_t)D * N;
        for (int i = 0; i < m; i++) {
            int g = t ^ a[i]; size_t p = base + g;
            uint8_t c = cov[p]++;
            if (c == 0) {
                kosten--; offen_weg((int32_t)p);
                for (int j = 0; j < m; j++) gain[g ^ a[j]]--;
                loss[t]++;
            } else if (c == 1) {
                loss[wer[p]]--;
            }
            wer[p] ^= (uint16_t)t;
        }
    }
}

static void entferne(int s) {
    inS[s] = 0;
    for (int D = 0; D < NC; D++) {
        const int *a = A[D]; int m = mu[D]; size_t base = (size_t)D * N;
        for (int i = 0; i < m; i++) {
            int g = s ^ a[i]; size_t p = base + g;
            wer[p] ^= (uint16_t)s;
            uint8_t c = --cov[p];
            if (c == 0) {
                kosten++; offen_dazu((int32_t)p);
                for (int j = 0; j < m; j++) gain[g ^ a[j]]++;
                loss[s]--;
            } else if (c == 1) {
                loss[wer[p]]++;
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 5) { fprintf(stderr, "spann_ls PROBLEM k seed zeit [START]\n"); return 2; }
    FILE *f = fopen(argv[1], "r"); char zeile[1 << 16];
    if (!fgets(zeile, sizeof zeile, f)) return 2;
    if (fscanf(f, "%d %d", &d, &NC) != 2) return 2;
    N = 1 << d;
    for (int g = 0; g < N; g++) if (fscanf(f, "%lld", &woerter[g]) != 1) return 2;
    for (int D = 0; D < NC; D++) {
        long long x; if (fscanf(f, "%lld %d", &x, &mu[D]) != 2) return 2;
        A[D] = malloc(sizeof(int) * mu[D]);
        for (int i = 0; i < mu[D]; i++) if (fscanf(f, "%d", &A[D][i]) != 1) return 2;
    }
    fclose(f);
    K = atoi(argv[2]); int seed = atoi(argv[3]); double zeit = atof(argv[4]);
    rng = 0x9E3779B97F4A7C15ull ^ ((uint64_t)seed * 0x2545F4914F6CDD1Dull); for (int i = 0; i < 20; i++) zuf();
    size_t P = (size_t)NC * N;
    cov = calloc(P, 1); wer = calloc(P, sizeof(uint16_t));
    offen = malloc(sizeof(int32_t) * P); offpos = malloc(sizeof(int32_t) * P);
    noffen = 0; for (size_t p = 0; p < P; p++) offen_dazu((int32_t)p);
    kosten = (long long)P;
    for (int t = 0; t < N; t++) { gain[t] = 0; for (int D = 0; D < NC; D++) gain[t] += mu[D]; }
    int ns = 0;
    if (argc > 5) {
        FILE *s = fopen(argv[5], "r"); int x;
        while (ns < K && fscanf(s, "%d", &x) == 1) if (!inS[x]) { S[ns++] = x; hinzu(x); }
        fclose(s);
        printf("Start aus %s: %d Elemente, Kosten %lld\n", argv[5], ns, kosten);
    }
    while (ns < K) { int x = (int)(zuf() % N); if (!inS[x]) { S[ns++] = x; hinzu(x); } }
    printf("Problem %s: |G|=%d, Nebenklassen %d, Punkte %zu, k=%d, seed %d, Zeit %.0f s; Startkosten %lld\n",
           argv[1], N, NC, P, K, seed, zeit, kosten);
    fflush(stdout);
    double t0 = jetzt(), tlast = t0;
    long long best = kosten, schritt = 0;
    int bestS[MAXK]; memcpy(bestS, S, sizeof(int) * K);
    int tmin = 3, tmax = 3 + K / 2;
    while (kosten > 0) {
        schritt++;
        if ((schritt & 63) == 0) {
            double t = jetzt();
            if (t - t0 > zeit) break;
            if (t - tlast > 60) { printf("  %.0f s: Schritte %lld, Kosten %lld, beste %lld\n", t - t0, schritt, kosten, best); fflush(stdout); tlast = t; }
        }
        /* offener Punkt */
        int32_t p = offen[zuf() % noffen];
        int D = p / N, g = p % N;
        /* Kandidaten t nach Gewinn */
        int kt[8]; int nk = 0;
        int32_t kg[8];
        for (int i = 0; i < mu[D]; i++) {
            int t = g ^ A[D][i];
            if (inS[t] || tabu_bis[t] > schritt) continue;
            int32_t v = gain[t] * 1024 + (int32_t)(zuf() & 1023);
            if (nk < 6) { int j = nk++; while (j > 0 && kg[j - 1] < v) { kg[j] = kg[j - 1]; kt[j] = kt[j - 1]; j--; } kg[j] = v; kt[j] = t; }
            else if (v > kg[5]) { int j = 5; while (j > 0 && kg[j - 1] < v) { kg[j] = kg[j - 1]; kt[j] = kt[j - 1]; j--; } kg[j] = v; kt[j] = t; }
        }
        if (nk == 0) {  /* alle tabu: zufaelliger Kandidat */
            int t = g ^ A[D][zuf() % mu[D]]; if (inS[t]) continue; kt[0] = t; nk = 1;
        }
        long long bestd = 1LL << 60; int bt = -1, bs = -1;
        for (int c = 0; c < nk; c++) {
            int t = kt[c];
            for (int i = 0; i < K; i++) shared[S[i]] = 0;
            for (int E = 0; E < NC; E++) {
                const int *a = A[E]; int m = mu[E]; size_t base = (size_t)E * N;
                for (int i = 0; i < m; i++) { size_t q = base + (t ^ a[i]); if (cov[q] == 1) shared[wer[q]]++; }
            }
            for (int i = 0; i < K; i++) {
                int s = S[i];
                long long dlt = (long long)(loss[s] - shared[s]) - gain[t];
                int tabu = tabu_bis[s] > schritt;
                if (tabu && !(kosten + dlt < best)) continue;  /* Aspiration */
                long long v = dlt * 64 + (long long)(zuf() & 63);
                if (v < bestd) { bestd = v; bt = t; bs = i; }
            }
        }
        if (bt < 0) continue;
        int s = S[bs];
        entferne(s); hinzu(bt); S[bs] = bt;
        tabu_bis[s] = schritt + tmin + (long long)(zuf() % (tmax - tmin + 1));
        tabu_bis[bt] = schritt + tmin + (long long)(zuf() % (tmax - tmin + 1));
        if (kosten < best) { best = kosten; memcpy(bestS, S, sizeof(int) * K); }
    }
    double t = jetzt() - t0;
    printf("ENDE: Kosten %lld, beste %lld, Schritte %lld, %.1f s\n", kosten, best, schritt, t);
    if (best == 0) {
        printf("LOESUNG %d Paare:", K); for (int i = 0; i < K; i++) printf(" %d", bestS[i]);
        printf("\nWOERTER:"); for (int i = 0; i < K; i++) printf(" %lld", woerter[bestS[i]]); printf("\n");
    } else {
        printf("BESTE %lld offen:", best); for (int i = 0; i < K; i++) printf(" %d", bestS[i]); printf("\n");
    }
    return 0;
}
