/* b2_zaehl.c  (Auftrag B2, Pruefcode, nur ganze Zahlen)
 *
 * Zweite, unabhaengige Zaehlung der P-invarianten selbstkomplementaeren Ueberdeckungen (Laenge 20, Radius 6).
 * Wortkonvention: Bit j = Spalte j, Bit 1 = Eintrag -1. Paar {w,-w} = Punktklasse. P aus Datei (9 Elemente,
 * Zeile "pi_0..pi_19 | t_0..t_19": Spalte j -> pi_j, Faktor t_j).
 * Bahnen von P auf den 2^19 Paaren werden hier selbst berechnet und nach (Groesse, kleinstes Wort) nummeriert:
 * Id 0..7 Groesse 1, 8..167 Groesse 3, 168.. Groesse 9. Bahnen der Woerter = Bahnen der Punkte (gleiche Objekte).
 * Bahn o ueberdeckt Punktbahn x, wenn ein Paar aus o Abstand <= 6 zu einem Punkt aus x hat; wegen der Invarianz
 * genuegt der Vertreter von o: cov[o] = { oid(rep_o ^ e) : |e| <= 6 } (Bitmenge ueber alle Punktbahnen).
 * Gewicht einer Bahn = Zahl ihrer Paare (= Zeilen).
 *
 * Verfahren (anders als im ersten Programm):
 *  - Kleine Bahnen (Gewicht 1 und 3, 168 Stueck): binaeres Verzweigen ueber Bahnen (drin / nicht drin).
 *    Gezaehlt werden ALLE Bahnmengen mit Gewicht <= B, die alle Punkte des Universums ueberdecken.
 *    Zustand ganzzahlig ueber einer Teilmenge D schwerer Punkte (wenigste kleine Ueberdecker zuerst);
 *    ist D ueberdeckt, wird mit allen Punkten nachgeprueft und gegebenenfalls an einem offenen Punkt
 *    ausserhalb von D weiterverzweigt (D ist nur Beschleunigung, keine Annahme).
 *    Schranke (Zaehlschranke): jede Ueberdeckung kostet mindestens
 *       sum_{u offen} m_u * min_{c verfuegbar, c ueberdeckt u} w_c / cov_m(c),
 *    cov_m(c) = gewichtete Zahl der offenen D-Punkte, die c ueberdeckt (m_u = Groesse der Punktbahn).
 *    Rechnung in Festkomma 2^32 mit Abrunden je Summand (Schranke wird nur kleiner, also gueltig).
 *    Ausserdem: ein offener D-Punkt ohne verfuegbaren Kandidaten -> abschneiden.
 *    Verzweigt wird an der verfuegbaren Bahn mit groesster Ueberdeckung unter den Kandidaten des
 *    offenen D-Punktes mit den wenigsten verfuegbaren Kandidaten: erst "drin", dann "nicht drin".
 *    Ist alles ueberdeckt, werden alle Obermengen aus unentschiedenen Bahnen mit Gewicht <= B kombinatorisch
 *    mitgezaehlt (jede Menge genau einmal, da die Zweige "drin"/"nicht drin" disjunkt sind).
 *  - genau eine Neunerbahn (neun1): je Neunerbahn o dasselbe Verfahren auf den von o offen gelassenen Punkten.
 *  - genau zwei Neunerbahnen (neun2): alle Paare o1 < o2 von Neunerbahnen; Filter ueber T Tabellen zu je
 *    q Probenpunkten: Tabelle t enthaelt alle Restmuster r (Probenpunkte), die eine kleine Menge S mit
 *    Gewicht <= B-18 ganz ueberdeckt (Teilmengen-Abschluss). Ein Paar ueberlebt nur, wenn sein Restmuster in
 *    allen Tabellen liegt (notwendig). Ueberlebende Paare: exakte Pruefung gegen alle kleinen Mengen S.
 *
 * Modi:
 *   info P_DATEI [MATRIX]
 *   klein P_DATEI B SPLIT LO HI [LISTE]    nur kleine Bahnen, Teilbaeume [LO,HI) der Tiefe SPLIT
 *   teilbaeume P_DATEI B SPLIT            Zahl der Teilbaeume
 *   neun1 P_DATEI B LO HI                 Neunerbahnen mit Index [LO,HI) (0..58199)
 *   neun2 P_DATEI B                       genau zwei Neunerbahnen
 *   test0 P_DATEI SEED K KD BK            kleine Bahnen gegen Durchprobieren (K zufaellige Punktbahnen)
 *   test1 P_DATEI SEED K B                neun1 gegen Durchprobieren
 *   test2 P_DATEI SEED K B                neun2 gegen Durchprobieren
 *   positiv1 P_DATEI O BSMALL             eine vorgegebene Neunerbahn (Index O), kleine Bahnen bis BSMALL
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#else
static int omp_get_thread_num(void) { return 0; }
static int omp_get_max_threads(void) { return 1; }
#endif

typedef unsigned long long u64;
#define NB 20
#define NWORDS (1 << NB)
#define MASK20 ((1u << NB) - 1)

static int NP;               /* Zahl der Punktbahnen */
static int NWF;              /* Woerter einer Bitmenge ueber alle Punktbahnen */
static int *oid;             /* Bahn-Id je Wort */
static unsigned *rep;        /* kleinstes Wort der Bahn */
static int *osz;             /* Groesse */
static u64 *cov;             /* cov[o*NWF + ...] */
static int N1, N3, N9;       /* Zahl der Bahnen je Groesse */
static int NSM;              /* kleine Bahnen = N1+N3 (=168) */
#define CW 3                 /* Woerter fuer kleine Kandidaten (<=192) */
static u64 (*smk)[CW];       /* kleine Ueberdecker je Punktbahn */
static int *nsmall;          /* Zahl kleiner Ueberdecker */
static int *hard;            /* Punktbahnen nach nsmall aufsteigend */
static u64 ALLSM[CW], ONES[CW];

static int pi_el[9][NB], t_el[9][NB], NEL;

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + 1e-9 * t.tv_nsec; }

static inline u64 *COV(int o) { return cov + (size_t)o * NWF; }
static inline int tb(const u64 *a, int i) { return (a[i >> 6] >> (i & 63)) & 1; }

static void read_P(const char *fn) {
  FILE *f = fopen(fn, "r"); if (!f) { perror(fn); exit(2); }
  char line[1024]; NEL = 0;
  while (fgets(line, sizeof line, f)) {
    if (line[0] == '#' || line[0] == '\n') continue;
    char *p = line;
    for (int j = 0; j < NB; j++) pi_el[NEL][j] = (int)strtol(p, &p, 10);
    while (*p == ' ') p++;
    if (*p != '|') { fprintf(stderr, "Format P\n"); exit(2); }
    p++;
    for (int j = 0; j < NB; j++) t_el[NEL][j] = (int)strtol(p, &p, 10);
    NEL++;
  }
  fclose(f);
  if (NEL != 9) { fprintf(stderr, "P: %d Elemente\n", NEL); exit(2); }
}

static unsigned apply(int g, unsigned w) {
  unsigned r = 0;
  for (int j = 0; j < NB; j++) {
    unsigned b = (w >> j) & 1;
    if (t_el[g][j] == -1) b ^= 1;
    r |= b << pi_el[g][j];
  }
  return r;
}
static inline unsigned canon(unsigned w) { unsigned v = w ^ MASK20; return w < v ? w : v; }

typedef struct { unsigned mn; int sz; } OS;
static int oscmp(const void *a, const void *b) {
  const OS *x = a, *y = b;
  if (x->sz != y->sz) return x->sz - y->sz;
  return x->mn < y->mn ? -1 : (x->mn > y->mn);
}

static int *ball; static int nball;

static void setup(const char *pfile, int quiet) {
  double t0 = now();
  read_P(pfile);
  /* Gruppe pruefen: Abschluss unter Verkettung (als Wirkung auf 20 Einheitswoerter + 0) */
  unsigned *omin = malloc(sizeof(unsigned) * NWORDS);
  for (unsigned w = 0; w < NWORDS; w++) omin[w] = 0xffffffffu;
  #pragma omp parallel for schedule(static)
  for (int w = 0; w < NWORDS; w++) {
    unsigned c = canon((unsigned)w), m = c;
    for (int g = 0; g < NEL; g++) { unsigned d = canon(apply(g, c)); if (d < m) m = d; }
    omin[w] = m;
  }
  /* Bahngroessen: Zahl der Klassen je omin */
  int *cnt = calloc(NWORDS, sizeof(int));
  for (unsigned w = 0; w < NWORDS; w++) if (canon(w) == w) cnt[omin[w]]++;
  int no = 0;
  for (unsigned w = 0; w < NWORDS; w++) if (cnt[w]) no++;
  OS *os = malloc(sizeof(OS) * no);
  no = 0;
  for (unsigned w = 0; w < NWORDS; w++) if (cnt[w]) { os[no].mn = w; os[no].sz = cnt[w]; no++; }
  qsort(os, no, sizeof(OS), oscmp);
  NP = no; NWF = (NP + 63) / 64;
  int *idof = malloc(sizeof(int) * NWORDS);
  rep = malloc(sizeof(unsigned) * NP); osz = malloc(sizeof(int) * NP);
  N1 = N3 = N9 = 0;
  for (int i = 0; i < NP; i++) {
    idof[os[i].mn] = i; rep[i] = os[i].mn; osz[i] = os[i].sz;
    if (os[i].sz == 1) N1++; else if (os[i].sz == 3) N3++; else if (os[i].sz == 9) N9++;
    else { fprintf(stderr, "Bahngroesse %d\n", os[i].sz); exit(3); }
  }
  oid = malloc(sizeof(int) * NWORDS);
  for (unsigned w = 0; w < NWORDS; w++) oid[w] = idof[omin[w]];
  free(idof); free(cnt); free(os); free(omin);
  NSM = N1 + N3;
  if (NSM > 64 * CW) { fprintf(stderr, "zu viele kleine Bahnen\n"); exit(3); }
  /* Kugel */
  nball = 0; ball = malloc(sizeof(int) * 70000);
  for (unsigned e = 0; e < NWORDS; e++) if (__builtin_popcount(e) <= 6) ball[nball++] = e;
  /* Ueberdeckung */
  cov = calloc((size_t)NP * NWF, sizeof(u64));
  if (!cov) { fprintf(stderr, "Speicher\n"); exit(3); }
  #pragma omp parallel for schedule(dynamic, 64)
  for (int o = 0; o < NP; o++) {
    u64 *c = COV(o); unsigned r = rep[o];
    for (int i = 0; i < nball; i++) { int x = oid[r ^ ball[i]]; c[x >> 6] |= 1ULL << (x & 63); }
  }
  smk = calloc(NP, sizeof *smk);
  nsmall = calloc(NP, sizeof(int));
  for (int c = 0; c < NSM; c++) {
    u64 *cc = COV(c);
    for (int x = 0; x < NP; x++) if (tb(cc, x)) { smk[x][c >> 6] |= 1ULL << (c & 63); nsmall[x]++; }
  }
  memset(ALLSM, 0, sizeof ALLSM); memset(ONES, 0, sizeof ONES);
  for (int c = 0; c < NSM; c++) ALLSM[c >> 6] |= 1ULL << (c & 63);
  for (int c = 0; c < N1; c++) ONES[c >> 6] |= 1ULL << (c & 63);
  /* Punkte nach Schwierigkeit */
  hard = malloc(sizeof(int) * NP);
  int maxn = 0; for (int x = 0; x < NP; x++) if (nsmall[x] > maxn) maxn = nsmall[x];
  int k = 0;
  for (int n = 0; n <= maxn; n++) for (int x = 0; x < NP; x++) if (nsmall[x] == n) hard[k++] = x;
  if (!quiet) fprintf(stderr, "# Aufbau: %d Bahnen (1:%d 3:%d 9:%d), Kugel %d, %.1f s\n", NP, N1, N3, N9, nball, now() - t0);
}

/* ----------------------------- Motor fuer kleine Bahnen ----------------------------- */
#define MAXWD 64
#define MAXK (64 * MAXWD)
typedef struct { int nn; int o[2]; u64 I[CW]; int w; u64 nsup; } Sol;

typedef struct {
  int B;                 /* Budget fuer das Gewicht der kleinen Bahnen */
  int wbase, nn, o[2];   /* Kontext: Neunerbahnen */
  const u64 *U0;         /* Universum (NWF Woerter) */
  int K, NWD; int wwt[MAXWD];
  u64 cD[64 * CW][MAXWD];
  u64 smD[MAXK][CW];
  int dpt[MAXK];
  /* Aufteilung */
  int split; long long subidx, lo, hi; long long *ticket; long long myticket; int top_owner;
  /* Ergebnisse */
  u64 cnt[3][128];
  long long nodes, fullchecks;
  Sol *sol; long nsol, capsol;
  int record;
  u64 *tmp;
} Eng;

static u64 BIN[256][256];
static void init_bin(void) {
  for (int n = 0; n < 256; n++) { BIN[n][0] = 1; for (int k = 1; k <= n; k++) BIN[n][k] = BIN[n - 1][k - 1] + (k < n ? BIN[n - 1][k] : 0); }
}

static Eng *eng_new(void) { Eng *e = calloc(1, sizeof(Eng)); e->tmp = malloc(sizeof(u64) * NWF); e->record = 1; return e; }

/* D waehlen: bis Kmax Punkte des Universums, schwerste zuerst; gruppiert nach Groesse (je eigenes Wort) */
static void eng_setup(Eng *e, const u64 *U0, int Kmax) {
  e->U0 = U0;
  int sel[MAXK], ns = 0;
  for (int i = 0; i < NP && ns < Kmax; i++) { int x = hard[i]; if (tb(U0, x)) sel[ns++] = x; }
  int pos = 0, nw = 0;
  for (int i = 0; i < MAXK; i++) e->dpt[i] = -1;
  for (int g = 0; g < 3; g++) {
    int want = g == 0 ? 1 : (g == 1 ? 3 : 9);
    int start = pos;
    for (int i = 0; i < ns; i++) if (osz[sel[i]] == want) e->dpt[pos++] = sel[i];
    if (pos > start) {
      int w0 = start / 64, w1 = (pos + 63) / 64;
      if (w1 > MAXWD) { fprintf(stderr, "D zu gross\n"); exit(3); }
      for (int w = w0; w < w1; w++) e->wwt[w] = want;
      pos = w1 * 64; nw = w1;          /* naechste Gruppe beginnt an Wortgrenze */
    }
  }
  e->NWD = nw; e->K = pos;
  memset(e->cD, 0, sizeof e->cD);
  memset(e->smD, 0, sizeof e->smD);
  for (int i = 0; i < e->K; i++) if (e->dpt[i] >= 0) {
    int x = e->dpt[i];
    for (int w = 0; w < CW; w++) e->smD[i][w] = smk[x][w];
    for (int w = 0; w < CW; w++) { u64 m = smk[x][w]; while (m) { int c = w * 64 + __builtin_ctzll(m); m &= m - 1; e->cD[c][i >> 6] |= 1ULL << (i & 63); } }
  }
}

static inline int wofc(int c) { return c < N1 ? 1 : 3; }

static void record_sol(Eng *e, const u64 *I, const u64 *Eexc, int w) {
  u64 und[CW]; int n1 = 0, n3 = 0;
  for (int i = 0; i < CW; i++) und[i] = ALLSM[i] & ~I[i] & ~Eexc[i];
  for (int i = 0; i < CW; i++) { n1 += __builtin_popcountll(und[i] & ONES[i]); n3 += __builtin_popcountll(und[i] & ~ONES[i]); }
  int R = e->B - w;
  u64 tot = 0;
  for (int b = 0; 3 * b <= R && b <= n3; b++) for (int a = 0; a + 3 * b <= R && a <= n1; a++) {
    u64 v = BIN[n1][a] * BIN[n3][b];
    e->cnt[e->nn][e->wbase + w + a + 3 * b] += v; tot += v;
  }
  if (e->record) {
    if (e->nsol == e->capsol) { e->capsol = e->capsol ? 2 * e->capsol : 1024; e->sol = realloc(e->sol, sizeof(Sol) * e->capsol); }
    Sol *s = &e->sol[e->nsol++];
    s->nn = e->nn; s->o[0] = e->o[0]; s->o[1] = e->o[1]; memcpy(s->I, I, sizeof(u64) * CW); s->w = e->wbase + w; s->nsup = tot;
  }
}

static inline int wpc(const Eng *e, const u64 *a) {
  int s = 0; for (int i = 0; i < e->NWD; i++) s += e->wwt[i] * __builtin_popcountll(a[i]); return s;
}

static void node(Eng *e, const u64 *I, const u64 *Ex, int w, const u64 *U, int depth) {
  int top = depth < e->split;
  if (depth == e->split) {
    long long idx = e->subidx++;
    if (idx < e->lo || idx >= e->hi) return;
    if (e->ticket) {
      if (idx != e->myticket) return;
      e->myticket = __atomic_fetch_add(e->ticket, 1, __ATOMIC_SEQ_CST);
    }
  }
  if (!top || e->top_owner) e->nodes++;
  int R = e->B - w;
  u64 A[CW];
  for (int i = 0; i < CW; i++) { A[i] = ALLSM[i] & ~I[i] & ~Ex[i]; if (R < 3) A[i] &= ONES[i]; if (R < 1) A[i] = 0; }
  int NWD = e->NWD;
  int any = 0; for (int i = 0; i < NWD; i++) if (U[i]) { any = 1; break; }
  if (!any) {
    /* D ueberdeckt: volle Pruefung */
    if (!top || e->top_owner) e->fullchecks++;
    u64 *t = e->tmp;
    memcpy(t, e->U0, sizeof(u64) * NWF);
    for (int i = 0; i < CW; i++) { u64 m = I[i]; while (m) { int c = i * 64 + __builtin_ctzll(m); m &= m - 1; const u64 *cc = COV(c); for (int k = 0; k < NWF; k++) t[k] &= ~cc[k]; } }
    int p = -1;
    for (int k = 0; k < NWF; k++) if (t[k]) { p = k * 64 + __builtin_ctzll(t[k]); break; }
    if (p < 0) { if (!top || e->top_owner) record_sol(e, I, Ex, w); return; }
    u64 Cp[CW]; int have = 0;
    for (int i = 0; i < CW; i++) { Cp[i] = smk[p][i] & A[i]; if (Cp[i]) have = 1; }
    if (!have) return;
    int c = -1; for (int i = 0; i < CW; i++) if (Cp[i]) { c = i * 64 + __builtin_ctzll(Cp[i]); break; }
    u64 I2[CW], E2[CW]; memcpy(I2, I, sizeof I2); memcpy(E2, Ex, sizeof E2);
    I2[c >> 6] |= 1ULL << (c & 63);
    node(e, I2, Ex, w + wofc(c), U, depth + 1);
    E2[c >> 6] |= 1ULL << (c & 63);
    node(e, I, E2, w, U, depth + 1);
    return;
  }
  /* Zaehlschranke */
  int cl[64 * CW], nc = 0, cv[64 * CW];
  for (int i = 0; i < CW; i++) {
    u64 m = A[i];
    while (m) {
      int c = i * 64 + __builtin_ctzll(m); m &= m - 1;
      int s = 0; const u64 *d = e->cD[c];
      for (int k = 0; k < NWD; k++) s += e->wwt[k] * __builtin_popcountll(d[k] & U[k]);
      if (s) { cv[c] = s; cl[nc++] = c; }
    }
  }
  /* Sortieren nach w_c / cv_c aufsteigend (exakt: w_a*cv_b < w_b*cv_a), Einfuegesortierung */
  for (int i = 1; i < nc; i++) {
    int c = cl[i], j = i - 1;
    long long wc = wofc(c), vc = cv[c];
    while (j >= 0) {
      int d = cl[j];
      long long lhs = wc * cv[d], rhs = (long long)wofc(d) * vc;
      if (lhs < rhs || (lhs == rhs && c < d)) { cl[j + 1] = d; j--; } else break;
    }
    cl[j + 1] = c;
  }
  u64 rem[MAXWD]; memcpy(rem, U, sizeof(u64) * NWD);
  u64 LB = 0, cap = (u64)R << 32;
  for (int i = 0; i < nc; i++) {
    int c = cl[i]; const u64 *d = e->cD[c];
    int nwv = 0; u64 anyr = 0;
    for (int k = 0; k < NWD; k++) { u64 x = d[k] & rem[k]; nwv += e->wwt[k] * __builtin_popcountll(x); rem[k] &= ~d[k]; anyr |= rem[k]; }
    if (nwv) {
      LB += (((u64)wofc(c) * (u64)nwv) << 32) / (u64)cv[c];
      if (LB > cap) return;
    }
    if (!anyr) break;
  }
  for (int k = 0; k < NWD; k++) if (rem[k]) return;   /* offener D-Punkt ohne Kandidat */
  /* Zielpunkt: offener D-Punkt mit den wenigsten verfuegbaren Kandidaten */
  int bp = -1, bn = 1 << 30;
  for (int k = 0; k < NWD; k++) {
    u64 m = U[k];
    while (m) {
      int i = k * 64 + __builtin_ctzll(m); m &= m - 1;
      int n = 0; for (int q = 0; q < CW; q++) n += __builtin_popcountll(e->smD[i][q] & A[q]);
      if (n < bn) { bn = n; bp = i; if (n <= 1) goto found; }
    }
  }
found:;
  /* Kandidat mit groesster Ueberdeckung unter den Kandidaten von bp */
  int bc = -1, bv = -1;
  for (int q = 0; q < CW; q++) {
    u64 m = e->smD[bp][q] & A[q];
    while (m) { int c = q * 64 + __builtin_ctzll(m); m &= m - 1; if (cv[c] > bv) { bv = cv[c]; bc = c; } }
  }
  if (bc < 0) return;
  u64 I2[CW], E2[CW], U2[MAXWD];
  memcpy(I2, I, sizeof I2); I2[bc >> 6] |= 1ULL << (bc & 63);
  for (int k = 0; k < NWD; k++) U2[k] = U[k] & ~e->cD[bc][k];
  node(e, I2, Ex, w + wofc(bc), U2, depth + 1);
  memcpy(E2, Ex, sizeof E2); E2[bc >> 6] |= 1ULL << (bc & 63);
  node(e, I, E2, w, U, depth + 1);
}

static void eng_run(Eng *e) {
  u64 I[CW] = {0}, Ex[CW] = {0}, U[MAXWD];
  memset(U, 0, sizeof U);
  for (int i = 0; i < e->K; i++) if (e->dpt[i] >= 0) U[i >> 6] |= 1ULL << (i & 63);
  e->subidx = 0;
  node(e, I, Ex, 0, U, 0);
}

/* ----------------------------- Ausgabe ----------------------------- */
static void print_counts(const char *tag, u64 c[3][128]) {
  for (int nn = 0; nn < 3; nn++) for (int w = 0; w < 128; w++) if (c[nn][w])
    printf("%s Neuner=%d Gewicht=%d Anzahl=%llu\n", tag, nn, w, c[nn][w]);
}

static void print_sol(FILE *f, const Sol *s) {
  fprintf(f, "L %d %llu |", s->w, s->nsup);
  for (int k = 0; k < s->nn; k++) fprintf(f, " %d", s->o[k]);
  for (int c = 0; c < NSM; c++) if ((s->I[c >> 6] >> (c & 63)) & 1) fprintf(f, " %d", c);
  fprintf(f, " | Vertreter");
  for (int k = 0; k < s->nn; k++) fprintf(f, " %u", rep[s->o[k]]);
  for (int c = 0; c < NSM; c++) if ((s->I[c >> 6] >> (c & 63)) & 1) fprintf(f, " %u", rep[c]);
  fprintf(f, "\n");
}

static int KD_DEFAULT = 512;

/* ---------------- klein ---------------- */
static int run_klein(int B, int split, long long lo, long long hi, const char *liste, int countonly) {
  u64 *U0 = malloc(sizeof(u64) * NWF); memset(U0, 0, sizeof(u64) * NWF);
  for (int x = 0; x < NP; x++) U0[x >> 6] |= 1ULL << (x & 63);
  int T = omp_get_max_threads();
  long long ticket = lo;
  Eng **es = malloc(sizeof(Eng *) * T);
  double t0 = now();
  long long nsub = 0;
  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    Eng *e = eng_new(); es[tid] = e;
    eng_setup(e, U0, KD_DEFAULT);
    e->B = B; e->wbase = 0; e->nn = 0; e->split = split; e->lo = lo; e->hi = countonly ? -1 : hi;
    e->ticket = countonly ? NULL : &ticket;
    e->top_owner = (tid == 0 && lo == 0 && !countonly) ? 1 : (countonly && tid == 0);
    if (!countonly) e->myticket = __atomic_fetch_add(&ticket, 1, __ATOMIC_SEQ_CST);
    if (!countonly || tid == 0) eng_run(e);
    if (countonly && tid == 0) nsub = e->subidx;
  }
  if (countonly) { printf("Teilbaeume Tiefe %d: %lld\n", split, nsub); return 0; }
  u64 c[3][128]; memset(c, 0, sizeof c);
  long long nodes = 0, fc = 0, nsol = 0;
  for (int t = 0; t < T; t++) { for (int a = 0; a < 3; a++) for (int w = 0; w < 128; w++) c[a][w] += es[t]->cnt[a][w]; nodes += es[t]->nodes; fc += es[t]->fullchecks; nsol += es[t]->nsol; }
  printf("# klein B=%d split=%d Bereich [%lld,%lld) Knoten=%lld Vollpruefungen=%lld Basisloesungen=%lld Zeit=%.1f s Threads=%d D=%d\n",
         B, split, lo, hi, nodes, fc, nsol, now() - t0, T, es[0]->K);
  print_counts("Z", c);
  if (liste) {
    FILE *f = fopen(liste, "w");
    for (int t = 0; t < T; t++) for (long i = 0; i < es[t]->nsol; i++) print_sol(f, &es[t]->sol[i]);
    fclose(f);
  }
  fflush(stdout);
  return 0;
}

/* ---------------- neun1 ---------------- */
static int run_neun1(int B, long long lo, long long hi, const u64 *Uq, int quiet, u64 out[3][128], const char *liste) {
  int T = omp_get_max_threads();
  Eng **es = malloc(sizeof(Eng *) * T);
  double t0 = now();
  if (hi > N9) hi = N9;
  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    Eng *e = eng_new(); es[tid] = e;
    u64 *U0 = malloc(sizeof(u64) * NWF);
    #pragma omp for schedule(dynamic, 16)
    for (long long k = lo; k < hi; k++) {
      int o = NSM + (int)k;
      const u64 *co = COV(o);
      for (int i = 0; i < NWF; i++) U0[i] = (Uq ? Uq[i] : ~0ULL) & ~co[i];
      if (NP % 64) U0[NWF - 1] &= (1ULL << (NP % 64)) - 1;
      eng_setup(e, U0, KD_DEFAULT);
      e->B = B - 9; e->wbase = 9; e->nn = 1; e->o[0] = o; e->split = 1 << 30; e->lo = 0; e->hi = 1LL << 62;
      e->ticket = NULL; e->top_owner = 1;
      if (e->B >= 0) eng_run(e);
    }
  }
  u64 c[3][128]; memset(c, 0, sizeof c);
  long long nodes = 0, fc = 0, nsol = 0;
  for (int t = 0; t < T; t++) { for (int a = 0; a < 3; a++) for (int w = 0; w < 128; w++) c[a][w] += es[t]->cnt[a][w]; nodes += es[t]->nodes; fc += es[t]->fullchecks; nsol += es[t]->nsol; }
  if (out) memcpy(out, c, sizeof c);
  if (!quiet) {
    printf("# neun1 B=%d Neunerbahnen [%lld,%lld) Knoten=%lld Vollpruefungen=%lld Basisloesungen=%lld Zeit=%.1f s\n", B, lo, hi, nodes, fc, nsol, now() - t0);
    print_counts("Z", c);
  }
  if (liste) {
    FILE *f = fopen(liste, "w");
    for (int t = 0; t < T; t++) for (long i = 0; i < es[t]->nsol; i++) print_sol(f, &es[t]->sol[i]);
    fclose(f);
  }
  return 0;
}

/* ---------------- neun2 ---------------- */
typedef struct { u64 I[CW]; int w; } SSet;
static SSet *SS; static int nSS;
static int GS_bmax, GS_cap; static u64 GS_I[CW];
static void gs_rec(int start, int w) {
  if (nSS == GS_cap) { GS_cap *= 2; SS = realloc(SS, sizeof(SSet) * GS_cap); }
  memcpy(SS[nSS].I, GS_I, sizeof GS_I); SS[nSS].w = w; nSS++;
  for (int c = start; c < NSM; c++) {
    int wc = c < N1 ? 1 : 3;
    if (w + wc > GS_bmax) continue;
    GS_I[c >> 6] |= 1ULL << (c & 63);
    gs_rec(c + 1, w + wc);
    GS_I[c >> 6] &= ~(1ULL << (c & 63));
  }
}
static void gen_small(int bmax) {
  /* alle Mengen kleiner Bahnen mit Gewicht <= bmax */
  if (SS) free(SS);
  GS_cap = 1024; SS = malloc(sizeof(SSet) * GS_cap); nSS = 0; GS_bmax = bmax;
  memset(GS_I, 0, sizeof GS_I);
  gs_rec(0, 0);
}

static int run_neun2(int B, const u64 *Uq, int quiet, u64 out[3][128], const char *liste, int Tt, int q) {
  double t0 = now();
  int bs = B - 18;
  u64 c[3][128]; memset(c, 0, sizeof c);
  if (bs < 0) { if (out) memcpy(out, c, sizeof c); return 0; }
  u64 *U0 = malloc(sizeof(u64) * NWF);
  for (int i = 0; i < NWF; i++) U0[i] = Uq ? Uq[i] : ~0ULL;
  if (NP % 64) U0[NWF - 1] &= (1ULL << (NP % 64)) - 1;
  gen_small(bs);
  /* Rest je kleiner Menge: U0 \ cov(S) */
  u64 *SR = malloc(sizeof(u64) * NWF * (size_t)nSS);
  #pragma omp parallel for schedule(dynamic, 16)
  for (int s = 0; s < nSS; s++) {
    u64 *r = SR + (size_t)s * NWF;
    memcpy(r, U0, sizeof(u64) * NWF);
    for (int i = 0; i < CW; i++) { u64 m = SS[s].I[i]; while (m) { int cc = i * 64 + __builtin_ctzll(m); m &= m - 1; const u64 *cv = COV(cc); for (int k = 0; k < NWF; k++) r[k] &= ~cv[k]; } }
  }
  /* Probenpunkte: Punkte des Universums mit den wenigsten Neuner-Ueberdeckern */
  int *n9 = calloc(NP, sizeof(int));
  #pragma omp parallel for schedule(dynamic, 64)
  for (int x = 0; x < NP; x++) { if (!tb(U0, x)) continue; int n = 0; for (int o = NSM; o < NP; o++) n += tb(COV(o), x); n9[x] = n; }
  int nu = 0; for (int x = 0; x < NP; x++) if (tb(U0, x)) nu++;
  int *pts = malloc(sizeof(int) * nu); nu = 0;
  for (int x = 0; x < NP; x++) if (tb(U0, x)) pts[nu++] = x;
  /* sortieren nach n9 aufsteigend */
  for (int i = 1; i < nu; i++) { int v = pts[i], j = i - 1; while (j >= 0 && (n9[pts[j]] > n9[v] || (n9[pts[j]] == n9[v] && pts[j] > v))) { pts[j + 1] = pts[j]; j--; } pts[j + 1] = v; }
  int npr = Tt * q; if (npr > nu) npr = nu;
  int T = (npr + q - 1) / q; if (T < 1) T = 1;
  int qt[16], probe[16][32];
  for (int t = 0; t < T; t++) qt[t] = 0;
  for (int i = 0; i < npr; i++) { int t = i % T; probe[t][qt[t]++] = pts[i]; }
  /* Tabellen: good_t[r] = es gibt S mit r Teilmenge von msk_t(S) */
  unsigned char *good[16];
  for (int t = 0; t < T; t++) {
    size_t sz = (size_t)1 << qt[t];
    good[t] = calloc(sz, 1);
    for (int s = 0; s < nSS; s++) {
      unsigned m = 0;
      for (int i = 0; i < qt[t]; i++) if (!tb(SR + (size_t)s * NWF, probe[t][i])) m |= 1u << i;  /* von S ueberdeckt */
      good[t][m] = 1;
    }
    for (int i = 0; i < qt[t]; i++) for (size_t r = 0; r < sz; r++) if ((r >> i) & 1) if (good[t][r]) good[t][r ^ ((size_t)1 << i)] = 1;
  }
  unsigned *sig[16];
  for (int t = 0; t < T; t++) {
    sig[t] = malloc(sizeof(unsigned) * N9);
    for (int k = 0; k < N9; k++) { unsigned m = 0; const u64 *co = COV(NSM + k); for (int i = 0; i < qt[t]; i++) if (tb(co, probe[t][i])) m |= 1u << i; sig[t][k] = m; }
  }
  long long surv = 0, pairs = 0, nsol = 0;
  int nth = omp_get_max_threads();
  u64 (*cc)[3][128] = calloc(nth, sizeof *cc);
  FILE *fl = liste ? fopen(liste, "w") : NULL;
  #pragma omp parallel reduction(+:surv,pairs,nsol)
  {
    int tid = omp_get_thread_num();
    u64 *Rr = malloc(sizeof(u64) * NWF);
    #pragma omp for schedule(dynamic, 8)
    for (int k1 = 0; k1 < N9; k1++) {
      const u64 *c1 = COV(NSM + k1);
      for (int k2 = k1 + 1; k2 < N9; k2++) {
        pairs++;
        int ok = 1;
        for (int t = 0; t < T; t++) {
          unsigned full = qt[t] == 32 ? 0xffffffffu : ((1u << qt[t]) - 1);
          unsigned r = ~(sig[t][k1] | sig[t][k2]) & full;
          if (!good[t][r]) { ok = 0; break; }
        }
        if (!ok) continue;
        surv++;
        const u64 *c2 = COV(NSM + k2);
        for (int i = 0; i < NWF; i++) Rr[i] = U0[i] & ~c1[i] & ~c2[i];
        for (int s = 0; s < nSS; s++) {
          const u64 *sr = SR + (size_t)s * NWF;
          int good2 = 1;
          for (int i = 0; i < NWF; i++) if (Rr[i] & sr[i]) { good2 = 0; break; }
          if (good2) {
            cc[tid][2][18 + SS[s].w]++; nsol++;
            if (fl) {
              #pragma omp critical
              {
                Sol so; so.nn = 2; so.o[0] = NSM + k1; so.o[1] = NSM + k2; memcpy(so.I, SS[s].I, sizeof so.I); so.w = 18 + SS[s].w; so.nsup = 1;
                print_sol(fl, &so);
              }
            }
          }
        }
      }
    }
    free(Rr);
  }
  if (fl) fclose(fl);
  for (int t = 0; t < nth; t++) for (int a = 0; a < 3; a++) for (int w = 0; w < 128; w++) c[a][w] += cc[t][a][w];
  if (out) memcpy(out, c, sizeof c);
  if (!quiet) {
    printf("# neun2 B=%d kleine Mengen=%d Tabellen=%d x %d Probenpunkte, Paare=%lld Ueberlebende=%lld Loesungen=%lld Zeit=%.1f s\n",
           B, nSS, T, qt[0], pairs, surv, nsol, now() - t0);
    print_counts("Z", c);
  }
  for (int t = 0; t < T; t++) { free(good[t]); free(sig[t]); }
  free(SR); free(n9); free(pts); free(U0); free(cc);
  return 0;
}

/* ----------------------------- Selbsttests ----------------------------- */
static u64 rng_s;
static u64 rnd(void) { rng_s += 0x9e3779b97f4a7c15ULL; u64 z = rng_s; z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL; z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL; return z ^ (z >> 31); }

static u64 *random_universe(int K, int **ptsout) {
  u64 *U = calloc(NWF, sizeof(u64));
  int *pts = malloc(sizeof(int) * K); int n = 0;
  while (n < K) { int x = (int)(rnd() % (u64)NP); if (!tb(U, x)) { U[x >> 6] |= 1ULL << (x & 63); pts[n++] = x; } }
  if (ptsout) *ptsout = pts; else free(pts);
  return U;
}

/* Durchprobieren: alle Mengen kleiner Bahnen mit Gewicht <= b, die die Punkte pts[0..K) ueberdecken */
static int BK, BQW; static u64 (*bqm)[32]; static u64 bcnt[128];
static void brute_rec(int start, int w, const u64 *covd, int b, int wbase) {
  int full = 1;
  for (int i = 0; i < BQW; i++) { u64 want = (i == BQW - 1 && BK % 64) ? ((1ULL << (BK % 64)) - 1) : ~0ULL; if ((covd[i] & want) != want) { full = 0; break; } }
  if (full) bcnt[wbase + w]++;
  for (int c = start; c < NSM; c++) {
    int wc = wofc(c);
    if (w + wc > b) continue;
    u64 nc[32];
    for (int i = 0; i < BQW; i++) nc[i] = covd[i] | bqm[c][i];
    brute_rec(c + 1, w + wc, nc, b, wbase);
  }
}
static void brute_small(const int *pts, int K, int b, int wbase, const u64 *pre /* bereits ueberdeckt (Bits ueber pts) oder NULL */) {
  BK = K; BQW = (K + 63) / 64;
  if (!bqm) bqm = malloc(sizeof(*bqm) * 64 * CW);
  for (int c = 0; c < NSM; c++) { memset(bqm[c], 0, sizeof bqm[c]); for (int i = 0; i < K; i++) if (tb(COV(c), pts[i])) bqm[c][i >> 6] |= 1ULL << (i & 63); }
  u64 z[32]; memset(z, 0, sizeof z);
  if (pre) memcpy(z, pre, sizeof(u64) * BQW);
  brute_rec(0, 0, z, b, wbase);
}

static int cmp_counts(const char *name, u64 a[128], u64 b[128]) {
  int bad = 0; u64 sa = 0;
  for (int w = 0; w < 128; w++) { if (a[w] != b[w]) bad++; sa += a[w]; }
  printf("%s: %s (Summe %llu;", name, bad ? "ABWEICHUNG" : "gleich", sa);
  for (int w = 0; w < 128; w++) if (a[w] || b[w]) printf(" %d:%llu/%llu", w, a[w], b[w]);
  printf(")\n");
  return bad;
}

static int run_test0(u64 seed, int K, int KD, int b) {
  rng_s = seed * 7919 + 17;
  int *pts; u64 *U = random_universe(K, &pts);
  /* Motor */
  int save = KD_DEFAULT; KD_DEFAULT = KD;
  Eng *e = eng_new(); eng_setup(e, U, KD);
  e->B = b; e->wbase = 0; e->nn = 0; e->split = 1 << 30; e->lo = 0; e->hi = 1LL << 62; e->ticket = NULL; e->top_owner = 1; e->record = 0;
  double t0 = now();
  eng_run(e);
  double t1 = now();
  KD_DEFAULT = save;
  memset(bcnt, 0, sizeof bcnt);
  brute_small(pts, K, b, 0, NULL);
  double t2 = now();
  char nm[200]; snprintf(nm, sizeof nm, "test0 seed=%llu K=%d D=%d B=%d Knoten=%lld Vollpruefungen=%lld (%.1f s / Durchprobieren %.1f s)", seed, K, e->K, b, e->nodes, e->fullchecks, t1 - t0, t2 - t1);
  return cmp_counts(nm, e->cnt[0], bcnt);
}

static void t1_rec(int start, int w, const u64 *cvd, int bb, const int *pts, int K, int QW, u64 *loc) {
  int full = 1;
  for (int i = 0; i < QW; i++) { u64 want = (i == QW - 1 && K % 64) ? ((1ULL << (K % 64)) - 1) : ~0ULL; if ((cvd[i] & want) != want) { full = 0; break; } }
  if (full) loc[9 + w]++;
  for (int cc = start; cc < NSM; cc++) {
    int wc = wofc(cc); if (w + wc > bb) continue;
    u64 nc[32];
    for (int i = 0; i < QW; i++) nc[i] = cvd[i];
    for (int i = 0; i < K; i++) if (tb(COV(cc), pts[i])) nc[i >> 6] |= 1ULL << (i & 63);
    t1_rec(cc + 1, w + wc, nc, bb, pts, K, QW, loc);
  }
}

static int run_test1(u64 seed, int K, int b) {
  rng_s = seed * 104729 + 3;
  int *pts; u64 *U = random_universe(K, &pts);
  u64 c[3][128];
  int save = KD_DEFAULT; KD_DEFAULT = 64;  /* klein, damit die Vollpruefung oft greift */
  double t0 = now();
  run_neun1(b, 0, N9, U, 1, c, NULL);
  double t1 = now();
  KD_DEFAULT = save;
  /* Durchprobieren: je Neunerbahn alle kleinen Mengen mit Gewicht <= b-9 */
  u64 tot[128]; memset(tot, 0, sizeof tot);
  int QW = (K + 63) / 64;
  #pragma omp parallel
  {
    u64 loc[128]; memset(loc, 0, sizeof loc);
    #pragma omp for schedule(dynamic, 64)
    for (int k = 0; k < N9; k++) {
      int o = NSM + k;
      u64 pre[32]; memset(pre, 0, sizeof pre);
      for (int i = 0; i < K; i++) if (tb(COV(o), pts[i])) pre[i >> 6] |= 1ULL << (i & 63);
      int bb = b - 9;
      if (bb >= 0) t1_rec(0, 0, pre, bb, pts, K, QW, loc);
    }
    #pragma omp critical
    for (int w = 0; w < 128; w++) tot[w] += loc[w];
  }
  double t2 = now();
  char nm[200]; snprintf(nm, sizeof nm, "test1 seed=%llu K=%d B=%d (%.1f s / Durchprobieren %.1f s)", seed, K, b, t1 - t0, t2 - t1);
  return cmp_counts(nm, c[1], tot);
}

static int run_test2(u64 seed, int K, int b) {
  rng_s = seed * 1299709 + 11;
  if (K > 64) K = 64;
  int *pts; u64 *U = random_universe(K, &pts);
  u64 c[3][128];
  double t0 = now();
  run_neun2(b, U, 1, c, NULL, 2, (K < 24 ? (K + 1) / 2 : 12));
  double t1 = now();
  /* Durchprobieren: alle Paare, alle kleinen Mengen mit Gewicht <= b-18 */
  int bs = b - 18;
  gen_small(bs < 0 ? 0 : bs);
  u64 full = K == 64 ? ~0ULL : ((1ULL << K) - 1);
  u64 *m9 = malloc(sizeof(u64) * N9), *ms = malloc(sizeof(u64) * nSS);
  for (int k = 0; k < N9; k++) { u64 m = 0; for (int i = 0; i < K; i++) if (tb(COV(NSM + k), pts[i])) m |= 1ULL << i; m9[k] = m; }
  for (int s = 0; s < nSS; s++) { u64 m = 0; for (int i = 0; i < K; i++) { int cvd = 0; for (int cc = 0; cc < NSM; cc++) if ((SS[s].I[cc >> 6] >> (cc & 63)) & 1) if (tb(COV(cc), pts[i])) { cvd = 1; break; } if (cvd) m |= 1ULL << i; } ms[s] = m; }
  u64 tot[128]; memset(tot, 0, sizeof tot);
  #pragma omp parallel
  {
    u64 loc[128]; memset(loc, 0, sizeof loc);
    #pragma omp for schedule(dynamic, 64)
    for (int k1 = 0; k1 < N9; k1++) for (int k2 = k1 + 1; k2 < N9; k2++) {
      u64 r = ~(m9[k1] | m9[k2]) & full;
      if (bs < 0) continue;
      for (int s = 0; s < nSS; s++) if ((r & ~ms[s]) == 0) loc[18 + SS[s].w]++;
    }
    #pragma omp critical
    for (int w = 0; w < 128; w++) tot[w] += loc[w];
  }
  double t2 = now();
  char nm[200]; snprintf(nm, sizeof nm, "test2 seed=%llu K=%d B=%d (%.1f s / Durchprobieren %.1f s)", seed, K, b, t1 - t0, t2 - t1);
  return cmp_counts(nm, c[2], tot);
}

/* ----------------------------- info ----------------------------- */
static int run_info(const char *mfile) {
  printf("Bahnen: %d (Groesse 1: %d, 3: %d, 9: %d)\n", NP, N1, N3, N9);
  long long asym = 0;
  #pragma omp parallel for reduction(+:asym) schedule(dynamic, 64)
  for (int a = 0; a < NP; a++) for (int b = 0; b < NP; b++) if (tb(COV(a), b) != tb(COV(b), a)) asym++;
  printf("Ueberdeckung symmetrisch: %s (%lld Abweichungen)\n", asym ? "nein" : "ja", asym);
  int mn[10], mx[10]; for (int i = 0; i < 10; i++) { mn[i] = 1 << 30; mx[i] = -1; }
  for (int x = 0; x < NP; x++) { int s = osz[x]; if (nsmall[x] < mn[s]) mn[s] = nsmall[x]; if (nsmall[x] > mx[s]) mx[s] = nsmall[x]; }
  for (int s = 1; s <= 9; s += (s == 1 ? 2 : 6)) printf("Punktbahnen Groesse %d: kleine Ueberdecker %d bis %d\n", s, mn[s], mx[s]);
  if (mfile) {
    FILE *f = fopen(mfile, "r"); char line[1024]; int r = 0; int ids[64]; int nid = 0;
    while (fgets(line, sizeof line, f)) {
      char *p = line; unsigned w = 0; int j = 0;
      while (1) { char *e; long v = strtol(p, &e, 10); if (e == p) break; if (v == -1) w |= 1u << j; j++; p = e; }
      if (j == 0) continue;
      if (j != NB) { fprintf(stderr, "Matrixbreite\n"); return 2; }
      ids[nid++] = oid[w]; r++;
    }
    fclose(f);
    /* verschiedene Bahnen */
    int uniq[64], nu = 0;
    for (int i = 0; i < nid; i++) { int dup = 0; for (int k = 0; k < nu; k++) if (uniq[k] == ids[i]) dup = 1; if (!dup) uniq[nu++] = ids[i]; }
    printf("Matrix %s: %d Zeilen in %d Bahnen:", mfile, r, nu);
    int wsum = 0;
    for (int k = 0; k < nu; k++) { printf(" %d(Gr.%d)", uniq[k], osz[uniq[k]]); wsum += osz[uniq[k]]; }
    printf("; Gewicht der Bahnen %d\n", wsum);
    /* ueberdeckt? */
    u64 *t = malloc(sizeof(u64) * NWF); memset(t, 0, sizeof(u64) * NWF);
    for (int k = 0; k < nu; k++) for (int i = 0; i < NWF; i++) t[i] |= COV(uniq[k])[i];
    int unc = 0; for (int x = 0; x < NP; x++) if (!tb(t, x)) unc++;
    printf("nicht ueberdeckte Punktbahnen: %d\n", unc);
  }
  return 0;
}

int main(int argc, char **argv) {
  init_bin();
  if (argc < 3) { fprintf(stderr, "Aufruf siehe Kopf\n"); return 1; }
  const char *m = argv[1];
  char *kd = getenv("B2_KD"); if (kd) KD_DEFAULT = atoi(kd);
  setup(argv[2], 0);
  if (!strcmp(m, "info")) return run_info(argc > 3 ? argv[3] : NULL);
  if (!strcmp(m, "klein")) return run_klein(atoi(argv[3]), atoi(argv[4]), atoll(argv[5]), atoll(argv[6]), argc > 7 ? argv[7] : NULL, 0);
  if (!strcmp(m, "teilbaeume")) return run_klein(atoi(argv[3]), atoi(argv[4]), 0, 0, NULL, 1);
  if (!strcmp(m, "neun1")) return run_neun1(atoi(argv[3]), atoll(argv[4]), atoll(argv[5]), NULL, 0, NULL, argc > 6 ? argv[6] : NULL);
  if (!strcmp(m, "neun2")) return run_neun2(atoi(argv[3]), NULL, 0, NULL, argc > 4 ? argv[4] : NULL, argc > 5 ? atoi(argv[5]) : 4, argc > 6 ? atoi(argv[6]) : 24);
  if (!strcmp(m, "test0")) return run_test0(atoll(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6])) ? 1 : 0;
  if (!strcmp(m, "test1")) return run_test1(atoll(argv[3]), atoi(argv[4]), atoi(argv[5])) ? 1 : 0;
  if (!strcmp(m, "test2")) return run_test2(atoll(argv[3]), atoi(argv[4]), atoi(argv[5])) ? 1 : 0;
  if (!strcmp(m, "positiv1")) {
    long long o = atoll(argv[3]); int bsm = atoi(argv[4]);
    return run_neun1(9 + bsm, o, o + 1, NULL, 0, NULL, argc > 5 ? argv[5] : NULL);
  }
  fprintf(stderr, "unbekannter Modus\n");
  return 1;
}
