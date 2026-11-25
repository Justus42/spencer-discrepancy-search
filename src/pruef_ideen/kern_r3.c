/*
 * kern_r3.c -- Suchcode (Auftrag P, Teil 1). Kein Pruefcode.
 *
 * Gegeben: eine +-1-Matrix mit 16 Zeilen, die ersten 15 Spalten zaehlen (n = 15, Schwelle 5*sqrt(15/8) = 6,847).
 * Wir streichen r Zeilen, behalten 16-r, und ersetzen sie durch k beliebige Zeilen in [-1,1]^15.
 *
 * Notwendige Bedingung (Herleitung im Protokoll): Eine Ersatzzeile a mit a.x > T und a.y > T hat Hamming-Abstand(x,y) <= 8.
 * Lochklassen = Klassen {x,-x}, die keine behaltene Zeile mit Betrag >= 7 trifft (Abstand zur Zeile in 5..10).
 * Die Loecher muessen sich auf k Ersatzzeilen verteilen und mit Vorzeichen s versehen lassen, so dass je Zeile gilt:
 * Abstand der Vertreter <= 6 -> gleiches Vorzeichen, >= 9 -> verschiedenes, 7 und 8 frei.
 * Zertifikat = kleine Menge von Loechern, die sich schon allein nicht auf k Zeilen verteilen laesst.
 *
 * Wort = 15-Bit-Zahl, Bit j = 1 heisst Eintrag -1. Klassenvertreter: Bit 14 = 0 (Zahl < 2^14).
 *
 * Aufruf: kern_r3 MATRIX r k AUSGABE [shard nshard] [restarts] [seed] [sekunden]
 *   Alle C(16,r) Streichmengen (lexikographisch, Index i), nur die mit i % nshard == shard.
 *   Je Streichmenge eine Zeile: "i a b c .. | loecher | kern | h1 h2 ...", oder "UNENTSCHIEDEN".
 * Bauen: gcc -O2 -o build/kern_r3 src/pruef_ideen/kern_r3.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define NC 15
#define MAXH 4096
#define MAXK 3
#define MAXS 96

static int NR;                 /* Zeilenzahl der Matrix */
static uint16_t rowbits[64];
static int K;

static uint16_t hx[MAXH];
static int nh;
static unsigned char typ[MAXH][MAXH]; /* 0 frei, 1 gleiches Vorzeichen, 2 verschieden */

static uint64_t rng_s = 88172645463325252ULL;
static uint64_t rnd(void) { rng_s ^= rng_s << 13; rng_s ^= rng_s >> 7; rng_s ^= rng_s << 17; return rng_s; }

static int read_matrix(const char *fn) {
    FILE *f = fopen(fn, "r");
    if (!f) { perror(fn); exit(2); }
    char line[4096];
    NR = 0;
    while (fgets(line, sizeof line, f)) {
        int v, n, c = 0; char *p = line; uint16_t w = 0; int any = 0;
        while (sscanf(p, "%d%n", &v, &n) == 1) {
            p += n; any = 1;
            if (c < NC) { if (v == -1) w |= (uint16_t)(1u << c); else if (v != 1) { fprintf(stderr, "Eintrag %d\n", v); exit(2); } }
            c++;
        }
        if (!any) continue;
        if (c < NC) { fprintf(stderr, "zu wenige Spalten\n"); exit(2); }
        rowbits[NR++] = w;
    }
    fclose(f);
    return NR;
}

/* Loecher fuer behaltene Zeilen (Maske) bestimmen */
static void holes(const int *kept, int nk) {
    nh = 0;
    for (int x = 0; x < (1 << 14); x++) {
        int ok = 1;
        for (int i = 0; i < nk; i++) {
            int d = __builtin_popcount(x ^ rowbits[kept[i]]);
            if (d < 5 || d > 10) { ok = 0; break; }
        }
        if (ok) { if (nh >= MAXH) { fprintf(stderr, "zu viele Loecher\n"); exit(3); } hx[nh++] = (uint16_t)x; }
    }
}

static void build_typ(void) {
    for (int i = 0; i < nh; i++) {
        for (int j = 0; j < nh; j++) {
            int D = __builtin_popcount(hx[i] ^ hx[j]);
            typ[i][j] = (i == j) ? 0 : (D <= 6 ? 1 : (D >= 9 ? 2 : 0));
        }
    }
}

/* ---- Verteilbarkeit einer Teilmenge sub[0..n-1] auf K Teile mit Vorzeichen ---- */
/* Wert = p*2+s. Domain: Bitmaske ueber 2K Werte. */
static int sub[MAXS], ns;
static unsigned char sol[MAXS];

static int dfs(unsigned char *dom, signed char *val, int *partused) {
    int best = -1, bc = 99;
    for (int v = 0; v < ns; v++) if (val[v] < 0) {
        int c = __builtin_popcount(dom[v]);
        if (c == 0) return 0;
        if (c < bc) { bc = c; best = v; }
    }
    if (best < 0) return 1;
    int v = best;
    int firstfree = -1;
    for (int p = 0; p < K; p++) if (!partused[p]) { firstfree = p; break; }
    for (int a = 0; a < 2 * K; a++) if (dom[v] >> a & 1) {
        int p = a >> 1, s = a & 1;
        if (!partused[p] && (p != firstfree || s != 0)) continue; /* Symmetriebruch */
        unsigned char nd[MAXS]; memcpy(nd, dom, ns);
        signed char nv[MAXS]; memcpy(nv, val, ns);
        int np[MAXK]; memcpy(np, partused, sizeof(int) * K);
        nv[v] = (signed char)a; np[p] = 1;
        int ok = 1;
        for (int w = 0; w < ns && ok; w++) if (nv[w] < 0) {
            unsigned char t = typ[sub[v]][sub[w]];
            if (!t) continue;
            unsigned char d = nd[w];
            /* Werte (p, s2) in Teil p unvertraeglich */
            unsigned char bad = 0;
            if (t == 1) bad = (unsigned char)(1u << (2 * p + (1 - s)));
            else        bad = (unsigned char)(1u << (2 * p + s));
            d &= (unsigned char)~bad;
            nd[w] = d;
            if (!d) ok = 0;
        }
        if (!ok) continue;
        if (dfs(nd, nv, np)) { memcpy(val, nv, ns); return 1; }
    }
    return 0;
}

/* sub[] gesetzt; liefert 1 wenn verteilbar, Loesung in sol */
static int verteilbar(void) {
    unsigned char dom[MAXS]; signed char val[MAXS]; int pu[MAXK] = {0};
    unsigned char full = (unsigned char)((1u << (2 * K)) - 1);
    for (int v = 0; v < ns; v++) { dom[v] = full; val[v] = -1; }
    if (ns == 0) return 1;
    int r = dfs(dom, val, pu);
    if (r) for (int v = 0; v < ns; v++) sol[v] = (unsigned char)val[v];
    return r;
}


/* Wachsen lassen bis unverteilbar; dann ausduennen. Rueckgabe Kerngroesse, Kern in core[] (Lochindizes), -1 wenn alles verteilbar */
static int finde_kern(int *order, int *core) {
    ns = 0;
    int stop = -1;
    for (int t = 0; t < nh && ns < MAXS - 1; t++) {
        sub[ns++] = order[t];
        /* schnelle Erweiterung der alten Loesung */
        int hv = sub[ns - 1], placed = 0;
        for (int a = 0; a < 2 * K && !placed; a++) {
            int p = a >> 1, s = a & 1, ok = 1;
            for (int v = 0; v < ns - 1 && ok; v++) {
                if ((sol[v] >> 1) != p) continue;
                unsigned char tt = typ[sub[v]][hv];
                if (tt == 1 && (sol[v] & 1) != s) ok = 0;
                if (tt == 2 && (sol[v] & 1) == s) ok = 0;
            }
            if (ok) { sol[ns - 1] = (unsigned char)a; placed = 1; }
        }
        if (placed) continue;
        if (!verteilbar()) { stop = t; break; }
    }
    if (stop < 0) return -1;
    int n = ns;
    int cur[MAXS]; memcpy(cur, sub, sizeof(int) * n);
    /* Ausduennen: rueckwaerts, dann nochmals vorwaerts */
    for (int pass = 0; pass < 2; pass++) {
        for (int i = n - 1; i >= 0; i--) {
            if (n <= 1) break;
            int tmp[MAXS], m = 0;
            for (int j = 0; j < n; j++) if (j != i) tmp[m++] = cur[j];
            memcpy(sub, tmp, sizeof(int) * m); ns = m;
            if (!verteilbar()) { memcpy(cur, tmp, sizeof(int) * m); n = m; }
        }
    }
    memcpy(core, cur, sizeof(int) * n);
    return n;
}

int main(int argc, char **argv) {
    if (argc < 5) { fprintf(stderr, "Aufruf: %s MATRIX r k AUSGABE [shard nshard] [restarts] [seed] [sekunden]\n", argv[0]); return 2; }
    int r = atoi(argv[2]); K = atoi(argv[3]);
    const char *out = argv[4];
    int shard = argc > 5 ? atoi(argv[5]) : 0, nshard = argc > 6 ? atoi(argv[6]) : 1;
    int restarts = argc > 7 ? atoi(argv[7]) : 30;
    uint64_t seed = argc > 8 ? strtoull(argv[8], 0, 10) : 1;
    double limit = argc > 9 ? atof(argv[9]) : 1e18;
    rng_s ^= seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 10; i++) rnd();
    read_matrix(argv[1]);
    if (K < 1 || K > MAXK || r < 0 || r > NR) { fprintf(stderr, "r/k\n"); return 2; }
    FILE *fo = fopen(out, "a");
    if (!fo) { perror(out); return 2; }
    time_t t0 = time(0);
    int idx[16]; for (int i = 0; i < r; i++) idx[i] = i;
    long long cnt = 0;
    while (1) {
        if (cnt % nshard == shard && (!getenv("NUR_INDEX") || atoll(getenv("NUR_INDEX")) == cnt)) {
            if (difftime(time(0), t0) > limit) { fprintf(fo, "# Zeitgrenze erreicht bei Index %lld\n", cnt); break; }
            int kept[64], nk = 0;
            for (int i = 0; i < NR; i++) { int d = 0; for (int j = 0; j < r; j++) if (idx[j] == i) d = 1; if (!d) kept[nk++] = i; }
            holes(kept, nk);
            build_typ();
            int order[MAXH], best = 999, bestcore[MAXS], core[MAXS];
            for (int rs = 0; rs < restarts; rs++) {
                for (int i = 0; i < nh; i++) order[i] = i;
                for (int i = nh - 1; i > 0; i--) { int j = (int)(rnd() % (uint64_t)(i + 1)); int t = order[i]; order[i] = order[j]; order[j] = t; }
                int c = finde_kern(order, core);
                if (c < 0) continue; /* Teilmenge bis MAXS-1 Loecher noch verteilbar: nichts entschieden, naechster Versuch */
                if (c < best) { best = c; memcpy(bestcore, core, sizeof(int) * c); }
            }
            fprintf(fo, "%lld", cnt);
            for (int j = 0; j < r; j++) fprintf(fo, " %d", idx[j]);
            fprintf(fo, " | %d | ", nh);
            if (best == 999) fprintf(fo, "0 | VERTEILBAR"); /* in allen Versuchen kein Kern gefunden */
            else { fprintf(fo, "%d |", best); for (int i = 0; i < best; i++) fprintf(fo, " %d", hx[bestcore[i]]); }
            fprintf(fo, "\n"); fflush(fo);
        }
        cnt++;
        /* naechste Kombination */
        int i = r - 1;
        while (i >= 0 && idx[i] == NR - r + i) i--;
        if (i < 0) break;
        idx[i]++;
        for (int j = i + 1; j < r; j++) idx[j] = idx[j - 1] + 1;
    }
    fclose(fo);
    return 0;
}
