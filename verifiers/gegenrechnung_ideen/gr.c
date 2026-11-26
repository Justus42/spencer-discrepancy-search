/* gr.c -- Auftrag C (Sitzung, 26.11.2026): Gegenrechnung der Ideenrunde vom 26.11. Pruefcode, kein Suchcode.
   Eigenes Programm; nichts aus notes/orchestrator uebernommen. Nur ganze Zahlen.

   Eingabe: Datei aus eingaben.py ("ANZAHL ZEILEN SPALTEN", dann die Matrizen), Eintraege -1/0/1, hoechstens 32 x 32.
   Konvention: x_0 = +1 fest (x und -x geben dieselben Betraege), "bis auf Vorzeichen" heisst also 2^(m-1) Vektoren.
   max|Ax| fuer jedes x: Gray-Code (gray_hist) oder Tiefensuche mit Schranke |s_i| - Rest > T (dfs_*).

   Unterbefehle (Ausgabe auf stdout):
     tab FILE c             je Matrix und c-Teilmenge gestrichener Spalten die exakte Diskrepanz (Gray-Code); Histogramm
     schwelle FILE c T [OUT] je c-Teilmenge: gibt es x mit max|s| <= T (Tiefensuche)? Zaehlt Teilmengen OHNE solches x
                            (dort disc >= T+1 bzw. T+2 nach Paritaet); OUT: Liste "Matrix Maske" dieser Teilmengen
     flach FILE k           Spalten 0..k-1 gestrichen; volles Histogramm von max|s| ueber alle x (Gray-Code)
     allein FILE LISTE d    fuer jede Zeile "Matrix Maske" der Liste: je Zeile i Zahl der x mit |s_i| >= d nur in Zeile i
     frei FILE c d SCHRITT [REST]  Spaltenteilmengen der Groesse c (jede SCHRITT-te, Index % SCHRITT == REST): Loecher (max|s| < d)
                            zaehlen, eine freie +-1-Zeile w suchen, die alle Loecher deckt (|<w,x>| >= d)
     freizeile FILE d       Nullspalten weg, je eine Zeile gestrichen: Loecher zaehlen, freie Zeile suchen (Positivkontrolle)
     ersatz1 FILE           Hadamard ohne Spalte j, neue Spalte c: flache x' (max|b| <= N/2-3), Bedingungen, gute c
     ersatz2 FILE           Hadamard ohne Spalten a<b, zwei neue Spalten: flache x'', Bedingungen je Zeilengruppe, Belegungen
     zirk n                 alle zirkulanten +-1-Matrizen n x n (erste Zeile mit c_0 = +1): Verteilung der exakten Diskrepanz
   Bauen: gcc -O3 -march=native -fopenmp -o build/gr verifiers/gegenrechnung_ideen/gr.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

#define MR 32
typedef struct { int anz, z, s; int8_t *a; } Datei;

static Datei lese(const char *p) {
    Datei d; FILE *f = fopen(p, "r");
    if (!f) { perror(p); exit(2); }
    if (fscanf(f, "%d %d %d", &d.anz, &d.z, &d.s) != 3) { fprintf(stderr, "Kopf\n"); exit(2); }
    if (d.z > MR || d.s > MR) { fprintf(stderr, "zu gross\n"); exit(2); }
    d.a = malloc((size_t)d.anz * d.z * d.s);
    for (long t = 0; t < (long)d.anz * d.z * d.s; t++) {
        int v; if (fscanf(f, "%d", &v) != 1 || v < -1 || v > 1) { fprintf(stderr, "Eintrag %ld\n", t); exit(2); }
        d.a[t] = (int8_t)v;
    }
    fclose(f); return d;
}
#define A(d,k,i,j) ((d).a[((long)(k)*(d).z + (i))*(d).s + (j)])

typedef struct {
    int m, T, z;
    int8_t col[MR][MR] __attribute__((aligned(32)));    /* col[j][i] */
    int8_t col2[MR][MR] __attribute__((aligned(32)));   /* 2*col */
    int8_t st[MR + 1][MR] __attribute__((aligned(32)));
    /* Aufzaehlung */
    uint32_t *xs; int8_t (*ss)[MR]; long nx, cap; int speichere_s;
} Ctx;

static void setze(Ctx *c, const Datei *d, int k, const int *spalten, int m) {
    memset(c->col, 0, sizeof c->col); memset(c->col2, 0, sizeof c->col2);
    c->m = m; c->z = d->z;
    for (int j = 0; j < m; j++) for (int i = 0; i < d->z; i++) {
        c->col[j][i] = A(*d, k, i, spalten[j]); c->col2[j][i] = 2 * c->col[j][i];
    }
}

static inline int maxabs(const int8_t *s) {
    int mx = 0;
    for (int i = 0; i < MR; i++) { int v = s[i] < 0 ? -s[i] : s[i]; mx = v > mx ? v : mx; }
    return mx;
}

/* Histogramm h[v] = Zahl der x (x_0 = +1) mit max|s| = v */
static void gray_hist(const Ctx *c, long *h) {
    int8_t s[MR] __attribute__((aligned(32)));
    for (int i = 0; i < MR; i++) { int t = 0; for (int j = 0; j < c->m; j++) t += c->col[j][i]; s[i] = (int8_t)t; }
    for (int v = 0; v <= MR; v++) h[v] = 0;
    uint32_t x = 0; h[maxabs(s)]++;
    uint64_t n = 1ULL << (c->m - 1);
    for (uint64_t t = 1; t < n; t++) {
        int j = __builtin_ctzll(t) + 1; const int8_t *q = c->col2[j];
        if ((x >> j) & 1) { for (int i = 0; i < MR; i++) s[i] += q[i]; }
        else { for (int i = 0; i < MR; i++) s[i] -= q[i]; }
        x ^= 1u << j; h[maxabs(s)]++;
    }
}

/* Tiefensuche: gibt es x mit max|s| <= T? Spalte 0 fest +1. */
static int dfs_ex(Ctx *c, int k) {
    if (k == c->m) return 1;
    int rest = c->m - k - 1; int8_t *s = c->st[k], *t = c->st[k + 1]; const int8_t *q = c->col[k];
    int dot = 0; for (int i = 0; i < MR; i++) dot += s[i] * q[i];
    int erst = dot > 0 ? -1 : 1;
    for (int r = 0; r < 2; r++) {
        int sg = r == 0 ? erst : -erst;
        if (k == 0 && sg != 1) continue;
        for (int i = 0; i < MR; i++) t[i] = s[i] + sg * q[i];
        if (maxabs(t) <= c->T + rest && dfs_ex(c, k + 1)) return 1;
    }
    return 0;
}
static int existiert(Ctx *c, int T) { c->T = T; memset(c->st[0], 0, MR); return dfs_ex(c, 0); }

/* alle x mit max|s| <= T aufzaehlen (x_0 = +1); Bit j = 1 heisst x_j = -1 */
static void dfs_auf(Ctx *c, int k, uint32_t x) {
    if (k == c->m) {
        if (c->nx == c->cap) {
            c->cap = c->cap ? 2 * c->cap : 4096;
            c->xs = realloc(c->xs, c->cap * sizeof *c->xs);
            if (c->speichere_s) c->ss = realloc(c->ss, c->cap * sizeof *c->ss);
        }
        c->xs[c->nx] = x; if (c->speichere_s) memcpy(c->ss[c->nx], c->st[k], MR);
        c->nx++; return;
    }
    int rest = c->m - k - 1; int8_t *s = c->st[k], *t = c->st[k + 1]; const int8_t *q = c->col[k];
    for (int sg = 1; sg >= -1; sg -= 2) {
        if (k == 0 && sg != 1) continue;
        for (int i = 0; i < MR; i++) t[i] = s[i] + sg * q[i];
        if (maxabs(t) <= c->T + rest) dfs_auf(c, k + 1, sg < 0 ? x | (1u << k) : x);
    }
}
static void aufzaehlen(Ctx *c, int T) { c->T = T; c->nx = 0; memset(c->st[0], 0, MR); dfs_auf(c, 0, 0); }

/* Teilmengen der Groesse c von n als Masken */
static long teilmengen(int n, int c, uint32_t **out) {
    long cap = 1024, cnt = 0; uint32_t *v = malloc(cap * sizeof *v);
    int idx[MR]; for (int i = 0; i < c; i++) idx[i] = i;
    for (;;) {
        uint32_t mk = 0; for (int i = 0; i < c; i++) mk |= 1u << idx[i];
        if (cnt == cap) { cap *= 2; v = realloc(v, cap * sizeof *v); }
        v[cnt++] = mk;
        int i = c - 1; while (i >= 0 && idx[i] == n - c + i) i--;
        if (i < 0) break;
        idx[i]++; for (int j = i + 1; j < c; j++) idx[j] = idx[j - 1] + 1;
    }
    *out = v; return cnt;
}
static int behalten(int n, uint32_t weg, int *sp) { int m = 0; for (int j = 0; j < n; j++) if (!((weg >> j) & 1)) sp[m++] = j; return m; }

/* ---------- tab ---------- */
static void cmd_tab(const char *fn, int c) {
    Datei d = lese(fn); uint32_t *tm; long nt = teilmengen(d.s, c, &tm);
    long ges[MR + 1] = {0};
    for (int k = 0; k < d.anz; k++) {
        long hd[MR + 1] = {0};
        #pragma omp parallel for schedule(dynamic, 1)
        for (long t = 0; t < nt; t++) {
            Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
            int sp[MR]; int m = behalten(d.s, tm[t], sp); setze(cx, &d, k, sp, m);
            long h[MR + 1]; gray_hist(cx, h); int disc = 0; while (h[disc] == 0) disc++;
            #pragma omp atomic
            hd[disc]++;
            free(cx);
        }
        printf("Matrix %d, c = %d (%ld Teilmengen):", k, c, nt);
        for (int v = 0; v <= MR; v++) if (hd[v]) { printf(" disc %d: %ld", v, hd[v]); ges[v] += hd[v]; }
        printf("\n");
    }
    printf("GESAMT c = %d:", c);
    for (int v = 0; v <= MR; v++) if (ges[v]) printf(" disc %d: %ld", v, ges[v]);
    printf("\n");
}

/* ---------- schwelle ---------- */
static void cmd_schwelle(const char *fn, int c, int T, const char *out) {
    Datei d = lese(fn); uint32_t *tm; long nt = teilmengen(d.s, c, &tm);
    FILE *fo = out ? fopen(out, "w") : NULL;
    long gesamt = 0, ohne = 0;
    for (int k = 0; k < d.anz; k++) {
        char *erg = calloc(nt, 1);
        #pragma omp parallel
        {
            Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
            #pragma omp for schedule(dynamic, 64)
            for (long t = 0; t < nt; t++) {
                int sp[MR]; int m = behalten(d.s, tm[t], sp); setze(cx, &d, k, sp, m);
                erg[t] = !existiert(cx, T);
            }
            free(cx);
        }
        long z = 0; for (long t = 0; t < nt; t++) if (erg[t]) { z++; if (fo) fprintf(fo, "%d %08x\n", k, tm[t]); }
        printf("Matrix %d: %ld von %ld Teilmengen ohne x mit max|s| <= %d\n", k, z, nt, T);
        fflush(stdout); gesamt += nt; ohne += z; free(erg);
    }
    printf("GESAMT c = %d, T = %d: %ld von %ld Faellen ohne x mit max|s| <= %d\n", c, T, ohne, gesamt, T);
    if (fo) fclose(fo);
}

/* ---------- flach ---------- */
static void cmd_flach(const char *fn, int k0) {
    Datei d = lese(fn);
    for (int k = 0; k < d.anz; k++) {
        Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
        int sp[MR]; int m = 0; for (int j = k0; j < d.s; j++) sp[m++] = j; setze(cx, &d, k, sp, m);
        long h[MR + 1]; gray_hist(cx, h);
        printf("Matrix %d, Spalten 0..%d gestrichen, m = %d, 2^%d Vektoren: max|s| =", k, k0 - 1, m, m - 1);
        long kum = 0;
        for (int v = 0; v <= MR; v++) if (h[v]) { kum += h[v]; printf(" %d:%ld(kum %ld)", v, h[v], kum); }
        int disc = 0; while (h[disc] == 0) disc++;
        printf("\n  exakte Diskrepanz %d\n", disc); free(cx);
    }
}

/* ---------- allein ---------- */
static void cmd_allein(const char *fn, const char *liste, int dd) {
    Datei d = lese(fn); FILE *f = fopen(liste, "r"); if (!f) { perror(liste); exit(2); }
    long cap = 1024, n = 0; int *mk = malloc(cap * sizeof *mk); uint32_t *ms = malloc(cap * sizeof *ms);
    int a; unsigned b;
    while (fscanf(f, "%d %x", &a, &b) == 2) {
        if (n == cap) { cap *= 2; mk = realloc(mk, cap * sizeof *mk); ms = realloc(ms, cap * sizeof *ms); }
        mk[n] = a; ms[n] = b; n++;
    }
    fclose(f);
    long *mn = malloc(n * sizeof *mn), *mx = malloc(n * sizeof *mx), *off = malloc(n * sizeof *off);
    #pragma omp parallel
    {
        Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
        #pragma omp for schedule(dynamic, 16)
        for (long t = 0; t < n; t++) {
            int sp[MR]; int m = behalten(d.s, ms[t], sp); setze(cx, &d, mk[t], sp, m);
            long cnt[MR] = {0}, unbedeckt = 0;
            int8_t s[MR] __attribute__((aligned(32)));
            for (int i = 0; i < MR; i++) { int u = 0; for (int j = 0; j < m; j++) u += cx->col[j][i]; s[i] = (int8_t)u; }
            uint32_t x = 0; uint64_t N = 1ULL << (m - 1);
            for (uint64_t g = 0; g < N; g++) {
                if (g) {
                    int j = __builtin_ctzll(g) + 1; const int8_t *q = cx->col2[j];
                    if ((x >> j) & 1) { for (int i = 0; i < MR; i++) s[i] += q[i]; }
                    else { for (int i = 0; i < MR; i++) s[i] -= q[i]; }
                    x ^= 1u << j;
                }
                int z = 0; for (int i = 0; i < MR; i++) { int v = s[i] < 0 ? -s[i] : s[i]; z += v >= dd; }
                if (z == 1) { for (int i = 0; i < MR; i++) { int v = s[i] < 0 ? -s[i] : s[i]; if (v >= dd) { cnt[i]++; break; } } }
                else if (z == 0) unbedeckt++;
            }
            long lo = 1L << 40, hi = 0; for (int i = 0; i < d.z; i++) { if (cnt[i] < lo) lo = cnt[i]; if (cnt[i] > hi) hi = cnt[i]; }
            mn[t] = lo; mx[t] = hi; off[t] = unbedeckt;
        }
        free(cx);
    }
    long lo = 1L << 40, hi = 0, l0 = 1L << 40, h0 = 0, n0 = 0, offen = 0;
    for (long t = 0; t < n; t++) {
        if (mn[t] < lo) lo = mn[t]; if (mx[t] > hi) hi = mx[t]; offen += off[t];
        if (mk[t] == 0) { n0++; if (mn[t] < l0) l0 = mn[t]; if (mx[t] > h0) h0 = mx[t]; }
    }
    printf("%ld Faelle, d = %d: Alleinanteil je Zeile zwischen %ld und %ld; unbedeckte x gesamt %ld\n", n, dd, lo, hi, offen);
    printf("Matrix 0 (%ld Faelle): Alleinanteil zwischen %ld und %ld\n", n0, l0, h0);
}

/* ---------- frei ---------- */
typedef struct { uint32_t *h; long n; int m, R; } Loecher;
static int deckt(const Loecher *L, uint32_t w) {
    for (long i = 0; i < L->n; i++) {
        int dist = __builtin_popcount(w ^ L->h[i]);
        if (dist > L->R && dist < L->m - L->R) {
            if (i > 1) { uint32_t t = L->h[i]; L->h[i] = L->h[1]; L->h[1] = t; } /* Bremser nach vorn */
            return 0;
        }
    }
    return 1;
}
static int kugel(const Loecher *L, uint32_t w, int ab, int rest, uint32_t *gef) {
    if (deckt(L, w)) { *gef = w; return 1; }
    if (rest == 0) return 0;
    for (int j = ab; j < L->m; j++) if (kugel(L, w ^ (1u << j), j + 1, rest - 1, gef)) return 1;
    return 0;
}
/* gibt 1 zurueck, wenn eine freie Zeile alle Loecher deckt; WLOG d(w, h0) <= R */
static int freie_zeile(Ctx *cx, int dd, long *nl, uint32_t *wgef) {
    aufzaehlen(cx, dd - 1); *nl = cx->nx;
    if (cx->nx == 0) { *wgef = 0; return 1; }
    if ((cx->m - dd) % 2) { fprintf(stderr, "m - d ungerade\n"); exit(2); }
    Loecher L = { cx->xs, cx->nx, cx->m, (cx->m - dd) / 2 };
    return kugel(&L, cx->xs[0], 0, L.R, wgef);
}
static void cmd_frei(const char *fn, int c, int dd, long schritt, long rest) {
    Datei d = lese(fn); uint32_t *tm; long nt = teilmengen(d.s, c, &tm);
    long faelle = 0, loesbar = 0, lmin = 1L << 40, lmax = 0;
    for (int k = 0; k < d.anz; k++) {
        long f = 0, l = 0, a = 1L << 40, b = 0;
        #pragma omp parallel reduction(+:f,l) reduction(min:a) reduction(max:b)
        {
            Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
            #pragma omp for schedule(dynamic, 16)
            for (long t = 0; t < nt; t++) {
                long idx = (long)k * nt + t; if (idx % schritt != rest) continue;
                int sp[MR]; int m = behalten(d.s, tm[t], sp); setze(cx, &d, k, sp, m);
                long nl; uint32_t w; int ok = freie_zeile(cx, dd, &nl, &w);
                f++; l += ok; if (nl < a) a = nl; if (nl > b) b = nl;
                if (ok) {
                    #pragma omp critical
                    printf("  LOESBAR Matrix %d Maske %08x Loecher %ld w %08x\n", k, tm[t], nl, w);
                }
            }
            free(cx->xs); free(cx);
        }
        faelle += f; loesbar += l; if (a < lmin) lmin = a; if (b > lmax) lmax = b;
        printf("Matrix %d: %ld Faelle, Loecher %ld..%ld, loesbar %ld\n", k, f, a, b, l); fflush(stdout);
    }
    printf("GESAMT c = %d, d = %d, Schritt %ld Rest %ld: %ld Faelle, Loecher (bis auf Vorzeichen) %ld..%ld, mit einer freien Zeile loesbar: %ld\n",
           c, dd, schritt, rest, faelle, lmin, lmax, loesbar);
}
static void cmd_freizeile(const char *fn, int dd) {
    Datei d = lese(fn);
    for (int k = 0; k < d.anz; k++) {
        int sp[MR], m = 0; for (int j = 0; j < d.s; j++) { int nz = 0; for (int i = 0; i < d.z; i++) nz |= A(d, k, i, j) != 0; if (nz) sp[m++] = j; }
        int ok = 0;
        for (int r = 0; r < d.z; r++) {
            Datei e = d; e.z = d.z - 1; e.anz = 1; e.a = malloc((size_t)e.z * d.s);
            for (int i = 0, ii = 0; i < d.z; i++) if (i != r) { memcpy(e.a + (long)ii * d.s, d.a + ((long)k * d.z + i) * d.s, d.s); ii++; }
            Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx); setze(cx, &e, 0, sp, m);
            long nl; uint32_t w; int l = freie_zeile(cx, dd, &nl, &w); ok += l;
            printf("Matrix %d ohne Zeile %d: m = %d, Loecher %ld, freie Zeile %s\n", k, r, m, nl, l ? "gefunden" : "nicht gefunden");
            free(cx->xs); free(cx); free(e.a);
        }
        printf("Matrix %d: %d von %d loesbar\n", k, ok, d.z);
    }
}

/* ---------- ersatz1 / ersatz2 ---------- */
typedef struct { uint32_t S, p; } Bed;
static int bedcmp(const void *a, const void *b) {
    const Bed *x = a, *y = b;
    if (x->S != y->S) return x->S < y->S ? -1 : 1;
    if (x->p != y->p) return x->p < y->p ? -1 : 1;
    return 0;
}
static long eindeutig(Bed *v, long n) {
    qsort(v, n, sizeof *v, bedcmp); long u = 0;
    for (long i = 0; i < n; i++) if (u == 0 || bedcmp(&v[i], &v[u - 1])) v[u++] = v[i];
    return u;
}
static void cmd_ersatz1(const char *fn) {
    Datei d = lese(fn); int N = d.z; int T = N / 2 - 3; /* N = 20: flach heisst max|b| <= 7 */
    long fall = 0, alle_null = 1;
    for (int k = 0; k < d.anz; k++) for (int j = 0; j < d.s; j++) {
        Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
        int sp[MR]; int m = behalten(d.s, 1u << j, sp); setze(cx, &d, k, sp, m); cx->speichere_s = 1;
        aufzaehlen(cx, T);
        Bed *b = malloc(cx->nx * sizeof *b); long nb = 0, leer = 0;
        for (long t = 0; t < cx->nx; t++) {
            uint32_t S = 0, p = 0;
            for (int i = 0; i < N; i++) { int v = cx->ss[t][i]; if (v == T || v == -T) { S |= 1u << i; if (v < 0) p |= 1u << i; } }
            if (!S) leer++;
            uint32_t lo = S & -S; if (p & lo) p ^= S;   /* Muster bis auf Vorzeichen */
            b[nb].S = S; b[nb].p = p; nb++;
        }
        long nu = eindeutig(b, nb); long gr[MR + 1] = {0};
        for (long t = 0; t < nu; t++) gr[__builtin_popcount(b[t].S)]++;
        long gut = 0;
        #pragma omp parallel for reduction(+:gut) schedule(static, 4096)
        for (long cc = 0; cc < (1L << N); cc++) {
            uint32_t c = (uint32_t)cc; int ok = 1;
            for (long t = 0; t < nu && ok; t++) { uint32_t q = c & b[t].S; if (q == b[t].p || q == (b[t].p ^ b[t].S)) ok = 0; }
            gut += ok;
        }
        printf("Matrix %d ohne Spalte %2d: %ld flache x', %ld ohne kritische Zeile, %ld Bedingungen (", k, j, cx->nx, leer, nu);
        for (int g = 0; g <= MR; g++) if (gr[g]) printf(" Groesse %d: %ld", g, gr[g]);
        printf(" ), gute Ersatzspalten (von 2^%d): %ld\n", N, gut);
        fall++; if (gut) alle_null = 0;
        free(b); free(cx->xs); free(cx->ss); free(cx);
    }
    printf("GESAMT %ld Faelle, gute Ersatzspalten in allen Faellen 0: %s\n", fall, alle_null ? "ja" : "NEIN");
}
static void cmd_ersatz2(const char *fn) {
    Datei d = lese(fn); int N = d.z; int T = N / 2 - 4; /* N = 20: max|b| <= 6 */
    long paare = 0, ausgeschl = 0, fmin = 1L << 40, fmax = 0, bmin = 1L << 40, bmax = 0, quer = 0;
    for (int k = 0; k < d.anz; k++) for (int a = 0; a < d.s; a++) for (int bb = a + 1; bb < d.s; bb++) {
        Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
        int sp[MR]; int m = behalten(d.s, (1u << a) | (1u << bb), sp); setze(cx, &d, k, sp, m); cx->speichere_s = 1;
        aufzaehlen(cx, T);
        uint32_t G1 = 0; for (int i = 0; i < N; i++) if (A(d, k, i, a) * A(d, k, i, bb) > 0) G1 |= 1u << i;
        uint32_t Gr[2] = { G1, ((1u << N) - 1) & ~G1 };
        Bed *b[2]; long nb[2] = {0, 0}; b[0] = malloc(cx->nx * sizeof(Bed)); b[1] = malloc(cx->nx * sizeof(Bed));
        for (long t = 0; t < cx->nx; t++) {
            uint32_t S = 0, p = 0;
            for (int i = 0; i < N; i++) { int v = cx->ss[t][i]; if (v == T || v == -T) { S |= 1u << i; if (v < 0) p |= 1u << i; } }
            int g = (S & Gr[0]) == S ? 0 : ((S & Gr[1]) == S ? 1 : -1);
            if (g < 0 || S == 0) { quer++; continue; }
            uint32_t lo = S & -S; if (p & lo) p ^= S;
            b[g][nb[g]].S = S; b[g][nb[g]].p = p; nb[g]++;
        }
        int leer_gruppe = 0; long bel[2];
        for (int g = 0; g < 2; g++) {
            long nu = eindeutig(b[g], nb[g]); nb[g] = nu;
            if (nu < bmin) bmin = nu; if (nu > bmax) bmax = nu;
            /* Gruppe auf lokale Bits abbilden */
            int idx[MR], ng = 0; for (int i = 0; i < N; i++) if ((Gr[g] >> i) & 1) idx[ng++] = i;
            Bed *L = malloc(nu * sizeof *L);
            for (long t = 0; t < nu; t++) {
                uint32_t S = 0, p = 0;
                for (int r = 0; r < ng; r++) { if ((b[g][t].S >> idx[r]) & 1) S |= 1u << r; if ((b[g][t].p >> idx[r]) & 1) p |= 1u << r; }
                L[t].S = S; L[t].p = p;
            }
            long z = 0;
            #pragma omp parallel for reduction(+:z) schedule(static, 16)
            for (long vw = 0; vw < (1L << (2 * ng)); vw++) {
                uint32_t v = (uint32_t)vw & ((1u << ng) - 1), w = (uint32_t)(vw >> ng); int ok = 1;
                for (long t = 0; t < nu && ok; t++) {
                    uint32_t S = L[t].S, P = (v ^ L[t].p) & S, Q = (w ^ L[t].p) & S;
                    if (!(~P & ~Q & S) || !(~P & Q) || !(P & ~Q) || !(P & Q)) ok = 0;
                }
                z += ok;
            }
            bel[g] = z; if (z == 0) leer_gruppe = 1; free(L);
        }
        paare++; ausgeschl += leer_gruppe;
        if (cx->nx < fmin) fmin = cx->nx; if (cx->nx > fmax) fmax = cx->nx;
        printf("Matrix %d Spalten %2d,%2d: %ld flache x'', Bedingungen %ld / %ld, Belegungen %ld / %ld%s\n", k, a, bb, cx->nx,
               nb[0], nb[1], bel[0], bel[1], leer_gruppe ? "" : "  UEBERLEBT");
        free(b[0]); free(b[1]); free(cx->xs); free(cx->ss); free(cx);
    }
    printf("GESAMT %ld Paare, flache x'' %ld..%ld, Bedingungen je Gruppe %ld..%ld, x'' mit S ueber beide Gruppen oder leer: %ld, ausgeschlossen %ld\n",
           paare, fmin, fmax, bmin, bmax, quer, ausgeschl);
}

/* ---------- zirk ---------- */
static void cmd_zirk(int n) {
    long hd[MR + 1] = {0}; long N = 1L << (n - 1);
    #pragma omp parallel
    {
        Ctx *cx = aligned_alloc(32, sizeof(Ctx)); memset(cx, 0, sizeof *cx);
        long lok[MR + 1] = {0};
        #pragma omp for schedule(dynamic, 64)
        for (long r = 0; r < N; r++) {
            int c[MR]; c[0] = 1; for (int j = 1; j < n; j++) c[j] = ((r >> (j - 1)) & 1) ? -1 : 1;
            memset(cx->col, 0, sizeof cx->col); memset(cx->col2, 0, sizeof cx->col2); cx->m = n; cx->z = n;
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) { cx->col[j][i] = c[((j - i) % n + n) % n]; cx->col2[j][i] = 2 * cx->col[j][i]; }
            long h[MR + 1]; gray_hist(cx, h); int disc = 0; while (h[disc] == 0) disc++;
            lok[disc]++;
        }
        #pragma omp critical
        for (int v = 0; v <= MR; v++) hd[v] += lok[v];
        free(cx);
    }
    printf("Zirkulant n = %d, %ld erste Zeilen (c_0 = +1):", n, N);
    int best = 0; for (int v = 0; v <= MR; v++) if (hd[v]) { printf(" disc %d: %ld", v, hd[v]); best = v; }
    printf("\n  groesste Diskrepanz %d\n", best);
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "siehe Kopf\n"); return 2; }
    const char *b = argv[1];
    if (!strcmp(b, "tab") && argc == 4) cmd_tab(argv[2], atoi(argv[3]));
    else if (!strcmp(b, "schwelle") && argc >= 5) cmd_schwelle(argv[2], atoi(argv[3]), atoi(argv[4]), argc > 5 ? argv[5] : NULL);
    else if (!strcmp(b, "flach") && argc == 4) cmd_flach(argv[2], atoi(argv[3]));
    else if (!strcmp(b, "allein") && argc == 5) cmd_allein(argv[2], argv[3], atoi(argv[4]));
    else if (!strcmp(b, "frei") && argc >= 6) cmd_frei(argv[2], atoi(argv[3]), atoi(argv[4]), atol(argv[5]), argc > 6 ? atol(argv[6]) : 0);
    else if (!strcmp(b, "freizeile") && argc == 4) cmd_freizeile(argv[2], atoi(argv[3]));
    else if (!strcmp(b, "ersatz1") && argc == 3) cmd_ersatz1(argv[2]);
    else if (!strcmp(b, "ersatz2") && argc == 3) cmd_ersatz2(argv[2]);
    else if (!strcmp(b, "zirk") && argc == 3) cmd_zirk(atoi(argv[2]));
    else { fprintf(stderr, "unbekannter Aufruf\n"); return 2; }
    return 0;
}
