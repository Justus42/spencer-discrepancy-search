/* gate_kern.c -- Gate zu Auftrag P, Teil 1 (Orchestrator, 25.11.2026). Eigener Pruefer, kein Code aus der Sitzung.
 * Prueft Zertifikate "IDX a b .. | LOCHZAHL | KERN | h1 h2 ..":
 *  (1) jede Klasse h ist ein Loch der behaltenen Zeilen (|<r,x>| <= 5),
 *  (2) Lochzahl neu gezaehlt,
 *  (3) die Kernklassen lassen sich nicht auf k Zeilen verteilen.
 * Verfahren fuer (3), anders als in der Sitzung: nur die Zeilenzuordnung wird aufgezaehlt (keine Vorzeichen);
 * je Zeile wird geprueft, ob die Bedingungen "gleich" (D <= 6) / "verschieden" (D >= 9) zweifaerbbar sind
 * (Breitensuche, jedes Mal neu). Symmetriebruch: eine neue Zeile wird nur als naechste freie eroeffnet.
 * Modus 0: Zertifikate pruefen. Modus 1: je Zertifikat jede Klasse einmal weglassen, zaehlen wie oft verteilbar.
 * Modus 2: Positivkontrolle: je Paar gestrichener Zeilen 24 Loecher (gleichmaessig gewaehlt), k = 2, muss verteilbar sein.
 * Aufruf: gate_kern MATRIX k MODUS [ZERT]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 15
static int rows[64], nrows;
static int cls[64], n, K;
static signed char rel[64][64]; /* 0 frei, 1 gleich, 2 verschieden */
static int rowof[64];
static long long nodes;

static int zweifaerbbar(int j, int upto) {
    int col[64], q[64], i, a, b, qh, qt;
    for (i = 0; i <= upto; i++) col[i] = -1;
    for (i = 0; i <= upto; i++) {
        if (rowof[i] != j || col[i] >= 0) continue;
        col[i] = 0; qh = qt = 0; q[qt++] = i;
        while (qh < qt) {
            a = q[qh++];
            for (b = 0; b <= upto; b++) {
                if (rowof[b] != j || b == a || !rel[a][b]) continue;
                int want = rel[a][b] == 1 ? col[a] : 1 - col[a];
                if (col[b] < 0) { col[b] = want; q[qt++] = b; }
                else if (col[b] != want) return 0;
            }
        }
    }
    return 1;
}
static int dfs(int t, int used) {
    int j, lim = used < K ? used + 1 : K;
    nodes++;
    if (t == n) return 1;
    for (j = 0; j < lim; j++) {
        rowof[t] = j;
        if (zweifaerbbar(j, t) && dfs(t + 1, j == used ? used + 1 : used)) return 1;
    }
    rowof[t] = -1;
    return 0;
}
static int verteilbar(void) {
    int a, b, d;
    for (a = 0; a < n; a++) for (b = 0; b < n; b++) {
        d = __builtin_popcount(cls[a] ^ cls[b]);
        rel[a][b] = (a == b) ? 0 : (d <= 6 ? 1 : (d >= 9 ? 2 : 0));
    }
    for (a = 0; a < n; a++) rowof[a] = -1;
    return dfs(0, 0);
}
static int istloch(int x, const int *del, int nd) {
    int i, j, skip, s;
    for (i = 0; i < nrows; i++) {
        skip = 0; for (j = 0; j < nd; j++) if (del[j] == i) skip = 1;
        if (skip) continue;
        s = M - 2 * __builtin_popcount(x ^ rows[i]);
        if (s > 5 || s < -5) return 0;
    }
    return 1;
}
int main(int argc, char **argv) {
    if (argc < 4) return 2;
    FILE *f = fopen(argv[1], "r"); char line[1 << 16]; int v, j;
    K = atoi(argv[2]); int modus = atoi(argv[3]);
    while (fgets(line, sizeof line, f)) {
        char *p = line; int w = 0, cnt = 0, off;
        while (cnt < M && sscanf(p, "%d%n", &v, &off) == 1) { if (v == -1) w |= 1 << cnt; else if (v != 1) return 3; cnt++; p += off; }
        if (cnt == M) rows[nrows++] = w;
    }
    fclose(f);
    if (nrows != 16) { printf("FEHLER: %d Zeilen\n", nrows); return 3; }
    if (modus == 2) {
        int a, b, ok = 0, tot = 0, hist[4096] = {0};
        for (a = 0; a < 16; a++) for (b = a + 1; b < 16; b++) {
            int del[2] = {a, b}, x, L = 0; static int holes[1 << 14];
            for (x = 0; x < (1 << 14); x++) if (istloch(x, del, 2)) holes[L++] = x;
            hist[L]++; n = 24; for (j = 0; j < n; j++) cls[j] = holes[(long long)j * L / n];
            nodes = 0; tot++; if (verteilbar()) ok++;
        }
        for (j = 0; j < 4096; j++) if (hist[j]) printf("  Lochzahl %d: %d Paare\n", j, hist[j]);
        printf("Positivkontrolle (2 gestrichen, k=%d, 24 Loecher je Paar): verteilbar %d von %d\n", K, ok, tot);
        return ok == tot ? 0 : 1;
    }
    f = fopen(argv[4], "r");
    int zeilen = 0, bestaetigt = 0, lochfehler = 0, zahlfehler = 0, nichtkern = 0;
    long long maxnodes = 0, sumnodes = 0, drop_sat = 0, drop_tot = 0;
    static int histL[4096], histK[64];
    while (fgets(line, sizeof line, f)) {
        int del[8], nd = 0, L, s, idx, off; char *p = line;
        if (strstr(line, "VERTEILBAR")) { printf("  Zeile mit VERTEILBAR: %s", line); zeilen++; continue; }
        if (sscanf(p, "%d%n", &idx, &off) != 1) continue; p += off;
        while (sscanf(p, "%d%n", &v, &off) == 1) { del[nd++] = v; p += off; }
        p = strchr(p, '|'); if (!p) { printf("  unlesbar: %s", line); zeilen++; continue; } p++;
        sscanf(p, "%d%n", &L, &off); p = strchr(p, '|') + 1;
        sscanf(p, "%d%n", &s, &off); p = strchr(p, '|') + 1;
        n = 0; while (sscanf(p, "%d%n", &v, &off) == 1) { cls[n++] = v; p += off; }
        zeilen++;
        int gut = 1, x, Lneu = 0;
        if (n != s) gut = 0;
        for (j = 0; j < n; j++) if (cls[j] < 0 || cls[j] >= (1 << 14) || !istloch(cls[j], del, nd)) { gut = 0; lochfehler++; }
        for (x = 0; x < (1 << 14); x++) if (istloch(x, del, nd)) Lneu++;
        if (Lneu != L) { zahlfehler++; gut = 0; }
        histL[Lneu]++; histK[n]++;
        if (modus == 0) {
            nodes = 0; if (verteilbar()) { nichtkern++; gut = 0; }
            if (nodes > maxnodes) maxnodes = nodes; sumnodes += nodes;
            if (gut) bestaetigt++;
        } else {
            int full[64], nn = n, o, i2;
            memcpy(full, cls, sizeof(int) * n);
            for (o = 0; o < nn; o++) { n = 0; for (i2 = 0; i2 < nn; i2++) if (i2 != o) cls[n++] = full[i2];
                drop_tot++; if (verteilbar()) drop_sat++; }
        }
    }
    printf("%s: k = %d, Zeilen %d\n", argv[4], K, zeilen);
    for (j = 0; j < 4096; j++) if (histL[j]) printf("  Lochzahl %d: %d Faelle\n", j, histL[j]);
    printf("  Kerngroessen:"); for (j = 0; j < 64; j++) if (histK[j]) printf(" %d:%d", j, histK[j]); printf("\n");
    if (modus == 0) printf("  bestaetigt %d von %d (Lochfehler %d, Lochzahl abweichend %d, verteilbar %d); Knoten gesamt %lld, groesster Fall %lld\n",
                           bestaetigt, zeilen, lochfehler, zahlfehler, nichtkern, sumnodes, maxnodes);
    else printf("  Kontrolle (eine Klasse weggelassen): verteilbar %lld von %lld\n", drop_sat, drop_tot);
    return (modus == 0 && bestaetigt != zeilen) ? 1 : 0;
}
