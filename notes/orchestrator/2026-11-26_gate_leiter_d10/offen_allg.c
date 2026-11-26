/* offen_allg.c -- Gate zu Auftrag D, Probe des Orchestrators. Zaehlt fuer eine beliebige +-1-Matrix (Zeilen mit m <= 32 Eintraegen)
   die Punkte x (bis auf Vorzeichen), die keine Zeile mit |<a_i,x>| >= d hat. Aufruf: offen_allg MATRIX d */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv) {
    static char z[4096]; unsigned r[128]; int n = 0, m = 0, d = atoi(argv[2]);
    FILE *f = fopen(argv[1], "r"); if (!f) return 2;
    while (fgets(z, sizeof z, f)) { if (z[0] == '#') continue; char *t = strtok(z, " \t\n"); int j = 0; unsigned v = 0;
        while (t) { int e = atoi(t); if (e != 1 && e != -1) return 4; if (e == -1) v |= 1u << j; j++; t = strtok(NULL, " \t\n"); }
        if (!j) continue; if (n && j != m) return 5; m = j; r[n++] = v; }
    int R = (m - d) / 2; long offen = 0; unsigned long N = 1UL << (m - 1);
    #pragma omp parallel for reduction(+:offen) schedule(static)
    for (unsigned long x = 0; x < N; x++) { int frei = 1;
        for (int i = 0; i < n; i++) { int p = __builtin_popcount(r[i] ^ (unsigned)x); if (p <= R || p >= m - R) { frei = 0; break; } }
        offen += frei; }
    printf("Zeilen=%d m=%d d=%d R=%d offen_bis_auf_Vorzeichen=%ld von=%lu\n", n, m, d, R, offen, N);
    return 0;
}
