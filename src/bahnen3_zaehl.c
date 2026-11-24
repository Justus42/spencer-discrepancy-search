/* bahnen3_zaehl.c -- Pruefcode (Teil 1-3, 25.11.2026). Kein Suchcode.
 *
 * Gruppe P (9 vorzeichenbehaftete Spaltenpermutationen aus Datei, Zeile: pi(0..n-1) und n Minus-Bits),
 * Wort x in {0,1}^n (Bit j = Spalte j, Bit 1 = Eintrag -1), Bild: Bit pi(j) von g.x = Bit j von x XOR Minus_j.
 * Zeilenpaar {x, ~x}, Vertreter mit Bit n-1 = 0. Bahnen von P auf den 2^(n-1) Zeilenpaaren, Groesse 1, 3, 9.
 * Nummerierung: erst Bahnen der Groesse 1, dann 3, dann 9 (je aufsteigend nach kleinstem Vertreter).
 * Ueberdeckungsmatrix M[p][o] = 1 <=> ein Wort der Bahn o (oder sein Komplement) liegt im Abstand <= R von
 * einem Wort der Punktklasse p (symmetrisch, wird geprueft).
 *
 * Zaehlung (wie bahnen_zaehl.c): Verzweigen am ersten unueberdeckten Punkt in statischer Ordnung (wenigste
 * Ueberdecker zuerst); Zweig i: "o_i drin, o_1..o_{i-1} nicht drin", jede Menge genau einmal. Gewicht einer
 * Bahn = Zahl ihrer Zeilen (1, 3, 9). Gezaehlt werden ALLE Bahnmengen mit Gewicht <= B und hoechstens max9
 * Neunerbahnen, die alles ueberdecken (auch nicht minimale), getrennt nach (Zahl der Neunerbahnen, Gewicht).
 * Sobald keine Neunerbahn mehr zulaessig ist, rechnet Phase B nur mit den 168 kleinen Bahnen auf den
 * verschiedenen Masken der offenen Punkte, mit Schranke aus paarweise fremden Masken.
 *
 * Aufruf:
 *   bahnen3_zaehl P.txt matrix.txt info
 *   bahnen3_zaehl P.txt matrix.txt zaehle B max9 [von bis [liste.txt]]   (von/bis: Bereich der ersten Verzweigung)
 *   bahnen3_zaehl P.txt matrix.txt pruefe w1 w2 ...                      (Woerter; ihre Bahnen ueberdecken?)
 *   bahnen3_zaehl P.txt matrix.txt selbsttest seed k b2   Kontrolle von phaseB und paare() auf k Zufallspunkten
 *   bahnen3_zaehl P.txt matrix.txt neun1 B [von bis [liste]]  genau eine Neunerbahn: je Neunerbahn o (Index von..bis-1)
 *       Phase B (kleine Bahnen, Budget B-9) auf den von o offen gelassenen Punkten.
 *   bahnen3_zaehl P.txt matrix.txt neun2 B [von bis [liste]]  genau zwei Neunerbahnen: alle kleinen Mengen S mit Gewicht
 *       <= B-18 (Index von..bis-1); p = erster offener Punkt; o1 laeuft ueber die Neunerbahnen, die p ueberdecken;
 *       o2 muss den Rest R ueberdecken (Kandidaten durch UND der Zeilen von 64 Probepunkten, dann volle Pruefung),
 *       und o2 ist keine p-Ueberdeckerin mit Id <= o1 (jedes Paar genau einmal).
 * Bauen: gcc -O3 -march=native -fopenmp -o build/bahnen3_zaehl src/bahnen3_zaehl.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define N 20
#define RAD 6
#define MAXT 64
typedef unsigned __int128 u128;

static int pi_[9][N], mi[9][N];
static int NT, W, N1, N3, N9, NS;           /* NS = N1 + N3 kleine Bahnen (Ids 0..NS-1) */
static int *bid; static uint32_t *rep; static int *gew;
static uint64_t *M;
static int *ordn;                             /* statische Ordnung der Punkte */
static int *kls, NK; static uint64_t (*klsmask)[3];  /* Klassen gleicher kleiner Masken */
static uint64_t SM1[3], SM3[3];               /* Masken der Einser-/Dreierbahnen */
static FILE *liste;
static double t0;

static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }
static uint32_t kan(uint32_t x) { return (x >> (N - 1)) & 1 ? x ^ ((1u << N) - 1) : x; }
static uint32_t wirk(int g, uint32_t x) {
    uint32_t y = 0;
    for (int j = 0; j < N; j++) y |= (((x >> j) & 1) ^ mi[g][j]) << pi_[g][j];
    return y;
}
static inline int tst(const uint64_t *b, int i) { return (b[i >> 6] >> (i & 63)) & 1; }
static inline void clr(uint64_t *b, int i) { b[i >> 6] &= ~(1ULL << (i & 63)); }
static inline void setb(uint64_t *b, int i) { b[i >> 6] |= 1ULL << (i & 63); }
static u128 binom(long a, int k) { if (k < 0 || a < k) return 0; u128 r = 1; for (int i = 0; i < k; i++) r = r * (u128)(a - i) / (u128)(i + 1); return r; }
static void druck128(u128 v, char *s) { char t[64]; int i = 0; if (!v) { strcpy(s, "0"); return; } while (v) { t[i++] = '0' + (int)(v % 10); v /= 10; } for (int j = 0; j < i; j++) s[j] = t[i - 1 - j]; s[i] = 0; }

static int B, MAX9;
typedef struct { u128 cnt[3][64]; int wahl[MAXT]; int tiefe; uint64_t *Ubuf, *Abuf; long knoten; } Faden;

/* Blatt: alle Teilmengen der verfuegbaren Bahnen mit Restbudget zaehlen */
static void blatt(Faden *f, int n9, int w, int b, int n9frei, long a1, long a3, long a9) {
    for (int k = 0; k <= n9frei && 9 * k <= b; k++)
        for (int j = 0; 3 * j + 9 * k <= b; j++)
            for (int i = 0; i + 3 * j + 9 * k <= b; i++) {
                u128 c = binom(a1, i) * binom(a3, j) * binom(a9, k);
                if (c) f->cnt[n9 + k][w + i + 3 * j + 9 * k] += c;
            }
    if (liste && f->tiefe >= 0) {
        #pragma omp critical(liste)
        {
            fprintf(liste, "%d %d %s:", n9, w, (a1 || a3 || a9) && b > 0 ? "(+Obermengen)" : "");
            for (int i = 0; i < f->tiefe; i++) fprintf(liste, " %u/%d", rep[f->wahl[i]], gew[f->wahl[i]]);
            fprintf(liste, "\n");
        }
    }
}

static inline int pc3(const uint64_t *m) { return __builtin_popcountll(m[0]) + __builtin_popcountll(m[1]) + __builtin_popcountll(m[2]); }

#define LPROBE 48  /* 96 und 192 getestet: kaum weniger Knoten, langsamer */
/* Phase B: nur kleine Bahnen, offene Punkte als Bitset U. Verzweigung am Punkt mit den wenigsten
 * verfuegbaren Kandidaten unter den ersten LPROBE offenen Punkten (statische Ordnung ordnB: wenigste kleine
 * Ueberdecker zuerst); Schranke: gierig paarweise fremde Kandidatenmasken dieser Punkte, je Mindestgewicht. */
static int *ordnB; static uint64_t (*sm)[3];
static void phaseB(Faden *f, int n9, int w, int b, const uint64_t *U, const uint64_t *avail, int d) {
    f->knoten++;
    uint64_t A[3] = { avail[0], avail[1], avail[2] };
    A[2] &= (NS & 63) ? ((1ULL << (NS & 63)) - 1) : ~0ULL;
    if (b < 3) { A[0] &= ~SM3[0]; A[1] &= ~SM3[1]; A[2] &= ~SM3[2]; }
    uint64_t pm[LPROBE][3]; int np = 0;
    for (int i = 0; i < NT && np < LPROBE; i++) {
        int q = ordnB[i];
        if (!tst(U, q)) continue;
        uint64_t m[3] = { sm[q][0] & A[0], sm[q][1] & A[1], sm[q][2] & A[2] };
        if (!(m[0] | m[1] | m[2])) return;
        memcpy(pm[np++], m, sizeof m);
    }
    if (np == 0) {
        uint64_t x1[3], x3[3];
        for (int v = 0; v < 3; v++) { x1[v] = A[v] & SM1[v]; x3[v] = A[v] & SM3[v]; }
        blatt(f, n9, w, b, 0, pc3(x1), pc3(x3), 0);
        return;
    }
    if (b <= 0) return;
    uint64_t used[3] = { 0, 0, 0 }; int lb = 0, best = 0, bestc = 1 << 30;
    for (int i = 0; i < np; i++) {
        const uint64_t *m = pm[i];
        int c = pc3(m);
        if (c < bestc) { bestc = c; best = i; }
        if (!((m[0] & used[0]) | (m[1] & used[1]) | (m[2] & used[2]))) {
            lb += ((m[0] & SM1[0]) | (m[1] & SM1[1]) | (m[2] & SM1[2])) ? 1 : 3;
            used[0] |= m[0]; used[1] |= m[1]; used[2] |= m[2];
            if (lb > b) return;
        }
    }
    uint64_t A2[3] = { A[0], A[1], A[2] };
    uint64_t *U2 = f->Ubuf + (size_t)(d + 1) * W;
    for (int v = 0; v < 3; v++) {
        uint64_t x = pm[best][v];
        while (x) {
            int o = v * 64 + __builtin_ctzll(x); x &= x - 1;
            clr(A2, o);
            if (gew[o] > b) continue;
            const uint64_t *ro = M + (size_t)o * W;
            for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
            f->wahl[f->tiefe++] = o;
            phaseB(f, n9, w + gew[o], b - gew[o], U2, A2, d + 1);
            f->tiefe--;
        }
    }
}

/* Phase A: alle Bahnen, Neunerbahnen noch zulaessig */
static void phaseA(Faden *f, int n9, int w, int b, const uint64_t *U, const uint64_t *avail, int d) {
    f->knoten++;
    if (n9 >= MAX9 || b < 9) { phaseB(f, n9, w, b, U, avail, d); return; }
    int p = -1;
    for (int i = 0; i < NT; i++) { int q = ordn[i]; if (tst(U, q)) { p = q; break; } }
    if (p < 0) {
        long a1 = 0, a3 = 0, a9 = 0;
        for (int o = 0; o < NT; o++) if (tst(avail, o)) { if (gew[o] == 1) a1++; else if (gew[o] == 3) a3++; else a9++; }
        blatt(f, n9, w, b, MAX9 - n9, a1, a3, a9);
        return;
    }
    uint64_t *A2 = f->Abuf + (size_t)(d + 1) * W, *U2 = f->Ubuf + (size_t)(d + 1) * W;
    memcpy(A2, avail, sizeof(uint64_t) * W);
    const uint64_t *r = M + (size_t)p * W;
    for (int v = 0; v < W; v++) {
        uint64_t x = r[v] & avail[v];
        while (x) {
            int o = v * 64 + __builtin_ctzll(x); x &= x - 1;
            clr(A2, o);
            if (gew[o] > b) continue;
            const uint64_t *ro = M + (size_t)o * W;
            for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
            f->wahl[f->tiefe++] = o;
            phaseA(f, n9 + (gew[o] == 9), w + gew[o], b - gew[o], U2, A2, d + 1);
            f->tiefe--;
        }
    }
}

/* Zahl der ungeordneten Paare {o1, o2} von Neunerbahnen, die alle Punkte in U ueberdecken (jedes Paar genau einmal):
 * p = erster Punkt von U in statischer Ordnung; o1 laeuft ueber die p-Ueberdecker; o2 deckt R = U \ cov(o1)
 * und ist keine p-Ueberdeckerin mit Id <= o1. Fuer Liste: kleine Menge S (sl, sn) und Gewicht wz. */
static long paare(Faden *f, const uint64_t *U, const int *sl, int sn, int wz) {
    int p = -1; for (int k = 0; k < NT; k++) if (tst(U, ordn[k])) { p = ordn[k]; break; }
    long tot = 0;
    if (p < 0) return (long)N9 * (N9 - 1) / 2;
    uint64_t *R = f->Ubuf + (size_t)(MAXT + 1) * W;
    const uint64_t *rp = M + (size_t)p * W;
    long vorher = 0;
    int probe[64];
    int W9a = NS >> 6;
    for (int o1 = NS; o1 < NT; o1++) {
        if (!tst(rp, o1)) continue;
        vorher++;
        const uint64_t *r1 = M + (size_t)o1 * W;
        int leer = 1;
        for (int u = 0; u < W; u++) { R[u] = U[u] & ~r1[u]; if (R[u]) leer = 0; }
        f->knoten++;
        if (leer) { tot += N9 - vorher; continue; }
        int np = 0;
        for (int kk = 0; kk < NT && np < 64; kk++) if (tst(R, ordn[kk])) probe[np++] = ordn[kk];
        for (int v = W9a; v < W; v++) {
            uint64_t x = ~0ULL;
            if (v == W9a && (NS & 63)) x &= ~((1ULL << (NS & 63)) - 1);
            if (v == W - 1 && (NT & 63)) x &= (1ULL << (NT & 63)) - 1;
            for (int j = 0; j < np && x; j++) x &= M[(size_t)probe[j] * W + v];
            while (x) {
                int o2 = v * 64 + __builtin_ctzll(x); x &= x - 1;
                if (tst(rp, o2) && o2 <= o1) continue;
                const uint64_t *r2 = M + (size_t)o2 * W; int ok = 1;
                for (int u = 0; u < W; u++) if (R[u] & ~r2[u]) { ok = 0; break; }
                if (!ok) continue;
                tot++;
                if (liste) {
                    #pragma omp critical(liste)
                    { fprintf(liste, "2 %d:", wz); for (int k = 0; k < sn; k++) fprintf(liste, " %u/%d", rep[sl[k]], gew[sl[k]]);
                      fprintf(liste, " %u/9 %u/9\n", rep[o1], rep[o2]); }
                }
            }
        }
    }
    return tot;
}

static Faden *neuer_faden(void) {
    Faden *f = calloc(1, sizeof(Faden));
    f->Ubuf = malloc(sizeof(uint64_t) * W * (MAXT + 2));  /* Ebene MAXT+1: Rest R in paare() */
    f->Abuf = malloc(sizeof(uint64_t) * W * (MAXT + 2));
    return f;
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "Aufruf: P.txt matrix.txt info|zaehle|pruefe ...\n"); return 1; }
    t0 = jetzt();
    FILE *fp = fopen(argv[1], "r");
    for (int g = 0; g < 9; g++) {
        for (int j = 0; j < N; j++) if (fscanf(fp, "%d", &pi_[g][j]) != 1) { fprintf(stderr, "P-Datei\n"); return 1; }
        for (int j = 0; j < N; j++) if (fscanf(fp, "%d", &mi[g][j]) != 1) { fprintf(stderr, "P-Datei\n"); return 1; }
    }
    fclose(fp);
    uint32_t NP = 1u << (N - 1);
    /* Bahnen */
    uint32_t *mn = malloc(sizeof(uint32_t) * NP); uint8_t *gr = malloc(NP);
    for (uint32_t x = 0; x < NP; x++) {
        uint32_t b[9]; int k = 0; uint32_t m = x;
        for (int g = 0; g < 9; g++) {
            uint32_t y = kan(wirk(g, x)); int neu = 1;
            for (int i = 0; i < k; i++) if (b[i] == y) neu = 0;
            if (neu) b[k++] = y;
            if (y < m) m = y;
        }
        mn[x] = m; gr[x] = k;
    }
    long z[10] = { 0 };
    for (uint32_t x = 0; x < NP; x++) if (mn[x] == x) z[gr[x]]++;
    N1 = z[1]; N3 = z[3]; N9 = z[9]; NT = N1 + N3 + N9; NS = N1 + N3;
    printf("Bahnen von P auf %u Zeilenpaaren: Groesse 1: %ld, Groesse 3: %ld, Groesse 9: %ld, andere: %ld; Punktklassen %d\n",
           NP, z[1], z[3], z[9], NP - z[1] - 3 * z[3] - 9 * z[9], NT);
    if (NS > 192) { fprintf(stderr, "zu viele kleine Bahnen\n"); return 1; }
    W = (NT + 63) / 64;
    bid = malloc(sizeof(int) * NP); rep = malloc(sizeof(uint32_t) * NT); gew = malloc(sizeof(int) * NT);
    int *idv = malloc(sizeof(int) * NP); int c1 = 0, c3 = N1, c9 = N1 + N3;
    for (uint32_t x = 0; x < NP; x++) if (mn[x] == x) {
        int id = gr[x] == 1 ? c1++ : gr[x] == 3 ? c3++ : c9++;
        idv[x] = id; rep[id] = x; gew[id] = gr[x];
    }
    for (uint32_t x = 0; x < NP; x++) bid[x] = idv[mn[x]];
    for (int o = 0; o < NS; o++) setb(gew[o] == 1 ? SM1 : SM3, o);
    /* Ueberdeckungsmatrix */
    int nm = 0; uint32_t *mask = malloc(sizeof(uint32_t) * (1u << N));
    for (uint32_t x = 0; x < (1u << N); x++) if (__builtin_popcount(x) <= RAD) mask[nm++] = x;
    M = calloc((size_t)NT * W, sizeof(uint64_t));
    #pragma omp parallel for schedule(dynamic, 64)
    for (int p = 0; p < NT; p++) {
        uint64_t *r = M + (size_t)p * W;
        for (int i = 0; i < nm; i++) { int o = bid[kan(rep[p] ^ mask[i])]; r[o >> 6] |= 1ULL << (o & 63); }
    }
    long asym = 0;
    #pragma omp parallel for reduction(+:asym) schedule(dynamic, 64)
    for (int p = 0; p < NT; p++) for (int o = 0; o < NT; o++) if (tst(M + (size_t)p * W, o) != tst(M + (size_t)o * W, p)) asym++;
    printf("Kugel %d Woerter; Ueberdeckungsmatrix %d x %d, asymmetrische Eintraege: %ld (%.1f s)\n", nm, NT, NT, asym, jetzt() - t0);
    /* Statistik je Punkttyp */
    int *gesamt = malloc(sizeof(int) * NT);
    for (int t = 1; t <= 9; t *= 3) {
        int mn9 = 1 << 30, mx9 = 0, mns = 1 << 30, mxs = 0, cnt = 0;
        for (int p = 0; p < NT; p++) {
            const uint64_t *r = M + (size_t)p * W; int s = 0, a = 0;
            for (int v = 0; v < W; v++) a += __builtin_popcountll(r[v]);
            for (int o = 0; o < NS; o++) s += tst(r, o);
            gesamt[p] = a;
            if (gew[p] != t) continue;
            cnt++;
            if (a - s < mn9) mn9 = a - s; if (a - s > mx9) mx9 = a - s; if (s < mns) mns = s; if (s > mxs) mxs = s;
        }
        printf("Punktklassen der Groesse %d (%d): Neunerbahnen als Ueberdecker %d..%d, kleine Bahnen %d..%d\n", t, cnt, mn9, mx9, mns, mxs);
    }
    long nichtklein = 0;
    for (int p = 0; p < NT; p++) { const uint64_t *r = M + (size_t)p * W; int s = 0; for (int o = 0; o < NS; o++) s += tst(r, o); if (!s) nichtklein++; }
    printf("Punktklassen ohne kleinen Ueberdecker: %ld\n", nichtklein);
    ordn = malloc(sizeof(int) * NT);
    for (int i = 0; i < NT; i++) ordn[i] = i;
    { /* stabil nach gesamt sortieren */
        int *tmp = malloc(sizeof(int) * NT);
        for (int wdt = 1; wdt < NT; wdt *= 2) {
            for (int lo = 0; lo < NT; lo += 2 * wdt) {
                int mid = lo + wdt < NT ? lo + wdt : NT, hi = lo + 2 * wdt < NT ? lo + 2 * wdt : NT, a = lo, b2 = mid, k = lo;
                while (a < mid && b2 < hi) tmp[k++] = gesamt[ordn[a]] <= gesamt[ordn[b2]] ? ordn[a++] : ordn[b2++];
                while (a < mid) tmp[k++] = ordn[a++];
                while (b2 < hi) tmp[k++] = ordn[b2++];
            }
            memcpy(ordn, tmp, sizeof(int) * NT);
        }
        free(tmp);
    }
    /* Klassen gleicher kleiner Masken */
    kls = malloc(sizeof(int) * NT); klsmask = malloc(sizeof(uint64_t[3]) * NT); NK = 0;
    {
        /* einfache Hash-Tabelle */
        int HS = 1 << 18; int *ht = malloc(sizeof(int) * HS); for (int i = 0; i < HS; i++) ht[i] = -1;
        for (int p = 0; p < NT; p++) {
            const uint64_t *r = M + (size_t)p * W;
            uint64_t m[3] = { r[0], r[1], r[2] & ((NS & 63) ? ((1ULL << (NS & 63)) - 1) : ~0ULL) };
            uint64_t h = (m[0] * 0x9E3779B97F4A7C15ULL) ^ (m[1] * 0xC2B2AE3D27D4EB4FULL) ^ (m[2] * 0x165667B19E3779F9ULL);
            int s = (int)((h >> 20) & (HS - 1));
            while (ht[s] >= 0 && memcmp(klsmask[ht[s]], m, sizeof m)) s = (s + 1) & (HS - 1);
            if (ht[s] < 0) { ht[s] = NK; memcpy(klsmask[NK], m, sizeof m); NK++; }
            kls[p] = ht[s];
        }
        free(ht);
    }
    printf("verschiedene kleine Masken: %d\n", NK);
    sm = malloc(sizeof(uint64_t[3]) * NT);
    int *kz = malloc(sizeof(int) * NT);
    for (int p = 0; p < NT; p++) { memcpy(sm[p], klsmask[kls[p]], sizeof(uint64_t[3])); kz[p] = pc3(sm[p]); }
    ordnB = malloc(sizeof(int) * NT);
    { int z2[200] = { 0 }; for (int p = 0; p < NT; p++) z2[kz[p] + 1]++; for (int k = 1; k < 200; k++) z2[k] += z2[k - 1];
      for (int p = 0; p < NT; p++) ordnB[z2[kz[p]]++] = p; }

    /* 48er pruefen */
    {
        FILE *fm = fopen(argv[2], "r"); int a[N]; int nz = 0; int b48[64]; int nb = 0;
        uint64_t *U = malloc(sizeof(uint64_t) * W); for (int v = 0; v < W; v++) U[v] = 0; for (int p = 0; p < NT; p++) setb(U, p);
        while (1) {
            int ok = 1; for (int j = 0; j < N; j++) if (fscanf(fm, "%d", &a[j]) != 1) { ok = 0; break; }
            if (!ok) break;
            uint32_t x = 0; for (int j = 0; j < N; j++) if (a[j] < 0) x |= 1u << j;
            int o = bid[kan(x)]; nz++;
            int neu = 1; for (int i = 0; i < nb; i++) if (b48[i] == o) neu = 0;
            if (neu) b48[nb++] = o;
        }
        fclose(fm);
        int rz = 0; printf("Matrix %s: %d Zeilen in %d Bahnen, Groessen:", argv[2], nz, nb);
        for (int i = 0; i < nb; i++) { printf(" %d", gew[b48[i]]); rz += gew[b48[i]]; }
        for (int i = 0; i < nb; i++) { const uint64_t *r = M + (size_t)b48[i] * W; for (int v = 0; v < W; v++) U[v] &= ~r[v]; }
        int off = 0; for (int v = 0; v < W; v++) off += __builtin_popcountll(U[v]);
        printf("; Zeilen der Bahnen zusammen %d; offene Punktklassen danach: %d -> %s\n", rz, off, off ? "KEINE Ueberdeckung" : "UEBERDECKUNG");
        printf("48er-Bahnen (Vertreter/Gewicht):"); for (int i = 0; i < nb; i++) printf(" %u/%d", rep[b48[i]], gew[b48[i]]); printf("\n");
        free(U);
    }
    fflush(stdout);
    if (!strcmp(argv[3], "info")) return 0;

    if (!strcmp(argv[3], "pruefe")) {
        uint64_t *U = malloc(sizeof(uint64_t) * W); for (int v = 0; v < W; v++) U[v] = 0; for (int p = 0; p < NT; p++) setb(U, p);
        int ids[256], ni = 0, rz = 0;
        for (int i = 4; i < argc; i++) {
            uint32_t x = (uint32_t)strtoul(argv[i], 0, 10); int o = bid[kan(x)];
            int neu = 1; for (int k = 0; k < ni; k++) if (ids[k] == o) neu = 0;
            if (neu) { ids[ni++] = o; rz += gew[o]; const uint64_t *r = M + (size_t)o * W; for (int v = 0; v < W; v++) U[v] &= ~r[v]; }
        }
        int off = 0; for (int v = 0; v < W; v++) off += __builtin_popcountll(U[v]);
        printf("pruefe: %d Bahnen, %d Zeilen, offene Punktklassen %d -> %s\n", ni, rz, off, off ? "KEINE Ueberdeckung" : "UEBERDECKUNG");
        return 0;
    }

    if (!strcmp(argv[3], "selbsttest")) {
        /* Zufaellige Punktmenge Q (k <= 64 Punkte): (a) phaseB (kleine Bahnen, Budget b2) gegen Durchprobieren aller
         * kleinen Mengen mit Gewicht <= b2; (b) paare() gegen alle ungeordneten Paare von Neunerbahnen. */
        unsigned long long sd = strtoull(argv[4], 0, 10) * 0x9E3779B97F4A7C15ULL + 12345; int k = atoi(argv[5]), b2 = atoi(argv[6]);
        uint64_t *U = calloc(W, sizeof(uint64_t)); int Q[64], nq = 0;
        while (nq < k) { sd ^= sd << 13; sd ^= sd >> 7; sd ^= sd << 17; int q = (int)(sd % NT); if (!tst(U, q)) { setb(U, q); Q[nq++] = q; } }
        Faden *f = neuer_faden();
        uint64_t AS[3] = { 0, 0, 0 }; for (int o = 0; o < NS; o++) setb(AS, o);
        memcpy(f->Ubuf, U, sizeof(uint64_t) * W);
        f->tiefe = 0; phaseB(f, 0, 0, b2, f->Ubuf, AS, 0);
        u128 brute[64] = { 0 };
        int st[64], sp = 0;
        void rek(int ab, int w) {
            /* deckt st[0..sp) alle Q? */
            int ok = 1;
            for (int j = 0; j < nq && ok; j++) { int c = 0; for (int t = 0; t < sp; t++) if (tst(M + (size_t)Q[j] * W, st[t])) { c = 1; break; } if (!c) ok = 0; }
            if (ok) brute[w]++;
            for (int o = ab; o < NS; o++) if (w + gew[o] <= b2) { st[sp++] = o; rek(o + 1, w + gew[o]); sp--; }
        }
        rek(0, 0);
        int gleich = 1;
        for (int w = 0; w <= b2; w++) {
            char x1[64], x2[64]; druck128(f->cnt[0][w], x1); druck128(brute[w], x2);
            printf("  (a) Gewicht %d: phaseB %s, Durchprobieren %s\n", w, x1, x2);
            if (f->cnt[0][w] != brute[w]) gleich = 0;
        }
        long pz = paare(f, U, NULL, 0, 0);
        uint64_t *mk = malloc(sizeof(uint64_t) * N9);
        for (int o = 0; o < N9; o++) { uint64_t m = 0; for (int j = 0; j < nq; j++) if (tst(M + (size_t)Q[j] * W, NS + o)) m |= 1ULL << j; mk[o] = m; }
        uint64_t voll = nq == 64 ? ~0ULL : (1ULL << nq) - 1; long bp = 0;
        #pragma omp parallel for reduction(+:bp) schedule(dynamic, 64)
        for (int a = 0; a < N9; a++) for (int c = a + 1; c < N9; c++) if ((mk[a] | mk[c]) == voll) bp++;
        printf("  (b) Paare von Neunerbahnen: paare() %ld, alle Paare durchprobiert %ld\n", pz, bp);
        printf("SELBSTTEST seed=%s k=%d b2=%d: %s\n", argv[4], k, b2, (gleich && pz == bp) ? "GLEICH" : "ABWEICHUNG");
        return (gleich && pz == bp) ? 0 : 1;
    }

    if (!strcmp(argv[3], "neun1") || !strcmp(argv[3], "neun2")) {
        int zwei = !strcmp(argv[3], "neun2");
        B = atoi(argv[4]);
        int nth = omp_get_max_threads();
        Faden **F = malloc(sizeof(Faden *) * nth);
        for (int i = 0; i < nth; i++) F[i] = neuer_faden();
        uint64_t *ALL = calloc(W, sizeof(uint64_t)); for (int p = 0; p < NT; p++) setb(ALL, p);
        uint64_t AS[3] = { 0, 0, 0 }; for (int o = 0; o < NS; o++) setb(AS, o);
        /* neun2: alle kleinen Mengen S mit Gewicht <= B-18 aufzaehlen */
        int nS = 0, (*Sl)[8] = NULL, *Sn = NULL, *Sw = NULL;
        int ntot = N9;
        if (zwei) {
            int b2 = B - 18; if (b2 < 0) b2 = -1;
            int cap = 1 << 20; Sl = malloc(sizeof(int[8]) * cap); Sn = malloc(sizeof(int) * cap); Sw = malloc(sizeof(int) * cap);
            int st[8], sp = 0;
            /* iterativ: Mengen aufsteigender Ids, Gewicht <= b2 */
            void rek(int ab, int w) {
                if (nS >= cap) { fprintf(stderr, "zu viele kleine Mengen\n"); exit(1); }
                memcpy(Sl[nS], st, sizeof st); Sn[nS] = sp; Sw[nS] = w; nS++;
                for (int o = ab; o < NS; o++) if (w + gew[o] <= b2 && sp < 8) { st[sp++] = o; rek(o + 1, w + gew[o]); sp--; }
            }
            if (b2 >= 0) rek(0, 0);
            ntot = nS;
            printf("neun2 B=%d: %d kleine Mengen mit Gewicht <= %d\n", B, nS, b2);
        }
        int von = 0, bis = ntot;
        if (argc > 6) { von = atoi(argv[5]); bis = atoi(argv[6]); if (bis > ntot) bis = ntot; }
        if (argc > 7) liste = fopen(argv[7], "w");
        printf("%s B=%d Bereich [%d, %d) von %d\n", argv[3], B, von, bis, ntot); fflush(stdout);
        long fertig = 0; double tlog = jetzt();
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = von; i < bis; i++) {
            Faden *f = F[omp_get_thread_num()];
            uint64_t *U2 = f->Ubuf;
            if (!zwei) {
                int o = NS + i;
                if (B >= 9) {
                    const uint64_t *ro = M + (size_t)o * W;
                    for (int u = 0; u < W; u++) U2[u] = ALL[u] & ~ro[u];
                    f->tiefe = 0; f->wahl[f->tiefe++] = o;
                    phaseB(f, 1, 9, B - 9, U2, AS, 1);
                }
            } else {
                memcpy(U2, ALL, sizeof(uint64_t) * W);
                for (int k = 0; k < Sn[i]; k++) { const uint64_t *ro = M + (size_t)Sl[i][k] * W; for (int u = 0; u < W; u++) U2[u] &= ~ro[u]; }
                f->cnt[2][18 + Sw[i]] += paare(f, U2, Sl[i], Sn[i], 18 + Sw[i]);
            }
            #pragma omp critical(fortschritt)
            {
                fertig++;
                if (jetzt() - tlog > 60 || fertig == bis - von) {
                    tlog = jetzt();
                    long kn = 0; for (int k = 0; k < nth; k++) kn += F[k]->knoten;
                    printf("  Fortschritt: %ld / %d fertig (zuletzt i=%d), Knoten %ld, %.0f s\n", fertig, bis - von, i, kn, jetzt() - t0);
                    fflush(stdout);
                }
            }
        }
        printf("ERGEBNIS %s B=%d Bereich [%d, %d) von %d: Ueberdeckungen (alle Bahnmengen, auch nicht minimale) nach Neunerbahnen und Zeilen:\n", argv[3], B, von, bis, ntot);
        int irgend = 0;
        for (int k = 0; k <= 2; k++) for (int w = 0; w <= B; w++) {
            u128 s2 = 0; for (int t = 0; t < nth; t++) s2 += F[t]->cnt[k][w];
            if (s2) { char buf[64]; druck128(s2, buf); printf("  %d Neunerbahnen, %d Zeilen (%d Woerter): %s\n", k, w, 2 * w, buf); irgend = 1; }
        }
        if (!irgend) printf("  keine\n");
        { long kn = 0; for (int k = 0; k < nth; k++) kn += F[k]->knoten; printf("Knoten %ld, Laufzeit %.1f s\n", kn, jetzt() - t0); }
        if (liste) fclose(liste);
        return 0;
    }

    if (strcmp(argv[3], "zaehle")) { fprintf(stderr, "unbekannter Modus\n"); return 1; }
    B = atoi(argv[4]); MAX9 = atoi(argv[5]);
    if (B > 60 || MAX9 > 2) { fprintf(stderr, "B <= 60, max9 <= 2\n"); return 1; }
    uint64_t *U = malloc(sizeof(uint64_t) * W), *avail = malloc(sizeof(uint64_t) * W);
    for (int v = 0; v < W; v++) { U[v] = 0; avail[v] = 0; }
    for (int p = 0; p < NT; p++) { setb(U, p); setb(avail, p); }
    if (MAX9 == 0) for (int o = NS; o < NT; o++) clr(avail, o);
    int p0 = ordn[0];
    const uint64_t *r0 = M + (size_t)p0 * W;
    int *cand = malloc(sizeof(int) * NT), nc = 0;
    for (int o = 0; o < NT; o++) if (tst(r0, o) && tst(avail, o)) cand[nc++] = o;
    int von = 0, bis = nc;
    if (argc > 7) { von = atoi(argv[6]); bis = atoi(argv[7]); if (bis > nc) bis = nc; }
    if (argc > 8) liste = fopen(argv[8], "w");
    printf("zaehle B=%d max9=%d: erste Verzweigung Punktklasse %u (Groesse %d), %d Kandidaten, Bereich [%d, %d)\n",
           B, MAX9, rep[p0], gew[p0], nc, von, bis);
    fflush(stdout);
    int nth = omp_get_max_threads();
    Faden **F = malloc(sizeof(Faden *) * nth);
    for (int i = 0; i < nth; i++) F[i] = neuer_faden();
    long fertig = 0; double tlog = jetzt();
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = von; i < bis; i++) {
        Faden *f = F[omp_get_thread_num()];
        int o = cand[i];
        if (gew[o] <= B && !(gew[o] == 9 && MAX9 == 0)) {
            uint64_t *A2 = f->Abuf, *U2 = f->Ubuf;
            memcpy(A2, avail, sizeof(uint64_t) * W);
            for (int j = 0; j <= i; j++) clr(A2, cand[j]);
            const uint64_t *ro = M + (size_t)o * W;
            for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
            f->tiefe = 0; f->wahl[f->tiefe++] = o;
            /* Puffer ab Tiefe 1 verwenden: phaseA schreibt in (d+1) */
            phaseA(f, gew[o] == 9, gew[o], B - gew[o], U2, A2, 1);
        }
        #pragma omp critical(fortschritt)
        {
            fertig++;
            if (jetzt() - tlog > 60 || fertig == bis - von) {
                tlog = jetzt();
                long kn = 0; for (int k = 0; k < nth; k++) kn += F[k]->knoten;
                printf("  Fortschritt: %ld / %d erste Zweige fertig (zuletzt i=%d), Knoten %ld, %.0f s\n", fertig, bis - von, i, kn, jetzt() - t0);
                fflush(stdout);
            }
        }
    }
    printf("ERGEBNIS B=%d max9=%d Bereich [%d, %d) von %d: Ueberdeckungen (alle Bahnmengen, auch nicht minimale) nach Neunerbahnen und Zeilen:\n", B, MAX9, von, bis, nc);
    int irgend = 0;
    for (int k = 0; k <= 2; k++) for (int w = 0; w <= B; w++) {
        u128 s = 0; for (int t = 0; t < nth; t++) s += F[t]->cnt[k][w];
        if (s) { char buf[64]; druck128(s, buf); printf("  %d Neunerbahnen, %d Zeilen (%d Woerter): %s\n", k, w, 2 * w, buf); irgend = 1; }
    }
    if (!irgend) printf("  keine\n");
    { long kn = 0; for (int k = 0; k < nth; k++) kn += F[k]->knoten; printf("Knoten %ld, Laufzeit %.1f s\n", kn, jetzt() - t0); }
    if (liste) fclose(liste);
    return 0;
}
