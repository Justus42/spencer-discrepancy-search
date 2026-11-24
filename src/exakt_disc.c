/*
 * exakt_disc.c -- Prüfcode, kein Suchcode. Exakte Diskrepanz per vollständiger Aufzählung.
 *
 * disc(A) = min über x in {-1,+1}^n von max_i |(A x)_i|, nur ganze Zahlen.
 * x_0 = +1 fest (x und -x geben dasselbe Maximum), die übrigen n-1 Vorzeichen laufen im
 * Gray-Code, je Schritt wird genau eine Spalte umgedreht. Zeilensummen als int8 in einem
 * Vektor mit 64 Spuren (m <= 64 Zeilen, Einträge in {-1,0,1}, n <= 40 Spalten).
 *
 * Aufruf:
 *   exakt_disc DATEI                       Diskrepanz der Matrix
 *   exakt_disc DATEI -t                    ... der Transponierten
 *   exakt_disc DATEI [-t] -s k             für jede k-Menge gestrichener Spalten (lexikographisch):
 *                                          eine Zeile "i,j,.. disc zeuge"
 *   exakt_disc DATEI [-t] -g G             nur prüfen, ob ein x mit Maximum < G existiert
 *                                          (bricht beim ersten ab; Ausgabe wie oben)
 * Zeuge: Hexzahl, Bit j = 1 heißt x_j = -1 (Spalten der Restmatrix in Originalreihenfolge).
 * Datei: Leerraum-getrennte ganze Zahlen, eine Matrixzeile je Textzeile.
 *
 * Bauen: gcc -O3 -march=native -o build/exakt_disc src/exakt_disc.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef int8_t v64 __attribute__((vector_size(64)));
typedef uint64_t u64x8 __attribute__((vector_size(64)));

#define MAXR 64
#define MAXC 40

static int A[MAXR][MAXC], R, C;

static void lies(const char *fn, int transp) {
    FILE *f = fopen(fn, "r");
    if (!f) { perror(fn); exit(2); }
    int B[MAXR > MAXC ? MAXR : MAXC][MAXR > MAXC ? MAXR : MAXC];
    char line[1 << 14];
    int r = 0, c = -1;
    while (fgets(line, sizeof line, f)) {
        int k = 0, v, off = 0, n;
        char *p = line;
        while (sscanf(p, "%d%n", &v, &n) == 1) {
            if (v < -1 || v > 1) { fprintf(stderr, "Eintrag %d nicht in {-1,0,1}\n", v); exit(2); }
            if (k >= 64 || r >= 64) { fprintf(stderr, "zu groß\n"); exit(2); }
            B[r][k++] = v; p += n; off += n;
        }
        if (k == 0) continue;
        if (c < 0) c = k;
        if (k != c) { fprintf(stderr, "Zeile %d hat %d statt %d Einträge\n", r, k, c); exit(2); }
        r++;
    }
    fclose(f);
    if (transp) { R = c; C = r; for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) A[i][j] = B[j][i]; }
    else        { R = r; C = c; for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) A[i][j] = B[i][j]; }
    if (R > MAXR || C > MAXC || R < 1 || C < 1) { fprintf(stderr, "Größe %d x %d nicht unterstützt\n", R, C); exit(2); }
}

static inline int any_ge(v64 r, int g) {
    v64 hi = (v64){0} + (int8_t)(g - 1);
    v64 lo = (v64){0} - (int8_t)(g - 1);
    v64 m = (r > hi) | (r < lo);
    u64x8 u = (u64x8)m;
    return (u[0] | u[1] | u[2] | u[3] | u[4] | u[5] | u[6] | u[7]) != 0;
}

/* Exakte Diskrepanz der Spalten cols[0..k-1]. Mit ziel > 0: Abbruch beim ersten x mit Maximum < ziel.
 * Rückgabe: bestes Maximum; *zeuge: Bitmaske über die Positionen in cols. */
static int disc_cols(const int *cols, int k, int ziel, uint64_t *zeuge) {
    v64 r = {0}, d[MAXC];
    for (int t = 0; t < k; t++) {
        v64 v = {0};
        for (int i = 0; i < R; i++) v[i] = (int8_t)A[i][cols[t]];
        r += v;                     /* x = (+1,...,+1) */
        d[t] = -(v + v);            /* Änderung beim nächsten Umdrehen von Spalte t */
    }
    int best = 0;
    for (int i = 0; i < R; i++) { int a = r[i] < 0 ? -r[i] : r[i]; if (a > best) best = a; }
    uint64_t bz = 0;
    if (ziel > 0 && best < ziel) { *zeuge = 0; return best; }
    uint64_t steps = (k >= 1) ? (1ULL << (k - 1)) : 1;
    for (uint64_t s = 1; s < steps; s++) {
        int t = __builtin_ctzll(s) + 1;
        r += d[t];
        d[t] = -d[t];
        if (!any_ge(r, best)) {
            int m = 0;
            for (int i = 0; i < R; i++) { int a = r[i] < 0 ? -r[i] : r[i]; if (a > m) m = a; }
            best = m;
            bz = (s ^ (s >> 1)) << 1;   /* Gray-Code, Bit 0 = Spalte cols[0] bleibt +1 */
            if (ziel > 0 && best < ziel) break;
            if (best == 0) break;
        }
    }
    *zeuge = bz;
    return best;
}

/* Nachrechnen des Zeugen, getrennt von der Aufzählung */
static int max_zeuge(const int *cols, int k, uint64_t z) {
    int m = 0;
    for (int i = 0; i < R; i++) {
        int s = 0;
        for (int t = 0; t < k; t++) s += ((z >> t) & 1) ? -A[i][cols[t]] : A[i][cols[t]];
        if (s < 0) s = -s;
        if (s > m) m = s;
    }
    return m;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Aufruf: exakt_disc DATEI [-t] [-s k] [-g G]\n"); return 2; }
    int transp = 0, strich = 0, ziel = 0;
    for (int a = 2; a < argc; a++) {
        if (!strcmp(argv[a], "-t")) transp = 1;
        else if (!strcmp(argv[a], "-s") && a + 1 < argc) strich = atoi(argv[++a]);
        else if (!strcmp(argv[a], "-g") && a + 1 < argc) ziel = atoi(argv[++a]);
        else { fprintf(stderr, "unbekannt: %s\n", argv[a]); return 2; }
    }
    lies(argv[1], transp);
    if (strich < 0 || strich >= C || strich > 6) { fprintf(stderr, "-s k mit 0 <= k < Spalten, k <= 6\n"); return 2; }
    int weg[8], cols[MAXC];
    for (int i = 0; i < strich; i++) weg[i] = i;
    long faelle = 0;
    int dmin = 1 << 30, dmax = -1;
    long hist[MAXC * 2 + 2] = {0};
    for (;;) {
        int k = 0, w = 0;
        for (int j = 0; j < C; j++) { if (w < strich && weg[w] == j) { w++; continue; } cols[k++] = j; }
        uint64_t z;
        int d = disc_cols(cols, k, ziel, &z);
        int chk = max_zeuge(cols, k, z);
        if (chk != d) { fprintf(stderr, "FEHLER: Zeuge ergibt %d statt %d\n", chk, d); return 3; }
        /* Zeuge in Originalspalten umrechnen */
        uint64_t zo = 0;
        for (int t = 0; t < k; t++) if ((z >> t) & 1) zo |= 1ULL << cols[t];
        if (strich > 0) {
            for (int i = 0; i < strich; i++) printf(i ? ",%d" : "%d", weg[i]);
            printf(" %d %llx\n", d, (unsigned long long)zo);
        } else {
            printf("matrix %d x %d%s: %s %d, zeuge %llx\n", R, C, transp ? " (transponiert)" : "",
                   ziel > 0 ? (d < ziel ? "gefunden max" : "kein x unter Ziel, exakt") : "disc", d,
                   (unsigned long long)zo);
        }
        faelle++; hist[d]++;
        if (d < dmin) dmin = d;
        if (d > dmax) dmax = d;
        /* nächste k-Menge */
        int i = strich - 1;
        while (i >= 0 && weg[i] == C - strich + i) i--;
        if (i < 0) break;
        weg[i]++;
        for (int j = i + 1; j < strich; j++) weg[j] = weg[j - 1] + 1;
    }
    if (strich > 0) {
        fprintf(stderr, "faelle %ld, min %d, max %d, verteilung:", faelle, dmin, dmax);
        for (int d = 0; d <= 2 * MAXC; d++) if (hist[d]) fprintf(stderr, " %d:%ld", d, hist[d]);
        fprintf(stderr, "\n");
    }
    return 0;
}
