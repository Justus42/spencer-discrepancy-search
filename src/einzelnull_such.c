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
static long long aufg_gesamt = -1, aufg_erledigt_jetzt = 0, erl_vorher = 0;
static int zaehlmodus = 0;
static int prof_q = -1, prof_s = -1;   /* genaue Zahl der Zeilen mit Traeger n-3 (q) und n-5 (s); -1: aus */
static int8_t *klasse;                 /* 0: Traeger n-1, 1: n-3, 2: n-5, 3: sonst */
static long long id_basis = 0;
static int schranke2 = 0;      /* 0 aus, sonst max. Zahl der Punkte */
static long long maxaufg = -1; /* nur Aufgaben < maxaufg (Messung) */
static double letzte_meldung = 0;

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
    int *cnt, *ml;
    char *excl;
    long long knoten[MAXK + 1];
    int maxt;
    int u8[MAXK + 2], u6[MAXK + 2];
    long long split_idx;
    long long ticket;
    long long zaehl;
} Faden;

static int B_total;
static int typgrenze;  /* -1: keine */

static void loesung(int *gew, int k) {
    if (zaehlmodus) return;
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
    int pref[MAXK + 2];
    /* Verluste und Filter */
    int *L2 = F->liste[k], *V2 = F->verl[k], n2 = 0;
    for (int i = 0; i < nL; i++) {
        int row = L[i];
        const uint64_t *h = Hb + (size_t)row * W;
        int s = 0; for (int w = 0; w < W; w++) s += pc(h[w] & cov[w]);
        if (s == gr[row]) continue;            /* nichts Neues */
        if (prof_q >= 0) {
            int kl = klasse[row];
            if (kl == 1 && F->u8[k] >= prof_q) continue;
            if (kl == 2 && F->u6[k] >= prof_s) continue;
            if (kl == 0 && r - (prof_q - F->u8[k]) - (prof_s - F->u6[k]) < 1) continue;
            if (kl == 3) continue;
        }
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
            int pi = 0; pref[0] = 0;
            for (int v = 0; v <= maxv && take > 0; v++) { int t = hist[v] < take ? hist[v] : take; for (int q = 0; q < t; q++) { pi++; pref[pi] = pref[pi - 1] + v; } sum += (long long)t * v; take -= t; }
            /* gain gesamt = need*maxgr - sum muss >= u */
            if ((long long)need * maxgr - sum < u) return;
        }
        if (prof_q >= 0) {
            /* genaue Profilschranke: restliche kleine Zeilen muessen kommen, je Klasse die kleinsten Verluste */
            int rq = prof_q - F->u8[k], rs6 = prof_s - F->u6[k], r10 = r - rq - rs6;
            if (r10 < 0) return;
            static __thread int hk[3][4096];
            for (int c3 = 0; c3 < 3; c3++) memset(hk[c3], 0, sizeof(int) * (rest + 1));
            int nk[3] = {0, 0, 0};
            for (int i = 0; i < n2; i++) { int kl = klasse[L2[i]]; if (kl < 3) { hk[kl][V2[i]]++; nk[kl]++; } }
            if (nk[1] < rq || nk[2] < rs6) return;
            int want[3] = {r10 < nk[0] ? r10 : nk[0], rq, rs6};
            long long lb = 0;
            for (int c3 = 0; c3 < 3; c3++) { int take = want[c3]; for (int v = 0; v <= rest && take > 0; v++) { int t = hk[c3][v] < take ? hk[c3][v] : take; lb += (long long)t * v; take -= t; } }
            /* Abdeckung: (want0+rq+rs6)*maxgr - lb >= u */
            if ((long long)(want[0] + rq + rs6) * maxgr - lb < u) return;
        }
    }
    /* Punktzählung */
    int *cnt = F->cnt, *ml = F->ml;
    memset(cnt, 0, sizeof(int) * P);
    for (int i = 0; i < P; i++) ml[i] = 1 << 29;
    for (int i = 0; i < n2; i++) {
        const uint64_t *h = Hb + (size_t)L2[i] * W;
        int l = V2[i];
        for (int w = 0; w < W; w++) {
            uint64_t v = h[w] & ~cov[w];
            while (v) { int b = __builtin_ctzll(v); int x = w * 64 + b; cnt[x]++; if (l < ml[x]) ml[x] = l; v &= v - 1; }
        }
    }
    /* Schranke 2: Punkte ohne gemeinsame lebende Zeile brauchen verschiedene Zeilen */
    if (schranke2 && rest < 4096) {
        uint64_t M[MAXW];
        for (int w = 0; w < W; w++) M[w] = cov[w];
        int nmax = r < n2 ? r : n2;   /* pref[0..nmax] gueltig */
        long long summe = 0;
        for (int j = 1; ; j++) {
            int bx = -1, bm = -1;
            for (int w = 0; w < W; w++) { uint64_t v = ~M[w]; while (v) { int b = __builtin_ctzll(v); int x = w * 64 + b; if (ml[x] > bm) { bm = ml[x]; bx = x; } v &= v - 1; } }
            if (bx < 0) break;
            if (bm >= (1 << 29)) return;          /* Punkt ohne lebende Zeile */
            if (j > r) return;                    /* mehr als r Zeilen noetig */
            summe += bm;
            int rr = r - j; if (rr > nmax) rr = nmax;
            if (summe + pref[rr] > rest) return;
            for (int i = 0; i < n2; i++) {
                const uint64_t *h = Hb + (size_t)L2[i] * W;
                if (h[bx >> 6] >> (bx & 63) & 1) for (int w = 0; w < W; w++) M[w] |= h[w];
            }
            if (j >= schranke2) break;
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
        if (k + 1 == teiltiefe && zaehlmodus) { F->split_idx++; excl[c] = 1; continue; }
        if (k + 1 == teiltiefe) {
            long long idx = F->split_idx++;
            if (idx != F->ticket || (maxaufg >= 0 && idx >= maxaufg)) { excl[c] = 1; continue; }
            /* eigene Aufgabe */
            long long gid = id_basis + idx;
            if (erledigt && gid < erl_max && (erledigt[gid >> 3] >> (gid & 7) & 1)) {
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
        F->u8[k + 1] = F->u8[k] + (prof_q >= 0 && klasse[c] == 1);
        F->u6[k + 1] = F->u6[k] + (prof_q >= 0 && klasse[c] == 2);
        const uint64_t *h = Hb + (size_t)c * W;
        for (int w = 0; w < W; w++) F->cov[k + 1][w] = cov[w] | h[w];
        knoten(F, k + 1, verlust + cv[ci], Lk, nk);
        if (k + 1 == teiltiefe) {
            if (!stopflag && !gefunden_stop) {
                long long idx = id_basis + F->ticket;
                #pragma omp critical
                {
                    aufg_erledigt_jetzt++;
                    if (erl_f) { fprintf(erl_f, "%lld\n", idx); fflush(erl_f); }
                    double tn = jetzt();
                    if (tn - letzte_meldung > 60) {
                        letzte_meldung = tn;
                        printf("   [%.0f s] Aufgaben erledigt (dieser Lauf) %lld, insgesamt %lld von %lld, zuletzt Nr. %lld\n",
                               tn - t0, aufg_erledigt_jetzt, aufg_erledigt_jetzt + erl_vorher, aufg_gesamt, idx);
                        fflush(stdout);
                    }
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

static int dritte = 0, bahn_von = -1, bahn_bis = -1;
static int profgrenze = 1 << 30, prof_a = -1, prof_b = -1;

static int profil(int a, int b, int r) {
    int x = ptyp[(size_t)a * ntyp + r], y = ptyp[(size_t)b * ntyp + r];
    return x > y ? (x << 6) | y : (y << 6) | x;
}

static int uf_find(int *p, int x) { while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; } return x; }

/* Elemente der vorzeichenbehafteten Permutationsgruppe, die {a,b} (modulo Vorzeichen) festlassen.
 * (gR)_j = s_j R_{pi(j)}. Rekursiv; jedes gefundene Element wird sofort auf alle Kandidaten angewandt (Union-Find),
 * hoechstens maxel Elemente (Teilmenge der Gruppe gibt feinere Bahnen, also immer noch vollstaendig). */
static int g_pi[MAXN], g_s[MAXN], g_used[MAXN];
static long long g_cnt;
static int *g_uf, *g_kand_idx, g_maxel = 200000;
static const int8_t *g_a, *g_b, *g_ta, *g_tb;
static int g_e1, g_e2;
static int8_t (*g_tmp);

static int g_tmp_index(int z, int bits);
static void g_anwenden(void) {
    g_cnt++;
    /* Kontrolle: g bildet {a,b} modulo Vorzeichen auf sich ab */
    for (int q = 0; q < 2; q++) {
        const int8_t *src = q ? g_b : g_a;
        int8_t e[MAXN]; for (int j = 0; j < n; j++) e[j] = g_s[j] * src[g_pi[j]];
        int ok = 0;
        for (int t2 = 0; t2 < 2 && !ok; t2++) for (int sg = -1; sg <= 1 && !ok; sg += 2) {
            const int8_t *tg = t2 ? g_b : g_a; int gl = 1;
            for (int j = 0; j < n; j++) if (e[j] != sg * tg[j]) { gl = 0; break; }
            if (gl) ok = 1;
        }
        if (!ok) { fprintf(stderr, "Gruppenelement falsch!\n"); exit(1); }
    }
    for (int r = 0; r < ntyp; r++) {
        if (g_kand_idx[r] < 0) continue;
        int8_t e[MAXN]; memset(e, 0, MAXN);
        for (int j = 0; j < n; j++) e[j] = g_s[j] * ent[r][g_pi[j]];
        int s0 = 0; for (int j = 0; j < n; j++) if (e[j]) { s0 = e[j]; break; }
        for (int j = 0; j < n; j++) e[j] *= s0;
        /* Index finden: Typzeilen nach (Nullspalte, Bits) adressiert */
        int z = -1; for (int j = 0; j < n; j++) if (!e[j]) { z = j; break; }
        int bits = 0, k = 0, first = 1;
        for (int j = 0; j < n; j++) if (j != z) { if (first) { first = 0; continue; } if (e[j] < 0) bits |= 1 << k; k++; }
        int img = g_tmp_index(z, bits);
        if (g_kand_idx[img] < 0) { fprintf(stderr, "Bild kein Kandidat!\n"); exit(1); }
        int x = uf_find(g_uf, g_kand_idx[r]), y = uf_find(g_uf, g_kand_idx[img]);
        if (x != y) g_uf[x > y ? x : y] = x < y ? x : y;
    }
}

static void g_rek(int j) {
    if (g_cnt >= g_maxel) return;
    if (j == n) { g_anwenden(); return; }
    for (int q = 0; q < n; q++) {
        if (g_used[q]) continue;
        for (int sj = -1; sj <= 1; sj += 2) {
            /* (g a)_j = sj * a_q muss e1 * ta_j sein; (g b)_j = sj * b_q muss e2 * tb_j sein */
            if (sj * g_a[q] != g_e1 * g_ta[j]) continue;
            if (sj * g_b[q] != g_e2 * g_tb[j]) continue;
            g_pi[j] = q; g_s[j] = sj; g_used[q] = 1;
            g_rek(j + 1);
            g_used[q] = 0;
            if (g_cnt >= g_maxel) return;
        }
    }
}

/* Typzeile aus (Nullspalte z, Bits der Eintraege nach dem ersten) */
static int *g_index_tab;
static int g_tmp_index(int z, int bits) { return g_index_tab[z * (1 << (n - 2)) + bits]; }

static int *dritte_bahnen(int a, int b, int t, int *nb) {
    if (!g_index_tab) {
        g_index_tab = malloc(sizeof(int) * n * (1 << (n - 2)));
        for (int r = 0; r < ntyp; r++) {
            int z = zero_col(r), bits = 0, k = 0, first = 1;
            for (int j = 0; j < n; j++) if (j != z) { if (first) { first = 0; continue; } if (ent[r][j] < 0) bits |= 1 << k; k++; }
            g_index_tab[z * (1 << (n - 2)) + bits] = r;
        }
    }
    g_kand_idx = malloc(sizeof(int) * ntyp);
    int nk = 0;
    for (int r = 0; r < ntyp; r++) {
        g_kand_idx[r] = -1;
        if (r == a || r == b) continue;
        if (ptyp[(size_t)a * ntyp + r] > t || ptyp[(size_t)b * ntyp + r] > t) continue;
        g_kand_idx[r] = nk++;
    }
    g_uf = malloc(sizeof(int) * nk);
    for (int i = 0; i < nk; i++) g_uf[i] = i;
    long long ges = 0;
    for (int sw = 0; sw < 2; sw++) for (int e2 = -1; e2 <= 1; e2 += 2) {
        g_a = ent[a]; g_b = ent[b];
        g_ta = sw ? ent[b] : ent[a]; g_tb = sw ? ent[a] : ent[b];
        g_e1 = 1; g_e2 = e2;
        memset(g_used, 0, sizeof g_used);
        g_cnt = 0;
        g_rek(0);
        ges += g_cnt;
    }
    int *rep = malloc(sizeof(int) * nk), nr = 0;
    int *inv = malloc(sizeof(int) * nk);
    for (int r = 0; r < ntyp; r++) if (g_kand_idx[r] >= 0) inv[g_kand_idx[r]] = r;
    /* Profil muss in der Bahn konstant sein (Kontrolle) */
    for (int i = 0; i < nk; i++) {
        int root = uf_find(g_uf, i);
        if (profil(a, b, inv[i]) != profil(a, b, inv[root])) { fprintf(stderr, "Profil nicht bahninvariant!\n"); exit(1); }
        if (root == i) rep[nr++] = inv[i];
    }
    /* nach Profil absteigend sortieren (grosse Profile zuerst? nein: kleine zuerst, die sind schnell) */
    for (int i = 1; i < nr; i++) { int x = rep[i], px = profil(a, b, x), j = i - 1; while (j >= 0 && profil(a, b, rep[j]) > px) { rep[j + 1] = rep[j]; j--; } rep[j + 1] = x; }
    printf("   Stabilisator {a,b}: %lld Elemente angewandt (Grenze %d je Fall), %d Kandidaten, %d Bahnen\n", ges, g_maxel, nk, nr);
    free(inv); free(g_uf); free(g_kand_idx);
    *nb = nr;
    return rep;
}

static void lauf_start(int *start, int nstart) {
    int nthr = omp_get_max_threads();
    int *L0 = malloc(sizeof(int) * nrows), n0 = 0;
    for (int r = 0; r < nrows; r++) {
        int ok = 1;
        for (int s = 0; s < nstart; s++) {
            if (r == start[s]) ok = 0;
            else if (typgrenze >= 0 && istyp(r) && ptyp[(size_t)start[s] * ntyp + r] > typgrenze) ok = 0;
        }
        if (ok && istyp(r) && profgrenze < (1 << 30) && profil(prof_a, prof_b, r) > profgrenze) ok = 0;
        if (ok) L0[n0++] = r;
    }
    next_ticket = nthr;
    aufg_gesamt = -1; aufg_erledigt_jetzt = 0;
    {
        Faden *F = calloc(1, sizeof(Faden));
        for (int k = 0; k <= K; k++) { F->liste[k] = malloc(sizeof(int) * nrows); F->verl[k] = malloc(sizeof(int) * nrows); }
        F->cnt = malloc(sizeof(int) * P); F->ml = malloc(sizeof(int) * P);
        F->excl = calloc(nrows, 1);
        int verl = 0;
        for (int s = 0; s < nstart; s++) {
            const uint64_t *h = Hb + (size_t)start[s] * W;
            int sc = 0; for (int w = 0; w < W; w++) sc += pc(h[w] & F->cov[0][w]);
            verl += maxgr - gr[start[s]] + sc;
            for (int w = 0; w < W; w++) F->cov[0][w] |= h[w];
            F->gew[s] = start[s];
        }
        if (P < 64) F->cov[0][0] |= ~0ULL << P;
        if (nstart) memcpy(F->cov[nstart], F->cov[0], sizeof F->cov[0]);
        double zz = zeitlimit; zeitlimit = 1e18; zaehlmodus = 1;
        int aa = alle; alle = 1;
        long long nl = n_loes, nv = n_loes_verteilt;
        knoten(F, nstart, verl, L0, n0);
        n_loes = nl; n_loes_verteilt = nv; alle = aa;
        zaehlmodus = 0; zeitlimit = zz;
        aufg_gesamt = F->split_idx;
        if (aufg_gesamt >= (1LL << 24)) { fprintf(stderr, "zu viele Aufgaben je Bahn (%lld)\n", aufg_gesamt); exit(1); }
        erl_vorher = 0;
        if (erledigt) for (long long i = 0; i < aufg_gesamt; i++) { long long id = id_basis + i; if (erledigt[id >> 3] >> (id & 7) & 1) erl_vorher++; }
        if (!dritte) { printf("   Zaehlvorlauf: %lld Aufgaben in Tiefe %d, davon %lld schon erledigt\n", aufg_gesamt, teiltiefe, erl_vorher); fflush(stdout); }
        for (int k = 0; k <= K; k++) { free(F->liste[k]); free(F->verl[k]); }
        free(F->cnt); free(F->ml); free(F->excl); free(F);
    }
    if (aufg_gesamt > 0 && erl_vorher == aufg_gesamt) { free(L0); return; }
    #pragma omp parallel
    {
        Faden *F = calloc(1, sizeof(Faden));
        for (int k = 0; k <= K; k++) { F->liste[k] = malloc(sizeof(int) * nrows); F->verl[k] = malloc(sizeof(int) * nrows); }
        F->cnt = malloc(sizeof(int) * P); F->ml = malloc(sizeof(int) * P);
        F->excl = calloc(nrows, 1);
        F->ticket = omp_get_thread_num();
        int verl = 0;
        for (int s = 0; s < nstart; s++) {
            const uint64_t *h = Hb + (size_t)start[s] * W;
            int sc = 0; for (int w = 0; w < W; w++) sc += pc(h[w] & F->cov[0][w]);
            verl += maxgr - gr[start[s]] + sc;
            for (int w = 0; w < W; w++) F->cov[0][w] |= h[w];
            F->gew[s] = start[s];
        }
        if (P < 64) F->cov[0][0] |= ~0ULL << P;
        if (nstart) memcpy(F->cov[nstart], F->cov[0], sizeof F->cov[0]);
        #pragma omp barrier
        knoten(F, nstart, verl, L0, n0);
        #pragma omp critical
        {
            for (int k = 0; k <= K; k++) knoten_tiefe[k] += F->knoten[k];
            if (F->maxt > max_tiefe) max_tiefe = F->maxt;
        }
        for (int k = 0; k <= K; k++) { free(F->liste[k]); free(F->verl[k]); }
        free(F->cnt); free(F->ml); free(F->excl); free(F);
    }
    if (zeitlimit < 1e17 && jetzt() - t0 > zeitlimit) stopflag = 1;
    free(L0);
}

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
        else if (!strcmp(argv[i], "-b")) schranke2 = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-3")) dritte = 1;
        else if (!strcmp(argv[i], "-q")) prof_q = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-S")) prof_s = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-G")) g_maxel = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-B")) { bahn_von = atoi(argv[++i]); bahn_bis = atoi(argv[++i]); }
        else if (!strcmp(argv[i], "-m")) maxaufg = atoll(argv[++i]);
        else { fprintf(stderr, "unbekannt: %s\n", argv[i]); return 2; }
    }
    W = (1 << (n - 1)) / 64; if (W < 1) W = 1; P = 1 << (n - 1);
    if (W > MAXW || K > MAXK) { fprintf(stderr, "zu gross\n"); return 2; }
    for (char *p = strtok(sups, ","); p; p = strtok(NULL, ",")) sup_ok[atoi(p)] = 1;
    t0 = jetzt();
    zeilen_bauen();
    B_total = K * maxgr - P;
    klasse = malloc(nrows);
    for (int r = 0; r < nrows; r++) { int ss = 0; for (int j = 0; j < n; j++) ss += ent[r][j] != 0; klasse[r] = ss == n - 1 ? 0 : ss == n - 3 ? 1 : ss == n - 5 ? 2 : 3; }
    if (prof_q >= 0 && prof_s < 0) prof_s = 0;
    if (prof_q >= 0) printf("Profil: genau %d Zeilen mit Traeger %d, %d mit Traeger %d (Rest Traeger %d, minimale Ueberdeckungen)\n", prof_q, n - 3, prof_s, n - 5, n - 1);
    printf("n=%d d=%d K=%d Traeger=%s: %d Zeilen (davon %d Typzeilen), %d Klassen, maxgr=%d, Budget B=%d\n",
           n, d, K, argv[0] ? sups : "", nrows, ntyp, P, maxgr, B_total);
    for (int s = n; s >= 1; s--) if (sup_ok[s]) {
        int c = 0, g = -1; for (int r = 0; r < nrows; r++) { int ss = 0; for (int j = 0; j < n; j++) ss += ent[r][j] != 0; if (ss == s) { c++; g = gr[r]; } }
        printf("  Traeger %d: %d Zeilen, je %d Klassen\n", s, c, g);
    }
    if (B_total < 0) { printf("Budget negativ: keine Loesung (Zaehlargument). Fertig, vollstaendig.\n"); return 0; }
    if (erl_name) {
        erl_max = 1LL << 34;
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
        int start[3], nstart = 0;
        typgrenze = -1;
        int rep = -1;
        if (sym) {
            int8_t e[MAXN]; memset(e, 0, MAXN);
            if (typ_same[tt]) { for (int j = 1; j < n; j++) e[j] = 1; for (int j = n - typ_d[tt]; j < n; j++) e[j] = -1; }
            else { e[0] = 1; e[1] = 0; for (int j = 2; j < n; j++) e[j] = 1; for (int j = n - typ_d[tt]; j < n; j++) e[j] = -1; }
            rep = finde_zeile(e);
            if (rep < 0 || ptyp[(size_t)a_row * ntyp + rep] != tt) { fprintf(stderr, "Vertreter falsch\n"); return 1; }
            start[0] = a_row; start[1] = rep; nstart = 2;
            typgrenze = tt;
            printf("== Typ %d (%s, d=%d, Schnitt %d): a = Zeile %d, Vertreter Zeile %d\n", tt, typ_same[tt] ? "gleiche Spalte" : "andere Spalte", typ_d[tt], typ_inter[tt], a_row, rep);
        }
        memset(knoten_tiefe, 0, sizeof knoten_tiefe);
        max_tiefe = 0;
        long long g_ges = 0, g_jetzt = 0, g_vorher = 0;
        int nbahn = 1, *bahnrep = NULL;
        if (sym && dritte) {
            bahnrep = dritte_bahnen(a_row, rep, tt, &nbahn);
            printf("   dritte Zeile: %d Bahnvertreter\n", nbahn);
            fflush(stdout);
        }
        for (int bi = 0; bi < nbahn; bi++) {
            if (stopflag || gefunden_stop) break;
            if (bahn_von >= 0 && (bi < bahn_von || bi > bahn_bis)) continue;
            profgrenze = 1 << 30;
            if (bahnrep) { start[2] = bahnrep[bi]; nstart = 3; profgrenze = profil(a_row, rep, bahnrep[bi]); prof_a = a_row; prof_b = rep; }
            id_basis = (long long)bi << 24;
            lauf_start(start, nstart);
            g_ges += aufg_gesamt; g_jetzt += aufg_erledigt_jetzt; g_vorher += erl_vorher;
            if (bahnrep && (bi % 20 == 0 || aufg_gesamt > 1000)) {
                printf("   Bahn %d/%d (Profil %d,%d): %lld Aufgaben, Zeit bisher %.1f s\n", bi, nbahn, profgrenze >> 6, profgrenze & 63, aufg_gesamt, jetzt() - ts);
                fflush(stdout);
            }
        }
        free(bahnrep);
        printf("   Aufgaben (Tiefe %d): %lld gesamt, %lld in diesem Lauf erledigt, %lld vorher; Loesungen bisher %lld (verteilt %lld)%s\n",
               teiltiefe, g_ges, g_jetzt, g_vorher, n_loes, n_loes_verteilt, stopflag ? " ABBRUCH Zeit" : "");
        printf("   Knoten je Tiefe:");
        for (int k = 0; k <= K; k++) if (knoten_tiefe[k]) printf(" %d:%lld", k, knoten_tiefe[k]);
        printf("; max. Tiefe %d; Zeit %.1f s\n", max_tiefe, jetzt() - ts);
        printf("   Typ %d: %s, Loesungen %lld\n", tt, stopflag ? "NICHT fertig" : (gefunden_stop ? "Loesung gefunden (Stopp)" : (bahn_von >= 0 ? "Teilbereich fertig" : "fertig")), n_loes - loes_vor);
        fflush(stdout);
        loes_vor = n_loes;
        if (stopflag || gefunden_stop) break;
        if (!sym) break;
    }
    printf("ENDE: Loesungen %lld (Nullen verteilt: %lld), %s, Gesamtzeit %.1f s\n", n_loes, n_loes_verteilt,
           stopflag ? "ABBRUCH (Zeit)" : (gefunden_stop ? "Stopp bei erster Loesung" : "vollstaendig"), jetzt() - t0);
    return 0;
}
