/*
 * kozyk.c -- Rechencode (Auftrag P, Teil 3). Kein Pruefcode.
 *
 * Kozyklische Matrizen: Gruppe E der Ordnung 2n mit zentraler Involution z; f: E -> {+1,-1} ungerade (f(zg) = -f(g)).
 * M[g][h] = f(g^-1 h) ueber den Vertretern der Nebenklassen von <z> (Vertreter = kleinerer Index von {g, zg}).
 * Alle f mit f(e) = +1 (2^(n-1) Stueck). Disc einer Matrix hat die Parität von n.
 *
 * Elementnummerierung (vom Pruefskript genauso benutzt):
 *   Faktoren F0, F1, F2 (Direktprodukt), Index = f0 + |F0|*(f1 + |F1|*f2).
 *   CYC(m): Index a in Z_m. DIC(m) (Ordnung 4m, <a,b | a^2m = 1, b^2 = a^m, b a b^-1 = a^-1>): Index i + 2m*j fuer a^i b^j.
 *   DIH(m) (Ordnung 2m, <r,s | r^m = s^2 = 1, s r s = r^-1>): Index i + m*j fuer r^i s^j.
 *
 * Aufruf: kozyk GRUPPE LCEIL LSIEB SHARD NSHARD PREFIX TMPDIR
 *   LCEIL: Gegenbeispiele mit max_i |<M_i,x>| <= LCEIL (wird gespeichert: PREFIX.shardS.bin, 3 Bytes je Funktion, Little Endian,
 *          Bit j = 1 heisst x_j = -1); -1: ohne Gegenbeispiele.
 *   LSIEB: Funktionen ohne x mit max <= LSIEB (Diskrepanz > LSIEB, also >= LSIEB+2) sind Ueberlebende; sie werden mit
 *          build/exakt_disc exakt bewertet. 0: kein Sieb.
 *   Funktionen von SHARD*N/NSHARD bis (SHARD+1)*N/NSHARD, N = 2^(n-1).
 * Ausgabe auf stdout: Zusammenfassung.
 * Bauen: gcc -O3 -march=native -o build/kozyk src/pruef_ideen/kozyk.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXO 64
typedef struct { int ord, n, z; int tab[MAXO][MAXO]; int inv[MAXO]; } Grp;
typedef int8_t v32 __attribute__((vector_size(32)));
typedef uint64_t u64x4 __attribute__((vector_size(32)));

enum { CYC = 0, DIC = 1, DIH = 2 };
static int fsize(int t, int m) { return t == CYC ? m : (t == DIC ? 4 * m : 2 * m); }
static int fmul(int t, int m, int a, int b) {
    if (t == CYC) return (a + b) % m;
    if (t == DIC) {
        int i = a % (2 * m), j = a / (2 * m), k = b % (2 * m), l = b / (2 * m);
        int ni = (j ? (i - k) : (i + k)) % (2 * m); if (ni < 0) ni += 2 * m;
        int nj = j + l;
        if (nj == 2) { nj = 0; ni = (ni + m) % (2 * m); }
        return ni + 2 * m * nj;
    }
    int i = a % m, j = a / m, k = b % m, l = b / m;
    int ni = (j ? (i - k) : (i + k)) % m; if (ni < 0) ni += m;
    return ni + m * (j ^ l);
}

static int build(Grp *G, const char *name) {
    int nf = 0, t[3] = {0}, m[3] = {0}, zc[3] = {0};
#define SPEC1(T, M, Z) nf = 1; t[0] = T; m[0] = M; zc[0] = Z
#define SPEC2(T0, M0, Z0, T1, M1, Z1) nf = 2; t[0] = T0; m[0] = M0; zc[0] = Z0; t[1] = T1; m[1] = M1; zc[1] = Z1
#define SPEC3(T0, M0, Z0, T1, M1, Z1, T2, M2, Z2) nf = 3; t[0] = T0; m[0] = M0; zc[0] = Z0; t[1] = T1; m[1] = M1; zc[1] = Z1; t[2] = T2; m[2] = M2; zc[2] = Z2
    if (!strcmp(name, "Z16")) { SPEC1(CYC, 16, 8); }
    else if (!strcmp(name, "Z20")) { SPEC1(CYC, 20, 10); }
    else if (!strcmp(name, "Z3xQ8")) { SPEC2(CYC, 3, 0, DIC, 2, 2); }
    else if (!strcmp(name, "Z36")) { SPEC1(CYC, 36, 18); }
    else if (!strcmp(name, "Dic9")) { SPEC1(DIC, 9, 9); }
    else if (!strcmp(name, "Z3xDic3")) { SPEC2(CYC, 3, 0, DIC, 3, 3); }
    else if (!strcmp(name, "Z40")) { SPEC1(CYC, 40, 20); }
    else if (!strcmp(name, "Z5xQ8")) { SPEC2(CYC, 5, 0, DIC, 2, 2); }
    else if (!strcmp(name, "Z5xD4")) { SPEC2(CYC, 5, 0, DIH, 4, 2); }
    else if (!strcmp(name, "Z5xZ4xZ2")) { SPEC3(CYC, 5, 0, CYC, 4, 2, CYC, 2, 0); }
    else if (!strcmp(name, "Dic10")) { SPEC1(DIC, 10, 10); }
    else if (!strcmp(name, "D20")) { SPEC1(DIH, 20, 10); }
    else if (!strcmp(name, "Z52")) { SPEC1(CYC, 52, 26); }
    else if (!strcmp(name, "Dic13")) { SPEC1(DIC, 13, 13); }
    else if (!strcmp(name, "Z56")) { SPEC1(CYC, 56, 28); }
    else if (!strcmp(name, "Z7xQ8")) { SPEC2(CYC, 7, 0, DIC, 2, 2); }
    else if (!strcmp(name, "Z7xD4")) { SPEC2(CYC, 7, 0, DIH, 4, 2); }
    else if (!strcmp(name, "Z7xZ4xZ2")) { SPEC3(CYC, 7, 0, CYC, 4, 2, CYC, 2, 0); }
    else if (!strcmp(name, "Dic14")) { SPEC1(DIC, 14, 14); }
    else if (!strcmp(name, "D28")) { SPEC1(DIH, 28, 14); }
    else if (!strcmp(name, "Z60")) { SPEC1(CYC, 60, 30); }
    else if (!strcmp(name, "Dic15")) { SPEC1(DIC, 15, 15); }
    else return 0;
    int sz[3] = {1, 1, 1}, ord = 1;
    for (int i = 0; i < nf; i++) { sz[i] = fsize(t[i], m[i]); ord *= sz[i]; }
    if (ord > MAXO) return 0;
    G->ord = ord; G->n = ord / 2;
    for (int a = 0; a < ord; a++) for (int b = 0; b < ord; b++) {
        int ca[3], cb[3], r = 0, mulf = 1;
        int aa = a, bb = b;
        for (int i = 0; i < nf; i++) { ca[i] = aa % sz[i]; aa /= sz[i]; cb[i] = bb % sz[i]; bb /= sz[i]; }
        for (int i = 0; i < nf; i++) { r += fmul(t[i], m[i], ca[i], cb[i]) * mulf; mulf *= sz[i]; }
        G->tab[a][b] = r;
    }
    int zi = 0, mulf = 1;
    for (int i = 0; i < nf; i++) { zi += zc[i] * mulf; mulf *= sz[i]; }
    G->z = zi;
    for (int a = 0; a < ord; a++) for (int b = 0; b < ord; b++) if (G->tab[a][b] == 0) G->inv[a] = b;
    return 1;
}

static int N, ordE;
static int idx_[MAXO][MAXO], sgn_[MAXO][MAXO];   /* Eintrag (g,h) = sgn * F[idx] */

static uint64_t rs = 0x9E3779B97F4A7C15ULL;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

static int n;
static uint32_t rows[MAXO];
static v32 colv[MAXO];

/* Gray-Code-Aufzaehlung: gibt es x (x_0 = +1) mit max_i |<M_i,x>| <= L ? Zeuge in *xw (Bit j = 1: x_j = -1). */
static inline int viol(v32 r, int L) {
    v32 hi = (v32){0} + (int8_t)L, lo = (v32){0} - (int8_t)L;
    v32 m = (r > hi) | (r < lo);
    u64x4 u = (u64x4)m;
    return (u[0] | u[1] | u[2] | u[3]) != 0;
}
static int exists_le(int L, uint32_t *xw) {
    v32 r = {0}, d[MAXO];
    for (int h = 0; h < n; h++) { r += colv[h]; d[h] = -(colv[h] + colv[h]); }
    uint32_t x = 0;
    if (!viol(r, L)) { *xw = 0; return 1; }
    uint64_t steps = 1ULL << (n - 1);
    for (uint64_t s = 1; s < steps; s++) {
        int t = __builtin_ctzll(s) + 1;
        r += d[t]; d[t] = -d[t]; x ^= 1u << t;
        if (!viol(r, L)) { *xw = x; return 1; }
    }
    return 0;
}
static int try_random(int L, int tries, uint32_t *xw) {
    uint32_t mask = (n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1);
    for (int t = 0; t < tries; t++) {
        uint32_t x = (uint32_t)rnd() & mask;
        int ok = 1;
        for (int g = 0; g < n; g++) {
            int s = n - 2 * __builtin_popcount(rows[g] ^ x);
            if (s > L || s < -L) { ok = 0; break; }
        }
        if (ok) { *xw = x; return 1; }
    }
    return 0;
}

static uint32_t mk[MAXO][MAXO], negm[MAXO];
static void build_matrix(uint64_t F) { /* F: n-1 Bit, Bit k-1 = F[k] = -1 fuer Vertreter Nr. k; F[0] = +1 */
    for (int g = 0; g < n; g++) {
        uint32_t rw = 0;
        for (int h = 0; h < n; h++) {
            int fv = (idx_[g][h] == 0) ? 1 : ((F >> (idx_[g][h] - 1) & 1) ? -1 : 1);
            int e = fv * sgn_[g][h];
            colv[h][g] = (int8_t)e;
            if (e < 0) rw |= 1u << h;
        }
        rows[g] = rw;
    }
    for (int h = 0; h < n; h++) for (int g = n; g < 32; g++) colv[h][g] = 0;
}

static int is_hadamard(void) {
    for (int a = 0; a < n; a++) for (int b = a + 1; b < n; b++)
        if (n - 2 * __builtin_popcount(rows[a] ^ rows[b]) != 0) return 0;
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 8) { fprintf(stderr, "Aufruf: kozyk GRUPPE LCEIL LSIEB SHARD NSHARD PREFIX TMPDIR\n"); return 2; }
    static Grp G;
    if (!build(&G, argv[1])) { fprintf(stderr, "Gruppe unbekannt\n"); return 2; }
    int Lc = atoi(argv[2]), Ls = atoi(argv[3]), sh = atoi(argv[4]), nsh = atoi(argv[5]);
    const char *prefix = argv[6], *tmpdir = argv[7];
    n = G.n; ordE = G.ord;
    if (n > 32) { fprintf(stderr, "n > 32 nicht unterstuetzt\n"); return 2; }
    /* Vertreter */
    int rep[MAXO], rank[MAXO], nr = 0;
    for (int g = 0; g < ordE; g++) rank[g] = -1;
    for (int g = 0; g < ordE; g++) {
        int zg = G.tab[G.z][g];
        if (g < zg) { rank[g] = nr; rep[nr++] = g; }
    }
    if (nr != n || rep[0] != 0) { fprintf(stderr, "Vertreter: %d statt %d\n", nr, n); return 3; }
    for (int a = 0; a < n; a++) for (int b = 0; b < n; b++) {
        int e = G.tab[G.inv[rep[a]]][rep[b]];
        if (rank[e] >= 0) { idx_[a][b] = rank[e]; sgn_[a][b] = 1; }
        else { int ze = G.tab[G.z][e]; idx_[a][b] = rank[ze]; sgn_[a][b] = -1; }
    }
    for (int g = 0; g < n; g++) {
        negm[g] = 0;
        for (int k = 0; k < n; k++) mk[g][k] = 0;
        for (int h = 0; h < n; h++) { mk[g][idx_[g][h]] |= 1u << h; if (sgn_[g][h] < 0) negm[g] |= 1u << h; }
    }
    if (!strcmp(argv[2], "ZEILEN")) {
        /* Testmodus: F-Werte von stdin, Ausgabe der Zeilen (als Zahlen, Bit h = 1: Eintrag -1) nach dem allgemeinen Weg;
           "mask=ok", wenn der Maskenweg (wie im Sieb) dieselben Zeilen liefert */
        unsigned long long Fv;
        while (scanf("%llu", &Fv) == 1) {
            build_matrix(Fv);
            uint32_t r1[MAXO]; memcpy(r1, rows, sizeof(uint32_t) * n);
            int ok = 1;
            for (int g = 0; g < n; g++) {
                uint32_t rw = negm[g];
                for (int k = 1; k < n; k++) if (Fv >> (k - 1) & 1) rw ^= mk[g][k];
                if (rw != r1[g]) ok = 0;
            }
            printf("%llu", Fv);
            for (int g = 0; g < n; g++) printf(" %u", r1[g]);
            printf(" | mask=%s\n", ok ? "ok" : "ABWEICHUNG");
        }
        return 0;
    }
    if (!strcmp(argv[2], "GRAYTEST")) {
        /* Gray-Code-Fortschreibung gegen frischen Aufbau, die ersten (SHARD) Schritte */
        uint64_t steps = (uint64_t)atoll(argv[4]); long long bad = 0;
        uint32_t rr[MAXO];
        for (uint64_t i = 0; i < steps; i++) {
            uint64_t Fg = i ^ (i >> 1);
            if (i == 0) { for (int g = 0; g < n; g++) rr[g] = negm[g]; }
            else { int b = __builtin_ctzll(i); for (int g = 0; g < n; g++) rr[g] ^= mk[g][b + 1]; }
            if ((i & 1023) == 0 || i < 4096) {
                build_matrix(Fg);
                for (int g = 0; g < n; g++) if (rr[g] != rows[g]) { bad++; break; }
            }
        }
        printf("GRAYTEST %s: %llu Schritte, Abweichungen: %lld\n", argv[1], (unsigned long long)steps, bad);
        return 0;
    }
    N = 1 << (n - 1);
    uint64_t lo = (uint64_t)N * sh / nsh, hi = (uint64_t)N * (sh + 1) / nsh;
    rs ^= (uint64_t)(sh + 1) * 0xD1B54A32D192ED03ULL; for (int i = 0; i < 8; i++) rnd();
    char fn[1024];
    FILE *fb = NULL;
    if (Lc >= 0) { snprintf(fn, sizeof fn, "%s.shard%d.bin", prefix, sh); fb = fopen(fn, "wb"); if (!fb) { perror(fn); return 2; } }
    snprintf(fn, sizeof fn, "%s.shard%d.ueberlebende.txt", prefix, sh);
    FILE *fu = fopen(fn, "w"); if (!fu) { perror(fn); return 2; }
    long long cnt = 0, ohne_gegen = 0, nsurv = 0, vorfilter = 0;
    if (n > 24 || getenv("KOZYK_SIEB")) {
        /* Nur Sieb, Gray-Code ueber F: rows[g] ^= mk[g][k] beim Umdrehen von f(Vertreter k). */
        if (Lc >= 0 || Ls <= 0) { fprintf(stderr, "Siebpfad: nur Sieb (LCEIL = -1, LSIEB > 0)\n"); return 2; }
        for (uint64_t i = lo; i < hi; i++) {
            uint64_t Fg = i ^ (i >> 1);
            if (i == lo) {
                for (int g = 0; g < n; g++) {
                    uint32_t rw = negm[g];
                    for (int k = 1; k < n; k++) if (Fg >> (k - 1) & 1) rw ^= mk[g][k];
                    rows[g] = rw;
                }
            } else {
                int b = __builtin_ctzll(i);
                for (int g = 0; g < n; g++) rows[g] ^= mk[g][b + 1];
            }
            cnt++;
            uint32_t x;
            if (try_random(Ls, 20000, &x)) vorfilter++;
            else { nsurv++; fprintf(fu, "%llu\n", (unsigned long long)Fg); }
        }
        fclose(fu);
        printf("%s shard %d/%d: Funktionen %lld, Sieb <= %d: gefunden %lld, Ueberlebende %lld\n", argv[1], sh, nsh, cnt, Ls, vorfilter, nsurv);
        return 0;
    }
    for (uint64_t F = lo; F < hi; F++) {
        build_matrix(F);
        cnt++;
        uint32_t x;
        if (Lc >= 0) {
            int ok = try_random(Lc, 4000, &x);
            if (!ok) ok = (n <= 24) ? exists_le(Lc, &x) : 0;
            if (ok) { unsigned char b[3] = { x & 255, (x >> 8) & 255, (x >> 16) & 255 }; fwrite(b, 1, 3, fb); }
            else { ohne_gegen++; unsigned char b[3] = {255, 255, 255}; fwrite(b, 1, 3, fb); fprintf(fu, "OHNE_GEGENBEISPIEL %llu\n", (unsigned long long)F); }
        }
        if (Ls > 0) {
            int ok = try_random(Ls, 300, &x);
            if (ok) vorfilter++;
            else {
                if (n <= 24) ok = exists_le(Ls, &x);
                if (!ok) { nsurv++; fprintf(fu, "%llu\n", (unsigned long long)F); }
            }
        }
    }
    fclose(fu); if (fb) fclose(fb);
    printf("%s shard %d/%d: Funktionen %lld, ohne Gegenbeispiel (<= %d): %lld, Sieb <= %d: Vorfilter %lld, Ueberlebende %lld\n",
           argv[1], sh, nsh, cnt, Lc, ohne_gegen, Ls, vorfilter, nsurv);
    /* Stufe 2: Ueberlebende exakt */
    if (Ls > 0 && nsurv > 0 && n <= 24) {
        snprintf(fn, sizeof fn, "%s.shard%d.ueberlebende.txt", prefix, sh);
        FILE *fi = fopen(fn, "r");
        snprintf(fn, sizeof fn, "%s.shard%d.exakt.txt", prefix, sh);
        FILE *fe = fopen(fn, "w");
        char line[256]; long long hist[130] = {0}; long long had = 0, had_ge = 0;
        char tmpf[1024]; snprintf(tmpf, sizeof tmpf, "%s/kozyk_%s_%d.mat", tmpdir, argv[1], sh);
        while (fgets(line, sizeof line, fi)) {
            if (line[0] == 'O') continue;
            uint64_t F = strtoull(line, 0, 10);
            build_matrix(F);
            FILE *fm = fopen(tmpf, "w");
            for (int g = 0; g < n; g++) { for (int h = 0; h < n; h++) fprintf(fm, "%d ", (rows[g] >> h & 1) ? -1 : 1); fprintf(fm, "\n"); }
            fclose(fm);
            char cmd[2048]; snprintf(cmd, sizeof cmd, "build/exakt_disc %s", tmpf);
            FILE *p = popen(cmd, "r"); char out[512] = {0};
            if (p) { if (!fgets(out, sizeof out, p)) out[0] = 0; pclose(p); }
            int d = -1; char *q = strstr(out, "disc "); if (q) d = atoi(q + 5);
            int hd = is_hadamard();
            fprintf(fe, "%llu %d %d\n", (unsigned long long)F, d, hd);
            if (d >= 0 && d < 130) hist[d]++;
            if (hd) had++;
            if (hd && d >= Ls + 2) had_ge++;
        }
        fclose(fi); fclose(fe);
        printf("%s shard %d: exakt (build/exakt_disc) der Ueberlebenden:", argv[1], sh);
        for (int d = 0; d < 130; d++) if (hist[d]) printf(" disc %d: %lld;", d, hist[d]);
        printf(" Hadamard unter den Ueberlebenden: %lld\n", had);
    }
    return 0;
}
