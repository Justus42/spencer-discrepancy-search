/* Auftrag A, Teil 4 neu: Nachrechnung von P1 (Sol, Abschnitt J). PRÜFCODE, nur ganze Zahlen.
 * n = 14, d = 7. Zeile a mit genau einer Null (Spalte z), sonst +-1. Klassen x in {+-1}^14 modulo Vorzeichen (8192).
 * Paritätshälften: Anzahl der -1 in x gerade/ungerade (bei n = 14 unter x -> -x invariant), je 4096 Klassen.
 * Behauptung: je Zeile und Hälfte 378 Klassen; streicht man eine beliebige Koordinate k, ist die Menge der 13-Wörter
 * (beide Vorzeichen jeder getroffenen Klasse) genau B(c,3) vereinigt B(~c,3) im 13-Würfel für ein c.
 * Geprüft für ALLE 57344 Zeilen (modulo Vorzeichen), beide Hälften, alle 14 Streichkoordinaten.
 * Kandidaten für c: a ohne Koordinate k, mit a_z = +1 oder -1; passt keiner, volle Zentrumssuche.
 * Bauen: gcc -O2 -o build/p1_pruef14 src/p1_pruef14.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 14
static unsigned char hit[1 << N], inS[1 << 13], ballmask[1 << 13];

static int wt(unsigned v) { return __builtin_popcount(v); }

static int ist_kugel(unsigned c) {
    for (unsigned w = 0; w < (1u << 13); w++) if (ballmask[w ^ c] != inS[w]) return 0;
    return 1;
}

int main(void) {
    long long faelle = 0, abw = 0, groesse_abw = 0, kandidat_fehl = 0, zeilen = 0;
    int a[N];
    for (unsigned w = 0; w < (1u << 13); w++) { int d = wt(w); ballmask[w] = (d <= 3 || d >= 10); }
    for (int z = 0; z < N; z++) {
        for (int bits = 0; bits < (1 << (N - 2)); bits++) {
            int k0 = 0, first = 1;
            for (int j = 0; j < N; j++) {
                if (j == z) { a[j] = 0; continue; }
                if (first) { a[j] = 1; first = 0; continue; }
                a[j] = (bits >> k0 & 1) ? -1 : 1; k0++;
            }
            zeilen++;
            for (int x = 0; x < (1 << N); x++) {
                int s = 0;
                for (int j = 0; j < N; j++) s += (x >> j & 1) ? -a[j] : a[j];
                hit[x] = (s >= 7 || s <= -7);
            }
            for (int h = 0; h < 2; h++) {
                int klassen = 0;
                for (int x = 0; x < (1 << N); x++)
                    if ((wt(x) & 1) == h && hit[x] && x < (x ^ ((1 << N) - 1))) klassen++;
                if (klassen != 378) groesse_abw++;
                for (int k = 0; k < N; k++) {
                    memset(inS, 0, sizeof inS);
                    int npkt = 0;
                    for (int x = 0; x < (1 << N); x++) {
                        if ((wt(x) & 1) != h || !hit[x]) continue;
                        unsigned lo = x & ((1u << k) - 1), hi = (unsigned)x >> (k + 1);
                        unsigned w = lo | (hi << k);
                        if (!inS[w]) { inS[w] = 1; npkt++; }
                    }
                    faelle++;
                    if (npkt != 756) { abw++; continue; }
                    int ok = 0;
                    for (int sz = 0; sz < 2 && !ok; sz++) {
                        unsigned c = 0; int q = 0;
                        for (int j = 0; j < N; j++) if (j != k) {
                            int v = (j == z) ? (sz ? -1 : 1) : a[j];
                            if (v < 0) c |= 1u << q;
                            q++;
                        }
                        if (ist_kugel(c)) ok = 1;
                    }
                    if (!ok) {
                        kandidat_fehl++;
                        int ok2 = 0;
                        for (unsigned c = 0; c < (1u << 13) && !ok2; c++) if (inS[c] && ist_kugel(c)) ok2 = 1;
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
    return (abw || groesse_abw) ? 1 : 0;
}
