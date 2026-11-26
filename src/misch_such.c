/* misch_such.c -- SUCHCODE Auftrag M (26.11.2026), Schritt 3: gemeinsame Suche "K Nebenklassen von <1,a> plus f freie Zeilen".
 *
 * Einheiten: K Paare und f freie Zeilen. Paar i hat das Wort a_i = (U_i, W_i) und die Zeilen a_i und b_i = a_i ^ MW
 * (MW = Maske der W-Spalten m1..m-1, also b_i = (U_i, -W_i)). Freie Zeile: ein Wort, eine Zeile.
 * Zug (u, j): Bit j im Wort der Einheit u kippen. Bei einem Paar kippt dabei Bit j in a_i UND b_i (U- oder W-Spalte gleich).
 * Klasse c = {x, -x} (kanonisch: Bit m-1 = 0) ist ueberdeckt, wenn eine Zeile Abstand <= R zu x oder -x hat.
 * Verfahren wie src/evo/p018.c (gewichtete Tabu-Suche mit Make/Break-Tabellen), aber auf Einheiten statt Zeilen:
 *   cs[c]: Bits 0-7 Zahl der Einheiten, die c ueberdecken; Bits 8-15 XOR der (Einheit+1).
 *   umaske(u,c): Bits j, nach deren Kippen die Einheit u die Klasse c ueberdeckt (ODER ueber ihre Zeilen).
 *   MK[u][j] = Summe Gewicht der offenen c mit Bit j in umaske(u,c)
 *   BK[u][j] = Summe Gewicht der c mit einzigem Ueberdecker u und Bit j nicht in umaske(u,c)
 *   Zugwert BK - MK = exakte Aenderung der gewichteten Zahl offener Klassen.
 * Aufruf: misch_such m d K m1 f STARTWERT SEKUNDEN PRAEFIX [--stag SEK]
 *   Ausgabe PRAEFIX.paar.txt (K Zeilen "U | W"), PRAEFIX.frei.txt (f Zeilen), PRAEFIX.matrix.txt (2K+f Zeilen), bester Stand.
 * Bauen: gcc -O3 -march=native -o build/misch_such src/misch_such.c   (Selbstpruefung: -DPRUEF)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define RAUSCHEN 0.01

typedef uint32_t u32;
typedef int32_t v8i __attribute__((vector_size(32)));
#define NV 4

static int m, R, K, F, NU, m1, nv;
static u32 FULL, TOP, MW;
static u32 *w;
static uint32_t *wt;
static uint16_t *cs;
static v8i *MK, *BK;
static int64_t *tabu;
static u32 *openl; static int32_t *opos; static long nopen;
static u32 **mRnoj; static long nRnoj;
static u32 *mR, *mR1, *mBall; static long nR, nR1, nBall;
static uint32_t *stamp; static uint32_t stampz = 0;
static v8i SH[NV];

static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs >> 12; rs ^= rs << 25; rs ^= rs >> 27; return rs * 2685821657736338717ULL; }
static inline double rndu(void) { return (rnd() >> 11) * (1.0 / 9007199254740992.0); }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline u32 canon(u32 x) { return (x & TOP) ? (x ^ FULL) : x; }
static inline int paar(int u) { return u < K; }
static inline int deckt(u32 r, u32 c) { int p = __builtin_popcount(r ^ c); return p <= R || p >= m - R; }
static inline int udeckt(int u, u32 a, u32 c) { return deckt(a, c) || (paar(u) && deckt(a ^ MW, c)); }
static inline u32 cmaske(u32 r, u32 c) {
    u32 x = r ^ c; int p = __builtin_popcount(x);
    if (p < R || m - p < R) return FULL;
    if (p == R || p == R + 1) return x;
    if (m - p == R || m - p == R + 1) return x ^ FULL;
    return 0;
}
static inline u32 umaske(int u, u32 c) { u32 a = w[u]; u32 r = cmaske(a, c); if (paar(u)) r |= cmaske(a ^ MW, c); return r; }
static inline int32_t mk(int i, int j) { return ((int32_t *)&MK[i * NV])[j]; }
static inline int32_t bk(int i, int j) { return ((int32_t *)&BK[i * NV])[j]; }
static inline void addbits(v8i *row, u32 bits, int32_t d) {
    v8i b = (v8i){0} + (int32_t)bits;
    for (int v = 0; v < nv; v++) row[v] += ((b >> SH[v]) & 1) * d;
}
static inline void open_add(u32 c) { opos[c] = nopen; openl[nopen++] = c; }
static inline void open_del(u32 c) { long p = opos[c]; u32 l = openl[--nopen]; openl[p] = l; opos[l] = p; opos[c] = -1; }
static inline void make_andere(u32 c, int ohne, int32_t d) {
    for (int v = 0; v < NU; v++) { if (v == ohne) continue; u32 mm = umaske(v, c); if (mm) addbits(&MK[v * NV], mm, d); }
}
static inline void break_von(u32 c, int s, int32_t d) {
    u32 mm = (~umaske(s, c)) & FULL; if (mm) addbits(&BK[s * NV], mm, d);
}
static inline u32 neuer_stempel(void) { if (++stampz == 0) { memset(stamp, 0, sizeof(uint32_t) * TOP); stampz = 1; } return stampz; }

static void zeile_neu(int u) {
    for (int v = 0; v < NV; v++) { MK[u * NV + v] = (v8i){0}; BK[u * NV + v] = (v8i){0}; }
    u32 a = w[u], st = neuer_stempel();
    int nr = paar(u) ? 2 : 1;
    for (int z = 0; z < nr; z++) {
        u32 r = z ? (a ^ MW) : a;
        for (long t = 0; t < nR; t++) {          /* Abstand genau R: moeglicher Verlust */
            u32 c = canon(r ^ mR[t]); if (stamp[c] == st) continue; stamp[c] = st;
            if ((cs[c] & 255) == 1) { u32 mm = (~umaske(u, c)) & FULL; if (mm) addbits(&BK[u * NV], mm, (int32_t)wt[c]); }
        }
    }
    st = neuer_stempel();
    for (int z = 0; z < nr; z++) {
        u32 r = z ? (a ^ MW) : a;
        for (long t = 0; t < nR1; t++) {         /* Abstand genau R+1: moeglicher Gewinn */
            u32 c = canon(r ^ mR1[t]); if (stamp[c] == st) continue; stamp[c] = st;
            if ((cs[c] & 255) == 0) { u32 mm = umaske(u, c); if (mm) addbits(&MK[u * NV], mm, (int32_t)wt[c]); }
        }
    }
}

static void kippe(int u, int j) {
    u32 ao = w[u], an = ao ^ (1u << j);
    uint16_t tag = (uint16_t)((u + 1) << 8);
    u32 *L = mRnoj[j];
    u32 st = neuer_stempel();
    int nr = paar(u) ? 2 : 1;
    for (int z = 0; z < nr; z++) {
        u32 ro = z ? (ao ^ MW) : ao, rn = ro ^ (1u << j);
        for (int s = 0; s < 2; s++) {
            u32 basis = s ? rn : ro;
            for (long t = 0; t < nRnoj; t++) {
                u32 c = canon(basis ^ L[t]);
                if (stamp[c] == st) continue; stamp[c] = st;
                int alt = udeckt(u, ao, c), neu = udeckt(u, an, c);
                if (alt == neu) continue;
                uint16_t v = cs[c];
                if (alt) {
                    v = (uint16_t)((v - 1) ^ tag); cs[c] = v;
                    int n = v & 255;
                    if (n == 0) { open_add(c); make_andere(c, u, (int32_t)wt[c]); }
                    else if (n == 1) break_von(c, (v >> 8) - 1, (int32_t)wt[c]);
                } else {
                    int n = v & 255;
                    if (n == 0) { open_del(c); make_andere(c, u, -(int32_t)wt[c]); }
                    else if (n == 1) break_von(c, (v >> 8) - 1, -(int32_t)wt[c]);
                    cs[c] = (uint16_t)((v + 1) ^ tag);
                }
            }
        }
    }
    w[u] = an;
    zeile_neu(u);
}

static void alles_neu(void) {
    long N = (long)TOP;
    memset(cs, 0, N * 2);
    for (int u = 0; u < NU; u++) {
        u32 st = neuer_stempel();
        int nr = paar(u) ? 2 : 1;
        for (int z = 0; z < nr; z++) {
            u32 r = z ? (w[u] ^ MW) : w[u];
            for (long t = 0; t < nBall; t++) { u32 c = canon(r ^ mBall[t]); if (stamp[c] == st) continue; stamp[c] = st; cs[c] = (uint16_t)((cs[c] + 1) ^ ((u + 1) << 8)); }
        }
    }
    nopen = 0;
    for (long c = 0; c < N; c++) { opos[c] = -1; if (!(cs[c] & 255)) open_add((u32)c); }
    for (int u = 0; u < NU; u++) zeile_neu(u);
}

static void schreibe(const char *praefix, const u32 *bw) {
    char p[4096], tmp[4200];
    snprintf(p, sizeof p, "%s.paar.txt", praefix); snprintf(tmp, sizeof tmp, "%s.tmp", p);
    FILE *f = fopen(tmp, "w"); if (!f) return;
    for (int i = 0; i < K; i++) { for (int j = 0; j < m; j++) { if (j == m1) fprintf(f, " |"); fprintf(f, "%s", (bw[i] >> j & 1) ? " -1" : "  1"); } fprintf(f, "\n"); }
    fclose(f); rename(tmp, p);
    snprintf(p, sizeof p, "%s.frei.txt", praefix); snprintf(tmp, sizeof tmp, "%s.tmp", p);
    f = fopen(tmp, "w"); if (!f) return;
    for (int i = K; i < NU; i++) { for (int j = 0; j < m; j++) fprintf(f, "%s", (bw[i] >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    fclose(f); rename(tmp, p);
    snprintf(p, sizeof p, "%s.matrix.txt", praefix); snprintf(tmp, sizeof tmp, "%s.tmp", p);
    f = fopen(tmp, "w"); if (!f) return;
    for (int i = 0; i < NU; i++) {
        int nr = paar(i) ? 2 : 1;
        for (int z = 0; z < nr; z++) { u32 r = z ? (bw[i] ^ MW) : bw[i]; for (int j = 0; j < m; j++) fprintf(f, "%s", (r >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    }
    fclose(f); rename(tmp, p);
}

int main(int argc, char **argv) {
    if (argc < 9) { fprintf(stderr, "Aufruf: %s m d K m1 f STARTWERT SEKUNDEN PRAEFIX [--stag SEK]\n", argv[0]); return 2; }
    m = atoi(argv[1]); int d = atoi(argv[2]); K = atoi(argv[3]); m1 = atoi(argv[4]); F = atoi(argv[5]);
    uint64_t seed = strtoull(argv[6], 0, 10); double sek = atof(argv[7]); const char *aus = argv[8];
    double stag = 1e18; for (int a = 9; a + 1 < argc; a++) if (!strcmp(argv[a], "--stag")) stag = atof(argv[a + 1]);
    R = (m - d) / 2; NU = K + F;
    if (m < 4 || m > 30 || 2 * (R + 1) >= m || NU < 1 || NU > 254 || m1 < 1 || m1 >= m) { fprintf(stderr, "Parameter ungueltig\n"); return 2; }
    double t0 = jetzt();
    rs = 0x9E3779B97F4A7C15ULL ^ (seed * 0xD1B54A32D192ED03ULL); for (int q = 0; q < 10; q++) rnd();
    FULL = (1u << m) - 1; TOP = 1u << (m - 1); nv = (m + 7) / 8;
    MW = FULL & ~((1u << m1) - 1);
    for (int v = 0; v < NV; v++) for (int l = 0; l < 8; l++) SH[v][l] = 8 * v + l;
    long N = (long)TOP;
    cs = calloc(N, 2); wt = malloc(N * 4); for (long c = 0; c < N; c++) wt[c] = 1;
    opos = malloc(N * sizeof(int32_t)); openl = malloc(N * sizeof(u32)); stamp = calloc(N, sizeof(uint32_t));
    w = malloc(NU * sizeof(u32));
    MK = aligned_alloc(32, NU * NV * sizeof(v8i)); BK = aligned_alloc(32, NU * NV * sizeof(v8i));
    tabu = calloc(NU * m, sizeof(int64_t));
    nR = nR1 = nBall = 0;
    for (u32 x = 0; x <= FULL; x++) { int p = __builtin_popcount(x); if (p == R) nR++; if (p == R + 1) nR1++; if (p <= R) nBall++; }
    mR = malloc(nR * 4); mR1 = malloc(nR1 * 4); mBall = malloc(nBall * 4);
    { long a = 0, b = 0, e = 0; for (u32 x = 0; x <= FULL; x++) { int p = __builtin_popcount(x); if (p == R) mR[a++] = x; if (p == R + 1) mR1[b++] = x; if (p <= R) mBall[e++] = x; } }
    mRnoj = malloc(m * sizeof(u32 *));
    nRnoj = 0; for (long t = 0; t < nR; t++) if (!(mR[t] & 1u)) nRnoj++;
    for (int j = 0; j < m; j++) { mRnoj[j] = malloc(nRnoj * 4); long q = 0; for (long t = 0; t < nR; t++) if (!(mR[t] >> j & 1)) mRnoj[j][q++] = mR[t]; }

    for (int i = 0; i < NU; i++) w[i] = (u32)rnd() & FULL;
    alles_neu();
    u32 *best = malloc(NU * sizeof(u32)); memcpy(best, w, NU * sizeof(u32));
    long bestopen = nopen; double tbest = jetzt(), tschreib = 0, tlog = jetzt();
    fprintf(stderr, "# misch_such m=%d d=%d R=%d K=%d (m1=%d, m2=%d) f=%d seed=%llu sek=%.0f; Klassen %ld, Woerter %d\n",
            m, d, R, K, m1, m - m1, F, (unsigned long long)seed, sek, N, 4 * K + 2 * F);
    schreibe(aus, best);
    int64_t it = 0, nbump = 0, nneu = 0;
    long lokalbest = nopen; double tlokal = jetzt();
    static int cand_i[1 << 14], cand_j[1 << 14];
    while (nopen > 0) {
        it++;
        if ((it & 255) == 0) {
            double t = jetzt();
            if (t - t0 > sek) break;
            if (t - tlokal > stag) {   /* Neustart vom Bestpunkt mit Stoerung */
                memcpy(w, best, NU * sizeof(u32));
                for (int q = 0; q < 6; q++) w[rnd() % NU] ^= 1u << (rnd() % m);
                for (long q = 0; q < N; q++) wt[q] = 1;
                memset(tabu, 0, NU * m * sizeof(int64_t));
                alles_neu();
                lokalbest = nopen; tlokal = t; nneu++;
            }
            if (t - tlog > 10) { fprintf(stderr, "t=%.0f it=%lld offen=%ld lokal=%ld best=%ld bumps=%lld neustarts=%lld\n", t - t0, (long long)it, nopen, lokalbest, bestopen, (long long)nbump, (long long)nneu); tlog = t; }
        }
        u32 c = openl[rnd() % nopen];
        int nc = 0, Dmin = 1 << 30;
        for (int u = 0; u < NU; u++) {
            int nr = paar(u) ? 2 : 1;
            for (int z = 0; z < nr; z++) {
                u32 r = z ? (w[u] ^ MW) : w[u];
                u32 x = c ^ r; int dd = __builtin_popcount(x); int D = dd < m - dd ? dd : m - dd;
                if (D < Dmin) { Dmin = D; nc = 0; }
                if (D == Dmin) { u32 bits = (dd <= m - dd) ? x : (x ^ FULL);
                    while (bits && nc < (1 << 14)) { int j = __builtin_ctz(bits); bits &= bits - 1; cand_i[nc] = u; cand_j[nc] = j; nc++; } }
            }
        }
        int wi = -1, wj = -1;
        if (rndu() < RAUSCHEN) { int q = rnd() % nc; wi = cand_i[q]; wj = cand_j[q]; }
        else {
            int32_t bs = INT32_MAX; int ties = 0;
            for (int u = 0; u < NU; u++) for (int j = 0; j < m; j++) {
                if (tabu[u * m + j] > it) continue;
                int32_t s = bk(u, j) - mk(u, j);
                if (s < bs) { bs = s; wi = u; wj = j; ties = 1; }
                else if (s == bs) { ties++; if (rnd() % ties == 0) { wi = u; wj = j; } }
            }
            if (wi < 0) { int q = rnd() % nc; wi = cand_i[q]; wj = cand_j[q]; }
            if (bs >= 0) {   /* kein gewichteter Gewinn: offene Klassen schwerer */
                uint32_t wmax = 0;
                for (long q = 0; q < nopen; q++) { u32 o = openl[q]; wt[o]++; if (wt[o] > wmax) wmax = wt[o]; make_andere(o, -1, 1); }
                ++nbump;
                if (wmax > (1u << 14)) {   /* Ueberlauf vermeiden: Gewichte halbieren, alles neu */
                    for (long q = 0; q < N; q++) wt[q] = (wt[q] + 1) / 2;
                    alles_neu();
                }
            }
        }
        kippe(wi, wj);
        tabu[wi * m + wj] = it + 6 + (long)(rnd() % 4);
#ifdef PRUEF
        if (it % 397 == 0) {
            long n0 = nopen; v8i *a1 = malloc(NU * NV * 32), *a2 = malloc(NU * NV * 32); uint16_t *c0 = malloc(N * 2);
            memcpy(a1, MK, NU * NV * 32); memcpy(a2, BK, NU * NV * 32); memcpy(c0, cs, N * 2);
            alles_neu();
            if (n0 != nopen || memcmp(c0, cs, N * 2) || memcmp(a1, MK, NU * NV * 32) || memcmp(a2, BK, NU * NV * 32)) { fprintf(stderr, "PRUEF FEHLER it=%lld\n", (long long)it); return 3; }
            /* direkte Zaehlung der offenen Klassen ueber alle Zeilen */
            long direkt = 0;
            for (long cc = 0; cc < N; cc++) { int ok = 0; for (int u = 0; u < NU && !ok; u++) ok = udeckt(u, w[u], (u32)cc); if (!ok) direkt++; }
            if (direkt != nopen) { fprintf(stderr, "PRUEF FEHLER direkt it=%lld\n", (long long)it); return 3; }
            free(a1); free(a2); free(c0);
        }
#endif
        if (nopen < lokalbest) { lokalbest = nopen; tlokal = jetzt(); }
        if (nopen < bestopen) {
            bestopen = nopen; memcpy(best, w, NU * sizeof(u32)); tbest = jetzt();
            if (tbest - tschreib > 1.0 || nopen == 0) { schreibe(aus, best); tschreib = tbest; }
        }
    }
    schreibe(aus, best);
    printf("%s nach %.1f s: beste Zahl offener Klassen %ld (offene Punkte %ld), t_best=%.1f, it=%lld, bumps=%lld, neustarts=%lld\n",
           bestopen == 0 ? "LOESUNG" : "ZEITGRENZE", jetzt() - t0, bestopen, 2 * bestopen, tbest - t0, (long long)it, (long long)nbump, (long long)nneu);
    return bestopen == 0 ? 0 : 1;
}
