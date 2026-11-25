/* Zaehl-/Hilfscode Auftrag S: alle Spaltenpermutationen pi mit pi(V) = V, V = linearer Anteil des
 * affinen Spanns L (enthaelt den Einsvektor). Mit Komplementieren x -> pi(x) ^ f (f = pi(c0) ^ c0)
 * ist das genau die Gruppe der vorzeichenbehafteten Spaltenpermutationen, die L festhalten (modulo V).
 * Eingabe: Problemdatei aus spann_zaehl.py export (Zeile 3: Paarwoerter c0 ^ v(g)), Laenge m.
 * Rueckverfolgung ueber Koordinaten; Pruefung: jedes Dualwort, dessen Traeger schon abgebildet ist,
 * muss auf ein Dualwort gehen. Am Ende volle Pruefung pi(Basis) in V.
 * Aufruf: spann_aut PROBLEM m AUSGABE [max]   (schreibt Permutationen, je Zeile m Bilder)
 * Bauen: gcc -O3 -march=native -o build/spann_aut src/spann_aut.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int m, d, N;
static uint32_t W[1 << 12];
static uint32_t basisV[32]; static int rV;
static uint8_t *istdual;   /* 2^m */
static uint32_t *dual; static int ndual;
static int ordn[32], pos[32];
static int *dualnach[32]; static int ndn[32];
static int pi_[32], benutzt[32];
static long long anzahl = 0, maxaus = 2000000;
static FILE *aus;

static int inV(uint32_t x) {  /* x in V <=> x orthogonal zu allen Dualwoertern: hier ueber Basis-Reduktion */
    for (int i = 0; i < rV; i++) if ((x ^ basisV[i]) < x) x ^= basisV[i];
    return x == 0;
}

static uint32_t bild(uint32_t h) { uint32_t y = 0; while (h) { int j = __builtin_ctz(h); h &= h - 1; y |= 1u << pi_[j]; } return y; }

static void rueck(int t) {
    if (t == m) {
        for (int i = 0; i < rV; i++) if (!inV(bild(basisV[i]))) return;
        anzahl++;
        if (anzahl <= maxaus) { for (int j = 0; j < m; j++) fprintf(aus, "%d ", pi_[j]); fprintf(aus, "\n"); }
        return;
    }
    int j = ordn[t];
    for (int b = 0; b < m; b++) {
        if (benutzt[b]) continue;
        pi_[j] = b; benutzt[b] = 1;
        int ok = 1;
        for (int i = 0; i < ndn[t] && ok; i++) if (!istdual[bild(dual[dualnach[t][i]])]) ok = 0;
        if (ok) rueck(t + 1);
        benutzt[b] = 0;
    }
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "spann_aut PROBLEM m AUSGABE [max]\n"); return 2; }
    m = atoi(argv[2]);
    if (argc > 4) maxaus = atoll(argv[4]);
    FILE *f = fopen(argv[1], "r"); char z[1 << 16];
    if (!fgets(z, sizeof z, f)) return 2;
    int nc; if (fscanf(f, "%d %d", &d, &nc) != 2) return 2;
    N = 1 << d;
    for (int g = 0; g < N; g++) { long long x; if (fscanf(f, "%lld", &x) != 1) return 2; W[g] = (uint32_t)x; }
    fclose(f);
    uint32_t ALLE = (m == 32) ? 0xffffffffu : ((1u << m) - 1);
    /* Basis von V aus W[g] ^ W[0] und ALLE, Stufenform mit verschiedenen fuehrenden Bits */
    rV = 0;
    for (int g = 0; g <= N; g++) {
        uint32_t x = (g == N) ? ALLE : (W[g] ^ W[0]);
        for (int i = 0; i < rV; i++) if ((x ^ basisV[i]) < x) x ^= basisV[i];
        if (x) { basisV[rV++] = x; for (int i = rV - 1; i > 0 && basisV[i] > basisV[i - 1]; i--) { uint32_t t = basisV[i]; basisV[i] = basisV[i - 1]; basisV[i - 1] = t; } }
    }
    printf("m=%d, Rang von V: %d\n", m, rV);
    istdual = calloc((size_t)1 << m, 1);
    dual = malloc(sizeof(uint32_t) << (m - rV));
    ndual = 0;
    for (uint32_t h = 0; h <= ALLE; h++) {
        int ok = 1;
        for (int i = 0; i < rV && ok; i++) if (__builtin_parity(h & basisV[i])) ok = 0;
        if (ok) { istdual[h] = 1; dual[ndual++] = h; }
        if (h == ALLE) break;
    }
    printf("Dualwoerter: %d\n", ndual);
    /* Koordinatenordnung: gierig so, dass Dualwoerter mit kleinem Traeger frueh geschlossen werden */
    int gewaehlt[32] = {0};
    for (int t = 0; t < m; t++) {
        int best = -1; long long bw = -1;
        for (int j = 0; j < m; j++) {
            if (gewaehlt[j]) continue;
            long long w = 0;
            uint32_t mask = 0; for (int s = 0; s < t; s++) mask |= 1u << ordn[s];
            mask |= 1u << j;
            for (int i = 1; i < ndual; i++) if ((dual[i] & ~mask) == 0) w += 1000000 / __builtin_popcount(dual[i]);
            if (w > bw) { bw = w; best = j; }
        }
        ordn[t] = best; gewaehlt[best] = 1; pos[best] = t;
    }
    for (int t = 0; t < m; t++) { dualnach[t] = malloc(sizeof(int) * ndual); ndn[t] = 0; }
    for (int i = 1; i < ndual; i++) {
        int mx = -1; uint32_t h = dual[i];
        while (h) { int j = __builtin_ctz(h); h &= h - 1; if (pos[j] > mx) mx = pos[j]; }
        dualnach[mx][ndn[mx]++] = i;
    }
    aus = fopen(argv[3], "w");
    rueck(0);
    fclose(aus);
    printf("Permutationen mit pi(V) = V: %lld%s\n", anzahl, anzahl > maxaus ? " (nur die ersten ausgegeben)" : "");
    return 0;
}
