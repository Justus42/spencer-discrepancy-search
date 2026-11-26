/* misch_alt.c -- SUCHCODE Auftrag M (26.11.2026), Schritt 3: gemeinsame Suche "K Nebenklassen von <1,a> plus f freie Zeilen",
 * zweite Fassung (die erste, src/misch_such.c mit Einzelbit-Zuegen, blieb bei (12,1) weit weg; siehe Laufprotokoll).
 *
 * Zustand: Paare (U_i | W_i), i < K, Zeilen (U_i, W_i), (U_i, -W_i); freie Zeilen FR_q, q < f. Klasse = {x, -x}.
 * Paar i ueberdeckt (y,z) genau dann, wenn |U_i y| + |W_i z| >= d. Freie Zeile r ueberdeckt x, wenn |<r,x>| >= d.
 * Runde = eine Halbphase plus Neuwahl der freien Zeilen:
 *   Halbphase (Seite 0: U fest, W frei; Seite 1: W fest, U frei): Klassen, die eine freie Zeile ueberdeckt, sind erledigt.
 *     Zug = eine ganze Zeile G_i der freien Seite durch ein beliebiges Wort g ersetzen (alle 2^(mg-1) Woerter, exakt bewertet,
 *     gewichtete Zahl offener Klassen). Tabu: das alte Wort von Zeile i fuer einige Zuege. Kein verbessernder Zug: Gewichte der
 *     offenen Klassen +1. Ende nach STAG Zuegen ohne neue Bestzahl der Phase; der beste Stand der Phase wird uebernommen.
 *   Freie Zeilen: nacheinander jede durch die Zeile ersetzen, die die meisten der sonst offenen Klassen ueberdeckt (exakt, alle Zeilen).
 * Neustart aus Zufall nach NEUSTART Runden ohne neue Bestzahl.
 * Aufruf: misch_alt m d K m1 f STARTWERT SEKUNDEN PRAEFIX [--stag N] [--neustart N]
 * Ausgabe PRAEFIX.paar.txt, PRAEFIX.frei.txt, PRAEFIX.matrix.txt (bester Stand).
 * Bauen: gcc -O3 -march=native -o build/misch_alt src/misch_alt.c   (Selbstpruefung der Zaehlung: -DPRUEF)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef uint32_t u32;
#define MAXK 32
static int m, m1, m2, d, K, F, R;
static u32 FULL;
static u32 U[MAXK], W[MAXK], FR[MAXK];
static uint32_t *wt; static uint8_t *freecnt; static long NC;
/* Phase */
static int mf, mg, NX, NZ, NG, L, mgb;
static uint8_t *Tlz;      /* [z*NG+gi] Stufe von |<g,z>| */
static int32_t *cls;      /* [q] globale Klasse */
static uint8_t *act, *pc, *px;
static uint8_t *kreq;     /* [i*NX+xi] Mindeststufe (L = nie) */
static int gcur[MAXK];
static int64_t *accG, *accL, *gain, *loss;
static int64_t *tabu;
static long nopen;

static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs >> 12; rs ^= rs << 25; rs ^= rs >> 27; return rs * 2685821657736338717ULL; }
static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static inline u32 canon(u32 p) { return (p & 1) ? (p ^ FULL) : p; }
static inline int deckt(u32 r, u32 p) { int c = __builtin_popcount(r ^ p); return c <= R || c >= m - R; }
static inline int absip(u32 a, u32 b, int n) { int s = n - 2 * __builtin_popcount(a ^ b); return s < 0 ? -s : s; }
static inline int paar_deckt(int i, u32 p) { return absip(U[i], p & ((1u << m1) - 1), m1) + absip(W[i], p >> m1, m2) >= d; }

static void freie_zaehlen(void) {
    memset(freecnt, 0, NC);
    for (long n = 0; n < NC; n++) { u32 p = (u32)n << 1; for (int q = 0; q < F; q++) freecnt[n] += deckt(FR[q], p); }
}

static void phase_aufbau(int seite) {
    mf = seite ? m2 : m1; mg = seite ? m1 : m2;
    NX = 1 << (mf - 1); NZ = 1 << mg; NG = 1 << (mg - 1); mgb = mg; L = mg / 2 + 1;
    u32 *Fr = seite ? W : U, *Gr = seite ? U : W;
    int par = mg & 1;
    for (int z = 0; z < NZ; z++) for (int gi = 0; gi < NG; gi++) { int v = absip((u32)gi << 1, (u32)z, mg); Tlz[(long)z * NG + gi] = (uint8_t)((v - par) / 2); }
    for (int i = 0; i < K; i++) for (int xi = 0; xi < NX; xi++) {
        int r = d - absip(Fr[i], (u32)xi << 1, mf);
        int k = (r <= par) ? 0 : (r - par + 1) / 2; if (k > L) k = L;
        kreq[i * NX + xi] = (uint8_t)k;
    }
    for (int i = 0; i < K; i++) { u32 g = Gr[i]; if (g & 1) g ^= (1u << mg) - 1; gcur[i] = (int)(g >> 1); }
    nopen = 0;
    for (int xi = 0; xi < NX; xi++) for (int z = 0; z < NZ; z++) {
        long q = ((long)xi << mgb) | z; u32 x = (u32)xi << 1, p;
        p = seite ? ((u32)z | (x << m1)) : (x | ((u32)z << m1));
        p = canon(p); cls[q] = (int32_t)(p >> 1);
        act[q] = freecnt[p >> 1] == 0;
        uint8_t c = 0, xr = 0;
        for (int i = 0; i < K; i++) if (Tlz[(long)z * NG + gcur[i]] >= kreq[i * NX + xi]) { c++; xr ^= (uint8_t)(i + 1); }
        pc[q] = c; px[q] = xr;
        if (act[q] && c == 0) nopen++;
    }
}
static void phase_zurueck(int seite) {
    u32 *Gr = seite ? U : W;
    for (int i = 0; i < K; i++) Gr[i] = (u32)gcur[i] << 1;
}
static void bewerten(void) {
    long A = (long)K * NZ * (L + 1);
    memset(accG, 0, A * 8); memset(accL, 0, A * 8);
    long NQ = (long)NX * NZ;
    for (long q = 0; q < NQ; q++) {
        if (!act[q] || pc[q] > 1) continue;
        int xi = (int)(q >> mgb), z = (int)(q & (NZ - 1)); int64_t w = wt[cls[q]];
        if (pc[q] == 0) { for (int i = 0; i < K; i++) accG[((long)i * NZ + z) * (L + 1) + kreq[i * NX + xi]] += w; }
        else { int i = px[q] - 1; accL[((long)i * NZ + z) * (L + 1) + kreq[i * NX + xi]] += w; }
    }
    memset(gain, 0, (long)K * NG * 8); memset(loss, 0, (long)K * NG * 8);
    int64_t pre[40], suf[40];
    for (int i = 0; i < K; i++) for (int z = 0; z < NZ; z++) {
        int64_t *aG = accG + ((long)i * NZ + z) * (L + 1), *aL = accL + ((long)i * NZ + z) * (L + 1);
        int64_t sG = 0, sL = 0; for (int k = 0; k <= L; k++) { sG += aG[k]; sL += aL[k]; }
        if (!sG && !sL) continue;
        int64_t s = 0; for (int t = 0; t < L; t++) { s += aG[t]; pre[t] = s; }            /* gedeckt, wenn Stufe t >= k */
        s = 0; for (int t = L - 1; t >= 0; t--) { s += aL[t + 1]; suf[t] = s; }          /* offen, wenn Stufe t < k */
        const uint8_t *T = Tlz + (long)z * NG; int64_t *gi_ = gain + (long)i * NG, *lo = loss + (long)i * NG;
        if (sG) for (int g = 0; g < NG; g++) gi_[g] += pre[T[g]];
        if (sL) for (int g = 0; g < NG; g++) lo[g] += suf[T[g]];
    }
}
static void setze(int i, int gneu) {
    int galt = gcur[i];
    long NQ = (long)NX * NZ;
    for (long q = 0; q < NQ; q++) {
        int xi = (int)(q >> mgb), z = (int)(q & (NZ - 1)); int k = kreq[i * NX + xi];
        int a = Tlz[(long)z * NG + galt] >= k, b = Tlz[(long)z * NG + gneu] >= k;
        if (a == b) continue;
        if (a) { pc[q]--; px[q] ^= (uint8_t)(i + 1); if (pc[q] == 0 && act[q]) nopen++; }
        else { if (pc[q] == 0 && act[q]) nopen--; pc[q]++; px[q] ^= (uint8_t)(i + 1); }
    }
    gcur[i] = gneu;
}
static long direkt_offen(void) {
    long n0 = 0;
    for (long n = 0; n < NC; n++) { u32 p = (u32)n << 1; int ok = 0; for (int i = 0; i < K && !ok; i++) ok = paar_deckt(i, p); for (int q = 0; q < F && !ok; q++) ok = deckt(FR[q], p); if (!ok) n0++; }
    return n0;
}
static u32 *ball; static long nball; static int32_t *zcnt; static uint8_t *pcov;
static void freie_neu(void) {
    if (F == 0) return;
    for (long n = 0; n < NC; n++) { u32 p = (u32)n << 1; int ok = 0; for (int i = 0; i < K && !ok; i++) ok = paar_deckt(i, p); pcov[n] = (uint8_t)ok; }
    for (int q = 0; q < F; q++) {
        long ns = 0;
        for (long n = 0; n < NC; n++) if (!pcov[n] && freecnt[n] - deckt(FR[q], (u32)n << 1) == 0) ns++;
        if (ns == 0 || ns > 40000) continue;
        memset(zcnt, 0, NC * 4);
        for (long n = 0; n < NC; n++) {
            if (pcov[n] || freecnt[n] - deckt(FR[q], (u32)n << 1) != 0) continue;
            u32 p = (u32)n << 1;
            for (long t = 0; t < nball; t++) zcnt[canon(p ^ ball[t]) >> 1]++;
        }
        int32_t bm = -1; long nb = 0; u32 br = 0;
        for (long r = 0; r < NC; r++) { if (zcnt[r] > bm) { bm = zcnt[r]; nb = 1; br = (u32)r; } else if (zcnt[r] == bm && rnd() % (++nb) == 0) br = (u32)r; }
        u32 neu = br << 1;
        if (neu != FR[q]) {
            for (long n = 0; n < NC; n++) { u32 p = (u32)n << 1; freecnt[n] = (uint8_t)(freecnt[n] - deckt(FR[q], p) + deckt(neu, p)); }
            FR[q] = neu;
        }
    }
}
static void schreibe(const char *praefix, const u32 *bU, const u32 *bW, const u32 *bF) {
    char p[4096], tmp[4200]; FILE *f;
    snprintf(p, sizeof p, "%s.paar.txt", praefix); snprintf(tmp, sizeof tmp, "%s.tmp", p);
    if (!(f = fopen(tmp, "w"))) return;
    for (int i = 0; i < K; i++) { for (int j = 0; j < m1; j++) fprintf(f, "%s", (bU[i] >> j & 1) ? " -1" : "  1"); fprintf(f, " |"); for (int j = 0; j < m2; j++) fprintf(f, "%s", (bW[i] >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    fclose(f); rename(tmp, p);
    snprintf(p, sizeof p, "%s.frei.txt", praefix); snprintf(tmp, sizeof tmp, "%s.tmp", p);
    if (!(f = fopen(tmp, "w"))) return;
    for (int q = 0; q < F; q++) { for (int j = 0; j < m; j++) fprintf(f, "%s", (bF[q] >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    fclose(f); rename(tmp, p);
    snprintf(p, sizeof p, "%s.matrix.txt", praefix); snprintf(tmp, sizeof tmp, "%s.tmp", p);
    if (!(f = fopen(tmp, "w"))) return;
    for (int i = 0; i < K; i++) for (int s = 0; s < 2; s++) {
        for (int j = 0; j < m1; j++) fprintf(f, "%s", (bU[i] >> j & 1) ? " -1" : "  1");
        for (int j = 0; j < m2; j++) fprintf(f, "%s", (((bW[i] >> j) ^ s) & 1) ? " -1" : "  1");
        fprintf(f, "\n");
    }
    for (int q = 0; q < F; q++) { for (int j = 0; j < m; j++) fprintf(f, "%s", (bF[q] >> j & 1) ? " -1" : "  1"); fprintf(f, "\n"); }
    fclose(f); rename(tmp, p);
}
static void zufall(void) {
    for (int i = 0; i < K; i++) { U[i] = (u32)rnd() & ((1u << m1) - 1); W[i] = (u32)rnd() & ((1u << m2) - 1); }
    for (int q = 0; q < F; q++) FR[q] = (u32)rnd() & FULL;
    for (long n = 0; n < NC; n++) wt[n] = 1;
    freie_zaehlen();
}

int main(int argc, char **argv) {
    if (argc < 9) { fprintf(stderr, "Aufruf: %s m d K m1 f STARTWERT SEKUNDEN PRAEFIX [--stag N] [--neustart N]\n", argv[0]); return 2; }
    m = atoi(argv[1]); d = atoi(argv[2]); K = atoi(argv[3]); m1 = atoi(argv[4]); F = atoi(argv[5]);
    uint64_t seed = strtoull(argv[6], 0, 10); double sek = atof(argv[7]); const char *aus = argv[8];
    int64_t stag = 300, neustart = 20;
    for (int a = 9; a + 1 < argc; a++) { if (!strcmp(argv[a], "--stag")) stag = atoll(argv[a + 1]); if (!strcmp(argv[a], "--neustart")) neustart = atoll(argv[a + 1]); }
    m2 = m - m1; R = (m - d) / 2;
    if (m < 4 || m > 26 || m1 < 2 || m2 < 2 || m1 > 13 || m2 > 13 || K < 1 || K >= MAXK || F < 0 || F >= MAXK || 2 * R + 1 >= m) { fprintf(stderr, "Parameter ungueltig\n"); return 2; }
    double t0 = jetzt();
    rs = 0x9E3779B97F4A7C15ULL ^ (seed * 0xD1B54A32D192ED03ULL); for (int q = 0; q < 10; q++) rnd();
    FULL = (1u << m) - 1; NC = 1L << (m - 1);
    wt = malloc(NC * 4); freecnt = malloc(NC); zcnt = malloc(NC * 4); pcov = malloc(NC);
    int mmax = m1 > m2 ? m1 : m2;
    Tlz = malloc((1L << mmax) * (1L << (mmax - 1)));
    cls = malloc(NC * 4); act = malloc(NC); pc = malloc(NC); px = malloc(NC);
    kreq = malloc((long)K << mmax);
    accG = malloc((long)K * (1L << mmax) * 16 * 8); accL = malloc((long)K * (1L << mmax) * 16 * 8);
    gain = malloc((long)K * (1L << mmax) * 8); loss = malloc((long)K * (1L << mmax) * 8);
    tabu = calloc((long)K << mmax, 8);
    nball = 0; for (u32 x = 0; x <= FULL; x++) if (__builtin_popcount(x) <= R) nball++;
    ball = malloc(nball * 4); { long q = 0; for (u32 x = 0; x <= FULL; x++) if (__builtin_popcount(x) <= R) ball[q++] = x; }

    zufall();
    u32 bU[MAXK], bW[MAXK], bF[MAXK]; memcpy(bU, U, sizeof U); memcpy(bW, W, sizeof W); memcpy(bF, FR, sizeof FR);
    long best = NC + 1; double tbest = 0, tlog = jetzt();
    printf("# misch_alt m=%d d=%d R=%d K=%d (m1=%d, m2=%d) f=%d seed=%llu sek=%.0f Klassen %ld, Woerter %d, stag=%lld neustart=%lld\n",
           m, d, R, K, m1, m2, F, (unsigned long long)seed, sek, NC, 4 * K + 2 * F, (long long)stag, (long long)neustart);
    int64_t it = 0, runde = 0, letzte_verb = 0, neustarts = 0, bumps = 0;
    int seite = 0;
    while (jetzt() - t0 < sek && best > 0) {
        phase_aufbau(seite);
        long pbest = nopen; int pg[MAXK]; memcpy(pg, gcur, sizeof(int) * K);
        int64_t ohne = 0;
        while (ohne < stag && nopen > 0) {
            it++; ohne++;
            if ((it & 15) == 0 && jetzt() - t0 > sek) break;
            bewerten();
            int64_t bs = INT64_MAX; int bi = -1, bg = -1; long ties = 0;
            for (int i = 0; i < K; i++) for (int g = 0; g < NG; g++) {
                if (g == gcur[i] || tabu[((long)i << (mg - 1)) + g] > it) continue;
                int64_t s = loss[(long)i * NG + g] - gain[(long)i * NG + g];
                if (s < bs) { bs = s; bi = i; bg = g; ties = 1; } else if (s == bs && rnd() % (++ties) == 0) { bi = i; bg = g; }
            }
            if (bi < 0) break;
            if (bs >= 0) {
                long NQ = (long)NX * NZ; uint32_t wmax = 0;
                for (long q = 0; q < NQ; q++) if (act[q] && pc[q] == 0) { uint32_t *w = &wt[cls[q]]; (*w)++; if (*w > wmax) wmax = *w; }
                bumps++;
                if (wmax > (1u << 20)) for (long n = 0; n < NC; n++) wt[n] = (wt[n] + 1) / 2;
            }
            tabu[((long)bi << (mg - 1)) + gcur[bi]] = it + 5 + (int64_t)(rnd() % 5);
            setze(bi, bg);
            if (nopen < pbest) { pbest = nopen; memcpy(pg, gcur, sizeof(int) * K); ohne = 0; }
        }
        for (int i = 0; i < K; i++) if (gcur[i] != pg[i]) setze(i, pg[i]);
        phase_zurueck(seite);
        memset(tabu, 0, ((long)K << mmax) * 8);
        freie_neu();
        long offen = direkt_offen();
#ifdef PRUEF
        if (offen > pbest) { printf("PRUEF FEHLER: direkt %ld > Phase %ld\n", offen, pbest); return 3; }
        phase_aufbau(seite); if (nopen != offen) { printf("PRUEF FEHLER: Aufbau %ld != direkt %ld\n", nopen, offen); return 3; }
#endif
        runde++;
        if (offen < best) {
            best = offen; tbest = jetzt() - t0; letzte_verb = runde;
            memcpy(bU, U, sizeof U); memcpy(bW, W, sizeof W); memcpy(bF, FR, sizeof FR);
            schreibe(aus, bU, bW, bF);
        }
        if (jetzt() - tlog > 30 || best == 0) { printf("  Runde %lld: offen %ld, bestes %ld, Zuege %lld, Neustarts %lld, %.1f s\n", (long long)runde, offen, best, (long long)it, (long long)neustarts, jetzt() - t0); fflush(stdout); tlog = jetzt(); }
        seite ^= 1;
        if (neustart > 0 && runde - letzte_verb >= neustart) { zufall(); letzte_verb = runde; neustarts++; seite = 0; printf("  Neustart %lld nach Runde %lld (bestes %ld), %.1f s\n", (long long)neustarts, (long long)runde, best, jetzt() - t0); fflush(stdout); }
    }
    schreibe(aus, bU, bW, bF);
    printf("%s nach %.1f s: beste Zahl offener Klassen %ld (offene Punkte %ld), t_best=%.1f, Runden %lld, Zuege %lld, Gewichtsschritte %lld, Neustarts %lld\n",
           best == 0 ? "LOESUNG" : "ZEITGRENZE", jetzt() - t0, best, 2 * best, tbest, (long long)runde, (long long)it, (long long)bumps, (long long)neustarts);
    return best == 0 ? 0 : 1;
}
