/* rm15.c -- Auftrag C (Sitzung, 26.11.2026): Nebenklassen von RM(1,5). Pruefcode, eigenes Programm.
   Jede Nebenklasse f + RM(1,5) hat genau einen Vertreter mit f(0) = f(e_1) = ... = f(e_5) = 0 (affine Funktion durch
   ihre Werte an 0 und e_i festgelegt); also 2^26 Vertreter. Walsh-Spektrum W(u) = sum_x (-1)^(f(x)+u.x) per schneller
   Hadamard-Transformation, Abstand zu RM(1,5) = (32 - max|W|)/2.
   Ausgabe: Zahl der Nebenklassen je Abstand; bei Abstand 12 Spektren, Grad (ANF) und Betragsmultimenge der Autokorrelation;
   je gerader Nebenklasse (gerades Gewicht) die Zahl der Stellen mit |W| >= 12.
   Bauen: gcc -O3 -march=native -fopenmp -o build/rm15 verifiers/gegenrechnung_ideen/rm15.c */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

static inline uint32_t vertreter(uint32_t idx) {
    /* verteile 26 Bits auf die Stellen ausser 0,1,2,4,8,16 */
    uint32_t f = 0; int b = 0;
    for (int p = 0; p < 32; p++) {
        if (p == 0 || p == 1 || p == 2 || p == 4 || p == 8 || p == 16) continue;
        if ((idx >> b) & 1) f |= 1u << p;
        b++;
    }
    return f;
}
static inline void walsh(uint32_t f, int *W) {
    for (int x = 0; x < 32; x++) W[x] = ((f >> x) & 1) ? -1 : 1;
    for (int h = 1; h < 32; h <<= 1)
        for (int i = 0; i < 32; i += 2 * h)
            for (int j = i; j < i + h; j++) { int a = W[j], b = W[j + h]; W[j] = a + b; W[j + h] = a - b; }
}
static int grad(uint32_t f) {
    /* Moebius-Transformation: ANF-Koeffizienten */
    uint32_t a = f;
    static const uint32_t M[5] = {0x55555555u, 0x33333333u, 0x0F0F0F0Fu, 0x00FF00FFu, 0x0000FFFFu};
    for (int k = 0; k < 5; k++) a ^= (a & M[k]) << (1 << k);
    int g = 0; for (int x = 0; x < 32; x++) if ((a >> x) & 1) { int w = __builtin_popcount(x); if (w > g) g = w; }
    return g;
}
static inline uint32_t verschiebe(uint32_t f, int a) { /* g(x) = f(x ^ a) */
    uint32_t g = 0; for (int x = 0; x < 32; x++) if ((f >> (x ^ a)) & 1) g |= 1u << x; return g;
}

#define MAXK 4096
typedef struct { int grad; uint8_t ak[17]; long n; long spek16, spek12; } Klasse; /* ak[v] = Zahl der a != 0 mit |r(a)| = 2v */

int main(void) {
    long abst[17] = {0}, stellen[33] = {0}, spekt[3] = {0}, gerade = 0;
    Klasse *K = calloc(MAXK, sizeof *K); int nk = 0;
    const uint32_t N = 1u << 26;
    #pragma omp parallel
    {
        long la[17] = {0}, ls[33] = {0}, lsp[3] = {0}, lg = 0;
        #pragma omp for schedule(static, 65536)
        for (uint32_t idx = 0; idx < N; idx++) {
            uint32_t f = vertreter(idx); int W[32]; walsh(f, W);
            int mx = 0, z12 = 0, z8 = 0;
            for (int u = 0; u < 32; u++) { int v = W[u] < 0 ? -W[u] : W[u]; if (v > mx) mx = v; z12 += v >= 12; z8 += v == 8; }
            la[(32 - mx) / 2]++;
            if (__builtin_popcount(f) % 2 == 0) { lg++; ls[z12]++; }
            if (mx == 8) {
                int typ = z8 == 16 ? 0 : (z8 == 12 ? 1 : 2); lsp[typ]++;
                Klasse k; memset(&k, 0, sizeof k); k.grad = grad(f);
                for (int a = 1; a < 32; a++) { int r = 32 - 2 * __builtin_popcount(f ^ verschiebe(f, a)); k.ak[(r < 0 ? -r : r) / 2]++; }
                #pragma omp critical
                {
                    int i; for (i = 0; i < nk; i++) if (K[i].grad == k.grad && !memcmp(K[i].ak, k.ak, 17)) break;
                    if (i == nk) { K[nk] = k; nk++; }
                    K[i].n++; if (typ == 0) K[i].spek16++; else if (typ == 1) K[i].spek12++;
                }
            }
        }
        #pragma omp critical
        {
            for (int i = 0; i < 17; i++) abst[i] += la[i];
            for (int i = 0; i < 33; i++) stellen[i] += ls[i];
            for (int i = 0; i < 3; i++) spekt[i] += lsp[i];
            gerade += lg;
        }
    }
    long s = 0;
    printf("Nebenklassen von RM(1,5) je Abstand (Summe muss 2^26 = 67108864 sein):\n");
    for (int i = 0; i <= 16; i++) { printf("  Abstand %2d: %ld\n", i, abst[i]); s += abst[i]; }
    printf("  Summe %ld\n", s);
    printf("Abstand 12: Spektrum [8^16 0^16]: %ld, [8^12 4^16 0^4]: %ld, anderes: %ld\n", spekt[0], spekt[1], spekt[2]);
    printf("Abstand 12 nach Grad und Betragsmultimenge der Autokorrelation (a != 0, |r| -> Anzahl):\n");
    for (int i = 0; i < nk; i++) {
        printf("  Grad %d, ", K[i].grad);
        for (int v = 0; v <= 16; v++) if (K[i].ak[v]) printf("|r|=%d:%d ", 2 * v, K[i].ak[v]);
        printf("-> %ld Nebenklassen (Spektrum 8^16: %ld, 8^12 4^16: %ld)\n", K[i].n, K[i].spek16, K[i].spek12);
    }
    printf("Gerade Nebenklassen: %ld; Stellen mit |W| >= 12 je gerader Nebenklasse:\n", gerade);
    for (int i = 0; i <= 32; i++) if (stellen[i]) printf("  %d Stellen: %ld\n", i, stellen[i]);
    return 0;
}
