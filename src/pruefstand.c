/*
 * pruefstand.c -- Suchcode (kein Beweis). Sucht zu einer Matrix einen Vorzeichenvektor x mit kleinem
 * max_i |(A x)_i|. Ein solcher x ist ein Gegenbeispiel gegen jede behauptete Diskrepanz > max.
 * Findet er nichts, ist das KEIN Beweis; dann exakt prüfen (build/exakt_disc, check_disc.py).
 *
 * Aufruf:
 *   pruefstand DATEI [-d D] [-z SEKUNDEN] [-s STARTWERT] [-t]
 *     -d D   Gegenbeispiel = x mit Maximum < D suchen, beim ersten Treffer aufhören.
 *            Ohne -d: D = kleinstes d mit d/sqrt(N) > 5/sqrt(8), N = max(Zeilen, Spalten)
 *            (Rekordschwelle), und danach weiter nach unten minimieren, bis die Zeit um ist.
 *     -z     Zeitlimit in Sekunden (Standard 10), -s Startwert (Standard 1), -t transponiert.
 *     -m     nur minimieren (keine Schwelle), Ausgabe des besten x.
 * Ausgabe: "GEGENBEISPIEL max M ..." mit x als +/- Folge, oder "kein Gegenbeispiel gefunden".
 *
 * Verfahren: Tabu-Suche mit Ziel T; Kosten = Summe über Zeilen von max(0, |r_i| - T).
 * Ein Zug dreht ein Vorzeichen um; der beste nicht tabuisierte Zug wird gewählt (Tabu-Dauer
 * zufällig), Neustart nach Stillstand. Bei Kosten 0 wird T = Maximum - 1 gesetzt.
 * Nur ganze Zahlen. Bauen: gcc -O3 -march=native -o build/pruefstand src/pruefstand.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MAXD 128
static int A[MAXD][MAXD], R, C;

static uint64_t rs = 88172645463325252ULL;
static inline uint64_t rnd(void) { rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17; return rs; }

static double jetzt(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }

static void lies(const char *fn, int transp) {
    FILE *f = fopen(fn, "r");
    if (!f) { perror(fn); exit(2); }
    static int B[MAXD][MAXD];
    char line[1 << 15];
    int r = 0, c = -1;
    while (fgets(line, sizeof line, f)) {
        int k = 0, v, n; char *p = line;
        while (sscanf(p, "%d%n", &v, &n) == 1) {
            if (k >= MAXD || r >= MAXD) { fprintf(stderr, "zu groß\n"); exit(2); }
            B[r][k++] = v; p += n;
        }
        if (!k) continue;
        if (c < 0) c = k;
        if (k != c) { fprintf(stderr, "Zeilenlänge uneinheitlich\n"); exit(2); }
        r++;
    }
    fclose(f);
    if (transp) { R = c; C = r; for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) A[i][j] = B[j][i]; }
    else        { R = r; C = c; for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) A[i][j] = B[i][j]; }
}

static int maxbetrag(const int *x) {
    int m = 0;
    for (int i = 0; i < R; i++) {
        int s = 0;
        for (int j = 0; j < C; j++) s += A[i][j] * x[j];
        if (s < 0) s = -s;
        if (s > m) m = s;
    }
    return m;
}

static inline int ex(int v, int T) { if (v < 0) v = -v; return v > T ? v - T : 0; }

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Aufruf: pruefstand DATEI [-d D] [-z SEK] [-s STARTWERT] [-t] [-m]\n"); return 2; }
    int D = -1, transp = 0, nurmin = 0; double zeit = 10; unsigned long seed = 1;
    for (int a = 2; a < argc; a++) {
        if (!strcmp(argv[a], "-d") && a + 1 < argc) D = atoi(argv[++a]);
        else if (!strcmp(argv[a], "-z") && a + 1 < argc) zeit = atof(argv[++a]);
        else if (!strcmp(argv[a], "-s") && a + 1 < argc) seed = strtoul(argv[++a], 0, 10);
        else if (!strcmp(argv[a], "-t")) transp = 1;
        else if (!strcmp(argv[a], "-m")) nurmin = 1;
        else { fprintf(stderr, "unbekannt: %s\n", argv[a]); return 2; }
    }
    lies(argv[1], transp);
    rs ^= seed * 0x9E3779B97F4A7C15ULL; for (int i = 0; i < 20; i++) rnd();
    int N = R > C ? R : C;
    int stopp_bei_d = (D > 0);
    if (D <= 0) { D = 1; while (8LL * D * D <= 25LL * N) D++; }   /* d/sqrt(N) > 5/sqrt(8) */
    if (nurmin) { stopp_bei_d = 0; }

    int x[MAXD], bx[MAXD], r[MAXD], tabu[MAXD];
    int best = 1 << 30;
    long schritte = 0, neustarts = 0;
    double t0 = jetzt(), t_best = 0;
    int T = -1;
    while (jetzt() - t0 < zeit) {
        /* Neustart */
        neustarts++;
        for (int j = 0; j < C; j++) { x[j] = (rnd() & 1) ? 1 : -1; tabu[j] = 0; }
        for (int i = 0; i < R; i++) { int s = 0; for (int j = 0; j < C; j++) s += A[i][j] * x[j]; r[i] = s; }
        int m = 0; for (int i = 0; i < R; i++) { int a = r[i] < 0 ? -r[i] : r[i]; if (a > m) m = a; }
        if (m < best) { best = m; memcpy(bx, x, sizeof(int) * C); t_best = jetzt() - t0; }
        if (best < D && stopp_bei_d) break;
        T = best - 1;
        int cost = 0; for (int i = 0; i < R; i++) cost += ex(r[i], T);
        int bestcost = cost; long letzte_verb = schritte, it = 0;
        while (cost > 0) {
            it++; schritte++;
            if ((schritte & 1023) == 0 && jetzt() - t0 >= zeit) break;
            int bj = -1, bd = 1 << 30, nb = 0;
            for (int j = 0; j < C; j++) {
                int dsum = 0;
                for (int i = 0; i < R; i++) {
                    if (!A[i][j]) continue;
                    int nr = r[i] - 2 * A[i][j] * x[j];
                    dsum += ex(nr, T) - ex(r[i], T);
                }
                int erlaubt = tabu[j] <= it || cost + dsum < bestcost;
                if (!erlaubt) continue;
                if (dsum < bd) { bd = dsum; bj = j; nb = 1; }
                else if (dsum == bd && (rnd() % (++nb)) == 0) bj = j;
            }
            if (bj < 0) { bj = rnd() % C; bd = 0; for (int i = 0; i < R; i++) bd += ex(r[i] - 2 * A[i][bj] * x[bj], T) - ex(r[i], T); }
            for (int i = 0; i < R; i++) r[i] -= 2 * A[i][bj] * x[bj];
            x[bj] = -x[bj];
            cost += bd;
            tabu[bj] = it + 1 + (int)(rnd() % (unsigned)(C / 4 + 2));
            if (cost < bestcost) { bestcost = cost; letzte_verb = schritte; }
            if (cost == 0) {
                int mm = 0; for (int i = 0; i < R; i++) { int a = r[i] < 0 ? -r[i] : r[i]; if (a > mm) mm = a; }
                if (mm < best) { best = mm; memcpy(bx, x, sizeof(int) * C); t_best = jetzt() - t0; }
                if (best < D && stopp_bei_d) break;
                T = best - 1;
                cost = 0; for (int i = 0; i < R; i++) cost += ex(r[i], T);
                bestcost = cost; letzte_verb = schritte;
            }
            if (schritte - letzte_verb > 2000L + 200L * C) break;   /* Stillstand: Neustart */
        }
        if (best < D && stopp_bei_d) break;
    }
    int chk = maxbetrag(bx);
    if (chk != best) { fprintf(stderr, "FEHLER: Nachrechnung %d statt %d\n", chk, best); return 3; }
    printf("matrix %d x %d%s, schwelle D=%d, bestes max %d (nach %.2f s), %ld schritte, %ld neustarts, %.1f s\n",
           R, C, transp ? " (transponiert)" : "", D, best, t_best, schritte, neustarts, jetzt() - t0);
    printf("x = ");
    for (int j = 0; j < C; j++) putchar(bx[j] > 0 ? '+' : '-');
    printf("\n");
    if (best < D) printf("GEGENBEISPIEL max %d < %d\n", best, D);
    else printf("kein Gegenbeispiel gefunden (kein x mit max < %d in %.0f s)\n", D, zeit);
    return 0;
}
