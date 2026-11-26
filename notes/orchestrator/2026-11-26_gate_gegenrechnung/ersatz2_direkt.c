/* ersatz2_direkt.c -- Gate zu Auftrag C (Orchestrator, 26.11.2026). Pruefcode, nur ganze Zahlen.
   Ende-zu-Ende-Kontrolle fuer "zwei ersetzte Spalten", ohne die Zerlegung in Zeilengruppen und Bedingungen:
   Fuer eine +-1-Matrix H (N x N), ein Spaltenpaar a<b und neue Spalten v, w in {+-1}^N wird direkt entschieden,
   ob die ersetzte Matrix Diskrepanz >= Z hat: gesucht wird ein x mit max_i |(Mx)_i| <= Z-2 (alle Werte gerade).
   x = (x'', s, t); x''_0 = +1 fest (x und -x gleichwertig), (s,t) alle vier Vorzeichenpaare. Die x'' werden nur
   nach max|b| sortiert (flache zuerst), nichts wird weggelassen.
   Modus "voll":  alle Paare, alle (v,w) mit v_0 = w_0 = +1 (Spaltenvorzeichen aendern die Diskrepanz nicht): Zahl der guten.
   Modus "probe": je Matrix 5 zufaellige Paare, je Paar K zufaellige (v,w); Zahl der guten. Dazu je Paar die Originalspalten.
   Aufruf: ersatz2_direkt DATEI Z voll | ersatz2_direkt DATEI Z probe K SAAT
   DATEI: "ANZAHL ZEILEN SPALTEN", dann die Matrizen (Format der Sitzung, runs/gegenrechnung_ideen/eingaben/).
   Bauen: gcc -O3 -march=native -fopenmp -o ersatz2_direkt ersatz2_direkt.c */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define MN 24
static int N, Z;
typedef struct { int8_t b[MN]; int mx; } P;
static int cmp(const void *p, const void *q){ return ((const P*)p)->mx - ((const P*)q)->mx; }
static P *baue(const int8_t *H, int a, int b, long *np){
    int cols[MN], m = 0; for (int j = 0; j < N; j++) if (j != a && j != b) cols[m++] = j;
    long n = 1L << (m - 1); P *p = malloc(n * sizeof(P));
    for (long x = 0; x < n; x++) { int mx = 0;
        for (int i = 0; i < N; i++) { int s = H[i*N + cols[0]];
            for (int j = 1; j < m; j++) s += (x >> (j-1) & 1) ? -H[i*N + cols[j]] : H[i*N + cols[j]];
            p[x].b[i] = (int8_t)s; if (abs(s) > mx) mx = abs(s); }
        p[x].mx = mx; }
    qsort(p, n, sizeof(P), cmp); *np = n; return p;
}
/* 1 = Diskrepanz >= Z, 0 = es gibt x mit max <= Z-2 */
static int gut(const P *p, long n, uint32_t v, uint32_t w){
    int8_t d[4][MN];
    for (int i = 0; i < N; i++) { int vi = (v >> i & 1) ? -1 : 1, wi = (w >> i & 1) ? -1 : 1;
        d[0][i] = vi + wi; d[1][i] = vi - wi; d[2][i] = -vi + wi; d[3][i] = -vi - wi; }
    for (long k = 0; k < n; k++) for (int q = 0; q < 4; q++) { int ok = 1;
        for (int i = 0; i < N; i++) if (abs(p[k].b[i] + d[q][i]) > Z - 2) { ok = 0; break; }
        if (ok) return 0; }
    return 1;
}
static uint64_t rs; static uint64_t rnd(void){ rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
int main(int argc, char **argv){
    if (argc < 4) return 2;
    FILE *f = fopen(argv[1], "r"); int anz, nz, ns; if (!f || fscanf(f, "%d %d %d", &anz, &nz, &ns) != 3 || nz != ns || nz > MN) return 2;
    N = nz; Z = atoi(argv[2]); int8_t *H = malloc(anz * N * N);
    for (int t = 0; t < anz * N * N; t++) { int x; if (fscanf(f, "%d", &x) != 1 || (x != 1 && x != -1)) return 2; H[t] = x; }
    if (!strcmp(argv[3], "voll")) {
        long long gesamt = 0, faelle = 0;
        for (int k = 0; k < anz; k++) for (int a = 0; a < N; a++) for (int b = a + 1; b < N; b++) {
            long n; P *p = baue(H + k*N*N, a, b, &n); long long g = 0; uint32_t lim = 1u << (N - 1);
            #pragma omp parallel for reduction(+:g) schedule(dynamic,64)
            for (uint32_t v = 0; v < lim; v++) for (uint32_t w = 0; w < lim; w++) g += gut(p, n, v << 1, w << 1);
            gesamt += g; faelle += (long long)lim * lim; free(p);
            if (g) printf("mat %d Spalten %d,%d: %lld gute (v,w)\n", k, a, b, g);
        }
        printf("N=%d Ziel %d voll: %d Matrizen, %d Paare je Matrix, %lld (v,w) geprueft, gute insgesamt %lld\n", N, Z, anz, N*(N-1)/2, faelle, gesamt);
    } else {
        long K = atol(argv[4]); rs = strtoull(argv[5], 0, 10) * 2654435761u + 88172645463325252ull; long long g = 0, kontr = 0; int PAARE = 5;
        for (int k = 0; k < anz; k++) for (int r = 0; r < PAARE; r++) {
            int a = rnd() % N, b = rnd() % N; if (a == b) { r--; continue; } if (a > b) { int h = a; a = b; b = h; }
            long n; P *p = baue(H + k*N*N, a, b, &n);
            uint32_t *vv = malloc(K * sizeof(uint32_t)), *ww = malloc(K * sizeof(uint32_t));
            for (long t = 0; t < K; t++) { vv[t] = rnd() & ((1u << N) - 1); ww[t] = rnd() & ((1u << N) - 1); }
            long long gg = 0;
            #pragma omp parallel for reduction(+:gg) schedule(dynamic,64)
            for (long t = 0; t < K; t++) gg += gut(p, n, vv[t], ww[t]);
            g += gg; free(vv); free(ww);
            uint32_t ov = 0, ow = 0; for (int i = 0; i < N; i++) { if (H[k*N*N + i*N + a] < 0) ov |= 1u << i; if (H[k*N*N + i*N + b] < 0) ow |= 1u << i; }
            kontr += gut(p, n, ov, ow) == 0; Z -= 2; kontr += 1000 * gut(p, n, ov, ow); Z += 2;   /* Originalspalten: bei Ziel Z nicht gut, bei Z-2 gut */
            free(p);
        }
        printf("N=%d Ziel %d Probe: %d Matrizen x %d Paare x %ld zufaellige (v,w): gute %lld; Kontrolle Originalspalten: bei Ziel %d nicht gut in %lld, bei Ziel %d gut in %lld von %d Faellen\n",
               N, Z, anz, PAARE, K, g, Z, kontr % 1000, Z - 2, kontr / 1000, PAARE * anz);
    }
    return 0;
}
