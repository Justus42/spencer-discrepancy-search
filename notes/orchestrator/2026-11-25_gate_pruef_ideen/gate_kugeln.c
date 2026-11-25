/* gate_kugeln.c -- Gate zu Auftrag P, Teil 5, Fall e = 0 (Orchestrator). Eigenes Programm.
 * Frage: Ueberdecken K Radius-R-Kugeln die 2^(m-1) Klassen {x, -x} von Q_m (gefalteter Abstand)?
 * Tiefensuche: verzweigt ueber die GROESSTE unueberdeckte Klasse (alle Mitten, die sie ueberdecken), ohne
 * Symmetriebruch an der Wurzel. Schranke: offen <= (Kugeln uebrig) * (groesster moeglicher Zuwachs einer Mitte jetzt);
 * gueltig, weil der Zuwachs jeder Mitte mit wachsender Ueberdeckung nur faellt.
 * Aufruf: gate_kugeln m R K [maxknoten]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define W 8
typedef struct { uint64_t w[W]; } bs;
static int m, R, K, NC, nw; static bs ball[512]; static int covlist[512][512], ncov[512];
static long long nodes, maxnodes = 0; static int wahl[64];
static int pc(const bs *a) { int s = 0, i; for (i = 0; i < nw; i++) s += __builtin_popcountll(a->w[i]); return s; }
static int dfs(const bs *cov, int left, int depth) {
    int i, c, open, top = -1, best = 0;
    nodes++; if (maxnodes && nodes > maxnodes) return -1;
    open = NC - pc(cov); if (!open) return 1; if (!left) return 0;
    for (c = 0; c < NC; c++) { int g = 0; for (i = 0; i < nw; i++) g += __builtin_popcountll(ball[c].w[i] & ~cov->w[i]); if (g > best) best = g; }
    if ((long long)open > (long long)left * best) return 0;
    for (c = NC - 1; c >= 0; c--) if (!((cov->w[c >> 6] >> (c & 63)) & 1)) { top = c; break; }
    for (i = 0; i < ncov[top]; i++) {
        bs n2 = *cov; int j, s = covlist[top][i], r;
        for (j = 0; j < nw; j++) n2.w[j] |= ball[s].w[j];
        wahl[depth] = s; r = dfs(&n2, left - 1, depth + 1); if (r) return r;
    }
    return 0;
}
int main(int argc, char **argv) {
    int a, b, d, full; m = atoi(argv[1]); R = atoi(argv[2]); K = atoi(argv[3]); if (argc > 4) maxnodes = atoll(argv[4]);
    NC = 1 << (m - 1); nw = (NC + 63) / 64; full = (1 << m) - 1;
    for (a = 0; a < NC; a++) { memset(&ball[a], 0, sizeof(bs));
        for (b = 0; b < NC; b++) { d = __builtin_popcount(a ^ b); if (d <= R || __builtin_popcount(a ^ b ^ full) <= R) { ball[a].w[b >> 6] |= 1ULL << (b & 63); covlist[b][ncov[b]++] = a; } } }
    bs z; memset(&z, 0, sizeof z); int r = dfs(&z, K, 0);
    printf("m = %d, R = %d, K = %d: Klassen %d, Kugelgroesse %d, Knoten %lld: %s", m, R, K, NC, pc(&ball[0]), nodes,
           r == 1 ? "UEBERDECKUNG GEFUNDEN, Mitten:" : (r == 0 ? "keine Ueberdeckung (vollstaendig)" : "Knotengrenze, unentschieden"));
    if (r == 1) { bs u; memset(&u, 0, sizeof u); for (a = 0; a < K && a < 64; a++) { printf(" %d", wahl[a]); for (b = 0; b < nw; b++) u.w[b] |= ball[wahl[a]].w[b]; } printf(" (nachgezaehlt: %d Klassen)", pc(&u)); }
    printf("\n"); return 0;
}
