/* bahnen3_aut.c -- Pruefcode (Teil 1, 25.11.2026). Kein Suchcode.
 * Alle vorzeichenbehafteten Spaltenpermutationen (pi, s), die S = {Zeilen} u {-Zeilen} einer +-1-Matrix
 * erhalten (Bild w[pi(j)] = s_j v[j]). Ruecksuche ueber die Spalten; je Vektor v die Bitmaske moeglicher Bilder;
 * Schnitt: Vektoren mit gleicher Maske muessen genau so viele sein, wie die Maske Elemente hat.
 * Ausgabe je Element eine Zeile: pi(0..n-1), dann n Bits (1 = Minus). Die Elemente werden in
 * src/bahnen3_gruppe.py voll nachgeprueft.
 * Aufruf: bahnen3_aut matrix.txt > elemente.txt     Bauen: gcc -O3 -o build/bahnen3_aut src/bahnen3_aut.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
static int n, m, S[64][64], pi_[64], sg[64], benutzt[64];
static uint64_t spalte[64][2]; /* [t][0]: u[t]==+1, [t][1]: u[t]==-1 */
static long knoten, anzahl;
static int cmp(const void *a, const void *b) { uint64_t x = *(uint64_t *)a, y = *(uint64_t *)b; return x < y ? -1 : x > y; }
static void suche(int k, const uint64_t *kand) {
    knoten++;
    if (k == n) {
        anzahl++;
        for (int j = 0; j < n; j++) printf("%d ", pi_[j]);
        printf(" ");
        for (int j = 0; j < n; j++) printf("%d ", sg[j] < 0);
        printf("\n");
        return;
    }
    uint64_t neu[64], srt[64];
    for (int t = 0; t < n; t++) {
        if (benutzt[t]) continue;
        for (int s = 1; s >= -1; s -= 2) {
            int ok = 1;
            for (int i = 0; i < m; i++) { neu[i] = kand[i] & spalte[t][(s * S[i][k]) < 0]; if (!neu[i]) { ok = 0; break; } srt[i] = neu[i]; }
            if (!ok) continue;
            qsort(srt, m, sizeof(uint64_t), cmp);
            for (int i = 0; i < m && ok; ) { int j = i; while (j < m && srt[j] == srt[i]) j++; if (__builtin_popcountll(srt[i]) != j - i) ok = 0; i = j; }
            if (!ok) continue;
            pi_[k] = t; sg[k] = s; benutzt[t] = 1;
            suche(k + 1, neu);
            benutzt[t] = 0;
        }
    }
}
int main(int argc, char **argv) {
    FILE *f = fopen(argv[1], "r"); int a[64][64], r = 0, c = 0; char line[4096];
    while (fgets(line, sizeof line, f)) { int k = 0; char *p = line; int v, l; while (sscanf(p, "%d%n", &v, &l) == 1) { a[r][k++] = v; p += l; } if (k) { c = k; r++; } }
    n = c;
    /* S = Zeilen u -Zeilen, ohne Doppelte */
    m = 0;
    for (int i = 0; i < r; i++) for (int s = 1; s >= -1; s -= 2) {
        int dup = 0;
        for (int q = 0; q < m && !dup; q++) { int g = 1; for (int j = 0; j < n; j++) if (S[q][j] != s * a[i][j]) { g = 0; break; } dup = g; }
        if (!dup) { for (int j = 0; j < n; j++) S[m][j] = s * a[i][j]; m++; }
    }
    for (int t = 0; t < n; t++) for (int i = 0; i < m; i++) spalte[t][S[i][t] < 0] |= 1ULL << i;
    uint64_t k0[64]; for (int i = 0; i < m; i++) k0[i] = (m == 64 ? ~0ULL : ((1ULL << m) - 1));
    suche(0, k0);
    fprintf(stderr, "n=%d |S|=%d: %ld Elemente, %ld Knoten\n", n, m, anzahl, knoten);
    return 0;
}
