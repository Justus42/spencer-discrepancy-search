/* Auftrag A, Teil 2/3: vollständige Suche nach Überdeckungen durch Zeilen mit Einträgen in {-1,0,1}.
 * SUCHCODE, prüft nichts. Treffer werden mit verifiers/pruefe_matrix.py nachgerechnet.
 *
 * Punkte: x in {+-1}^n mit x_0 = +1 (Klassen modulo Vorzeichen), P = 2^(n-1).
 * Zeile a trifft x, wenn |<a,x>| >= d. Gesucht: K Zeilen, die alle P Klassen treffen.
 * Zugelassene Träger: Liste -s (z. B. "10" oder "10,8,6"). Zeilen modulo Vorzeichen.
 *
 * Verlust einer Zeile beim Hinzufügen: (maxgr - |H|) + |H geschnitten überdeckt|.
 * Summe der Verluste <= B = K*maxgr - P (Budget). Schranke: Summe der r kleinsten aktuellen Verluste <= Restbudget.
 * Verzweigung an dem unüberdeckten Punkt mit den wenigsten lebenden Zeilen; Geschwister-Ausschluss
 * (Zweig i verbietet die Zeilen 1..i-1), so wird jede Zeilenmenge höchstens einmal besucht.
 *
 * Symmetrie (-y): Zeilen mit Träger n-1 ("Typzeilen"). Paartyp = (gleiche Nullspalte?, Abstand modulo Komplement).
 * Typen sind nach Schnittgröße geordnet. Für jeden Typ t: erste Zeile a = (0,1,...,1), zweite = Vertreter von t,
 * alle Typzeilen-Paare haben Typ <= t. Vollständig, sobald mindestens zwei Typzeilen in jeder Lösung stehen
 * (wird geprüft: K*maxgr - P < (K-1)*(maxgr - zweitgr) heißt mindestens 2 Typzeilen). Typ t erzwingt: t ist das Maximum.
 *
 * Parallel: alle Fäden gehen den flachen Baum bis Tiefe -t gleich ab, Knoten in Tiefe -t sind Aufgaben (Ticket).
 * Fortsetzung: erledigte Aufgaben stehen in der Datei -r (eine Nummer je Zeile), werden übersprungen.
 *
 * Aufruf: einzelnull_such -n 11 -d 6 -K 11 -s 10 [-y] [-T typ|-1] [-t Teiltiefe] [-z Sekunden] [-r erledigt.txt] [-a alle]
 *   -a: alle Lösungen zählen (sonst Stopp bei der ersten). -o Datei: Lösungen als Matrizen anhängen.
 * Bauen: gcc -O3 -march=native -fopenmp -o build/einzelnull_such src/einzelnull_such.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <omp.h>

#define MAXN 16
#define MAXW 512
#define MAXK 24

static int n, d, K, W, P;
static int sup_ok[MAXN + 1];
static int nrows, ntyp;           /* Zeilen 0..ntyp-1 haben Träger n-1 */
static int8_t (*ent)[MAXN];
static uint64_t *Hb;              /* nrows * W */
static int *gr;                   /* Treffergröße */
static int maxgr;
static uint8_t *ptyp;             /* ntyp*ntyp Paartyp-Index (Rang) */
static int ntypes;
static int typ_same[64], typ_d[64], typ_inter[64];
static int *ptlist;               /* je Punkt: Zeilen, die ihn treffen */
static int *ptstart;

static double t0, zeitlimit = 1e18;
static volatile int stopflag = 0, gefunden_stop = 0;
static int alle = 0;
static int teiltiefe = 4;
static long long next_ticket;     /* atomar */
static char *erledigt = NULL;     /* Bitfeld der erledigten Aufgaben */
static long long erl_max = 0;
static FILE *erl_f = NULL, *out_f = NULL;
static long long n_loes = 0, n_loes_verteilt = 0;
static long long knoten_tiefe[MAXK + 1];
static int max_tiefe = 0;
static long long aufg_gesamt = -1, aufg_erledigt_jetzt = 0;

static double jetzt(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + 1e-9 * ts.tv_nsec; }
static inline int pc(uint64_t v) { return __builtin_popcountll(v); }

static void zeilen_bauen(void) {
    int cap = 0;
    for (int s = n; s >= 1; s--) if (sup_ok[s]) {
        /* C(n,s) * 2^(s-1) */
        long long c = 1; for (int i = 0; i < s; i++) c = c * (n - i) / (i + 1);
        cap += (int)(c << (s - 1));
    }
    ent = calloc(cap, sizeof *ent);
    nrows = 0; ntyp = 0;
    int order[MAXN + 1], no = 0;
    if (sup_ok[n - 1]) order[no++] = n - 1;
    for (int s = n; s >= 1; s--) if (sup_ok[s] && s != n - 1) order[no++] = s;
    for (int oi = 0; oi < no; oi++) {
        int s = order[oi];
        for (int m = 0; m < (1 << n); m++) {
            if (__builtin_popcount(m) != s) continue;
            int cols[MAXN], c = 0;
            for (int j = 0; j < n; j++) if (m >> j & 1) cols[c++] = j;
            for (int b = 0; b < (1 << (s - 1)); b++) {
                int8_t *e = ent[nrows];
                memset(e, 0, MAXN);
                e[cols[0]] = 1;
                for (int k = 1; k < s; k++) e[cols[k]] = (b >> (k - 1) & 1) ? -1 : 1;
                nrows++;
            }
        }
        if (s == n - 1) ntyp = nrows;
    }
    Hb = calloc((size_t)nrows * W, 8);
    gr = calloc(nrows, sizeof(int));
    maxgr = 0;
    for (int r = 0; r < nrows; r++) {
        for (int x = 0; x < P; x++) {
            int s = ent[r][0];
            for (int j = 1; j < n; j++) s += (x >> (j - 1) & 1) ? -ent[r][j] : ent[r][j];
            if (s >= d || s <= -d) { Hb[(size_t)r * W + (x >> 6)] |= 1ULL << (x & 63); gr[r]++; }
        }
        if (gr[r] > maxgr) maxgr = gr[r];
    }
    /* Punkt -> Zeilen */
    ptstart = calloc(P + 1, sizeof(int));
    for (int r = 0; r < nrows; r++) for (int x = 0; x < P; x++) if (Hb[(size_t)r * W + (x >> 6)] >> (x & 63) & 1) ptstart[x + 1]++;
    for (int x = 0; x < P; x++) ptstart[x + 1] += ptstart[x];
    ptlist = malloc(sizeof(int) * (ptstart[P] + 1));
    int *fill = calloc(P, sizeof(int));
    for (int r = 0; r < nrows; r++) for (int x = 0; x < P; x++) if (Hb[(size_t)r * W + (x >> 6)] >> (x & 63) & 1) ptlist[ptstart[x] + fill[x]++] = r;
    free(fill);
}

static int zero_col(int r) { for (int j = 0; j < n; j++) if (!ent[r][j]) return j; return -1; }

/* roher Typ: same*32 + dmin */
static int rohtyp(int r1, int r2) {
    int z1 = zero_col(r1), z2 = zero_col(r2), dist = 0, m = 0;
    for (int j = 0; j < n; j++) if (ent[r1][j] && ent[r2][j]) { m++; if (ent[r1][j] != ent[r2][j]) dist++; }
    int dm = dist < m - dist ? dist : m - dist;
    return (z1 == z2) * 32 + dm;
}

static int inter(int r1, int r2) { int s = 0; for (int w = 0; w < W; w++) s += pc(Hb[(size_t)r1 * W + w] & Hb[(size_t)r2 * W + w]); return s; }

static int roh2rang[64];

static void typen_bauen(void) {
    /* alle rohen Typen relativ zu Zeile 0 sammeln */
    int roh_inter[64]; for (int i = 0; i < 64; i++) roh_inter[i] = -1;
    for (int r = 1; r < ntyp; r++) {
        int t = rohtyp(0, r), s = inter(0, r);
        if (roh_inter[t] < 0) roh_inter[t] = s;
        else if (roh_inter[t] != s) { fprintf(stderr, "Typ nicht schnitt-invariant!\n"); exit(1); }
    }
    /* Rang: nach (Schnitt, same, d) */
    ntypes = 0;
    int lst[64];
    for (int t = 0; t < 64; t++) if (roh_inter[t] >= 0) lst[ntypes++] = t;
    for (int i = 0; i < ntypes; i++) for (int j = i + 1; j < ntypes; j++) {
        int a = lst[i], b = lst[j];
        long ka = (long)roh_inter[a] * 1000 + a, kb = (long)roh_inter[b] * 1000 + b;
        if (kb < ka) { lst[i] = b; lst[j] = a; }
    }
    for (int i = 0; i < 64; i++) roh2rang[i] = -1;
    for (int i = 0; i < ntypes; i++) { roh2rang[lst[i]] = i; typ_same[i] = lst[i] / 32; typ_d[i] = lst[i] % 32; typ_inter[i] = roh_inter[lst[i]]; }
    ptyp = malloc((size_t)ntyp * ntyp);
    for (int a = 0; a < ntyp; a++) for (int b = 0; b < ntyp; b++)
        ptyp[(size_t)a * ntyp + b] = (a == b) ? 255 : roh2rang[rohtyp(a, b)];
}

static int finde_zeile(const int8_t *e) {
    for (int r = 0; r < nrows; r++) if (!memcmp(ent[r], e, n)) return r;
    return -1;
}

/* ---------------- Suche ---------------- */
typedef struct {
    int *liste[MAXK + 1];
    int *verl[MAXK + 1];
    uint64_t cov[MAXK + 1][MAXW];
    int gew[MAXK + 1];
    int *cnt;
    char *excl;
    long long knoten[MAXK + 1];
    int maxt;
    long long split_idx;
    long long ticket;
    long long zaehl;
} Faden;

static int B_total;
static int typgrenze;  /* -1: keine */

static void loesung(int *gew, int k) {
    int verteilt = 0, z0 = -1;
    for (int i = 0; i < k; i++) { int z = zero_col(gew[i]); if (z0 < 0) z0 = z; else if (z != z0) verteilt = 1; }
    #pragma omp critical
    {
        n_loes++;
        if (verteilt) n_loes_verteilt++;
        if (out_f && (n_loes <= 100000)) {
            fprintf(out_f, "# Loesung %lld, %d Zeilen, Nullen %s\n", n_loes, k, verteilt ? "verteilt" : "in einer Spalte");
            for (int i = 0; i < k; i++) { for (int j = 0; j < n; j++) fprintf(out_f, "%2d ", ent[gew[i]][j]); fprintf(out_f, "\n"); }
            fflush(out_f);
        }
        if (!alle) gefunden_stop = 1;
    }
}

static int istyp(int r) { return r < ntyp; }

/* Kind-Liste bauen: aus Eltern-Liste L (lebend), ohne ausgeschlossene, ohne c, typverträglich mit c */
static void dfs(Faden *F, int k, int verlust, int *L, int nL, int tiefe_abs);

static void knoten(Faden *F, int k, int verlust, int *L, int nL) {
    if (stopflag || gefunden_stop) return;
    if ((++F->zaehl & 4095) == 0 && jetzt() - t0 > zeitlimit) stopflag = 1;
    int haupt = (k >= teiltiefe) || omp_get_thread_num() == 0;  /* flacher Baum: nur Faden 0 zaehlt */
    if (haupt) F->knoten[k]++;
    if (k > F->maxt) F->maxt = k;
    uint64_t *cov = F->cov[k];
    int u = 0; for (int w = 0; w < W; w++) u += pc(~cov[w]);
    if (u == 0) { if (haupt) loesung(F->gew, k); return; }
    if (k >= K) return;
    int r = K - k;
    int rest = B_total - verlust;
    if (rest < 0) return;
    /* Verluste und Filter */
    int *L2 = F->liste[k], *V2 = F->verl[k], n2 = 0;
    for (int i = 0; i < nL; i++) {
        int row = L[i];
        const uint64_t *h = Hb + (size_t)row * W;
        int s = 0; for (int w = 0; w < W; w++) s += pc(h[w] & cov[w]);
        if (s == gr[row]) continue;            /* nichts Neues */
        int l = maxgr - gr[row] + s;
        if (l > rest) continue;
        L2[n2] = row; V2[n2] = l; n2++;
    }
    /* Schranke: r kleinste Verluste (bzw. alle, wenn weniger), muss ueberdecken */
    {
        int need = r < n2 ? r : n2;
        if (need == 0) return;
        /* Teilauswahl der kleinsten: Zählen nach Wert (Verluste <= rest <= B_total) */
        static __thread int hist[4096];
        int maxv = rest;
        if (maxv < 4096) {
            memset(hist, 0, sizeof(int) * (maxv + 1));
            for (int i = 0; i < n2; i++) hist[V2[i]]++;
            long long sum = 0; int take = need;
            for (int v = 0; v <= maxv && take > 0; v++) { int t = hist[v] < take ? hist[v] : take; sum += (long long)t * v; take -= t; }
            /* gain gesamt = need*maxgr - sum muss >= u */
            if ((long long)need * maxgr - sum < u) return;
        }
    }
    /* Punktzählung */
    int *cnt = F->cnt;
    memset(cnt, 0, sizeof(int) * P);
    for (int i = 0; i < n2; i++) {
        const uint64_t *h = Hb + (size_t)L2[i] * W;
        for (int w = 0; w < W; w++) {
            uint64_t v = h[w] & ~cov[w];
            while (v) { int b = __builtin_ctzll(v); cnt[w * 64 + b]++; v &= v - 1; }
        }
    }
    int best = -1, bc = 1 << 30;
    for (int w = 0; w < W; w++) {
        uint64_t v = ~cov[w];
        while (v) { int b = __builtin_ctzll(v); int x = w * 64 + b; if (cnt[x] < bc) { bc = cnt[x]; best = x; } v &= v - 1; }
    }
    if (bc == 0) return;
    /* Kandidaten: Zeilen in L2, die best treffen, nach Verlust sortiert */
    int cand[8192], cv[8192], nc = 0;
    for (int i = 0; i < n2; i++) if (Hb[(size_t)L2[i] * W + (best >> 6)] >> (best & 63) & 1) { cand[nc] = L2[i]; cv[nc] = V2[i]; nc++; }
    /* Einfügesortierung nach Verlust (stabil) */
    for (int i = 1; i < nc; i++) { int a = cand[i], b = cv[i], j = i - 1; while (j >= 0 && cv[j] > b) { cand[j + 1] = cand[j]; cv[j + 1] = cv[j]; j--; } cand[j + 1] = a; cv[j + 1] = b; }
    char *excl = F->excl;
    int *Lk = malloc(sizeof(int) * (n2 + 1));
    for (int ci = 0; ci < nc; ci++) {
        if (stopflag || gefunden_stop) break;
        int c = cand[ci];
        /* Teilbaum-Aufgabe? */
        if (k + 1 == teiltiefe) {
            long long idx = F->split_idx++;
            if (idx != F->ticket) { excl[c] = 1; continue; }
            /* eigene Aufgabe */
            if (erledigt && idx < erl_max && (erledigt[idx >> 3] >> (idx & 7) & 1)) {
                #pragma omp atomic capture
                F->ticket = next_ticket++;
                excl[c] = 1; continue;
            }
        }
        int nk = 0;
        for (int i = 0; i < n2; i++) {
            int row = L2[i];
            if (excl[row] || row == c) continue;
            if (typgrenze >= 0 && istyp(c) && istyp(row) && ptyp[(size_t)c * ntyp + row] > typgrenze) continue;
            Lk[nk++] = row;
        }
        F->gew[k] = c;
        const uint64_t *h = Hb + (size_t)c * W;
        for (int w = 0; w < W; w++) F->cov[k + 1][w] = cov[w] | h[w];
        knoten(F, k + 1, verlust + cv[ci], Lk, nk);
        if (k + 1 == teiltiefe) {
            if (!stopflag && !gefunden_stop) {
                long long idx = F->ticket;
                #pragma omp critical
                {
                    aufg_erledigt_jetzt++;
                    if (erl_f) { fprintf(erl_f, "%lld\n", idx); fflush(erl_f); }
                }
            }
            #pragma omp atomic capture
            F->ticket = next_ticket++;
        }
        excl[c] = 1;
    }
    for (int ci = 0; ci < nc; ci++) excl[cand[ci]] = 0;
    free(Lk);
}

static void dfs(Faden *F, int k, int verlust, int *L, int nL, int tiefe_abs) { (void)tiefe_abs; knoten(F, k, verlust, L, nL); }

int main(int argc, char **argv) {
    n = 11; d = 6; K = 11;
    char sups[64] = "10";
    int sym = 0, nurtyp = -1000;
    const char *erl_name = NULL, *out_name = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-n")) n = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-d")) d = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-K")) K = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-s")) strncpy(sups, argv[++i], 63);
        else if (!strcmp(argv[i], "-y")) sym = 1;
        else if (!strcmp(argv[i], "-T")) nurtyp = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-t")) teiltiefe = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-z")) zeitlimit = atof(argv[++i]);
        else if (!strcmp(argv[i], "-r")) erl_name = argv[++i];
        else if (!strcmp(argv[i], "-o")) out_name = argv[++i];
        else if (!strcmp(argv[i], "-a")) alle = 1;
        else { fprintf(stderr, "unbekannt: %s\n", argv[i]); return 2; }
    }
    W = (1 << (n - 1)) / 64; if (W < 1) W = 1; P = 1 << (n - 1);
    if (W > MAXW || K > MAXK) { fprintf(stderr, "zu gross\n"); return 2; }
    for (char *p = strtok(sups, ","); p; p = strtok(NULL, ",")) sup_ok[atoi(p)] = 1;
    t0 = jetzt();
    zeilen_bauen();
    B_total = K * maxgr - P;
    printf("n=%d d=%d K=%d Traeger=%s: %d Zeilen (davon %d Typzeilen), %d Klassen, maxgr=%d, Budget B=%d\n",
           n, d, K, argv[0] ? sups : "", nrows, ntyp, P, maxgr, B_total);
    for (int s = n; s >= 1; s--) if (sup_ok[s]) {
        int c = 0, g = -1; for (int r = 0; r < nrows; r++) { int ss = 0; for (int j = 0; j < n; j++) ss += ent[r][j] != 0; if (ss == s) { c++; g = gr[r]; } }
        printf("  Traeger %d: %d Zeilen, je %d Klassen\n", s, c, g);
    }
    if (B_total < 0) { printf("Budget negativ: keine Loesung (Zaehlargument). Fertig, vollstaendig.\n"); return 0; }
    if (erl_name) {
        erl_max = 1LL << 32;
        erledigt = calloc(erl_max / 8, 1);
        FILE *f = fopen(erl_name, "r");
        long long v, c = 0;
        if (f) { while (fscanf(f, "%lld", &v) == 1) { if (v >= 0 && v < erl_max) { erledigt[v >> 3] |= 1 << (v & 7); c++; } } fclose(f); }
        printf("Fortsetzung: %lld erledigte Aufgaben gelesen aus %s\n", c, erl_name);
        erl_f = fopen(erl_name, "a");
    }
    if (out_name) out_f = fopen(out_name, "a");
    int nthr = omp_get_max_threads();
    int typ_von = -1, typ_bis = -1;
    int a_row = -1;
    if (sym) {
        if (!sup_ok[n - 1]) { fprintf(stderr, "-y braucht Traeger n-1\n"); return 2; }
        typen_bauen();
        /* Mindestens 2 Typzeilen in jeder Loesung? Zweitgroesste Groesse */
        int zweit = 0; for (int r = ntyp; r < nrows; r++) if (gr[r] > zweit) zweit = gr[r];
        /* mit hoechstens 1 Typzeile: Gesamtgroesse <= maxgr + (K-1)*zweit */
        if (maxgr + (K - 1) * zweit >= P) { fprintf(stderr, "Symmetrie nicht zulaessig: Loesung mit <= 1 Typzeile moeglich\n"); return 2; }
        printf("Typen (Rang: gleiche Spalte?, Abstand, Schnitt mit a):\n");
        for (int t = 0; t < ntypes; t++) printf("  %d: %s, d=%d, Schnitt %d\n", t, typ_same[t] ? "gleich" : "andere", typ_d[t], typ_inter[t]);
        int8_t e[MAXN]; memset(e, 0, MAXN); for (int j = 1; j < n; j++) e[j] = 1;
        a_row = finde_zeile(e);
        typ_von = 0; typ_bis = ntypes - 1;
        if (nurtyp >= 0) typ_von = typ_bis = nurtyp;
    }
    long long loes_vor = 0;
    for (int tt = typ_von; tt <= typ_bis; tt++) {
        double ts = jetzt();
        int start[2], nstart = 0;
        typgrenze = -1;
        if (sym) {
            int8_t e[MAXN]; memset(e, 0, MAXN);
            if (typ_same[tt]) { for (int j = 1; j < n; j++) e[j] = 1; for (int j = n - typ_d[tt]; j < n; j++) e[j] = -1; }
            else { e[0] = 1; e[1] = 0; for (int j = 2; j < n; j++) e[j] = 1; for (int j = n - typ_d[tt]; j < n; j++) e[j] = -1; }
            int rep = finde_zeile(e);
            if (rep < 0 || ptyp[(size_t)a_row * ntyp + rep] != tt) { fprintf(stderr, "Vertreter falsch\n"); return 1; }
            start[0] = a_row; start[1] = rep; nstart = 2;
            typgrenze = tt;
            printf("== Typ %d (%s, d=%d, Schnitt %d): a = Zeile %d, Vertreter Zeile %d\n", tt, typ_same[tt] ? "gleiche Spalte" : "andere Spalte", typ_d[tt], typ_inter[tt], a_row, rep);
        }
        /* Startliste */
        int *L0 = malloc(sizeof(int) * nrows), n0 = 0;
        for (int r = 0; r < nrows; r++) {
            int ok = 1;
            for (int s = 0; s < nstart; s++) {
                if (r == start[s]) ok = 0;
                else if (typgrenze >= 0 && istyp(r) && ptyp[(size_t)start[s] * ntyp + r] > typgrenze) ok = 0;
            }
            if (ok) L0[n0++] = r;
        }
        next_ticket = nthr;
        memset(knoten_tiefe, 0, sizeof knoten_tiefe);
        max_tiefe = 0;
        aufg_gesamt = -1; aufg_erledigt_jetzt = 0;
        long long split_max = 0;
        #pragma omp parallel
        {
            Faden *F = calloc(1, sizeof(Faden));
            for (int k = 0; k <= K; k++) { F->liste[k] = malloc(sizeof(int) * nrows); F->verl[k] = malloc(sizeof(int) * nrows); }
            F->cnt = malloc(sizeof(int) * P);
            F->excl = calloc(nrows, 1);
            F->ticket = omp_get_thread_num();
            memset(F->cov[0], 0, sizeof F->cov[0]);
            int verl = 0;
            for (int s = 0; s < nstart; s++) {
                const uint64_t *h = Hb + (size_t)start[s] * W;
                int sc = 0; for (int w = 0; w < W; w++) sc += pc(h[w] & F->cov[0][w]);
                verl += maxgr - gr[start[s]] + sc;
                for (int w = 0; w < W; w++) F->cov[0][w] |= h[w];
                F->gew[s] = start[s];
            }
            /* ungenutzte Bits (P < 64) als ueberdeckt markieren */
            if (P < 64) F->cov[0][0] |= ~0ULL << P;
            if (nstart) memcpy(F->cov[nstart], F->cov[0], sizeof F->cov[0]);
            /* Hilfsfeld fuer Tiefen ab nstart: knoten() nutzt cov[k] */
            #pragma omp barrier
            /* bei Teilung muss teiltiefe > nstart sein */
            knoten(F, nstart, verl, L0, n0);
            #pragma omp critical
            {
                for (int k = 0; k <= K; k++) knoten_tiefe[k] += F->knoten[k];
                if (F->maxt > max_tiefe) max_tiefe = F->maxt;
                if (F->split_idx > split_max) split_max = F->split_idx;
            }
        }
        if (zeitlimit < 1e17 && jetzt() - t0 > zeitlimit) stopflag = 1;
        printf("   Aufgaben (Tiefe %d): %lld gesamt, %lld in diesem Lauf erledigt; Loesungen bisher %lld (verteilt %lld)%s\n",
               teiltiefe, split_max, aufg_erledigt_jetzt, n_loes, n_loes_verteilt, stopflag ? " ABBRUCH Zeit" : "");
        printf("   Knoten je Tiefe:");
        for (int k = 0; k <= K; k++) if (knoten_tiefe[k]) printf(" %d:%lld", k, knoten_tiefe[k]);
        printf("; max. Tiefe %d; Zeit %.1f s\n", max_tiefe, jetzt() - ts);
        printf("   Typ %d: %s, Loesungen %lld\n", tt, stopflag ? "NICHT fertig" : (gefunden_stop ? "Loesung gefunden (Stopp)" : "fertig"), n_loes - loes_vor);
        fflush(stdout);
        loes_vor = n_loes;
        free(L0);
        if (stopflag || gefunden_stop) break;
        if (!sym) break;
    }
    printf("ENDE: Loesungen %lld (Nullen verteilt: %lld), %s, Gesamtzeit %.1f s\n", n_loes, n_loes_verteilt,
           stopflag ? "ABBRUCH (Zeit)" : (gefunden_stop ? "Stopp bei erster Loesung" : "vollstaendig"), jetzt() - t0);
    return 0;
}
