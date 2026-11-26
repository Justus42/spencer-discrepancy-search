/* kern3_such.c -- Suchcode Auftrag K (26.11.2026): Codes als Vereinigung von K Nebenklassen des Kerns D = <1,a,b>.
 *
 * Laenge m, Radius R, Typ (n0,n1,n2,n3): Block i liegt zusammenhaengend, Label (a_j,b_j) = (i&1, i>>1).
 * a = Bloecke 1 und 3, b = Bloecke 2 und 3, 1 = alle Spalten. D = {0,a,b,a+b} und Komplemente (8 Woerter).
 * Q = F_2^m / D (2^(m-3) Klassen). S = Klassen mit Abstand <= R zu D. Nebenklasse c+D deckt x  <=>  x+c in S.
 * Gesucht: c_1..c_K mit  Vereinigung (c_i + S) = Q.
 *
 * Modi:
 *   info m R n0 n1 n2 n3                         |S|, Bahnen, Ueberlappungen
 *   eval m R n0 n1 n2 n3                         stdin: Vertreter als 0/1-Strings (Bit j = Spalte j); Ausgabe offene Klassen
 *   dfs  m R n0 n1 n2 n3 K SEK [MATRIX]          vollstaendige Suche (Ergebnis KEINE / GEFUNDEN / ZEITGRENZE)
 *   ls   m R n0 n1 n2 n3 K SEK SEED [MATRIX]     lokale Suche (Tabu), Ergebnis GEFUNDEN oder beste Restzahl
 *   lsw  wie ls, mit Punktgewichten (offene Punkte +1 je Schritt, Zugwahl nach gewichteter Aenderung)
 *
 * Vollstaendige Suche (Begruendung der Schnitte im Protokoll notes/laeufe/2026-11-26_kern3.md):
 *   - key(x) = Rang der Bahn von x unter G (Permutationen in den Bloecken, Tausch gleich grosser Bloecke) in der Ordnung
 *     (ov(x), Bahncode), ov(x) = |S geschnitten (x+S)|. key ist G-invariant und haengt nur von Differenzen ab.
 *   - O. B. d. A.: c_1 = 0 und c_2 = Bahnvertreter r des Paars mit groesstem key; alle anderen Paare haben key <= key(r).
 *   - Danach Verzweigen ueber einen offenen Punkt p mit den wenigsten zulaessigen Deckern; Geschwister werden im
 *     spaeteren Teilbaum verboten.
 *   - Schranke: g(c) = |U geschnitten (c+S)|. Jede der r noch fehlenden Nebenklassen erfuellt g(c) >= |U| - (Summe der
 *     r-1 groessten g); Summe der r groessten g < |U| => Schnitt.
 * Kein Pruefcode: Treffer werden mit verifiers/kern3/ und verifiers/pruefe_matrix.py geprueft.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef uint64_t u64;
static int m, R, n[4], off[4], K;
static uint32_t bm[4], A, B, ONE, Dw[8];
static int piv[3], npiv[32], nnp;
static uint32_t dtab[8];
static int Q, NW, QB;          /* QB = m-3 */
static uint32_t *pt;           /* Index -> Punkt (Pivots 0) */
static u64 *Sbits;             /* S als Bitmenge */
static int *Slist, Ssize;
static u64 *T;                 /* T[c] = c ^ S, je NW Woerter */
static int *ov, *bcode, *keyrank, nbahn, *bahnrep, *bahnov;
static double tlimit, t0;
static long long knoten = 0, blatt = 0;
static int zeit_aus = 0;

static double jetzt(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + 1e-9 * ts.tv_nsec; }
static int pc32(uint32_t x) { return __builtin_popcount(x); }

static int idx_von(uint32_t x) {   /* x beliebig -> Klassenindex */
    int pat = 0;
    for (int i = 0; i < 3; i++) pat |= ((x >> piv[i]) & 1) << i;
    x ^= dtab[pat];
    int id = 0;
    for (int i = 0; i < nnp; i++) id |= ((x >> npiv[i]) & 1) << i;
    return id;
}

static void aufbau(void) {
    int o = 0;
    for (int i = 0; i < 4; i++) { off[i] = o; bm[i] = (n[i] ? (((1u << n[i]) - 1) << o) : 0); o += n[i]; }
    m = o; ONE = (m == 32) ? 0xffffffffu : ((1u << m) - 1);
    A = bm[1] | bm[3]; B = bm[2] | bm[3];
    uint32_t d4[4] = {0, A, B, A ^ B};
    for (int i = 0; i < 4; i++) { Dw[i] = d4[i]; Dw[i + 4] = d4[i] ^ ONE; }
    int k = 0;
    for (int i = 0; i < 4 && k < 3; i++) if (n[i]) piv[k++] = off[i];
    if (k < 3) { fprintf(stderr, "Typ hat weniger als 3 nichtleere Bloecke\n"); exit(2); }
    for (int d = 0; d < 8; d++) {
        int pat = 0;
        for (int i = 0; i < 3; i++) pat |= ((Dw[d] >> piv[i]) & 1) << i;
        dtab[pat] = Dw[d];
    }
    for (int p = 0; p < 8; p++) {  /* jede Pivotbelegung genau einmal */
        int c = 0;
        for (int d = 0; d < 8; d++) { int pat = 0; for (int i = 0; i < 3; i++) pat |= ((Dw[d] >> piv[i]) & 1) << i; if (pat == p) c++; }
        if (c != 1) { fprintf(stderr, "Pivotwahl fehlerhaft\n"); exit(3); }
    }
    nnp = 0;
    for (int j = 0; j < m; j++) if (j != piv[0] && j != piv[1] && j != piv[2]) npiv[nnp++] = j;
    QB = m - 3; Q = 1 << QB; NW = (Q + 63) / 64;
    pt = malloc(sizeof(uint32_t) * Q);
    for (int id = 0; id < Q; id++) { uint32_t x = 0; for (int i = 0; i < nnp; i++) if ((id >> i) & 1) x |= 1u << npiv[i]; pt[id] = x; }
    for (int id = 0; id < Q; id++) if (idx_von(pt[id]) != id) { fprintf(stderr, "Index fehlerhaft\n"); exit(3); }
    Sbits = calloc(NW, 8); Slist = malloc(sizeof(int) * Q); Ssize = 0;
    for (int id = 0; id < Q; id++) {
        int dm = 99;
        for (int d = 0; d < 8; d++) { int v = pc32(pt[id] ^ Dw[d]); if (v < dm) dm = v; }
        if (dm <= R) { Sbits[id >> 6] |= 1ull << (id & 63); Slist[Ssize++] = id; }
    }
    T = calloc((size_t)Q * NW, 8);
    if (!T) { fprintf(stderr, "kein Speicher\n"); exit(4); }
    for (int c = 0; c < Q; c++) { u64 *t = T + (size_t)c * NW; for (int s = 0; s < Ssize; s++) { int x = c ^ Slist[s]; t[x >> 6] |= 1ull << (x & 63); } }
    ov = malloc(sizeof(int) * Q);
    for (int c = 0; c < Q; c++) { u64 *t = T + (size_t)c * NW; int v = 0; for (int w = 0; w < NW; w++) v += __builtin_popcountll(t[w] & Sbits[w]); ov[c] = v; }
    /* Bahncode: Blockgewichte, Komplement durch D, Sortieren in Gruppen gleich grosser Bloecke, Minimum */
    bcode = malloc(sizeof(int) * Q);
    for (int c = 0; c < Q; c++) {
        int best = 1 << 30;
        for (int d = 0; d < 8; d++) {
            uint32_t x = pt[c] ^ Dw[d];
            int w[4];
            for (int i = 0; i < 4; i++) w[i] = pc32(x & bm[i]);
            for (int i = 0; i < 4; i++) for (int j = i + 1; j < 4; j++) if (n[i] == n[j] && w[j] < w[i]) { int t2 = w[i]; w[i] = w[j]; w[j] = t2; }
            /* zweiter Durchgang fuer Gruppen der Groesse 3 und 4 (einfaches Sortieren) */
            for (int rep = 0; rep < 4; rep++) for (int i = 0; i < 3; i++) for (int j = i + 1; j < 4; j++) if (n[i] == n[j] && w[j] < w[i]) { int t2 = w[i]; w[i] = w[j]; w[j] = t2; }
            int code = ((w[0] * 33 + w[1]) * 33 + w[2]) * 33 + w[3];
            if (code < best) best = code;
        }
        bcode[c] = best;
    }
    /* Bahnen sortieren nach (ov, code) */
    int *ord = malloc(sizeof(int) * Q), nb = 0;
    int *seen_code = malloc(sizeof(int) * Q);
    for (int c = 0; c < Q; c++) {
        int f = 0;
        for (int b = 0; b < nb; b++) if (seen_code[b] == bcode[c]) { f = 1; break; }
        if (!f) { seen_code[nb] = bcode[c]; ord[nb] = c; nb++; }
    }
    /* Einfuegesortierung nach (ov, code) */
    for (int i = 1; i < nb; i++) {
        int c = ord[i], j = i - 1;
        while (j >= 0 && (ov[ord[j]] > ov[c] || (ov[ord[j]] == ov[c] && bcode[ord[j]] > bcode[c]))) { ord[j + 1] = ord[j]; j--; }
        ord[j + 1] = c;
    }
    nbahn = nb; bahnrep = malloc(sizeof(int) * nb); bahnov = malloc(sizeof(int) * nb);
    keyrank = malloc(sizeof(int) * Q);
    for (int b = 0; b < nb; b++) { bahnrep[b] = ord[b]; bahnov[b] = ov[ord[b]]; }
    for (int c = 0; c < Q; c++) {
        for (int b = 0; b < nb; b++) if (bcode[ord[b]] == bcode[c]) { keyrank[c] = b; break; }
        if (ov[c] != ov[bahnrep[keyrank[c]]]) { fprintf(stderr, "ov nicht bahninvariant\n"); exit(3); }
    }
    free(ord); free(seen_code);
}

static inline int tbit(int c, int x) { return (T[(size_t)c * NW + (x >> 6)] >> (x & 63)) & 1; }
static inline int popand(const u64 *a, const u64 *b) { int v = 0; for (int w = 0; w < NW; w++) v += __builtin_popcountll(a[w] & b[w]); return v; }

static int loesung[64], gefunden = 0;
static int chosen[64];

/* Ebenen-Speicher */
#define MAXK 40
static u64 *Ulev[MAXK + 2];
static int *Llev[MAXK + 2], Llen[MAXK + 2];
static int *gbuf[MAXK + 2];
static u64 *goodbits;

static int cmp_desc(const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; return y - x; }

static int rr_global;

static void dfs(int k, int cnt) {
    /* k Nebenklassen gewaehlt (chosen[0..k-1]); U = Ulev[k], Kandidaten Llev[k] */
    if (gefunden || zeit_aus) return;
    if (cnt == 0) { gefunden = 1; for (int i = 0; i < k; i++) loesung[i] = chosen[i]; for (int i = k; i < K; i++) loesung[i] = chosen[0]; return; }
    int r = K - k;
    if (r <= 0) return;
    u64 *U = Ulev[k];
    int *L = Llev[k], nL = Llen[k];
    if ((++knoten & 255) == 0 && jetzt() - t0 > tlimit) { zeit_aus = 1; return; }
    if (r == 1) {
        blatt++;
        int p = -1;
        for (int w = 0; w < NW; w++) if (U[w]) { p = w * 64 + __builtin_ctzll(U[w]); break; }
        for (int i = 0; i < nL; i++) {
            int c = L[i];
            if (!tbit(c, p)) continue;
            const u64 *t = T + (size_t)c * NW;
            int ok = 1;
            for (int w = 0; w < NW; w++) if (U[w] & ~t[w]) { ok = 0; break; }
            if (ok) { chosen[k] = c; gefunden = 1; for (int j = 0; j <= k; j++) loesung[j] = chosen[j]; return; }
        }
        return;
    }
    int *g = gbuf[k];
    for (int i = 0; i < nL; i++) g[i] = popand(U, T + (size_t)L[i] * NW);
    /* Summe der r groessten g: Teilauswahl ueber Kopie */
    static int tmp[1 << 20];
    int top = 0, topr1 = 0, topr2 = 0;
    {
        /* r ist klein: r-mal Maximum suchen (ohne Kopie sortieren) */
        memcpy(tmp, g, sizeof(int) * nL);
        for (int j = 0; j < r && j < nL; j++) {
            int bi = j;
            for (int i = j + 1; i < nL; i++) if (tmp[i] > tmp[bi]) bi = i;
            int t2 = tmp[j]; tmp[j] = tmp[bi]; tmp[bi] = t2;
            top += tmp[j];
            if (j < r - 1) topr1 += tmp[j];
            if (j < r - 2) topr2 += tmp[j];
        }
    }
    if (top < cnt) return;
    int thr = cnt - topr1;
    if (thr < 1) thr = 1;
    /* Gute Kandidaten */
    memset(goodbits + (size_t)k * NW, 0, 8 * NW);
    u64 *GB = goodbits + (size_t)k * NW;
    int ngood = 0;
    for (int i = 0; i < nL; i++) if (g[i] >= thr) { GB[L[i] >> 6] |= 1ull << (L[i] & 63); ngood++; }
    if (ngood == 0) return;
    /* Verzweigungspunkt: offener Punkt mit den wenigsten guten Deckern (|GB geschnitten T_p|) */
    int bestp = -1, bestc = 1 << 30, probe = 0;
    int schritt = cnt > 2048 ? cnt / 2048 : 1, zaehl = 0;
    for (int w = 0; w < NW && bestc > 0; w++) {
        u64 x = U[w];
        while (x) {
            int p = w * 64 + __builtin_ctzll(x); x &= x - 1;
            if ((zaehl++ % schritt) != 0) continue;
            int v = popand(GB, T + (size_t)p * NW);  /* Decker von p: c mit p in c^S  <=>  c in p^S */
            probe++;
            if (v < bestc) { bestc = v; bestp = p; if (v == 0) break; }
        }
    }
    if (bestc == 0) return;
    /* Decker von bestp sammeln, absteigend nach g */
    int nd = 0;
    int *dk = malloc(sizeof(int) * bestc * 2 + 8);
    for (int i = 0; i < nL; i++) if (g[i] >= thr && tbit(L[i], bestp)) { dk[2 * nd] = g[i]; dk[2 * nd + 1] = L[i]; nd++; }
    /* sortieren absteigend nach g (Paare) */
    for (int i = 1; i < nd; i++) { int a0 = dk[2 * i], a1 = dk[2 * i + 1], j = i - 1; while (j >= 0 && dk[2 * j] < a0) { dk[2 * j + 2] = dk[2 * j]; dk[2 * j + 3] = dk[2 * j + 1]; j--; } dk[2 * j + 2] = a0; dk[2 * j + 3] = a1; }
    /* Kandidaten fuer Kinder: nur gute (g >= thr); Geschwister davor verboten */
    for (int i = 0; i < nd && !gefunden && !zeit_aus; i++) {
        int c = dk[2 * i + 1];
        GB[c >> 6] &= ~(1ull << (c & 63));   /* c selbst und fruehere Geschwister raus */
        u64 *U2 = Ulev[k + 1];
        const u64 *t = T + (size_t)c * NW;
        int cnt2 = 0;
        for (int w = 0; w < NW; w++) { U2[w] = U[w] & ~t[w]; cnt2 += __builtin_popcountll(U2[w]); }
        /* im Kind braucht jede der r-1 Nebenklassen g' >= cnt2 - (Summe der r-2 groessten g'), und g >= g' */
        int thr2 = cnt2 - topr2;
        int *L2 = Llev[k + 1], n2 = 0;
        for (int j = 0; j < nL; j++) {
            int x = L[j];
            if (g[j] < thr2) continue;
            if (!((GB[x >> 6] >> (x & 63)) & 1)) continue;
            if (keyrank[x ^ c] > rr_global) continue;
            L2[n2++] = x;
        }
        Llen[k + 1] = n2;
        chosen[k] = c;
        dfs(k + 1, cnt2);
    }
    free(dk);
}

static void schreibe_matrix(const char *datei, int *sol, int k) {
    FILE *f = fopen(datei, "w");
    if (!f) { perror(datei); return; }
    for (int i = 0; i < k; i++) {
        uint32_t x = pt[sol[i]];
        uint32_t zeilen[4] = {x, x ^ A, x ^ B, x ^ A ^ B};
        for (int z = 0; z < 4; z++) {
            for (int j = 0; j < m; j++) fprintf(f, "%s%d", j ? " " : "", ((zeilen[z] >> j) & 1) ? -1 : 1);
            fprintf(f, "\n");
        }
    }
    fclose(f);
}

static void druck_vertreter(int *sol, int k) {
    for (int i = 0; i < k; i++) {
        printf("VERTRETER ");
        for (int j = 0; j < m; j++) printf("%d", (pt[sol[i]] >> j) & 1);
        printf("\n");
    }
}

static int offen_von(int *sol, int k) {
    u64 *u = calloc(NW, 8);
    for (int i = 0; i < k; i++) { const u64 *t = T + (size_t)sol[i] * NW; for (int w = 0; w < NW; w++) u[w] |= t[w]; }
    int c = 0;
    for (int w = 0; w < NW; w++) c += __builtin_popcountll(u[w]);
    free(u);
    return Q - c;
}

static void lauf_dfs(const char *mdatei) {
    for (int k = 0; k <= K + 1; k++) {
        Ulev[k] = calloc(NW, 8); Llev[k] = malloc(sizeof(int) * Q); gbuf[k] = malloc(sizeof(int) * Q);
    }
    goodbits = calloc((size_t)NW * (K + 2), 8);
    t0 = jetzt();
    /* Wurzel: c_1 = 0 */
    u64 *U0 = calloc(NW, 8);
    int cnt0 = 0;
    for (int x = 0; x < Q; x++) if (!tbit(0, x)) { U0[x >> 6] |= 1ull << (x & 63); cnt0++; }
    /* g an der Wurzel, Schwelle fuer alle uebrigen K-1 */
    int *g0 = malloc(sizeof(int) * Q);
    for (int c = 0; c < Q; c++) g0[c] = (c == 0) ? 0 : popand(U0, T + (size_t)c * NW);
    int *tmp = malloc(sizeof(int) * Q);
    memcpy(tmp, g0, sizeof(int) * Q);
    qsort(tmp, Q, sizeof(int), cmp_desc);
    int top = 0, topr1 = 0;
    for (int j = 0; j < K - 1; j++) { top += tmp[j]; if (j < K - 2) topr1 += tmp[j]; }
    int thr0 = cnt0 - topr1;
    printf("INFO |S| = %d, Q = %d, Bahnen = %d, offen nach c_1 = %d, Summe der %d groessten g = %d, Schwelle = %d\n", Ssize, Q, nbahn, cnt0, K - 1, top, thr0);
    if (K == 1 || top < cnt0) { printf("ERGEBNIS KEINE (Wurzelschranke) knoten=0 zeit=%.2f\n", jetzt() - t0); return; }
    int c2_versucht = 0, c2_fertig = 0;
    for (int b = 0; b < nbahn && !gefunden && !zeit_aus; b++) {
        int r = bahnrep[b];
        if (r == 0) continue;
        if (g0[r] < thr0) continue;
        rr_global = b;
        c2_versucht++;
        chosen[0] = 0; chosen[1] = r;
        int *L = Llev[2], nL = 0;
        for (int c = 1; c < Q; c++) {
            if (c == r) continue;
            if (keyrank[c] > b || keyrank[c ^ r] > b) continue;
            if (g0[c] < thr0) continue;
            L[nL++] = c;
        }
        Llen[2] = nL;
        const u64 *t = T + (size_t)r * NW;
        int cnt = 0;
        for (int w = 0; w < NW; w++) { Ulev[2][w] = U0[w] & ~t[w]; cnt += __builtin_popcountll(Ulev[2][w]); }
        dfs(2, cnt);
        if (!zeit_aus) c2_fertig++;
    }
    double dt = jetzt() - t0;
    if (gefunden) {
        int of = offen_von(loesung, K);
        printf("ERGEBNIS GEFUNDEN knoten=%lld blatt=%lld zeit=%.2f offen_kontrolle=%d\n", knoten, blatt, dt, of);
        druck_vertreter(loesung, K);
        if (mdatei) schreibe_matrix(mdatei, loesung, K);
    } else if (zeit_aus) {
        printf("ERGEBNIS ZEITGRENZE knoten=%lld blatt=%lld zeit=%.2f c2_fertig=%d von %d\n", knoten, blatt, dt, c2_fertig, c2_versucht);
    } else {
        printf("ERGEBNIS KEINE knoten=%lld blatt=%lld zeit=%.2f c2_bahnen=%d\n", knoten, blatt, dt, c2_versucht);
    }
}

/* ---------- lokale Suche (Tabu) ---------- */
static uint64_t rng_s;
static uint64_t rnd(void) { rng_s ^= rng_s << 13; rng_s ^= rng_s >> 7; rng_s ^= rng_s << 17; return rng_s; }

static int *wgt;   /* Punktgewichte (nur lsw) */
static inline long long wsum(const u64 *a, const u64 *b) {
    long long v = 0;
    for (int w = 0; w < NW; w++) { u64 x = a[w] & b[w]; while (x) { v += wgt[w * 64 + __builtin_ctzll(x)]; x &= x - 1; } }
    return v;
}

static void lauf_ls(unsigned seed, const char *mdatei, int gew) {
    rng_s = 0x9E3779B97F4A7C15ull ^ ((uint64_t)seed * 0x1234567ull + 1);
    int *cur = malloc(sizeof(int) * K), *best = malloc(sizeof(int) * K);
    int *cnt = calloc(Q, sizeof(int));
    u64 *U = calloc(NW, 8), *C1 = calloc(NW, 8), *Uq = calloc((size_t)K * NW, 8);
    int *tabu = calloc(Q, sizeof(int));
    for (int i = 0; i < K; i++) cur[i] = rnd() % Q;
    for (int i = 0; i < K; i++) for (int s = 0; s < Ssize; s++) cnt[cur[i] ^ Slist[s]]++;
    int bestoff = 1 << 30;
    long long it = 0;
    t0 = jetzt();
    int *ulist = malloc(sizeof(int) * Q);
    wgt = malloc(sizeof(int) * Q); for (int x = 0; x < Q; x++) wgt[x] = 1;
    while (1) {
        memset(U, 0, 8 * NW); memset(C1, 0, 8 * NW);
        int unc = 0;
        for (int x = 0; x < Q; x++) { if (cnt[x] == 0) { U[x >> 6] |= 1ull << (x & 63); ulist[unc++] = x; } else if (cnt[x] == 1) C1[x >> 6] |= 1ull << (x & 63); }
        if (unc < bestoff) { bestoff = unc; memcpy(best, cur, sizeof(int) * K); }
        if (unc == 0) break;
        if ((it & 15) == 0 && jetzt() - t0 > tlimit) break;
        it++;
        for (int i = 0; i < K; i++) { const u64 *t = T + (size_t)cur[i] * NW; for (int w = 0; w < NW; w++) Uq[(size_t)i * NW + w] = t[w] & C1[w]; }
        int lost[64]; long long wlost[64];
        for (int i = 0; i < K; i++) { int v = 0; for (int w = 0; w < NW; w++) v += __builtin_popcountll(Uq[(size_t)i * NW + w]); lost[i] = v; }
        if (gew) { for (int j = 0; j < unc; j++) wgt[ulist[j]]++; for (int i = 0; i < K; i++) wlost[i] = wsum(Uq + (size_t)i * NW, Uq + (size_t)i * NW); }
        int p = ulist[rnd() % unc];
        long long bd = 1ll << 60; int bi = -1, bc = -1, nb = 0;
        for (int s = 0; s < Ssize; s++) {
            int c = p ^ Slist[s];
            const u64 *t = T + (size_t)c * NW;
            int a = popand(t, U);
            long long wa = gew ? wsum(t, U) : 0;
            for (int i = 0; i < K; i++) {
                if (cur[i] == c) continue;
                int bq = popand(t, Uq + (size_t)i * NW);
                int d0 = lost[i] - a - bq;   /* Aenderung der offenen Zahl */
                long long d = gew ? (wlost[i] - wa - wsum(t, Uq + (size_t)i * NW)) : d0;
                int erlaubt = tabu[c] <= it || unc + d0 < bestoff;
                if (!erlaubt) continue;
                if (d < bd) { bd = d; bi = i; bc = c; nb = 1; }
                else if (d == bd) { nb++; if (rnd() % nb == 0) { bi = i; bc = c; } }
            }
        }
        if (bi < 0) continue;
        int alt = cur[bi];
        for (int s = 0; s < Ssize; s++) cnt[alt ^ Slist[s]]--;
        for (int s = 0; s < Ssize; s++) cnt[bc ^ Slist[s]]++;
        cur[bi] = bc;
        tabu[alt] = it + 5 + (int)(rnd() % 10);
    }
    double dt = jetzt() - t0;
    int of = offen_von(best, K);
    if (of == 0) {
        printf("ERGEBNIS GEFUNDEN iter=%lld zeit=%.2f offen_kontrolle=%d\n", it, dt, of);
        druck_vertreter(best, K);
        if (mdatei) schreibe_matrix(mdatei, best, K);
    } else {
        printf("ERGEBNIS NICHT_ERREICHT beste_restzahl=%d iter=%lld zeit=%.2f\n", of, it, dt);
        druck_vertreter(best, K);
    }
}

int main(int argc, char **argv) {
    if (argc < 8) { fprintf(stderr, "Aufruf: siehe Kopf der Datei\n"); return 2; }
    const char *modus = argv[1];
    m = atoi(argv[2]); R = atoi(argv[3]);
    for (int i = 0; i < 4; i++) n[i] = atoi(argv[4 + i]);
    if (n[0] + n[1] + n[2] + n[3] != m) { fprintf(stderr, "Typ passt nicht zu m\n"); return 2; }
    aufbau();
    if (!strcmp(modus, "info")) {
        printf("m=%d R=%d Typ=(%d,%d,%d,%d) |S|=%d Q=%d Bahnen=%d\n", m, R, n[0], n[1], n[2], n[3], Ssize, Q, nbahn);
        for (int b = 0; b < nbahn; b++) printf("BAHN %d rep=%d ov=%d code=%d\n", b, bahnrep[b], bahnov[b], bcode[bahnrep[b]]);
        return 0;
    }
    if (!strcmp(modus, "eval")) {
        char buf[256]; int sol[256], k = 0;
        while (scanf("%255s", buf) == 1) {
            if ((int)strlen(buf) != m) { fprintf(stderr, "Laenge falsch: %s\n", buf); return 2; }
            uint32_t x = 0;
            for (int j = 0; j < m; j++) if (buf[j] == '1') x |= 1u << j;
            sol[k++] = idx_von(x);
        }
        int of = offen_von(sol, k);
        printf("OFFEN_KLASSEN %d OFFEN_PUNKTE %d K %d |S| %d\n", of, of * 8, k, Ssize);
        int mk = -1;
        for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++) if (keyrank[sol[i] ^ sol[j]] > mk) mk = keyrank[sol[i] ^ sol[j]];
        printf("GROESSTER_KEY %d VON %d BAHNEN\n", mk, nbahn);
        return 0;
    }
    if (argc < 10) { fprintf(stderr, "K und SEK fehlen\n"); return 2; }
    K = atoi(argv[8]); tlimit = atof(argv[9]);
    if (K > MAXK) { fprintf(stderr, "K zu gross\n"); return 2; }
    if (!strcmp(modus, "dfs")) { lauf_dfs(argc > 10 ? argv[10] : NULL); return 0; }
    if (!strcmp(modus, "ls")) { if (argc < 11) return 2; lauf_ls((unsigned)atoi(argv[10]), argc > 11 ? argv[11] : NULL, 0); return 0; }
    if (!strcmp(modus, "lsw")) { if (argc < 11) return 2; lauf_ls((unsigned)atoi(argv[10]), argc > 11 ? argv[11] : NULL, 1); return 0; }
    fprintf(stderr, "unbekannter Modus\n");
    return 2;
}
