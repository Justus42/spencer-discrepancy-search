/* bahnen_zaehl.c -- Pruefcode (Teil A, 24.11.2026). Kein Suchcode.
 *
 * Laenge n = 5k, sigma dreht jeden 5er-Block (Bits 5j..5j+4) um eine Stelle, dazu Komplement.
 * Bahnen der Gruppe G = <sigma, Komplement> (Ordnung 10): volle Bahnen (10 Woerter) und feste
 * Paare {s, ~s}, s konstant auf jedem Block.
 *
 * Zaehlt vollstaendig (Verzweigen nach unueberdecktem Punkt, je Zweig "o_i drin, o_1..o_{i-1}
 * nicht drin"; jede Menge genau einmal) die t-Mengen voller Bahnen, die zusammen mit
 * (wahlweise) allen festen Paaren den Wuerfel {0,1}^n mit Radius R ueberdecken.
 *
 * Aufruf: bahnen_zaehl n R t mitpaaren(0/1) [liste.txt] [-- rep1 rep2 ...]
 *   Mit Vertretern nach "--": nur pruefen, ob diese Bahnen (plus ggf. Paare) ueberdecken.
 * Bauen: gcc -O3 -march=native -fopenmp -o build/bahnen_zaehl src/bahnen_zaehl.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int n, R, NB, NF, NP, NT, W;      /* NB Bloecke, NF volle, NP Paare, NT gesamt, W Woerter je Bitset */
static int *bid;                          /* Bahn-Nummer je Wort */
static uint32_t *rep;                     /* kleinster Vertreter je Bahn */
static uint64_t *M;                       /* M[p*W ..]: Bahnen o mit d(Bahn o, Bahn p) <= R (symmetrisch) */
static int *ordnung;                      /* Punkt-Bahnen nach statischer Zeilenstaerke */
static FILE *liste;

static uint32_t rot(uint32_t x) {
    uint32_t y = 0;
    for (int b = 0; b < NB; b++) {
        uint32_t blk = (x >> (5 * b)) & 31u;
        blk = ((blk << 1) | (blk >> 4)) & 31u;
        y |= blk << (5 * b);
    }
    return y;
}

static int pc(uint64_t v) { return __builtin_popcountll(v); }

static long long binom(long long a, int k) {
    if (k < 0 || a < k) return 0;
    long long r = 1;
    for (int i = 0; i < k; i++) r = r * (a - i) / (i + 1);
    return r;
}

/* zaehlt t-Mengen aus avail (nur volle Bahnen), die alle Punkte in U ueberdecken */
static long long zaehle(int t, const uint64_t *U, const uint64_t *avail, int *wahl, int tiefe) {
    int leer = 1;
    for (int w = 0; w < W; w++) if (U[w]) { leer = 0; break; }
    if (leer) {
        long long a = 0;
        for (int w = 0; w < W; w++) a += pc(avail[w]);
        if (liste && t > 0) { /* Erweiterungen werden nur gezaehlt, nicht aufgelistet */
            fprintf(liste, "# Teilmenge deckt schon, %d frei aus %lld:", t, a);
            for (int i = 0; i < tiefe; i++) fprintf(liste, " %u", rep[wahl[i]]);
            fprintf(liste, "\n");
        } else if (liste) {
            for (int i = 0; i < tiefe; i++) fprintf(liste, "%u ", rep[wahl[i]]);
            fprintf(liste, "\n");
        }
        return binom(a, t);
    }
    if (t == 0) return 0;
    if (t == 1) {
        uint64_t X[W];
        memcpy(X, avail, sizeof(uint64_t) * W);
        for (int w = 0; w < W; w++) {
            uint64_t u = U[w];
            while (u) {
                int p = w * 64 + __builtin_ctzll(u); u &= u - 1;
                const uint64_t *r = M + (size_t)p * W;
                int nz = 0;
                for (int v = 0; v < W; v++) { X[v] &= r[v]; nz |= (X[v] != 0); }
                if (!nz) return 0;
            }
        }
        long long c = 0;
        for (int v = 0; v < W; v++) {
            uint64_t x = X[v];
            while (x) {
                int o = v * 64 + __builtin_ctzll(x); x &= x - 1;
                c++;
                if (liste) {
                    for (int i = 0; i < tiefe; i++) fprintf(liste, "%u ", rep[wahl[i]]);
                    fprintf(liste, "%u\n", rep[o]);
                }
            }
        }
        return c;
    }
    /* Verzweigungspunkt: erster unueberdeckter Punkt in statischer Ordnung */
    int p = -1;
    for (int i = 0; i < NT; i++) { int q = ordnung[i]; if ((U[q >> 6] >> (q & 63)) & 1) { p = q; break; } }
    const uint64_t *r = M + (size_t)p * W;
    uint64_t A2[W], U2[W];
    memcpy(A2, avail, sizeof(uint64_t) * W);
    long long tot = 0;
    for (int v = 0; v < W; v++) {
        uint64_t x = r[v] & avail[v];
        while (x) {
            int o = v * 64 + __builtin_ctzll(x); x &= x - 1;
            A2[o >> 6] &= ~(1ULL << (o & 63));       /* o und alle frueheren Kandidaten raus */
            const uint64_t *ro = M + (size_t)o * W;
            for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
            wahl[tiefe] = o;
            tot += zaehle(t - 1, U2, A2, wahl, tiefe + 1);
        }
    }
    return tot;
}

int main(int argc, char **argv) {
    if (argc < 5) { fprintf(stderr, "Aufruf: n R t mitpaaren [liste] [-- reps]\n"); return 1; }
    n = atoi(argv[1]); R = atoi(argv[2]); int t = atoi(argv[3]); int mitp = atoi(argv[4]);
    int argrep = -1;
    for (int i = 5; i < argc; i++) if (!strcmp(argv[i], "--")) { argrep = i + 1; break; }
    if (argc > 5 && strcmp(argv[5], "--")) liste = fopen(argv[5], "w");
    NB = n / 5;
    uint32_t N = 1u << n, alle = N - 1;
    bid = malloc(sizeof(int) * N);
    for (uint32_t x = 0; x < N; x++) bid[x] = -1;
    rep = malloc(sizeof(uint32_t) * N);
    /* volle Bahnen zuerst (aufsteigend nach kleinstem Vertreter), dann feste Paare */
    NF = 0;
    for (int pass = 0; pass < 2; pass++)
        for (uint32_t x = 0; x < N; x++) {
            if (bid[x] >= 0) continue;
            int fest = (rot(x) == x);
            if (fest != pass) continue;
            int id = NF++;
            rep[id] = x;
            uint32_t y = x;
            for (int j = 0; j < 5; j++) { bid[y] = id; bid[y ^ alle] = id; y = rot(y); }
        }
    NT = NF;
    NP = 1 << (NB - 1);
    NF = NT - NP;
    W = (NT + 63) / 64;
    printf("n=%d R=%d: %d volle Bahnen, %d feste Paare\n", n, R, NF, NP);
    /* Kugel-Masken */
    int nm = 0; uint32_t *mask = malloc(sizeof(uint32_t) * N);
    for (uint32_t x = 0; x < N; x++) if (__builtin_popcount(x) <= R) mask[nm++] = x;
    M = calloc((size_t)NT * W, sizeof(uint64_t));
    #pragma omp parallel for schedule(dynamic, 16)
    for (int p = 0; p < NT; p++) {
        uint64_t *r = M + (size_t)p * W;
        for (int i = 0; i < nm; i++) { int o = bid[rep[p] ^ mask[i]]; r[o >> 6] |= 1ULL << (o & 63); }
    }
    /* Symmetrie pruefen */
    long long asym = 0;
    for (int p = 0; p < NT; p++) for (int o = 0; o < NT; o++)
        if (((M[(size_t)p * W + (o >> 6)] >> (o & 63)) & 1) != ((M[(size_t)o * W + (p >> 6)] >> (p & 63)) & 1)) asym++;
    printf("Kugel %d Woerter, Ueberdeckungsmatrix asymmetrische Eintraege: %lld\n", nm, asym);
    int *gr = malloc(sizeof(int) * NT);
    int mn = 1 << 30, mx = 0;
    for (int p = 0; p < NT; p++) {
        int c = 0; for (int w = 0; w < W; w++) {
            uint64_t m = M[(size_t)p * W + w];
            if (w == (NF >> 6)) m &= (NF & 63) ? ((1ULL << (NF & 63)) - 1) : 0; /* nur volle */
            if (w > (NF >> 6)) m = 0;
            c += pc(m);
        }
        gr[p] = c; if (c < mn) mn = c; if (c > mx) mx = c;
    }
    printf("volle Bahnen, die eine Punkt-Bahn ueberdecken: min %d, max %d\n", mn, mx);
    ordnung = malloc(sizeof(int) * NT);
    for (int i = 0; i < NT; i++) ordnung[i] = i;
    for (int i = 1; i < NT; i++) { int a = ordnung[i], j = i; while (j > 0 && gr[ordnung[j - 1]] > gr[a]) { ordnung[j] = ordnung[j - 1]; j--; } ordnung[j] = a; }

    uint64_t U[W], avail[W];
    for (int w = 0; w < W; w++) { U[w] = 0; avail[w] = 0; }
    for (int p = 0; p < NT; p++) U[p >> 6] |= 1ULL << (p & 63);
    for (int o = 0; o < NF; o++) avail[o >> 6] |= 1ULL << (o & 63);
    if (mitp) for (int o = NF; o < NT; o++) { const uint64_t *r = M + (size_t)o * W; for (int w = 0; w < W; w++) U[w] &= ~r[w]; }
    int offen = 0; for (int w = 0; w < W; w++) offen += pc(U[w]);
    printf("mit Paaren=%d: offene Punkt-Bahnen vor der Wahl: %d\n", mitp, offen);

    if (argrep > 0) {
        for (int i = argrep; i < argc; i++) {
            uint32_t x = (uint32_t)strtoul(argv[i], 0, 10);
            int o = bid[x];
            printf("Vertreter %u -> Bahn %d (kleinster Vertreter %u, %s)\n", x, o, rep[o], o < NF ? "voll" : "Paar");
            const uint64_t *r = M + (size_t)o * W; for (int w = 0; w < W; w++) U[w] &= ~r[w];
        }
        offen = 0; for (int w = 0; w < W; w++) offen += pc(U[w]);
        printf("offene Punkt-Bahnen nach den Vertretern: %d -> %s\n", offen, offen ? "KEINE Ueberdeckung" : "UEBERDECKUNG");
        return 0;
    }
    /* parallel ueber die erste Verzweigung */
    int p0 = -1;
    for (int i = 0; i < NT; i++) { int q = ordnung[i]; if ((U[q >> 6] >> (q & 63)) & 1) { p0 = q; break; } }
    long long tot = 0;
    if (p0 < 0) { long long a = 0; for (int w = 0; w < W; w++) a += pc(avail[w]); tot = binom(a, t); }
    else {
        const uint64_t *r = M + (size_t)p0 * W;
        int nc = 0, *cand = malloc(sizeof(int) * NT);
        for (int o = 0; o < NF; o++) if ((r[o >> 6] >> (o & 63)) & 1) cand[nc++] = o;
        printf("erste Verzweigung: Punkt-Bahn %u, %d Kandidaten\n", rep[p0], nc);
        if (liste) { /* Liste nur einfaedig */
            for (int i = 0; i < nc; i++) {
                uint64_t A2[W], U2[W]; int wahl[16];
                memcpy(A2, avail, sizeof A2);
                for (int j = 0; j <= i; j++) A2[cand[j] >> 6] &= ~(1ULL << (cand[j] & 63));
                const uint64_t *ro = M + (size_t)cand[i] * W;
                for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
                wahl[0] = cand[i];
                tot += zaehle(t - 1, U2, A2, wahl, 1);
            }
        } else {
            #pragma omp parallel for schedule(dynamic, 1) reduction(+:tot)
            for (int i = 0; i < nc; i++) {
                uint64_t A2[W], U2[W]; int wahl[16];
                memcpy(A2, avail, sizeof A2);
                for (int j = 0; j <= i; j++) A2[cand[j] >> 6] &= ~(1ULL << (cand[j] & 63));
                const uint64_t *ro = M + (size_t)cand[i] * W;
                for (int u = 0; u < W; u++) U2[u] = U[u] & ~ro[u];
                wahl[0] = cand[i];
                tot += zaehle(t - 1, U2, A2, wahl, 1);
            }
        }
    }
    printf("ERGEBNIS n=%d R=%d t=%d mitPaaren=%d: %lld ueberdeckende %d-Mengen voller Bahnen\n", n, R, t, mitp, tot, t);
    if (liste) fclose(liste);
    return 0;
}
