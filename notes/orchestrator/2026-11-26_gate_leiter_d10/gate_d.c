/* gate_d.c -- Gate zu Auftrag D (Orchestrator). Eigener Zaehler, kein Code aus src/zerl_halb.c oder src/zerl_offen.py.
   Liest ein Paar (U | W) mit K Zeilen (Zeilen "u ... | w ..."), zaehlt die offenen Klassen (y, z) mit y_0 = z_0 = +1,
   also max_i (|U_i y| + |W_i z|) < d, ueber alle 2^(m1-1) * 2^(m2-1) Klassen.
   Aufruf: gate_d PAAR d      Ausgabe: K m1 m2 d offen gesamt, dazu ob U bzw. W paarweise orthogonale Zeilen hat. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define KM 16
int main(int argc, char **argv) {
    static char z[4096]; int U[KM][32], W[KM][32], K = 0, m1 = 0, m2 = 0, d = atoi(argv[2]);
    FILE *f = fopen(argv[1], "r"); if (!f) return 2;
    while (fgets(z, sizeof z, f)) {
        if (z[0] == '#' || !strchr(z, '|')) continue;
        if (K >= KM) return 3;
        char *t = strtok(z, " \t\n"); int seite = 0, a = 0, b = 0;
        while (t) { if (t[0] == '|') seite = 1; else { int v = atoi(t); if (v != 1 && v != -1) return 4; if (seite) W[K][b++] = v; else U[K][a++] = v; } t = strtok(NULL, " \t\n"); }
        if (K && (a != m1 || b != m2)) return 5;
        m1 = a; m2 = b; K++;
    }
    fclose(f);
    long n1 = 1L << (m1 - 1), n2 = 1L << (m2 - 1), offen = 0;
    unsigned char *A = malloc(n1 * KM), *B = malloc(n2 * KM); memset(A, 0, n1 * KM); memset(B, 0, n2 * KM);
    for (long y = 0; y < n1; y++) for (int i = 0; i < K; i++) { int s = U[i][0]; for (int j = 1; j < m1; j++) s += ((y >> (j - 1)) & 1) ? -U[i][j] : U[i][j]; A[y * KM + i] = (unsigned char)abs(s); }
    for (long x = 0; x < n2; x++) for (int i = 0; i < K; i++) { int s = W[i][0]; for (int j = 1; j < m2; j++) s += ((x >> (j - 1)) & 1) ? -W[i][j] : W[i][j]; B[x * KM + i] = (unsigned char)abs(s); }
    for (long y = 0; y < n1; y++) { const unsigned char *a = A + y * KM;
        for (long x = 0; x < n2; x++) { const unsigned char *b = B + x * KM; unsigned char mx = 0;
            for (int i = 0; i < KM; i++) { unsigned char s = (unsigned char)(a[i] + b[i]); if (s > mx) mx = s; }
            offen += mx < d; } }
    int ou = 1, ow = 1;
    for (int i = 0; i < K; i++) for (int k = i + 1; k < K; k++) { int s = 0, t = 0; for (int j = 0; j < m1; j++) s += U[i][j] * U[k][j]; for (int j = 0; j < m2; j++) t += W[i][j] * W[k][j]; if (s) ou = 0; if (t) ow = 0; }
    printf("K=%d m1=%d m2=%d d=%d offen=%ld gesamt=%ld U_orthogonal=%d W_orthogonal=%d\n", K, m1, m2, d, offen, n1 * n2, ou, ow);
    return 0;
}
