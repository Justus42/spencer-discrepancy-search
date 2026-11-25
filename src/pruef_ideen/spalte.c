/*
 * spalte.c -- Rechencode (Auftrag P, Teil 2). Kein Pruefcode.
 *
 * ctrl_16 (16 Zeilen, erste 15 Spalten) ohne Zeile r (r = -1: keine Zeile gestrichen), Spalte c verdeckt.
 * Alle 2^15 (bei r = -1: 2^16) Vorzeichenspalten a (Bit i = Zeile i, Bit 1 = -1) werden exakt bewertet: Zahl der offenen Klassen
 * (Klassen {x,-x} von x in {+-1}^15 mit max_i |(M x)_i| <= 5, also kein Zeilenbetrag >= 7).
 * Normierung der Klasse: x_c = +1. Die uebrigen 14 Koordinaten laufen frei (16384 Klassen).
 * Zeile i hat dann den Wert s_i + a_i (s_i = Summe ueber die anderen 14 Spalten). Die Klasse ist offen fuer a genau dann,
 * wenn fuer alle i |s_i + a_i| <= 5 gilt: bei |s_i| <= 4 frei, bei s_i = +6 nur a_i = -1, bei s_i = -6 nur a_i = +1,
 * bei |s_i| >= 8 nie.
 *
 * Aufruf: spalte MATRIX rmin rmax
 *   je (r, c) mit rmin <= r <= rmax, c = 0..14: "r c | min | Zahl der Optima | Wert der echten Spalte | echte Spalte optimal (0/1) | Kontrolle"
 *   Kontrolle: direkte Zaehlung (alle Klassen, alle Zeilen, ganze Matrix) fuer die echte Spalte und 40 Zufallsspalten stimmt (OK)
 * Bauen: gcc -O3 -march=native -o build/spalte src/pruef_ideen/spalte.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NC 15
static int M[16][NC];
static int NRZ;

static uint64_t rng_s = 88172645463325252ULL;
static uint64_t rnd(void) { rng_s ^= rng_s << 13; rng_s ^= rng_s >> 7; rng_s ^= rng_s << 17; return rng_s; }

static void lies(const char *fn) {
    FILE *f = fopen(fn, "r"); if (!f) { perror(fn); exit(2); }
    char line[4096]; NRZ = 0;
    while (fgets(line, sizeof line, f)) {
        int v, n, c = 0; char *p = line;
        while (sscanf(p, "%d%n", &v, &n) == 1) { p += n; if (c < NC) M[NRZ][c] = v; c++; }
        if (c) NRZ++;
    }
    fclose(f);
}

/* Direkte Zaehlung: ganze Matrix mit Spalte c durch a ersetzt, Zeile r gestrichen, alle 2^14 Klassen mit x_14 = +1 (Bit 14 = 0) */
static int direkt(int r, int c, int a, const int *rows, int nr) {
    int offen = 0;
    for (int x = 0; x < (1 << 14); x++) {
        int xs[NC];
        for (int j = 0; j < NC; j++) xs[j] = (x >> j & 1) ? -1 : 1;
        int ok = 1;
        for (int t = 0; t < nr && ok; t++) {
            int i = rows[t], s = 0;
            for (int j = 0; j < NC; j++) {
                int e = (j == c) ? ((a >> i & 1) ? -1 : 1) : M[i][j];
                s += e * xs[j];
            }
            if (s >= 7 || s <= -7) ok = 0;
        }
        offen += ok;
    }
    return offen;
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "Aufruf\n"); return 2; }
    lies(argv[1]);
    int rmin = atoi(argv[2]), rmax = atoi(argv[3]);
    if (NRZ != 16) { fprintf(stderr, "16 Zeilen erwartet, %d\n", NRZ); return 2; }
    static uint16_t care[1 << 14], val[1 << 14];
    static int cnt[1 << 16];
    for (int r = rmin; r <= rmax; r++) {
        int rows[16], nr = 0;
        for (int i = 0; i < 16; i++) if (i != r) rows[nr++] = i;
        for (int c = 0; c < NC; c++) {
            /* Klassenliste aufbauen. Koordinaten ausser c: 14 Stueck, auf Bits 0..13 abgebildet. x_c = +1. */
            int cols[14], q = 0;
            for (int j = 0; j < NC; j++) if (j != c) cols[q++] = j;
            int m = 0;
            for (int x = 0; x < (1 << 14); x++) {
                int ca = 0, va = 0, nie = 0;
                for (int t = 0; t < nr && !nie; t++) {
                    int i = rows[t], s = 0;
                    for (int b = 0; b < 14; b++) s += M[i][cols[b]] * ((x >> b & 1) ? -1 : 1);
                    if (s >= 8 || s <= -8) nie = 1;
                    else if (s == 6) { ca |= 1 << i; va |= 1 << i; }      /* a_i = -1 (Bit 1) */
                    else if (s == -6) { ca |= 1 << i; }                    /* a_i = +1 (Bit 0) */
                }
                if (!nie) { care[m] = (uint16_t)ca; val[m] = (uint16_t)va; m++; }
            }
            int best = 1 << 30, nopt = 0;
            for (int a = 0; a < (1 << 16); a++) {
                if (r >= 0 && (a >> r & 1)) continue; /* Bit der gestrichenen Zeile ist bedeutungslos */
                int k = 0;
                for (int t = 0; t < m; t++) k += (((a ^ val[t]) & care[t]) == 0);
                cnt[a] = k;
                if (k < best) { best = k; nopt = 1; } else if (k == best) nopt++;
            }
            int echt = 0;
            for (int i = 0; i < 16; i++) if (M[i][c] == -1 && i != r) echt |= 1 << i;
            int ok = 1;
            /* Kontrolle durch direkte Zaehlung */
            if (direkt(r, c, echt, rows, nr) != cnt[echt]) ok = 0;
            for (int z = 0; z < 40 && ok; z++) {
                int a = (int)(rnd() & 0xFFFF);
                if (r >= 0) a &= ~(1 << r);
                if (direkt(r, c, a, rows, nr) != cnt[a]) ok = 0;
            }
            printf("%d %d | %d | %d | %d | %d | %s\n", r, c, best, nopt, cnt[echt], cnt[echt] == best, ok ? "OK" : "ABWEICHUNG");
            fflush(stdout);
        }
    }
    return 0;
}
