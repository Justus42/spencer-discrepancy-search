/* b2_aequiv.c  (Auftrag B2, Pruefcode, nur ganze Zahlen)
 *
 * Gleichwertigkeit selbstkomplementaerer Codes unter vorzeichenbehafteten Spaltenpermutationen.
 * Eigenes Verfahren (keine Fremdbibliothek): Der Code (R Zeilen, C Spalten, Eintraege +-1; Wortmenge
 * = Zeilen und ihre Negative) wird als Graph kodiert:
 *   Ecken r+, r- je Zeile, c+, c- je Spalte; Kanten r+ - r-, c+ - c-;
 *   r^s - c^t genau dann, wenn M[r][c] * s * t = +1.
 *   Anfangsfaerbung: Zeilenecken Farbe 0, Spaltenecken Farbe 1.
 * Farbtreue Graphautomorphismen = Paare (Zeilenperm. mit Vorzeichen, Spaltenperm. mit Vorzeichen), die M
 * erhalten; bei paarweise (bis aufs Vorzeichen) verschiedenen Zeilen ist die Projektion auf den
 * Spaltenteil injektiv, also |Aut(Code)| = |Aut(Graph)|.
 * Kanonische Form: Individualisieren und Verfeinern (Farbverfeinerung mit Hashsignaturen, die nur von
 * Farben abhaengen, also isomorphieinvariant), Spur-Vergleich, Blatt mit maximaler (Spur, Zertifikat).
 * Zahl der Blaetter mit dem besten Zertifikat = |Aut|.
 *
 * Modi:
 *   b2_aequiv aut DATEI            eine Matrix; Ordnung und alle Automorphismen als Spaltenabbildung
 *                                   Zeile: "pi_0 .. pi_{C-1} | t_0 .. t_{C-1}" (Spalte j -> pi_j, Faktor t_j)
 *   b2_aequiv klassen DATEI         viele Matrizen (Bloecke, getrennt durch Leerzeilen oder '#');
 *                                   je Matrix: Klasse, |Aut|, Abbildung auf den Klassenvertreter
 *   b2_aequiv selbsttest            interne Tests
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _OPENMP
#include <omp.h>
#endif

typedef unsigned long long u64;
#define MAXR 64
#define MAXC 32
#define MAXV (2*MAXR+2*MAXC)
#define VW ((MAXV+63)/64)
#define MAXDEG (MAXR+MAXC+2)

typedef struct {
  int R, C, V;
  int M[MAXR][MAXC];
  u64 adj[MAXV][VW];
  int nb[MAXV][MAXDEG];
  int deg[MAXV];
} Graph;

static u64 RNDC[MAXV + 1];

static u64 mix64(u64 x) {
  x ^= x >> 33; x *= 0xff51afd7ed558ccdULL; x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL; x ^= x >> 33; return x;
}

static void init_rnd(void) {
  u64 s = 0x1234567890abcdefULL;
  for (int i = 0; i <= MAXV; i++) { s += 0x9e3779b97f4a7c15ULL; RNDC[i] = mix64(s); }
}

static void addedge(Graph *g, int a, int b) {
  g->adj[a][b >> 6] |= 1ULL << (b & 63);
  g->adj[b][a >> 6] |= 1ULL << (a & 63);
  g->nb[a][g->deg[a]++] = b;
  g->nb[b][g->deg[b]++] = a;
}

/* Ecken: Zeile r Vorzeichen s (0:+,1:-) -> 2r+s; Spalte c Vorzeichen t -> 2R+2c+t */
static int build_graph(Graph *g) {
  int R = g->R, C = g->C;
  g->V = 2 * R + 2 * C;
  memset(g->adj, 0, sizeof g->adj);
  memset(g->deg, 0, sizeof g->deg);
  for (int r = 0; r < R; r++) for (int c = 0; c < C; c++)
    if (g->M[r][c] != 1 && g->M[r][c] != -1) return -1;
  /* Zeilen paarweise verschieden bis aufs Vorzeichen */
  for (int a = 0; a < R; a++) for (int b = a + 1; b < R; b++) {
    int eq = 1, neg = 1;
    for (int c = 0; c < C; c++) { if (g->M[a][c] != g->M[b][c]) eq = 0; if (g->M[a][c] != -g->M[b][c]) neg = 0; }
    if (eq || neg) return -2;
  }
  for (int r = 0; r < R; r++) addedge(g, 2 * r, 2 * r + 1);
  for (int c = 0; c < C; c++) addedge(g, 2 * R + 2 * c, 2 * R + 2 * c + 1);
  for (int r = 0; r < R; r++) for (int s = 0; s < 2; s++)
    for (int c = 0; c < C; c++) for (int t = 0; t < 2; t++) {
      int v = g->M[r][c] * (s ? -1 : 1) * (t ? -1 : 1);
      if (v == 1) addedge(g, 2 * r + s, 2 * R + 2 * c + t);
    }
  return 0;
}

/* ---- Verfeinerung ---- */
typedef struct { int col; u64 h; int v; } Key;
static int keycmp(const void *a, const void *b) {
  const Key *x = a, *y = b;
  if (x->col != y->col) return x->col < y->col ? -1 : 1;
  if (x->h != y->h) return x->h < y->h ? -1 : 1;
  return 0;
}

/* col[v] in [0,k); liefert neues k; trace wird fortgeschrieben */
static int refine(const Graph *g, int *col, int k, u64 *trace) {
  int V = g->V;
  Key key[MAXV];
  for (;;) {
    for (int v = 0; v < V; v++) {
      u64 h = 0;
      for (int i = 0; i < g->deg[v]; i++) h += RNDC[col[g->nb[v][i]]];
      key[v].col = col[v]; key[v].h = mix64(h); key[v].v = v;
    }
    qsort(key, V, sizeof(Key), keycmp);
    int nk = 0;
    u64 tr = *trace;
    for (int i = 0; i < V; i++) {
      if (i > 0 && keycmp(&key[i], &key[i - 1]) != 0) nk++;
      col[key[i].v] = nk;
      if (i == 0 || keycmp(&key[i], &key[i - 1]) != 0) tr = mix64(tr ^ (u64)key[i].col * 0x100000001b3ULL ^ key[i].h);
    }
    nk++;
    tr = mix64(tr + (u64)nk);
    *trace = tr;
    if (nk == k) return k;
    k = nk;
  }
}

/* ---- Suche ---- */
#define MAXDEPTH MAXV
typedef struct {
  const Graph *g;
  int store_leaves;         /* 1: alle besten Blaetter speichern (fuer Automorphismen) */
  int have;
  u64 besttr[MAXDEPTH + 1]; int bestlen;
  u64 bestcert[MAXV * VW];
  int bestlab[MAXV];
  long long nbest;
  long long nodes;
  int (*leaves)[MAXV]; long long nleaves, capleaves;
  u64 curtr[MAXDEPTH + 1];
} Search;

static void make_cert(const Graph *g, const int *lab, u64 *cert) {
  /* lab[v] = Position; inv[pos] = v; Zertifikat Zeile pos = {lab[u] : u ~ inv[pos]} */
  int V = g->V, inv[MAXV];
  for (int v = 0; v < V; v++) inv[lab[v]] = v;
  memset(cert, 0, sizeof(u64) * V * VW);
  for (int p = 0; p < V; p++) {
    int v = inv[p];
    for (int i = 0; i < g->deg[v]; i++) { int q = lab[g->nb[v][i]]; cert[p * VW + (q >> 6)] |= 1ULL << (q & 63); }
  }
}

static int cmp_trace(const u64 *a, int la, const u64 *b, int lb) {
  int l = la < lb ? la : lb;
  for (int i = 0; i < l; i++) if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
  return 0; /* Praefix gleich */
}

static void search_node(Search *S, int *col, int k, int depth) {
  const Graph *g = S->g;
  int V = g->V;
  S->nodes++;
  /* Spurvergleich mit dem besten Pfad */
  if (S->have) {
    int l = depth + 1 < S->bestlen ? depth + 1 : S->bestlen;
    int c = cmp_trace(S->curtr, l, S->besttr, l);
    if (c < 0) return;           /* schlechter: abschneiden */
  }
  if (k == V) {
    u64 cert[MAXV * VW];
    make_cert(g, col, cert);
    int c;
    if (!S->have) c = 1;
    else {
      int l = depth + 1 < S->bestlen ? depth + 1 : S->bestlen;
      c = cmp_trace(S->curtr, l, S->besttr, l);
      if (c == 0 && depth + 1 != S->bestlen) c = (depth + 1 > S->bestlen) ? 1 : -1;
      if (c == 0) c = memcmp(cert, S->bestcert, sizeof(u64) * V * VW) == 0 ? 0 : 2;
      if (c == 2) {
        c = 0;
        for (int i = 0; i < V * VW; i++) if (cert[i] != S->bestcert[i]) { c = cert[i] > S->bestcert[i] ? 1 : -1; break; }
      }
    }
    if (c > 0) {
      S->have = 1; S->bestlen = depth + 1;
      memcpy(S->besttr, S->curtr, sizeof(u64) * (depth + 1));
      memcpy(S->bestcert, cert, sizeof(u64) * V * VW);
      memcpy(S->bestlab, col, sizeof(int) * V);
      S->nbest = 1;
      if (S->store_leaves) { S->nleaves = 0; }
    } else if (c == 0) {
      S->nbest++;
    } else return;
    if (S->store_leaves) {
      if (S->nleaves == S->capleaves) {
        S->capleaves = S->capleaves ? 2 * S->capleaves : 1024;
        S->leaves = realloc(S->leaves, sizeof(int[MAXV]) * S->capleaves);
      }
      memcpy(S->leaves[S->nleaves++], col, sizeof(int) * V);
    }
    return;
  }
  /* Zielzelle: erste nicht einelementige Zelle kleinster Groesse */
  int size[MAXV]; memset(size, 0, sizeof(int) * k);
  for (int v = 0; v < V; v++) size[col[v]]++;
  int tc = -1;
  for (int c = 0; c < k; c++) if (size[c] > 1 && (tc < 0 || size[c] < size[tc])) tc = c;
  for (int v = 0; v < V; v++) if (col[v] == tc) {
    int nc[MAXV];
    for (int u = 0; u < V; u++) {
      int cu = col[u];
      if (cu > tc) nc[u] = cu + 1;
      else if (cu == tc) nc[u] = (u == v) ? tc : tc + 1;
      else nc[u] = cu;
    }
    u64 tr = mix64(S->curtr[depth] ^ 0xabcdefULL ^ (u64)tc);
    int nk = refine(g, nc, k + 1, &tr);
    S->curtr[depth + 1] = tr;
    search_node(S, nc, nk, depth + 1);
  }
}

static void run_search(const Graph *g, Search *S, int store) {
  memset(S, 0, sizeof *S);
  S->g = g; S->store_leaves = store;
  int col[MAXV];
  for (int v = 0; v < g->V; v++) col[v] = (v < 2 * g->R) ? 0 : 1;
  u64 tr = 0x5555ULL ^ ((u64)g->R << 20) ^ (u64)g->C;
  int k = refine(g, col, 2, &tr);
  S->curtr[0] = tr;
  search_node(S, col, k, 0);
}

/* Abbildung der Graphecken -> Spaltenabbildung (pi, t): Spalte c -> pi[c], Faktor t[c] */
static int to_colmap(const Graph *g, const int *gam, int *pi, int *t) {
  int R = g->R, C = g->C;
  for (int c = 0; c < C; c++) {
    int v = gam[2 * R + 2 * c];
    if (v < 2 * R) return -1;
    int w = v - 2 * R;
    pi[c] = w >> 1; t[c] = (w & 1) ? -1 : 1;
  }
  return 0;
}

/* ---- Einlesen ---- */
static int read_matrices(FILE *f, Graph **out) {
  int cap = 16, n = 0;
  Graph *G = malloc(sizeof(Graph) * cap);
  char line[4096];
  int r = 0, C = -1;
  Graph cur; memset(&cur, 0, sizeof cur);
  for (;;) {
    char *ok = fgets(line, sizeof line, f);
    int blank = 1;
    if (ok) { for (char *p = line; *p; p++) if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') { blank = 0; break; } if (line[0] == '#') blank = 1; }
    if (!ok || blank) {
      if (r > 0) {
        cur.R = r; cur.C = C;
        if (n == cap) { cap *= 2; G = realloc(G, sizeof(Graph) * cap); }
        G[n++] = cur;
        memset(&cur, 0, sizeof cur); r = 0; C = -1;
      }
      if (!ok) break;
      continue;
    }
    int c = 0; char *p = line;
    while (*p) {
      while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
      if (!*p) break;
      char *e; long v = strtol(p, &e, 10);
      if (e == p) { fprintf(stderr, "Lesefehler: %s\n", line); exit(2); }
      if (c >= MAXC || r >= MAXR) { fprintf(stderr, "zu gross\n"); exit(2); }
      cur.M[r][c++] = (int)v; p = e;
    }
    if (C < 0) C = c; else if (C != c) { fprintf(stderr, "ungleiche Zeilenlaenge\n"); exit(2); }
    r++;
  }
  *out = G;
  return n;
}

static void compose_inv(int V, const int *lab1, const int *lab2, int *gam) {
  /* gam = lab1^{-1} o lab2 : Ecke v -> lab1^{-1}(lab2(v)) */
  int inv1[MAXV];
  for (int v = 0; v < V; v++) inv1[lab1[v]] = v;
  for (int v = 0; v < V; v++) gam[v] = inv1[lab2[v]];
}

static int mode_aut(const char *fn) {
  FILE *f = fopen(fn, "r"); if (!f) { perror(fn); return 2; }
  Graph *G; int n = read_matrices(f, &G); fclose(f);
  if (n != 1) { fprintf(stderr, "genau eine Matrix erwartet (%d)\n", n); return 2; }
  int e = build_graph(&G[0]);
  if (e) { fprintf(stderr, "Matrix ungeeignet (%d)\n", e); return 2; }
  Search *S = calloc(1, sizeof(Search));
  run_search(&G[0], S, 1);
  printf("# R=%d C=%d Knoten=%lld Ordnung=%lld\n", G[0].R, G[0].C, S->nodes, S->nbest);
  for (long long i = 0; i < S->nleaves; i++) {
    int gam[MAXV], pi[MAXC], t[MAXC];
    compose_inv(G[0].V, S->bestlab, S->leaves[i], gam);
    if (to_colmap(&G[0], gam, pi, t)) { fprintf(stderr, "Abbildung vertauscht Farben\n"); return 3; }
    for (int c = 0; c < G[0].C; c++) printf("%d ", pi[c]);
    printf("|");
    for (int c = 0; c < G[0].C; c++) printf(" %d", t[c]);
    printf("\n");
  }
  return 0;
}

typedef struct { int idx; } Item;
static u64 **CERTS; static int CERTLEN;
static int certcmp_idx(const void *a, const void *b) {
  int x = *(const int *)a, y = *(const int *)b;
  int c = memcmp(CERTS[x], CERTS[y], sizeof(u64) * CERTLEN);
  if (c) return c;
  return x < y ? -1 : (x > y);
}

static int mode_klassen(const char *fn) {
  FILE *f = fopen(fn, "r"); if (!f) { perror(fn); return 2; }
  Graph *G; int n = read_matrices(f, &G); fclose(f);
  if (n < 1) return 2;
  int V = 2 * G[0].R + 2 * G[0].C;
  CERTLEN = V * VW;
  CERTS = malloc(sizeof(u64 *) * n);
  int (*labs)[MAXV] = malloc(sizeof(int[MAXV]) * n);
  long long *aut = malloc(sizeof(long long) * n);
  long long *nodes = malloc(sizeof(long long) * n);
  for (int i = 0; i < n; i++) {
    if (G[i].R != G[0].R || G[i].C != G[0].C) { fprintf(stderr, "Groessen verschieden\n"); return 2; }
    if (build_graph(&G[i])) { fprintf(stderr, "Matrix %d ungeeignet\n", i); return 2; }
    CERTS[i] = malloc(sizeof(u64) * CERTLEN);
  }
  #pragma omp parallel for schedule(dynamic, 4)
  for (int i = 0; i < n; i++) {
    Search *S = calloc(1, sizeof(Search));
    run_search(&G[i], S, 0);
    memcpy(CERTS[i], S->bestcert, sizeof(u64) * CERTLEN);
    memcpy(labs[i], S->bestlab, sizeof(int) * V);
    aut[i] = S->nbest; nodes[i] = S->nodes;
    free(S);
  }
  int *ord = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) ord[i] = i;
  qsort(ord, n, sizeof(int), certcmp_idx);
  /* Klassen nach erstem Vorkommen in Eingabereihenfolge nummerieren */
  int *cls = malloc(sizeof(int) * n), *rep = malloc(sizeof(int) * n);
  int ncls = 0;
  int *grp_first = malloc(sizeof(int) * n);
  for (int a = 0; a < n;) {
    int b = a;
    while (b < n && memcmp(CERTS[ord[a]], CERTS[ord[b]], sizeof(u64) * CERTLEN) == 0) b++;
    int first = ord[a]; /* kleinster Index, da Sortierung bei Gleichheit nach Index */
    for (int j = a; j < b; j++) grp_first[ord[j]] = first;
    a = b;
  }
  int *clsid_of_first = malloc(sizeof(int) * n);
  for (int i = 0; i < n; i++) clsid_of_first[i] = -1;
  for (int i = 0; i < n; i++) {
    int fi = grp_first[i];
    if (clsid_of_first[fi] < 0) { clsid_of_first[fi] = ncls; rep[ncls] = fi; ncls++; }
    cls[i] = clsid_of_first[fi];
  }
  long long totnodes = 0;
  for (int i = 0; i < n; i++) totnodes += nodes[i];
  printf("# Matrizen=%d Klassen=%d Knoten=%lld\n", n, ncls, totnodes);
  for (int c = 0; c < ncls; c++) {
    long long cnt = 0;
    for (int i = 0; i < n; i++) if (cls[i] == c) cnt++;
    printf("# Klasse %d: Vertreter %d, |Aut|=%lld, Anzahl=%lld\n", c, rep[c], aut[rep[c]], cnt);
  }
  /* je Matrix: Index Klasse |Aut| und Abbildung Matrix -> Vertreter */
  for (int i = 0; i < n; i++) {
    int r = rep[cls[i]];
    if (aut[i] != aut[r]) { fprintf(stderr, "Widerspruch: |Aut| innerhalb einer Klasse verschieden (%d,%d)\n", i, r); return 3; }
    int gam[MAXV], pi[MAXC], t[MAXC];
    /* lab_r^{-1} o lab_i bildet Graph i auf Graph r ab */
    compose_inv(V, labs[r], labs[i], gam);
    if (to_colmap(&G[i], gam, pi, t)) { fprintf(stderr, "Farbfehler\n"); return 3; }
    printf("%d %d %lld ", i, cls[i], aut[i]);
    for (int c = 0; c < G[i].C; c++) printf("%d ", pi[c]);
    printf("|");
    for (int c = 0; c < G[i].C; c++) printf(" %d", t[c]);
    printf("\n");
  }
  return 0;
}

int main(int argc, char **argv) {
  init_rnd();
  if (argc >= 3 && !strcmp(argv[1], "aut")) return mode_aut(argv[2]);
  if (argc >= 3 && !strcmp(argv[1], "klassen")) return mode_klassen(argv[2]);
  fprintf(stderr, "Aufruf: b2_aequiv aut DATEI | klassen DATEI\n");
  return 1;
}
