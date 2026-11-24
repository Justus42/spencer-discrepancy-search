/* Auftrag A, Teil 4 neu: Nachrechnung von P1 (Sol, Abschnitt J). PRÜFCODE, nur ganze Zahlen.
 * n = 14, d = 7. Zeile a mit genau einer Null (Spalte z), sonst +-1. Klassen x in {+-1}^14 modulo Vorzeichen (8192).
 * Paritätshälften: Anzahl der -1 in x gerade/ungerade (bei n = 14 unter x -> -x invariant), je 4096 Klassen.
 * Behauptung: je Zeile und Hälfte 378 Klassen; streicht man eine beliebige Koordinate k, ist die Menge der 13-Wörter
 * (beide Vorzeichen jeder getroffenen Klasse) genau B(c,3) vereinigt B(~c,3) im 13-Würfel für ein c.
 * Geprüft für ALLE 57344 Zeilen (modulo Vorzeichen), beide Hälften, alle 14 Streichkoordinaten.
 * Kandidaten für c: a ohne Koordinate k, mit a_z = +1 oder -1 (falls k != z); bei keinem Treffer volle Zentrumssuche.
 * Bauen: gcc -O2 -o build/p1_pruef14 src/p1_pruef14.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 14
static unsigned char inS[1 << 13], inB[1 << 13];

static int wt(unsigned v) { return __builtin_popcount(v); }

int main(void) {
    long long faelle = 0, abw = 0, groesse_abw = 0, kandidat_fehl = 0;
    long long zeilen = 0;
    int a[N];
    for (int z = 0; z < N; z++) {
        for (int bits = 0; bits < (1 << (N - 2)); bits++) {
            /* Zeile: a_z = 0, erster Nicht-Null-Eintrag +1 (Vorzeichen normiert) */
            int k0 = 0, first = 1;
            for (int j = 0; j < N; j++) {
                if (j == z) { a[j] = 0; continue; }
                if (first) { a[j] = 1; first = 0; continue; }
                a[j] = (bits >> k0 & 1) ? -1 : 1; k0++;
            }
            zeilen++;
            /* Treffer über alle x in {+-1}^14 (Bit 1 = -1), beide Vorzeichen getrennt aufgezählt */
            for (int h = 0; h < 2; h++) {
                int klassen = 0;
                for (int k = 0; k < N; k++) {
                    memset(inS, 0, sizeof inS);
                    int npkt = 0;
                    for (int x = 0; x < (1 << N); x++) {
                        if ((wt(x) & 1) != h) continue;
                        int s = 0;
                        for (int j = 0; j < N; j++) s += (x >> j & 1) ? -a[j] : a[j];
                        if (s >= 7 || s <= -7) {
                            unsigned w = 0; int q = 0;
                            for (int j = 0; j < N; j++) if (j != k) { w |= (unsigned)(x >> j & 1) << q; q++; }
                            if (!inS[w]) { inS[w] = 1; npkt++; }
                            if (k == 0 && x < (x ^ ((1 << N) - 1))) klassen++;
                        }
                    }
                    faelle++;
                    if (k == 0 && klassen != 378) groesse_abw++;
                    if (npkt != 756) { abw++; continue; }
                    /* Kandidaten c */
                    int ok = 0;
                    for (int sz = 0; sz < 2 && !ok; sz++) {
                        unsigned c = 0; int q = 0;
                        for (int j = 0; j < N; j++) if (j != k) {
                            int v = (j == z) ? (sz ? -1 : 1) : a[j];
                            if (v < 0) c |= 1u << q;
                            q++;
                        }
                        int gl = 1;
                        for (unsigned w = 0; w < (1u << 13) && gl; w++) {
                            int d = wt(w ^ c);
                            int inb = (d <= 3 || d >= 10);
                            if (inb != inS[w]) gl = 0;
                        }
                        if (gl) ok = 1;
                    }
                    if (!ok) {
                        kandidat_fehl++;
                        /* volle Zentrumssuche */
                        int ok2 = 0;
                        for (unsigned c = 0; c < (1u << 13) && !ok2; c++) {
                            if (!inS[c]) continue;
                            int gl = 1;
                            for (unsigned w = 0; w < (1u << 13) && gl; w++) { int d = wt(w ^ c); if ((d <= 3 || d >= 10) != inS[w]) gl = 0; }
                            if (gl) ok2 = 1;
                        }
                        if (!ok2) abw++;
                    }
                }
            }
        }
        fprintf(stderr, "Nullspalte %d fertig\n", z);
    }
    printf("Zeilen: %lld, Faelle (Zeile x Haelfte x Streichkoordinate): %lld\n", zeilen, faelle);
    printf("Klassenzahl je Zeile und Haelfte != 378: %lld\n", groesse_abw);
    printf("Abweichungen (keine antipodale Kugel vom Radius 3): %lld\n", abw);
    printf("Kandidat aus der Zeile passte nicht (volle Suche noetig): %lld\n", kandidat_fehl);
    return abw || groesse_abw;
}
