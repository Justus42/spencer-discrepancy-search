/* Auftrag A, Teil 4 neu: selbstkomplementäre Überdeckungscodes im m-Würfel (Standard m = 13, Radius R = 3).
 * SUCHCODE, prüft nichts. Ergebnis wird mit src/sc_pruef.py und verifiers/pruefe_matrix.py nachgerechnet.
 * Code = P Paare {c, ~c}; gesucht: jeder Punkt hat Abstand <= R zu einem Wort. 2P Wörter.
 * Tabu-Suche (Zufallsstart): unüberdeckten Punkt p zufällig wählen, alle Züge "Zentrum i, Bit j kippen", die p überdecken
 * (Zentrum oder Komplement im Abstand R+1 von p), bewerten; besten nicht-tabu Zug nehmen (Aspiration bei Lösung).
 * Aufruf: sc13_tabu -m 13 -R 3 -P 14 -s SEED -z SEKUNDEN -o DATEI
 * Bauen: gcc -O3 -march=native -o build/sc13_tabu src/sc13_tabu.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

static int m = 13, R = 3, P = 14;
static unsigned MASK;
static int *cnt;
static unsigned *off, noff;            /* Gewicht <= R */
static unsigned *rand_off[32]; static int nrand[32];  /* Gewicht == R, Bit j nicht gesetzt */
static unsigned c[256];
static long long tabu_bis[256][32];
static uint64_t rs = 88172645463325252ULL;
static uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }

static int U;
static void setze(unsigned x, int v) {
    for (unsigned k = 0; k < noff; k++) {
        unsigned p = x ^ off[k];
        if (v > 0) { if (cnt[p]++ == 0) U--; }
        else { if (--cnt[p] == 0) U++; }
    }
}
static void add(unsigned x, int v) { setze(x, v); setze(x ^ MASK, v); }

/* Änderung der Zahl unüberdeckter Punkte, wenn Bit j von Zentrum x gekippt wird */
static int delta(unsigned x, int j) {
    unsigned y = x ^ (1u << j);
    int d = 0;
    for (int k = 0; k < nrand[j]; k++) {
        unsigned o = rand_off[j][k];
        if (cnt[x ^ o] == 1) d++;
        if (cnt[y ^ o] == 0) d--;
    }
    return 2 * d;   /* Komplement spiegelbildlich */
}

int main(int argc, char **argv) {
    long long seed = 1; double zl = 600; const char *out = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-m")) m = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-R")) R = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-P")) P = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-s")) seed = atoll(argv[++i]);
        else if (!strcmp(argv[i], "-z")) zl = atof(argv[++i]);
        else if (!strcmp(argv[i], "-o")) out = argv[++i];
    }
    MASK = (1u << m) - 1;
    rs ^= (uint64_t)seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 20; i++) rnd();
    cnt = calloc(1u << m, sizeof(int));
    off = malloc(sizeof(unsigned) << m); noff = 0;
    for (unsigned o = 0; o <= MASK; o++) if (__builtin_popcount(o) <= R) off[noff++] = o;
    for (int j = 0; j < m; j++) {
        rand_off[j] = malloc(sizeof(unsigned) * noff); nrand[j] = 0;
        for (unsigned k = 0; k < noff; k++) if (__builtin_popcount(off[k]) == R && !(off[k] >> j & 1)) rand_off[j][nrand[j]++] = off[k];
    }
    U = 1 << m;
    for (int i = 0; i < P; i++) { c[i] = rnd() & MASK; add(c[i], 1); }
    double t0 = jetzt();
    long long it = 0; int best = U;
    unsigned *unc = malloc(sizeof(unsigned) << m);
    while (U > 0) {
        it++;
        if ((it & 1023) == 0 && jetzt() - t0 > zl) break;
        /* zufälliger unüberdeckter Punkt */
        unsigned p;
        do { p = rnd() & MASK; } while (cnt[p]);
        int bd = 1 << 30, nb = 0, bi = -1, bj = -1;
        for (int i = 0; i < P; i++) {
            for (int o = 0; o < 2; o++) {
                unsigned x = o ? c[i] ^ MASK : c[i];
                unsigned diff = x ^ p;
                if (__builtin_popcount(diff) != R + 1) continue;
                while (diff) {
                    int j = __builtin_ctz(diff); diff &= diff - 1;
                    int dl = delta(c[i], j);
                    if (tabu_bis[i][j] > it && U + dl > 0) continue;
                    if (dl < bd) { bd = dl; nb = 1; bi = i; bj = j; }
                    else if (dl == bd && rnd() % (++nb) == 0) { bi = i; bj = j; }
                }
            }
        }
        if (bi < 0) {   /* kein Zug: zufälliges Zentrum ein Bit Richtung p */
            bi = rnd() % P;
            unsigned diff = c[bi] ^ p; if (__builtin_popcount(diff) > m / 2) diff ^= MASK;
            if (!diff) continue;
            int k = rnd() % __builtin_popcount(diff);
            while (k--) diff &= diff - 1;
            bj = __builtin_ctz(diff);
        }
        add(c[bi], -1);
        c[bi] ^= 1u << bj;
        add(c[bi], 1);
        tabu_bis[bi][bj] = it + 3 + rnd() % 8;
        if (U < best) best = U;
    }
    double t = jetzt() - t0;
    printf("m=%d R=%d P=%d (%d Woerter) seed=%lld: %s, Rest %d Punkte (bester %d), %lld Schritte, %.1f s\n",
           m, R, P, 2 * P, seed, U == 0 ? "GELOEST" : "ungeloest", U, best, it, t);
    if (U == 0 && out) {
        FILE *f = fopen(out, "w");
        for (int i = 0; i < P; i++) {
            for (int q = 0; q < 2; q++) {
                unsigned x = q ? c[i] ^ MASK : c[i];
                for (int j = 0; j < m; j++) fputc((x >> j & 1) ? '1' : '0', f);
                fputc('\n', f);
            }
        }
        fclose(f);
    }
    return U == 0 ? 0 : 3;
}
