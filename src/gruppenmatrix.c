/*
 * gruppenmatrix.c -- Suchcode (kein Beweis). Vollständige Aufzählung von Gruppenmatrizen A[g][h] = f(g^-1 h),
 * f: G -> {+1,-1}, G beliebige Gruppe der Ordnung n <= 31 (Verknüpfungstafel aus Datei), bis auf Symmetrie,
 * je Klasse Gegenbeispielsuche. Nach dem Muster von zirkulant.c (dort G = Z_n).
 *
 * f als Bitmaske (Bit x = 1 heißt f(x) = -1). Erlaubte Symmetrien, die disc(A) gleich lassen:
 *   f(x) -> f(k x), f(x) -> f(x k)   (Zeilen- bzw. Spaltenpermutation),
 *   f -> f nach einem Automorphismus von G (gleichzeitige Zeilen- und Spaltenpermutation),
 *   f -> -f (Komplement).
 * NICHT benutzt: f(x) -> f(x^-1) (Transponierte; bei nichtabelschem G nicht disc-gleich; bei abelschem G
 * ist es ohnehin ein Automorphismus). Die Symmetriegruppe H = { x -> phi(k x l) } wird aus der Tafel und der
 * Automorphismenliste der Datei berechnet (die Datei wird vom Python-Skript src/gruppen_erzeuge.py erzeugt;
 * dieses Programm prüft jeden Automorphismus noch einmal auf Homomorphie und Bijektivität).
 * Vertreter einer Klasse = kleinstes f der Bahn (mit Komplement) als ganze Zahl.
 *
 * Rand (-r): zusätzliche Einser-Zeile und Nullspalte, also (n+1) x (n+1); die Nullspalte wird weggelassen.
 * Gestrichene Spalten (-S k): nach der vollen Matrix (u = obere Schranke der Diskrepanz) alle k-Mengen
 * gestrichener Spalten, aber nur für Klassen mit u >= d - k (Streichen von k Spalten ändert disc um höchstens k).
 *
 * Je Klasse (und je Spaltenmenge):
 *  1. Filter: Tabu-Suche nach x mit max |(A x)_i| < d. Gelingt das nicht im Budget: Überlebender.
 *  2. Abstieg ab dem Gegenbeispiel: kleinstes gefundenes Maximum u = obere Schranke für disc.
 *     Jedes gefundene x wird unabhängig von der Tabu-Buchführung neu nachgerechnet.
 * Überlebende und Vertreter mit größtem u werden danach exakt geprüft (build/exakt_disc, Prüfcode).
 *
 * Aufruf: gruppenmatrix GRP d [-r] [-S k] [-p THREADS] [-o PRAEFIX] [-b BUDGET] [-a] [-T k/m]
 *   (-T: nur Teil k von m der Blöcke; -a: alle Klassen mit u nach PRAEFIX_alle.txt)
 * Ausgabe: Zusammenfassung auf stdout; PRAEFIX_ueberlebende.txt, PRAEFIX_top.txt (Vertreter hex [i j] u).
 * Bauen: gcc -O3 -march=native -pthread -o build/gruppenmatrix src/gruppenmatrix.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define MAXN 32
#define MAXAUT 1024
#define TOPMAX 2000

static int n, d, rand_, strike, nthreads = 4, budget = 400;
static const char *praefix = "gruppe";
static uint32_t maske;
static int tab[MAXN][MAXN], inv[MAXN], mulinv[MAXN][MAXN];   /* mulinv[g][h] = g^-1 h */
static int aut[MAXAUT][MAXN], naut;
static int nH;
static uint32_t (*htab)[4][256];       /* je Symmetrie: Bildtabelle je Byte (Bit y -> Bit sigma(y)) */

static uint64_t naechster_block = 0, bloecke, g_teil = 0, g_teile = 1;
static pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
static FILE *f_ueb, *f_alle;
static long g_klassen = 0, g_ueb = 0, g_hist[2 * MAXN + 2], g_bearb = 0, g_paare = 0;
static int g_umax = -1;
typedef struct { uint32_t a; int i, j; } eintrag_t;
static eintrag_t g_top[TOPMAX]; static int g_ntop = 0;
static long g_filterschritte = 0;

/* ---------------------------------------------------------------- Gruppe lesen und Symmetrien bilden */
static void lies_gruppe(const char *fn) {
    FILE *f = fopen(fn, "r");
    if (!f) { perror(fn); exit(2); }
    char line[4096];
    int zeile = 0, gelesen = 0;
    n = 0; naut = 0;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (!strncmp(line, "abelsch", 7)) continue;
        if (!strncmp(line, "aut ", 4)) {
            char *p = line + 4; int v, k, c = 0;
            if (naut >= MAXAUT) { fprintf(stderr, "zu viele Automorphismen\n"); exit(2); }
            while (sscanf(p, "%d%n", &v, &k) == 1) { if (c < MAXN) aut[naut][c] = v; c++; p += k; }
            if (c != n) { fprintf(stderr, "Automorphismus mit %d statt %d Einträgen\n", c, n); exit(2); }
            naut++;
            continue;
        }
        if (!n) { n = atoi(line); if (n < 2 || n > 31) { fprintf(stderr, "Ordnung %d nicht unterstützt\n", n); exit(2); } continue; }
        if (gelesen < n) {
            char *p = line; int v, k, c = 0;
            while (sscanf(p, "%d%n", &v, &k) == 1) { if (c < MAXN) tab[gelesen][c] = v; c++; p += k; }
            if (c != n) { fprintf(stderr, "Tafelzeile %d hat %d Einträge\n", gelesen, c); exit(2); }
            gelesen++;
        }
        (void)zeile;
    }
    fclose(f);
    if (gelesen != n) { fprintf(stderr, "Tafel unvollständig\n"); exit(2); }
    /* Selbstprüfung: neutrales Element 0, Inverse, Assoziativität, Automorphismen */
    for (int g = 0; g < n; g++) {
        if (tab[0][g] != g || tab[g][0] != g) { fprintf(stderr, "0 nicht neutral\n"); exit(2); }
        inv[g] = -1;
        for (int h = 0; h < n; h++) { if (tab[g][h] < 0 || tab[g][h] >= n) { fprintf(stderr, "Tafel außerhalb\n"); exit(2); } if (tab[g][h] == 0) inv[g] = h; }
        if (inv[g] < 0) { fprintf(stderr, "kein Inverses\n"); exit(2); }
    }
    for (int a = 0; a < n; a++) for (int b = 0; b < n; b++) for (int c = 0; c < n; c++)
        if (tab[tab[a][b]][c] != tab[a][tab[b][c]]) { fprintf(stderr, "nicht assoziativ\n"); exit(2); }
    for (int g = 0; g < n; g++) for (int h = 0; h < n; h++) mulinv[g][h] = tab[inv[g]][h];
    for (int k = 0; k < naut; k++) {
        int seen[MAXN] = {0};
        for (int g = 0; g < n; g++) {
            if (aut[k][g] < 0 || aut[k][g] >= n || seen[aut[k][g]]++) { fprintf(stderr, "Automorphismus %d keine Bijektion\n", k); exit(2); }
        }
        for (int a = 0; a < n; a++) for (int b = 0; b < n; b++)
            if (aut[k][tab[a][b]] != tab[aut[k][a]][aut[k][b]]) { fprintf(stderr, "Automorphismus %d kein Homomorphismus\n", k); exit(2); }
    }
}

static int cmp_perm(const void *x, const void *y) { return memcmp(x, y, MAXN); }

static void bilde_symmetrien(void) {
    /* alle x -> phi(k x l) ; dedupliziert; Standard: phi = Identität gehört zur Autorenliste (Datei enthält sie) */
    long total = (long)n * n * (naut ? naut : 1);
    uint8_t (*P)[MAXN] = calloc(total, MAXN);
    long c = 0;
    for (int k = 0; k < n; k++) for (int l = 0; l < n; l++) for (int a = 0; a < (naut ? naut : 1); a++) {
        for (int x = 0; x < n; x++) {
            int y = tab[tab[k][x]][l];
            P[c][x] = naut ? aut[a][y] : y;
        }
        c++;
    }
    qsort(P, total, MAXN, cmp_perm);
    long m = 0;
    for (long i = 0; i < total; i++) if (i == 0 || memcmp(P[i], P[i - 1], MAXN)) { if (m != i) memcpy(P[m], P[i], MAXN); m++; }
    nH = (int)m;
    htab = malloc((size_t)nH * sizeof *htab);
    for (int h = 0; h < nH; h++) {
        /* Bit y von f wandert auf Bit P[y] */
        for (int p = 0; p < 4; p++) for (int v = 0; v < 256; v++) {
            uint32_t w = 0;
            for (int i = 0; i < 8; i++) { int s = 8 * p + i; if (s < n && ((v >> i) & 1)) w |= 1u << P[h][s]; }
            htab[h][p][v] = w;
        }
    }
    free(P);
}

/* f kanonisch, wenn kein Element der Bahn (mit Komplement) kleiner ist */
static int kanonisch(uint32_t a) {
    for (int h = 0; h < nH; h++) {
        const uint32_t (*t)[256] = htab[h];
        uint32_t b = t[0][a & 255] | t[1][(a >> 8) & 255] | t[2][(a >> 16) & 255] | t[3][(a >> 24) & 255];
        if (b < a) return 0;
        if (((~b) & maske) < a) return 0;
    }
    return 1;
}

/* ---------------------------------------------------------------- Tabu-Suche (wie zirkulant.c, m Spalten) */
typedef struct { uint64_t s; } rng_t;
static inline uint64_t rnd(rng_t *g) { uint64_t x = g->s; x ^= x << 13; x ^= x >> 7; x ^= x << 17; return g->s = x; }
static inline int ex(int v, int T) { if (v < 0) v = -v; return v > T ? v - T : 0; }

static int tabu(int R, int m, int8_t A[][MAXN], int *x, int *r, int T, int maxschritte, rng_t *g, long *zaehler) {
    int tb[MAXN] = {0};
    int cost = 0; for (int i = 0; i < R; i++) cost += ex(r[i], T);
    if (!cost) return 1;
    int bestcost = cost;
    for (int it = 1; it <= maxschritte; it++) {
        (*zaehler)++;
        int bj = -1, bd = 1 << 30, nb = 0;
        for (int j = 0; j < m; j++) {
            int ds = 0, s2 = 2 * x[j];
            for (int i = 0; i < R; i++) ds += ex(r[i] - s2 * A[i][j], T) - ex(r[i], T);
            if (tb[j] > it && cost + ds >= bestcost) continue;
            if (ds < bd) { bd = ds; bj = j; nb = 1; }
            else if (ds == bd && (rnd(g) % (++nb)) == 0) bj = j;
        }
        if (bj < 0) continue;
        int s2 = 2 * x[bj];
        for (int i = 0; i < R; i++) r[i] -= s2 * A[i][bj];
        x[bj] = -x[bj];
        cost += bd;
        tb[bj] = it + 1 + (int)(rnd(g) % (unsigned)(m / 4 + 2));
        if (cost < bestcost) bestcost = cost;
        if (!cost) return 1;
    }
    return 0;
}

static int maxabs(int R, const int *r) { int mm = 0; for (int i = 0; i < R; i++) { int a = r[i] < 0 ? -r[i] : r[i]; if (a > mm) mm = a; } return mm; }

/* Matrix R x m in A; Liefert u (beste gefundene obere Schranke); *ueb = 1, wenn kein x mit max < d gefunden */
static int bearbeite(int8_t A[][MAXN], int R, int m, uint64_t seed, int *ueb, long *zaehler) {
    rng_t g = { 0x9E3779B97F4A7C15ULL ^ (seed * 0xD1B54A32D192ED03ULL) ^ ((uint64_t)n << 40) };
    for (int i = 0; i < 5; i++) rnd(&g);
    int x[MAXN], r[MAXN + 1], bx[MAXN], best = 1 << 30;
    int ok = 0;
    for (int start = 0; start < 8 && !ok; start++) {
        for (int j = 0; j < m; j++) x[j] = (rnd(&g) & 1) ? 1 : -1;
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < m; j++) s += A[i][j] * x[j]; r[i] = s; }
        int mm = maxabs(R, r);
        if (mm < best) { best = mm; memcpy(bx, x, sizeof x); }
        if (mm < d) { ok = 1; break; }
        if (tabu(R, m, A, x, r, d - 1, budget * 4, &g, zaehler)) { ok = 1; best = maxabs(R, r); memcpy(bx, x, sizeof x); }
    }
    *ueb = !ok;
    if (!ok) return best;
    {   /* Nachrechnen des Gegenbeispiels, unabhängig von r */
        int mm = 0;
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < m; j++) s += A[i][j] * bx[j]; if (s < 0) s = -s; if (s > mm) mm = s; }
        if (mm != best || mm >= d) { fprintf(stderr, "FEHLER Nachrechnung\n"); exit(3); }
    }
    for (int start = 0; start < 3; start++) {
        if (start == 0) memcpy(x, bx, sizeof x);
        else for (int j = 0; j < m; j++) x[j] = (rnd(&g) & 1) ? 1 : -1;
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < m; j++) s += A[i][j] * x[j]; r[i] = s; }
        int mm = maxabs(R, r); if (mm < best) best = mm;
        while (best > 0 && tabu(R, m, A, x, r, best - 1, budget, &g, zaehler)) {
            /* jedes neue Maximum aus x neu berechnen, nicht aus der laufend geführten Summe */
            int nn = 0;
            for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < m; j++) s += A[i][j] * x[j]; if (s < 0) s = -s; if (s > nn) nn = s; }
            if (nn != maxabs(R, r) || nn >= best) { fprintf(stderr, "FEHLER Abstieg\n"); exit(3); }
            best = nn;
        }
    }
    return best;
}

static void baue_matrix(uint32_t a, int8_t A[][MAXN]) {
    for (int g = 0; g < n; g++) for (int h = 0; h < n; h++) A[g][h] = ((a >> mulinv[g][h]) & 1) ? -1 : 1;
    if (rand_) for (int h = 0; h < n; h++) A[n][h] = 1;
}

static void *arbeiter(void *arg) {
    (void)arg;
    long kl = 0, ue = 0, hist[2 * MAXN + 2] = {0}, zaehler = 0, bearb = 0, paare = 0;
    int umax = -1; eintrag_t top[TOPMAX]; int ntop = 0;
    int8_t A[MAXN + 1][MAXN], B[MAXN + 1][MAXN];
    int R = n + rand_;
    for (;;) {
        uint64_t b = __atomic_fetch_add(&naechster_block, 1, __ATOMIC_RELAXED) * g_teile + g_teil;
        if (b >= bloecke) break;
        uint64_t lo = b << 12, hi = lo + (1ULL << 12);
        if (hi > (1ULL << n)) hi = 1ULL << n;
        for (uint64_t aa = lo; aa < hi; aa++) {
            uint32_t a = (uint32_t)aa;
            if (!kanonisch(a)) continue;
            kl++;
            baue_matrix(a, A);
            int ueb;
            int u = bearbeite(A, R, n, a, &ueb, &zaehler);
            if (!strike) {
                if (ueb) {
                    ue++;
                    pthread_mutex_lock(&mx); fprintf(f_ueb, "%x %d\n", a, u); fflush(f_ueb); pthread_mutex_unlock(&mx);
                    continue;
                }
                if (f_alle) { pthread_mutex_lock(&mx); fprintf(f_alle, "%x %d\n", a, u); pthread_mutex_unlock(&mx); }
                hist[u]++;
                if (u > umax) { umax = u; ntop = 0; }
                if (u == umax && ntop < TOPMAX) { top[ntop].a = a; top[ntop].i = top[ntop].j = -1; ntop++; }
                continue;
            }
            /* gestrichene Spalten: nur Klassen mit u >= d - strike (bei Überlebendem der vollen Matrix immer) */
            if (f_alle && !ueb) { pthread_mutex_lock(&mx); fprintf(f_alle, "%x %d\n", a, u); pthread_mutex_unlock(&mx); }
            if (!ueb && u < d - strike) continue;
            bearb++;
            int m = n - strike;
            for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) {
                /* Spalten i und j streichen (strike == 2) */
                for (int r2 = 0; r2 < R; r2++) {
                    int c = 0;
                    for (int s = 0; s < n; s++) if (s != i && s != j) B[r2][c++] = A[r2][s];
                }
                paare++;
                int ueb2;
                int u2 = bearbeite(B, R, m, ((uint64_t)a << 10) ^ (uint64_t)(i * 32 + j), &ueb2, &zaehler);
                if (ueb2) {
                    ue++;
                    pthread_mutex_lock(&mx); fprintf(f_ueb, "%x %d %d %d\n", a, i, j, u2); fflush(f_ueb); pthread_mutex_unlock(&mx);
                    continue;
                }
                hist[u2]++;
                if (u2 > umax) { umax = u2; ntop = 0; }
                if (u2 == umax && ntop < TOPMAX) { top[ntop].a = a; top[ntop].i = i; top[ntop].j = j; ntop++; }
            }
        }
    }
    pthread_mutex_lock(&mx);
    g_klassen += kl; g_ueb += ue; g_filterschritte += zaehler; g_bearb += bearb; g_paare += paare;
    for (int i = 0; i < 2 * MAXN + 2; i++) g_hist[i] += hist[i];
    if (umax > g_umax) { g_umax = umax; g_ntop = 0; }
    if (umax == g_umax) for (int i = 0; i < ntop && g_ntop < TOPMAX; i++) g_top[g_ntop++] = top[i];
    pthread_mutex_unlock(&mx);
    return 0;
}

static int cmp_eintrag(const void *x, const void *y) {
    const eintrag_t *p = x, *q = y;
    if (p->a != q->a) return p->a < q->a ? -1 : 1;
    if (p->i != q->i) return p->i - q->i;
    return p->j - q->j;
}

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "Aufruf: gruppenmatrix GRP d [-r] [-S k] [-p THREADS] [-o PRAEFIX] [-b BUDGET] [-a] [-T k/m]\n"); return 2; }
    int alle = 0, teil = 1, teile = 1;
    const char *grp = argv[1];
    d = atoi(argv[2]);
    for (int i = 3; i < argc; i++) {
        if (!strcmp(argv[i], "-r")) rand_ = 1;
        else if (!strcmp(argv[i], "-S") && i + 1 < argc) strike = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-p") && i + 1 < argc) nthreads = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-o") && i + 1 < argc) praefix = argv[++i];
        else if (!strcmp(argv[i], "-b") && i + 1 < argc) budget = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-a")) alle = 1;
        else if (!strcmp(argv[i], "-T") && i + 1 < argc) { if (sscanf(argv[++i], "%d/%d", &teil, &teile) != 2) return 2; }
        else { fprintf(stderr, "unbekannt: %s\n", argv[i]); return 2; }
    }
    if (strike != 0 && strike != 2) { fprintf(stderr, "-S: nur 0 oder 2\n"); return 2; }
    if (strike && rand_) { fprintf(stderr, "-S und -r nicht zusammen\n"); return 2; }
    lies_gruppe(grp);
    maske = (uint32_t)((1ULL << n) - 1);
    bilde_symmetrien();
    char fn[512];
    snprintf(fn, sizeof fn, "%s_ueberlebende.txt", praefix);
    f_ueb = fopen(fn, "w");
    if (alle) { snprintf(fn, sizeof fn, "%s_alle.txt", praefix); f_alle = fopen(fn, "w"); }
    bloecke = ((1ULL << n) + 4095) >> 12;
    g_teil = teil - 1; g_teile = teile;
    struct timespec t0, t1; clock_gettime(CLOCK_MONOTONIC, &t0);
    pthread_t th[64];
    for (int i = 0; i < nthreads; i++) pthread_create(&th[i], 0, arbeiter, 0);
    for (int i = 0; i < nthreads; i++) pthread_join(th[i], 0);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    fclose(f_ueb);
    if (f_alle) fclose(f_alle);
    double sek = (t1.tv_sec - t0.tv_sec) + 1e-9 * (t1.tv_nsec - t0.tv_nsec);
    snprintf(fn, sizeof fn, "%s_top.txt", praefix);
    FILE *ft = fopen(fn, "w");
    qsort(g_top, g_ntop, sizeof g_top[0], cmp_eintrag);
    for (int i = 0; i < g_ntop; i++) {
        if (strike) fprintf(ft, "%x %d %d %d\n", g_top[i].a, g_top[i].i, g_top[i].j, g_umax);
        else fprintf(ft, "%x %d\n", g_top[i].a, g_umax);
    }
    fclose(ft);
    printf("gruppe %s n=%d |H|=%d (Automorphismen %d)%s%s d=%d: klassen %ld, ueberlebende %ld, beste obere schranke u_max %d (%d vertreter%s), zeit %.1f s, tabu-schritte %ld\n",
           grp, n, nH, naut, rand_ ? " mit rand" : "", strike ? " ohne 2 spalten" : "", d, g_klassen, g_ueb, g_umax, g_ntop,
           g_ntop >= TOPMAX ? ", gekappt" : "", sek, g_filterschritte);
    if (strike) printf("klassen mit u >= d-%d bearbeitet: %ld, spaltenpaare %ld\n", strike, g_bearb, g_paare);
    printf("verteilung u:");
    for (int i = 0; i < 2 * MAXN + 2; i++) if (g_hist[i]) printf(" %d:%ld", i, g_hist[i]);
    printf("\n");
    return 0;
}
