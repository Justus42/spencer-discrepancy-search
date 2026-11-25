/* Suchcode Auftrag S: exakte Entscheidung "gibt es S in G mit |S| <= k und S + A_D = G fuer alle D?"
 * G = F_2^d (Paare des affinen Spanns), A_D = Muster der Nebenklasse D (aus src/spann_zaehl.py export).
 *
 * Vollstaendige Tiefensuche: Verzweigung am offenen Punkt mit den wenigsten erlaubten Kandidaten,
 * Zweig i: "c_i drin, c_1..c_{i-1} ausgeschlossen" (jede Menge hoechstens einmal je Fall).
 * Schranken: je Nebenklasse D: offene Punkte u_D <= Summe der b groessten Neuueberdeckungen in D
 * (b = Restbudget), ebenso global ueber alle Nebenklassen.
 *
 * Symmetrie (optional, Datei mit Erzeugern linearer Abbildungen phi von G, die das Problem erhalten,
 * aus src/spann_sym.py): Gruppe Gamma = <Erzeuger, Translationen>. Bahnen der 4-Mengen {0,a,b,c} unter
 * Gamma, absteigend nach Groesse nummeriert O_0, O_1, ... Jede Loesung mit >= 4 Elementen enthaelt eine
 * 4-Teilmenge; sei i der kleinste Index einer Bahn unter ihren 4-Teilmengen. Ein Bild unter Gamma enthaelt
 * dann den Vertreter Q_i und keine 4-Teilmenge aus O_0..O_{i-1}. Fall i: S enthaelt Q_i, verbotene
 * 4-Mengen werden beim Hinzufuegen sofort ausgeschlossen. Ohne Symmetriedatei: nur 0 in S (Translation).
 *
 * Aufruf: spann_dfs PROBLEM k [zeitlimit_s] [tiefe_aufteilung] [alle] [ERZEUGER] [VON BIS]
 * VON BIS (Auftrag R9, optional): nur Aufgaben mit VON <= i < BIS bearbeiten; Erzeugung und Reihenfolge unveraendert.
 * Bauen: gcc -O3 -march=native -fopenmp -o build/spann_dfs src/spann_dfs.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#include <time.h>

#define MAXN 4096
#define MAXC 512
#define MAXMU 1024
#define MAXK 64

static int d, N, NC;
static int mu[MAXC];
static int *A[MAXC];
static long long woerter[MAXN];
static int K;
static double zeitlimit = 1e18;
static int alle = 0;
static double t0;
static volatile int stopp = 0, gefunden = 0, zeit_aus = 0;
static long long gesamt_loesungen = 0;

/* Symmetrie */
static int sym = 0;
static int32_t *orbrang = NULL;  /* N^3: Rang der Bahn von {0,a,b,c} (0 = groesste); -1 fuer ungueltig */


static double jetzt(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return ts.tv_sec + 1e-9 * ts.tv_nsec; }

typedef struct {
    uint8_t *cov;      /* NC*N */
    int16_t *neu;      /* NC*N: offene Punkte in s + A_D (Index D*N+s) */
    int16_t *avail;    /* NC*N: erlaubte Kandidaten des Punktes (D,g) */
    uint16_t *aus;     /* N: ausgeschlossen (Zaehler) */
    uint8_t *drin;     /* N */
    int u[MAXC];
    long long U;
    int S[MAXK]; int ns;
    int *stapel; int nst;  /* durch Symmetrie ausgeschlossene Elemente */
    int fall;
    long long knoten;
} Zustand;

static void init(Zustand *z) {
    z->cov = calloc((size_t)NC * N, 1);
    z->neu = malloc(sizeof(int16_t) * (size_t)NC * N);
    z->avail = malloc(sizeof(int16_t) * (size_t)NC * N);
    z->aus = calloc(N, sizeof(uint16_t)); z->drin = calloc(N, 1);
    for (int D = 0; D < NC; D++) {
        z->u[D] = N;
        for (int g = 0; g < N; g++) { z->neu[D * N + g] = mu[D]; z->avail[D * N + g] = mu[D]; }
    }
    z->U = (long long)NC * N; z->ns = 0; z->knoten = 0;
    z->stapel = malloc(sizeof(int) * (size_t)N * MAXK * MAXK); z->nst = 0; z->fall = 0;
}

static void frei(Zustand *z) { free(z->cov); free(z->neu); free(z->avail); free(z->aus); free(z->drin); free(z->stapel); }

static void dazu_roh(Zustand *z, int c) {
    z->drin[c] = 1; z->S[z->ns++] = c;
    for (int D = 0; D < NC; D++) {
        const int *a = A[D]; int m = mu[D];
        uint8_t *cv = z->cov + D * N; int16_t *nu = z->neu + D * N;
        for (int i = 0; i < m; i++) {
            int g = c ^ a[i];
            if (cv[g]++ == 0) {
                z->u[D]--; z->U--;
                for (int j = 0; j < m; j++) nu[g ^ a[j]]--;
            }
        }
    }
}

static void weg_roh(Zustand *z, int c) {
    z->drin[c] = 0; z->ns--;
    for (int D = 0; D < NC; D++) {
        const int *a = A[D]; int m = mu[D];
        uint8_t *cv = z->cov + D * N; int16_t *nu = z->neu + D * N;
        for (int i = 0; i < m; i++) {
            int g = c ^ a[i];
            if (--cv[g] == 0) {
                z->u[D]++; z->U++;
                for (int j = 0; j < m; j++) nu[g ^ a[j]]++;
            }
        }
    }
}

/* Ausschliessen (Zaehler): gibt 0 zurueck, wenn ein offener Punkt keinen Kandidaten mehr hat */
static int sperre(Zustand *z, int c) {
    if (z->aus[c]++ > 0) return 1;
    int ok = 1;
    for (int D = 0; D < NC; D++) {
        const int *a = A[D]; int m = mu[D];
        int16_t *av = z->avail + D * N; uint8_t *cv = z->cov + D * N;
        for (int i = 0; i < m; i++) { int g = c ^ a[i]; if (--av[g] == 0 && cv[g] == 0) ok = 0; }
    }
    return ok;
}
static void entsperre(Zustand *z, int c) {
    if (--z->aus[c] > 0) return;
    for (int D = 0; D < NC; D++) {
        const int *a = A[D]; int m = mu[D]; int16_t *av = z->avail + D * N;
        for (int i = 0; i < m; i++) av[c ^ a[i]]++;
    }
}

static inline int32_t orb(int a, int b, int c) { return orbrang[((size_t)a * N + b) * N + c]; }

/* c hinzufuegen; mit Symmetrie: alle z sperren, die mit c und zwei Elementen von S eine verbotene 4-Menge bilden.
 * Rueckgabe: Zahl der gesperrten Elemente (auf dem Stapel), negativ kodiert -(n+1), wenn ein Punkt tot ist. */
static int dazu(Zustand *z, int c) {
    int n0 = z->ns;
    dazu_roh(z, c);
    if (!sym || n0 < 2) return 0;
    int cnt = 0, ok = 1;
    for (int i = 0; i < n0; i++) for (int j = i + 1; j < n0; j++) {
        int a = z->S[i] ^ c, b = z->S[j] ^ c;
        for (int x = 0; x < N; x++) {
            if (z->drin[x]) continue;
            int r = orb(a, b, x ^ c);
            if (r < z->fall) {
                z->stapel[z->nst++] = x; cnt++;
                if (!sperre(z, x)) ok = 0;
            }
        }
    }
    return ok ? cnt : -(cnt + 1);
}
static void weg(Zustand *z, int c, int r) {
    int cnt = r >= 0 ? r : -(r + 1);
    for (int i = 0; i < cnt; i++) entsperre(z, z->stapel[--z->nst]);
    weg_roh(z, c);
}

/* Summe der b groessten Werte von v[s] ueber erlaubte s */
static long long topb(const int16_t *v, const uint16_t *aus, const uint8_t *drin, int b, int16_t *puf) {
    int n = 0;
    for (int s = 0; s < N; s++) if (!aus[s] && !drin[s] && v[s] > 0) puf[n++] = v[s];
    if (n <= b) { long long t = 0; for (int i = 0; i < n; i++) t += puf[i]; return t; }
    static __thread int hist[MAXMU + 1];
    int mx = 0;
    for (int i = 0; i < n; i++) { hist[puf[i]]++; if (puf[i] > mx) mx = puf[i]; }
    long long t = 0; int rest = b;
    for (int w = mx; w > 0; w--) {
        if (hist[w] && rest > 0) { int k = hist[w] < rest ? hist[w] : rest; t += (long long)k * w; rest -= k; }
        hist[w] = 0;
    }
    return t;
}

static int schranke_ok(Zustand *z, int b, int16_t *puf, int32_t *tot) {
    for (int D = 0; D < NC; D++) {
        if (z->u[D] == 0) continue;
        if ((long long)b * mu[D] < z->u[D]) return 0;
    }
    for (int D = 0; D < NC; D++) {
        if (z->u[D] == 0) continue;
        if (topb(z->neu + D * N, z->aus, z->drin, b, puf) < z->u[D]) return 0;
    }
    memset(tot, 0, sizeof(int32_t) * N);
    for (int D = 0; D < NC; D++) { if (!z->u[D]) continue; const int16_t *nu = z->neu + D * N; for (int s = 0; s < N; s++) tot[s] += nu[s]; }
    {
        long long t = 0;
        int32_t best[MAXK];
        int nb = 0;
        for (int s = 0; s < N; s++) {
            if (z->aus[s] || z->drin[s]) continue;
            int32_t v = tot[s];
            if (nb < b) { int i = nb++; while (i > 0 && best[i - 1] < v) { best[i] = best[i - 1]; i--; } best[i] = v; }
            else if (v > best[b - 1]) { int i = b - 1; while (i > 0 && best[i - 1] < v) { best[i] = best[i - 1]; i--; } best[i] = v; }
        }
        for (int i = 0; i < nb; i++) t += best[i];
        if (t < z->U) return 0;
    }
    return 1;
}

static void melde(Zustand *z) {
    #pragma omp critical
    {
        gesamt_loesungen++;
        if (!gefunden || alle) {
            printf("LOESUNG (Fall %d) %d Paare:", z->fall, z->ns);
            for (int i = 0; i < z->ns; i++) printf(" %d", z->S[i]);
            printf("\nWOERTER:");
            for (int i = 0; i < z->ns; i++) printf(" %lld", woerter[z->S[i]]);
            printf("\n"); fflush(stdout);
        }
        gefunden = 1;
        if (!alle) stopp = 1;
    }
}

/* Verzweigungspunkt waehlen; gibt Kandidatenzahl zurueck (0 = tot), Kandidaten in kand */
static int waehle(Zustand *z, int *kand) {
    int bestD = -1, bestg = -1, bestv = 1 << 30;
    for (int D = 0; D < NC; D++) {
        if (!z->u[D]) continue;
        const uint8_t *cv = z->cov + D * N; const int16_t *av = z->avail + D * N;
        for (int g = 0; g < N; g++) if (!cv[g] && av[g] < bestv) { bestv = av[g]; bestD = D; bestg = g; }
    }
    if (bestv == 0 || bestD < 0) return 0;
    int nk = 0;
    for (int i = 0; i < mu[bestD]; i++) { int c = bestg ^ A[bestD][i]; if (!z->aus[c] && !z->drin[c]) kand[nk++] = c; }
    int gew[MAXMU];
    for (int i = 0; i < nk; i++) { int s = 0; for (int D = 0; D < NC; D++) if (z->u[D]) s += z->neu[D * N + kand[i]]; gew[i] = s; }
    for (int i = 1; i < nk; i++) { int c = kand[i], w = gew[i], j = i; while (j > 0 && gew[j - 1] < w) { kand[j] = kand[j - 1]; gew[j] = gew[j - 1]; j--; } kand[j] = c; gew[j] = w; }
    return nk;
}

static void suche(Zustand *z, int16_t *puf, int32_t *tot) {
    if (stopp) return;
    z->knoten++;
    if ((z->knoten & 0xfff) == 0 && jetzt() - t0 > zeitlimit) { zeit_aus = 1; stopp = 1; return; }
    if (z->U == 0) { melde(z); return; }
    int b = K - z->ns;
    if (b <= 0) return;
    if (!schranke_ok(z, b, puf, tot)) return;
    int kand[MAXMU];
    int nk = waehle(z, kand);
    int gesperrt[MAXMU], ng = 0, ok = 1;
    for (int i = 0; i < nk && ok && !stopp; i++) {
        int c = kand[i];
        int r = dazu(z, c);
        if (r >= 0) suche(z, puf, tot);
        weg(z, c, r);
        if (!sperre(z, c)) ok = 0;
        gesperrt[ng++] = c;
    }
    for (int i = ng - 1; i >= 0; i--) entsperre(z, gesperrt[i]);
}

/* Aufgaben: Fall + Praefix (hinzugefuegte Elemente in Reihenfolge, durch Verzweigung gesperrte Elemente) */
typedef struct { int fall, nd, ns; int dazu[MAXK]; int sp[MAXMU * 4]; } Aufgabe;
static Aufgabe *aufg; static int naufg, kapaz;

static void ablegen(Zustand *z, int *spl, int nsp) {
    if (naufg == kapaz) { kapaz = kapaz ? 2 * kapaz : 1024; aufg = realloc(aufg, sizeof(Aufgabe) * kapaz); }
    Aufgabe *a = &aufg[naufg++];
    a->fall = z->fall; a->nd = z->ns; memcpy(a->dazu, z->S, sizeof(int) * z->ns);
    a->ns = nsp; memcpy(a->sp, spl, sizeof(int) * nsp);
}

static void sammle(Zustand *z, int tiefe, int16_t *puf, int32_t *tot, int *spl, int nsp) {
    if (z->U == 0 || tiefe == 0) { ablegen(z, spl, nsp); return; }
    int b = K - z->ns;
    if (b <= 0) return;
    if (!schranke_ok(z, b, puf, tot)) return;
    int kand[MAXMU];
    int nk = waehle(z, kand);
    int ng = 0, ok = 1, lok[MAXMU];
    for (int i = 0; i < nk && ok; i++) {
        int c = kand[i];
        int r = dazu(z, c);
        if (r >= 0) sammle(z, tiefe - 1, puf, tot, spl, nsp + ng);
        weg(z, c, r);
        if (!sperre(z, c)) ok = 0;
        spl[nsp + ng] = c; lok[ng++] = c;
    }
    for (int i = ng - 1; i >= 0; i--) entsperre(z, lok[i]);
}

/* ---- Bahnen der 4-Mengen {0,a,b,c} ---- */
static int ngen; static int gen[64][16];
static int anw(int g, int x) { int y = 0, i = 0; while (x) { if (x & 1) y ^= gen[g][i]; x >>= 1; i++; } return y; }
static int32_t *eltern;
static int32_t finde(int32_t x) { while (eltern[x] != x) { eltern[x] = eltern[eltern[x]]; x = eltern[x]; } return x; }
static inline size_t tidx(int a, int b, int c) {  /* sortiert */
    int t;
    if (a > b) { t = a; a = b; b = t; }
    if (b > c) { t = b; b = c; c = t; }
    if (a > b) { t = a; a = b; b = t; }
    return ((size_t)a * N + b) * N + c;
}
static void vereinige(size_t x, size_t y) { int32_t p = finde((int32_t)x), q = finde((int32_t)y); if (p != q) eltern[p] = q; }
static int nfall; static int frep[1 << 20][3]; static long long fgross[1 << 20];

static void bahnen4(void) {
    size_t M = (size_t)N * N * N;
    eltern = malloc(sizeof(int32_t) * M);
    for (size_t i = 0; i < M; i++) eltern[i] = (int32_t)i;
    for (int a = 1; a < N; a++) for (int b = a + 1; b < N; b++) for (int c = b + 1; c < N; c++) {
        size_t me = tidx(a, b, c);
        for (int g = 0; g < ngen; g++) vereinige(me, tidx(anw(g, a), anw(g, b), anw(g, c)));
        vereinige(me, tidx(a, a ^ b, a ^ c));
        vereinige(me, tidx(b, b ^ a, b ^ c));
        vereinige(me, tidx(c, c ^ a, c ^ b));
    }
    /* Groessen */
    int32_t *id = malloc(sizeof(int32_t) * M);
    for (size_t i = 0; i < M; i++) id[i] = -1;
    long long *gr = calloc(M, sizeof(long long));  /* nur an Wurzeln benutzt */
    long long gesamt = 0;
    for (int a = 1; a < N; a++) for (int b = a + 1; b < N; b++) for (int c = b + 1; c < N; c++) { gr[finde((int32_t)tidx(a, b, c))]++; gesamt++; }
    nfall = 0;
    for (int a = 1; a < N; a++) for (int b = a + 1; b < N; b++) for (int c = b + 1; c < N; c++) {
        int32_t r = finde((int32_t)tidx(a, b, c));
        if (id[r] < 0) { id[r] = nfall; frep[nfall][0] = a; frep[nfall][1] = b; frep[nfall][2] = c; fgross[nfall] = gr[r]; nfall++; }
    }
    /* nach Groesse absteigend sortieren */
    int *ord = malloc(sizeof(int) * nfall); for (int i = 0; i < nfall; i++) ord[i] = i;
    for (int i = 1; i < nfall; i++) { int x = ord[i], j = i; while (j > 0 && fgross[ord[j - 1]] < fgross[x]) { ord[j] = ord[j - 1]; j--; } ord[j] = x; }
    int *rang = malloc(sizeof(int) * nfall); for (int i = 0; i < nfall; i++) rang[ord[i]] = i;
    int (*rep2)[3] = malloc(sizeof(int) * 3 * nfall); long long *gr2 = malloc(sizeof(long long) * nfall);
    for (int i = 0; i < nfall; i++) { memcpy(rep2[rang[i]], frep[i], sizeof(int) * 3); gr2[rang[i]] = fgross[i]; }
    for (int i = 0; i < nfall; i++) { memcpy(frep[i], rep2[i], sizeof(int) * 3); fgross[i] = gr2[i]; }
    orbrang = malloc(sizeof(int32_t) * M);
    for (size_t i = 0; i < M; i++) orbrang[i] = 1 << 30;
    for (int a = 1; a < N; a++) for (int b = 1; b < N; b++) for (int c = 1; c < N; c++) {
        if (a == b || b == c || a == c) continue;
        orbrang[((size_t)a * N + b) * N + c] = rang[id[finde((int32_t)tidx(a, b, c))]];
    }
    printf("4-Mengen {0,a,b,c}: %lld, Bahnen unter Gamma: %d; Groessen (absteigend):", gesamt, nfall);
    for (int i = 0; i < nfall && i < 40; i++) printf(" %lld", fgross[i]);
    if (nfall > 40) printf(" ...");
    printf("\n"); fflush(stdout);
    free(eltern); free(id); free(gr); free(ord); free(rang); free(rep2); free(gr2);
}

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "spann_dfs PROBLEM k [zeit] [tiefe] [alle] [ERZEUGER]\n"); return 2; }
    FILE *f = fopen(argv[1], "r");
    char zeile[1 << 16];
    if (!fgets(zeile, sizeof zeile, f)) return 2;
    if (fscanf(f, "%d %d", &d, &NC) != 2) return 2;
    N = 1 << d;
    for (int g = 0; g < N; g++) if (fscanf(f, "%lld", &woerter[g]) != 1) return 2;
    for (int D = 0; D < NC; D++) {
        long long x; if (fscanf(f, "%lld %d", &x, &mu[D]) != 2) return 2;
        A[D] = malloc(sizeof(int) * mu[D]);
        for (int i = 0; i < mu[D]; i++) if (fscanf(f, "%d", &A[D][i]) != 1) return 2;
    }
    fclose(f);
    K = atoi(argv[2]);
    if (argc > 3) zeitlimit = atof(argv[3]);
    int T = argc > 4 ? atoi(argv[4]) : 2;
    if (argc > 5) alle = atoi(argv[5]);
    if (argc > 6) {
        FILE *g = fopen(argv[6], "r"); ngen = 0;
        while (ngen < 64) { int ok = 1; for (int i = 0; i < d; i++) if (fscanf(g, "%d", &gen[ngen][i]) != 1) { ok = 0; break; } if (!ok) break; ngen++; }
        fclose(g); sym = 1;
    }
    int von = 0, bis = -1, bereich = 0;
    if (argc > 8) { von = atoi(argv[7]); bis = atoi(argv[8]); bereich = 1; }
    int mumin = 1 << 30; for (int D = 0; D < NC; D++) if (mu[D] < mumin) mumin = mu[D];
    printf("Problem %s: d=%d |G|=%d Nebenklassen=%d min mu=%d, k=%d, Zeitlimit %.0f s, Aufteilungstiefe %d, Threads %d, Symmetrie %s (%d Erzeuger)\n",
           argv[1], d, N, NC, mumin, K, zeitlimit, T, omp_get_max_threads(), sym ? "ja" : "nein", ngen);
    fflush(stdout);
    t0 = jetzt();
    if (sym) bahnen4(); else { nfall = 1; }
    Zustand z; init(&z);
    int16_t *puf = malloc(sizeof(int16_t) * N); int32_t *tot = malloc(sizeof(int32_t) * N);
    int *spl = malloc(sizeof(int) * MAXMU * 4);
    for (int fl = 0; fl < nfall; fl++) {
        z.fall = fl;
        if (!sym) { dazu(&z, 0); sammle(&z, T, puf, tot, spl, 0); weg(&z, 0, 0); continue; }
        int q[4] = {0, frep[fl][0], frep[fl][1], frep[fl][2]}, r[4], ok = 1, i;
        for (i = 0; i < 4; i++) { if (z.aus[q[i]]) { ok = 0; break; } r[i] = dazu(&z, q[i]); if (r[i] < 0) { ok = 0; i++; break; } }
        if (ok) sammle(&z, T, puf, tot, spl, 0);
        for (int j = i - 1; j >= 0; j--) weg(&z, q[j], r[j]);
    }
    frei(&z);
    printf("Aufgaben: %d (%.1f s)\n", naufg, jetzt() - t0); fflush(stdout);
    if (!bereich) bis = naufg;
    if (bis > naufg) bis = naufg;
    if (von < 0) von = 0;
    long long knoten = 0; int fertig = 0;
    #pragma omp parallel reduction(+:knoten)
    {
        Zustand w; init(&w);
        int16_t *pf = malloc(sizeof(int16_t) * N); int32_t *tt = malloc(sizeof(int32_t) * N);
        int rr[MAXK];
        #pragma omp for schedule(dynamic, 1)
        for (int i = von; i < bis; i++) {
            if (stopp) continue;
            Aufgabe *a = &aufg[i];
            w.fall = a->fall;
            int ok = 1, j;
            for (j = 0; j < a->nd; j++) { rr[j] = dazu(&w, a->dazu[j]); if (rr[j] < 0) { ok = 0; j++; break; } }
            int nd = j, s;
            for (s = 0; s < a->ns && ok; s++) if (!sperre(&w, a->sp[s])) { ok = 0; s++; break; }
            int nsp = s;
            if (ok) suche(&w, pf, tt);
            for (s = nsp - 1; s >= 0; s--) entsperre(&w, a->sp[s]);
            for (j = nd - 1; j >= 0; j--) weg(&w, a->dazu[j], rr[j]);
            #pragma omp atomic
            fertig++;
            if (fertig % 500 == 0) {
                #pragma omp critical
                { printf("  Fortschritt %d/%d Aufgaben, %.0f s\n", fertig, naufg, jetzt() - t0); fflush(stdout); }
            }
        }
        knoten += w.knoten;
        frei(&w);
    }
    double t = jetzt() - t0;
    if (gefunden) printf("ERGEBNIS k=%d: LOESBAR (Loesungen gezaehlt: %lld%s)\n", K, gesamt_loesungen, alle ? ", alle" : ", Abbruch nach erster");
    else if (zeit_aus) printf("ERGEBNIS k=%d: UNENTSCHIEDEN (Zeitlimit, %d von %d Aufgaben fertig)\n", K, fertig, naufg);
    else printf("ERGEBNIS k=%d: KEINE Loesung mit <= %d Paaren (vollstaendig)\n", K, K);
    printf("Knoten %lld, Laufzeit %.1f s\n", knoten, t);
    if (bereich) {
        const char *erg = gefunden ? "LOESUNG" : (zeit_aus ? "ZEITLIMIT" : "OHNE_LOESUNG");
        printf("BEREICH %d %d von %d Aufgaben insgesamt: Knoten %lld, %s\n", von, bis, naufg, knoten, erg);
    }
    return 0;
}
