/* zweibahn_zaehl.c -- Zaehlcode (kein Suchcode), Auftrag Z, 25.11.2026.
 *
 * Selbstkomplementaere Codes der Laenge m = 2q + f, invariant unter g = gleichzeitige zyklische
 * Verschiebung zweier Spaltenbloecke der Laenge q (Bits 0..q-1 und q..2q-1), f Fixspalten (Bits 2q..m-1).
 * Ein Code ist eine Vereinigung von <g>-Bahnen auf Wortpaaren {w, ~w}. Gezaehlt werden ALLE
 * Teilmengen (k volle Bahnen) + (kurze Bahnen mit Gesamtgroesse <= B), die mit Radius R ueberdecken:
 * Punkt x ist ueberdeckt, wenn d(x,w) <= R oder d(x,w) >= m-R fuer ein Wort w des Codes.
 *
 * Unabhaengig von notes/orchestrator/2026-11-25_vortest/zweibahn.c geschrieben (nicht gelesen).
 *
 * Verfahren:
 *  - Punktklassen = Bahnen von <g, Komplement> auf {0,1}^m, Vertreter = kleinstes Wort (oberstes Bit 0).
 *    Eine Wortbahn O ueberdeckt eine Punktklasse P genau dann, wenn ein g^k(o) im Abstand <=R oder >=m-R
 *    vom Vertreter p liegt (beide Mengen g- und komplementinvariant).
 *  - Hart = Punktklasse, die keine erlaubte kurze Bahn (Groesse <= B) ueberdeckt.
 *  - k = 2: geordnete Paare (A,B) voller Bahnen, A != B. A nur Vertreter der Klassen unter einer Untergruppe
 *    des Normalisators von <g> (Blockverschiebungen, Multiplikatoren, Blocktausch, Fixspaltenpermutationen,
 *    Komplement ganzer Bloecke und Fixspalten), gewichtet mit der Klassengroesse; Option -n: alle A, Gewicht 1.
 *    B muss alle harten Klassen ueberdecken, die A offen laesst: B laeuft ueber die vollen Bahnen, die die erste
 *    solche Klasse h1 ueberdecken (Kugel um h1). Ergebnis / 2 = ungeordnete Paare.
 *  - Rest: exakte Zaehlung aller Teilmengen kurzer Bahnen mit Gewicht <= B, die den Rest ueberdecken
 *    (Verzweigen an der Bedingung mit den wenigsten Moeglichkeiten, Ausschluss frueherer Geschwister,
 *    am Blatt Rucksack-Zaehlung der freien Bahnen). Ausgabe als Verteilung nach Zahl kurzer Paare.
 *  - Option -v: jedes Blatt (A, B, gewaehlte kurze Bahnen) per Brute-Force ueber alle 2^m Punkte nachpruefen,
 *    ohne Klassen (nur fuer m <= 24).
 *
 * Aufruf: zweibahn_zaehl q f R k B [-n] [-v] [-o datei] [-z zeitgrenze_s]
 * Bauen:  gcc -O3 -march=native -fopenmp -o build/zweibahn_zaehl src/zweibahn_zaehl.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>

typedef unsigned __int128 u128;
static int q, f, m, R, K, BUD, NOSYM = 0, VERIFY = 0;
static double TLIM = 0;
static uint32_t M1, MF, FULL;
static FILE *OUT = NULL;

static inline uint32_t rot1(uint32_t w) {
    uint32_t b1 = w & M1, b2 = (w >> q) & M1;
    b1 = ((b1 << 1) | (b1 >> (q - 1))) & M1;
    b2 = ((b2 << 1) | (b2 >> (q - 1))) & M1;
    return b1 | (b2 << q) | (w & MF);
}
static inline uint32_t canon(uint32_t w) {
    uint32_t best = FULL, x = w;
    for (int k = 0; k < q; k++) {
        if (x < best) best = x;
        uint32_t c = x ^ FULL;
        if (c < best) best = c;
        x = rot1(x);
    }
    return best;
}
static inline int is_canon(uint32_t w) {
    uint32_t x = w;
    for (int k = 0; k < q; k++) {
        if (x < w || (x ^ FULL) < w) return 0;
        x = rot1(x);
    }
    return 1;
}
static inline int osize(uint32_t w) {
    uint32_t x = rot1(w);
    int k = 1;
    while (x != w && x != (w ^ FULL)) { x = rot1(x); k++; }
    return k;
}
static inline int near(uint32_t a, uint32_t b) {
    int d = __builtin_popcount(a ^ b);
    return d <= R || d >= m - R;
}
static inline void rots(uint32_t w, uint32_t *r) {
    for (int k = 0; k < q; k++) { r[k] = w; w = rot1(w); }
}
static inline int covers(const uint32_t *r, uint32_t p) {
    for (int k = 0; k < q; k++) if (near(r[k], p)) return 1;
    return 0;
}

/* ---------- Normalisator-Erzeuger ---------- */
static int units[64], nunits;
static uint32_t perm_blocks(uint32_t w, const int *pi) { /* new pos pi[i] gets old bit i, beide Bloecke */
    uint32_t r = w & MF;
    for (int i = 0; i < q; i++) {
        if (w >> i & 1) r |= 1u << pi[i];
        if (w >> (q + i) & 1) r |= 1u << (q + pi[i]);
    }
    return r;
}
static int NGEN;
static uint32_t gen_apply(int gi, uint32_t w) {
    uint32_t b1 = w & M1, b2 = (w >> q) & M1, fx = w & MF;
    if (gi == 0) { b1 = ((b1 << 1) | (b1 >> (q - 1))) & M1; return b1 | (b2 << q) | fx; }
    if (gi == 1) { b2 = ((b2 << 1) | (b2 >> (q - 1))) & M1; return b1 | (b2 << q) | fx; }
    gi -= 2;
    if (gi < nunits) {
        int pi[64]; for (int i = 0; i < q; i++) pi[i] = (units[gi] * i) % q;
        return perm_blocks(w, pi);
    }
    gi -= nunits;
    if (gi == 0) return b2 | (b1 << q) | fx;
    gi -= 1;
    if (gi < f - 1) { /* Transposition Fixspalten gi, gi+1 */
        int a = 2 * q + gi, b = a + 1;
        uint32_t x = ((w >> a) ^ (w >> b)) & 1;
        return w ^ (x << a) ^ (x << b);
    }
    gi -= (f > 1 ? f - 1 : 0);
    if (gi == 0) return w ^ M1;
    if (gi == 1) return w ^ (M1 << q);
    gi -= 2;
    return w ^ (1u << (2 * q + gi)); /* gi < f */
}

/* ---------- Zaehlmaschine fuer kurze Bahnen ---------- */
#define WM 4
typedef struct { uint64_t b[WM]; } msk;
static int NS;               /* erlaubte kurze Bahnen */
static uint32_t *SREP; static int *SWT; static uint32_t *SROT; /* NS*q */
static msk LIGHT[64];        /* LIGHT[r] = Kandidaten mit Gewicht <= r */
static msk ALLS;
static inline int mz(msk a) { for (int i = 0; i < WM; i++) if (a.b[i]) return 0; return 1; }
static inline msk mand(msk a, msk b) { for (int i = 0; i < WM; i++) a.b[i] &= b.b[i]; return a; }
static inline msk mandn(msk a, msk b) { for (int i = 0; i < WM; i++) a.b[i] &= ~b.b[i]; return a; }
static inline int mpop(msk a) { int c = 0; for (int i = 0; i < WM; i++) c += __builtin_popcountll(a.b[i]); return c; }
static inline void mset(msk *a, int i) { a->b[i >> 6] |= 1ull << (i & 63); }
static int mcmp(const void *x, const void *y) { return memcmp(x, y, sizeof(msk)); }

typedef struct {
    msk *k; int nk; u128 *hist; /* hist[w] w = Gewicht kurzer Bahnen */
    /* Blattausgabe */
    uint32_t A, Bw; int haveA, haveB; long long leaves;
} eng_t;

static void leaf_out(eng_t *E, msk C, u128 *dp, int r, int wC);
static void rec(eng_t *E, msk C, msk X, int wC, int r) {
    int best = -1, bc = 1 << 30; msk bav = {{0}};
    msk lt = LIGHT[r < 63 ? r : 63];
    for (int i = 0; i < E->nk; i++) {
        if (!mz(mand(E->k[i], C))) continue;
        msk av = mand(mandn(E->k[i], X), lt);
        int c = mpop(av);
        if (c == 0) return;
        if (c < bc) { bc = c; best = i; bav = av; if (c == 1) break; }
    }
    if (best < 0) {
        msk fr = mandn(mandn(ALLS, C), X);
        u128 dp[64]; memset(dp, 0, sizeof dp); dp[0] = 1;
        for (int s = 0; s < NS; s++) if (fr.b[s >> 6] >> (s & 63) & 1) {
            int w = SWT[s];
            for (int t = r; t >= w; t--) dp[t] += dp[t - w];
        }
        for (int t = 0; t <= r; t++) E->hist[wC + t] += dp[t];
        E->leaves++;
        leaf_out(E, C, dp, r, wC);
        return;
    }
    for (int s = 0; s < NS; s++) if (bav.b[s >> 6] >> (s & 63) & 1) {
        msk C2 = C; mset(&C2, s);
        rec(E, C2, X, wC + SWT[s], r - SWT[s]);
        mset(&X, s);
    }
}

/* Brute-Force-Nachpruefung eines Blatts ohne Klassen */
static long long VER_OK = 0, VER_BAD = 0;
static int verify_leaf(uint32_t *words, int nw) {
    for (uint32_t x = 0; x <= FULL; x++) {
        int ok = 0;
        for (int i = 0; i < nw && !ok; i++) { int d = __builtin_popcount(x ^ words[i]); if (d <= R) ok = 1; }
        if (!ok) return 0;
        if (x == FULL) break;
    }
    return 1;
}
static int expand(uint32_t rep, uint32_t *out) { /* alle Woerter der Bahn inkl. Komplemente */
    int n = 0; uint32_t x = rep;
    for (int k = 0; k < q; k++) {
        uint32_t a[2] = {x, x ^ FULL};
        for (int j = 0; j < 2; j++) { int dup = 0; for (int i = 0; i < n; i++) if (out[i] == a[j]) dup = 1; if (!dup) out[n++] = a[j]; }
        x = rot1(x);
    }
    return n;
}
static void leaf_out(eng_t *E, msk C, u128 *dp, int r, int wC) {
    if (!OUT && !VERIFY) return;
    u128 tot = 0; for (int t = 0; t <= r; t++) tot += dp[t];
    if (VERIFY) {
        uint32_t words[4096]; int nw = 0;
        if (E->haveA) nw += expand(E->A, words + nw);
        if (E->haveB) nw += expand(E->Bw, words + nw);
        for (int s = 0; s < NS; s++) if (C.b[s >> 6] >> (s & 63) & 1) nw += expand(SREP[s], words + nw);
        int ok = verify_leaf(words, nw);
        #pragma omp atomic
        VER_OK += ok;
        #pragma omp atomic
        VER_BAD += !ok;
    }
    if (OUT) {
        #pragma omp critical(out)
        {
            fprintf(OUT, "A=%x B=%x kurz=", E->haveA ? E->A : 0, E->haveB ? E->Bw : 0);
            int first = 1;
            for (int s = 0; s < NS; s++) if (C.b[s >> 6] >> (s & 63) & 1) { fprintf(OUT, "%s%x/%d", first ? "" : ",", SREP[s], SWT[s]); first = 0; }
            if (first) fprintf(OUT, "-");
            fprintf(OUT, " kurzgewicht=%d ergaenzungen=%llu\n", wC, (unsigned long long)tot);
        }
    }
}

/* Bedingungen fuer eine Restliste bauen, dedupliziert; gibt -1 zurueck, wenn ein Punkt unerreichbar */
static int build_constraints(const uint32_t *pts, long n, msk **outk) {
    msk *k = malloc(sizeof(msk) * (n > 0 ? n : 1));
    long nk = 0;
    for (long i = 0; i < n; i++) {
        msk a = {{0}};
        for (int s = 0; s < NS; s++) if (covers(SROT + (long)s * q, pts[i])) mset(&a, s);
        if (mz(a)) { free(k); return -1; }
        k[nk++] = a;
    }
    qsort(k, nk, sizeof(msk), mcmp);
    long u = 0;
    for (long i = 0; i < nk; i++) if (u == 0 || memcmp(&k[i], &k[u - 1], sizeof(msk))) k[u++] = k[i];
    /* Obermengen entfernen (impliziert), nur bei maessiger Zahl */
    if (u <= 5000) {
        char *dead = calloc(u, 1);
        for (long i = 0; i < u; i++) for (long j = 0; j < u; j++) if (i != j && !dead[j]) {
            msk x = mandn(k[j], k[i]); /* k[j] subset of k[i]? */
            if (mz(x)) { if (memcmp(&k[i], &k[j], sizeof(msk))) { dead[i] = 1; break; } }
        }
        long v = 0; for (long i = 0; i < u; i++) if (!dead[i]) k[v++] = k[i];
        u = v; free(dead);
    }
    *outk = k; return (int)u;
}

static void print128(u128 x) {
    char buf[64]; int n = 0;
    if (x == 0) { printf("0"); return; }
    while (x) { buf[n++] = '0' + (int)(x % 10); x /= 10; }
    while (n) putchar(buf[--n]);
}

int main(int argc, char **argv) {
    if (argc < 6) { fprintf(stderr, "Aufruf: %s q f R k B [-n] [-v] [-o datei] [-z sek]\n", argv[0]); return 2; }
    q = atoi(argv[1]); f = atoi(argv[2]); R = atoi(argv[3]); K = atoi(argv[4]); BUD = atoi(argv[5]);
    for (int i = 6; i < argc; i++) {
        if (!strcmp(argv[i], "-n")) NOSYM = 1;
        else if (!strcmp(argv[i], "-v")) VERIFY = 1;
        else if (!strcmp(argv[i], "-o") && i + 1 < argc) OUT = fopen(argv[++i], "w");
        else if (!strcmp(argv[i], "-z") && i + 1 < argc) TLIM = atof(argv[++i]);
    }
    m = 2 * q + f;
    if (m > 31 || BUD > 62 || K < 0 || K > 2) { fprintf(stderr, "Parameter ausserhalb\n"); return 2; }
    if (VERIFY && m > 24) { fprintf(stderr, "-v nur fuer m <= 24\n"); return 2; }
    M1 = (1u << q) - 1; FULL = (m == 32) ? 0xffffffffu : ((1u << m) - 1); MF = FULL & ~((1u << (2 * q)) - 1);
    double t0 = omp_get_wtime();
    printf("# zweibahn_zaehl q=%d f=%d m=%d R=%d k=%d B=%d sym=%s threads=%d\n", q, f, m, R, K, BUD, NOSYM ? "nein" : "ja", omp_get_max_threads());
    nunits = 0; for (int u = 2; u < q; u++) { int a = u, b = q; while (b) { int t = a % b; a = b; b = t; } if (a == 1) units[nunits++] = u; }
    NGEN = 2 + nunits + 1 + (f > 1 ? f - 1 : 0) + 2 + f;

    /* 1. alle Punktklassen-Vertreter (aufsteigend) */
    uint32_t half = 1u << (m - 1);
    int NCH = 4096; uint32_t chunk = (half + NCH - 1) / NCH;
    long *cnt = calloc(NCH + 1, sizeof(long));
    #pragma omp parallel for schedule(dynamic, 16)
    for (int c = 0; c < NCH; c++) {
        uint64_t lo = (uint64_t)c * chunk, hi = lo + chunk; if (hi > half) hi = half;
        long n = 0; for (uint64_t x = lo; x < hi; x++) n += is_canon((uint32_t)x);
        cnt[c + 1] = n;
    }
    for (int c = 0; c < NCH; c++) cnt[c + 1] += cnt[c];
    long NP = cnt[NCH];
    uint32_t *P = malloc(sizeof(uint32_t) * NP); unsigned char *PS = malloc(NP);
    #pragma omp parallel for schedule(dynamic, 16)
    for (int c = 0; c < NCH; c++) {
        uint64_t lo = (uint64_t)c * chunk, hi = lo + chunk; if (hi > half) hi = half;
        long j = cnt[c]; for (uint64_t x = lo; x < hi; x++) if (is_canon((uint32_t)x)) { P[j] = (uint32_t)x; PS[j] = (unsigned char)osize((uint32_t)x); j++; }
    }
    free(cnt);
    long bysize[65] = {0}; for (long i = 0; i < NP; i++) bysize[PS[i]]++;
    printf("Bahnen (Paare) nach Groesse:"); for (int s = 1; s <= q; s++) if (bysize[s]) printf(" %d:%ld", s, bysize[s]);
    printf("  gesamt %ld  (%.1f s)\n", NP, omp_get_wtime() - t0);
    { unsigned long long pts = 0; for (int s = 1; s <= q; s++) pts += (unsigned long long)bysize[s] * s; printf("Kontrolle: Summe Groessen = %llu, 2^(m-1) = %u\n", pts, half); }

    /* 2. erlaubte kurze Bahnen */
    NS = 0; for (long i = 0; i < NP; i++) if (PS[i] < q && PS[i] <= BUD) NS++;
    if (NS > 64 * WM) { fprintf(stderr, "zu viele kurze Bahnen: %d\n", NS); return 2; }
    SREP = malloc(sizeof(uint32_t) * (NS + 1)); SWT = malloc(sizeof(int) * (NS + 1)); SROT = malloc(sizeof(uint32_t) * (NS + 1) * q);
    { int j = 0; for (long i = 0; i < NP; i++) if (PS[i] < q && PS[i] <= BUD) { SREP[j] = P[i]; SWT[j] = PS[i]; rots(P[i], SROT + (long)j * q); j++; } }
    memset(LIGHT, 0, sizeof LIGHT); memset(&ALLS, 0, sizeof ALLS);
    for (int s = 0; s < NS; s++) { mset(&ALLS, s); for (int r = SWT[s]; r < 64; r++) mset(&LIGHT[r], s); }
    printf("erlaubte kurze Bahnen (Groesse <= %d): %d\n", BUD, NS);

    /* 3. harte Klassen */
    unsigned char *HARD = malloc(NP); long nhard = 0;
    #pragma omp parallel for reduction(+:nhard) schedule(static)
    for (long i = 0; i < NP; i++) {
        int h = 1;
        for (int s = 0; s < NS && h; s++) if (covers(SROT + (long)s * q, P[i])) h = 0;
        HARD[i] = (unsigned char)h; nhard += h;
    }
    printf("harte Klassen: %ld von %ld  (%.1f s)\n", nhard, NP, omp_get_wtime() - t0);

    u128 hist[128]; memset(hist, 0, sizeof hist);
    long long survivors = 0; u128 surv_w = 0; long long leaves = 0;

    if (K == 0) {
        long nr = 0; uint32_t *pts = malloc(sizeof(uint32_t) * NP);
        for (long i = 0; i < NP; i++) pts[nr++] = P[i];
        msk *kk; int nk = build_constraints(pts, nr, &kk);
        if (nk >= 0) {
            printf("Bedingungen nach Bereinigung: %d\n", nk); fflush(stdout);
            eng_t E = {0}; E.k = kk; E.nk = nk; E.hist = hist;
            msk z = {{0}}; rec(&E, z, z, 0, BUD); leaves = E.leaves; free(kk);
        } else printf("ein Punkt von keiner kurzen Bahn erreichbar\n");
        free(pts);
        goto report;
    }

    /* 4. Klassen voller Bahnen unter dem Normalisator */
    uint32_t *AREP; u128 *AW; long NA = 0;
    {
        long nfull = bysize[q];
        AREP = malloc(sizeof(uint32_t) * nfull); AW = malloc(sizeof(u128) * nfull);
        if (NOSYM) {
            for (long i = 0; i < NP; i++) if (PS[i] == q) { AREP[NA] = P[i]; AW[NA] = 1; NA++; }
        } else {
            uint64_t *vis = calloc(half / 64 + 1, 8);
            uint32_t *stk = malloc(sizeof(uint32_t) * (nfull + 1));
            u128 sum = 0;
            for (long i = 0; i < NP; i++) {
                if (PS[i] != q) continue;
                uint32_t r0 = P[i];
                if (vis[r0 >> 6] >> (r0 & 63) & 1) continue;
                long sp = 0, csz = 0; stk[sp++] = r0; vis[r0 >> 6] |= 1ull << (r0 & 63);
                while (sp) {
                    uint32_t x = stk[--sp]; csz++;
                    for (int g = 0; g < NGEN; g++) {
                        uint32_t y = canon(gen_apply(g, x));
                        if (!(vis[y >> 6] >> (y & 63) & 1)) { vis[y >> 6] |= 1ull << (y & 63); stk[sp++] = y; }
                    }
                }
                AREP[NA] = r0; AW[NA] = csz; NA++; sum += csz;
            }
            free(vis); free(stk);
            printf("Normalisator-Klassen voller Bahnen: %ld (Erzeuger %d), Summe Klassengroessen ", NA, NGEN);
            print128(sum); printf(" = %ld volle Bahnen%s\n", nfull, sum == (u128)nfull ? "" : "  FEHLER");
        }
        printf("(%.1f s)\n", omp_get_wtime() - t0); fflush(stdout);
    }

    uint64_t *seen = (K == 2) ? calloc(half / 64 + 1, 8) : NULL;
    uint32_t *ball = NULL; long VB = 0;
    if (K == 2) { /* Kugelgroesse */
        long c = 1; VB = 1; for (int i = 1; i <= R; i++) { c = c * (m - i + 1) / i; VB += c; }
        ball = malloc(sizeof(uint32_t) * VB);
    }
    uint32_t *UA = malloc(sizeof(uint32_t) * NP); unsigned char *UAH = malloc(NP);
    uint32_t *UH = malloc(sizeof(uint32_t) * NP);
    long long cls_done = 0;
    for (long a = 0; a < NA; a++) {
        if (TLIM > 0 && omp_get_wtime() - t0 > TLIM) { printf("ZEITGRENZE nach %lld von %ld Klassen\n", cls_done, NA); goto report_partial; }
        uint32_t A = AREP[a], rA[64]; rots(A, rA);
        /* Rest nach A */
        long nua = 0, nuh = 0;
        for (long i = 0; i < NP; i++) if (!covers(rA, P[i])) { UA[nua] = P[i]; UAH[nua] = HARD[i]; nua++; if (HARD[i]) UH[nuh++] = P[i]; }
        if (K == 1) {
            if (nuh > 0) { cls_done++; continue; }
            msk *kk; int nk = build_constraints(UA, nua, &kk);
            if (nk < 0) { cls_done++; continue; }
            u128 h2[128]; memset(h2, 0, sizeof h2);
            eng_t E = {0}; E.k = kk; E.nk = nk; E.hist = h2; E.A = A; E.haveA = 1;
            msk z = {{0}}; rec(&E, z, z, 0, BUD); free(kk); leaves += E.leaves;
            int any = 0; for (int t = 0; t <= BUD; t++) { hist[t] += h2[t] * AW[a]; if (h2[t]) any = 1; }
            if (any) { survivors++; surv_w += AW[a]; }
            cls_done++; continue;
        }
        /* K == 2: Kandidaten B */
        long nb = 0; uint32_t *Bl;
        if (nuh == 0) {
            Bl = malloc(sizeof(uint32_t) * bysize[q]);
            for (long i = 0; i < NP; i++) if (PS[i] == q && P[i] != A) Bl[nb++] = P[i];
        } else {
            uint32_t h1 = UH[0];
            /* Kugel um h1 */
            long nbz = 0;
            { int pos[64]; int d = 0; pos[0] = -1; uint32_t x = h1; ball[nbz++] = x;
              /* iterative Aufzaehlung aller Teilmengen <= R Positionen */
              int idx[64]; int depth = 0; idx[0] = 0;
              while (1) {
                  if (depth < R && idx[depth] < m) {
                      int i = idx[depth];
                      x ^= 1u << i; ball[nbz++] = x; pos[depth] = i;
                      depth++; idx[depth] = i + 1; continue;
                  }
                  if (depth == 0) break;
                  depth--; x ^= 1u << pos[depth]; idx[depth] = pos[depth] + 1;
              }
              (void)d;
            }
            if (nbz != VB) { fprintf(stderr, "Kugelfehler %ld %ld\n", nbz, VB); return 3; }
            #pragma omp parallel for schedule(static)
            for (long i = 0; i < nbz; i++) ball[i] = canon(ball[i]);
            Bl = malloc(sizeof(uint32_t) * (nbz + 1));
            for (long i = 0; i < nbz; i++) {
                uint32_t y = ball[i];
                if (seen[y >> 6] >> (y & 63) & 1) continue;
                seen[y >> 6] |= 1ull << (y & 63);
                if (y != A && osize(y) == q) Bl[nb++] = y;
            }
            for (long i = 0; i < nbz; i++) { uint32_t y = ball[i]; seen[y >> 6] &= ~(1ull << (y & 63)); }
        }
        long long sv = 0, lv = 0;
        u128 h2[128]; memset(h2, 0, sizeof h2);
        #pragma omp parallel
        {
            u128 hl[128]; memset(hl, 0, sizeof hl); long long svl = 0, lvl = 0;
            uint32_t *rest = NULL; long restcap = 0;
            #pragma omp for schedule(dynamic, 64)
            for (long j = 0; j < nb; j++) {
                uint32_t Bw = Bl[j], rB[64]; rots(Bw, rB);
                int ok = 1;
                for (long i = 0; i < nuh; i++) if (!covers(rB, UH[i])) { ok = 0; break; }
                if (!ok) continue;
                svl++;
                if (!rest) { restcap = nua + 1; rest = malloc(sizeof(uint32_t) * restcap); }
                long nr = 0;
                for (long i = 0; i < nua; i++) if (!covers(rB, UA[i])) rest[nr++] = UA[i];
                msk *kk; int nk = build_constraints(rest, nr, &kk);
                if (nk < 0) continue;
                eng_t E = {0}; E.k = kk; E.nk = nk; E.hist = hl; E.A = A; E.haveA = 1; E.Bw = Bw; E.haveB = 1;
                msk z = {{0}}; rec(&E, z, z, 0, BUD); free(kk); lvl += E.leaves;
            }
            free(rest);
            #pragma omp critical
            { for (int t = 0; t < 128; t++) h2[t] += hl[t]; sv += svl; lv += lvl; }
        }
        free(Bl);
        for (int t = 0; t <= BUD; t++) hist[t] += h2[t] * AW[a];
        survivors += sv; surv_w += (u128)sv * AW[a]; leaves += lv;
        cls_done++;
        if (NA <= 200 || cls_done % 500 == 0) {
            printf("  Klasse %lld/%ld A=%x Gewicht ", cls_done, NA, A); print128(AW[a]);
            printf(" Rest %ld hart %ld Kandidaten B %ld harte ueberdeckt %lld  (%.1f s)\n", nua, nuh, nb, sv, omp_get_wtime() - t0);
            fflush(stdout);
        }
    }
    goto report;
report_partial:
    printf("UNVOLLSTAENDIG\n");
report:
    printf("Blaetter (ungewichtet): %lld\n", leaves);
    if (K == 2) { printf("geordnete Paare (A Vertreter), die alle harten Klassen ueberdecken: %lld, gewichtet ", survivors); print128(surv_w); printf("\n"); }
    if (K == 1) { printf("Klassen A mit Ueberdeckung: %lld, gewichtet ", survivors); print128(surv_w); printf("\n"); }
    printf("Ueberdeckungen nach Zahl kurzer Paare (k=%d volle Bahnen%s):\n", K, K == 2 ? ", ungeordnet" : "");
    u128 tot = 0;
    for (int t = 0; t <= BUD; t++) {
        u128 v = hist[t];
        if (K == 2) { if (v & 1) printf("  WARNUNG ungerade geordnete Zahl bei %d\n", t); v /= 2; }
        if (v) { printf("  kurz %d Paare (Paare gesamt %d, Woerter %d): ", t, K * q + t, 2 * (K * q + t)); print128(v); printf("\n"); }
        tot += v;
    }
    printf("SUMME Ueberdeckungen: "); print128(tot); printf("\n");
    if (VERIFY) printf("Brute-Force-Nachpruefung der Blaetter: %lld ok, %lld falsch\n", VER_OK, VER_BAD);
    printf("Laufzeit %.1f s\n", omp_get_wtime() - t0);
    if (OUT) fclose(OUT);
    return 0;
}
