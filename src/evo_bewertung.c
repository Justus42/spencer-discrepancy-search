/* evo_bewertung.c -- Bewerter fuer Auftrag E (Evolutionsschleife), Pruefcode.
 *
 * Aufruf:  evo_bewertung m R DATEI
 *   DATEI: k Zeilen, je mindestens m ganze Zahlen +1/-1 (Leerraum-getrennt).
 *          Gelesen werden die ersten m Eintraege je Zeile; weitere Eintraege
 *          werden ignoriert und auf stderr gemeldet. Leere Zeilen zaehlen nicht.
 *   Jede Zeile w ist ein Paar {w, -w}. Eine Klasse {x, -x} ist offen, wenn kein
 *   Wort und kein Komplement im Hamming-Abstand <= R liegt.
 * Ausgabe (stdout, eine Zeile):  offen=<Zahl> klassen=<2^(m-1)> k=<k> m=<m> R=<R>
 * Rechnet exakt ueber alle 2^(m-1) Klassen, nur ganze Zahlen. Teilt keinen Code
 * mit Suchprogrammen. Nach der Kontrolle eingefroren (runs/evo/BEWERTER.sha256).
 * Fehlercodes: 0 ok, 2 Eingabefehler.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAXK 4096

static uint64_t wort[MAXK];

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Aufruf: %s m R DATEI\n", argv[0]);
        return 2;
    }
    char *ende;
    long m = strtol(argv[1], &ende, 10);
    if (*ende || m < 2 || m > 34) { fprintf(stderr, "m ungueltig (2..34)\n"); return 2; }
    long R = strtol(argv[2], &ende, 10);
    if (*ende || R < 0 || R >= m) { fprintf(stderr, "R ungueltig\n"); return 2; }
    FILE *f = fopen(argv[3], "r");
    if (!f) { fprintf(stderr, "Datei nicht lesbar: %s\n", argv[3]); return 2; }

    int k = 0;
    long ignoriert = 0;
    char zeile[1 << 16];
    int nr = 0;
    while (fgets(zeile, sizeof zeile, f)) {
        nr++;
        if (strlen(zeile) == sizeof zeile - 1) { fprintf(stderr, "Zeile %d zu lang\n", nr); return 2; }
        char *p = zeile;
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) continue;                       /* leere Zeile */
        if (k >= MAXK) { fprintf(stderr, "zu viele Zeilen\n"); return 2; }
        uint64_t w = 0;
        long spalte = 0;
        while (*p) {
            while (*p && isspace((unsigned char)*p)) p++;
            if (!*p) break;
            char *q;
            long v = strtol(p, &q, 10);
            if (q == p) { fprintf(stderr, "Zeile %d: keine ganze Zahl\n", nr); return 2; }
            if (*q && !isspace((unsigned char)*q)) { fprintf(stderr, "Zeile %d: keine ganze Zahl\n", nr); return 2; }
            p = q;
            if (spalte < m) {
                if (v == -1) w |= (uint64_t)1 << spalte;      /* Bit 1 = Eintrag -1 */
                else if (v != 1) { fprintf(stderr, "Zeile %d Spalte %ld: Eintrag %ld nicht +1/-1\n", nr, spalte + 1, v); return 2; }
            } else {
                ignoriert++;
            }
            spalte++;
        }
        if (spalte < m) { fprintf(stderr, "Zeile %d: nur %ld Eintraege, m = %ld\n", nr, spalte, m); return 2; }
        wort[k++] = w;
    }
    fclose(f);
    if (k == 0) { fprintf(stderr, "keine Zeilen\n"); return 2; }
    if (ignoriert) fprintf(stderr, "Hinweis: %ld Eintraege jenseits von Spalte %ld ignoriert\n", ignoriert, m);

    /* Klassen: x mit Bit m-1 = 0 vertritt {x, Komplement von x}.
     * Ueberdeckt, wenn fuer ein i gilt: d(x, w_i) <= R  oder  d(x, ~w_i) = m - d(x, w_i) <= R. */
    const uint64_t klassen = (uint64_t)1 << (m - 1);
    uint64_t offen = 0;
    #pragma omp parallel for schedule(static) reduction(+:offen)
    for (int64_t xi = 0; xi < (int64_t)klassen; xi++) {
        uint64_t x = (uint64_t)xi;
        int gedeckt = 0;
        for (int i = 0; i < k; i++) {
            long d = __builtin_popcountll(x ^ wort[i]);
            if (d <= R || m - d <= R) { gedeckt = 1; break; }
        }
        if (!gedeckt) offen++;
    }
    printf("offen=%llu klassen=%llu k=%d m=%ld R=%ld\n",
           (unsigned long long)offen, (unsigned long long)klassen, k, m, R);
    return 0;
}
