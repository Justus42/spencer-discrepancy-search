/*
 * ueberdeckung10.c -- Rechencode (Auftrag P, Teil 5, Schritt S6).
 * Frage: Lassen sich die 512 Antipodenklassen von {0,1}^10 mit K Kugeln vom Radius 2 (je 56 Klassen) ueberdecken?
 * Exakte Tiefensuche: erste Kugel um Klasse 0 (Translationssymmetrie), dann immer die kleinste unueberdeckte Klasse,
 * Schranke: unueberdeckt <= Summe der K_rest groessten momentanen Zuwaechse (jede Kugel traegt hoechstens ihren Zuwachs bei).
 * Aufruf: ueberdeckung10 K
 * Bauen: gcc -O3 -march=native -o build/ueberdeckung10 src/pruef_ideen/ueberdeckung10.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define N 512
typedef struct { uint64_t w[8]; } bs;
static bs ball[N];
static int rep[N], idx_of[1024];
static int enth[N][56];
static long long knoten = 0;
static int K;
static int sol[64];

static inline int pc(const bs *a, const bs *cov) { int s = 0; for (int i = 0; i < 8; i++) s += __builtin_popcountll(a->w[i] & ~cov->w[i]); return s; }
static inline int cnt(const bs *a) { int s = 0; for (int i = 0; i < 8; i++) s += __builtin_popcountll(a->w[i]); return s; }

static int rec(bs cov, int rest, int depth) {
    knoten++;
    int c = cnt(&cov), unab = N - c;
    if (unab == 0) return 1;
    if (rest == 0) return 0;
    int gain[N]; int hist[57] = {0};
    for (int i = 0; i < N; i++) { gain[i] = pc(&ball[i], &cov); hist[gain[i]]++; }
    int top = 0, r = rest;
    for (int g = 56; g >= 0 && r > 0; g--) { int t = hist[g] < r ? hist[g] : r; top += t * g; r -= t; }
    if (top < unab) return 0;
    int first = -1;
    for (int i = 0; i < N; i++) if (!(cov.w[i >> 6] >> (i & 63) & 1)) { first = i; break; }
    /* Kandidaten: Kugeln, die first enthalten, nach Zuwachs absteigend */
    int cand[56], cg[56];
    for (int j = 0; j < 56; j++) { cand[j] = enth[first][j]; cg[j] = gain[cand[j]]; }
    for (int a = 0; a < 56; a++) for (int b = a + 1; b < 56; b++) if (cg[b] > cg[a]) { int t = cg[a]; cg[a] = cg[b]; cg[b] = t; t = cand[a]; cand[a] = cand[b]; cand[b] = t; }
    for (int j = 0; j < 56; j++) {
        bs n2 = cov;
        for (int i = 0; i < 8; i++) n2.w[i] |= ball[cand[j]].w[i];
        sol[depth] = cand[j];
        if (rec(n2, rest - 1, depth + 1)) return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    K = argc > 1 ? atoi(argv[1]) : 10;
    int n = 0;
    for (int x = 0; x < 1024; x++) { int y = x ^ 1023; if (x < y) { idx_of[x] = n; idx_of[y] = n; rep[n++] = x; } }
    for (int c = 0; c < N; c++) {
        memset(&ball[c], 0, sizeof(bs));
        for (int x = 0; x < 1024; x++) {
            int d = __builtin_popcount(x ^ rep[c]);
            if (d <= 2 || 10 - d <= 2) { int k = idx_of[x]; ball[c].w[k >> 6] |= 1ULL << (k & 63); }
        }
    }
    int fill[N] = {0};
    for (int c = 0; c < N; c++) { if (cnt(&ball[c]) != 56) { fprintf(stderr, "Kugelgroesse %d\n", cnt(&ball[c])); return 2; } for (int i = 0; i < N; i++) if (ball[c].w[i >> 6] >> (i & 63) & 1) enth[i][fill[i]++] = c; }
    for (int i = 0; i < N; i++) if (fill[i] != 56) { fprintf(stderr, "Enthaltensein %d\n", fill[i]); return 2; }
    clock_t t0 = clock();
    sol[0] = 0;
    int ok = rec(ball[0], K - 1, 1);
    printf("K = %d Kugeln: %s; Knoten %lld; Zeit %.1f s\n", K, ok ? "UEBERDECKUNG GEFUNDEN" : "keine Ueberdeckung", knoten, (double)(clock() - t0) / CLOCKS_PER_SEC);
    if (ok) { printf("Zentren (Klassenvertreter):"); for (int i = 0; i < K; i++) printf(" %d", rep[sol[i]]); printf("\n"); }
    return 0;
}
