/* zerl_halb.c -- SUCHCODE Auftrag H (25.11.2026): Kern-<1,a>-Codes als Paar (U,W), Halbproblem-Loeser.
 *
 * Paar (U,W): U ist K x m1, W ist K x m2, Eintraege +1/-1. Woerter (U_i,W_i), (U_i,-W_i) und Komplemente.
 * Mit u = U y, v = W z:  Ueberdeckung mit d  <=>  fuer alle y, z:  max_i (|u_i| + |v_i|) >= d.
 * Die Bedingung ist symmetrisch in U und W. Hier heisst die feste Haelfte F (K x mf), die gesuchte G (K x mg).
 * Je Punkt x der festen Seite (x_0 = +1) ist r_i(x) = d - |F_i x| der Anforderungsvektor an G; x ist erfuellt im Punkt z,
 * wenn es ein i mit |G_i z| >= r_i gibt. Kosten = Zahl offener Klassen (x,z) (mit Vielfachheiten, exakt).
 *
 * Modi:
 *   ls    FEST d mg [--seed s] [--zeit sek] [--aus DATEI] [--fest-rechts] [--start PAAR]
 *         Tabu-Suche ueber ganze Zeilenwoerter von G. Ausgabe "U | W" (F links, ausser --fest-rechts).
 *   dfs   FEST d mg [--zeit sek] [--aus DATEI] [--fest-rechts]
 *         vollstaendige Tiefensuche: nur maximale Anforderungsvektoren; Symmetriebrechung ueber Spaltenvorzeichen
 *         (erste belegte Zeile = lauter +1) und Spaltenreihenfolge (Intervall-Klassen gleicher Spalten);
 *         Schranke je Anforderungsvektor: offene z <= Summe der Kugelvolumina der freien Zeilen.
 *         Meldet LOESUNG, KEINE LOESUNG (vollstaendig) oder ZEITGRENZE.
 *   brute FEST d mg    alle (2^(mg-1))^K Belegungen ohne Symmetrie (nur fuer kleine Tests der dfs).
 *   alt   K m1 m2 d [--seed s] [--zeit sek] [--aus DATEI] [--start PAAR] [--phase n]
 *         abwechselnde Suche aus Zufallsstart: U fest, W per Tabu verbessern; W fest, U verbessern; usw.
 * Dateien: FEST = Matrix (Zeilen +-1, Leerraum). PAAR = Zeilen "U_i | W_i".
 * Bauen: gcc -O3 -march=native -o build/zerl_halb src/zerl_halb.c
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef uint64_t u64;
#define MAXK 16
#define MAXM 16

/* ---------- Zufall ---------- */
static u64 rs = 88172645463325252ULL;
static inline u64 rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }
static inline int rndi(int n) { return (int)(rnd() % (u64)n); }

static double t0;
static double jetzt(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + 1e-9 * ts.tv_nsec; }
static double zeitgrenze = 1e18;
static int ausgabe_leise = 0;

/* ---------- Problem ---------- */
static int K, mf, mg, d;
static uint32_t Ffix[MAXK];
static int NZ, NX, gmin, L;
static int P, NW;
static int8_t *R = NULL;   /* P x K */
static int64_t *Wt = NULL; /* P */
static int alle_eins;      /* alle Gewichte 1 */
static u64 *T = NULL;      /* K x L x NW */
static u64 lastmask;
static int volL[MAXM + 3];
static int unerfuellbar;

static inline int lvlidx(uint32_t c, uint32_t zm) {
    int s = mg - 2 * __builtin_popcount(c ^ zm);
    if (s < 0) s = -s;
    return (s - gmin) >> 1;
}
static inline int volr(int r) {
    if (r <= gmin) return NZ;
    if (r > mg) return 0;
    return volL[(r - gmin + 1) / 2];
}

static int cmpK(const void *a, const void *b) { return memcmp(a, b, (size_t)K); }

static void baue_anforderungen(int nur_maximal) {
    NX = 1 << (mf - 1);
    NZ = 1 << (mg - 1);
    gmin = mg & 1;
    L = (mg - gmin) / 2 + 1;
    int8_t *buf = malloc((size_t)NX * K);
    int n = 0;
    for (int x = 0; x < NX; x++) {
        uint32_t xm = (uint32_t)x << 1;
        int8_t r[MAXK];
        int krit = 1;
        for (int i = 0; i < K; i++) {
            int a = mf - 2 * __builtin_popcount(Ffix[i] ^ xm);
            if (a < 0) a = -a;
            int ri = d - a;
            if (ri <= gmin) { krit = 0; break; }
            if (ri > mg) ri = mg + 1;
            r[i] = (int8_t)ri;
        }
        if (krit) { memcpy(buf + (size_t)n * K, r, K); n++; }
    }
    qsort(buf, n, K, cmpK);
    free(R); free(Wt);
    R = malloc((size_t)(n + 1) * K);
    Wt = malloc(sizeof(int64_t) * (n + 1));
    P = 0;
    for (int a = 0; a < n; a++) {
        if (P > 0 && memcmp(R + (size_t)(P - 1) * K, buf + (size_t)a * K, K) == 0) { Wt[P - 1]++; continue; }
        memcpy(R + (size_t)P * K, buf + (size_t)a * K, K);
        Wt[P] = 1;
        P++;
    }
    free(buf);
    if (nur_maximal && P > 1) {
        char *weg = calloc(P, 1);
        for (int a = 0; a < P; a++) {
            for (int b = 0; b < P && !weg[a]; b++) {
                if (a == b || weg[b]) continue;
                int dom = 1; /* b >= a punktweise, b != a  -> a dominiert (leichter) */
                for (int i = 0; i < K; i++) if (R[(size_t)b * K + i] < R[(size_t)a * K + i]) { dom = 0; break; }
                if (dom) weg[a] = 1;
            }
        }
        int q = 0;
        for (int a = 0; a < P; a++) if (!weg[a]) { memmove(R + (size_t)q * K, R + (size_t)a * K, K); Wt[q] = 1; q++; }
        P = q;
        free(weg);
    }
    alle_eins = 1;
    for (int p = 0; p < P; p++) if (Wt[p] != 1) alle_eins = 0;
    NW = (P + 63) / 64;
    if (NW == 0) NW = 1;
    lastmask = (P % 64 == 0) ? ~0ULL : ((1ULL << (P % 64)) - 1);
    if (P == 0) lastmask = 0;
    free(T);
    T = calloc((size_t)K * L * NW, sizeof(u64));
    for (int i = 0; i < K; i++)
        for (int l = 0; l < L; l++)
            for (int p = 0; p < P; p++)
                if (R[(size_t)p * K + i] <= gmin + 2 * l) T[((size_t)i * L + l) * NW + p / 64] |= 1ULL << (p % 64);
    int h[MAXM + 3] = {0};
    for (int z = 0; z < NZ; z++) h[lvlidx(0, (uint32_t)z << 1)]++;
    volL[L] = 0;
    for (int l = L - 1; l >= 0; l--) volL[l] = volL[l + 1] + h[l];
    unerfuellbar = 0;
    for (int p = 0; p < P; p++) {
        int ok = 0;
        for (int i = 0; i < K; i++) if (R[(size_t)p * K + i] <= mg) ok = 1;
        if (!ok) unerfuellbar++;
    }
}

static inline int64_t wzahl_offen(const u64 *X) { /* gewichtete Zahl der p nicht in X */
    int64_t s = 0;
    for (int w = 0; w < NW; w++) {
        u64 x = ~X[w] & (w == NW - 1 ? lastmask : ~0ULL);
        if (alle_eins) s += __builtin_popcountll(x);
        else while (x) { int b = __builtin_ctzll(x); s += Wt[w * 64 + b]; x &= x - 1; }
    }
    return s;
}
static inline int64_t wzahl_offen_g(const u64 *X, const int64_t *g) { /* mit Gewichten je p (Zeile z der Gewichtstafel) */
    int64_t s = 0;
    for (int w = 0; w < NW; w++) {
        u64 x = ~X[w] & (w == NW - 1 ? lastmask : ~0ULL);
        while (x) { int b = __builtin_ctzll(x); s += g[w * 64 + b]; x &= x - 1; }
    }
    return s;
}
static int64_t *Wz = NULL;   /* dynamische Gewichte je (z,p), nur Verfahren "gewicht" */
static int verfahren = 0;    /* 0 = Tabu, 1 = dynamische Gewichte */
static int64_t glatt = 0;    /* Glaettung alle `glatt` Erhoehungen (0 = aus) */
static int64_t erhoehungen = 0;

/* ---------- Dateien ---------- */
static int lies_matrix(const char *pfad, uint32_t *rows, int *k, int *m) {
    FILE *f = fopen(pfad, "r");
    if (!f) { perror(pfad); exit(2); }
    char line[4096];
    *k = 0; *m = -1;
    while (fgets(line, sizeof line, f)) {
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '#' || *s == '\n' || *s == 0) continue;
        int c = 0; uint32_t bits = 0; char *e;
        for (;;) {
            long v = strtol(s, &e, 10);
            if (e == s) break;
            if (v != 1 && v != -1) { fprintf(stderr, "%s: Eintrag %ld nicht +-1\n", pfad, v); exit(2); }
            if (v == -1) bits |= 1u << c;
            c++; s = e;
        }
        if (*m < 0) *m = c;
        if (c != *m) { fprintf(stderr, "%s: ungleiche Zeilenlaengen\n", pfad); exit(2); }
        rows[(*k)++] = bits;
    }
    fclose(f);
    return 0;
}
static void lies_paar(const char *pfad, uint32_t *U, uint32_t *W, int *k, int *m1, int *m2) {
    FILE *f = fopen(pfad, "r");
    if (!f) { perror(pfad); exit(2); }
    char line[4096];
    *k = 0; *m1 = *m2 = -1;
    while (fgets(line, sizeof line, f)) {
        char *s = line;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '#' || *s == '\n' || *s == 0) continue;
        uint32_t b[2] = {0, 0}; int c[2] = {0, 0}; int seite = 0; char *e;
        for (;;) {
            while (*s == ' ' || *s == '\t') s++;
            if (*s == '|') { seite = 1; s++; continue; }
            long v = strtol(s, &e, 10);
            if (e == s) break;
            if (v == -1) b[seite] |= 1u << c[seite];
            c[seite]++; s = e;
        }
        if (*m1 < 0) { *m1 = c[0]; *m2 = c[1]; }
        U[*k] = b[0]; W[*k] = b[1]; (*k)++;
    }
    fclose(f);
}
static void schreibe_paar(const char *pfad, const uint32_t *U, int m1, const uint32_t *W, int m2, int k, const char *kopf) {
    FILE *f = pfad ? fopen(pfad, "w") : stdout;
    if (!f) { perror(pfad); exit(2); }
    if (kopf) fprintf(f, "# %s\n", kopf);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < m1; j++) fprintf(f, "%2d ", (U[i] >> j & 1) ? -1 : 1);
        fprintf(f, "|");
        for (int j = 0; j < m2; j++) fprintf(f, " %2d", (W[i] >> j & 1) ? -1 : 1);
        fprintf(f, "\n");
    }
    if (pfad) fclose(f);
}

/* ---------- Tabu-Suche ueber Zeilenwoerter von G ---------- */
static uint32_t cw[MAXK];
static int64_t *CZ = NULL;
/* je Zeile i: Punkte z, an denen der Wert der Zeile die Kosten aendert (cz nicht konstant ueber die Stufen), Werte minus Minimum */
static int *relz = NULL, *nrel = NULL;
static int64_t *relv = NULL, *relbase = NULL;
static void baue_rel(void) {
    free(relz); free(nrel); free(relv); free(relbase);
    relz = malloc(sizeof(int) * (size_t)K * NZ); nrel = calloc(K, sizeof(int));
    relv = malloc(sizeof(int64_t) * (size_t)K * NZ * L); relbase = calloc(K, sizeof(int64_t));
    for (int i = 0; i < K; i++) {
        for (int z = 0; z < NZ; z++) {
            const int64_t *cz = CZ + ((size_t)i * NZ + z) * L;
            int64_t mn = cz[0], mx = cz[0];
            for (int l = 1; l < L; l++) { if (cz[l] < mn) mn = cz[l]; if (cz[l] > mx) mx = cz[l]; }
            relbase[i] += mn;
            if (mx > mn) {
                int k = nrel[i]++;
                relz[(size_t)i * NZ + k] = z << 1;
                for (int l = 0; l < L; l++) relv[((size_t)i * NZ + k) * L + l] = cz[l] - mn;
            }
        }
    }
}
static inline int64_t bewerte(int i, uint32_t c, int64_t grenze) {
    int64_t s = relbase[i];
    const int *rz = relz + (size_t)i * NZ;
    const int64_t *rv = relv + (size_t)i * NZ * L;
    int n = nrel[i];
    for (int k = 0; k < n; k++) { s += rv[(size_t)k * L + lvlidx(c, (uint32_t)rz[k])]; if (s > grenze) return s; }
    return s;
}
static int *tabu = NULL;

static int64_t berechne_CZ(void) {
    static u64 *pre = NULL, *suf = NULL, *O = NULL;
    static int cap = 0;
    if (cap < NW) { free(pre); free(suf); free(O); cap = NW;
        pre = malloc(sizeof(u64) * (MAXK + 1) * NW); suf = malloc(sizeof(u64) * (MAXK + 1) * NW); O = malloc(sizeof(u64) * NW); }
    int64_t tot = 0;
    for (int z = 0; z < NZ; z++) {
        uint32_t zm = (uint32_t)z << 1;
        const u64 *Mi[MAXK];
        for (int i = 0; i < K; i++) Mi[i] = T + ((size_t)i * L + lvlidx(cw[i], zm)) * NW;
        memset(pre, 0, sizeof(u64) * NW);
        for (int i = 0; i < K; i++) for (int w = 0; w < NW; w++) pre[(i + 1) * NW + w] = pre[i * NW + w] | Mi[i][w];
        memset(suf + (size_t)K * NW, 0, sizeof(u64) * NW);
        for (int i = K - 1; i >= 0; i--) for (int w = 0; w < NW; w++) suf[i * NW + w] = suf[(i + 1) * NW + w] | Mi[i][w];
        tot += wzahl_offen(pre + (size_t)K * NW);
        const int64_t *gz = verfahren ? Wz + (size_t)z * P : NULL;
        for (int i = 0; i < K; i++) {
            int voll = 1;
            for (int w = 0; w < NW; w++) { O[w] = pre[i * NW + w] | suf[(i + 1) * NW + w]; if (~O[w] & (w == NW - 1 ? lastmask : ~0ULL)) voll = 0; }
            int64_t *cz = CZ + ((size_t)i * NZ + z) * L;
            if (voll) { for (int l = 0; l < L; l++) cz[l] = 0; continue; }
            u64 X[1024];
            for (int l = 0; l < L; l++) {
                const u64 *Tl = T + ((size_t)i * L + l) * NW;
                for (int w = 0; w < NW; w++) X[w] = O[w] | Tl[w];
                cz[l] = gz ? wzahl_offen_g(X, gz) : wzahl_offen(X);
            }
        }
    }
    baue_rel();
    return tot;
}

static int64_t ls_kosten(void) { /* Kosten ohne CZ */
    int64_t tot = 0;
    u64 X[1024];
    for (int z = 0; z < NZ; z++) {
        uint32_t zm = (uint32_t)z << 1;
        memset(X, 0, sizeof(u64) * NW);
        for (int i = 0; i < K; i++) { const u64 *M = T + ((size_t)i * L + lvlidx(cw[i], zm)) * NW; for (int w = 0; w < NW; w++) X[w] |= M[w]; }
        tot += wzahl_offen(X);
    }
    return tot;
}

/* Nachbarschaft: alle Woerter (mg <= 12) oder Abstand <= 2 (normiert, Bit 0 = 0) */
static int kandidaten(uint32_t c, uint32_t *out) {
    int n = 0;
    if (mg <= 12) { for (int a = 0; a < NZ; a++) out[n++] = (uint32_t)a << 1; return n; }
    uint32_t full = (1u << mg) - 1;
    for (int a = 0; a < mg; a++) {
        uint32_t c1 = c ^ (1u << a); if (c1 & 1) c1 ^= full; out[n++] = c1;
        for (int b = a + 1; b < mg; b++) { uint32_t c2 = c ^ (1u << a) ^ (1u << b); if (c2 & 1) c2 ^= full; out[n++] = c2; }
    }
    for (int s = 0; s < 64; s++) out[n++] = ((uint32_t)rndi(NZ)) << 1;
    return n;
}

/* eine Tabu-Phase; cw ist Start und Ergebnis (bestes); gibt beste Kosten */
static int64_t tabu_phase(int64_t max_iter, int64_t stagnation, double ende, int64_t *iter_gesamt) {
    free(CZ); CZ = malloc(sizeof(int64_t) * (size_t)K * NZ * L);
    free(tabu); tabu = calloc((size_t)K * NZ, sizeof(int));
    static uint32_t kand[1 << 15];
    uint32_t best[MAXK];
    memcpy(best, cw, sizeof best);
    int64_t cost = berechne_CZ();
    int64_t bestc = cost, letzte = 0;
    if (cost == 0) return 0;
    for (int64_t it = 1; it <= max_iter; it++) {
        if ((it & 15) == 0 && jetzt() > ende) break;
        int64_t bv = INT64_MAX; int bi = -1; uint32_t bc = 0; int ties = 0;
        for (int i = 0; i < K; i++) {
            int nk = kandidaten(cw[i], kand);
            for (int a = 0; a < nk; a++) {
                uint32_t c = kand[a];
                if (c == cw[i]) continue;
                int ist_tabu = tabu[(size_t)i * NZ + (c >> 1)] > it;
                int64_t grenze = ist_tabu ? (bestc - 1 < bv ? bestc - 1 : bv) : bv;
                int64_t s = bewerte(i, c, grenze);
                if (s > grenze) continue;
                if (s < bv) { bv = s; bi = i; bc = c; ties = 1; }
                else if (s == bv) { ties++; if (rndi(ties) == 0) { bi = i; bc = c; } }
            }
        }
        if (bi < 0) { /* alles tabu: zufaelliger Zug */
            bi = rndi(K); bc = ((uint32_t)rndi(NZ)) << 1;
        }
        tabu[(size_t)bi * NZ + (cw[bi] >> 1)] = (int)(it + 3 + rndi(K + 4));
        cw[bi] = bc;
        cost = berechne_CZ();
        if (iter_gesamt) (*iter_gesamt)++;
        if (cost < bestc) { bestc = cost; memcpy(best, cw, sizeof best); letzte = it; if (cost == 0) break; }
        if (it - letzte > stagnation) break;
    }
    memcpy(cw, best, sizeof best);
    return bestc;
}

/* gewichtete Kosten (Gewichte Wz) nach Tausch der Woerter der Zeilen i und j */
static int64_t gew_kosten_tausch(int i, int j, int64_t grenze) {
    static u64 X[1024];
    int64_t s = 0;
    for (int z = 0; z < NZ; z++) {
        uint32_t zm = (uint32_t)z << 1;
        memset(X, 0, sizeof(u64) * NW);
        for (int k = 0; k < K; k++) {
            uint32_t c = (k == i) ? cw[j] : (k == j) ? cw[i] : cw[k];
            const u64 *M = T + ((size_t)k * L + lvlidx(c, zm)) * NW;
            for (int w = 0; w < NW; w++) X[w] |= M[w];
        }
        s += wzahl_offen_g(X, Wz + (size_t)z * P);
        if (s > grenze) return s;
    }
    return s;
}
static int tauschzuege = 1;
static int64_t tausch_anzahl = 0;

/* Verfahren "gewicht": dynamische Gewichte je offenem Paar (z,p) (Breakout). An einem lokalen Minimum (kein Zug senkt die
   gewichteten Kosten) wird das Gewicht jedes offenen Paars um seine Grundvielfachheit erhoeht. Ziel bleibt die wahre Restzahl 0. */
static int64_t gewicht_phase(int64_t max_iter, int64_t stagnation, double ende, int64_t *iter_gesamt) {
    free(CZ); CZ = malloc(sizeof(int64_t) * (size_t)K * NZ * L);
    free(tabu); tabu = calloc((size_t)K * NZ, sizeof(int));
    free(Wz); Wz = malloc(sizeof(int64_t) * (size_t)NZ * (P > 0 ? P : 1));
    for (int z = 0; z < NZ; z++) for (int p = 0; p < P; p++) Wz[(size_t)z * P + p] = Wt[p];
    static uint32_t kand[1 << 15];
    static u64 X[1024];
    uint32_t best[MAXK];
    memcpy(best, cw, sizeof best);
    int64_t cost = berechne_CZ();
    int64_t bestc = cost, letzte = 0;
    if (cost == 0) return 0;
    for (int64_t it = 1; it <= max_iter; it++) {
        if ((it & 15) == 0 && jetzt() > ende) break;
        int64_t cur = bewerte(0, cw[0], INT64_MAX);
        int64_t bv = cur - 1; int bi = -1; uint32_t bc = 0; int ties = 0;
        for (int i = 0; i < K; i++) {
            int nk = kandidaten(cw[i], kand);
            for (int a = 0; a < nk; a++) {
                uint32_t c = kand[a];
                if (c == cw[i]) continue;
                if (tabu[(size_t)i * NZ + (c >> 1)] > it) continue;
                int64_t s = bewerte(i, c, bv);
                if (s > bv) continue;
                if (s < bv || bi < 0) { bv = s; bi = i; bc = c; ties = 1; }
                else { ties++; if (rndi(ties) == 0) { bi = i; bc = c; } }
            }
        }
        int ti = -1, tj = -1;
        if (bi < 0 && tauschzuege) { /* lokales Minimum: Tausch zweier Zeilenwoerter pruefen */
            int64_t tb = cur - 1; int tt = 0;
            for (int i = 0; i < K; i++) for (int j = i + 1; j < K; j++) {
                if (cw[i] == cw[j]) continue;
                int64_t v = gew_kosten_tausch(i, j, tb);
                if (v > tb) continue;
                if (v < tb || ti < 0) { tb = v; ti = i; tj = j; tt = 1; }
                else { tt++; if (rndi(tt) == 0) { ti = i; tj = j; } }
            }
        }
        if (bi >= 0) {
            tabu[(size_t)bi * NZ + (cw[bi] >> 1)] = (int)(it + 2 + rndi(4));
            cw[bi] = bc;
        } else if (ti >= 0) {
            uint32_t h = cw[ti]; cw[ti] = cw[tj]; cw[tj] = h;
            tausch_anzahl++;
        } else {
            for (int z = 0; z < NZ; z++) {
                uint32_t zm = (uint32_t)z << 1;
                memset(X, 0, sizeof(u64) * NW);
                for (int i = 0; i < K; i++) { const u64 *M = T + ((size_t)i * L + lvlidx(cw[i], zm)) * NW; for (int w = 0; w < NW; w++) X[w] |= M[w]; }
                for (int w = 0; w < NW; w++) {
                    u64 x = ~X[w] & (w == NW - 1 ? lastmask : ~0ULL);
                    while (x) { int b = __builtin_ctzll(x); Wz[(size_t)z * P + w * 64 + b] += Wt[w * 64 + b]; x &= x - 1; }
                }
            }
            if (glatt > 0 && ++erhoehungen % glatt == 0)
                for (size_t q = 0; q < (size_t)NZ * P; q++) { int64_t b = Wt[q % P]; if (Wz[q] > b) Wz[q] -= b; }
        }
        cost = berechne_CZ();
        if (iter_gesamt) (*iter_gesamt)++;
        if (cost < bestc) { bestc = cost; memcpy(best, cw, sizeof best); letzte = it; if (cost == 0) break; }
        if (it - letzte > stagnation) break;
    }
    memcpy(cw, best, sizeof best);
    return bestc;
}

/* ---------- vollstaendige Tiefensuche ---------- */
static u64 *MS = NULL;
static int zugewiesen[MAXK];
static uint32_t dw[MAXK];
static int64_t knoten = 0;
static int abgebrochen = 0;
static int *cntp = NULL;
static int64_t *volsum = NULL;
static int64_t knoten_tiefe[MAXK + 1];

typedef struct { uint32_t c; int64_t g; } Kand;
static int cmpkand(const void *a, const void *b) { const Kand *x = a, *y = b; return (x->g < y->g) - (x->g > y->g); }

static int dfs(int tiefe, int nint, const int *ib) {
    knoten++; knoten_tiefe[tiefe]++;
    if ((knoten & 255) == 0 && jetzt() > zeitgrenze) { abgebrochen = 1; return 0; }
    if (abgebrochen) return 0;
    u64 *mask = MS + (size_t)tiefe * NZ * NW;
    memset(cntp, 0, sizeof(int) * P);
    int offen = 0;
    for (int z = 0; z < NZ; z++)
        for (int w = 0; w < NW; w++) {
            u64 x = ~mask[(size_t)z * NW + w] & (w == NW - 1 ? lastmask : ~0ULL);
            while (x) { cntp[w * 64 + __builtin_ctzll(x)]++; offen = 1; x &= x - 1; }
        }
    if (!offen) { for (int i = 0; i < K; i++) if (!zugewiesen[i]) dw[i] = 0; return 1; }
    if (tiefe == K) return 0;
    /* Schranke und Wahl */
    int bp = -1; int64_t bslack = INT64_MAX;
    for (int p = 0; p < P; p++) {
        if (!cntp[p]) continue;
        int64_t cap = 0;
        for (int i = 0; i < K; i++) if (!zugewiesen[i]) cap += volr(R[(size_t)p * K + i]);
        if (cntp[p] > cap) return 0;
        if (cap - cntp[p] < bslack) { bslack = cap - cntp[p]; bp = p; }
    }
    int bi = -1, bv = -1;
    for (int i = 0; i < K; i++) if (!zugewiesen[i]) { int v = volr(R[(size_t)bp * K + i]); if (v > bv) { bv = v; bi = i; } }
    /* Kandidaten: je Intervall-Klasse k_j Minus-Eintraege am Ende */
    int groesse[MAXM]; for (int j = 0; j < nint; j++) groesse[j] = ib[j + 1] - ib[j];
    int kk[MAXM]; memset(kk, 0, sizeof kk);
    int nk = 0, cap = 1;
    for (int j = 0; j < nint; j++) cap *= groesse[j] + 1;
    Kand *ks = malloc(sizeof(Kand) * (cap + 1));
    for (;;) {
        int ok = 1;
        if (tiefe == 0) { for (int j = 0; j < nint; j++) if (kk[j]) ok = 0; }
        else { /* lex(k) <= lex(groesse-k) */
            for (int j = 0; j < nint; j++) { int a = kk[j], b = groesse[j] - kk[j]; if (a < b) break; if (a > b) { ok = 0; break; } }
        }
        if (ok) {
            uint32_t c = 0;
            for (int j = 0; j < nint; j++) for (int t = 0; t < kk[j]; t++) c |= 1u << (ib[j + 1] - 1 - t);
            int64_t g = 0;
            for (int z = 0; z < NZ; z++) {
                const u64 *Tl = T + ((size_t)bi * L + lvlidx(c, (uint32_t)z << 1)) * NW;
                for (int w = 0; w < NW; w++) g += __builtin_popcountll(Tl[w] & ~mask[(size_t)z * NW + w]);
            }
            ks[nk].c = c; ks[nk].g = g; nk++;
        }
        int j = 0;
        while (j < nint) { kk[j]++; if (kk[j] <= groesse[j]) break; kk[j] = 0; j++; }
        if (j == nint) break;
    }
    qsort(ks, nk, sizeof(Kand), cmpkand);
    u64 *kind = MS + (size_t)(tiefe + 1) * NZ * NW;
    zugewiesen[bi] = 1;
    for (int a = 0; a < nk && !abgebrochen; a++) {
        uint32_t c = ks[a].c;
        /* Kandidat ohne Zugewinn: dann loest jede Fortsetzung, die mit ihm loest, auch mit jedem anderen Wort dieser Zeile;
           der erste Kandidat (groesster Zugewinn) wird immer versucht, also genuegt es, Null-Kandidaten ab dem zweiten zu ueberspringen. */
        if (ks[a].g == 0 && a > 0) continue;
        for (int z = 0; z < NZ; z++) {
            const u64 *Tl = T + ((size_t)bi * L + lvlidx(c, (uint32_t)z << 1)) * NW;
            for (int w = 0; w < NW; w++) kind[(size_t)z * NW + w] = mask[(size_t)z * NW + w] | Tl[w];
        }
        int nib[MAXM + 2], nn = 0; nib[0] = 0;
        for (int j = 0; j < nint; j++) {
            int kj = 0; for (int col = ib[j]; col < ib[j + 1]; col++) if (c >> col & 1) kj++;
            if (groesse[j] - kj > 0) nib[++nn] = ib[j + 1] - kj;
            if (kj > 0) nib[++nn] = ib[j + 1];
        }
        dw[bi] = c;
        if (dfs(tiefe + 1, nn, nib)) { free(ks); return 1; }
    }
    zugewiesen[bi] = 0;
    free(ks);
    return 0;
}

/* ---------- Brute Force (Test) ---------- */
static int brute_rek(int i, u64 *mask) {
    if (i == K) {
        for (int z = 0; z < NZ; z++) for (int w = 0; w < NW; w++) if (~mask[(size_t)z * NW + w] & (w == NW - 1 ? lastmask : ~0ULL)) return 0;
        return 1;
    }
    u64 *kind = malloc(sizeof(u64) * NZ * NW);
    for (int a = 0; a < NZ; a++) {
        uint32_t c = (uint32_t)a << 1;
        for (int z = 0; z < NZ; z++) {
            const u64 *Tl = T + ((size_t)i * L + lvlidx(c, (uint32_t)z << 1)) * NW;
            for (int w = 0; w < NW; w++) kind[(size_t)z * NW + w] = mask[(size_t)z * NW + w] | Tl[w];
        }
        if (brute_rek(i + 1, kind)) { dw[i] = c; free(kind); return 1; }
    }
    free(kind);
    return 0;
}

/* ---------- main ---------- */
static const char *opt(int argc, char **argv, const char *name, const char *def) {
    for (int a = 1; a < argc - 1; a++) if (strcmp(argv[a], name) == 0) return argv[a + 1];
    return def;
}
static int flag(int argc, char **argv, const char *name) {
    for (int a = 1; a < argc; a++) if (strcmp(argv[a], name) == 0) return 1;
    return 0;
}

static void ausgabe_loesung(const char *aus, int fest_rechts, const uint32_t *G, const char *kopf) {
    if (fest_rechts) schreibe_paar(aus, G, mg, Ffix, mf, K, kopf);
    else schreibe_paar(aus, Ffix, mf, G, mg, K, kopf);
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Aufruf: siehe Kopf von src/zerl_halb.c\n"); return 2; }
    t0 = jetzt();
    const char *modus = argv[1];
    u64 seed = (u64)atoll(opt(argc, argv, "--seed", "1"));
    rs ^= seed * 0x9E3779B97F4A7C15ULL; for (int a = 0; a < 20; a++) rnd();
    double sek = atof(opt(argc, argv, "--zeit", "600"));
    zeitgrenze = t0 + sek;
    const char *aus = opt(argc, argv, "--aus", NULL);
    int fest_rechts = flag(argc, argv, "--fest-rechts");
    setvbuf(stdout, NULL, _IOLBF, 0);
    verfahren = strcmp(opt(argc, argv, "--verfahren", "gewicht"), "tabu") ? 1 : 0;
    int64_t stag = atoll(opt(argc, argv, "--stag", "20000"));
    glatt = atoll(opt(argc, argv, "--glatt", "0"));
    int stoer = atoi(opt(argc, argv, "--stoer", "2"));
    tauschzuege = !flag(argc, argv, "--ohne-tausch");

    if (!strcmp(modus, "ls") || !strcmp(modus, "dfs") || !strcmp(modus, "brute")) {
        int k, m;
        lies_matrix(argv[2], Ffix, &k, &m);
        K = k; mf = m; d = atoi(argv[3]); mg = atoi(argv[4]);
        if (K > MAXK || mf > MAXM || mg > 15) { fprintf(stderr, "zu gross\n"); return 2; }
        int nur_max = strcmp(modus, "ls") != 0;
        baue_anforderungen(nur_max);
        printf("# %s: fest %s (%d x %d)%s, d=%d, gesucht %d x %d; Anforderungsvektoren %d (%s), unerfuellbar %d; Punkte je Seite %d / %d\n",
               modus, argv[2], K, mf, fest_rechts ? " als rechte Haelfte" : "", d, K, mg, P, nur_max ? "nur maximale" : "alle kritischen, mit Vielfachheit",
               unerfuellbar, NX, NZ);
        if (!strcmp(modus, "ls")) {
            const char *start = opt(argc, argv, "--start", NULL);
            if (start) {
                uint32_t U[MAXK], W[MAXK]; int kk, a1, a2;
                lies_paar(start, U, W, &kk, &a1, &a2);
                for (int i = 0; i < K; i++) { uint32_t c = fest_rechts ? U[i] : W[i]; if (c & 1) c ^= (1u << mg) - 1; cw[i] = c; }
            } else for (int i = 0; i < K; i++) cw[i] = ((uint32_t)rndi(NZ)) << 1;
            int64_t iters = 0, runde = 0, best = INT64_MAX;
            uint32_t bestw[MAXK];
            while (jetzt() < zeitgrenze) {
                int64_t c = verfahren ? gewicht_phase(INT64_MAX, stag, zeitgrenze, &iters) : tabu_phase(INT64_MAX, stag, zeitgrenze, &iters);
                runde++;
                if (c < best) { best = c; memcpy(bestw, cw, sizeof bestw); }
                printf("  Runde %ld: Restzahl %ld (bisher bestes %ld), Iterationen %ld, Tauschzuege %ld, %.1f s\n", (long)runde, (long)c, (long)best, (long)iters, (long)tausch_anzahl, jetzt() - t0);
                if (best == 0) break;
                /* Stoerung: 2 zufaellige Zeilen neu */
                for (int s = 0; s < stoer; s++) cw[rndi(K)] = ((uint32_t)rndi(NZ)) << 1;
            }
            if (best == 0) {
                printf("LOESUNG nach %.1f s, %ld Iterationen\n", jetzt() - t0, (long)iters);
                char kopf[256]; snprintf(kopf, sizeof kopf, "zerl_halb ls, fest %s, d=%d, seed %lu", argv[2], d, (unsigned long)seed);
                ausgabe_loesung(aus, fest_rechts, bestw, kopf);
                return 0;
            }
            printf("ZEITGRENZE nach %.1f s, beste Restzahl %ld, %ld Iterationen\n", jetzt() - t0, (long)best, (long)iters);
            if (aus) { char kopf[256]; snprintf(kopf, sizeof kopf, "zerl_halb ls ungeloest, Restzahl %ld", (long)best); ausgabe_loesung(aus, fest_rechts, bestw, kopf); }
            return 1;
        }
        if (!strcmp(modus, "dfs")) {
            if (unerfuellbar) { printf("KEINE LOESUNG (vollstaendig): %d Anforderungsvektoren ohne erfuellbaren Eintrag, 0 Knoten\n", unerfuellbar); return 3; }
            MS = calloc((size_t)(K + 1) * NZ * NW, sizeof(u64));
            cntp = malloc(sizeof(int) * (P + 1));
            int ib[2] = {0, mg};
            memset(zugewiesen, 0, sizeof zugewiesen);
            int r = dfs(0, 1, ib);
            printf("# Knoten je Tiefe:");
            for (int t = 0; t <= K; t++) printf(" %ld", (long)knoten_tiefe[t]);
            printf("\n");
            if (r) {
                printf("LOESUNG nach %.1f s, %ld Knoten\n", jetzt() - t0, (long)knoten);
                char kopf[256]; snprintf(kopf, sizeof kopf, "zerl_halb dfs, fest %s, d=%d", argv[2], d);
                ausgabe_loesung(aus, fest_rechts, dw, kopf);
                return 0;
            }
            if (abgebrochen) { printf("ZEITGRENZE nach %.1f s, %ld Knoten\n", jetzt() - t0, (long)knoten); return 1; }
            printf("KEINE LOESUNG (vollstaendig) nach %.1f s, %ld Knoten\n", jetzt() - t0, (long)knoten);
            return 3;
        }
        if (!strcmp(modus, "brute")) {
            u64 *m0 = calloc((size_t)NZ * NW, sizeof(u64));
            int r = (P == 0) ? 1 : brute_rek(0, m0);
            printf(r ? "LOESUNG (brute)\n" : "KEINE LOESUNG (brute)\n");
            return r ? 0 : 3;
        }
    }
    if (!strcmp(modus, "alt")) {
        int KK = atoi(argv[2]), m1 = atoi(argv[3]), m2 = atoi(argv[4]); int dd = atoi(argv[5]);
        int64_t phase_stag = atoll(opt(argc, argv, "--phase", "300"));
        uint32_t U[MAXK], W[MAXK];
        const char *start = opt(argc, argv, "--start", NULL);
        if (start) { int kk, a1, a2; lies_paar(start, U, W, &kk, &a1, &a2); }
        else for (int i = 0; i < KK; i++) { U[i] = (uint32_t)(rnd() & ((1u << m1) - 1)); W[i] = (uint32_t)(rnd() & ((1u << m2) - 1)); }
        K = KK; d = dd;
        int64_t best = INT64_MAX, iters = 0; uint32_t bU[MAXK], bW[MAXK];
        int seite = 0; int64_t runde = 0; double letzte_meldung = 0;
        while (jetzt() < zeitgrenze) {
            /* seite 0: U fest, W frei; seite 1: W fest, U frei */
            if (seite == 0) { mf = m1; mg = m2; memcpy(Ffix, U, sizeof U); for (int i = 0; i < K; i++) { uint32_t c = W[i]; if (c & 1) c ^= (1u << mg) - 1; cw[i] = c; } }
            else { mf = m2; mg = m1; memcpy(Ffix, W, sizeof W); for (int i = 0; i < K; i++) { uint32_t c = U[i]; if (c & 1) c ^= (1u << mg) - 1; cw[i] = c; } }
            baue_anforderungen(0);
            int64_t c = verfahren ? gewicht_phase(INT64_MAX, phase_stag, zeitgrenze, &iters) : tabu_phase(INT64_MAX, phase_stag, zeitgrenze, &iters);
            if (seite == 0) memcpy(W, cw, sizeof W); else memcpy(U, cw, sizeof U);
            runde++;
            if (c < best) { best = c; memcpy(bU, U, sizeof U); memcpy(bW, W, sizeof W); }
            if (jetzt() - letzte_meldung > 30 || c == 0) {
                printf("  Runde %ld (%s frei): Restzahl %ld, bestes %ld, Anforderungsvektoren %d, Iterationen %ld, %.1f s\n",
                       (long)runde, seite ? "U" : "W", (long)c, (long)best, P, (long)iters, jetzt() - t0);
                letzte_meldung = jetzt() - t0;
            }
            if (best == 0) break;
            seite ^= 1;
        }
        char kopf[256];
        if (best == 0) {
            printf("LOESUNG nach %.1f s, %ld Iterationen, %ld Runden\n", jetzt() - t0, (long)iters, (long)runde);
            snprintf(kopf, sizeof kopf, "zerl_halb alt K=%d (%d,%d) d=%d seed %lu", K, m1, m2, d, (unsigned long)seed);
            if (aus) schreibe_paar(aus, bU, m1, bW, m2, K, kopf);
            return 0;
        }
        printf("ZEITGRENZE nach %.1f s, beste Restzahl %ld, %ld Iterationen, %ld Runden\n", jetzt() - t0, (long)best, (long)iters, (long)runde);
        snprintf(kopf, sizeof kopf, "zerl_halb alt ungeloest, Restzahl %ld, K=%d (%d,%d) d=%d seed %lu", (long)best, K, m1, m2, d, (unsigned long)seed);
        if (aus) schreibe_paar(aus, bU, m1, bW, m2, K, kopf);
        return 1;
    }
    fprintf(stderr, "unbekannter Modus\n");
    return 2;
}
