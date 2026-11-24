/* Suchcode (Test C, 24.11.2026): Gegenbeispiele fuer die Bauregel auf Ordnung 40.
 *
 * H = Hadamard-Matrix der Ordnung 20 (Textdatei), M = [[H,H],[H,-H]] (40 x 40).
 * Muster 13: links 1 Spalte, rechts 3 Spalten gestrichen, Ueberlappung (die linke gehoert zu den rechten).
 * Muster 22: links 2, rechts 2 gestrichen, Ueberlappung genau 1.
 * Fuer JEDE Auswahl wird ein Vorzeichenvektor x in {-1,+1}^36 mit max_i |(Ax)_i| <= T gesucht
 * (A = 40 x 36). Ein Treffer zeigt disc(A) <= T < 12 und erledigt diese Auswahl.
 * Tabu-Suche auf cost = sum_i max(0,|s_i|-T)^2. Ausgabe je Auswahl eine Zeile:
 *   SEL <muster> <links> <rechts> gefunden <x als 36 Zeichen, 1 = -1> <max|Ax|> <schritte>
 *   SEL <muster> <links> <rechts> offen <schritte>
 * Das Ergebnis wird NICHT geglaubt, sondern von hadamard_tests.py (ganzzahlig) nachgerechnet.
 *
 * Aufruf: gegenbeispiel40 H.txt muster(13|22) T sekunden max_schritte_je_start max_starts seed
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define NR 40
#define MC 36
static int H[20][20];
static int8_t A[NR][MC];
static uint64_t rs;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }

static inline int ex(int s, int T) { int a = s < 0 ? -s : s; return a > T ? (a - T) * (a - T) : 0; }

/* Tabu-Suche; gibt 1 bei Erfolg, x in xo (+1/-1), Schritte in *steps */
static int suche(int T, long maxs, int maxst, int8_t *xo, long *steps, int *mx) {
    int8_t x[MC];
    int s[NR];
    for (int st = 0; st < maxst; st++) {
        for (int j = 0; j < MC; j++) x[j] = (rnd() & 1) ? 1 : -1;
        for (int i = 0; i < NR; i++) { int t = 0; for (int j = 0; j < MC; j++) t += A[i][j] * x[j]; s[i] = t; }
        long tabu[MC]; memset(tabu, 0, sizeof tabu);
        int cost = 0; for (int i = 0; i < NR; i++) cost += ex(s[i], T);
        int best = cost;
        for (long k = 1; k <= maxs; k++) {
            (*steps)++;
            if (cost == 0) {
                int m = 0; for (int i = 0; i < NR; i++) { int a = s[i] < 0 ? -s[i] : s[i]; if (a > m) m = a; }
                memcpy(xo, x, MC); *mx = m; return 1;
            }
            int bj = -1, bd = 1 << 30, nb = 0;
            for (int j = 0; j < MC; j++) {
                int d = 0;
                for (int i = 0; i < NR; i++) d += ex(s[i] - 2 * A[i][j] * x[j], T) - ex(s[i], T);
                int ok = tabu[j] <= k || cost + d < best;
                if (!ok) continue;
                if (d < bd) { bd = d; bj = j; nb = 1; }
                else if (d == bd) { nb++; if (rnd() % nb == 0) bj = j; }
            }
            if (bj < 0) bj = rnd() % MC;
            for (int i = 0; i < NR; i++) s[i] -= 2 * A[i][bj] * x[bj];
            x[bj] = -x[bj];
            cost = 0; for (int i = 0; i < NR; i++) cost += ex(s[i], T);
            if (cost < best) best = cost;
            tabu[bj] = k + 4 + rnd() % 8;
        }
    }
    return 0;
}

static void baue(const int *linksweg, int nl, const int *rechtsweg, int nr) {
    int c = 0;
    for (int j = 0; j < 20; j++) { int w = 0; for (int t = 0; t < nl; t++) if (linksweg[t] == j) w = 1; if (w) continue;
        for (int i = 0; i < 20; i++) { A[i][c] = H[i][j]; A[20 + i][c] = H[i][j]; } c++; }
    for (int j = 0; j < 20; j++) { int w = 0; for (int t = 0; t < nr; t++) if (rechtsweg[t] == j) w = 1; if (w) continue;
        for (int i = 0; i < 20; i++) { A[i][c] = H[i][j]; A[20 + i][c] = -H[i][j]; } c++; }
    if (c != MC) { fprintf(stderr, "Spaltenzahl %d\n", c); exit(2); }
}

int main(int argc, char **argv) {
    if (argc < 8) { fprintf(stderr, "Aufruf: %s H.txt muster T sekunden max_schritte max_starts seed\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "r"); if (!f) { perror("H"); return 1; }
    for (int i = 0; i < 20; i++) for (int j = 0; j < 20; j++) if (fscanf(f, "%d", &H[i][j]) != 1) { fprintf(stderr, "H lesen\n"); return 1; }
    fclose(f);
    int muster = atoi(argv[2]), T = atoi(argv[3]); double lim = atof(argv[4]);
    long maxs = atol(argv[5]); int maxst = atoi(argv[6]); rs = 88172645463325252ULL ^ (uint64_t)atol(argv[7]) * 0x9E3779B97F4A7C15ULL;
    for (int i = 0; i < 5; i++) rnd();
    double t0 = now(); long n = 0, gef = 0, offen = 0;
    int8_t x[MC];
    if (muster == 13) {
        for (int l = 0; l < 20; l++)
            for (int a = 0; a < 20; a++) for (int b = a + 1; b < 20; b++) for (int c = b + 1; c < 20; c++) {
                if (l != a && l != b && l != c) continue;
                if (now() - t0 > lim) { printf("ABBRUCH Zeitlimit nach %ld Auswahlen\n", n); fflush(stdout); goto ende; }
                int lw[1] = {l}, rw[3] = {a, b, c}; baue(lw, 1, rw, 3);
                long steps = 0; int mx = 0; int ok = suche(T, maxs, maxst, x, &steps, &mx); n++;
                if (ok) { gef++; printf("SEL 13 %d %d,%d,%d gefunden ", l, a, b, c); for (int j = 0; j < MC; j++) putchar(x[j] < 0 ? '1' : '0'); printf(" %d %ld\n", mx, steps); }
                else { offen++; printf("SEL 13 %d %d,%d,%d offen %ld\n", l, a, b, c, steps); }
                fflush(stdout);
            }
    } else {
        for (int l1 = 0; l1 < 20; l1++) for (int l2 = l1 + 1; l2 < 20; l2++)
            for (int r1 = 0; r1 < 20; r1++) for (int r2 = r1 + 1; r2 < 20; r2++) {
                int ue = (r1 == l1) + (r1 == l2) + (r2 == l1) + (r2 == l2);
                if (ue != 1) continue;
                if (now() - t0 > lim) { printf("ABBRUCH Zeitlimit nach %ld Auswahlen\n", n); fflush(stdout); goto ende; }
                int lw[2] = {l1, l2}, rw[2] = {r1, r2}; baue(lw, 2, rw, 2);
                long steps = 0; int mx = 0; int ok = suche(T, maxs, maxst, x, &steps, &mx); n++;
                if (ok) { gef++; printf("SEL 22 %d,%d %d,%d gefunden ", l1, l2, r1, r2); for (int j = 0; j < MC; j++) putchar(x[j] < 0 ? '1' : '0'); printf(" %d %ld\n", mx, steps); }
                else { offen++; printf("SEL 22 %d,%d %d,%d offen %ld\n", l1, l2, r1, r2, steps); }
                fflush(stdout);
            }
    }
ende:
    printf("ENDE muster %d T %d auswahlen %ld gefunden %ld offen %ld sekunden %.1f\n", muster, T, n, gef, offen, now() - t0);
    return 0;
}
